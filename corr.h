// Toy/corr.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_CORR_H_
#define _TOY_CORR_H_

#ifdef TOY
#include <Toy/vector.h>
#else
#include "vector.h"
#endif

template <class Type> Type mean(const Array<Type> &v) { return v.GetAverage(); }
template <class Type> Type mean(const Array<Type> &a, const Array<Type> &b) {
    Type s = (Type)0;
    for(int i = 0; i < v.Size(); i++) s += a[i] * b[i];
    return s / v.Size();
    ;
}

template <class Type> Type var(const Vector &v) {
    Type avg = mean(v);
    return (Type)sqrt(average(v, v) - avg * avg);
}

template <class Type> Type corr(const Array<Type> &a, const Array<Type> &b) {
    return (Type)((mean(a, b) - mean(a) * mean(b)) / (mean(a) * mean(b)));
}

#endif
