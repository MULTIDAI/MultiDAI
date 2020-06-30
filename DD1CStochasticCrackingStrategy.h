#include "StochasticCrackingStrategy.h"

class DD1CStochasticCrackingStrategy : public StochasticCrackingStrategy {
public:
  pointIndexArr crack(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree, int level);
};
