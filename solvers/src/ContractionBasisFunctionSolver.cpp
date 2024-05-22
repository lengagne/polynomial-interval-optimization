#include "ContractionBasisFunctionSolver.h"
#include <string.h>

ContractionBasisFunctionSolver::ContractionBasisFunctionSolver(AbstractCSP* pb,  
                                                               AbstractBasisFunction* bf,
                                                               uint bissection_mode)
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
//     std::cout<<"Creating ContractionBasisFunctionSolver for problem : "<< pb_->get_problem_name()<<std::endl;
}

ContractionBasisFunctionSolver::~ContractionBasisFunctionSolver()
{
    results.clear();
}

void ContractionBasisFunctionSolver::init(double eps)
{
    BasisFunctionSolver::init(eps);
    
    output_Interval_value.resize(nb_fun_);
    for (unsigned int i=0;i<nb_fun_;i++)
    {
        output_Interval[i].set_name("output"+std::to_string(i));
        output_Interval_value[i].init(bounds_[i],"output_value"+std::to_string(i));
        output_Interval_value[i].set_as_output();
    }

    infos.resize(nb_fun_);
    for (int i=0;i<nb_fun_;i++)
    {
        infos[i] = new IntervalEstimatorContractor(bf_,output_Interval_value[i]);
    }
    if(solve_optim_)
    {
        info_crit_ = new IntervalEstimator(bf_);
    }

    BasisFunctionSolver::init_end();
    
    std::cout<<"fin preparation "<<std::endl;
}

void ContractionBasisFunctionSolver::set_next()
{
    BasisFunctionSolver::set_next();
    for (int i=0;i<nb_intermediate_;i++)
    {
        Interval v = infos_intermediate_update[i]->update_from_inputs();
        Intermediate_to_update[i].update( v);
    }
    
    if(cpt_iter_ > 1)
        for (int i=0;i<nb_fun_;i++)
            output_Interval_value[i].update(current_value_.out[i]);    
}

void ContractionBasisFunctionSolver::update( Result & current_value_)
{
    for (int i=0;i<nb_var_;i++)
    {
        input_Interval[i].update( current_value_.in[i]);
    }
    if(cpt_iter_ > 1)
        for (int i=0;i<nb_fun_;i++)
            output_Interval_value[i].update(current_value_.out[i]);

    for (int i=0;i<nb_intermediate_;i++)
    {
        Interval v = infos_intermediate_update[i]->update_from_inputs();
        Intermediate_to_update[i].update( v);
    }    
}

void ContractionBasisFunctionSolver::update_input()
{
    // set the input value after contraction
    for (int i=0;i<nb_var_;i++)
        current_value_.in[i] = input_Interval[i].value_;    
}
