#include "LeafStatisticExperiment.h"

LeafStatisticExperiment::~LeafStatisticExperiment() = default;

LeafStatisticExperiment::LeafStatisticExperiment(std::string data_set_name_, std::string query_set_name_,
                                                 long num_points_, long num_queries_, int dimensionality_,
                                                 uint strategy_switch_size_,  StaticKDTree* verification_tree_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_, strategy_switch_size_, verification_tree_){
  experiment_name = "LeafStatistic";
  DataImporter importer;
  std::string data_set = E::data_sets[data_set_name];
  if (num_points == -1)
    data = importer.load_all_data(data_set, dimensionality);
  else
    data = importer.loadData(data_set, num_points, dimensionality);
  std::string query_set = E::query_sets[query_set_name];
  if (num_queries == -1)
    queries = importer.load_all_queries(query_set, dimensionality);
  else
    queries = importer.loadQueries(query_set, num_queries, dimensionality);
  verification_tree = new StaticKDTree(pointVec()); 
  verification_tree->do_verify = false;
  avg = 1;
}


valueIndexArr LeafStatisticExperiment::run_(tree_type tree_t){
  std::pair<KDTree*, double> tree_time = E::init_tree(tree_t, data, get_final_partition_size(tree_t), strategy_switch_size);
  KDTree* tree = tree_time.first;
  size_t counter = 0;
  for (query_t q : queries){
    E::measure_query_time(tree, q, verification_tree);
    if (counter % (queries.size()/10) == 0)
      std::cout << "done " << counter << " queries\n";
    counter += 1;
  }
  std::map<int, int> levels = H::find_leaf_levels(tree);
  valueIndexArr ret;
  for (std::pair<int, int> n : levels) {
    ret.push_back(valueIndex(n.first, n.second));
  }
  return ret;
}


