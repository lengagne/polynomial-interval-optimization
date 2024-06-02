#include "BissectionIntervalSolver.h"
#include <string.h>

BissectionIntervalSolver::BissectionIntervalSolver(AbstractCSP* pb,
                                                    uint bissection_mode)
{
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
    bissection_type_ = bissection_mode;
}

BissectionIntervalSolver::~BissectionIntervalSolver()
{
    results.clear();
}

void BissectionIntervalSolver::evaluate(const std::vector<Interval> &in,
                                        std::vector<Interval> &out)
{
    Result tmp;
    tmp.in = in;
    out.resize(nb_fun_);
    pb_->function(tmp.in,out);
}

param_optim BissectionIntervalSolver::solve_optim(double eps)
{
    precision_ = eps;
    
    std::cout<<"BissectionIntervalSolver::solve_optim"<<std::endl;
    save_filename_ = pb_->get_problem_name() + "_Precision" + std::to_string(precision_)+ "_BisMod" + std::to_string(bissection_type_)+ "_Interval";

    start_preparation_time_ = get_cpu_time();
    
    switch(bissection_type_)
    {
        case(0):    std::cout<<"Bissection : MinFirst"<<std::endl;  break;
        case(1):    std::cout<<"Bissection : MaxFirst"<<std::endl;  break;
        default :   std::cerr<<"Bissection type not defined "<<std::endl;   std::exit(63);  break;
    }    
    
    
    current_vector_.clear();
    bounds_ = pb_->get_bound();
    bounds_input_ = pb_->get_input();

    Result tmp(pb_->get_input(), nb_fun_, pb_->get_criteria());

    optim_crit_ = std::numeric_limits<double>::max();
    find_one_feasible_ =false;        
    cpt_iter_ = 0;
    if (warm_start_)
    {        
        if (! load_warm_start_filename(warm_start_filename_,tmp))
        {
            current_vector_.push_back(tmp);  
        }
    }
    else
    {
        current_vector_.push_back(tmp);            
        optim_ = tmp;
    }       

    std::cout<<"warm_start_filename = "<< warm_start_filename_<<std::endl;    
    std::cout<<"save_filename = "<< save_filename_<<".sop"<<std::endl;
    
    bool test;
    nb_valid_box_=0;
    nb_maybe_box_=0;
    
    
    start_computation_time_ = get_cpu_time();
    std::cout<<"preparation time : "<< start_computation_time_ - start_preparation_time_ <<" seconds."<<std::endl;

    
    if(current_vector_.size() == 0)
    {
        std::cout<<"We already load optim results (nothing  in the pile)"<<std::endl;
        return set_results();
    }    
    
    save_current_state(save_filename_);
//     std::cout<<"il y a "<< nb_fun_ <<" contraintes."<<std::endl;
    do{
        
        cpt_iter_++;
        if (cpt_iter_%save_each_iter_ == 0)
        {
//             std::cout<<cpt_iter_<<" crit ! "<< optim_crit_ <<std::endl;
            save_current_state(save_filename_);
            cpt_iter_ = 0;
            saved_iter_ ++;
        }                
        test = true;
        // We do not use reference because we pop_back !!
        Result  current_value = current_vector_.back();
        current_vector_.pop_back();

//         std::cout<<"current_value_ = "<< current_value <<std::endl;
//        for (int i=0;i<nb_var_;i++)
//            std::cout<<"in["<<i<<"] = "<< current_value.in[i] <<std::endl;

        pb_->function(current_value.in,current_value.out);
//        std::cout<<"crit = "<< current_value.out[nb_fun_]<<std::endl;
        if(Inf(current_value.out[nb_fun_]) < optim_crit_)
        {
            check_constraint type = INSIDE;
            for (int i=0;i<nb_fun_;i++) // if(!current_value.ctr_ok[i] )
            {
//                std::cout<<"contraintes("<<i<<") = "<< current_value.out[i] <<std::endl;
                switch(test_Interval(current_value.out[i], bounds_[i]))  
                {
                    case(OUTSIDE)   :   
//                                         std::cout<<"ctr("<<i<<") OUTSIDE "<< current_value.out[i]<<std::endl;
                                        type = OUTSIDE;
                                        break;
                    case(INSIDE)    :   
//                                         std::cout<<"ctr("<<i<<") INSIDE "<< current_value.out[i]<<std::endl;
                                        current_value.ctr_ok[i] = true;
                                        break;
                    case(OVERLAP)   :   
//                                         std::cout<<"ctr("<<i<<") OVERLAP "<< current_value.out[i]<<std::endl;
                                        type = OVERLAP;
                                        current_value.ctr_ok[i] = false;
                                        break;
                }
//                 std::cout<<"output_Interval["<<i<<"] = "<< current_value.out[i]<<std::endl;
                if(type==OUTSIDE)
                    break;
            }
            switch(type)
            {
                case(OUTSIDE)   :   break;
                case(INSIDE)    :   // evaluate the criteria
                                    if (Sup(current_value.out[nb_fun_]) < optim_crit_)
                                    {
                                        find_one_feasible_ = true;
                                        optim_crit_ =  Sup(current_value.out[nb_fun_]);
                                        optim_ = current_value;
//                                         std::cout<<"optim_crit_ = "<<current_value.out[nb_fun_]<<std::endl<<std::endl;                                        
//                                         save_current_state(warm_start_filename_);
                                    }
                case(OVERLAP)   :   
                                    Result low, high;
                                    bissect(current_value, current_vector_);/*
                                    
                                    if(bissect(current_value, low,high))
                                    {
                                        current.push_back(low);
                                        current.push_back(high);
                                    }                    */
                                    break;
            }
        }
        if(current_vector_.size() == 0) test = false;

    }while(test);


    current_time_ = get_cpu_time();
    return set_results();
}
