// Toy/util.h
//
// Last Modified : 2002/10/31
//

#ifndef _TOY_UTIL_H_
#define _TOY_UTIL_H_

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BOOLEAN;
#ifndef TRUE
#define TRUE (BOOLEAN)1
#endif
#ifndef FALSE
#define FALSE (BOOLEAN)0
#endif

#ifndef __AFX_H__
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef long BOOL;
typedef long LONG;
typedef unsigned long ULONG;
#endif

#define SEP_CHARS " ,()\t\n\r"      // Separator
#define FLOAT_CHARS "0123456789.+-" // charaters for float
#define INT_CHARS "0123456789+-"    // charaters for int

inline BOOLEAN IsInt(const char *str) {
    return strspn(str, INT_CHARS) == strlen(str) ? TRUE : FALSE;
}
inline BOOLEAN IsFloat(const char *str) {
    return strspn(str, FLOAT_CHARS) == strlen(str) ? TRUE : FALSE;
}
inline BOOLEAN IsDouble(const char *str) { return IsFloat(str); }

template <class Type> void Swap(Type *x, Type *y) {
    Type z = *x;
    *x = *y;
    *y = z;
}
template <class Type> void Swap(Type &x, Type &y) {
    Type z = x;
    x = y;
    y = z;
}
template <class Type> BOOLEAN Find(Type y[], int size, const Type &x) {
    for(int i = 0; i < size; i++)
        if(x == y[i]) return TRUE;
    return FALSE;
}
template <class Type>
BOOLEAN Find(Type y[], int size, const Type &x, int *pos) {
    for(*pos = 0; *pos < size; (*pos)++)
        if(x == y[*pos]) return TRUE;
    return FALSE;
}

template <class Type>
BOOLEAN BinarySearch(Type y[], int size, const Type &x, int *pos) {
    int min = 0, max = size - 1;
    while(min <= max) {
        *pos = (min + max) / 2;
        if(y[*pos] > x)
            max = (*pos) - 1;
        else if(y[*pos] < x)
            min = (*pos) + 1;
        else
            return TRUE;
    }
    *pos = min;
    return FALSE;
}
template <class Type> int BinarySearch(Type y[], int size, const Type &x) {
    int pos;
    ::BinarySearch(y, size, x, &pos);
    return pos;
}
template <class Type> int OrderedInsert(Type y[], int size, const Type &x) {
    for(int i = size; i; i--) {
        if(y[i - 1] > x)
            y[i] = y[i - 1];
        else
            return i;
    }
    return 0;
}

inline void SwapBytes(BYTE *bytes, int nBytes) {
    for(int i = (nBytes - 1) / 2; i >= 0; i--)
        ::Swap(bytes[i], bytes[nBytes - i - 1]);
}
template <class Type> void SwapBytes(Type &t) {
    SwapBytes((BYTE *)&t, sizeof(t));
}

#ifdef __AFX_H__

inline BOOLEAN IsEscaped() {
    MSG msg;
    while(PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE)) {
        if(msg.message == WM_KEYDOWN || msg.wParam == VK_ESCAPE) return TRUE;
    }
    return FALSE;
}

inline void OnBitmapButton(CButton &button) {
    //    button.ModifyStyle(BS_TOP, BS_VCENTER);
    button.ModifyStyleEx(WS_EX_DLGMODALFRAME, NULL);
    button.ShowWindow(SW_HIDE);
    button.ShowWindow(SW_SHOW);
}

inline void OffBitmapButton(CButton &button) {
    //    button.ModifyStyle(BS_VCENTER, BS_TOP);
    button.ModifyStyleEx(NULL, WS_EX_DLGMODALFRAME);
    button.ShowWindow(SW_HIDE);
    button.ShowWindow(SW_SHOW);
}

#else

typedef char TCHAR;
typedef const TCHAR *LPCSTR;
typedef const TCHAR *LPCTSTR;

class CString {
    char *m_buf;
    int m_bufSize;

    void Init() {
        m_buf = NULL;
        m_bufSize = 0;
    }
    void Alloc(int size) {
        if(size > m_bufSize) { // new allocation
            int bufSize = ((size + 63) / 64) * 64;
            char *buf = new char[bufSize];
            for(int i = 0; i < m_bufSize; i++) buf[i] = m_buf[i];
            Free();
            m_bufSize = bufSize;
            m_buf = buf;
        }
    }
    void Free() {
        if(m_buf && m_bufSize > 0) delete[](m_buf);
        m_buf = NULL;
        m_bufSize = 0;
    }

  public:
    // Constructors

    CString() { // constucts empty CString
        Init();
    }
    CString(const CString &string) { // copy constructor
        Init();
        operator=(string);
    }
    CString(TCHAR ch, int nRepeat = 1) { // from a single character
        Init();
        Alloc(nRepeat + 1);
        for(int i = 0; i < nRepeat; i++) m_buf[i] = ch;
        m_buf[nRepeat] = 0x00;
    }
    CString(LPCSTR lpsz) { // from an ANSI string
        Init();
        operator=(lpsz);
    }

    // Attributes & Operations

    int GetLength() const { // get data length
        int pos = 0;
        if(m_buf)
            for(; pos < m_bufSize && m_buf[pos]; pos++)
                ;
        return pos;
    }
    BOOLEAN IsEmpty() const { // TRUE if zero length
        return (m_buf && m_buf[0]) ? FALSE : TRUE;
    }
    void Empty() { // clear contents to empty
        if(m_buf) m_buf[0] = 0x00;
    }

    // return single character at zero-based index
    TCHAR GetAt(int nIndex) const { return m_buf[nIndex]; }
    // return single character at zero-based index
    TCHAR operator[](int nIndex) const { return m_buf[nIndex]; }
    // set a single character at zero-based index
    void SetAt(int nIndex, TCHAR ch) {
        if(nIndex < m_bufSize) m_buf[nIndex] = ch;
    }
    // return pointer to const string
    operator LPCTSTR() const { return m_buf; }

    // overloaded assignment

    // ref-counted copy from another CString
    const CString &operator=(const CString &stringSrc) {
        int length = stringSrc.GetLength();
        Alloc(length + 1);
        for(int i = 0; i <= length; i++) m_buf[i] = stringSrc.m_buf[i];
        return (*this);
    }
    // set string content to single character
    const CString &operator=(TCHAR ch) {
        Alloc(2);
        m_buf[0] = ch;
        m_buf[1] = 0x00;
        return (*this);
    }
    // copy string content from ANSI string (converts to TCHAR)
    const CString &operator=(LPCSTR lpsz) {
        Alloc(strlen(lpsz) + 1);
        int i = 0;
        do { m_buf[i] = lpsz[i]; } while(lpsz[i++]);
        return (*this);
    }
    // copy string content from unsigned chars
    const CString &operator=(const unsigned char *psz) {
        return operator=((LPCTSTR)psz);
    }

    // string concatenation

    // concatenate from another CString
    const CString &operator+=(const CString &string) {
        return operator+=(string.m_buf);
    }
    // concatenate a single character
    const CString &operator+=(TCHAR ch) {
        int length = GetLength();
        Alloc(GetLength() + 1);
        m_buf[length] = ch;
        m_buf[length + 1] = 0x00;
        return (*this);
    }
    // concatenate a UNICODE character after converting it to TCHAR
    const CString &operator+=(LPCTSTR lpsz) {
        int length = GetLength();
        Alloc(length + strlen(lpsz));
        for(int i = 0; lpsz[i]; i++) m_buf[length++] = lpsz[i];
        m_buf[length] = 0x00;
        return (*this);
    }

    friend CString operator+(const CString &string1, const CString &string2);
    friend CString operator+(const CString &string, TCHAR ch);
    friend CString operator+(TCHAR ch, const CString &string);
    friend CString operator+(const CString &string, LPCTSTR lpsz);
    friend CString operator+(LPCTSTR lpsz, const CString &string);

    // searching

    // find character starting at left, -1 if not found
    int Find(TCHAR ch) const;
    // find character starting at right
    int ReverseFind(TCHAR ch) const;
    // find character starting at zero-based index and going right
    int Find(TCHAR ch, int nStart) const;
    // find first instance of any character in passed string
    int FindOneOf(LPCTSTR lpszCharSet) const;
    // find first instance of substring
    int Find(LPCTSTR lpszSub) const;
    // find first instance of substring starting at zero-based index
    int Find(LPCTSTR lpszSub, int nStart) const;

    // simple formatting

    // printf-like formatting using passed string
    void Format(LPCTSTR lpszFormat, ...);

    // simple sub-string extraction

    // return nCount characters starting at zero-based nFirst
    CString Mid(int nFirst, int nCount) const;
    // return all characters starting at zero-based nFirst
    CString Mid(int nFirst) const;
    // return first nCount characters in string
    CString Left(int nCount) const;
    // return nCount characters from end of string
    CString Right(int nCount) const;

    //  characters from beginning that are also in passed string
    CString SpanIncluding(LPCTSTR lpszCharSet) const;
    // characters from beginning that are not also in passed string
    CString SpanExcluding(LPCTSTR lpszCharSet) const;

    // upper/lower/reverse conversion

    // NLS aware conversion to uppercase
    void MakeUpper();
    // NLS aware conversion to lowercase
    void MakeLower();
    // reverse string right-to-left
    void MakeReverse();

    // trimming whitespace (either side)

    // remove whitespace starting from right edge
    void TrimRight();
    // remove whitespace starting from left side
    void TrimLeft();

    // trimming anything (either side)

    // remove continuous occurrences of chTarget starting from right
    void TrimRight(TCHAR chTarget);
    // remove continuous occcurrences of characters in passed string,
    // starting from right
    void TrimRight(LPCTSTR lpszTargets);
    // remove continuous occurrences of chTarget starting from left
    void TrimLeft(TCHAR chTarget);
    // remove continuous occcurrences of characters in
    // passed string, starting from left

    void TrimLeft(LPCTSTR lpszTargets);

    friend BOOLEAN operator==(const CString &s1, const CString &s2);
    friend BOOLEAN operator==(const CString &s1, LPCTSTR s2);
    friend BOOLEAN operator==(LPCTSTR s1, const CString &s2);
    friend BOOLEAN operator!=(const CString &s1, const CString &s2);
    friend BOOLEAN operator!=(const CString &s1, LPCTSTR s2);

    // Implementation
  public:
    ~CString() { Free(); }

  protected:
};

inline BOOLEAN operator==(const CString &s1, const CString &s2) {
    return strcmp(s1.m_buf, s2.m_buf) == 0 ? TRUE : FALSE;
}
inline BOOLEAN operator==(const CString &s1, LPCTSTR s2) {
    return strcmp(s1.m_buf, s2) == 0 ? TRUE : FALSE;
}
inline BOOLEAN operator==(LPCTSTR s1, const CString &s2) {
    return strcmp(s1, s2.m_buf) == 0 ? TRUE : FALSE;
}
inline BOOLEAN operator!=(const CString &s1, const CString &s2) {
    return operator!=(s1, s2) ? FALSE : TRUE;
}
inline BOOLEAN operator!=(const CString &s1, LPCTSTR s2) {
    return operator!=(s1, s2) ? FALSE : TRUE;
}
inline BOOLEAN operator!=(LPCTSTR s1, const CString &s2) {
    return operator!=(s1, s2) ? FALSE : TRUE;
}

#endif

//
inline char *GetFormat(BOOLEAN &) {
    static char format[] = "%d";
    return format;
}
inline char *GetFormat(int &) {
    static char format[] = "%d";
    return format;
}
inline const char *GetFormat(long &) {
    static char format[] = "%d";
    return format;
}
inline const char *GetFormat(float &) {
    static char format[] = "%f";
    return format;
}
inline const char *GetFormat(double &) {
    static char format[] = "%lf";
    return format;
}
inline const char *GetFormat(char &) {
    static char format[] = "%c";
    return format;
}
inline const char *GetFormat(char *) {
    static char format[] = "%s";
    return format;
}

//
inline char *GetNextToken(char *str, const char *separators = SEP_CHARS) {
    if(str == NULL) return NULL;
    char *next = str;
    for(; *next && strchr(separators, *next); next++)
        ;
    for(; *next && !strchr(separators, *next); next++)
        ;
    return next;
}

#define ToString(s) ToStr(s)

#ifdef __AFX_H__
inline CString ToStr(int d) {
    CString str;
    str.Format("%d", d);
    return str;
}
inline CString ToStr(double lf) {
    CString str;
    str.Format("%f", lf);
    if(str.Find('.')) str.TrimRight('0');
    str.TrimRight('.');
    return str;
}
#else
inline char *ToStr(int d) {
    static char str[32];
    sprintf(str, "%d", d);
    return str;
}
inline char *ToStr(double lf) {
    static char str[32];
    sprintf(str, "%f", lf);
    for(char *c = str; *c; c++)
        if(*c == '.') {
            for(; *c; c++)
                ;
            for(c--; *c == '0'; c--) *c = 0x00;
            break;
        }
    if(str[strlen(str) - 1] == '.') str[strlen(str) - 1] = 0x00;
    return str;
}
inline char *ToStr(float f) { return ::ToStr((double)f); }
inline char *ToStr(const char *s) {
    static char str[32];
    sprintf(str, "%s", s);
    return str;
}
#endif

inline char *ToStr(const char *format, ...) {
    va_list vargs;
    static char str[1024];
    va_start(vargs, format);
    vsprintf(str, format, vargs);
    va_end(vargs);
    return str;
}

//
inline BOOLEAN GetValue(const char *str, int *i) {
    return (sscanf(str, "%d", i) == 1) ? TRUE : FALSE;
}
inline BOOLEAN GetValue(const char *str, int &i) { return GetValue(str, &i); }
inline BOOLEAN GetValue(const char *str, float *f) {
    return (sscanf(str, "%f", f) == 1) ? TRUE : FALSE;
}
inline BOOLEAN GetValue(const char *str, float &f) { return GetValue(str, &f); }
inline BOOLEAN GetValue(const char *str, double *d) {
    return (sscanf(str, "%lf", d) == 1) ? TRUE : FALSE;
}
inline BOOLEAN GetValue(const char *str, double &d) {
    return GetValue(str, &d);
}
inline BOOLEAN GetValue(const char *str, CString *s) {
    *s = str;
    BOOLEAN result = (sscanf(str, "%s", (LPCTSTR)(*s)) == 1) ? TRUE : FALSE;
    return result;
}
inline BOOLEAN GetValue(const char *str, CString &s) {
    return GetValue(str, &s);
}

template <class Type> BOOLEAN IsValid(const char *str) {
    Type type;
    return GetValue(str, type);
}
inline BOOLEAN IsValidInt(const char *str) { return IsValid<int>(str); }
inline BOOLEAN IsValidFloat(const char *str) { return IsValid<float>(str); }
inline BOOLEAN IsValidDouble(const char *str) { return IsValid<double>(str); }
inline BOOLEAN IsValidString(const char *str) { return IsValid<CString>(str); }

inline int ToInt(const char *str) {
    int d;
    GetValue(str, d);
    return d;
}
inline float ToFloat(const char *str) {
    float f;
    GetValue(str, f);
    return f;
}
inline double ToDouble(const char *str) {
    float lf;
    GetValue(str, lf);
    return lf;
}

//
template <class Type> int GetValids(char *str) {
    CString token;
    int nValids = 0;
    for(char *next = str; *next; next = GetNextToken(next))
        if(IsValid<Type>(next)) nValids++;
    return nValids;
}
inline int GetValidInts(char *str) { return GetValids<int>(str); }
inline int GetValidFloats(char *str) { return GetValids<float>(str); }
inline int GetValidDoubles(char *str) { return GetValids<double>(str); }
// int GetValidStrings(char *str) { return GetValids<CString &>(str); }

template <class Type> int GetValues(char *str, Type array[], int nMax) {
    int nValids = 0;
    for(char *next = str; *next && nValids < nMax; next = GetNextToken(next))
        if(GetValue(next, array[nValids]))
            nValids++;
        else
            break;
    return nValids;
}

#define LOOP(i, f, t, s) for(i = f; i <= t; i += s)
#define LOOP_INT(i, f, t, s) for(int i = f; i <= t; i += s)
#define LOOP_FLOAT(i, f, t, s) for(float i = f; i <= t + TolZero; i += s)
#define LOOP_DOUBLE(i, f, t, s) for(double i = f; i <= t + TolZero; i += s)

/*
size_t	GetFileSize(const char *filename);
time_t	GetFileTime(const char *filename);
BOOLEAN	IsFileAccessable(const char *format, ...)
{
    va_list vargs;
    char    filename[1024];
    va_start(vargs, format);
    vsprintf(filename, format, vargs);
    va_end(vargs);
    FILE *fp = fopen(filename, "r");
    if (fp) {
        fclose(fp);
        return TRUE;
    }
    return FALSE;
}
*/

template <class Type> inline BOOLEAN fwrite(const Type *t, FILE *fp) {
    return (fwrite(t, sizeof(Type), 1, fp) == 1);
}
template <class Type>
inline BOOLEAN fwrite(const Type t[], size_t size, FILE *fp) {
    return (fwrite(t, sizeof(Type), size, fp) == size);
}
template <class Type> inline BOOLEAN fread(Type *t, FILE *fp) {
    return (fread(t, sizeof(Type), 1, fp) == 1);
}
template <class Type> inline BOOLEAN fread(Type t[], size_t size, FILE *fp) {
    return (fread(t, sizeof(Type), size, fp) == size);
}

/*
//
BOOLEAN fwrite(const char *str, FILE *fp);
BOOLEAN fwrite(const CString &str, FILE *fp);
BOOLEAN fwrite(const CString str[], size_t size, FILE *fp);
BOOLEAN fread(CString &str, FILE *fp);
BOOLEAN fread(CString str[], size_t size, FILE *fp);
*/

inline FILE *FileCreate(const char *format, ...) {
    va_list vargs;
    char filename[1024];

    va_start(vargs, format);
    vsprintf(filename, format, vargs);
    va_end(vargs);

    return fopen(filename, "w");
}

inline FILE *FileOpen(const char *format, ...) {
    va_list vargs;
    char filename[1024];

    va_start(vargs, format);
    vsprintf(filename, format, vargs);
    va_end(vargs);

    return fopen(filename, "r");
}

inline BOOLEAN FileAppend(const char *filename, const char *format, ...) {
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

inline BOOLEAN FileRedirect(const char *filename, const char *format, ...) {
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
