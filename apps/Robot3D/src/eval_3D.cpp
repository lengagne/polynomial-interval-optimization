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
    uint nb_max_basis = 2;
//     uint nb_max_basis = nb_basis;
    
    std::cout<<"on va evaluer "<< nb_basis <<" basis functions."<<std::endl;
    int ndof = 6;    
    Problem3D_with_torque_limit pb("../../../model/kuka_lwr.xml"        ,"kuka_lwr_3_link",1.0,0,1);
//     int ndof = 4;
//     Problem3D_with_torque_limit pb("../../../model/kuka_lwr_4dof.xml"        ,"kuka_lwr_7_link",1.0,0,1);
    
    std::vector<double> diam;
//     diam.push_back(2.0);
//     diam.push_back(1.0);
//     diam.push_back(0.5);
//     diam.push_back(0.1);
//     diam.push_back(0.05);
//     diam.push_back(0.01);
//     diam.push_back(0.005);    
    diam.push_back(0.001);
    diam.push_back(0.001);
    
    
    std::vector<Interval> out_inter;
    std::vector<Interval> q(ndof);
    
    std::vector<double> middle(ndof);
    for (int i=0;i<ndof;i++)
        middle[i] = 0.5;
    
    AbstractSolver* solver;
    
    std::vector< std::vector< std::vector< Interval > > > resultats;
    resultats.resize(nb_basis);
    for (int i=0;i<nb_basis;i++)
    {
        resultats[i].resize(diam.size());
    }
    for (int i=0;i<nb_max_basis;i++)    
    {
//         choice_solver.choose(&pb,&solver,i,0);          
  
        for (int j=0;j<diam.size();j++)
        {  
            std::cout<<"**************************************"<<std::endl;
            std::cout<<"**************************************"<<std::endl;
            
//             LazyReset();
            choice_solver.choose(&pb,&solver,i,0);          
            for (int k=0;k<ndof;k++)
            {
                q[k] = middle[k]+Hull(-diam[j],diam[j]);
            }                  
           
            solver->evaluate(q,out_inter);
            resultats[i][j].resize(6+ndof);
            for (int k=0;k<6+ndof;k++)
                resultats[i][j][k] = out_inter[k];
            
            std::cout<<"DIAM = "<< diam[j]<<std::endl;
            std::cout<<"solver = " << choice_solver.get_solver_name(i) <<std::endl;
            std::cout<<"X = "<< out_inter[0]<<"  D = "<< Diam(out_inter[0])<<std::endl;
            std::cout<<"Y = "<< out_inter[1]<<"  D = "<< Diam(out_inter[1])<<std::endl;
            std::cout<<"Z = "<< out_inter[2]<<"  D = "<< Diam(out_inter[2])<<std::endl;
            std::cout<<"COM_X = "<< out_inter[3]<<"  D = "<< Diam(out_inter[3])<<std::endl;
            std::cout<<"COM_Y = "<< out_inter[4]<<"  D = "<< Diam(out_inter[4])<<std::endl;
            
            for (int j=0;j<ndof;j++)
                std::cout<<"TORQUE("<<j<<") = "<< out_inter[5+j]<<"  D = "<< Diam(out_inter[5+j])<<std::endl;
            std::cout<<"CRITERE = "<< out_inter[5+ndof]<<"  D = "<< Diam(out_inter[5+ndof])<<std::endl;            
                        
            
            std::string cmd = "mv *Lazy*.cpp  code_"+ std::to_string(i) + "_" + std::to_string(j) + ".cpp -v" ;
            system(cmd.c_str());
            delete solver;
        }   
//         delete solver;
    }

    std::ofstream outfile_diam ("eval_3D_diam.tex");
    std::ofstream outfile_value("eval_3D_value.tex");
    
    outfile_diam<<std::scientific << std::setprecision(2)<<std::setiosflags(std::ios_base::scientific);
    outfile_value<<std::scientific << std::setprecision(2)<<std::setiosflags(std::ios_base::scientific);
    std::cout<<std::scientific << std::setprecision(2)<<std::setiosflags(std::ios_base::scientific);
    
    // on créé les fichiers Latex
    outfile_diam<<"\\begin{table}[htb]\n \\resizebox{\\textwidth}{!}{ \n  \\centering \n \\begin{tabular}{|c|c||";
    for (int i=0;i<6+ndof;i++)
            outfile_diam<<"c|";
    outfile_diam<<"} \n  \\hline "<< std::endl;
    outfile_diam<<"diameter & basis function & $X$ & $Y$ & $Z$ & $COM_X$ & $COM_Y$ &";
    for (int i=0;i<ndof;i++)
            outfile_diam<<"$ \\Gamma_"<<i+1<<"$ &";
    outfile_diam<<" $\\sum \\Gamma_i^2$\\\\ \\hline  \\hline  "<< std::endl;
    
    outfile_value<<"\\begin{table}[htb]\n \\resizebox{\\textwidth}{!}{ \n \\centering \n \\begin{tabular}{|c|c||";
    for (int i=0;i<6+ndof;i++)
            outfile_value<<"c|";
    outfile_value<<"} \n  \\hline "<< std::endl;
    outfile_value<<"diameter & basis function  & $X$ & $Y$ & $Z$ & $COM_X$ & $COM_Y$ &";
    for (int i=0;i<ndof;i++)
            outfile_value<<"$ \\Gamma_"<<i+1<<"$ &";
    outfile_value<<" $\\sum \\Gamma_i^2$\\\\ \\hline  \\hline " << std::endl;    
    
    for (int j=0;j<diam.size();j++)
    {
        outfile_diam<<"\\multirow{"<<nb_max_basis<<"}{*}{"<<diam[j]<<"}";//<< 0.5 + Hull(-diam[j],diam[j])<<" $} ";
        outfile_value<<"\\multirow{"<<nb_max_basis<<"}{*}{"<<diam[j]<<"}";        

        for (int i=0;i<nb_max_basis;i++)
        {
            outfile_diam<<"& "<<replace(choice_solver.get_solver_name(i));//<< 0.5 + Hull(-diam[j],diam[j])<<" $} ";
            outfile_value<<" & "<<replace(choice_solver.get_solver_name(i));        
            for (int k=0;k<6+ndof;k++)
            {
                outfile_diam<<" & ";
                scientificFormat(outfile_diam, Diam(resultats[i][j][k]));
                outfile_value<<" & [";
                scientificFormat(outfile_value, Inf(resultats[i][j][k]));
                outfile_value<<" :";
                scientificFormat(outfile_value, Sup(resultats[i][j][k]));
                outfile_value<<" ] ";            
            }
            
            if (i < nb_max_basis -1 )
            {                
                outfile_diam<<"\\\\ \\cline{2-"<<ndof+8<<"} "<<std::endl;
                outfile_value<<"\\\\ \\cline{2-"<<ndof+8<<"} "<<std::endl;
            }else
            {
                outfile_diam<<"\\\\ \\hline \\hline "<<std::endl;
                outfile_value<<"\\\\ \\hline \\hline "<<std::endl;
            }            
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
