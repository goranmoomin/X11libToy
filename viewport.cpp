#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#endif
#ifdef TOY
#include "canvas.h"
#include "mutil.h"
#include "viewport.h"
#else
#include "canvas.h"
#include "mutil.h"
#include "viewport.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Viewport

Viewport::Viewport() { SetVTRM(0., 0., 1.); }

Viewport::Viewport(int width, int height) {
    SetVTRM(0., 0., 1.);
    Init(width, height);
}

void Viewport::Init(int left, int top, int right, int bottom) {
    m_window.Set(left, top, right, bottom);
    m_domain.Set(left, top, right, bottom);
    SetRange(left, top, right, bottom);
}

void Viewport::operator=(const Viewport &viewport) {
    m_viewpoint = viewport.m_viewpoint;
    m_rx = viewport.m_rx;
    m_ry = viewport.m_ry;
    m_rz = viewport.m_rz;
    m_scale = viewport.m_scale;
    m_real = viewport.m_real;
    m_view = viewport.m_view;
    m_domain = viewport.m_domain;
    m_window = viewport.m_window;
}

void Viewport::operator=(const Canvas &canvas) {
    m_viewpoint = canvas.Viewport::m_viewpoint;
    m_rx = canvas.Viewport::m_rx;
    m_ry = canvas.Viewport::m_ry;
    m_rz = canvas.Viewport::m_rz;
    m_scale = canvas.Viewport::m_scale;
    m_real = canvas.Viewport::m_real;
    m_view = canvas.Viewport::m_view;
    m_domain = canvas.Viewport::m_domain;
    m_window = canvas.Viewport::m_window;
}

/////////////////////////////////////////////////////////////////////////////
// Viewport::VTRM

void Viewport::SetVTRM(double x, double y, double z, BOOLEAN bNormalize) {
    if(x * x + y * y + z * z < 0.01)
        m_viewpoint.Set(1., 1., 1.);
    else
        m_viewpoint.Set(x, y, z);
    SetVTRM(bNormalize);
}

void Viewport::SetVTRM(BOOLEAN bNormalize) {
    m_rz = m_viewpoint;
    m_rz.Normalize();

    if(SQR(m_viewpoint.x) + SQR(m_viewpoint.y) < 0.001)
        m_rx.Set(0., 50., 0.);
    else
        m_rx.Set(0., 0., 50.);
    m_rx -= m_rz;
    m_rx ^= m_rz;
    m_rx.Normalize();

    m_ry = m_rz ^ m_rx;

    if(!bNormalize) {
        double norm = m_viewpoint.GetNorm();
        m_rx *= norm;
        m_ry *= norm;
        m_rz *= norm;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Viewport::SetRange

void Viewport::SetXRange(double x1, double x2) {
    m_view.SetX(x1, x2);
    m_scale.x = (DomainWidth() - 1) / ViewWidth();
}
void Viewport::SetXRange(double x1, double x2, double order) {
    SetXRange(GetLower(x1, order), GetUpper(x2, order));
}
void Viewport::SetXRange(const Vector &vx, double order) {
    SetXRange(GetLower(vx.GetMin(), order), GetUpper(vx.GetMax(), order));
}

void Viewport::SetYRange(double y1, double y2) {
    m_view.SetY(y1, y2);
    m_scale.y = (DomainHeight() - 1) / ViewHeight();
}
void Viewport::SetYRange(double y1, double y2, double order) {
    SetYRange(GetLower(y1, order), GetUpper(y2, order));
}
void Viewport::SetYRange(const Vector &vy, double order) {
    SetYRange(vy.GetMin(), vy.GetMax(), order);
}
void Viewport::SetRange(double x1, double y1, double x2, double y2) {
    SetXRange(x1, x2);
    SetYRange(y1, y2);
}
void Viewport::SetRange(double x1, double y1, double x2, double y2,
                        double order) {
    SetXRange(x1, x2, order);
    SetYRange(y1, y2, order);
}
void Viewport::SetRange(const Vector &v) {
    SetXRange((double)v.From(), (double)v.To(), GetOrder10(v.Size() - 1));
    double min = v.GetMin();
    double max = v.GetMax();
    SetYRange(min, max, GetOrder10(max - min));
}
void Viewport::SetRange(const Vector &v, double order) {
    SetXRange((double)v.From(), (double)v.To(), order);
    double min = v.GetMin();
    double max = v.GetMax();
    SetYRange(min, max, GetOrder10(max - min));
}
void Viewport::SetRange(const Vector &vx, const Vector &vy) {
    SetRange(vx.GetMin(), vx.GetMax(), vy.GetMin(), vy.GetMax());
}
void Viewport::SetRange(const Vector &vx, const Vector &vy, double order) {
    SetRange(vx.GetMin(), vx.GetMax(), vy.GetMin(), vy.GetMax(), order);
}

/////////////////////////////////////////////////////////////////////////////
// Domain
void Viewport::SetDomain(int x1, int y1, int x2, int y2) {
    if(x2 <= 0) x2 = Right() + x2;
    if(y2 <= 0) y2 = Bottom() + y2;
    m_domain.Set(x1, y1, x2, y2);
    SetRange(ViewLeft(), ViewRight(), ViewTop(), ViewBottom());
}

/////////////////////////////////////////////////////////////////////////////
// Range

void Viewport::ResetWinToView() {
    m_view.Set(WinToView(m_domain.MinX(), m_domain.MaxY()),
               WinToView(m_domain.MaxX(), m_domain.MinY()));
}

void Viewport::SetRange3D(double min, double max) {
    SetRange3D(Box3DD(min, min, min, max, max, max));
}

void Viewport::SetRange3D(double minX, double maxX, double minY, double maxY,
                          double minZ, double maxZ) {
    SetRange3D(Box3DD(minX, minY, minZ, maxX, maxY, maxZ));
}

void Viewport::SetRange3D(const Box3DD &real) {
    R2V(real, m_view);
    SetRange(m_view);
}

void Viewport::SetRange3D(const Vector3DD &v) {
    SetRange3D(v.GetMin(), v.GetMax());
}

#ifdef WIN32
BOOLEAN Viewport::CalcCenterAndScale(CPoint sp, CPoint ep, int tolerance) {
    int dx = abs(ep.x - sp.x);
    int dy = abs(ep.y - sp.y);
    if(dx < tolerance || dy < tolerance) return FALSE;

    /*-- evaluate the center of window coordinate */
    int x_center = (sp.x + ep.x) / 2;
    int y_center = (sp.y + ep.y) / 2;
    //    ViewToWin(x_center, y_center, &m_winCnt.x, &m_winCnt.y);

    double h_ratio = (double)(dx) / (m_view.MaxX() - m_view.MinX());
    double v_ratio = (double)(dy) / (m_view.MaxY() - m_view.MinY());

    if(h_ratio < v_ratio)
        m_scale /= v_ratio;
    else
        m_scale /= h_ratio;

    ResetWinToView();

    return TRUE;
}

BOOLEAN Viewport::CalcCenterAndClip(CPoint dp, int tolerance) {
    if(abs(dp.x) < tolerance || abs(dp.y) < tolerance) return FALSE;

    //    m_winCnt.x -= (double)dp.x/m_scale.x;
    //    m_winCnt.y += (double)dp.y/m_scale.y;

    ResetWinToView();

    return TRUE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
//

void Viewport::SetIsoScale() {
    if(m_scale.x > m_scale.y) {
        double dX = m_view.Width() * (m_scale.x / m_scale.y - 1) / 2;
        m_view.MinX() -= dX;
        m_view.MaxX() += dX;
        m_scale.x = m_scale.y;
    } else if(m_scale.x < m_scale.y) {
        double dY = m_view.Height() * (m_scale.y / m_scale.x - 1) / 2;
        m_view.MinY() -= dY;
        m_view.MaxY() += dY;
        m_scale.y = m_scale.x;
    }
    //    ResetRange();
}

BOOLEAN Viewport::IsZView() {
    return (m_viewpoint.x < TolZero && m_viewpoint.y < TolZero) ? TRUE : FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Viewport::Visiblity Checking Functions

BOOLEAN Viewport::IsVisibleRealBox(const Box3DD &real) {
    Box2DD view;
    R2V(real, view);
    return IsVisibleViewBox(view);
}

BOOLEAN Viewport::IsVisibleViewBox(const Box2DD &view) {
    return m_view && view;
}

/////////////////////////////////////////////////////////////////////////////
// Viewport::Transformatin Functions

// Real to view coord.
double Viewport::R2VX(double x, double y, double z) {
    return x * m_rx.x + y * m_rx.y + z * m_rx.z;
}
double Viewport::R2VX(const double a[]) { return R2VX(a[0], a[1], a[2]); }
double Viewport::R2VX(const Vector3D &v) { return R2VX(v.x, v.y, v.z); }
double Viewport::RealToViewX(double x, double y, double z) {
    return R2VX(x, y, z);
}
double Viewport::RealToViewX(const double v[]) {
    return R2VX(v[0], v[1], v[2]);
}
double Viewport::RealToViewX(const Vector3D &v) { return R2VX(v.x, v.y, v.z); }

double Viewport::R2VY(double x, double y, double z) {
    return x * m_ry.x + y * m_ry.y + z * m_ry.z;
}
double Viewport::R2VY(const double a[]) { return R2VY(a[0], a[1], a[2]); }
double Viewport::R2VY(const Vector3D &v) { return R2VY(v.x, v.y, v.z); }
double Viewport::RealToViewY(double x, double y, double z) {
    return R2VY(x, y, z);
}
double Viewport::RealToViewY(const double v[]) {
    return R2VY(v[0], v[1], v[2]);
}
double Viewport::RealToViewY(const Vector3D &v) { return R2VY(v.x, v.y, v.z); }

double Viewport::R2VZ(double x, double y, double z) {
    return x * m_rz.x + y * m_rz.y + z * m_rz.z;
}
double Viewport::R2VZ(const double v[]) { return R2VZ(v[0], v[1], v[2]); }
double Viewport::R2VZ(const Vector3D &v) { return R2VZ(v.x, v.y, v.z); }
double Viewport::RealToViewZ(double x, double y, double z) {
    return R2VZ(x, y, z);
}
double Viewport::RealToViewZ(const double a[]) {
    return R2VZ(a[0], a[1], a[2]);
}
double Viewport::RealToViewZ(const Vector3D &v) { return R2VZ(v.x, v.y, v.z); }

void Viewport::R2V(double x, double y, double z, double *vx, double *vy) {
    *vx = R2VX(x, y, z);
    *vy = R2VY(x, y, z);
}
void Viewport::R2V(const Vector3D &r, double *vx, double *vy) {
    R2V(r.x, r.y, r.z, vx, vy);
}
void Viewport::R2V(double x, double y, double z, double *vx, double *vy,
                   double *vz) {
    *vx = R2VX(x, y, z);
    *vy = R2VY(x, y, z);
    *vz = R2VZ(x, y, z);
}
V2D Viewport::R2V(double x, double y, double z) {
    return V2D(R2VX(x, y, z), R2VY(x, y, z));
}
void Viewport::R2V(const Vector3D &r, double *vx, double *vy, double *vz) {
    R2V(r.x, r.y, r.z, vx, vy, vz);
}
void Viewport::R2V(const Box3DD &r, Box2DD &v) {
    double vx, vy;
    R2V(r.left, r.bottom, r.lower, &vx, &vy);
    v.Set(vx, vy);
    R2V(r.left, r.bottom, r.upper, &vx, &vy);
    v.Update(vx, vy);
    R2V(r.left, r.top, r.lower, &vx, &vy);
    v.Update(vx, vy);
    R2V(r.left, r.top, r.upper, &vx, &vy);
    v.Update(vx, vy);
    R2V(r.right, r.bottom, r.lower, &vx, &vy);
    v.Update(vx, vy);
    R2V(r.right, r.bottom, r.upper, &vx, &vy);
    v.Update(vx, vy);
    R2V(r.right, r.top, r.lower, &vx, &vy);
    v.Update(vx, vy);
    R2V(r.right, r.top, r.upper, &vx, &vy);
    v.Update(vx, vy);
}
void Viewport::R2V(const Box3DD &r, Box3DD &v) {
    double vx, vy, vz;
    R2V(r.left, r.bottom, r.lower, &vx, &vy, &vz);
    v.Set(vx, vy, vz);
    R2V(r.left, r.bottom, r.upper, &vx, &vy, &vz);
    v.Update(vx, vy, vz);
    R2V(r.left, r.top, r.lower, &vx, &vy, &vz);
    v.Update(vx, vy, vz);
    R2V(r.left, r.top, r.upper, &vx, &vy, &vz);
    v.Update(vx, vy, vz);
    R2V(r.right, r.bottom, r.lower, &vx, &vy, &vz);
    v.Update(vx, vy, vz);
    R2V(r.right, r.bottom, r.upper, &vx, &vy, &vz);
    v.Update(vx, vy, vz);
}
void Viewport::RealToView(double x, double y, double z, double *vx,
                          double *vy) {
    *vx = R2VX(x, y, z);
    *vy = R2VY(x, y, z);
}
void Viewport::RealToView(const Vector3D &r, double *vx, double *vy) {
    R2V(r.x, r.y, r.z, vx, vy);
}
void Viewport::RealToView(double x, double y, double z, double *vx, double *vy,
                          double *vz) {
    *vx = R2VX(x, y, z);
    *vy = R2VY(x, y, z);
    *vz = R2VZ(x, y, z);
}
V2D Viewport::RealToView(double x, double y, double z) {
    return V2D(R2VX(x, y, z), R2VY(x, y, z));
}
void Viewport::RealToView(const Vector3D &r, double *vx, double *vy,
                          double *vz) {
    R2V(r.x, r.y, r.z, vx, vy, vz);
}
void Viewport::RealToView(const Box3DD &r, Box2DD &v) { R2V(r, v); }
void Viewport::RealToView(const Box3DD &r, Box3DD &v) { R2V(r, v); }

//
// View to window coord.
//
int Viewport::V2WX(double vx) {
    return (int)((vx - m_view.left) * m_scale.x + 0.5) + m_domain.left;
}
int Viewport::ViewToWinX(double vx) { return V2WX(vx); }
int Viewport::V2WY(double vy) {
    // return m_domain.bottom - (int)((vy-m_view.top)*m_scale.y+0.5);
    // return m_domain.bottom - (int)((vy-m_view.top)*m_scale.y);
    return m_domain.top + (int)((m_view.bottom - vy) * m_scale.y);
}
int Viewport::ViewToWinY(double vy) { return V2WY(vy); }

V2DI Viewport::V2W(double vx, double vy) { return V2DI(V2WX(vx), V2WY(vy)); }
void Viewport::V2W(double vx, double vy, Point &w) { V2W(vx, vy, &w.x, &w.y); }
void Viewport::ViewToWin(double vx, double vy, int *wx, int *wy) {
    V2W(vx, vy, wx, wy);
}
V2DI Viewport::ViewToWin(double vx, double vy) {
    return V2DI(V2WX(vx), V2WY(vy));
}
void Viewport::ViewToWin(double vx, double vy, Point &w) {
    V2W(vx, vy, &w.x, &w.y);
}

//
// Real to window coord.
//
int Viewport::R2WX(double x, double y, double z) { return V2WX(R2VX(x, y, z)); }
int Viewport::R2WX(const double v[]) { return R2WX(v[0], v[1], v[2]); }
int Viewport::R2WX(const Vector3D &v) { return R2WX(v.x, v.y, v.z); }
int Viewport::RealToWinX(double x, double y, double z) {
    return V2WX(R2VX(x, y, z));
}
int Viewport::RealToWinX(const double v[]) { return R2WX(v[0], v[1], v[2]); }
int Viewport::RealToWinX(const Vector3D &v) { return R2WX(v.x, v.y, v.z); }

int Viewport::R2WY(double x, double y, double z) { return V2WY(R2VY(x, y, z)); }
int Viewport::R2WY(const double v[]) { return R2WY(v[0], v[1], v[2]); }
int Viewport::R2WY(const Vector3D &v) { return R2WY(v.x, v.y, v.z); }
int Viewport::RealToWinY(double x, double y, double z) {
    return V2WY(R2VY(x, y, z));
}
int Viewport::RealToWinY(const double v[]) { return R2WY(v[0], v[1], v[2]); }
int Viewport::RealToWinY(const Vector3D &v) { return R2WY(v.x, v.y, v.z); }

V2DI Viewport::R2W(double x, double y, double z) {
    return Vector2DI(RealToWinX(x, y, z), RealToWinY(x, y, z));
}
void Viewport::R2W(const Vector3D &r, int *vx, int *vy) {
    R2W(r.x, r.y, r.z, vx, vy);
}
void Viewport::R2W(double x, double y, double z, Point &v) {
    R2W(x, y, z, &v.x, &v.y);
}
void Viewport::R2W(const Vector3D &r, Point &v) {
    R2W(r.x, r.y, r.z, &v.x, &v.y);
}
void Viewport::RealToWin(double x, double y, double z, int *wx, int *wy) {
    R2W(x, y, z, wx, wy);
}
V2DI Viewport::RealToWin(double x, double y, double z) {
    V2DI view;
    R2W(x, y, z, &view.x, &view.y);
    return view;
}
void Viewport::RealToWin(const Vector3D &v, int *vx, int *vy) {
    R2W(v.x, v.y, v.z, vx, vy);
}
void Viewport::RealToWin(double x, double y, double z, Point &v) {
    R2W(x, y, z, &v.x, &v.y);
}
void Viewport::RealToWin(const Vector3D &r, Point &v) {
    R2W(r.x, r.y, r.z, &v.x, &v.y);
}

ClipCode Viewport::R2WClip(const Vector3D &v, int *wx, int *wy) {
    RealToWin(v, wx, wy);
    return m_window.GetClipCode(*wx, *wy);
}
ClipCode Viewport::RealToWinClip(const Vector3D &v, int *wx, int *wy) {
    return R2WClip(v, wx, wy);
}

//
// Window to view coord.
//
double Viewport::W2VX(int wx) {
    return (double)(wx - m_domain.MinX()) / m_scale.x + m_view.MinX();
}
double Viewport::WinToViewX(int wx) {
    return (double)(wx - m_domain.MinX()) / m_scale.x + m_view.MinX();
}
double Viewport::W2VY(int wy) {
    return (double)(m_domain.MaxY() - wy) / m_scale.y + m_view.MinY();
}
double Viewport::WinToViewY(int wy) {
    return (double)(m_domain.MaxY() - wy) / m_scale.y + m_view.MinY();
}

void Viewport::W2V(int wx, int wy, double *vx, double *vy) {
    *vx = ViewToWinX(wx);
    *vy = ViewToWinY(wy);
}
V2D Viewport::W2V(int wx, int wy) {
    return V2D(ViewToWinX(wx), ViewToWinY(wy));
}
void Viewport::WinToView(int wx, int wy, double *vx, double *vy) {
    *vx = ViewToWinX(wx);
    *vy = ViewToWinY(wy);
}
V2D Viewport::WinToView(int wx, int wy) {
    return V2D(ViewToWinX(wx), ViewToWinY(wy));
}

//
// Window to real coord.
//
void Viewport::W2R(int wx, int wy, double *x, double *y, double *z) {
    double vx = ViewToWinX(wx);
    double vy = ViewToWinY(wy);

    double t = m_rx.x * m_ry.y - m_rx.y * m_ry.x;
    if(fabs(t) < TolZero) { // z-view
        t = SQR(m_rx.x) + SQR(m_rx.y);
        *x = vx * m_rx.x / t;
        *y = vx * m_rx.y / t;
        *z = vy;
    } else {
        *x = (vx * m_ry.y - vy * m_rx.y) / t;
        *y = (vy * m_rx.x - vx * m_ry.x) / t;
        *z = 0.f;
    }
}
void Viewport::WinToReal(int wx, int wy, double *x, double *y, double *z) {
    W2R(wx, wy, x, y, z);
}

void Viewport::W2R(int wx, int wy, double *x, double *y) {
    double t = m_rx.x * m_ry.y - m_rx.y * m_ry.x;

    if(fabs(t) < TolZero) { // z-view
        t = SQR(m_rx.x) + SQR(m_rx.y);
        *x = wx * m_rx.x / t;
        *y = wx * m_rx.y / t;
    } else {
        *x = (wx * m_ry.y - wy * m_rx.y) / t;
        *y = (wy * m_rx.x - wx * m_ry.x) / t;
    }
}
void Viewport::WinToReal(int wx, int wy, double *x, double *y) {
    W2R(wx, wy, x, y);
}

/////////////////////////////////////////////////////////////////////////////
// Push/Pop Viewport

ViewportStack::ViewportStack(int size, BOOLEAN bZoomIn) {
    m_size = size;
    m_viewport = size > 0 ? new Viewport[size] : NULL;
    m_now = -1;
    m_bAutoScale = TRUE;
    m_bZoomIn = bZoomIn;
}

ViewportStack::~ViewportStack() {
    if(m_viewport) delete[] m_viewport;
}

void ViewportStack::Reset() { m_now = -1; }

void ViewportStack::Push(const Viewport &viewport) {
    if(m_now + 1 >= (int)m_size)
        for(unsigned i = 1; i < m_size; i++) m_viewport[i - 1] = m_viewport[i];
    else
        m_now++;
    m_viewport[m_now] = viewport;
}

BOOLEAN ViewportStack::Pop() {
    if(m_now >= 0) {
        m_now--;
        return TRUE;
    }
    return FALSE;
}

BOOLEAN ViewportStack::Pop(Viewport &viewport) {
    if(m_now < 0) {
        if(m_bAutoScale) {
            viewport.m_scale *= m_bZoomIn ? 0.6 : 1. / 0.6;
            viewport.ResetWinToView();
            return TRUE;
        } else
            return FALSE;
    } else {
        viewport = m_viewport[m_now];
        m_now--;
        return TRUE;
    }
}
