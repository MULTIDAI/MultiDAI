#pragma once
#include "KDTree.h"
#include <iostream>
#include <vector>


class LazyCKDTree : public KDTree {
protected:
  pointIndexArr crack_tree(KDNodePtr branch, point_t low, point_t high, size_t dim);
public:
  virtual pointIndexArr query_(KDNodePtr branch, point_t low, point_t high, size_t dim);
  LazyCKDTree() = default; 
  explicit LazyCKDTree(pointVec point_array, size_t min_size = 0);
};
