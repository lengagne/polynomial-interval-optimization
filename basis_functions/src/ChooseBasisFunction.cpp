#include "ChooseBasisFunction.h"

#include "ApproxMinvoFunction.h"
#include "MinVoFunction.h"
#include "MinNoFunction.h"
#include "BernsteinFunction.h"
#include "BSplinesFunction.h"
#include "RecursiveBSplinesFunction.h"
#include "RecursiveBSplinesFunction2.h"


ChooseBasisFunction::ChooseBasisFunction()
{
//     basis_functions_.clear();
//     basis_functions_.push_back("Bernstein");
//     basis_functions_.push_back("BernsteinReOrder");
//     basis_functions_.push_back("BSplines");
//     basis_functions_.push_back("BSplinesReOrder");
//     basis_functions_.push_back("ApproxMinVo");
//     basis_functions_.push_back("ApproxMinVoReOrder");
//     basis_functions_.push_back("MinVo");
//     basis_functions_.push_back("MinVoReOrder");
//     basis_functions_.push_back("MinNo");
//     basis_functions_.push_back("MinNoReOrder");
//     basis_functions_.push_back("MinVarianceB");
//     basis_functions_.push_back("MinVarianceBReOrder");
//     basis_functions_.push_back("RecursiveBSplines");
//     basis_functions_.push_back("RecursiveBSplinesReOrder");
//     basis_functions_.push_back("RecursiveBSplines2");    
//     basis_functions_.push_back("RecursiveBSplines2ReOrder");   
    
/*    basis_functions_[0] = "Bernstein";
    basis_functions_[1] = "BernsteinReOrder";
    basis_functions_[2] = "BSplines";
    basis_functions_[3] = "BSplinesReOrder";
    basis_functions_[4] = "ApproxMinVo";
    basis_functions_[5] = "ApproxMinVoReOrder";
    basis_functions_[6] = "MinVo";
    basis_functions_[7] = "MinVoReOrder";
    basis_functions_[8] = "MinNo";
    basis_functions_[9] = "MinNoReOrder";
    basis_functions_[10] = "MinVarianceB";
    basis_functions_[11] = "MinVarianceBReOrder";
    basis_functions_[12] = "RecursiveBSplines";
    basis_functions_[13] = "RecursiveBSplinesReOrder";
    basis_functions_[14] = "RecursiveBSplines2";    
    basis_functions_[15] = "RecursiveBSplines2ReOrder"; */   

    basis_functions_[0] = "Bernstein";
    basis_functions_[1] = "BSplines";
    basis_functions_[2] = "ApproxMinVo";
    basis_functions_[3] = "MinVo";
    basis_functions_[4] = "MinNo";
    basis_functions_[5] = "MinVarianceB";
    basis_functions_[6] = "RecursiveBSplines";
}

void ChooseBasisFunction::choose(AbstractBasisFunction** bf, 
                                 uint index)
{
    switch(index)
    {
//         case(0): *bf = new BernsteinFunction();     break;
//         case(1): *bf = new BernsteinFunction(true); break;
//         case(2): *bf = new BSplinesFunction();      break;
//         case(3): *bf = new BSplinesFunction(true);      break;
//         case(4): *bf = new ApproxMinvoFunction();   break;
//         case(5): *bf = new ApproxMinvoFunction(true);   break;
//         case(6): *bf = new MinVoFunction();         break;
//         case(7): *bf = new MinVoFunction(true);         break;
//         case(8): *bf = new MinNoFunction();         break;
//         case(9): *bf = new MinNoFunction(true);         break;
//         case(10): *bf = new OptimizedBasisFunction("MinVarianceB");  break;
//         case(11): *bf = new OptimizedBasisFunction("MinVarianceB",true);  break;
//         case(12): *bf = new RecursiveBSplinesFunction(); break;
//         case(13): *bf = new RecursiveBSplinesFunction(true); break;
//         case(14): *bf = new RecursiveBSplinesFunction2();break;
//         case(15): *bf = new RecursiveBSplinesFunction2(true);break;
        
        case(0): *bf = new BernsteinFunction();     break;
        case(1): *bf = new BSplinesFunction();      break;
        case(2): *bf = new ApproxMinvoFunction();   break;
        case(3): *bf = new MinVoFunction();         break;
        case(4): *bf = new MinNoFunction();         break;
        case(5): *bf = new OptimizedBasisFunction("MinVarianceB");  break;
        case(6): *bf = new RecursiveBSplinesFunction(); break;
        
        default:
        std::cerr<<"Error in FILE "<< __FILE__<<" at line "<< __LINE__<<" the index "<<index <<" is not defined yet"<<std::endl;
        exit(123);
    }    
}


