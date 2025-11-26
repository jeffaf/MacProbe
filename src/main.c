#include "macprobe.h"
#include <stdio.h>

int main(void) {
  printf("MacProbe - MacOS Security Check\n");
  printf("====================================\n\n");

  print_system_info();

  printf("\n[+] Security Posture:\n");
  check_sip_status();
  check_gatekeeper_status();
  check_tcc_permissions();
  scan_applications_for_entitlements();

  return 0;
}
