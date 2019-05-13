#ifndef _ZMAP_
#define _ZMAP_

#ifdef TOY
#include "Toy\box.h"
#include "Toy\canvas.h"
#else
#include "box.h"
#include "canvas.h"
#endif

class ZMap {
  public:
    float **Z;
    float x0, y0, x1, y1;
    float dx, dy, dx2, dy2;
    int ni, nj;

    float Zd; // lower z-value of solid block
    float Zu; // upper z-value of solid block
    float Zb; // base z-value, not geometry
    float max_z, min_z;

    // Creator and destructor
    ZMap(int nx = 0, int ny = 0);
    ~ZMap();

    // Operator
    void ResetZ();
    BOOLEAN IsInDomain(int i, int j) {
        return (i >= 0 && j >= 0 && i < ni && j < nj);
    }
    Box3DD GetEstimateBox() const;

// Draw
#ifdef __AFX_H__
    BOOLEAN DrawFrame(Canvas *pCanvas, int prec, Box2DI *mb, COLORREF color);
#else
    BOOLEAN DrawFrame(Canvas *pCanvas, int prec, Box2DI *mb,
                      unsigned long color);
#endif
    BOOLEAN DrawShade(Canvas *pCanvas, int prec, Box2DI *mb, BOOLEAN bStrip);
    BOOLEAN DrawContour(Canvas *pCanvas, int prec);
};

//  --  constants.
#define Theta_Tol 1.E-3 // tol. for a point's in/out (triangle) check

//  --  macro functions.
#define GetZX(z, i) ((z)->x0 + (float)(i) * (z)->dx)
#define GetZY(z, j) ((z)->y0 + (float)(j) * (z)->dy)
#define GetZI(z, x) ((int)floor(((x) - (z)->x0) / (z)->dx + .001))
#define GetZJ(z, y) ((int)floor(((y) - (z)->y0) / (z)->dy + .001))

#define GetZIu(z, x) ((int)ceil(((x) - (z)->x0) / (z)->dx - .001))
#define GetZJu(z, y) ((int)ceil(((y) - (z)->y0) / (z)->dy - .001))

#define GetZId(z, x) ((int)floor(((x) - (z)->x0) / (z)->dx + .001))
#define GetZJd(z, y) ((int)floor(((y) - (z)->y0) / (z)->dy + .001))

#define GetZIr(z, x) ((int)floor(((x) - (z)->x0) / (z)->dx + 0.5))
#define GetZJr(z, y) ((int)floor(((y) - (z)->y0) / (z)->dy + 0.5))

#define GetZIo(z, x) ((int)floor(((x) - (z)->x0) / (z)->dx))
#define GetZJo(z, y) ((int)floor(((y) - (z)->y0) / (z)->dy))

//  --  Miscellaneous
float **alloc_2r(int ni, int nj);
void free_2r(float **array, int n);

#endif
