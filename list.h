// Toy/list.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_LIST_H_
#define _TOY_LIST_H_

#define WhileList(list, node)                                                  \
    for(node = (list)->First(); node != list; node = node->Next)

template <class Type> class Node {
    // Data
  public:
    Type *Next, *Prev;

    // Constructor
  public:
    Node() { Next = Prev = (Type *)this; }

    // Operators
    Type *InsertNext(Type *node) {
        if(Next) Next->Prev = node;
        node->Next = Next;
        node->Prev = (Type *)this;
        Next = node;
        return node;
    }
    Type *InsertPrev(Type *node) {
        if(node->Prev) Prev->Next = node;
        node->Prev = Prev;
        node->Next = (Type *)this;
        Prev = node;
        return node;
    }
    Type *Cut() {
        Next->Prev = Prev;
        Prev->Next = Next;
        return (Type *)this;
    }
    Type *MoveNext(Type *node) { return InsertNext(node->Cut()); }
    Type *MovePrev(Type *node) { return InsertPrev(node->Cut()); }
    Type *DeleteNext() {
        Type *next = Next;
        Next = Next->Next;
        Next->Prev = (Type *)this;
        delete next;
        return Next;
    }
    Type *DeletePrev() {
        Type *prev = Prev;
        Prev = Prev->Prev;
        Prev->Next = (Type *)this;
        delete prev;
        return Prev;
    }
};

template <class Type> class LinkedList: public Node<Type> {
    // Constructor and Destructor
  public:
    LinkedList() {}
    ~LinkedList() { Free(); }

    // Operators
  public:
    Type *Append(Type *node) { return node ? InsertPrev(node) : NULL; }
    Type *Move(Type *node) { return Append(node->Cut()); }
    void Free() {
        while(Node<Type>::Next != (Node<Type> *)this) Node<Type>::DeleteNext();
    }
    BOOLEAN IsEmpty() { return (Node<Type>::Next == this) ? TRUE : FALSE; }
    void Move(LinkedList<Type> *list) {
        ::Swap(Node<Type>::Next, list->Next);
        ::Swap(Node<Type>::Prev, list->Prev);
    }
    void DeleteFirst() { Node<Type>::DeleteNext(); }
    void DeleteLast() { Node<Type>::DeletePrev(); }
    BOOLEAN Delete(Type *node) {
        for(Type *_node = First(); _node != this; _node = _node->Next)
            if(node == _node) {
                node->Prev->DeleteNext();
                return TRUE;
            }
        return FALSE;
    }
    BOOLEAN Delete(int pos) { return Delete(GetNode(pos)); }
    int GetLength() {
        int size = 0;
        for(Type *node = First(); node != this; node = node->Next) size++;
        return size;
    }
    int GetPos(Type *node) const {
        int pos = 0;
        for(Type *_node = First(); _node != this; _node = _node->Next) {
            if(node == _node) return pos;
            pos++;
        }
        return -1;
    }

    Type *First() const { return Node<Type>::Next; }
    Type *Second() const { return (Node<Type>::Next)->Next; }
    Type *Last() const { return Node<Type>::Prev; }
    Type *GetNode(int pos) {
        Type *node;
        for(node = First(); node != this; node = node->Next, pos--)
            if(pos <= 0) return node;
        return node;
    }
    void Swap(int pos1, int pos2) {
        Node<Type> *node1 = GetNode(pos1);
        Node<Type> *node2 = GetNode(pos2);
        ::Swap(node1->Prev, node2->Prev);
        ::Swap(node1->Next, node2->Next);
    }
    void Sort() {
        BOOLEAN bAgain;
        do {
            bAgain = FALSE;
            for(Type *node = First(); node != this; node = node->Next)
                if(*node < *(node->next)) {
                    node = node->Prev->MoveNext(node->Next);
                    bAgain = TRUE;
                }
        } while(bAgain);
    }
};

#define LOOP_LIST(_list_, _node_)                                              \
    for(_node_ = (_list_).Next; _node_ != (&_list_); _node_ = _node_->Next)
#define DO_LIST(_list_, _node_) while((_node_ = (_node_)->Next) != &_list_)

#endif
