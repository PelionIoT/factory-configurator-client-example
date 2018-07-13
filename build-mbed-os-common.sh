#!/bin/bash
#----------------------------------------------------------------------------
#Copyright 2016-2017 ARM Ltd.
# 
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
# 
#    http://www.apache.org/licenses/LICENSE-2.0
# 
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#----------------------------------------------------------------------------

# halt on error
set -e

if [ -z "$FCC_EXAMPLE_TOP" ]; then
    echo "ASSERT: callee did not set FCC_EXAMPLE_TOP=<top-tree-level>, exiting..."
    exit 1
fi

TOOLCHAIN=
PLATFORM=
INTERFACE=
MBED_APP_FILE=
ENABLE_RAM_PROFILING=

toolchains=( ARM ARMC6 GCC_ARM IAR )
platforms=( K64F NUCLEO_F429ZI UBLOX_EVK_ODIN_W2 NUCLEO_F411RE)
interfaces=( FCE_SERIAL_INTERFACE FCE_ETHERNET_INTERFACE )
mbed_app_files=( DEFAULT LITTLE_FS )

function usage() {
    echo ""
    echo "mbedOS build script for various platforms"
    echo ""
    echo "   -h --help"
    echo "   -t --toolchain     toolchain <ARM, GCC_ARM, IAR>" 
    echo "   -p --platform      platform type <K64F, NUCLEO_F429ZI, UBLOX_EVK_ODIN_W2, NUCLEO_F411RE>"
    echo "   -i --iface         communication interface <FCE_SERIAL_INTERFACE, FCE_ETHERNET_INTERFACE>"
    echo "   -c --cfile         mbed app file - for default used mbed_app.json"
    echo ""
    echo "   e.g.: $0 --toolchain=GCC_ARM --platform=K64F --iface=FCE_SERIAL_INTERFACE"
    echo ""
}

is_valid_param () { 
    local array="$1[@]"
    local seeking="$2"
    local in=1
    for element in "${!array}"; do
        if [[ "$element" == "$seeking" ]]; then
            in=0
            break
        fi
    done
    return $in
}

for i in "$@"; do
    PARAM=$(echo "$i" | awk -F= '{print $1}')
    VALUE=$(echo "$i" | awk -F= '{print $2}')

    case "$PARAM" in
        -h | --help)
            usage
            exit
            ;;
        -t | --toolchain)
            if is_valid_param toolchains "$VALUE"; then TOOLCHAIN="$VALUE"; else echo "Invalid toolchain '$VALUE'"; fi
            ;;
        -p | --platform)
            if is_valid_param platforms "$VALUE"; then PLATFORM="$VALUE"; else echo "Invalid platform '$VALUE'"; fi
            ;;
        -i | --iface)
            if is_valid_param interfaces "$VALUE"; then INTERFACE="$VALUE"; else echo "Invalid interface '$VALUE'"; fi
            ;;
        -c | --cfile)
            if is_valid_param mbed_app_files "$VALUE"; then  MBED_APP_FILE="$VALUE"; else echo "Invalid mbed app config '$VALUE'"; fi
            ;;
        --profiling)
            TOOLCHAIN="ARMC6"
            PLATFORM="K64F"
            INTERFACE="FCE_ETHERNET_INTERFACE"
            ENABLE_RAM_PROFILING=-DFCC_MEM_STATS_ENABLED
            MBED_APP_FILE="DEFAULT"
            ;;
        *)
            echo "ERROR: unknown parameter \"$PARAM\""
            usage
            exit 1
        ;;
    esac
done

# check that necessary user params fulfilled
if [ -z "$TOOLCHAIN" ] || [ -z "$PLATFORM" ] || [ -z "$INTERFACE" ]; then
    usage
    exit
fi

if [ "$PLATFORM" = "NUCLEO_F411RE" ] && [ "$INTERFACE" != "FCE_SERIAL_INTERFACE" ]; then
	echo "Error: NUCLEO_F411RE platform supports only Serial interface"
	exit
fi
	
if  [ "LITTLE_FS"  =  "$MBED_APP_FILE" ]; then
    MBED_APP_FILE="$FCC_EXAMPLE_TOP/devenv/configs/ltfs_mbed_app.json"
    FS="LFS"; 
else
    MBED_APP_FILE="$FCC_EXAMPLE_TOP/mbed_app.json"	
    FS="FAT"
fi



# set build profile
CFLAGS="$FCC_EXAMPLE_TOP/mbed-os/tools/profiles/release.json"
if [[ ! -z "$DEBUG" ]]; then
    CFLAGS="$FCC_EXAMPLE_TOP/mbed-os/tools/profiles/debug.json"
fi


echo ""
echo "***********************************"
echo "*  TOOLCHAIN=${TOOLCHAIN}"
echo "*  PLATFORM=${PLATFORM}"
echo "*  INTERFACE=${INTERFACE}"
echo "*  CFLAGS_FILE=${CFLAGS}"
echo "*  MBED_APP_FILE=${MBED_APP_FILE}"
if [[ ! -z "$ENABLE_RAM_PROFILING" ]]; then
    echo "*  PROFILING=yes"
fi
echo "***********************************"
echo ""

# Fetch all repos
mbed deploy

# Add mbedignore in result compilation of Linux to prevent compiling this folder in mbed-os
# Without that, compilation of mbed-os after Linux or FreeRTOS will fail
if [ -d  __x86_x64_NativeLinux_mbedtls ]; then
    echo '*' >  __x86_x64_NativeLinux_mbedtls/.mbedignore
fi
if [ -d  __K64F_FreeRTOS_mbedtls ]; then
    echo '*' >  __K64F_FreeRTOS_mbedtls/.mbedignore
fi
