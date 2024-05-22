#include "CompareSolver.h"
#include <fstream>

void CompareSolver::compare(const std::string & filename,
                            double p,
                            uint bissection_mode,
                            int id,
                            const std::string& warm_start_filename)
{
    double precision = p;
    if (id == -1)
    {
        std::ofstream file;
        file.open(filename+".csv");        
        file << "nb_iteration\ttemps\tcritère\tsolver"<<std::endl;
        
        std::cout<<"Start solver comparison"<<std::endl;
        uint nb = choice_solver_.get_nb_solver();
        std::cout<<"We will compare "<< nb <<" solveurs"<<std::endl;        
        for (int i=0;i<nb;i++)
//         for (int i=7;i<9;i++)
        {
            AbstractSolver* solver;
            std::cout<<"\n\nsolver : "<< choice_solver_.get_solver_name(i)<<std::endl;
            choice_solver_.choose(pb_,&solver,i,bissection_mode);
            std::cout<<"solver number "<<i<< std::endl;
           
            param_optim result;
            solver->set_warm_start_filename(warm_start_filename);
            result = solver->solve_optim(precision);
            
            file<<result.nb_bissections<<"\t"<<result.computation_time<<"\t"<<result.optim<<"\t"<<choice_solver_.get_solver_name(i)<<std::endl;
            delete solver;
            std::cout<<"fin du solveur "<< i <<std::endl;
            sleep(1);
        }
        std::cout<<"fin de la comparaison"<<std::endl;
        file.close();
    }else
    {
        AbstractSolver* solver;
        std::cout<<"\n\nsolver : "<< choice_solver_.get_solver_name(id)<<std::endl;
        choice_solver_.choose(pb_,&solver,id,bissection_mode);
        std::cout<<"solver number "<<id<< std::endl;
        param_optim result;
        solver->set_warm_start_filename(warm_start_filename);
        result = solver->solve_optim(precision);
        
        delete solver;
        std::cout<<"fin du solveur "<< id <<std::endl;
        
    }
    LazyCleanFiles();
}
