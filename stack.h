// Toy/stack.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_STACK_H_
#define _TOY_STACK_H_

template <class Type> class Stack {
    Type *m_stack;
    int m_size;
    int m_now;

  public:
    Stack(int size) {
        m_size = size;
        m_stack = size > 0 ? new Type[size] : NULL;
        m_now = -1;
    }
    ~Stack() {
        if(m_stack) delete[] m_stack;
    }
    void Reset() { m_now = -1; }
    void Push(const Type &type) {
        if(m_now + 1 >= m_size)
            for(int i = 1; i < m_size; i++) m_stack[i - 1] = m_stack[i];
        else
            m_now++;
        m_stack[m_now] = type;
    }
    BOOLEAN Pop() {
        if(m_now >= 0) {
            m_now--;
            return TRUE;
        }
        return FALSE;
    }
    BOOLEAN Pop(Type &type) {
        if(m_now < 0) return FALSE;
        type = m_stack[m_now];
        m_now--;
        return TRUE;
    }
};

#endif
