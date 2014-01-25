/******************************************************************************************
    
    // genetic algorithms curve fit demo
    // @author Jason Dietrich
    // email: jrd730@gmail.com

    // dependencies: OpenGL, GLUT

    // command line: ./graph <input_file>

    // input file format: <target_x-coordinate>  <target_y-coordinate>\n

    // description: generates and displays a set of power series that attempt to pass through
    // every (x,y) coordinate specified in the input file. Power series are evolved over many
    // generations using a genetic algorithm. Click anywhere inside the window to add new 
    // target points for the power series to try to pass through. The number of points that
    // can be accurately included in a power series depends on the complexity of the figure 
    // and the maximum degree specified in the constants file.

    // key commands (with OpenGL window highlighted):
    //     't' - steps simulation forward by one generation
    //     'g' - continuously steps through generations until target fitness is reached 
    //            or maximum generations are reached, press again to pause simulation
    //     'c' - clears all target points
    //     'r' - re-randomizes the population of power series
    //     '-' - reduces the number of curves displayed
    //     '+' - increases the number of curves displayed

*********************************************************************************************/
 
#include "PSeries.h"
#include "Vertex.h"
#include "GA_CONSTANTS.h"


using namespace std;

static int height = 600;
static int width = 800;
static int display_modemask = 0X0000;

static float graphXMin = -12;
static float graphXMax = 12;
float graphXRange = graphXMax - graphXMin;
float pixToXCoord = graphXRange/width;

static float graphYMin = -9;
static float graphYMax = 9;
float graphYRange = graphYMax - graphYMin;
float pixToYCoord = graphYRange/height;

vector <PSeries> ps;
PSeries* derivative; //just take dervative of best..
vector <vertex> targetPoint;

bool going (false);
bool DERIVATIVE_KEY_MODE(false);
int generation = 0;
int showBest = 1;

long double randomFloat ();
void initialize ();
void score ();
void update ();
void displayBest ();

bool operator < (PSeries first, PSeries second){return first.difference < second.difference;}

void  initializeViewMatrix ()
{
    pixToXCoord = graphXRange/width;
    pixToYCoord = graphYRange/height;
}

long double randomFloat (){
    long double x=((long double)rand()/(long double)RAND_MAX);
    return x;
}

PSeries randomPSeries (int maxdegree)
{
    PSeries p;
    for (int j=0; j<=maxdegree; j++){
        p.addTerm( -(randomFloat() * initial[j])/2.0 + (randomFloat() * initial[j]) );
    }

    return p;
}

void initialize ()
{
    generation = 0;
    ps.clear();
    ps.resize(MAX_POPULATION);
    for (int i=0; i<MAX_POPULATION; i++){
         ps[i] = randomPSeries(MAX_DEGREE);
    }
}

void score ()
{
    for (unsigned i=0; i<ps.size(); i++){
        long double totalDifference = 0;
        for (unsigned j=0; j<targetPoint.size(); j++){
            totalDifference += abs (ps[i].evaluate(targetPoint[j].x) - targetPoint[j].y );
        }
        ps[i].difference = totalDifference;
        ps[i].avgDifference = totalDifference/targetPoint.size();
    }
    stable_sort (ps.begin(), ps.end());
}

void update ()
{
    vector <PSeries> newGen;
    newGen.resize(MAX_POPULATION+CLONES);

    unsigned i=0;
    for (; i<CLONES; i++){
        newGen[i] = ps[i];
    }
    for (; i<newGen.size(); i++){
        PSeries parent1 = ps[ rand()%ps.size() * PARENTING_RATE ];
        PSeries parent2 = ps[ rand()%ps.size() * PARENTING_RATE ];
        PSeries child;

        int crossover = rand()%(MAX_DEGREE+1);
        for (int j=0; j<crossover; j++){
            child.addTerm(parent1.coefficient[j]);
        }
        for (int j=crossover; j<=MAX_DEGREE; j++){
            child.addTerm(parent2.coefficient[j]);
        }

        if (randomFloat() <= MUTATION_RATE){
            int mutatedGene = rand()%(MAX_DEGREE+1);
            if (rand()%2 == 1)
                child.coefficient[mutatedGene] += (randomFloat () * modifier[mutatedGene]);
            else
                child.coefficient[mutatedGene] -= (randomFloat () * modifier[mutatedGene]);
        }
        newGen[i] = child;
    }
    ps = newGen;
    generation++;
    score();

    delete derivative;
    derivative = NULL;
    derivative = ps[0].derivative(); 
    //cout << "Derivative: " << derivative << endl;
}

void displayBest ()
{
    cout << "Best candidate from generation " << generation << "\n";
    cout << "Y =";
    for (unsigned j=0; j<ps[0].coefficient.size(); j++){
        if (ps[0].coefficient[j] != 0){
            cout << " ("  << ps[0].coefficient[j] << " * X^" << j << ") ";
            if (j < ps[0].coefficient.size()-1)
                cout << "+";
        }
    }
    cout << "\n";
    cout << "\ttotal difference  : " << ps[0].difference << "\n";
    cout << "\taverage difference: " << ps[0].avgDifference << "\n";
    cout << "\tderivative: " << derivative << "\n\n";
}

static void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // x/y axis
    glColor3f (1, 1, 1);
    glBegin (GL_LINES);
        glVertex2f (0, graphYMax);
        glVertex2f (0, graphYMin);
        glVertex2f (graphXMax, 0);
        glVertex2f (graphXMin, 0);
    glEnd();

    // target points
    glColor3f (0, 0, 1);
    glPointSize (6.0);
    glBegin (GL_POINTS);
        for (unsigned i=0; i<targetPoint.size(); i++){
            glVertex2f (targetPoint[i].x, targetPoint[i].y);
        }
    glEnd();

    // best curves
    for (int i=showBest; i>=0; i--){
        glColor3f ((long double) i/(long double)showBest, (((long double)showBest - (long double) i)/(long double)showBest), 0);
        //glColor3f (0, 0, 0);
        glBegin (GL_LINE_STRIP);
            for (long double x = graphXMin; x <= graphXMax; x += .125){
                glVertex2f (x, ps[i].evaluate(x));
            }
        glEnd();
    }


    // draw the derivative of the BEST CURVE
    if(derivative != NULL){
        if (display_modemask & 1){
            derivative->display();  
        } else if(display_modemask & 2) {
            derivative->display_vectorfield(&ps[0]); // vector field of ps[0] 
        }
    }

    glFlush();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{

    if(key != 'd' && key != 'D' && DERIVATIVE_KEY_MODE){
        switch(key){
            case '1':
                // graph the derivative curve instead
                display_modemask=0x0001;
            break;
            case '2':
                // graph tangent lines
                display_modemask=0x0002;
            break;
            default:
                display_modemask=0x0000;
            break;
        }
        DERIVATIVE_KEY_MODE = false;
    }


    switch (key){
        case 't':
        case 'T':
            update();
            displayBest();
         break;

        case 'g':
        case 'G':
            going = !going;
         break;

        case 'c':
        case 'C':
            targetPoint.clear();
            generation = 0;
        break;

        case 'r':
        case 'R':
            initialize();
        break;

        case '+':
        case '=':
            showBest *= 2;
        if (showBest >= MAX_POPULATION)
            showBest = MAX_POPULATION-1;
        break;

        case 'D':
        case 'd':
            DERIVATIVE_KEY_MODE=true;
        break;
        case '-':
            showBest /= 2;
            if (showBest < 1)
                showBest = 1;
        break;

    }

    //glutPostRedisplay();
}

static void mouse (int button, int state, int x, int y)
{
    switch (button){
        case GLUT_LEFT_BUTTON:
            vertex newpoint ( x*pixToXCoord + graphXMin,
                              -y*pixToYCoord + graphYMax);
            targetPoint.push_back(newpoint);
            // cout << "mouse clicked at " << x << " " << y << endl;
            // cout << "new point at " << newpoint.x << " " << newpoint.y << endl;
        break;
    }
}

static void motion (int x, int y)
{
    vertex newpoint ( x*pixToXCoord + graphXMin,
                      -y*pixToYCoord + graphYMax);
    targetPoint.push_back(newpoint);
    // cout << "mouse clicked at " << x << " " << y << endl;
    // cout << "new point at " << newpoint.x << " " << newpoint.y << endl;
    // cout << "points: " << targetPoint.size() << endl;
}

static void idle(void)
{
    if (going){
        if (ps[0].avgDifference > TARGET_AVG_DIFFERENCE && generation < MAX_GENERATIONS){
            update();
        }
    }
    glutPostRedisplay();
    //sleep (15);
}

static void resize(int w, int h)
{
    width = w;
    height = h;
    initializeViewMatrix();
    glViewport (0,0,(GLsizei)width, (GLsizei)height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (graphXMin, graphXMax, graphYMin, graphYMax);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char *argv[])
{
    srand (time (0));
    ifstream fin;
    cout << "Loading target points...";
    fin.open(argv[1]);
    if (!fin.fail()){
        while (fin.good()){
            long double newX, newY;
            fin >> newX >> newY;
            vertex newTargetPoint (newX, newY);
            targetPoint.push_back(newTargetPoint);
        }
        cout << "complete!\n";
    }
    else{
        cout << "\nError: input file not found\n";
    }

    cout << "Initializing population...";
    initialize ();
    cout << "complete!\n\n";

    score ();
    displayBest();

    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Curve Fit");
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    glClearColor(0,0,0,0);
    glutMainLoop();
    return EXIT_SUCCESS;
}

