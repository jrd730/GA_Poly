#include "PSeries.h"

PSeries :: PSeries ()
{
    avgDifference = 10000;
    difference = 100000;
}

PSeries :: ~PSeries ()
{

}

void PSeries :: addTerm (long double newCoefficient)
{
    coefficient.push_back(newCoefficient);
}

long double PSeries :: evaluate (long double x)
{
    long double y = 0;
    for (unsigned i=0; i<coefficient.size(); i++){
        y += coefficient[i] * pow(x, i);
    }
    return y;
}
