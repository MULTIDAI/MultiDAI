#include "LazyCrackingStrategy.h"


pointIndexArr LazyCrackingStrategy::crack(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree, int level) {
  return crack_(branch, low, high, dim, tree);
}

pointIndexArr LazyCrackingStrategy::crack_(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree) {
  int next_dim = (dim + 1) % low.size();
  KDNodePtr middle_partition;
  pointIndexArr ret;

  if (high[dim] > branch->max_bound[dim]){ // only crack on low
    KDNodePtrs nodes = KDTree::crack_in_two(branch, low[dim], dim);
    KDNodePtr low_partition = nodes->at(0);
    middle_partition = nodes->at(1);
    // Insert new cracked nodes as children of current node
    branch->add_slice(low_partition->min_bound[dim], low_partition);
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
  } else if (low[dim] < branch->min_bound[dim]){ // only crack on high
    KDNodePtrs nodes = KDTree::crack_in_two(branch, high[dim], dim);
    middle_partition = nodes->at(0);
    KDNodePtr high_partition = nodes->at(1);
    // Insert new cracked nodes as children of current node
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
    branch->add_slice(high_partition->min_bound[dim], high_partition);
  } else { // crack on both
    KDNodePtrs nodes = KDTree::crack_in_three(branch, low[dim], high[dim], dim);
    KDNodePtr low_partition = nodes->at(0);
    middle_partition = nodes->at(1);
    KDNodePtr high_partition = nodes->at(2);
    // Insert new cracked nodes as children of current node
    branch->add_slice(low_partition->min_bound[dim], low_partition);
    branch->add_slice(middle_partition->min_bound[dim], middle_partition);
    branch->add_slice(high_partition->min_bound[dim], high_partition);
  }
  // Recurse on middle partition
  ret = tree->query_(middle_partition, low, high, next_dim);
  return ret;
}
