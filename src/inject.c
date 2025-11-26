#include <stdio.h>
#include <syslog.h>

__attribute__((constructor)) static void customConstructor(int argc,
                                                           const char **argv) {
  (void)argc;
  (void)argv;
  printf("\n[+] MacProbe: Injection Successful\n");
  syslog(LOG_ERR, "MacProbe: Dylib injection successful in %s\n", argv[0]);
}
