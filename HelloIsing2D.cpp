# include <iostream>
# include <vector>
# include <random>



int random_spin() 
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // use this number to create a  seed
    std::uniform_int_distribution<> rpin(0, 1); // define the range

    int num = rpin(gen);

    if (num == 0) {num = -1;}

    return num;

}


int random_position(int p)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rosition(0, p);

    return rosition(gen);

}



struct LocalSpin
{
    int spin = random_spin();

    operator int();
};
LocalSpin::operator int()
{
    return spin;
}



class IsingLattice
{
private:

    // Rows
    int M;
    // Columns
    int N;

    // Coupeling Constant
    double J;
    // External Mag Field
    double B;

    // Place spins on the lattice
    std::vector< std::vector<int> > create_lattice() {
        std::vector< std::vector<int> > lattice;

        // int lattice[M][N];


        lattice.resize(M);
        for (int i = 0; i < M; i++) {
            lattice[i].resize(N);
        }

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                // std::cout << r;
                // lattice[i][j] = random_spin();
                // random_spin() = lattice[i][j];
                int v = random_spin();
                lattice[i][j] = v;

            }
        }

        std::cout << "M = " << M << " N = " << N << std::endl;

        return lattice;
    }

public:

    double energy;
    std::vector<std::vector<int>> lattice;

    double CalcEnergy() {
        double e = 0.0;
        double eb;
        double eiu; // nearest neighbor up
        double eid; // nearest neighbor down
        double ejl; // nearest neighbor left
        double ejr; // nearest neighbor right

        int iu; // up one
        int id; // down one
        int jl; // left one
        int jr; // right one

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {

                // std::cout << "i: " << i << " j: " << j << std::endl;

                eb = -B*lattice[i][j];

                iu = i - 1;
                if ((iu) < 0) {
                    eiu = 0.0;
                } else {
                    eiu = -J * lattice[iu][j] * lattice[i][j];
                    // std::cout << "\t" << iu << " - " << j <<  " - " << eiu << std::endl;
                }

                id = i + 1;
                if ((id) >= M) {
                    eid = 0.0;
                } else {
                    eid = -J * lattice[id][j] * lattice[i][j];
                    // std::cout << "\t" << id << " - " << j <<  " - " << eid << std::endl;
                }

                jl = j - 1;
                if ((jl) < 0) {
                    ejl = 0.0;
                } else {
                    ejl = -J * lattice[i][jl] * lattice[i][j];
                    // std::cout << "\t" << i << " - " << jl << " - " << ejl << std::endl;
                }

                jr = j + 1;
                if ((jr) >= N) {
                    ejr = 0.0;
                } else {
                    ejr = -J * lattice[i][jr] * lattice[i][j];
                    // std::cout << "\t" << i << " - " << jr << " - " << ejr << std::endl;
                }

                e = e + eb + eiu + eid + ejl + ejr;
            }
        }

        // std::cout << "E in Function " << e << std::endl;
        return e;
    }

    void print_lattice() {
        for (int i = 0; i < M; i++){
            for (int j = 0; j < N; j++){
                std::cout << lattice[i][j] << "\t";
            }
            std::cout << "\n";
        }
    }

    // Constructor
    IsingLattice(int rows, int columns, double coupeling, double magfield)
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
    const int R = 2; const int C = 2; const double B = 1.0; const double J = 1.0;

    // int random_xy[2] = {0, 0}; // Empty array for storing xy position in later
    int max_steps = 10;
    
    // std::cout << "Input Dimensions of RxC" << std::endl;
    // std::cout << "Rows: ";
    // std::cin >> R;
    // std::cout << "Columns: ";
    // std::cin >> C;

    std::cout << "Max Steps: " << max_steps << std::endl;
    std::cout << "Dimensions: " << R << " x " << C << std::endl;
    
    // std::cout << "Input External Field and Coupeling Constant" << std::endl;
    // std::cout << "External Field B (float/double - default 1.0): ";
    // std::cin >> B;
    // std::cout << "Coupling Constant J (float/double - default 1.0): ";
    // std::cin >> J;

    std::cout << "External Field and Coupling Constant - B = " << B << " J = " << J <<std::endl;

    IsingLattice ising_lattice(R, C, J, B);

    std::cout << "Initial Lattice" << std::endl;
    for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                std::cout << ising_lattice.lattice[i][j] << "\t" ;
            }
            std::cout << "\n";
        }

    ising_lattice.energy = ising_lattice.CalcEnergy();

    std::cout << "Initial Energy = " << ising_lattice.energy << std::endl;

    for (int s = 0; s < max_steps; s++) {
        int r = random_position(R - 1); int c = random_position(C - 1);
    
        // std::cout << "S: " << s <<" R = " << r << " C = " << c << std::endl;
        ising_lattice.lattice[r][c] = ising_lattice.lattice[r][c] * -1;

        // ising_lattice.print_lattice();

        std::cout << "Updated Energy = " << ising_lattice.energy << std::endl;
        }


    //     // std::cout << ising_lattice.lattice[x][y];

    // }


    // std::cout << "E = " << ising_lattice.Energy();

}


