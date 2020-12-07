// ----------------------------------------------------------------------------
// Copyright 2019-2020 ARM Ltd.
//  
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//  
//     http://www.apache.org/licenses/LICENSE-2.0
//  
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------
 #ifndef FCC_NANOCLIENT_ENABLED
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "mcc_common_setup.h"
#include "mbed-trace-helper.h"
#include "mbed-trace/mbed_trace.h"

bool fcc_mbed_trace_initialization(){
    return  mbed_trace_helper_init(TRACE_ACTIVE_LEVEL_ALL | TRACE_MODE_COLOR, false);
}

bool fcc_platform_initialization() {
    if ( mcc_platform_init() != 0){
        return false;
    }
    return true;
}

bool fcc_run_program (void (*func)(void)){
    return mcc_platform_run_program(func);
}

void fcc_platform_sw_build_info(){
    mcc_platform_sw_build_info();
}

bool fcc_platform_storage_init( void ){
    if ( mcc_platform_storage_init() != 0){
        return false;
    }
    return true;
}

void fcc_mbed_trace_free( void ){
    mbed_trace_helper_finish();
}
#endif //FCC_NANOCLIENT_ENABLED

