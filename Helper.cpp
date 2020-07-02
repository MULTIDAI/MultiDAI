#include "Helper.h"


void H::print_arr(std::vector< std::pair< std::vector< double >, size_t > > const& vec ) {
  for (auto i = vec.begin(); i != vec.end(); ++i){
    std::cout << "[";
    print_vec(i->first);
    std::cout << "], ";
  }
  std::cout << "\n";
}

  


bool H::intersection(value_t x_low, value_t x_high, value_t y_low, value_t y_high){
  return !(x_low > y_high || x_high < y_low);


  // (y_low <= x_low && x_low < y_high) ||
  //     (y_low <= x_high && x_high < y_high) ||
  //     (x_low <= y_low && y_low < x_high) ||
  //     (x_low <= y_high && y_high < x_high);
}


template<typename T, typename A>
void H::print_vec( std::vector<T,A> const& vec ) {
  for (auto i = vec.begin(); i != vec.end(); ++i)
    std::cout << *i << ' ';
}

void H::printLeafNode(KDNodePtr n){
  pointIndexArr::iterator b = n->begin;
  pointIndexArr::iterator e = n->end;
  std::cout << "(node: [";
  print_vec<double>(n->min_bound);
  std::cout << "], [";
  print_vec<double>(n->max_bound);
  std::cout << "], [";
  while (b < e){
    std::cout << "(";
    print_vec<double>(b->first);
    std::cout << "), ";
    b = std::next(b);
  }
  std::cout << "])";
}

bool H::verify_node_bounds(KDNodePtr node){
  bool ret = true;
  for (auto it = node->begin; it != node->end; it++) {
    for (size_t i = 0; i < node->min_bound.size(); i++){
      ret &= node->min_bound[i] < it->first[i];
      ret &= node->max_bound[i] > it->first[i];
    }
  }
  return ret;
}


pointVec H::point_set_2d_grid(value_t n) {
  pointVec points;
  for (value_t i = 0; i < n; i++){
    for (value_t j = 0; j < n; j++){
      points.push_back({i, j});
    }
  }
  return points;
}

void H::print_tree_level(KDNodePtr &node , int level){
  if (level > 0){
    for (auto it = node->CI.begin(); it != node->CI.end(); it++){
      print_tree_level(it->second, level-1);
    }
  } else {
    printLeafNode(node); std::cout << " \n";
  }
}

// void H::print_tree(KDTree &tree){
  
// }

bool H::verify_final_partition_size(KDNodePtr branch, size_t min_size) {
  if (branch->get_partition_size() < min_size){
    return branch->get_size() == 0;
  }
  bool ret = true;
  for (auto it = branch->CI.begin(); it != branch->CI.end(); it++){
    ret &= verify_final_partition_size(it->second, min_size);
  }
  return ret;
}

void H::print_first_kdnodeptrs(KDNodePtrs ptrs){
  for (auto it = ptrs->begin(); it != ptrs->end(); it++){
    std::cout << "(";
    H::print_vec((*it)->begin->first); std::cout << "), ";
  }
}

pointIndexArr H::node_to_point_index_arr(KDNodePtr n){
  pointIndexArr ret = pointIndexArr();
  auto it = n->begin;
  while (it != n->end){
    ret.push_back(*it);
    it++;
  }
  return ret;
}

pointIndexArr H::nodes_to_point_index_arr(KDNodePtrs nodes){
  pointIndexArr ret = pointIndexArr();
  for (KDNodePtr n: (*nodes)){
    pointIndexArr res = node_to_point_index_arr(n);
    ret.insert(ret.begin(), res.begin(), res.end());
  }
  return ret;
}

void H::find_wrong_nodes(KDNodePtr branch) {
  for (uint dim = 0; dim < branch->min_bound.size(); dim++) {
    if (branch->min_bound[dim] > branch->max_bound[dim]) {
      OUT << "Nodes has wrong bounds\n";
      OUT << "dim: " << dim << " min, max: " << branch->min_bound[dim] << ", " << branch->max_bound[dim] << " size of branch: " << branch->get_size() << "\n";
    }
  }
  for (const auto& any : branch->CI) {
    KDNodePtr n = any.second;
    find_wrong_nodes(n);
  }
}

KDNodePtrs H::find_leafs(KDNodePtr branch) {
  if (branch->CI.empty()){
    return std::make_shared<std::vector<KDNodePtr>>(std::vector<KDNodePtr>{branch});
  }
  KDNodePtrs ret = new_KDNodePtrs();
  for (const auto& n : branch->CI) {
    KDNodePtrs res = find_leafs(n.second);
    ret->insert(ret->end(), res->begin(), res->end());
  }
  return ret;
}

std::map<int, int> H::find_leaf_levels(KDTree* tree) {
  std::map<int, int> ret = std::map<int, int>();
  find_leaf_levels_rec(tree->root, 1, &ret);
  return ret;
}

void H::find_leaf_levels_rec(KDNodePtr branch, int level, std::map<int, int>* values) {
  for (std::pair<double, KDNodePtr> n : branch->CI) {
    if (n.second->CI.empty()) {
      auto location = values->find(level);
      if ( location == values->end() ) {
        OUT << level << " does not exist\n";
        values->insert(std::pair<int, int>(level, 1));
      } else {
        // OUT << level << " does exist\n";
        location->second++;
      }
    } else {
      find_leaf_levels_rec(n.second, level+1, values);
    }
  }

}
