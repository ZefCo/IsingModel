# include <iostream>
# include <map>
# include <math.h>




// Init the arrays that hold the Boltzman values
std::map<int, double> joltzman(float T) 
{
    std::map<int, double> jolt;
    for (int i = -2; i <= 2; i++) {
        jolt[2*i] = exp((-4.0 * i) / T);  // for loop goes from -2 to +2
    }
    return jolt;
}

// Init the arrays that hold the Boltzman values
std::map<int, double> holtzman(float T, float H) 
{
    std::map<int, double> holt;
    for (int i = -1; i <=1; i++ ) {
        holt[2*i] = exp((-H * i) / T);
    }
    return holt;
}


main()
{

    std::map<int, double> jolt = joltzman(1.0);
    std::map<int, double> holt = holtzman(1.0, 1.0);    

    for( std::map<int, double>::iterator i = jolt.begin(); i != jolt.end(); i++) {
        std::cout << (*i).first << ": " << (*i).second << std::endl;
    }

    std::cout << jolt[-4] << std::endl;
    // std::cout << "Hello World";
    // return 0;
    // int L = 4;

    // for (int i = -2; i <= 2; i++) {
    //     for (int j = -2; j <= 2; j++ ) {
    //         int ip = (i + 1) % L;
    //         int im = (i - 1 + L) % L;
    //         int jp = (j + 1) % L;
    //         int jm = (j - 1 + L) % L;

    //         // The % operator takes the remainder. and since the maximum value here is L that means the most I can get is L

    //         std::cout << "I = " << i << "\tIP = " << ip << "\tIM = " << im << "\tJ = " << j << "\tJP = " << jp << "\tJM = " << jm << std::endl;

    //     }
    // }
    // for (int i = -2; i <= 2; i++) {
        // std::cout << i << std::endl;
        // int i = (i + 1) % L;
        // std::cout << j << std::endl;

    // }
}