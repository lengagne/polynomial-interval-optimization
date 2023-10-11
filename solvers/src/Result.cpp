#include "Result.h"


Result::Result()
{
    in.clear();
    out.clear();
    error.clear();
    ctr_ok.clear();
    error_ok.clear();
    info_defined = false;
}
    
Result::Result( const std::vector<Interval> &input,
                uint nb_value,
                bool optim)
{
    in = input;     
    out.resize(nb_value+optim);
    ctr_ok.resize(nb_value);
    for (int i=0;i<nb_value;i++)
    {
        out[i] = 0.;
        ctr_ok[i] = false;
    }    

    error.resize(nb_value+optim);
    error_ok.resize(nb_value+optim);
    for (int i=0;i<nb_value+optim;i++)
    {
        error[i] = Hull(-42.0,42.42);   // 
        error_ok[i] = false;
    }  
    info_defined = false;
}
 
void Result::operator=( const Result& res)
{
//     std::cout<<"res = "<< res <<std::endl;
    in = res.in;
    out = res.out;
    error = res.error;
    ctr_ok = res.ctr_ok;
    error_ok = res.error_ok;
    info_defined = false;
}
 
    
Result::~Result()
{
    in.clear();
    out.clear();
    error.clear();
    ctr_ok.clear();
    error_ok.clear();
}

std::ostream& operator<< (std::ostream& stream, const Result& res)
{
    stream<<std::endl;
    for (int i=0;i<res.in.size();i++)
    {
        stream<<"INPUT["<<i<<"] = "<< res.in[i]<<std::endl;
    }
    for (int i=0;i<res.out.size();i++)
    {
        stream<<"OUTPUT["<<i<<"] = "<< res.out[i]<<std::endl;
    }
    return stream;
}
    
