#include "AbstractSolver.h"
#include <string.h>



bool AbstractSolver::check_size(   const Result& in)
{
    double w = 0.;
    for (unsigned int i=0;i<nb_var_;i++)
    {
        double t = Diam(in.in[i]);
        if (t>w)
        {
            w = t;
        }
    }

    if (w < precision_)
        return false;
    return true;
}

bool AbstractSolver::bissect( const Result& in,
                                        std::vector<Result>& Pile)
{
//     std::cout<<"on bissect"<<std::endl;
    // check precision
    Result out1 = in;
    Result out2 = in;
    
    int id = -1;
    double w = 0.;
    
//     for (unsigned int i=0;i<nb_var_;i++)
//         std::cout<<"inf_sup_proba("<<i<<") = "<< in.inf_sup_proba[i]/in.nb_info <<std::endl;
//     
//     for (unsigned int i=0;i<nb_var_;i++)
//         std::cout<<"bissect_weight("<<i<<") = "<< in.bissect_weight[i] <<std::endl;
    
    for (unsigned int i=0;i<nb_var_;i++)
    {
        double t = Diam(in.in[i]);
        if (bissection_type_ ==2)
        {
//             std::cout<<"weight("<<i<<") = "<< in.bissect_weight[i] <<std::endl;
//             std::cout<<"in.nb_info = "<< in.nb_info <<std::endl;
//             std::cout<<"inf_sup_proba("<<i<<") = "<< in.inf_sup_proba[i]/in.nb_info <<std::endl;
//             std::cout<<"bissect_weight("<<i<<") = "<< in.bissect_weight[i] <<std::endl;
            t*= in.bissect_weight[i]; 
//             t*= fabs(in.inf_sup_proba[i])/in.nb_info; 
        }
        if (Diam(in.in[i]) > precision_ && t >w)
        {
            w = t;
            id = i;
        }
    }      
    if (id == -1)
        return false;
//     std::cout<<"we will cut "<< id ;
       
    switch (bissection_type_)
    {
        case(0):
//             std::cout<<"  First inferior part"<<std::endl;
            out2.in[id] = Hull( Inf(in.in[id]), Mid(in.in[id]));
            out1.in[id] = Hull( Mid(in.in[id]), Sup(in.in[id]));
            break;
            
        case(1):
            out2.in[id] = Hull( Mid(in.in[id]), Sup(in.in[id]));
            out1.in[id] = Hull( Inf(in.in[id]), Mid(in.in[id]));
            break;
            
//         case(2):   
// 
//             if(in.inf_sup_proba[id] /in.nb_info <= 0.0 )
//             {
// //                 std::cout<<"  First inferior part"<<std::endl<<std::endl;
//                 out1.in[id] = Hull( Inf(in.in[id]), Mid(in.in[id]));
//                 out2.in[id] = Hull( Mid(in.in[id]), Sup(in.in[id]));                
//             }else
//             {
// //                 std::cout<<"  First superior part"<<std::endl<<std::endl;
//                 out1.in[id] = Hull( Mid(in.in[id]), Sup(in.in[id]));
//                 out2.in[id] = Hull( Inf(in.in[id]), Mid(in.in[id]));                
//             }
//             break;
        default:
                std::cerr<<"File : "<< __FILE__<<" at line "<< __LINE__ <<std::endl;
                std::cerr<<"Case not planned for the moment "<<std::endl;
                exit(123);
    }     
    
    Pile.push_back(out1);
    Pile.push_back(out2);
    
    return true;
}


void AbstractSolver::close_files()
{
    if (file_open)
    {
        file_safe_.close();
        file_safe_.close();
        if(print_)
        {
            // dummy to plot
            file_gnuplot_input_<<"set xrange [ "<< Inf(bounds_input_[0]) <<":"<<Sup(bounds_input_[0])<<"] "<<std::endl;
            if( bounds_input_.size()>1)
                file_gnuplot_input_<<"set yrange [ "<< Inf(bounds_input_[1]) <<":"<<Sup(bounds_input_[1])<<"] "<<std::endl;
            file_gnuplot_input_<<"plot x= 0,-500+0.1*x notitle lt 3 lw 3"<<std::endl;

            if( bounds_.size()>0)
                file_gnuplot_output_<<"set xrange [ "<< Inf(bounds_[0]) <<":"<<Sup(bounds_[0])<<"] "<<std::endl;
            if( bounds_.size()>1)
                file_gnuplot_output_<<"set yrange [ "<< Inf(bounds_[1]) <<":"<<Sup(bounds_[1])<<"] "<<std::endl;
            file_gnuplot_output_<<"plot x= 0,-500+0.1*x notitle lt 3 lw 3"<<std::endl;

            file_gnuplot_input_.close();
            file_gnuplot_output_.close();
            std::string cmd1 = "gnuplot "+file_prefix_+"input.gp";
            std::string cmd2 = "gnuplot "+file_prefix_+"output.gp";
    //        std::cout<<"CREATING FILES"<<std::endl;
            int dummy = system(cmd1.c_str());
            std::cout<<"If you want to get the png for the output please type :" <<std::endl;
            std::cout<<cmd2<<std::endl;
        }
        file_open = false;
    }
}

void AbstractSolver::prepare_files(const std::string& filename)
{
    file_prefix_ = filename;
    file_safe_.open(filename+"safe.txt");
    file_maybe_.open(filename+"maybe.txt");
    if(print_)
    {
        std::string cmd1 = "rm "+file_prefix_+"input.gp "+file_prefix_+"output.gp "+ filename+"safe.txt " + filename+"maybe.txt";
        int dummy = system(cmd1.c_str());
        file_gnuplot_input_.open(filename+"input.gp");
        file_gnuplot_output_.open(filename+"output.gp");
        file_gnuplot_input_<<"set terminal png  fontscale 1.0 size 1200, 800 "<<std::endl;
        file_gnuplot_input_<<"set output '"<< filename+"input.png" <<"'"<<std::endl;
        file_gnuplot_input_<<"# set terminal postscript enhanced color fontscale 1.0 "<<std::endl;
        file_gnuplot_input_<<"# set output '"<< filename+"input.eps" <<"'"<<std::endl;
        file_gnuplot_input_<<"set grid"<<std::endl;
        file_gnuplot_output_<<"set terminal png  fontscale 1.0 size 1200, 800 "<<std::endl;
        file_gnuplot_output_<<"set output '"<< filename+"output.png" <<"'"<<std::endl;
        file_gnuplot_output_<<"# set terminal postscript enhanced color fontscale 1.0 "<<std::endl;
        file_gnuplot_output_<<"# set output '"<< filename+"input.eps" <<"'"<<std::endl;
        file_gnuplot_output_<<"set grid"<<std::endl;
        cpt_rec_input_ = 1;
        cpt_rec_output_ = 1;
    }
    file_open = true;
}

check_constraint AbstractSolver::test_Interval( const Interval &in ,
                                        const Interval & bound)
{
    if ( Inf(in) >= Inf(bound) && Sup(in) <= Sup(bound))
        return INSIDE;
    if ( Inf(in) > Sup(bound) ||  Sup(in) < Inf(bound))
        return OUTSIDE;
    return OVERLAP;
}
