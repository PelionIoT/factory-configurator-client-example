cmake_minimum_required(VERSION 2.6)

# pal services
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/mbed-client-pal/Source/PAL-Impl/Services-API)

# mbed-trace and services
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/mbed-trace)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/mbed-trace/mbed-trace)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/nanostack-libservice/mbed-client-libservice)

# factory-configurator-client
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/factory-configurator-client/factory-configurator-client/factory-configurator-client)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/factory-configurator-client/ftcd-comm-base/ftcd-comm-base)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/factory-configurator-client/ftcd-comm-socket/ftcd-comm-socket)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/factory-configurator-client/fcc-bundle-handler/fcc-bundle-handler)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/factory-configurator-client/key-config-manager/key-config-manager)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/factory-configurator-client/mbed-trace-helper/mbed-trace-helper)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/factory-configurator-client/fcc-output-info-handler/fcc-output-info-handler)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client/factory-configurator-client/utils/utils)

# mbed-cloud-client-platform-common
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mbed-cloud-client-platform-common/include)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DMBED_TRACE_MAX_LEVEL=TRACE_LEVEL_INFO -DPAL_NUMBER_OF_PARTITIONS=1 -DPAL_PRIMARY_PARTITION_PRIVATE=0 -DPAL_SECONDARY_PARTITION_PRIVATE=0)
else()
	add_definitions(-DMBED_TRACE_MAX_LEVEL=TRACE_LEVEL_ERROR -DPAL_NUMBER_OF_PARTITIONS=1 -DPAL_PRIMARY_PARTITION_PRIVATE=0 -DPAL_SECONDARY_PARTITION_PRIVATE=0)
endif()

# example source files
SET(
	factory-configurator-client-example
	${CMAKE_CURRENT_SOURCE_DIR}/source/factory_client_example.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/fce_${SA_PV_OS_NAME}_socket_helper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/fce_common_helper.h
)

message ("*********************************************************************")
message ("factory-configurator-client-example = [[${factory-configurator-client-example}]]")
message ("*********************************************************************")

# On Linux, we might find different names for the network interface
# so we have to find it, and pass to the code, however, this may NOT work
# when building on one machine and running on a different one!
execute_process(COMMAND ip -o -4 route show to default
						COMMAND awk "{print $5}"
						COMMAND head -n1
						OUTPUT_STRIP_TRAILING_WHITESPACE
						OUTPUT_VARIABLE  ETHNAME)
set(TMPD \"${ETHNAME}\")
add_definitions(-DETHERNET_LINUX_IFACE_NAME=${TMPD})
message("ETHERNET_LINUX_IFACE_NAME=${ETHNAME}")

CREATE_TEST_LIBRARY(factory-configurator-client-example "${factory-configurator-client-example}" "")

add_subdirectory(mbed-cloud-client/factory-configurator-client ${CMAKE_BINARY_DIR}/factory-configurator-client)
add_subdirectory(mbed-cloud-client/mbed-client-pal ${CMAKE_BINARY_DIR}/mbed-client-pal)
add_subdirectory(mbed-cloud-client/mbed-trace ${CMAKE_BINARY_DIR}/mbed-trace)
add_subdirectory(mbed-cloud-client-platform-common ${CMAKE_BINARY_DIR}/mbed-cloud-client-platform-common)

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
		set (SRC_LIBS ${SRC_LIBS} PARENT_SCOPE)
		set (TEST_LIBS ${TEST_LIBS} PARENT_SCOPE)
endif()

