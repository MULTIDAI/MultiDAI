#include "ConvergenceExperiment.h"

ConvergenceExperiment::~ConvergenceExperiment() = default;

ConvergenceExperiment::ConvergenceExperiment(std::string data_set_name_, std::string query_set_name_,
                                             long num_points_, long num_queries_, int dimensionality_,
                                             uint strategy_switch_size_,  StaticKDTree* verification_tree_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_, strategy_switch_size_, verification_tree_){
  experiment_name = "Convergence";
  plotcustomization.logx = true;
  plotcustomization.exp_y = true;
  plotcustomization.exp_x = true;
  plotcustomization.ylabel_offset = 1;
}


valueIndexArr ConvergenceExperiment::run_(tree_type tree){
  DataImporter importer;
  std::string data_set = E::data_sets[data_set_name];
  pointVec data = importer.loadData(data_set, num_points, dimensionality);
  std::string query_set = E::query_sets[query_set_name];
  query_vec queries = importer.loadQueries(query_set, num_queries, dimensionality);
  std::vector<double> time = E::measure_convergence(tree, queries, data, get_final_partition_size(tree), strategy_switch_size, verification_tree);
  valueIndexArr ret;
  time[1] += time[0];
  for (double i = 1; i < time.size(); i++)
    ret.push_back(valueIndex(i, time[i]));
  return ret;
}


