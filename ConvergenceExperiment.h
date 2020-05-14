#pragma once
#include "Experiment.h"

class ConvergenceExperiment : public Experiment {
  public:
  explicit ConvergenceExperiment(std::string data_set_name, std::string query_set_name,
                                 long num_points, long num_queries, int dimensionality,
                                 uint strategy_switch_size, StaticKDTree* verification_tree);
  virtual ~ConvergenceExperiment();
  valueIndexArr run_(tree_type tree);
};

