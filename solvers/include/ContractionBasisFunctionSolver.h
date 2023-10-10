#ifndef ContractionBasisFunctionSolver_H
#define ContractionBasisFunctionSolver_H
#include <iostream>
#include <fstream>

#include "BasisFunctionSolver.h"
#include "IntervalEstimatorContractor.h"


class ContractionBasisFunctionSolver: public BasisFunctionSolver
{
    public:
        ContractionBasisFunctionSolver(AbstractCSP* pb,  
                                       AbstractBasisFunction* bf,
                                       uint bissection_mode);

        virtual ~ContractionBasisFunctionSolver();

        virtual void evaluate( const std::vector<Interval> &in,
                               std::vector<Interval> &out)
        {
            std::cerr<<"Function evaluate not defined yet for ContractionBasisFunctionSolver"<<std::endl;
        }

        /// Update the intermediate values
        void update(Result & current_value);

    protected:
        
        virtual void init(double eps);

        virtual void set_next();
        
        virtual void update_input();
        
    std::vector<MogsInterval> output_Interval_value;

    std::vector< std::vector<MogsInterval*> > inter_intermediate_to_contract_;
    std::vector< std::vector<IntervalEstimatorContractor*> > infos_intermediate_to_contract_;
    std::vector< std::vector< unsigned int > > id_intermediate_to_contract_;


       


};

#endif // ContractionBasisFunctionSolver_H
