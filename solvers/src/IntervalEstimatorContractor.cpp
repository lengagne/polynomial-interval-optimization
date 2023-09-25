#include "IntervalEstimatorContractor.h"

IntervalEstimatorContractor::IntervalEstimatorContractor(AbstractBasisFunction* bf):IntervalEstimator(bf)
{
    
}
        
unsigned int IntervalEstimatorContractor::prepare_coeffs( const MogsInterval& out, unsigned int num_out)
{  
    return IntervalEstimator::prepare_coeffs(out,num_out);
//     start_counter_ = start_counter;
//     num_out_ = num_out;
//     out.get_dependancies_with_order(dep_,order_);
//     nb_in_ = dep_.size();
//     if(nb_in_ == 0)
//     {
//         std::cerr<<"ERROR in "<< __FILE__<<" at line "<<__LINE__<<std::endl;
//         std::cerr<<"num_out = "<< num_out <<std::endl;
//         std::cerr<<"name = "<< out.name_ <<std::endl;
//         std::cerr<<"For the moment try to see constant value "<<std::endl;
//         exit(0);
//     }
//     //coefdep_
//     coefdep_.push_back(1);
//     for (int i=0;i<nb_in_-1;i++)
//         coefdep_.push_back( coefdep_[i] * (order_[i]+1));
// 
//     local_M_.resize(nb_in_);
//     local_M_inverse_.resize(nb_in_);
//     for (int j=0;j<nb_in_;j++)
//         local_M_[nb_in_-j-1].resize(order_[j]+1,order_[j]+1);
// 
//     for( int i=0;i<nb_in_;i++)
//     {
//         Interval val = dep_[i]->value_;
//         bf_->get_basis_coeff_matrix(val, order_[i],local_M_[nb_in_-i-1],local_M_inverse_[nb_in_-i-1]);
//     }
// 
//     coefdep_inputs_.push_back(1);
//     for (int i=0;i<nb_in_;i++)  if(! dep_[i]->rely_on_error())
//     {
//         dep_inputs_.push_back(dep_[i]);
//         order_inputs_.push_back(order_[i]);
//     }
//     for (int i=0;i<dep_inputs_.size();i++)
//         coefdep_inputs_.push_back( coefdep_inputs_[i] * (order_inputs_[i]+1));
// 
//     for (int i=0;i<nb_in_;i++)  if(! dep_[nb_in_-i-1]->rely_on_error())
//         local_M_inverse_inputs_.push_back(local_M_inverse_[i]);
// 
//     unsigned long long nb_coeff_ = 1;
//     for (int i = 0;i<dep_.size();i++)
//         nb_coeff_ *= order_[i]+1;
//     nb_coeff_inputs_ = 1;
//     for (int i = 0;i<dep_inputs_.size();i++)
//         nb_coeff_inputs_ *= order_inputs_[i]+1;
// 
//     std::list<unsigned int > coeff_inputs_, coeff_errors_;
//     std::vector<unsigned int > mem_index_input(nb_coeff_inputs_);
//     std::map<unsigned int, LazyVariable> MCT_coeff_;
// 
//     uint nb_valid_coeff_ = start_counter_;
//     nb_sparse_inputs_ = 0;
//     nb_sparse_errors_ = 0;
//     
//     for( std::map<mem*,LazyVariable>::const_iterator it = out.dependances_.begin(); it != out.dependances_.end();it++)
//     {        
//         unsigned int index = get_index(it->first );        
//         MCT_coeff_[index] = out.get_value(it->first);
//         if(! MCT_coeff_[index].is_null())
//         {
//             if( it->first->contains_error())
//             {
//                 nb_sparse_errors_++;
//                 coeff_errors_.push_back(index);
//             }else
//             {
//                 unsigned int index2 = get_index_input(it->first);
//                 nb_sparse_inputs_++;
//                 coeff_inputs_.push_back(index2);
//                 mem_index_input[index2] = index;
//             }
//         }
//     }
// 
//     coeff_errors_.sort();
//     coeff_inputs_.sort();
//     
//     kron_solver_inputs_ = new Kronecker(local_M_inverse_inputs_);
//     kron_solver_errors_ = new Kronecker(local_M_inverse_);
//     kron_solver_errors_->prepare_line_product_interval(coeff_errors_);
// 
//     sparse_coeff_errors_.resize(nb_sparse_errors_);
//   
//      nb_control_point_inputs_ = kron_solver_inputs_->get_nb_control_point();
//     
//     // deal with error
//     for(std::list<unsigned int >::const_iterator it = coeff_errors_.begin(); it != coeff_errors_.end(); it++)
//     {
//         LazyAddOutput(MCT_coeff_[*it],num_out_,nb_valid_coeff_++);
//     }  
// 
//     for (int i=0;i<nb_control_point_inputs_;i++)
//     {
//         LazyVariable out = 0;
//         for(std::list<unsigned int >::const_iterator it = coeff_inputs_.begin(); it != coeff_inputs_.end(); it++)
//         {
//             double v = kron_solver_inputs_->get_value(i,*it);
//             out += MCT_coeff_[mem_index_input[*it]]* v;
//         }
//         LazyAddOutput(out,num_out_,nb_valid_coeff_++);
//     }
//     
//     return nb_valid_coeff_;
}

// check_constraint IntervalEstimatorContractor::update_from_inputs( Interval& out, Interval& bound)
check_constraint IntervalEstimatorContractor::update_from_inputs( Result& res, Interval& bound,uint index_ctr)
{  
    return IntervalEstimator::update_from_inputs(res, bound,index_ctr);
    
//     double MAX =  std::numeric_limits<double>::max();
//     double MIN = -std::numeric_limits<double>::max();
// 
//     double ss = Sup(bound);
//     double ii = Inf(bound);
//     unsigned int cpt = start_counter_;
//     bool inf_inside,sup_inside, both_side;
//     // deal with error   
//     for (unsigned int i=0;i<nb_sparse_errors_;i++)
//         sparse_coeff_errors_(i) = LazyUpdate(num_out_,cpt++);
//     
//     Interval error = kron_solver_errors_->line_product(sparse_coeff_errors_);
//     Interval Iv = LazyUpdate(num_out_,cpt++) + error;
//     
//     for (int i=1;i<nb_control_point_inputs_;i++)
//     {
//         Interval value = LazyUpdate(num_out_,cpt++);
//         
//         Interval tmpIv = value + error;
//         Iv = Hull(Iv,tmpIv);
//         double iv = Inf(Iv);
//         double sv = Sup(Iv);
// 
//         inf_inside = (iv >= ii && iv <= ss);
//         sup_inside = (sv >= ii && sv <= ss);       
//         both_side = (iv <= ii && sv >= ss);
//         if ( inf_inside != sup_inside || both_side)
//         {
//             return OVERLAP;            
//         }
//     }
//     out = Iv;
//     if (Intersection(out,bound))
//     {
//         if (inf_inside)
//             return INSIDE;
//         else 
//             return OVERLAP;
//     }
//     
//     return OUTSIDE;
}
