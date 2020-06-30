#include "Experiment.h"


Experiment::~Experiment() = default;

Experiment::Experiment(std::string data_set_name_, std::string query_set_name_,
                       long num_points_, long num_queries_, int dimensionality_, uint strategy_switch_size_,
                       StaticKDTree* verification_tree_) :
  verification_tree(verification_tree_), data_set_name(data_set_name_), query_set_name(query_set_name_),
  num_points(num_points_), num_queries(num_queries_), dimensionality(dimensionality_), strategy_switch_size(strategy_switch_size_){
  if (!Experiment::file_exists(E::data_sets[data_set_name]))
    std::cout << "WARNING! " << data_set_name << " does not exist!\n";
  if (!Experiment::file_exists(E::query_sets[query_set_name]))
    std::cout << "WARNING! " << query_set_name << " does not exist!\n";
  custom_final_partition_size = {};
}

long Experiment::get_final_partition_size(tree_type tree) {
  auto it = custom_final_partition_size.find( tree );
   if ( it == custom_final_partition_size.end() ) {
      return final_partition_size;
   }
   else {
      return it->second;
   }
}

void Experiment::add_custom_final_partition_size(tree_type tree, long value) {
  custom_final_partition_size[tree] = value;
}

void Experiment::add_custom_final_partition_size(std::vector<std::pair<tree_type, long>> trees) {
  for (std::pair<tree_type, long> tree : trees)
    custom_final_partition_size[tree.first] = tree.second;
}

std::string Experiment::generate_image_file_name(std::string extra) {
  filename file_location = "results/img/";
  std::string data_set_name_(data_set_name);
  std::replace(data_set_name_.begin(), data_set_name_.end(), ' ', '_');
  std::string query_set_name_(query_set_name);
  std::replace(query_set_name_.begin(), query_set_name_.end(), ' ', '_');
  std::string experiment_name_(experiment_name);
  std::replace(experiment_name_.begin(), experiment_name_.end(), ' ', '_');

  return  file_location + std::to_string(dimensionality) + "d_" + experiment_name_ + "_" + data_set_name_ + "_"
    + query_set_name_ + "_" + std::to_string(num_points) + "_" + std::to_string(num_queries) + extra + custom_file_addition; 
}

std::string Experiment::generate_data_file_name(tree_type tree, std::string extra) {
  filename file_location = "results/data/";
  return  file_location + std::to_string(dimensionality) + "d_" + experiment_name + "_" + data_set_name + "_" + query_set_name + "_" + E::tree_string(tree) + "_"
    + std::to_string(num_points) + "_" + std::to_string(num_queries) + extra + ".dat"; 
}


std::string Experiment::generate_plot_title() {
  return std::to_string(dimensionality) + "d " + experiment_name + " " + data_set_name + ", " + query_set_name + ", " + std::to_string(num_points) + " points, " + std::to_string(num_queries) + " queries";
}

inline bool Experiment::file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

void Experiment::plot(bool latex) {
  if (files.empty()){
    std::cout << experiment_name << " generating files\n";
    for (tree_type tree : trees){
      files.push_back(std::pair<std::string, tree_type>(generate_data_file_name(tree), tree));
    }
  }
  Plot::plot_to_image(&files, generate_image_file_name(), generate_plot_title(),
                      xlabel, ylabel, plotcustomization, latex);
}

valueIndexArr Experiment::run_(tree_type ){
  return valueIndexArr{};
}


void Experiment::run(bool fast){
  std::cout << "Experiment: " << generate_plot_title() << "\n";
  FileWriter writer;
  for (tree_type tree : trees){
    valueIndexArr avg_vector;
    std::cout << "Running: " << E::tree_string(tree) << "\n";
    std::string out_name = generate_data_file_name(tree);
    if (Experiment::file_exists(out_name) && fast){
      files.push_back(std::pair<std::string, tree_type>(out_name, tree));
      continue;
    }
    for (uint i = 0; i < avg; i++) {
      std::cout << "avg: " << i+1 << "\n";
      if (avg_vector.empty()) {
        avg_vector = run_(tree);
      } else {
        valueIndexArr res = run_(tree);
        for (uint i = 0; i < res.size(); i++) {
          avg_vector[i].second += res[i].second;
        }
      }
    }
    for (uint i = 0; i < avg_vector.size(); i++) {
      avg_vector[i].second /= avg;
    }
    files.push_back(std::pair<std::string, tree_type>(writer.write_file_data(out_name, avg_vector, E::tree_string(tree)), tree));
  }
}
