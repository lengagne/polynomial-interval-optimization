#include <iostream>
#include <vector>
#include <fstream>
#include "MogsInterval.h"
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
    ChooseBasisFunction choice;
    
    if(argv ==2)
    {
        if (std::string(argc[1]) == "help")
        {
            std::cout<<"Use ./PlotBasisFunctions min_order max_order min_bf max_bf"<<std::endl;
            exit(0);
        }
    }
    
    Interval inter= Hull(-1,1.);
//    std::cout<<"inter = "<< inter <<std::endl;
    uint min_order = 1;
    if (argv>1)
        min_order = atoi(argc[1]);    
    uint max_order = 10;
    if (argv>2)
        max_order = atoi(argc[2]);
    
    uint min_bfi = 1;    
    uint max_bfi = choice.get_nb_basis_type();
    std::vector<uint> vorder, vbf;
    for (int i=min_order;i<=max_order;i++)   vorder.push_back(i);
    for (int i=min_bfi;i<=max_bfi;i++)   vbf.push_back(i);
    
    AbstractBasisFunction* bf;
    
    std::ofstream outfile ("basis_functions.tex");
    
    outfile<<"\\begin{table}[htb]\n";
    outfile<<"\\tiny \n";
    outfile<<"\\begin{tabular}{|c|c|c|c|}\n \\hline \n";
    outfile<<"type & order&  $\\mathbf B^{-1} $ \\\\  \\hline \\hline\n";
    
    for (int j=0;j<vbf.size();j++) for (int i=0;i<vorder.size();i++) 
    {    
        uint order = vorder[i];
        uint bfi = vbf[j];
        
        std::string basis_type = choice.get_basis_type(j);
        std::cout<<std::endl<<"basis = "<< basis_type <<std::endl;
        std::cout<<"order = "<< order <<std::endl;
        choice.choose(&bf,j);
        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M(order+1,order+1),N(order+1,order+1);
        
        bf->get_basis_coeff_matrix(inter,order,M,N);
        std::cout<<"Matrice = "<< M <<std::endl;
        std::cout<<"Inverse = "<< N <<std::endl;
        
        outfile<<basis_type <<" & " << order <<" &  "<< latex_matrix(N)<<" \\\\ \\hline\n";
        
        delete bf;
        
        std::vector< std::vector<double> > values (order+1+1);
        unsigned int nb = 1001;

        std::ofstream data, gnuplot, cpoint;
        data.open ("log.txt");
    //     cpoint.open("control_point.txt");
        gnuplot.open("gnuplot.txt");

//         gnuplot<<"set terminal png size 1200,900 enhanced font \"Helvetica\" 14 "<<std::endl;
//         gnuplot<<"set output '"<<basis_type<<"_"<< order<<".png"<<std::endl;
        gnuplot<<"set term postscript color eps level3 "<<std::endl;
        gnuplot<<"set output '"<<basis_type<<"_"<< order<<".eps"<<std::endl;
        gnuplot<<"plot ";
        for (int j=0;j<order+1;j++)
        {
            gnuplot<<" \"log.txt\" u 1:"<<j+2 <<" w l lw 2";
            gnuplot<<" title \"b"<<j+1<<"\",";
        }
        gnuplot<<std::endl;
        gnuplot.close();

        for (int i=0;i<order+1;i++)
        {
            double dt = Diam(inter)/(order);
            double t = Inf(inter) + i * dt;
        }

        for (int i=0;i<nb;i++)
        {
            double dt = Diam(inter)/(nb-1);
            double t = Inf(inter) + i * dt;
    //         double out = 0;
            data<<t<<"\t";
            for (int j=0;j<order+1;j++)
            {
                double tmp = 0; 
                for(int k =0;k<order+1;k++) tmp += pow(t,k) * M(k,j);
                values[j].push_back(tmp);
    //             out+= tmp*control_point[j];
                data<<tmp<<"\t";
            }
            data<< std::endl;
        }
        data.close();
        system("gnuplot gnuplot.txt");
        
    }
    
    outfile<<"\\end{tabular} \n";
    outfile<<"\\end{table}\n";
    outfile.close();

}
