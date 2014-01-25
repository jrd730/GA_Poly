#include "PSeries.h"



PSeries :: PSeries ()
{
    avgDifference = 10000;
    difference = 100000;
}

PSeries :: ~PSeries ()
{

}

ostream & operator<<(ostream &os, const PSeries *obj)
{ 
    if(!obj){
        return os;
    }

    int num_coefficients = obj->coefficient.size();
    if (!num_coefficients)
    {
       return os;
    }
    os << obj->coefficient[0];
    for(int i=1; i < num_coefficients; i++){
        os << ", " << obj->coefficient[i];
    }

    return os;
}

ostream & operator<<(ostream &os, const PSeries &obj)
{
    int num_coefficients = obj.coefficient.size();
    if (!num_coefficients)
    {
       return os;
    }
    os << obj.coefficient[0];
    for(int i=1; i < num_coefficients; i++){
        os << ", " << obj.coefficient[i];
    }
    
    return os;
}



PSeries*  PSeries :: derivative ()
{
    PSeries* ps = new PSeries;
    int num_coefficients = this->coefficient.size();
    
    // skip the first one since its a constant...
    for(int i=1; i < num_coefficients; i++){
        ps->addTerm(this->coefficient[i] * i);
    }
    return ps;
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
