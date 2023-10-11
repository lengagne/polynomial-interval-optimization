#include "BasisFunctionSolver.h"
#include <string.h>
#include <time.h>


void BasisFunctionSolver::init(double eps)
{
    MogsIntervalInit();
    LazyReset();
    
    ts = get_cpu_time();
    tsglobal = ts;
    precision_ = eps;
    bounds_input_ = pb_->get_input();
    bounds_ = pb_->get_bound();

    bool test;
    nb_valid_box_=0;
    nb_maybe_box_=0;    
//     ignored_space_ = 0.0;
    
    input_Interval.resize(nb_var_);
    output_Interval.resize(nb_fun_+ pb_->get_criteria());

    for (unsigned int i=0;i<nb_var_;i++)
    {
        input_Interval[i].init( bounds_input_[i], "in_"+std::to_string(i));
    }
    pb_->prepare_non_linear(input_Interval);
    pb_->function(input_Interval,output_Interval);
    
    nb_intermediate_ = MogsInterval::get_nb_intermediate();
    
    Result tmp(pb_->get_input(), nb_fun_+nb_intermediate_, pb_->get_criteria());
    
    if(solve_optim_)
        optim_ = tmp;
    current_vector_.push_back(tmp);    
    
    
    MogsInterval::get_intermediate_to_compute(Intermediate_to_compute);
    MogsInterval::get_intermediate_to_update(Intermediate_to_update);
    Intermediate_to_update.resize(nb_intermediate_);
    
    infos_intermediate_update.resize(nb_intermediate_);
    
//     std::cout<<"nb_intermediate_ = "<< nb_intermediate_ <<std::endl;
    for (int i=0;i<nb_intermediate_;i++)
    {
        infos_intermediate_update[i] = new IntervalEstimator( bf_);
        infos_intermediate_update[i]->prepare_coeffs(Intermediate_to_compute[i], i);
    }
    

}

void BasisFunctionSolver::init_end()
{
    for (int i=0;i<nb_fun_;i++)
    {
        infos[i]->prepare_coeffs(output_Interval[i], nb_intermediate_+i);
    }   
    if(solve_optim_)
    {
        info_crit_->prepare_coeffs(output_Interval[nb_fun_], nb_intermediate_+ nb_fun_);
    }
    
    double before_prepa_coeff = get_cpu_time() - ts;    
    
    double before_compil = get_cpu_time() - ts;
    LazyPrepare();

    preparation_time_ = get_cpu_time() - ts;
    std::cout<<"preparation time : "<< preparation_time_ <<" seconds."<<std::endl;
    ts  = get_cpu_time();

    init_done = true;
}
void BasisFunctionSolver::set_next()
{    
    current_value_ = current_vector_.back();
    current_vector_.pop_back();    
    for (int i=0;i<nb_var_;i++)
    {
        input_Interval[i].update( current_value_.in[i]);
    }
   
//     std::cout<<"current_value_ = "<< current_value_<<std::endl;
    
    for (int i=0;i<nb_intermediate_;i++)
    {
        Interval v = infos_intermediate_update[i]->update_from_inputs();
//         std::cout<<"intermediate("<<i<<") = "<< v <<std::endl;
        Intermediate_to_update[i].update( v);
    }         
}

param_optim BasisFunctionSolver::set_results    ()
{
    double te = get_cpu_time();
    switch(bissection_type_)
    {
        case(0):    std::cout<<"Bissection : MinFirst"<<std::endl;  break;
        case(1):    std::cout<<"Bissection : MaxFirst"<<std::endl;  break;
        case(2):    std::cout<<"Bissection : Smart"<<std::endl;  break;
        default :   std::cerr<<"Bissection type not defined "<<std::endl;   std::exit(63);  break;
    }
    
    
    std::cout<<"Number of Bissections : "<< cpt_iter_ <<std::endl;
    std::cout<<"Number of valid boxes : "<< nb_valid_box_ <<std::endl;
    std::cout<<"Number of possible boxes : "<< nb_maybe_box_<<std::endl;
//     std::cout<<"Size of ignored space  : "<< ignored_space_<<std::endl;
    std::cout<<"computation time (wo prep): "<< te - ts <<" seconds."<<std::endl;
    std::cout<<"Time per iteration : "<< (te - ts)/cpt_iter_ <<" seconds."<<std::endl;
    std::cout<<"total time : "<< te - tsglobal <<" seconds."<<std::endl;
    close_files();
    if(find_one_feasible_)
    {
        std::cout<<"crit = "<< optim_crit_ <<std::endl;
        for (int i=0;i<nb_var_;i++)
            std::cout<<"input["<<i<<"] = "<< optim_.in[i]<<std::endl;
    }else
        std::cout<<"crit =  -1 \nno feasible solution found"<<std::endl;
    
    param_optim out;
    out.nb_bissections = cpt_iter_;
    out.nb_valid_boxes = nb_valid_box_;
    out.nb_possible_boxes = nb_maybe_box_;
//     out.ignored_space_ = ignored_space_;
    out.computation_time = te - ts;
    out.computation_time_wo_prep = out.computation_time+ preparation_time_;
    out.optim = optim_crit_;    
    out.nb_intermediate = nb_intermediate_;
    out.solution_found = find_one_feasible_;
    return out;
}

param_optim BasisFunctionSolver::solve_optim(double eps)
{
    solve_optim_ = true;
    init(eps);
    
    optim_crit_ = std::numeric_limits<double>::max();
    find_one_feasible_ =false;
    cpt_iter_ = 0;
    bool test;
    
    std::vector<double> bissect_weight(nb_var_);
    uint max_iter = 1e3;
    do{
        cpt_iter_++;
        test = true;
        set_next();        
//         if (print_ ) //&& cpt_iter_ %100 == 0)
//         {
//             std::cout<<cpt_iter_<<" "<< optim_crit_ <<std::endl;
//             for (int i=0;i<nb_var_;i++)
//             {
//                 std::cout<<"\t in["<<i<<"] = "<<  current_value_.in[i]<<std::endl;
//             }
//         }
        check_constraint type_optim = OVERLAP;
        Interval tmp_crit = Hull(-std::numeric_limits<double>::max(),optim_crit_);
        if(find_one_feasible_)  // if one solution found check if we can found better
        {
//             type_optim = info_crit_->update_from_inputs(tmp_crit,tout);
            type_optim = info_crit_->update_from_inputs(current_value_,tmp_crit, nb_fun_);   
        }

        if( type_optim != OUTSIDE)
        {
            // check the constraint
            check_constraint type = INSIDE;
            for (int i=0;i<nb_fun_;i++)  
            {
                if(!current_value_.ctr_ok[i] )
                {
                    switch(infos[i]->update_from_inputs(current_value_, bounds_[i],i))    
                    {
                        case(OUTSIDE)   :   //if (print_) std::cout<<" ctr("<< i<<") =  OUTSIDE"<<std::endl;
                                            type = OUTSIDE;
                                            break;
                        case(INSIDE)    :   //if (print_) std::cout<<" ctr("<< i<<") =  INSIDE"<<std::endl;
                                            current_value_.ctr_ok[i] = true;
                                            break;
                        case(OVERLAP)   :   //if (print_) std::cout<<" ctr("<< i<<") =  OVERLAP"<<std::endl;
                                            type = OVERLAP;
                                            current_value_.ctr_ok[i] = false;
                                            break;
                    }
                    if(type==OUTSIDE)
//                     if(type==OUTSIDE || type ==OVERLAP)
                        break;
                }
            }
            update_input();               
                        
            // check the optimal
            switch(type)
            {
                case(OUTSIDE)   :   break;
                case(INSIDE)    :   
                                    if(!find_one_feasible_)
                                    {
                                        type_optim = info_crit_->update_from_inputs(current_value_,tmp_crit, nb_fun_);
                                    }
                                    if (type_optim == INSIDE)
                                    {
                                        find_one_feasible_ = true;
//                                         optim_crit_ =  Sup(tmp_crit);
                                        optim_crit_ =  Sup(current_value_.out[nb_fun_]);
                                        optim_ = current_value_;
                                        Result result1, result2;
                                        if(bissect(current_value_, result1,result2))
                                        {
                                            current_vector_.push_back(result2);
                                            current_vector_.push_back(result1);
                                        }
//                                         else
//                                         {
//                                             nb_maybe_box_++;
//                                             ignored_space_ += current_value_.get_volume();
//                                         }

                                    }else  if (type_optim == OVERLAP)//if (Inf(tmp_crit) < optim_crit_)
                                    {
                                        Result result1, result2;
                                        if(bissect(current_value_, result1,result2))
                                        {
                                            current_vector_.push_back(result2);
                                            current_vector_.push_back(result1);
                                        }
//                                         else
//                                         {
//                                             nb_maybe_box_++;
//                                             ignored_space_ += current_value_.get_volume();
//                                         }
                                    }
                                    break;
                case(OVERLAP)   :   
                                    Result result1, result2;
                                    if(bissect(current_value_, result1,result2))
                                    {
                                        current_vector_.push_back(result2);
                                        current_vector_.push_back(result1);
                                    }
//                                     else
//                                     {
//                                         nb_maybe_box_++;
//                                         ignored_space_ += current_value_.get_volume();
//                                     }
                                    break;
            }
        }
        if(current_vector_.size() == 0) test = false;
    }while(test ); // && cpt_iter_ < max_iter_ );
//     }while(test && cpt_iter_ < max_iter );

    return set_results();
}
