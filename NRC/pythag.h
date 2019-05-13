#ifndef _NRC_PYTHAG_H_
#define _NRC_PYTHAG_H_

// (C) Copr. 1986-92 Numerical Recipes Software "5""Z.

#include <math.h>

template <class Type> Type pythag(Type a, Type b) {
    Type absa = (Type)fabs(a);
    Type absb = (Type)fabs(b);
    if(absa > absb)
        return (Type)(absa * sqrt(1.0 + SQR(absb / absa)));
    else
        return (Type)(absb == 0.0 ? 0.0 : absb * sqrt(1.0 + SQR(absa / absb)));
}

#endif
