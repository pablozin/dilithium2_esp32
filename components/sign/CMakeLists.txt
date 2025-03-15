idf_component_register(SRCS "sign.c"
                    INCLUDE_DIRS "." "../common"
                    REQUIRES "packing" "polyvec" "poly" "randombytes" "symmetric" "fips202")
