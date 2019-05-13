// NRC/rk4.h
//
// Last Modified : 2006/11/29
//

#ifndef _NRC_RK4_H_
#define _NRC_RK4_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifdef TOY
#include "../vector.h"
#else
#include "../vector.h"
#endif

template <class Type> class RK4 {
    Array<Type> _dym, _dyt, _yt;
    int m_dim;
    double *dym, *dyt, *yt;
    Array<Type> _y, _dydx;

  public:
    Type *y, *dydx;

    RK4(int dim = 0) { Alloc(dim); }
    void Alloc(int dim) {
        m_dim = dim;
        _y.Alloc(1, dim);
        y = _y.m_buf;
        _dydx.Alloc(1, dim);
        dydx = _dydx.m_buf;
        _dym.Alloc(1, dim);
        dym = _dym.m_buf;
        _dyt.Alloc(1, dim);
        dyt = _dyt.m_buf;
        _yt.Alloc(1, dim);
        yt = _yt.m_buf;
    }

    void Evolute(Type x, Type h) {
        Type hh = h / 2;
        Type h6 = h / 6;
        Type xh = x + hh;
        int i;
        for(i = 1; i <= m_dim; i++) yt[i] = y[i] + hh * dydx[i];
        Derivs(xh, yt, dyt);
        for(i = 1; i <= m_dim; i++) yt[i] = y[i] + hh * dyt[i];
        Derivs(xh, yt, dym);
        for(i = 1; i <= m_dim; i++) {
            yt[i] = y[i] + h * dym[i];
            dym[i] += dyt[i];
        }
        Derivs(x + h, yt, dyt);
        for(i = 1; i <= m_dim; i++)
            y[i] += h6 * (dydx[i] + dyt[i] + dym[i] * 2);
    }
    // virtual void Derivs(Type t, Type y[], Type dydt[]) = NULL;
    virtual void Derivs(Type t, Type y[], Type dydt[]) {}
};

template <class Type>
void rk4(Type y[], Type dydx[], int n, Type x, Type h, Type yout[],
         void (*derivs)(Type, Type[], Type[])) {
    int i;
    Type xh, hh, h6;

    static Array<Type> dym, dyt, yt;
    dym.Alloc(1, n);
    dyt.Alloc(1, n);
    yt.Alloc(1, n);
    hh = h * 0.5;
    h6 = h / 6.0;
    xh = x + hh;
    for(i = 1; i <= n; i++) yt[i] = y[i] + hh * dydx[i];
    (*derivs)(xh, yt, dyt);
    for(i = 1; i <= n; i++) yt[i] = y[i] + hh * dyt[i];
    (*derivs)(xh, yt, dym);
    for(i = 1; i <= n; i++) {
        yt[i] = y[i] + h * dym[i];
        dym[i] += dyt[i];
    }
    (*derivs)(x + h, yt, dyt);
    for(i = 1; i <= n; i++)
        yout[i] = y[i] + h6 * (dydx[i] + dyt[i] + 2.0 * dym[i]);
}

template <class Type>
void rk4(Array<Type> &y, Array<Type> &dydx, Type x, Type h, Array<Type> &yout,
         void (*derivs)(Type, Type[], Type[])) {
    rk4(y.Offset(1), dydx.Offset(1), y.Size(), x, h, yout.Offset(1), derivs);
}

#endif
