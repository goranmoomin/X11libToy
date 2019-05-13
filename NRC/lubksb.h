#ifndef _NRC_LUBKSB_H_
#define _NRC_LUBKSB_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <Toy/vector.h>
#else
#include "Toy/vector.h"
#endif

template <class Type> void lubksb(Type **a, int n, int *indx, Type b[]) {
    int i, ii = 0, ip, j;
    Type sum;

    for(i = 1; i <= n; i++) {
        ip = indx[i];
        sum = b[ip];
        b[ip] = b[i];
        if(ii)
            for(j = ii; j <= i - 1; j++) sum -= a[i][j] * b[j];
        else if(sum)
            ii = i;
        b[i] = sum;
    }
    for(i = n; i >= 1; i--) {
        sum = b[i];
        for(j = i + 1; j <= n; j++) sum -= a[i][j] * b[j];
        b[i] = sum / a[i][i];
    }
}

template <class Type>
void lubksb(DoubleArray<Type> &a, Array<int> &indx, Array<Type> &b) {
    //    lubksb(a()+a.RowFrom()*a.ColSize()-1, indx.Size(), indx(1), b(1));
    a.Offset(1, 1, TRUE);
    lubksb(a(), indx.Size(), indx.Offset(1), b.Offset(1));
    a.Offset(1, 1, FALSE);
}

#endif
