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

#include "PlatIncludes.h"
#include "pal_BSP.h"

#include "pal.h"
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
static pal_args_t g_args = { 0 };

// Set primary and backup File System  path using MBED_CLOUD_CLIENT_CONFIG_DIR and MBED_CLOUD_CLIENT_CONFIG_BACKUP_DIR definitions
#if defined(MBED_CLOUD_CLIENT_CONFIG_DIR) || defined(MBED_CLOUD_CLIENT_CONFIG_BACKUP_DIR)
static int setup_file_system_dir(void)
{
    palStatus_t pal_status = PAL_SUCCESS;
    pal_fsStorageID_t  dataID; 


#ifdef MBED_CLOUD_CLIENT_CONFIG_DIR
    //Create directory
    pal_status = pal_fsMkDir(MBED_CLOUD_CLIENT_CONFIG_DIR);
    if (pal_status != PAL_SUCCESS) {
        if (pal_status != PAL_ERR_FS_NAME_ALREADY_EXIST) {
            tr_error("setup_file_system_dir: can't pal_fsMkdir %s - error %ld", MBED_CLOUD_CLIENT_CONFIG_DIR, pal_status);
            return pal_status;
        }
    }

    //Make mount for primary partition.
    dataID = PAL_FS_PARTITION_PRIMARY;
    pal_status = pal_fsSetMountPoint(dataID, MBED_CLOUD_CLIENT_CONFIG_DIR);
    if (pal_status != PAL_SUCCESS) {
        tr_error("setup_file_system_dir: - can't set fsSetMountPoint to PAL (%d, %s)\n", (int)dataID, MBED_CLOUD_CLIENT_CONFIG_DIR);
        return pal_status;

    }
    tr_info("Primary config dir/mountpoint set to %s", MBED_CLOUD_CLIENT_CONFIG_DIR);

#endif

#ifdef MBED_CLOUD_CLIENT_CONFIG_BACKUP_DIR
    pal_status = pal_fsMkDir(MBED_CLOUD_CLIENT_CONFIG_BACKUP_DIR);
    if (pal_status != PAL_SUCCESS) {
        if (pal_status != PAL_ERR_FS_NAME_ALREADY_EXIST) {
            tr_error("setup_file_system_dir: can't pal_fsMkdir %s - error %d", MBED_CLOUD_CLIENT_CONFIG_BACKUP_DIR, pal_status);
            return pal_status;
        }
    }

    dataID = PAL_FS_PARTITION_SECONDARY;
    pal_status = pal_fsSetMountPoint(dataID, MBED_CLOUD_CLIENT_CONFIG_BACKUP_DIR);
    if (pal_status != PAL_SUCCESS) {
        tr_error("setup_file_system_dir: can't set fsSetMountPoint to PAL (%d, %s)", (int)dataID, MBED_CLOUD_CLIENT_CONFIG_BACKUP_DIR);
        return pal_status;
    }
    tr_info("Secondary config dir/mountpoint set to %s", MBED_CLOUD_CLIENT_CONFIG_BACKUP_DIR);
#endif

    return 0;
}
#else
/* This is the default case -  File System main and backup directories configured by pal/esfs using default paths*/
static int setup_file_system_dir(void)
{
    return 0;
}
#endif

/**
* Device factory flow
* - Runs in a task of its own
*/
static void factory_flow_task(pal_args_t *args)
{
    bool success;
    fcc_status_e fcc_status = FCC_STATUS_SUCCESS;
    palStatus_t pal_status;
    int res;

    FtcdCommBase *ftcd_comm = NULL;
    ftcd_comm_status_e ftcd_comm_status = FTCD_COMM_STATUS_SUCCESS;
    ftcd_comm_status_e ftcd_comm_status_first_err = FTCD_COMM_STATUS_SUCCESS;
    uint8_t *input_message = NULL;
    uint32_t input_message_size = 0;
    uint8_t *response_message = NULL;
    size_t response_message_size = 0;

    // Unused parameter
    (void)(args);

    fcc_status = fcc_init();
    if (fcc_status != FCC_STATUS_SUCCESS) {
        tr_error("Failed initializing factory configurator client\n");
        return;
    }

    res = setup_file_system_dir();
    if (res != 0) {
        tr_error("setup_file_system_dir failed with status %d! - exit\n", res);
        goto out0;
    }

    setvbuf(stdout, (char *)NULL, _IONBF, 0); /* Avoid buffering on test output */

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

    // wait for message from communication layer
    ftcd_comm_status = ftcd_comm->wait_for_message(&input_message, &input_message_size);
    if (ftcd_comm_status == FTCD_COMM_STATUS_SUCCESS) {

        fcc_status = fcc_storage_delete();
        if (fcc_status != FCC_STATUS_SUCCESS) {
            tr_error("Failed to reset storage\n");
            goto out2;
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

out2:
    ftcd_comm->finish();
    delete ftcd_comm;
    fce_destroy_comm_interface();

out1:
    mbed_trace_helper_finish();

out0:
    fcc_status = fcc_finalize();
    if (fcc_status != FCC_STATUS_SUCCESS) {
        tr_error("Failed finalizing factory client\n");
    }

    fflush(stdout);
}


/**
* Example main
*/
int main(int argc, char * argv[])
{
    bspStatus_t bsp_result = BSP_SUCCESS;
    bool success = false;

    g_args.argc = 0;
    g_args.argv = NULL;

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

    bsp_result = initPlatform(NULL);
    if (bsp_result == BSP_SUCCESS) {
       // setvbuf(stdout, (char *)NULL, _IONBF, 0); /* Avoid buffering on test output */
        success = runProgram(&factory_flow_task, &g_args);
    }

    // Print dynamic RAM statistics in case ENABLE_RAM_PROFILING cflag introduced
    fcc_stats_print_summary();

    return success ? factory_example_success : EXIT_FAILURE;
}
