#include "ChooseSolver.h"

#include "BissectionBasisFunctionSolver.h"
#include "BissectionIntervalSolver.h"
#include "ContractionIntervalSolver.h"
#include "ContractionBasisFunctionSolver.h"


ChooseSolver::ChooseSolver()
{
    uint nb = choice_.get_nb_basis_type();
    uint cpt = 0;
    solver_name_[cpt++] = "BissectionInterval";
    std::string name = "BissectionBasis";    
    for (int i=0;i<nb;i++)
    {
        std::string n = name+ std::string("_")+choice_.get_basis_type(i);
        solver_name_[cpt++] = n;
        nb_solver_++;
    }
    name = "ContractionBasis";    
    for (int i=0;i<nb;i++)
    {
        std::string n = name+ std::string("_")+choice_.get_basis_type(i);
        solver_name_[cpt++] = n;
        nb_solver_++;
    }
    solver_name_[cpt++] = "ContractionInterval";
    nb_solver_ = solver_name_.size();
}

void ChooseSolver::choose( AbstractCSP* pb,
                            AbstractSolver** solver, 
                            uint index,
                            uint bissection_mode
                         )
{
    if (index == 0)
    {
        *solver = new BissectionIntervalSolver(pb,bissection_mode);
        return;
    }

    
    uint nb = choice_.get_nb_basis_type();
    AbstractBasisFunction * bf;    
    
    if (index >=1 && index < 1+nb)
    {
        uint bf_index = index - 1;
        choice_.choose(&bf,bf_index);
        * solver = new BissectionBasisFunctionSolver(pb,bf,bissection_mode);
        return;
    }

    if (index >=1+nb && index < 1+2*nb)
    {
        uint bf_index = index - 1-nb;
        choice_.choose(&bf,bf_index);
        *solver = new ContractionBasisFunctionSolver(pb,bf,bissection_mode);
        return;
    }    
    
    if (index == 2*nb+1)
    {
        *solver = new ContractionIntervalSolver(pb,bissection_mode);
        return;
    }        
    std::cerr<<"Error in FILE "<< __FILE__<<" at line "<< __LINE__<<" the index "<< index <<" is not defined yet"<<std::endl;
    exit(123);
}

