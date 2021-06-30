Securing Peripherals on i.MX using OP-TEE
========================

This chapter describes the process of configuring an i.MX peripheral for secure access using OP-TEE only.  It also describes the Windows behavior toward i.MX peripherals.

Note: In the text to follow, we assume you are familiar with the required build tools, general [boot flow](#boot-sequence), and process to [build ARM32 firmware](#build-arm32-firmware) or [build ARM64 firmware](#build-arm64-firmware).

## OP-TEE
Locking down specific peripherals for secure access occurs during the OP-TEE portion of boot, when OP-TEE configures the CSU.

By default, the CSU registers are initialized to allow access from both normal and secure world for all peripherals.

To override this default configuration, add an override entry to the access_control global array.  You can find this array in optee_os/core/arch/arm/plat-imx/imx6.c


    static struct csu_csl_access_control access_control[] = {
        /* TZASC1   - CSL16 [7:0] */
        /* TZASC2   - CSL16 [23:16] */
        {16, ((CSU_TZ_SUPERVISOR << 0)  | (CSU_TZ_SUPERVISOR << 16))},
    }

The first field is the CSU CSL register index to secure the required device.  The second field is the required CSU CSL register value.  This value will override the default CSU initialization value.  OP-TEE provides some useful defines to create this value:

    /*
     * Grant R+W access:
     * - Just to TZ Supervisor execution mode, and
     * - Just to a single device
     */
    #define CSU_TZ_SUPERVISOR		0x22

    /*
     * Grant R+W access:
     * - To all execution modes, and
     * - To a single device
     */
    #define CSU_ALL_MODES			0xFF

Note: Each CSU CSL register is responsible for two peripheral devices.  You must set the override value carefully to ensure you are securing the intended peripheral device.

## Windows
Any access to a secure peripheral from a non-secure environment will cause system failure. To avoid this scenario, we have added code into the PEP driver to automatically read the CSU registers and determine if a Windows-enabled peripheral can be interacted with from a non-secure environment. If it can't, Windows will automatically hide the secured peripheral to avoid the potential system failure.
