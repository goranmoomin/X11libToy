#ifndef _NRC_JACOBI_
#define _NRC_JACOBI_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <Toy/vector.h>
#else
#include "Toy/vector.h"
#endif

#define ROTATE(a, i, j, k, l)                                                  \
    {                                                                          \
        g = a[i][j];                                                           \
        h = a[k][l];                                                           \
        a[i][j] = g - s * (h + g * tau);                                       \
        a[k][l] = h + s * (g - h * tau);                                       \
    }

template <class Type>
void jacobi(Type **a, int n, Type d[], Type **v, int *nrot) {
    Array<Type> b(1, n);
    Array<Type> z(1, n);
    int i, j, iq, ip;
    for(ip = 1; ip <= n; ip++) {
        for(iq = 1; iq <= n; iq++) v[ip][iq] = (Type)0;
        v[ip][ip] = (Type)1;
    }
    for(ip = 1; ip <= n; ip++) {
        b[ip] = d[ip] = a[ip][ip];
        z[ip] = (Type)0;
    }
    *nrot = 0;
    for(i = 1; i <= 50; i++) {
        Type sm = (Type)0;
        for(ip = 1; ip <= n - 1; ip++)
            for(iq = ip + 1; iq <= n; iq++) sm += fabs(a[ip][iq]);
        if(fabs(sm) < TolZero) return;
        Type tresh = i < 4 ? 0.2 * sm / (n * n) : 0.;
        for(ip = 1; ip <= n - 1; ip++)
            for(iq = ip + 1; iq <= n; iq++) {
                Type g = 100.0 * fabs(a[ip][iq]);
                if(i > 4 && fabs(fabs(d[ip]) + g - fabs(d[ip])) < TolZero
                   && fabs(fabs(d[iq]) + g - fabs(d[iq])) < TolZero)
                    a[ip][iq] = (Type)0;
                else if(fabs(a[ip][iq]) > tresh) {
                    Type h = d[iq] - d[ip], t;
                    if(fabs(fabs(h) + g - fabs(h)) < TolZero)
                        t = (a[ip][iq]) / h;
                    else {
                        Type theta = 0.5 * h / (a[ip][iq]);
                        t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));
                        if(theta < 0.) t = -t;
                    }
                    Type c = 1.0 / sqrt(1 + t * t);
                    Type s = t * c;
                    Type tau = s / (1.0 + c);
                    h = t * a[ip][iq];
                    z[ip] -= h;
                    z[iq] += h;
                    d[ip] -= h;
                    d[iq] += h;
                    a[ip][iq] = Type(0);
                    for(j = 1; j <= ip - 1; j++) ROTATE(a, j, ip, j, iq);
                    for(j = ip + 1; j <= iq - 1; j++) ROTATE(a, ip, j, j, iq);
                    for(j = iq + 1; j <= n; j++) ROTATE(a, ip, j, iq, j);
                    for(j = 1; j <= n; j++) ROTATE(v, j, ip, j, iq);
                    (*nrot)++;
                }
            }
        for(ip = 1; ip <= n; ip++) {
            b[ip] += z[ip];
            d[ip] = b[ip];
            z[ip] = (Type)0;
        }
    }
    Error("Too many iterations in routine jacobi");
}

template <class Type>
void jacobi(DoubleArray<Type> &a, Array<Type> &d, DoubleArray<Type> &v,
            int *nrot) {
    a.Offset(1, 1, TRUE);
    v.Offset(1, 1, TRUE);
    jacobi(a(), d.Size(), d.Offset(1), v(), nrot);
    a.Offset(1, 1, FALSE);
    v.Offset(1, 1, FALSE);
}

#undef ROTATE

#endif
