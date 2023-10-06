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
    data_format( const std::string& filename);
    
    ~data_format();
    
    void add_data( const std::string& line, const std::string& name, const std::string& pattern);
    
    void plot();
       
    std::string filename_;
    
    std::map< std::string, std::string> infos;
};

void add (const std::string &in, 
          std::list<std::string> & vec);

void create_csv(const std::vector< data_format*> datas,
                const std::string& ordonnee,
                const std::string& abscisse,
                const std::string& parameter,
                const std::string& filetype,
                const std::string& filename);


#endif
