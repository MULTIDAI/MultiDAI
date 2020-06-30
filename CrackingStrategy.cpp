#include "CrackingStrategy.h"

CrackingStrategy::query_loc CrackingStrategy::query_location(point_t lower_bound, point_t middle,
                                                             point_t upper_bound, point_t low,
                                                             point_t high, size_t dim){
                                                               

  if (low[dim] < middle[dim] && middle[dim] <= high[dim] ) // query is around middle
    {

      if(low[dim] < lower_bound[dim] && high[dim] < upper_bound[dim]){
      return both_high;
      }
      if(high[dim] > upper_bound[dim] && low[dim] > lower_bound[dim]){
        return both_low;
      }
      if(low[dim] < lower_bound[dim] && high[dim] > upper_bound[dim])
        return none;
      return both;
    }
  else if (low[dim] < middle[dim]) {// Something is in left crack
    if(high[dim] < lower_bound[dim])
      return none;
    if (low[dim] < lower_bound[dim])
        return left_single;
    return left;
  }
  else{
    if (high[dim] > upper_bound[dim])
      return right_single;
    return right;
  }
}

pointIndexArr CrackingStrategy::crack(KDNodePtr branch, point_t low, point_t high, size_t dim, KDTree* tree, int level) {
  std::cout << "WARNING: SOMEONE IS USING EMPTY CRACK\n";
  return pointIndexArr();
}
