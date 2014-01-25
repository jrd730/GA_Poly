/**
	PSeries.h

	In mathematics a power series is a polynomial equation of the form:

		y = Ax^0 + Bx^1 + Cx^2 + Dx^3 + ... + Nx^n

	Utilizing a power series of sufficiently high degree (the value of the
	greatest exponent) it is possible to approximate many non-polynomial 
	equations including sinusoidals and logarithmics to an arbitrary 
	degree of precision.


*/

#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;


#ifndef PSERIES_H
#define PSERIES_H

class PSeries
{
    public:
        PSeries ();
        ~PSeries ();

        PSeries*  derivative();
        void addTerm (long double newCoefficient);
        long double evaluate (long double x);

        vector <long double> coefficient;

        long double difference;
        long double avgDifference;

    private:


};


std::ostream &operator<<(std::ostream &os, const PSeries *obj);
std::ostream &operator<<(std::ostream &os, const PSeries &obj);

#endif