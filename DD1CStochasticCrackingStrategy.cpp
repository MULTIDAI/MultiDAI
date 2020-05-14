#include "DD1CStochasticCrackingStrategy.h"



pointIndexArr DD1CStochasticCrackingStrategy::crack(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree) {
  
  value_t pmid = (branch->min_bound[dim] + branch->max_bound[dim]) / 2;
  KDNodePtrs center_cracks = KDTree::crack_in_two(branch, pmid, dim);

  return crack_query_bounds(low, high, center_cracks, branch, tree, dim);
}
