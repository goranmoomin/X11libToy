#ifndef WIN32
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#endif
#include "graph.h"

/////////////////////////////////////////////////////////////////////////////
// Graph::Push/Pop points
void Graph::PushPoint(int x, int y) {
    Point pt;
    pt.x = x;
    pt.y = y;
    m_pts.Push(pt);
}
void Graph::PushPoint(double x, double y) {
    Point pt;
    pt.x = ViewToWinX(x);
    pt.y = ViewToWinY(y);
    m_pts.Push(pt);
}
void Graph::PushPoint(const Point &p) { PushPoint(p.x, p.y); }
void Graph::PushPoint(const V2D &p) { PushPoint(p.x, p.y); }
void Graph::PushPoint(const V3D &p) { PushPoint(RealToWinX(p), RealToWinY(p)); }
void Graph::PushPoint(double x, double y, double z) {
    Point pt;
    pt.x = RealToWinX(x, y, z);
    pt.y = RealToWinY(x, y, z);
    m_pts.Push(pt);
}
void Graph::PopPoint(int n) {
    if(n == 0)
        m_pts.SetSize(0);
    else
        m_pts.Pop(n);
}
void Graph::PopPoint(int *x, int *y) {
    *x = m_pts.Last().x;
    *y = m_pts.Last().x;
    m_pts.Pop();
}

/////////////////////////////////////////////////////////////////////////////
// Graph::Graph

void Graph::DrawString(int x, int y, const char *format, ...) {
    va_list vargs;
    char str[1024];

    va_start(vargs, format);
    vsprintf(str, format, vargs);
    va_end(vargs);

    _DrawString_(x, y, TA_CENTER | TA_BASELINE, str);
}

void Graph::DrawString(int x, int y, UINT align, const char *format, ...) {
    va_list vargs;
    char str[1024];

    va_start(vargs, format);
    vsprintf(str, format, vargs);
    va_end(vargs);

    _DrawString_(x, y, align, str);
}

void Graph::DrawString(Align hAlign, int y, const char *format, ...) {
    va_list vargs;
    char str[1024];

    va_start(vargs, format);
    vsprintf(str, format, vargs);
    va_end(vargs);

    switch(hAlign) {
    case LEFT: _DrawString_(0, y, TA_LEFT | TA_BASELINE, str); break;
    case CENTER:
        _DrawString_(Width() / 2, y, TA_CENTER | TA_BASELINE, str);
        break;
    case RIGHT: _DrawString_(Width(), y, TA_RIGHT | TA_BASELINE, str); break;
    }
}

void Graph::DrawString(int x, Align vAlign, const char *format, ...) {
    va_list vargs;
    char str[1024];

    va_start(vargs, format);
    vsprintf(str, format, vargs);
    va_end(vargs);

    switch(vAlign) {
    case TOP: _DrawString_(x, 0, TA_CENTER | TA_TOP, str); break;
    case CENTER:
        _DrawString_(x, Height() / 2, TA_CENTER | TA_BASELINE, str);
        break;
    case BOTTOM: _DrawString_(x, Height(), TA_CENTER | TA_BOTTOM, str); break;
    }
}

void Graph::DrawString(Align hAlign, Align vAlign, const char *format, ...) {
    va_list vargs;
    char str[1024];

    va_start(vargs, format);
    vsprintf(str, format, vargs);
    va_end(vargs);

    int x = 0, y = 0;
    UINT align;
    switch(hAlign) {
    case LEFT: align = TA_LEFT; break;
    case CENTER:
        align = TA_CENTER;
        x += Width() / 2;
        break;
    case RIGHT:
        align = TA_RIGHT;
        x += Width();
        break;
    }
    switch(vAlign) {
    case TOP: align |= TA_TOP; break;
    case CENTER:
        align |= TA_BASELINE;
        y += Height() / 2;
        break;
    case BOTTOM:
        align |= TA_BOTTOM;
        y += Height();
        break;
    }
    _DrawString_(x, y, align, str);
}

void Graph::DrawString(double x, double y, const char *format, ...) {
    va_list vargs;
    char str[1024];

    va_start(vargs, format);
    vsprintf(str, format, vargs);
    va_end(vargs);

    _DrawString_(ViewToWinX(x), ViewToWinY(y), TA_CENTER | TA_BASELINE, str);
}

void Graph::DrawString3D(double x, double y, double z, const char *format,
                         ...) {
    va_list vargs;
    char str[1024];

    va_start(vargs, format);
    vsprintf(str, format, vargs);
    va_end(vargs);

    _DrawString_(RealToWinX(x, y, z), RealToWinY(x, y, z),
                 TA_CENTER | TA_BASELINE, str);
}

/////////////////////////////////////////////////////////////////////////////
// Graph::Grid

// DrawXGrid
void Graph::DrawXGrid(int step, int grid, BOOLEAN flag) {
    DrawXGrid(grid > 0 ? DomainBottom() : DomainTop(), step, grid, flag);
}
void Graph::DrawXGrid(int y, int step, int grid, BOOLEAN flag) {
    if(flag) DrawXLine(y, DomainLeft() - 1, DomainRight() + 1);
    for(int i = 0; i <= step; i++)
        DrawYLine(DomainLeft() + i * (DomainRight() - DomainLeft()) / step, y,
                  y + grid);
}
void Graph::DrawXGrid(double y, int step, int grid, BOOLEAN flag) {
    DrawXGrid(V2WY(y), step, grid, flag);
}
void Graph::DrawXGrid(int y, double from, double to, int step, int grid,
                      BOOLEAN flag) {
    if(flag) DrawXLine(y, DomainLeft() - 1, DomainRight() + 1);
    for(int i = 0; i <= step; i++)
        DrawYLine(V2WX((to - from) * i / step + from), y, y - grid);
}
void Graph::DrawXGrid(int y, double from, double to, double step, int grid,
                      BOOLEAN flag) {
    if(flag) DrawXLine(y, DomainLeft() - 1, DomainRight() + 1);
    for(double x = from; x <= to + TolZero; x += step)
        DrawYLine(V2WX(x), y, y - grid);
}
void Graph::DrawXGrid(double y, double from, double to, double step, int grid,
                      BOOLEAN flag) {
    DrawXGrid(V2WY(y), from, to, step, grid, flag);
}
// DrawXLog10Grid
void Graph::DrawXLog10Grid(int step, int grid1, int grid2, BOOLEAN flag) {
    DrawXLog10Grid(grid1 > 0 ? DomainBottom() : DomainTop(), step, grid1, grid2,
                   flag);
}
void Graph::DrawXLog10Grid(int y, int step, int grid1, int grid2,
                           BOOLEAN flag) {
    if(flag) DrawXLine(y, DomainLeft() - 1, DomainRight() + 1);
    for(int i = 0; i <= step; i++) {
        int x = DomainLeft() + i * (DomainRight() - DomainLeft()) / step;
        DrawYLine(x, y, y + grid1);
        for(int j = 1; j < 10; j++)
            DrawYLine(x
                          + (int)(log(j) / log(10)
                                  * (DomainRight() - DomainLeft()) / step),
                      y, y + grid2);
    }
}
void Graph::DrawXLog10Grid(double y, int step, int grid1, int grid2,
                           BOOLEAN flag) {
    DrawXLog10Grid(V2WY(y), step, grid1, grid2, flag);
}
void Graph::DrawXLog10Grid(int y, double from, double to, int step, int grid1,
                           int grid2, BOOLEAN flag) {
    if(flag) DrawXLine(y, DomainLeft() - 1, DomainRight() + 1);
    int dx = V2WX(to) - V2WX(from);
    for(int i = 0; i <= step; i++) {
        int x = V2WX((to - from) * i / step + from);
        DrawYLine(x, y, y + grid1);
        for(int j = 1; j < 10; j++)
            DrawYLine(x + (int)(log(j) / log(10) * dx / step), y, y + grid2);
    }
}
/*
void Graph::DrawXLog10Grid(int y, double from, double to, double step,
        int grid1, int grid2, BOOLEAN flag)
{
    if (flag) DrawXLine(y, DomainLeft()-1, DomainRight()+1);
    for (double x = from;  x <= to+TolZero;  x += step) {
        DrawYLine(V2WX(x), y, y+grid1);
        for (int j = 1;  j < 10;  j++)
            DrawYLine(V2WX(x)+(int)(step*log(j)/log(10)), y, y+grid2);
    }
}
void Graph::DrawXLog10Grid(double y, double from, double to, double step,
        int grid1, int grid2, BOOLEAN flag)
{
    DrawXLog10Grid(V2WY(y), from, to, step, grid1, grid2, flag);
}
*/

// DrawYGrid
void Graph::DrawYGrid(int step, int grid, BOOLEAN flag) {
    DrawYGrid(grid > 0 ? DomainLeft() : DomainRight(), step, grid, flag);
}
void Graph::DrawYGrid(int x, int step, int grid, BOOLEAN flag) {
    if(flag) DrawYLine(x, DomainBottom() - 1, DomainTop() + 1);
    for(int i = 0; i <= step; i++)
        DrawXLine(DomainTop() - i * (DomainTop() - DomainBottom()) / step, x,
                  x + grid);
}
void Graph::DrawYGrid(double x, int step, int grid, BOOLEAN flag) {
    DrawYGrid(V2WX(x), step, grid, flag);
}
void Graph::DrawYGrid(int x, double from, double to, int step, int grid,
                      BOOLEAN flag) {
    if(flag) DrawYLine(x, DomainBottom() - 1, DomainTop());
    for(int i = 0; i <= step; i++)
        DrawXLine(V2WY((to - from) * i / step + from), x, x + grid);
}
void Graph::DrawYGrid(int x, double from, double to, double step, int grid,
                      BOOLEAN flag) {
    if(flag) DrawYLine(x, DomainBottom() - 1, DomainTop() + 1);
    for(double y = from; y <= to + TolZero; y += step)
        DrawXLine(V2WY(y), x, x + grid);
}
void Graph::DrawYGrid(double x, double from, double to, double step, int grid,
                      BOOLEAN flag) {
    DrawYGrid(V2WX(x), from, to, step, grid, flag);
}

// DrawYLog10Grid
void Graph::DrawYLog10Grid(int step, int grid1, int grid2, BOOLEAN flag) {
    DrawYLog10Grid(grid1 > 0 ? DomainLeft() : DomainRight(), step, grid1, grid2,
                   flag);
}
void Graph::DrawYLog10Grid(int x, int step, int grid1, int grid2,
                           BOOLEAN flag) {
    if(flag) DrawYLine(x, DomainBottom() - 1, DomainTop() + 1);
    for(int i = 0; i <= step; i++) {
        int y = DomainTop() - i * (DomainTop() - DomainBottom()) / step;
        DrawXLine(y, x, x + grid1);
        for(int j = 1; j < 10; j++)
            DrawXLine(y
                          + (int)(log(j) / log(10)
                                  * (DomainTop() - DomainBottom()) / step),
                      x, x + grid2);
    }
}
void Graph::DrawYLog10Grid(double x, int step, int grid1, int grid2,
                           BOOLEAN flag) {
    DrawYLog10Grid(V2WX(x), step, grid1, grid2, flag);
}
void Graph::DrawYLog10Grid(int x, double from, double to, int step, int grid1,
                           int grid2, BOOLEAN flag) {
    if(flag) DrawYLine(x, DomainBottom() - 1, DomainTop() + 1);
    double dy = V2WY(to) - V2WY(from);
    for(int i = 0; i <= step; i++) {
        // DrawXLine(V2WY((to-from)*i/step+from), x, x+grid1);
        int y = V2WY((to - from) * i / step + from);
        DrawXLine(y, x, x + grid1);
        for(int j = 1; j < 10; j++)
            DrawXLine(y + (int)(log(j) / log(10) * dy / step), x, x + grid2);
    }
}
/*
void Graph::DrawYLog10Grid(int x, double from, double to, double step,
        int grid1, int grid2, BOOLEAN flag)
{
    if (flag) DrawYLine(x, DomainBottom()-1, DomainTop()+1);
    for (double y = from;  y <= to+TolZero;  y += step)
        DrawXLine(V2WY(y), x, x+grid1);
}
void Graph::DrawYLog10Grid(double x, double from, double to, double step,
        int grid1, int grid2, BOOLEAN flag)
{
    DrawYLog10Grid(V2WX(x), from, to, step, grid1, grid2, flag);
}
*/

// DrawGrid
void Graph::DrawGrids(int xstep, int ystep, int grid) {
    PushPoint(DomainLeft(), DomainTop());
    PushPoint(DomainRight(), DomainTop());
    PushPoint(DomainRight(), DomainBottom());
    PushPoint(DomainLeft(), DomainBottom());
    PushPoint(DomainLeft(), DomainTop());
    DrawLines();
    DrawXGrid(xstep, grid, (BOOLEAN)FALSE);
    DrawXGrid(xstep, -grid, (BOOLEAN)FALSE);
    DrawYGrid(ystep, grid, (BOOLEAN)FALSE);
    DrawYGrid(ystep, -grid, (BOOLEAN)FALSE);
}
// DrawLog10Grids
void Graph::DrawLog10Grids(int xstep, int ystep, int grid1, int grid2) {
    PushPoint(DomainLeft(), DomainTop());
    PushPoint(DomainRight(), DomainTop());
    PushPoint(DomainRight(), DomainBottom());
    PushPoint(DomainLeft(), DomainBottom());
    PushPoint(DomainLeft(), DomainTop());
    DrawLines();
    DrawXLog10Grid(xstep, grid1, grid2, (BOOLEAN)FALSE);
    DrawXLog10Grid(xstep, -grid1, -grid2, (BOOLEAN)FALSE);
    DrawYLog10Grid(ystep, grid1, grid2, (BOOLEAN)FALSE);
    DrawYLog10Grid(ystep, -grid1, -grid2, (BOOLEAN)FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Graph::Label

char *LabelR(const char *format, double x) {
    static char s[32];
    sprintf(s, format, x);
    if(atof(s) == -atof(s)) sprintf(s, format, 0.);
    return s;
}
char *LabelL(const char *format, double x) {
    char *t;
    for(t = LabelR(format, x); t[0] == ' '; t++)
        ;
    return t;
}

// DrawXLabel
void Graph::DrawXLabel(const char *format, int step, int gap) {
    DrawXLabel(gap < 0 ? DomainBottom() : DomainTop(), format, step, gap);
}
void Graph::DrawXLabel(int y, const char *format, int step, int gap) {
    DrawXLabel(y, format, ViewLeft(), ViewRight(), step, gap);
}
void Graph::DrawXLabel(int y, const char *format, double from, double to,
                       int step, int gap) {
    for(int i = 0; i <= step; i++) {
        double x = (to - from) * i / step + from;
        _DrawString_(ViewToWinX(x), y + gap,
                     TA_CENTER | (gap > 0 ? TA_TOP : TA_BOTTOM),
                     LabelL(format, x));
    }
}
void Graph::DrawXLabel(int y, const char *format, double from, double to,
                       double step, int gap) {
    for(double x = from; x <= to; x += step)
        _DrawString_(ViewToWinX(x), y + gap,
                     TA_CENTER | (gap > 0 ? TA_TOP : TA_BOTTOM),
                     LabelL(format, x));
}

// DrawYLabel
void Graph::DrawYLabel(const char *format, int step, int gap) {
    DrawYLabel(gap > 0 ? DomainLeft() : DomainRight(), format, step, gap);
}
void Graph::DrawYLabel(int x, const char *format, int step, int gap) {
    DrawYLabel(x, format, ViewBottom(), ViewTop(), step, gap);
}
void Graph::DrawYLabel(int x, const char *format, double from, double to,
                       int step, int gap) {
    for(int i = 0; i <= step; i++) {
        double y = (to - from) * i / step + from;
        _DrawString_(x - gap, ViewToWinY(y),
                     (gap > 0 ? TA_RIGHT : TA_LEFT) | TA_BASELINE,
                     LabelR(format, y));
    }
}
void Graph::DrawYLabel(int x, const char *format, double from, double to,
                       double step, int gap) {
    for(double y = from; y <= to; y += step)
        _DrawString_(x - gap, ViewToWinY(y),
                     (gap > 0 ? TA_RIGHT : TA_LEFT) | TA_BASELINE,
                     LabelR(format, y));
}

// DrawLabel
void Graph::DrawLabels(const char *format, int xstep, int ystep, int gap) {
    DrawXLabel(format, xstep, gap);
    DrawYLabel(format, ystep, gap);
}

// DrawGridAndLabels
void Graph::DrawXGridAndLabels(const char *format, int xstep) {
    DrawXGrid(xstep);
    DrawXLabel(format, xstep);
}
void Graph::DrawYGridAndLabels(const char *format, int ystep) {
    DrawYGrid(ystep);
    DrawYLabel(format, ystep);
}
void Graph::DrawGridAndLabels(const char *format, int xstep, int ystep) {
    DrawGrids(xstep, ystep);
    DrawLabels(format, xstep, ystep);
}

/////////////////////////////////////////////////////////////////////////////
// Graph::Draw

// DrawMoveTo
void Graph::MoveTo(int x, int y) {
    m_cx = x;
    m_cy = y;
}
void Graph::MoveTo(int x, double y) { MoveTo(x, ViewToWinY(y)); }
void Graph::MoveTo(double x, int y) { MoveTo(ViewToWinX(x), y); }
void Graph::MoveTo(double x, double y) { MoveTo(ViewToWinX(x), ViewToWinY(y)); }
void Graph::MoveTo(double p[]) { MoveTo(p[0], p[1]); }
void Graph::MoveTo(const Point &p) { MoveTo(p.x, p.y); }
void Graph::MoveTo(const Vector2DI &p) { MoveTo(p.x, p.y); }
void Graph::MoveTo(const Vector2DD &p) { MoveTo(p.x, p.y); }

// DrawDot
void Graph::DrawDot(int x, int y) { _DrawDot_(x, y); }
void Graph::DrawDot(int x, double y) { DrawDot(x, ViewToWinY(y)); }
void Graph::DrawDot(double x, int y) { DrawDot(ViewToWinX(x), y); }
void Graph::DrawDot(double x, double y) {
    DrawDot(ViewToWinX(x), ViewToWinY(y));
}
void Graph::DrawDot(double p[]) { DrawDot(p[0], p[1]); }
void Graph::DrawDot(const Point &p) { DrawDot(p.x, p.y); }
void Graph::DrawDot(const Vector2DI &p) { DrawDot(p.x, p.y); }
void Graph::DrawDot(const Vector2DD &p) { DrawDot(p.x, p.y); }

// DrawDots
void Graph::DrawDots(int x[], int y[], int size) {
    for(int i = 0; i < size; i++) DrawDot(x[i], y[i]);
}
void Graph::DrawDots(double y[], int size) {
    for(int i = 0; i < size; i++) DrawDot((double)i, y[i]);
}
void Graph::DrawDots(double x[], double y[], int size) {
    for(int i = 0; i < size; i++) DrawDot(x[i], y[i]);
}
void Graph::DrawDots(Vector2D *a, int size) {
    for(int i = 0; i < size; i++) DrawDot(a[i]);
}

// DrawLine
void Graph::DrawLine(int x1, int y1, int x2, int y2) {
    _DrawLine_(x1, y1, x2, y2);
}
void Graph::DrawLine(const V2DI &sp, const V2DI &ep) {
    DrawLine(sp.x, sp.y, ep.x, ep.y);
}
void Graph::DrawLine(double x1, double y1, double x2, double y2) {
    DrawLine(ViewToWinX(x1), ViewToWinY(y1), ViewToWinX(x2), ViewToWinY(y2));
}
void Graph::DrawLine(double x, double y, int dx, int dy) {
    int x1 = ViewToWinX(x), y1 = ViewToWinY(y);
    DrawLine(x1, y1, x1 + dx, y1 + dy);
}
void Graph::DrawLine(double *p1, double *p2) {
    DrawLine(ViewToWinX(p1[0]), ViewToWinY(p1[1]), ViewToWinX(p2[0]),
             ViewToWinY(p2[1]));
}
void Graph::DrawLine(const Point &sp, const Point &ep) {
    DrawLine(sp.x, sp.y, ep.x, ep.y);
}
void Graph::DrawLine(const V2D &sp, const V2D &ep) {
    DrawLine(sp.x, sp.y, ep.x, ep.y);
}
void Graph::DrawLine(double x1, double y1, double z1, double x2, double y2,
                     double z2) {
    DrawLine(RealToWin(x1, y1, z1), RealToWin(x2, y2, z2));
}

// DrawXLine
void Graph::DrawXLine(int y) { DrawLine(Left(), y, Right(), y); }
void Graph::DrawXLine(int y, int x1, int x2) { DrawLine(x1, y, x2, y); }
void Graph::DrawXLine(double y) {
    DrawLine(DomainLeft(), ViewToWinY(y), DomainRight(), ViewToWinY(y));
}
void Graph::DrawXLine(double y, double x1, double x2) {
    DrawLine(ViewToWinX(x1), ViewToWinY(y), ViewToWinX(x2), ViewToWinY(y));
}

// DrawYLine
void Graph::DrawYLine(int x) { DrawLine(x, DomainTop(), x, DomainBottom()); }
void Graph::DrawYLine(int x, int y1, int y2) { DrawLine(x, y1, x, y2); }
void Graph::DrawYLine(double x) { DrawYLine(ViewToWinX(x)); }
void Graph::DrawYLine(double x, double y1, double y2) {
    DrawLine(ViewToWinX(x), ViewToWinY(y1), ViewToWinX(x), ViewToWinY(y2));
}

// DrawLineTo
void Graph::DrawLineTo(int x, int y) { DrawLine(m_cx, m_cy, x, y); }
void Graph::DrawLineTo(int x, double y) { DrawLineTo(x, ViewToWinY(y)); }
void Graph::DrawLineTo(double x, int y) { DrawLineTo(ViewToWinX(x), y); }
void Graph::DrawLineTo(double x, double y) {
    DrawLineTo(ViewToWinX(x), ViewToWinY(y));
}
void Graph::DrawLineTo(const V2D &pt) {
    DrawLineTo(ViewToWinX(pt.x), ViewToWinY(pt.y));
}

//
void Graph::DrawLine(double a, double b, double c) {
    if(fabs(b) > fabs(a)) {
        double x1 = ViewLeft();
        double y1 = -(c + a * x1) / b;
        double x2 = ViewRight();
        double y2 = -(c + a * x2) / b;
        if(y1 > y2) {
            ::Swap(x1, x2);
            ::Swap(y1, y2);
        }
        if(y1 > ViewTop() || y2 < ViewBottom()) return;
        if(y1 < ViewBottom()) {
            y1 = ViewBottom();
            x1 = -(c + b * y1) / a;
        }
        if(y2 > ViewTop()) {
            y2 = ViewTop();
            x2 = -(c + b * y2) / a;
        }
        DrawLine(x1, y1, x2, y2);
    } else {
        double y1 = ViewTop();
        double x1 = -(c + b * y1) / a;
        double y2 = ViewBottom();
        double x2 = -(c + b * y2) / a;
        if(x1 > x2) {
            ::Swap(x1, x2);
            ::Swap(y1, y2);
        }
        if(x1 > ViewRight() || x2 < ViewLeft()) return;
        if(x1 < ViewLeft()) {
            x1 = ViewLeft();
            y1 = -(c + a * x1) / b;
        }
        if(x2 > ViewRight()) {
            x2 = ViewRight();
            y2 = -(c + a * x2) / b;
        }
        DrawLine(x1, y1, x2, y2);
    }
}

// DrawArrow
void Graph::DrawArrow(int x1, int y1, int x2, int y2, int a) {
    DrawLine(x1, y1, x2, y2);
    /*
        if (a < 2) return;
        const double cos15 = cos(20*M_PI/180);
        const double sin15 = sin(20*M_PI/180);
        double u = x2-x1, v = y2-y1;
        double r = sqrt(u*u+v*v);
        if (r < TolZero) return;
        u *= a / r;  v *= a / r;
        double uc = u*cos15, vc = v*cos15;
        double us = u*sin15, vs = v*sin15;
        int vs_uc = vs > uc ? (int)floor(vs-uc+0.9) : -(int)floor(uc-vs+0.9);
        int us_vc = us > vc ? (int)floor(us-vc+0.9) : -(int)floor(vc-us+0.9);
        DrawLine(x2+vs_uc, y2-(int)floor(vc+us+0.9), x2, y2);
        DrawLine(x2-(int)floor(uc+vs+0.9), y2+us_vc, x2, y2);
    */
    double theta = atan2(y1 - y2, x1 - x2);
    double dtheta = 30 * M_PI / 180;
    double dx, dy;
    dx = a * cos(theta + dtheta);
    dy = a * sin(theta + dtheta);
    PushPoint(x2 + (int)(dx > 0 ? ceil(dx) : -(int)ceil(-dx)),
              y2 + (int)(dy > 0 ? ceil(dy) : -(int)ceil(-dy)));
    PushPoint(x2, y2);
    dx = a * cos(theta - dtheta);
    dy = a * sin(theta - dtheta);
    PushPoint(x2 + (int)(dx > 0 ? ceil(dx) : -(int)ceil(-dx)),
              y2 + (int)(dy > 0 ? ceil(dy) : -(int)ceil(-dy)));
    DrawLines();
}
void Graph::DrawArrow(double x1, double y1, double x2, double y2, int a) {
    DrawArrow(ViewToWinX(x1), ViewToWinY(y1), ViewToWinX(x2), ViewToWinY(y2),
              a);
}
void Graph::DrawArrow(double x, double y, int u, int v, int a) {
    int wx = ViewToWinX(x), wy = ViewToWinY(y);
    DrawArrow(wx, wy, wx + u, wy + v, a);
}

void Graph::DrawArrow(int x, int y, double r, double a, int d) {
    double x2 = x + r * cos(a), y2 = y - r * sin(a);
    DrawLine(x, y, (int)x2, (int)y2);
    double da = M_PI / 6;
    PushPoint((int)(x2 - d * cos(a + da) + 0.5),
              (int)(y2 + d * sin(a + da) + 0.5));
    PushPoint((int)x2, (int)y2);
    PushPoint((int)(x2 - d * cos(a - da) + 0.5),
              (int)(y2 + d * sin(a - da) + 0.5));
    DrawLines();
}

// DrawLines
void Graph::DrawLines(int *x, int *y, int size) {
    if(size <= 0) return;
    m_pts.Alloc(size);
    m_pts[0].x = x[0];
    m_pts[0].y = y[0];
    for(int i = 1; i < size; i++) {
        m_pts[i].x = x[i];
        m_pts[i].y = y[i];
    }
    _DrawLines_(m_pts, size);
    m_pts.SetSize(0);
}
void Graph::DrawLines(IVector &x, IVector &y) {
    DrawLines(x(), y(), ::GetMin(x.Size(), y.Size()));
}
void Graph::DrawLines(double *x, double *y, int size) {
    if(size <= 0) return;
    m_pts.Alloc(size);
    m_pts[0].x = ViewToWinX(x[0]);
    m_pts[0].y = ViewToWinY(y[0]);
    for(int i = 1; i < size; i++) {
        m_pts[i].x = ViewToWinX(x[i]);
        m_pts[i].y = ViewToWinY(y[i]);
    }
    _DrawLines_(m_pts, size);
    m_pts.SetSize(0);
}
void Graph::DrawLines(Vector &x, Vector &y) {
    DrawLines(x(), y(), ::GetMin(x.Size(), y.Size()));
}
void Graph::DrawLines(Point *lpPoints, int nCount) {
    _DrawLines_(lpPoints, nCount);
}
void Graph::DrawLines(BOOLEAN bPop) {
    if(m_pts.IsEmpty()) return;
    _DrawLines_(m_pts, m_pts.Size());
    if(bPop) PopPoint(0);
}
void Graph::DrawLines(ColorEnum color) {
    _SetForeground_(GetRGB(color));
    DrawLines();
}

// DrawXLines
void Graph::DrawXLines(double *x, int size) {
    if(size > 0) MoveTo(x[0], 0.);
    for(int i = 1; i < size; i++) DrawLineTo(x[i], (double)i);
}
void Graph::DrawXLines(Vector &v) {
    if(v.Size() > 0) MoveTo(v.First(), (double)v.From());
    for(int i = v.From() + 1; i <= v.To(); i++)
        DrawLineTo(v.m_buf[i], (double)i);
}

// DrawYLines
void Graph::DrawYLines(double *y, int size) {
    if(size > 0) MoveTo(0., y[0]);
    for(int i = 1; i < size; i++) DrawLineTo((double)i, y[i]);
}
void Graph::DrawYLines(Vector &v) {
    if(v.Size() > 0) MoveTo((double)v.From(), v.First());
    for(int i = v.From() + 1; i <= v.To(); i++)
        DrawLineTo((double)i, v.m_buf[i]);
}

// DrawRectangle
void Graph::DrawRectangle(int x1, int y1, int x2, int y2) {
    _DrawRectangle_(x1, y1, x2, y2);
}
void Graph::DrawRectangle(const Box2DI &rect) {
    DrawRectangle(rect.MinX(), rect.MinY(), rect.MaxX(), rect.MaxY());
}

/*
void Graph::DrawRectangle(double x1, double y1, int x2, int y2)
{
    DrawRectangle(m_domain.MinX()+ViewToWinX(x1),
                    m_domain.MinY()+ViewToWinY(y1),
                    m_domain.MinX()+x2,
                    m_domain.MinY()+y2);
}
*/

void Graph::DrawRectangle(double x1, double y1, double x2, double y2) {
    DrawRectangle(ViewToWinX(x1), ViewToWinY(y1), ViewToWinX(x2),
                  ViewToWinY(y2));
}
void Graph::DrawRectangle(const Box2DD &rect) {
    DrawRectangle(rect.MinX(), rect.MinY(), rect.MaxX(), rect.MaxY());
}

void Graph::DrawEllipse(int x, int y, int width, int height) {
    _DrawEllipse_(x, y, width, height);
}

void Graph::DrawEllipse(double x, double y, int width, int height) {
    DrawEllipse(ViewToWinX(x), ViewToWinY(y), width, height);
}
/*
void Graph::DrawEllipse(double x, double y, double width, double height) {
    DrawEllipse(ViewToWinX(x), ViewToWinY(y), xscale(width), yscale(height));
}
*/

void Graph::DrawHoneycomb(int x, int y, int r) {
    int h = (int)ceil(r * sqrt(3) / 2.);
    MoveTo(x, y - r);
    DrawLineTo(x + h, y - r / 2);
    DrawLineTo(x + h, y + r / 2);
    DrawLineTo(x, y + r);
    DrawLineTo(x - h, y + r / 2);
    DrawLineTo(x - h, y - r / 2);
    DrawLineTo(x, y - r);
}

void Graph::DrawStar(int x, int y, int r) {
    MoveTo(x, y - r);
    for(int i = 0; i <= 5; i++)
        DrawLineTo(x + (int)(r * sin(i * 144 * M_PI / 180)),
                   y - (int)(r * cos(i * 144 * M_PI / 180)));
}

void Graph::DrawCross(int x, int y, int r) {
    DrawLine(x - r, y, x + r, y);
    DrawLine(x, y - r, x, y + r);
}

void Graph::DrawTimes(int x, int y, int r) {
    DrawLine(x - r, y - r, x + r, y + r);
    DrawLine(x + r, y - r, x - r, y + r);
}

void Graph::DrawLozenge(int x, int y, int w, int h) {
    /*
        DrawLine(x, y-h, x+w, y);
        DrawLineTo(x, y+h);
        DrawLineTo(x-w, y);
        DrawLineTo(x, y-h);
    */
    PushPoint(x, y - h);
    PushPoint(x + w, y);
    PushPoint(x, y + h);
    PushPoint(x - w, y);
    PushPoint(x, y - h);
    DrawLines();
}

void Graph::DrawAstral(int x, int y, int r) {
    DrawLine(x - r, y, x + r, y);
    int dx = (int)r / 2;
    int dy = (int)ceil(r * sqrt(3) / 2);
    DrawLine(x - dx, y - dy, x + dx, y + dy);
    DrawLine(x - dx, y + dy, x + dx, y - dy);
}

// FillRectangle
void Graph::FillRectangle(int x1, int y1, int x2, int y2) {
    _FillRectangle_(x1, y1, x2, y2);
}
void Graph::FillRectangle(const Box2DI &rect) {
    FillRectangle(rect.MinX(), rect.MinY(), rect.MaxX(), rect.MaxY());
}
void Graph::FillRectangle(double x1, double y1, double x2, double y2) {
    FillRectangle(ViewToWinX(x1), ViewToWinY(y1), ViewToWinX(x2),
                  ViewToWinY(y2));
}
void Graph::FillRectangle(double x, double y, int w, int h) {
    int wx = ViewToWinX(x);
    int wy = ViewToWinY(y);
    FillRectangle(wx - (w - 1) / 2, wy - (h - 1) / 2, wx + w / 2, wy + h / 2);
}
void Graph::FillRectangle(const Box2DD &rect) {
    FillRectangle(rect.MinX(), rect.MinY(), rect.MaxX(), rect.MaxY());
}

// FillRectangle
void Graph::FillCircle(int x, int y, int r) { _FillCircle_(x, y, r); }
void Graph::FillCircle(double x, double y, int r) {
    FillCircle(ViewToWinX(x), ViewToWinY(y), r);
}
void Graph::FillCircle(const Vector2DI &p, int r) { FillCircle(p.x, p.y, r); }
void Graph::FillCircle(const Vector2D &p, int r) {
    FillCircle(ViewToWinX(p.x), ViewToWinY(p.y), r);
}
void Graph::FillSquare(int x, int y, int r) {
    FillRectangle(x - r, y - r, x + r, y + r);
}

void Graph::DrawTriangle(int x, int y, int r) {
    int dx = (int)(r * sqrt(3) / 2);
    PushPoint(x, y - r);
    PushPoint(x + dx, y + r / 2);
    PushPoint(x - dx, y + r / 2);
    PushPoint(x, y - r);
    DrawLines();
}
void Graph::DrawSquare(int x, int y, int r) {
    int d = r / 2;
    PushPoint(x - d, y - d);
    PushPoint(x + d, y - d);
    PushPoint(x + d, y + d);
    PushPoint(x - d, y + d);
    PushPoint(x - d, y - d);
    DrawLines();
}

void Graph::FillHoneycomb(int x, int y, int r) {
    for(int i = (int)ceil(r * sqrt(3) / 2.); i > 0; i--) {
        DrawLine(x - i, y - r + i / 2, x - i, y + r - (i - 1) / 2);
        DrawLine(x + i, y - r + i / 2, x + i, y + r - (i - 1) / 2);
    }
    DrawLine(x, y - r, x, y + r);
}

// FillPolygon
BOOLEAN Graph::FillPolygon(Point *lpPoints, int nCount) {
    return _FillPolygon_(lpPoints, nCount);
}
BOOLEAN Graph::FillPolygon(BOOLEAN bPop) {
    BOOLEAN result = _FillPolygon_(m_pts, m_pts.Size());
    if(bPop) PopPoint(0);
    return result;
}

/////////////////////////////////////////////////////////////////////////////
// Graph::Grid3D

/*
inline int ball(double x) {
    return (int)x==x ? (x>0?(int)x+1:(int)x-1) : (x>0?(int)x+2:(int)x-2);
}

void Graph::DrawXGrid3D(int step, int grid)
{
    DrawXGrid3D(grid>0?m_dy1:m_dy2, m_dz1, step, grid);
}
void Graph::DrawXGrid3D(double y, double z, int step, int grid)
{
    DrawXGrid3D(y, z, m_dx1, m_dx2, step, grid);
}
void Graph::DrawXGrid3D(double y, double z, double from, double to, int step,
int grid)
{
    double r = sqrt(m_xr*m_xr+m_yr*m_yr*m_zr*m_zr);
    for (int i = 0;  i <= step;  i++) {
        double x = (to-from)*i/step+from;
        int x1 = RealToWinX(x, y, z);   int y1 = RealToWinY(x, y, z);
        int x2 = x1+ball(grid*m_xr/r)+1;  int y2 = y1-(int)(grid*m_yr*m_zr/r);
        DrawLine(x1, y1, x2, y2);
    }
}
void Graph::DrawXGrid3D(double y, double z, double from, double to, double step,
int grid) { double r = sqrt(m_xr*m_xr+m_yr*m_yr*m_zr*m_zr); for (double x =
from;  x <= to;  x += step) { int x1 = RealToWinX(x, y, z);          int y1 =
RealToWinY(x, y, z); int x2 = x1+ball(grid*m_xr/r)+1;  int y2 =
y1-(int)(grid*m_yr*m_zr/r); DrawLine(x1, y1, x2, y2);
    }
}

void Graph::DrawYGrid3D(int step, int grid)
{
    DrawYGrid3D(grid > 0 ? m_dx1 : m_dx2, m_dz1, step, grid);
}
void Graph::DrawYGrid3D(double x, double z, int step, int grid)
{
    DrawYGrid3D(x, z, m_dy1, m_dy2, step, grid);
}
void Graph::DrawYGrid3D(double x, double z, double from, double to, int step,
int grid) { double r = sqrt(m_yr*m_yr+m_xr*m_xr*m_zr*m_zr); for (int i = 0;  i
<= step;  i++) { double y = (to-from)*i/step+from; int x1 = RealToWinX(x, y, z);
int y1 = RealToWinY(x, y, z); int x2 = x1+ball(grid*m_yr/r)+1;  int y2 =
y1+(int)(grid*m_xr*m_zr/r); DrawLine(x1, y1, x2, y2);
    }
}
void Graph::DrawYGrid3D(double x, double z, double from, double to, double step,
int grid) { double r = sqrt(m_yr*m_yr+m_xr*m_xr*m_zr*m_zr); for (double y =
from;  y <= to;  y += step) { int x1 = RealToWinX(x, y, z);          int y1 =
RealToWinY(x, y, z); int x2 = x1+ball(grid*m_yr/r)+1;  int y2 =
y1+(int)(grid*m_xr*m_zr/r); DrawLine(x1, y1, x2, y2);
    }
}

void Graph::DrawZGrid3D(int step, int grid)
{
    DrawZGrid3D(m_dx1, m_dy1, step, grid);
}
void Graph::DrawZGrid3D(double x, double y, int step, int grid)
{
    DrawZGrid3D(x, y, m_dy1, m_dy2, step, grid);
}
void Graph::DrawZGrid3D(double x, double y, double from, double to, int step,
int grid) { for (int i = 0;  i <= step;  i++) { double z =
(to-from)*i/step+from; int x1 = RealToWinX(x, y, z);  int y1 = RealToWinY(x, y,
z); int x2 = x1+grid;       int y2 = y1; DrawLine(x1, y1, x2, y2);
    }
}
void Graph::DrawZGrid3D(double x, double y, double from, double to, double step,
int grid) { for (double z = from;  z <= to;  z += step) { int x1 = RealToWinX(x,
y, z);  int y1 = RealToWinY(x, y, z); int x2 = x1+grid;       int y2 = y1;
        DrawLine(x1, y1, x2, y2);
    }
}

void Graph::DrawGrid3D(int xstep, int ystep, int zstep, int grid) {
    MoveTo(RealToWinX(m_dx1, m_dy1, m_dz1), RealToWinY(m_dx1, m_dy1, m_dz1));
    DrawLineTo(RealToWinX(m_dx2, m_dy1, m_dz1),
                        RealToWinY(m_dx2, m_dy1, m_dz1));
    DrawLineTo(RealToWinX(m_dx2, m_dy2, m_dz1),
                        RealToWinY(m_dx2, m_dy2, m_dz1));
    DrawLineTo(RealToWinX(m_dx1, m_dy2, m_dz1),
                        RealToWinY(m_dx1, m_dy2, m_dz1));
    DrawLineTo(RealToWinX(m_dx1, m_dy1, m_dz1),
                        RealToWinY(m_dx1, m_dy1, m_dz1));
    MoveTo(RealToWinX(m_dx1, m_dy1, m_dz1),RealToWinY(m_dx1, m_dy1, m_dz1));
    DrawLineTo(RealToWinX(m_dx1, m_dy1, m_dz2),
                        RealToWinY(m_dx1, m_dy1, m_dz2));
    DrawXGrid3D(m_dy1, m_dz1,
        m_dx1+(m_dx2-m_dx1)/xstep, m_dx2-(m_dx2-m_dx1)/xstep, xstep-2, grid);
    DrawXGrid3D(m_dy2, m_dz1,
        m_dx1+(m_dx2-m_dx1)/xstep, m_dx2-(m_dx2-m_dx1)/xstep, xstep-2, -grid);
    DrawYGrid3D(m_dx1, m_dz1,
        m_dy1+(m_dy2-m_dy1)/ystep, m_dy2-(m_dy2-m_dy1)/ystep, ystep-2, grid);
    DrawYGrid3D(m_dx2, m_dz1,
        m_dy1+(m_dy2-m_dy1)/ystep, m_dy2-(m_dy2-m_dy1)/ystep, ystep-2, -grid);
    DrawZGrid3D(m_dx1, m_dy1, m_dz1+(m_dz2-m_dz1)/zstep, m_dz2, zstep-1, grid);
}

/////////////////////////////////////////////////////////////////////////////
// Graph::Label3D

void Graph::DrawXLabel3D(char *format, int step, int gap) {
    DrawXLabel3D(gap>0?m_dy1:m_dy2, m_dz1, format, step, gap);
}
void Graph::DrawXLabel3D(double y, double z, char *format, int step, int gap) {
    DrawXLabel3D(y, z, format, m_dx1, m_dx2, step, gap);
}
void Graph::DrawXLabel3D(double y, double z, char *format, double from, double
to, int step, int gap) { UINT align = gap > 0 ? (TA_CENTER | TA_TOP) : (TA_LEFT
| TA_BOTTOM); for (int i = 0;  i <= step ;  i++) { double x =
(to-from)*i/step+from; int x1 = RealToWinX(x, y, z);  int y1 = RealToWinY(x, y,
z)+gap; DrawString(x1, y1, align, LabelL(format, x));
    }
}
void Graph::DrawXLabel3D(double y, double z, char *format, double from, double
to, double step, int gap) { UINT align = gap > 0 ? (TA_CENTER | TA_TOP) :
(TA_LEFT | TA_BOTTOM); for (double x = from;  x<=to && x<=m_dx2;  x += step)
        DrawString(m_x1, m_y1, align,LabelL(format, x));
}

void Graph::DrawYLabel3D(char *format, int step, int gap) {
    DrawYLabel3D(gap>0?m_dx1:m_dx2, m_dz1, format, step, gap);
}
void Graph::DrawYLabel3D(double x, double z, char *format, int step, int gap) {
    DrawYLabel3D(x, z, format, m_dy1, m_dy2, step, gap);
}
void Graph::DrawYLabel3D(double x, double z, char *format, double from, double
to, int step, int gap) { UINT align = gap > 0 ? (TA_RIGHT | TA_BOTTOM) :
(TA_LEFT | TA_TOP); double r = sqrt(m_yr*m_yr+m_xr*m_xr*m_zr*m_zr); for (int i =
0;  i <= step ;  i++) { double y = (to-from)*i/step+from; int x1 = RealToWinX(x,
y, z)-ball(gap*m_yr/r); int y1 = RealToWinY(x, y, z)+(int)(gap*m_xr*m_zr/r);
        DrawString(x1, y1, align, LabelL(format, y));
    }
}
void Graph::DrawYLabel3D(double x, double z, char *format, double from, double
to, double step, int gap){ UINT align = gap > 0 ? (TA_RIGHT | TA_BOTTOM) :
(TA_LEFT | TA_TOP); double r = sqrt(m_xr*m_xr+m_yr*m_yr*m_zr*m_zr); for (double
y = from;  y<=to && y<=m_dy2;  y += step) { int x1 = RealToWinX(x, y,
z)-ball(gap*m_yr/r); int y1 = RealToWinY(x, y, z)+(int)(gap*m_xr*m_zr/r);
        DrawString(x1, y1, align, LabelL(format, y));
    }
}

void Graph::DrawZLabel3D(char *format, int step, int gap) {
    DrawZLabel3D(m_dx1, m_dy1, format, step, gap);
}
void Graph::DrawZLabel3D(double x, double y, char *format, int step, int gap) {
    DrawZLabel3D(x, y, format, m_dz1, m_dz2, step, gap);
}
void Graph::DrawZLabel3D(double x, double y, char *format, double from, double
to, int step, int gap)
{
    for (int i = 0;  i <= step ;  i++) {
        double z = (to-from)*i/step+from;
        int x1 = RealToWinX(x, y, z)-gap;  int y1 = RealToWinY(x, y, z);
        DrawString(x1, y1, TA_RIGHT | TA_CENTER,  LabelL(format, z));
    }
}
void Graph::DrawZLabel3D(double x, double y, char *format, double from, double
to, double step, int gap)
{
    for (double z = from;  z<=to && z<=m_dz2;  z += step) {
        int x1 = RealToWinX(x, y, z)-gap;  int y1 = RealToWinY(x, y, z);
        DrawString(x1, y1, TA_RIGHT | TA_CENTER, LabelL(format, z));
    }
}
*/

void Graph::MoveTo3D(double x, double y, double z) {
    MoveTo(RealToViewX(x, y, z), RealToViewY(x, y, z));
}

void Graph::DrawDot3D(double x, double y, double z) {
    DrawDot(RealToWinX(x, y, z), RealToWinY(x, y, z));
}

void Graph::DrawDots3D(double *x, double *y, double *z, int size) {
    for(int i = 0; i < size; i++) DrawDot3D(x[i], y[i], z[i]);
}
void Graph::DrawDots3D(Vector &x, Vector &y, Vector &z) {
    DrawDots3D(x(), y(), z(), GetMin(x.Size(), y.Size(), z.Size()));
}

void Graph::DrawLine3D(double x1, double y1, double z1, double x2, double y2,
                       double z2) {
    DrawLine(RealToWinX(x1, y1, z1), RealToWinY(x1, y1, z1),
             RealToWinX(x2, y2, z2), RealToWinY(x2, y2, z2));
}

void Graph::DrawLineTo3D(double x, double y, double z) {
    DrawLineTo(RealToWinX(x, y, z), RealToWinY(x, y, z));
}

void Graph::DrawLines3D(double *x, double *y, double *z, int size) {
    if(size > 0) MoveTo3D(x[0], y[0], z[0]);
    for(int i = 1; i < size; i++) DrawLineTo3D(x[i], y[i], z[i]);
}
void Graph::DrawLines3D(Vector &x, Vector &y, Vector &z) {
    DrawLines3D(x(), y(), z(), GetMin(x.Size(), y.Size(), z.Size()));
}

void Graph::DrawCubic3D(double x1, double y1, double z1, double x2, double y2,
                        double z2) {
    DrawLine3D(x1, y1, z1, x2, y1, z1);
    DrawLine3D(x1, y1, z1, x1, y2, z1);
    DrawLine3D(x1, y1, z1, x1, y1, z2);
    DrawLine3D(x2, y1, z2, x1, y1, z2);
    DrawLine3D(x2, y1, z2, x2, y2, z2);
    DrawLine3D(x2, y1, z2, x2, y1, z1);
    DrawLine3D(x2, y2, z1, x1, y2, z1);
    DrawLine3D(x2, y2, z1, x2, y1, z1);
    DrawLine3D(x2, y2, z1, x2, y2, z2);
    DrawLine3D(x1, y2, z2, x2, y2, z2);
    DrawLine3D(x1, y2, z2, x1, y1, z2);
    DrawLine3D(x1, y2, z2, x1, y2, z1);
}

void Graph::DrawArrow3D(double x1, double y1, double z1, double x2, double y2,
                        double z2, int a) {
    DrawArrow(RealToWinX(x1, y1, z1), RealToWinY(x1, y1, z1),
              RealToWinX(x2, y2, z2), RealToWinY(x2, y2, z2), a);
}

/////////////////////////////////////////////////////////////////////////////
// Graph::DrawFunction

void Graph::DrawFunction(double (*func)(double)) {
    ColorEnum color[] = {
        RedColor,       BlueColor,        GreenColor,    DarkYelloColor,
        MagentaColor,   DarkCyanColor,    DarkRedColor,  DarkBlueColor,
        DarkGreenColor, DarkMagentaColor, DarkGrayColor,
    };
    _SetForeground_(GetRGB(color[m_nColor % 10]));
    MoveTo(ViewLeft(), func(ViewLeft()));
    for(int i = DomainLeft() + 1; i <= DomainRight(); i++)
        DrawLineTo(i, ViewToWinY(func(WinToViewX(i))));
    m_nColor++;
}

void Graph::DrawFunction(double (*func)(double), ColorEnum color) {
    _SetForeground_(GetRGB(color));
    MoveTo(ViewLeft(), func(ViewLeft()));
    for(int i = DomainLeft() + 1; i <= DomainRight(); i++)
        DrawLineTo(i, ViewToWinY(func(WinToViewX(i))));
}

void Graph::DrawFunction(double (*func)(double), const char *title) {
    DrawFunction(func);
    DrawXLine(DomainTop() + m_nTitle * 15 + 10, DomainRight() - 100,
              DomainRight() - 70);
    _DrawString_(DomainRight() - 60, DomainTop() + m_nTitle * 15 + 10,
                 TA_LEFT | TA_BASELINE, title);
    m_nTitle++;
}
