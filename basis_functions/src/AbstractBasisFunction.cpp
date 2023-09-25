#include "AbstractBasisFunction.h"
       
void AbstractBasisFunction::get_basis_coeff_matrix( const Interval& inter,
                                            uint order,
                                            Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                            Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse)
{

    std::map<uint, Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >::iterator it ;
    
//     std::cout<<"looking for order "<< order <<" if already asked. "<<std::endl;
    it = mat_order_.find(order);      
    if(it == mat_order_.end())
    {
//         std::cout<<"order "<< order<<" must be computed"<<std::endl;
        compute_basis_coeff_matrix(inter, order,mat,mat_inverse);        
        mat_order_[order] = mat;
        mat_inverse_order_[order] = mat_inverse;
    }
    else
    {
//         std::cout<<"order "<< order<<" already computed, used the previous one"<<std::endl;
        mat = mat_order_[order];
        mat = re_order(mat);
        mat_inverse = mat_inverse_order_[order];
    }
}

Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> AbstractBasisFunction::re_order(Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& M)
{
    if (re_order_)
    {
        uint nb = M.rows();
        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> mat(nb,nb);
        for (int k=0;k<(nb+1)/2;k++)
        {
            for(int i=0;i<nb;i++)
                mat(i,k*2) = M(i,k);
        }
        for (int k=0;k<(nb)/2;k++)
        {
            for(int i=0;i<nb;i++)           
                mat(i,k*2+1) = M(i,nb-1-k);
        }
        return mat;
    }else
        return M;
    
}
