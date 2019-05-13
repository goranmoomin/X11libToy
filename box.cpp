#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#endif
#include "box.h"
#include "clip.h"
#include "graph.h"
#include "mutil.h"

template <> int Box2D<int>::Width() const { return right - left + 1; }
template <> int Box2D<int>::Height() const { return top - bottom + 1; }
template <> float Box2D<float>::Width() const { return right - left; }
template <> float Box2D<float>::Height() const { return top - bottom; }
template <> double Box2D<double>::Width() const { return right - left; }
template <> double Box2D<double>::Height() const { return top - bottom; }

template <> int Box3D<int>::Width() const { return right - left + 1; }
template <> int Box3D<int>::Height() const { return top - bottom + 1; }
template <> int Box3D<int>::Depth() const { return upper - lower + 1; }
template <> float Box3D<float>::Width() const { return right - left; }
template <> float Box3D<float>::Height() const { return top - bottom; }
template <> float Box3D<float>::Depth() const { return upper - lower; }
template <> double Box3D<double>::Width() const { return right - left; }
template <> double Box3D<double>::Height() const { return top - bottom; }
template <> double Box3D<double>::Depth() const { return upper - lower; }
