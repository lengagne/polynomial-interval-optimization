#ifndef BasisFunctionSolver_H
#define BasisFunctionSolver_H
#include <iostream>
#include <fstream>

#include "AbstractSolver.h"
// #include "ComputedTree.h"
#include "IntervalEstimator.h"


class BasisFunctionSolver: public AbstractSolver
{
    public:
       
        virtual param_optim solve_optim(double eps=1e-3);

    protected:
        
        virtual void init(double eps);
        
        virtual void init_end();
        
        virtual void set_next();
        
        virtual param_optim set_results();
        
//         void set_max_iter( uint m)
//         {
//             max_iter_ = m;
//         }
        
        virtual void update_input() = 0;

        bool solve_optim_ = false;

        std::vector<Result> current_vector_;
        Result current_value_;

        std::vector<MogsInterval> input_Interval,output_Interval;
        std::vector<MogsInterval> Intermediate_to_compute;
        std::vector<MogsInterval> Intermediate_to_update;
        unsigned int nb_intermediate_;

        std::vector<IntervalEstimator*> infos;
        std::vector<IntervalEstimator*> infos_intermediate_update;
        
        IntervalEstimator* info_crit_;

        std::vector<MogsInterval> intermediate_;

        bool init_done = false;
        
        bool find_one_feasible_ =false;
        
        double optim_crit_;
        
        AbstractBasisFunction* bf_;
        
//         uint max_iter_ = 1e20;
};

#endif // BasisFunctionSolver_H
