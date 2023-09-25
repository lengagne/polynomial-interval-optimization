#ifndef RecursiveBSplinesFunction_H
#define RecursiveBSplinesFunction_H

#include "AbstractBasisFunction.h"
#include "Polynomial.h"

class RecursiveBSplinesFunction: public AbstractBasisFunction
{
    public:
       
        RecursiveBSplinesFunction(bool re_order=false): AbstractBasisFunction()
        {
            re_order_ = re_order;
        }         
        
        ~RecursiveBSplinesFunction()
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
