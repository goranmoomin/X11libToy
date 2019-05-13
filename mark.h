// Toy/mark.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_MARK_H_
#define _TOY_MARK_H_

#include <Toy/vector.h>

class Mark: public Array<BOOLEAN> {
    double m_min, m_max;
    int m_count;

  public:
    // Constructor
    Mark(double min, double max, int grid) : Array(grid + 1) {
        m_min = min;
        m_max = max;
        Reset();
    }

    //  Operator
    void Reset() {
        Array::operator=((BOOLEAN)FALSE);
        m_count = 0;
    }
    void Set(double value) {
        int m = (int)((value - m_min) / (m_max - m_min) * (To - From)) + From;
        if(m < From || m > To) return;
        if((*this)[m] == FALSE) m_count++;
        (*this)[m] = TRUE;
    }
    double GetOffset(int tab) {
        return (double)((m_max - m_min) * (tab - From) / (To - From));
    }
    int GetCount() { return m_count; }
    double GetMin() {
        if(m_count > 0)
            for(int i = From; i <= To; i++)
                if((*this)[i]) return GetOffset(i);
        return m_max; // It'll be meaningless.
    }
    double GetMax() {
        if(m_count > 0)
            for(int i = To(); i >= From; i--)
                if((*this)[i]) return GetOffset(i);
        return m_min; // It'll be meaningless.
    }
};

#endif
