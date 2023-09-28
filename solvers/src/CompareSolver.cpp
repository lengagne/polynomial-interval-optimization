#include "CompareSolver.h"
#include <fstream>

void CompareSolver::compare(const std::string & filename,
                            double p,
                            int id)
{
    std::ofstream file;
    file.open(filename+".csv");
    double precision = p;
    file << "nb_iteration\ttemps\tcritère\tsolver"<<std::endl;
    
    if (id == -1)
    {
        std::cout<<"Start solver comparison"<<std::endl;
        uint nb = choice_solver_.get_nb_solver();
        std::cout<<"We will compare "<< nb <<" solveurs"<<std::endl;        
        for (int i=0;i<nb;i++)
    //     int i = 1;
        {
            AbstractSolver* solver;
            std::cout<<"\n\nsolver : "<< choice_solver_.get_solver_name(i)<<std::endl;
            choice_solver_.choose(pb_,&solver,i);
            std::cout<<"solver number "<<i<< std::endl;
            param_optim result;
            result = solver->solve_optim(precision);
            
            file<<result.nb_bissections<<"\t"<<result.computation_time<<"\t"<<result.optim<<"\t"<<choice_solver_.get_solver_name(i)<<std::endl;
            delete solver;
            std::cout<<"fin du solveur "<< i <<std::endl;
        }
        std::cout<<"fin de la comparaison"<<std::endl;
        file.close();
    }else
    {
        AbstractSolver* solver;
        std::cout<<"\n\nsolver : "<< choice_solver_.get_solver_name(id)<<std::endl;
        choice_solver_.choose(pb_,&solver,id);
        std::cout<<"solver number "<<id<< std::endl;
        param_optim result;
        result = solver->solve_optim(precision);
        
        file<<result.nb_bissections<<"\t"<<result.computation_time<<"\t"<<result.optim<<"\t"<<choice_solver_.get_solver_name(id)<<std::endl;
        delete solver;
        std::cout<<"fin du solveur "<< id <<std::endl;
        
    }
}
