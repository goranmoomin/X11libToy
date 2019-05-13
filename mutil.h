// Toy/mutil.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_MUTIL_H_
#define _TOY_MUTIL_H_

#include "util.h"

#include <cmath>

#ifndef M_E
#define M_E 2.7182818284590452354 /* e */
#endif
#ifndef M_LOG2E
#define M_LOG2E 1.4426950408889634074 /* log_2 e */
#endif
#ifndef M_LOG10E
#define M_LOG10E 0.43429448190325182765 /* log_10 e */
#endif
#ifndef M_LN2
#define M_LN2 0.69314718055994530942 /* log_e 2 */
#endif
#ifndef M_LN10
#define M_LN10 2.30258509299404568402 /* log_e 10 */
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif
#ifndef M_PI2
#define M_PI2 6.28318530717958647692 /* pi*2 */
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923 /* pi/2 */
#endif
#ifndef M_PI_4
#define M_PI_4 0.78539816339744830962 /* pi/4 */
#endif
#ifndef M_1_PI
#define M_1_PI 0.31830988618379067154 /* 1/pi */
#endif
#ifndef M_2_PI
#define M_2_PI 0.63661977236758134308 /* 2/pi */
#endif
#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257390 /* 2/sqrt(pi) */
#endif
#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880 /* sqrt(2) */
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440 /* 1/sqrt(2) */
#endif

#define TolZero (1.e-12)
#define TolZeroF (1.e-6f)

inline BOOLEAN IsSame(float x, float y, float tol = TolZeroF) {
    return (fabs(x - y) < tol) ? TRUE : FALSE;
}

template <class Type> int GetSign(const Type &t) {
    return t > 0 ? 1 : (t < 0 ? -1 : 0);
}

template <class Type> Type GetMin(const Type &x, const Type &y) {
    return x < y ? x : y;
}
template <class Type> Type GetMax(const Type &x, const Type &y) {
    return x > y ? x : y;
}
template <class Type> Type GetMin(const Type &x, const Type &y, const Type &z) {
    return GetMin(GetMin(x, y), z);
}
template <class Type> Type GetMax(const Type &x, const Type &y, const Type &z) {
    return GetMax(GetMax(x, y), z);
}
template <class Type> Type GetMax(const Type *array, const int size) {
#ifdef _DEBUG_
    if(size < 0) return Error("Empty array");
#endif
    Type max = array[0];
    for(int i = 1; i < size; i++)
        if(max < array[i]) max = array[i];
    return max;
}
template <class Type> Type GetMin(const Type *array, int size) {
#ifdef _DEBUG_
    if(size < 0) return Error("Empty array");
#endif
    Type min = array[0];
    for(int i = 1; i < size; i++)
        if(min > array[i]) min = array[i];
    return min;
}
template <class Type> double GetSum(const Type *array, int size) {
#ifdef _DEBUG_
    if(size < 0) return Error("Empty array");
#endif
    double sum = 0.;
    for(int i = 0; i < size; i++) sum += array[i];
    return sum;
}
template <class Type> double GetMean(const Type *array, int size) {
#ifdef _DEBUG_
    if(size < 0) return Error("Empty array");
#endif
    return GetSum(array, size) / size;
}

template <class Type> int GetRound(Type x) {
    return (int)((x > 0.) ? floor(x) + 0.5 : ceil(x) - 0.5);
}

#ifndef SQR
template <class Type> Type SQR(Type x) { return x * x; }
#endif

template <class Type> Type Norm(Type x, Type y) { return sqrt(x * x + y * y); }
template <class Type> Type Norm2(Type x, Type y) { return x * x + y * y; }
template <class Type> Type Dot(Type x1, Type y1, Type x2, Type y2) {
    return x1 * x2 + y1 * y2;
}

template <class Type> inline Type Factorial(Type n) {
    Type f = 1;
    for(Type i = 2; i <= n; i++) f *= i;
    return f;
}
template <class Type> inline Type Permutations(Type n, Type m) {
    return Factorial(n) / Factorial(n - m);
}
template <class Type> inline Type Combinations(Type n, Type m) {
    // printf("::Combinations(%d, %d) = %d / (%d x %d)\n", n, m,
    //	Factorial(n), Factorial(n-m), Factorial(m));
    return Permutations(n, m) / Factorial(m);
}
template <class Type> inline Type Pow(Type x, int n) {
    Type p;
    for(p = (Type)1; n > 0; n--) p *= x;
    return p;
}

template <class Type> Type Modulate(Type p, Type m) {
    if(p < 0) p += m * (int)(-p / m) + m;
    p -= (int)(p / m) * m;
    return p;
}

inline double GetUpper(double x, double order = 1.) {
    if(x >= 0)
        return (double)((int)(x / order) == x / order
                            ? (int)(x / order) * order
                            : (int)(x / order + 1.) * order);
    else
        return (double)(-(int)(-x / order) * order);
}
inline double GetLower(double x, double order = 1.) {
    if(x >= 0)
        return (double)((int)(x / order) * order);
    else
        return (double)((int)(-x / order) == -x / order
                            ? -(int)(-x / order) * order
                            : -(int)(-x / order + 1.) * order);
}
inline double GetOrder10(double x) { return (double)pow(10, (int)(log10(x))); }
inline double GetOrder(double x, double d) {
    return (double)pow(d, (double)((int)(log(x) / log(d))));
}

template <class Type> inline Type pow(Type x, int d) {
    Type x_d = 1;
    for(x_d = 1; d; d--) x_d *= x;
    return x_d;
}

template <class Type>
BOOLEAN GetLineEqn(Type x1, Type y1, Type x2, Type y2, Type v[]) {
    v[2] = (double)(x1 * y2 - y1 * x2);
    if(v[2] == 0) {
        if(x1 == x2) return FALSE; /* same pt */
        v[0] = (double)(y1);
        v[1] = (double)(-x1);
        v[2] = 0.;
    } else {
        v[0] = (double)(y2 - y1);
        v[1] = (double)(x1 - x2);
    }
    return TRUE;
}

template <class Type>
BOOLEAN GetLineCross(Type a1, Type b1, Type c1, Type a2, Type b2, Type c2,
                     Type *x, Type *y) {
    double D = a1 * b2 - b1 * a2;
    if(D == 0) return FALSE;
    *x = (b1 * c2 - b2 * c1) / D;
    *y = (a2 * c1 - a1 * c2) / D;
    return TRUE;
}

template <class Type>
double GetDistance(Type a, Type b, Type c, Type x1, Type y1) {
    // (x2, y2) : the cross point of ax+by+c = 0 and bx-ay+d = 0
    double d = -b * x1 + a * y1;
    double x2 = -(b * d + a * c) / (a * a + b * b);
    double y2 = (a * d - b * c) / (a * a + b * b);
    return sqrt(SQR(x1 - x2) + SQR(y1 - y2));
}

#endif
