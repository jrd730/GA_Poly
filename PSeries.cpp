#include "PSeries.h"



PSeries :: PSeries ()
{
    avgDifference = 10000;
    difference = 100000;
}

PSeries :: ~PSeries ()
{

}

void PSeries :: display(){
    glColor3f ((long double) 0, (long double) 0.2,  0.99);
    glBegin (GL_LINE_STRIP);
        // instead of [-12 to 12] use graphXMin graphXMax
        for (long double x = -12; x <= 12; x += .125){
            glVertex2f (x, this->evaluate(x));
        }
    glEnd();
}

void PSeries :: display_vectorfield(PSeries* ps){
    if (ps){
        glBegin (GL_LINES);
        glColor3f ((long double) 0.3, (long double) 0.6,  0.5);
        long double dx = 0.055;
        for (long double x1 = -12; x1 <= 12; x1 += 1.5){
            long double y1 = ps->evaluate(x1);
            glVertex2f (x1, y1);

            long double x2 = x1+dx;
            long double y2 = ps->evaluate(x2);
            long double dy = y2-y1;

            long double derivative = this->evaluate(x1);
            glVertex2f (x1 + dx*25, y1 + dy*25);
            //cout << x << ", " <<  ps->evaluate(x) << " :: " << x+dx << ", " <<  ps->evaluate(x+dx) << endl;
        }
        glEnd();
    } else {
        return;
    }
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
