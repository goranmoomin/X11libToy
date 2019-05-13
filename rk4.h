// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifndef _RK4_H_
#define _RK4_H_

#include <Toy/vector.h>

template <class Type>
void rk4(const Array<Type> &y, const Array<Type> &dydx, Type x, Type h,
         Array<Type> &yout,
         void (*derivs)(Type, const Array<Type> &, Array<Type> &)) {
    int n = y.Size();

    Array<Type> dym(1, n);
    Array<Type> dyt(1, n);
    Array<Type> yt(1, n);

    Type hh = h * 0.5;
    Type h6 = h / 6.0;
    Type xh = x + hh;
    yt = y + hh * dydx;
    (*derivs)(xh, yt, dyt);
    yt = y + hh * dyt;
    (*derivs)(xh, yt, dym);
    for(int i = 1; i <= n; i++) {
        yt[i] = y[i] + h * dym[i];
        dym[i] += dyt[i];
    }
    (*derivs)(x + h, yt, dyt);
    for(int i = 1; i <= n; i++)
        yout[i] = y[i] + h6 * (dydx[i] + dyt[i] + 2.0 * dym[i]);
}

#endif
