#include "DatasetGenerator.h"
#include "FileWriter.h"


pointVec GenData::generate_coordinate_dataset(double spacing_x, double spacing_y , point_t low, point_t high){
  double x_low = low.at(0);
  double x_high = high.at(0);
  pointVec res;
  while(x_low < x_high){
    double y_low = low.at(1);
    double y_high = high.at(1);
    while(y_low < y_high){
      point_t current = {x_low, y_low};
      res.push_back(current);
      y_low += spacing_y;
    }
    x_low += spacing_x;
  }
  return res;
}
// TODO: should we add a vector describing the domain of the data points?
pointVec GenData::generate_random_dataset(int amount_of_points, int dimensionality){
  pointVec res;
  std::default_random_engine re;
  std::uniform_real_distribution<double> unif(0.0,1.0);
  for (int i = 0; i < amount_of_points; i++){
    point_t pt;
    for (int d = 0; d < dimensionality; d++){
      double dim_val = unif(re);
      pt.push_back(dim_val);
    }
    res.push_back(pt);
  }
  return res;
}

std::vector<std::vector<double>> GenData::visualization(pointVec points) {
    std::vector<std::vector<double>> res;
  for (auto it = points.begin(); it != points.end(); it++){
    point_t q = (*it);
    res.push_back(q);
  }
  return res;
}
