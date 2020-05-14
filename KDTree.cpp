/*
 * file: KDTree.hpp
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

#include "KDTree.h"
#include "Helper.h"


KDNodePtr KDTree::make_tree(const pointIndexArr::iterator &begin,  //
                            const pointIndexArr::iterator &end    //
                            ) {
  KDNodePtr ptr = std::make_shared< KDNode >(begin, end, 0, begin->first.size());
  return ptr;
}

KDTree::KDTree(pointVec point_array, size_t min_size_): min_size{min_size_} {
  for (ulong i = 0; i < point_array.size(); i++) {
    arr.push_back(pointIndex(point_array.at(i), i));
  }
  // OUT << "Created arr, size: " << arr.size() << "\n";
  // OUT << "arr[1].second: " << arr[1].second << " arr[1].first: ";
  // H::print_vec(arr[1].first); std::cout << "\n";
  

  // OUT << "arr[2].second: " << arr[2].second << "\n";
  // OUT << "std::prev(arr.end())->second: " << std::prev(arr.end())->second << "\n";

  auto begin = arr.begin();
  auto end = arr.end();

  root = KDTree::make_tree(begin, end);
}

KDTree::~KDTree() = default;

KDNodePtrs KDTree::crack_in_two(KDNodePtr &node, value_t pivot, size_t dim){
  // std::cout << "_____________ ENTERING " << __FUNCTION__ << ", pivot: " << pivot << " ________________\n";
  pointIndexArr::iterator b = node->begin;
  pointIndexArr::iterator e = std::prev(node->end);
  // OUT << "node bounds: min: " << node->min_bound[0] << "," << node->min_bound[1] << " max: " << node->max_bound[0] << "," << node->max_bound[1]<< " partition size: " << node->get_partition_size() << "\n";
  while (b < e){ 
    if (b->first[dim] < pivot){
      b = std::next(b);
    } else {
      while (e->first[dim] >= pivot && e > b){
        e = std::prev(e);
      }
      point_t temp = b->first;
      b->first = e->first;
      e->first = temp;
      b = std::next(b);
      e = std::prev(e);
    }
  }

  if (e < node->begin) {
    e = node->begin;
  } else if (b < node->end && b == e && e->first[dim] < pivot) {
    e = std::next(e);
  } else if (b > e) {
    e = std::next(e);
  } 


  point_t center_bound_min(node->min_bound);
  point_t center_bound_max(node->max_bound);
  
  center_bound_min.erase(center_bound_min.begin() + dim);
  center_bound_max.erase(center_bound_max.begin() + dim);
  center_bound_min.insert(center_bound_min.begin() + dim, pivot);
  center_bound_max.insert(center_bound_max.begin() + dim, pivot);
  
  KDNodePtr left = std::make_shared< KDNode >(node->begin, e, node->min_bound,
                                              center_bound_max, dim);
  KDNodePtr right = std::make_shared< KDNode >(e, node->end, center_bound_min,
                                               node->max_bound, dim);
 
 
  return std::make_shared<std::vector<KDNodePtr>>
    (std::vector<KDNodePtr>{left, right});
}


KDNodePtrs KDTree::crack_in_three(KDNodePtr &node, value_t pivot_low,
                                  value_t pivot_high, size_t dim){
  KDNodePtrs node_pair_low = crack_in_two(node, pivot_low, dim);
  KDNodePtrs node_pair_high = crack_in_two(node_pair_low->at(1), pivot_high, dim);
  KDNodePtrs ret = std::make_shared<std::vector<KDNodePtr>>
    (std::vector<KDNodePtr>{node_pair_low->at(0), node_pair_high->at(0), 
                              node_pair_high->at(1)});
  return ret;
}


bool do_materialize(point_t low, point_t high, pointIndexArr::iterator it) {
  bool included = true;
  for (uint i = 0; i < low.size(); i++){
    included &= (low[i] <= it->first[i] && it->first[i] < high[i]);
  }
  return included;
}


std::pair<KDNodePtrs, pointIndexArr> KDTree::m_crack_in_two(KDNodePtr &node, value_t pivot, point_t low, point_t high, size_t dim){
  // std::cout << "_____________ ENTERING " << __FUNCTION__ << ", pivot: " << pivot << " ________________\n";
  pointIndexArr::iterator b = node->begin;
  pointIndexArr::iterator e = std::prev(node->end);
  pointIndexArr ret;
  while (b <= e){ 
    while (b <= e && b->first[dim] < pivot) {
      if (do_materialize(low, high, b)){
        ret.push_back(*b);
      }
      b = std::next(b);
    }
    while (b <= e && e->first[dim] >= pivot){
      if (do_materialize(low, high, e)){
        ret.push_back(*e);
      }
      e = std::prev(e);
    }
    if (b <= e && do_materialize(low, high, b)){
      ret.push_back(*b);
    }
    if (b <= e && do_materialize(low, high, e)){
      ret.push_back(*e);
    }
    if (b < e) {
      point_t temp = b->first;
      b->first = e->first;
      e->first = temp;
      b = std::next(b);
      e = std::prev(e);
    }
  }

  if (e < node->begin) {
    e = node->begin;

  } else if (b < node->end && b == e && e->first[dim] < pivot) {
    e = std::next(e);
  } else if (b > e) {
    e = std::next(e);
  } 


  point_t center_bound_min(node->min_bound);
  point_t center_bound_max(node->max_bound);
  
  center_bound_min.erase(center_bound_min.begin() + dim);
  center_bound_max.erase(center_bound_max.begin() + dim);
  center_bound_min.insert(center_bound_min.begin() + dim, pivot);
  center_bound_max.insert(center_bound_max.begin() + dim, pivot);
  
  KDNodePtr left = std::make_shared< KDNode >(node->begin, e, node->min_bound,
                                              center_bound_max, dim);
  KDNodePtr right = std::make_shared< KDNode >(e, node->end, center_bound_min,
                                               node->max_bound, dim);
 

  KDNodePtrs nodes = std::make_shared<std::vector<KDNodePtr>>
    (std::vector<KDNodePtr>{left, right}); 
  return std::pair<KDNodePtrs, pointIndexArr>(nodes, ret);
}

pointIndexArr KDTree::query_(KDNodePtr branch, point_t low, point_t high, size_t dim) {
  std::cout << "ALERT SOMETHING IS USING KDTREE QUERY_!!!!!!!\n";
  return pointIndexArr();
}

pointIndexArr KDTree::query(point_t low, point_t high){
  return query_(root, low, high, 0);
}

/* Check if the current branch is entirely within the query */
bp_pair KDTree::check_inclusion(KDNodePtr branch, point_t low,
                                point_t high) {
  bool node_is_subset = true;
  pointIndexArr res;
  for (ulong i = 0; i < low.size(); i++){
    node_is_subset &= low[i] <= branch->min_bound[i] && high[i] >= branch->max_bound[i];
  }
  if (node_is_subset){
    res = H::node_to_point_index_arr(branch);
  }
  return bp_pair(node_is_subset, res);
}

/* if some children: go into relevant subtrees */
bp_pair KDTree::traverse_tree(KDNodePtr branch, point_t low,
                              point_t high, size_t dim){
  int next_dim = (dim + 1) % low.size();
  pointIndexArr ret = pointIndexArr();
  if (branch->get_size() > 0){
    ci_iter it = branch->find_slice(low[dim]);
    while (it != branch->CI.end() && it->first < high[dim]){
      point_t new_low(low);
      point_t new_high(high);
      if(low[dim] < it->second->min_bound[dim]){
        new_low[dim] = it->second->min_bound[dim];
      }
      if(high[dim] > it->second->max_bound[dim]){
        new_high[dim] = it->second->max_bound[dim];
      }
      pointIndexArr nodes = query_(it->second, new_low, new_high, next_dim);
      ret.insert(ret.end(), nodes.begin(), nodes.end());
      it++;
    }
    return bp_pair(true, ret);
  }
  return bp_pair(false, ret);
}



pointIndexArr KDTree::final_partition_extract(KDNodePtr branch, point_t low, point_t high){
  pointIndexArr ret;
  for (auto it = branch->begin; it != branch->end; it++){
    bool included = true;
    for (uint i = 0; i < low.size(); i++){
      included &= (low[i] <= (*it).first[i] && (*it).first[i] < high[i]);
    }
    if (included){
      ret.push_back(*it);
    }
  }
  return ret;
}

pointIndexArr KDTree::crack_query_bounds(point_t low, point_t high,
                                         KDNodePtrs strategic_cracks,
                                         KDNodePtr branch,
                                         size_t dim){

  
  KDNodePtr lower = strategic_cracks->at(0);
  KDNodePtr upper = strategic_cracks->at(1);
  KDNodePtrs cracks;
  size_t next_dim = (dim + 1) % low.size();
  query_loc l = query_location(lower->min_bound, upper->min_bound, upper->max_bound, low, high, dim);

  switch (l) {
  case left:
    cracks = crack_in_three(lower, low[dim], high[dim], dim);
    for (KDNodePtr n : (*cracks))
      branch->add_slice(n->min_bound[dim], n);
    branch->add_slice(upper->min_bound[dim], upper);
    return query_(cracks->at(1), low, high, next_dim);
  case left_single:
    cracks = crack_in_two(lower, high[dim], dim);
    for (KDNodePtr n : (*cracks))
      branch->add_slice(n->min_bound[dim], n);
    branch->add_slice(upper->min_bound[dim], upper);
    return query_(cracks->at(0), low, high, next_dim);
  case right:
    cracks = crack_in_three(upper, low[dim], high[dim], dim);
    for (KDNodePtr n : (*cracks))
      branch->add_slice(n->min_bound[dim], n);
    branch->add_slice(lower->min_bound[dim], lower);
    return query_(cracks->at(1), low, high, next_dim);
  case right_single:
    cracks = crack_in_two(upper, low[dim], dim);
    for (KDNodePtr n : (*cracks))
      branch->add_slice(n->min_bound[dim], n);
    branch->add_slice(lower->min_bound[dim], lower);
    return query_(cracks->at(1), low, high, next_dim);
  default: // both
    // Cracking lower
    cracks = crack_in_two(lower, low[dim], dim);
    for (KDNodePtr n : (*cracks))
      branch->add_slice(n->min_bound[dim], n);
    pointIndexArr ret = query_(cracks->at(1), low, high, next_dim);
    // Cracking upper
    cracks = crack_in_two(upper, high[dim], dim);
    for (KDNodePtr n : (*cracks))
      branch->add_slice(n->min_bound[dim], n);
    pointIndexArr temp = query_(cracks->at(0), low, high, next_dim);
    ret.insert(ret.end(), temp.begin(), temp.end());
    return ret;
  }

}

KDTree::query_loc KDTree::query_location(point_t lower_bound, point_t middle,
                                         point_t upper_bound, point_t low,
                                         point_t high, size_t dim){
  if (low[dim] < middle[dim] && middle[dim] <= high[dim] ) // query is around middle
    return both;
  else if (low[dim] < middle[dim]) {// Something is in left crack
    if (low[dim] < lower_bound[dim])
      return left_single;
    return left;
  }
  else{
    if (high[dim] > upper_bound[dim])
      return right_single;
    return right;
  }
}
