#ifndef __RESULT_H__
#define __RESULT_H__

#include "Interval.h"
#include "utils.h"

class Result
{
public:
    Result();
    
    Result(const std::vector<Interval> &input,
           uint nb_ctr,
           bool optim);
 
    void operator=( const Result& res);
    
    ~Result();
    
    std::vector<Interval> in;
    std::vector<Interval> out;
    std::vector<Interval> error;
    std::vector<bool> ctr_ok;
    std::vector<bool> error_ok;
    
    std::vector<double> bissect_weight;
    std::vector<double> inf_sup_proba;
    
//     bool info_defined = false;
    uint nb_info;
    
    
    friend std::ostream& operator<< (std::ostream& stream, const Result& res);
};



#endif // __RESULT_H__
