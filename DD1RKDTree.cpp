/*
 * file: DD1RKDTree.hpp
 * author: J. Frederico Carvalho
 *
 * This is an adaptation of the KD-tree implementation in rosetta code
 * https://rosettacode.org/wiki/K-d_tree
 *
 * It is a reimplementation of the C code using C++.  It also includes a few
 * more queries than the original, namely finding all points at a distance
 * smaller than some given distance to a point.
 *
 */

#include "DD1RKDTree.h"


DD1RKDTree::DD1RKDTree(pointVec point_array, size_t min_size_): KDTree(point_array, min_size_) {}

DD1RKDTree::~DD1RKDTree() = default;


pointIndexArr DD1RKDTree::crack_tree(KDNodePtr branch, point_t low, point_t high,
                                     size_t dim){
  std::uniform_real_distribution<double> unif(branch->min_bound[dim], branch->max_bound[dim]);
  std::default_random_engine re;
  value_t pmid = unif(re);
  KDNodePtrs center_cracks = KDTree::crack_in_two(branch, pmid, dim);

  return crack_query_bounds(low, high, center_cracks, branch, dim);
}

pointIndexArr DD1RKDTree::query_(KDNodePtr branch, point_t low, point_t high, size_t dim) {
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

  return crack_tree(branch, low, high, dim);
}

