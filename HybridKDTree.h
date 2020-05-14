#pragma once
#include "KDTree.h"
#include "CrackingStrategy.h"
#include "Helper.h"
#include <cmath>

class HybridKDTree : public KDTree {
private:
  long num_points;
  enum slice_type
    {s_both = 0,
     s_one = 1,
     s_artificial = 2
    };
  std::vector<size_t> r;
  uint strategy_switch_size;
  KDNodePtrs refine(KDNodePtr branch, point_t low, point_t high, size_t dim);
  slice_type determineSliceType(KDNodePtr branch, point_t low, point_t high, size_t dim);
  KDNodePtrs slice_artificial(KDNodePtr branch, point_t low, point_t high, size_t dim);
  pointIndexArr QUASII_query_(KDNodePtr branch, point_t low, point_t high, size_t dim);
  CrackingStrategy* C;
public:
  pointIndexArr query_(KDNodePtr branch, point_t low, point_t high, size_t dim);
  HybridKDTree() = default; 
  explicit HybridKDTree(pointVec point_array, CrackingStrategy* C, size_t min_size,
                        uint strategy_switch_size);
};
