#pragma once
#include <vector>
#include <iostream>
#ifndef KDTREE_H
#include "KDTree.h"
#endif


#ifndef HELPER_H
#define HELPER_H

class H {
public:
  static void print_arr(std::vector< std::pair< std::vector< double >, size_t > > const& );

  template<typename T, typename A>
  static void print_vec( std::vector<T,A> const& );
  
  static void printLeafNode(KDNodePtr );
  static pointVec point_set_2d_grid(value_t );
  static void print_tree_level(KDNodePtr &, int);
  static void print_tree(KDTree &);
  static bool verify_node_bounds(KDNodePtr );
  static void print_first_kdnodeptrs(KDNodePtrs nodes);
  static pointIndexArr node_to_point_index_arr(KDNodePtr node);
  static pointIndexArr nodes_to_point_index_arr(KDNodePtrs nodes);
  static bool verify_final_partition_size(KDNodePtr branch, size_t min_size);
  static bool intersection(value_t x_low, value_t x_high, value_t y_low, value_t y_high);
  static void find_wrong_nodes(KDNodePtr branch);
  static KDNodePtrs find_leafs(KDNodePtr branch);
};
#endif

