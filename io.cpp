#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#endif
#include <sys/stat.h>
#ifdef TOY
#include "io.h"
#else
#include "io.h"
#endif

size_t GetFileSize(const char *filename) {
    CFileFind finder;
    if(!finder.FindFile(filename)) return 0;

    struct stat buf;
    stat(filename, &buf);
    return (size_t)buf.st_size;
}

time_t GetFileTime(const char *filename) {
    CFileFind finder;
    if(!finder.FindFile(filename)) return 0;

    struct stat buf;
    stat(filename, &buf);
    return buf.st_atime;
}

BOOLEAN IsFileExist(const char *filename) {
    CFileFind finder;
    return finder.FindFile(filename) ? TRUE : FALSE;

    /*
        FILE *fp = fopen(filename, "r");
        if (fp) {
            fclose(fp);
            return TRUE;
        }
        return FALSE;
    */
}

//
BOOLEAN fwrite(const char *str, FILE *fp) {
    size_t length = strlen(str);
    if(fwrite(&length, sizeof(size_t), 1, fp) != 1) return FALSE;
    if(fwrite(str, sizeof(char), length, fp) != length) return FALSE;
    return TRUE;
}

BOOLEAN fwrite(const CString &str, FILE *fp) {
    size_t length = str.GetLength();
    if(fwrite(&length, sizeof(size_t), 1, fp) != 1) return FALSE;
    if(fwrite((LPCTSTR)str, sizeof(char), length, fp) != length) return FALSE;
    return TRUE;
}

BOOLEAN fwrite(const CString str[], size_t count, FILE *fp) {
    for(size_t i = 0; i < count; i++)
        if(!fwrite(str[i], fp)) return FALSE;
    return TRUE;
}

BOOLEAN fread(CString &str, FILE *fp) {
    static char buffer[1024];
    size_t length;
    if(fread(&length, sizeof(size_t), 1, fp) != 1) return FALSE;
    if(length == 0) {
        str = "";
        return TRUE;
    }
    if(length >= 1024) return FALSE;
    if(fread(&buffer, sizeof(char), length, fp) != length) return FALSE;
    buffer[length] = 0x00;
    str = buffer;
    return TRUE;
}

BOOLEAN fread(CString str[], size_t count, FILE *fp) {
    for(size_t i = 0; i < count; i++)
        if(!fread(str[i], fp)) return FALSE;
    return TRUE;
}
