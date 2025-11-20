#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_tcc_permissions(void) {
  char *user_home = getenv("HOME");
  if (!user_home)
    return;

  char path[1024];
  int access_found = 0;

  printf("  [?] Checking TCC (Privacy) Permissions for current context:\n");

  // Check Full Disk Access (FDA) / TCC by trying to read TCC.db (usually
  // requires FDA) Note: Reading the system TCC.db usually requires FDA.
  // FIX: STR31-C. Guarantee that storage for strings has sufficient space
  int ret = snprintf(path, sizeof(path),
                     "/Library/Application Support/com.apple.TCC/TCC.db");
  if (ret < 0 || (size_t)ret >= sizeof(path)) {
    // Handle error or truncation
  } else if (access(path, R_OK) == 0) {
    printf("    [!] Full Disk Access (System): AVAILABLE\n");
    access_found = 1;
  }

  // Check User FDA/TCC
  ret = snprintf(path, sizeof(path),
                 "%s/Library/Application Support/com.apple.TCC/TCC.db",
                 user_home);
  if (ret >= 0 && (size_t)ret < sizeof(path)) {
    if (access(path, R_OK) == 0) {
      printf("    [!] Full Disk Access (User): AVAILABLE\n");
      access_found = 1;
    }
  }

  // Check Safari
  ret = snprintf(path, sizeof(path), "%s/Library/Safari/History.db", user_home);
  if (ret >= 0 && (size_t)ret < sizeof(path)) {
    if (access(path, R_OK) == 0) {
      printf("    [!] Safari History: AVAILABLE\n");
      access_found = 1;
    }
  }

  // Check Messages
  ret = snprintf(path, sizeof(path), "%s/Library/Messages/chat.db", user_home);
  if (ret >= 0 && (size_t)ret < sizeof(path)) {
    if (access(path, R_OK) == 0) {
      printf("    [!] Messages/SMS: AVAILABLE\n");
      access_found = 1;
    }
  }

  // Check Mail
  ret = snprintf(path, sizeof(path), "%s/Library/Mail", user_home);
  if (ret >= 0 && (size_t)ret < sizeof(path)) {
    if (access(path, R_OK) == 0) {
      // Directory access might be allowed, check a subdir if possible, but
      // directory list is often blocked Let's just report directory access for
      // now
      printf("    [!] Mail Directory: ACCESSIBLE\n");
      access_found = 1;
    }
  }

  if (!access_found) {
    printf("    [-] No sensitive TCC data accessible (Clean/Sandboxed)\n");
  }
}
