// ----------------------------------------------------------------------------
// Copyright 2017-2018 ARM Ltd.
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

#ifdef FCE_SERIAL_INTERFACE

#include "fce_common_helper.h"
#include "ftcd_comm_serial.h"

FtcdCommBase *fce_create_comm_interface(void)
{
    const uint8_t msg_header_token[] = FTCD_MSG_HEADER_TOKEN_FCC;
    return new FtcdCommSerial(FTCD_COMM_NET_ENDIANNESS_LITTLE, msg_header_token, true);
}

void fce_destroy_comm_interface(void)
{
}

#endif // FCE_SERIAL_INTERFACE
