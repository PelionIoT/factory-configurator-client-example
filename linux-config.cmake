# global defines and configuration goes here...

set(SA_PV_OS_NAME "linux")

SET(PAL_TLS_BSP_DIR ${NEW_CMAKE_SOURCE_DIR}/mbed-cloud-client/mbed-client-pal/Configs/mbedTLS)

add_definitions(
	-DMBED_CONF_MBED_TRACE_ENABLE=1
	-DMBED_CONF_MBED_TRACE_FEA_IPV6=0
	-DMBEDTLS_CONFIG_FILE="\\"${PAL_TLS_BSP_DIR}/mbedTLSConfig_${OS_BRAND}.h"\\"
	-DPAL_USER_DEFINED_CONFIGURATION="\\"../source/sotp_fs_config_linux.h"\\"
)
