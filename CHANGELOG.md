# Changelog for Factory Configurator Client example application

## Release 4.11.0

- Updated to Mbed OS 6.14.0.
- * Updated Mbed TLS to 2.27.0 in `pal-platform`.

## Release 4.10.0 (30.06.2021)

* Updated to Mbed OS 6.12.0.

## Release 4.9.0 (21.05.2021)

* Updated to Mbed OS 6.9.0.
* [LINUX] Updated Mbed TLS to 2.25.0.

## Release 4.8.0 (24.03.2021)

* Updated to Mbed OS 6.8.0.
* Updated cURL to 7.75.0 in `pal-platform`.
* Updated parsec-se-driver to 0.4.0.
* Removed support for SXOS platform in the application.
* K66F PSA has been removed.
* DISCO_L475VG_IOT01A target bootloader was increased from 36kB to 38kB.
* Introduction of upgraded bootloader that is coming together with upgraded Update client.
  * Upgraded update client bootloaders are located in prebuilt-bl folder. Legacy bootloaders are located in tools folder.

## Release 4.7.1 (18.01.2021)
No changes

## Release 4.7.0 (07.12.2020)

* Updated to Mbed OS 6.5.0.
* Added the `MIMXRT1050_EVK` target.
* `NUCLEO_F411RE` targets now use the Mbed TLS non-volatile seed mechanism instead of `mbedtls_psa_inject_entropy`. This reduces the ROM size of these targets by 4KiB.
  * All targets use standard C libraries instead of the small C libraries. This prevents potential issues with certain drivers that may have dependencies on `c_lib` implementation. Future releases will further optimize the configurations.
  * `minimal-printf` is enabled by default in Mbed OS 6. This also means that `floating-point` support is disabled by default. For information about changing these configurations, see `minimal-printf` in the Mbed OS documentation.
* Explicitly defined the minimum programming size (`qspif.QSPI_MIN_PROG_SIZE`) for `DISCO_L475VG_IOT01A` to be `1`. In Mbed OS, the default QSPIF value was changed to 256, which breaks backward compatibility with existing bootloaders (see https://github.com/ARMmbed/mbed-os/issues/13795).
* Fixed bugs that caused firmware updates on `DISCO_L475VG_IOT01A` to fail after the factory provisioning flow.
* Removed support for targets that Mbed OS 6 no longer supports:
    * LPC55S69.
    * Ublox EVK Odin W2.
    * Nucleo F411RE as a Platform Security Architecture (PSA) target.    
    * Nucleo F303RE. Nucleo F411RE is the reference for non-TRNG usage with Device Management Client.

## Release 4.6.0 (24.08.2020)

* pal-platform: Updated Mbed TLS to 2.22.0 in `pal-platform`.
* Added support for PARSEC. To compile FCCE with PARSEC, set the `PARSEC_TPM_SE_SUPPORT` CMake flag to `ON` and use the `linux-psa-config.cmake` configuration file.
  In this configuration, the device bootstrap private key is generated on the Trusted Platform Module (TPM) during factory provisioning.

## Release 4.5.0 (04.06.2020)

* **Breaking changes** (Due to update of SE ATECC608A driver , the application is not compatible with previous releases of SE ATECC608A driver).
  * Updated SE ATECC608A driver COMPONENT_ATECC608A.lib.
  * Updated mbed-cloud-client-platform-common platform.lib - includes adaptation for new SE ATECC608A driver.

## Release 4.4.0 (08.04.2020)

* Updated to Mbed OS 5.15.1.

## Release 4.3.0 (06.02.2020)

* Added support for Atmel secure element (ATECC608A) with K64F target board (`mbed_app_atmel_conf.json`). Atmel SE holds pre-provisioned bootstrap key and certificate that Device Management Client uses for secure connection with Device Management.
* Platform-application-specific code moved from `mbed-cloud-client-common-platform` folder to `source\platform` folder.

## Release 4.2.1 (20.12.2019)

No changes.

## Release 4.2.0 (18.12.2019)

* Added support for Nucleo F303RE
* [Mbed OS] Updated to Mbed OS 5.15.0.

## Release 4.1.0 (28.11.2019)

* Bugfix: The storage is deleted only after the first message is received from the Factory Tool and not upon initialization.
* [Mbed OS] Updated to Mbed OS 5.14.2.

## Release 4.0.0 (25.09.2019)

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
