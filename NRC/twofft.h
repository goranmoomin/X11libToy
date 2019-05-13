#ifndef _TWOFFT_H_
#define _TWOFFT_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <NRC/four1.h>
#else
#include "NRC/four1.h"
#endif

template <class Type>
void twofft(Type data1[], Type data2[], Type fft1[], Type fft2[],
            unsigned long n) {
    unsigned long nn2 = 2 + n + n;
    unsigned long nn3 = nn2 + 1;
    unsigned long jj, j;
    for(j = 1, jj = 2; j <= n; j++, jj += 2) {
        fft1[jj - 1] = data1[j];
        fft1[jj] = data2[j];
    }
    four1(fft1, n, 1);
    fft2[1] = fft1[2];
    fft1[2] = fft2[2] = (Type)0;
    for(j = 3; j <= n + 1; j += 2) {
        Type rep = (fft1[j] + fft1[nn2 - j]) / 2;
        Type rem = (fft1[j] - fft1[nn2 - j]) / 2;
        Type aip = (fft1[j + 1] + fft1[nn3 - j]) / 2;
        Type aim = (fft1[j + 1] - fft1[nn3 - j]) / 2;
        fft1[j] = rep;
        fft1[j + 1] = aim;
        fft1[nn2 - j] = rep;
        fft1[nn3 - j] = -aim;
        fft2[j] = aip;
        fft2[j + 1] = -rem;
        fft2[nn2 - j] = aip;
        fft2[nn3 - j] = rem;
    }
}

template <class Type>
void twofft(Array<Type> &data1, Array<Type> &data2, Array<Type> &fft1,
            Array<Type> &fft2, unsigned long n) {
    twofft(data1.Offset(1), data2.Offset(1), fft1.Offset(1), fft2.Offset(1), n);
}

#endif
