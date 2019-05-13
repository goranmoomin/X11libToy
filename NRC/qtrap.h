#ifndef _QTRAP_H_
#define _QTRAP_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#include <math.h>
#define EPS 1.0e-5
#define JMAX 20

#ifdef TOY
#include <NRC/trapzd.h>
#include <Toy/message.h>
#else
#include "NRC/trapzd.h"
#include "Toy/message.h"
#endif

// Return the integral of the function func from a to b.
// The parameters EPS can be set to the desired fractional accuracy and JMAX
// so that 2 to the power JMAX-1 is the maximum allowed number of steps.
// Integration is performed by the trapezoidal rule.
template <class Type> Type qtrap(Type (*func)(Type), Type a, Type b) {
    Type olds = (Type)(-1.0e30);
    for(int j = 1; j <= JMAX; j++) {
        Type s = trapzd(func, a, b, j);
        if(fabs(s - olds) < EPS * fabs(olds)) return s;
        olds = s;
    }
    Error("Too many steps in routine qtrap");
    return (Type)0;
}

#undef EPS
#undef JMAX

#endif
