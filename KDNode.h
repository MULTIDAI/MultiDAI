#pragma once
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <iterator>
#include <limits>
#include <float.h>

#define OUT std::cout << __FUNCTION__ << ": "

using value_t = double;
using point_t = typename std::vector< value_t >;
using indexArr = typename std::vector< ulong >;
using pointIndex = typename std::pair< point_t, ulong >;
using pointIndexArr = typename std::vector< pointIndex >;
using bp_pair = std::pair<bool, pointIndexArr>;

class KDNode {
public:
  using KDNodePtr = typename std::shared_ptr< KDNode >;
  using KDNodePtrs = typename std::shared_ptr< std::vector<KDNodePtr> >;
  using ci_type = typename std::map<value_t, KDNodePtr>;
  using ci_iter = typename ci_type::iterator;
  size_t index;
  point_t x;
  pointIndexArr::iterator begin;
  pointIndexArr::iterator end;
  point_t min_bound;
  point_t max_bound;
  size_t level = 0;
  size_t dim;
  std::map<value_t, KDNodePtr> CI;
private:

public:
  // initializer
  KDNode(const pointIndexArr::iterator &,  //
         const pointIndexArr::iterator &, size_t, size_t);
  KDNode(const pointIndexArr::iterator &,  //
         const pointIndexArr::iterator &, point_t &, point_t &, size_t);
  KDNode(const point_t &, const size_t &, 
         const pointIndexArr::iterator &,  //
         const pointIndexArr::iterator &, size_t);
  KDNode(const pointIndex &, 
         const pointIndexArr::iterator &,  //
         const pointIndexArr::iterator &, size_t);
  ~KDNode();

  // getter
  value_t coord(const size_t &);
  KDNodePtr get_slice(value_t);
  size_t get_size();
  size_t get_partition_size();
  ci_iter find_slice(value_t );
  // setter
  void add_slice(value_t, KDNodePtr);
  KDNodePtr add_default_child();
  KDNodePtr add_child(size_t dim_);


  // conversions
  explicit operator bool();
  explicit operator point_t();
  explicit operator size_t();
  explicit operator pointIndex();
};

using KDNodePtr = typename std::shared_ptr< KDNode >;
using KDNodePair = typename std::pair<KDNodePtr, KDNodePtr>;
using KDNodePtrs = typename std::shared_ptr< std::vector<KDNodePtr> >;
using ci_type = typename std::map<value_t, KDNodePtr>;
using ci_iter = typename ci_type::iterator;
KDNodePtr new_KDNodePtr();
KDNodePtrs new_KDNodePtrs(KDNodePtr n = nullptr);

inline double dist(const point_t &, const point_t &);
inline double dist(const KDNodePtr &, const KDNodePtr &);

using pointVec = std::vector< point_t >;
