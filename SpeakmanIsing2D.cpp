# include <iostream>
# include <vector>
# include <random>
# include <math.h>
# include <map>
# include <fstream>
# include <numeric>
# include <string>
// # include "numRec.h"

// Declaring seed to be a global variable. Trying to avoid declaring anything else as global
long seed;


/* RAN2 from Numerical Methods in C
Keeping this here in case I want to use it. It works, but it works differently then ran2 from cpp. This chunk came from Numerical Recipies in C
as given by Dr. Bassler. The ran2 used here is from Numerical Recipies in C++, which is written by the same people but as you can see uses some values differently.
**/
// definitions for ran2
# define IM1 2147483563
# define IM2 2147483399
# define AM (1.0/IM1)
# define IMM1 (IM1-1)
# define IA1 40014
# define IA2 40692
# define IQ1 53668
# define IQ2 52774
# define IR1 12211
# define IR2 3791
# define NTAB 32
# define NDIV (1+IMM1/NTAB)
# define EPS 1.2e-7
# define RNMX (1.0-EPS)

// ran2, copied from page 282 of Numerical Recipies (defines at top)
float ran2(long *idum)
{

    int j;
    long k;
    static long idum2 = 123456789;
    static long iy = 0;
    static long iv[NTAB];
    float temp;

    if (*idum <= 0) {                     // This just initilzes
        if (-(*idum) < 1) *idum = 1;  // prevents idum from being 0
        else *idum = -(*idum);

        idum2 = (*idum);

        for (j = NTAB + 7; j >=0; j--) {  // shuffles the values
            k = (*idum) / IQ1;
            *idum = IA1 * (*idum - k*IQ1) - k*IR1;  // pretty sure this is *idum - (k*IQ1), hence the lack of spacing
            if (*idum < 0) *idum += IM1; 
            if (j < NTAB) iv[j] = *idum;
        }
        iy = iv[0];
    }

    k = (*idum) / IQ1;
    *idum = IA1 * (*idum - k*IQ1) - k*IQ1;

    if (*idum < 0) *idum += IM1;

    k = idum2 / IQ2;
    idum2 = IA2 * (idum2 - k*IQ2) - k*IQ2;

    if (idum2 < 0) idum2 += IM2;

    j = iy / NDIV;
    iy = iv[j] - idum2;
    iv[j] = *idum;

    if (iy < 1) iy += IMM1;
    if ((temp = AM* iy) > RNMX) return RNMX;
    else return temp;

}

#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX



// Generate random int: by default this is from [0, 1]
// Note it actually generates a number between [0, 2) so when being used it should thought of as [min, max + 1)
int random_int(int min = 0, int max = 2)
{

    float rval = ran2(&seed);
    int i = min + ((max - min) * rval);
    // std::cout << "Rval: " << rval << " Rnt: " << i << std::endl;

    return i;
}



// Gets a random int between 0 and 2 (remember it's [0, 1) ) and then turns the 0 into -1
int random_spin() 
{
    int spin;
    spin = random_int();
    if (spin == 0) { spin = -1; }
    // std::cout << "Spin: " << spin << std::endl;

    return spin;

}



// Just pauses the script and waits for the user to press enter
void pause_for_input() {
    std::cout << "\nPress Enter to continue\n";
    // Clears the buffer of any extra bits
    fflush(stdin);
    getchar();

}



// Decides if the spin needs to be flipped. Will return -1 if it needs to be flipped or 1 if it remains the same.
// That way I can just multiply the flipped value by the current spin and not have to do any if statements on the
// return value
int flip_spin(float delta_e)
{
    int flip = 1;

    float roll = ran2(&seed);
    if (roll < delta_e) {flip = -1;}

    return flip;

}



// For finding the average of a vector
float ave_vector(std::vector<float> in_vector) {
    float ave;

    ave = std::reduce(in_vector.begin(), in_vector.end(), 0.0) / in_vector.size();

    return ave;
}



// Generates a local spin
struct LocalSpin
{
    int spin = random_spin();

    operator int();
};
LocalSpin::operator int()
{
    return spin;
}



// Class for storing the Lattice
class IsingLattice
{
// Private section, for values that shouldn't be changed (easily)
private:

    // Rows
    int R;
    // Columns
    int C;

    // Coupeling Constant
    double J;
    // External Mag Field
    double H;
    // Tempurature
    double T;

    // Boltzman Energy. jolt ties to J, holt ties to h
    // Saving these as Maps, with their key as the DE and values as the actual exp
    std::map<int, double> joltzman;
    std::map<int, double> holtzman;

    // Place spins on the lattice
    std::vector< std::vector<int> > init_lattice() {

        std::vector< std::vector<int> > lattice;

        // Generate lattice vector
        lattice.resize(R);
        for (int i = 0; i < R; i++) {
            lattice[i].resize(C);
        }

        // Place spins on lattice
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                int v = random_spin();
                lattice[i][j] = v;

            }
        }

        return lattice;
    }


    // Calculates the initial energy
    int init_energy() {
        int local_energy = 0;

        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {

                local_energy += DeltaEnergy(i, j);
            }
        }

        return local_energy;

    }


    // Calculates the initial magnitization
    int init_magnitization() {

        int local_mag = 0;

        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {

                local_mag += lattice[i][j];

            }
        }

        return local_mag;

    }


    // Initialize the arrays that hold the Boltzman values
    // joltzman for J Boltzman
    void init_joltzman() {
        for (int i = -2; i <= 2; i++) {
            joltzman[2*i] = exp((-4.0 * i) / T);  // for loop goes from -2 to +2
        }

    }

    // Initialize the arrays that hold the Boltzman values
    // holtzman for H Boltzman (not a Dune reference... or is it?)
    void init_holtzman() {
        for (int i = -1; i <=1; i++ ) {
            holtzman[i] = exp((-H * i) / T);
        }

    }


public:

    float energy; //, Energy; 
    float mag, Mag;
    std::vector<std::vector<int>> lattice;


    // Gets the boltzman energy
    float get_boltzman(int j, int h) {
        float e;
        e = joltzman[j] * holtzman[h];
        
        return e;

    }


    // In case I need the total spins
    float get_total_spins() {
        return (R * C);
    }


    // Returns the DE from a specific location on the lattice
    int DeltaEnergy(int row, int col) {
        int e;

        int rowu = (row + 1) % R; // up one
        int rowd = (row - 1 + R) % R; // down one
        int coll = (col + 1) % C; // left one
        int colr = (col - 1 + C) % C; // right one

        e = (lattice[rowu][col] + lattice[rowd][col] + lattice[row][coll] + lattice[row][colr]) * lattice[row][col]; 

        return e;
    }


    // You "flip" the spin outside the lattice, then pass it in to let the lattice figure out if it needs to be flipped
    void flip_spin(int flip, int row, int col, int e_mu) {

        if (flip == -1) {
            lattice[row][col] = lattice[row][col] * flip;
            Mag = Mag + (2 * lattice[row][col]);
            mag = Mag / get_total_spins();
            // Energy = Energy + e_mu;
            // energy = Energy / get_total_spins();
        }

    }

    
    // Just prints the Boltzman J and H Maps
    void print_boltzman() {
        
        std::cout << "\nJ Botlzman Energy:" << std::endl;
        for( std::map<int, double>::iterator i = joltzman.begin(); i != joltzman.end(); i++) {
            std::cout << "\tKey: " << (*i).first << "\tValue: " << (*i).second << std::endl;
        }

        std::cout << "H Boltzman Energy" << std::endl;
        for( std::map<int, double>::iterator i = holtzman.begin(); i != holtzman.end(); i++) {
            std::cout << "\tKey: " << (*i).first << "\tValue: " << (*i).second << std::endl;
        }
        std::cout << std::endl;

    }


    // Prints the Lattice
    void print_lattice() {
        for (int i = 0; i < R; i++){
            for (int j = 0; j < C; j++){
                if (lattice[i][j] == 1) { std::cout << "+" << "\t"; }
                else if (lattice[i][j] == -1) { std::cout << "-" << "\t"; } 
            }
            std::cout << "\n";
        }

        std::cout << "\n";
    }

    // Constructor
    IsingLattice(int rows, int columns, double temp, double coupeling, double magfield)
    {
        R = rows;
        C = columns;
        T = temp;
        J = coupeling;
        H = magfield;

        lattice = init_lattice();
        init_joltzman();
        init_holtzman();

        Mag = init_magnitization();
        mag = Mag / (get_total_spins());
        // Energy = init_energy();
        // energy = Energy / (get_total_spins());

    };

};



// writes the Mag and mag to a csv file
void write_csv(std::string filename, std::vector<float> Mags, std::vector<float> mags) { //, std::vector<float> Energy, std::vector<float> energy) {
    std::ofstream fileout(filename);

    fileout << "quarter,M,m\n";//,E,e\n";

    for (float i = 0; i < Mags.size(); i++) {
        float index = ((i + 1) / 4);
        fileout << index << "," << Mags.at(i) << "," << mags.at(i) << "\n"; //"," << Energy.at(i) << "," << energy.at(i) << "\n";
    }

    fileout.close();
}



// This part takes the "measurement"
main()
{

    const float J = 1.0; 
    float T, H, r, delta_e, h_e; 
    int D, max_sweeps, print_first_lattice, print_final_lattice;
    int flip, de_index, he_index;
    long init_seed;
    // std::vector<float> ave_mag_storage; std::vector<float> ave_Mag_storage;
    std::vector<float> mag_storage, Mag_storage; // , energy_storage, Energy_storage;

    std::cout << "Input Seed: ";
    std::cin >> seed;
    init_seed = seed;

    std::cout << "Input Dimensions of the lattice: RxC" << std::endl;
    std::cout << "Square Lattice Dimensions (M x M): ";
    std::cin >> D;
    // Total number of spins, and steps that make up one sweep
    int sweep_steps = D*D;
    
    std::cout << "Input the Tempurature: ";
    std::cin >> T;
    std::cout << "Input External Magnetic Field: ";
    std::cin >> H;

    std::cout << "Input the Maximum Number of SWEEPS\nSweep for this lattice would be " << D*D  << " Steps\nSweeps: ";
    std::cin >> max_sweeps;


    std::cout << "\nInputs:" << std::endl;
    std::cout << "Seed: " << seed << std::endl;
    std::cout << "Dimensions of Lattice: " << D << " x " << D << std::endl;
    std::cout << "Tempurature: " << T << "\tB Field: " << H << "\tJ = k (1, dimensionless)" << std::endl;
    std::cout << "Sweeps: " << max_sweeps << "\tTotal Steps: " << max_sweeps * sweep_steps << std::endl;
    std::cout << "Measurement will be taken every QUARTER sweep" << std::endl;

    // pause_for_input();

    IsingLattice ising_lattice(D, D, T, J, H);


    std::cout << "Total Spins: " << ising_lattice.get_total_spins() << std::endl;
    std::cout << "Initial Mag: " << ising_lattice.Mag << "\tInitial Mag per Spin: " << ising_lattice.mag << std::endl;
    // std::cout << "Initial Energy: " << ising_lattice.Energy << std::endl;
    // ising_lattice.print_boltzman();

    std::cout << "\nDo you wish to see the inital lattice? 1 for yes 0 for no: ";
    std::cin >> print_first_lattice;

    if (print_first_lattice == 1) {
            std::cout << "Initial Lattice" << std::endl;
            ising_lattice.print_lattice();
    }

    // exit(0);

    // std::cout << "\nDo you want to see the Mag and mag output at the end?\nA high Max Steps may clutter the console\n1 for yes, 0 for no: ";
    // std::cin >> print_mags_to_console;
    std::cout << "\nDo you wish to see the final Lattice at the end?\n1 for yes, 0 for no: ";
    std::cin >> print_final_lattice;

    std::cout << "Ready to begin simulation" << std::endl;
    pause_for_input();

    // starting counter from 1 so I can take the modulous and output how many sweeps the simulation has completed, that way I know
    // it's still working.
    for (int s = 1; s < (max_sweeps + 1); s++) {
        std::vector<float> swe_mag_storage; std::vector<float> swe_Mag_storage;
        // Taking the measurement every 1/4 sweep. Doing a for loop in a for loop makes the most sense
        for (int q = 1; q <=4; q++) {
            for (int j = 0; j < (sweep_steps / 4); j++){
                r = random_int(0, D); int c = random_int(0, D);

                de_index = ising_lattice.DeltaEnergy(r, c);  // These two actually do *not* return the energy, but instead return the key to the values of their energy
                he_index = ising_lattice.lattice[r][c];  // You have to use these indicies to get the proper energy

                delta_e = ising_lattice.get_boltzman(de_index, he_index);

                int flip = flip_spin(delta_e);

                ising_lattice.flip_spin(flip, r, c, de_index + he_index);

                // std::cout << "Energy = " << ising_lattice.Energy << " e = " << ising_lattice.energy << std::endl;
                // exit(0);

                // // These commented out sections are for when I want to see it step by step. Useful for when this started, but probably not
                // // going to come back into effect anytime soon.
                // if (flip == -1) {
                //     ising_lattice.lattice[r][c] = ising_lattice.lattice[r][c] * flip;
                //     ising_lattice.Mag = ising_lattice.Mag + (2 * ising_lattice.lattice[r][c]);
                //     ising_lattice.mag = ising_lattice.Mag / ising_lattice.get_total_spins();
                //     // std::cout << "--- Flipping Spin ---" << std::endl;
                // }

                // the changes to Mag and mag were already taken care of in ising_lattice.flip_spin(), so I can just pull the values now
                // swe_mag_storage.push_back(ising_lattice.mag); // store each mag for averaging later
                // swe_Mag_storage.push_back(ising_lattice.Mag); // store each Mag for averaging later

                // std::cout << "S: " << s <<" R = " << r << " C = " << c << std::endl;
                // std::cout << "J-E (Index) = " << de_index << "\tH-E (Index) = " << he_index << "\tDelta-E = " << delta_e <<  std::endl;
                // // std::cout << "Flip is " << flip << std::endl;
                // std::cout << "Mag = " << ising_lattice.Mag << "\tMag per Spin = " << ising_lattice.mag << std::endl;

                // std::cout << "Updated Lattice: " << std::endl;
                // ising_lattice.print_lattice();

                // pause_for_input();
            }
        
            // Grabs the last mag and Mag of the sweep and pushes it into the vector. mag and Mag were found at ising_lattice.flip_spin()
            // So I can just grab the last value
            mag_storage.push_back(ising_lattice.mag);
            Mag_storage.push_back(ising_lattice.Mag);
            // energy_storage.push_back(ising_lattice.energy);
            // Energy_storage.push_back(ising_lattice.Energy);

            // // In case you want to see when the steps are completed. Useful for knowing it's still working.
            // std::cout << "Completed Sweep " << s << std::endl;
            // std::cout << "Random Seed to this point " << seed << std::endl;
        }
        // exit(0);
        // Just to make sure it's working
        if ((s % 100000) == 0) {
            std::cout << "Complted Sweep " << s << std::endl;
        }


    }

    // if (print_mags_to_console == 1) {    
    //     std::cout << "Magnitization\tMagnitization per spin" << std::endl;
    //     for (int i = 0; i < Mag_storage.size(); i++) {
    //         std::cout << Mag_storage[i] << "\t\t" << mag_storage[i] << std::endl;
    //     }
    // }

    if (print_final_lattice == 1) {
        std::cout << "Final Lattice:" << std::endl;
        ising_lattice.print_lattice();
    }

    std::string filename = "mME_Temp_" + std::to_string(T) + "_Size_" + std::to_string(D) + "x" + std::to_string(D) + "_seed_" + std::to_string(init_seed) + "_sweeps_" + std::to_string(max_sweeps) + ".csv";

    write_csv(filename, Mag_storage, mag_storage); //, Energy_storage, energy_storage);
    std::cout << "Mag, mag, E, and e were output to the file " << filename << std::endl;


}


