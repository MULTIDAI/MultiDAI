#include "PartitionSizeExperiment.h"

PartitionSizeExperiment::~PartitionSizeExperiment() = default;

PartitionSizeExperiment::PartitionSizeExperiment(std::string data_set_name_, std::string query_set_name_,
                                                 long num_points_, long num_queries_, uint strategy_switch_size_, StaticKDTree* verification_tree_,
                                                 long max_final_partition_size_,
                                                 int step_, int init_, int dimensionality_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_, strategy_switch_size_ ,verification_tree_),
  max_final_partition_size(max_final_partition_size_), step(step_), init(init_) {
  experiment_name = "Partition size " + std::to_string(max_final_partition_size);
  xlabel = "final partition size";
  trees = std::vector<tree_type>{lazyckd, ckd, quasii};
}


valueIndexArr PartitionSizeExperiment::run_(tree_type tree){
  DataImporter importer;
  std::string data_set = E::data_sets[data_set_name];
  pointVec data = importer.loadData(data_set, num_points, dimensionality);
  std::string query_set = E::query_sets[query_set_name];
  queryList queries = importer.loadQueries(query_set, num_queries, dimensionality);
  return E::measure_final_partition_size(tree, queries, data, max_final_partition_size, step, init, strategy_switch_size, verification_tree);

}


