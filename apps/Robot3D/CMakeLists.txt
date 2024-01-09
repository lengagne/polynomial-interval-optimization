add_subdirectory(ThirdParty)


if(NOT MogsCore_FOUND)
    find_package(MogsCore QUIET)
endif()

if(MogsCore_FOUND)
    message("MogsCore_FOUND")   
    
    include_directories(    include
                            ${MogsCore_INCLUDE_DIRS}
                            ../data_formatting/include/)

    add_executable (Robot3D 
                    src/test_Robot_3D.cpp
                    src/Problem3D.cpp
                    src/Problem3D_with_torque_limit.cpp
                    src/contruct_filename_3D.cpp)

    target_link_libraries(	Robot3D
                            PIO
                            ${MogsCore_LIBRARIES})

    add_executable (Eval3D  
                    src/eval_3D.cpp
                    src/Problem3D.cpp
                    src/Problem3D_with_torque_limit.cpp
                    ../data_formatting/src/data_format.cpp)

    target_link_libraries(	Eval3D
                            PIO
                            ${MogsCore_LIBRARIES})
                            
                                                        
else(MogsCore_FOUND)
    message("MogsCore NOT FOUND, so we do not compile Robot3D")
endif(MogsCore_FOUND)
