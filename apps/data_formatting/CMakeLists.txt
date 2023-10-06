find_package(Qt4 REQUIRED COMPONENTS QtCore QtXml)

include(${QT_USE_FILE})


include_directories(include	
                    ${QT_INCLUDES})

add_definitions(${QT_DEFINITIONS} -DQT_NO_KEYWORDS)

add_executable (Synthetize 
                    src/create_files.cpp
                    src/data_format.cpp)
                    
target_link_libraries(	Synthetize
                        PIO
                        ${QT_LIBRARIES})

# add_executable (Synthetize3D src/create_files_robot_3D.cpp)
# target_link_libraries(	Synthetize3D
#                         PIO
#                         ${QT_LIBRARIES})

