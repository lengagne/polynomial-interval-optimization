#include <iostream>
#include <vector>
#include <fstream>
#include "MogsInterval.h"
#include "ChooseBasisFunction.h"

int main( int argv, char** argc)
{
    Interval inter= Hull(-1,1.);
//    std::cout<<"inter = "<< inter <<std::endl;
    
    uint max_order = 2;
    if (argv>1)
        max_order = atoi(argc[1]);
    
    uint max_bfi = 5;
    if (argv>2)
        max_bfi = atoi(argc[2]);
    
    std::vector<uint> vorder, vbf;
    for (int i=1;i<=max_order;i++)   vorder.push_back(i);
    for (int i=2;i<=max_bfi;i++)   vbf.push_back(i);
    
    AbstractBasisFunction* bf;
    
    for (int i=0;i<vorder.size();i++)   for (int j=0;j<vbf.size();j++)
    {    
        uint order = vorder[i];
        uint bfi = vbf[j];
        
        
        std::string basis_type = get_basis_type(bfi);
        choose_basis_function(&bf,basis_type);

        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M(order+1,order+1),N(order+1,order+1);
        
        bf->get_basis_coeff_matrix(inter,order,M,N);
        
        std::cout<<"order = "<< order<<"\t type = "<< basis_type <<" determinant = "<< M.determinant()<<std::endl;
    }
}
