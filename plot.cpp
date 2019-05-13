#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include "plot.h"

Toyplot::Toyplot() {
    m_width = 620;
    m_height = 600;
    m_minX = m_minY = 0.;
    m_maxX = m_maxY = 1.;
    strcpy(m_title, "Toyplot");
}

void Toyplot::Refresh() {
    if(!Canvas::IsCreated())
        Create(m_width, m_height, m_title);
    else
        Canvas::Clear();
    Canvas::SetDomain(55, 25, -20, -40);
    Canvas::SetRange(m_minX, m_minY, m_maxX, m_maxY);
    Canvas::DrawGrids(10, 10, 3);
    Canvas::DrawLabels("%.1f", 5, 5);
}

void Toyplot::Flush() {
    Refresh();
    Canvas::Flush();
}

void Toyplot::SetRange(double left, double top, double right, double bottom) {
    if(left > right) ::Swap(left, right);
    if(left == right) right++;
    double orderX = ::GetOrder10(right - left);
    m_minX = ::GetLower(left, orderX);
    m_maxX = ::GetUpper(right, orderX);
    if(top < bottom) ::Swap(top, bottom);
    if(top == bottom) top++;
    double orderY = ::GetOrder10(top - bottom);
    m_minY = ::GetLower(bottom, orderY);
    m_maxY = ::GetUpper(top, orderY);
}

template <class Type> void Toyplot::DrawDots(const Array<Type> &v) {
    SetRange(v.From(), v.To(), v.GetMin(), v.GetMax());
    Refresh();
    LOOP_ARRAY_INT(v, i) Canvas::DrawDot((double)i, (double)v[i]);
    Canvas::Flush();
}

template <class Type> void Toyplot::RedrawDots(const Array<Type> &v) {
    LOOP_ARRAY_INT(v, i) Canvas::DrawDot((double)i, (double)v[i]);
    Flush();
}

// template <class Type> void Toyplot::DrawLines(const Array<Type> &v)
void Toyplot::DrawLines(Vector &v) {
    SetRange(v.From(), v.GetMin(), v.To(), v.GetMax());
    Refresh();
    int prevForeground = Canvas::SetForeground("blue");
    Canvas::DrawYLines(v);
    Canvas::SetForeground(prevForeground);
    Canvas::Flush();
}
