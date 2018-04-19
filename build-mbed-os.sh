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


cd $(dirname $0)

MY_DIR=$(pwd)
FCC_EXAMPLE_TOP=$MY_DIR

source build-mbed-os-common.sh

mbed compile \
	-v -c --artifact-name factory-configurator-client-example --build "BUILD/$PLATFORM/$TOOLCHAIN/$INTERFACE" -t "$TOOLCHAIN" -m "$PLATFORM" --profile="$CFLAGS" -D"$INTERFACE"

# Find correct bootloader
echo $PLATFORM
BOARD=$(echo $PLATFORM | tr '[:upper:]' '[:lower:]')
echo $BOARD

#create combined with bootloader executable 
python tools/combine_bootloader_with_app.py -a "BUILD/$PLATFORM/$TOOLCHAIN/$INTERFACE/factory-configurator-client-example.bin" -o "BUILD/$PLATFORM/$TOOLCHAIN/$INTERFACE/factory-configurator-client-example.hex" -m "$BOARD"
