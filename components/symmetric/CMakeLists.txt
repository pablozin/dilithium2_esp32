idf_component_register(SRCS "symmetric-shake.c"
                    INCLUDE_DIRS "." "../common"
                    REQUIRES "fips202" "symmetric" "aes256ctr" "mbedtls")
