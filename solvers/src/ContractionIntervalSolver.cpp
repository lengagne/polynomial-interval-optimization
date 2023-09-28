#include "ContractionIntervalSolver.h"
#include <string.h>

ContractionIntervalSolver::ContractionIntervalSolver(AbstractCSP* pb)
{
    std::cout<<"GIT BRANCH = "<<  GIT_BRANCH <<std::endl;
    int dummy = system("date");
    pb_ = pb;
    pb_->init();
    nb_fun_ = pb_->get_nb_out();
    nb_var_ = pb_->get_nb_in();
    std::cout<<"Creating ContractionIntervalSolver for problem : "<< pb_->get_problem_name()<<std::endl;
}

ContractionIntervalSolver::~ContractionIntervalSolver()
{
    close_files();
    results.clear();
}

param_optim ContractionIntervalSolver::solve_optim(double eps)
{
   std::cout<<"ContractionIntervalSolver::solve_optim"<<std::endl;
    double ts = get_cpu_time();
    double tsglobal = ts;
    precision_ = eps;
    bounds_input_ = pb_->get_input();
    bounds_ = pb_->get_bound();

    std::vector<Result> current;
    bounds_ = pb_->get_bound();
    bounds_input_ = pb_->get_input();

    Result tmp(pb_->get_input(), nb_fun_, pb_->get_criteria());
    current.push_back(tmp);

    bool test;
    nb_valid_box_=0;
    nb_maybe_box_=0;

    input_Interval.resize(nb_var_);
    output_Interval.resize(nb_fun_+1);
    for (unsigned int i=0;i<nb_var_;i++)
        input_Interval[i].init( bounds_input_[i], "input_"+std::to_string(i));

    pb_->function(input_Interval,output_Interval);
    for (unsigned int i=0;i<nb_fun_;i++)
        output_Interval[i].set_name("output"+std::to_string(i));

    Interval tmp_crit;
    double optim_crit = std::numeric_limits<double>::max();
    bool find_one_feasible =false;
    Result optim = tmp;

    std::cout<<"preparation time : "<< get_cpu_time() - ts <<" seconds."<<std::endl;
    cpt_iter_ = 0;
    do{
        cpt_iter_++;
        test = true;
        // We do not use reference because we pop_back !!
        Result  current_value = current.back();
        current.pop_back();

        for (int i=0;i<nb_var_;i++)
        {
            input_Interval[i].update( current_value.in[i]);
//            std::cout<<"input("<<i<<") = "<< current_value.in[i]<<std::endl;
        }


        std::vector<Interval> previous_old;
        if(cpt_iter_ > 1)
        {
            for (int i=0;i<nb_fun_;i++)
                previous_old.push_back(current_value.out[i]);
        }else
        {
            for (int i=0;i<nb_fun_;i++)
                previous_old.push_back(bounds_[i]);
        }

        for (int i=0;i<nb_fun_;i++)
        {
            output_Interval[i].update(previous_old[i]);
//            std::cout<<"output("<<i<<") = "<< previous_old[i]<<std::endl;
        }

        output_Interval[nb_fun_].update_from_inputs();
        tmp_crit = output_Interval[nb_fun_].value() ;
        if(Inf(tmp_crit) < optim_crit)
        {

            check_constraint type = INSIDE;
            for (int i=0;i<nb_fun_;i++) if(!current_value.ctr_ok[i] )
            {
                output_Interval[i].update_from_inputs();
                switch(output_Interval[i].contract(Hull ( MAX(Inf(bounds_[i]),Inf(previous_old[i])),MIN(Sup(bounds_[i]),Sup(previous_old[i]))),bounds_[i]))
                {
                    case(OUTSIDE)   :   type = OUTSIDE;
                                        // std::cout<<"function "<< i<< " is OUTSIDE "<<std::endl;
                                        break;
                    case(INSIDE)    :   current_value.ctr_ok[i] = true;
                                        // std::cout<<"function "<< i<< " is INSIDE "<<std::endl;
                                        break;
                    case(OVERLAP)   :   // std::cout<<"function "<< i<< " is OVERLAP "<<std::endl;
                                        type = OVERLAP;
                                        current_value.ctr_ok[i] = false;
                                        break;
                }
                if(type==OUTSIDE)
                    break;
                                    // update the input from the reference
                for (int j=0;j<nb_var_;j++)
                {
//                    input_Interval[j].update_vector_from_ref(CTvector);
//                    std::cout<<"After input("<<j<<") = "<< input_Interval[j]<<std::endl;
                    current_value.in[j] = input_Interval[j].value();
                }
//                output_Interval_value[i].update_vector_from_ref(CTvector);
//                std::cout<<"After output_Interval_value("<<i<<") = "<< output_Interval[i]<<std::endl;
                current_value.out[i] = output_Interval[i].value();
            }

            switch(type)
            {
                case(OUTSIDE)   :   //std::cout<<"OUTSIDE"<<std::endl<<std::endl;
                                    break;
                case(INSIDE)    :   //std::cout<<"INSIDE"<<std::endl<<std::endl;
                                    output_Interval[nb_fun_].update_from_inputs();
                                    tmp_crit = output_Interval[nb_fun_].value() ;
                                    if (Sup(tmp_crit) < optim_crit)
                                    {
                                        find_one_feasible = true;
                                        optim_crit =  Sup(tmp_crit);
                                        Result low, high;
                                        if(bissect(current_value, low,high))
                                        {
                                            current.push_back(low);
                                            current.push_back(high);
                                        }                         
                                        else
                                            optim = current_value;

                                    }else if (Inf(tmp_crit) < optim_crit)
                                    {
                                        Result low, high;
                                        if(bissect(current_value, low,high))
                                        {
                                            current.push_back(low);
                                            current.push_back(high);
                                        }                                                  }
                                    break;
                case(OVERLAP)   :   //std::cout<<"OVERLAP"<<std::endl<<std::endl;
                                    Result low, high;
                                    if(bissect(current_value, low,high))
                                    {
                                        current.push_back(low);
                                        current.push_back(high);
                                    }              
                                    break;
            }
        }
        if(current.size() == 0) test = false;
    }while(test);
    double te = get_cpu_time();
    std::cout<<"Number of Bissections : "<< cpt_iter_ <<std::endl;
    std::cout<<"Number of valid boxes : "<< nb_valid_box_ <<std::endl;
    std::cout<<"Number of possible boxes : "<< nb_maybe_box_<<std::endl;
    std::cout<<"computation time (wo prep): "<< te - ts <<" seconds."<<std::endl;
    std::cout<<"Time per iteration : "<< (te - ts)/cpt_iter_ <<" seconds."<<std::endl;
    std::cout<<"total time : "<< te - tsglobal <<" seconds."<<std::endl<<std::endl;
    close_files();

    if(find_one_feasible)
    {
        std::cout<<"crit = "<< optim_crit <<std::endl;
        for (int i=0;i<nb_var_;i++)
            std::cout<<"input["<<i<<"] = "<< optim.in[i]<<std::endl;
    }else
        std::cout<<"crit =  -1 \nno feasible solution found"<<std::endl;
    
    param_optim out;
    out.nb_bissections = cpt_iter_;
    out.nb_valid_boxes = nb_valid_box_;
    out.nb_possible_boxes = nb_maybe_box_;
    out.computation_time = te - ts;
    out.computation_time_wo_prep = out.computation_time+ preparation_time_;
    out.optim = optim_crit;    
    out.nb_intermediate = 0;
    out.solution_found = find_one_feasible;
    return out;    
}