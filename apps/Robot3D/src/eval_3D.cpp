#include <iostream>
#include <vector>

#include "Solver.h"
#include "ChooseSolver.h"
#include "Problem3D_with_torque_limit.h"
#include <locale.h>

std::vector<double> steps = {2,1,0.5,0.1,0.01};

int main( int argc, char** argv)
{
    int entree  = -1;
    if (argc == 2)
    {
        entree = atoi(argv[1]);
    }else
    {
        std::cerr<<"ERROR you must specify which one you want to have a look" <<std::endl;
        exit(1);
    }
    
    std::cout<<"Evaluation of the outputs for several input" <<std::endl;
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "C");
    
    ChooseBasisFunction choice;
    ChooseSolver choice_solver;
    uint nb_basis = choice.get_nb_basis_type();
    
    std::cout<<"on va evaluer "<< nb_basis <<" basis functions."<<std::endl;
    Problem3D_with_torque_limit pb("../model/kuka_lwr.xml"        ,"kuka_lwr_7_link",1.0,0,1);
//     Problem3D_with_torque_limit pb("../model/kuka_lwr_4dof.xml"        ,"kuka_lwr_7_link",1.0,0,1);
    
    std::vector<double> diam;
    diam.push_back(1.0);
    diam.push_back(0.5);
    diam.push_back(0.25);
    diam.push_back(0.05);
    diam.push_back(0.005);
    
//     for (int i=0;i<nb_basis;i++)
    int i = entree;
    {
        AbstractSolver* solver;
        std::cout<<"\n\nsolver : "<< choice_solver.get_solver_name(i)<<std::endl;
        
        choice_solver.choose(&pb,&solver,i,0);    
        std::vector<Interval> out_inter;
        std::vector<Interval> q(6);
        for (int j=0;j<steps.size();j++)
        {
            std::cout<<"DIAM = "<< diam[j]<<std::endl;
            for (int i=0;i<6;i++)
            {
                q[i] = 0.5+Hull(-diam[j],diam[j]);
                std::cout<<"q["<<i<<"] = "<< q[i]<<std::endl;
            }
            
            solver->evaluate(q,out_inter);
            std::cout<<"X = "<< out_inter[0]<<"  D = "<< Diam(out_inter[0])<<std::endl;
            std::cout<<"Y = "<< out_inter[1]<<"  D = "<< Diam(out_inter[1])<<std::endl;
            std::cout<<"Z = "<< out_inter[2]<<"  D = "<< Diam(out_inter[2])<<std::endl;
            for (int i=0;i<6;i++)
                std::cout<<"TORQUE("<<i<<") = "<< out_inter[3+i]<<"  D = "<< Diam(out_inter[3+i])<<std::endl;
            std::cout<<"CRITERE = "<< out_inter[9]<<"  D = "<< Diam(out_inter[9])<<std::endl;            
            
            std::cout<<"Latex interval";
            for (int i=0;i<10;i++)
            {
                std::cout<<" & "<< out_inter[i];
            }
            std::cout<<std::endl;

            std::cout<<"Latex Diam";
            for (int i=0;i<10;i++)
            {
                std::cout<<" & "<< Diam(out_inter[i]);
            }
            std::cout<<std::endl<<std::endl;
            
            
        }
        delete solver;
    }

    
        
    

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
