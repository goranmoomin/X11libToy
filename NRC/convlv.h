#ifndef _CONVLV_H_
#define _CONVLV_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include <NRC/realft.h>
#include <NRC/twofft.h>
#include <Toy/message.h>
#else
#include "NRC/realft.h"
#include "NRC/twofft.h"
#include "Toy/message.h"
#endif

template <class Type>
void convlv(Type data[], unsigned long n, Type respns[], unsigned long m,
            int isign, Type ans[]) {
    Type dum, mag2;
    unsigned long i;

    Array<Type> fft(1, n << 1);

    for(i = 1; i <= (m - 1) / 2; i++) respns[n + 1 - i] = respns[m + 1 - i];
    for(i = (m + 3) / 2; i <= n - (m - 1) / 2; i++) respns[i] = (Type)0;
    twofft(data, respns, fft(), ans, n);
    unsigned long no2 = n >> 1;
    for(i = 2; i <= n + 2; i += 2) {
        if(isign == 1) {
            ans[i - 1] =
                (fft[i - 1] * (dum = ans[i - 1]) - fft[i] * ans[i]) / no2;
            ans[i] = (fft[i] * dum + fft[i - 1] * ans[i]) / no2;
        } else if(isign == -1) {
            if(fabs(mag2 = SQR(ans[i - 1]) + SQR(ans[i])) < TolZero)
                Error("Deconvolving at response zero in convlv");
            ans[i - 1] = (fft[i - 1] * (dum = ans[i - 1]) + fft[i] * ans[i])
                         / mag2 / no2;
            ans[i] = (fft[i] * dum - fft[i - 1] * ans[i]) / mag2 / no2;
        } else
            Error("No meaning for isign in convlv");
    }
    ans[2] = ans[n + 1];
    realft(ans, n, -1);
}

template <class Type>
void convlv(Array<Type> &data, Array<Type> &respns, unsigned int m, int isign,
            Array<Type> &ans) {
    int n = data.Size();
    if(ans.Size() != 2 * n) ans.Alloc(data.From(), data.From() + 2 * n - 1);
    convlv(data.Offset(1), n, respns.Offset(1), m, isign, ans.Offset(1));
}

#endif
