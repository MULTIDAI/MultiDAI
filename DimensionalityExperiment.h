#pragma once
#include "Experiment.h"

class DimensionalityExperiment : public Experiment {
  std::vector<StaticKDTree*> verification_trees;
  public:
  explicit DimensionalityExperiment(std::string data_set_name, std::string query_set_name,
                                    long num_points, long num_queries, int dimensionality,
                                    uint strategy_switch_size, StaticKDTree* verification_tree);
  virtual ~DimensionalityExperiment();
  valueIndexArr run_(tree_type tree);
};

