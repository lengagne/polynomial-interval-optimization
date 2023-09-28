
#include <iostream>

template < typename T >
void ProblemPoint2D::functions(std::vector<T> & in,
                               std::vector<T> & out)
{
    out[0] = 0.;
    out[1] = 0.;
    out[2] = 0.;
    for (int i=0;i<2;i++)
    {
        T p1 = in[i] - P1[i];
        T p2 = in[i] - P2[i];
        T p3 = in[i] - P3[i];
        out[0] +=  p1*p1;
        out[1] +=  p2*p2;
        out[2] +=  p3*p3;
    }
}
