# include <vector>
# include <iostream>
# include <random>



int random_value() 
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // use this number to create a  seed
    std::uniform_int_distribution<> distr(0, 1); // define the range

    int num = distr(gen);

    if (num == 0) {num = -1;}

    return num;

}


std::vector<std::vector<int>> matrix(int m, int n) {
    std::vector< std::vector<int> > a;

    // for (int i=0; i<10; i++) {
    //     for (int j=0; j<10; j++) {
    //         int x = a[i][j];
    //     }
    // }
    // int m = 10;
    // int n = 10;
    a.resize(m);
    for (int i = 0; i < m; i++) 
    {
        a[i].resize(n);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = random_value();
        }
    }

    return a;

}

int main() 
{
    int m = 10;
    int n = 10;

    std::vector<std::vector<int>> a = matrix(m, n);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << a[i][j] << "\t";
        }
        std::cout << "\n";
    }

}