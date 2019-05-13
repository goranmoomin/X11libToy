#ifndef _CORREL_H_
#define _CORREL_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <NRC/realft.h>
#include <NRC/twofft.h>
#else
#include "NRC/realft.h"
#include "NRC/twofft.h"
#endif

template <class Type>
void correl(Type data1[], Type data2[], unsigned long n, Type ans[]) {
    Array<Type> fft(1, n << 1);
    twofft(data1, data2, fft(), ans, n);
    unsigned long no2 = n >> 1;
    for(unsigned long i = 2; i <= n + 2; i += 2) {
        Type dum = ans[i - 1];
        ans[i - 1] = (fft[i - 1] * ans[i - 1] + fft[i] * ans[i]) / no2;
        ans[i] = (fft[i] * dum - fft[i - 1] * ans[i]) / no2;
    }
    ans[2] = ans[n + 1];
    realft(ans, n, -1);
}

template <class Type>
void correl(Array<Type> &data1, Array<Type> &data2, Array<Type> &ans) {
    CHECK_MATCH(data1, data2);
    int n = data1.Size();
    if(ans.Size() != 2 * n) ans.Alloc(data1.From(), data1.From() + 2 * n - 1);
    correl(data1.Offset(1), data2.Offset(1), n, ans());
}

#endif
