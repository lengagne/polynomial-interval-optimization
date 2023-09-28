add_subdirectory(ThirdParty)


if(NOT MogsCore_FOUND)
    find_package(MogsCore QUIET)
endif()

if(MogsCore_FOUND)
    message("MogsCore_FOUND")

    message("MogsCore_INCLUDE_DIRS : " ${MogsCore_INCLUDE_DIRS})
    
    
    include_directories(    include
                            ${MogsCore_INCLUDE_DIRS})

    add_executable (Robot3D 
                            src/test_Robot_3D.cpp
                            src/Problem3D.cpp
                            src/Problem3D_with_torque_limit.cpp)

    target_link_libraries(	Robot3D
                            PIO
                            ${MogsCore_LIBRARIES})

    add_executable (Eval3D  
                            src/eval_3D.cpp
                            src/Problem3D.cpp
                            src/Problem3D_with_torque_limit.cpp)

    target_link_libraries(	Eval3D
                            PIO
                            ${MogsCore_LIBRARIES})
                            
                                                        
else(MogsCore_FOUND)
    message("MogsCore NOT FOUND, so we do not compile Robot3D")
endif(MogsCore_FOUND)
