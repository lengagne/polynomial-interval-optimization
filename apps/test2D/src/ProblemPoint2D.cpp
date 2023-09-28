#include "ProblemPoint2D.h"

void ProblemPoint2D::init()
{
    get_criteria_ = false;  // no optimization
    
    nb_var_ = 2;  // 2 dimensions
    nb_fun_ = 3;  // 3 points

    input_.resize(nb_var_);
    input_[0] = Hull (-10,10);
    input_[1] = Hull (-10,10);

    input_name_.push_back("X");
    input_name_.push_back("Y");

    bound_.resize(nb_fun_);
    bound_[0] = Power(Hull (4,5),2);
    bound_[1] = Power(Hull (4,5),2);
    bound_[2] = Power(Hull (3,4),2);

    output_name_.push_back("out1");
    output_name_.push_back("out2");
    output_name_.push_back("out3");

    P1.resize(2);
    P2.resize(2);
    P3.resize(2);
    P1[0] = 0.5;    P1[1] = 0.5;
    P2[0] = 3;     P2[1] = 3;
    P3[0] = 5;     P3[1] = -4;
}

void ProblemPoint2D::prepare_non_linear( std::vector<MogsInterval>& in)
{

}
