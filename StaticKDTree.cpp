/*
 * file: StaticKDTree.hpp
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

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <vector>
#include <iostream>

#include "StaticKDTree.h"

StaticKDNode::StaticKDNode() = default;

StaticKDNode::StaticKDNode(const point_t &pt, const size_t &idx_, const StaticKDNodePtr &left_,
                           const StaticKDNodePtr &right_) {
  x = pt;
  index = idx_;
  left = left_;
  right = right_;
}

StaticKDNode::StaticKDNode(const pointIndex &pi, const StaticKDNodePtr &left_,
                           const StaticKDNodePtr &right_) {
  x = pi.first;
  index = pi.second;
  left = left_;
  right = right_;
}

StaticKDNode::~StaticKDNode() = default;

double StaticKDNode::coord(const size_t &idx) { return x.at(idx); }
StaticKDNode::operator bool() { return (!x.empty()); }
StaticKDNode::operator point_t() { return x; }
StaticKDNode::operator size_t() { return index; }
StaticKDNode::operator pointIndex() { return pointIndex(x, index); }

StaticKDNodePtr NewStaticKDNodePtr() {
  StaticKDNodePtr mynode = std::make_shared< StaticKDNode >();
  return mynode;
}

inline double dist2(const point_t &a, const point_t &b) {
  double distc = 0;
  for (size_t i = 0; i < a.size(); i++) {
    double di = a.at(i) - b.at(i);
    distc += di * di;
  }
  return distc;
}

inline double dist2(const StaticKDNodePtr &a, const StaticKDNodePtr &b) {
  return dist2(a->x, b->x);
}

static_comparer::static_comparer(size_t idx_) : idx{idx_} {};

inline bool static_comparer::compare_idx(const pointIndex &a,  //
                                         const pointIndex &b   //
                                         ) {
  return (a.first.at(idx) < b.first.at(idx));  //
}

inline void sort_on_idx(const pointIndexArr::iterator &begin,  //
                        const pointIndexArr::iterator &end,    //
                        size_t idx) {
  static_comparer comp(idx);
  comp.idx = idx;

  using std::placeholders::_1;
  using std::placeholders::_2;

  std::sort(begin, end, std::bind(&static_comparer::compare_idx, comp, _1, _2));
}

using pointVec = std::vector< point_t >;

StaticKDNodePtr StaticKDTree::make_tree(const pointIndexArr::iterator &begin,  //
                                        const pointIndexArr::iterator &end,    //
                                        const size_t &length,                  //
                                        const size_t &level                    //
                                        ) {
  if (begin == end) {
    return NewStaticKDNodePtr();  // empty tree
  }

  size_t dim = begin->first.size();

  if (length > 1) {
    sort_on_idx(begin, end, level);
  }

  auto middle = begin + (length / 2);

  auto l_begin = begin;
  auto l_end = middle;
  auto r_begin = middle + 1;
  auto r_end = end;

  size_t l_len = length / 2;
  size_t r_len = length - l_len - 1;

  StaticKDNodePtr left;
  if (l_len > 0 && dim > 0) {
    left = make_tree(l_begin, l_end, l_len, (level + 1) % dim);
  } else {
    left = leaf;
  }
  StaticKDNodePtr right;
  if (r_len > 0 && dim > 0) {
    right = make_tree(r_begin, r_end, r_len, (level + 1) % dim);
  } else {
    right = leaf;
  }

  // StaticKDNode result = StaticKDNode();
  return std::make_shared< StaticKDNode >(*middle, left, right);
}

StaticKDTree::StaticKDTree(pointVec point_array, size_t min_size_) {
  min_size = min_size_;
  leaf = std::make_shared< StaticKDNode >();
  // iterators
  pointIndexArr arr;
  for (size_t i = 0; i < point_array.size(); i++) {
    arr.push_back(pointIndex(point_array.at(i), i));
  }

  auto begin = arr.begin();
  auto end = arr.end();

  size_t length = arr.size();
  size_t level = 0;  // starting

  root = StaticKDTree::make_tree(begin, end, length, level);
}

StaticKDTree::~StaticKDTree() = default;

StaticKDNodePtr StaticKDTree::nearest_(   //
                                       const StaticKDNodePtr &branch,  //
                                       const point_t &pt,        //
                                       const size_t &level,      //
                                       const StaticKDNodePtr &best,    //
                                       const double &best_dist   //
                                          ) {
  double d, dx, dx2;

  if (!bool(*branch)) {
    return NewStaticKDNodePtr();  // basically, null
  }

  point_t branch_pt(*branch);
  size_t dim = branch_pt.size();

  d = dist2(branch_pt, pt);
  dx = branch_pt.at(level) - pt.at(level);
  dx2 = dx * dx;

  StaticKDNodePtr best_l = best;
  double best_dist_l = best_dist;

  if (d < best_dist) {
    best_dist_l = d;
    best_l = branch;
  }

  size_t next_lv = (level + 1) % dim;
  StaticKDNodePtr section;
  StaticKDNodePtr other;

  // select which branch makes sense to check
  if (dx > 0) {
    section = branch->left;
    other = branch->right;
  } else {
    section = branch->right;
    other = branch->left;
  }

  // keep nearest neighbor from further down the tree
  StaticKDNodePtr further = nearest_(section, pt, next_lv, best_l, best_dist_l);
  if (!further->x.empty()) {
    double dl = dist2(further->x, pt);
    if (dl < best_dist_l) {
      best_dist_l = dl;
      best_l = further;
    }
  }
  // only check the other branch if it makes sense to do so
  if (dx2 < best_dist_l) {
    further = nearest_(other, pt, next_lv, best_l, best_dist_l);
    if (!further->x.empty()) {
      double dl = dist2(further->x, pt);
      if (dl < best_dist_l) {
        best_dist_l = dl;
        best_l = further;
      }
    }
  }

  return best_l;
};

// default caller
StaticKDNodePtr StaticKDTree::nearest_(const point_t &pt) {
  size_t level = 0;
  // StaticKDNodePtr best = branch;
  double branch_dist = dist2(point_t(*root), pt);
  return nearest_(root,          // beginning of tree
                  pt,            // point we are querying
                  level,         // start from level 0
                  root,          // best is the root
                  branch_dist);  // best_dist = branch_dist
};

point_t StaticKDTree::nearest_point(const point_t &pt) {
  return point_t(*nearest_(pt));
};
size_t StaticKDTree::nearest_index(const point_t &pt) {
  return size_t(*nearest_(pt));
};

pointIndex StaticKDTree::nearest_pointIndex(const point_t &pt) {
  StaticKDNodePtr Nearest = nearest_(pt);
  return pointIndex(point_t(*Nearest), size_t(*Nearest));
}

pointIndexArr StaticKDTree::neighborhood_(  //
                                          const StaticKDNodePtr &branch,          //
                                          const point_t &pt,                //
                                          const double &rad,                //
                                          const size_t &level               //
                                            ) {
  double d, dx, dx2;

  if (!bool(*branch)) {
    // branch has no point, means it is a leaf,
    // no points to add
    return pointIndexArr();
  }

  size_t dim = pt.size();

  double r2 = rad * rad;

  d = dist2(point_t(*branch), pt);
  dx = point_t(*branch).at(level) - pt.at(level);
  dx2 = dx * dx;

  pointIndexArr nbh, nbh_s, nbh_o;
  if (d <= r2) {
    nbh.push_back(pointIndex(*branch));
  }

  //
  StaticKDNodePtr section;
  StaticKDNodePtr other;
  if (dx > 0) {
    section = branch->left;
    other = branch->right;
  } else {
    section = branch->right;
    other = branch->left;
  }

  nbh_s = neighborhood_(section, pt, rad, (level + 1) % dim);
  nbh.insert(nbh.end(), nbh_s.begin(), nbh_s.end());
  if (dx2 < r2) {
    nbh_o = neighborhood_(other, pt, rad, (level + 1) % dim);
    nbh.insert(nbh.end(), nbh_o.begin(), nbh_o.end());
  }

  return nbh;
};

pointIndexArr StaticKDTree::neighborhood(  //
                                         const point_t &pt,               //
                                         const double &rad) {
  size_t level = 0;
  return neighborhood_(root, pt, rad, level);
}

pointVec StaticKDTree::neighborhood_points(  //
                                           const point_t &pt,                 //
                                           const double &rad) {
  size_t level = 0;
  pointIndexArr nbh = neighborhood_(root, pt, rad, level);
  pointVec nbhp;
  nbhp.resize(nbh.size());
  std::transform(nbh.begin(), nbh.end(), nbhp.begin(),
                 [](pointIndex x) { return x.first; });
  return nbhp;
}

indexArr StaticKDTree::neighborhood_indices(  //
                                            const point_t &pt,                  //
                                            const double &rad) {
  size_t level = 0;
  pointIndexArr nbh = neighborhood_(root, pt, rad, level);
  indexArr nbhi;
  nbhi.resize(nbh.size());
  std::transform(nbh.begin(), nbh.end(), nbhi.begin(),
                 [](pointIndex x) { return x.second; });
  return nbhi;
}

void print_pointt(point_t p){
  std::cout << "(";
  for (auto it = p.begin(); it != p.end(); it++){
    std::cout << (*it) << ", "; 
  }
  std::cout << ")";
}

bool compare_points(point_t p1, point_t p2, size_t dim){
  std::cout << "comparing on dim: " << dim << " values: " << p1[dim] << ", " << p2[dim] <<"\n";
  return (p1[dim] < p2[dim]);
}

pointIndexArr StaticKDTree::static_query_(StaticKDNodePtr branch, point_t low, point_t high, size_t dim){
  pointIndexArr ret;
  if (!bool(*branch)) {
    // branch has no point, means it is a leaf,
    // no points to add
    return pointIndexArr();
  }

  auto low_it = low.begin();
  auto high_it = high.begin();

  bool inside = true;
  // For every dimension, test if point is within bounds
  for (double v:branch->x){
    inside &= (*low_it) <= v && v < (*high_it); 
    low_it++;
    high_it++;
  }
  if (inside){ // inside
    ret.push_back(pointIndex(*branch));
  }

  if (branch->x[dim] > low[dim]){
    pointIndexArr res = static_query_(branch->left, low, high, (dim+1) % low.size());
    ret.insert(ret.end(), res.begin(), res.end());
  }
  if (branch->x[dim] < high[dim]){
    pointIndexArr res = static_query_(branch->right, low, high, (dim+1) % low.size());
    ret.insert(ret.end(), res.begin(), res.end());
  }
  return ret;
}

pointIndexArr StaticKDTree::query(point_t low, point_t high){
  // std::cout << "querying from "; print_pointt(low); std::cout << " to "; print_pointt(high); std::cout << "\n";
  return static_query_(root, low, high, 0);
}
