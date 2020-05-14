#pragma once
/*
 * file: KDTree.hpp
 * author: J. Frederico Carvalho
 *
 * This is an adaptation of the KD-tree implementation in rosetta code
 *  https://rosettacode.org/wiki/K-d_tree
 * It is a reimplementation of the C code using C++.
 * It also includes a few more queries than the original
 *
 */
#include "KDNode.h"


class KDTree {

  KDNodePtr make_tree(const pointIndexArr::iterator &begin,  //
                      const pointIndexArr::iterator &end    //
                      );
protected:
  enum query_loc {left = 0,
                  left_single = 1,
                  right = 2,
                  right_single = 3,
                  both = 4
  };
  query_loc query_location(point_t lower_bound, point_t middle, point_t upper_bound, point_t low, point_t high, size_t dim); 

  bp_pair check_inclusion(KDNodePtr branch, point_t low, point_t high);
  bp_pair traverse_tree(KDNodePtr branch, point_t low, point_t high, size_t dim);
  pointIndexArr crack_query_bounds(point_t low, point_t high, KDNodePtrs strategic_cracks,
                                   KDNodePtr branch, size_t dim);


public:
  size_t min_size;
  KDNodePtr root;
  KDTree() = default;
  explicit KDTree(pointVec point_array, size_t min_size = 0);
  pointIndexArr arr;
  static KDNodePtrs crack_in_two(KDNodePtr &, value_t, size_t);
  static KDNodePtrs crack_in_three(KDNodePtr &, value_t, value_t, size_t);
  static std::pair<KDNodePtrs, pointIndexArr> m_crack_in_two(KDNodePtr &, value_t pivot, point_t low, point_t high, size_t dim);
  virtual pointIndexArr query(point_t, point_t);
  virtual pointIndexArr query_(KDNodePtr branch, point_t low, point_t high, size_t dim);
  virtual ~KDTree();
  static pointIndexArr final_partition_extract(KDNodePtr branch, point_t low, point_t high);
};

