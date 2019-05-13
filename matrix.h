// Toy/matrix.h
//
// Last Modified : 2006/11/29
//

#ifndef _TOY_MATRIX_H_
#define _TOY_MATRIX_H_

#ifdef TOY
#include <Toy/vector.h>
#else
#include "vector.h"
#endif

#define LOOP_ROW(m, i) for((i) = (m).m_rowFrom; (i) <= (m).m_rowTo; (i)++)
#define LOOP_ROW_INT(m, i) for(int i = (m).m_rowFrom; i <= (m).m_rowTo; i++)
#define LOOP_COL(m, j) for((j) = (m).m_colFrom; (j) <= (m).m_colTo; (j)++)
#define LOOP_COL_INT(m, j) for(int j = (m).m_colFrom; j <= (m).m_colTo; j++)
#define LOOP_MATRIX(m, i, j) LOOP_ROW(m, i) LOOP_COL(m, j)
#define LOOP_MATRIX_INT(m, i, j) LOOP_ROW_INT(m, i) LOOP_COL_INT(m, j)
#define LOOP_UPPER_MATRIX(m, i, j)                                             \
    LOOP_ROW(m, i) for((j) = (i) + 1; (j) <= (m).m_colTo; (j)++)
#define LOOP_UPPER_MATRIX_INT(m, i, j)                                         \
    LOOP_ROW_INT(m, i) for(int j = (i) + 1; (j) <= (m).ColTo(); (j)++)
#define LOOP_LOWER_MATRIX(m, i, j)                                             \
    LOOP_ROW(m, i) for((j) = (m).ColFrom(); (j) < (i); (j)++)
#define LOOP_LOWER_MATRIX_INT(m, i, j)                                         \
    LOOP_ROW_INT(m, i) for(int j = (m).m_colFrom; (j) < (i); (j)++)

template <class Type> class TripleArray;
class Image;

//////////////////////////////////////////////////////////////////////////////
//

#define IS_MATRIX_MATCH(m1, m2)                                                \
    ((m1).ColFrom() == (m2).ColFrom() && (m1).ColTo() == (m2).ColTo()          \
     && (m1).RowFrom() == (m2).RowFrom() && (m1).RowTo() == (m2).RowTo())

#ifdef _DEBUG
#define CHECK_MATRIX_MATCH(m1, m2)                                             \
    if(!IS_MATRIX_MATCH(m1, m2)) Error("Size of matrix are different.");
#define CHECK_ROW_RANGE(m, i)                                                  \
    if(i < (m).RowFrom() || i > (m).RowTo())                                   \
        Error(                                                                 \
            "Out of range in class DoubleArray : ColFrom() = %d, "             \
            "ColTo() = %d, Ref = %d\n",                                        \
            RowFrom(), RowTo(), i);
#else
#define CHECK_MATRIX_MATCH(m1, m2)
#define CHECK_ROW_RANGE(m, i)
#endif

#define _LOOP_ROW_(i) for(int i = m_rowFrom; i <= m_rowTo; i++)
#define _LOOP_COL_(i) for(int i = m_colFrom; i <= m_colTo; i++)
#define _LOOP_MATRIX_(i, j) _LOOP_ROW_(i) _LOOP_COL_(j)

#ifndef __AFX_H__

typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

#endif

template <class Type> class DoubleArray {
  public:
    int m_rowFrom, m_rowTo;
    int m_colFrom, m_colTo;
    Type **m_buf;
    Array<Type> *m_vector;

  private:
    BOOLEAN m_bNeedFree;

  public:
    // Constructor and Destructor
    DoubleArray(int rowFrom, int rowTo, int colFrom, int colTo) {
        Init();
        Alloc(rowFrom, rowTo, colFrom, colTo, FALSE);
    }
    DoubleArray(int rowSize = 0, int colSize = 0) {
        Init();
        Alloc(rowSize, colSize, FALSE);
    }
    DoubleArray(const DoubleArray<Type> &m) {
        Init();
        Alloc(m.RowFrom(), m.RowTo(), m.ColFrom(), m.ColTo(), FALSE);
        *this = m;
    }
    DoubleArray(const DoubleArray<Type> &m1, const DoubleArray<Type> &m2) {
        Init();
        InnerProduct(m1, m2);
    }
    DoubleArray(const Type **matrix, int rowSize, int colSize) {
        Init();
        Alloc(matrix, rowSize, colSize, FALSE);
    }
    DoubleArray(const char *fn, const char *mode = "rt",
                BOOLEAN bTranspose = FALSE, int offset = 0) {
        Init();
        Load(fn, mode, bTranspose, offset);
    }
    ~DoubleArray() { Free(); }
    // Operator
    void Init() {
        m_rowFrom = 0;
        m_rowTo = -1;
        m_colFrom = 0;
        m_colTo = -1;
        m_buf = NULL;
        m_vector = NULL;
        m_bNeedFree = FALSE;
    }
    template <class type1> Type **Alloc(const DoubleArray<type1> &m) {
        return Alloc(m.RowFrom(), m.RowTo(), m.ColFrom(), m.ColTo());
    }
    Type **Alloc(int rowSize, int colSize, BOOLEAN bNeedFree = TRUE) {
        return Alloc(0, rowSize - 1, 0, colSize - 1, bNeedFree);
    }
    Type **Alloc(int rowFrom, int rowTo, int colFrom, int colTo,
                 BOOLEAN bNeedFree = TRUE) {
        if(m_rowFrom == rowFrom && m_rowTo == rowTo && m_colFrom == colFrom
           && m_colTo == colTo)
            return m_buf;
        if(bNeedFree) Free();
        m_rowFrom = rowFrom;
        m_rowTo = rowTo;
        m_colFrom = colFrom;
        m_colTo = colTo;
        if(rowTo >= rowFrom && colTo >= colFrom) {
            int i;
            m_buf = new Type *[RowSize()] - rowFrom;
            m_buf[rowFrom] = new Type[Size()] - colFrom;
            for(i = rowFrom + 1; i <= rowTo; i++)
                m_buf[i] = m_buf[i - 1] + ColSize();
            m_vector = new Array<Type>[RowSize()] - m_rowFrom;
            for(i = rowFrom; i <= rowTo; i++) {
                m_vector[i].m_from = m_colFrom;
                m_vector[i].m_to = m_colTo;
                m_vector[i].m_size = ColSize();
                m_vector[i].m_buf = m_buf[i];
                m_vector[i].m_bufSize = 0;
            }
            m_bNeedFree = TRUE;
        } else {
            m_buf = NULL;
            m_vector = NULL;
            m_bNeedFree = FALSE;
        }
        return m_buf;
    }
    void Alloc(const Type **matrix, int rowSize, int colSize,
               BOOLEAN bNeedFree = TRUE) {
        Alloc(rowSize, colSize, bNeedFree);
        *this = matrix;
    }
    void Free() {
        if(m_buf && m_bNeedFree) {
            delete[](m_buf[m_rowFrom] + m_colFrom);
            delete[](m_buf + m_rowFrom);
        }
        if(m_vector && m_bNeedFree) { delete[](m_vector + m_rowFrom); }
        Init();
    }

    int RowFrom() const { return m_rowFrom; }
    int RowTo() const { return m_rowTo; }
    int ColFrom() const { return m_colFrom; }
    int ColTo() const { return m_colTo; }
    int RowSize() const { return m_rowTo - m_rowFrom + 1; }
    int ColSize() const { return m_colTo - m_colFrom + 1; }
    int Size() const { return ColSize() * RowSize(); }

    void Transpose() {
        Type **matrix = m_buf;
        delete[](m_vector + m_rowFrom);
        Alloc(m_colFrom, m_colTo, m_rowFrom, m_rowTo, FALSE);
        _LOOP_MATRIX_(i, j) m_buf[i][j] = matrix[j][i];
        delete[](matrix[m_rowFrom] + m_colFrom);
        delete[](matrix + m_rowFrom);
    }

    BOOLEAN IsEmpty() const {
        return RowSize() > 0 && ColSize() > 0 ? FALSE : TRUE;
    }
    Type GetMin() const {
        Type min = m_buf[m_rowFrom][m_colFrom];
        _LOOP_MATRIX_(i, j) if(min > m_buf[i][j]) min = m_buf[i][j];
        return min;
    }
    Type GetMin(int col) const {
        Type min = m_buf[m_rowFrom][col];
        for(int i = m_rowFrom + 1; i <= m_rowTo; i++)
            if(min > m_buf[i][col]) min = m_buf[i][col];
        return min;
    }
    void GetMaxPos(int *row, int *col) const {
        *row = m_rowFrom;
        *col = m_colFrom;
        _LOOP_MATRIX_(i, j)
        if(m_buf[*row][*col] < m_buf[i][j]) { *row = i, *col = j; }
    }
    void GetMinPos(int *row, int *col) const {
        *row = m_rowFrom;
        *col = m_colFrom;
        _LOOP_MATRIX_(i, j)
        if(m_buf[*row][*col] > m_buf[i][j]) { *row = i, *col = j; }
    }
    int GetMinPos(int col) const {
        int pos = m_rowFrom;
        for(int i = m_rowFrom + 1; i <= m_rowTo; i++)
            if(m_buf[pos][col] > m_buf[i][col]) pos = i;
        return pos;
    }
    Type GetMax() const {
        Type max = m_buf[m_rowFrom][m_colFrom];
        _LOOP_MATRIX_(i, j) if(max < m_buf[i][j]) max = m_buf[i][j];
        return max;
    }
    Type GetMax(int col) const {
        Type max = m_buf[m_rowFrom][col];
        for(int i = m_rowFrom + 1; i <= m_rowTo; i++)
            if(max < m_buf[i][col]) max = m_buf[i][col];
        return max;
    }
    int GetMaxPos(int col) const {
        int pos = m_rowFrom;
        for(int i = m_rowFrom + 1; i <= m_rowTo; i++)
            if(m_buf[pos][col] < m_buf[i][col]) pos = i;
        return pos;
    }
    Type GetSum() const {
        Type sum = (Type)0;
        _LOOP_MATRIX_(i, j) sum += m_buf[i][j];
        return sum;
    }
    Type GetSum(int col) const {
        Type sum = (Type)0;
        _LOOP_ROW_(i) sum += m_buf[i][col];
        return sum;
    }
    double GetMean() const {
        return Size() == 0 ? (Type)0 : (double)GetSum() / Size();
    }
    Type GetMean(int col) const { return (Type)(GetSum(col) / RowSize()); }
    Type GetMean(int rowFrom, int colFrom, int rowTo, int colTo) {
        if(rowFrom > rowTo) ::Swap(rowFrom, rowTo);
        if(rowFrom < m_rowFrom) rowFrom = m_rowFrom;
        if(rowTo > m_rowTo) rowTo = m_rowTo;
        if(colFrom > colTo) ::Swap(colFrom, colTo);
        if(colFrom < m_colFrom) colFrom = m_colFrom;
        if(colTo > m_colTo) colTo = m_colTo;
        if(rowFrom > rowTo || colFrom > colTo) return (Type)0;
        Type sum = (Type)0;
        _LOOP_MATRIX_(i, j) sum += m_buf[i][j];
        return (Type)(sum / ((rowTo - rowFrom + 1) * (colTo - colFrom + 1)));
    }
    double GetVar() const {
        Type mean = GetMean();
        double var = 0.;
        _LOOP_MATRIX_(i, j) var += SQR(mean - m_buf[i][j]);
        return var / Size();
    }
    double GetVar(int col) const {
        Type mean = GetMean(col);
        double var = 0.;
        _LOOP_ROW_(i) var += SQR(mean - m_buf[i][col]);
        return var / RowSize();
    }
    Type GetStd() const { return (Type)sqrt(GetVar()); }
    Type GetStd(int col) const { return (Type)sqrt(GetVar(col)); }
    Type GetRowCorr(int i, int j) const {
        Type sum = (Type)0;
        _LOOP_COL_(k) sum += m_buf[i][k] * m_buf[j][k];
        return sum;
    }
    Type GetColCorr(int i, int j) const {
        Type sum = (Type)0;
        _LOOP_ROW_(k) sum += m_buf[k][i] * m_buf[k][j];
        return sum;
    }

    BOOLEAN IsOnBoundary(int row, int col) const {
        if(row == m_rowFrom || row == m_rowTo) return TRUE;
        if(col == m_colFrom || col == m_colTo) return TRUE;
        return FALSE;
    }
    BOOLEAN IsInDomain(int row, int col) const {
        if(row < m_rowFrom || row > m_rowTo) return FALSE;
        if(col < m_colFrom || col > m_colTo) return FALSE;
        return TRUE;
    }

    BOOLEAN Load(const char *fn, const char *mode, BOOLEAN bTranspose,
                 int offset = 0) {
        if(!strcmp(mode, "bmp"))
            return LoadBitmap(fn, offset);
        else if(strchr(mode, 't'))
            return LoadText(fn, bTranspose, offset);
        else if(strchr(mode, 'b'))
            return LoadBinary(fn);
        return FALSE;
    }
    BOOLEAN LoadText(const char *fn, BOOLEAN bTranspose = FALSE,
                     int offset = 0) {
        Free();
        // get the row and col size
        FILE *fp = fopen(fn, "rt");
        if(!fp) return FALSE;
        Type s;
        char line[10240];
        int nRow = 0, nCol = 0;
        while(fgets(line, 10240, fp))
            if(GetValue(line, s)) {
                nCol = GetValids<Type>(line);
                if(nCol > 0) break;
            }
        nRow++;
        while(fgets(line, 10240, fp))
            if(::GetValue(line, s)) nRow++;
        fclose(fp);
        // alloc memory
        if(bTranspose)
            Alloc(offset, offset + nCol - 1, offset, offset + nRow - 1);
        else
            Alloc(offset, offset + nRow - 1, offset, offset + nCol - 1);
        // read data
        fp = fopen(fn, "rt");
        if(!fp) return FALSE;
        if(bTranspose) {
            Type *buf = new Type[ColSize()];
            for(int row = 0; row < nRow && fgets(line, 10240, fp);)
                if(::GetValues(line, buf, nCol) > 0) {
                    for(int col = 0; col < nCol; col++)
                        m_buf[offset + col][offset + row] = buf[col];
                    row++;
                }
            delete buf;
        } else {
            for(int row = 0; row < nRow && fgets(line, 10240, fp);)
                if(::GetValues(line, m_buf[offset + row] + offset, nCol) > 0)
                    row++;
        }
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
        // read matrix size
        int rowFrom, rowTo, colFrom, colTo;
        if(fread(&rowFrom, sizeof(rowFrom), 1, fp) != 1) return FALSE;
        if(fread(&rowTo, sizeof(rowTo), 1, fp) != 1) return FALSE;
        if(fread(&colFrom, sizeof(colFrom), 1, fp) != 1) return FALSE;
        if(fread(&colTo, sizeof(colTo), 1, fp) != 1) return FALSE;
        // alloc memory
        Alloc(rowFrom, rowTo, colFrom, colTo);
        // load data
        _LOOP_ROW_(i)
        if(fread(m_buf[i] + ColFrom(), sizeof(Type), (size_t)ColSize(), fp)
           != (size_t)ColSize()) {
            Free();
            return FALSE;
        }
        return TRUE;
    }
    BOOLEAN LoadBitmap(const char *fn, int offset = 0) {
        Free();
        FILE *fp = fopen(fn, "rb");
        if(!fp) return FALSE;
        BOOLEAN result = ReadBitmap(fp, offset);
        fclose(fp);
        return result;
    }
    BOOLEAN ReadBitmap(FILE *fp, int offset = 0) {
        Free();
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

        // fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET);

        Alloc(offset, offset + bmpInfoHeader.biHeight - 1, offset,
              offset + bmpInfoHeader.biWidth - 1);

        int lineSize = (bmpInfoHeader.biWidth + 3) / 4 * 4;
        int size = bmpInfoHeader.biHeight * lineSize;
        BYTE *line = new BYTE[size];
        fseek(fp, bmpFileHeader.bfOffBits, SEEK_SET);
        if(fread(line, sizeof(BYTE), size, fp) != (size_t)size) {
            delete line;
            return Warning("Cannot read the full image of bitmap file");
        }
        _LOOP_MATRIX_(i, j)
        m_buf[RowTo() - i][j] =
            (Type)line[(i - offset) * lineSize + j - offset];
        delete line;
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
    BOOLEAN WriteText(FILE *fp, const char *format = NULL) const {
        if(!fp) return FALSE;
        if(format)
            _LOOP_MATRIX_(i, j) fprintf(fp, format, m_buf[i][j]);
        else
            _LOOP_MATRIX_(i, j)
            fprintf(fp, "%s%c", ::ToStr(m_buf[i][j]),
                    j == m_colTo ? '\n' : '\t');
        return TRUE;
    }
    BOOLEAN SaveBinary(const char *fn) const {
        FILE *fp = fopen(fn, "wb");
        if(!fp) return FALSE;
        BOOLEAN result = WriteBinary(fp);
        fclose(fp);
        return result;
    }
    BOOLEAN WriteBinary(FILE *fp) const {
        if(!fp) return FALSE;
        // write matrix size
        if(fwrite(&m_rowFrom, sizeof(int), 1, fp) != 1) return FALSE;
        if(fwrite(&m_rowTo, sizeof(int), 1, fp) != 1) return FALSE;
        if(fwrite(&m_colFrom, sizeof(int), 1, fp) != 1) return FALSE;
        if(fwrite(&m_colTo, sizeof(int), 1, fp) != 1) return FALSE;
        // Save data
        _LOOP_ROW_(i)
        if(fwrite(m_buf[i] + m_colFrom, sizeof(Type), (size_t)ColSize(), fp)
           != (size_t)ColSize())
            return FALSE;
        return TRUE;
    }
    BOOLEAN Save3D(const char *fn, const char *format = NULL) const {
        FILE *fp = fopen(fn, "wt");
        if(!fp) return FALSE;
        BOOLEAN result = FALSE;
        if(format) _LOOP_ROW_(i) {
                _LOOP_COL_(j) {
                    fprintf(fp, "%d  %d  ", i, j);
                    fprintf(fp, format, m_buf[i][j]);
                    fprintf(fp, "\n");
                }
                fprintf(fp, "\n");
            }
        else
            _LOOP_ROW_(i) {
                _LOOP_COL_(j) fprintf(fp, "%d  %d  %f\n", i, j, m_buf[i][j]);
                fprintf(fp, "\n");
            }
        fclose(fp);
        return result;
    }

    typedef Type **PPType;
    operator PPType() const { return m_buf; }
    Type **operator()(void) const { return m_buf; }
    template <class INT> Type **operator()(INT i) { return m_buf + i; }
    template <class INT> Type **operator()(INT i, INT j) {
        return m_buf + ColSize() * i + j;
    }
    template <class INT> void Shift(INT i, INT j, BOOLEAN bFlag) {
        // return m_buf+ColSize()*(m_rowFrom-i)+m_colFrom-j;
        if(bFlag) {
            LOOP_ROW_INT(*this, k) m_buf[k] += m_colFrom - j;
            m_buf += m_rowFrom - i;
        } else {
            m_buf -= m_rowFrom - i;
            LOOP_ROW_INT(*this, k) m_buf[k] -= m_colFrom - j;
        }
    }
    template <class INT> void Offset(INT i, INT j, BOOLEAN bFlag) {
        // return m_buf+ColSize()*(m_rowFrom-i)+m_colFrom-j;
        if(bFlag) {
            LOOP_ROW_INT(*this, k) m_buf[k] += m_colFrom - j;
            m_buf += m_rowFrom - i;
        } else {
            m_buf -= m_rowFrom - i;
            LOOP_ROW_INT(*this, k) m_buf[k] -= m_colFrom - j;
        }
    }
    template <class INT> Array<Type> &operator[](INT i) {
        CHECK_ROW_RANGE(*this, i);
        return m_vector[i];
    }
    template <class INT> Array<Type> &operator[](INT i) const {
        CHECK_ROW_RANGE(*this, i);
        return m_vector[i];
    }
    Array<Type> &First() { return m_vector[m_rowFrom]; }
    Array<Type> &First() const { return m_vector[m_rowFrom]; }
    Array<Type> &Last() { return m_vector[m_rowTo]; }
    Array<Type> &Last() const { return m_vector[m_rowTo]; }
    Type operator=(const Type s) {
        _LOOP_MATRIX_(i, j) m_buf[i][j] = s;
        return s;
    }
    DoubleArray<Type> &operator=(Type **m) {
        _LOOP_MATRIX_(i, j) m_buf[i][j] = (Type)(m[i][j]);
        //_LOOP_COL_(i) memcpy(m_buf[i], m[i], sizeof(Type));
        return (*this);
    }
    DoubleArray<Type> &operator=(const DoubleArray<Type> &m) {
        if(!IS_MATRIX_MATCH(*this, m))
            Alloc(m.RowFrom(), m.RowTo(), m.ColFrom(), m.ColTo());
        return operator=(m.m_buf);
    }
    BOOLEAN operator==(const DoubleArray<Type> &m) const {
        if(IS_MATRIX_MATCH(*this, m)) _LOOP_MATRIX_(i, j)
        if(IsSame(m_buf[i][j], m.m_buf[i][j]) == FALSE) return FALSE;
        return TRUE;
    }
    DoubleArray<Type> &operator+=(const DoubleArray<Type> &m) {
        CHECK_MATRIX_MATCH(*this, m);
        _LOOP_MATRIX_(i, j) m_buf[i][j] += m.m_buf[i][j];
        return (*this);
    }
    DoubleArray<Type> &operator+=(Type **m) {
        _LOOP_MATRIX_(i, j) m_buf[i][j] += m[i][j];
        return (*this);
    }
    DoubleArray<Type> &operator+=(const Type &t) {
        _LOOP_MATRIX_(i, j) m_buf[i][j] += t;
        return (*this);
    }
    DoubleArray<Type> &operator-=(const DoubleArray<Type> &m) {
        CHECK_MATRIX_MATCH(*this, m);
        _LOOP_MATRIX_(i, j) m_buf[i][j] -= m.m_buf[i][j];
        return (*this);
    }
    DoubleArray<Type> &operator-=(Type **m) {
        _LOOP_MATRIX_(i, j) m_buf[i][j] -= m[i][j];
        return (*this);
    }
    DoubleArray<Type> &operator-=(const Type &t) {
        _LOOP_MATRIX_(i, j) m_buf[i][j] -= t;
        return (*this);
    }
    DoubleArray<Type> &operator*=(const Type &t) {
        _LOOP_MATRIX_(i, j) m_buf[i][j] *= t;
        return (*this);
    }
    DoubleArray<Type> &operator/=(const Type &t) {
        _LOOP_MATRIX_(i, j) m_buf[i][j] /= t;
        return (*this);
    }

    void Identify() {
        if(m_rowFrom != m_colFrom || m_rowTo != m_colTo)
            Error("Trial of identifying non-squre matrix.");
        *this = (Type)0;
        _LOOP_ROW_(i) m_buf[i][i] = (Type)1;
    }

    Type Inverse(); // return value is determinant
    void Normalize(int col) {
        double norm = 0.;
        _LOOP_ROW_(i) norm += SQR(m_buf[i][col]);
        norm = sqrt(norm);
        _LOOP_ROW_(i) m_buf[i][col] = (Type)(m_buf[i][col] / norm);
    }
    Type **InnerProduct(const DoubleArray<Type> &m1,
                        const DoubleArray<Type> &m2,
                        BOOLEAN bFlag = TRUE) { // m = m1 & m2
#ifdef _DEBUG
        if(m1.ColSize() != m2.RowSize())
            Error("Error in DoubleArray::InnerProduct : (%d, %d) (%d, %d)\n",
                  m1.RowSize(), m1.ColSize(), m2.RowSize(), m2.ColSize());
#endif
        if(bFlag) Alloc(m1.RowFrom(), m1.RowTo(), m2.ColFrom(), m2.ColTo());
        Type **pm2 = m2.m_buf;
        _LOOP_MATRIX_(i, j) {
            Type *pm1i = m1.m_buf[i];
            int from = m1.ColFrom(), to = m1.ColTo();
            Type temp = (Type)0;
            for(int k = from; k <= to; k++) temp += pm1i[k] * pm2[k][j];
            m_buf[i][j] = temp;
        }
        return m_buf;
    }
    Type InnerProduct(Array<Type> &v1, Array<Type> &v2) // result = v1 & M & v2
    {
#ifdef _DEBUG
        if(v1.Size() != RowSize() || v2.Size() != ColSize())
            Error(
                "Error in DoubleArray::InnerProduct : (%d) & (%d x %d) & (%d) "
                "\n",
                v1.Size(), RowSize(), ColSize(), v2.Size());
#endif
        Type s = (Type)0;
        _LOOP_MATRIX_(i, j)
        s += v1.m_buf[i] * m_buf[i][j] * v2.m_buf[j];
        return s;
    }
    void TripleProduct(const DoubleArray<Type> &m1, const DoubleArray<Type> &m2,
                       const DoubleArray<Type> &m3) {
        Alloc(m1.RowFrom(), m1.RowTo(), m3.ColFrom(), m3.ColTo());
        Type **pm1 = m1.m_buf, **pm2 = m2.m_buf, **pm3 = m3.m_buf;
        _LOOP_MATRIX_(i, j) {
            Type temp = (Type)0;
            LOOP_MATRIX_INT(m2, k, l)
            temp += pm1[i][k] * pm2[k][l] * pm3[l][j];
            m_buf[i][j] = temp;
        }
    }
    void OuterProduct(const Array<Type> &v1, const Array<Type> &v2) {
#ifdef _DEBUG
        if(v1.Size() != RowSize() || v2.Size() != ColSize())
            Error(
                "Error in DoubleArray::InnerProduct : (%d) & (%d x %d) & (%d) "
                "\n",
                v1.Size(), RowSize(), ColSize(), v2.Size());
#endif
        _LOOP_MATRIX_(i, j) m_buf[i][j] = v1.m_buf[i] * v2.m_buf[j];
    }
    Type GetTrace() {
#ifdef _DEBUG
        if(m_rowFrom != m_colFrom || m_rowTo != m_colTo)
            Error("Error in class DoubleArray::GetTrace()");
#endif
        Type sum = (Type)0;
        _LOOP_ROW_(i) sum += m_buf[i][i];
        return sum;
    }

    void SVDecomp(Vector &w, DoubleArray<float> &v);
    void SVDInverse();

    void Echo(const char *format = NULL) const {
        if(format) _LOOP_ROW_(i) {
                _LOOP_COL_(j) printf(format, m_buf[i][j] + TolZero);
                printf("\n");
            }
        else
            _LOOP_MATRIX_(i, j)
            printf("%s%c", ::ToStr(m_buf[i][j] + TolZero),
                   j == m_colTo ? '\n' : '\t');
        printf("\n");
    }

#ifdef __AFX_H__
    friend class TripleArray<Type>;
#else
    template <class T> friend class TripleArray;
    friend class Image;
#endif
};

template <class Type>
DoubleArray<Type> operator+(const DoubleArray<Type> &m1,
                            const DoubleArray<Type> &m2) {
    DoubleArray<Type> matrix(m1);
    matrix += m2;
    return matrix;
}
template <class Type>
DoubleArray<Type> operator+(const DoubleArray<Type> &m1, const Type *m2) {
    DoubleArray<Type> matrix(m1);
    matrix += m2;
    return matrix;
}
template <class Type>
DoubleArray<Type> operator+(const Type *m1, const DoubleArray<Type> &m2) {
    DoubleArray<Type> matrix(m1);
    matrix += m2;
    return matrix;
}

template <class Type>
DoubleArray<Type> operator-(const DoubleArray<Type> &m1,
                            const DoubleArray<Type> &m2) {
    DoubleArray<Type> matrix(m1);
    matrix -= m2;
    return matrix;
}
template <class Type>
DoubleArray<Type> operator-(const DoubleArray<Type> &m1, const Type *m2) {
    DoubleArray<Type> matrix(m1);
    matrix -= m2;
    return matrix;
}
template <class Type>
DoubleArray<Type> operator-(const Type m1[], const DoubleArray<Type> &m2) {
    DoubleArray<Type> matrix(m1);
    matrix -= m2;
    return matrix;
}

template <class Type> // M *= t
DoubleArray<Type> operator*(const DoubleArray<Type> &m, Type s) {
    DoubleArray<Type> matrix(m);
    matrix *= s;
    return matrix;
}
template <class Type> // M(n,m) = A(n,l) x B(l,m)
DoubleArray<Type> operator*(const DoubleArray<Type> &m1,
                            const DoubleArray<Type> &m2) {
#ifdef _DEBUG
    if(m1.ColFrom() != m2.RowFrom() || m1.ColTo() != m2.RowTo())
        Error("Size of matrix are not match to operator*(m1, m2).");
#endif
    DoubleArray<Type> matrix(m1.RowFrom(), m1.RowTo(), m2.ColFrom(),
                             m2.ColTo());
    LOOP_MATRIX_INT(matrix, i, j) {
        matrix[i][j] = (Type)0;
        LOOP_COL_INT(m1, k) matrix[i][j] += m1[i][k] * m2[k][j];
    }
    return matrix;
}
template <class Type> // V'(n) = M(n,m) x V(m)
Array<Type> operator*(const DoubleArray<Type> &m, const Array<Type> &v) {
#ifdef _DEBUG
    if(m.ColFrom() != v.From() || m.ColTo() != v.To())
        Error("Size of matrix are not match to operator*(m, v).");
#endif
    Array<Type> vector(m.RowFrom(), m.RowTo());
    LOOP_ARRAY_INT(vector, i) {
        vector[i] = (Type)0;
        LOOP_ARRAY_INT(v, j) vector[i] += m[i][j] * v[j];
    }
    return vector;
}

template <class Type> // V'(m) = V(n) x M(n,m)
Array<Type> operator*(const Array<Type> &v, const DoubleArray<Type> &m) {
#ifdef _DEBUG
    if(v.From() != m.RowFrom() || v.To() != m.RowTo())
        Error("Size of matrix are not match to operator*(v, m).");
#endif
    Array<Type> vector(m.ColFrom(), m.ColTo());
    LOOP_ARRAY_INT(vector, i) {
        vector[i] = (Type)0;
        LOOP_ARRAY_INT(v, j) vector[i] += v[j] * m[j][i];
    }
    return vector;
}

template <class Type> // M /= t
DoubleArray<Type> operator/(const DoubleArray<Type> &m, Type s) {
    DoubleArray<Type> matrix(m);
    matrix /= s;
    return matrix;
}

template <class Type>
Array<Type> &InnerProduct(const DoubleArray<Type> &m, const Array<Type> &x,
                          Array<Type> &y) {
    y.InnerProduct(m, x);
}

template <class Type>
Type TripleProduct(Type *x, const DoubleArray<Type> &m, Type *y) {
    Type temp = (Type)0;
    LOOP_MATRIX_INT(m, i, j) temp += x[i] * m.m_buf[i][j] * y[j];
    return temp;
}

template <class Type>
Type TripleProduct(const Array<Type> &x, const DoubleArray<Type> &m,
                   const Array<Type> &y) {
    return TripleProduct(x.m_buf, m, y.m_buf);
}

typedef DoubleArray<BOOLEAN> BMatrix;
typedef DoubleArray<WORD> WMatrix;
typedef DoubleArray<int> IMatrix;
typedef DoubleArray<long> LMatrix;
typedef DoubleArray<float> FMatrix;
typedef DoubleArray<double> DMatrix;

typedef DoubleArray<double> Matrix;
// typedef DoubleArray<int>    Image;

#endif
