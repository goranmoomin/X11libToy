// Toy/canvas.h
//
// Last Modified : 2002/11/09
//

#ifndef _TOY_CANVAS_H_
#define _TOY_CANVAS_H_

#include "color.h"
#include "graph.h"

#define LOOP_CANVAS_INT(c, i, j)                                               \
    LOOP_CANVAS_HEIGHT_INT(c, j) LOOP_CANVAS_WIDTH_INT(c, i)
#define LOOP_CANVAS_WIDTH(c, i) for(i = c.Left(); i <= c.Right(); i++)
#define LOOP_CANVAS_WIDTH_INT(c, i) for(int i = c.Left(); i <= c.Right(); i++)
#define LOOP_CANVAS_HEIGHT(c, i) for(i = c.Top(); i <= c.Bottom(); i++)
#define LOOP_CANVAS_HEIGHT_INT(c, i) for(int i = c.Top(); i <= c.Bottom(); i++)
#define LOOP_DOMAIN_WIDTH(c, i)                                                \
    for(i = c.DomainLeft(); i <= c.DomainRight(); i++)
#define LOOP_DOMAIN_WIDTH_INT(c, i)                                            \
    for(int i = c.DomainLeft(); i <= c.DomainRight(); i++)
#define LOOP_DOMAIN_HEIGHT(c, i)                                               \
    for(i = c.DomainBottom(); i <= c.DomainTop(); i++)
#define LOOP_DOMAIN_HEIGHT_INT(c, i)                                           \
    for(int i = c.DomainBottom(); i <= c.DomainTop(); i++)

inline COLORREF GetSpectrumColor(double r) {
    int b = (int)(r * 255);
    if(b > 0) return RGB(255, 255 - b, 255 - b);
    if(b < 0) return RGB(255 + b, 255 + b, 255);
    return RGB(255, 255, 255);
}

#ifdef __AFX_H__ // Visual C++ / MFC

#include "list.h"

class Object: public CGdiObject, public Node<Object> {};
class ObjectList: public LinkedList<Object> {
    CGdiObject *m_currentObject;

  public:
    ObjectList() { m_currentObject = NULL; }
    CPen *FindPen(int nPenStyle, int nWidth, COLORREF crColor) {
        Object *pObject;
        EXTLOGPEN logPen;
        LOOP_LIST(*this, pObject) {
            if(((CPen *)pObject)->GetExtLogPen(&logPen))
                if(logPen.elpPenStyle == (UINT)nPenStyle)
                    if(logPen.elpWidth == (UINT)nWidth)
                        if(logPen.elpColor == crColor) return (CPen *)pObject;
        }
        return NULL;
    }
    CBrush *FindBrush(COLORREF crColor) {
        Object *pObject;
        LOGBRUSH logBrush;
        LOOP_LIST(*this, pObject) {
            if(((CBrush *)pObject)->GetLogBrush(&logBrush))
                if(logBrush.lbColor == crColor) return (CBrush *)pObject;
        }
        return NULL;
    }
    CPen *GetPen(int nPenStyle, int nWidth, COLORREF crColor) {
        CPen *pen = FindPen(nPenStyle, nWidth, crColor);
        if(!pen) {
            pen = (CPen *)Append(new Object);
            pen->CreatePen(nPenStyle, nWidth, crColor);
        }
        m_currentObject = pen;
        return pen;
    }
    CBrush *GetBrush(COLORREF crColor) {
        CBrush *brush = FindBrush(crColor);
        if(!brush) {
            brush = (CBrush *)Append(new Object);
            brush->CreateSolidBrush(crColor);
        }
        m_currentObject = brush;
        return brush;
    }
    CPen *GetCurrentPen() { return (CPen *)m_currentObject; }
    CBrush *GetCurrentBrush() { return (CBrush *)m_currentObject; }
};

class Hook;

class Canvas: public Graph, public Color {
    CWnd *m_pWnd;
    CWnd *m_pClipWnd;

    // DC
    CDC *m_pDC;
    int m_nDC;
    // pen
    CPen *m_pPen;
    ObjectList m_penList;
    int m_penStyle;
    int m_penWidth;
    COLORREF m_penColor;
    //
    int m_pushedPenStyle;
    int m_pushedPenWidth;
    int m_pushedPenColor;
    // Brush
    CBrush *m_pBrush;
    ObjectList m_brushList;
    COLORREF m_brushColor;
    //
    COLORREF m_pushedBrushColor;
    // Palette
    CPalette *m_pPalette;
    // Font
    CFont *m_pFont;
    // Bitmap
    CDC *m_pBitmapDC;
    CBitmap m_Bitmap;
    CBitmap *m_pBitmap;
    BYTE *m_pBits;
    CPen *m_pBitmapPen;
    CBrush *m_pBitmapBrush;
    CPalette *m_pBitmapPalette;

    //
    CPen *m_pPushedPen;
    CPen *m_pBitmapPushedPen;
    CBrush *m_pPushedBrush;
    CBrush *m_pBitmapPushedBrush;

  public:
    Canvas(CWnd *pWnd = NULL, CWnd *pClipWnd = NULL);
    ~Canvas();
    void Create(CWnd *pWnd, CWnd *pClipWnd = NULL);

    //
    CWnd *GetWnd() { return m_pWnd; }
    BOOLEAN IsValid() { return m_pWnd ? TRUE : FALSE; }

    //
    BOOLEAN HasDC();
    CDC *CreateDC();
    CDC *StartDC(CDC *pDC = NULL);
    void EndDC(CDC *pDC = NULL, BOOLEAN bTerminate = FALSE);
    CDC *GetDC() { return m_pDC; }

    //
    int Width() { return Viewport::Rect().Width(); }
    int Height() { return Viewport::Rect().Height(); }

    //
    void SetPalette(RGBQUAD *rgb, int nPalette);
    void SetPen(int style, int width, ColorEnum colorEnum);
    void SetPen(int style, int width, COLORREF color);
    int SetPenStyle(int style);
    int GetPenWidth();
    int SetPenWidth(int width);
    COLORREF SetPenColor(ColorEnum colorEnum);
    // COLORREF SetPenColor(const char *color);
    COLORREF SetPenColor(COLORREF crColor);
    COLORREF SetBrushColor(ColorEnum colorEnum);
    // COLORREF SetBrushColor(const char *color);
    COLORREF SetBrushColor(COLORREF color);
    void SetBrush(CBrush &brush);
    void SetTextColor(ColorEnum colorEnum);
    // void     SetTextColor(const char *color);
    void SetTextColor(COLORREF color);

    CFont *SelectObject(CFont *pFont);

    void ColorInvert();

    //
    int ExcludeClipRect(LPCRECT lpRect);
    int SetROP2(int nDrawMode);
    COLORREF GetBkColor();
    COLORREF SetBkColor(COLORREF crColor);
    void SetBkColor(ColorEnum color);
    void PushPen();
    void PopPen();
    void PushBrush();
    void PopBrush();

    CSize GetTextExtent(const CString &str) const {
        return m_pDC->GetTextExtent(str);
    }

    //
    int GetDCStackSize() { return m_nDC; }

    friend class Hook;

#else  // Linux / X11

class Canvas: public Graph {
    BOOLEAN m_bCreated;

  public:
    // Data
    Display *m_display;
    int m_screen;
    GC m_gc;
    Window m_window;
    Pixmap m_pixmap;
    int m_border;
    int m_depth;
    XEvent m_event;

  public:
    // Creator
    Canvas();
    Canvas(int width, int height, const char *format = " ", ...);
    Canvas(int width, int height, BOOLEAN bBitmap, const char *format = " ",
           ...);
    ~Canvas();

    //
    void Create(int width, int height, BOOLEAN bBitmap = TRUE,
                const char *format = " ", ...);
    BOOLEAN IsCreated() { return m_bCreated; }
    void Flush();
    void SetTitle(const char *format, ...);
    int SetFunction(int);

    int GetPenWidth();
    int SetPenWidth(int);
    int SetLineStyle(int); // LineSolid, LineOnOffDash, LineDoubleDash
    int SetCapStyle(int);  // CapNotLast, CapButt, CapRound, CapProjecting
    int SetJoinStyle(int); // JoinMiter, JoinRound, JoinBevel

    COLORREF SetPenColor(ColorEnum colorEnum) {
        return SetForeground(colorEnum);
    }
    // COLORREF SetPenColor(const char *color) { return SetForeground(color); }
    COLORREF SetPenColor(COLORREF crColor) { return SetForeground(crColor); }

    void Copy();
    void Undo(int, int, int, int);
    void UndoAll();
#endif // __AFX_H__ or X11

    /////////////////////////////////////////////////////////////////////////
    // Bitmap

    // Attributes
  private:
    BOOLEAN m_bVisibleMode;
    BOOLEAN m_bBitmapMode;
    int m_bitmapWidth, m_bitmapHeight;

    //
  public:
    BOOLEAN IsVisibleMode() { return m_bVisibleMode; }
    void OnVisibleMode() { m_bVisibleMode = TRUE; }
    void OffVisibleMode() { m_bVisibleMode = FALSE; }

    /////////////////////////////////////////////////////////////////////////
    // Color

    // Attributes
  private:
    COLORREF m_foreground;
    COLORREF m_background;

    // Implementation
  public:
    COLORREF GetForeground() const { return m_foreground; }
    COLORREF GetBackground() const { return m_background; }
    COLORREF SetForeground(COLORREF crColor);
    COLORREF SetForeground(BYTE r, BYTE g, BYTE b);
    COLORREF SetForeground(ColorEnum colorEnum);
    // COLORREF SetForeground(const char *strColor);
    COLORREF SetBackground(COLORREF crColor);
    COLORREF SetBackground(BYTE r, BYTE g, BYTE b);
    COLORREF SetBackground(ColorEnum colorEnum);
    // COLORREF SetBackground(const char *strColor);
    COLORREF SetSpectrumColor(double r) {
        return SetForeground(::GetSpectrumColor(r));
    }

    /////////////////////////////////////////////////////////////////////////
    // Viewport
    //

    // Attributes
  private:
    Viewport m_viewport; // for backup

    // Implementation
  public:
    void operator=(const Viewport &);
    void PushViewport() { m_viewport = *this; }
    void PopViewport() { *this = m_viewport; }

    /////////////////////////////////////////////////////////////////////////
    // Virtual drawing functions will be used by class Graph
    // They use window coordinate

    // Implementation
  public:
    virtual COLORREF _SetForeground_(COLORREF crColor);
    virtual COLORREF _SetBackground_(COLORREF crColor);
    virtual void _DrawString_(int x, int y, UINT align, const char *str);
    virtual void _DrawDot_(int x, int y);
    virtual void _DrawLine_(int x1, int y1, int x2, int y2);
    virtual void _DrawLines_(Point *lpPoints, int nCount);
    virtual void _DrawRectangle_(int x, int y, int width, int height);
    virtual void _DrawEllipse_(int x, int y, int width, int height);
    virtual void _FillRectangle_(int x1, int y1, int x2, int y2);
    virtual void _FillCircle_(int x, int y, int r);
    virtual BOOLEAN _FillPolygon_(Point *lpPoints, int nCount);
    //
    void Clear();
    void Clear(ColorEnum colorEnum) { Clear(GetRGB(colorEnum)); }
    // void Clear(const char *color) { Clear(GetRGB(color)); }
    void Clear(COLORREF crColor);
    void Clear(BYTE r, BYTE g, BYTE b) { Clear(RGB(r, g, b)); }
    void Clear(int x1, int y1, int x2, int y2);
    void Clear(int x1, int y1, int x2, int y2, COLORREF crColor);
    void ClearDomain() {
        Clear(DomainLeft(), DomainTop(), DomainRight(), DomainBottom());
    }
    void ClearDomain(ColorEnum colorEnum) { ClearDomain(GetRGB(colorEnum)); }
    // void ClearDomain(const char *color) { ClearDomain(GetRGB(color)); }
    void ClearDomain(COLORREF crColor) {
        Clear(DomainLeft(), DomainTop(), DomainRight(), DomainBottom(),
              crColor);
    }
    void ClearDomain(BYTE r, BYTE g, BYTE b) {
        Clear(DomainLeft(), DomainTop(), DomainRight(), DomainBottom(),
              RGB(r, g, b));
    }

    //
    BOOLEAN IsBitmapMode();
    void OnBitmapMode() { m_bBitmapMode = TRUE; }
    void OffBitmapMode() { m_bBitmapMode = FALSE; }
    BOOLEAN BitmapMode();
    int BitmapWidth() { return m_bitmapWidth; }
    int BitmapHeight() { return m_bitmapHeight; }
    BOOLEAN HasValidBitmap();
    BOOLEAN HasBitmap();
#ifdef __AFX_H__
    CRect GetBitmapRect();
#endif
    BOOLEAN CreateBitmap();
    void DestroyBitmap();
    void ResetBitmapDC();
    BOOLEAN CopyToBitmap();
    BOOLEAN CopyFromBitmap();
    BOOLEAN CopyFromBitmap(int x1, int y1, int x2, int y2);
    BOOLEAN CopyFromBitmap(Point &sp, BOOLEAN bBitmapUpdate);
    BOOLEAN CopyFromBitmap(const Box2DI &mb);
    BOOLEAN CopyFromBitmap(int x, int y, int w, int h, int wx, int wy);

#ifdef __AFX_H__
    void Clear(LPCRECT lpRect) { Clear(lpRect, GetBackground()); }
    void Clear(LPCRECT lpRect, COLORREF crColor);
#endif
    //    void DrawString(int x, int y, UINT align, const char *string,
    //						COLORREF crColor);
};

#endif
