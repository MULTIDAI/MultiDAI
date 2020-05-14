#pragma once
#include "LazyCKDTree.h"

class CKDTree : public LazyCKDTree {
  
public:
  CKDTree() = default; 
  explicit CKDTree(pointVec point_array, size_t min_size = 0);
  virtual pointIndexArr query_(KDNodePtr branch, point_t low, point_t high, size_t dim);

protected:
  // bp_pair check_inclusion(KDNodePtr branch, point_t low, point_t high);
  bp_pair traverse_tree(KDNodePtr branch, point_t low, point_t high, size_t dim);
  std::pair<int, KDNodePtrs > crack_tree(KDNodePtr branch, point_t low, point_t high, size_t dim);
  pointIndexArr crack_tree_and_recurse(KDNodePtr branch, point_t low, point_t high, size_t dim);
  pointIndexArr query_irrelevant_branch(KDNodePtr &branch, point_t low, point_t high, double new_low_val, double new_high, size_t dim);
  point_t alter_query(point_t query, value_t new_val, int dim);
};
