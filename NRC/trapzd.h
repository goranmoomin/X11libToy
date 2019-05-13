#ifndef _TRAPZD_H_
#define _TRAPZD_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#define FUNC(x) ((*func)(x))

// This routine computes the nth stage of refinement of an extended
// trapezoidal rule. func is input as a pointer to the function to be
// integrated between a and b, also input. When called with n = 1, the
// routine returns the crudest estimate of \int_a^b f(x)dx. Subsequent
// calls with n = 2, 3, ... (in that sequential order) will improve the
// accuracy by adding 2^{n-2} additional interior points.
template <class Type> Type trapzd(Type (*func)(Type), Type a, Type b, int n) {
    static float s;

    if(n == 1) {
        s = (Type)(0.5 * (b - a) * (FUNC(a) + FUNC(b)));
        return s;
    } else {
        int it = 1;
        for(int i = 1; i < n - 1; i++) it <<= 1;
        Type tnm = (Type)it;
        Type del = (b - a) / tnm;
        Type x = a + 0.5 * del;
        Type sum = (Type)0;
        for(int j = 1; j <= it; j++, x += del) sum += FUNC(x);
        s = (Type)(0.5 * (s + (b - a) * sum / tnm));
        return s;
    }
}

#undef FUNC

#endif
