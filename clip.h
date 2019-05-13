// Toy/clip.h
//
// This originated from Yun-Chan CHUNG at May. 18. 1990
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_CLIP_H_
#define _TOY_CLIP_H_

#ifdef TOY
#include <Toy/box.h>
#else
#include "box.h"
#endif

#define viewPortXmin 0
#define viewPortYmin 0
#define viewPortXmax 1280
#define viewPortYmax 1024

ClipCode OutCode(Point p);
ClipCode OutCodeBox2DR(Box2DR *mb, double x, double y);
inline ClipCode OutCodeBox2DR(Box3DR *mb, double x, double y) {
    return OutCodeBox2DR((Box2DR *)mb, x, y);
}
ClipCode OutCodeBox2DI(const Box2DI &, int, int);
ClipCode OutCodeBox2DI(const Box2DI &, Index2D);
ClipCode OutCodeZ(double testz, double stdz, double tol);
ClipCode SubDivide(Point *pt1, Point pt2, ClipCode);
int Clip_HiddenLine(Point *P1, Point *P2);
BOOLEAN CheckClipBox2DI(const Box2DI &, int, int, int, int, ClipCode, ClipCode);
BOOLEAN CheckClipBox2DI(const Box2DI &, Index2D, Index2D, ClipCode, ClipCode);
BOOLEAN CheckClipBox2DI(const Box2DI &, int, int, int, int);
BOOLEAN CheckClipBox2DI(const Box2DI &, Index2D, Index2D);
BOOLEAN CheckClipBox2DR(Box2DR *, Vector2D, Vector2D, ClipCode, ClipCode);
BOOLEAN CheckLineCrossBox2DR(Box2DR *, Vector2D, Vector2D);
BOOLEAN CheckLineCross(Point pt1[2], Point pt2[2], double *x, double *y);
BOOLEAN CheckClip(Box3DR *a, Box2DR *b);
BOOLEAN CheckClipBox2DRs(Box2DR *a, Box2DR *b);
BOOLEAN CheckClipBox2DIs(Box2DI *a, Box2DI *b);

BOOLEAN CheckLineCross(Vector3D *p1, Vector3D *p2);

#endif
