




// Feel free to modify any of these values though be aware that modifying any value
// prefixed by MAX will directly affect memory consumption and processing time.

#define MAX_POPULATION 1500 // how many individuals in the population 
#define MAX_GENERATIONS 1000 // the maximum number of generations to run a simulation 
#define MAX_DEGREE 4  // how many terms to use in each power series
#define PARENTING_RATE .75 // percentage of population to use in crossover
#define MUTATION_RATE .1 // percentage of new population that is mutated
#define TARGET_AVG_DIFFERENCE .125 // necessary average distance to all points 
#define CLONES 10 // number of clones directly copied over from previous generation

// initial specifies the range for the intial values for coefficients in a power series.
// Each index maps to a unique term in a power series.
const long double initial [] = {24.0, 6.0, 2.0, .01, .0001, .00000001, .00000000001, .00000000000001, .00000000000001};

// modifier specifies a range that each coefficient can be modified by during mutation.
// The values decrease with increasing exponents to prevent the function from diverging too quickly
const long double modifier [] = {10.0, 1.0, .1, .0001, .00001, .00000001, .000000001, .0000000000001, .0000000000001};
