#pragma once
#include "CrackingStrategy.h"

class LazyCrackingStrategy : public CrackingStrategy {
public:
  pointIndexArr crack(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree);
  static pointIndexArr crack_(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree);
};
