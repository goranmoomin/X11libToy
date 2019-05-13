#ifndef _NRC_COSFT1_H_
#define _NRC_COSFT1_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <NRC/realft.h>
#include <Toy/vector.h>
#else
#include "NRC/realft.h"
#include "Toy/vector.h"
#endif

template <class Type> void cosft1(Type y[], int n) {
    double theta = M_PI / n;
    double wtemp = sin(0.5 * theta);
    double wpr = -2.0 * wtemp * wtemp;
    double wpi = sin(theta);
    double wi = 0., wr = 1.;
    Type sum = (Type)(0.5 * (y[1] - y[n + 1]));
    y[1] = (Type)(0.5 * (y[1] + y[n + 1]));
    int n2 = n + 2;
    int j;
    for(j = 2; j <= (n >> 1); j++) {
        wr = (wtemp = wr) * wpr - wi * wpi + wr;
        wi = wi * wpr + wtemp * wpi + wi;
        Type y1 = (Type)(0.5 * (y[j] + y[n2 - j]));
        Type y2 = (Type)(y[j] - y[n2 - j]);
        y[j] = y1 - wi * y2;
        y[n2 - j] = y1 + wi * y2;
        sum += wr * y2;
    }
    realft(y, n, 1);
    y[n + 1] = y[2];
    y[2] = sum;
    for(j = 4; j <= n; j += 2) {
        sum += y[j];
        y[j] = sum;
    }
}

template <class Type> void cosft1(Array<Type> &data) {
    cosft1(data.Offset(1), data.Size() - 1);
}

#endif
