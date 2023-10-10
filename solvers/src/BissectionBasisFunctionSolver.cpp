#include "BissectionBasisFunctionSolver.h"
#include <string.h>
#include <time.h>


BissectionBasisFunctionSolver::BissectionBasisFunctionSolver(AbstractCSP* pb, 
                                                             AbstractBasisFunction*bf,
                                                            uint bissection_mode
                                                            )
{
    LazyReset();
    bf_ = bf;
    std::cout<<"GIT BRANCH = "<<  GIT_BRANCH <<std::endl;
    int dummy = system("date");
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
    
    bissection_type_ = bissection_mode;
}

BissectionBasisFunctionSolver::~BissectionBasisFunctionSolver()
{
    close_files();
    results.clear();
}


void BissectionBasisFunctionSolver::init(double eps)
{
    if (init_done)  return;
    BasisFunctionSolver::init(eps);

    infos.resize(nb_fun_);
    for (int i=0;i<nb_fun_;i++)
    {
        infos[i] = new IntervalEstimator(bf_);
    }

    if(solve_optim_)
    {
        info_crit_ = new IntervalEstimator( bf_);
    }
    
    BasisFunctionSolver::init_end();
}


void BissectionBasisFunctionSolver::update_input()
{

    
}
