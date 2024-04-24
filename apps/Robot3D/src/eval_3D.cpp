#include <iostream>
#include <vector>

#include "Solver.h"
#include "ChooseSolver.h"
#include "data_format.h"
#include "Problem3D_with_torque_limit.h"
#include <locale.h>



int main( int argc, char** argv)
{   
    std::cout<<"Evaluation of the outputs for several input" <<std::endl;
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "C");
    
    ChooseBasisFunction choice;
    ChooseSolver choice_solver;
    uint nb_basis = choice.get_nb_basis_type()+1;
    nb_basis = 2;
    
    std::cout<<"on va evaluer "<< nb_basis <<" basis functions."<<std::endl;
//     int ndof = 6;    
//     Problem3D_with_torque_limit pb("../../../model/kuka_lwr.xml"        ,"kuka_lwr_7_link",1.0,0,1);
    int ndof = 4;
    Problem3D_with_torque_limit pb("../../../model/kuka_lwr_4dof.xml"        ,"kuka_lwr_7_link",1.0,0,1);
    
    std::vector<double> diam;
    diam.push_back(2.0);
    diam.push_back(1.0);
//     diam.push_back(0.5);
    diam.push_back(0.1);
//     diam.push_back(0.05);
    diam.push_back(0.01);
//     diam.push_back(0.005);
    diam.push_back(0.001);
    
    
    std::ofstream outfile_diam ("eval_3D_diam.tex");
    std::ofstream outfile_value("eval_3D_value.tex");
    
    outfile_diam<<std::scientific << std::setprecision(2)<<std::setiosflags(std::ios_base::scientific);
    outfile_value<<std::scientific << std::setprecision(2)<<std::setiosflags(std::ios_base::scientific);
    std::cout<<std::scientific << std::setprecision(2)<<std::setiosflags(std::ios_base::scientific);
    
    outfile_diam<<"\\begin{table}[htb]\n \\resizebox{\\textwidth}{!}{ \n  \\centering \n \\begin{tabular}{|c|c||";
    for (int i=0;i<4+ndof;i++)
            outfile_diam<<"c|";
    outfile_diam<<"|} \n  \\hline "<< std::endl;
    outfile_diam<<"input & eval & $X$ & $Y$ & $Z$ & ";
    for (int i=0;i<ndof;i++)
            outfile_diam<<"$ \\Gamma_"<<i+1<<"$ &";
    outfile_diam<<" $\\sum \\Gamma_i^2$\\\\ \\hline  \\hline  "<< std::endl;
    
    outfile_value<<"\\begin{table}[htb]\n \\resizebox{\\textwidth}{!}{ \n \\centering \n \\begin{tabular}{|c|c||";
    for (int i=0;i<4+ndof;i++)
            outfile_value<<"c|";
    outfile_value<<"} \n  \\hline "<< std::endl;
    outfile_value<<"input & eval & $X$ & $Y$ & $Z$ &  ";
    for (int i=0;i<ndof;i++)
            outfile_value<<"$ \\Gamma_"<<i+1<<"$ &";
    outfile_value<<" $\\sum \\Gamma_i^2$\\\\ \\hline  \\hline " << std::endl;    
    
    
    
    std::vector<Interval> out_inter;
    std::vector<Interval> q(6);
    
    std::vector<double> middle(6);
    middle[0] = -2.35;
    middle[1] = -0.15;
    middle[2] = -1.75;
    middle[3] =  1.05;
    
    AbstractSolver* solver;
    
    for (int j=0;j<diam.size();j++)
    {
        outfile_diam<<"\\multirow{2}{*}{$";//<< 0.5 + Hull(-diam[j],diam[j])<<" $} ";
        outfile_value<<"\\multirow{2}{*}{$";//<< 0.5 + Hull(-diam[j],diam[j])<<" $} ";

        outfile_diam<<" [";
        scientificFormat(outfile_diam, 0.5-diam[j]);
        outfile_diam<<" :";
        scientificFormat(outfile_diam, 0.5+diam[j]);
        outfile_diam<<" ] $}  ";

        outfile_value<<" [";
        scientificFormat(outfile_value, 0.5-diam[j]);
        outfile_value<<" :";
        scientificFormat(outfile_value, 0.5+diam[j]);
        outfile_value<<" ] $}  ";        
        
        
        std::cout<<"DIAM = "<< diam[j]<<std::endl;
        for (int i=0;i<ndof;i++)
        {
            q[i] = middle[i]+Hull(-diam[j],diam[j]);
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
            
            for (int j=0;j<ndof;j++)
                std::cout<<"TORQUE("<<j<<") = "<< out_inter[3+j]<<"  D = "<< Diam(out_inter[3+j])<<std::endl;
            std::cout<<"CRITERE = "<< out_inter[3+ndof]<<"  D = "<< Diam(out_inter[3+ndof])<<std::endl;            
            
            std::cout<<"Latex interval";
            for (int j=0;j<4+ndof;j++)
            {
                outfile_diam<<" & ";
                scientificFormat(outfile_diam, Diam(out_inter[j]));
                outfile_value<<" & [";
                scientificFormat(outfile_value, Inf(out_inter[j]));
                outfile_value<<" :";
                scientificFormat(outfile_value, Sup(out_inter[j]));
                outfile_value<<" ] ";
                
                std::cout<<" & "<< out_inter[j];
            }
            if (i <nb_basis -1)
            {                
                outfile_diam<<"\\\\ \\cline{2-"<<ndof+6<<"} "<<std::endl;
                outfile_value<<"\\\\ \\cline{2-"<<ndof+6<<"} "<<std::endl;
            }else
            {
                outfile_diam<<"\\\\ \\hline \\hline "<<std::endl;
                outfile_value<<"\\\\ \\hline \\hline "<<std::endl;
            }
            std::cout<<std::endl;

            std::cout<<"Latex Diam";
            for (int j=0;j<4+ndof;j++)
            {
                std::cout<<" & "<< Diam(out_inter[j]);
            }
            std::cout<<std::endl<<std::endl;
            delete solver;
        }               
    }
    
    outfile_diam<<"\\end{tabular} \n } \n";
    outfile_diam<<"\\caption{Diameter of the end effector position, joint torque and sum of square joint torque for several inputs considering several basis functions. } \n";
    outfile_diam<<"\\label{tab_eval_6dof_diam} \n \\end{table} \n";

    outfile_value<<"\\end{tabular} \n } \n";
    outfile_value<<"\\caption{Value of the end effector position, joint torque and sum of square joint torque for several inputs considering several basis functions. } \n";
    outfile_value<<"\\label{tab_eval_6dof_value} \n \\end{table} \n";    
    
    outfile_diam.close();
    outfile_value.close();
    return 0;
}
