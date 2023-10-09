#include <iostream>
#include <vector>
#include <locale.h>

#include "CompareSolver.h"
#include "Problem3D.h"
#include "Problem3D_with_torque_limit.h"

int main( int argc, char** argv)
{
	// this application is required due to QXmlValidator in MOGS
    QCoreApplication a(argc, argv);

    setlocale(LC_ALL, "C");

	double precision = 0.001;
	unsigned int model = 0;
    uint basisfunction = 2;
    int test = -1;
	if(argc > 1)
        precision = atof(argv[1]);
	if(argc > 2)
        model = atof(argv[2]);
	if(argc > 3)
        test = atoi(argv[3]);

    std::cout<<"precision = "<< precision <<std::endl;
    std::cout<<"model = "<< model <<std::endl;
    
	bool print=false;;
    AbstractCSP* pb;
    switch(model)
    {
        case(0): pb = new Problem3D("../model/kuka_lwr.xml","kuka_lwr_7_link");  break;
        case(1): pb = new Problem3D("../model/kuka_lwr_4dof.xml","kuka_lwr_7_link");  break;
        case(10): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"        ,"kuka_lwr_7_link",1.0,0,0);  break;
        case(11): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"        ,"kuka_lwr_7_link",0.1,0,0);  break;
        case(12): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"        ,"kuka_lwr_7_link",1.0,1,0);  break;
        case(13): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"        ,"kuka_lwr_7_link",0.1,1,0);  break;
        case(14): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"       ,"kuka_lwr_7_link",1.0,0,1);  break;
        case(15): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"       ,"kuka_lwr_7_link",0.1,0,1);  break;
        case(16): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"       ,"kuka_lwr_7_link",1.0,1,1);  break;
        case(17): pb = new Problem3D_with_torque_limit("../model/kuka_lwr.xml"       ,"kuka_lwr_7_link",0.1,1,1);  break;

        case(20): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"   ,"kuka_lwr_7_link",1.0,0,0);  break;
        case(21): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"   ,"kuka_lwr_7_link",0.1,0,0);  break;
        case(22): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"   ,"kuka_lwr_7_link",1.0,1,0);  break;
        case(23): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"   ,"kuka_lwr_7_link",0.1,1,0);  break;
        case(24): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"  ,"kuka_lwr_7_link",1.0,0,1);  break;
        case(25): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"  ,"kuka_lwr_7_link",0.1,0,1);  break;
        case(26): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"  ,"kuka_lwr_7_link",1.0,1,1);  break;
        case(27): pb = new Problem3D_with_torque_limit("../model/kuka_lwr_4dof.xml"  ,"kuka_lwr_7_link",0.1,1,1);  break;
    }
    
    CompareSolver cp(pb);   
    cp.compare("Robot3D_model_"+std::to_string(model),precision,test);
    delete pb;
    return 0;
}
