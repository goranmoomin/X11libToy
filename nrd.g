#ifndef _NRC_
#define _NRC_

#include <nrd.h>

#define Shift(m, b) m.Shift(1-m.RowFrom(), 1-m.ColFrom(), b)

void jacobi(double **a, int n, double d[], double **v, int *nrot);
void jacobi(Matrix &a, Vector &d, Matrix &v, int *nrot)
{
    Shift(a, TRUE);
    Shift(v, TRUE);
    jacobi(a(), d.Size(), d.Offset(1), v(), nrot);
    Shift(a, FALSE);
    Shift(v, FALSE);
}

void svdcmp(double **a, int m, int n, double w[], double **v);
void svdcmp(Matrix &a, Vector &w, Matrix &v)
{
    Shift(a, TRUE);
    Shift(v, TRUE);
    svdcmp(a(), a.RowSize(), a.ColSize(), w.Offset(1), v());
    Shift(a, FALSE);
    Shift(v, FALSE);
}

#endif
