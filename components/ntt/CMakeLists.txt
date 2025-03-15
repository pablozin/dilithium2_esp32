idf_component_register(SRCS "ntt.c"
                    INCLUDE_DIRS "." "../common" # Inclui pasta própria, dilithium para params/consts possivelmente, fips202 se usar hashes para NTT
                    REQUIRES "reduce")
