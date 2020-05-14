#pragma once
#include <string>
#include <sys/stat.h>
#include "ExperimentHelper.h"
#include "FileWriter.h"
#include "DataImporter.h"
#include "ExperimentPlot.h"

class Experiment {
protected:
  StaticKDTree* verification_tree;
  long final_partition_size = 200;
  std::string data_set_name;
  std::string query_set_name;
  long num_points;
  long num_queries;
  int dimensionality;
  uint strategy_switch_size;
  std::string title;
  std::string xlabel = "Number of queries";
  std::string ylabel = "Time (sec)";
  std::string generate_plot_title();
  std::string generate_data_file_name(tree_type tree, std::string extra="");
  std::string generate_image_file_name(std::string extra="");
  std::vector<std::pair<std::string, tree_type>> files;
  inline bool file_exists (const std::string& name);
  virtual valueIndexArr run_(tree_type tree);
public:
  plot_customization plotcustomization = plot_customization();
  uint avg = 5;
  std::string custom_file_addition = "";
  std::string experiment_name;
  std::vector<tree_type> trees = E::all_trees;
  explicit Experiment(std::string data_set_name, std::string query_set_name, long num_points, long num_queries, int dimensionality, uint strategy_switch_size_, StaticKDTree* verification_tree);
  virtual ~Experiment();
  virtual void run(bool fast=false);
  virtual void plot(bool latex=false);
};
