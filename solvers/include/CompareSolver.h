#ifndef CompareSolver_H
#define CompareSolver_H
#include <iostream>
#include <fstream>

#include "AbstractSolver.h"
#include "ChooseSolver.h"

class CompareSolver
{
    public:
        CompareSolver(AbstractCSP* pb):pb_(pb)
        {
            
            
        }

        virtual ~CompareSolver()
        {
            
        }

        void compare(   const std::string & filename,
                        double precision = 0.001
                    );


    protected:
    private:
        AbstractCSP *pb_;
        
        ChooseSolver choice_solver_;

};

#endif // DiscretizationSolver_H
