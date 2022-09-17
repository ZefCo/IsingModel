#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include "csv.h"
namespace fs = std::filesystem;


struct csvFile {
    std::vector<long> index;
    std::vector<int> M;
    std::vector<float> m;
};


int StringToNumber(std::string inputString) {
    std::istringstream converter(inputString);
    int result;

    converter >> result;
    return result;
}



csvFile parserow(std::string row, csvFile dataframe) {
    std::istringstream r(row);
    std::string p;
    int position = 0;

    while (getline(r, p, ',')) {
        switch (position) {
            case 0:
                // int conm = StringToNumber(M)
                
                dataframe.index.push_back(std::stol(p));
                break;
            case 1:
                // int M = StringToNumber(p);
                dataframe.M.push_back(std::stoi(p));

                break;
            case 2:
                dataframe.m.push_back(std::stof(p));
                break;
        }
        position++;
    }

    return dataframe;

}


csvFile parsecsv(std::string testfile, csvFile dataframe) {
    std::ifstream InFile(testfile);
    std::string testline;
    int Field = 0;

    while(getline(InFile, testline, '\n')) {
        switch (Field)
        {
        // Header case, I don't want to do anything with it.
        case 0:
            break;

        // all other cases. Formatted as long, int, float
        default:
            dataframe = parserow(testline, dataframe);
            break;
        }
        Field++;

    }

    InFile.close();

    return dataframe;

}



main() {
    std::string testfile, testline;
    // csvFile mM;
    // std::vector<long> index;
    // // std::vector<int> Mag;
    // // std::vector<float> mag;
    
    testfile = "D:\\Coding\\Cpp\\IsingModel\\DataFiles\\Test.csv";

    // std::cout << testfile << std::endl;

    // mM = parsecsv(testfile, mM);

    // for (int i = 0; i < mM.index.size(); i++) {
    //     std::cout << "Index = " << mM.index.at(i) << "\tM = " << mM.M.at(i) << "\tm = " << mM.m.at(i) << std::endl; 
    // }

    // // do {
    // //     currentline = getline(InFile, testline, ',');
    // //     std::cout << currentline;
    // // }

    io::CSVReader<3> in(testfile);
    in.read_header(io::ignore_extra_column, "vendor", "size", "speed");
    std::string vendor; int size; double speed;
    while(in.read_row(vendor, size, speed)){
        
  }

}