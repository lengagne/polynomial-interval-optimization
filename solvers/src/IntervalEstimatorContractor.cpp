#include "IntervalEstimatorContractor.h"

IntervalEstimatorContractor::IntervalEstimatorContractor(AbstractBasisFunction* bf,
const MogsInterval& b):IntervalEstimator(bf)
{
    bound_ = b;
}
        
unsigned int IntervalEstimatorContractor::prepare_coeffs( const MogsInterval& out, unsigned int num_out)
{  
//     std::cout<<"prepare_coeffs out = "<< out <<std::endl;
    num_out_ = num_out;
    local_dual_ = out.dual(bound_);
    local_dual_.get_dependancies_with_order(dep_,order_);
    nb_in_ = dep_.size();    
          
    //coefdep_
    coefdep_.push_back(1);
    for (int i=0;i<nb_in_-1;i++)
        coefdep_.push_back( coefdep_[i] * (order_[i]+1));
    
    local_M_.resize(nb_in_);
    local_M_inverse_.resize(nb_in_);
    for (int j=0;j<nb_in_;j++)
        local_M_[nb_in_-j-1].resize(order_[j]+1,order_[j]+1);  
        
    for( int i=0;i<nb_in_;i++)
    {
        Interval val = dep_[i]->value_;
        bf_->get_basis_coeff_matrix(val, order_[i],local_M_[nb_in_-i-1],local_M_inverse_[nb_in_-i-1]);
    }

    coefdep_inputs_.push_back(1);
    for (int i=0;i<nb_in_;i++)  if(! dep_[i]->rely_on_error())
    {
        dep_inputs_.push_back(dep_[i]);
        order_inputs_.push_back(order_[i]);
    }
    for (int i=0;i<dep_inputs_.size();i++)
        coefdep_inputs_.push_back( coefdep_inputs_[i] * (order_inputs_[i]+1));
    
    
    for (int i=0;i<nb_in_;i++)  if(! dep_[nb_in_-i-1]->rely_on_error())
        local_M_inverse_inputs_.push_back(local_M_inverse_[i]);

    uint nb_coeff_ = 1;
    for (int i = 0;i<dep_.size();i++)
        nb_coeff_ *= order_[i]+1;
    
    kron_solver_inputs_ = new Kronecker(local_M_inverse_inputs_);
    kron_solver_errors_ = new Kronecker(local_M_inverse_);

    
    nb_control_point_inputs_ = kron_solver_inputs_->get_nb_control_point();    


        std::list<unsigned int > coeff_inputs_, coeff_errors_;
    std::vector<unsigned int > mem_index_input(nb_control_point_inputs_);
    std::map<unsigned int, LazyVariable> MCT_coeff_;
// 
    uint nb_valid_coeff = 0;
    nb_sparse_errors_ = 0;
//     
    for( std::map<mem*,LazyVariable>::const_iterator it = local_dual_.dependances_.begin(); it != local_dual_.dependances_.end();it++)
    {        
        unsigned int index = get_index(it->first );        
        MCT_coeff_[index] = local_dual_.get_value(it->first);
        if(! MCT_coeff_[index].is_null())
        {
//             std::cout<<"index = "<< index <<std::endl;
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
    
    sparse_coeff_errors_.resize(nb_sparse_errors_); 
       
    
    coeff_errors_.sort();
    coeff_inputs_.sort();
    kron_solver_errors_->prepare_line_product_interval(coeff_errors_);
    
    
    std::map<unsigned int, unsigned int> coeff_index_contract;
    for(std::list<unsigned int >::const_iterator it = coeff_inputs_.begin(); it != coeff_inputs_.end(); it++)
    {
        coeff_index_contract[*it] = nb_valid_coeff;        
    }




    
    
     // deal with input
    Eigen::Matrix<LazyVariable,Eigen::Dynamic,1> dual(nb_control_point_inputs_);
    for (int i=0;i<nb_control_point_inputs_;i++)
    {
        dual(i) = 0;
        for(std::list<unsigned int >::const_iterator it = coeff_inputs_.begin(); it != coeff_inputs_.end(); it++)
        {
            double v = kron_solver_inputs_->get_value(i,*it);
            dual(i) += MCT_coeff_[mem_index_input[*it]]* v;
        }
        LazyAddOutput(dual(i),num_out_,nb_valid_coeff++);
    }
        
    // deal with error
    for(std::list<unsigned int >::const_iterator it = coeff_errors_.begin(); it != coeff_errors_.end(); it++)
    {
        LazyAddOutput(MCT_coeff_[*it],num_out_,nb_valid_coeff++);
    }          
    
    unsigned int nb_dep = local_dual_.dependances_.size();
    for( std::map<mem*,LazyVariable>::const_iterator it = local_dual_.dependances_.begin(); it != local_dual_.dependances_.end();it++)
    {
        const mem& m = *(it->first);
        if(/*m.is_it_output() &&*/ !m.contains_error()&& m.is_contractable(contraction_mode_) )
        {
            std::vector<unsigned int> contractor_index;
            unsigned int index = get_index_input(it->first);
            std::list<unsigned int> tmp;
            tmp.push_back(index);
            contractor_index.push_back(index);
//             std::cout<<"index = "<< index <<std::endl;
            Kronecker* tmp_kron = new Kronecker(local_M_inverse_inputs_);
            specific_kron_solvers_.push_back(tmp_kron);

            contractor_input tmp_contr;
            tmp_contr.is_output = m.is_it_output(); // remember is it is an output
            tmp_contr.dependancy = it->first;
            tmp_contr.index = index;
            contractors_.push_back(tmp_contr);
        }
//        else
//        {
//            std::cout<< std::endl<< std::endl<<"we wont contract "<< m<< std::endl;
//        }
    }
    
    // deal with dual     
    unsigned int nb_c = contractors_.size();
    for (int i=0;i<nb_c;i++)
    {
        contractor_input& c = contractors_[i];
        LazyVariable v = MCT_coeff_[mem_index_input[c.index]];
        LazyAddOutput(v,num_out,nb_valid_coeff++);  // permettra juste de vérifier que v != 0
        Eigen::Matrix<LazyVariable,Eigen::Dynamic,1> dual_tmp = dual;
        for (int j=0;j<nb_control_point_inputs_;j++)
        {
            dual_tmp(j) -= specific_kron_solvers_[i]->get_value(j,c.index) * v;            
            LazyAddOutput(dual_tmp(j),num_out,nb_valid_coeff++);
        }         
    }
    
    return nb_valid_coeff;
}

// check_constraint IntervalEstimatorContractor::update_from_inputs( Interval& out, Interval& bound)
check_constraint IntervalEstimatorContractor::update_from_inputs( Result& res, Interval& bound,uint index_ctr)
{  
    unsigned int cpt = 0;
    Eigen::Matrix<double,Eigen::Dynamic,1> dual(nb_control_point_inputs_);
    dual(0) = LazyUpdate(num_out_,cpt++);
    Interval out = dual(0);
    for (int i=1;i<nb_control_point_inputs_;i++)
    {
        dual(i) = LazyUpdate(num_out_,cpt++);
        out = Hull(out,dual(i));
    }    
    
    for (unsigned int i=0;i<nb_sparse_errors_;i++)
        sparse_coeff_errors_(i) = LazyUpdate(num_out_,cpt++);    

    Interval error = kron_solver_errors_->line_product(sparse_coeff_errors_);
   

    unsigned int nb_c = contractors_.size();
    check_constraint result = OVERLAP;
    for (int i=0;i<nb_c;i++)
    {
        contractor_input& c = contractors_[i];
        double v =  LazyUpdate(num_out_,cpt++); 
        Interval input = LazyUpdate(num_out_,cpt++); 
        for (int j=1;j<nb_control_point_inputs_;j++)
        {
            double  val = LazyUpdate(num_out_,cpt++);
            input = Hull( input, val); 
        }
        input = input + error;
        if (v !=0.0)
        {
            input = -input / v;//
            
            if(c.dependancy->contract(input) == OUTSIDE )
            {
                return OUTSIDE;
            }
            if( c.is_output)    // Be Carefull we assume output is the last value !!
            {

                if ( Inf(input) >= -1.0 && Sup(input)<= 1.0)   // due to ref
                {
                    result= INSIDE;
                }
            }            
        }
    }
    
    return result;
}
