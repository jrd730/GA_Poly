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
            glVertex2f (x, derivative->evaluate(x));
        }
    glEnd();
}

void PSeries :: display_vectorfield(PSeries* ps){
    if (ps){
        glBegin (GL_LINES);
        glColor3f ((long double) 0.3, (long double) 0.6,  0.4);
        for (long double x = -12; x <= 12; x += .125){
            glVertex2f (x, ps->evaluate(x));
            float dx = 0.001;
            float dy = ps->evaluate(x+dx);
            glVertex2f (x+dx, ps->evaluate(x)+dy);
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
