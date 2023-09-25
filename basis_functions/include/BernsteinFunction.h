#ifndef BernsteinFunction_H
#define BernsteinFunction_H

#include "AbstractBasisFunction.h"


class BernsteinFunction: public AbstractBasisFunction
{
    public:
       
        BernsteinFunction(bool re_order=false): AbstractBasisFunction()
        {
            re_order_ = re_order;
        }           
        
        ~BernsteinFunction()
        {
            
        }
        
        virtual void compute_basis_coeff_matrix(const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);

    protected:
    private:
};

#endif // AbstractBasisFunction_H
