#ifdef IPOPT_FOUND

#ifndef MinVoFunction_H
#define MinVoFunction_H

#include "OptimizedBasisFunction.h"


class MinVoFunction: public OptimizedBasisFunction
{
    public:
       
        MinVoFunction(bool re_order=false);
        
        ~MinVoFunction()
        {
            
        }
        
    protected:
    private:
};

#endif // AbstractBasisFunction_H

#endif //  IPOPT_FOUND
