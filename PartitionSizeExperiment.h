#pragma once
#include "Experiment.h"

class PartitionSizeExperiment : public Experiment {
  static const long default_max_final_partition_size = 500;
  long max_final_partition_size = default_max_final_partition_size;
  int step;
  int init;
public:
  explicit PartitionSizeExperiment(std::string data_set_name, std::string query_set_name, long num_points, long num_queries, uint strategy_switch_size, StaticKDTree* verification_tree, long max_final_partition_size=default_max_final_partition_size, int step=10, int init=10, int dimensionality=2);
  virtual ~PartitionSizeExperiment();
  virtual valueIndexArr run_(tree_type tree);
};
