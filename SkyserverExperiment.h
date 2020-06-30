#pragma once
#include "Experiment.h"

struct plot_exps {
  plot_exps(): plot_conv(true),plot_cum(true) { }   // default Constructor
  bool plot_conv = true;
  bool plot_cum = true;
};



class SkyserverExperiment: public Experiment {
  std::vector<std::pair<std::string, tree_type>> generate_convergence();
  std::vector<std::pair<std::string, tree_type>> generate_cumulative();
  query_vec queries;
  pointVec data;
  plot_exps plot_experiments;
public:
  bool plot_only = false;
  explicit SkyserverExperiment(std::string data_set_name, std::string query_set_name,
                               long num_points, long num_queries, int dimensionality,
                               uint strategy_switch_size, StaticKDTree* verification_tree,
                               plot_exps=plot_exps(), bool plot_only = false);
  virtual ~SkyserverExperiment();
  valueIndexArr run_(tree_type tree);
  virtual void plot(bool latex=false);
  void plot_conv(bool latex);
  void plot_cum(bool latex);
};
