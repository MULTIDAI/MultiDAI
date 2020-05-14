#pragma once
#include <fstream>
#include <vector>
#include <iostream>

using value_t = double;
using point_t = typename std::vector< value_t >;
using pointVec = std::vector< point_t >;
using valueIndex = typename std::pair< size_t, value_t >;
using valueIndexArr = typename std::vector< valueIndex >;
using filename = std::string;
using query_vec = std::vector<std::pair<point_t, point_t> >;

class FileWriter {
public:
  filename write_file_data(filename file, valueIndexArr xs, std::string tree);
  filename write_queries(filename file_name, query_vec queries);
  filename write_data_points(filename file_name, pointVec data_points);
  filename generic_vector_vector_writer(filename file_name, std::vector<std::vector<double>> data);
  filename generic_vector_pair_vector_writer(filename file_name, query_vec queries);
};
