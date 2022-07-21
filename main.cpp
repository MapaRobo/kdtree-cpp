#include <vector>
#include "localplan.h"

int main() {
  std::vector<std::vector<double>> vec;
  int i, j = 0;
  for (int i = 0; i < 5; i++) {
    vec.push_back({0, i});
  }
  for (int j = 0; j < 5; j++) {
    vec.push_back({j, 4});
  }
  cloudToPath(vec, 3); 

}
