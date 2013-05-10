/**
	PSeries.h

	In mathematics a power series is a polynomial equation of the form:

		y = Ax^0 + Bx^1 + Cx^2 + Dx^3 + ... + Nx^n

	Utilizing a power series of sufficiently high degree (the value of the
	greatest exponent) it is possible to approximate many non-polynomial 
	equations including sinusoidals and logarithmics to an arbitrary 
	degree of precision.


*/
#include <cmath>
#include <vector>
using namespace std;

class PSeries
{
    public:
        PSeries ();
        ~PSeries ();

        void addTerm (long double newCoefficient);
        long double evaluate (long double x);

        vector <long double> coefficient;

        long double difference;
        long double avgDifference;

    private:

};
