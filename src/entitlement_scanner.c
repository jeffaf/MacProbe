#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Function to check a single app for the dangerous entitlement combination
void check_app_entitlements(const char *app_path) {
  char command[2048];
  char output[4096];
  FILE *fp;
  int has_disable_library_validation = 0;
  int has_allow_unsigned_executable_memory = 0;

  // Construct the codesign command
  // Using absolute path for codesign for security (ENV33-C)
  // We use -d --entitlements - to dump entitlements to stdout
  // We wrap app_path in quotes to handle spaces
  int ret =
      snprintf(command, sizeof(command),
               "/usr/bin/codesign -d --entitlements - \"%s\" 2>&1", app_path);
  if (ret < 0 || (size_t)ret >= sizeof(command)) {
    // Path too long, skip
    return;
  }

  fp = popen(command, "r");
  if (fp == NULL) {
    return;
  }

  // Read output
  while (fgets(output, sizeof(output), fp) != NULL) {
    if (strstr(output, "com.apple.security.cs.disable-library-validation") !=
        NULL) {
      has_disable_library_validation = 1;
    }
    if (strstr(output,
               "com.apple.security.cs.allow-unsigned-executable-memory") !=
        NULL) {
      has_allow_unsigned_executable_memory = 1;
    }
  }

  pclose(fp);

  // Check if both dangerous entitlements are present
  if (has_disable_library_validation && has_allow_unsigned_executable_memory) {
    printf("    [!] VULNERABLE: %s\n", app_path);
    printf("        -> Has 'disable-library-validation' AND "
           "'allow-unsigned-executable-memory'\n");
    printf("        -> Potential DYLD_INSERT_LIBRARIES injection target!\n");

    // Improved logic: Parse Info.plist to find CFBundleExecutable
    char info_plist_path[2048];
    char binary_name[256] = {0};
    char binary_path[2048];

    snprintf(info_plist_path, sizeof(info_plist_path), "%s/Contents/Info.plist",
             app_path);

    FILE *plist_fp = fopen(info_plist_path, "r");
    if (plist_fp) {
      char line[1024];
      int found_key = 0;
      while (fgets(line, sizeof(line), plist_fp)) {
        if (strstr(line, "<key>CFBundleExecutable</key>")) {
          found_key = 1;
          continue;
        }
        if (found_key) {
          // Next line should contain <string>BinaryName</string>
          char *start = strstr(line, "<string>");
          char *end = strstr(line, "</string>");
          if (start && end) {
            start += 8; // Skip <string>
            size_t len = end - start;
            if (len < sizeof(binary_name)) {
              strncpy(binary_name, start, len);
              binary_name[len] = '\0';
            }
          }
          break;
        }
      }
      fclose(plist_fp);
    }

    // Fallback if parsing failed
    if (strlen(binary_name) == 0) {
      // Fallback heuristic
      char app_name[256];
      const char *last_slash = strrchr(app_path, '/');
      if (last_slash) {
        strncpy(app_name, last_slash + 1, sizeof(app_name) - 1);
        app_name[sizeof(app_name) - 1] = '\0';
        char *dot = strrchr(app_name, '.');
        if (dot)
          *dot = '\0';
        strncpy(binary_name, app_name, sizeof(binary_name) - 1);
      }
    }

    snprintf(binary_path, sizeof(binary_path), "%s/Contents/MacOS/%s", app_path,
             binary_name);

    // Prompt user
    printf("        [?] Test injection on this app? (y/n): ");
    char response = 'n';
    scanf(" %c", &response); // Space before %c to skip whitespace

    if (response == 'y' || response == 'Y') {
      printf("        [*] Attempting injection...\n");
      char inject_cmd[4096];
      // Use absolute path to inject.dylib. Assuming it's in CWD for now.
      char cwd[1024];
      if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // Append & to run in background so we don't block MacProbe
        snprintf(inject_cmd, sizeof(inject_cmd),
                 "DYLD_INSERT_LIBRARIES=%s/inject.dylib \"%s\" &", cwd,
                 binary_path);
        printf("        [*] Running in background: %s\n", inject_cmd);
        printf("        [*] Check stdout/syslog for 'Hello from dylib!'\n");

        // Run the command
        system(inject_cmd);
      }
    }
  }
}

void scan_applications_for_entitlements(void) {
  DIR *dir;
  struct dirent *entry;
  const char *apps_dir = "/Applications";
  char full_path[1024];

  printf("\n[+] Entitlement Scanner (DYLD Injection Candidates):\n");
  printf("    Scanning /Applications for risky entitlements...\n");

  dir = opendir(apps_dir);
  if (dir == NULL) {
    printf("    [-] Failed to open /Applications directory.\n");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    // Check for .app extension
    if (entry->d_type == DT_DIR && strstr(entry->d_name, ".app") != NULL) {
      // Construct full path
      int ret = snprintf(full_path, sizeof(full_path), "%s/%s", apps_dir,
                         entry->d_name);
      if (ret >= 0 && (size_t)ret < sizeof(full_path)) {
        check_app_entitlements(full_path);
      }
    }
  }

  closedir(dir);
  printf("    [=] Scan complete.\n");
}
