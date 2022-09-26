#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <map>
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



std::vector<float> AutoCorrelation(std::vector<float> values) {

    std::vector<float> xOFt;
    float tmax = values.size();

    for (float t = 0; t < tmax; t++) {
        // std::cout << "First Iteration" << std::endl;
        float alpha = 1 / (tmax - t);
        float term1 = 0, term2 = 0, term3 = 0;

        for (float tp = 0; tp < tmax - t; tp++) {
            // std::cout << "Second Iteration" << std::endl;
            term1 += values.at(tp + t) * values.at(tp + t);
            term2 += values.at(tp);
            term3 += values.at(tp + t);
        }

        float xt = (alpha * term1) - ((alpha * term2) * (alpha * term3));

        if (xt < 0.1) break;

        xOFt.push_back(xt);

    }

    // std::cout << "Auto Cor is of size " << xOFt.size() << std::endl;

    // float xOFt0 = xOFt[0];
    // for (int i; i < xOFt.size(); i++) {
    //     xOFt[i] = xOFt[i] / xOFt0;
    // }

    return xOFt;
    
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



csvFile parsecsv2(fs::path inPath) {
    csvFile dataFrame;
    std::ifstream inFile;
    std::string inLine, tokens;
    int Row = 0;

    inFile.open(inPath);

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



template<typename T>
std::map<float, float> histogram(std::vector<T> v, long bins) {

    std::map<float, float> hist;
    float s = 2 / (float)bins;
    float m, key, nextkey, count;

    // for (float i = 0; i < bins; i++) {hist[-1.0 + (i * s)] = 0;}

    // std::cout << "Bin size is " << bins << std::endl;

    // std::cout << "Generated Bin vector of length " << hist.size() << std::endl;

    for (int j = 0; j < v.size(); j++) {
        m = v.at(j);

        hist[m] += 1.0;
        count += 1.0;

        // for (float i = 0; i < bins; i++) {
        //     key = -1.0 + (i * s);
        //     nextkey = key + s;
        //     // if ((key <= m) && (m < (key + s))) {hist[key] += 1.0; count += 1; break;}
        //     if ((key <= m) && (m < nextkey)) {
        //         hist[key] += 1.0; 
        //         count += 1; 
        //         break;
        //     }

        // }
    }

    for (std::map<float, float>::iterator u = hist.begin(); u != hist.end(); u++) {
        (*u).second = (*u).second / count;
    }

    return hist;

}



void write_csv(fs::path master_filename, std::map<float, float> v) {

    std::ofstream fileout(master_filename);

    fileout << "Bin,Count\n";
    for (std::map<float, float>::iterator u = v.begin(); u != v.end(); u++) {
        fileout << (*u).first << "," << (*u).second << "\n";
    }

    std::cout << "Output File to " << master_filename << std::endl;

}




int main() {
    std::string filename;
    int return_file_code;
    fs::path DataFiles, OutFile;
    std::vector<float> tau;
    long bins, range;
    std::vector<int> bin_counts;
    std::map<float, float> hist;

    std::cout << "Input range of bins, L**2, where L is the size of the lattice: ";
    std::cin >> bins;
    fflush(stdin);


    // DataFiles = "D:\\Coding\\Cpp\\IsingModel\\OtherFiles\\DataFiles\\HistogramTest";
    std::cout << "Input path to file folder: ";
    std::cin >> DataFiles;
    fflush(stdin);

    std::cout << "Input output path: ";
    std::cin >> OutFile;
    fflush(stdin);

    csvFile mM;
    std::cout << "Importing files from " << DataFiles << std::endl;
    // std::cout << subDataFiles << std::endl;
    
    for (fs::path mMdata : fs::directory_iterator{DataFiles}) {
        csvFile localmM;

        // std::cout << mMdata << std::endl;
        localmM = parsecsv2(mMdata);
        // std::cout << localmM.m.size() << std::endl;

        // std::cout << "Putting into dataframe" << std::endl;

        // std::merge(mM.m.begin(), mM.m.end(), localmM.m.begin(), localmM.m.end(), back_inserter(mM.m));
        mM.m.insert(mM.m.end(), localmM.m.begin(), localmM.m.end());

    }

    std::cout << "Finished Import, mM.m is of size " << mM.m.size() << std::endl;

    hist = histogram(mM.m, bins);

    write_csv(OutFile, hist);

}