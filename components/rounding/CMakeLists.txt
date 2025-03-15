idf_component_register(SRCS "rounding.c"
                    INCLUDE_DIRS "." "../common/" # Inclui pasta própria, dilithium para params de rounding
                    REQUIRES "") # Depende de dilithium para parametros
