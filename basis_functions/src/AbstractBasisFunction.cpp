#include "AbstractBasisFunction.h"
       
void AbstractBasisFunction::get_basis_coeff_matrix( const Interval& inter,
                                                    uint order,
                                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                                    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat_inverse
                                                  )
{
    inter_ = inter;
    std::map<uint, Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >::iterator it ;
    
//     std::cout<<"looking for order "<< order <<" if already asked. "<<std::endl;
    it = mat_order_.find(order);      
    if(it == mat_order_.end())
    {
        compute_basis_coeff_matrix(inter, order,mat,mat_inverse);        
        mat_order_[order] = mat;
        mat_inverse_order_[order] = mat_inverse;
    }
    else
    {
        mat = mat_order_[order];
        mat_inverse = mat_inverse_order_[order];
    }
    
    
    
}

void AbstractBasisFunction::get_time_max( const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& mat,
                                          Eigen::Matrix<double,Eigen::Dynamic,1>& tmax)
{
    uint s = mat.rows();
    tmax.resize(s);      
    double inf = Inf(inter_);
    double diam = Diam (inter_);
    
    int nb = 11;
    double delta = diam / (nb-1);
//     std::cout<<"mat = "<< mat <<std::endl;
//     std::cout<<"s = "<< s <<std::endl;
    for (int i=0;i<s;i++)
    {
        // compute each value
        double max = 0;
        double maxt = inf;
        for (int j=0;j<nb;j++)
        {
            double t = inf + j *delta;
            
            double val = 0;
            for (int k=0;k<s;k++)
                val += mat (k,i) * pow (t,k);
            
            if (val > max)
            {
                max = val;
                maxt = t;
            }          
        }
        tmax(i) = maxt;
    }
//     std::cout<<"tmax = "<< tmax.transpose() <<std::endl;
    
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
