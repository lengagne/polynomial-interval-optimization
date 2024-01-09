#include "data_format3D.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <array>
#include <functional>

// std::list<std::string> solver_order_;
// std::list<std::string> bissect_order_;
// std::list<std::string> pb_order_;

data_format3D::data_format3D( const std::string& filename)
{
    filename_ = filename;
    std::string line;
    std::ifstream file;
    file.open(filename);
    infos["filename"] = filename;
    if(file.is_open())
    {
//         std::cout<<" reading "<< filename_ <<std::endl;
        while(getline(file,line))
        {
            add_data(line,"problem", "problem =");
            add_data(line,"precision", "precision = ");
            add_data(line,"solver", "solver : ");            
            add_data(line,"prep_time", "preparation time : ");
            add_data(line,"nb_iter", "Number of Bissections :");
            add_data(line,"comput_time", "computation time (wo prep):");
            add_data(line,"time_per_iter", "Time per iteration :");
            add_data(line,"total_time", "total time :");
            add_data(line,"(D-H:M:S.ms)", "total time :");
            add_data(line,"type", "Bissection :");
            add_data(line,"criteria", "crit = ");
            add_data(line,"bissection_mode", "bissection_mode =");
            
            if (loof_for(line,"DUE TO TIME LIMIT"))
            {
                time_out_ = true;      
                infos["prep_time"] = "TIMEOUT";
                infos["nb_iter"] = "TIMEOUT";
                infos["comput_time"] = "TIMEOUT";
                infos["time_per_iter"] = "TIMEOUT";
                infos["total_time"] = "TIMEOUT";                   
                std::cout<<"TIMEOUT: ";
            }
            if (loof_for(line,"CANCELLED AT"))
            {
                time_out_ = true;                
                infos["prep_time"] = "CANCELLED";
                infos["nb_iter"] = "CANCELLED";
                infos["comput_time"] = "CANCELLED";
                infos["time_per_iter"] = "CANCELLED";
                infos["total_time"] = "CANCELLED";      
                std::cout<<"CANCELLED: ";
            }         
            
            if (loof_for(line,"Cannot load the library, stopping program"))
            {
                time_out_ = true;                
                infos["prep_time"] = "CANNOT_LOAD_LIBRARY";
                infos["nb_iter"] = "CANNOT_LOAD_LIBRARY";
                infos["comput_time"] = "CANNOT_LOAD_LIBRARY";
                infos["time_per_iter"] = "CANNOT_LOAD_LIBRARY";
                infos["total_time"] = "CANNOT_LOAD_LIBRARY";      
                std::cout<<"CANNOT_LOAD_LIBRARY: ";                
            }
            
        }        
    }else
    {
        std::cerr<<"error cannot found "<< filename <<std::endl;
    }   		
    
    if (time_out_)
    {
        std::cout<<"Problem "<<  infos["problem"]<< "  solver = "<< infos["solver"] <<" Bissection = "<< infos["bissection_mode"] <<std::endl;
        
        std::cout<<"rm -frv "<< filename<<std::endl;
//         std::cout<<"sbatch job.sh "<< infos["ndof"]<<" "<< infos["problem"]<<" " << infos["precision"]<<" " << infos["bissection"]<<" "<<infos["solver"]<<std::endl;
        std::cout<<"sbatch job3D_long.sh "<< infos["precision"]<<" "<< infos["problem"]<<" " << get_bissection(infos["bissection_mode"]) <<" "<< get_solver(infos["solver"])<<std::endl;        
        std::cout<<std::endl;     
        infos["criteria"] = filename;
    }
}
