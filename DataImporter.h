#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>


// TODO: Should these be included through other files?

using value_t = double;
using point_t = typename std::vector< value_t >;
using pointVec = std::vector< point_t >;
using filename = std::string;
using query_vec = std::vector<std::pair<point_t, point_t> >;

class DataImporter {
public:
  inline bool file_exists (const std::string& name);
  std::vector<std::pair<double, double>> load_experiment_result(filename file);
  pointVec loadData(filename file, int amount, int dimensionality=2);
  pointVec load_all_data(filename file, int dimensionality=2);
  query_vec loadQueries(filename file, int amount, int dimensionality=2);
  query_vec load_all_queries(filename file, int dimensionality=2);
  point_t read_point(std::istringstream &iss, int dimensionality);
  std::pair<point_t, point_t> read_query(std::istringstream &iss, int dimensionality);
};
