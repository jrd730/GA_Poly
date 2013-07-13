GA_Poly
=======

A stupidly simple genetic algorithm for generating best-fit polynomials. 
Yes, I am aware that there are statistical methods for doing exactly this.
I made this more as a proof of concept to demonstrate the power and also some of the limitations of genetic algorithms.
It is also somewhat entertaining to watch the simulation unfold.

Dependencies
------------

OpenGL: all the major operating systems contain native support for OpenGL, hopefully yours does too.
GLUT (OpenGL Utility Toolkit): a useful API that handles window management and callbacks

$ sudo apt-get install freeglut3-dev




Usage
-----

$ make  
$ graph <optional input file>  

This will open up the OpenGL window showing target points from the input if specified.
The input file should contain x, y coordinate pairs. 
Click anywhere in the window to add new points.

Keys
----

- 't'  step forward one generation
- 'g'  continuously step forward, press again to halt
- 'r'  re-randomize population
- 'c'  clear target points
- '+'  display more polynomials
- '-'  dispayer fewer polynomials

