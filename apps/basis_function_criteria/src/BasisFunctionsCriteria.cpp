#include <iostream>
#include <vector>
#include <fstream>
#include "BasisFunctionProblem.h"
#include "ChooseBasisFunction.h"

std::string latex_matrix  ( const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& M)
{
    std::string out;
    
    out += "$ \\left[ \\begin\{array\}\{";
    for (int i=0;i<M.rows();i++)    out += "c";
    out += "\}\n";
    for (int i=0;i<M.rows();i++) 
    {
        for (int j=0;j<M.rows();j++) 
        {
            out += std::to_string( M(i,j));
            if ( j < M.rows()-1)
                out += " & ";
            else
                out += " \\\\ \n";
        }
    }
    out += "\\end\{array\} \\right]$\n";    
    
    return out; 
    
}

int main( int argv, char** argc)
{
    std::cout<<"BasisFunction Criteria"<<std::endl;
    ChooseBasisFunction choice;
    
    Interval inter= Hull(-1,1.);

    
    uint min_order = 1;
    uint max_order = 6;
    
    uint min_bfi = 0;    
    uint max_bfi = choice.get_nb_basis_type();

    AbstractBasisFunction* bf;
     
    std::ofstream outfile ("compare_criteria.tex");
    // create the plots    
    std::cout<<"creating plots"<<std::endl;
    outfile<<"\\begin{table}[htb]\n";
    outfile<<"\\centering";
    outfile<<"\\begin{tabular}{|c|c|c|c|c|}\n \\hline \n";
    outfile<<"order & type &  Min Vo & Min No & Min Va  \\\\  \\hline \\hline\n";   
    
    
    for (int order=min_order;order<=max_order;order++) 
    {
        std::cout<<"DEGREE : "<< order <<std::endl;
        
        outfile<<"\\multirow{8}{*}{"<< order <<"}" ;
        
        
        BasisFunctionProblem<double> crit_MinVo(order,"MinVo");
        BasisFunctionProblem<double> crit_MinNo(order,"MinNo");
        BasisFunctionProblem<double> crit_MinVa(order,"MinVariance");
        for (int j=min_bfi;j<max_bfi;j++) 
//         int j=1;
        {    
        
            std::string basis_type = choice.get_basis_type(j);
            std::cout<<std::endl<<"basis = "<< basis_type <<std::endl;
    //         std::cout<<"order = "<< order <<std::endl;
            choice.choose(&bf,j);
            Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M(order+1,order+1),N(order+1,order+1);
            
            bf->get_basis_coeff_matrix(inter,order,M,N);
            std::cout<<"Matrice = "<< M <<std::endl;
//             std::cout<<"Inverse = "<< N <<std::endl;
            
            double crit_vo = crit_MinVo.CostFunction(M);
            std::cout<<"for Minvo : crit = "<< crit_vo <<std::endl;
            double crit_no = crit_MinNo.CostFunction(M);
            std::cout<<"for MinNo : crit = "<< crit_no <<std::endl;
            double crit_va = crit_MinVa.CostFunction(M);
            std::cout<<"for MinVariance : crit = "<< crit_va <<std::endl;                        
            
            if (j==1)
            {
                outfile<<"&"<<basis_type<<"&"<< crit_vo<<"&"<< crit_no<<" & \\textbf{" << crit_va<<"}";
            }else if (j==3)
            {
                outfile<<"&"<<basis_type<<"&"<< crit_vo<<"& \\textbf{" << crit_no<<"} &" << crit_va;
            }else if (j==4)
            {
                outfile<<"&"<<basis_type<<"&\\textbf{" << crit_vo<<"} &"<< crit_no<<" &" << crit_va;
            }else
                outfile<<"&"<<basis_type<<"&"<< crit_vo<<"&"<< crit_no<<" &" << crit_va;
            
            if (j==max_bfi-1)
                outfile<<"  \\\\ \\hline  \\hline " << std::endl;
            else
                outfile<<"  \\\\ \\cline{2-5}"<< std::endl;
            delete bf;
        }
    }
    
    outfile<<"\\end{tabular} \n";
    outfile<<"\\caption{Values of the criteria from order 1 to 6.   \}"<<std::endl;
    outfile<<"\\label{tab_criteria}"<<std::endl;
    outfile<<"\\end{table}\n";    
    outfile.close();
}
