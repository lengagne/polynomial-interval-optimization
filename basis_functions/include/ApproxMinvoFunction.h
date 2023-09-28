#ifndef ApproxMinvoFunction_H
#define ApproxMinvoFunction_H

#include "AbstractBasisFunction.h"


class ApproxMinvoFunction: public AbstractBasisFunction
{
    public:
       
        ApproxMinvoFunction(bool re_order=false): AbstractBasisFunction()
        {
            re_order_ = re_order;
        }               
        
        virtual void compute_basis_coeff_matrix(const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);

    protected:
    private:
};

#endif // AbstractBasisFunction_H