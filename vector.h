// Toy/vector.h
//
// Last Modified : 2006/12/29
//

#ifndef _TOY_VECTOR_H_
#define _TOY_VECTOR_H_

#ifdef TOY
#include <Toy/message.h>
#include <Toy/mutil.h>
#include <Toy/util.h>
#else
#include "message.h"
#include "mutil.h"
#include "util.h"
#endif

#define LOOP_ARRAY(v, i) for(i = (v).m_from; i <= (v).m_to; i++)
#define LOOP_ARRAY_INT(v, i) for(int i = (v).m_from; i <= (v).m_to; i++)
#define LOOP_VECTOR(v, i) for(i = (v).m_from; i <= (v).m_to; i++)
#define LOOP_VECTOR_INT(v, i) for(int i = (v).m_from; i <= (v).m_to; i++)

#define IS_MATCH_ARRAY(v1, v2)                                                 \
    ((v1).From() == (v2).From() && (v2).To() == (v2).To())

#ifdef _DEBUG
#define CHECK_MATCH(v1, v2)                                                    \
    if(!IS_MATCH_ARRAY(v1, v2)) Error("Range of arrays are different.")
#define CHECK_SIZE(v, s)                                                       \
    if(v.Size() < s) Error("Size of array is less than %d.", s)
#define CHECK_RANGE(v, i)                                                      \
    if((int)(i) < (v).From() || (int)(i) > (v).To())                           \
    Error(                                                                     \
        "Out of range in class Array : "                                       \
        "From() = %d, To() = %d, Ref = %d\n",                                  \
        (v).From(), (v).To(), i)
#else
#define CHECK_MATCH(v1, v2)
#define CHECK_RANGE(v, i)
#define CHECK_SIZE(v, s)
#endif

template <class Type> class Array2D;
template <class Type> class Array3D;
template <class Type> class DoubleArray;
template <class Type> class TripleArray;

/////////////////////////////////////////////////////////////////////////////
// Array

#define _LOOP_ARRAY_(i) for(int i = m_from; i <= m_to; i++)
#define _LOOP_VECTOR_(i) for(int i = m_from; i <= m_to; i++)

template <class Type> class Array {
  public:
    int m_from, m_to;

  private:
    int m_size;
    int m_bufSize;
    int m_blockSize; // size of block to allocate

  public:
    Type *m_buf;

    // Constructor and Destructor
  public:
    Array(int size = 0) {
        Init();
        Alloc(0, size - 1);
    }
    Array(int from, int to) {
        Init();
        Alloc(from, to);
    }
    Array(const Array<Type> &v) {
        Init();
        Alloc(v);
        *this = v;
    }
    // Array(const Type a[], const int size)  { Init();  Alloc(a, size);
    // }
    Array(const char *fn, int offset = 0) {
        Init();
        Load(fn, "rt", offset);
    }
    Array(const char *fn, const char *mode, int offset = 0) {
        Init();
        Load(fn, mode, offset);
    }
    ~Array() { Free(); }

    // Operator
  private:
    void Init() {
        m_buf = NULL;
        m_from = 0;
        m_to = -1;
        m_size = m_bufSize = 0;
        m_blockSize = 8;
    }

  public:
    Type *Alloc(int size) { return Alloc(0, size - 1); }
    Type *Alloc(const Type a[], const int size) {
        return Alloc(0, size - 1);
        *this = a;
    }
    Type *Alloc(const Array<Type> &v) { return Alloc(v.From(), v.To()); }
    Type *Alloc(int from, int to) {
        int size = to - from + 1;
        if(size > m_bufSize) { // new allocation
            int bufSize =
                ((size + m_blockSize - 1) / m_blockSize) * m_blockSize;
            Type *buf = new Type[bufSize] - from;
            for(int i = m_from; i <= m_to; i++) buf[i] = m_buf[i];
            Free();
            m_bufSize = bufSize;
            m_buf = buf;
        } else if(size > 0) // use not-used area
            m_buf += m_from - from;
        m_from = from;
        m_to = to;
        m_size = size;
        return m_buf;
    }
    void Free() {
        if(m_buf && m_bufSize > 0) delete[](m_buf + m_from);
        m_buf = NULL;
        m_bufSize = 0;
        m_from = 0;
        m_to = -1;
    }

    int From() const { return m_from; }
    int To() const { return m_to; }
    int Size() const { return m_size; }
    int BufSize() const { return m_bufSize; }

    int SetSize(int size) {
        Alloc(m_from, m_from + size - 1);
        return Size();
    }
    void SetBlockSize(int blockSize) { m_blockSize = blockSize; }
    int IncreaseSize(int size) {
        Alloc(m_from, m_to + size);
        return Size();
    }
    Type Push(const Type &t) {
        IncreaseSize(1);
        Last() = t;
        return t;
    }
    Type Pop(int n = 1) {
        Type t = m_buf[m_to - n + 1];
        IncreaseSize(-n);
        return t;
    }

    BOOLEAN IsEmpty() const { return (Size() == 0); }
    BOOLEAN IsValid() const { return (Size() == 0); }
    BOOLEAN IsValid(int pos) const { return pos >= m_from && pos <= m_to; }

    void Set(Type t[], int size) {
        if(size > Size()) SetSize(size);
        for(int i = 0; i < size; i++) m_buf[i + m_from] = t[i];
    }

    Type GetMin() const { return ::GetMin(m_buf + m_from, Size()); }
    Type GetMin(int from, int to) const {
        return ::GetMin(m_buf + from - m_from, to - from + 1);
    }
    Type GetMax() const { return ::GetMax(m_buf + m_from, Size()); }
    Type GetMax(int from, int to) const {
        return ::GetMax(m_buf + from - m_from, to - from + 1);
    }
    int GetMaxPos() const {
        int pos = m_from;
        for(int i = m_from + 1; i <= m_to; i++)
            if(m_buf[pos] < m_buf[i]) pos = i;
        return pos;
    }
    int GetMinPos() const {
        int pos = m_from;
        for(int i = m_from + 1; i <= m_to; i++)
            if(m_buf[pos] > m_buf[i]) pos = i;
        return pos;
    }
    Type &First(int offset = 0) { return m_buf[m_from + offset]; }
    Type &First(int offset = 0) const { return m_buf[m_from + offset]; }
    Type &Last(int offset = 0) { return m_buf[m_to - offset]; }
    Type &Last(int offset = 0) const { return m_buf[m_to - offset]; }
    Type GetRange() const { return GetMax() - GetMin(); }

    void Insert(int pos, Type &x, BOOLEAN bIncreaseSize = TRUE) {
        if(pos < m_from || pos > m_to + (bIncreaseSize ? 1 : 0)) return;
        if(bIncreaseSize) IncreaseSize(1);
        for(int i = m_to; i > pos; i--) m_buf[i] = m_buf[i - 1];
        m_buf[pos] = x;
    }
    void Delete(int pos, BOOLEAN bDecreaseSize = TRUE) {
        if(pos < m_from || pos > m_to) return;
        for(int i = pos; i < m_to; i++) m_buf[i] = m_buf[i + 1];
        if(bDecreaseSize) IncreaseSize(-1);
    }
    BOOLEAN IsInDomain(int i) const {
        return i >= m_from && i <= m_to ? TRUE : FALSE;
    }

    BOOLEAN Load(const char *fn, const char *mode, int offset = 0) {
        if(strchr(mode, 't'))
            return LoadText(fn, offset);
        else if(strchr(mode, 'b'))
            return LoadBinary(fn);
        return FALSE;
    }
    BOOLEAN LoadText(const char *fn, int offset = 0) {
        Free();
        // count the line number
        FILE *fp = fopen(fn, "rt");
        if(!fp) return FALSE;
        Type t;
        char line[256];
        int size = 0;
        while(fgets(line, 256, fp))
            if(GetValue(line, &t)) size++;
        fclose(fp);
        // alloc memory
        Alloc(offset, offset + size - 1);
        // read data
        fp = fopen(fn, "rt");
        if(!fp) return FALSE;
        for(int pos = 0; pos < size && fgets(line, 256, fp);)
            if(GetValue(line, &m_buf[offset + pos])) pos++;
        fclose(fp);
        return TRUE;
    }
    BOOLEAN LoadBinary(const char *fn) {
        Free();
        FILE *fp = fopen(fn, "rb");
        if(!fp) return FALSE;
        BOOLEAN result = ReadBinary(fp);
        fclose(fp);
        return result;
    }
    BOOLEAN ReadBinary(FILE *fp) {
        Free();
        if(!fp) return FALSE;
        // read vector size
        int from, to;
        if(fread(&from, sizeof(int), 1, fp) != 1) return FALSE;
        if(fread(&to, sizeof(int), 1, fp) != 1) return FALSE;
        // alloc memory and copy
        Alloc(from, to);
        // read data
        if(fread(m_buf + m_from, sizeof(Type), (size_t)Size(), fp)
           != (size_t)Size()) {
            Alloc(0);
            return FALSE;
        }
        return TRUE;
    }
    BOOLEAN Save(const char *fn, const char *mode,
                 const char *format = NULL) const {
        FILE *fp = fopen(fn, mode);
        if(!fp) return FALSE;
        BOOLEAN result = FALSE;
        if(strchr(mode, 't'))
            result = WriteText(fp, format);
        else if(strchr(mode, 'b'))
            result = WriteBinary(fp);
        fclose(fp);
        return result;
    }
    BOOLEAN SaveText(const char *fn, const char *format = NULL) const {
        FILE *fp = fopen(fn, "wt");
        if(!fp) return FALSE;
        BOOLEAN result = WriteText(fp, format);
        fclose(fp);
        return result;
    }
    BOOLEAN SaveBinary(const char *fn) {
        FILE *fp = fopen(fn, "wb");
        if(!fp) return FALSE;
        BOOLEAN result = WriteBinary(fp);
        fclose(fp);
        return result;
    }
    BOOLEAN WriteText(FILE *fp, const char *format = NULL) const {
        if(!fp) return FALSE;
        if(!format)
            _LOOP_ARRAY_(i) fprintf(fp, "%s\n", ::ToString(m_buf[i]));
        else
            _LOOP_ARRAY_(i) fprintf(fp, format, m_buf[i]);
        // fprintf(fp, "\n");
        return TRUE;
    }
    BOOLEAN WriteBinary(FILE *fp) const {
        if(!fp) return FALSE;
        // write range of array
        if(fwrite(&m_from, sizeof(m_from), 1, fp) != 1) return FALSE;
        if(fwrite(&m_to, sizeof(m_to), 1, fp) != 1) return FALSE;
        // write data
        return fwrite(&m_buf[m_from], sizeof(Type), Size(), fp)
                       == (size_t)Size()
                   ? TRUE
                   : FALSE;
    }

    void LineSpace(Type min, Type max) { LineSpace(min, max, Size()); }
    void LineSpace(Type min, Type max, int size) {
        Alloc(size);
        if(min > max) {
            Type swap = min;
            min = max;
            max = min;
        }
        _LOOP_ARRAY_(i)
        m_buf[i] = min + (i - m_from) * (max - min) / (size - 1);
    }

    Type GetSum() const { return GetSum(m_from, m_to); }
    Type GetSum(int from, int to) const {
        Type sum = (Type)0;
        for(int i = from; i <= to; i++) sum += m_buf[i];
        return sum;
    }
    Type GetSum(Array<int> &index) const {
        Type sum = (Type)0;
        LOOP_ARRAY_INT(index, i) sum += m_buf[index[i]];
        return sum;
    }
    double GetMean() const { return GetMean(m_from, m_to); }
    double GetMean(int from, int to) const {
        return (double)GetSum(from, to) / (to - from + 1);
    }
    double GetMean(Array<int> &index) const {
        return (double)GetSum(index) / index.Size();
    }
    double GetCM(int from, int to) const {
        Type s1 = (Type)0, s2 = (Type)0;
        for(int i = from; i <= to; i++) {
            s1 += i * m_buf[i];
            s2 += m_buf[i];
        }
        return (double)s1 / s2;
    }
    double GetCM() const { return GetCM(m_from, m_to); }
    Type GetNorm() const { return (Type)sqrt(GetNorm2()); }
    Type GetNorm(int from, int to) const {
        return (Type)sqrt(GetNorm2(from, to));
    }
    Type GetNorm(Array<int> &index) const {
        return (Type)sqrt(GetNorm2(index));
    }
    Type GetNorm2() const { return GetNorm2(m_from, m_to); }
    Type GetNorm2(int from, int to) const {
        Type norm2 = (Type)0;
        for(int i = from; i <= to; i++) norm2 += m_buf[i] * m_buf[i];
        return norm2;
    }
    Type GetNorm2(Array<int> &index) const {
        Type norm2 = (Type)0;
        LOOP_ARRAY_INT(index, i) norm2 += SQR(m_buf[index.m_buf[i]]);
        return norm2;
    }
    double GetVar() const { return GetVar(m_from, m_to); }
    double GetVar(int from, int to) const {
        double mean = GetMean(from, to);
        double var = 0.;
        for(int i = from; i <= to; i++) var += SQR(mean - m_buf[i]);
        return var / (to - from + 1.);
    }
    double GetVar(Array<int> index) const {
        Type mean = GetMean(index);
        Type var = (Type)0;
        LOOP_ARRAY_INT(index, i) var += SQR(mean - m_buf[index[i]]);
        return (double)var / index.Size();
    }
    double GetStd() const { return sqrt(GetVar()); }
    double GetStd(int from, int to) const { return sqrt(GetVar(from, to)); }
    double GetStd(Array<int> index) const { return sqrt(GetVar(index)); }
    double GetCV() const {
        double mean = GetMean();
        double s2 = GetNorm2();
        return sqrt(s2 / (m_to - m_from + 1.) - mean * mean) / mean;
    }

    Array<Type> &Normalize(Type norm = (Type)1) {
        norm /= GetNorm();
        _LOOP_ARRAY_(i) m_buf[i] *= norm;
        return (*this);
    }
    Array<Type> GetUnit() const {
        Array<Type> unit(*this);
        unit /= GetNorm();
        return unit;
    }
    int Find(Type t) {
        _LOOP_ARRAY_(i) if(t == m_buf[i]) return i;
        return m_from - 1;
    }
    int Find(Type t, Type tolerance) {
        _LOOP_ARRAY_(i)
        if(t - tolerance <= m_buf[i] && t + tolerance >= m_buf[i]) return i;
        return m_from - 1;
    }
    int BinarySearch(const Type &x) {
        return ::BinarySearch(m_buf + m_from, m_size, x);
    }
    BOOLEAN BinarySearch(const Type &x, int *pos) {
        return ::BinarySearch(m_buf + m_from, m_size, x, pos);
    }

    void Reverse() {
        for(int i = (m_from + m_to - 1) / 2; i >= m_from; i--)
            ::Swap(m_buf[i], m_buf[m_to - i + m_from]);
    }

    void Diff(const Array<Type> v) {
        Alloc(v.From(), v.To() - 1);
        _LOOP_ARRAY_(i) m_buf[i] = v[i + 1] - v[i];
    }

    /*
    void Shift(int tab) {
        if (tab > 0)
            for (int i = m_to-tab;  i >= m_from;  i--) m_buf[i+tab] = m_buf[i];
        else if (tab < 0)
            for (int i = m_from-tab;  i <= m_to;  i++) m_buf[i+tab] = m_buf[i];
    }
    */
    void Shift(int offset, BOOLEAN bFlag = FALSE) {
        if(offset < 0 && -offset <= m_size) {
            for(int i = m_from - offset; i <= m_to; i++)
                m_buf[i + offset] = m_buf[i];
            if(bFlag) { // update reference
                m_from -= offset;
                m_size += offset;
                m_buf += offset;
            }
        } else if(offset > 0 && offset <= m_size) {
            for(int i = m_to - offset; i >= m_from; i--)
                m_buf[i + offset] = m_buf[i];
        }
    }
    Array<Type> &InnerProduct(const DoubleArray<Type> &m,
                              const Array<Type> &x) {
        Alloc(m.RowFrom(), m.RowTo());
        _LOOP_ARRAY_(i) m_buf[i] = m[i] & x;
        return (*this);
    }
    Array<Type> &InnerProduct(const Array<Type> &x,
                              const DoubleArray<Type> &m) {
        Alloc(m.ColFrom(), m.ColTo());
        _LOOP_ARRAY_(j) {
            Type s = (Type)0;
            LOOP_ARRAY_INT(x, i) s += x.m_buf[i] * m[i][j];
            m_buf[j] = s;
        }
        return (*this);
    }

    typedef Type *PType;
    operator PType() const { return m_buf; }
    Type *operator()(void) const { return m_buf; }
    template <class INT> Type *operator()(INT i) { return m_buf + i; }
    template <class INT> Type *Offset(INT i) { return m_buf + m_from - i; }
    template <class INT> Type &operator[](INT i) {
        CHECK_RANGE(*this, i);
        return m_buf[i];
    }
    template <class INT> Type &operator[](INT i) const {
        CHECK_RANGE(*this, i);
        return m_buf[i];
    }

    Type operator=(const Type s) {
        _LOOP_ARRAY_(i) m_buf[i] = (Type)s;
        return s;
    }
    Array<Type> &operator=(const Type a[]) { // this = a
        //_LOOP_ARRAY_(i) m_buf[i] = a[i];
        memcpy(m_buf + m_from, a + m_from, Size() * sizeof(Type));
        return (*this);
    }
    Array<Type> &operator=(const Array<Type> &v) { // this = v
        if(!IS_MATCH_ARRAY(*this, v)) Alloc(v.From(), v.To());
        _LOOP_ARRAY_(i) m_buf[i] = v.m_buf[i];
        return (*this);
    }
    BOOLEAN operator==(const Array<Type> &v) const { // this == v
        if(From() != v.From() || To() != v.To()) return FALSE;
        _LOOP_ARRAY_(i)
        if(IsSame(m_buf[i], v.m_buf[i]) == FALSE) return FALSE;
        return TRUE;
    }
    BOOLEAN operator!=(const Array<Type> &v) const { // this == v
        if(From() != v.From() || To() != v.To()) return TRUE;
        _LOOP_ARRAY_(i)
        if(IsSame(m_buf[i], v.m_buf[i]) == TRUE) return FALSE;
        return TRUE;
    }
    Array<Type> &operator+=(const Array<Type> &v) { // this += v
        if(IsEmpty())
            operator=(v);
        else {
            CHECK_MATCH(*this, v);
            _LOOP_ARRAY_(i) m_buf[i] += v[i];
        }
        return (*this);
    }
    Array<Type> &operator+=(const Type a[]) { // this += a
        if(IsEmpty())
            operator=(a);
        else
            _LOOP_ARRAY_(i) m_buf[i] += a[i];
        return (*this);
    }
    Array<Type> &operator+=(Type s) { // this += s
        _LOOP_ARRAY_(i) m_buf[i] += s;
        return (*this);
    }
    Array<Type> &operator-=(const Array<Type> &v) { // this -= v
        if(IsEmpty()) {
            Alloc(v.From(), v.To());
            _LOOP_ARRAY_(i) m_buf[i] = -v.m_buf[i];
        } else {
            CHECK_MATCH(*this, v);
            _LOOP_ARRAY_(i) m_buf[i] -= v[i];
        }
        return (*this);
    }
    Array<Type> &operator-=(const Type a[]) { // this -= a
        _LOOP_ARRAY_(i) m_buf[i] -= a[i];
        return (*this);
    }
    Array<Type> &operator-=(Type s) { // this -= s
        _LOOP_ARRAY_(i) m_buf[i] -= s;
        return (*this);
    }
    Array<Type> &operator*=(Type s) { // this /= s
        _LOOP_ARRAY_(i) m_buf[i] *= s;
        return (*this);
    }
    Array<Type> &operator*=(const Array<Type> &v) { // this *= v
        CHECK_MATCH(*this, v);
        _LOOP_ARRAY_(i) m_buf[i] *= v[i];
        return (*this);
    }
    Array<Type> &operator/=(Type s) { // this /= t
        _LOOP_ARRAY_(i) m_buf[i] /= s;
        return (*this);
    }
    Array<Type> &operator/=(const Array<Type> &v) { // this *= v
        CHECK_MATCH(*this, v);
        _LOOP_ARRAY_(i) m_buf[i] /= v[i];
        return (*this);
    }
    /*
    Array<Type> operator*(const DoubleArray<Type> &m) {	// return V * M
        if (Size() != m.RowSize())
            Error("Unmatching size between vector and matrix.");
        Array<Type> v(m.GetCol());
        LOOP_ARRAY_INT(v, i) {
            v[i] = (Type)0;
            LOOP_ARRAY_INT(*this, j) v[i] += m_buf[j] * m[j][i];
        }
        return v;
    }
    Array<Type> &operator*=(const DoubleArray<Type> &m) {
        if (From() != m.Row
        if (Size() != m.GetRow())
            Error("Unmatching size between vector and matrix.");
        Array<Type> v(m.GetCol());
        LOOP_ARRYA_INT(v, i) {
            v[i] = (Type)0;
            LOOP_ARRAY_INT(*this, ) v[i] += m_buf[j] * m[j][i];
        }
        if (!IS_MATCH_ARRAY(v, *this) Alloc(v.Size());
        if (Size() != v.Size()) Alloc(v.Size());
        _LOOP_ARRAY_(i) m_buf[i] = v[i];
        return (*this);
    }
    */

    void Echo(const char *format = NULL) const {
        if(format) {
            _LOOP_ARRAY_(i) printf(format, m_buf[i] + TolZero);
            printf("\n");
        } else
            _LOOP_ARRAY_(i) printf("%s\n", ::ToString(m_buf[i] + TolZero));
        printf("\n");
    }
    void Echo(FILE *fp, const char *format = NULL) const {
        if(format)
            _LOOP_ARRAY_(i) fprintf(fp, format, m_buf[i] + TolZero);
        else
            _LOOP_ARRAY_(i) fprintf(fp, "%s\n", ::ToString(m_buf[i] + TolZero));
        fprintf(fp, "\n");
    }
    void Echo(const char *filename, const char *format) const {
        FILE *fp = fopen(filename, "wt");
        Echo(fp, format);
        fclose(fp);
    }

#ifdef __AFX_H__
    friend class Array2D<Type>;
    friend class Array3D<Type>;
    friend class DoubleArray<Type>;
    friend class TripleArray<Type>;
#else
    template <class T> friend class Array2D;
    template <class T> friend class Array3D;
    template <class T> friend class DoubleArray;
    template <class T> friend class TripleArray;
#endif
};

template <class Type> // return v1 + v2
Array<Type> operator+(const Array<Type> &v1, const Array<Type> &v2) {
    Array<Type> array(v1);
    return (array += v2);
}
template <class Type> // return v1 + a2
Array<Type> operator+(const Array<Type> &v1, const Type a2[]) {
    Array<Type> array(v1);
    return (array += a2);
}
template <class Type> // return a1 + v2
Array<Type> operator+(const Type a1[], const Array<Type> &v2) {
    Array<Type> array(a1);
    return (array += v2);
}

template <class Type> // return -v
Array<Type> operator-(const Array<Type> &v) {
    Array<Type> array(v);
    array *= -1;
    return array;
}
template <class Type> // return v1 - v2
Array<Type> operator-(const Array<Type> &v1, const Array<Type> &v2) {
    Array<Type> array(v1);
    return (array -= v2);
}
template <class Type>
Array<Type> operator-(const Array<Type> &v1, const Type a2[]) {
    Array<Type> array(v1);
    return (array -= a2);
}
template <class Type> // return a1 - v2
Array<Type> operator-(const Type a1[], const Array<Type> &v2) {
    Array<Type> array(a1);
    return (array -= v2);
}
template <class Type> // return innerproduct between v1 and v2
Type operator&(const Array<Type> &v1, const Array<Type> &v2) {
    CHECK_MATCH(v1, v2);
    Type t = (Type)0;
    LOOP_ARRAY_INT(v1, i) t += v1.m_buf[i] * v2.m_buf[i];
    return t;
}
template <class Type> // return innerproduct between v and a
Type operator&(const Array<Type> &v, const Type a[]) {
    Type t = (Type)0;
    LOOP_ARRAY_INT(v, i) t += v.m_buf[i] * a[i];
    return t;
}
template <class Type> // return innerproduct between a and v
Type operator&(const Type a[], const Array<Type> &v) {
    Type t = (Type)0;
    LOOP_ARRAY_INT(v, i) t += a[i] * v.m_buf[i];
    return t;
}
template <class Type, class Scalar> // return v * s
Array<Type> operator*(const Array<Type> &v, Scalar s) {
    Array<Type> result(v.From(), v.To());
    LOOP_ARRAY_INT(v, i) result[i] = (Type)(v[i] * s);
    return result;
}
template <class Type, class Scalar> // return s * v
Array<Type> operator*(Scalar t, const Array<Type> &v) {
    return operator*(v, t);
}
template <class Type> // return v1[i]*v2[i]
Array<Type> operator*(const Array<Type> v1, const Array<Type> &v2) {
    Array<Type> result(v1);
    result *= v2;
    return result;
}
template <class Type, class Scalar> // return v / s
Array<Type> operator/(const Array<Type> &v, Scalar s) {
    Array<Type> result(v.From(), v.To());
    LOOP_ARRAY_INT(v, i) result[i] = (Type)(v[i] / s);
    return result;
}
template <class Type> // return v1[i]/v2[i]
Array<Type> operator/(const Array<Type> v1, const Array<Type> &v2) {
    CHECK_MATCH(v1, v2);
    Array<Type> result(v1);
    result *= v2;
    return result;
}

typedef Array<BOOLEAN> BVector;
typedef Array<WORD> WVector;
typedef Array<int> IVector;
typedef Array<long> LVector;
typedef Array<float> FVector;
typedef Array<double> DVector;

typedef Array<double> Vector;
typedef Array<int> Index;

/////////////////////////////////////////////////////////////////////////////
// Array2D

template <class Type> class Array2D {
  public:
    Type x, y;

    // Constructor
    Array2D() {}
    Array2D(const Type &t) { Set(t); }
    Array2D(const Type &_x, const Type &_y) { Set(_x, _y); }
    Array2D(const Array2D<Type> &v) { Set(v); }
    Array2D(const Array<Type> &v) { Set(v); }
    Array2D(Type a[]) { Set(a); }

    //  Operator
    void Set(const Type &t) { x = y = t; }
    void Set(const Type &_x, const Type &_y) {
        x = _x;
        y = _y;
    }
    void Set(const Array2D<Type> &v) {
        x = v.x;
        y = v.y;
    }
    void Set(const Array<Type> &v) {
        CHECK_SIZE(v, 2);
        x = v.First(0);
        y = v.First(1);
    }
    void Set(Type a[]) {
        x = a[0];
        y = a[1];
    }
    void SetCyl(Type r, Type a) {
        x = r * cos(a);
        y = r * sin(a);
    }

    void Add(Type a[]) {
        x += a[0];
        y += a[1];
    }
    void Add(Type dx, Type dy) {
        x += dx;
        y += dy;
    }

    double GetNorm2() const { return x * x + y * y; }
    double GetNorm() const { return sqrt(x * x + y * y); }
    Array2D<Type> &Normalize() {
        double n = GetNorm();
        x = (Type)(x / n);
        y = (Type)(y / n);
        return (*this);
    }
    Array2D<Type> GetUnit() const {
        double n = GetNorm();
        return Array2D<Type>((Type)(x / n), (Type)(y / n));
    }
    Type GetMin() const { return ::GetMin(x, y); }
    Type GetMax() const { return ::GetMax(x, y); }

    //
    Type operator=(const Type &t) {
        x = y = t;
        return t;
    }
    Array2D<Type> &operator=(const Array2D<Type> &v) {
        x = v.x;
        y = v.y;
        return (*this);
    }
    Array2D<Type> &operator=(const Array<Type> &v) {
        CHECK_SIZE(v, 2);
        x = v.First(0);
        y = v.First(1);
        return (*this);
    }
    Array2D<Type> &operator=(Type a[]) {
        x = a[0];
        y = a[1];
        return (*this);
    }
    //
    BOOLEAN operator==(const Type &t) const {
        if(x != t) return FALSE;
        if(y != t) return FALSE;
        return TRUE;
    }
    BOOLEAN operator==(const Array2D<Type> &v) const {
        if(x != v.x) return FALSE;
        if(y != v.y) return FALSE;
        return TRUE;
    }
    BOOLEAN operator==(const Array<Type> &v) const {
        CHECK_SIZE(v, 2);
        if(x != v.First(0)) return FALSE;
        if(y != v.First(1)) return FALSE;
        return TRUE;
    }
    BOOLEAN operator==(Type a[]) const {
        if(x != a[0]) return FALSE;
        if(y != a[1]) return FALSE;
        return TRUE;
    }
    BOOLEAN operator!=(const Type &t) const {
        return operator==(t) ? FALSE : TRUE;
    }
    BOOLEAN operator!=(const Array2D<Type> &v) const {
        return operator==(v) ? FALSE : TRUE;
    }
    BOOLEAN operator!=(const Array<Type> &v) const {
        return operator==(v) ? FALSE : TRUE;
    }
    BOOLEAN operator!=(Type a[]) const { return operator==(a) ? FALSE : TRUE; }
    //
    Array<Type> &operator+=(const Type &t) {
        x += t;
        y += t;
        return (*this);
    }
    Array2D<Type> &operator+=(const Array2D<Type> &v) {
        x += v.x;
        y += v.y;
        return (*this);
    }
    Array2D<Type> &operator+=(const Array<Type> &v) {
        CHECK_SIZE(v, 2);
        x += v.First(0);
        y += v.First(1);
        return (*this);
    }
    Array2D<Type> &operator+=(const Type a[]) {
        x += a[0];
        y += a[1];
        return (*this);
    }
    //
    Array<Type> &operator-=(const Type &t) {
        x -= t;
        y -= t;
        return (*this);
    }
    Array2D<Type> &operator-=(const Array2D<Type> &v) {
        x -= v.x;
        y -= v.y;
        return (*this);
    }
    Array2D<Type> &operator-=(const Array<Type> &v) {
        CHECK_SIZE(v, 2);
        x -= v.First(0);
        y -= v.First(1);
        return (*this);
    }
    Array2D<Type> &operator-=(const Type a[]) {
        x -= a[0];
        y -= a[1];
        return (*this);
    }
    //
    Array2D<Type> &operator*=(const Type &t) {
        x *= t;
        y *= t;
        return (*this);
    }
    Array2D<Type> &operator*=(const Array2D<Type> &v) {
        x *= v.x;
        y *= v.y;
        return (*this);
    }
    Array2D<Type> &operator*=(const Array<Type> &v) {
        CHECK_SIZE(v, 2);
        x *= v.First(0);
        y *= v.First(1);
        return (*this);
    }
    Array2D<Type> &operator*=(Type a[]) {
        x *= a[0];
        y *= a[1];
        return (*this);
    }
    //
    Array2D<Type> &operator/=(const Type &t) {
        x /= t;
        y /= t;
        return (*this);
    }
    Array2D<Type> &operator/=(const Array2D<Type> &v) {
        x /= v.x;
        y /= v.y;
        return (*this);
    }
    Array2D<Type> &operator/=(const Array<Type> &v) {
        CHECK_SIZE(v, 2);
        x /= v.First(0);
        y /= v.First(1);
        return (*this);
    }
    Array2D<Type> &operator/=(Type a[]) {
        x *= a[0];
        y *= a[1];
        return (*this);
    }

    void Echo(const char *format = NULL) const {
        if(format) {
            printf(format, x);
            printf("\t");
            printf(format, y);
        } else {
            printf(::ToString(x));
            printf("\t");
            printf(::ToString(y));
        }
        printf("\n");
    }
    void Echo(FILE *fp, const char *format = NULL) const {
        if(format) {
            fprintf(fp, format, x);
            fprintf(fp, "\t");
            fprintf(fp, format, y);
        } else {
            fprintf(fp, ::ToString(x));
            fprintf(fp, "\t");
            fprintf(fp, ::ToString(y));
        }
        fprintf(fp, "\n");
    }
    void Echo(const char *filename, const char *format) const {
        FILE *fp = fopen(filename, "wt");
        Echo(fp, format);
        fclose(fp);
    }
};

template <class Type>
Array2D<Type> operator+(const Array2D<Type> &v1, const Array2D<Type> &v2) {
    return Array2D<Type>(v1.x + v2.x, v1.y + v2.y);
}
template <class Type>
Array2D<Type> operator-(const Array2D<Type> &v1, const Array2D<Type> &v2) {
    return Array2D<Type>(v1.x - v2.x, v1.y - v2.y);
}
template <class Type> Array2D<Type> operator*(const Array2D<Type> &v, Type s) {
    return Array2D<Type>(v.x * s, v.y * s);
}
template <class Type> Array2D<Type> operator*(Type s, const Array2D<Type> &v) {
    return Array2D<Type>(v.x * s, v.y * s);
}
template <class Type> Array2D<Type> operator/(const Array2D<Type> &v, Type s) {
    return Array2D<Type>(v.x / s, v.y / s);
}
template <class Type> // Inner product
Type operator&(const Array2D<Type> &v1, const Array2D<Type> &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

typedef Array2D<int> Vector2DI;
typedef Array2D<int> V2DI;
// typedef Array2D<int>	 Point2DI;
// typedef Array2D<int>	 P2DI;
typedef Array2D<int> Index2D;
typedef Array2D<float> Vector2DR;
typedef Array2D<float> V2DR;
// typedef Array2D<float>	 Point2DR;
// typedef Array2D<float>	 P2DR;
typedef Array2D<double> Vector2DD;
typedef Array2D<double> V2DD;
// typedef Array2D<double>  Point2DD;
// typedef Array2D<double>  P2DD;
typedef Array2D<double> Vector2D;
typedef Array2D<double> V2D;
// typedef Array2D<double>  Point2D;
// typedef Array2D<double>  P2D;

/////////////////////////////////////////////////////////////////////////////
// Array3D

template <class Type> class Array3D {
  public:
    Type x, y, z;

    // Constructor
    Array3D() {}
    Array3D(const Type &t) { Set(t); }
    Array3D(const Type &_x, const Type &_y, const Type &_z) { Set(_x, _y, _z); }
    Array3D(const Array3D<Type> &v) { Set(v); }
    Array3D(const Array<Type> &v) { Set(v); }
    Array3D(Type a[]) { Set(a); }

    //  Operator
    void Set(const Type &t) { x = y = z = t; }
    void Set(const Type &_x, const Type &_y, const Type &_z) {
        x = _x;
        y = _y;
        z = _z;
    }
    void Set(const Array3D<Type> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    void Set(const Array<Type> &v) {
        CHECK_SIZE(v, 3);
        x = v.First(0);
        y = v.First(1);
        z = v.First(2);
    }
    void Set(Type a[]) {
        x = a[0];
        y = a[1];
        z = a[2];
    }
    //
    Type GetNorm2() const { return x * x + y * y + z * z; }
    double GetNorm() const { return sqrt(x * x + y * y + z * z); }
    Array3D<Type> &Normalize() {
        double n = GetNorm();
        x = (Type)(x / n);
        y = (Type)(y / n);
        z = (Type)(z / n);
        return (*this);
    }
    Array3D<Type> GetUnit() const {
        double n = GetNorm();
        return Array3D<Type>((Type)(x / n), (Type)(y / n), (Type)(z / n));
    }
    Type GetMin() const { return ::GetMin(x, y, z); }
    Type GetMax() const { return ::GetMax(x, y, z); }
    // [-pi, pi];
    double GetPhi() const { return atan2((double)y, (double)x); }
    // [0, pi];
    double GetTheta() const { return acos(z / GetNorm()); }

    //
    Type operator=(const Type &t) {
        x = y = z = t;
        return t;
    }
    Array3D<Type> &operator=(const Array3D<Type> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return (*this);
    }
    Array3D<Type> &operator=(const Array<Type> &v) {
        CHECK_SIZE(v, 3);
        x = v.First(0);
        y = v.First(1);
        z = v.First(2);
        return (*this);
    }
    Array3D<Type> &operator=(Type a[]) {
        x = a[0];
        y = a[1];
        z = a[2];
        return (*this);
    }
    //
    BOOLEAN operator==(const Type &t) const {
        if(x != t) return FALSE;
        if(y != t) return FALSE;
        if(z != t) return FALSE;
        return TRUE;
    }
    BOOLEAN operator==(const Array3D<Type> &v) const {
        if(x != v.x) return FALSE;
        if(y != v.y) return FALSE;
        if(z != v.z) return FALSE;
        return TRUE;
    }
    BOOLEAN operator==(const Array<Type> &v) const {
        CHECK_SIZE(v, 3);
        if(x != v.First(0)) return FALSE;
        if(y != v.First(1)) return FALSE;
        if(z != v.First(2)) return FALSE;
        return TRUE;
    }
    BOOLEAN operator==(Type a[]) const {
        if(x != a[0]) return FALSE;
        if(y != a[1]) return FALSE;
        if(z != a[2]) return FALSE;
        return TRUE;
    }
    BOOLEAN operator!=(const Type &t) const {
        return operator==(t) ? FALSE : TRUE;
    }
    BOOLEAN operator!=(const Array3D<Type> &v) const {
        return operator==(v) ? FALSE : TRUE;
    }
    BOOLEAN operator!=(const Array<Type> &v) const {
        return operator==(v) ? FALSE : TRUE;
    }
    BOOLEAN operator!=(Type a[]) const { return operator==(a) ? FALSE : TRUE; }
    //
    Array<Type> &operator+=(const Type &t) {
        x += t;
        y += t;
        z += t;
        return (*this);
    }
    Array3D<Type> &operator+=(const Array3D<Type> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return (*this);
    }
    Array3D<Type> &operator+=(const Array<Type> &v) {
        CHECK_SIZE(v, 3);
        x += v.First(0);
        y += v.First(1);
        z += v.First(2);
        return (*this);
    }
    Array3D<Type> &operator+=(const Type a[]) {
        x += a[0];
        y += a[1];
        z += a[2];
        return (*this);
    }
    //
    Array<Type> &operator-=(const Type &t) {
        x -= t;
        y -= t;
        z -= t;
        return (*this);
    }
    Array3D<Type> &operator-=(const Array3D<Type> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return (*this);
    }
    Array3D<Type> &operator-=(const Array<Type> &v) {
        CHECK_SIZE(v, 3);
        x -= v.First(0);
        y -= v.First(1);
        z -= v.First(2);
        return (*this);
    }
    Array3D<Type> &operator-=(const Type a[]) {
        x -= a[0];
        y -= a[1];
        z -= a[2];
        return (*this);
    }
    //
    Array3D<Type> &operator*=(const Type &t) {
        x *= t;
        y *= t;
        z *= t;
        return (*this);
    }
    Array3D<Type> &operator*=(const Array3D<Type> &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return (*this);
    }
    Array3D<Type> &operator*=(const Array<Type> &v) {
        CHECK_SIZE(v, 3);
        x *= v.First(0);
        y *= v.First(1);
        z *= v.First(2);
        return (*this);
    }
    Array3D<Type> &operator*=(const Type a[]) {
        x *= a[0];
        y *= a[1];
        z *= a[2];
        return (*this);
    }
    //
    Array3D<Type> &operator/=(const Type &t) {
        x /= t;
        y /= t;
        z /= t;
        return (*this);
    }
    Array3D<Type> &operator/=(const Array3D<Type> &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return (*this);
    }
    Array3D<Type> &operator/=(const Array<Type> &v) {
        CHECK_SIZE(v, 3);
        x /= v.First(0);
        y /= v.First(1);
        z /= v.First(2);
        return (*this);
    }
    Array3D<Type> &operator/=(Type a[]) {
        x /= a[0];
        y /= a[1];
        z /= a[2];
        return (*this);
    }
    //
    Array3D<Type> operator^=(const Array3D<Type> &v) {
        Set(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        return (*this);
    }
    Array3D<Type> operator^=(const Array<Type> &v) {
        CHECK_SIZE(v, 3);
        Set(y * v.First(2) - z * v.First(1), z * v.First(0) - x * v.First(2),
            x * v.First(1) - y * v.First(0));
        return (*this);
    }
    Array3D<Type> operator^=(Type a[]) {
        Set(y * a[2] - z * a[1], z * a[0] - x * a[2], x * a[1] - y * a[0]);
        return (*this);
    }

    void Echo(const char *format = NULL) const {
        if(format) {
            printf(format, x);
            printf("\t");
            printf(format, y);
            printf("\t");
            printf(format, z);
        } else {
            printf(::ToString(x));
            printf("\t");
            printf(::ToString(y));
            printf("\t");
            printf(::ToString(z));
        }
        printf("\n");
    }
    void Echo(FILE *fp, const char *format = NULL) const {
        if(format) {
            fprintf(fp, format, x);
            fprintf(fp, "\t");
            fprintf(fp, format, y);
            fprintf(fp, "\t");
            fprintf(fp, format, z);
        } else {
            fprintf(fp, ::ToString(x));
            fprintf(fp, "\t");
            fprintf(fp, ::ToString(y));
            fprintf(fp, "\t");
            fprintf(fp, ::ToString(z));
        }
        fprintf(fp, "\n");
    }
    void Echo(const char *filename, const char *format) const {
        FILE *fp = fopen(filename, "wt");
        Echo(fp, format);
        fclose(fp);
    }
};

template <class Type>
Array3D<Type> operator+(const Array3D<Type> &v1, const Array3D<Type> &v2) {
    return Array3D<Type>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
template <class Type>
Array3D<Type> operator-(const Array3D<Type> &v1, const Array3D<Type> &v2) {
    return Array3D<Type>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
template <class Type> Array3D<Type> operator*(const Array3D<Type> &v, Type s) {
    return Array3D<Type>(v.x * s, v.y * s, v.z * s);
}
template <class Type> Array3D<Type> operator*(Type s, const Array3D<Type> &v) {
    return Array3D<Type>(v.x * s, v.y * s, v.z * s);
}
template <class Type> Array3D<Type> operator/(const Array3D<Type> &v, Type s) {
    return Array3D<Type>(v.x / s, v.y / s, v.z / s);
}
template <class Type> // Inner product
Type operator&(const Array3D<Type> &v1, const Array3D<Type> &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
template <class Type> // Cross product
Array3D<Type> operator^(const Array3D<Type> &v1, const Array3D<Type> &v2) {
    return Array3D<Type>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                         v1.x * v2.y - v1.y * v2.x);
}

typedef Array3D<int> Vector3DI;
typedef Array3D<int> Index3D;
typedef Array3D<int> Point3DI;
typedef Array3D<int> V3DI;
typedef Array3D<float> Vector3DR;
typedef Array3D<float> V3DR;
typedef Array3D<float> Point3DR;
typedef Array3D<double> Vector3DD;
typedef Array3D<double> V3DD;
typedef Array3D<double> Point3DD;
typedef Array3D<double> Vector3D;
typedef Array3D<double> Point3D;
typedef Array3D<double> V3D;
typedef Array3D<double> P3D;

/////////////////////////////////////////////////////////////////////////////
// Global Functions
/*
template <class Type = float>
Array<Type> LineSpace(Type min, Type max, int size)
{
    Array<Type> v;
    v.LineSpace(min, max, size);
    return v;
}
*/

template <class Type>
Type GetDistance(const Array<Type> &v1, const Array<Type> &v2) {
    return (Type)sqrt(GetDistance2(v1, v2));
}
template <class Type> Type GetDistance(const Array<Type> &v, Type a[]) {
    return (Type)sqrt(GetDistance2(v, a));
}
template <class Type> Type GetDistance(Type a[], const Array<Type> &v) {
    return (Type)sqrt(GetDistance2(a, v));
}
template <class Type>
Type GetDistance(const Array2D<Type> &v1, const Array2D<Type> &v2) {
    return (Type)sqrt(GetDistance2(v1, v2));
}
template <class Type>
Type GetDistance(const Array3D<Type> &v1, const Array3D<Type> &v2) {
    return (Type)sqrt(GetDistance2(v1, v2));
}
template <class Type>
Type GetDistance2(const Array<Type> &v1, const Array<Type> &v2) {
    CHECK_MATCH(v1, v2);
    Type dis2 = (Type)0;
    LOOP_ARRAY_INT(v1, i) dis2 += SQR(v1.m_buf[i] - v2.m_buf[i]);
    return dis2;
}
template <class Type> Type GetDistance2(const Array<Type> &v, Type a[]) {
    Type dis2 = (Type)0;
    LOOP_ARRAY_INT(v, i) dis2 += SQR(v.m_buf[i] - a[i]);
    return dis2;
}
template <class Type> Type GetDistance2(Type a[], const Array<Type> &v) {
    Type dis2 = (Type)0;
    LOOP_ARRAY_INT(v, i) dis2 += SQR(v.m_buf[i] - a[i]);
    return dis2;
}
template <class Type>
Type GetDistance2(const Array2D<Type> &v1, const Array2D<Type> &v2) {
    return SQR(v1.x - v2.x) + SQR(v1.y - v2.y);
}
template <class Type>
Type GetDistance2(const Array3D<Type> &v1, const Array3D<Type> &v2) {
    return SQR(v1.x - v2.x) + SQR(v1.y - v2.y) + SQR(v1.z - v2.z);
}

template <class Type>
Type GetAngle(const Array<Type> &v1, const Array<Type> &v2) {
    CHECK_MATCH(v1, v2);
    return (Type)((v1 & v2) / sqrt(v1.GetNorm2() * v2.GetNorm2()));
}

template <class Type>
Type GetAngle(const Array<Type> &v1, const Array<Type> &v2,
              const Array<Type> &c) {
    CHECK_MATCH(c, v1);
    CHECK_MATCH(c, v2);
    Type n12 = (Type)0, n1 = (Type)0, n2 = (Type)0;
    LOOP_ARRAY_INT(c, i) {
        Type d1 = v1[i] - c[i], d2 = v2[i] - c[i];
        n12 += d1 * d2;
        n1 += d1 * d1;
        n2 += d2 * d2;
    }
    return (Type)(n12 / sqrt(n1 * n2));
}

// eqn[0]*x + eqn[1]*y = eqn[2]
template <class Type>
BOOLEAN GetLineEqn(Type x1, Type y1, Type x2, Type y2, Vector3D &eqn) {
    eqn.y = (double)(x1 * y2 - y1 * x2);
    if(eqn.y == 0) {
        if(x1 == x2) return FALSE; // It's same pt
        eqn.x = (double)(y1);
        eqn.y = (double)(-x1);
        eqn.z = 0.;
    } else {
        eqn.x = (double)(y2 - y1);
        eqn.y = (double)(x1 - x2);
    }
    return TRUE;
}

template <class Type>
Type GetLinearFit(const Array<Type> &xa, const Array<Type> &ya, Type x) {
    int i;
    for(i = xa.From() + 1; i < xa.To() && xa[i] < x; i++)
        ;
    return ((xa[i] - x) * ya[i - 1] + (x - xa[i - 1]) * ya[i])
           / (xa[i] - xa[i - 1]);
}

template <class Type>
void GetLinearFit(Type *xa, Type *ya, int n, Vector3D &v) {
    Type X = (Type)0;
    Type Y = (Type)0;
    Type X2 = (Type)0;
    Type Y2 = (Type)0;
    Type XY = (Type)0;
    for(int i = 0; i < n; i++) {
        X += xa[i];
        Y += ya[i];
        X2 += xa[i] * xa[i];
        Y2 += ya[i] * ya[i];
        XY += xa[i] * ya[i];
    }
    v.x = n * XY - X * Y;
    v.y = X * X - X2 * n;
    v.z = X2 * Y - X * XY;
    /*
    if (X2 == X*X) {
        v.x = 1.;
        v.y = 0.;
        v.z = -X;
    } else if (Y2 == Y*Y) {
        v.x = 0.;
        v.y = 1.;
        v.z = -Y;
    } else {
        v.x = Y2*X-XY*Y;
        v.y = X2*Y-XY*X;
        v.z = -(X*v.x+Y*v.y)/n;
    }
    */
}

template <class Type>
void GetLinearFit(Array<Type> &xa, Array<Type> &ya, Vector3D &v) {
    GetLinearFit(xa(), ya(), xa.Size(), v);
}

#endif
