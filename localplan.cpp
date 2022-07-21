#include <time.h>
#include <cmath>
#include <math.h>  
#include <cstdlib>
#include <iostream>
#include <vector>

#include "kdtree.hpp"
#include "plotter.h"
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

std::vector<std::vector<double>> kdsearch(Kdtree::KdTree tree, std::vector<double> point, int depth);

Kdtree::KdTree makeKDTree(std::vector<std::vector<double>> cloud) {
  Kdtree::KdNodeVector nodes;
  for (int i = 0; i < cloud.size(); ++i) {
    //allSearch.push_back(cloud[i]);
    nodes.push_back(Kdtree::KdNode(cloud[i]));
    std::cout << cloud[i][0] << ' ' << cloud[i][1] << '\n';
  }
  Kdtree::KdTree tree(&nodes);
  return tree;
  }


std::vector<std::vector<double>> cloudToPath(Kdtree::KdTree tree, double radius, double meanx, double meany) {
  std::vector<std::vector<double>> allSearch;
  double center[2] = {meanx, meany};
  Kdtree::KdNodeVector result;
  double startdist = 16;

  /*

  double viewVec[2] = {meanx - location[0], meany - location[1]};
  double perpVec[2] = {10 * viewVec[1], -10 * viewVec[0]};
  std::vector<double> location1(2);
  std::vector<double> closest1(2);
  location1[0] = meanx + perpVec[0];
  location1[1] = meany + perpVec[1];
  tree.k_nearest_neighbors(location1, 1, &result);
  (*point1)[0] = result[0].point[0];
  (*point1)[1] = result[0].point[1];

  std::vector<double> location2(2);
  std::vector<double> closest2(2);
  location2[0] = meanx - perpVec[0];
  location2[1] = meany - perpVec[1];
  tree.k_nearest_neighbors(location2, 1, &result);
  (*point2)[0] = result[0].point[0];
  (*point2)[1] = result[0].point[1];
  */


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
  datalog(allSearch, "data.dat");
 
  return allSearch;
}

std::vector<std::vector<double>> straightLine(std::vector<double> point1, std::vector<double> point2) {
  
  std::vector<std::vector<double>> vecOut;
  double x1 = point1[0]; 
  double y1 = point1[1]; 
  double x2 = point2[0]; 
  double y2 = point2[1]; 


  for (double i = 0; i <= 1; i += 0.01) {
    
    std::vector<double> vecTemp(2);
    vecTemp[0] = x1 + (x2 - x1) * i;
    vecTemp[1] = y1 + (y2 - y1) * i;
    vecOut.push_back(vecTemp);
  }
  return vecOut;
}

std::vector<std::vector<double>>  minMaxPath(std::vector<std::vector<double>> path, std::vector<double> pos) {
  std::vector<std::vector<double>> vecOut;
  int mini = 0;
  int maxi = 0;

  double maxAng = atan2(path[0][1] - pos[1], path[0][0] - pos[0]);
  double minAng = atan2(path[0][1] - pos[1], path[0][0] - pos[0]);

  double offset = 0;
  double del = 0;
  double angle = maxAng;
  double prev = 0;

  for (int i = 0; i < path.size(); i++) {
    prev = angle;
    angle = atan2(path[i][1] - pos[1], path[i][0] - pos[0]) + offset;
    del = angle - prev;
    //std::cout << path[i][1] - pos[1] << ' ' << path[i][0] - pos[0] << '\n';
    if ((del) > 3) {
      offset =  -2 * 3.14159265359;
      angle += offset;
    }
    if (del < -3) {
      offset = 2 * 3.14159265359;
      angle += offset;
    }
    if (angle > maxAng) {
      maxAng = angle;
      maxi = i;
    }
    if (angle < minAng) {
      minAng = angle;
      mini = i;
    }
    
  }
  vecOut.push_back(path[mini]);
  vecOut.push_back(path[maxi]);
  return vecOut;
}

std::vector<std::vector<double>> minMaxCloud(std::vector<std::vector<double>> minMaxPath, double radius, std::vector<double> loc) {
  std::vector<double> slope1(2);
  slope1[0] = minMaxPath[0][0] - loc[0];
  slope1[1] = minMaxPath[0][1] - loc[1];

  std::vector<double> slope2(2);
  slope2[0] = minMaxPath[1][0] - loc[0];
  slope2[1] = minMaxPath[1][1] - loc[1];

  std::vector<double> normslope1(2);
  normslope1[0] = slope1[0] / sqrt(slope1[0] * slope1[0] + slope1[1] * slope1[1]);  
  normslope1[1] = slope1[1] / sqrt(slope1[0] * slope1[0] + slope1[1] * slope1[1]);  

  std::vector<double> normslope2(2);
  normslope2[0] = slope2[0] / sqrt(slope2[0] * slope2[0] + slope2[1] * slope2[1]);  
  normslope2[1] = slope2[1] / sqrt(slope2[0] * slope2[0] + slope2[1] * slope2[1]);  
  
  minMaxPath[0][0] -= radius * normslope1[1];
  minMaxPath[0][1] += radius * normslope1[0];

  minMaxPath[1][0] += radius * normslope2[1];
  minMaxPath[1][1] -= radius * normslope2[0];

  return minMaxPath;
}

bool lineSegSide(std::vector<double> point1, std::vector<double> point2, std::vector<double> point, std::vector<double> origin) {
  double value = (point[0] - point1[0])*(point2[1] - point1[1]) - (point[1] - point1[1])*(point2[0] - point1[0]);
  double valueO = (origin[0] - point1[0])*(point2[1] - point1[1]) - (origin[1] - point1[1])*(point2[0] - point1[0]);
  return !(signbit(value) ^ signbit(valueO));
}