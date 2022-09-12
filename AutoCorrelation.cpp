#include <iostream>
#include <fstream>
#include <string.h>
#include <filesystem>


namespace fs = std::filesystem;


int ReadFile(std::string filename) {
    std::ifstream infile(filename);
    if(!infile) {
        std::cerr << "File coulnd't be read!" << std::endl;
        std::cerr << "Error Code: " << strerror(errno) << std::endl;
        
        return -1;
    }

    int count = 0;
    infile >> count;
    std::cout << "File: " << filename << std::endl;
    std::cout << "This file has " << count << " items." << std::endl;

    int num = 0;
    for (int i = 0; i < count; i++) {
        infile >> num;
        std::cout << num << std::endl;
    }

    infile.close();
    return 0;

}

// # include "my_nrutil.h"

// void correl(float data1[], float data2[], unsigned long n, float ans[]) {
    
//     void realft(float data[], unsigned long n, int isign);
//     void twofft(float dat1[], float dat2[], float fft1[], float fft2[], unsigned long n);
    
//     unsigned long no2, i;
//     float dum, *fft;

//     fft = vector(1, n<<1);
//     twofft(data1, data2, fft, ans, n);
//     no2 = n>>1;

//     for (i = 2; i <=n + 2; i += 2) {
//         ans[i - 1] = (fft[i - 1] * (dum = ans[i - 1]) + fft[i] * ans[i]) / no2;
//         ans[i] = (fft[i] * dum - fft[i - 1] * ans[i]) / no2;
//     }

//     ans[2] = ans[n + 1];
//     realft(ans, n, -1);
//     free_vector(fft, 1, n<<1);

// }


int CheckFile(std::string filename) {
    // magMag_Temp_3_0_Size_20x20_seed_1523531177.csv
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



int main() {
    std::string filename;
    int return_file_code;

    filename = "D:\\Coding\\Cpp\\IsingModel\\magMag_Temp_3_0_Size_20x20_seed_1523531177.csv";

    int filestatus = CheckFile(filename);

    // if (filestatus == 0) {
    //     std::cout << "File Does not exist"
    // }

    // return_file_code = ReadFile(filename);

    // if (return_file_code == 0) {std::cout << "File Read Successfully" << std::endl;}
    // else {std::cout << "Error reading file" << std::endl;}

}