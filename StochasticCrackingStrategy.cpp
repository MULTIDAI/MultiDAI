#include "StochasticCrackingStrategy.h"
#include "Helper.h"

pointIndexArr StochasticCrackingStrategy::crack_query_bounds(point_t low, point_t high,
                                                             KDNodePtrs strategic_cracks,
                                                             KDNodePtr branch,
                                                             KDTree* tree, size_t dim){

  
  KDNodePtr lower = strategic_cracks->at(0);
  KDNodePtr upper = strategic_cracks->at(1);
  KDNodePtrs cracks;
  size_t next_dim = (dim + 1) % low.size();
  query_loc l = query_location(lower->min_bound, upper->min_bound, upper->max_bound, low, high, dim);
  int i = 0;
  for (KDNodePtr n : (*strategic_cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Strategic cracks FUCKED UP: Min bound is greater than max bound. " << i << "\n";
        // n->min_bound = n->max_bound;
      }
      i++;
    }
  switch (l) {
  case left:
    cracks = KDTree::crack_in_three(lower, low[dim], high[dim], dim);
    i = 0;
    for (KDNodePtr n : (*cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Left FUCKED UP: Min bound is greater than max bound. " << i << "\n";
        OUT << "LOW: "; H::print_vec(low);
        OUT << "HIGH: "; H::print_vec(high);
        // n->min_bound = n->max_bound;
      } else {
        branch->add_slice(n->min_bound[dim], n);
      
      }
      i++;
    }    branch->add_slice(upper->min_bound[dim], upper);
    return tree->query_(cracks->at(1), low, high, next_dim);

  case left_single:
    cracks = KDTree::crack_in_two(lower, high[dim], dim);
    i = 0;
    for (KDNodePtr n : (*cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Left single FUCKED UP: Min bound is greater than max bound. " << i << "\n";
        OUT << "Bounds of strategic cracks, Min: ";
        H::print_vec(lower->min_bound);
        std::cout << " Max: ";
        H::print_vec(upper->max_bound);
        std::cout << "\n";
        OUT << "Low: "; H::print_vec(low); std::cout << "\n";
        OUT << "High: "; H::print_vec(high); std::cout << "\n";
        OUT << "crack lower Bound "; H::print_vec(n->min_bound);std::cout << "\n";
        OUT << "crack upper Bound "; H::print_vec(n->max_bound);std::cout << "\n NODE:";
        H::printLeafNode(n);
        std::cout << "\n";
        // n->min_bound = n->max_bound;
      }
      i++;
      branch->add_slice(n->min_bound[dim], n);
    }
    branch->add_slice(upper->min_bound[dim], upper);
    return tree->query_(cracks->at(0), low, high, next_dim);

  case right:
    cracks = KDTree::crack_in_three(upper, low[dim], high[dim], dim);
    i = 0;
    for (KDNodePtr n : (*cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Right FUCKED UP: Min bound is greater than max bound. " << i << "\n";
        OUT << "LOW: "; H::print_vec(low);
        OUT << "HIGH: "; H::print_vec(high);
        OUT << "crack lower Bound "; H::print_vec(n->min_bound);std::cout << "\n";
        OUT << "crack upper Bound "; H::print_vec(n->max_bound);std::cout << "\n NODE:";
        H::printLeafNode(n);        // n->min_bound = n->max_bound;
      }
      i++;
      branch->add_slice(n->min_bound[dim], n);
    }
    branch->add_slice(lower->min_bound[dim], lower);
    return tree->query_(cracks->at(1), low, high, next_dim);

  case right_single:
    cracks = KDTree::crack_in_two(upper, low[dim], dim);
    i = 0;
    for (KDNodePtr n : (*cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Right single FUCKED UP: Min bound is greater than max bound. " << i <<" \n";
        OUT << "Low: "; H::print_vec(low); std::cout << "\n";
        OUT << "High: "; H::print_vec(high); std::cout << "\n";
        OUT << "crack lower Bound "; H::print_vec(n->min_bound);std::cout << "\n";
        OUT << "crack upper Bound "; H::print_vec(n->max_bound);std::cout << "\n NODE:";
        H::printLeafNode(n);
        // n->min_bound = n->max_bound;
      }
      i++;
      branch->add_slice(n->min_bound[dim], n);
    }    branch->add_slice(lower->min_bound[dim], lower);
    return tree->query_(cracks->at(1), low, high, next_dim);

  case both_low: // crack and query low at 1, query high
  {
    cracks = KDTree::crack_in_two(lower, low[dim], dim);
    int i = 0;
    for (KDNodePtr n : (*cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Both high FUCKED UP: Min bound is greater than max bound." << i <<" \n";
        OUT << "LOW: "; H::print_vec(low);
        OUT << "HIGH: "; H::print_vec(high);
        OUT << "crack lower Bound "; H::print_vec(n->min_bound);std::cout << "\n";
        OUT << "crack upper Bound "; H::print_vec(n->max_bound);std::cout << "\n NODE:";
        H::printLeafNode(n);
        std::cout << "\n";        

        // n->min_bound = n->max_bound;
      }
      i++;
      branch->add_slice(n->min_bound[dim], n);
    }    
    branch->add_slice(upper->min_bound[dim], upper);
    pointIndexArr low_res = tree->query_(cracks->at(1), low, high, next_dim);
    pointIndexArr high_res = tree->query_(upper, low, high, next_dim);
    low_res.insert(low_res.end(), high_res.begin(), high_res.end());
    return low_res;
    }

  case both_high: // crack and query high at 0, query low
  {
    cracks = KDTree::crack_in_two(upper, high[dim], dim);
    for (KDNodePtr n : (*cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Both high FUCKED UP: Min bound is greater than max bound." << i <<" \n";
        OUT << "LOW: "; H::print_vec(low);
        OUT << "HIGH: "; H::print_vec(high);
        OUT << "crack lower Bound "; H::print_vec(n->min_bound);std::cout << "\n";
        OUT << "crack upper Bound "; H::print_vec(n->max_bound);std::cout << "\n NODE:";
        H::printLeafNode(n);
        std::cout << "\n";

        // n->min_bound = n->max_bound;
      }
      i++;
      branch->add_slice(n->min_bound[dim], n);
    }    
    branch->add_slice(lower->min_bound[dim], lower);
    pointIndexArr high_res = tree->query_(cracks->at(0), low, high, next_dim);
    pointIndexArr low_res = tree->query_(lower, low, high, next_dim);
    low_res.insert(low_res.end(), high_res.begin(), high_res.end());
    return low_res;
  }

  case none: {
    pointIndexArr low_res = tree->query_(lower, low, high, next_dim);
    pointIndexArr high_res = tree->query_(upper, low, high, next_dim);
    low_res.insert(low_res.end(), high_res.begin(), high_res.end());
    return low_res; 
  }
  
  default: // both
    // Cracking lower
    int i = 0;
    cracks = KDTree::crack_in_two(lower, low[dim], dim);
    for (KDNodePtr n : (*cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Both FUCKED UP in low part: Min bound is greater than max bound." << i <<" \n";
        OUT << "LOW: "; H::print_vec(low);
        OUT << "HIGH: "; H::print_vec(high);
        OUT << "crack lower Bound "; H::print_vec(n->min_bound);std::cout << "\n";
        OUT << "crack upper Bound "; H::print_vec(n->max_bound);std::cout << "\n NODE:";
        H::printLeafNode(n);
        std::cout << "\n";

        // n->min_bound = n->max_bound;
      }
      i++;
      branch->add_slice(n->min_bound[dim], n);
    }
    pointIndexArr ret = tree->query_(cracks->at(1), low, high, next_dim);
    // Cracking upper
    i = 0;
    cracks = KDTree::crack_in_two(upper, high[dim], dim);
    for (KDNodePtr n : (*cracks)){
      if(n->min_bound[dim] > n->max_bound[dim]){
        OUT << "Both High FUCKED UP in high part: Min bound is greater than max bound." << i <<" \n";
        OUT << "LOW: "; H::print_vec(low);
        OUT << "HIGH: "; H::print_vec(high);
        OUT << "crack lower Bound "; H::print_vec(n->min_bound);std::cout << "\n";
        OUT << "crack upper Bound "; H::print_vec(n->max_bound);std::cout << "\n NODE:";
        H::printLeafNode(n);
        std::cout << "\n";

        // n->min_bound = n->max_bound;
      }
      i++;
      branch->add_slice(n->min_bound[dim], n);
    }
    pointIndexArr temp = tree->query_(cracks->at(0), low, high, next_dim);
    ret.insert(ret.end(), temp.begin(), temp.end());
    return ret;
  }

}



