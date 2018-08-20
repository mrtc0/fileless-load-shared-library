#include <stdio.h>

void __attribute__ ((constructor)) hello_init(void);

void hello_init(void) {
    fprintf(stdout,"[+] Hello!\n");
}
