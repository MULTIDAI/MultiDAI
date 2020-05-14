#pragma once
#include "KDTree.h"


class CrackingStrategy {
protected:
  enum query_loc {left = 0,
                  left_single = 1,
                  right = 2,
                  right_single = 3,
                  both = 4, 
                  both_low = 5,
                  both_high = 6,
                  none = 7
  };
  query_loc query_location(point_t lower_bound, point_t middle, point_t upper_bound, point_t low, point_t high, size_t dim); 
public:
  virtual pointIndexArr crack(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree);
};

