#include "ExperimentHelper.h"
#include "DD1CStochasticCrackingStrategy.h"
#include "DD1RStochasticCrackingStrategy.h"
#include "MDD1RStochasticCrackingStrategy.h"
#include "CrackingStrategy.h"
#include "LazyCrackingStrategy.h"
#include "ModularCKDTree.h"
#include "HybridKDTree.h"
#include "HybridKDTree_level.h"
#include "DD1RKDTree.h"


// using query_vec = std::vector<std::pair<point_t, point_t> >;;

std::map<std::string, std::string> E::data_sets =
  {{"random 60000", "data/random_60k_points.data"},
   {"random 240000", "data/random_240k_points.data"},
   {"grid 560000", "data/grid_05_01_0_-30_400_40.data"},
   {"Skyserver main", "data/skyserver_data.data"},
   {"Skyserver main 1 in 2", "data/skyserver_data_1_in_2.data"},
   {"Skyserver main 1 in 4", "data/skyserver_data_1_in_4.data"},
   {"Skyserver main 1 in 8", "data/skyserver_data_1_in_8.data"},
   {"Skyserver main 1 in 16", "data/skyserver_data_1_in_16.data"},
   {"Skyserver main 1 in 32", "data/skyserver_data_1_in_32.data"},
   {"Skyserver main 1 in 64", "data/skyserver_data_1_in_64.data"},
   {"Skyserver main 1 in 128", "data/skyserver_data_1_in_128.data"},
   {"Skyserver main 1 in 256", "data/skyserver_data_1_in_256.data"},
   {"Skyserver main 1 in 512", "data/skyserver_data_1_in_512.data"},
   {"Skyserver main 1 in 1024", "data/skyserver_data_1_in_1024.data"},
   {"random 240000 3d","data/random_3d_240000_points.data"},
   {"random 240000 10d","data/random_10d_240000_points.data"},
   {"random 240000 20d","data/random_20d_240000_points.data"},
  };

std::map<std::string, std::string> E::query_sets =
  {{"random 100","data/random_100_queries.data"},
   {"random 1000", "data/random_1k_queries.data"},
   {"random 2000 2d", "data/random_2000_2d_queries.data"},
   {"random 2000 3d", "data/random_2000_3d_queries.data"},
   {"random 10000 3d", "data/random_10000_3d_queries.data"},
   {"random 2000 10d", "data/random_2000_10d_queries.data"},
   {"random 10000 20d", "data/random_10000_20d_queries.data"},
   {"sequential 100", "data/sequential_100_queries.data"},
   {"sequential 1000", "data/sequential_1000_queries.data"},
   {"sequential 1000 reversed", "data/sequential_1000_reversed_queries.data"},
   {"variable size 2000", "data/random_variable_size_2k_queries.data"},
   {"zoomin 1000", "data/zoomin_1000_queries.data"},
   {"zoomout 1000", "data/zoomout_1000_queries.data"},
   {"random bounded 1000", "data/random_bounded_1000_queries.data"},
   {"z sequential 441", "data/z_sequential_queries_0_05.data"},
   {"skyserver 5000", "data/skyserver_workload_5000.data"},
   {"random 2000 3d", "data/random_2000_3d_queries.data"},
   {"skyserver 10000", "data/skyserver_workload_10000.data"},
   {"skyserver 10000 max 5 dup", "data/skyserver_workload_10000_max_5_dup.data"},
   {"skyserver 10000 no dup", "data/skyserver_workload_10000_no_dup.data"},
   {"skyserver regular", "data/skyserver_queries.data"},
   {"skyserver regular 1 in 4", "data/skyserver_queries_1_in_4.data"},
   {"skyserver regular reversed", "data/skyserver_queries_reversed.data"},
   {"skyserver sorted dim 0", "data/skyserver_sorted_dim_0.data"},
   {"skyserver sorted dim 0 reversed", "data/skyserver_sorted_dim_0_reversed.data"},
   {"skyserver sorted dim 1", "data/skyserver_sorted_dim_1.data"},
   {"skyserver sorted dim 1 reversed", "data/skyserver_sorted_dim_1_reversed.data"},
   {"skyserver sorted size dim 0", "data/skyserver_sorted_size_dim_0.data"},
   {"skyserver sorted size dim 0 reversed", "data/skyserver_sorted_size_dim_0_reversed.data"},
   {"skyserver sorted size dim 1", "data/skyserver_sorted_size_dim_1.data"},
   {"skyserver sorted size dim 1 reversed", "data/skyserver_sorted_size_dim_1_reversed.data"},
   {"skyserver sorted total size", "data/skyserver_sorted_total_size.data"},
   {"skyserver sorted total size reversed", "data/skyserver_sorted_total_size_reversed.data"},
  };

 // "dark-violet" 
 // "#009e73"    
 // "#56b4e9" 
 // "#e69f00" 
 // "#f0e442" 
 // "#0072b2" 
 // "#e51e10" 
 // "black"   
 // "gray50"  

// http://phrogz.net/css/distinct-colors.html


std::map<tree_type, std::string> E::tree_colors =
  {{ckd, "#660000"}, // #660000
   {lazyckd, "#ff2200"}, //  #ff2200
   {quasii, "#ffaa00"}, //  #ffaa00
   {dd1c, "#00f241"}, //  #00f241
   {dd1r, "#36593f"}, //  #36593f
   {hdd1r, "#99ddff"}, //  #004d73
   {hdd1r_l, "#004d73"}, //  #99ddff
   {hdd1c, "#a033ff"}, //  #a033ff
   {mdd1r, "#ff66a3"}, //  #ff66a3
   {kd, "black"}, // 
   {hmdd1r, "#ADFF2F"},
   {hdd1c_l, "#FF4500"},
   {modularlazyckd, "#00008B"},
   {dd1r_cp, "#FF4500"},
  };

std::map<tree_type, std::string> E::tree_point_types =
  {{ckd, "3"},     // 1
   {lazyckd, "1"}, // 1 3
   {quasii, "2"},  // 1 3
   {dd1c, "8"},    // 2 3
   {dd1r, "3"},   // 2
   {hdd1r, "4"},   // 2
   {hdd1r_l, "1"}, // 2
   {hdd1c, "10"},   // 2 3
   {mdd1r, "2"},   // 2
   {kd, "4"},      // 1
   {hdd1c_l, "9"},
   {hmdd1r, "6"},
   {modularlazyckd, "7"},
   {dd1r_cp, "8"},
  };


std::string E::tree_string(tree_type t){
  switch (t) {
  case lazyckd:
    return "Lazy CKD";
  case ckd:
    return "CKD";
  case quasii:
    return "QUASII";
  case kd:
    return "Static";
  case dd1c:
    return "kd-C";
  case dd1r:
    return "kd-R";
  case hdd1r:
    return "kd-HR";
  case hdd1r_l:
    return "kd-HR_l";
  case hdd1c_l:
    return "kd-HC_l";
  case hdd1c:
    return "kd-HC";
  case dd1r_cp:
    return "dd1r_cp";
  case mdd1r:
    return "kd-MR";
  case hmdd1r:
    return "kd-HMR";
  case modularlazyckd:
    return "modularlazyckd";
  default:
    std::cout << "ALERT, in tree_string, default tree_type\n";
    return "statickd";
  }
}

std::vector<tree_type> E::all_trees = std::vector<tree_type>{lazyckd, ckd, quasii, kd, dd1c, dd1r, mdd1r, hdd1r, hdd1c};

std::pair<KDTree*, double> E::init_tree(tree_type t, pointVec points, size_t final_partition_size, uint strategy_switch_size){
  KDTree* tree;
  CrackingStrategy *C;
  std::chrono::steady_clock::time_point start;
  dsec acc;
  switch (t)
    {
    case lazyckd:
      start = std::chrono::steady_clock::now();
      tree = new LazyCKDTree(points, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case ckd:
      start = std::chrono::steady_clock::now();
      tree = new CKDTree(points, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case kd:    
      start = std::chrono::steady_clock::now();
      tree = new StaticKDTree(points);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    
    case quasii:
      start = std::chrono::steady_clock::now();
      tree = new QUASII(points, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case dd1c:
      C = new DD1CStochasticCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new ModularCKDTree(points, C, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case dd1r:
      C = new DD1RStochasticCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new ModularCKDTree(points, C, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case dd1r_cp:
      start = std::chrono::steady_clock::now();
      tree = new DD1RKDTree(points, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case mdd1r:
      C = new MDD1RStochasticCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new ModularCKDTree(points, C, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case hmdd1r:
      C = new MDD1RStochasticCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new HybridKDTree(points, C, final_partition_size, strategy_switch_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case hdd1r:
      C = new DD1RStochasticCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new HybridKDTree(points, C, final_partition_size, strategy_switch_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case hdd1r_l:
      C = new DD1RStochasticCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new HybridKDTree_level(points, C, final_partition_size, 6);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case hdd1c_l:
      C = new DD1RStochasticCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new HybridKDTree_level(points, C, final_partition_size, 6);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case hdd1c:
      C = new DD1CStochasticCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new HybridKDTree(points, C, final_partition_size, strategy_switch_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    case modularlazyckd:
      C = new LazyCrackingStrategy();
      start = std::chrono::steady_clock::now();
      tree = new ModularCKDTree(points, C, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      // start = std::chrono::steady_clock::now();
      // tree = new TemplateKDTree<LazyCrackingStrategy>(points, 10);
      // acc = (std::chrono::steady_clock::now() - start);
      
      break;
    default:
      std::cout << "WARNING: going into default in init_tree\n";
      start = std::chrono::steady_clock::now();
      tree = new QUASII(points, final_partition_size);
      acc = (std::chrono::steady_clock::now() - start);
      break;
    }
  return std::pair<KDTree *, double>(tree, (acc).count());
}

void E::exec_queries(KDTree* tree, query_vec queries){
  query_vec::iterator it;
  dsec acc;
  for (it = queries.begin(); it != queries.end(); ++it){
    point_t low = it->first;
    point_t high = it->second;
    auto start = std::chrono::steady_clock::now();
    pointIndexArr res = tree->query(low, high);
    acc = acc + (std::chrono::steady_clock::now() - start); 
  }
}

pointIndexArr E::exec_query(KDTree* tree, std::pair<point_t, point_t> query){
  point_t low = query.first;
  point_t high = query.second;

  pointIndexArr res = tree->query(query.first, query.second);
  return res;
}

pointVec E::query_res_scan(point_t low, point_t high, pointVec allPoints){
  pointVec scanPointsResult;
  for(point_t pt: allPoints){
    if(shouldIncludePoint(pt, low, high)){
      scanPointsResult.push_back(pt);
    }
  }
  return scanPointsResult;
}

void remove_intersection(pointIndexArr& a, pointIndexArr& b, std::vector<point_t>& v1, std::vector<point_t>& v2){
  for (pointIndex v : a)
    v1.push_back(v.first);
  for (pointIndex v : b)
    v2.push_back(v.first);
  uint iter_v1 = 0;
  while (iter_v1 < v1.size()) {
    uint iter_v2 = 0;
    bool increase_iterator = true;
    while (iter_v2 < v2.size()) {
      if (v1[iter_v1] == v2[iter_v2]) {
        v1.erase(v1.begin() + iter_v1);
        v2.erase(v2.begin() + iter_v2);
        increase_iterator = false;
        break;
      }
      iter_v2 ++;
    }
    if (increase_iterator)
      iter_v1 ++;
  }
}

bool E::verify_pointIndexArr_eq(pointIndexArr a1, pointIndexArr a2){
  if(a1.size() != a2.size()){
    OUT << "------------------------------------------------------------\n";
    OUT << "WRONG SIZE, sizes: " << a1.size() << ", " << a2.size() << "\n";
    if (a1.size() < 10 && a2.size() < 10) {
      OUT << "a1: [";
      for (pointIndex p : a1) {
        H::print_vec(p.first);
        std::cout << ", ";
      }
      std::cout << "]\n";
      OUT << "a2: [";
      for (pointIndex p : a2) {
        H::print_vec(p.first);
        std::cout << ", ";
      }
      std::cout << "]\n";
    }
    std::vector<point_t> a1_filtered = std::vector<point_t>();
    std::vector<point_t> a2_filtered = std::vector<point_t>();
    OUT << "filtering\n";
    remove_intersection(a1, a2, a1_filtered, a2_filtered);
      
    OUT << "a1: [";
    for (point_t p : a1_filtered) {
      H::print_vec(p);
      std::cout << ", ";
    }
    std::cout << "]\n";
    OUT << "a2: [";
    for (point_t p : a2_filtered) {
      H::print_vec(p);
      std::cout << ", ";
    }
    std::cout << "]\n";


    // for(auto k : a1) {
    //   OUT << "(" << k.second << ": ";
    //   H::print_vec(k.first);
    //   std::cout << "), ";
    // }
    // std::cout << std::endl;
    // for(auto k : a2) std::cout << k << ' ';
    // for (pointIndex p1 : a1){
    //   H::print_vec(p1.first);
    //   std::cout << ", ";
    // }
    // std::cout << "\n";
    // for (pointIndex p2 : a2){
    //   H::print_vec(p2.first);
    //   std::cout << ", ";
    // }
    // std::cout << "\n";
    return false;
  }
  std::sort(a1.begin(), a1.end());
  std::sort(a2.begin(), a2.end());
  uint i = 0;
  bool res = true;

  while(i < a1.size()){
    bool temp = a1.at(i).first == a2.at(i).first;
    res &= temp;
    if(!temp){
      OUT << "WRONG POINT, sizes: " << a1.size() << ", " << a2.size() << "\n";
      // for (pointIndex p1 : a1){
      //   H::print_vec(p1.first);
      //   std::cout << ", ";
      // }
      // std::cout << "\n";
      // for (pointIndex p2 : a2){
      //   H::print_vec(p2.first);
      //   std::cout << ", ";
      // }
    } 
    i++;
  }
  return res;
}


bool E::verify_query_res(point_t low, point_t high, pointVec allPoints, pointIndexArr query_res){
  bool res = true;
  // Remove all points from all points which are not part of the result 
  pointVec scanPointsResult = query_res_scan(low, high, allPoints);
  
  if(scanPointsResult.size() != query_res.size()){
    return false;
  }
  pointVec query_vec;
  pointIndexArr::iterator it_2 = query_res.begin();
  while(it_2 < query_res.end()){
    point_t curr = it_2->first;
    query_vec.push_back(curr);
    it_2++;
  }

  res = vector_equality(scanPointsResult, query_vec);
  return res;

}



bool E::vector_equality(pointVec v1, pointVec v2){
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  uint i = 0;
  bool res = true;
  while(i < v1.size()){
    bool temp = v1.at(i) == v2.at(i);
    res &= v1.at(i) == v2.at(i);
    if(!temp){
      std::cout << "WRONG RESULT, point from v2: (" << v2.at(i).at(0) << ", " << v2.at(i).at(1) << ") \n";
      std::cout << " was wrongly added \n";
    }
    i++;
  }
  return res;
}

bool vector_comparator (point_t pt1,point_t pt2) { 
  if (pt1.at(0) < pt2.at(0)){
    return true;
  } else if(pt1.at(0) == pt2.at(0)){
    return pt1.at(1) <= pt2.at(1);
  }
  return false;
}

pointVec E::pointIndexArr_to_PointVec(pointIndexArr pt_index_arr){
  pointVec res;
  pointIndexArr::iterator it = pt_index_arr.begin();
  while(it < pt_index_arr.end()){
    point_t curr = it->first;
    res.push_back(curr);
    it++;
  }
  return res;
}

bool E::shouldIncludePoint(point_t pt, point_t low, point_t high){
  int i = 0;
  bool shouldInclude = true;
  for (value_t val : pt){
    shouldInclude &= low.at(i) <= val && val < high.at(i);
    i++;
  }
  return shouldInclude;
}

double E::measure_query_time(KDTree* tree, query_t q, StaticKDTree* verification_tree){
  pointIndexArr useless;
  auto start = std::chrono::steady_clock::now();
  useless = tree->query(q.first, q.second);
  dsec res = (std::chrono::steady_clock::now() - start);
  if (verification_tree->do_verify)
    if (!verify_pointIndexArr_eq(useless, verification_tree->query(q.first, q.second))) {
      OUT << "query was: ";
      H::print_vec(q.first);
      std::cout << ", ";
      H::print_vec(q.second);
      std::cout << "\n";
    }
  return (res).count();
}

double E::measure_tree_cumulative(pointVec points, tree_type type, query_vec queries, size_t final_partition_size, uint strategy_switch_size, StaticKDTree* verification_tree){
  std::pair<KDTree*, double> tree_time = init_tree(type, points, final_partition_size, strategy_switch_size);
  KDTree* tree = tree_time.first;
  // std::chrono::duration<double> acc(tree_time.second);
  double acc = tree_time.second;
  for(query_t q : queries){
    acc = acc + measure_query_time(tree, q, verification_tree);
  }
  delete tree;
  return acc;
}

std::vector<double> E::measure_convergence(tree_type t, query_vec queries, pointVec points, size_t final_partition_size, uint strategy_switch_size, StaticKDTree* verification_tree){
  std::pair<KDTree*, double> tree_time = init_tree(t, points, final_partition_size, strategy_switch_size);
  KDTree* tree = tree_time.first;
  std::vector<double> res;
  res.push_back(tree_time.second);
  size_t counter = 0;
  for (query_t q : queries){
    res.push_back(measure_query_time(tree, q, verification_tree));
    if (counter % (queries.size()/10) == 0)
      std::cout << "done " << counter << " queries\n";
    counter += 1;
  }
  delete tree;
  // res[0] += tree_time.second;
  return res;
}

double E::measure_variable_data(pointVec points, tree_type type, query_vec queries,
                                size_t final_partition_size, uint strategy_switch_size, StaticKDTree* verification_tree){
  std::pair<KDTree*, double> tree_time = init_tree(type, points, final_partition_size, strategy_switch_size);
  KDTree* tree = tree_time.first;
  // std::chrono::duration<double> acc(tree_time.second);
  double acc = tree_time.second;
  for(query_t q : queries){
    acc = acc + measure_query_time(tree, q, verification_tree);
  }
  delete tree;
  return acc;
}

valueIndexArr E::measure_final_partition_size(tree_type t, query_vec queries, pointVec points, 
                                              size_t max_final_partition_size, int step, int init,
                                              uint strategy_switch_size,
                                              StaticKDTree* verification_tree){
  valueIndexArr res;
  for (size_t p_size = init; p_size <= max_final_partition_size; p_size += step){
    std::cout << "initialized tree with psize: " << p_size << "\n";
    std::pair<KDTree*, double> tree = init_tree(t, points, p_size, strategy_switch_size);
    res.push_back(valueIndex(p_size, tree.second));
    for (query_t q : queries){
      res[res.size()-1].second += measure_query_time(tree.first, q, verification_tree);
    }
    delete tree.first;
  }
  return res;
}

