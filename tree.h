// Toy/tree.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_TREE_H_
#define _TOY_TREE_H_

template <class Type> class CTree {
    // Data
  public:
    Type *Parent, *Left, *Right;
    int Layer;

    // Constructor
  public:
    CTree() {
        Left = Right = NULL;
        Layer = 0;
    }
    ~CTree() {
        if(Left) delete Left;
        if(Right) delete Right;
    }

    // Operators
    Type *MakeLeft() {
        Left = new Type;
        Left->Layer = Layer + 1;
        return Left;
    }
    Type *MakeRight() {
        Right = new Type;
        Right->Layer = Layer + 1;
        return Right;
    }
    void TreeScan(void (*function)(Type &tree)) {
        if(Left) Left->TreeScan(function);
        function((Type &)(*this));
        if(Right) Right->TreeScan(function);
    }
};

#endif
