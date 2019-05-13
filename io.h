// Toy/io.h
//
// Last Modified : 2011/02/01
//

#ifndef _TOY_IO_H_
#define _TOY_IO_H_

#ifdef TOY
#include <Toy/util.h>
#else
#include "util.h"
#endif

size_t GetFileSize(const char *filename);
time_t GetFileTime(const char *filename);
BOOLEAN IsFileExist(const char *format, ...) {
    va_list vargs;
    char filename[1024];
    va_start(vargs, format);
    vsprintf(filename, format, vargs);
    va_end(vargs);
    FILE *fp = fopen(filename, "r");
    if(fp) {
        fclose(fp);
        return TRUE;
    }
    return FALSE;
}

//
/*
template <class Type> BOOLEAN fwrite(const Type &t, FILE *fp) {
    return (fwrite(&t, sizeof(Type), 1, fp) == 1) ? TRUE : FALSE;
}
template <class Type> BOOLEAN fwrite(const Type t[], size_t count, FILE *fp) {
    return (fwrite(t, sizeof(Type), count, fp) == count) ? TRUE : FALSE;
}
template <class Type> BOOLEAN fread(Type &t, FILE *fp) {
    return (fread(&t, sizeof(Type), 1, fp) == 1) ? TRUE : FALSE;
}
template <class Type> BOOLEAN fread(Type t[], size_t count, FILE *fp) {
    return (fread(t, sizeof(Type), count, fp) == count) ? TRUE : FALSE;
}
*/

//
BOOLEAN fwrite(const char *str, FILE *fp);
BOOLEAN fwrite(const CString &str, FILE *fp);
BOOLEAN fwrite(const CString str[], size_t count, FILE *fp);
BOOLEAN fread(CString &str, FILE *fp);
BOOLEAN fread(CString str[], size_t count, FILE *fp);

BOOLEAN CreateFile(const char *filename) {
    FILE *fp = fopen(filename, "wt");
    if(!fp) return FALSE;
    fclose(fp);
    return TRUE;
}

BOOLEAN AppendFile(const char *filename, const char *format, ...) {
    va_list vargs;
    char msg[1024];

    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);

    FILE *fp = fopen(filename, "at");
    if(!fp) return FALSE;
    fprintf(fp, msg);
    fclose(fp);
    return TRUE;
}

BOOLEAN RedirectFile(const char *filename, const char *format, ...) {
    va_list vargs;
    char msg[1024];

    va_start(vargs, format);
    vsprintf(msg, format, vargs);
    va_end(vargs);

    FILE *fp = fopen(filename, "at");
    if(!fp) return FALSE;
    fprintf(fp, msg);
    fclose(fp);
    printf(msg);
    return TRUE;
}

#endif
