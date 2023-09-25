cmake_minimum_required( VERSION 2.8 )
project (PIO)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++0x -msse2 -fPIC")

if(CRRI)
	message("We are on the CCRI")
	set(EIGEN3_INCLUDE_DIR "/opt/apps/gcc-4.8.4/eigen-3.3.1/include/eigen3")
else(CRRI)
	message("We are on normal computer")
	find_package(Eigen3 REQUIRED)
endif(CRRI)


if (NOT LAZYCPP_FOUND)
    find_package(LazyCPP REQUIRED)
endif()

find_package( IPOPT QUIET)


 ## TCMALLOC used to optimized memory allocation
 find_package(PkgConfig REQUIRED)
 pkg_check_modules(TCMALLOC libtcmalloc_minimal)
 if(TCMALLOC_FOUND)
   message("TCMALLOC FOUND")
   include_directories(${TCMALLOC_INCLUDE_DIRS})
   link_directories(${TCMALLOC_LIBRARY_DIRS})
   list(APPEND MG_LIBRARIES ${TCMALLOC_LIBRARIES})
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
 endif(TCMALLOC_FOUND)

foreach(p LIB INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")


file( GLOB_RECURSE PIO_SOURCES src/*.cpp )
FILE ( GLOB_RECURSE PIO_HEADERS include/*.h   include/*.hxx )

file( GLOB_RECURSE PIO_PIO_BasisFunctions_SOURCES basis_functions/src/*.cpp )
FILE ( GLOB_RECURSE PIO_PIO_BasisFunctions_HEADERS basis_functions/include/*.h)

file( GLOB_RECURSE PIO_PIO_Solvers_SOURCES solvers/src/*.cpp )
FILE ( GLOB_RECURSE PIO_PIO_Solvers_HEADERS solvers/include/*.h)


include_directories(	include
                        basis_functions/include
                        solvers/include
                        ${EIGEN3_INCLUDE_DIR}
                        ${TCMALLOC_INCLUDE_DIRS}
                        ${LazyCPP_INCLUDE_DIRS}
                        )

link_directories(	${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}
                    ${TCMALLOC_LIBRARY_DIRS})


add_library(PIO SHARED ${PIO_SOURCES} ${PIO_HEADERS})
add_library(PIO_PIO_BasisFunctions SHARED ${PIO_BasisFunctions_SOURCES} ${PIO_HEADERS})
add_library(PIO_PIO_Solvers SHARED ${PIO_Solvers_SOURCES} ${PIO_HEADERS})

target_link_libraries(	PIO_BasisFunctions
                        ${TCMALLOC_LIBRARIES}
                        ${IPOPT_LIBRARIES}
                        )

target_link_libraries(	PIO_Solvers
                        PIO_BasisFunctions
                        ${LazyCPP_LIBRARIES}
                        )                        


target_link_libraries(	PIO
                        PIO_BasisFunctions
                        PIO_Solvers
                        )
#                         
# 
# add_executable (PlotPIO_BasisFunctions EXCLUDE_FROM_ALL
#                 basis_functions/apps/PlotPIO_BasisFunctions.cpp)
# 
# target_link_libraries(	PlotPIO_BasisFunctions
#                         PIO)
# 
# 
# add_executable (Test2D EXCLUDE_FROM_ALL
#                 apps/test2D.cpp
#                 apps_problem/ProblemPoint2D.cpp)
# 
# target_link_libraries(	Test2D
#                         PIO)
#                         
# add_executable (TestDivision EXCLUDE_FROM_ALL
#                 apps/test_division.cpp
#                 apps_problem/ProblemDivision.cpp)
# 
# target_link_libraries(	TestDivision
#                         PIO)
#                         
# 
# add_executable (TestTrigo2D EXCLUDE_FROM_ALL
#                 apps/testTrigo2D.cpp
#                 apps_problem/ProblemTrigo2D.cpp)
# 
# target_link_libraries(	TestTrigo2D
#                         PIO)
#                         
# add_executable (EvalPoly EXCLUDE_FROM_ALL
#                         apps/eval_polynome.cpp
#                         apps_problem/ProblemPoly.cpp
#                         )
# 
# target_link_libraries(	EvalPoly
#                         ${LazyCPP_LIBRARIES}
#                         PIO
#                         )
#                         
# add_executable (GetDeterminant EXCLUDE_FROM_ALL
#                         basis_functions/apps/GetDeterminant.cpp
#                         apps_problem/ProblemPoly.cpp
#                         )
# 
# target_link_libraries(	GetDeterminant
#                         ${LazyCPP_LIBRARIES}
#                         PIO
#                         )                     
#                      
#              
# add_executable (TestKronecker EXCLUDE_FROM_ALL
#                 unitTest/TestKronecker.cpp)
#                 
# target_link_libraries(	TestKronecker
#                         ${LazyCPP_LIBRARIES}
#                         PIO
#                         )                                  
# 
# 
#                         
# if(USE_QT)
#     ### QT used for graphical interfaces strings and xml
#     find_package(Qt4 REQUIRED COMPONENTS QTCORE QTGUI QTXML QtXmlPatterns)
#     include(${QT_USE_FILE})
#     add_definitions(${QT_DEFINITIONS} -DQT_NO_KEYWORDS)
#     add_executable (Synthetize apps/create_files.cpp)
#     add_executable (Synthetize3D apps/create_files_robot_3D.cpp)
#     target_link_libraries(	Synthetize
#                             ${LazyCPP_LIBRARIES}
#                         PIO
#                             ${QT_LIBRARIES})
#     target_link_libraries(	Synthetize3D
#                             ${LazyCPP_LIBRARIES}
#                         PIO
#                             ${QT_LIBRARIES})
# endif(USE_QT)
# 
# add_executable (Robot2D EXCLUDE_FROM_ALL
#                         apps/test_Robot_2D.cpp
#                         apps_problem/Problem1.cpp
#                         apps_problem/Problem4.cpp
#                         apps_problem/Problem7.cpp
#                         apps_problem/Problem1Torque.cpp
#                         apps_problem/Problem4Torque.cpp
#                         apps_problem/Problem7Torque.cpp)
# 
# target_link_libraries(	Robot2D
#                         PIO)
# 
# find_package(MogsCore QUIET)
# 
# if(MogsCore_FOUND)
#     message("MogsCore_FOUND")
# 
#     include_directories(${MogsCore_INCLUDE_DIRS})
# 
#     add_executable (Robot3D EXCLUDE_FROM_ALL
#                             apps/test_Robot_3D.cpp
#                             apps_problem/Problem3D.cpp
#                             apps_problem/Problem3D_with_torque_limit.cpp)
# 
#     target_link_libraries(	Robot3D
#                             PIO
#                             ${MogsCore_LIBRARIES})
# 
#     add_executable (Eval3D  EXCLUDE_FROM_ALL
#                             apps/eval_3D.cpp
#                             apps_problem/Problem3D.cpp
#                             apps_problem/Problem3D_with_torque_limit.cpp)
# 
#     target_link_libraries(	Eval3D
#                             PIO
#                             ${MogsCore_LIBRARIES})
#                             
#                                                         
# else(MogsCore_FOUND)
#     message("MogsCore NOT FOUND, so we do not compile Robot3D")
# endif(MogsCore_FOUND)
# 
# add_custom_target( problems
#   DEPENDS TestTrigo2D Eval3D Robot3D Robot2D GetDeterminant EvalPoly TestDivision Test2D)
# 
# 
# ### installation of the files
# set (INSTALL_LIB_PATH lib )
# SET(INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR}/include/Mogs2/PIO)
# SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )
# 
# ### Configure file
# get_property(Include_Directories DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# SET(PIO_INCLUDE_DIRS ${INSTALL_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} ${TCMALLOC_INCLUDE_DIRS} ${LazyCPP_INCLUDE_DIRS} CACHE INTERNAL "")
# SET(PIO_LIBRARY_DIRS  ${CMAKE_INSTALL_LIBDIR}/lib  CACHE INTERNAL "")
# SET(PIO_LIBRARIES PIO CACHE INTERNAL "")
# SET(PIO_FLAGS ${CMAKE_CXX_FLAGS} CACHE INTERNAL "")
# SET(PIO_FOUND "TRUE" CACHE INTERNAL "")
# 
# configure_file(PIOConfig.cmake.in "${PROJECT_BINARY_DIR}/PIOConfig.cmake" @ONLY)
# 
# 
# install( FILES ${PIO_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIRS})
# install( TARGETS PIO LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)
# 
# install ( 	FILES "${PROJECT_BINARY_DIR}/PIOConfig.cmake"
# 			DESTINATION "${INSTALL_LIB_PATH}/PIOConfig" COMPONENT dev )
# 
