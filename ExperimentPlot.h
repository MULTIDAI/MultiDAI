#pragma once
#include "libs/gnuplot.h"
#include "ExperimentHelper.h"

struct axis_range {
  axis_range(): use(false),low(0),high(0) { }   // default Constructor
  bool use = false;
  double low = 0;
  double high = 0;
};

struct key_location {
  key_location(): use(false),location("") { }   // default Constructor
  bool use = false;
  std::string location;
};

struct plot_customization {
  plot_customization(): enable_xtics(true), enable_ytics(true), enable_xlabel(true), enable_ylabel(true),
                        enable_keys(false), logx(false), logy(true), exp_x(false), exp_y(false),
                        short_xlabel(true), point_size(0.5), ylabel_offset(2.5),
                        ylabel_name(""), yrange(axis_range()), xrange(axis_range()), keylocation(key_location()) {}
  bool enable_xtics = true;
  bool enable_ytics = true;
  bool enable_xlabel = true;
  bool enable_ylabel = true;
  bool enable_keys = false;
  bool logx = false;
  bool logy = true; 
  bool exp_x = false;
  bool exp_y = false; 
  bool short_xlabel = true;
  double point_size = 0.5;
  double ylabel_offset = 2.5;
  std::string ylabel_name;
  axis_range yrange;
  axis_range xrange;
  key_location keylocation;
};

class Plot{
public:
  static void plot_to_image(std::vector<std::pair<std::string, tree_type>> *names, std::string out,
                            std::string title, std::string xlabel, std::string ylabel,
                            plot_customization plotcustomization, bool latex=false);
};

