#pragma once
#include "KDTree.h"
#include <iostream>
#include <vector>

enum slice_type
  {s_both = 0,
   s_one = 1,
   s_artificial = 2
  };

class QUASII : public KDTree {
private:
  std::vector<size_t> tau;
  KDNodePtrs refine(KDNodePtr branch, point_t low, point_t high, size_t dim);
  slice_type determineSliceType(KDNodePtr branch, point_t low, point_t high, size_t dim);
  KDNodePtrs slice_artificial(KDNodePtr branch, point_t low, point_t high, size_t dim);
protected:
  pointIndexArr query_(KDNodePtr branch, point_t low, point_t high, size_t dim);
public:
  QUASII() = default; 
  explicit QUASII(pointVec point_array, size_t min_size);
};
