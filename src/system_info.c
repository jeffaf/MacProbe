#include "macprobe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <sys/types.h>

void print_system_info(void) {
  char model[256];
  size_t len = sizeof(model);
  char os_version[256];
  size_t os_len = sizeof(os_version);

  if (sysctlbyname("hw.model", model, &len, NULL, 0) == 0) {
    printf("Model: %s\n", model);
  } else {
    printf("Model: Unknown\n");
  }

  if (sysctlbyname("kern.osproductversion", os_version, &os_len, NULL, 0) ==
      0) {
    printf("OS Version: %s\n", os_version);
  } else {
    // Fallback for older macOS
    if (sysctlbyname("kern.osrelease", os_version, &os_len, NULL, 0) == 0) {
      printf("Kernel Release: %s\n", os_version);
    }
  }

  if (is_virtual_machine()) {
    printf("Environment: VIRTUAL MACHINE DETECTED!\n");
  } else {
    printf("Environment: Physical Hardware (Likely)\n");
  }
}

bool is_virtual_machine(void) {
  int is_vm = 0;
  size_t len = sizeof(is_vm);

  // Check 1: sysctl kern.hv_vmm_present (Apple Silicon)
  if (sysctlbyname("kern.hv_vmm_present", &is_vm, &len, NULL, 0) == 0) {
    if (is_vm == 1)
      return true;
  }

  // Check 2: machdep.cpu.features for VMM
  char features[1024];
  len = sizeof(features);
  if (sysctlbyname("machdep.cpu.features", features, &len, NULL, 0) == 0) {
    if (strstr(features, "VMM") != NULL)
      return true;
  }

  return false;
}
