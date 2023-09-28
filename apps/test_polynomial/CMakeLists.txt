include_directories( include)

add_executable (EvalPoly 
                src/eval_polynome.cpp
                src/ProblemPoly.cpp
                )

target_link_libraries(	EvalPoly
                        ${PIO_LIBRARIES}
                        )
                        
