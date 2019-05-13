#ifndef _QGAUS_H_
#define _QGAUS_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

template <class Type> Type qgaus(Type (*func)(Type), Type a, Type b) {
    static Type x[] = {0.,           0.1488743389, 0.4333953941,
                       0.6794095682, 0.8650633666, 0.9739065285};
    static Type w[] = {0.,           0.2955242247, 0.2692667193,
                       0.2190863625, 0.1494513491, 0.0666713443};

    Type xm = (b + a) / 2;
    Type xr = (b - a) / 2;
    Type s = (Type)0;
    for(int j = 1; j <= 5; j++) {
        Type dx = xr * x[j];
        s += w[j] * ((*func)(xm + dx) + (*func)(xm - dx));
    }
    return s *= xr;
}

#endif
