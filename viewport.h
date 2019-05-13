// Toy/viewport.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_VIEWPORT_H_
#define _TOY_VIEWPORT_H_

#ifdef TOY
#include <Toy/box.h>
#include <Toy/vector.h>
#else
#include "box.h"
#include "vector.h"
#endif

class Canvas;

class Viewport {
  public:
    Vector3D m_viewpoint; // viewpoint
    Vector3D m_rx,        // X Win Vector
        m_ry,             // Y Win Vector
        m_rz;             // Z Win Vector
    Vector3D m_scale;     // current scale

    Box3DD m_real;   // range in real coord.
    Box2DD m_view;   // range in view coord.
    Box2DI m_domain; // range in domain coord.
    Box2DI m_window; // range in window coord.

    Viewport();
    Viewport(int width, int height);
#ifdef __AFX_H__
    Viewport(const CRect &rect);
    void Init(const CRect &rect) {
        Init(rect.left, rect.top, rect.right, rect.bottom);
    }
#endif
    void Init(int left, int top, int right, int bottom);
    void Init(int width, int height) { Init(0, 0, width - 1, height - 1); }

    void operator=(const Viewport &viewport);
    void operator=(const Canvas &canvas);

    Box3DD &Real() { return m_real; }
    Box2DD &View() { return m_view; }
    Box2DI &Domain() { return m_domain; }
    Box2DI &Win() { return m_window; }
    Box2DI &Rect() { return m_window; }
    Vector3D &Viewpoint() { return m_viewpoint; }

    //
    int Left() { return m_window.left; }
    int Right() { return m_window.right; }
    int Top() { return m_window.bottom; }
    int Bottom() { return m_window.top; }
    int Width() { return m_window.right - m_window.left + 1; }
    int Height() { return m_window.top - m_window.bottom + 1; }
    //
    double ViewLeft() { return m_view.left; }
    double ViewRight() { return m_view.right; }
    double ViewTop() { return m_view.top; }
    double ViewBottom() { return m_view.bottom; }
    double ViewWidth() { return m_view.right - m_view.left; }
    double ViewHeight() { return m_view.top - m_view.bottom; }
    //
    int DomainLeft() { return m_domain.left; }
    int DomainRight() { return m_domain.right; }
    int DomainTop() { return m_domain.top; }
    int DomainBottom() { return m_domain.bottom; }
    int DomainMidX() { return (m_domain.right + m_domain.left) / 2; }
    int DomainMidY() { return (m_domain.bottom + m_domain.top) / 2; }
    int DomainWidth() { return m_domain.right - m_domain.left + 1; }
    int DomainHeight() { return m_domain.top - m_domain.bottom + 1; }

    //
    BOOLEAN IsInWindow(int x, int y) { return m_window.IsInDomain(x, y); }
    BOOLEAN IsInDomain(int x, int y) { return m_domain.IsInDomain(x, y); }
    BOOLEAN IsInView(double x, double y) { return m_view.IsInDomain(x, y); }

    //
    void SetVTRM(BOOLEAN bNormalize = TRUE);
    void SetVTRM(double x, double y, double z, BOOLEAN bNormalize = TRUE);
    void SetVTRM(const Vector3D &v, BOOLEAN bNormalize = TRUE) {
        SetVTRM(v.x, v.y, v.z);
    }

    void SetDomain(int x1, int y1, int x2, int y2);
#ifdef __AFX_H__
    void SetDomain(const CRect &rect) {
        SetDomain(rect.left, rect.top, rect.right, rect.bottom);
    }
#endif

    void SetXRange(double x1, double x2);
    void SetXRange(double x1, double x2, double order);
    void SetXRange(const Vector &vx, double order);
    void SetYRange(double y1, double y2);
    void SetYRange(double y1, double y2, double order);
    void SetYRange(const Vector &vy, double order);
    void SetRange(double x1, double y1, double x2, double y2);
    void SetRange(double x1, double y1, double x2, double y2, double order);
    void SetRange(const Vector &v);
    void SetRange(const Vector &v, double order);
    void SetRange(const Vector &vx, const Vector &vy);
    void SetRange(const Vector &vx, const Vector &vy, double order);
    template <class type> void SetRange(const Box2D<type> range) {
        SetXRange((double)range.MinX(), (double)range.MaxX());
        SetYRange((double)range.MinY(), (double)range.MaxY());
    }
    void SetRangeScroll(double dx, double dy) {
        SetXRange(m_view.MinX() + dx, m_view.MaxX() + dx);
        SetYRange(m_view.MinY() + dy, m_view.MaxY() + dy);
    }
    void SetRangeExtend(double dx, double dy) {
        SetXRange(m_view.MinX() - dx, m_view.MaxX() + dx);
        SetYRange(m_view.MinY() - dy, m_view.MaxY() + dy);
    }
    void ResetRange() {
        SetXRange(m_view.MinX(), m_view.MaxX());
        SetYRange(m_view.MinY(), m_view.MaxY());
    }

    void ResetWinToView();

    void SetRange3D(double min, double max);
    void SetRange3D(double minX, double maxX, double minY, double maxY,
                    double minZ, double maxZ);
    void SetRange3D(const Box3DD &mb);
    void SetRange3D(const Vector3DD &v);
#ifdef __AFX_H__
    BOOLEAN CalcCenterAndScale(CPoint sp, CPoint ep, int tolerance);
    BOOLEAN CalcCenterAndClip(CPoint dp, int tolerance);
#endif
    void SetIsoScale();
    BOOLEAN IsZView();

    BOOLEAN IsVisibleRealBox(const Box3DD &real);
    BOOLEAN IsVisibleViewBox(const Box2DD &view);

    // Real to view coord.
    //
    double R2VX(double x, double y, double z);
    double R2VX(const double v[]);
    double R2VX(const Vector3D &v);
    double RealToViewX(double x, double y, double z);
    double RealToViewX(const double v[]);
    double RealToViewX(const Vector3D &v);
    //
    double R2VY(double x, double y, double z);
    double R2VY(const double v[]);
    double R2VY(const Vector3D &v);
    double RealToViewY(double x, double y, double z);
    double RealToViewY(const double v[]);
    double RealToViewY(const Vector3D &v);
    //
    double R2VZ(double x, double y, double z);
    double R2VZ(const double v[]);
    double R2VZ(const Vector3D &v);
    double RealToViewZ(double x, double y, double z);
    double RealToViewZ(const double v[]);
    double RealToViewZ(const Vector3D &v);
    //
    void R2V(double x, double y, double z, double *vx, double *vy);
    void R2V(const Vector3D &r, double *vx, double *vy);
    void R2V(double x, double y, double z, double *vx, double *vy, double *vz);
    V2D R2V(double x, double y, double z);
    void R2V(const Vector3D &r, double *vx, double *vy, double *vz);
    void R2V(const Box3DD &r, Box2DD &v);
    void R2V(const Box3DD &r, Box3DD &v);
    void RealToView(double x, double y, double z, double *vx, double *vy);
    void RealToView(const Vector3D &r, double *vx, double *vy);
    void RealToView(double x, double y, double z, double *vx, double *vy,
                    double *vz);
    V2D RealToView(double x, double y, double z);
    void RealToView(const Vector3D &v, double *vx, double *vy, double *vz);
    void RealToView(const Box3DD &r, Box2DD &v);
    void RealToView(const Box3DD &r, Box3DD &v);

    // View to window coord.
    //
    int V2WX(double vx);
    int ViewToWinX(double vx);
    //
    int V2WY(double vy);
    int ViewToWinY(double vy);
    //
    template <class Type> void V2W(double vx, double vy, Type *wx, Type *wy) {
        *wx = (Type)V2WX(vx);
        *wy = (Type)V2WY(vy);
    }
    V2DI V2W(double vx, double vy);
    void V2W(double vx, double vy, Point &w);
    void ViewToWin(double vx, double vy, int *wx, int *wy);
    V2DI ViewToWin(double vx, double vy);
    void ViewToWin(double vx, double vy, Point &w);

    // Real to window coord.
    //
    int R2WX(double x, double y, double z);
    int R2WX(const double v[]);
    int R2WX(const Vector3D &v);
    int RealToWinX(double x, double y, double z);
    int RealToWinX(const double v[]);
    int RealToWinX(const Vector3D &v);
    //
    int R2WY(double x, double y, double z);
    int R2WY(const double v[]);
    int R2WY(const Vector3D &v);
    int RealToWinY(double x, double y, double z);
    int RealToWinY(const double v[]);
    int RealToWinY(const Vector3D &v);
    //
    template <class Type>
    void R2W(double x, double y, double z, Type *wx, Type *wy) {
        *wx = (Type)R2WX(x, y, z);
        *wy = (Type)R2WY(x, y, z);
    }
    V2DI R2W(double x, double y, double z);
    void R2W(const Vector3D &r, int *vx, int *vy);
    void R2W(double x, double y, double z, Point &w);
    void R2W(const Vector3D &r, Point &w);
    void RealToWin(double x, double y, double z, int *vx, int *vy);
    V2DI RealToWin(double x, double y, double z);
    void RealToWin(const Vector3D &v, int *vx, int *vy);
    void RealToWin(double x, double y, double z, Point &w);
    void RealToWin(const Vector3D &r, Point &w);
    ClipCode R2WClip(const Vector3D &v, int *vx, int *vy);
    ClipCode RealToWinClip(const Vector3D &v, int *vx, int *vy);

    double V2WScaleX() { return m_scale.x; }
    double ViewToWinScaleX() { return m_scale.x; }
    double V2WScaleY() { return m_scale.y; }
    double ViewToWinScaleY() { return m_scale.y; }
    double W2VScaleX() { return 1. / m_scale.x; }
    double WinToViewScaleX() { return 1. / m_scale.x; }
    double W2VScaleY() { return 1. / m_scale.y; }
    double WinToViewScaleY() { return 1. / m_scale.y; }

    // Window to view coord.
    //
    double W2VX(int vx);
    double WinToViewX(int vx);
    //
    double W2VY(int vy);
    double WinToViewY(int vy);
    //
    void W2V(int vx, int vy, double *x, double *y);
    V2D W2V(int vx, int vy);
    void WinToView(int vx, int vy, double *x, double *y);
    V2D WinToView(int vx, int vy);

    // Window to real coord.
    void W2R(int vx, int vy, double *x, double *y);
    void W2R(int vx, int vy, double *x, double *y, double *z);
    void WinToReal(int vx, int vy, double *x, double *y);
    void WinToReal(int vx, int vy, double *x, double *y, double *z);

    // Clip
    ClipCode DomainClipCode(int x, int y) { return m_domain.GetClipCode(x, y); }
    ClipCode DomainlipCode(const V2DI &v) { return m_domain.GetClipCode(v); }

    BOOLEAN CheckDomainClipOfLine(const V2DI &sp, const V2DI &ep,
                                  ClipCode scode, ClipCode ecode) {
        return m_domain.CheckClipOfLine(sp, ep, scode, ecode);
    }
};

class ViewportStack {
    Viewport *m_viewport;
    unsigned m_size;
    int m_now;
    BOOLEAN m_bAutoScale;
    BOOLEAN m_bZoomIn;

  public:
    ViewportStack(int size, BOOLEAN bZoomIn = FALSE);
    ~ViewportStack();
    void Reset();
    void Push(const Viewport &viewport);
    BOOLEAN Pop();
    BOOLEAN Pop(Viewport &viewport);
};

#endif
