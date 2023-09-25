#ifndef __INTERVALESTIMATORCONTRACTOR_H__
#define __INTERVALESTIMATORCONTRACTOR_H__

#include "IntervalEstimator.h"


class IntervalEstimatorContractor : public IntervalEstimator{
    public:

        IntervalEstimatorContractor(AbstractBasisFunction* bf);
        
//         ~IntervalEstimatorContractor();
        
        virtual unsigned int  prepare_coeffs( const MogsInterval& out, unsigned int num_out);

//         virtual check_constraint update_from_inputs( Interval& out, Interval& bound);        
        virtual check_constraint update_from_inputs( Result& res, Interval& bound,uint index_ctr);
};


#endif // OUTPUT_INFOS_H
