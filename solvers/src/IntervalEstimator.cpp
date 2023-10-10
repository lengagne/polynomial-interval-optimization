#include "IntervalEstimator.h"

double get_cpu_time1(){
    return (double)clock() / CLOCKS_PER_SEC;
}

check_constraint test_Interval( const Interval &in ,
                                const Interval & bound)
{
    if ( Inf(in) >= Inf(bound) && Sup(in) <= Sup(bound))
        return INSIDE;
    if ( Inf(in) > Sup(bound) ||  Sup(in) < Inf(bound))
        return OUTSIDE;
    return OVERLAP;
}

IntervalEstimator::IntervalEstimator( AbstractBasisFunction* bf, bool avoid_error)
{
    bf_ = bf;
    avoid_error_computation_ = avoid_error;
}

unsigned int IntervalEstimator::get_index_input(mem* m) const
{
    unsigned int index = 0;
    unsigned int cpt = 1;
    m->compute_recurence();
    for (int i = 0;i<dep_inputs_.size();i++)
    {
        std::map<MogsInterval*, unsigned int>::const_iterator it = m->recurence.find(dep_inputs_[i]);
        if(it != m->recurence.end())
            index += (it->second ) * coefdep_inputs_[i];
    }
    return index;
}

unsigned int IntervalEstimator::get_index(mem* m) const
{
    unsigned int index = 0;
    unsigned int cpt = 1;
    m->compute_recurence();
    for (int i = 0;i<dep_.size();i++)
    {
        std::map<MogsInterval*, unsigned int>::const_iterator it = m->recurence.find(dep_[i]);
        if(it != m->recurence.end())
            index += (it->second ) * coefdep_[i];
    }
    return index;
}

void IntervalEstimator::guess_next_bissection( uint i,
                                                Result& res,
                                                bool inf_first) const
{
//     std::cout<<"nb_in = "<< local_M_inverse_inputs_.size()<< " \t nb control point = " << nb_control_point_inputs_<<std::endl;
    if (local_M_inverse_inputs_.size() == 2 && nb_control_point_inputs_ == 4)
    {
//         std::cout<<"mode = 1 "<< std::endl;
        res.bissect_weight = cut_direction_[i];
        res.bissect_inf_sup =  inf_first;
        res.mode = 1;
        res.bissect_bool = cut_inf_[i];
        
        
//         for (int i=0;i<res.bissect_weight.size();i++)
//             std::cout<<"RES BISSECT("<<i<<") = "<< res.bissect_weight[i]<<std::endl;
    }else
    {
//         std::cout<<"mode = 0 "<< std::endl;
        res.mode = 0;
    }
}

unsigned int IntervalEstimator::prepare_coeffs( const MogsInterval& out, unsigned int num_out)
{      
    num_out_ = num_out;
    out.get_dependancies_with_order(dep_,order_);
    nb_in_ = dep_.size();
    if(nb_in_ == 0)
    {
        std::cerr<<"ERROR in "<< __FILE__<<" at line "<<__LINE__<<std::endl;
        std::cerr<<"num_out = "<< num_out <<std::endl;
        std::cerr<<"name = "<< out.name_ <<std::endl;
        std::cerr<<"For the moment try to see constant value "<<std::endl;
        exit(0);
    }
    //coefdep_
    coefdep_.push_back(1);
    for (int i=0;i<nb_in_-1;i++)
        coefdep_.push_back( coefdep_[i] * (order_[i]+1));

    local_M_.resize(nb_in_);
    local_M_inverse_.resize(nb_in_);
    for (int j=0;j<nb_in_;j++)
        local_M_[nb_in_-j-1].resize(order_[j]+1,order_[j]+1);

    
    t_max_.resize(nb_in_);
    for( int i=0;i<nb_in_;i++)
    {
        Interval val = dep_[i]->value_;
        bf_->get_basis_coeff_matrix(val, order_[i],local_M_[nb_in_-i-1],local_M_inverse_[nb_in_-i-1]);
        bf_->get_time_max( local_M_[nb_in_-i-1], t_max_[i]);
    }
    
    for (int i=0;i<nb_in_;i++)  if(! dep_[nb_in_-i-1]->rely_on_error())
        local_M_inverse_inputs_.push_back(local_M_inverse_[i]);    
    
    kron_solver_inputs_ = new Kronecker(local_M_inverse_inputs_);
    kron_solver_errors_ = new Kronecker(local_M_inverse_);

    nb_control_point_inputs_ = kron_solver_inputs_->get_nb_control_point();

    coefdep_inputs_.push_back(1);
    for (int i=0;i<nb_in_;i++)  if(! dep_[i]->rely_on_error())
    {
        dep_inputs_.push_back(dep_[i]);
        order_inputs_.push_back(order_[i]);
    }
    for (int i=0;i<dep_inputs_.size();i++)
        coefdep_inputs_.push_back( coefdep_inputs_[i] * (order_inputs_[i]+1));

    unsigned long long nb_coeff_ = 1;
    for (int i = 0;i<dep_.size();i++)
        nb_coeff_ *= order_[i]+1;

    std::list<unsigned int > coeff_inputs_, coeff_errors_;
    std::vector<unsigned int > mem_index_input(nb_control_point_inputs_);
    std::map<unsigned int, LazyVariable> MCT_coeff_;

    uint nb_valid_coeff = 0;
    nb_sparse_errors_ = 0;
    
    for( std::map<mem*,LazyVariable>::const_iterator it = out.dependances_.begin(); it != out.dependances_.end();it++)
    {        
        unsigned int index = get_index(it->first );        
        MCT_coeff_[index] = out.get_value(it->first);
        
        if(! MCT_coeff_[index].is_null())
        {
            if( it->first->contains_error())
            {
                nb_sparse_errors_++;
                coeff_errors_.push_back(index);
            }else
            {
                unsigned int index2 = get_index_input(it->first);
                coeff_inputs_.push_back(index2);
                mem_index_input[index2] = index;
            }
        }
    }

    coeff_errors_.sort();
    coeff_inputs_.sort();
    
    kron_solver_errors_->prepare_line_product_interval(coeff_errors_);
    
    sparse_coeff_errors_.resize(nb_sparse_errors_);
    
     // deal with input
    for (int i=0;i<nb_control_point_inputs_;i++)
    {
        LazyVariable out = 0;
        for(std::list<unsigned int >::const_iterator it = coeff_inputs_.begin(); it != coeff_inputs_.end(); it++)
        {
            double v = kron_solver_inputs_->get_value(i,*it);
            out += MCT_coeff_[mem_index_input[*it]]* v;
        }
        LazyAddOutput(out,num_out_,nb_valid_coeff++);
    }
    
    // deal with error
    for(std::list<unsigned int >::const_iterator it = coeff_errors_.begin(); it != coeff_errors_.end(); it++)
    {
        LazyAddOutput(MCT_coeff_[*it],num_out_,nb_valid_coeff++);
    }      
    
    
    // prepare smart bissection process
    cut_direction_.resize(nb_control_point_inputs_);
    cut_inf_.resize(nb_control_point_inputs_);
    
    for (int i=0;i<nb_control_point_inputs_;i++)
    {
        cut_direction_[i].resize(local_M_inverse_inputs_.size());
        cut_inf_[i].resize(local_M_inverse_inputs_.size());
        
    }
    
    double ratio = 1.0;
    if (local_M_inverse_inputs_.size() == 2 && nb_control_point_inputs_ == 4)
    {
        cut_direction_[0][0] = 1.0;    // ne devrait pas servir
        cut_direction_[0][1] = 1.0;
        
        cut_inf_[0][0] = true;
        cut_inf_[0][1] = true;
        
        cut_direction_[1][0] = 1;       // il vaut mieux couper la deuxième variable
        cut_direction_[1][1] = ratio;   // de préférences on coupe la deuxième variable
//         cut_direction_[1][1] = 1.0;   // de préférences on coupe la deuxième variable

        cut_inf_[1][0] = true;
        cut_inf_[1][1] = false;

        
        cut_direction_[2][0] = ratio;   // il vaut mieux couper la première variable
//         cut_direction_[2][0] = 1.0;   // il vaut mieux couper la première variable
        cut_direction_[2][1] = 1;       // 
        
        cut_inf_[2][0] = false;
        cut_inf_[2][1] = true;        
        
        cut_direction_[3][0] = 1;   // pas de préférence (donc la plus grande)
        cut_direction_[3][1] = 1;   // pas de préférence (donc la plus grande)

        cut_inf_[3][0] = false;
        cut_inf_[3][1] = false;
        
    }
    

    
    return nb_valid_coeff;
}

Interval IntervalEstimator::update_from_inputs( )
{
    unsigned int cpt = 0;  
    Interval out = LazyUpdate(num_out_,cpt++);
    for (int i=1;i<nb_control_point_inputs_;i++)
    {
        Interval value = LazyUpdate(num_out_,cpt++);
        out = Hull(out,value);
    }

    for (unsigned int i=0;i<nb_sparse_errors_;i++)
        sparse_coeff_errors_(i) = LazyUpdate(num_out_,cpt++);
    
    Interval error = kron_solver_errors_->line_product(sparse_coeff_errors_);
    return out + error;
}

// check_constraint IntervalEstimator::update_from_inputs( Interval& out, Interval& bound)
check_constraint IntervalEstimator::update_from_inputs( Result& res, Interval& bound,uint index_ctr)
{  
    Interval& out = res.out[index_ctr];
    double MAX =  std::numeric_limits<double>::max();
    double MIN = -std::numeric_limits<double>::max();

    double ss = Sup(bound);
    double ii = Inf(bound);
    unsigned int cpt = 0;
    bool inf_inside,sup_inside, both_side;
    Interval Iv = LazyUpdate(num_out_,cpt++);
    for (int i=1;i<nb_control_point_inputs_;i++)
    {
        Interval value = LazyUpdate(num_out_,cpt++);
        Iv = Hull(Iv,value);
        double iv = Inf(Iv);
        double sv = Sup(Iv);

        inf_inside = (iv >= ii && iv <= ss);
        sup_inside = (sv >= ii && sv <= ss);       
        both_side = (iv <= ii && sv >= ss);
        if ( inf_inside != sup_inside || both_side)
        {
            out = Iv;
            guess_next_bissection( i , res , inf_inside );
//             std::cout<<"on overlap avec info de bissection" <<std::endl;
//             std::cout<<"OVERLAP inside = "<< inf_inside<< "  outside = "<< sup_inside <<" both side = "<< both_side <<std::endl;
            return OVERLAP;            
        }
    }

    if ( Diam( res.error[num_out_]) >= OFFSET_ERROR_TH)
    {
        // deal with error   
        for (unsigned int i=0;i<nb_sparse_errors_;i++)
            sparse_coeff_errors_(i) = LazyUpdate(num_out_,cpt++);
        
        Interval error = kron_solver_errors_->line_product(sparse_coeff_errors_);
        res.error[num_out_] = error;
    }

    out = Iv + res.error[num_out_];
    if (Intersection(out,bound))
    {
        if (inf_inside)
            return INSIDE;
        else 
        {
//             std::cout<<"OVERLAP FINAL"<<std::endl;
            return OVERLAP;
        }
    }
    
    return OUTSIDE;
}
