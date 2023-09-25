#ifndef MinNoFunction_H
#define MinNoFunction_H

#include "OptimizedBasisFunction.h"


class MinNoFunction: public OptimizedBasisFunction
{
    public:
       
        MinNoFunction(bool re_order=false);
        
        ~MinNoFunction()
        {
            
        }
        
    protected:
    private:
};

#endif // AbstractBasisFunction_H
