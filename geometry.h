// Toy/geometry.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_GEOMETRY_H_
#define _TOY_GEOMETRY_H_

#ifdef TOY
#include <Toy/vector.h>
#else
#include "vector.h"
#endif

typedef Array2D<int> Point2DI;
typedef Array2D<int> P2DI;
typedef Array2D<float> Point2DR;
typedef Array2D<float> P2DR;
typedef Array2D<double> Point2DD;
typedef Array2D<double> P2DD;
typedef Array2D<double> Point2D;
typedef Array2D<double> P2D;

/*
class Line2D {
public:
    Point2D m_sp, m_ep;

    Line2D() {}
    Line2D(const Point2D &sp, const Point2D &ep);
    Line2D(float x1, float y1, float x2, float y2);

    Point2D GetSP() const { return m_sp; }
    Point2D GetEP() const { return m_ep; }
    Point2D &GetSP() { return m_sp; }
    Point2D &GetEP() { return m_ep; }
};

class Triangle3D {
public:
    Point2D m_pt[3];
    Line2D  m_line[3];

    Point2D operator[](const int &i) const { return m_pt[i]; }
};

BOOLEAN IsSamePoint2D(float x1, float y1, float x2, float y2,
                                        float tol = TolZeroF);
BOOLEAN IsSamePoint2D(const Point2D &pt1, const Point2D &pt2,
                                        float tol = TolZeroF);
*/

#endif
