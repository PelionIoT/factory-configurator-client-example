# Changelog for Factory Configurator Client example application

## Release 4.0.0 

* [Mbed OS] Updated to Mbed OS 5.14.0.
* [Mbed OS] We now officially support Platform Security Architecture (PSA) on V7 single boards, including K64F, K66F, and NUCLEO_F411RE.
* [Mbed OS] Added support for the Discovery L475VG IOT01A target board.
* [Mbed OS] Removed `CY8CKIT_062_WIFI_BT_PSA`, which is no longer supported starting from Mbed OS 5.13.1.  
* [Linux] Updated to Mbed TLS 2.19.1.
* Added support for UNISOC SXOS SDK v8p2.1 for the UIS8908A NB-IoT board.
 

## Release 3.4.0 (28.08.2019)

* [Linux] Updated to Mbed TLS 2.18.1.
* [Mbed OS] Removed the legacy ESFS-SOTP configurations from the applications. Only KVStore is supported for client storage.

## Release 3.3.0 (20.06.2019)

* Updated to Mbed OS 5.13
* Build FCCE with PSA over `Linux_x86` for development and testing purposes. There is no security underneath. Please refer to FCCE documentation (part of the FCU zip file) for detailed build instructions.

## Release 3.2.0 (31.05.2019)

* Updated to Mbed OS 5.12.4.
* [Linux] network Ethernet interface information is available in the `ETHERNET_LINUX_IFACE_NAME` environment variable.

