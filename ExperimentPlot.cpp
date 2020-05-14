#include "ExperimentPlot.h"


void Plot::plot_to_image(std::vector<std::pair<std::string, tree_type>> *names, std::string out,
                         std::string title, std::string xlabel, std::string ylabel,
                         plot_customization plotcustomization, bool latex){
  GnuplotPipe gp;
  double tex_x_size = 8;
  double tex_y_size = 6;
  gp.sendLine("set xlabel '" + xlabel + "' offset 0,0.5");
  gp.sendLine("set ylabel '" + ylabel + "' offset " + std::to_string(plotcustomization.ylabel_offset) + ",0");
  if (plotcustomization.logx)
    gp.sendLine("set logscale x 10");
  if (plotcustomization.exp_x) {
    gp.sendLine("set format x \"$10^{%T}$\""); 
    gp.sendLine("set xrange [0.9:]");
  } else if (plotcustomization.short_xlabel) {
    gp.sendLine("set format x \"$%2.0t$\\\\texttimes$10^{%L}$\""); 
    // gp.sendLine("set xrange [-0.5:]");
  }
  if (plotcustomization.keylocation.use)
    gp.sendLine("set key " + plotcustomization.keylocation.location);
  if (plotcustomization.yrange.use)
    gp.sendLine("set yrange [" + std::to_string(plotcustomization.yrange.low) + ":" +
                std::to_string(plotcustomization.yrange.high) +  "]");
  if (plotcustomization.exp_y)
    gp.sendLine("set format y \"$10^{%T}$\""); 
  if (plotcustomization.logy)
    gp.sendLine("set logscale y 10");
  if (!plotcustomization.enable_xtics)
    gp.sendLine("unset xtics");
  if (!plotcustomization.enable_ytics){
    // gp.sendLine("unset ytics");
    gp.sendLine("set format y \"\""); 
    tex_x_size -= 0.5;
  }
  if (!plotcustomization.enable_xlabel)
    gp.sendLine("unset xlabel");
  if (!plotcustomization.enable_ylabel){
    gp.sendLine("unset ylabel");
    tex_x_size -= 0.5;
  }
  if (!plotcustomization.enable_keys)
    gp.sendLine("unset key");
  if (latex) {
    gp.sendLine("set terminal epslatex size " + std::to_string(tex_x_size) + "cm," + std::to_string(tex_y_size) + "cm");
    out += ".tex";
  }
  else {
    gp.sendLine("set title '" + title + "'");
    gp.sendLine("set terminal png size 800,600");
    out += ".png";
  }
  // gp.sendLine("set xtics auto");
  gp.sendLine("set output '" + out + "'");
  std::string name;
  for (std::pair<std::string, tree_type> n : (*names)){
    std::cout << "adding name: " << n.first << "\n";
    name += "'" + n.first + "' title columnheader(1) ps " + std::to_string(plotcustomization.point_size) + " lc \"" + E::tree_colors[n.second] + "\", ";
  }
  std::cout << ("gnuplot: " + name) << "\n";
  gp.sendLine("plot " + name);
  std::cout << "Created " << out << "\n";
}

