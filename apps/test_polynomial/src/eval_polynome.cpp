#include <iostream>
#include <vector>

#include "ProblemPoly.h"
#include "CompareSolver.h"
#include <locale.h>

std::vector<double> steps = {2,1,0.5,0.1,0.01};

int main( int argc, char** argv)
{
	// this application is required due to QXmlValidator in MOGS
//     QCoreApplication a(argc, argv);
// 
//     setlocale(LC_ALL, "C");

	double precision = 0.001;
	if(argc > 1)
        precision = atof(argv[1]);

    std::cout<<"precision = "<< precision <<std::endl;

    AbstractCSP* pb = new ProblemPoly();
    CompareSolver cp(pb);   
    cp.compare("EvalPoly_",precision);


    return 0;
}
