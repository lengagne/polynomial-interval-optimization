#ifdef IPOPT_FOUND

#ifndef MinVariance_H
#define MinVariance_H

#include "OptimizedBasisFunction.h"


class MinVariance: public OptimizedBasisFunction
{
    public:
       
        MinVariance(bool re_order=false);
        
        ~MinVariance()
        {
            
        }
        
    protected:
    private:
};

#endif // AbstractBasisFunction_H

#endif //  IPOPT_FOUND
