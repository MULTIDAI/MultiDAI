#include "LazyCKDTree.h"
#include "Helper.h"

LazyCKDTree::LazyCKDTree(pointVec point_array, size_t min_size_) : KDTree(point_array, min_size_) {}


/* If no children: branch on dimension, on both high and low
 * then go to next dimension recursively,
 * if too few values, filter out relevant values and return (TODO) */
pointIndexArr LazyCKDTree::crack_tree(KDNodePtr branch, point_t low, point_t high,
                                      size_t dim){
  int next_dim = (dim + 1) % low.size();
  KDNodePtr middle_partition;
  pointIndexArr ret;

  if (high[dim] > branch->max_bound[dim]){ // only crack on low
    KDNodePtrs nodes = crack_in_two(branch, low[dim], dim);
    KDNodePtr low_partition = nodes->at(0);
    middle_partition = nodes->at(1);
    // Insert new cracked nodes as children of current node
    branch->add_slice(low_partition->min_bound[dim], low_partition);
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
  } else if (low[dim] < branch->min_bound[dim]){ // only crack on high
    KDNodePtrs nodes = crack_in_two(branch, high[dim], dim);
    middle_partition = nodes->at(0);
    KDNodePtr high_partition = nodes->at(1);
    // Insert new cracked nodes as children of current node
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
    branch->add_slice(high_partition->min_bound[dim], high_partition);
  } else { // crack on both
    KDNodePtrs nodes = crack_in_three(branch, low[dim], high[dim], dim);
    KDNodePtr low_partition = nodes->at(0);
    middle_partition = nodes->at(1);
    KDNodePtr high_partition = nodes->at(2);
    // Insert new cracked nodes as children of current node
    branch->add_slice(low_partition->min_bound[dim], low_partition);
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
    branch->add_slice(high_partition->min_bound[dim], high_partition);
  }
  // Recurse on middle partition
  ret = query_(middle_partition, low, high, next_dim);
  return ret;
  
}



pointIndexArr LazyCKDTree::query_(KDNodePtr branch, point_t low,
                                  point_t high, size_t dim) {
  // std::cout << "_____________ ENTERING " << __FUNCTION__ << " ________________\n";

  bp_pair subset_check = check_inclusion(branch, low, high);
  if (subset_check.first){
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

  return crack_tree(branch, low, high, dim);
}





