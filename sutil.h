// Toy/sutil.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_SUITL_H_
#define _TOY_SUITL_H_

#include "string.h"

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

#ifndef WIN32

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
    void Alloc(int bufSize) {
        if(size > m_bufSize) { // new allocation
            bufSize = ((bufsize + 63) / 64) * 64;
            Type *buf = new Type[bufSize];
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
        for(int i = 0; i < nRepeat; i++) m_buffer[i] = ch;
        m_buffer[nRepeat] = 0x00;
    }
    CString(LPCSTR lpsz) { // from an ANSI string
        Init();
        operator=(lpsz);
    }

    // Attributes & Operations

    int GetLength() const { // get data length
        return m_buffer ? strlen(m_buffer) : 0;
    }
    BOOLEAN IsEmpty() const { // TRUE if zero length
        return (m_buffer && m_buffer[0]) ? FALSE : TRUE;
    }
    void Empty() { // clear contents to empty
        if(m_buffer) m_buffer[0] = 0x00;
    }

    // return single character at zero-based index
    TCHAR GetAt(int nIndex) const { return m_buffer[nIndex]; }
    // return single character at zero-based index
    TCHAR operator[](int nIndex) const { return m_buffer[nIndex]; }
    // set a single character at zero-based index
    void SetAt(int nIndex, TCHAR ch) {
        if(nIndex < m_bufSize) m_buffer[nIndex] = ch;
    }
    // return pointer to const string
    operator LPCTSTR() const { return m_buffer; }

    // overloaded assignment

    // ref-counted copy from another CString
    const CString &operator=(const CString &stringSrc) {
        int length = stringSrc.GetLength();
        Alloc(length + 1);
        for(int i = 0; i <= length; i++) m_buffer[i] = stringSrc.m_buffer[i];
        return (*this);
    }
    // set string content to single character
    const CString &operator=(TCHAR ch) {
        Alloc(2);
        m_buffer[0] = ch;
        m_buffer[1] = 0x00;
        return (*this);
    }
    // copy string content from ANSI string (converts to TCHAR)
    const CString &operator=(LPCSTR lpsz) {
        Alloc(strlen(lpsz) + 1);
        for(int i = strlen(lpsz); i >= 0; i--) m_buffer[i] = lpsz[i];
        return (*this);
    }
    // copy string content from unsigned chars
    const CString &operator=(const unsigned char *psz) {
        return operator=((LPCTSTR)psz);
    }

    // string concatenation

    // concatenate from another CString
    const CString &operator+=(const CString &string) {
        return operator+=(string.m_buffer);
    }
    // concatenate a single character
    const CString &operator+=(TCHAR ch) {
        int length = GetLength();
        Alloc(GetLength() + 1);
        m_buffer[length] = ch;
        m_buffer[length + 1] = 0x00;
    }
    // concatenate a UNICODE character after converting it to TCHAR
    const CString &operator+=(LPCTSTR lpsz) {
        int length = GetLength();
        Alloc(length + strlen(lpsz));
        for(int i = 0; lpsz[i]; i++) m_buffer[length++] = lpsz[i];
        m_buffer[length] = 0x00;
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
    return strcmp(s1.m_buffer, s2.m_buffer) == 0 ? TRUE : FALSE;
}
inline BOOLEAN operator==(const CString &s1, LPCTSTR s2) {
    return strcmp(s1.m_buffer, s2) == 0 ? TRUE : FALSE;
}
inline BOOLEAN operator==(LPCTSTR s1, const CString &s2) {
    return strcmp(s1, s2.m_buffer) == 0 ? TRUE : FALSE;
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

#endif
