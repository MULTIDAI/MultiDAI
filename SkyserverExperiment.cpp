#include "SkyserverExperiment.h"
#include <sys/stat.h>

SkyserverExperiment::~SkyserverExperiment() = default;

SkyserverExperiment::SkyserverExperiment(std::string data_set_name_, std::string query_set_name_,
                                             long num_points_, long num_queries_, int dimensionality_,
                                         uint strategy_switch_size_,  StaticKDTree* verification_tree_,
                                         plot_exps plot_exps_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_,
             strategy_switch_size_, verification_tree_), plot_experiments(plot_exps_){
  experiment_name = "Skyserver";
  plotcustomization.logx = true;
  plotcustomization.exp_x = true;
  plotcustomization.exp_y = true;
  plotcustomization.ylabel_offset = 1;
  // DataImporter importer;
  // std::string data_set = E::data_sets[data_set_name];
  // data = importer.load_all_data(data_set, dimensionality);
  // std::string query_set = E::query_sets[query_set_name];
  // queries = importer.load_all_queries(query_set, dimensionality);
}

valueIndexArr SkyserverExperiment::run_(tree_type tree){
  std::vector<double> time = E::measure_convergence(tree, queries, data, final_partition_size, strategy_switch_size, verification_tree);
  valueIndexArr ret;
  // time[1] += time[0];
  for (double i = 0; i < time.size(); i++)
    ret.push_back(valueIndex(i, time[i]));
  return ret;
}

std::vector<std::pair<std::string, tree_type>> SkyserverExperiment::generate_convergence() {
  std::vector<std::pair<std::string, tree_type>> ret = std::vector<std::pair<std::string, tree_type>>{};
  DataImporter importer;
  FileWriter writer;
  
  for (tree_type tree : trees){
    filename f = generate_data_file_name(tree);
    if (!Experiment::file_exists(f))
      continue;
    std::vector<std::pair<double, double>> time = importer.load_experiment_result(f);
    valueIndexArr conv_time;
    time[1].second += time[0].second;
    int val = 1;
    int points_in_plot = 0;
    int step_val = 0;
    for (int i = 1; i < time.size(); i = std::max(step_val, i + 1)) {
      points_in_plot++;
      conv_time.push_back(valueIndex(time[i].first, time[i].second));
      if (i / val == 10)
        val = i;
      step_val = i * 1.002;
    }
    ret.push_back(std::pair<std::string, tree_type>(writer.write_file_data(generate_data_file_name(tree, "_Convergence"),
                                                                           conv_time, E::tree_string(tree)), tree));
  }
  return ret;
}
std::vector<std::pair<std::string, tree_type>> SkyserverExperiment::generate_cumulative() {
  std::vector<std::pair<std::string, tree_type>> ret = std::vector<std::pair<std::string, tree_type>>{};
  DataImporter importer;
  FileWriter writer;
  
  for (tree_type tree : trees){
    filename f = generate_data_file_name(tree);
    if (!Experiment::file_exists(f))
      continue;
    std::vector<std::pair<double, double>> time = importer.load_experiment_result(f);
    valueIndexArr cum_time;
    value_t acc = 0;
    size_t counter = 0;
    int step = 100;
    for (std::pair<double, double> v : time){
      acc += v.second;
      if ((counter % step) == 0)
        cum_time.push_back(valueIndex(v.first, acc));
      counter += 1; // first value is init time
    }
    ret.push_back(std::pair<std::string, tree_type>(writer.write_file_data(generate_data_file_name(tree, "_Cumulative"),
                                                                           cum_time, E::tree_string(tree)), tree));
  }
  return ret;
}

void SkyserverExperiment::plot_conv(bool latex) {
  std::vector<std::pair<std::string, tree_type>> conv_files = generate_convergence();
  if (!conv_files.empty())
    Plot::plot_to_image(&conv_files, generate_image_file_name("_Convergence"),
                        generate_plot_title(), xlabel, ylabel, plotcustomization, latex);
  
}

void SkyserverExperiment::plot_cum(bool latex) {
  plot_customization cum_ps(plotcustomization);
  cum_ps.logx = false;
  cum_ps.exp_x = false;
  
  std::vector<std::pair<std::string, tree_type>> cum_files = generate_cumulative();
  if (!cum_files.empty())
    Plot::plot_to_image(&cum_files, generate_image_file_name("_Cumulative"),
                        generate_plot_title(), xlabel, ylabel, cum_ps, latex);
}

void SkyserverExperiment::plot(bool latex) {
  OUT << "Using skyserver plot\n";
  if (plot_experiments.plot_conv)
    plot_conv(latex);
  if (plot_experiments.plot_cum)
    plot_cum(latex);
}
