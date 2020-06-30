#include "WorkloadContextExperiment.h"

WorkloadContextExperiment::~WorkloadContextExperiment() = default;

WorkloadContextExperiment::
WorkloadContextExperiment(std::string data_set_name_, std::string query_set_name_, long num_points_,
                          long num_queries_, std::string context_workload_name_, long context_num_queries_,
                          bool use_context_time_, int dimensionality_, uint strategy_switch_size_,
                          StaticKDTree* verification_tree_) :
  Experiment(data_set_name_, query_set_name_, num_points_, num_queries_, dimensionality_, strategy_switch_size_ ,verification_tree_),
  context_workload_name(context_workload_name_), context_num_queries(context_num_queries_), use_context_time(use_context_time_){
  experiment_name = "WorkloadContextExperiment";
  plotcustomization.logx = true;
  plotcustomization.exp_y = true;
  plotcustomization.exp_x = true;
  plotcustomization.ylabel_offset = 1;
}


valueIndexArr WorkloadContextExperiment::run_(tree_type tree_t){
  DataImporter importer;
  std::string data_set = E::data_sets[data_set_name];
  pointVec data = importer.loadData(data_set, num_points, dimensionality);
  std::string context_workload_location = E::query_sets[context_workload_name];
  query_vec context_workload = importer.loadQueries(context_workload_location, context_num_queries, dimensionality);
  std::string query_set = E::query_sets[query_set_name];
  query_vec queries = importer.loadQueries(query_set, num_queries, dimensionality);

  std::pair<KDTree*, double> tree_time = E::init_tree(tree_t, data, get_final_partition_size(tree_t), strategy_switch_size);
  KDTree* tree = tree_time.first;
  // std::chrono::duration<double> acc(tree_time.second);
  std::vector<valueIndex> res;
  size_t counter = 0;
  size_t index = 0;
  for (query_t q : context_workload){
    res.push_back(valueIndex(res.size() + 1, E::measure_query_time(tree, q, verification_tree)));
    if (counter % (context_workload.size()/10) == 0)
      std::cout << "done " << counter << " context queries\n";
    counter += 1;
    index += 1;
  }
  res[0].second += tree_time.second;
  if (!use_context_time)
    res.clear();
  counter = 0;
  for (query_t q : queries){
    res.push_back(valueIndex(res.size() + 1, E::measure_query_time(tree, q, verification_tree)));
    if (counter % (queries.size()/10) == 0)
      std::cout << "done " << counter << " queries\n";
    counter += 1;
  }
  delete tree;
  // res[0] += tree_time.second;
  return res;
}


