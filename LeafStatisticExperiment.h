#pragma once
#include "Experiment.h"

class LeafStatisticExperiment : public Experiment {
  query_vec queries;
  pointVec data;
public:
  explicit LeafStatisticExperiment(std::string data_set_name, std::string query_set_name,
                                   long num_points, long num_queries, int dimensionality,
                                   uint strategy_switch_size, StaticKDTree* verification_tree);
  virtual ~LeafStatisticExperiment();
  valueIndexArr run_(tree_type tree);
};

