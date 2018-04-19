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

#ifdef FCE_ETHERNET_INTERFACE

#include "fce_common_helper.h"
#include "EthernetInterface.h"
#include "ftcd_comm_socket.h"

EthernetInterface g_socket_iface;

FtcdCommBase *fce_create_comm_interface(void)
{
    // Opens Ethernet interface connection
    int rc = g_socket_iface.connect();
    if (rc != 0) {
        printf("Failed connecting to Ethernet interface!\n");
        return NULL;
    }

    const uint8_t msg_header_token[] = FTCD_MSG_HEADER_TOKEN_FCC;
    return new FtcdCommSocket(&g_socket_iface, FTCD_IPV4, FTD_SERVER_TCP_PORT, FTCD_COMM_NET_ENDIANNESS_LITTLE,msg_header_token, true);
}

void fce_destroy_comm_interface(void)
{
    g_socket_iface.disconnect();
}

#endif //FCE_ETHERNET_INTERFACE
