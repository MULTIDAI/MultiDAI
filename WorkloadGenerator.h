#pragma once
#include <random>
#include "KDNode.h"
#include "ExperimentHelper.h"
class Gen {
public:
  static  query_t generate_random_bounded_query(std::default_random_engine re, std::uniform_real_distribution<double> unif_x, std::uniform_real_distribution<double> unif_y, double high_x, double high_y);
  static void generate_random_bounded_queries(std::vector<query_t> *res, double queries_per_section_, std::default_random_engine re, std::uniform_real_distribution<double> unif_x, std::uniform_real_distribution<double> unif_y, double high_x, double high_y);
  static void generate_zoom_in_queries(std::vector<query_t> *res, double queries_per_section, double section_size, point_t low, point_t high);
  
  static std::vector<query_t> generate_random_queries_in_bounded_sections(double sections, double queries_per_section, double section_size);
  static std::vector<query_t> generate_zoom_out_workload(double sections, double queries_per_section, double section_size);
  static std::vector<query_t> generate_zoom_in_workload(double sections, double queries_per_section, double section_size);
  static std::vector<query_t> generate_z_sequential_workload(double query_size);
  static std::vector<query_t> generate_sequential_workload(double query_size);
  static std::vector<query_t> generate_random_workload(int amount, double query_max_size, int dimensionality);
  static std::vector<std::vector<double>> boxxy_visualisation(std::vector<query_t> queries);
  static std::vector<std::vector<double>> boxxy_access_pattern(std::vector<query_t> queries, uint dim);
};

