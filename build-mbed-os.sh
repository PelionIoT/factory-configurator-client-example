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
	-v -c --artifact-name factory-configurator-client-example --build "BUILD/$PLATFORM/$TOOLCHAIN/$INTERFACE/$FS" -t "$TOOLCHAIN" -m "$PLATFORM" --profile="$CFLAGS" -D"$INTERFACE" --app-config "$MBED_APP_FILE"

# Find correct bootloader
echo $PLATFORM
BOARD=$(echo $PLATFORM | tr '[:upper:]' '[:lower:]')
echo $BOARD

# This may cause issues when compiling with different mbed_app.json. 
# Currenlty there is no use-case for thiss, so only Warning is printed.
# Should be resolved once Jira ticket IOTUC-499 will be solved
echo -e "\033[33mWARNING: json file that is used in combine script is mbed_app.json!!!\033[0m"

#create combined with bootloader executable 
python tools/combine_bootloader_with_app.py -a "BUILD/$PLATFORM/$TOOLCHAIN/$INTERFACE/$FS/factory-configurator-client-example.bin" -o "BUILD/$PLATFORM/$TOOLCHAIN/$INTERFACE/$FS/factory-configurator-client-example.hex" -m "$BOARD"

