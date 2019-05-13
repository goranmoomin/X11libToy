#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#endif
#include "image.h"
#include "message.h"

BOOLEAN Image::LoadPGM(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if(!fp) return FALSE;

    char line[256];

    fgets(line, 1024, fp);
    if(strcmp(line, "P5\n")) return FALSE;

    fgets(line, 1024, fp);

    int w, h;
    fgets(line, 1024, fp);
    sscanf(line, "%d  %d", &w, &h);

    int gray;
    fgets(line, 1024, fp);
    sscanf(line, "%d\n", &gray);

    Alloc(h, w);
    for(int i = 0; i < h; i++) fread(m_buf[i], w, 1, fp);

    fclose(fp);

    return TRUE;
}

int dir[8][2] = {{1, 0},  {1, -1}, {0, -1}, {-1, -1},
                 {-1, 0}, {-1, 1}, {0, 1},  {1, 1}};

void Image::MakeMonochrome(BYTE th) {
    LOOP_MATRIX_INT(*this, i, j)
    m_buf[i][j] = m_buf[i][j] >= th ? TRUE : FALSE;
}

/*
int Image::NumberOfNearests(int i, int j) const
{
    if (i == RowFrom() || i == RowTo())
        return (j == ColFrom() || j == ColTo()) ? 3 : 5;
    return (j == ColFrom() || j == ColTo()) ? 5 : 8;
}
*/

void Image::GetNeighborhood() {
    nbhd.Alloc(RowFrom(), RowTo(), ColFrom(), ColTo());
    nbhd = 0;
    for(int k = 0; k < 8; k++) {
        int rowFrom = RowFrom() + (dir[k][0] > 0 ? dir[k][0] : 0);
        int rowTo = RowTo() + (dir[k][0] < 0 ? dir[k][0] : 0);
        int colFrom = ColFrom() + (dir[k][1] > 0 ? dir[k][1] : 0);
        int colTo = ColTo() + (dir[k][1] < 0 ? dir[k][1] : 0);
        for(int i = rowFrom; i <= rowTo; i++)
            for(int j = colFrom; j <= colTo; j++)
                if(m_buf[i][j]) nbhd[i - dir[k][0]][j - dir[k][1]]++;
    }
}

void Image::Erode(int nRepeat) {
    int i, j;
    for(; nRepeat > 0; nRepeat--) {
        GetNeighborhood();
        //    LOOP_MATRIX_INT(m_buf, i, j)
        //	if (nbhd[i][j] < NumberOfNearests(m_buf, i, j)) m_buf[i][j] =
        //FALSE;
        for(i = RowFrom() + 1; i < RowTo(); i++)
            for(j = ColFrom() + 1; j < ColTo(); j++)
                if(nbhd[i][j] < 8) m_buf[i][j] = FALSE;
        for(i = RowFrom() + 1; i < RowTo(); i++) {
            if(nbhd[i][ColFrom()] < 5) m_buf[i][ColFrom()] = FALSE;
            if(nbhd[i][ColTo()] < 5) m_buf[i][ColTo()] = FALSE;
        }
        for(j = ColFrom() + 1; j < ColTo(); j++) {
            if(nbhd[RowFrom()][j] < 5) m_buf[RowFrom()][j] = FALSE;
            if(nbhd[RowTo()][j] < 5) m_buf[RowTo()][j] = FALSE;
        }
        if(nbhd[RowFrom()][ColFrom()] < 3) m_buf[RowFrom()][ColFrom()] = FALSE;
        if(nbhd[RowTo()][ColFrom()] < 3) m_buf[RowTo()][ColFrom()] = FALSE;
        if(nbhd[RowFrom()][ColTo()] < 3) m_buf[RowFrom()][ColTo()] = FALSE;
        if(nbhd[RowTo()][ColTo()] < 3) m_buf[RowTo()][ColTo()] = FALSE;
    }
}

void Image::Dilate(int nRepeat) {
    for(; nRepeat > 0; nRepeat--) {
        GetNeighborhood();
        LOOP_MATRIX_INT(*this, i, j)
        if(nbhd[i][j] > 0) m_buf[i][j] = TRUE;
    }
}
