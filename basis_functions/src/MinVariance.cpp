#ifdef IPOPT_FOUND

#include "MinVariance.h"
       
MinVariance::MinVariance(bool re_order): OptimizedBasisFunction("MinVariance",re_order)
{
    for (uint i=2;i<=2;i++)
    {
        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M(i+1,i+1);
        switch(i)
        {
            case(2):
                M(0,0) = 0.249987443692760158420185;
                M(0,1) = 0.500025112614479794181932;
                M(0,2) = 0.249987443692760158420185;
                M(1,0) = 0.499999999369356629141947;
                M(1,1) = -0;
                M(1,2) = -0.499999999369356629141947;
                M(2,0) = 0.250012556307239897090966;
                M(2,1) = -0.500025112614479794181932;
                M(2,2) = 0.250012556307239897090966;
                break;

            default:
                break;
        }
        mat_order_[i]= M;
        mat_inverse_order_[i] = M.inverse();
    }    
}
       
#endif // IPOPT_FOUND
