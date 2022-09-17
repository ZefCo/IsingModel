#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;



struct csvFile {
    std::vector<long> index;
    std::vector<int> M;
    std::vector<float> m;
};



template<typename T>
std::vector<T> slice(std::vector<T> &v, int m, int n)
{
    std::vector<T> vec(n - m + 1);
    std::copy(v.begin() + m, v.begin() + n + 1, vec.begin());
    return vec;
}




int CheckFile(std::string filename) {
    fs::path FilePath(filename);

    // std::cout << "Exists: " << fs::exists(APath) << std::endl
    //           << "Root Name: " << APath.root_name() << std::endl
    //           << "Root Path: " << APath.root_path() << std::endl
    //           << "Relative Path: " << APath.relative_path() << std::endl;

    if (fs::exists(FilePath) == 0) {
        std::cout << "File does not exists" << std::endl
                  << "Realtive Path that was input:\n\t" << FilePath.relative_path() << std::endl
                  << "Unable to proceed, exiting" << std::endl;
                  
        exit(0);
    }

    return 0;

}



csvFile parsecsv2(std::string testfile) {
    csvFile dataFrame;
    std::ifstream inFile;
    std::string inLine, tokens;
    int Row = 0;

    inFile.open(testfile);

    while(getline(inFile, inLine, '\n')) {
        // std::cout << inLine << std::endl;
        // dataframe = parserow(inLine, dataframe);
        switch (Row) {
            case 0: break;
            default: 
                std::istringstream r(inLine);
                int p = 0;
                while(getline(r, tokens, ',')){
                    switch (p) {
                        case 0:
                            dataFrame.index.push_back(std::stol(tokens));
                            // std::cout << tokens << "\t";
                            break;
                        case 1:
                            dataFrame.M.push_back(std::stoi(tokens));
                            // std::cout << tokens << "\t";
                            break;
                        case 2:
                            dataFrame.m.push_back(std::stof(tokens));
                            // std::cout << tokens << std::endl;
                            break;
                    }
                    p++;
                }
                break;
        }
        Row++;
    }
    // std::cout << "Number of Rows = " << Row << std::endl;

    // while(!inFile.eof()) {
    //     std::string token;
    //     inFile >> inLine;
    //     std::cout << inLine << std::endl;

    //     while(getchar(inLine, token, ',')) {

    //     }
    // }
    inFile.close();
    return dataFrame;

}

// int ReadFile(std::string filename) {
//     std::ifstream infile(filename);
//     if(!infile) {
//         std::cerr << "File coulnd't be read!" << std::endl;
//         std::cerr << "Error Code: " << strerror(errno) << std::endl;
        
//         return -1;
//     }

//     int count = 0;
//     infile >> count;
//     std::cout << "File: " << filename << std::endl;
//     std::cout << "This file has " << count << " items." << std::endl;

//     int num = 0;
//     for (int i = 0; i < count; i++) {
//         infile >> num;
//         std::cout << num << std::endl;
//     }

//     infile.close();
//     return 0;

// }


std::vector<float> autocorrelation(std::vector<float> inputVector) {
    std::vector<float> acvector;
    long tmax = inputVector.size();

    for (int t = 0; t < tmax; t++) {
        float alpha = 1 / ((float)tmax - (float)t);
        float m1 = 0, m2 = 0, m3 = 0;
        float x;
        int tpf = tmax - t;

        for (int tp = 0; tp < tpf; tp++) {
            //     // std::cout << inputVector[tp] << std::endl;
            //     std::cout << m1 << std::endl;
            //     m1 += inputVector[tp];
            // }

            // float m1 = alpha * inputVector[]
            for (int tp = 0; tp < tpf; tp++) {m1 += inputVector[tp];}
            for (int tp = 0; tp < tpf; tp++) {m2 += inputVector[tp + t];}
            for (int tp = 0; tp < tpf; tp++) {m3 += inputVector[tp + t] * inputVector[tp];}

            x = (alpha * m3) - (alpha * m1 * alpha * m2);
            // // std::cout << alpha << "\t" << m1 << "\t" << m2 << "\t" << m3 << std::endl;
            // // std::cout << x << std::endl;

            acvector.push_back(x);
        }

        // if (x > 0) {acvector.push_back(x);}
        // else break;
    }

    // for (int i = 0; i < tmax; i++) {
    //     std::cout << inputVector[i] << std::endl;
    // }


    // std::cout << "T max = " << tmax << std::endl;
    // std::cout << "Test = " << inputVector[tmax - 1] << std::endl;

    return acvector;

}



int main() {
    csvFile mM;
    std::string filename;
    int return_file_code;

    filename = "D:\\Coding\\Cpp\\IsingModel\\DataFiles\\3_5K\\mME_Temp_3.500000_Size_20x20_seed_123456789_sweeps_10000000_part_1.csv";
    // filename = "D:\\Coding\\Cpp\\IsingModel\\DataFiles\\Test.csv";

    // return_file_code = CheckFile(filename);

    // std::cout << filestatus << std::endl;

    // return_file_code = ReadFile(filename);

    // if (return_file_code == 0) {std::cout << "File Read Successfully" << std::endl;}
    // else {std::cout << "Error reading file" << std::endl; exit(0);}

    mM = parsecsv2(filename);

    std::cout << "Size of mM = " << mM.m.size();

    for (int i = 0; i < 10; i++) {
        std::cout << "Index = " << i << "\tm = " << mM.m.at(i) << std::endl;
    }

    // std::vector<float> submM = slice(mM.m, 0, 100);

    // std::cout << submM.size();

    // std::vector<float> autoCorr = autocorrelation(mM.m);

    // std::cout << "Length of autocorrelation vector " << autoCorr.size() << std::endl;

    // for (int i = 0; i < autoCorr.size(); i++) {
    //     std::cout << autoCorr.at(i) << std::endl; 
    // }

}