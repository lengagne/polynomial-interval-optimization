#include "BasisFunctionSolver.h"
#include <string.h>
#include <time.h>


void BasisFunctionSolver::compute_intermediate_for(uint num_function)
{
    {
        for(auto const& id : intermediate_needed_[num_function])
        {
            if (!intermediate_updated_[id])
            {
                Interval v = infos_intermediate_update[id]->update_from_inputs();
                Intermediate_to_update[id].update( v);
                intermediate_updated_[id] = true;
            }
        }
    }
}

void BasisFunctionSolver::get_all_intermediate_dependancies(const std::list<uint>& id_to_add,
                                                            std::list<uint> & full_list)
{
    std::list<uint> tmp;    
    for (auto const& id : id_to_add) 
    {
        if ( std::find ( full_list.begin(), full_list.end(), id) == full_list.end()) // if not id not in the list
        {
            full_list.push_back(id);
            tmp = infos_intermediate_update[id]->get_dep_intermediate();            
            get_all_intermediate_dependancies(tmp,full_list);
        }
    }
    full_list.sort();
}

void BasisFunctionSolver::init(double eps)
{
    MogsIntervalInit();
//     LazyReset();
    
    start_preparation_time_ = get_cpu_time();
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
//         std::cout<<"init input "<< i <<std::endl;
        input_Interval[i].init( bounds_input_[i], "input_"+std::to_string(i));
    }
    pb_->prepare_non_linear(input_Interval);
    pb_->function(input_Interval,output_Interval);
    nb_intermediate_ = MogsInterval::get_nb_intermediate();
    
    Result tmp(pb_->get_input(), nb_fun_+nb_intermediate_, pb_->get_criteria());
    
    optim_crit_ = std::numeric_limits<double>::max();
    find_one_feasible_ =false;        
    cpt_iter_ = 0;
    if (warm_start_)
    {   
        std::cout<<"Try to read "<< warm_start_filename_ <<std::endl;
        if (! load_warm_start_filename(warm_start_filename_,tmp))
        {
            std::cout<<"Failed to load file "<< warm_start_filename_ <<std::endl;
            current_vector_.push_back(tmp);  
            optim_ = tmp;
        }
    }
    else
    {
        std::cout<<"No warm start"<<std::endl;
        current_vector_.push_back(tmp);  
        optim_ = tmp;
    }
    
    std::cout<<"warm_start_filename = "<< warm_start_filename_<<std::endl;
    
    MogsInterval::get_intermediate_to_compute(Intermediate_to_compute);
    MogsInterval::get_intermediate_to_update(Intermediate_to_update);
    Intermediate_to_update.resize(nb_intermediate_);
    
    infos_intermediate_update.resize(nb_intermediate_);
    
    std::cout<<"nb_intermediate_ = "<< nb_intermediate_ <<std::endl;
    for (int i=0;i<nb_intermediate_;i++)
    {
        infos_intermediate_update[i] = new IntervalEstimator( bf_);        
        infos_intermediate_update[i]->prepare_coeffs(Intermediate_to_compute[i], i);
    }
    
    intermediate_updated_.resize(nb_intermediate_);
    
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
    
    intermediate_needed_.resize(nb_fun_ + solve_optim_);
    for (int i=0;i<nb_fun_;i++)
    {
        std::list<uint> tmp = infos[i]->get_dep_intermediate();
        intermediate_needed_[i].clear();
        get_all_intermediate_dependancies(tmp, intermediate_needed_[i]);
    }
    
    if(solve_optim_)
    {
        std::list<uint> tmp = info_crit_->get_dep_intermediate();
        intermediate_needed_[nb_fun_].clear();  
        get_all_intermediate_dependancies(tmp, intermediate_needed_[nb_fun_]);        
    }
       
    double before_compil = get_cpu_time() ;
    
    LazyPrepare(save_filename_,warm_start_);
    current_time_ = get_cpu_time();
    
    if (preparation_duration_ == 0.0)
        preparation_duration_ = current_time_ - start_preparation_time_;
    std::cout<<"preparation time : "<< preparation_duration_ <<" seconds."<<std::endl;
    start_computation_time_  = get_cpu_time();

}
void BasisFunctionSolver::set_next()
{    
    current_value_ = current_vector_.back();
    current_vector_.pop_back();         
    for (int i=0;i<nb_var_;i++)
    {
        input_Interval[i].update( current_value_.in[i]);        
    }   
    for (int i=0;i<nb_intermediate_;i++)
    {
        intermediate_updated_[i] = false;
    }         
}

param_optim BasisFunctionSolver::solve_optim(double eps)
{
    solve_optim_ = true;
    switch(bissection_type_)
    {
        case(0):    std::cout<<"Bissection : MinFirst"<<std::endl;  break;
        case(1):    std::cout<<"Bissection : MaxFirst"<<std::endl;  break;
//         case(2):    std::cout<<"Bissection : Smart"<<std::endl;  break;
        default :   std::cerr<<"Bissection type not defined "<<std::endl;   std::exit(63);  break;
    }   
    
    init(eps);
    
    std::cout<<"save_filename = "<< save_filename_ <<".sop"<<std::endl;
    save_current_state(save_filename_);
    std::vector<double> bissect_weight(nb_var_);
    
    if(current_vector_.size() == 0)
    {
        std::cout<<"We may already load optim results (nothing in the pile)"<<std::endl;
        return set_results();
    }
    
    do{
        cpt_iter_++;
        if (cpt_iter_%save_each_iter_ == 0)
        {
            save_current_state(save_filename_);
            cpt_iter_ = 0;
            saved_iter_ ++;            
        }
                
        set_next();        
                
        check_constraint type_optim = OVERLAP;
        Interval tmp_crit = Hull(-std::numeric_limits<double>::max(),optim_crit_);
        if(find_one_feasible_)  // if one solution found check if we can found better
        {
            compute_intermediate_for(nb_fun_);                        
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
                    compute_intermediate_for(i);
                    switch(infos[i]->update_from_inputs(current_value_, bounds_[i],i))    
                    {
                        case(OUTSIDE)   :   
                                            type = OUTSIDE;
                                            break;
                        case(INSIDE)    :   
                                            current_value_.ctr_ok[i] = true;
                                            break;
                        case(OVERLAP)   :   
                                            type = OVERLAP;
                                            current_value_.ctr_ok[i] = false;
                                            break;
                    }
                    if(type==OUTSIDE)
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
                                        compute_intermediate_for(nb_fun_);
                                        type_optim = info_crit_->update_from_inputs(current_value_,tmp_crit, nb_fun_);                                        
                                    }

                                    if (type_optim == INSIDE)
                                    {
                                        find_one_feasible_ = true;
                                        optim_crit_ =  Sup(current_value_.out[nb_fun_]);
                                        optim_ = current_value_;
                                        bissect(current_value_,current_vector_);

                                    }else  if (type_optim == OVERLAP)
                                    {
                                        bissect(current_value_,current_vector_);
                                    }
                                    break;
                case(OVERLAP)   :   
                                    bissect(current_value_,current_vector_);
                                    break;
            }
        }
    }while(current_vector_.size() ); 

    return set_results();
}
