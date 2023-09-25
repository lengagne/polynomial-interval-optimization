#ifndef ContractionIntervalSolver_H
#define ContractionIntervalSolver_H
#include <iostream>
#include <fstream>

#include "AbstractSolver.h"


class ContractionIntervalSolver: public AbstractSolver
{
    public:
        ContractionIntervalSolver(AbstractCSP* pb);

        virtual ~ContractionIntervalSolver();

        virtual void evaluate( const std::vector<Interval> &in,
                               std::vector<Interval> &out)
        {
            std::cerr<<"Function evaluate not defined yet for ContractionIntervalSolver"<<std::endl;
        }

        virtual param_optim solve_optim(double eps=1e-3);



    protected:
        std::vector<IntervalsContainers> input_Interval,output_Interval;
    private:



};

#endif // ContractionIntervalSolver_H
