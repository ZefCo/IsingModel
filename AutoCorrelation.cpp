#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;


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

    // if this can't find the file it will quit the script.
    // Should update this so that it causes the function to skip
    int nothing = CheckFile(testfile);

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






int main() {
    csvFile mM;
    std::string filename;
    int return_file_code;

    filename = "D:\\Coding\\Cpp\\IsingModel\\DataFiles\\Test.csv";

    // return_file_code = CheckFile(filename);

    // std::cout << filestatus << std::endl;

    // return_file_code = ReadFile(filename);

    // if (return_file_code == 0) {std::cout << "File Read Successfully" << std::endl;}
    // else {std::cout << "Error reading file" << std::endl; exit(0);}

    mM = parsecsv(filename, mM);

    // for (int i = 0; i < mM.index.size(); i++) {
    //     std::cout << "Index = " << mM.index.at(i) << "\tM = " << mM.M.at(i) << "\tm = " << mM.m.at(i) << std::endl; 
    // }

}