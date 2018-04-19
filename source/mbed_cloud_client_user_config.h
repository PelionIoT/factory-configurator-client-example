// ----------------------------------------------------------------------------
// Copyright 2017-2018 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
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

#ifndef MBED_CLOUD_CLIENT_USER_CONFIG_H
#define MBED_CLOUD_CLIENT_USER_CONFIG_H

// Defines to pass MbedCloudClientConfigCheck.h checks (the values are meaningless)

#define MBED_CLOUD_CLIENT_ENDPOINT_TYPE                                "default"
#define MBED_CLOUD_CLIENT_SUPPORT_UPDATE
#define MBED_CLOUD_CLIENT_LIFETIME                                     0
#define SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE                             128
#define MBED_CLOUD_CLIENT_UPDATE_BUFFER                                1024
#define MBED_CLOUD_CLIENT_TRANSPORT_MODE_TCP

#endif /* MBED_CLOUD_CLIENT_USER_CONFIG_H */
