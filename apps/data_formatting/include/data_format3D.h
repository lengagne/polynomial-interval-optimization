#ifndef __DATAFORMAT3D_H__
#define __DATAFORMAT3D_H__

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <list>

#include "data_format.h"

class data_format3D : public data_format
{
public:
    data_format3D( const std::string& filename);
    

};

// void create_latex( const std::vector< data_format3D*> datas,
//                    const std::string filename,
//                    std::vector<std::string> & columns);
// 
// void create_latex_subpart( std::ofstream& outfile,
//                            uint index,
//                            std::vector<std::string> & columns,
//                            const std::vector< data_format3D*> datas,
//                            const std::string& entete = "");     
// 
// void init_order();

#endif
