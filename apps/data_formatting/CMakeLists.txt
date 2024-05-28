find_package(Qt4 REQUIRED COMPONENTS QtCore QtXml)

include(${QT_USE_FILE})


include_directories(include	
                    ${QT_INCLUDES})

add_definitions(${QT_DEFINITIONS} -DQT_NO_KEYWORDS)

add_executable (Synthetize 
                src/create_files.cpp
                src/data_format.cpp)

add_executable (Synthetize3D
                src/create_files_3D.cpp
                src/data_format.cpp
                src/data_format3D.cpp)
                
add_executable (SelectData
                src/select_files.cpp
                src/data_format.cpp
                src/data_format3D.cpp)                
                                        
target_link_libraries(	Synthetize
                        ${QT_LIBRARIES})
                    
target_link_libraries(	Synthetize3D
                        ${QT_LIBRARIES})
                        
target_link_libraries(	SelectData
                        ${QT_LIBRARIES})

