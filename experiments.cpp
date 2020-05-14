#include "ExperimentHelper.h"
#include "SkyserverExperiment.h"
#include "ConvergenceExperiment.h"
#include "CumulativeExperiment.h"
#include "PartitionSizeExperiment.h"
#include "VariableDataExperiment.h"
#include "DimensionalityExperiment.h"
#include "StrategySwitchSizeExperiment.h"
#include "WorkloadGenerator.h"
#include "DatasetGenerator.h"
#include <sys/stat.h>
#include <string.h>

std::vector<tree_type> fast_names = std::vector<tree_type>
  {lazyckd, quasii, hdd1r, dd1r};

std::vector<tree_type> original_comparison_names = std::vector<tree_type>
  {ckd, lazyckd, quasii, kd};

std::vector<tree_type> dd1r_names = std::vector<tree_type>
  {quasii, hmdd1r, lazyckd};

std::vector<tree_type> skyserver_comp = std::vector<tree_type>
  {ckd, lazyckd, quasii};

std::vector<tree_type> skyserver_trees = std::vector<tree_type>
  {quasii, ckd, lazyckd, hdd1r, dd1r, dd1c};  // kd

std::vector<std::string> skyserver_workloads = std::vector<std::string>
  {
   // "skyserver sorted dim 0", "skyserver sorted dim 0 reversed",
   "skyserver regular", "skyserver regular reversed",
   "skyserver sorted total size", "skyserver sorted total size reversed",
   "skyserver sorted dim 1", "skyserver sorted dim 1 reversed",
   "skyserver sorted size dim 0", "skyserver sorted size dim 0 reversed",
   "skyserver sorted size dim 1", "skyserver sorted size dim 1 reversed",
    };

std::vector<std::string> skyserver_workloads_with_sequential = std::vector<std::string>
  {
   "skyserver sorted dim 0", "skyserver sorted dim 0 reversed",
   "skyserver regular", "skyserver regular reversed",
   "skyserver sorted total size", "skyserver sorted total size reversed",
   "skyserver sorted dim 1", "skyserver sorted dim 1 reversed",
   "skyserver sorted size dim 0", "skyserver sorted size dim 0 reversed",
   "skyserver sorted size dim 1", "skyserver sorted size dim 1 reversed",
    };

std::vector<std::string> skyserver_workloads_only_sequential = std::vector<std::string>
  {
   "skyserver sorted dim 0", "skyserver sorted dim 0 reversed",
    };

std::vector<std::string> skyserver_workloads_plot = std::vector<std::string>
  {
   "skyserver sorted dim 0", "skyserver sorted dim 0 reversed",
   "skyserver regular", "skyserver regular reversed",
   "skyserver sorted total size", "skyserver sorted total size reversed",
   // "skyserver sorted dim 1", "skyserver sorted dim 1 reversed",
   // "skyserver sorted size dim 0", "skyserver sorted size dim 0 reversed",
   // "skyserver sorted size dim 1", "skyserver sorted size dim 1 reversed",
    };

std::string standard_dataset = "random 240000 3d";
std::string sky4 = "Skyserver main 1 in 4";
std::string sky8 = "Skyserver main 1 in 8";
std::string sky16 = "Skyserver main 1 in 16";
std::string sky32 = "Skyserver main 1 in 32";
std::string sky64 = "Skyserver main 1 in 64";
std::string sky128 = "Skyserver main 1 in 128";

inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

key_location gen_location(std::string loc) {
    key_location kl;
  kl.use = true;
  kl.location = loc;
  return kl;
}

void random_3d_data() {
  std::string name = "data/random_3d_240000_points.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    pointVec data = GenData::generate_random_dataset(240000, 3);
    writer.write_data_points(name, data);
  }
}

void random_10d_data() {
  std::string name = "data/random_10d_240000_points.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    pointVec data = GenData::generate_random_dataset(240000, 10);
    writer.write_data_points(name, data);
  }
}

void random_20d_data() {
  std::string name = "data/random_20d_240000_points.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    pointVec data = GenData::generate_random_dataset(240000, 20);
    writer.write_data_points(name, data);
  }
}

void generate_datasets() {
  random_3d_data();
  random_10d_data();
  random_20d_data();
}

void zoomin_queries() {
  std::string name = "data/zoomin_1000_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_zoom_in_workload(5, 200, 0.2);
    writer.write_queries(name, queries);
  }
}

void zoomout_queries() {
  std::string name = "data/zoomout_1000_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_zoom_out_workload(5, 200, 0.2);
    writer.write_queries(name, queries);
  }
}

void random_bounded_queries() {
  std::string name = "data/random_bounded_1000_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_random_queries_in_bounded_sections(5, 200, 0.2);
    writer.write_queries(name, queries);
  }
}

void sequential_queries() {
  std::string name = "data/sequential_100_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_sequential_workload(0.01);
    writer.write_queries(name, queries);
  }
}

void sequential_queries_1000_reversed() {
  std::string name = "data/sequential_1000_reversed_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_sequential_workload(0.001);
    std::reverse(queries.begin(), queries.end());
    writer.write_queries(name, queries);
  }
}

void sequential_queries_1000() {
  std::string name = "data/sequential_1000_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_sequential_workload(0.001);
    writer.write_queries(name, queries);
  }
}

void z_sequential_queries() {
  std::string name = "data/z_sequential_queries_0_05.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_z_sequential_workload(0.05);
    writer.write_queries(name, queries);
  }
}

void random_3d_queries() {
  std::string name = "data/random_2000_3d_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_random_workload(2000, 0.01, 3);
    writer.write_queries(name, queries);
  }
}

void random_3d_queries_10000() {
  std::string name = "data/random_10000_3d_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_random_workload(10000, 0.01, 3);
    writer.write_queries(name, queries);
  }
}

void random_10d_queries() {
  std::string name = "data/random_2000_10d_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_random_workload(2000, 0.01, 10);
    writer.write_queries(name, queries);
  }
}

void random_20d_queries() {
  std::string name = "data/random_10000_20d_queries.data";
  if (!file_exists(name)) {
    std::cout << "generating " << name << "\n";
    FileWriter writer;
    std::vector<std::pair<point_t, point_t>> queries = Gen::generate_random_workload(10000, 0.01, 20);
    writer.write_queries(name, queries);
  }
}

void generate_workloads() {
  sequential_queries();
  sequential_queries_1000();
  sequential_queries_1000_reversed();
  z_sequential_queries();
  zoomin_queries();
  zoomout_queries();
  random_bounded_queries();
  random_3d_queries();
  random_3d_queries_10000();
  random_10d_queries();
  random_20d_queries();
}

std::vector<Experiment*> gen_experiments(bool all=false, bool verify=true, bool dimensionality_exp=false) {
  std::vector<Experiment*> experiments;
  DataImporter importer;
  std::string sd = "random 240000 3d"; // standard_dataset
  long snp = 240000; // standard_num_points
  uint sss = 1000; // strategy_switch_size
  StaticKDTree* vt; // verification_tree
  if (verify)
    vt = new StaticKDTree(importer.loadData(E::data_sets[sd], snp, 2)); 
  else 
    vt = nullptr;
  experiments.push_back(new ConvergenceExperiment(sd, "random 10000 3d", snp, 10000, 2, sss, vt));
  // experiments.push_back(new ConvergenceExperiment(sd, "sequential 100", snp, 100, 2, sss, vt));
  experiments.push_back(new ConvergenceExperiment(sd, "sequential 1000", snp, 1000, 2, sss, vt));
  if (all)
    experiments.push_back(new ConvergenceExperiment(sd, "z sequential 441", snp, 441, 2, sss, vt));
  // experiments.push_back(new ConvergenceExperiment(sd, "variable size 2000", snp, 2000, 2, sss,  vt));
  if (all)
    experiments.push_back(new ConvergenceExperiment(sd, "zoomin 1000", snp, 1000, 2, sss, vt));
  if (all)
    experiments.push_back(new ConvergenceExperiment(sd, "zoomout 1000", snp, 1000, 2, sss, vt));
  if (all)
    experiments.push_back(new ConvergenceExperiment(sd, "random bounded 1000", snp, 1000, 2, sss, vt));
  experiments.push_back(new CumulativeExperiment(sd, "random 10000 3d", snp, 10000, 2, sss, vt));
  // experiments.push_back(new CumulativeExperiment(sd, "sequential 100", snp, 100, 2, sss, vt));
  experiments.push_back(new CumulativeExperiment(sd, "sequential 1000", snp, 1000, 2, sss, vt));
  if (all)
    experiments.push_back(new CumulativeExperiment(sd, "z sequential 441", snp, 441, 2, sss, vt));
  // experiments.push_back(new CumulativeExperiment(sd, "variable size 2000", snp, 2000, 2, vt));
  if (all)
    experiments.push_back(new CumulativeExperiment(sd, "zoomin 1000", snp, 1000, 2, sss, vt));
  if (all)
    experiments.push_back(new CumulativeExperiment(sd, "zoomout 1000", snp, 1000, 2, sss, vt));
  if (all)
    experiments.push_back(new CumulativeExperiment(sd, "random bounded 1000", snp, 1000, 2, sss, vt));
  if (all)
    experiments.push_back(new PartitionSizeExperiment(sd, "random 10000 3d", snp, 10000, sss, vt));
  // experiments.push_back(new VariableDataExperiment(sd, "random 10000 3d", snp, 10000, 2, sss, vt));
  if (all || dimensionality_exp)
    experiments.push_back(new DimensionalityExperiment("random 240000 20d", "random 10000 20d", snp, 10000, 20, sss, nullptr));  
  if (all)
    experiments.push_back(new StrategySwitchSizeExperiment(sd, "random 10000 3d", snp, 10000, 2, sss, vt));  
  if (all)
    experiments.push_back(new StrategySwitchSizeExperiment(sd, "sequential 1000", snp, 1000, 2, sss, vt));  
    
  return experiments;
}

void set_trees(std::vector<Experiment*> experiments, std::vector<tree_type> trees) {
  for (Experiment* e : experiments) {
    e->trees = trees;
  }
}

void run_experiments(std::vector<Experiment*> experiments, bool fast=false) {
  for (Experiment* e : experiments) {
    e->run(fast);
  }
}

void plot_experiments(std::vector<Experiment*> experiments, bool latex=false) {
  for (Experiment* e : experiments) {
    e->plot(latex);
  }
}

void set_custom_file_addition(std::vector<Experiment*> experiments, std::string text) {
  for (Experiment* e : experiments) {
    e->custom_file_addition = text;
  }
}

void run_standard_experiments() {
  bool all = false;
  bool verify = true;
  bool dimensionality = true;
  bool do_run = true;
  bool run_fast = false;
  bool plot = false;
  bool epslatex = true;
  generate_datasets();
  generate_workloads();
  std::vector<Experiment*> experiments = gen_experiments(all, verify, dimensionality);
  set_trees(experiments, fast_names);
  if (do_run)
    run_experiments(experiments, run_fast);
  if (plot)
    plot_experiments(experiments, epslatex);

  experiments = gen_experiments(all, verify, dimensionality);
  set_custom_file_addition(experiments, "_comp");
  set_trees(experiments, original_comparison_names);
  if (do_run)
    run_experiments(experiments, run_fast);
  if (plot)
    plot_experiments(experiments, epslatex);
}

void run_skyserver_experiments(std::string sd, std::vector<std::string> workloads=skyserver_workloads,
                               std::vector<tree_type> trees=skyserver_trees) {
  uint sss = 1000; // strategy_switch_size
  StaticKDTree* vt; // verification_tree
  vt = new StaticKDTree(pointVec()); 
  vt->do_verify = false;
  for (std::string w : workloads){
    Experiment* experiment = new SkyserverExperiment(sd, w, -1, -1, 2, sss, vt);
    experiment->trees = trees;
    experiment->run();
  }
}

void gen_skyserver_graphs(std::string sd) {
  uint sss = 1000; // strategy_switch_size
  StaticKDTree* vt = nullptr; 
  bool epslatex = true;

  for (std::string w : skyserver_workloads_plot){
    Experiment* experiment_conv = new SkyserverExperiment(sd, w, -1, -1, 2, sss, vt);
    experiment_conv->trees = fast_names;
    // experiment_conv->custom_file_addition = "_Convergence";
    experiment_conv->plot(epslatex);

    Experiment* experiment_conv_comp = new SkyserverExperiment(sd, w, -1, -1, 2, sss, vt);
    experiment_conv_comp->trees = std::vector<tree_type>
      {ckd, lazyckd, quasii};
    experiment_conv_comp->custom_file_addition = "_comp";
    experiment_conv_comp->plot(epslatex);
  }
}

Experiment* set_experiment_vars(Experiment* e, std::vector<tree_type> trees, std::string file_addition, axis_range yrange, key_location keylocation, bool enable_ytics=true, bool enable_ylabel=true) {
  e->trees = trees;
  e->custom_file_addition = file_addition;
  e->plotcustomization.yrange = yrange;
  e->plotcustomization.keylocation = keylocation;
  e->plotcustomization.enable_ytics = enable_ytics;
  e->plotcustomization.enable_ylabel = enable_ylabel;
  return e;
}

std::vector<Experiment*> report_experiments() {
  uint sss = 1000; // strategy_switch_size
  std::vector<tree_type> ocn = original_comparison_names;
  std::vector<tree_type> fn = fast_names;
  std::vector<tree_type> scn = skyserver_comp;
  std::string sd = standard_dataset;
  std::string sd20 = "random 240000 20d";
  int sp = 240000;
  std::string rq = "random 10000 3d";
  int rqp = 10000;
  std::string rq20 = "random 10000 20d";
  std::string sq = "sequential 1000";
  int sqp = 1000;
  std::string skr = "skyserver regular";
  std::string sks = "skyserver sorted total size";
  std::string sksr = "skyserver sorted total size reversed";
  std::string skd = "skyserver sorted dim 0";

  axis_range yr;
  key_location kl;

  key_location kl_br = gen_location("bottom right");

  key_location kl_bl = gen_location("bottom left");

  key_location kl_tl = gen_location("top left");

  key_location kl_cbr = gen_location("at 10500,1.2");

  key_location kl_ctr = gen_location("at 1500,90");

  key_location kl_ctr2 = gen_location("at 1800,1");

  key_location kl_ctl = gen_location("at 550,90");

  key_location kl_dim = gen_location("at 13,850"); 

  key_location kl_skytr = gen_location("at 1800000,850");

  key_location kl_skytl = gen_location("at 600000,8500");

  key_location kl_skycl = gen_location("at 3000,0.03");

  axis_range yr4_comp;
  yr4_comp.use = true;
  yr4_comp.low = 10;
  yr4_comp.high = 10000;

  axis_range yr4;
  yr4.use = true;
  yr4.low = 10;
  yr4.high = 1000;

  plot_exps plot_conv;
  plot_conv.plot_cum = false;
  plot_exps plot_cum;
  plot_cum.plot_conv = false;
  bool all = true;

  std::vector<Experiment*> ex;
  if (all)
    ex.push_back(set_experiment_vars(new ConvergenceExperiment(sd, rq, sp, rqp, 2, sss, nullptr), ocn, "_comp", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new CumulativeExperiment(sd, rq, sp, rqp, 2, sss, nullptr), ocn, "_comp", yr, kl_cbr));
  if (all)
    ex.push_back(set_experiment_vars(new ConvergenceExperiment(sd, rq, sp, rqp, 2, sss, nullptr), fn, "", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new CumulativeExperiment(sd, rq, sp, rqp, 2, sss, nullptr), fn, "", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new ConvergenceExperiment(sd, sq, sp, sqp, 2, sss, nullptr), ocn, "_comp", yr, kl_ctr));
  if (all)
    ex.push_back(set_experiment_vars(new CumulativeExperiment(sd, sq, sp, sqp, 2, sss, nullptr), ocn, "_comp", yr, kl_br));
  if (all)
    ex.push_back(set_experiment_vars(new ConvergenceExperiment(sd, sq, sp, sqp, 2, sss, nullptr), fn, "", yr, kl_ctr2));
  if (all)
    ex.push_back(set_experiment_vars(new CumulativeExperiment(sd, sq, sp, sqp, 2, sss, nullptr), fn, "", yr, kl_ctl));
  if (all)
    ex.push_back(set_experiment_vars(new DimensionalityExperiment(sd20, rq20, sp, rqp, 20, sss, nullptr), ocn, "_comp", yr, kl_dim));
  if (all)
    ex.push_back(set_experiment_vars(new DimensionalityExperiment(sd20, rq20, sp, rqp, 20, sss, nullptr), fn, "", yr, kl_dim));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, skr, -1, -1, 2, sss, nullptr, plot_conv), ocn, "_comp", yr, kl_skytr));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, skr, -1, -1, 2, sss, nullptr, plot_cum), ocn, "_comp", yr, kl_skytl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, skr, -1, -1, 2, sss, nullptr, plot_conv), fn, "", yr, kl_skytr));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, skr, -1, -1, 2, sss, nullptr, plot_cum), fn, "", yr, kl_br));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, sks, -1, -1, 2, sss, nullptr, plot_conv), ocn, "_comp", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, sks, -1, -1, 2, sss, nullptr, plot_cum), ocn, "_comp", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, sks, -1, -1, 2, sss, nullptr, plot_conv), fn, "", yr, kl_skytr));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, sks, -1, -1, 2, sss, nullptr, plot_cum), fn, "", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, sksr, -1, -1, 2, sss, nullptr, plot_conv), ocn, "_comp", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, sksr, -1, -1, 2, sss, nullptr, plot_cum), ocn, "_comp", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, sksr, -1, -1, 2, sss, nullptr, plot_conv), fn, "", yr, kl_skytr));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, sksr, -1, -1, 2, sss, nullptr, plot_cum), fn, "", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky64, skd, -1, -1, 2, sss, nullptr, plot_conv), ocn, "_comp", yr, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky64, skd, -1, -1, 2, sss, nullptr, plot_cum), ocn, "_comp", yr, kl_br));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky64, skd, -1, -1, 2, sss, nullptr, plot_conv), fn, "", yr, kl_skycl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky64, skd, -1, -1, 2, sss, nullptr, plot_cum), fn, "", yr, kl_br));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, skr, -1, -1, 2, sss, nullptr, plot_cum), ocn, "_hz_comp", yr4_comp, kl_br));
  // if (all)
  //   ex.push_back(set_experiment_vars(new SkyserverExperiment(sky4, skr, -1, -1, 2, sss, nullptr, plot_cum), fn, "_hz", yr4, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky8, skr, -1, -1, 2, sss, nullptr, plot_cum), ocn, "_hz_comp", yr4_comp, kl_br, false, false));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky8, skr, -1, -1, 2, sss, nullptr, plot_cum), fn, "_hz", yr4, kl));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky16, skr, -1, -1, 2, sss, nullptr, plot_cum), ocn, "_hz_comp", yr4_comp, kl, false, false));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky16, skr, -1, -1, 2, sss, nullptr, plot_cum), fn, "_hz", yr4, kl, false, false));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky32, skr, -1, -1, 2, sss, nullptr, plot_cum), ocn, "_hz_comp", yr4_comp, kl, false, false));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky32, skr, -1, -1, 2, sss, nullptr, plot_cum), fn, "_hz", yr4, kl, false, false));
  if (all)
    ex.push_back(set_experiment_vars(new SkyserverExperiment(sky64, skr, -1, -1, 2, sss, nullptr, plot_cum), fn, "_hz", yr4, kl, false, false));

  return ex;
}

void plot_report_experiments() {
  bool epslatex = true;
  for (Experiment* e : report_experiments()) {
    e->plot(epslatex);
  }
}


int main(int argc, char *argv[]){
  generate_workloads();
  generate_datasets();
  // if (argc > 1) {
  //    if (argc != 2) {
  //    OUT << "Too many arguments: " << argc << "\n";
  //    for (int i = 0; i < argc; ++i) 
  //      std::cout << argv[i] << "\n"; 
  //    return 0;
  //  }    
  //  if (strcmp(argv[1], "sky128") == 0)
  //    run_skyserver_experiments(sky128, skyserver_workloads_with_sequential);
  //  else if (strcmp(argv[1], "sky64") == 0)
  //    run_skyserver_experiments(sky64, skyserver_workloads_with_sequential);
  //  else if (strcmp(argv[1], "sky32") == 0)
  //    run_skyserver_experiments(sky32);
  //  else if (strcmp(argv[1], "sky16") == 0)
  //    run_skyserver_experiments(sky16);
  //  else if (strcmp(argv[1], "sky8") == 0)
  //    run_skyserver_experiments(sky8);
//    else if (strcmp(argv[1], "sky4") == 0)
  //    run_skyserver_experiments(sky4);
  //  else if (strcmp(argv[1], "custom_run") == 0)
  //    run_skyserver_experiments(sky8, std::vector<std::string>{"skyserver regular"},
  //                              std::vector<tree_type>{dd1r});
  //  else if (strcmp(argv[1], "standard") == 0)
  //    run_standard_experiments();
  //  else if (strcmp(argv[1], "skytest") == 0)
  //    run_skyserver_experiments(sky32, skyserver_workloads_only_sequential);
  //  else if (strcmp(argv[1], "test") == 0)
  //    OUT << "HEY\n";
  //  else 
  //    OUT << "Argument \"" << argv[1] << "\" did not match expected input\n";
  //  return 0;
  // } 
  run_standard_experiments();
  plot_report_experiments();

  // run_skyserver_experiments(sd64, skyserver_workloads_with_sequential);
  // run_skyserver_experiments(sd32);
  // run_skyserver_experiments(sd16);
  // run_skyserver_experiments(sd8);
  // run_skyserver_experiments(sd4);
  // gen_skyserver_graphs(sd128);
  // gen_skyserver_graphs(sd64);
  // gen_skyserver_graphs(sd32);
  // gen_skyserver_graphs(sd16);
  // gen_skyserver_graphs(sd8);
  return 0;
}


