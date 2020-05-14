#include "ModularCKDTree.h"

ModularCKDTree::ModularCKDTree(pointVec point_array_, CrackingStrategy* C_, size_t min_size_) :
  KDTree(point_array_, min_size_), C(C_) {}


pointIndexArr ModularCKDTree::query_(KDNodePtr branch, point_t low, point_t high, size_t dim) {
  bp_pair subset_check = check_inclusion(branch, low, high);
  if (subset_check.first){
    return subset_check.second;
  }
  
  bp_pair traverse_check = traverse_tree(branch, low, high, dim);
  if (traverse_check.first){
    return traverse_check.second;
  }

  if (branch->get_partition_size() < min_size){
    return final_partition_extract(branch, low, high);
  }

  return C->crack(branch, low, high, dim, this);
}
