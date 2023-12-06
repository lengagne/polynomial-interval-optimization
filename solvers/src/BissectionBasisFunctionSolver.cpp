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

void BissectionBasisFunctionSolver::evaluate(   const std::vector<Interval> &in,
                                                std::vector<Interval> &out)
{
    solve_optim_ = true;
    init(1e-3);
    out.resize(nb_fun_);
    Result tmp;
    tmp.in = in;   
    current_vector_.push_back(tmp);
    set_next();
    
    for (int i=0;i<nb_fun_;i++)
    {
        compute_intermediate_for(i);
        out[i] = infos[i]->update_from_inputs();
    }
    if(solve_optim_)
    {
        std::cout<<"calcul de la deniere "<<std::endl;
        out[nb_fun_] = info_crit_->update_from_inputs();
    }else
    {
        std::cout<<"on ne calcule pas le dernier"<<std::endl;
    }
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
