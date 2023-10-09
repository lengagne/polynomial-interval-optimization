
#include "data_format.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <array>
#include <functional>

std::list<std::string> solver_order_;

data_format::data_format( const std::string& filename)
{
    filename_ = filename;
    std::string line;
    std::ifstream file;
    file.open(filename);
    
    if(file.is_open())
    {
//         std::cout<<" reading "<< filename_ <<std::endl;
        while(getline(file,line))
        {
            add_data(line,"ndof", "ndof =");
            add_data(line,"problem", "npb =");
            add_data(line,"precision", "precision = ");
            add_data(line,"solver", "solver : ");            
            add_data(line,"prep_time", "preparation time : ");
            add_data(line,"nb_iter", "Number of Bissections :");
            add_data(line,"comput_time", "computation time (wo prep):");
            add_data(line,"time_per_iter", "Time per iteration :");
            add_data(line,"total_time", "total time :");
            
            if (loof_for(line,"DUE TO TIME LIMIT"))
            {
                time_out_ = true;
                std::cout<<filename<<" : TIMEOUT"<<std::endl;
            }
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
    
//     std::cout<<name <<" : "<< sub <<std::endl;
    infos[name] = sub;
    
    return;
}

bool data_format::loof_for(const std::string& line, const std::string& pattern)
{
    size_t pos = line.find(pattern);
    
    if (pos == std::string::npos)
        return false;
    return true;
}

void data_format::plot()
{
    std::cout<<"file : "<< filename_<<std::endl;
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
//         std::cout<<in<<std::endl;
    }
}

void create_csv(const std::vector< data_format*> datas,
                const std::string& ordonnee,
                const std::string& abscisse,
                const std::string& parameter,
                const std::string& filetype,
                const std::string& filename,
                bool ratio
               )
{
//     std::cout<<"create files for "<< filetype <<std::endl;
    
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
        
//         std::cout<< t<<std::endl;
        
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
            
            if ( parameter != "solver")
            {
                type_parameter.sort();            
            }else
            {
                type_parameter = solver_order_;
            }
        }        
        
        
        // create_file
        std::string complete_filename ="";
        complete_filename += filename+"_"+ ordonnee + "_"+t;
        if (ratio)
            complete_filename +="_ratio";
        complete_filename += ".csv";
               
        std::ofstream outfile (complete_filename);
        outfile<<"#"<< ordonnee<<"/"<< parameter;
        if (ratio)  outfile<<"%";
        outfile << "\t";
        for (auto& p : type_parameter)
            outfile<<p<<"\t";
        outfile<<std::endl;

        for (auto& a : type_abscisse)
        {
            outfile << a <<"\t";
            for (auto& p : type_parameter)
            {                
                double coeff = 1.0;
                if (ratio)
                {
                    std::string ref = type_parameter.front();
                    for (auto& d : local_data)
                    {
                        if (d->infos[abscisse] == a && d->infos[parameter] == ref)
                        {
                            
                            if (d->infos.find(ordonnee) != d->infos.end()) {
                                coeff = toDouble ( d->infos[ordonnee]) ;
                            }                            
                        }
//                         if (d->infos.find(ordonnee) != d->infos.end()) {
//                             outfile<< d->infos[ordonnee]<<"\t";
//                         } else {
//                             outfile<< "?\t";
//                         }                        
                    }
                }
                
                
                
                for (auto& d : local_data)
                {
                    if (d->infos[abscisse] == a && d->infos[parameter] == p)
                    {
                        if (d->infos.find(ordonnee) != d->infos.end()) {
                            if (ratio)
                            {
                                outfile<< toDouble(d->infos[ordonnee])/coeff * 100 <<"\t";
                            }else
                            {
                                outfile<< d->infos[ordonnee]<<"\t";
                            }
                        } else {
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

void create_latex( const std::vector< data_format*> datas,
                   const std::string filename,
                   std::vector<std::string> & columns)
{
    std::cout<<"CREATE LATEX With "<< filename <<std::endl;
    std::ofstream outfile (filename+".tex");
    
    std::cout<<"On a "<< columns.size() <<" colonnes "<<std::endl;
    uint cs = columns.size();
    
    outfile <<"\\begin{tabular}{|";
    for (int i=0;i<cs;i++)  outfile<< columns[i]<<"|";
    outfile <<"}\n";
    
    
    
    
    outfile <<"\\end tabular\n";
    
    
    outfile.close();
    
}

void init_order()
{
    solver_order_.clear();
    solver_order_.push_back("BissectionInterval");
    solver_order_.push_back("BissectionBasis_Bernstein");
    solver_order_.push_back("BissectionBasis_MinVariance");
    solver_order_.push_back("BissectionBasis_BSplines");
    solver_order_.push_back("BissectionBasis_MinNo");
    solver_order_.push_back("BissectionBasis_ApproxMinVo");    
    solver_order_.push_back("BissectionBasis_MinVo");
    solver_order_.push_back("BissectionBasis_RecursiveBSplines");
    solver_order_.push_back("BissectionBasis_RecursiveBSplines2");
    solver_order_.push_back("ContractionInterval");
    solver_order_.push_back("ContractionBasis_Bernstein");
    solver_order_.push_back("ContractionBasis_MinVariance");
    solver_order_.push_back("ContractionBasis_MinNo");
    solver_order_.push_back("ContractionBasis_ApproxMinVo");
    solver_order_.push_back("ContractionBasis_MinVo");
    solver_order_.push_back("ContractionBasis_RecursiveBSplines");
    solver_order_.push_back("ContractionBasis_RecursiveBSplines2");        
}

double toDouble(std::string s){
    std::replace(s.begin(), s.end(), '.', ',');
    return std::atof(s.c_str());
}

