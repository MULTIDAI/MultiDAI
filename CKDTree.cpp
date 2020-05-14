#include "CKDTree.h"
#include "Helper.h"

CKDTree::CKDTree(pointVec point_array, size_t min_size_) : LazyCKDTree(point_array, min_size_) {}

/* if some children: go into relevant subtrees */
bp_pair CKDTree::traverse_tree(KDNodePtr branch, point_t low,
                               point_t high, size_t dim){
  int next_dim = (dim + 1) % low.size();
  pointIndexArr ret = pointIndexArr();
  if (branch->get_size() > 0){
    ci_iter it = branch->find_slice(low[dim]);
    while (it != branch->CI.end() && it->first < high[dim]){
      point_t new_high;
      KDNodePtr curr_branch = it->second;
      point_t next_min_bound;
      if(std::next(it) != branch->CI.end()){
        next_min_bound = std::next(it)->second->min_bound;
        if(next_min_bound[dim] < high[dim]){
          new_high = alter_query(high, next_min_bound[dim], dim);
        }
      }
      pointIndexArr nodes;
      if(new_high.size() > 0){
        nodes = query_(it->second, low, new_high, next_dim);  
      }else {
        nodes = query_(it->second, low, high, next_dim);
      }
      ret.insert(ret.end(), nodes.begin(), nodes.end());
      it++;
    }
    return bp_pair(true, ret);
  }
  return bp_pair(false, ret);
}

std::pair<int, KDNodePtrs> CKDTree::crack_tree(KDNodePtr branch, point_t low, point_t high, size_t dim){
  KDNodePtrs nodes;
  int middle_partition_index;
  if (high[dim] > branch->max_bound[dim]){
    middle_partition_index = 1;
    nodes = crack_in_two(branch, low[dim], dim);
    KDNodePtr low_partition = nodes->at(0);
    KDNodePtr middle_partition = nodes->at(1);
    // Insert new cracked nodes as children of current node
    branch->add_slice(low_partition->min_bound[dim], low_partition);
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
  } else if (low[dim] < branch->min_bound[dim]){
    middle_partition_index = 0;
    nodes = crack_in_two(branch, high[dim], dim);
    KDNodePtr middle_partition = nodes->at(0);
    KDNodePtr high_partition = nodes->at(1);    
    
    // Insert new cracked nodes as children of current node
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
    branch->add_slice(high_partition->min_bound[dim], high_partition);
  } else {
    middle_partition_index = 1;
    nodes = crack_in_three(branch, low[dim], high[dim], dim);
    KDNodePtr low_partition = nodes->at(0);
    KDNodePtr middle_partition = nodes->at(1);
    KDNodePtr high_partition = nodes->at(2);
    // Insert new cracked nodes as children of current node
    branch->add_slice(low_partition->min_bound[dim], low_partition);
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
    branch->add_slice(high_partition->min_bound[dim], high_partition);
  }
  return std::pair<int, KDNodePtrs>(middle_partition_index, nodes);
}

point_t CKDTree::alter_query(point_t query, value_t new_val, int dim){
  point_t new_query(query);
  new_query[dim] = new_val;
  return new_query;
}

pointIndexArr CKDTree::query_irrelevant_branch(KDNodePtr &branch, point_t low, point_t high, double new_low_val, double new_high_val, size_t dim){
  point_t new_high = alter_query(high, new_high_val, dim);
  point_t new_low = alter_query(low, new_low_val, dim);
  return query_(branch, new_low, new_high, (dim + 1) % low.size());
}

/* If no children: branch on dimension, on both high and low
 * then go to next dimension recursively,
 * if too few values, filter out relevant values and return (TODO) */
pointIndexArr CKDTree::crack_tree_and_recurse(KDNodePtr branch, point_t low, point_t high,
                                              size_t dim){
  int next_dim = (dim + 1) % low.size();
  std::pair<int, KDNodePtrs > slices = crack_tree(branch, low, high, dim);

  // TODO: enable loop to query all partitions if relevant

  int middle_partition_index = slices.first;
  KDNodePtr middle_partition = (slices.second->at(middle_partition_index));


  // Set new query bounds based on surrounding slices
  if(middle_partition_index == 1){
    KDNodePtr low_partition = slices.second->at(0);
    query_irrelevant_branch(low_partition,low, high, low_partition->min_bound[dim], middle_partition->min_bound[dim], dim);
    if(slices.second->size()==3){
      KDNodePtr high_partition = slices.second->at(2);
      query_irrelevant_branch(high_partition, low, high, middle_partition->max_bound[dim], high_partition->max_bound[dim], dim);
    }
  } else if (middle_partition_index == 0) {
    KDNodePtr high_partition = slices.second->at(1);
    query_irrelevant_branch(high_partition, low, high, middle_partition->max_bound[dim], high_partition->max_bound[dim], dim);
  }
  return query_(middle_partition, low, high, next_dim);
  // Recurse on middle partition
}

pointIndexArr CKDTree::query_(KDNodePtr branch, point_t low, point_t high, size_t dim){
  bp_pair subset_check = check_inclusion(branch, low, high);

  if(subset_check.first){
    return subset_check.second;
  }

  // Find next dimension to query
  
  bp_pair traverse_check = traverse_tree(branch, low, high, dim);

  if (traverse_check.first){
    return traverse_check.second;
  }
  if (branch->get_partition_size() < min_size){
    return final_partition_extract(branch, low, high);
  }
  return crack_tree_and_recurse(branch, low, high, dim);
}
