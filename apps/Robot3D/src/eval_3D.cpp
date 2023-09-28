#include <iostream>
#include <vector>

#include "Solver.h"
#include "Problem3D.h"
#include "Problem3D_with_torque_limit.h"
#include <locale.h>

std::vector<double> steps = {2,1,0.5,0.1,0.01};

int main( int argc, char** argv)
{
// 	// this application is required due to QXmlValidator in MOGS
//     QCoreApplication a(argc, argv);
// 
//     setlocale(LC_ALL, "C");
// 
// 	double precision = 0.001;
// 	unsigned int model = 6;
// 	unsigned int solveur = 1;
// 	if(argc > 1)
//         precision = atof(argv[1]);
// 	if(argc > 2)
//         model = atoi(argv[2]);
// 	if(argc > 3)
//         solveur = atoi(argv[3]);
// 
//     if (model !=4 && model != 6)
//     {
//         std::cerr<<"Error we consider only model 4 or 6 (corresponding to the number of dof)"<<std::endl;
//         return 1664;
//     }
// 
//     std::cout<<"precision = "<< precision <<std::endl;
//     std::cout<<"model = "<< model <<std::endl;
// 
//     std::vector<Interval> q(model);
// 
// 
//     if (solveur == 1)
//     {
//         AbstractCSP* pb;
//         if (model == 4)
//         {
//             pb = new Problem3D_with_torque_limit("../data/kuka_lwr_4dof.xml"        ,"kuka_lwr_7_link",1.0,0,0);
//         }else
//         {
//             pb = new Problem3D_with_torque_limit("../data/kuka_lwr.xml"        ,"kuka_lwr_7_link",1.0,0,0);
//         }
// 
//         AbstractSolver* solver_inter = new BissectionIntervalSolver(pb);
//         for (int j=0;j<steps.size();j++)
//         {
// //             std::cout<<"step = "<< steps[j]<<std::endl;
//             for (int i=0;i<model;i++)
//             {
//                 q[i] = 0.5+Hull(-steps[j]/2, steps[j]/2);
//                 std::cout<<"q["<<i<<"] = "<< q[i]<<std::endl;
//             }
// 
//             std::vector<Interval> out_inter;
//             solver_inter->evaluate(q,out_inter);
//             std::cout<<"X = "<< out_inter[0]<<"  D = "<< Diam(out_inter[0])<<std::endl;
//             std::cout<<"Y = "<< out_inter[1]<<"  D = "<< Diam(out_inter[1])<<std::endl;
//             std::cout<<"Z = "<< out_inter[2]<<"  D = "<< Diam(out_inter[2])<<std::endl;
//             std::cout<<std::endl<<std::endl;
//         }
// 
//         delete pb;
//         delete solver_inter;
//     }
//     else //if (solveur == 2)
//     {
//         AbstractCSP* pb;
//         if (model == 4)
//         {
//             pb = new Problem3D_with_torque_limit("../data/kuka_lwr_4dof.xml"        ,"kuka_lwr_7_link",1.0,0,0);
//         }else
//         {
//             pb = new Problem3D_with_torque_limit("../data/kuka_lwr.xml"        ,"kuka_lwr_7_link",1.0,0,0);
//         }
//         AbstractSolver* solver_splines;
//         if (solveur == 2)
//             solver_splines = new BissectionBasisFunctionSolver(pb,"Bernstein");
//         else if (solveur == 3)
//             solver_splines = new BissectionBasisFunctionSolver(pb,"BBasisFunction");
//         else if (solveur == 4)
//             solver_splines = new BissectionBasisFunctionSolver(pb,"ApproxMinvo");
//         else if (solveur == 5)
//             solver_splines = new BissectionBasisFunctionSolver(pb,"Minvo");
//         else{
//             std::cerr<<"Error the solveur "<< solveur <<" is not implemented."<<std::endl; 
//         }        
//         
//         
//         std::vector<Interval> out_splines;
//         for (int j=0;j<steps.size();j++)
//         {
//             std::cout<<"step = "<< steps[j]<<std::endl;
//             for (int i=0;i<model;i++)
//             {
//                 q[i] = 0.5+Hull(-steps[j]/2, steps[j]/2);
//                 std::cout<<"q["<<i<<"] = "<< q[i]<<std::endl;
//             }
//             solver_splines->evaluate(q,out_splines);
//             std::cout<<"X = "<< out_splines[0]<<"  D = "<< Diam(out_splines[0])<<std::endl;
//             std::cout<<"Y = "<< out_splines[1]<<"  D = "<< Diam(out_splines[1])<<std::endl;
//             std::cout<<"Z = "<< out_splines[2]<<"  D = "<< Diam(out_splines[2])<<std::endl;
//             std::cout<<std::endl<<std::endl;
//         }
//         delete pb;
//         delete solver_splines;
//     }
// 


    return 0;
}
