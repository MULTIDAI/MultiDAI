#pragma once
#include "KDTree.h"
#include "CrackingStrategy.h"
#include "Helper.h"
#include <cmath>

class HybridKDTree_level : public KDTree {
private:
  long num_points;
  enum slice_type
    {s_both = 0,
     s_one = 1,
     s_artificial = 2
    };
  std::map<int, std::vector<size_t>> tau;
  KDNodePtrs refine(KDNodePtr branch, point_t low, point_t high, size_t dim, int branch_size);
  slice_type determineSliceType(KDNodePtr branch, point_t low, point_t high, size_t dim);
  KDNodePtrs slice_artificial(KDNodePtr branch, point_t low, point_t high, size_t dim, int branch_size);
  pointIndexArr QUASII_query_(KDNodePtr branch, point_t low, point_t high, size_t dim, int branch_size);
  bp_pair traverse_tree_level(KDNodePtr branch, point_t low, point_t high, size_t level);
  CrackingStrategy* C;
public:
  uint strategy_switch_level = 6;
  pointIndexArr query_(KDNodePtr branch, point_t low, point_t high, size_t level);
  HybridKDTree_level() = default; 
  explicit HybridKDTree_level(pointVec point_array, CrackingStrategy* C, size_t min_size,
                              uint strategy_switch_size);
};
