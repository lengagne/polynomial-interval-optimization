#include <iostream>
#include <vector>
#include <locale.h>

#include "CompareSolver.h"
#include "Problem3D.h"
#include "Problem3D_with_torque_limit.h"

int main( int argc, char** argv)
{
    std::cout<<"type_of_problem : 3D"<<std::endl;
	// this application is required due to QXmlValidator in MOGS
    QCoreApplication a(argc, argv);

    setlocale(LC_ALL, "C");

	double precision = 0.001;
	unsigned int model = 0;
    uint basisfunction = 1;
    uint bissection_mode = 0;    
    int test = -1;
    
	if(argc > 1)
        precision = atof(argv[1]);
	if(argc > 2)
        model = atof(argv[2]);
	if(argc > 3)
        bissection_mode = atoi(argv[3]);    
	if(argc > 4)
        test = atoi(argv[4]);
    
    std::cout<<"precision = "<< precision <<std::endl;
    std::cout<<"model = "<< model <<std::endl;
    std::cout<<"bissection_mode = "<< bissection_mode <<std::endl;
    std::cout<<"test = "<< test <<std::endl;
    
	bool print=false;;
    AbstractCSP* pb;
    switch(model)
    {
        // full torque, target (0.4,0.4,0.7), critere torque
        case(1): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"   ,"kuka_lwr_7_link",1.0,0,1);  break;
        
        // full torque, target (0.6,0.6,0.6), critere torque
        case(2): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"   ,"kuka_lwr_7_link",1.0,1,1);  break;

        // 10% torque, target (0.4,0.4,0.7), critere torque
        case(3): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"   ,"kuka_lwr_7_link",0.1,0,1);  break;
        
        // 10% torque, target (0.6,0.6,0.6), critere torque
        case(4): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"   ,"kuka_lwr_7_link",0.1,1,1);  break;  
        
        // full torque, target (0.4,0.4,0.7), critere torque
        case(5): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"   ,"kuka_lwr_7_link",1.0,0,1);  break;
        
        // full torque, target (0.6,0.6,0.6), critere torque
        case(6): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"   ,"kuka_lwr_7_link",1.0,1,1);  break;

        // 10% torque, target (0.4,0.4,0.7), critere torque
        case(7): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"   ,"kuka_lwr_7_link",0.1,0,1);  break;
        
        // 10% torque, target (0.6,0.6,0.6), critere torque
        case(8): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"   ,"kuka_lwr_7_link",0.1,1,1);  break;  
        
        default: 
            std::cerr<<"Error case "<< model <<" not defined"<<std::endl;
            exit(0);
    }
    
    CompareSolver cp(pb);   
    cp.compare("Robot3D_model_"+std::to_string(model),precision,bissection_mode,test);
    delete pb;
    return 0;
}
