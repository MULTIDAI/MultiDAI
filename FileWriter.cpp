#include "FileWriter.h"
#include <iostream>
#include <fstream>
#include <regex>
#include "Helper.h"
using namespace std;




filename FileWriter::write_file_data(filename file_name, valueIndexArr xs, std::string tree){
  ofstream myfile;
  myfile.open (file_name);
  // file = std::regex_replace(file, std::regex("_"), "\\\\_");
  std::replace(tree.begin(), tree.end(), ' ', '-');
  myfile << tree << "\n";
  for(auto it_x = xs.begin(); it_x < xs.end(); it_x++){
    myfile << it_x->first << " " << it_x->second << "\n";
  }
  myfile.close();
  return file_name;
}

filename FileWriter::write_queries(filename file_name, query_vec queries){
  ofstream myfile;
  myfile.open (file_name);
  int i = 0;
  for(auto it = queries.begin(); it < queries.end(); it++){
    myfile << "2 " << i << " ";
    std::vector<double> low = (*it).first;
    std::vector<double> high = (*it).second;
    for(auto l = low.begin(); l < low.end(); l++){
      myfile << *l << " ";
    }
    for(auto l = high.begin(); l < high.end(); l++){
      myfile << (*l) << " ";
    }
    myfile << "\n";
    i++;
  }
  myfile.close();
  return file_name;
}

filename FileWriter::write_data_points(filename file_name, pointVec data_points){
  ofstream myfile;
  myfile.open (file_name);
  for(auto it = data_points.begin(); it < data_points.end(); it++){
    std::vector<double> line = (*it);
    myfile << "1 " << "999999 ";
    for(auto l = line.begin(); l < line.end(); l++){
      myfile << (*l) << " ";
    }
    myfile << "\n";
  }
  return file_name;
}

filename FileWriter::generic_vector_vector_writer(filename file_name, std::vector<std::vector<double>> data){
  ofstream myfile;
  myfile.open (file_name);
  for(auto it = data.begin(); it < data.end(); it++){
    std::vector<double> line = (*it);
    for(auto l = line.begin(); l < line.end(); l++){
      myfile << (*l) << " ";
    }
    myfile << "\n";
  }
  return file_name;
}
