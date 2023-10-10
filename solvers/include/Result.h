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
    std::vector<bool> bissect_bool;
    bool bissect_inf_sup;
    
    uint mode = 0;  // 0 normal , 1 smart bissection (considering weight)
    
    friend std::ostream& operator<< (std::ostream& stream, const Result& res);
};



#endif // __RESULT_H__
