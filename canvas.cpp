#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "canvas.h"
#include "message.h"

/////////////////////////////////////////////////////////////////////////////
// Canvas::Canvas

Canvas::Canvas() {
    m_bCreated = FALSE;
    m_pixmap = (Pixmap)NULL;
}

Canvas::Canvas(int width, int height, const char *format, ...) {
    va_list vargs;
    char title[1024];
    va_start(vargs, format);
    vsprintf(title, format, vargs);
    va_end(vargs);
    Create(width, height, (BOOLEAN)TRUE, title);
}

Canvas::Canvas(int width, int height, BOOLEAN bBitmap, const char *format,
               ...) {
    va_list vargs;
    char title[1024];
    va_start(vargs, format);
    vsprintf(title, format, vargs);
    va_end(vargs);
    Create(width, height, bBitmap, title);
}

void Canvas::Create(int width, int height, BOOLEAN bBitmap, const char *format,
                    ...) {
    if((m_display = XOpenDisplay(NULL)) == NULL)
        Error("Cannot open m_display.\n");
    m_screen = DefaultScreen(m_display);
    m_gc = DefaultGC(m_display, m_screen);
    m_border = 1;
    m_depth = DefaultDepth(m_display, m_screen);
    m_font = XQueryFont(m_display, XGContextFromGC(m_gc));
    m_foreground = BlackPixel(m_display, m_screen);
    m_background = WhitePixel(m_display, m_screen);
    m_window = XCreateSimpleWindow(m_display, RootWindow(m_display, m_screen),
                                   0, 0, width, height, m_border, m_foreground,
                                   m_background);
    XSetWindowAttributes att;
    att.save_under = True;
    XChangeWindowAttributes(m_display, m_window, CWSaveUnder, &att);
    m_pixmap = (Pixmap)NULL;

    va_list vargs;
    char title[1024];
    va_start(vargs, format);
    vsprintf(title, format, vargs);
    va_end(vargs);
    XStoreName(m_display, m_window, title);

    XSelectInput(m_display, m_window, ExposureMask);
    XMapWindow(m_display, m_window);
    do
        XNextEvent(m_display, &m_event);
    while(m_event.type != Expose && m_event.xexpose.count != 0);
    att.backing_store = Always;
    XChangeWindowAttributes(m_display, m_window, CWBackingStore, &att);

    Viewport::Init(width, height);

    if(bBitmap) {
        m_bVisibleMode = FALSE;
        m_bBitmapMode = TRUE;
        m_pixmap = (Pixmap)XCreatePixmap(m_display, m_window, Width(), Height(),
                                         m_depth);
        Clear();
    } else {
        m_bVisibleMode = TRUE;
        m_bBitmapMode = FALSE;
    }
    m_bCreated = TRUE;
}

Canvas::~Canvas() {
    if(m_bCreated) DestroyBitmap();
    XDestroyWindow(m_display, m_window);
}

void Canvas::Flush() {
    if(IsBitmapMode()) {
        XCopyArea(m_display, m_pixmap, m_window, m_gc, 0, 0, Width(), Height(),
                  0, 0);
    }
    XCheckWindowEvent(m_display, m_window, ExposureMask, &m_event);
}

void Canvas::SetTitle(const char *format, ...) {
    va_list vargs;
    char title[1024];
    va_start(vargs, format);
    vsprintf(title, format, vargs);
    va_end(vargs);
    XStoreName(m_display, m_window, title);
}

void Canvas::operator=(const Viewport &viewport) {
    m_viewpoint = viewport.m_viewpoint;
    m_rx = viewport.m_rx;
    m_ry = viewport.m_ry;
    m_rz = viewport.m_rz;
    m_scale = viewport.m_scale;
    m_real = viewport.m_real;
    //    m_window  = viewport.m_window;
    m_view = viewport.m_view;
}

/////////////////////////////////////////////////////////////////////////////
// Canvas::Mode

int Canvas::SetFunction(int function) {
    XGCValues xgc;
    XGetGCValues(m_display, m_gc, GCFunction, &xgc);
    XSetFunction(m_display, m_gc, function);
    return xgc.function;
}

COLORREF Canvas::_SetForeground_(COLORREF crColor) {
    XSetForeground(m_display, m_gc, crColor);
    COLORREF prevForeground = m_foreground;
    m_foreground = crColor;
    return prevForeground;
}
COLORREF Canvas::SetForeground(COLORREF crColor) {
    return _SetForeground_(crColor);
}
COLORREF Canvas::SetForeground(BYTE r, BYTE g, BYTE b) {
    return _SetForeground_(RGB(r, g, b));
}
/*
COLORREF Canvas::SetForeground(const char *strColor)
{
    XColor c0, c1;
    XAllocNamedColor(m_display, DefaultColormap(m_display, m_screen),
                                                strColor, &c1, &c0);
    return SetForeground(c1.pixel);
}
*/
COLORREF Canvas::SetForeground(ColorEnum colorEnum) {
    return _SetForeground_(GetRGB(colorEnum));
}

COLORREF Canvas::_SetBackground_(COLORREF crColor) {
    XSetBackground(m_display, m_gc, crColor);
    COLORREF prevBackground = m_background;
    m_background = crColor;
    return prevBackground;
}
COLORREF Canvas::SetBackground(COLORREF crColor) {
    return _SetBackground_(crColor);
}
COLORREF Canvas::SetBackground(BYTE r, BYTE g, BYTE b) {
    return _SetBackground_(RGB(r, g, b));
}
COLORREF Canvas::SetBackground(ColorEnum colorEnum) {
    return _SetBackground_(GetRGB(colorEnum));
}
/*
COLORREF Canvas::SetBackground(const char *strColor)
{
    XColor c0, c1;
    XAllocNamedColor(m_display, DefaultColormap(m_display, m_screen),
                                                strColor, &c1, &c0);
    return SetBackground(c1.pixel);
}
*/

int Canvas::GetPenWidth() {
    XGCValues xgc;
    XGetGCValues(m_display, m_gc, GCLineWidth, &xgc);
    return xgc.line_width;
}

int Canvas::SetPenWidth(int width) {
    XGCValues xgc;
    XGetGCValues(m_display, m_gc,
                 GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle, &xgc);
    XSetLineAttributes(m_display, m_gc, width, xgc.line_style, xgc.cap_style,
                       xgc.join_style);
    return xgc.line_width;
}
int Canvas::SetLineStyle(int lineStyle) {
    XGCValues xgc;
    XGetGCValues(m_display, m_gc,
                 GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle, &xgc);
    XSetLineAttributes(m_display, m_gc, xgc.line_width, lineStyle,
                       xgc.cap_style, xgc.join_style);
    return xgc.line_style;
}

int Canvas::SetCapStyle(int capStyle) {
    XGCValues xgc;
    XGetGCValues(m_display, m_gc,
                 GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle, &xgc);
    XSetLineAttributes(m_display, m_gc, xgc.line_width, xgc.line_style,
                       capStyle, xgc.join_style);
    return xgc.cap_style;
}

int Canvas::SetJoinStyle(int joinStyle) {
    XGCValues xgc;
    XGetGCValues(m_display, m_gc,
                 GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle, &xgc);
    XSetLineAttributes(m_display, m_gc, xgc.line_width, xgc.line_style,
                       xgc.cap_style, joinStyle);
    return xgc.join_style;
}

/////////////////////////////////////////////////////////////////////////////
// Canvas::Virtual functions

void Canvas::Clear() {
    //    XClearWindow(m_display, m_window);
    Clear(Left(), Top(), Right(), Bottom(), m_background);
}
void Canvas::Clear(COLORREF crColor) {
    SetBackground(crColor);
    Clear(Left(), Top(), Right(), Bottom(), crColor);
}
void Canvas::Clear(int x1, int y1, int x2, int y2) {
    //    XClearArea(m_display, m_window, x1, y1, x2-x1+1, y2-y1+1, FALSE);
    Clear(x1, y1, x2, y2, m_background);
}
void Canvas::Clear(int x1, int y1, int x2, int y2, COLORREF crColor) {
    COLORREF foreground = SetForeground(crColor);
    _FillRectangle_(x1, y1, x2, y2);
    SetForeground(foreground);
}

BOOLEAN Canvas::_FillPolygon_(Point *lpPoints, int nCount) {
    int result = Success;
    if(m_bVisibleMode) {
        result = XFillPolygon(m_display, m_window, m_gc, lpPoints, nCount,
                              Convex, CoordModeOrigin);
    }
    if(m_bBitmapMode)
        result = XFillPolygon(m_display, m_pixmap, m_gc, lpPoints, nCount,
                              Convex, CoordModeOrigin);
    return result == Success ? TRUE : FALSE;
}

void Canvas::_DrawString_(int x, int y, UINT align, const char *str) {
    switch(align & TA_CENTER) {
    case TA_CENTER: x -= XTextWidth(m_font, str, strlen(str)) / 2; break;
    case TA_RIGHT: x -= XTextWidth(m_font, str, strlen(str)); break;
    }
    switch(align & TA_BASELINE) {
    case TA_TOP: y += m_font->ascent; break;
    case TA_BASELINE: y += (m_font->ascent - m_font->descent) / 2; break;
    case TA_BOTTOM: y -= m_font->descent; break;
    }

    if(m_bVisibleMode)
        XDrawString(m_display, m_window, m_gc, x, y, str, strlen(str));
    if(m_bBitmapMode)
        XDrawString(m_display, m_pixmap, m_gc, x, y, str, strlen(str));
}

void Canvas::_DrawDot_(int x, int y) {
    if(m_bVisibleMode) XDrawPoint(m_display, m_window, m_gc, x, y);
    if(m_bBitmapMode) XDrawPoint(m_display, m_pixmap, m_gc, x, y);
    MoveTo(x, y);
    // XGCValues xgc;
    // XGetGCValues(m_display, m_gc, GCLineWidth , &xgc);
    // DrawLine(x-xgc.line_width, y, x, y);
}

void Canvas::_DrawLine_(int x1, int y1, int x2, int y2) {
    if(m_bVisibleMode) XDrawLine(m_display, m_window, m_gc, x1, y1, x2, y2);
    if(m_bBitmapMode) XDrawLine(m_display, m_pixmap, m_gc, x1, y1, x2, y2);
    MoveTo(x2, y2);
}

void Canvas::_DrawLines_(Point *lpPoints, int nCount) {
    if(m_bVisibleMode)
        XDrawLines(m_display, m_window, m_gc, lpPoints, nCount,
                   CoordModeOrigin);
    if(m_bBitmapMode)
        XDrawLines(m_display, m_pixmap, m_gc, lpPoints, nCount,
                   CoordModeOrigin);
    MoveTo(lpPoints[nCount - 1].x, lpPoints[nCount - 1].y);
}

void Canvas::_DrawRectangle_(int _x1, int _y1, int _x2, int _y2) {
    int x1, x2, y1, y2;
    if(_x1 < _x2) {
        x1 = _x1;
        x2 = _x2;
    } else {
        x1 = _x2;
        x2 = _x1;
    }
    if(_y1 < _y2) {
        y1 = _y1;
        y2 = _y2;
    } else {
        y1 = _y2;
        y2 = _y1;
    }
    if(m_bVisibleMode)
        XDrawRectangle(m_display, m_window, m_gc, x1, y1, x2 - x1 + 1,
                       y2 - y1 + 1);
    if(m_bBitmapMode)
        XDrawRectangle(m_display, m_pixmap, m_gc, x1, y1, x2 - x1 + 1,
                       y2 - y1 + 1);
    MoveTo(x1, y1);
}

void Canvas::_DrawEllipse_(int x, int y, int w, int h) {
    if(m_bVisibleMode)
        XDrawArc(m_display, m_window, m_gc, x - w, y - h, w * 2 + 1, h * 2 + 1,
                 0, 360 * 64);
    if(m_bBitmapMode)
        XDrawArc(m_display, m_pixmap, m_gc, x - w, y - h, w * 2 + 1, h * 2 + 1,
                 0, 360 * 64);
    MoveTo(x, y);
}

void Canvas::_FillRectangle_(int _x1, int _y1, int _x2, int _y2) {
    int x1, x2, y1, y2;
    if(_x1 < _x2) {
        x1 = _x1;
        x2 = _x2;
    } else {
        x1 = _x2;
        x2 = _x1;
    }
    if(_y1 < _y2) {
        y1 = _y1;
        y2 = _y2;
    } else {
        y1 = _y2;
        y2 = _y1;
    }
    if(m_bVisibleMode)
        XFillRectangle(m_display, m_window, m_gc, x1, y1, x2 - x1 + 1,
                       y2 - y1 + 1);
    if(m_bBitmapMode)
        XFillRectangle(m_display, m_pixmap, m_gc, x1, y1, x2 - x1 + 1,
                       y2 - y1 + 1);
    MoveTo(x1, y1);
}

void Canvas::_FillCircle_(int x, int y, int r) {
    if(m_bVisibleMode)
        XFillArc(m_display, m_window, m_gc, x - r + 1, y - r + 1, 2 * r - 1,
                 2 * r - 1, 0, 360 * 64);
    if(m_bBitmapMode)
        XFillArc(m_display, m_pixmap, m_gc, x - r + 1, y - r + 1, 2 * r - 1,
                 2 * r - 1, 0, 360 * 64);
    MoveTo(x, y);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOLEAN Canvas::IsBitmapMode() {
    return (m_pixmap && m_bBitmapMode) ? TRUE : FALSE;
}

BOOLEAN Canvas::CreateBitmap() {
    DestroyBitmap();
    m_pixmap =
        (Pixmap)XCreatePixmap(m_display, m_window, Width(), Height(), m_depth);
    return TRUE;
}

void Canvas::DestroyBitmap() {
    if(m_bCreated && m_pixmap) {
        XFreePixmap(m_display, m_pixmap);
        m_pixmap = (Pixmap)NULL;
    }
}

BOOLEAN Canvas::CopyToBitmap() {
    if(m_pixmap == (Pixmap)NULL) CreateBitmap();
    XCopyArea(m_display, m_window, m_pixmap, m_gc, 0, 0, Width(), Height(), 0,
              0);
    return TRUE;
}

BOOLEAN Canvas::CopyFromBitmap(int x1, int y1, int x2, int y2) {
    XCopyArea(m_display, m_pixmap, m_window, m_gc, x1, y1, x2 - x1 + 1,
              y2 - y1 + 1, x1, y1);
    return TRUE;
}

BOOLEAN Canvas::CopyFromBitmap() {
    XCopyArea(m_display, m_pixmap, m_window, m_gc, 0, 0, Width(), Height(), 0,
              0);
    // Flush();
    return TRUE;
}

BOOLEAN Canvas::CopyFromBitmap(const Box2DI &box) {
    return CopyFromBitmap(box.MinX(), box.MinY(), box.MaxX(), box.MaxY());
}

BOOLEAN Canvas::CopyFromBitmap(int x, int y, int w, int h, int wx, int wy) {
    XCopyArea(m_display, m_pixmap, m_window, m_gc, x, y, w, h, wx, wy);
    return TRUE;
}

BOOLEAN Canvas::BitmapMode() {
    if(!CreateBitmap()) return FALSE;
    OffVisibleMode();
    OnBitmapMode();
}
