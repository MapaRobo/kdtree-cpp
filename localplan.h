#include <vector>
#include "kdtree.hpp"


double distanceSquared(std::vector<double> point1, std::vector<double> point2);

void print_nodes(const Kdtree::KdNodeVector &nodes) ;

std::vector<std::vector<double>> cloudToPath(Kdtree::KdTree tree, double radius, double meanx, double meany);

Kdtree::KdTree makeKDTree(std::vector<std::vector<double>> cloud);

std::vector<std::vector<double>> viewCone(Kdtree::KdTree tree, std::vector<double> location, double meanx, double meany);

std::vector<std::vector<double>> straightLine(std::vector<double> point1, std::vector<double> point2);

std::vector<std::vector<double>>  minMaxPath(std::vector<std::vector<double>> path, std::vector<double> pos);

std::vector<std::vector<double>> minMaxCloud(std::vector<std::vector<double>> minMaxPath, double radius, std::vector<double> loc);

bool lineSegSide(std::vector<double> point1, std::vector<double> point2, std::vector<double> point, std::vector<double> origin);