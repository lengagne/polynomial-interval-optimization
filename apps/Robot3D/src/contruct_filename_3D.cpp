#include "construct_filename_3D.h"

std::string construct_filename_3D(
        double precision,
        uint npb,
        uint bissection_mode,
        uint type_bissection // intervale, basis, ...
        )
{
    std::string filename = "save3D";
    filename += "_prec" + std::to_string(precision);
    filename += "_npb" + std::to_string(npb);    
    filename += "_bimo" + std::to_string(bissection_mode);
    filename += "_tybi" + std::to_string(type_bissection);
    filename += "_v1.sop"; // Save Optim Process
    
    return filename;
}

