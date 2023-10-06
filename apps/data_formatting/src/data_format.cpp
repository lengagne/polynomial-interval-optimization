
#include "data_format.h"

#include <iostream>
#include <string>
#include <fstream>

data_format::data_format( const std::string& filename)
{
    filename_ = filename;
    std::string line;
    std::ifstream file;
    file.open(filename);
    
    if(file.is_open())
    {
        std::cout<<" reading "<< filename_ <<std::endl;
        while(getline(file,line))
        {
            add_data(line,"ndof", "ndof =");
            add_data(line,"problem", "npb =");
            add_data(line,"precision", "precision = ");
            add_data(line,"solver", "solver : ");            
            add_data(line,"prep_time", "preparation time : ");
            add_data(line,"nb_iter", "Number of Bissections :");
            add_data(line,"comput_time", "computation time (wo prep):");
            add_data(line,"comput_time", "computation time (wo prep):");
            add_data(line,"time_per_iter", "Time per iteration :");
            add_data(line,"total_time", "total time :");
        }        
    }else
    {
        std::cerr<<"error cannot found "<< filename <<std::endl;
    }
}

void data_format::add_data( const std::string& line, const std::string& name, const std::string& pattern)
{
    size_t pos = line.find(pattern);
    
    if (pos == std::string::npos)
        return ;
    
    std::string sub = line.substr(pos+pattern.size()) ;    
    pos  = sub.find("seconds.");
    if (pos != std::string::npos)
        sub = sub.substr( 0,pos);
    
    std::cout<<name <<" : "<< sub <<std::endl;
    infos[name] = sub;
    
    return;
}

void data_format::plot()
{
    for (auto& [a,b] : infos)
        std::cout<< a <<" : "<< b <<std::endl;
    std::cout<<std::endl;
}

void add (const std::string &in, 
          std::list<std::string> & vec)
{
    bool test = true;
    for ( auto&i : vec)
    {
        if ( in == i)
        {               
            test = false;
        }
    }
    if (test){
        vec.push_back(in);    
        std::cout<<in<<std::endl;
    }
}

void create_csv(const std::vector< data_format*> datas,
                const std::string& ordonnee,
                const std::string& abscisse,
                const std::string& parameter,
                const std::string& filetype,
                const std::string& filename)
{
    std::cout<<"create files for "<< filetype <<std::endl;
    
    std::list<std::string> type_inputs;
    
    for (auto& d : datas)
    {
        std::string t  = d->infos[filetype];
        add( t, type_inputs);
    }
    
    for ( auto& t : type_inputs)
    {
        std::vector< data_format*> local_data;
        for (auto& d : datas)   if (t ==  d->infos[filetype])
        {
            local_data.push_back(d);
        }
        
        std::cout<< t<<std::endl;
        
        std::list<std::string> type_abscisse;
        // get the common absciss
        for (auto& d : local_data)
        {
            std::string t  = d->infos[abscisse];
            add( t, type_abscisse);            
            type_abscisse.sort();            
        }
        
        std::list<std::string> type_parameter;
        // get the common absciss
        for (auto& d : local_data)
        {
            std::string t  = d->infos[parameter];
            add( t, type_parameter);            
            type_parameter.sort();            
        }        
        
        
        // create_file
        std::ofstream outfile (filename+"_"+ ordonnee + "_"+t+".csv");
        outfile<<"#"<< ordonnee<<"/"<< parameter << "\t";
        for (auto& p : type_parameter)
            outfile<<p<<"\t";
        outfile<<std::endl;

        for (auto& a : type_abscisse)
        {
            outfile << a <<"\t";
            for (auto& p : type_parameter)
            {                
                for (auto& d : local_data)
                {
                    if (d->infos[abscisse] == a && d->infos[parameter] == p)
                    {
                        d->plot();
                        std::cout<<"looking for "<< ordonnee<<std::endl;
                        if (d->infos.find(ordonnee) != d->infos.end()) {
                            std::cout<<"found "<<std::endl<<std::endl;
                            outfile<< d->infos[ordonnee]<<"\t";
                        } else {
                            std::cout<<"not found "<<std::endl<<std::endl;
                            outfile<< "?\t";
                        }
                        
                        break;
                    }
                }
            }
            outfile<< std::endl;;
        }
    }
    
    
        
    
}

