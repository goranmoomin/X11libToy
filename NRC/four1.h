#ifndef _NRC_FOUR1_H_
#define _NRC_FOUR1_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <Toy/vector.h>
#else
#include "Toy/vector.h"
#endif

template <class Type> void four1(Type data[], unsigned long nn, int isign) {
    unsigned long n = nn << 1, m;
    unsigned long i, j = 1;
    for(i = 1; i < n; i += 2) {
        if(j > i) {
            ::Swap(data[j], data[i]);
            ::Swap(data[j + 1], data[i + 1]);
        }
        m = n >> 1;
        while(m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    unsigned long mmax = 2;
    while(n > mmax) {
        unsigned long istep = mmax << 1;
        double theta = isign * (6.28318530717959 / mmax);
        double wtemp = sin(0.5 * theta);
        double wpr = -2.0 * wtemp * wtemp;
        double wpi = sin(theta);
        double wr = 1.;
        double wi = 0.;
        for(m = 1; m < mmax; m += 2) {
            for(i = m; i <= n; i += istep) {
                j = i + mmax;
                Type tempr = wr * data[j] - wi * data[j + 1];
                Type tempi = wr * data[j + 1] + wi * data[j];
                data[j] = data[i] - tempr;
                data[j + 1] = data[i + 1] - tempi;
                data[i] += tempr;
                data[i + 1] += tempi;
            }
            wr = (wtemp = wr) * wpr - wi * wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
        }
        mmax = istep;
    }
}

template <class Type> void four1(Array<Type> &data, int isign) {
    four1(data.Offset(1), data.Size() / 2, isign);
}

#endif
