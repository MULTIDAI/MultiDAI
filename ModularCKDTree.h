#pragma once
#include "KDTree.h"
#include "CrackingStrategy.h"

class ModularCKDTree : public KDTree {
  CrackingStrategy* C;
public:
  pointIndexArr query_(KDNodePtr branch, point_t low, point_t high, size_t dim);
  ModularCKDTree() = default; 
  explicit ModularCKDTree(pointVec point_array, CrackingStrategy* C, size_t min_size = 0);
};

