#ifndef Recursive2Function_H
#define Recursive2Function_H

#include "AbstractBasisFunction.h"
#include "Polynomial.h"

class Recursive2Function: public AbstractBasisFunction
{
    public:
       
        Recursive2Function(): AbstractBasisFunction()
        {

        }         
        
        ~Recursive2Function()
        {
            
        }
        
        virtual void compute_basis_coeff_matrix(const Interval& inter,
                                                uint order,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse);

        std::string get_name() const
        {
            return "Recursive2";
        }
        
    protected:
    private:
};

#endif // AbstractBasisFunction_H
