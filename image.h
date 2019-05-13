// Toy/image.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_IMAGE_H_
#define _TOY_IMAGE_H_

#include "matrix.h"

class Image: public DoubleArray<BYTE> {
  private:
    IMatrix nbhd;

    void GetNeighborhood();

  public:
    BOOLEAN LoadPGM(const char *filename);
    void MakeMonochrome(BYTE th);
    void Erode(int nRepeat = 1);
    void Dilate(int nRepeat = 1);
};

#endif
