
add_executable (BasisFunctionsCriteria
                src/BasisFunctionsCriteria.cpp)

target_link_libraries(	BasisFunctionsCriteria
                        ${PIO_LIBRARIES})
                        
MESSAGE ("PIO_LIBRARIES = " ${PIO_LIBRARIES})
