// Toy/box.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_BOX_H_
#define _TOY_BOX_H_

#ifdef TOY
#include <Toy/cliptype.h>
#include <Toy/vector.h>
#else
#include "cliptype.h"
#include "vector.h"
#endif

#ifdef __AFX_H__
typedef POINT Point; // POINT  : point data structure in MFC
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
typedef XPoint Point; // XPoint : point data structure in X-Window
#endif

#define LOOP_WIDTH(b, i) for(i = (b).MinX(); i <= (b).MaxX(); i++)
#define LOOP_WIDTH_INT(b, i) for(int i = (b).MinX(); i <= (b).MaxX(); i++)
#define LOOP_HEIGHT(b, i) for(i = (b).MinY(); i <= (b).MaxY(); i++)
#define LOOP_HEIGHT_INT(b, i) for(int i = (b).MinY(); i <= (b).MaxY(); i++)

/////////////////////////////////////////////////////////////////////////////
// Box1D

template <class Type> class Box1D {
    // data
    BOOLEAN m_bInit;

  public:
    Type min, max;

    Box1D() { m_bInit = (BOOLEAN)FALSE; }
    Box1D(Type x) { Set(x); }
    Box1D(Type x1, Type x2) { Set(x1, x2); }
    Box1D(Type *a, int npt) { Set(a, npt); }
    Box1D(const Array<Type> &v) { Set(v); }
    void Set(Type x) {
        min = max = x;
        m_bInit = TRUE;
    }
    void Set(Type x1, Type x2) {
        if(x1 < x2) {
            min = x1;
            max = x2;
        } else {
            min = x2;
            max = x1;
        }
    }
    void Set(Type *a, int npt) {
        if(npt) Set(a[0]);
        for(int i = 1; i < npt; i++) Update(a[i]);
    }
    void Set(const Array<Type> &v) {
        if(!v.IsEmpty()) Set(v.First());
        for(int i = v.From() + 1; i <= v.To(); i++) Update(v.m_buf[i]);
    }
    void Update(Type x) {
        if(m_bInit) {
            if(x < min) min = x;
            if(x > max) max = x;
        } else {
            Set(x);
        }
    }
    void Update(Type *a, int npt) {
        for(int i = 0; i < npt; i++) Update(a[i]);
    }
    void Update(const Array<Type> &v) { LOOP_ARRAY_INT(v, i) Update(v[i]); }
    void Extend(Type dx) {
        min -= dx;
        max += dx;
    }
    BOOLEAN IsInDomain(Type x) const {
        return (x >= min && x <= max) ? TRUE : FALSE;
    }
    ClipCode GetClipCode(Type x, Type y) const {
        ClipCode code = CLIP_CENTER;
        if(x < min)
            code = CLIP_LEFT;
        else if(x > max)
            code = CLIP_RIGHT;
        return code;
    }
    Box1D<Type> operator*(Type s) const {
        return Box1D<Type>(min * s, max * s);
    }
    Box1D<Type> operator/(Type s) const {
        return Box1D<Type>(min / s, max / s);
    }
};

template <class Type>
BOOLEAN operator==(const Box1D<Type> &b1, const Box1D<Type> &b2) {
    if(b1.max != b2.min) return FALSE;
    if(b1.min != b2.max) return FALSE;
    return TRUE;
}

template <class Type>
BOOLEAN operator&&(const Box1D<Type> &b1, const Box1D<Type> &b2) {
    if(b1.max < b2.min) return FALSE;
    if(b1.min > b2.max) return FALSE;
    return TRUE;
}

typedef Box1D<int> Box1DI;
typedef Box1D<float> Box1DR;
typedef Box1D<double> Box1DD;

/////////////////////////////////////////////////////////////////////////////
// Box2D

template <class Type> class Box2D {
    // data
    BOOLEAN m_bInitX, m_bInitY;

  public:
    Type left, top, right, bottom;

    Box2D() { m_bInitX = m_bInitY = FALSE; }
    Box2D(Type x, Type y) {
        SetX(x);
        SetY(y);
    }
    Box2D(const Array2D<Type> &v) { Set(v.x, v.y); }
    Box2D(Type x1, Type y1, Type x2, Type y2) {
        SetX(x1, x2);
        SetY(y1, y2);
    }
    Box2D(const Array2D<Type> &v1, const Array2D<Type> &v2) { Set(v1, v2); }
    Box2D(Array2D<Type> a[], int npt) { Set(a, npt); }
    void SetX(Type x) {
        left = right = x;
        m_bInitX = TRUE;
    }
    void SetX(Type x1, Type x2) {
        if(x1 < x2) {
            left = x1;
            right = x2;
        } else {
            left = x2;
            right = x1;
        }
    }
    void SetY(Type y) {
        bottom = top = y;
        m_bInitY = TRUE;
    }
    void SetY(Type y1, Type y2) {
        if(y1 < y2) {
            bottom = y1;
            top = y2;
        } else {
            bottom = y2;
            top = y1;
        }
    }
    void Set(Type x, Type y) {
        SetX(x);
        SetY(y);
    }
    void Set(Type x1, Type y1, Type x2, Type y2) {
        SetX(x1, x2);
        SetY(y1, y2);
    }
    void Set(const Array2D<Type> &v) { Set(v.x, v.y); }
    void Set(const Array2D<Type> &v1, const Array2D<Type> &v2) {
        SetX(v1.x, v2.x);
        SetY(v1.y, v2.y);
    }
    void Set(Array2D<Type> a[], int npt) {
        if(npt) Set(a[0].x, a[0].y);
        for(int i = 1; i < npt; i++) Update(a[i].x, a[i].y);
    }
    void Update(Type x, Type y) {
        if(m_bInitX) {
            if(x < left) left = x;
            if(x > right) right = x;
        } else {
            SetX(x);
        }
        if(m_bInitY) {
            if(y < bottom) bottom = y;
            if(y > top) top = y;
        } else {
            SetY(y);
        }
    }
    void Update(const Array2D<Type> &v) { Update(v.x, v.y); }
    void Update(Array2D<Type> a[], int npt) {
        for(int i = 0; i < npt; i++) Update(a[i].x, a[i].y);
    }
    void ExtendX(Type dx) {
        left -= dx;
        right += dx;
    }
    void ExtendY(Type dy) {
        bottom -= dy;
        top += dy;
    }
    void Extend(Type dx, Type dy) {
        ExtendX(dx);
        ExtendY(dy);
    }
    void Extend(Type d) { Extend(d, d); }

    Type MinX() const { return left; }
    Type &MinX() { return left; }
    Type MaxX() const { return right; }
    Type &MaxX() { return right; }
    Type MinY() const { return bottom; }
    Type &MinY() { return bottom; }
    Type MaxY() const { return top; }
    Type &MaxY() { return top; }
    Type Width() const;
    Type Height() const;
    BOOLEAN IsInDomainX(Type x) const {
        return (x >= left && x <= right) ? TRUE : FALSE;
    }
    BOOLEAN IsInDomainY(Type y) const {
        return (y >= bottom && y <= top) ? TRUE : FALSE;
    }
    BOOLEAN IsInDomain(Type x, Type y) const {
        if(!IsInDomainX(x)) return FALSE;
        if(!IsInDomainY(y)) return FALSE;
        return TRUE;
    }
    BOOLEAN IsInDomain(const Array2D<Type> &v) const {
        return IsInDomain(v.x, v.y);
    }
    ClipCode GetClipCode(const Array2D<Type> &v) const {
        return GetClipCode(v.x, v.y);
    }
    BOOLEAN CheckClipOfLine(Index2D sp, Index2D ep, ClipCode scode,
                            ClipCode ecode);

#ifdef __AFX_H__
    operator CRect() const {
        CRect rect;
        rect.left = left rect.right = right;
        rect.top = bottom;
        rect.bottom = top;
        return rect;
    }
    void Set(CRect &rect) {
        left = rect.left;
        right = rect.right;
        top = rect.bottom;
        bottom = rect.top;
    }
#endif
    Box2D<Type> &operator=(const Box2D<Type> &box) {
        left = box.left;
        right = box.right;
        bottom = box.bottom;
        top = box.top;
        return (*this);
    }
    void operator&=(const Box2D<Type> &box) {
        if(box.m_bInitX) {
            if(m_bInitX) {
                left = ::GetMax(left, box.left);
                right = ::GetMin(right, box.right);
            } else {
                m_bInitX = TRUE;
                left = box.left;
                right = box.right;
            }
        }
        if(box.m_bInitY) {
            if(m_bInitY) {
                bottom = ::GetMax(bottom, box.bottom);
                top = ::GetMin(top, box.top);
            } else {
                m_bInitX = TRUE;
                bottom = box.bottom;
                top = box.top;
            }
        }
    }
    Box2D<Type> operator*(Type s) const {
        return Box2D<Type>(left * s, top * s, right * s, bottom * s);
    }
    Box2D<Type> operator/(Type s) const {
        return Box2D<Type>(left / s, top / s, right / s, bottom / s);
    }

    ClipCode GetClipCode(Type x, Type y) const {
        ClipCode code = CLIP_CENTER;
        if(x < left)
            code = CLIP_LEFT;
        else if(x > right)
            code = CLIP_RIGHT;
        if(y < bottom)
            code |= CLIP_DOWN;
        else if(y > top)
            code |= CLIP_UP;
        return code;
    }
};

template <class Type>
BOOLEAN operator&&(const Box2D<Type> &b1, const Box2D<Type> &b2) {
    if(b1.right < b2.left) return FALSE;
    if(b1.left > b2.right) return FALSE;
    if(b1.top < b2.bottom) return FALSE;
    if(b1.bottom > b2.top) return FALSE;
    return TRUE;
}

template <class Type>
BOOLEAN operator==(const Box2D<Type> &b1, const Box2D<Type> &b2) {
    if(b1.left != b2.left) return FALSE;
    if(b1.right != b2.right) return FALSE;
    if(b1.bottom != b2.bottom) return FALSE;
    if(b1.top != b2.top) return FALSE;
    return TRUE;
}

#ifdef __AFX_H__
template <class Type>
BOOLEAN operator==(const Box2D<Type> box, const CRect &rect) {
    if(box.left != rect.left) return FALSE;
    if(box.right != rect.right) return FALSE;
    if(box.bottom != rect.top) return FALSE;
    if(box.top != rect.bottom) return FALSE;
    return TRUE;
}
template <class Type>
BOOLEAN operator==(const CRect &rect, const Box2D<Type> &box) {
    return operator==(box, rect);
}
template <class Type>
BOOLEAN operator!=(const Box2D<Type> &box, const CRect &rect) {
    return operator==(box, rect) ? FALSE : TRUE;
}
template <class Type>
BOOLEAN operator!=(const CRect &rect, const Box2D<Type> &box) {
    return operator==(box, rect) ? FALSE : TRUE;
}
#endif

typedef Box2D<int> Box2DI;
typedef Box2D<float> Box2DR;
typedef Box2D<double> Box2DD;

/////////////////////////////////////////////////////////////////////////////
// Box3D
template <class Type> class Box3D {
    // data
    BOOLEAN m_bInitX, m_bInitY, m_bInitZ;

  public:
    Type left, top, right, bottom, lower, upper;

    Box3D() { m_bInitX = m_bInitY = m_bInitZ = (BOOLEAN)FALSE; }
    Box3D(Type x, Type y) {
        SetX(x);
        SetY(y);
    }
    Box3D(const Array3D<Type> &v) {
        left = right = v.x;
        m_bInitX = TRUE;
        top = bottom = v.y;
        m_bInitY = TRUE;
        lower = upper = v.z;
        m_bInitY = TRUE;
    }
    Box3D(Type x1, Type y1, Type z1, Type x2, Type y2, Type z2) {
        Set(x1, y1, z1, x2, y2, z2);
    }
    Box3D(const Array3D<Type> &v1, const Array3D<Type> &v2) { Set(v1, v2); }
    Box3D(Array3D<Type> *v, int npt) { Set(v, npt); }
    void SetX(Type x) {
        left = right = x;
        m_bInitX = TRUE;
    }
    void SetX(Type x1, Type x2) {
        if(x1 < x2) {
            left = x1;
            right = x2;
        } else {
            left = x2;
            right = x1;
        }
    }
    void SetY(Type y) {
        bottom = top = y;
        m_bInitY = TRUE;
    }
    void SetY(Type y1, Type y2) {
        if(y1 < y2) {
            bottom = y1;
            top = y2;
        } else {
            bottom = y2;
            top = y1;
        }
    }
    void SetZ(Type z) {
        bottom = top = z;
        m_bInitZ = TRUE;
    }
    void SetZ(Type z1, Type z2) {
        if(z1 < z2) {
            lower = z1;
            upper = z2;
        } else {
            lower = z2;
            upper = z1;
        }
    }
    void Set(Type x, Type y, Type z) {
        SetX(x);
        SetY(y);
        SetZ(z);
    }
    void Set(Type x1, Type y1, Type z1, Type x2, Type y2, Type z2) {
        SetX(x1, x2);
        SetY(y1, y2);
        SetZ(z1, z2);
    }
    void Set(const Array3D<Type> &v) { Set(v.x, v.y, v.z); }
    void Set(const Array3D<Type> &v1, const Array3D<Type> &v2) {
        SetX(v1.x, v2.x);
        SetY(v1.y, v2.y);
        SetX(v1.z, v2.z);
    }
    void Set(Array3D<Type> a[], int npt) {
        if(npt) Set(a[0].x, a[0].y);
        for(int i = 1; i < npt; i++) Update(a[i].x, a[i].y);
    }
    void Update(Type x, Type y, Type z) {
        if(m_bInitX) {
            if(x < left) left = x;
            if(x > right) right = x;
        } else {
            SetX(x);
        }
        if(m_bInitY) {
            if(y < bottom) bottom = y;
            if(y > top) top = y;
        } else {
            SetY(y);
        }
        if(m_bInitZ) {
            if(z < bottom) lower = z;
            if(z > top) upper = z;
        } else {
            SetZ(z);
        }
    }
    void Update(const Array3D<Type> &v) { Update(v.x, v.y, v.z); }
    void Update(Array3D<Type> a[], int npt) {
        for(int i = 0; i < npt; i++) Update(a[i].x, a[i].y, a[i].z);
    }
    void ExtendX(Type dx) {
        left -= dx;
        right += dx;
    }
    void ExtendY(Type dy) {
        bottom -= dy;
        top += dy;
    }
    void ExtendZ(Type dz) {
        lower -= dz;
        upper += dz;
    }
    void Extend(Type dx, Type dy, Type dz) {
        ExtendX(dx);
        ExtendY(dy);
        ExtendZ(dz);
    }
    void Extend(Type d) { Extend(d, d, d); }

    Type MinX() const { return left; }
    Type &MinX() { return left; }
    Type MaxX() const { return right; }
    Type &MaxX() { return right; }
    Type MinY() const { return bottom; }
    Type &MinY() { return bottom; }
    Type MaxY() const { return top; }
    Type &MaxY() { return top; }
    Type MinZ() const { return lower; }
    Type &MinZ() { return lower; }
    Type MaxZ() const { return upper; }
    Type &MaxZ() { return upper; }
    Type Width() const;
    Type Height() const;
    Type Depth() const;
    BOOLEAN IsInDomainX(Type x) const {
        return (x >= left && x <= right) ? TRUE : FALSE;
    }
    BOOLEAN IsInDomainY(Type y) const {
        return (y >= bottom && y <= top) ? TRUE : FALSE;
    }
    BOOLEAN IsInDomainZ(Type z) const {
        return (z >= lower && z <= upper) ? TRUE : FALSE;
    }
    BOOLEAN IsInDomain(Type x, Type y, Type z) const {
        if(!IsInDomainX(x)) return FALSE;
        if(!IsInDomainY(y)) return FALSE;
        if(!IsInDomainZ(z)) return FALSE;
        return TRUE;
    }
    BOOLEAN IsInDomain(const Array3D<Type> &v) const {
        return IsInDomain(v.x, v.y, v.z);
    }

    void operator&=(const Box3D<Type> &box) {
        if(box.m_bInitX) {
            if(m_bInitX) {
                left = ::GetMax(left, box.left);
                right = ::GetMin(right, box.right);
            } else {
                m_bInitX = TRUE;
                left = box.left;
                right = box.right;
            }
        }
        if(box.m_bInitY) {
            if(m_bInitY) {
                bottom = ::GetMax(bottom, box.bottom);
                top = ::GetMin(top, box.top);
            } else {
                m_bInitX = TRUE;
                bottom = box.bottom;
                top = box.top;
            }
        }
        if(box.m_bInitZ) {
            if(m_bInitZ) {
                lower = ::GetMax(lower, box.lower);
                upper = ::GetMin(upper, box.upper);
            } else {
                m_bInitZ = TRUE;
                lower = box.lower;
                upper = box.upper;
            }
        }
    }
    Box3D<Type> operator*(Type s) const {
        return Box3D<Type>(left * s, top * s, lower * s, right * s, bottom * s,
                           upper * s);
    }
    Box3D<Type> operator/(Type s) const {
        return Box3D<Type>(left / s, top / s, lower / s, right / s, bottom / s,
                           upper / s);
    }
};

template <class Type>
BOOLEAN operator==(const Box3D<Type> &b1, const Box3D<Type> &b2) {
    if(b1.left != b2.left) return FALSE;
    if(b1.right != b2.right) return FALSE;
    if(b1.bottom != b2.bottom) return FALSE;
    if(b1.top != b2.top) return FALSE;
    if(b1.upper != b2.lower) return FALSE;
    if(b1.lower != b2.upper) return FALSE;
    return TRUE;
}

template <class Type>
BOOLEAN operator&&(const Box3D<Type> &b1, const Box3D<Type> &b2) {
    if(b1.right < b2.left) return FALSE;
    if(b1.left > b2.right) return FALSE;
    if(b1.top < b2.bottom) return FALSE;
    if(b1.bottom > b2.top) return FALSE;
    if(b1.upper < b2.lower) return FALSE;
    if(b1.lower > b2.upper) return FALSE;
    return TRUE;
}

typedef Box3D<int> Box3DI;
typedef Box3D<float> Box3DR;
typedef Box3D<double> Box3DD;

#endif // _TOY_BOX_H_
