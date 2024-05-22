#include "ChooseBasisFunction.h"

#include "ApproxMinvoFunction.h"
#include "MinVoFunction.h"
#include "MinNoFunction.h"
#include "MinVariance.h"
#include "BernsteinFunction.h"
#include "BSplinesFunction.h"
#include "RecursiveFunction.h"
#include "Recursive2Function.h"


ChooseBasisFunction::ChooseBasisFunction()
{
    uint cpt = 0;
    basis_functions_[cpt++] = "Bernstein";      // 0 
#ifdef IPOPT_FOUND    
    basis_functions_[cpt++] = "MinVariance";    // 1
#endif    
    basis_functions_[cpt++] = "BSplines";       // 2
#ifdef IPOPT_FOUND
    basis_functions_[cpt++] = "MinNo";          // 3 
#endif        
#ifdef IPOPT_FOUND
    basis_functions_[cpt++] = "MinVo";          // 4 
#endif    
    basis_functions_[cpt++] = "ApproxMinVo";    // 5    

    basis_functions_[cpt++] = "Recursive";// 6 
    basis_functions_[cpt++] = "Recursive2";// 7 
}

void ChooseBasisFunction::choose(AbstractBasisFunction** bf, 
                                 uint index)
{
    uint cpt = 0;
    
    if (index == cpt ++)    //  0
    {
        *bf = new BernsteinFunction(); 
        return;
    }

#ifdef IPOPT_FOUND 
    if (index == cpt ++)    //  1
    {
        *bf = new MinVariance(); 
        return;
    }
#endif // IPOPT_FOUND
    
    
    if (index == cpt ++)    //  2
    {
        *bf = new BSplinesFunction(); 
        return;
    }

#ifdef IPOPT_FOUND     
    if (index == cpt ++)    //  3
    {
        *bf = new MinNoFunction(); 
        return;
    }
#endif // IPOPT_FOUND
    
#ifdef IPOPT_FOUND    
    if (index == cpt ++)    //  4
    {
        *bf = new MinVoFunction();
        return;
    }    
#endif // IPOPT_FOUND


    if (index == cpt ++)    //  5
    {
        *bf = new ApproxMinvoFunction(); 
            return;
    }
   
    if (index == cpt ++)    //  6
    {
        *bf = new RecursiveFunction(); 
        return;
    }


    if (index == cpt ++)    //  7
    {
        *bf = new Recursive2Function(); 
        return;
    }    
        std::cerr<<"Error in FILE "<< __FILE__<<" at line "<< __LINE__<<" the index "<<index <<" is not defined yet"<<std::endl;
        exit(123);
}


