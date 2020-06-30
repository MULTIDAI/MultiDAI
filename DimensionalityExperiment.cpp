#include "DimensionalityExperiment.h"

DimensionalityExperiment::~DimensionalityExperiment() = default;

DimensionalityExperiment::DimensionalityExperiment(std::string data_set_name_, std::string query_set_name_,
                                                   long num_points_, long num_queries_, int dimensionality_,
                                                   uint strategy_switch_size_, StaticKDTree* verification_tree_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_, strategy_switch_size_, verification_tree_){
  experiment_name = "Dimensionality";
  verification_trees = std::vector<StaticKDTree*>();
  xlabel = "Dimensionality";
  plotcustomization.short_xlabel = false;
  plotcustomization.point_size = 2;

  plotcustomization.xrange.use = true;
  plotcustomization.xrange.low = 1.5;
  plotcustomization.xrange.high = 20.5;
}


valueIndexArr DimensionalityExperiment::run_(tree_type tree){
  DataImporter importer;
  std::string data_set = E::data_sets[data_set_name];
  std::string query_set = E::query_sets[query_set_name];
  
  valueIndexArr ret;
  uint tree_index = 0;
  for (int dim = 2; dim <= dimensionality; dim++){
    std::cout << "dimensionality: " << dim << "\n";
    pointVec data = importer.loadData(data_set, num_points, dim);
    query_vec queries = importer.loadQueries(query_set, num_queries, dim);
    if (verification_trees.size() < tree_index + 1){
      OUT << "Creating verification tree\n"; 
      verification_trees.push_back(new StaticKDTree(data));
    }
    double time = E::measure_tree_cumulative(data, tree, queries, get_final_partition_size(tree), strategy_switch_size, verification_trees[tree_index]);
    ret.push_back(valueIndex(dim, time));
    tree_index++;
  }
  return ret;
}


