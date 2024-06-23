#ifndef __DATAFORMAT_H__
#define __DATAFORMAT_H__

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <list>

std::ostream& scientificFormat(std::ostream& os, double value);

std::ostream& scientificFormat(std::ostream& os, std::string& s);

class data_format
{
public:
    data_format()
    {
        
    }
    
    data_format( const std::string& filename);
    
    data_format( const data_format& f);
    
    ~data_format()
    {
        infos.clear();
    }
    
    void add_data( const std::string& line, const std::string& name, const std::string& pattern);    
    
    std::string extract_filename(const std::string& chemin);
    
    bool loof_for(const std::string& line, const std::string& pattern);
    
    void plot();
    
    virtual void print_re_run();
    
    void set_date_time(std::string& line);
    
    bool operator<( data_format& other)  ;    
    
    long int date_ = 0;
       
    std::string filename_;
    
    std::map< std::string, std::string> infos;
    
    bool time_out_ = false;
    bool fail_ = false;

};

// Comparaison des pointeurs
// bool operator<(const data_format* lhs, const data_format* rhs) 
// {
//     // Assurez-vous de gérer le cas où les pointeurs sont null
//     if (!lhs || !rhs) throw std::invalid_argument("Null pointer comparison");
// 
//     return *lhs < *rhs;
// }



void add (const std::string &in, 
          std::list<std::string> & vec);

// void create_csv(const std::vector< data_format*> datas,
//                 const std::string& ordonnee,
//                 const std::string& abscisse,
//                 const std::string& parameter,
//                 const std::string& filetype,
//                 const std::string& filename,
//                 bool ratio =false
//                );

void create_latex( const std::vector< data_format*> datas,
                   const std::string filename,
                   std::vector<std::string> & columns,
                   std::vector<std::string> & columns_average,
                   std::vector<std::string> & common,
                   std::vector<std::string> & common_average,
                   std::vector<std::string> & remove,
                   const std::string& main_title="empty",
                   const std::string& average_title="empty",
                   const std::string& label="label"
                 );

void create_latex( const std::vector< data_format*> datas,
                   const std::string filename,
                   std::vector<std::string> & columns,
                   const std::string &titre ="",
                   const std::string &label ="label"
                 );

void create_latex_subpart( std::ofstream& outfile,
                           uint index,
                           std::vector<std::string> & columns,
                           const std::vector< data_format*> datas,
                           const std::string& entete = "");

std::string get_bissection( const std::string in);

std::string get_solver( const std::string in);

void init_order();

std::list< std::string> look_for( const std::vector< data_format*> datas,
                                    std::string name);

std::vector< data_format*> remove_extra_info( const std::vector< data_format*>& datas);

std::list<std::string> re_order(const std::list<std::string>& input, 
                                const std::list<std::string>& dic);

double toDouble(std::string s);

bool is_number(const std::string& s);

std::string replace (const std::string& in);

std::string time_format( const std::string & in);

void prepare_re_run(const std::vector< data_format*> datas);

std::string to_string_with_precision(const double a_value, const int n = 6);

void set_pourcentage( const std::vector< data_format*> datas);

#endif
