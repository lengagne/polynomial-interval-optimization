#include <iostream>
#include <vector>

#include "CompareSolver.h"
#include "Problem1.h"
#include "Problem4.h"
#include "Problem7.h"

#include "Problem1Torque.h"
#include "Problem4Torque.h"
#include "Problem7Torque.h"

#include "construct_filename_2D.h"



int main( int argc, char** argv)
{
    std::cout<<"type_of_problem : 2D"<<std::endl;
    if (argc == 2 )
    {
        std::string com = argv[1];
        if (com == "help")
        {
            std::cout<<" You must run : \n ./Robot2D ndof nbpb precision"<<std::endl;
            return 0;
        }
    }    
    
    if (argc ==2 && (std::string( argv[1]) == "h" || std::string( argv[1]) =="help"))
    {
        std::cout<<"Call ./Robot2D ndof problem precision bissection_mode basis_function"<<std::endl;
        exit(0);
    }    
    
	unsigned int ndof = 2;
    unsigned int npb = 1;
    double precision = 0.001;
    uint bissection_mode = 0;
    int test = -1;
	if(argc > 1)
        ndof = atoi(argv[1]);
	if(argc > 2)
        npb = atoi(argv[2]);    
	if(argc > 3)
        precision = atof(argv[3]);   
	if(argc > 4)
        bissection_mode = atoi(argv[4]);        //MinFirst or Maxfirst
	if(argc > 5)
        test = atoi(argv[5]);   

    std::string save_filename;    
    if(test != -1)
    {
        if (argc >= 7)
        {
            save_filename = argv[6];
        }else
        {
            save_filename =  construct_filename_2D(ndof,npb,precision,bissection_mode,test);
        }
    }
    
    
    std::cout<<"******************************" <<std::endl;
    std::cout<<"ndof = "<< ndof <<std::endl;
    std::cout<<"npb = "<< npb <<std::endl;
    std::cout<<"precision = "<< precision <<std::endl;

	bool print=false;;
    AbstractCSP* pb;
    switch(npb)
    {
        case(1): pb = new Problem1(ndof,0.2,1.7);   break;
        case(2): pb = new Problem1(ndof,0.4,1.4);   break;
        case(3): pb = new Problem1(ndof,0.6,1.1);   break;
        case(4): pb = new Problem4(ndof,0.2,1.7);   break;
        case(5): pb = new Problem4(ndof,0.4,1.4);   break;
        case(6): pb = new Problem4(ndof,0.6,1.1);   break;
        case(7): pb = new Problem7(ndof,0.2,1.7);   break;
        case(8): pb = new Problem7(ndof,0.4,1.4);   break;
        case(9): pb = new Problem7(ndof,0.6,1.1);   break;

        case(11): pb = new Problem1Torque(ndof,0.2,1.7);   break;
        case(12): pb = new Problem1Torque(ndof,0.4,1.4);   break;
        case(13): pb = new Problem1Torque(ndof,0.6,1.1);   break;
        case(14): pb = new Problem4Torque(ndof,0.2,1.7);   break;
        case(15): pb = new Problem4Torque(ndof,0.4,1.4);   break;
        case(16): pb = new Problem4Torque(ndof,0.6,1.1);   break;
        case(17): pb = new Problem7Torque(ndof,0.2,1.7);   break;
        case(18): pb = new Problem7Torque(ndof,0.4,1.4);   break;
        case(19): pb = new Problem7Torque(ndof,0.6,1.1);   break;	
        default: std::cout<<"Problem not found"<<std::endl; exit(0);
    }
    
    CompareSolver cp(pb);   
    cp.compare("Robot2D_"+std::to_string(ndof)+"_ndof_pb_"+std::to_string(npb),precision,bissection_mode,test,save_filename);
        
    delete pb;
    
    return 0;
}
