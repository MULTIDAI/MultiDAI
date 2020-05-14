#include "DataImporter.h"
#include "Helper.h"


std::vector<std::pair<double, double>> DataImporter::load_experiment_result(filename file){
  std::vector<std::pair<double, double>> res;
  std::ifstream infile(file);
  std::string line;
  std::getline(infile, line);
  while (std::getline(infile, line)) {
    std::istringstream iss(line);
    value_t index, value;
    iss >> index;
    iss >> value;
    res.push_back(std::pair<double, double>(index, value));
  }
  return res;
}


pointVec DataImporter::loadData(filename file, int amount = 1000, int dimensionality){
  pointVec res;
  std::ifstream infile(file);
  std::string line;
    
  while (std::getline(infile, line) && amount > 0)
    {
      std::istringstream iss(line);
      int method, row;
      iss >> method;
      iss >> row;
      
      point_t pt = read_point(iss, dimensionality);
      
      if(pt.size() != (uint) dimensionality){
        std::cout << "BREAKING: The data set has dimensionality " << pt.size() << " while the desired dimensionality was " << dimensionality << " \n";
        break;
      }
      
      if(method == 1){
        res.push_back(pt);
        amount--;
      }
    }
  if (amount > 0){
    std::cout << "There are not enough data points to fetch, missing: "<< amount <<" data points.  \n";
  }
  if (amount > 0){
    std::cout << "There are not enough data points to fetch, missing: "<< amount <<" data points.  \n";
  }
  return res;
}

pointVec DataImporter::load_all_data(filename file, int dimensionality){
  pointVec res;
  std::ifstream infile(file);
  std::string line;
    
  while (std::getline(infile, line))
    {
      std::istringstream iss(line);
      int method, row;
      iss >> method;
      iss >> row;
      
      point_t pt = read_point(iss, dimensionality);
      
      if(pt.size() != (uint) dimensionality){
        std::cout << "BREAKING: The data set has dimensionality " << pt.size() << " while the desired dimensionality was " << dimensionality << " \n";
        break;
      }
      
      if(method == 1){
        res.push_back(pt);
      }
    }
  return res;
}

queryList DataImporter::loadQueries(filename file, int amount, int dimensionality){
  queryList res;
  std::ifstream infile(file);
  std::string line;
  while (std::getline(infile, line) && amount > 0){
    std::istringstream iss(line);
    int method, row;
    iss >> method;
    iss >> row;
    value_t data_point;
    point_t temp;

    std::pair<point_t,point_t> query = read_query(iss, dimensionality);
    point_t low = query.first;
    point_t high = query.second;
    if(high.size() != low.size() || high.size() < (uint) dimensionality){
      std::cout << "BREAKING: Query low has dimensionality " << low.size() << " High has dimensionality "<< high.size() <<" while the desired dimensionality was " << dimensionality << " \n";
      break;
    }
    if(method == 2){
      std::pair<point_t, point_t> query = {low, high};
      res.push_back(query);
      amount--;
    }
  }
  if (amount > 0){
    std::cout << "There are not enough queries to fetch, missing: "<< amount <<" queries.  \n";
  }
  infile.close();
  return res;
}

queryList DataImporter::load_all_queries(filename file, int dimensionality){
  queryList res;
  std::ifstream infile(file);
  std::string line;
  while (std::getline(infile, line)){
    std::istringstream iss(line);
    int method, row;
    iss >> method;
    iss >> row;
    point_t temp;

    std::pair<point_t,point_t> query = read_query(iss, dimensionality);
    point_t low = query.first;
    point_t high = query.second;
    if(high.size() != low.size() || high.size() < (uint) dimensionality){
      std::cout << "BREAKING: Query low has dimensionality " << low.size() << " High has dimensionality "<< high.size() <<" while the desired dimensionality was " << dimensionality << " \n";
      break;
    }
    if(method == 2){
      std::pair<point_t, point_t> query = {low, high};
      res.push_back(query);
    }
  }
  infile.close();
  return res;
}

point_t DataImporter::read_point(std::istringstream &iss, int dimensionality){
  value_t data_point;
  point_t pt;
  int i = 0;
  while(!(iss.eof()) && i < dimensionality){
    iss >> data_point;
    pt.push_back(data_point);
    i++;
  }
  return pt;
}

std::pair<point_t, point_t> DataImporter::read_query(std::istringstream &iss, int dimensionality){
  value_t data_point;
  point_t temp;

  while(!(iss.eof())){
    iss >> data_point;
    temp.push_back(data_point);
  }
  if(temp.size() < (dimensionality / 2)){
    std::cout << "WARNING: Dimensionality of queries in file is not " << dimensionality << "/n";
  }
  int i = 0;
  point_t low;
  point_t high;
  for(value_t v : temp){
    if(i < dimensionality){
      low.push_back(v);
    }

    if((i >= (temp.size() / 2) && i < ((temp.size() / 2) + dimensionality)) ){
      high.push_back(v);
    }
    i++;
  } 

  std::pair<point_t, point_t> res = {low, high};
  return res;
}
