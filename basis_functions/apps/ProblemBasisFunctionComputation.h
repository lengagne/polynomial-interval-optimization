#ifndef ProblemBasisFunctionComputation_H
#define ProblemBasisFunctionComputation_H

#include <AbstractCSP.h>
#include "BasisFunctionProblem.h"

class ProblemBasisFunctionComputation : public AbstractCSP
{
    public:
        ProblemBasisFunctionComputation(uint order, const std::string& criteria_type):AbstractCSP(),order_(order),criteria_type_(criteria_type)
        {
            pb_name_ = "ProblemBasisFunctionComputation";
        }

        template < typename T >
        void functions( BasisFunctionProblem<T> *pb, 
                        std::vector<T> & in,
                        std::vector<T> & out);

        void function( std::vector<double>& input,
                      std::vector<double>& output)
        {
            functions<double>(pb_double_,input,output);
        }

        void function( std::vector<Interval>& input,
                      std::vector<Interval>& output)
        {
            functions<Interval>(pb_interval_,input,output);
        }

        void function( std::vector<MogsInterval>& input,
                      std::vector<MogsInterval>& output)
        {
            functions<MogsInterval>(pb_mogs_interval_,input,output);
        }
        void function( std::vector<IntervalsContainers>& input,
                       std::vector<IntervalsContainers>& output)
        {
            functions<IntervalsContainers>(pb_intervals_containers_,input,output);
        }

        virtual void init();

        virtual void prepare_non_linear( std::vector<MogsInterval>& in);

    protected:

    private:
        uint order_;
        std::string criteria_type_;
        
        BasisFunctionProblem<double> * pb_double_;
        BasisFunctionProblem<Interval> * pb_interval_;
        BasisFunctionProblem<MogsInterval> * pb_mogs_interval_;
        BasisFunctionProblem<IntervalsContainers> * pb_intervals_containers_;

};

#include "ProblemBasisFunctionComputation.hxx"

#endif // ProblemBasisFunctionComputation_H
