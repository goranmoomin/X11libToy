// Toy/message.h
//
// LastModified : 2002/10/31
//

#ifndef _TOY_MESSAGE_H_
#define _TOY_MESSAGE_H_

#include <stdarg.h>
#include <stdlib.h>
#ifdef TOY
#include <Toy/util.h>
#else
#include "util.h"
#endif

inline BOOLEAN Confirm(const char *format, ...) {
    va_list vargs;
    char msg[1024];
    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);
#ifdef __AFX_H__
    if(AfxMessageBox(msg, MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
        return TRUE;
    else
        return FALSE;
#else
    fprintf(stderr, msg);
    fprintf(stderr, "\n");
    return FALSE;
#endif
}

inline BOOLEAN Warning(const char *format, ...) {
    va_list vargs;
    char msg[1024];
    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);
#ifdef __AFX_H__
    AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION);
#else
    fprintf(stderr, msg);
    fprintf(stderr, "\n");
#endif
    return FALSE;
}

inline void Error(const char *format, ...) {
    va_list vargs;
    char msg[1024];
    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);
#ifdef __AFX_H__
    AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION);
#else
    fprintf(stderr, msg);
    fprintf(stderr, "\n");
    exit(-1);
#endif
}

inline void Echo(FILE *fp, const char *format, ...) {
    va_list vargs;
    char msg[1024];
    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);
    printf(msg);
    fprintf(fp, msg);
}

#endif
