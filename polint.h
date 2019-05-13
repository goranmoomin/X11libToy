// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#ifndef _POLINT_H_
#define _POLINT_H_

template <class Type>
void polint(const Array<Type> &xa, const Array<Type> &ya, int n, Type x,
            Type *y, Type *dy) {
    Array<Type> c(n);
    Array<Type> d(n);

    Type dif = fabs(x - xa[1]);
    int ns = 1;
    for(int i = 1; i <= n; i++) {
        Type dift = fabs(x - xa[i]);
        if(dift < dif) {
            ns = i;
            dif = dift;
        }
        c[i] = ya[i];
        d[i] = ya[i];
    }
    *y = ya[ns--];
    for(int m = 1; m < n; m++) {
        for(i = 1; i <= n - m; i++) {
            Type ho = xa[i] - x;
            Type hp = xa[i + m] - x;
            Type w = c[i + 1] - d[i];
            Type den = ho - hp;
            if(den == 0.0) Error("Error in routine polint");
            den = w / den;
            d[i] = hp * den;
            c[i] = ho * den;
        }
        *y += (*dy = (2 * ns < (n - m) ? c[ns + 1] : d[ns--]));
    }
}

#endif
