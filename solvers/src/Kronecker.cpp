#include "Kronecker.h"
#include <Eigen/Sparse>
#include <unsupported/Eigen/KroneckerProduct>

Kronecker::Kronecker( const std::vector< Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >& mats)
{
    basis_matrices_ = mats;
    nb_basis_ = basis_matrices_.size();
    size_matrices_.resize(nb_basis_);
    for (int i=0;i<nb_basis_;i++)
    {
        uint s = mats[i].rows();
        size_matrices_[i] = s;
    }
    offsets_.resize(nb_basis_);
    for (int i=0;i<nb_basis_;i++)
    {   
        uint off = 1;
        for (int j=i+1;j<nb_basis_;j++)
            off *= size_matrices_[j];                
        offsets_[i] = off;
    }
    
    nb_control_points_ = 1;
    for (int i=0;i<nb_basis_;i++)
        nb_control_points_ *= size_matrices_[i];
    
}

uint Kronecker::get_nb_control_point()const
{
    return nb_control_points_;
}

double Kronecker::get_value(uint lin, uint col) const
{
    double out = 1;
    for (int i=0;i<nb_basis_;i++)
    {
        out *= basis_matrices_[i]((lin/offsets_[i])%size_matrices_[i], (col/offsets_[i])%size_matrices_[i]);
    }
    return out;
}

Interval Kronecker::line_product(Eigen::Matrix<double,Eigen::Dynamic,1> & vec)
{
    if (vec.size() != line_products_interval_.size())
    {
        std::cerr<<"Error in "<< __FILE__<<" at line "<< __LINE__<<" : size not aligned"<<std::endl;
        std::cout<<"vec.size() = "<< vec.size()<<std::endl;
        std::cout<<"line_products_interval_.size() = "<< line_products_interval_.size()<<std::endl;
        exit(1);
    }    
    uint nb = vec.size();
    Interval out = 0;
    for (int i=0;i<nb;i++)
    {
        out+= line_products_interval_(i)*vec(i);
    }

    return out;    
}

void Kronecker::prepare_line_product_interval(const std::list<unsigned int > & vec)
{
    uint s = vec.size();
    line_products_interval_.resize(s);
    uint col = 0;
    for(std::list<unsigned int >::const_iterator it = vec.begin(); it != vec.end(); it++)
    {        
        line_products_interval_(col) = get_value(0,*it);
        col ++;
    }
    
    col = 0;
    for(std::list<unsigned int >::const_iterator it = vec.begin(); it != vec.end(); it++)
    {
        for (int j=1;j<nb_control_points_;j++)
        {
            line_products_interval_(col) = Hull( line_products_interval_(col), get_value(j,*it));
            
        }
        col++;
    }
}
