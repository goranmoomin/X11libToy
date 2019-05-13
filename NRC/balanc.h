#ifndef _NRC_BALANC_H_
#define _NRC_BALANC_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <Toy/matrix.h>
#else
#include "Toy/matrix.h"
#endif

#define RADIX 2.0

template <class Type> void balanc(Type **a, int n) {
    int last, j, i;
    Type s, r, g, f, c, sqrdx;

    sqrdx = RADIX * RADIX;
    last = 0;
    while(last == 0) {
        last = 1;
        for(i = 1; i <= n; i++) {
            r = c = 0.0;
            for(j = 1; j <= n; j++)
                if(j != i) {
                    c += fabs(a[j][i]);
                    r += fabs(a[i][j]);
                }
            if(c && r) {
                g = r / RADIX;
                f = 1.0;
                s = c + r;
                while(c < g) {
                    f *= RADIX;
                    c *= sqrdx;
                }
                g = r * RADIX;
                while(c > g) {
                    f /= RADIX;
                    c /= sqrdx;
                }
                if((c + r) / f < 0.95 * s) {
                    last = 0;
                    g = 1.0 / f;
                    for(j = 1; j <= n; j++) a[i][j] *= g;
                    for(j = 1; j <= n; j++) a[j][i] *= f;
                }
            }
        }
    }
}
#undef RADIX

template <class Type> void balanc(DoubleArray<Type> &a) {
    a.Offset(1, 1, TRUE);
    balanc(a, a.RowSize());
    a.Offset(1, 1, FALSE);
}

#endif
