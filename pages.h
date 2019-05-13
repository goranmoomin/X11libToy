// Toy/pages.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_PAGES_H_
#define _TOY_PAGES_H_

#ifdef TOY
#include <Toy/vector.h>
#else
#include "vector.h"
#endif

#ifdef _DEBUG
#define CHECK_PAGE_RANGE(v, i)                                                 \
    if((i) < 0 || (i) >= (v).PageSize() * (v).PageLength())                    \
    Error("Out of range in class PagedList")
#else
#define CHECK_PAGE_RANGE(v, i)
#endif

template <class Type> class PagedList {
    Array<Type *> m_pagePointer; // poiner table of pages
    int m_pageSize;              // data size per page
    int m_size;

    // Constructor and Destructor
  public:
    PagedList(int pageSize = 1) {
        m_pagePointer.SetBlockSize(64);
        m_pageSize = pageSize;
        m_size = 0;
    }
    ~PagedList() { Free(); }

    // Operators
    void Free() {
        LOOP_ARRAY_INT(m_pagePointer, i) delete m_pagePointer[i];
        m_pagePointer.SetSize(0);
        m_size = 0;
    }
    void SetPageSize(int pageSize) {
        if(m_size > 0 && pageSize != m_pageSize) {
            PagedList<Type> pages(pageSize);
            for(int i = 0; i < m_size; i++) pages.Push((*this)[i]);
            ::Swap(m_pagePointer, pages.m_pagePointer);
            ::Swap(m_pageSize, pages.m_pageSize);
        } else
            m_pageSize = pageSize;
    }
    int PageSize() const { return m_pageSize; }
    int PageLength() const { return m_pagePointer.Size(); }
    int Size() const { return m_size; }
    Type Push(const Type &t) {
        if(m_size % m_pageSize == 0) m_pagePointer.Push(new Type[m_pageSize]);
        (*this)[m_size] = t;
        m_size++;
        return t;
    }
    Type Pop() {
        m_size--;
        Type t = (*this)[m_size];
        if(m_size % m_pageSize == 0) delete m_pagePointer.Pop();
        return t;
    }
    Type First() { return (*this)[0]; }
    Type Last() { return (*this)[m_size - 1]; }
    template <class INT> Type *operator()(INT i) {
        CHECK_PAGE_RANGE(*this, i);
        return m_pagePointer[i / m_pageSize] + (i % m_pageSize);
    }
    template <class INT> Type &operator[](INT i) {
        CHECK_PAGE_RANGE(*this, i);
        return m_pagePointer[i / m_pageSize][i % m_pageSize];
    }
};

#endif
