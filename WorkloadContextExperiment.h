#pragma once
#include "Experiment.h"

class WorkloadContextExperiment : public Experiment {
  std::string context_workload_name;
  long context_num_queries;
  bool use_context_time = true;
public:
  explicit WorkloadContextExperiment(std::string data_set_name, std::string query_set_name, long num_points,
                                     long num_queries, std::string context_workload_name, long context_num_queries,
                                     bool use_context_time,
                                     int dimensionality, uint strategy_switch_size, StaticKDTree* verification_tree);
  virtual ~WorkloadContextExperiment();
  virtual valueIndexArr run_(tree_type tree);
};
