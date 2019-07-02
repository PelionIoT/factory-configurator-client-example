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

#include <cstdlib>

#include "fce_common_helper.h"
#include "ftcd_comm_socket.h"
#include "pv_macros.h"
#include "pv_log.h"

#ifndef ETHERNET_LINUX_IFACE_NAME
#error ETHERNET_LINUX_IFACE_NAME is undefined, please specify network ethernet interface, i.e.: ETHERNET_LINUX_IFACE_NAME=eth0
#endif

static const char *g_socket_iface = ETHERNET_LINUX_IFACE_NAME;


FtcdCommBase *fce_create_comm_interface(void)
{
    const uint8_t msg_header_token[] = FTCD_MSG_HEADER_TOKEN_FCC;
    const char *env_socket_iface = NULL;

    // Check if "ETHERNET_LINUX_IFACE_NAME" exists in the system environment, if yes - take it
    // otherwise use the one selected during the build process, please check the CMakeLists.txt in the example's root tree directory.
    env_socket_iface = std::getenv("ETHERNET_LINUX_IFACE_NAME");
    if ((env_socket_iface != NULL) && (pv_str_n_len(env_socket_iface, 1) > 0)) {
        SA_PV_LOG_INFO("env_socket_iface=%s", env_socket_iface);
        g_socket_iface = env_socket_iface;
    }

    return new FtcdCommSocket(g_socket_iface, FTCD_IPV4, FTD_SERVER_TCP_PORT, FTCD_COMM_NET_ENDIANNESS_LITTLE, msg_header_token, true);
}

void fce_destroy_comm_interface(void)
{
}
