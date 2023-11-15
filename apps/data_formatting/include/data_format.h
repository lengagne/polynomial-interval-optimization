#ifndef __DATAFORMAT_H__
#define __DATAFORMAT_H__

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <list>

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
    
    bool loof_for(const std::string& line, const std::string& pattern);
    
    void plot();
       
    std::string filename_;
    
    std::map< std::string, std::string> infos;
    
    bool time_out_ = false;
};



void add (const std::string &in, 
          std::list<std::string> & vec);

void create_csv(const std::vector< data_format*> datas,
                const std::string& ordonnee,
                const std::string& abscisse,
                const std::string& parameter,
                const std::string& filetype,
                const std::string& filename,
                bool ratio =false
               );

void create_latex( const std::vector< data_format*> datas,
                   const std::string filename,
                   std::vector<std::string> & columns,
                   std::vector<std::string> & common);

void create_latex( const std::vector< data_format*> datas,
                   const std::string filename,
                   std::vector<std::string> & columns,
                   const std::string &titre =""
                 );

void create_latex_subpart( std::ofstream& outfile,
                           uint index,
                           std::vector<std::string> & columns,
                           const std::vector< data_format*> datas,
                           const std::string& entete = "");

std::string get_bissection( const std::string in);

std::string get_solver( const std::string in);

void init_order();

std::list<std::string> re_order(const std::list<std::string>& input, 
                                const std::list<std::string>& dic);

double toDouble(std::string s);

std::string replace (const std::string in);


#endif
