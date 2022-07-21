#include <vector>
#include "kdtree.hpp"


double distanceSquared(std::vector<double> point1, std::vector<double> point2);

void print_nodes(const Kdtree::KdNodeVector &nodes) ;

std::vector<std::vector<double>> cloudToPath(std::vector<std::vector<double>> cloud, double radius);


