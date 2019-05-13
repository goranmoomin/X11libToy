#ifndef _REALFT_H_
#define _REALFT_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <NRC/four1.h>
#else
#include "NRC/four1.h"
#endif

template <class Type> void realft(Type data[], unsigned long n, int isign) {
    double theta = 3.141592653589793 / (double)(n >> 1);
    Type c1 = (Type)(0.5), c2;
    if(isign == 1) {
        c2 = (Type)(-0.5);
        four1(data, n >> 1, 1);
    } else {
        c2 = (Type)(0.5);
        theta = -theta;
    }
    double wtemp = sin(0.5 * theta);
    double wpr = -2.0 * wtemp * wtemp;
    double wpi = sin(theta);
    double wr = 1.0 + wpr;
    double wi = wpi;
    unsigned long np3 = n + 3;
    for(unsigned long i = 2; i <= (n >> 2); i++) {
        unsigned long i1, i2, i3, i4;
        i4 = 1 + (i3 = np3 - (i2 = 1 + (i1 = i + i - 1)));
        Type h1r = c1 * (data[i1] + data[i3]);
        Type h1i = c1 * (data[i2] - data[i4]);
        Type h2r = -c2 * (data[i2] + data[i4]);
        Type h2i = c2 * (data[i1] - data[i3]);
        data[i1] = h1r + wr * h2r - wi * h2i;
        data[i2] = h1i + wr * h2i + wi * h2r;
        data[i3] = h1r - wr * h2r + wi * h2i;
        data[i4] = -h1i + wr * h2i + wi * h2r;
        wr = (wtemp = wr) * wpr - wi * wpi + wr;
        wi = wi * wpr + wtemp * wpi + wi;
    }
    if(isign == 1) {
        Type temp = data[1];
        data[1] = data[1] + data[2];
        data[2] = temp - data[2];
    } else {
        Type temp = data[1];
        data[1] = c1 * (data[1] + data[2]);
        data[2] = c1 * (temp - data[2]);
        four1(data, n >> 1, -1);
    }
}

template <class Type> void realft(Array<Type> &data, int isign) {
    realft(data.Offset(1), data.Size(), isign);
}

#endif
