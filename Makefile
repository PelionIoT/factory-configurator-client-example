## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

export MBED_CLOUD_SERVICE = ${EXTERNAL_APP_FOLDER}

# Name of the module
LOCAL_NAME := ${MBED_CLOUD_SERVICE}

# list all modules APIs that are necessary to compile this module
LOCAL_API_DEPENDS := \
                    ${MBED_CLOUD_SERVICE}/mbed-cloud-client \
                    platform/service/posix \
                    ${API_PLATFORM_DEPENDS} \

LOCAL_ADD_INCLUDE +=     \
                         ${MBED_CLOUD_SERVICE}/source/include \
                         ${MBED_CLOUD_SERVICE}/source/platform/include \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/factory-configurator-client/factory-configurator-client \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/fcc-bundle-handler/fcc-bundle-handler \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/fcc-output-info-handler/fcc-output-info-handler \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/key-config-manager/key-config-manager \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/crypto-service/crypto-service \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/mbed-trace-helper/mbed-trace-helper \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/utils/utils \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/ftcd-comm-base/ftcd-comm-base \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/factory-configurator-client/ftcd-comm-serial/ftcd-comm-serial \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/mbed-trace \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/nanostack-libservice/mbed-client-libservice \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/mbed-client-pal/Source \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/mbed-client-pal/Source/PAL-Impl/Services-API \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/mbed-client-pal/Configs/pal_config \
                         ${MBED_CLOUD_SERVICE}/mbed-cloud-client/mbed-client-pal/Configs/pal_config/SXOS \
 
LOCAL_EXPORT_FLAG += __SXOS__

LOCAL_EXPORT_FLAG += MBED_CONF_MBED_TRACE_ENABLE=1
LOCAL_EXPORT_FLAG += PAL_NUMBER_OF_PARTITIONS=1
LOCAL_EXPORT_FLAG += "'PAL_USER_DEFINED_CONFIGURATION=\"SXOS/sxos_sotp.h\"'"
LOCAL_EXPORT_FLAG += "'MBED_CLIENT_USER_CONFIG_FILE=\"source/mbed_cloud_client_user_config.h\"'"
LOCAL_EXPORT_FLAG += "'MBED_CLOUD_CLIENT_USER_CONFIG_FILE=\"source/mbed_cloud_client_user_config.h\"'"
#FIXME: uncomment when porting CEC tests
#LOCAL_EXPORT_FLAG += CERT_RENEWAL_TEST
#LOCAL_EXPORT_FLAG += CERT_ENROLLMENT_EST_MOCK
LOCAL_EXPORT_FLAG += MBED_TRACE_TMP_LINE_LENGTH=1024
LOCAL_EXPORT_FLAG += "PAL_SIMULATOR_FLASH_OVER_FILE_SYSTEM=1"
LOCAL_EXPORT_FLAG += FCE_SERIAL_INTERFACE

# Compile the sub-modules, except when the "service" must be used as a library.
# list all the modules that need to be compiled prior to using this module
LOCAL_MODULE_DEPENDS := ${MBED_CLOUD_SERVICE}/mbed-cloud-client

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := no

# This is a top-level module
IS_TOP_LEVEL := yes

# Generates the CoolWatcher headers automatically.
AUTO_XMD2H ?= no

# code is not in one "src/" directory as SDK expects by default
USE_DIFFERENT_SOURCE_LAYOUT := yes
USE_DIFFERENT_SOURCE_LAYOUT_ARM := yes

C++_SRC += ${wildcard source/factory_client_example.cpp}
C++_SRC += ${wildcard source/fce_mbed_os_serial_helper.cpp}

C++_SRC += ${wildcard mbed-cloud-client/factory-configurator-client/ftcd-comm-base/source/*.cpp}
C++_SRC += ${wildcard mbed-cloud-client/factory-configurator-client/ftcd-comm-serial/source/*.cpp}

C_SRC += ${wildcard source/platform/*.c}
C++_SRC += ${wildcard source/platform/*.cpp}

C_SRC += ${wildcard source/platform/SXOS/*.c}
C++_SRC += ${wildcard source/platform/SXOS/*.cpp}


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk
