#include "WorkloadGenerator.h"
#include "FileWriter.h"
#include <algorithm>


query_vec Gen::generate_random_queries_in_bounded_sections(double sections, double queries_per_section, double section_size){
  std::vector<query_t> res;
  std::default_random_engine re;
  while(sections > 0)
    {
      // Construct uniform distribution within field 0.0 and 1.0 on both x and y axis
      std::uniform_real_distribution<double> unif(0.0,1.0);
        
      // Construct box of correct size in x dimension
      double low_x = unif(re);
      double high_x = low_x + section_size;

      // Construct box of correct size in y dimension
      double low_y = unif(re);
      double high_y = low_y + section_size;

      point_t low = {low_x, low_y};
      point_t high = {high_x, high_y};
        
      res.push_back(query_t(low, high));
      query_t section_query = query_t(point_t(low_x, low_y), point_t(high_x, high_y));
      double queries_per_section_ = queries_per_section;
      std::uniform_real_distribution<double> unif_x(low_x, high_x);
      std::uniform_real_distribution<double> unif_y(low_y, high_y);
      generate_random_bounded_queries(&res, queries_per_section_,re, unif_x, unif_y, high_x, high_y);
      sections--;
    }
  return res;
}

query_vec Gen::generate_random_workload(int amount, double query_max_size, int dimensionality){
  std::uniform_real_distribution<double> unif(0.0,1.0);
  std::default_random_engine re;
  std::vector<query_t> res;
  for(int i = 0; i < amount; i++){
    point_t low;
    point_t high;
    for(int d = 0; d < dimensionality; d++){
      double low_val = unif(re);
      std::uniform_real_distribution<double> unif_interval(low_val,low_val + query_max_size);
      double high_val = unif_interval(re);
      low.push_back(low_val);
      high.push_back(high_val);      
    }
    query_t q = {low, high};
    res.push_back(q);
  }
  return res;
}

void Gen::generate_random_bounded_queries(std::vector<query_t> *vec, double queries_per_section_, std::default_random_engine re, std::uniform_real_distribution<double> unif_x, std::uniform_real_distribution<double> unif_y, double high_x, double high_y){
  while(queries_per_section_ > 0){
    // Construct new uniform distributions within current section 
    // x-coordinates
    double low_x_inside = unif_x(re);
    std::uniform_real_distribution<double> unif_x_high(low_x_inside, high_x);
    double high_x_inside = unif_x_high(re);

    // y-coordinates
    double low_y_inside = unif_y(re);
    std::uniform_real_distribution<double> unif_y_high(low_y_inside, high_y);
    double high_y_inside = unif_y_high(re);

    // Construct query
    point_t low = {low_x_inside, low_y_inside};
    point_t high = {high_x_inside, high_y_inside};
    query_t res = query_t(low, high);
    // query_t res = generate_random_bounded_query(re, unif_x, unif_y, high_x, high_y);
    vec->push_back(res);
    queries_per_section_--;
  }
}

std::vector<std::pair<point_t, point_t>> Gen::generate_zoom_in_workload(double sections, double queries_per_section, double section_size){
  std::vector<query_t> res;
  std::default_random_engine re;
  while(sections > 0){
    // Construct uniform distribution within field 0.0 and 1.0 on both x and y axis
    std::uniform_real_distribution<double> unif(0.0,1.0);
        
    // Construct box of correct size in x dimension
    double low_x = unif(re);
    double high_x = low_x + section_size;

    // Construct box of correct size in y dimension
    double low_y = unif(re);
    double high_y = low_y + section_size;

    point_t low = {low_x, low_y};
    point_t high = {high_x, high_y};
    query_t section_query = query_t(low, high);
    res.push_back(section_query);
    double queries_per_section_ = queries_per_section;
    generate_zoom_in_queries(&res, queries_per_section_, section_size, low, high);
    sections--;
  }
  return res;
}

void Gen::generate_zoom_in_queries(std::vector<query_t> *vec, double queries_per_section, double section_size, point_t low, point_t high){
  double step_size = (section_size / queries_per_section) / 2;

  while(queries_per_section > 0){
    low.at(0) += step_size;
    low.at(1) += step_size;
    high.at(0) -= step_size;
    high.at(1) -= step_size;
    point_t low_new = {low.at(0), low.at(1)};
    point_t high_new = {high.at(0), high.at(1)};
    query_t query = query_t(low_new, high_new);
    vec->push_back(query);
    queries_per_section--;
  }
}
  

query_t Gen::generate_random_bounded_query(std::default_random_engine re, std::uniform_real_distribution<double> unif_x, std::uniform_real_distribution<double> unif_y, double high_x, double high_y){
  // NOTE: Currently not working due to random engine not being passed around properly
    
  // Construct new uniform distributions within current section 
  // x-coordinates
  double low_x_inside = unif_x(re);
  std::uniform_real_distribution<double> unif_x_high(low_x_inside, high_x);
  double high_x_inside = unif_x_high(re);

  // y-coordinates
  double low_y_inside = unif_y(re);
  std::uniform_real_distribution<double> unif_y_high(low_y_inside, high_y);
  double high_y_inside = unif_y_high(re);

  // Construct query
  point_t low = {low_x_inside, low_y_inside};
  point_t high = {high_x_inside, high_y_inside};
  query_t res = query_t(low, high);
  return res;
}


std::vector<query_t> Gen::generate_zoom_out_workload(double sections, double queries_per_section, double section_size){
  std::vector<query_t> res = generate_zoom_in_workload(sections, queries_per_section, section_size);
  std::reverse(res.begin(), res.end());
  return res;
}


std::vector<query_t> Gen::generate_z_sequential_workload(double query_size) {
  std::vector<query_t> res;
  value_t delta = (query_size/2);
  value_t x_coord = 0; 
  value_t y_coord = 0;
  value_t x_direction = 1;
  value_t y_direction = -1;
  res.push_back(query_t(point_t{x_coord, y_coord}, point_t{x_coord + query_size, y_coord + query_size}));
  while (x_coord < 1 || y_coord < 1) {
    if (x_coord < (0 + delta) && x_direction == -1) {
      x_coord = 0;
      y_coord += query_size;
      x_direction = 1;
      y_direction = -1;
    } else if (x_coord >= 1 - delta && x_direction == 1) {
      x_coord = 1;
      y_coord += query_size;
      x_direction = -1;
      y_direction = 1;
    } else if (y_coord < 0 + delta && y_direction == -1) {
      y_coord = 0;
      x_coord += query_size;
      y_direction = 1;
      x_direction = -1;
    } else if (y_coord >= 1 - delta && y_direction == 1) {
      y_coord = 1;
      x_coord += query_size;
      y_direction = -1;
      x_direction = 1;
    } else {
      x_coord += x_direction * query_size;
      y_coord += y_direction * query_size;
      if (x_coord < 0 + delta)
        x_coord = 0;
      if (y_coord < 0 + delta)
        y_coord = 0;
    }
    res.push_back(query_t(point_t{x_coord, y_coord}, point_t{x_coord + query_size, y_coord + query_size}));
  }
  return res;
}

std::vector<query_t> Gen::generate_sequential_workload(double query_size) {
  std::vector<query_t> res;
  value_t x_coord = 0; 
  value_t y_coord = 0;
  res.push_back(query_t(point_t{x_coord, y_coord}, point_t{x_coord + query_size, y_coord + query_size}));
  while (x_coord < 1) {
    x_coord += query_size;
    y_coord += query_size;
    res.push_back(query_t(point_t{x_coord, y_coord}, point_t{x_coord + query_size, y_coord + query_size}));
  }
  // std::cout << "Done generating points, size: " << res.size() << "\n";
  return res;
}

std::vector<std::vector<double>> Gen::boxxy_visualisation(std::vector<query_t> queries){
  std::vector<std::vector<double>> res;
  for (auto it = queries.begin(); it != queries.end(); it++){
    query_t q = (*it);
    double xvar = (q.second[0] - q.first[0])/2;
    double yvar = (q.second[1] - q.first[1])/2;
    double xcenter = q.first[0] + xvar;
    double ycenter = q.first[1] + yvar;
    res.push_back(std::vector<double>{xcenter, ycenter, xvar, yvar});
  }
  return res;
}

std::vector<std::vector<double>> Gen::boxxy_access_pattern(std::vector<query_t> queries, uint dim){
  std::vector<std::vector<double>> res;
  double query_index = 1;
  for (auto it = queries.begin(); it != queries.end(); it++){
    query_t q = (*it);
    double var = (q.second[dim] - q.first[dim])/2;
    double center = q.first[dim] + var;
    res.push_back(std::vector<double>{query_index, center, 0, var});
    query_index++;
  }
  return res;
}
