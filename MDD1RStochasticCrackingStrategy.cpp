#include "MDD1RStochasticCrackingStrategy.h"

pointIndexArr MDD1RStochasticCrackingStrategy::crack(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree) {
  
  std::uniform_real_distribution<double> unif(branch->min_bound[dim], branch->max_bound[dim]);
  std::default_random_engine re;
  value_t pmid = unif(re);
  std::pair<KDNodePtrs, pointIndexArr> res = KDTree::m_crack_in_two(branch, pmid, low, high, dim);
  
  KDNodePtr lower = res.first->at(0);
  KDNodePtr upper = res.first->at(1);


  branch->add_slice(lower->min_bound[dim], lower);
  branch->add_slice(upper->min_bound[dim], upper);

  return res.second;
}
