#include "ProblemPoly.h"

ProblemPoly::ProblemPoly(    )
{
    get_criteria_ = true;
    
}

void ProblemPoly::init()
{
    nb_var_ = 1;
    nb_fun_ = 1;
    
   input_.resize(nb_var_);
    for (int i=0;i<nb_var_;i++)
        input_[i] = Hull (-30,30);    
//         input_[i] = Hull (-0.8,-0.7);    

    
    bound_.resize(nb_fun_);
    bound_[0] = Hull (0,1e6);
    
    output_name_.push_back("F(x)");
    output_name_.push_back("criteria");

}

void ProblemPoly::prepare_non_linear( std::vector<MogsInterval>& in)
{

}
