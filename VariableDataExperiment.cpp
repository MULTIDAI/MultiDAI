#include "VariableDataExperiment.h"

VariableDataExperiment::~VariableDataExperiment() = default;

VariableDataExperiment::VariableDataExperiment(std::string data_set_name_, std::string query_set_name_,
                                             long num_points_,
                                               long num_queries_, int dimensionality_, uint strategy_switch_size_, StaticKDTree* verification_tree_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_, strategy_switch_size_, verification_tree_){
  experiment_name = "Variable Data";
  xlabel = "number of points";
}


valueIndexArr VariableDataExperiment::run_(tree_type tree){
  DataImporter importer;
  std::string data_set = E::data_sets[data_set_name];
  std::string query_set = E::query_sets[query_set_name];
  queryList queries = importer.loadQueries(query_set, num_queries, dimensionality);
  int step = 1000;
  valueIndexArr ret;
  for (double i = 1000; i <= num_points; i += step){
    if (((int)i) % (num_points/10) == 0)
      std::cout << "number of points: " << i << "\n";
    pointVec data = importer.loadData(data_set, i, dimensionality);
    // queryList queries = importer.loadQueries(data_set.second, i);
    double time = E::measure_tree_cumulative(data, tree, queries, final_partition_size, strategy_switch_size, verification_tree);
    ret.push_back(valueIndex(i, time));
  }
  return ret;
}


