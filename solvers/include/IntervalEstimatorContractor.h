#ifndef __INTERVALESTIMATORCONTRACTOR_H__
#define __INTERVALESTIMATORCONTRACTOR_H__

#include "IntervalEstimator.h"

typedef struct contractor_input{
    mem* dependancy;
//    Eigen::Matrix<double,Eigen::Dynamic,1> X2;
//    double value;
    bool is_output;
    unsigned int index;
//    unsigned int index_value;
}contractor_input;

class IntervalEstimatorContractor : public IntervalEstimator{
    public:

        IntervalEstimatorContractor(AbstractBasisFunction* bf,
                                    const MogsInterval& b);
        
//         ~IntervalEstimatorContractor();
        
        virtual unsigned int  prepare_coeffs( const MogsInterval& out, unsigned int num_out);

//         virtual check_constraint update_from_inputs( Interval& out, Interval& bound);        
        virtual check_constraint update_from_inputs( Result& res, Interval& bound,uint index_ctr);
        
        void define_bound(const MogsInterval& b);
        
        
// private:
        MogsInterval bound_;
        MogsInterval local_dual_;
        

        unsigned int contraction_mode_=0;
        
        std::vector<Kronecker*> specific_kron_solvers_;
        
        std::vector<contractor_input> contractors_;
};


#endif // OUTPUT_INFOS_H
