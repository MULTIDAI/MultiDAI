#pragma once
#include <random>
#include "KDNode.h"
#include "ExperimentHelper.h"
#include <random>


class GenData {
public:
  static pointVec generate_random_dataset(int size, int dimensionality);
  static pointVec generate_coordinate_dataset(double spacing_x,double spacing_y, point_t low, point_t high);
};

