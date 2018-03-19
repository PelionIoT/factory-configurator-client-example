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


#include "fce_common_helper.h"
#include "ftcd_comm_socket.h"
#include "PlatIncludes.h"

static const void *g_socket_iface = palTestGetNetWorkInterfaceContext();

FtcdCommBase *fce_create_comm_interface(void)
{
    const uint8_t msg_header_token[] = FTCD_MSG_HEADER_TOKEN_FCC;
    return new FtcdCommSocket(g_socket_iface, FTCD_IPV4, FTD_SERVER_TCP_PORT, FTCD_COMM_NET_ENDIANNESS_LITTLE, msg_header_token, true);
}

void fce_destroy_comm_interface(void)
{
}
