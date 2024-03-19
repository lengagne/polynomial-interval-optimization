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
    
    data_format3D( const data_format3D& f);
    
    virtual void print_re_run();

};

#endif
