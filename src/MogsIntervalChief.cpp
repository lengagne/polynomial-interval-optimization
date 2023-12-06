#include "MogsIntervalChief.h"

MogsIntervalChief::MogsIntervalChief()
{
    //ctor
    mem tmp;
//    monomials_.push_back(*double_value_);
    double_value_ = check_input(&tmp);   // allows to define a double
    intermediate_.clear();
}

MogsIntervalChief::~MogsIntervalChief()
{
    //dtor
}

MogsInterval * MogsIntervalChief::add_intermediate_pointer( const MogsInterval& input)
{
    intermediate_.push_back(input);
    MogsInterval tmp;
    tmp.is_an_intermediare = true;
    tmp.init(Hull(0,0),"Intermediare_pointeur");
    intermediate_in_.push_back(tmp);
    MogsInterval * out = & (intermediate_in_[intermediate_in_.size()-1]);
    return out;
}

MogsInterval MogsIntervalChief::add_intermediate(const MogsInterval& inter, unsigned int M)
{
    std::map<unsigned long long , MogsInterval>::const_iterator it = memory_.find(inter.id_);
    if(it != memory_.end())
    {
//        std::cout<<"return the save one"<<std::endl;
        return it->second;
    }
    MogsInterval new_inter;
    new_inter.is_an_intermediare = true;
    new_inter.init(Interval(-2.), "Intermediate_variable_"+std::to_string(intermediate_.size()));
    new_inter.ref_->is_an_intermediare = true;
    new_inter.ref_->id_intermediate_ = intermediate_.size();
    memory_[inter.id_] = new_inter;
    intermediate_.push_back(inter);
    new_inter.is_an_intermediare = true;
    new_inter.id_intermediate_ = intermediate_.size();
    intermediate_in_.push_back(new_inter);
    return new_inter;
}


mem* MogsIntervalChief::check_input(mem* in)
{
    std::list<mem >::iterator it = std::find(monomials_.begin(), monomials_.end(), *in);
    if(it == monomials_.end())
    {
        monomials_.push_back(*in);
        return & (*std::find(monomials_.begin(), monomials_.end(), *in));
    }
    else
    {
        delete in;
        return &(*it);
    }
}
/*
unsigned int MogsIntervalChief::get_max_level()
{
    unsigned int v = 0;
    for (int i=0;i<intermediate_in_.size();i++)
        if(intermediate_in_[i].level_ > v )
            v = intermediate_in_[i].level_;
    return v;
}*/

mem* MogsIntervalChief::get_mem(const mem& in)
{
    std::list<mem >::iterator it = std::find(monomials_.begin(), monomials_.end(), in);
    if(it == monomials_.end())
    {
        return 0;
    }
    else
    {
        return &(*it);
    }
}

void MogsIntervalChief::reset()
{
    intermediate_.clear();
    intermediate_in_.clear();
    memory_.clear();
    monomials_.clear();
    
    mem tmp;
//    monomials_.push_back(*double_value_);
    double_value_ = check_input(&tmp);   // allows to define a double    
}
