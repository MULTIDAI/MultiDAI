#pragma once
#include "KDTree.h"
/*
 * file: StaticKDTree.hpp
 * author: J. Frederico Carvalho
 *
 * This is an adaptation of the KD-tree implementation in rosetta code
 *  https://rosettacode.org/wiki/K-d_tree
 * It is a reimplementation of the C code using C++.
 * It also includes a few more queries than the original
 *
 */

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

// using point_t = std::vector< double >;
// using indexArr = std::vector< size_t >;
// using pointIndex = typename std::pair< std::vector< double >, size_t >;

class StaticKDNode {
public:
  using StaticKDNodePtr = std::shared_ptr< StaticKDNode >;
  size_t index;
  point_t x;
  StaticKDNodePtr left;
  StaticKDNodePtr right;

  // initializer
  StaticKDNode();
  StaticKDNode(const point_t &, const size_t &, const StaticKDNodePtr &,
               const StaticKDNodePtr &);
  StaticKDNode(const pointIndex &, const StaticKDNodePtr &, const StaticKDNodePtr &);
  ~StaticKDNode();

  // getter
  double coord(const size_t &);

  // conversions
  explicit operator bool();
  explicit operator point_t();
  explicit operator size_t();
  explicit operator pointIndex();
};

using StaticKDNodePtr = std::shared_ptr< StaticKDNode >;
using StaticKDNodePtrs = typename std::shared_ptr< std::vector<StaticKDNodePtr> >;


StaticKDNodePtr NewStaticKDNodePtr();

inline double dist(const point_t &, const point_t &);
inline double dist(const StaticKDNodePtr &, const StaticKDNodePtr &);

// Need for sorting
class static_comparer {
public:
  size_t idx;
  explicit static_comparer(size_t idx_);
  inline bool compare_idx(
                          const std::pair< std::vector< double >, size_t > &,  //
                          const std::pair< std::vector< double >, size_t > &   //
                          );
};

using pointIndexArr = typename std::vector< pointIndex >;

inline void sort_on_idx(const pointIndexArr::iterator &,  //
                        const pointIndexArr::iterator &,  //
                        size_t idx);

using pointVec = std::vector< point_t >;

class StaticKDTree : public KDTree {
  StaticKDNodePtr root;
  StaticKDNodePtr leaf;
  StaticKDNodePtr make_tree(const pointIndexArr::iterator &begin,  //
                            const pointIndexArr::iterator &end,    //
                            const size_t &length,                  //
                            const size_t &level                    //
                            );

public:
  bool do_verify = true;
  StaticKDTree() = default;
  explicit StaticKDTree(pointVec point_array, size_t min_size = 0);

private:
  StaticKDNodePtr nearest_(           //
                           const StaticKDNodePtr &branch,  //
                           const point_t &pt,        //
                           const size_t &level,      //
                           const StaticKDNodePtr &best,    //
                           const double &best_dist   //
                                      );

  // default caller
  StaticKDNodePtr nearest_(const point_t &pt);

public:
  point_t nearest_point(const point_t &pt);
  size_t nearest_index(const point_t &pt);
  pointIndex nearest_pointIndex(const point_t &pt);

protected:
  pointIndexArr static_query_(StaticKDNodePtr branch, point_t low, point_t high, size_t dim);
public:
  pointIndexArr query(point_t, point_t);
private:
  pointIndexArr neighborhood_(  //
                              const StaticKDNodePtr &branch,  //
                              const point_t &pt,        //
                              const double &rad,        //
                              const size_t &level       //
                                );

public:
  pointIndexArr neighborhood(  //
                             const point_t &pt,       //
                             const double &rad);

  pointVec neighborhood_points(  //
                               const point_t &pt,         //
                               const double &rad);

  indexArr neighborhood_indices(  //
                                const point_t &pt,          //
                                const double &rad);
  ~StaticKDTree();
};
