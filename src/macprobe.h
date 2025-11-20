#ifndef MACPROBE_H
#define MACPROBE_H

#include <stdbool.h>

// System Info
void print_system_info(void);
bool is_virtual_machine(void);

// Security Checks
void check_sip_status(void);
void check_gatekeeper_status(void);
void check_tcc_permissions(void);

#endif
