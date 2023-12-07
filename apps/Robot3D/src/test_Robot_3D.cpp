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
	unsigned int problem = 0;
    uint basisfunction = 1;
    uint bissection_mode = 0;    
    int test = -1;
       
    if (argc ==2 && (std::string( argv[1]) == "h" || std::string( argv[1]) =="help"))
    {
        std::cout<<"Call ./Robot3D precision problem bissection_mode basis_function"<<std::endl;
        exit(0);
    }
    
	if(argc > 1)
        precision = atof(argv[1]);
	if(argc > 2)
        problem = atof(argv[2]);
	if(argc > 3)
        bissection_mode = atoi(argv[3]);    
	if(argc > 4)
        test = atoi(argv[4]);
    
    std::cout<<"precision = "<< precision <<std::endl;
    std::cout<<"problem = "<< problem <<std::endl;
    std::cout<<"bissection_mode = "<< bissection_mode <<std::endl;
    std::cout<<"test = "<< test <<std::endl;
    
	bool print=false;;
    
    uint critere = 0;
    uint target = 0;
    double coeff_torque = 1.0;
    std::string robot_file;
    
    AbstractCSP* pb;
    
    if (problem < 10)
        critere = 0;
    else
        critere = 1;
    
    switch(problem)
    {
        // full torque, target (0.4,0.4,0.7)
        case(1):
        case(11):
                coeff_torque = 1.0;
                target = 0;
                robot_file = "../model/kuka_lwr_4dof.xml";
                break;
        // full torque, target (0.6,0.6,0.6)
        case(2):
        case(12):    
                coeff_torque = 1.0;
                target = 1;
                robot_file = "../model/kuka_lwr_4dof.xml"; 
                break;
        // 10% torque, target (0.4,0.4,0.7)
        case(3):
        case(13):
                coeff_torque = 0.1;
                target = 0;
                robot_file = "../model/kuka_lwr_4dof.xml";
                break;
        
        // 10% torque, target (0.6,0.6,0.6)
        case(4):
        case(14):
                coeff_torque = 0.1;
                target = 1;
                robot_file = "../model/kuka_lwr_4dof.xml";
                break;
        
        // full torque, target (0.4,0.4,0.7)
        case(5):
        case(15):
                coeff_torque = 1.0;
                target = 0;
                robot_file = "../model/kuka_lwr.xml";
                break;
        
        // full torque, target (0.6,0.6,0.6)
        case(6):
        case(16):
                coeff_torque = 1.0;
                target = 1;
                robot_file = "../model/kuka_lwr.xml";
                break;

        // 10% torque, target (0.4,0.4,0.7)
        case(7):
        case(17):
                coeff_torque = 0.1;
                target = 0;
                robot_file = "../model/kuka_lwr.xml";
                break;
        
        // 10% torque, target (0.6,0.6,0.6)
        case(8):
        case(18):
                coeff_torque = 0.1;
                target = 1;
                robot_file = "../model/kuka_lwr.xml";
                break;
        
        default: 
            std::cerr<<"Error case "<< problem <<" not defined"<<std::endl;
            exit(0);
    }
    std::cout<<"robot_file = "<< robot_file <<std::endl;
    std::cout<<"coeff_torque = "<< coeff_torque <<std::endl;
    std::cout<<"target = "<< target <<std::endl;
    std::cout<<"critere = "<< critere <<std::endl;
    
    pb = new Problem3D_with_torque_limit( mogs_string(robot_file.c_str()) ,"kuka_lwr_7_link",coeff_torque,target,critere);        
    
    CompareSolver cp(pb);   
    cp.compare("Robot3D_model_"+std::to_string(problem),precision,bissection_mode,test);
    delete pb;
    return 0;
}
