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
 #ifndef FACTORY_CLIENT_EXAMPLE_UTILS_H
 #define FACTORY_CLIENT_EXAMPLE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifndef FCC_NANOCLIENT_ENABLED
#include "mcc_common_setup.h"
#include "mbed-trace-helper.h"
#endif
#include "factory_configurator_client.h"
#include "mbed-trace/mbed_trace.h"

bool fcc_mbed_trace_initialization( void );

bool fcc_platform_initialization( void );

bool fcc_run_program( void (*func)(void));

void fcc_platform_sw_build_info( void );

bool fcc_platform_storage_init( void );

void fcc_mbed_trace_free( void );

#endif// FACTORY_CLIENT_EXAMPLE_UTILS_H