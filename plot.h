// Toy/plot.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_PLOT_H_
#define _TOY_PLOT_H_

#ifdef TOY
#include <Toy/canvas.h>
#else
#include "canvas.h"
#endif

class Toyplot: public Canvas {
    int m_width, m_height;
    char m_title[64];
    double m_minX, m_minY, m_maxX, m_maxY;

  public:
    Toyplot();

    void SetWidth(int width) { m_width = width; }
    void SetHeight(int height) { m_height = height; }
    void SetSize(int width, int height) {
        m_width = width;
        m_height = height;
    }
    void SetTitle(const char *title) { strcpy(m_title, title); }
    void Refresh();
    void Flush();
    void SetRange(double left, double top, double right, double bottom);

    template <class Type> void DrawDots(const Array<Type> &v);
    template <class Type> void RedrawDots(const Array<Type> &v);
    // template <class Type> void DrawLines(const Array<Type> &v);
    void DrawLines(Vector &v);
};

#endif
