# global defines and configuration goes here...
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

set(SA_PV_OS_NAME "linux")
OPTION(LINK_WITH_ESFS_SOTP "Explicitly link esfs and sotp library" ON)

SET(PAL_TLS_BSP_DIR ${NEW_CMAKE_SOURCE_DIR}/mbed-cloud-client/mbed-client-pal/Configs/mbedTLS)

add_definitions(
    -DMBED_CONF_MBED_TRACE_ENABLE=1
    -DFCC_ENABLE_FULL_TRACING
    -DMBEDTLS_CONFIG_FILE="\\"${PAL_TLS_BSP_DIR}/mbedTLSConfig_${OS_BRAND}.h"\\"
    -DPAL_USER_DEFINED_CONFIGURATION="\\"source/pal_config_linux.h"\\"
    -DMBED_CLOUD_CLIENT_USER_CONFIG_FILE="\\"source/mbed_cloud_client_user_config.h"\\"
    -DFCE_ETHERNET_INTERFACE
)
