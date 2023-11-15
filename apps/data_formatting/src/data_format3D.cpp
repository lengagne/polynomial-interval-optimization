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
            add_data(line,"bissection", "Bissection :");
            add_data(line,"criteria", "crit = ");
            
            if (loof_for(line,"DUE TO TIME LIMIT"))
            {
                time_out_ = true;                
            }
            if (loof_for(line,"CANCELLED AT"))
            {
                time_out_ = true;                
            }            
        }        
    }else
    {
        std::cerr<<"error cannot found "<< filename <<std::endl;
    }   		
    
    if (time_out_)
    {
        std::cout<<"rm -frv "<< filename<<std::endl;
//         std::cout<<"sbatch job.sh "<< infos["ndof"]<<" "<< infos["problem"]<<" " << infos["precision"]<<" " << infos["bissection"]<<" "<<infos["solver"]<<std::endl;
    }
}
