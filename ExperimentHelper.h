#pragma once
#include <vector>
#include <iostream>
#include <chrono>
#include "LazyCKDTree.h"
#include "QUASII.h"
#include "StaticKDTree.h"
#include "CKDTree.h"
#include "KDTree.h"
#include "Helper.h"


enum tree_type { ckd = 0,
                 lazyckd = 1,
                 quasii = 2,
                 kd = 3,
                 dd1c = 4,
                 dd1r = 5,
                 mdd1r = 6,
                 hmdd1r = 7,
                 modularlazyckd = 8,
                 dd1r_cp = 9,
                 hdd1r = 10,
};

using valueIndex = typename std::pair< size_t, value_t >;
using valueIndexArr = typename std::vector< valueIndex >;
using dsec = std::chrono::duration<double>;
using query_t = std::pair<point_t, point_t>;
using queryList = std::vector<query_t >;
class E {
public:
  static std::map<std::string, std::string> data_sets;
  static std::map<std::string, std::string> query_sets;
  static std::map<tree_type, std::string> tree_colors;
  static std::pair<KDTree *, double> init_tree(tree_type t, pointVec points, size_t final_partition_size, uint strategy_switch_size);
  static double avg_tree_cumulative(pointVec points, tree_type type, queryList queries, double avg_runs, size_t final_partition_size = 0);
  static double measure_tree_cumulative(pointVec points, tree_type type, queryList queries, size_t final_partition_size, uint strategy_switch_size, StaticKDTree* verification_tree);
  static double measure_variable_data(pointVec points, tree_type type, queryList queries, size_t final_partition_size, uint strategy_switch_size, StaticKDTree* verification_tree);
  static double measure_query_time(KDTree* tree, query_t query, StaticKDTree* verification_tree);
  static pointVec query_res_scan(point_t low, point_t high, pointVec allPoints);
  static bool verify_query_res(point_t low, point_t high, pointVec allPoints, pointIndexArr query_res);
  static void exec_queries(KDTree* tree, queryList queries);
  static pointIndexArr exec_query(KDTree* tree, query_t query);
  static pointVec pointIndexArr_to_PointVec(pointIndexArr pt_index_arr);
  static bool shouldIncludePoint(point_t pt, point_t low, point_t high);
  static bool vector_equality(pointVec v1, pointVec v2);
  static std::vector<double> measure_convergence(tree_type t, queryList queries, pointVec points, size_t final_partition_size, uint strategy_switch_size, StaticKDTree* verification_tree);
  static std::vector<double> avg_measure_convergence(tree_type t, queryList queries, pointVec points, double avg_runs, size_t final_partition_size, uint strategy_switch_size);
  static valueIndexArr measure_final_partition_size(tree_type t, queryList queries, pointVec points,
                                                    size_t max_final_partition_size, int step, int init, uint strategy_switch_size, StaticKDTree* verification_tree);
  static std::vector<double> avg_measure_final_partition_size(tree_type t, queryList queries, pointVec points, double avg_runs, size_t max_final_partition_size);

  static std::string generate_file_name(std::string base, std::string data_set_name,
                                        size_t data_size, size_t query_size);
  static std::string generate_plot_title(std::string experiment, std::string data_set_name,
                                         long num_points, long num_queries);
  static bool verify_pointIndexArr_eq(pointIndexArr a1, pointIndexArr a2);
  static std::string tree_string(tree_type t);
  static std::vector<tree_type> all_trees;
};

