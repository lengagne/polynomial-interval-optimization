#include "data_format.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <array>
#include <functional>
#include <regex>

std::list<std::string> solver_order_;
std::list<std::string> bissect_order_;
std::list<std::string> pb_order_;

data_format::data_format( const std::string& filename)
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
            add_data(line,"ndof", "ndof =");
            add_data(line,"problem", "npb =");
            add_data(line,"precision", "precision = ");
            add_data(line,"solver", "solver : ");            
            add_data(line,"prep_time", "preparation time : ");
            add_data(line,"nb_iter", "Number of Bissections :");
            add_data(line,"comput_time", "computation time (wo prep):");
            add_data(line,"time_per_iter", "Time per iteration :");
            add_data(line,"total_time", "total time :");
            add_data(line,"(D-H:M:S.ms)", "total time :");
            add_data(line,"type", "Bissection : ");
            add_data(line,"criteria", "crit = ");            
            
            if (loof_for(line,"DUE TO TIME LIMIT"))
            {
                time_out_ = true;      
                infos["prep_time"] = "TIMEOUT";
                infos["nb_iter"] = "TIMEOUT";
                infos["comput_time"] = "TIMEOUT";
                infos["time_per_iter"] = "TIMEOUT";
                infos["total_time"] = "TIMEOUT";                   
            }
            if (loof_for(line,"CANCELLED AT"))
            {
                time_out_ = true;                
                infos["prep_time"] = "CANCELLED";
                infos["nb_iter"] = "CANCELLED";
                infos["comput_time"] = "CANCELLED";
                infos["time_per_iter"] = "CANCELLED";
                infos["total_time"] = "CANCELLED";                   
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
        std::cout<<"rm -frv "<< filename<<std::endl;
        std::cout<<"sbatch job_long.sh "<< infos["ndof"]<<" "<< infos["problem"]<<" " << infos["precision"]<<" " << get_bissection(infos["bissection"]) <<" "<< get_solver(infos["solver"])<<std::endl;
        infos["criteria"] = filename;
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
    
    if (name == "(D-H:M:S.ms)")
    {
        infos[name] = time_format( sub);
    }else
    {
        infos[name] = sub;
    }
    
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

void create_latex( std::ofstream& outfile,
                   const std::vector< data_format*> datas,
                   std::vector<std::string> & columns,
                   const std::string &titre
                 )
{
    
//     std::cout<<"create table"<<std::endl;
//     std::cout<<"On a "<< columns.size() <<" colonnes "<<std::endl;
    uint cs = columns.size();
//     outfile <<"\\begin{longtable}{|";
    
    
    outfile <<"\\begin{table}\n \\tiny \n \\begin{tabular}{|";
    
    for (int i=0;i<cs;i++)  outfile<< "c|";
    outfile <<"}\n";
    outfile<<"\\hline\n"; 
    
//     for (int i=0;i<cs-1;i++)  outfile<< replace(columns[i])<<" & ";
//     outfile<< replace(columns[cs-1])<<" \\\\ \\hline  \\endfirsthead \\hline \n";    

//     for (int i=0;i<cs-1;i++)  outfile<< replace(columns[i])<<" & ";
//     outfile<< replace(columns[cs-1])<<" \\\\ \\hline  \\endhead \\hline \n";        
    
        for (int i=0;i<cs-1;i++)  outfile<< replace(columns[i])<<" & ";
    outfile<< replace(columns[cs-1])<<" \\\\ \\hline  \\hline \n";        
    
//     std::cout<<"datas.size() = "<< datas.size()<<std::endl;
    create_latex_subpart( outfile, 0, columns, datas);
    outfile <<"\\end{tabular}\n";
    if (titre != "")
    {
        outfile<<"\n \\caption\{"<< titre<<"\}"<<std::endl;
    }
    outfile <<"\\end{table}\n\n";
//     outfile <<"\\normalsize \n";
//     outfile <<"\\end{longtable}\n\n";
//     std::cout<<"end of table"<<std::endl;
    
}

void create_latex( const std::vector< data_format*> datas,
                   const std::string filename,
                   std::vector<std::string> & columns,
                   std::vector<std::string> & common,
                   std::vector<std::string> & remove,
                   const std::string main_title
                 )
{
    std::cout<<"CREATE LATEX With "<< filename <<std::endl;
    std::vector< std::vector< std::string > > differences;
    for (auto& d : datas)
    {
        bool add = true;
        // on vérifie les remove
        for (int i=0;i<remove.size();i++)
        {
            for (auto& info : d->infos)
            {
//                 std::cout<<"info : "<< info.first<<" -- " << info.second <<std::endl;
                if (info.first.find(remove[i]) != std::string::npos ||  info.second.find(remove[i]) != std::string::npos)
                {
//                     std::cout<<"on ignore "<< info.first<<" -- " << info.second <<" due to "<< remove[i] <<std::endl;
                    add = false;
                    break;
                }
            }
        }
        
        if (add)
        {
            std::vector<std::string> local_diff;
            for (int i=0;i<common.size();i++)
            {
                local_diff.push_back( d->infos[common[i]]);
            }
            
            
            for (int i=0;i<differences.size();i++)
            {
                bool mem = true;
                for (int j=0;j<common.size();j++)
                {
                    if ( local_diff[j] != differences[i][j])
                        mem = false;
                }
                
                if (mem)
                {
                    add = false;
                }
            }
            if (add)
                differences.push_back(local_diff);
        }
    }
    
//     std::cout<<"on a va générer "<< differences.size()<<" tableaux pour "<<std::endl;
    std::ofstream outfile (filename+".tex");
    outfile<<"% \\usepackage{longtable}"<<std::endl;
    
    for (int i=0;i<differences.size();i++)
    {
//         for (int j=0;j<common.size();j++)
//         {
//             std::cout<<"\t"<< common[j]<<":"<< differences[i][j];
//         }
//         std::cout<<std::endl;
        
        
        // on trie
        std::vector< data_format*> local_datas;
        for (auto& d : datas)
        {
            bool test = true;
            for (int j=0;j<common.size();j++)
            {
                if (differences[i][j] != d->infos[common[j]])
                {
                    test = false;
                    break;
                }
            }
            if (test)
                local_datas.push_back(d);
        }
        
        std::string caption = main_title;
        for (int j=0;j<common.size();j++)
        {
            caption += common[j] + ": " + differences[i][j];
            if (j != common.size()-1)
                caption += ", ";
        }
        
        create_latex( outfile, local_datas, columns, caption);
    }
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
            outfile<<replace(entete);
        
        if ( local_data.size() == 1)
        {
            data_format* d = local_data[0];
            for (int i=index;i<columns.size()-1;i++)
                outfile << replace(d->infos[columns[i]])<<" & ";
            outfile << replace(d->infos[columns[columns.size()-1]]) <<"\\\\ "; 
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

std::string get_bissection( const std::string in)
{
    if (in == "MinFirst")
        return std::string("0");
    if (in == "MaxFirst")
        return std::string("1");
    return in;
}

std::string get_solver( const std::string in)
{
    uint cpt = 0;
    for (auto& s : solver_order_)
//     for (int i=0;i<solver_order_.size();i++)
    {
        if (in == s)
            return std::to_string(cpt);
        cpt++;
    }
    return in;
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
    solver_order_.push_back("ContractionBasis_BSplines");
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

std::string replace (const std::string &in)
{
    
//     out =  std::regex_replace(out, std::regex("Bissection"), "Bis_");
//     out =  std::regex_replace(out, std::regex("BissectionBasis"), "Bis");
//     out =  std::regex_replace(out, std::regex("ContractionBasis"), "Cont");
//     out =  std::regex_replace(out, std::regex("Contraction"), "Cont_");    
    
    std::string out =  std::regex_replace(in, std::regex("BissectionBasis_"), "");
    out =  std::regex_replace(out, std::regex("Bissection"), "");
    out =  std::regex_replace(out, std::regex("ContractionBasis_"), "");
    out =  std::regex_replace(out, std::regex("Contraction"), "");
    
    out =  std::regex_replace(out, std::regex("RecursiveBSplines"), "Recursive");
    
    out =  std::regex_replace(out, std::regex(","), ".");
    out =  std::regex_replace(out, std::regex("_"), "\\textunderscore ");
    
    return out;
}

std::string time_format( const std::string & in)
{
    // Convertir la chaîne en un entier
    long tempsEnSecondes = std::stol(in);
    double temps = toDouble(in);
//     std::cout<<"temps = "<< temps <<std::endl;

    // Calculer le nombre de jours, heures, minutes et secondes
    int jours = tempsEnSecondes / (24 * 3600);
    tempsEnSecondes = tempsEnSecondes % (24 * 3600);
    int heures = tempsEnSecondes / 3600;
    tempsEnSecondes %= 3600;
    int minutes = tempsEnSecondes / 60;
    tempsEnSecondes %= 60;
    
    double secondes = temps - jours*24*3600 - heures * 3600 - minutes * 60;
//     int secondes = tempsEnSecondes;

    // Construire la chaîne résultante
    std::string dureeStr;
    char str[200];
    if (jours)
    {
        dureeStr = std::to_string(jours) + "-" +
                           std::to_string(heures) + ":" +
                           std::to_string(minutes) + ":" +
                           std::to_string((int)secondes);
        
        sprintf(str,"%d-%02d:%02d:%02d", jours, heures,minutes,(int)secondes);
                           
    }else
    if (heures)
    {
        dureeStr =  std::to_string(heures) + ":" +
                    std::to_string(minutes) + ":" +
                    std::to_string((int)secondes);        
        sprintf(str,"%02d:%02d:%02d",  heures,minutes,(int)secondes);
    }else 
    if (minutes)
    {
        dureeStr =  std::to_string(minutes) + ":" +
                    std::to_string((int)secondes);        
        sprintf(str,"%02d:%02d",  minutes,(int)secondes);
    }else
    {
        long ms = secondes * 1000;
        dureeStr =  std::to_string(ms/1000.0);    
        sprintf(str,"%.3f",  secondes);
    }
        
//     printf("Formaté = %s\n", str);
    dureeStr = std::string(str);
   

    return replace(dureeStr);    
}
