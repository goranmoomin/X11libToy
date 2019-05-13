// Toy/tensor.h
//
// LastModified : 2002/10/20
//

#ifndef _TOY_TENSOR_H_
#define _TOY_TENSOR_H_

#ifdef TOY
#include <Toy/matrix.h>
#else
#include "matrix.h"
#endif

#define LOOP_DEP(t, k) for((k) = (t).DepFrom(); (k) <= (t).DepTo(); (k)++)
#define LOOP_DEP_INT(t, k) for(int k = (t).DepFrom(); k <= (t).DepTo(); k++)
#define LOOP_TENSOR(t, i, j, k) LOOP_ROW(t, i) LOOP_COL(t, j) LOOP_DEP(t, k)
#define LOOP_TENSOR_INT(t, i, j, k)                                            \
    LOOP_ROW_INT(t, i) LOOP_COL_INT(t, j) LOOP_DEP_INT(t, k)

//////////////////////////////////////////////////////////////////////////////
//

#define IS_TENSOR_MATCH(t1, t2)                                                \
    ((t1).ColFrom() == (t2).ColFrom() && (t1).ColTo() == (t2).ColTo()          \
     && (t1).RowFrom() == (t2).RowFrom() && (t1).RowTo() == (t2).RowTo()       \
     && (t1).DepFrom() == (t2).DepFrom() && (t1).DepTo() == (t2).DepTo())
#ifdef _DEBUG
#define CHECK_TENSOR_MATCH(t1, t2)                                             \
    if(!IS_TENSOR_MATCH(t1, t2)) Error("Size of tensor are different.");
#else
#define CHECK_TENSOR_MATCH(t1, t2)
#endif

#define _LOOP_DEP_(i) for(int i = m_depFrom; i <= m_depTo; i++)
#define _LOOP_TENSOR_(i, j, k) _LOOP_ROW_(i) _LOOP_COL_(j) _LOOP_DEP_(k)

template <class Type> class TripleArray {
    int m_rowFrom, m_rowTo;
    int m_colFrom, m_colTo;
    int m_depFrom, m_depTo;
    Type ***m_buf;
    DoubleArray<Type> *m_matrix;
    BOOLEAN m_bNeedFree;

  public:
    // Constructor and Destructor
    TripleArray(int rowFrom, int rowTo, int colFrom, int colTo, int depFrom,
                int depTo) {
        m_buf = NULL;
        Alloc(rowFrom, rowTo, colFrom, colTo, depFrom, depTo, FALSE);
    }
    TripleArray(int rowSize = 0, int colSize = 0, int depSize = 0) {
        m_buf = NULL;
        Alloc(rowSize, colSize, depSize, FALSE);
    }
    TripleArray(const TripleArray<Type> &t) {
        m_buf = NULL;
        Alloc(t.RowFrom(), t.RowTo(), t.ColFrom(), t.ColTo(), t.DepFrom(),
              t.DepTo(), FALSE);
    }
    TripleArray(const char *fn, const char *mode = "rt", int offset = 0) {
        m_buf = NULL;
        m_matrix = NULL;
        Load(fn, mode, offset);
    }
    ~TripleArray() { Free(); }
    // Operator
    void Init() {
        m_rowFrom = 0;
        m_rowTo = -1;
        m_colFrom = 0;
        m_colTo = -1;
        m_depFrom = 0;
        m_depTo = -1;
        m_buf = NULL;
        m_matrix = NULL;
        m_bNeedFree = FALSE;
    }
    Type ***Alloc(const TripleArray &t) {
        return Alloc(t.RowFrom(), t.RowTo(), t.ColFrom(), t.ColTo(),
                     t.DepFrom(), t.DepTo());
    }
    Type ***Alloc(int rowSize, int colSize, int depSize,
                  BOOLEAN bNeedFree = TRUE) {
        return Alloc(0, rowSize - 1, 0, colSize - 1, 0, depSize - 1, bNeedFree);
    }
    Type ***Alloc(int rowFrom, int rowTo, int colFrom, int colTo, int depFrom,
                  int depTo, BOOLEAN bNeedFree = TRUE) {
        if(bNeedFree) Free();
        m_rowFrom = rowFrom;
        m_rowTo = rowTo;
        m_colFrom = colFrom;
        m_colTo = colTo;
        m_depFrom = depFrom;
        m_depTo = depTo;
        if(rowTo >= rowFrom && colTo >= colTo && depTo >= depFrom) {
            int i, j;
            m_buf = new Type **[RowSize()] - rowFrom;
            m_buf[rowFrom] = new Type *[RowSize() * ColSize()] - colFrom;
            m_buf[rowFrom][colFrom] = new Type[Size()] - depFrom;
            for(j = colFrom + 1; j <= colTo; j++)
                m_buf[rowFrom][j] = m_buf[rowFrom][j - 1] + DepSize();
            for(i = rowFrom + 1; i <= rowTo; i++) {
                m_buf[i] = m_buf[i - 1] + ColSize();
                m_buf[i][colFrom] =
                    m_buf[i - 1][colFrom] + ColSize() * DepSize();
                for(j = colFrom + 1; j <= colTo; j++)
                    m_buf[i][j] = m_buf[i][j - 1] + DepSize();
            }
            m_matrix = new DoubleArray<Type>[RowSize()] - m_rowFrom;
            for(i = rowFrom; i <= rowTo; i++) {
                m_matrix[i].m_rowFrom = m_colFrom;
                m_matrix[i].m_rowTo = m_colTo;
                m_matrix[i].m_colFrom = m_depFrom;
                m_matrix[i].m_colTo = m_depTo;
                m_matrix[i].m_buf = m_buf[i];
                m_matrix[i].m_vector = new Array<Type>[ColSize()] - m_colFrom;
                m_matrix[i].m_bNeedFree = FALSE;
                for(j = colFrom; j <= colTo; j++) {
                    m_matrix[i].m_vector[j].m_from = m_depFrom;
                    m_matrix[i].m_vector[j].m_to = m_depTo;
                    m_matrix[i].m_vector[j].m_size = DepSize();
                    m_matrix[i].m_vector[j].m_buf = m_buf[i][j];
                    m_matrix[i].m_vector[j].m_bufSize = 0;
                }
            }
            m_bNeedFree = TRUE;
        } else {
            m_buf = NULL;
            m_matrix = NULL;
            m_bNeedFree = FALSE;
        }
        return m_buf;
    }
    Type ***Alloc(const Type ***tensor, int rowSize, int colSize, int depSize,
                  BOOLEAN bNeedFree = TRUE) {
        Alloc(rowSize, colSize, depSize, bNeedFree);
        *this = tensor;
        return m_buf;
    }
    void Free() {
        if(m_buf && m_bNeedFree) {
            delete[](m_buf[m_colFrom][m_depFrom] + m_depFrom);
            delete[](m_buf[m_colFrom] + m_colFrom);
            delete[](m_buf + m_rowFrom);
            m_buf = NULL;
        }
        if(m_matrix && m_bNeedFree) {
            _LOOP_ROW_(i) delete[](m_matrix[i].m_vector + m_colFrom);
            delete[](m_matrix + m_rowFrom);
            m_matrix = NULL;
        }
        m_rowFrom = 0;
        m_rowTo = -1;
        m_colFrom = 0;
        m_colTo = -1;
        m_depFrom = 0;
        m_depTo = -1;
    }

    int RowFrom() const { return m_rowFrom; }
    int RowTo() const { return m_rowTo; }
    int ColFrom() const { return m_colFrom; }
    int ColTo() const { return m_colTo; }
    int DepFrom() const { return m_depFrom; }
    int DepTo() const { return m_depTo; }
    int RowSize() const { return m_rowTo - m_rowFrom + 1; }
    int ColSize() const { return m_colTo - m_colFrom + 1; }
    int DepSize() const { return m_depTo - m_depFrom + 1; }
    int Size() const { return ColSize() * RowSize() * DepSize(); }

    BOOLEAN IsEmpty() const { return (Size == 0); }
    Type GetMin() const {
        Type min = m_buf[m_rowFrom][m_colFrom][m_depFrom];
        _LOOP_TENSOR_(i, j, k) if(min > m_buf[i][j][k]) min = m_buf[i][j][k];
        return min;
    }
    Type GetMax() const {
        Type max = m_buf[m_rowFrom][m_colFrom][m_depFrom];
        _LOOP_TENSOR_(i, j, k) if(max < m_buf[i][j][k]) max = m_buf[i][j][k];
        return max;
    }
    Type GetSum() const {
        Type sum = (Type)0;
        _LOOP_TENSOR_(i, j, k) sum += m_buf[i][j][k];
        return sum;
    }
    Type GetMean() const {
        return Size() == 0 ? (Type)0 : (Type)(GetSum() / Size());
    }

    BOOLEAN IsOnBoundary(int dep, int row, int col) {
        if(row == m_rowFrom || row == m_rowTo) return TRUE;
        if(col == m_colFrom || col == m_colTo) return TRUE;
        if(dep == m_depFrom || dep == m_depTo) return TRUE;
        return FALSE;
    }
    BOOLEAN IsInDomain(int dep, int row, int col) {
        if(row < m_rowFrom || row > m_rowTo) return FALSE;
        if(col < m_colFrom || col > m_colTo) return FALSE;
        if(dep < m_depFrom || dep > m_depTo) return FALSE;
        return TRUE;
    }

    BOOLEAN Load(const char *fn, const char *mode, int offset = 0) {
        if(!strcmp(mode, "bmp")) return LoadBitmap(fn, offset);
        return FALSE;
    }
    BOOLEAN LoadBitmap(const char *fn, int offset = 0) {
        FILE *fp = fopen(fn, "rb");
        if(!fp) return FALSE;
        BOOLEAN result = ReadBitmap(fp, offset);
        fclose(fp);
        return result;
    }
    BOOLEAN ReadBitmap(FILE *fp, int offset = 0) {
        if(!fp) return FALSE;
        BITMAPFILEHEADER bmpFileHeader;
        BITMAPINFOHEADER bmpInfoHeader;
        // if (fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp) != 1)
        //    return Warning("Cannot read the file header of bitmap file.");
        if(fread(&bmpFileHeader.bfType, sizeof(WORD), 1, fp) != 1)
            return Warning("Cannot read the file header of bitmap file.");
        if(fread(&bmpFileHeader.bfSize, sizeof(DWORD), 1, fp) != 1)
            return Warning("Cannot read the file header of bitmap file.");
        if(fread(&bmpFileHeader.bfReserved1, sizeof(WORD), 1, fp) != 1)
            return Warning("Cannot read the file header of bitmap file.");
        if(fread(&bmpFileHeader.bfReserved2, sizeof(WORD), 1, fp) != 1)
            return Warning("Cannot read the file header of bitmap file.");
        if(fread(&bmpFileHeader.bfOffBits, sizeof(DWORD), 1, fp) != 1)
            return Warning("Cannot read the file header of bitmap file.");
        if(fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp) != 1)
            return Warning("Cannot read the info header of bitmap file.");

        fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET);

        Alloc(offset, offset, offset, offset + bmpInfoHeader.biHeight - 1,
              offset, offset + bmpInfoHeader.biWidth - 1);

        int lineSize = (bmpInfoHeader.biWidth + 3) / 4 * 4;
        int size = bmpInfoHeader.biHeight * lineSize;
        BYTE *line = new BYTE[size];
        fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET);
        if(fread(line, sizeof(BYTE), size, fp) != (size_t)size) {
            delete line;
            return Warning("Cannot read the full image of bitmap file");
        }
        _LOOP_COL_(i)
        _LOOP_DEP_(j) m_buf[offset][ColTo() - i + offset][j] =
            (Type)line[(i - offset) * lineSize + j - offset];
        delete line;
    }

    typedef Type ***PPPType;
    operator PPPType() const { return m_buf; }
    Type ***operator()(void) const { return m_buf; }
    template <class INT> DoubleArray<Type> &operator[](INT i) {
        CHECK_ROW_RANGE(*this, i);
        return m_matrix[i];
    }
    template <class INT> DoubleArray<Type> &operator[](INT i) const {
        CHECK_ROW_RANGE(*this, i);
        return m_matrix[i];
    }
    Type operator=(const Type s) {
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] = s;
        return s;
    }
    TripleArray<Type> &operator=(Type ***t) {
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] = t[i][j][k];
        return (*this);
    }
    TripleArray<Type> &operator=(const TripleArray<Type> &t) {
        if(!IS_TENSOR_MATCH(*this, t))
            Alloc(t.RowFrom(), t.RowTo(), t.ColFrom(), t.ColTo(), t.DepFrom(),
                  t.DepTo());
        return operator=(t.m_buf);
    }
    TripleArray<Type> &operator+=(const TripleArray<Type> &m) {
        CHECK_TENSOR_MATCH(*this, m);
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] += m.m_buf[i][j][k];
        return (*this);
    }
    TripleArray<Type> &operator+=(const Type **m) {
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] += m[i][j][k];
        return (*this);
    }
    TripleArray<Type> &operator-=(const TripleArray<Type> &m) {
        CHECK_TENSOR_MATCH(*this, m);
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] -= m.m_buf[i][j][k];
        return (*this);
    }
    TripleArray<Type> &operator-=(Type **m) {
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] -= m[i][j][k];
        return (*this);
    }
    TripleArray<Type> &operator-=(const Type &t) {
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] -= t;
        return (*this);
    }
    TripleArray<Type> &operator*=(const Type &t) {
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] *= t;
        return (*this);
    }
    TripleArray<Type> &operator/=(const Type &t) {
        _LOOP_TENSOR_(i, j, k) m_buf[i][j][k] /= t;
        return (*this);
    }
};

typedef TripleArray<BOOLEAN> BTensor;
typedef TripleArray<int> ITensor;
typedef TripleArray<long> LTensor;
typedef TripleArray<float> FTensor;
typedef TripleArray<double> DTensor;

typedef TripleArray<double> Tensor;

#endif
