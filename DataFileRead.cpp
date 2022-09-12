#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <filesystem>

namespace fs = std::filesystem;

struct Data {
    int index;
    float M;
    float m;
};


int main() {
    // magMag_Temp_3_0_Size_20x20_seed_1523531177.csv
    fs::path APath("D:\\Coding\\Cpp\\IsingModel\\magMag_Temp_3_0_Size_20x20_seed_1523531177.csv");

    std::cout << "Exists: " << fs::exists(APath) << std::endl
              << "Root Name: " << APath.root_name() << std::endl
              << "Root Path: " << APath.root_path() << std::endl
              << "Relative Path: " << APath.relative_path() << std::endl;


    return 0;
}