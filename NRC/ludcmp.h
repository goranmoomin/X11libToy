#ifndef _NRC_LUDCMP_H_
#define _NRC_LUDCMP_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <Toy/matrix.h>
#else
#include "Toy/matrix.h"
#endif

#define TINY 1.0e-20;

template <class Type> BOOLEAN ludcmp(Type **a, int n, int *indx, Type *d) {
    int i, imax, j, k;
    Type big, dum, sum, temp;
    Array<Type> vv(1, n);

    *d = (Type)1;
    for(i = 1; i <= n; i++) {
        big = (Type)0;
        for(j = 1; j <= n; j++)
            if((temp = fabs(a[i][j])) > big) big = temp;
        if(big == 0.0) return Warning("Singular matrix in routine ludcmp");
        vv[i] = 1.0 / big;
    }
    for(j = 1; j <= n; j++) {
        for(i = 1; i < j; i++) {
            sum = a[i][j];
            for(k = 1; k < i; k++) sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
        }
        big = (Type)0;
        for(i = j; i <= n; i++) {
            sum = a[i][j];
            for(k = 1; k < j; k++) sum -= a[i][k] * a[k][j];
            a[i][j] = sum;
            if((dum = vv[i] * fabs(sum)) >= big) {
                big = dum;
                imax = i;
            }
        }
        if(j != imax) {
            for(k = 1; k <= n; k++) {
                dum = a[imax][k];
                a[imax][k] = a[j][k];
                a[j][k] = dum;
            }
            *d = -(*d);
            vv[imax] = vv[j];
        }
        indx[j] = imax;
        if(a[j][j] == 0.0) a[j][j] = TINY;
        if(j != n) {
            dum = 1.0 / (a[j][j]);
            for(i = j + 1; i <= n; i++) a[i][j] *= dum;
        }
    }
}

template <class Type>
void ludcmp(DoubleArray<Type> &a, Array<int> &indx, Type *d) {
    a.Offset(1, 1, TRUE);
    ludcmp(a(), indx.Size(), indx.Offset(1), d);
    a.Offset(1, 1, FALSE);
}

#endif
