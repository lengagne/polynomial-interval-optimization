#include "ContractionBasisFunctionSolver.h"
#include <string.h>

ContractionBasisFunctionSolver::ContractionBasisFunctionSolver(AbstractCSP* pb,  AbstractBasisFunction* bf)
{
    LazyReset();
    bf_ = bf;
    std::cout<<"GIT BRANCH = "<<  GIT_BRANCH <<std::endl;
    int dummy = system("date");
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
//     std::cout<<"Creating ContractionBasisFunctionSolver for problem : "<< pb_->get_problem_name()<<std::endl;
}

ContractionBasisFunctionSolver::~ContractionBasisFunctionSolver()
{
    close_files();
    results.clear();
}

void ContractionBasisFunctionSolver::init(double eps)
{
    if (init_done)  return;
    BasisFunctionSolver::init(eps);

    input_Interval.resize(nb_var_);
    output_Interval.resize(nb_fun_ + pb_->get_criteria());

    
    
    // use of intermediate
    id_intermediate_to_contract_.resize(nb_fun_);
    infos_intermediate_to_contract_.resize(nb_fun_);
    inter_intermediate_to_contract_.resize(nb_fun_);

//     intermediate_Interval_value_.resize(nb_fun_);

    for (int i=0;i<nb_fun_;i++)
    {
        id_intermediate_to_contract_[i].clear();
        inter_intermediate_to_contract_[i].clear();
        output_Interval[i].get_intermediate(inter_intermediate_to_contract_[i], id_intermediate_to_contract_[i]);
        unsigned int cpt = 0;
        while (cpt < inter_intermediate_to_contract_[i].size())
        {
                std::vector<MogsInterval*> tmp;
                std::vector<unsigned int>  tmp_id;
                Intermediate_to_compute[id_intermediate_to_contract_[i][cpt]].get_intermediate(tmp,tmp_id);
                for (int k=0;k<tmp.size();k++)
                {
                    inter_intermediate_to_contract_[i].push_back(tmp[k]);
                    id_intermediate_to_contract_[i].push_back(tmp_id[k]);
                }
                cpt++;
        }
    }

    for (int i=0;i<nb_intermediate_;i++)
    {
        infos_intermediate_update[i] = new IntervalEstimator(bf_);
        infos_intermediate_update[i]->prepare_coeffs(Intermediate_to_compute[i], i);
    }

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
        infos[i] = new IntervalEstimatorContractor(bf_);
//         infos[i]->prepare_coeffs(output_Interval[i]/*,output_Interval_value[i]*/,nb_intermediate_+i);
    }
    if(solve_optim_)
    {
        info_crit_ = new IntervalEstimator(bf_);
//         info_crit_->prepare_coeffs(output_Interval[nb_fun_], nb_intermediate_+nb_fun_);
    }

    unsigned int cpt =  nb_intermediate_+nb_fun_ +1;
    for (int i=0;i<nb_fun_;i++)
    {
        unsigned int nb = inter_intermediate_to_contract_[i].size();
//         intermediate_Interval_value_[i].resize(nb);
        for (int j =0;j<nb;j++)
        {
//             intermediate_Interval_value_[i][j].set_name("intermediate_"+std::to_string(i)+"_"+std::to_string(j));
//             intermediate_Interval_value_[i][j].init(Hull(-1,1),"intermediate_"+std::to_string(i)+"_"+std::to_string(j));
//             intermediate_Interval_value_[i][j].set_as_output();
            Intermediate_to_compute[id_intermediate_to_contract_[i][j]].init(Hull(-1,1),"intermediate_to_compute"+std::to_string(i)+"_"+std::to_string(j));
            infos_intermediate_to_contract_[i].push_back(new IntervalEstimatorContractor(bf_));
            Intermediate_to_compute[id_intermediate_to_contract_[i][j]].set_as_output();
            
            infos_intermediate_to_contract_[i][j] = new IntervalEstimatorContractor(bf_);
            infos_intermediate_to_contract_[i][j]->prepare_coeffs(Intermediate_to_compute[id_intermediate_to_contract_[i][j]]/*,
                                                                 intermediate_Interval_value_[i][j]*/, cpt++);
        }
    }   

    BasisFunctionSolver::init_end();
}

void ContractionBasisFunctionSolver::set_next()
{
    BasisFunctionSolver::set_next();
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