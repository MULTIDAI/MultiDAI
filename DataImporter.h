#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>


// TODO: Should these be included through other files?

using value_t = double;
using point_t = typename std::vector< value_t >;
using pointVec = std::vector< point_t >;
using filename = std::string;
using queryList = std::vector<std::pair<point_t, point_t> >;

class DataImporter {
public:
  std::vector<std::pair<double, double>> load_experiment_result(filename file);
  pointVec loadData(filename file, int amount, int dimensionality=2);
  pointVec load_all_data(filename file, int dimensionality=2);
  queryList loadQueries(filename file, int amount, int dimensionality=2);
  queryList load_all_queries(filename file, int dimensionality=2);
  point_t read_point(std::istringstream &iss, int dimensionality);
  std::pair<point_t, point_t> read_query(std::istringstream &iss, int dimensionality);
};
