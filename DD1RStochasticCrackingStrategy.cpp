#include "DD1RStochasticCrackingStrategy.h"

pointIndexArr DD1RStochasticCrackingStrategy::crack(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree) {
  
  std::uniform_real_distribution<double> unif(branch->min_bound[dim], branch->max_bound[dim]);
  std::default_random_engine re;
  value_t pmid = unif(re);
  KDNodePtrs center_cracks = KDTree::crack_in_two(branch, pmid, dim);

  return crack_query_bounds(low, high, center_cracks, branch, tree, dim);
}
