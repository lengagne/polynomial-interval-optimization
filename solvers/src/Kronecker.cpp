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

double Kronecker::get_pos(uint id, uint p) const
{
//     std::cout<<std::endl;
//     std::cout<<"id = "<< id <<std::endl;
//     std::cout<<"p = "<< p <<std::endl;
//     std::cout<<"pos_max_.size() = "<< pos_max_.size() <<std::endl;
//     std::cout<<"pos_max_[id].size() = "<< pos_max_[id].size() <<std::endl;
//     std::cout<<"offsets_[id] = "<< offsets_[id] <<std::endl;
//     std::cout<<"size_matrices_[id] = "<< size_matrices_[id] <<std::endl;
//     std::cout<<"it = "<< (p/offsets_[id])%size_matrices_[id] <<std::endl;
    return  pos_max_[id]((p/offsets_[id])%size_matrices_[id]);
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

void Kronecker::help_bissection(uint id,Result& res, bool inf_sup) const
{
//     std::cout<<"update the res infos "<< id <<std::endl;
    if (!res.info_defined)
    {
        res.bissect_weight.resize(nb_basis_);
        res.inf_sup_proba.resize(nb_basis_);
        for (int i=0;i<nb_basis_;i++)
        {
            res.bissect_weight[i] =  1;
            res.inf_sup_proba[i] = 0.0;
        }
        res.nb_info = 0;
    }
    
    
    res.nb_info++;
    
    for (int i=0;i<nb_basis_;i++)
    {
        res.bissect_weight[i] *= 1+0.2*(1.0*(id%offsets_[i]))/(1.0*offsets_[i]);
        res.inf_sup_proba[i] += pos_inf_[id][i] ^ inf_sup;
    }
    
//     for (int i=0;i<nb_basis_;i++)
//         std::cout<<"weight["<<i<<"] = "<< 1+0.5*(1.0*( offsets_[i] - id%offsets_[i]))/(1.0*offsets_[i]) <<std::endl;    
    
//     for (int i=0;i<nb_basis_;i++)
//         std::cout<<"vec["<<i<<"] = "<< pos_inf_[id][i] <<std::endl;    
//     for (int i=0;i<nb_basis_;i++)
//         std::cout<<"res.inf_sup_proba["<<i<<"] = "<< res.inf_sup_proba[i] <<std::endl;        
//     std::cout<<"res.nb_info = "<< res.nb_info <<std::endl;
//     std::cout <<std::endl;
    
    res.info_defined = true;
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

void Kronecker::set_maximum_position(std::vector< Eigen::Matrix<double,Eigen::Dynamic,1> >& max)
{
    pos_max_ = max;
//     for (int i=0;i<pos_max_.size();i++)
//         std::cout<<"pos_max_("<<i<<") = "<< pos_max_[i].transpose()<<std::endl;
    
    pos_.resize(nb_control_points_);   
    pos_inf_.clear();
    for (int i=0;i<nb_control_points_;i++)
    {
        std::vector<bool> tmp_inf;
        pos_[i].resize(nb_basis_);
        for (int j=0;j<nb_basis_;j++)
        {
            pos_[i](j) = get_pos(j,i);
//             tmp_inf.push_back( pos_[i](j) <= 0);
            tmp_inf.push_back(false);
        }
        pos_inf_.push_back(tmp_inf);
    }
}
