#include "Enclave_t.h" /* print_string */

#include <stdio.h> /* vsnprintf */
#include <string.h>
#include <algorithm>    // std::max


int printf(const char* fmt, ...)
{
    char buf[BUFSIZ] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}


// the actual buffer of *inp is in untrusted memory
// You can read from it, but never write to it
int ecall_compute_secrete_operation(int* inp, int size) {
    // decrypt inp
    // ....

    int res = 0;

    for (int i = 0; i < size; i++) {
        res += inp[i];
    }

    // encrypt res
    // ....

    printf("Returning to App.cpp\n");
    return res;
}
