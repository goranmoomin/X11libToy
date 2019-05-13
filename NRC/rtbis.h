#ifndef _NRC_RTBIS_H_
#define _NRC_RTBIS_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#include <math.h>
#ifdef TOY
#include "../message.h"
#else
#include "../message.h"
#endif

#define JMAX 40

template <class Type>
BOOLEAN rtbis(Type (*func)(Type), Type x1, Type x2, Type xacc, Type *y) {
    Type dx, xmid;

    Type f = (*func)(x1);
    Type fmid = (*func)(x2);
    if(f * fmid >= 0) {
        Warning(
            "Root must be bracketed for bisection in rtbis\n"
            "-- f(%f)=%f and f(%f)=%f",
            x1, f, x2, fmid);
        return FALSE;
    }
    Type rtb = f < 0.0 ? (dx = x2 - x1, x1) : (dx = x1 - x2, x2);
    for(int j = 1; j <= JMAX; j++) {
        fmid = (*func)(xmid = rtb + (dx *= 0.5));
        if(fmid <= 0.0) rtb = xmid;
        if(fabs(dx) < xacc || fmid == 0.0) {
            *y = rtb;
            return TRUE;
        }
    }
    Warning("Too many bisections in rtbis");
    return FALSE;
}
#undef JMAX

template <class Type>
Type rtbis(Type (*func)(Type), Type x1, Type x2, Type xacc) {
    Type y;
    if(rtbis(func, x1, x2, xacc, &y)) return y;
    Error("Error - NRC::rtbis()");
}

#endif
