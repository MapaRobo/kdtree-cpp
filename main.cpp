#include <vector>
#include <iostream>
#include <cmath>
#include "localplan.h"
#include "kdtree.hpp"
#include "plotter.h"

int main() {
  std::vector<std::vector<double>> vecTree;
  std::vector<std::vector<double>> vec;
  int i, j = 0;
  vec.push_back({1, 1});
  vecTree.push_back({1, 1});
  for (double i = 1; i < 4; i++) {
    vec.push_back({0, i});
    vecTree.push_back({0, i});
  }
  vec.push_back({1, 3});
  for (double i = 1; i < 4; i++) {
    vec.push_back({i, 4});
    vecTree.push_back({i, 4});
  }
  vec.push_back({3, 3});
  double sizeVec = vec.size();
  double meanx = 0;
  double meany = 0;
  for (int i = 0; i < sizeVec; ++i) {
    meanx += vec[i][0];
    meany += vec[i][1];
  }
  meanx /= sizeVec;
  meany /= sizeVec;

  double center[2] = {meanx, meany};

  Kdtree::KdTree tree = makeKDTree(vecTree);


  std::vector<double> loc(2);
  loc[0] = -2.5;
  loc[1] = 8;
  std::vector<double> point1(2);
  std::vector<double> point2(2);

  double rad = 2;
  std::vector<std::vector<double>> path = cloudToPath(tree, rad, meanx, meany); 
  std::vector<std::vector<double>> mmPath = minMaxPath(path, loc);
  std::vector<std::vector<double>> mmCloud = minMaxCloud(mmPath, rad, loc);
  std::vector<std::vector<double>> conevec1 = straightLine(mmCloud[0], loc);
  std::vector<std::vector<double>> conevec2 = straightLine(mmCloud[1], loc);
  
  std::vector<double> point(2);

  std::vector<std::vector<double>> sidePath;

  //std::cout << path.size() << '\n';

  for (int i = 0; i < path.size(); i++) {
    std::cout << i << '\n';
    if (lineSegSide(mmCloud[0], mmCloud[1], path[i], loc)) {
      std::cout << "good!\n";
      sidePath.push_back(path[i]);
    }

  }

  std::cout << "end of prog\n";
  //v1[0] = 

  datalog(sidePath, "side.dat");
  datalog(conevec1, "cone1.dat");
  datalog(conevec2, "cone2.dat");
  datalog(vec, "obj.dat");

  std::cout << "end of prog";
  
}
