#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

BOOLEAN Confirm(const char *format, ...) {
    va_list vargs;
    char msg[1024];

    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);

    fprintf(stderr, msg);
    fprintf(stderr, "\n");
    return FALSE;
}

BOOLEAN Warning(const char *format, ...) {
    va_list vargs;
    char msg[1024];

    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);

    fprintf(stderr, msg);
    fprintf(stderr, "\n");

    return FALSE;
}

void Error(const char *format, ...) {
    va_list vargs;
    char msg[1024];

    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);

    fprintf(stderr, msg);
    fprintf(stderr, "\n");

    exit(-1);
}
