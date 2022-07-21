#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "plotter.h"
 
int datalog(std::vector<std::vector<double>> vec, std::string name) {
    std::ofstream file(name, std::ofstream::trunc);
    file << "#x y" << std::endl;
    for(int i=0; i<vec.size(); i++){
        file << vec[i][0] << ' ' << vec[i][1] << std::endl;
    }
    file.close();
    return 0;
}
