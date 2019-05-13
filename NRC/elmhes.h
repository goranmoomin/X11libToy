#ifndef _NRC_ELMHES_H_
#define _NRC_ELMHES_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include "../matrix.h"
#else
#include "../matrix.h"
#endif

#define SWAP(g, h)                                                             \
    {                                                                          \
        y = (g);                                                               \
        (g) = (h);                                                             \
        (h) = y;                                                               \
    }

template <class Type> void elmhes(Type **a, int n) {
    int m, j, i;
    Type y, x;

    for(m = 2; m < n; m++) {
        x = 0.0;
        i = m;
        for(j = m; j <= n; j++) {
            if(fabs(a[j][m - 1]) > fabs(x)) {
                x = a[j][m - 1];
                i = j;
            }
        }
        if(i != m) {
            for(j = m - 1; j <= n; j++) SWAP(a[i][j], a[m][j])
            for(j = 1; j <= n; j++) SWAP(a[j][i], a[j][m])
        }
        if(x) {
            for(i = m + 1; i <= n; i++) {
                if((y = a[i][m - 1]) != 0.0) {
                    y /= x;
                    a[i][m - 1] = y;
                    for(j = m; j <= n; j++) a[i][j] -= y * a[m][j];
                    for(j = 1; j <= n; j++) a[j][m] += y * a[j][i];
                }
            }
        }
    }
}
#undef SWAP

template <class Type> void elmhes(DoubleArray<Type> &a) {
    a.Offset(1, 1, TRUE);
    elmhes(a, a.RowSize());
    a.Offset(1, 1, FALSE);
}

#endif
