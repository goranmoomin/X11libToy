// Toy/graph.h
//
// Last Modified : 2002/11/04
//

#ifndef _TOY_GRAPH_H_
#define _TOY_GRAPH_H_

#ifdef TOY
#include <Toy/color.h>
#include <Toy/viewport.h>
#else
#include "color.h"
#include "viewport.h"
#endif

#ifndef __AFX_H__
#define TA_LEFT 0      // 00000
#define TA_RIGHT 2     // 00010
#define TA_CENTER 6    // 00110
#define TA_TOP 0       // 00000
#define TA_BOTTOM 8    // 01000
#define TA_BASELINE 24 // 11000
#endif

class Canvas;

typedef enum { LEFT, RIGHT, TOP, BOTTOM, CENTER } Align;

class Graph: public Viewport {
  public:
    // Data
    int m_cx, m_cy;
    int m_nColor, m_nTitle;
    Array<Point> m_pts;
#ifndef __AFX_H__
    XFontStruct *m_font;
#endif

  public:
    //
    Graph() { m_nColor = m_nTitle = 0; }

    //
    // virtual drawing functions (they will be linked as system dependent)
    //
    virtual COLORREF _SetForeground_(COLORREF foreground) = 0;
    virtual COLORREF _SetBackground_(COLORREF background) = 0;
    virtual void _DrawString_(int x, int y, UINT align, const char *) = 0;
    virtual void _DrawDot_(int x, int y) = 0;
    virtual void _DrawLine_(int x1, int y1, int x2, int y2) = 0;
    virtual void _DrawLines_(Point *lpPoints, int nCount) = 0;
    virtual void _DrawRectangle_(int x1, int y1, int x2, int y2) = 0;
    virtual void _DrawEllipse_(int x, int y, int width, int height) = 0;
    virtual void _FillRectangle_(int x1, int y1, int x2, int y2) = 0;
    virtual void _FillCircle_(int x, int y, int r) = 0;
    virtual BOOLEAN _FillPolygon_(Point *lpPoints, int nCount) = 0;

    //
    //  void DrawString(int x, int y, UINT align, const char *str);
    void DrawString(int x, int y, UINT align, const char *format, ...);
    void DrawString(int x, int y, const char *format, ...); // CENTER | BASELINE
    void DrawString(Align h, int y, const char *format, ...);
    void DrawString(int x, Align v, const char *format, ...);
    void DrawString(Align h, Align v, const char *format, ...);
    void DrawString(double x, double y, const char *format, ...);
    void DrawString3D(double x, double y, double z, const char *format, ...);
    //
    void DrawXGrid(int step, int grid = 3, BOOLEAN flag = TRUE);
    void DrawXGrid(int y, int step, int grid, BOOLEAN flag = TRUE);
    void DrawXGrid(double y, int step, int grid, BOOLEAN flag = TRUE);
    void DrawXGrid(int y, double from, double to, int step, int grid,
                   BOOLEAN flag = TRUE);
    void DrawXGrid(int y, double from, double to, double step, int grid,
                   BOOLEAN flag = TRUE);
    void DrawXGrid(double y, double from, double to, double step, int grid,
                   BOOLEAN flag = TRUE);
    void DrawXLog10Grid(int step, int grid1, int grid2, BOOLEAN flag = TRUE);
    void DrawXLog10Grid(int y, int step, int grid1, int grid2,
                        BOOLEAN flag = TRUE);
    void DrawXLog10Grid(double y, int step, int grid1, int grid2,
                        BOOLEAN flag = TRUE);
    void DrawXLog10Grid(int y, double from, double to, int step, int grid1,
                        int grid2, BOOLEAN flag = TRUE);
    void DrawYGrid(int step, int grid = 3, BOOLEAN flag = TRUE);
    void DrawYGrid(int x, int step, int grid, BOOLEAN flag = TRUE);
    void DrawYGrid(double x, int step, int grid, BOOLEAN flag = TRUE);
    void DrawYGrid(int x, double from, double to, int step, int grid,
                   BOOLEAN flag = TRUE);
    void DrawYGrid(double x, double from, double to, double step, int grid,
                   BOOLEAN flag = TRUE);
    void DrawYGrid(int x, double from, double to, double step, int grid,
                   BOOLEAN flag = TRUE);
    void DrawYLog10Grid(int step, int grid1, int grid2, BOOLEAN flag = TRUE);
    void DrawYLog10Grid(int x, int step, int grid1, int grid2,
                        BOOLEAN flag = TRUE);
    void DrawYLog10Grid(double x, int step, int grid1, int grid2,
                        BOOLEAN flag = TRUE);
    void DrawYLog10Grid(int x, double from, double to, int step, int grid1,
                        int grid2, BOOLEAN flag = TRUE);
    void DrawXGrids(int step, int grid, BOOLEAN flag = TRUE) {
        DrawXGrid(step, grid, flag);
        DrawXGrid(step, -grid, flag);
    }
    void DrawYGrids(int step, int grid, BOOLEAN flag = TRUE) {
        DrawYGrid(step, grid, flag);
        DrawYGrid(step, -grid, flag);
    }
    void DrawXLog10Grids(int step, int grid1, int grid2, BOOLEAN flag = TRUE) {
        DrawXLog10Grid(step, grid1, grid2, flag);
        DrawXLog10Grid(step, -grid1, -grid2, flag);
    }
    void DrawYLog10Grids(int step, int grid1, int grid2) {
        DrawYLog10Grid(step, grid1, grid2);
        DrawYLog10Grid(step, -grid1, -grid2);
    }
    void DrawGrids(int xstep, int ystep, int grid = 3);
    void DrawLog10Grids(int xstep, int ystep, int grid1, int grid2);
    //
    void DrawXLabel(const char *format, int step, int gap = 5);
    void DrawXLabel(int, const char *format, int, int);
    void DrawXLabel(int, const char *format, double, double, int, int);
    void DrawXLabel(int, const char *format, double, double, double, int);
    void DrawYLabel(const char *format, int step, int gap = 5);
    void DrawYLabel(int, const char *format, int, int);
    void DrawYLabel(int, const char *format, double, double, int, int);
    void DrawYLabel(int, const char *format, double, double, double, int);
    void DrawLabels(const char *format, int xstep, int ystep, int gap = 5);
    //
    void DrawXGridAndLabels(const char *format, int xstep);
    void DrawYGridAndLabels(const char *format, int ystep);
    void DrawGridAndLabels(const char *format, int xstep, int ystep);
    //
    void GetCurrentPos(int *cx, int *cy) {
        *cx = m_cx;
        *cy = m_cy;
    }
    int GetCurrentXPos() { return m_cx; }
    int GetCurrentYPos() { return m_cy; }
#ifdef __AFX_H__
    CPoint GetCurrentPos() { return CPoint(m_cx, m_cy); }
#endif
    //
    void PushPoint(int x, int y);
    void PushPoint(double x, double y);
    void PushPoint(double x, double y, double z);
    void PushPoint(const Point &p);
    void PushPoint(const V2D &p);
    void PushPoint(const V3D &p);
    void PopPoint(int n = 1);
    void PopPoint(int *x, int *y);
    //
    void MoveTo(int x, int y); // window cord.
    void MoveTo(int x, double y);
    void MoveTo(double x, int y);
    void MoveTo(double x, double y); // view cord.
    void MoveTo(double p[]);
    void MoveTo(const Point &p);
    void MoveTo(const V2DI &p);
    void MoveTo(const V2D &p);
    void MoveRel(int dx, int dy) { MoveTo(m_cx + dx, m_cy + dy); }
    //
    void DrawDot(int x, int y);
    void DrawDot(int x, double y);
    void DrawDot(double x, int y);
    void DrawDot(double x, double y);
    void DrawDot(double p[]);
    void DrawDot(const Point &p);
    void DrawDot(const V2DI &p);
    void DrawDot(const V2D &p);
    //
    void DrawDots(int x[], int y[], int);
    void DrawDots(double y[], int size);
    void DrawDots(double x[], double y[], int size);
    void DrawDots(V2D *a, int size);
    //
    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawLine(double x1, double y1, double x2, double y2); // window
    void DrawLine(double x, double y, int dx, int dy);
    void DrawLine(double p1[], double p2[]);
    void DrawLine(const Point &sp, const Point &ep);
    void DrawLine(const V2DI &sp, const V2DI &ep);
    void DrawLine(const V2D &sp, const V2D &ep);
    void DrawLine(double x1, double y1, double z1, double x2, double y2,
                  double z2);                    // world
    void DrawLine(double a, double b, double c); // ax+by+c = 0
    void DrawXLine(int y);
    void DrawXLine(int y, int x1, int x2);
    void DrawXLine(double y);
    void DrawXLine(double y, double x1, double x2);
    void DrawYLine(int x);
    void DrawYLine(int x, int y1, int y2);
    void DrawYLine(double x);
    void DrawYLine(double x, double y1, double y2);

    void DrawLineTo(int x, int y);
    void DrawLineTo(int x, double y);
    void DrawLineTo(double x, int y);
    void DrawLineTo(float x, float y) { DrawLineTo((double)x, (double)y); }
    void DrawLineTo(double x, double y);
    void DrawLineTo(double p[]) { DrawLineTo(p[0], p[1]); }
    void DrawLineTo(const V2D &);
    void DrawLineRel(int dx, int dy) {
        DrawLine(m_cx, m_cy, m_cx + dx, m_cy + dy);
    }
    //
    void DrawArrow(int x1, int y1, int x2, int y2, int a);
    void DrawArrow(double x1, double y1, double x2, double y2, int a);
    void DrawArrow(double x, double y, int u, int v, int a);
    void DrawArrow(int x, int y, double r, double angle, int a);
    void DrawLines(int *, int *, int);
    void DrawLines(double *, double *, int);
    void DrawLines(IVector &x, IVector &y);
    void DrawLines(Vector &x, Vector &y);
    void DrawLines(Point *lpPoints, int nCount);
    void DrawLines(BOOLEAN bPop = TRUE);
    void DrawLines(ColorEnum color);
    void DrawXLines(double *, int);
    void DrawXLines(Vector &);
    void DrawYLines(double *, int);
    void DrawYLines(Vector &);
    //
    void DrawRectangle(int x1, int y1, int x2, int y2);
    void DrawRectangle(const Box2DI &rect);
    void DrawRectangle(double x1, double y1, int x2, int y2);
    void DrawRectangle(double x1, double y1, double x2, double y2);
    void DrawRectangle(const Box2DD &rect);
    //
    void DrawEllipse(int x, int y, int width, int height);
    void DrawEllipse(double x, double y, int width, int height);
    void DrawCircle(int x, int y, int r) { DrawEllipse(x, y, r, r); }
    void DrawCircle(int x, int y, double r) {
        DrawEllipse(x, y, (int)(r * ViewToWinScaleX()),
                    (int)(r * ViewToWinScaleY()));
    }
    void DrawCircle(double x, double y, int r) { DrawEllipse(x, y, r, r); }
    void DrawCircle(Vector2D &v, int r) { DrawCircle(v.x, v.y, r); }
    void DrawCircle(Vector2D &v, double r) { DrawCircle(v.x, v.y, r); }
    void DrawCircle(double x, double y, double r) {
        DrawEllipse(x, y, (int)(r * ViewToWinScaleX()),
                    (int)(r * ViewToWinScaleY()));
    }
    void DrawTriangle(int x, int y, int r);
    void DrawTriangle(double x, double y, int r) {
        DrawTriangle(V2WX(x), V2WY(y), r);
    }
    void DrawSquare(int x, int y, int r);
    void DrawSquare(double x, double y, int r) {
        DrawSquare(V2WX(x), V2WY(y), r);
    }
    //
    void DrawHoneycomb(int x, int y, int r);
    void DrawHoneycomb(double x, double y, int r) {
        DrawHoneycomb(V2WX(x), V2WY(y), r);
    }
    //
    void DrawStar(int x, int y, int r);
    void DrawStar(double x, double y, int r) { DrawStar(V2WX(x), V2WY(y), r); }
    void DrawStar(double p[], int r) { DrawStar(p[0], p[1], r); }
    void DrawCross(int x, int y, int r);
    void DrawCross(double x, double y, int r) {
        DrawCross(V2WX(x), V2WY(y), r);
    }
    void DrawCross(double p[], int r) { DrawCross(p[0], p[1], r); }
    void DrawTimes(int x, int y, int r);
    void DrawTimes(double x, double y, int r) {
        DrawTimes(V2WX(x), V2WY(y), r);
    }
    void DrawTimes(double p[], int r) { DrawTimes(p[0], p[1], r); }
    void DrawTimes(const V2D &pt, int r) { DrawTimes(pt.x, pt.y, r); }
    void DrawTimes(const V3D &pt, int r) { DrawTimes(R2WX(pt), R2WY(pt), r); }
    void DrawLozenge(int x, int y, int w, int h);
    void DrawLozenge(double x, double y, int w, int h) {
        DrawLozenge(ViewToWinX(x), ViewToWinY(y), w, h);
    }
    void DrawLozenge(double p[], int w, int h) {
        DrawLozenge(p[0], p[1], w, h);
    }
    void DrawLozenge(const Point &pt, int w, int h) {
        DrawLozenge(pt.x, pt.y, w, h);
    }
    void DrawLozenge(const V2D &pt, int w, int h) {
        DrawLozenge(pt.x, pt.y, w, h);
    }
    void DrawLozenge(const V3D &pt, int w, int h) {
        DrawLozenge(R2WX(pt), R2WY(pt), w, h);
    }
    void DrawDiamond(int x, int y, int r) { DrawLozenge(x, y, r, r); }
    void DrawDiamond(double x, double y, int r) { DrawLozenge(x, y, r, r); }
    void DrawDiamond(double p[], int r) { DrawLozenge(p[0], p[1], r, r); }
    void DrawAstral(int x, int y, int r);
    void DrawAstral(double x, double y, int r) {
        DrawAstral(ViewToWinX(x), ViewToWinY(y), r);
    }
    void DrawAstral(double p[], int r) { DrawAstral(p[0], p[1], r); }
    void DrawAstral(const Point &pt, int r) { DrawAstral(pt.x, pt.y, r); }
    void DrawAstral(const V2D &pt, int r) { DrawAstral(pt.x, pt.y, r); }
    void DrawAstral(const V3D &pt, int r) { DrawAstral(R2WX(pt), R2WY(pt), r); }
    void DrawBox(int x, int y, int r) {
        DrawRectangle(x - r, y - r, x + r, y + r);
    }
    void DrawBox(double x, double y, int r) {
        DrawBox(ViewToWinX(x), ViewToWinY(y), r);
    }
    void DrawBox(double p[], int r) { DrawBox(p[0], p[1], r); }
    void DrawBoxDot(int x, int y, int r) {
        DrawBox(x, y, r);
        DrawDot(x, y);
    }
    void DrawBoxDot(double x, double y, int r) {
        DrawBoxDot(ViewToWinX(x), ViewToWinY(y), r);
    }
    void DrawBoxDot(double p[], int r) { DrawBoxDot(p[0], p[1], r); }
    //
    void FillRectangle(int x1, int y1, int x2, int y2);
    void FillRectangle(const Box2DI &rect);
    void FillRectangle(double x1, double y1, double x2, double y2);
    void FillRectangle(double x, double y, int w, int h);
    void FillRectangle(const Box2DD &rect);
    //
    void FillCircle(int x, int y, int r);
    void FillCircle(double x, double y, int r);
    void FillCircle(const Vector2DI &p, int r);
    void FillCircle(const Vector2D &p, int r);
    //
    void FillSquare(int x, int y, int r);
    void FillSquare(double x, double y, int r) {
        FillSquare(V2WX(x), V2WY(y), r);
    }
    //
    void FillHoneycomb(int x, int y, int r);
    void FillHoneycomb(double x, double y, int r) {
        FillHoneycomb(V2WX(x), V2WY(y), r);
    }
    //
    BOOLEAN FillPolygon(Point *lpPoints, int nCount);
    BOOLEAN FillPolygon(BOOLEAN pPop = TRUE);
    //
    void DrawXGrid3D(int, int);
    void DrawXGrid3D(double, double, int, int);
    void DrawXGrid3D(double, double, double, double, int, int);
    void DrawXGrid3D(double, double, double, double, double, int);
    void DrawYGrid3D(int, int);
    void DrawYGrid3D(double, double, int, int);
    void DrawYGrid3D(double, double, double, double, int, int);
    void DrawYGrid3D(double, double, double, int);
    void DrawYGrid3D(double, double, double, double, double, int);
    void DrawZGrid3D(int, int);
    void DrawZGrid3D(double, double, int, int);
    void DrawZGrid3D(double, double, double, double, int, int);
    void DrawZGrid3D(double, double, double, double, double, int);
    void DrawGrid3D(int, int, int, int);
    //
    void DrawXLabel3D(char *, int, int);
    void DrawXLabel3D(double, double, char *, int, int);
    void DrawXLabel3D(double, double, char *, double, double, int, int);
    void DrawXLabel3D(double, double, char *, double, double, double, int);
    void DrawYLabel3D(char *, int, int);
    void DrawYLabel3D(double, double, char *, int, int);
    void DrawYLabel3D(double, double, char *, double, double, int, int);
    void DrawYLabel3D(double, double, char *, double, double, double, int);
    void DrawZLabel3D(char *, int, int);
    void DrawZLabel3D(double, double, char *, int, int);
    void DrawZLabel3D(double, double, char *, double, double, int, int);
    void DrawZLabel3D(double, double, char *, double, double, double, int);
    void DrawZLabel3D(char *, double, double, double, int);
    //
    void MoveTo3D(double, double, double);
    void MoveTo3D(const Point3D &pt) { MoveTo3D(pt.x, pt.y, pt.z); }
    //
    void DrawDot3D(double, double, double);
    void DrawDots3D(double *, double *, double *, int);
    void DrawDots3D(Vector &, Vector &, Vector &);
    void DrawDots3D(const Point3D &);
    //
    void DrawLine3D(double, double, double, double, double, double);
    void DrawLine3D(const Vector3D &sp, const Vector3D &ep);

    void DrawLineTo3D(double, double, double);
    void DrawLineTo3D(const Point3D &pt) { DrawLineTo3D(pt.x, pt.y, pt.z); }

    BOOLEAN DrawLines3D(Vector3D *pts, int npt, Box2DR *viewBox = NULL);
    void DrawLines3D(double *, double *, double *, int);
    void DrawLines3D(Vector &, Vector &, Vector &);
    void DrawLines3D(const Point3D &);
    //
    void DrawCubic3D(double, double, double, double, double, double);

    //
    void DrawArrow3D(double, double, double, double, double, double, int);

    //
    void DrawFunction(double (*func)(double));
    void DrawFunction(double (*func)(double), ColorEnum color);
    void DrawFunction(double (*func)(double), const char *title);

    //
    friend class Canvas;
};

#endif
