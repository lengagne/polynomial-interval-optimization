
#include <iostream>

template < typename T >
void ProblemBasisFunctionComputation::functions(BasisFunctionProblem<T>* pb, 
                                                std::vector<T> & in,
                                                std::vector<T> & out)
{

    out = pb->Constraints(in);
//     out[0] = cos(in[0]);
//     out[0] += cos(in[0]) * cos(in[1]) - sin(in[0]) * sin(in[1]);
//     out[1] = sin(in[0]) ;
//     out[1] +=  cos(in[0]) * sin(in[1]) + cos(in[1]) * sin(in[0]);
//    std::cout<<"ProblemBasisFunctionComputation out[0] = "<< out[0] <<std::endl;
}
