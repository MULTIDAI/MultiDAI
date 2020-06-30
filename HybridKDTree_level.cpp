#include "HybridKDTree_level.h"

HybridKDTree_level::HybridKDTree_level(pointVec point_array, CrackingStrategy* C_, size_t min_size_,
                                       uint strategy_switch_level_) :
  KDTree(point_array, min_size_), C(C_), strategy_switch_level(strategy_switch_level_) {
  size_t dim = point_array[0].size();

  
  strategy_switch_level = dim * 3;

  root->add_default_child();
}

bp_pair HybridKDTree_level::traverse_tree_level(KDNodePtr branch, point_t low,
                                                point_t high, size_t level){
  int next_level = level + 1;
  int dim = level % low.size();
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
      pointIndexArr nodes = query_(it->second, new_low, new_high, next_level);
      ret.insert(ret.end(), nodes.begin(), nodes.end());
      it++;
    }
    return bp_pair(true, ret);
  }
  return bp_pair(false, ret);
}

pointIndexArr HybridKDTree_level::QUASII_query_(KDNodePtr branch, point_t low, point_t high, size_t dim, int branch_size) {
  pointIndexArr ret;
  KDNodePtrs new_children;
  ci_iter it = branch->find_slice(low[dim]);
  std::vector<value_t> to_remove;
  std::vector<std::pair<value_t, KDNodePtr>> to_add;
  while (it != branch->CI.end() && it->first < high[dim]){
    KDNodePtrs refined = refine(it->second, low, high, dim, branch_size);
    if (refined->size() > 1)
      to_remove.push_back(it->first);
    for (KDNodePtr s : *refined){
      to_add.push_back(std::pair<value_t, KDNodePtr>(s->min_bound[dim], s));
      bool intersection_not_empty = true;
      for (uint i = 0; i < low.size(); i++){
        intersection_not_empty &= H::intersection(low[i], high[i], s->min_bound[i], s->max_bound[i]);
      }
      if (intersection_not_empty) {
        pointIndexArr res;
        if (dim == (low.size()-1)) {
          res = final_partition_extract(s, low, high);
        } else {
          if (s->get_size() == 0) {
            s->add_default_child();
          }
          res = QUASII_query_(s, low, high, dim+1, branch_size);
        }
        ret.insert(ret.end(), res.begin(), res.end());
      } 
    }
    it++;
  }
  for (value_t v : to_remove)
    branch->CI.erase(v);
  for (std::pair<value_t, KDNodePtr> v : to_add) {
    if (v.second->end <= v.second->begin) {
      continue;
    }
    branch->add_slice(v.first, v.second);
  }
  return ret;
}


pointIndexArr HybridKDTree_level::query_(KDNodePtr branch, point_t low, point_t high, size_t level) {
  bp_pair subset_check = check_inclusion(branch, low, high);
  if (subset_check.first){
    return subset_check.second;
  }

  if (branch->get_partition_size() < min_size){
    return final_partition_extract(branch, low, high);
  }
  
  if (level >= strategy_switch_level) {
    if(branch->CI.size() == 0){
      branch->add_child(0);
    }
    int branch_size = branch->get_partition_size();
    auto location = tau.find(branch_size);
    if ( location == tau.end() ) {

      float root_dim = 1.0/(low.size());
      float n_over_tau = ((float)branch_size)/((float) min_size);
      float r = std::ceil(std::pow(n_over_tau, root_dim));
      tau[branch_size] = std::vector<size_t>();
      for (int l = low.size()-1; l >= 0; l--){
        tau[branch_size].push_back(std::pow(r, l) * min_size);
      }
    }
    return QUASII_query_(branch, low, high, 0, branch_size);
  }

  bp_pair traverse_check = traverse_tree_level(branch, low, high, level);
  if (traverse_check.first){
    return traverse_check.second;
  }
  return C->crack(branch, low, high, level % low.size(), this, level);
}

KDNodePtrs HybridKDTree_level::refine(KDNodePtr branch, point_t low, point_t high, size_t dim, int branch_size){

  if (branch->get_partition_size() <= tau[branch_size][dim]) 
    return new_KDNodePtrs(branch);
  KDNodePtrs ret = new_KDNodePtrs();
  KDNodePtrs cracks = new_KDNodePtrs();
  slice_type t = determineSliceType(branch, low, high, dim);
  switch (t){
  case s_both:
    cracks = crack_in_three(branch, low[dim], high[dim], dim);
    break;
  case s_one:
    if (high[dim] >= branch->max_bound[dim])
      cracks = crack_in_two(branch, low[dim], dim);
    else
      cracks = crack_in_two(branch, high[dim], dim);
    break;
  default:
    cracks = slice_artificial(branch, low, high, dim, branch_size);
  }
  for (KDNodePtr s : *cracks) {
    s->level = branch->level;
    bool intersection_not_empty = H::intersection(low[dim], high[dim], s->min_bound[dim], s->max_bound[dim]);
    if (s->get_partition_size() > tau[branch_size][dim] && intersection_not_empty &&
        low[dim] != s->max_bound[dim] && high[dim] != s->min_bound[dim]) {
      KDNodePtrs res = slice_artificial(s, low, high, dim, branch_size);
      ret->insert(ret->end(), res->begin(), res->end());
    } else {
      ret->push_back(s);
    }
  }
  return ret;
}

HybridKDTree_level::slice_type HybridKDTree_level::determineSliceType(KDNodePtr branch, point_t low,
                                                                      point_t high, size_t dim){
  if (high[dim] > branch->max_bound[dim] && low[dim] < branch->min_bound[dim]){
    return s_artificial;
  } else if (high[dim] < branch->max_bound[dim] && low[dim] > branch->min_bound[dim]){
    return s_both;
  } else {
    return s_one;
  }
}

KDNodePtrs HybridKDTree_level::slice_artificial(KDNodePtr branch, point_t low,
                                                point_t high, size_t dim, int branch_size){

  if (branch->get_partition_size() <= tau[branch_size][dim]) {
    return std::make_shared<std::vector<KDNodePtr>>(std::vector<KDNodePtr>{branch});
  }
  
  value_t c = (branch->min_bound[dim] + branch->max_bound[dim])/2;
  KDNodePtrs slices = crack_in_two(branch, c, dim);
  KDNodePtrs ret = new_KDNodePtrs();

  KDNodePtr first = slices->at(0);
  KDNodePtr second = slices->at(1);

  if (first->get_partition_size() == 0 && second->begin->first[dim] == std::prev(second->end)->first[dim]) {
    ret->push_back(first);
    ret->push_back(second);
    return ret;
  } else if (second->get_partition_size() == 0 && first->begin->first[dim] == std::prev(first->end)->first[dim]) {
    ret->push_back(first);
    ret->push_back(second);
    return ret;
  }

  for (KDNodePtr s : *slices){
    KDNodePtrs res = slice_artificial(s, low, high, dim, branch_size);
    ret->insert(ret->end(), res->begin(), res->end());
  }
  return ret;
}


