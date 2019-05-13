// Toy/graph.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_HOOK_H_
#define _TOY_HOOK_H_

#ifdef TOY
#include <Toy/canvas.h>
#else
#include "canvas.h"
#endif

class Hook: public Canvas {
  public:
    typedef enum {
        DEFAULT,
        ZOOM_IN,
        ZOOM_SCROLL,
    } HookType;
    typedef enum { KEY_NONE, KEY_LB_DOWN, KEY_RB_DOWN } Status;
    typedef enum {
        DRAW_NONE,
        DRAW_CROSS,
        DRAW_FOCUS,
        DRAW_BAND,
        DRAW_SOLID_BOX,
        DRAW_BOX,
        DRAW_TILE,
        DRAW_WINDOW,
        DRAW_VLINE
    } RubberType;

    HookType m_type;
    Status m_status;
    BOOLEAN m_isActivated;
    BOOLEAN m_isRepeat;
    CPoint m_sp, m_ep;
    RubberType m_keyNoneDraw, m_keyLBDraw, m_keyRBDraw;
    BOOLEAN m_bRubberDrawn;

  public:
    void DrawRubberCross(const CPoint &point);
    void DrawRubberFocus(const CPoint &point);
    void DrawRubberBand(const CPoint &sp, const CPoint &ep);
    void DrawRubberSolidBox(const CPoint &sp, const CPoint &ep);
    void DrawRubberBox(const CPoint &sp, const CPoint &ep);
    void DrawRubberTile(const CPoint &dp);
    void DrawRubberWindow(const CPoint &point);
    void DrawRubberVLine(const CPoint &point);

  private:
    BOOLEAN Rubber(RubberType);

  public:
    Hook(RubberType keyNoneDraw = DRAW_NONE, RubberType keyLDDraw = DRAW_NONE,
         RubberType keyRDDraw = DRAW_NONE,
         // RB Down will be off MsgMap if it is DRAW_NONE
         BOOLEAN isRepeat = FALSE);

    void Activate();
    void Activate(HookType type);
    void Activate(RubberType keyNoneDraw, RubberType keyLDDraw = DRAW_NONE,
                  RubberType keyRDDraw = DRAW_NONE, BOOLEAN isRepeat = FALSE);
    void Free();
    BOOLEAN IsActivated() { return m_isActivated; }
    void SetType(HookType type) { m_type = type; }
    HookType GetType() { return m_type; }

    RubberType GetKeyNoneDrawType() { return m_keyNoneDraw; }
    RubberType GetKeyLBDrawType() { return m_keyLBDraw; }
    RubberType GetKeyRBDrawType() { return m_keyRBDraw; }

    CPoint StartPoint() { return m_sp; }
    CPoint EndPoint() { return m_ep; }
    CPoint Movement() { return m_ep - m_sp; }

    BOOLEAN IsLeftButtonDown() { return (m_status == KEY_LB_DOWN); }
    BOOLEAN IsRightButtonDown() { return (m_status == KEY_RB_DOWN); }

    void DrawRubber(BOOLEAN bUpdate = FALSE);
    void EraseRubber(BOOLEAN bUpdate = FALSE);

    virtual BOOLEAN OnLButtonDownProc(UINT nFlags, CPoint point);
    virtual BOOLEAN OnLButtonUpProc(UINT nFlags, CPoint point);
    virtual BOOLEAN OnRButtonDownProc(UINT nFlags, CPoint point);
    virtual BOOLEAN OnRButtonUpProc(UINT nFlags, CPoint point);
    virtual BOOLEAN OnMouseMoveProc(UINT nFlags, CPoint point);

    // virtual function to use after Free()
    virtual void EndHook() {}
    // virtual function to use after LB Up in ZOOM_IN
    virtual void ZoomIn(CPoint sp, CPoint ep) {}
    // virtual function to use during LB move in ZOOM_SCROLL
    virtual void DrawScroll(CPoint dp) {}
    // virtual function to use after LB up in ZOOM_SCROLL
    virtual void ZoomScroll(CPoint dp) {}
};

class ZoomF: public Hook {
  public:
    ZoomF() : Hook(DRAW_FOCUS, DRAW_NONE, DRAW_NONE, TRUE) {}
    BOOLEAN OnLButtonDownProc(UINT nFlags, CPoint point);
    virtual void ZoomFocus(CPoint pt) = 0;
};

#endif
