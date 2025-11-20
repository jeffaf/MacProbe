#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration for SIP check
// This is a private API but widely known.
// We can also use popen("csrutil status") as a fallback or primary if we want
// to be safe from linking errors. For this example, let's use the robust popen
// method to avoid private header complexity in a simple C tool.

void check_sip_status(void) {
  FILE *fp;
  char path[1035];
  int sip_enabled = 0;

  /* Open the command for reading. */
  fp = popen("csrutil status", "r");
  if (fp == NULL) {
    printf("  [-] SIP: Failed to check (popen error)\n");
    return;
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path), fp) != NULL) {
    if (strstr(path, "System Integrity Protection status: enabled") != NULL) {
      sip_enabled = 1;
    }
  }

  pclose(fp);

  if (sip_enabled) {
    printf("  [+] SIP: Enabled\n");
  } else {
    printf("  [!] SIP: DISABLED (High Risk)\n");
  }
}

void check_gatekeeper_status(void) {
  FILE *fp;
  char path[1035];
  int gk_enabled = 0;

  fp = popen("spctl --status", "r");
  if (fp == NULL) {
    printf("  [-] Gatekeeper: Failed to check\n");
    return;
  }

  while (fgets(path, sizeof(path), fp) != NULL) {
    if (strstr(path, "assessments enabled") != NULL) {
      gk_enabled = 1;
    }
  }

  pclose(fp);

  if (gk_enabled) {
    printf("  [+] Gatekeeper: Enabled\n");
  } else {
    printf("  [!] Gatekeeper: DISABLED\n");
  }
}
