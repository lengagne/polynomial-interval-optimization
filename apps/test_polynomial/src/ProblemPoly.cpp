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

    
    bound_.resize(nb_fun_);
    bound_[0] = Hull (0,2);
    
    output_name_.push_back("F(x)");
    output_name_.push_back("criteria");

}

void ProblemPoly::prepare_non_linear( std::vector<MogsInterval>& in)
{
    in[0].prepare_son(SON_COS_ERROR);
    in[0].prepare_son(SON_SIN_ERROR);
}
