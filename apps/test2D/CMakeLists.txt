include_directories( include)

add_executable (Test2D 
                src/test2D.cpp
                src/ProblemPoint2D.cpp
                )

target_link_libraries(	Test2D
                        ${PIO_LIBRARIES}
                        )
                        
