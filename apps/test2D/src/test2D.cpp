#include <iostream>
#include <vector>

#include "CompareSolver.h"
#include "ProblemPoint2D.h"

int main( int argv, char** argc)
{

    
    std::cout<<"This executable compares the results of different solvers, using different basis functions"<<std::endl;
    std::cout<<"The problem consist in finding the intersection of circles"<<std::endl;
    
    AbstractSolver* solver;
    bool print=true;
    double precision = 0.001;
    ProblemPoint2D* pb = new ProblemPoint2D();
    
    CompareSolver cp(pb);   
    cp.compare("test2D",precision);
        
    return 0;
}
