#include "KDNode.h"

#define MIN_VAL -41
#define MAX_VAL 401

KDNode::KDNode(const pointIndexArr::iterator &begin_, 
               const pointIndexArr::iterator &end_, size_t dim_, size_t dims):
  begin{begin_}, end{end_}, dim{dim_} {
    x = {};
    index = 0;
    max_bound = {};
    for (ulong i = 0; i < dims; i++){
      max_bound.push_back(MAX_VAL);
    }
    min_bound = {}; 
    for (ulong i = 0; i < dims; i++){
      min_bound.push_back(MIN_VAL);
    }
                            }


KDNode::KDNode(const pointIndexArr::iterator &begin_, 
               const pointIndexArr::iterator &end_,
               point_t &min_bound_, point_t &max_bound_, size_t dim_):
  begin{begin_}, end{end_}, dim{dim_} {
    x = {};
    index = 0;
    min_bound = min_bound_;
    max_bound = max_bound_;
    
                            }


KDNode::KDNode(const point_t &pt, const size_t &idx_, const pointIndexArr::iterator &begin_, 
               const pointIndexArr::iterator &end_, size_t dim_):
  begin{begin_}, end{end_}, dim{dim_}  {
    x = pt;
    index = idx_;
    begin = begin_;
    end = end_;

                            }

KDNode::KDNode(const pointIndex &pi, 
               const pointIndexArr::iterator &begin_,
               const pointIndexArr::iterator &end_, size_t dim_):
  begin{begin_}, end{end_}, dim{dim_} {
    x = pi.first;
    index = pi.second;
    begin = begin_;
    end = end_;
                            }

KDNode::~KDNode() = default;

value_t KDNode::coord(const size_t &idx) { return x.at(idx); }
KDNodePtr KDNode::get_slice(value_t key) { return CI[key]; }
void KDNode::add_slice(value_t key, KDNodePtr val) {
  val->level = level + 1;
  CI[key] = val;
}

KDNodePtr KDNode::add_default_child() {
  KDNodePtr new_child = std::make_shared< KDNode >(begin, end, min_bound,
                                                   max_bound, (dim+1) % min_bound.size());
  add_slice(min_bound[(dim+1)% min_bound.size()], new_child);
  return new_child;
}

KDNodePtr KDNode::add_child(size_t dim_) {
  KDNodePtr new_child = std::make_shared< KDNode >(begin, end, min_bound,
                                                   max_bound, dim_ % min_bound.size());
  add_slice(min_bound[(dim_ % min_bound.size())], new_child);
  return new_child;
}

size_t KDNode::get_size() { return CI.size(); }

KDNode::operator bool() { return (!x.empty()); }
KDNode::operator point_t() { return x; }
KDNode::operator size_t() { return index; }
KDNode::operator pointIndex() { return pointIndex(x, index); }

size_t KDNode::get_partition_size() {
  // std::cout << __FUNCTION__ << " bool: " << (begin == end) << "\n";
  if (begin == end){
    // std::cout << __FUNCTION__ << " returning 0\n";
    return 0;}
  return (std::prev(end)->second - begin->second)+1; // (std::prev(end)->second - begin->second)+1
}

inline double dist2(const point_t &a, const point_t &b) {
  double distc = 0;
  for (size_t i = 0; i < a.size(); i++) {
    double di = a.at(i) - b.at(i);
    distc += di * di;
  }
  return distc;
}

inline double dist2(const KDNodePtr &a, const KDNodePtr &b) {
  return dist2(a->x, b->x);
}

using pointVec = std::vector< point_t >;

KDNodePtrs new_KDNodePtrs(KDNodePtr n){
  if (n)
    return std::make_shared<std::vector<KDNodePtr>>(std::vector<KDNodePtr>{n});
  return std::make_shared<std::vector<KDNodePtr>>(std::vector<KDNodePtr>{});
}

ci_iter KDNode::find_slice(value_t v){
  ci_iter it = CI.lower_bound(v);

  // handling off by one search
  if (it != CI.begin() && it->first != v){
    it--;
  }

  return it;
}
