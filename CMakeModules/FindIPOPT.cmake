# - Try to find IPOPT
# Once done this will define
#  IPOPT_FOUND - System has IpOpt
#  IPOPT_INCLUDE_DIRS - The IpOpt include directories
#  IPOPT_LIBRARY_DIRS - The library directories needed to use IpOpt
#  IPOPT_LIBRARIES    - The libraries needed to use IpOpt


if (IPOPT_INCLUDE_DIR)
  # in cache already
  SET(IPOPT_FIND_QUIETLY TRUE)
endif (IPOPT_INCLUDE_DIR)

find_path(IPOPT_INCLUDE_DIR NAMES IpNLP.hpp
PATHS  "$ENV{IPOPT_HOME}/include/coin"
	"/usr/include/coin"
	"/usr/local/include/coin"

)

find_library( IPOPT_LIBRARY 
		ipopt
		PATHS "$ENV{IPOPT_HOME}/lib"
		"/usr/lib"
		"/usr/local/lib")   

#wrong config under Debian workaround
add_definitions( -DHAVE_CSTDDEF )


# set optional path to HSL Solver
find_path(IPOPT_HSL_LIBRARY_DIR 
	NAMES libcoinhsl.so
		libhsl.dylib
	PATHS "$ENV{IPOPT_HSL_LIBRARY_PATH}"
		"$ENV{HOME}/opt/HSL/lib"
		"/usr/local/lib"
)

find_library( IPOPT_ASL_LIBRARY 
		coinasl
		PATHS "$ENV{IPOPT_HOME}/lib"
		"/usr/lib"
		"/usr/local/lib")
		
find_library( IPOPT_BLAS_LIBRARY 
		coinblas
		PATHS "$ENV{IPOPT_HOME}/lib"
		"/usr/lib"
		"/usr/local/lib")
		
find_library( IPOPT_HSL_LIBRARY 
		coinhsl
		PATHS "$ENV{IPOPT_HOME}/lib"
		"/usr/lib"
		"/usr/local/lib")
		
find_library( IPOPT_LAPACK_LIBRARY 
		coinlapack
		PATHS "$ENV{IPOPT_HOME}/lib"
		"/usr/lib"
		"/usr/local/lib")
		
find_library( IPOPT_METIS_LIBRARY 
		coinmetis
		PATHS "$ENV{IPOPT_HOME}/lib"
		"/usr/lib"
		"/usr/local/lib")
		
find_library( IPOPT_MUMPS_LIBRARY 
		coinmumps
		PATHS "$ENV{IPOPT_HOME}/lib"
		"/usr/lib"
		"/usr/local/lib")

set(IPOPT_INCLUDE_DIRS "${IPOPT_INCLUDE_DIR}" )
set(IPOPT_LIBRARIES ${IPOPT_LIBRARY} ${IPOPT_ASL_LIBRARY} ${IPOPT_BLAS_LIBRARY} ${IPOPT_LAPACK_LIBRARY} ${IPOPT_MUMPS_LIBRARY} ${IPOPT_METIS_LIBRARY})
		
IF( IPOPT_HSL_LIBRARY_DIR)
	IF( NOT IPOPT_FIND_QUIETLY )
		message ( "IPOPT_HSL_LIBRARY_DIR found at ${IPOPT_HSL_LIBRARY_DIR} ")
	ENDIF(NOT IPOPT_FIND_QUIETLY)
	set(IPOPT_LIBRARY_DIR ${IPOPT_HSL_LIBRARY_DIR})
	LIST( APPEND IPOPT_LIBRARY_DIRS "${IPOPT_HSL_LIBRARY_DIR}")
	set(IPOPT_LIBRARIES ${IPOPT_LIBRARIES} ${IPOPT_HSL_LIBRARY} )
ENDIF(IPOPT_HSL_LIBRARY_DIR)

IF( IPOPT_METIS_LIBRARY_LIBRARY_DIR)
	IF( NOT IPOPT_FIND_QUIETLY )
		message ( "IPOPT_METIS_LIBRARY found at ${IPOPT_HSL_LIBRARY_DIR} ")
	ENDIF(NOT IPOPT_FIND_QUIETLY)
	set(IPOPT_LIBRARY_DIR ${IPOPT_METIS_LIBRARY_DIR})
	LIST( APPEND IPOPT_LIBRARY_DIRS "${IPOPT_HSL_LIBRARY_DIR}")
	set(IPOPT_LIBRARIES ${IPOPT_LIBRARIES} ${IPOPT_METIS_LIBRARY} )
ENDIF(IPOPT_METIS_LIBRARY_LIBRARY_DIR)

set(IPOPT_LIBRARIES ${IPOPT_LIBRARIES}  "-ldl")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBCPLEX_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(IPOPT  DEFAULT_MSG
				IPOPT_LIBRARY IPOPT_INCLUDE_DIR)

mark_as_advanced(IPOPT_INCLUDE_DIR IPOPT_LIBRARY )
