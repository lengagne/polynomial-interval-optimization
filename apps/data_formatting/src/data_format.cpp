#include "data_format.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <array>
#include <functional>

std::list<std::string> solver_order_;
std::list<std::string> bissect_order_;
std::list<std::string> pb_order_;

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
            add_data(line,"bissection", "Bissection : ");
            
            if (loof_for(line,"DUE TO TIME LIMIT"))
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

data_format::data_format( const data_format& f)
{
    filename_ = f.filename_;
    infos = f.infos;
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
    
    sub.erase(std::remove(sub.begin(), sub.end(), ' '), sub.end());
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
    if (test && in !=""){
        vec.push_back(in);    
//         std::cout<<"adding "<< in<<std::endl;
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
    outfile<<"% \\usepackage{longtable}"<<std::endl;
    outfile <<"\\begin{longtable}{|";
    for (int i=0;i<cs;i++)  outfile<< "c|";
    outfile <<"}\n";
    outfile<<"\\hline\n"; 
    for (int i=0;i<cs-1;i++)  outfile<< columns[i]<<" & ";
    outfile<< columns[cs-1]<<" \\\\ \\hline \n";    
    std::cout<<"datas.size() = "<< datas.size()<<std::endl;
    create_latex_subpart( outfile, 0, columns, datas);
    outfile <<"\n\\end{longtable}\n";
    outfile.close();
}

void create_latex_subpart( std::ofstream& outfile,
                           uint index,
                           std::vector<std::string> & columns,
                           const std::vector< data_format*> datas,
                           const std::string& entete)
{
    if (index >= columns.size())
    {
        return;
    }
    
//     for (int i=0;i<index;i++)   std::cout<<"\t";
//     std::cout<<"looking for "<< columns[index]<<"  in "<< datas.size() <<std::endl;
    
    std::string ref = columns[index];
    for (int i=0;i<index;i++)   std::cout<<"\t";
//     std::cout<<"ref = "<< ref <<std::endl;
    std::list<std::string> type_data;
    // get the common absciss
    for (auto& d : datas)
    {
        std::string t  = d->infos[ref];
//         std::cout<<"t = "<< t <<std::endl;
        add( t, type_data);            
        type_data.sort();            
    }
    
    if (ref == "solver")
        type_data = re_order( type_data,solver_order_);
    
    if (ref == "bissection")
        type_data = re_order( type_data,bissect_order_);
    
    if (ref == "problem")
        type_data = re_order( type_data,pb_order_);
    
    uint cpt = 0;
    for (auto& t : type_data)
    {
        std::vector< data_format*> local_data;
        for (auto& d : datas)
        {
            if ( d->infos[ref] == t)
            {
                local_data.push_back(d);
            }            
        }
       
        if (cpt)
            outfile<<entete;       
        
        if ( local_data.size() == 1)
        {
            data_format* d = local_data[0];
            for (int i=index;i<columns.size()-1;i++)
                outfile << d->infos[columns[i]]<<" & ";
            outfile <<d->infos[columns[columns.size()-1]] <<"\\\\ "; 
            outfile <<" \\cline{"<< index+1 <<"-"<< columns.size() <<"}\n";               
        }else
        {
            if (entete == "")
            {
                outfile <<" \\hline \n";   
            }
            outfile<<"\\multirow{"<<local_data.size()<<"}{*}{"<< t <<"} & ";
            create_latex_subpart ( outfile, index+1, columns, local_data, entete+" & ");
            outfile <<" \\cline{"<< index+1 <<"-"<< index+2 <<"}";   
        }
        cpt ++;
    }    
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
    
    bissect_order_.clear();
    bissect_order_.push_back("MinFirst");
    bissect_order_.push_back("MaxFirst");
    bissect_order_.push_back("Smart");
    
    for (int i=0;i<30;i++)
        pb_order_.push_back(std::to_string(i));
}

std::list<std::string> re_order(const std::list<std::string>& input, 
                                const std::list<std::string>& dic)
{
    std::list<std::string> out;
    
//     for (auto& j:input)
//         std::cout<<"in = "<< j <<"!"<<std::endl;
//     
//     for (auto& j:dic)
//         std::cout<<"dic = "<< j <<"!"<<std::endl;    
    
    for (auto& i: dic)
    {
        for (auto& j:input)
        {
//             std::cout<<"test egalite ("<< i<<"=="<< j<<")"  <<std::endl;
            if (i == j)
            {
//                 std::cout<<"egalite "<< i<< " " << j <<std::endl;
                out.push_back(i);
            }
        }
    }
    
/*    for (auto& j:out)
        std::cout<<"out = "<< j <<std::endl;   */ 
    return out;
}

double toDouble(std::string s){
    std::replace(s.begin(), s.end(), '.', ',');
    return std::atof(s.c_str());
}

