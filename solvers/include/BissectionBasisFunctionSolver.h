#ifndef BissectionBasisFunctionSolver_H
#define BissectionBasisFunctionSolver_H
#include <iostream>
#include <fstream>

#include "BasisFunctionSolver.h"
#include "IntervalEstimator.h"


class BissectionBasisFunctionSolver: public BasisFunctionSolver
{
    public:
        BissectionBasisFunctionSolver(AbstractCSP* pb, 
                                      AbstractBasisFunction* bf,
                                      uint bissection_mode);

        virtual ~BissectionBasisFunctionSolver();

    protected:
    private:

        virtual void init(double eps);
        
        virtual void update_input();

};

#endif // BissectionBasisFunctionSolver_H
