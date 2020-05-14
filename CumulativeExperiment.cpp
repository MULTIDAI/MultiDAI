#include "CumulativeExperiment.h"

CumulativeExperiment::~CumulativeExperiment() = default;

CumulativeExperiment::CumulativeExperiment(std::string data_set_name_, std::string query_set_name_,
                                             long num_points_, long num_queries_, int dimensionality_,
                                             uint strategy_switch_size_,
                                             StaticKDTree* verification_tree_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_,strategy_switch_size_, verification_tree_){
  experiment_name = "Cumulative";
  plotcustomization.ylabel_offset = 1.5;
}


valueIndexArr CumulativeExperiment::run_(tree_type tree){
  DataImporter importer;
  std::string data_set = E::data_sets[data_set_name];
  pointVec data = importer.loadData(data_set, num_points, dimensionality);
  std::string query_set = E::query_sets[query_set_name];
  queryList queries = importer.loadQueries(query_set, num_queries, dimensionality);
  std::vector<value_t> time = E::measure_convergence(tree, queries, data, final_partition_size, strategy_switch_size, verification_tree);
  valueIndexArr ret;
  value_t acc = 0;
  size_t counter = 0;
  int step = num_queries / 100;
  for (value_t v : time){
    acc += v;
    if ((counter % step) == 0)
      ret.push_back(valueIndex(counter, acc));
    counter += 1; // first value is init time
  }
  return ret;
}


// valueIndexArr CumulativeExperiment::run_(tree_type tree){
//   DataImporter importer;
//   std::string data_set = E::data_sets[data_set_name];
//   pointVec data = importer.loadData(data_set, num_points, dimensionality);
//   std::string query_set = E::query_sets[query_set_name];
//   int step = num_queries / 100;
//   valueIndexArr ret;
//   for (double i = 0; i <= num_queries; i += step){
//     if (((int)i) % (num_queries/10) == 0)
//       std::cout << "number of queries: " << i << "\n";
//     queryList queries = importer.loadQueries(query_set, i, dimensionality);
//     // queryList queries = importer.loadQueries(data_set.second, i);
//     double time = E::measure_tree_cumulative(data, tree, queries, final_partition_size);
//     ret.push_back(valueIndex(i, time));
//   }
//   return ret;
// }


