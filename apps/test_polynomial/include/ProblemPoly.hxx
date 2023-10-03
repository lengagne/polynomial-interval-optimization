
#include <iostream>

template < typename T >
void ProblemPoly::functions(   std::vector<T> & in,
                            std::vector<T> & out)
{
    T & X = in[0];
    
    out[0] = 2 + X - X*X + 1.4*X*X*X - 0.42*X*X*X*X ;// + 1.5 *X*X*X*X*X;
    
    out[1] = (X-1.42)*(X-1.42)+42;
}
