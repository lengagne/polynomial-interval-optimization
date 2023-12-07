#include <iostream>
#include <vector>

#include "Solver.h"
#include "ChooseSolver.h"
#include "data_format.h"
#include "Problem3D_with_torque_limit.h"
#include <locale.h>

std::vector<double> steps = {2,1,0.5,0.1,0.01};

int main( int argc, char** argv)
{   
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
    diam.push_back(0.1);
    diam.push_back(0.05);
    diam.push_back(0.01);
    diam.push_back(0.005);
    diam.push_back(0.001);
    
    
    std::ofstream outfile_diam ("eval_3D_diam.tex");
    std::ofstream outfile_value("eval_3D_value.tex");
    
    outfile_diam<<"\\begin{table}[htb]\n \\tiny \n \\centering \n \\begin{tabular}{|c|c||c|c|c|c|c|c|c|c|c|c|} \n  \\hline "<< std::endl;
    outfile_diam<<"input & eval & $X$ & $Y$ & $Z$ & $\\Gamma_1$ & $\\Gamma_2$ & $\\Gamma_3$ & $\\Gamma_4$ & $\\Gamma_5$ & $\\Gamma_6$ & $\\sum \\Gamma_i^2$\\\\ \\hline  \\hline  "<< std::endl;
    
    outfile_value<<"\\begin{table}[htb]\n \\tiny \n \\centering \n \\begin{tabular}{|c|c||c|c|c|c|c|c|c|c|c|c|} \n  \\hline "<< std::endl;
    outfile_value<<"input & eval & $X$ & $Y$ & $Z$ & $\\Gamma_1$ & $\\Gamma_2$ & $\\Gamma_3$ & $\\Gamma_4$ & $\\Gamma_5$ & $\\Gamma_6$ & $\\sum \\Gamma_i^2$\\\\ \\hline  \\hline " << std::endl;    
    
    
    
    std::vector<Interval> out_inter;
    std::vector<Interval> q(6);
    
    AbstractSolver* solver;
    
    for (int j=0;j<steps.size();j++)
    {
        outfile_diam<<"\\multirow{2}{*}{$"<< 0.5 + Hull(-diam[j],diam[j])<<" $} ";
        outfile_value<<"\\multirow{2}{*}{$"<< 0.5 + Hull(-diam[j],diam[j])<<" $} ";
        
        
        std::cout<<"DIAM = "<< diam[j]<<std::endl;
        for (int i=0;i<6;i++)
        {
            q[i] = 0.5+Hull(-diam[j],diam[j]);
            std::cout<<"q["<<i<<"] = "<< q[i]<<std::endl;
        }        
        for (int i=0;i<nb_basis;i++)
        {                        
            choice_solver.choose(&pb,&solver,i,0);  
            outfile_diam<<" & "<< replace(choice_solver.get_solver_name(i));
            outfile_value<<" & "<< replace(choice_solver.get_solver_name(i));
            
            solver->evaluate(q,out_inter);
            std::cout<<"X = "<< out_inter[0]<<"  D = "<< Diam(out_inter[0])<<std::endl;
            std::cout<<"Y = "<< out_inter[1]<<"  D = "<< Diam(out_inter[1])<<std::endl;
            std::cout<<"Z = "<< out_inter[2]<<"  D = "<< Diam(out_inter[2])<<std::endl;
            
            std::cout<<"TORQUE("<<i<<") = "<< out_inter[3+i]<<"  D = "<< Diam(out_inter[3+i])<<std::endl;
            std::cout<<"CRITERE = "<< out_inter[9]<<"  D = "<< Diam(out_inter[9])<<std::endl;            
            
            std::cout<<"Latex interval";
            for (int j=0;j<10;j++)
            {
                outfile_diam<<" & "<< Diam(out_inter[j]) ;
                outfile_value<<" & "<< out_inter[j];
                std::cout<<" & "<< out_inter[j];
            }
            if (i <nb_basis -1)
            {                
                outfile_diam<<"\\\\ \\cline{2-12} "<<std::endl;
                outfile_value<<"\\\\ \\cline{2-12} "<<std::endl;
            }else
            {
                outfile_diam<<"\\\\ \\hline \\hline "<<std::endl;
                outfile_value<<"\\\\ \\hline \\hline "<<std::endl;
            }
            std::cout<<std::endl;

            std::cout<<"Latex Diam";
            for (int j=0;j<10;j++)
            {
                std::cout<<" & "<< Diam(out_inter[j]);
            }
            std::cout<<std::endl<<std::endl;
            delete solver;
        }               
    }
    
    outfile_diam<<"\\end{tabular} \n";
    outfile_diam<<"\\caption{Diameter of the end effector position, joint torque and sum of square joint torque for several inputs considering several basis functions. } \n";
    outfile_diam<<"\\label{tab_eval_6dof_diam} \n \\end{table} \n";

    outfile_value<<"\\end{tabular} \n";
    outfile_value<<"\\caption{Value of the end effector position, joint torque and sum of square joint torque for several inputs considering several basis functions. } \n";
    outfile_value<<"\\label{tab_eval_6dof_value} \n \\end{table} \n";    
    
    outfile_diam.close();
    outfile_value.close();
    return 0;
}
