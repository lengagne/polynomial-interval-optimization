#include "ProblemBasisFunctionComputation.h"

void ProblemBasisFunctionComputation::init()
{
    pb_double_ = new  BasisFunctionProblem<double> (order_,criteria_type_);
    pb_interval_ = new BasisFunctionProblem<Interval>(order_,criteria_type_);
    pb_mogs_interval_ = new BasisFunctionProblem<MogsInterval>(order_,criteria_type_); 
    pb_intervals_containers_ =new BasisFunctionProblem<IntervalsContainers>(order_,criteria_type_);
    
    
    
    nb_var_ = pb_double_->GetNumberParam();
    nb_fun_ = pb_double_->GetNumberConstraints(); // les contraintes + 1 critere

    input_.resize(nb_var_);
    for (int i=0;i<nb_var_;i++)
    {
        input_[i] = Hull (-1,1);
        input_name_.push_back("root_"+std::to_string(i+1));
    }

    std::vector<double> low,high;
    pb_double_->GetConstraintLimits( low, high);
    bound_.resize(nb_fun_);
    for (int i=0;i<nb_fun_;i++)
    {
        bound_[i] = Hull(low[i],high[i]);
        output_name_.push_back("ctr_"+std::to_string(i+1));
    }
}

void ProblemBasisFunctionComputation::prepare_non_linear( std::vector<MogsInterval>& in)
{

}
