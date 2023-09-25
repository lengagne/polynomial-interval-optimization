#include <iostream>
#include <vector>

#include "Solver.h"
#include "ProblemBasisFunctionComputation.h"

int main( int argc, char** argv)
{
    if (argc == 2 )
    {
        std::string com = argv[1];
        if (com == "help")
        {
            std::cout<<" You must run : \n ./OptimBasisFunctionInterval solveur problem order precision basisfuntion"<<std::endl;
            return 0;
        }
    }
    
    unsigned int solveur = 2;
    unsigned int problem = 1;
	unsigned int order = 2;
	double precision = 0.001;
    uint basisfunction = 2;
	if(argc > 1)
        solveur = atoi(argv[1]);
	if(argc > 2)
        problem= atoi(argv[2]);
	if(argc > 3)
        order = atoi(argv[3]);
	if(argc > 4)
        precision = atof(argv[4]);
	if(argc > 5)
        basisfunction = atof(argv[5]);      

    std::cout<<"order = "<< order <<std::endl;
    std::cout<<"precision = "<< precision <<std::endl;
    
    AbstractBasisFunction* bf;
    std::string basis_type = get_basis_type(basisfunction);
    choose_basis_function(&bf,basis_type);


    AbstractCSP* pb = new ProblemBasisFunctionComputation(order,"MinVo");
    AbstractSolver* solver;
    switch(solveur)
    {
        case(0) :
            std::cout<<"*********** solve_discretization *************"<<std::endl;
            solver = new DiscretizationSolver(pb);
            break;
        case(1) :
            std::cout<<"*********** solve_bissection_interval *************"<<std::endl;
            solver = new BissectionIntervalSolver(pb);
            break;
        case(2):
            solver = new BissectionBasisFunctionSolver(pb,bf);
            break;
        case(3):
            std::cout<<"*********** solve_contraction_interval*************"<<std::endl;
            solver = new ContractionIntervalSolver(pb);
            break;
        case(4):
            std::cout<<"*********** solve_contraction_interval*****"<< basis_type<<"********"<<std::endl;
            solver = new ContractionBasisFunctionSolver(pb,bf);
            break;

        default: std::cout<<"solveur not defined"<<std::endl;
    }
    solver->set_print(true);
    solver->solve_optim(precision,false);
    return 0;
}
