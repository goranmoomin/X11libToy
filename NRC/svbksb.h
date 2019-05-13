#ifndef _NRC_SVBKSB_H_
#define _NRC_SVBKSB_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

template <class Type>
void svbksb(Type **u, Type w[], Type **v, int m, int n, Type b[], Type x[]) {
    int jj, j, i;
    Type s;

    Array<Type> tmp(1, n);
    for(j = 1; j <= n; j++) {
        s = 0.0;
        if(w[j]) {
            for(i = 1; i <= m; i++) s += u[i][j] * b[i];
            s /= w[j];
        }
        tmp[j] = s;
    }
    for(j = 1; j <= n; j++) {
        s = 0.0;
        for(jj = 1; jj <= n; jj++) s += v[j][jj] * tmp[jj];
        x[j] = s;
    }
}

template <class Type>
void svbksb(DoubleArray<Type> &u, Array<Type> &w, DoubleArray<Type> &v,
            Array<Type> &b, Array, Type > &x) {
    u.Offset(1, 1, TRUE);
    v.Offset(1, 1, TRUE);
    w.Offset(1, 1, TRUE);
    svdcmp(u(), u.RowSize(), u.ColSize(), w(), v(), b.Offset(1), x.Offset(1));
    u.Offset(1, 1, FALSE);
    v.Offset(1, 1, FALSE);
    w.Offset(1, 1, FALSE);
}

#endif
