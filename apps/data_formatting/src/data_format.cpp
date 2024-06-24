#include "data_format.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <array>
#include <functional>
#include <regex>
#include <iomanip>
#include <locale.h>
#include <cmath>

std::list<std::string> solver_order_;
std::list<std::string> bissect_order_;
std::list<std::string> pb_order_;

bool is_double(const std::string& s)
{
    std::string ss =  std::regex_replace(s, std::regex(","), ".");
    std::istringstream iss(ss);
    double d;
    return iss >> d >> std::ws && iss.eof();
}

std::ostream& scientificFormat(std::ostream& os, double value) {
    // Calcul de l'exposant pour qu'il soit un multiple de 3
    int exponent = static_cast<int>(std::floor(std::log10(std::abs(value)) / 3) * 3);
    
    exponent = (exponent/3) * 3;
    
    double mantisse = value * std::pow(10, -exponent);
    
    int precision = 3;
    if (abs(mantisse) > 100)
        precision = 1;
    else if (abs(mantisse) > 10)
        precision = 2;    

    os << std::fixed << std::setprecision(precision) ;
    os << mantisse;
    
    // Configuration de l'affichage en format scientifique avec 1 chiffre avant la virgule
//     os << std::scientific ;
    if (exponent !=0)
    {
        os<<"e"<< exponent;
    }
    
    os << std::scientific<< std::setprecision(3) ;
    return os;
}


std::ostream& scientificFormat(std::ostream& os, std::string& s)
{
    if ( is_double(s))
    {
        scientificFormat( os, std::stod(s));
        return os;
    }
    os << s;
    return os;
}

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
            
            if (loof_for(line, "CEST") || loof_for(line, "CET"))
            {
                set_date_time(line);
                
            }            
            
            add_data(line,"save_file", "save_filename =");
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
            add_data(line,"save_filename", "save_filename = ");            
            add_data(line,"nb_intermediate", "nb_intermediate_ = ");      
            
            

            
            if (loof_for(line,"DUE TO TIME LIMIT"))
            {
                time_out_ = true;      
//                 infos["prep_time"] = "TIMEOUT";
                infos["nb_iter"] = "TIMEOUT";
                infos["comput_time"] = "TIMEOUT";
                infos["time_per_iter"] = "TIMEOUT";
                infos["total_time"] = filename;
            }
            else if (loof_for(line,"CANCELLED AT"))
            {
                fail_ = true;                
//                 infos["prep_time"] = "CANCELLED";
                infos["nb_iter"] = "CANCELLED";
                infos["comput_time"] = "CANCELLED";
                infos["time_per_iter"] = "CANCELLED";
                infos["total_time"] = filename;
            }        
            if (loof_for(line,"Cannot load the library, stopping program"))
            {
                fail_ = true;                
//                 infos["prep_time"] = "CANNOT_LOAD_LIBRARY";
                infos["nb_iter"] = "CANNOT_LOAD_LIBRARY";
                infos["comput_time"] = "CANNOT_LOAD_LIBRARY";
                infos["time_per_iter"] = "CANNOT_LOAD_LIBRARY";
                infos["total_time"] = filename;
 
            }            
            if (loof_for (line, "RERUN WAITING"))
            {
                fail_ = false;
                time_out_ = false;
//                 infos["prep_time"] = "RERUN WAITING";
                infos["nb_iter"] = "RERUN WAITING";
                infos["comput_time"] = "RERUN WAITING";
                infos["time_per_iter"] = "RERUN WAITING";
                infos["total_time"] = filename;
            }    
            if (loof_for (line, "Some of your processes may have been killed"))
            {
                fail_ = true;
                time_out_ = false;
//                 infos["prep_time"] = "MEMORY ISSUE";
                infos["nb_iter"] = "MEMORY ISSUE";
                infos["comput_time"] = "MEMORY ISSUE";
                infos["time_per_iter"] = "MEMORY ISSUE";
                infos["total_time"] = filename;
            }                 
        }        
    }else
    {
        std::cerr<<"error cannot found "<< filename <<std::endl;
    }   		

}

data_format::data_format( const data_format& f)
{
    filename_ = f.filename_;
    infos = f.infos;
    fail_ = f.fail_;
    time_out_ = f.time_out_;
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

std::string data_format::extract_filename(const std::string& chemin) {
    // Trouve la dernière occurrence du séparateur de chemin (/ ou \) 
    // et extrait tout ce qui suit
    size_t position = chemin.find_last_of("/\\");
    if (position != std::string::npos) {
        return chemin.substr(position + 1);
    } else {
        // Aucun séparateur trouvé, le chemin est déjà le nom du fichier
        return chemin;
    }
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

void data_format::print_re_run()
{
    if (infos["prep_time"] == "MEMORY ISSUE")
    {
        std::cout<<"rm -v "<< extract_filename(infos["filename"]) <<" ;   sbatch job_extra_memory.sh "<< infos["ndof"]<<" "<< infos["problem"]<<" " << infos["precision"]<<" " << get_bissection(infos["type"]) <<" "<< get_solver(infos["solver"])<<" "<< infos["save_file"] <<std::endl;        
    }else
    {
        std::cout<<"echo \"RERUN WAITING\" >> "<< extract_filename(infos["filename"]) <<" ;   sbatch job.sh "<< infos["ndof"]<<" "<< infos["problem"]<<" " << infos["precision"]<<" " << get_bissection(infos["type"]) <<" "<< get_solver(infos["solver"])<<" "<< infos["save_file"] <<std::endl;
    }
}

void data_format::set_date_time(std::string& line)
{
    std::istringstream iss(line);
    bool fail = false;

    std::string day;
    iss >> day;    
    std::string month;
    iss >> month;
    
    uint month_i = 0;
    if (month =="févr.")    month_i = 2;
    else if (month =="mars")    month_i = 3;
    else if (month =="avril")    month_i = 4;
    else if (month =="mai")    month_i = 5;
    else if (month =="juin")    month_i = 6;
    else fail = true;
        
    iss>> day;
    uint day_i = std::stoi(day);
    
    std::string time;
    iss>>time;
    
    std::string year;
    iss>>year; //remove CET
    iss>>year;
        
    date_ = (std::stoi(year)-2020)* (365*31*24*60*60) + month_i * (31*24*60*60) + day_i *(24*60*60) ;
    
    if (fail)
    {
        std::cerr<<"Cannot detect the date : "<< line<<std::endl;
    }
}

bool data_format::operator<( data_format& other) 
{
    
    if (date_ == other.date_)
    {
        return infos["filename"] < other.infos["filename"];
    }
    
    return date_ < other.date_;
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

void create_latex( std::ofstream& outfile,
                   const std::vector< data_format*> datas,
                   std::vector<std::string> & columns,
                   const std::string &titre,
                   const std::string &label)
{
    
//     std::cout<<"create table"<<std::endl;
//     std::cout<<"On a "<< columns.size() <<" colonnes "<<std::endl;
    uint cs = columns.size();
//     outfile <<"\\begin{longtable}{|";
    
    
    outfile <<"\\begin{table}\n  \\resizebox{\\textwidth}{!}{   \n \\begin{tabular}{|";
    
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
        outfile<<" } \n \\caption\{"<< titre<<"}"<<std::endl;
        outfile<<" \\label\{fig"<< label<<"}"<<std::endl;
    }
    outfile <<"\\end{table}\n\n";
//     outfile <<"\\normalsize \n";
//     outfile <<"\\end{longtable}\n\n";
//     std::cout<<"end of table"<<std::endl;
    
}

void create_latex( const std::vector< data_format*> datas,
                   const std::string filename,
                   std::vector<std::string> & columns,
                   std::vector<std::string> & columns_average,
                   std::vector<std::string> & common,
                   std::vector<std::string> & average_on,
                   std::vector<std::string> & remove,
                   const std::string& main_title,
                   const std::string& average_title,
                   const std::string& label
                 )
{
    std::cout<<"CREATE LATEX With "<< filename <<" "<< datas.size() <<std::endl;
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
                    std::cout<<"on ignore "<< info.first<<" -- " << info.second <<" due to "<< remove[i] <<std::endl;
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
        
        create_latex( outfile, local_datas, columns, caption,label + "_"+ std::to_string(i+1));
    }
    
    // list precision and solver
    int nb_average_on = average_on.size();
    
    std::vector< std::vector<std::string> > list_on_average( nb_average_on);
    std::vector<int> sizes(nb_average_on);
    std::vector<int> counter(nb_average_on);
    for (int i=0;i<nb_average_on;i++)
    {
        std::list<std::string> list_to_vec;
        list_to_vec = look_for(datas,average_on[i]);
        if (average_on[i] == "precision" || average_on[i] == "ndof")
        {
            list_to_vec.sort();
        }else if (average_on[i] == "solver")   
        {
            list_to_vec = re_order(list_to_vec,solver_order_);
        }
        
        list_on_average[i] = std::vector<std::string>(list_to_vec.begin(),list_to_vec.end());
        sizes[i] = list_on_average[i].size();
        counter[i] = 0;
//         std::cout<<"We found "<< list_on_average[i].size() <<" different values for "<< average_on[i]<<std::endl;
    }
     
    std::vector< data_format*> average_data;
    
    while( counter[0] < sizes[0])
    {
        data_format* d = new data_format();
        for (int i=0;i<nb_average_on;i++)
        {
            d->infos[average_on[i]] = list_on_average[i][counter[i]];
        }

        for (int i=0;i<columns_average.size();i++)
        {
            d->infos[columns_average[i]] = "0";
        }        
        average_data.push_back(d);
        int cpt = nb_average_on-1;
        counter[cpt] ++;
        while(cpt>0)
        {
            if (counter[cpt] >= sizes[cpt])
            {
                counter[cpt] =0;
                counter[cpt-1]++;
            }
            cpt--;
        }
    }

    for (auto& a : average_data)
    {
        uint nb = 0;
        std::vector<double> values(columns_average.size());
        for (int i=0;i<columns_average.size();i++)
            values[i] = 0.0;
        for (auto& d : datas)
        {
            bool test = true;
            for (int i=0;i<nb_average_on;i++)
            {
                if (d->infos[average_on[i]] != a->infos[average_on[i]])
                {
                    test = false;
                    break;
                }
            }
            
            if (test) //&& d->infos["criteria"] != "-1")
            {
                nb ++;
                for (int i=0;i<columns_average.size();i++)
                {
                    if (columns_average[i] != "(D-H:M:S.ms)")
                        values[i] += toDouble(d->infos[columns_average[i]]);
                    
                }
//                 nb_iter += std::stol( d->infos["nb_iter"] );
//                 total_time += toDouble(d->infos["total_time"] );
//                 prep_time += toDouble(d->infos["prep_time"] );
//                 comput_time += toDouble(d->infos["comput_time"] );
//                 total_time_percent += toDouble(d->infos["total_time (%)"] );
//                 nb_iter_percent += toDouble(d->infos["nb_iter (%)"] );
//                 comput_time_percent += toDouble(d->infos["comput_time (%)"] );
            }
        }
        
        if(nb !=0)
        {
            for (int i=0;i<columns_average.size();i++)
            {
                if (columns_average[i] == "nb_iter")
                    a->infos[columns_average[i]] = std::to_string((int) values[i]/nb);
                else
                {
                    a->infos[columns_average[i]] = std::to_string( values[i]/nb);
                    //a->infos[columns_average[i]] = to_string_with_precision( values[i]/nb,2);
                }
            }
//             a->infos["nb_iter"]  = to_string_with_precision(nb_iter / nb,0);            
//             a->infos["total_time"]  = std::to_string( total_time / nb);  
            a->infos["(D-H:M:S.ms)"] = time_format(a->infos["total_time"]);            
//             a->infos["prep_time"]  = std::to_string( prep_time / nb);    
//             a->infos["comput_time"]  = std::to_string( comput_time / nb);
//             a->infos["time_per_iter"] = to_string_with_precision(total_time/nb,2);
//             a->infos["total_time (%)"] = to_string_with_precision(total_time_percent/nb,2);
//             a->infos["nb_iter (%)"]  = to_string_with_precision(nb_iter_percent/nb,2);
//             a->infos["comput_time (%)"] = to_string_with_precision(comput_time_percent/nb,2);
        }
    }
//     set_pourcentage(average_data);
    
    std::vector<std::string> col;
    for (int i=0;i<average_on.size();i++)
        col.push_back(average_on[i]);
    for (int i=0;i<columns_average.size();i++)
        col.push_back(columns_average[i]);
    
    create_latex( outfile, average_data, col,average_title,"average_"+label);
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
//     for (int i=0;i<index;i++)   std::cout<<"\t";
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
        
        if (ref == "solver" && local_data.size() >1)
        {
//             std::cout<<"We have several inputs for this case in files : "<<std::endl;
            
            double max_prep_time = 0;
            for (auto& d : local_data)
            {
//                 std::cout<<d->infos["filename"]<<" ("<<d->infos["solver"]<<")  ("<<d->infos["prep_time"]<<") date = "<< d->date_ <<std::endl;
                double pt = toDouble(d->infos["prep_time"]);
            if (pt > max_prep_time) max_prep_time = pt;
                
            }
            // on récupère la plus vielle
            auto max_iter = std::max_element(local_data.begin(), local_data.end(), [](data_format* lhs, data_format* rhs){ return *lhs < *rhs;});
            
            
            // Vérifier si le vecteur n'est pas vide
            if (max_iter != local_data.end()) {
                auto& maxValue = *max_iter;
//                 std::cout<<"We keep "<< maxValue->infos["filename"] <<" : ("<<maxValue->infos["prep_time"]<<")" <<std::endl;
                maxValue->infos["prep_time"] = std::to_string(max_prep_time);
                
                double t1 = toDouble(maxValue->infos["comput_time"]);
                double t2 = toDouble(maxValue->infos["total_time"]);
                
                if (t1 + max_prep_time > t2)
                {
//                     std::cout<<"We replace total time for "<< maxValue->infos["filename"]  <<std::endl,
                    maxValue->infos["total_time"] = std::to_string( (int) t1 + max_prep_time ); // (int) we assume very big number (no need for coma precision)
                }
                
//                 for (auto& l : local_data)
//                 {
//                         std::cout<<"\t\t "<< l->infos["filename"] <<std::endl;
//                 }
                
                
                // Supprimer toutes les autres valeurs
                local_data.clear();
                local_data.push_back(maxValue);
                // Afficher le résultat
//                 std::cout << "La plus grande valeur est : " << v[0] << std::endl;
            } 
//             else {
//                 std::cout << "Le vecteur est vide." << std::endl;
//             }
        }
        
        if ( local_data.size() == 1)
        {
            prepare_re_run(local_data);    
            data_format* d = local_data[0];
            for (int i=index;i<columns.size();i++)
            {
//                 std::cout<<"columns["<<i<<"] = "<< columns[i] <<std::endl;
                if (columns[i] == "prep_time" || columns[i] == "time_per_iter")
                {
                    outfile <<toScientificString(d->infos[columns[i]]) ;
                }else if (columns[i] == "comput_time (%)"|| columns[i] == "total_time (%)"|| columns[i] == "nb_iter (%)")
                {
                    std::cout<<"d->infos["<<columns[i]<<"] = "<< d->infos[columns[i]] <<std::endl;
                    std::cout<<"d->infos["<<columns[i]<<"] = "<< toPercentageString(d->infos[columns[i]])<<std::endl<<std::endl;
                    outfile <<toPercentageString(d->infos[columns[i]]) ; 
                }else if (columns[i] == "comput_time" || columns[i] == "total_time")
                {
                    outfile <<toDoubleTimeString(d->infos[columns[i]]) ;
                }else if (columns[i] == "comput_time" || columns[i] == "criteria")
                {
                    if (d->infos[columns[i]] == "-1")
                        outfile <<" infeasible ";
                    else
                        outfile <<toScientificString(d->infos[columns[i]],2);
                }else
                {
                    outfile <<replace(d->infos[columns[i]])<< std::scientific<< std::setprecision(3);
                }
                
                if (i == columns.size()-1)
                {
                    outfile<< " \\\\ " ;
                }else
                {
                    outfile<< " &" ;
                }
            }
            outfile <<" \\cline{"<< index+1 <<"-"<< columns.size() <<"}\n";               
        }else
        {
            if (entete == "")
            {
                outfile <<" \\hline \n";   
            }
            int nblin = local_data.size();
            if (nblin > 9)  nblin = 9;
            
            outfile<<"\\multirow{"<<nblin <<"}{*}{"<< replace(t) <<"} & ";
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
    solver_order_.push_back("BissectionBasis_MinVo");
    solver_order_.push_back("BissectionBasis_ApproxMinVo");       
    solver_order_.push_back("BissectionBasis_Recursive");
    solver_order_.push_back("BissectionBasis_Recursive2");
    solver_order_.push_back("ContractionInterval");
    solver_order_.push_back("ContractionBasis_Bernstein");
    solver_order_.push_back("ContractionBasis_MinVariance");
    solver_order_.push_back("ContractionBasis_BSplines");
    solver_order_.push_back("ContractionBasis_MinNo");    
    solver_order_.push_back("ContractionBasis_MinVo");
    solver_order_.push_back("ContractionBasis_ApproxMinVo");
    solver_order_.push_back("ContractionBasis_Recursive");
    solver_order_.push_back("ContractionBasis_Recursive2");        
    
    bissect_order_.clear();
    bissect_order_.push_back("MinFirst");
    bissect_order_.push_back("MaxFirst");
    bissect_order_.push_back("Smart");
    
    for (int i=0;i<30;i++)
        pb_order_.push_back(std::to_string(i));
}

std::list< std::string> look_for( const std::vector< data_format*> datas,
                                    std::string name)
{
    std::list<std::string> out;
    for (auto& d : datas)
    {
        // test precision
        bool test = true;
        for ( auto& p : out)
        {
            if (d->infos[name] == p)
            {
                test = false;
                break;
            }
        }
        if (test)
        {
//             std::cout<<"We found "<< name<<" : "<< d->infos[name] <<std::endl;
            out.push_back( d->infos[name] );
        }
    }
    return out;
}

std::list<std::string> re_order(const std::list<std::string>& input, 
                                const std::list<std::string>& dic)
{
    std::list<std::string> out;
    
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

bool is_number(const std::string& s)
{
    if (s =="") return false;
    long double ld;
    return((std::istringstream(s) >> ld >> std::ws).eof());
}


double toDouble(std::string s){
    std::replace(s.begin(), s.end(), '.', ',');
    return std::atof(s.c_str());
}

std::string toDoubleTimeString (std::string s)
{
    double val = toDouble(s);
    if (val < 1)
        return toScientificString(s);
    if (val < 10)
        return to_string_with_precision(val,3);
    if (val < 100)
        return to_string_with_precision(val,2);    
    if (val < 1000)
        return to_string_with_precision(val,1);
    else
    {
        int v = val;
        return std::to_string(v);
    }
}

std::string toPercentageString(std::string s)
{
    if (is_double(s))
    {
        double val = toDouble(s);
        if (val <= 0.0)
        {
            std::cout<<"is negative"<<std::endl;
            return s;
        }
        int p = 0;
        
        while ( val > pow(10,p++));
        while ( val < pow(10,p--));
        
        int pp = 2 -p;
        if (pp < 1) pp = 1;
        return to_string_with_precision(val,pp);
    }
    std::cout<<"is not double "<<std::endl;
    return "";
}

std::string toScientificString (std::string s, uint offset )
{
    double val = toDouble(s);
    int expo=0;
    
    while ( val > pow(10,expo+3))
    {
        expo += 3;
    }
    
    while ( val < pow(10,expo))
        expo -= 3;    
    
    double m = val / pow(10,expo);
    std::string out;
    if (m < 10)
        out = to_string_with_precision(m,offset+2);
    else if (m < 100)
        out = to_string_with_precision(m,offset+1);    
    else
        out = to_string_with_precision(m,offset);    
    if (expo != 0 )
        out += "e" + std::to_string(expo);
    
    return out;
}


std::string replace (const std::string &in)
{
//         std::cout<<"in = "<< in <<std::endl;
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
    out =  std::regex_replace(out, std::regex("%"), "\\%");

    out =  std::regex_replace(out, std::regex("MinNo"), "MinNorm");
    out =  std::regex_replace(out, std::regex("MinVo"), "MinVolume");
    
    
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

void prepare_re_run(const std::vector< data_format*> datas)
{
    for (auto& d : datas)
    {
        if (d->time_out_ || d->fail_)
        {
            d->print_re_run();
        }
    }
}

std::string to_string_with_precision(const double a_value, const int n )
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}

void set_pourcentage( const std::vector< data_format*> datas)
{
    std::vector<std::string> common_ref;
    common_ref.push_back("precision");
    common_ref.push_back("problem");
    common_ref.push_back("ndof");
    common_ref.push_back("type");
    
    for (auto& t : datas)
    {
        bool test = false;
        // look for the reference
        for (auto& r : datas)
        {
            bool test_eq = true;
            if( is_number(r->infos["total_time"]) && (r->infos["solver"] == "BissectionInterval" || r->infos["solver"] == "ContractionInterval" ))
            {         
               for (int k=0;k<common_ref.size();k++)
               {
                   if ( t->infos[common_ref[k]] != r->infos[common_ref[k]])
                    {
                        test_eq = false;
                        break;
                    }
                }
                if (test_eq)
                {
//                     std::cout<<"reference pourcentage : "<< r->infos["total_time"] <<std::endl;
                    test = true;
                    t->infos["total_time (%)"] = to_string_with_precision(100.*toDouble(t->infos["total_time"]) / toDouble(r->infos["total_time"]),2);
                    t->infos["nb_iter (%)"] = to_string_with_precision(100.*toDouble(t->infos["nb_iter"]) / toDouble(r->infos["nb_iter"]),2);
                    t->infos["comput_time (%)"] = to_string_with_precision(100.*toDouble(t->infos["comput_time"]) / toDouble(r->infos["comput_time"]),2);
                    
                    std::cout<<"nb_iter (%) : "<< t->infos["nb_iter (%)"] <<std::endl;
                    
                    break;
                }                
            }
        }
//         if(!test)
//         {
//             std::cout<<"on n'a pas trouvé la référence"<<std::endl;
//         }        
    }    
}

