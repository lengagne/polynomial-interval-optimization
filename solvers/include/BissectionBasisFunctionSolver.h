#ifndef BissectionBasisFunctionSolver_H
#define BissectionBasisFunctionSolver_H
#include <iostream>
#include <fstream>

#include "BasisFunctionSolver.h"
// #include "ComputedTree.h"
#include "IntervalEstimator.h"


class BissectionBasisFunctionSolver: public BasisFunctionSolver
{
    public:
        BissectionBasisFunctionSolver(AbstractCSP* pb, AbstractBasisFunction* bf);

        virtual ~BissectionBasisFunctionSolver();

//         virtual param_optim solve_optim(double eps=1e-3);

    protected:
    private:

        virtual void init(double eps);
        
        virtual void update_input();

};

#endif // BissectionBasisFunctionSolver_H
