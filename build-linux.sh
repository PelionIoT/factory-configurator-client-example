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

# set default BUILD_TYPE=Debug
BUILD_TYPE=${1:-Debug}

# deploy components
mbed deploy

# deploy repositories and generate CMake build files
python pal-platform/pal-platform.py -v deploy --target=x86_x64_NativeLinux_mbedtls generate

# all auto generated makefiles goes here
cd __x86_x64_NativeLinux_mbedtls

# generate Makefiles
cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_TOOLCHAIN_FILE=../pal-platform/Toolchain/GCC/GCC.cmake -DEXTARNAL_DEFINE_FILE=../linux-config.cmake

# build the example target
make factory-configurator-client-example.elf
