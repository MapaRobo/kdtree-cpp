#include <time.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "kdtree.hpp"
#include "test.h"
#include "localplan.h"

#define PI 3.14159265359

double distanceSquared(std::vector<double> point1, std::vector<double> point2) {
  double delx = point1[0] - point2[0];
  double dely = point1[1] - point2[1];
  double squaredDistance = delx * delx + dely * dely;
  return (squaredDistance);
}

void print_nodes(const Kdtree::KdNodeVector &nodes) {
  size_t i,j;
  for (i = 0; i < nodes.size(); ++i) {
    if (i > 0)
      std::cout << " ";
    std::cout << "(";
    for (j = 0; j < nodes[i].point.size(); j++) {
      if (j > 0)
        std::cout << ",";
      std::cout << nodes[i].point[j];
    }
    std::cout << ")";
  }
  std::cout << std::endl;
}

std::vector<std::vector<double>> cloudToPath(std::vector<std::vector<double>> cloud, double radius) {
  std::vector<std::vector<double>> allSearch;
  std::cout << "Size of point cloud: " << cloud.size() << '\n';
  Kdtree::KdNodeVector nodes;
  double meanx = 0;
  double meany = 0;
  for (int i = 0; i < cloud.size(); ++i) {
    nodes.push_back(Kdtree::KdNode(cloud[i]));
    std::cout << cloud[i][0] << ' ' << cloud[i][1] << '\n';
    meanx += cloud[i][0];
    meany += cloud[i][1];
  }
  double center[2] = {meanx / ((double) cloud.size()), meany / ((double) cloud.size())};

  Kdtree::KdTree tree(&nodes);
  Kdtree::KdNodeVector result;

  double startdist = 16;

  for (double rad = 0; rad < 2 * PI; rad += PI / 48) {
    for (double dist = startdist; dist > 0; dist -= 0.01) {
      std::vector<double> test_point(2);
      std::vector<double> closest(2);
      test_point[0] = center[0] + dist * cos(rad);
      test_point[1] = center[1] + dist * sin(rad);
      tree.k_nearest_neighbors(test_point, 1, &result);
      closest[0] = result[0].point[0];
      closest[1] = result[0].point[1];
      double dist2 = distanceSquared(test_point, closest);
      if (dist2 < radius * radius) {
        allSearch.push_back(test_point);
        break; 
      }
    }
  }
  datalog(allSearch);
  return allSearch;
}
