// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
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
 
// Note: this macro is needed on armcc to get the the PRI*32 macros
// from inttypes.h in a C++ code.
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "mcc_common_setup.h"
#include "factory_configurator_client.h"
#include "ftcd_comm_base.h"
#include "fce_common_helper.h"
#include "mbed-trace/mbed_trace.h"
#include "mbed-trace-helper.h"
#include "fcc_malloc.h"
#include "fcc_stats.h"
#include "fcc_bundle_handler.h"

#define TRACE_GROUP     "fce"  // Maximum 4 characters

static int factory_example_success = EXIT_FAILURE;

/**
* Device factory flow
* - Runs in a task of its own
*/
static void factory_flow_task()
{
    bool success;
    fcc_status_e fcc_status = FCC_STATUS_SUCCESS;

    FtcdCommBase *ftcd_comm = NULL;
    ftcd_comm_status_e ftcd_comm_status = FTCD_COMM_STATUS_SUCCESS;
    ftcd_comm_status_e ftcd_comm_status_first_err = FTCD_COMM_STATUS_SUCCESS;
    uint8_t *input_message = NULL;
    uint32_t input_message_size = 0;
    uint8_t *response_message = NULL;
    size_t response_message_size = 0;
    bool is_storage_deleted = false;

#if defined(__SXOS__)
    mcc_platform_do_wait(3000);
#else
    setvbuf(stdout, (char *)NULL, _IONBF, 0); /* Avoid buffering on test output */
#endif

    mcc_platform_sw_build_info();

    // Initialize storage
    success = mcc_platform_storage_init() == 0;
    if (success != true) {
        tr_error("Failed initializing mcc platform storage\n");
        return;
    }

    fcc_status = fcc_init();
    if (fcc_status != FCC_STATUS_SUCCESS) {
        tr_error("Failed initializing factory configurator client\n");
        return;
    }

    // Create communication interface object
    ftcd_comm = fce_create_comm_interface();
    if (ftcd_comm == NULL) {
        tr_error("Failed creating communication object\n");
        goto out1;
    }

    //init ftcd_comm object
    success = ftcd_comm->init();
    if (success != true) {
        tr_error("Failed instantiating communication object\n");
        goto out2;
    }

    mbed_tracef(TRACE_LEVEL_CMD, TRACE_GROUP, "Factory flow begins...");

    while (true) {
        factory_example_success = EXIT_FAILURE;
        // wait for message from communication layer
        ftcd_comm_status = ftcd_comm->wait_for_message(&input_message, &input_message_size);
        if (ftcd_comm_status == FTCD_COMM_STATUS_SUCCESS) {

            // If this is the first message and the storage wasn't deleted yet, delete the storage.
            // The call to this function should be after wait_for_message success, to prevent a case when the storage is deleted after resetting the board.
            // This way we make sure that the storage is deleted only when factory flow is actually running.
            if (!is_storage_deleted) {
                fcc_status = fcc_storage_delete();
                if (fcc_status != FCC_STATUS_SUCCESS) {
                    tr_error("Failed to reset storage\n");
                    goto out2;
                }
                is_storage_deleted = true;
                mbed_tracef(TRACE_LEVEL_CMD, TRACE_GROUP, "Storage is erased");
            }           

            // process request and get back response
            fcc_status = fcc_bundle_handler(input_message, input_message_size, &response_message, &response_message_size);
            if ((fcc_status == FCC_STATUS_BUNDLE_RESPONSE_ERROR) || (response_message == NULL) || (response_message_size == 0)) {
                ftcd_comm_status = FTCD_COMM_FAILED_TO_PROCESS_DATA;
                mbed_tracef(TRACE_LEVEL_CMD, TRACE_GROUP, "Failed to process data");
            }
        } else {
            tr_error("Failed getting factory message");
        }

        ftcd_comm_status_first_err = ftcd_comm_status;
        ftcd_comm_status = ftcd_comm->send_response(response_message, response_message_size, ftcd_comm_status);
        if (ftcd_comm_status != FTCD_COMM_STATUS_SUCCESS) {
            ftcd_comm->send_response(NULL, 0, ftcd_comm_status);
            if (ftcd_comm_status_first_err == FTCD_COMM_STATUS_SUCCESS) {
                ftcd_comm_status_first_err = ftcd_comm_status;
            }
        }

        if (input_message) {
            fcc_free(input_message);
        }
        if (response_message) {
            fcc_free(response_message);
        }

        if (ftcd_comm_status_first_err == FTCD_COMM_STATUS_SUCCESS) {
            // Success
            mbed_tracef(TRACE_LEVEL_CMD, TRACE_GROUP, "Successfully processed comm message");
            factory_example_success = EXIT_SUCCESS;
        }

        if (fcc_is_session_finished()) {
            break;
        }
    }

out2:
    ftcd_comm->finish();
    delete ftcd_comm;
    fce_destroy_comm_interface();

out1:

    fcc_status = fcc_finalize();
    if (fcc_status != FCC_STATUS_SUCCESS) {
        tr_error("Failed finalizing factory client\n");
    }

    if (factory_example_success == EXIT_SUCCESS) {
        mbed_tracef(TRACE_LEVEL_CMD, TRACE_GROUP, "Successfully completed factory flow");
    }
    mbed_trace_helper_finish();

    fflush(stdout);
}

/**
* Example main
*/
#if defined(__SXOS__)
extern "C"
int mbed_cloud_application_entrypoint(void)
#else
int main(int argc, char * argv[])
#endif
{
    bool success = false;

    // careful, mbed-trace initialization may happen at this point if and only if we 
    // do NOT use mutex by passing "true" at the second param for this functions.
    // In case mutex is used, this function MUST be moved *after* pal_init()
    success = mbed_trace_helper_init(TRACE_ACTIVE_LEVEL_ALL | TRACE_MODE_COLOR, false);
    if (!success) {
        // Nothing much can be done here, trace module should be initialized before file system
        // and if failed - no tr_* print is eligible.
        return EXIT_FAILURE;
    }

    success = false;

    success = (mcc_platform_init() == 0);
    if (success) {
        success = mcc_platform_run_program(&factory_flow_task);
    }

    // Print dynamic RAM statistics in case ENABLE_RAM_PROFILING cflag introduced
    fcc_stats_print_summary();

    return success ? factory_example_success : EXIT_FAILURE;
}
