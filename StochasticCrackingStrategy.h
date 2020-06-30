#pragma once
#include "CrackingStrategy.h"

class StochasticCrackingStrategy : public CrackingStrategy {
protected:
  pointIndexArr crack_query_bounds(point_t low, point_t high, KDNodePtrs strategic_cracks,
                                   KDNodePtr branch, KDTree* tree, size_t dim, int level);
};
