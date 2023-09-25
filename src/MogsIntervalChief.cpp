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
//    std::cout<<std::endl;
//    std::cout<<"adding intermediate  " << intermediate_.size() <<std::endl;
//    std::cout<<"inter.guess_size() = "<< inter.guess_size() <<std::endl;
//    for (int i=0;i<intermediate_.size();i++)
//        if(intermediate_[i] == inter)
//            return intermediate_in_[i];

    std::map<unsigned long long , MogsInterval>::const_iterator it = memory_.find(inter.id_);
    if(it != memory_.end())
    {
//        std::cout<<"return the save one"<<std::endl;
        return it->second;
    }
//    std::cout<<"create a new one "<<std::endl;

    if (inter.rely_on_error())
    {
//        std::cout<<"create with seperated error" <<std::endl;
        MogsInterval* no_error = new MogsInterval();
        MogsInterval* error = new MogsInterval();
        for(std::map<mem*,LazyVariable>::const_iterator itmem = inter.dependances_.begin(); itmem != inter.dependances_.end(); itmem++)
            if( !(itmem->first)->contains_error())
            {
//                std::cout<<"*(itmem->first) does not rely on error : "<< *(itmem->first)<<std::endl;
                no_error->dependances_[itmem->first] = itmem->second;
            }


        for(std::map<mem*,LazyVariable>::const_iterator itmem = inter.dependances_.begin(); itmem != inter.dependances_.end(); itmem++)
            if( (itmem->first)->contains_error())
                error->dependances_[itmem->first] = itmem->second;

////        std::cout<<"*no_error = "<< *no_error <<std::endl;
//        std::cout<<"*error = "<< *error <<std::endl;

        error->id_intermediate_ = intermediate_.size();
        intermediate_.push_back(*error);

//        MogsInterval* no_error_to_init = new MogsInterval();
        MogsInterval* error_to_init = new MogsInterval();


//        no_error_to_init->init(Interval(), "NOERRIntermediate_variable_"+std::to_string(intermediate_.size()));
        error_to_init->is_error();
        error_to_init->init(Interval(), "ERRIntermediate_variable_"+std::to_string(intermediate_.size()));
        error_to_init->ref_->is_an_intermediare = true;
        error_to_init->ref_->is_error();

//        no_error_to_init->level_ = inter.level_ + 1;
        error_to_init->level_ = inter.level_ + 1;
        error_to_init->ref_->id_intermediate_ = intermediate_.size();
//        intermediate_.push_back(inter);
        intermediate_in_.push_back(*error_to_init);
        if(no_error->guess_size() > M/2)
        {
            MogsInterval out = add_intermediate(*no_error)+   (*error_to_init);
            return out;
        }

        // MogsInterval sum = (*no_error_to_init) + (*error_to_init);
        MogsInterval sum = (*no_error) + (*error_to_init);
        delete no_error;
//        std::cout<<" sum "<<sum <<std::endl;
//        memory_[&inter] = sum;
        memory_[inter.id_] = sum;
//        intermediate_in_.push_back(*no_error_to_init);
//        std::cout<<"sum.guess_size() = "<< sum.guess_size()<<std::endl;
        return sum;
    }

    // no error;
//    std::cout<<"no error add "<< inter<<std::endl;


    MogsInterval new_inter;
    new_inter.is_an_intermediare = true;
    new_inter.init(Interval(-2.), "Intermediate_variable_"+std::to_string(intermediate_.size()));
    new_inter.ref_->is_an_intermediare = true;
    new_inter.ref_->id_intermediate_ = intermediate_.size();
    new_inter.level_ = inter.level_ + 1;
    memory_[inter.id_] = new_inter;
    intermediate_.push_back(inter);
//    memory_[inter] = new_inter;
//    std::cout<<"stoking "<< new_inter<<std::endl;
    new_inter.is_an_intermediare = true;
    new_inter.id_intermediate_ = intermediate_.size();
    intermediate_in_.push_back(new_inter);
//    std::cout<<"new_inter.guess_size() = "<< new_inter.guess_size()<<std::endl;
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

unsigned int MogsIntervalChief::get_max_level()
{
    unsigned int v = 0;
    for (int i=0;i<intermediate_in_.size();i++)
        if(intermediate_in_[i].level_ > v )
            v = intermediate_in_[i].level_;
    return v;
}

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
}
