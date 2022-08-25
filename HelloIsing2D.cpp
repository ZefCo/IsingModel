# include <iostream>
# include <vector>
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



// typedef struct LocalSpin
// {
//     int spin = 0;
//     // double energy;

//     operator int();
// };
// LocalSpin::operator int()
// {
//     return spin;
// }



class IsingLattice
{
private:
    // Place spins on the lattice
    std::vector< std::vector<int> > create_lattice() {
        std::vector< std::vector<int> > a;

        a.resize(M);
        for (int i = 0; i < M; i++) 
        {
            a[i].resize(N);
        }

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                // std::cout << r;
                a[i][j] = random_value();
            }
        }

        return a;
    }

public:
    // Rows
    int M;
    // Columns
    int N;

    // Coupeling Constant
    double J;
    // External Mag Field
    double B;

    std::vector<std::vector<int>> lattice;

    double Energy() {
        double e = 0;

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++) 
            {
                e = e + (B*lattice[i][j]);
            }
        }
        return e;
    }

    // Constructor
    IsingLattice(int rows = 10, int columns = 10, double coupeling = 1.0, double magfield = 1.0)
    {
        M = rows;
        N = columns;
        J = coupeling;
        B = magfield;
        lattice = create_lattice();
    };

};



main()
{
    int R;
    int C;
    
    std::cout << "Input Dimensions of RxC" << std::endl;
    std::cout << "Rows: ";
    std::cin >> R;
    std::cout << "Columns: ";
    std::cin >> C;
    std::cout << "Dimensions: " << R << " x " << C << std::endl;
    
    IsingLattice ising_lattice(R, C);

    std::cout << "J = " << ising_lattice.J << " B = " << ising_lattice.B << std::endl;
    std::cout << "M = " << ising_lattice.M << " N = " << ising_lattice.N << std::endl;

    for (int i = 0; i < R; i++) 
    {
        for (int j = 0; j < C; j++) 
        {
            std::cout << ising_lattice.lattice[i][j] << "\t" ;
        }
        std::cout << "\n";
    }

    std::cout << "E = " << ising_lattice.Energy();

}


