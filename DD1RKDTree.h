#pragma once
/*
 * file: DD1RKDTree.hpp
 * author: J. Frederico Carvalho
 *
 * This is an adaptation of the KD-tree implementation in rosetta code
 *  https://rosettacode.org/wiki/K-d_tree
 * It is a reimplementation of the C code using C++.
 * It also includes a few more queries than the original
 *
 */
#include <random>
#include "KDTree.h"

class DD1RKDTree : public KDTree {
  pointIndexArr crack_tree(KDNodePtr branch, point_t low, point_t high,
                           size_t dim);

public:
  DD1RKDTree() = default;
  explicit DD1RKDTree(pointVec point_array, size_t min_size = 0);
  pointIndexArr query_(KDNodePtr branch, point_t low, point_t high, size_t dim);
  ~DD1RKDTree();
};

