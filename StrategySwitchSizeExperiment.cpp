#include "StrategySwitchSizeExperiment.h"

StrategySwitchSizeExperiment::~StrategySwitchSizeExperiment() = default;

StrategySwitchSizeExperiment::StrategySwitchSizeExperiment(std::string data_set_name_, std::string query_set_name_,
                                                           long num_points_, long num_queries_, int dimensionality_,
                                                           uint strategy_switch_size_, StaticKDTree* verification_tree_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_, strategy_switch_size_, verification_tree_){
  experiment_name = "StrategySwitchSize";
  xlabel = "Strategy switch size";
}


valueIndexArr StrategySwitchSizeExperiment::run_(tree_type tree){
  DataImporter importer;
  std::string data_set = E::data_sets[data_set_name];
  std::string query_set = E::query_sets[query_set_name];
  pointVec data = importer.loadData(data_set, num_points, dimensionality);
  query_vec queries = importer.loadQueries(query_set, num_queries, dimensionality);
  
  valueIndexArr ret;
  for (int sss = 200; sss <= 10000; sss *= 1.5){
    std::cout << "strategy_switch_size: " << sss << "\n";
    double time = E::measure_tree_cumulative(data, tree, queries, get_final_partition_size(tree), sss, verification_tree);
    ret.push_back(valueIndex(sss, time));
  }
  return ret;

}
