#ifndef __CONSTRUCT_FILENAME_3D__
#define __CONSTRUCT_FILENAME_3D__

#include <string>

std::string construct_filename_3D(
        double precision,
        uint npb,
        uint bissection_mode,
        uint type_bissection // intervale, basis, ...
        );

#endif
