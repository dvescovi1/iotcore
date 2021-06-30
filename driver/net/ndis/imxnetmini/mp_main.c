/*
* Copyright 2019 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the disclaimer
* below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* * Neither the name of NXP nor the names of its contributors may be used to
* endorse or promote products derived from this software without specific prior
* written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS
* LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "precomp.h"

extern NDIS_OID ENETSupportedOids[];
extern ULONG    ENETSupportedOidsSize;
extern ENET_PHY_CMD AR8031Config;  /* TODO */

// Global var of ENET Ndis Handler
NDIS_HANDLE          NdisMiniportDriverHandle;
MP_MDIO_DRIVER       MDIODriver;

NDIS_STATUS DriverEntry(_In_ PVOID DriverObject, _In_ PVOID RegistryPath);

#pragma NDIS_INIT_FUNCTION(DriverEntry)
#pragma NDIS_PAGEABLE_FUNCTION(MpUnloadDriver)
#pragma NDIS_PAGEABLE_FUNCTION(MpInitializeEx)

/*++
Routine Description:
    In the context of its this function, a miniport driver associates itself with NDIS,
    specifies the NDIS version that it is using, and registers its entry points.
Arguments:
    DriverObject    pointer to the driver object.
    RegistryPath    pointer to the driver registry path.
Return Value:
    NDIS_STATUS_SUCCESS or NDIS_STATUS_FAILURE
--*/
_Use_decl_annotations_
NDIS_STATUS DriverEntry(PVOID DriverObject, PVOID RegistryPath) {
    NDIS_STATUS                           Status;
    NDIS_MINIPORT_DRIVER_CHARACTERISTICS  MpChar;

    PAGED_CODE();
    #if DBG
    KeQuerySystemTimePrecise(&DriverStartTime);
    #endif
    DBG_DRV_METHOD_BEG_WITH_PARAMS("Driver: 0x%016p, '%S')", DriverObject, ((PUNICODE_STRING)RegistryPath)->Buffer);
    DBG_DRV_PRINT_INFO("***********************************************************************************");
    DBG_DRV_PRINT_INFO("*** NXP ndis miniport driver, date: %s %s                        ***", __DATE__, __TIME__);
    DBG_DRV_PRINT_INFO("***********************************************************************************");
    do {
        RtlZeroMemory(&MDIODriver, sizeof(MP_MDIO_DRIVER));
        #if DBG
        RtlStringCbPrintfA(MDIODriver.MDIODrv_DeviceName, sizeof(MDIODriver.MDIODrv_DeviceName) - 1, "ENETx");
        #endif
        NdisAllocateSpinLock(&MDIODriver.MDIODrv_DriverSpinLock);
        // Fill-in adapter characteristics before calling NdisMRegisterMiniport
        NdisZeroMemory(&MpChar, sizeof(MpChar));
        {C_ASSERT(sizeof(MpChar) >= NDIS_SIZEOF_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_2); }
        MpChar.Header.Type                  = NDIS_OBJECT_TYPE_MINIPORT_DRIVER_CHARACTERISTICS;
        MpChar.Header.Size                  = NDIS_SIZEOF_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_2;
        MpChar.Header.Revision              = NDIS_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_2;
        MpChar.MajorNdisVersion             = ENET_NDIS_MAJOR_VERSION;
        MpChar.MinorNdisVersion             = ENET_NDIS_MINOR_VERSION;
        MpChar.MajorDriverVersion           = NIC_MAJOR_DRIVER_VERSION;
        MpChar.MinorDriverVersion           = NIC_MINOR_DRIVER_VERISON;
        MpChar.InitializeHandlerEx          = MpInitializeEx;
        MpChar.HaltHandlerEx                = MpHaltEx;
        MpChar.UnloadHandler                = MpUnloadDriver;
        MpChar.PauseHandler                 = MpPause;
        MpChar.RestartHandler               = MpRestart;
        MpChar.OidRequestHandler            = MpOidRequest;
        MpChar.SendNetBufferListsHandler    = MpSendNetBufferLists;
        MpChar.ReturnNetBufferListsHandler  = MpReturnNetBufferLists;
        MpChar.CancelSendHandler            = MpCancelSendNetBufferLists;
        MpChar.CheckForHangHandlerEx        = MpCheckForHangEx;
        MpChar.ResetHandlerEx               = MpResetEx;
        MpChar.DevicePnPEventNotifyHandler  = MpDevicePnPEventNotify;
        MpChar.ShutdownHandlerEx            = MpShutdownEx;
        MpChar.CancelOidRequestHandler      = MpCancelOidRequest;
        // Now register Miniport
        Status = NdisMRegisterMiniportDriver(DriverObject, RegistryPath, NULL, &MpChar, &NdisMiniportDriverHandle);
        if (NDIS_STATUS_SUCCESS != Status) {
            Status = NDIS_STATUS_FAILURE;
            DBG_DRV_PRINT_ERROR_WITH_STATUS("NdisMRegisterMiniportDriver() failed.");
            break;
        }
    }  while (0);
    DBG_DRV_METHOD_END_WITH_STATUS(Status);
    return (Status);
}

/*++
Routine Description:
    NDIS MiniportUnloadDriver callback handler.
    NDIS calls MiniportUnloadDriver at IRQL = PASSIVE_LEVEL.
Arguments:
    DriverObject Not used
Return Value:
    None
--*/
_Use_decl_annotations_
VOID MpUnloadDriver(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);

    PAGED_CODE();
    DBG_DRV_METHOD_BEG();
    NdisMDeregisterMiniportDriver(NdisMiniportDriverHandle);  // Deregister Miniport driver
    DBG_DRV_METHOD_END();
}

/*++
Routine Description:
    This function finds and initializes the ENET adapter. When the ENET driver calls
    the NdisMRegisterMiniport from its DriverEntry function, NDIS will call
    ENETInitialize in the context of NdisMRegisterMiniport.
    NDIS calls MiniportInitializeEx at IRQL = PASSIVE_LEVEL.
Parameters:
    MiniportAdapterHandle
        Specifies a handle identifying the ENET driver, which is assigned by
        the NDIS library. It is a required parameter in subsequent calls
        to NdisXXX functions.
    MiniportDriverContext
        Specifies a handle used only during initialization for calls to
        NdisXXX configuration and initialization functions. For example,
        this handle is passed to NDIS when we registered the driver
    MiniportInitParameters
        Initialization parameters
Returns:
    NDIS_STATUS_SUCCESS if initialization is success.
    NDIS_STATUS_FAILURE or NDIS_STATUS_UNSUPPORTED_MEDIA if not.

*/
_Use_decl_annotations_
NDIS_STATUS MpInitializeEx(NDIS_HANDLE MiniportAdapterHandle, NDIS_HANDLE MiniportDriverContext, PNDIS_MINIPORT_INIT_PARAMETERS  MiniportInitParameters)
{
    NDIS_STATUS                                     Status = NDIS_STATUS_SUCCESS;
    NDIS_MINIPORT_INTERRUPT_CHARACTERISTICS         Interrupt;
    NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES   RegistrationAttributes;
    NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES        GeneralAttributes;
    NDIS_PM_CAPABILITIES                            PowerManagementCapabilities;
    PMP_ADAPTER                                     pAdapter = NULL;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR                 pResDesc;
    ULONG                                           index;
    UNREFERENCED_PARAMETER(MiniportDriverContext);

    PAGED_CODE();

    DBG_ENET_DEV_METHOD_BEG_WITH_PARAMS("Adapter handle: 0x%p", MiniportAdapterHandle);
    for(;;) {
        // Allocate MP_ADAPTER structure
        if ((Status = MpAllocAdapterBlock(&pAdapter, MiniportAdapterHandle)) != NDIS_STATUS_SUCCESS) {
            DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("MpAllocAdapterBlock() failed.");
            break;
        }
        #if DBG
        RtlStringCbPrintfA(pAdapter->ENETDev_DeviceName, sizeof(pAdapter->ENETDev_DeviceName) - 1, "ENET_%d", NdisInterlockedIncrement(&MDIODriver.MDIODrv_MacDeviceCount) - 1);
        RtlStringCbCopyA(pAdapter->ENETDev_PHYDevice.PHYDev_DeviceName, sizeof(pAdapter->ENETDev_PHYDevice.PHYDev_DeviceName) - 1, pAdapter->ENETDev_DeviceName);
        #endif
        // Register miniport
        NdisZeroMemory(&RegistrationAttributes, sizeof(NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES));
        NdisZeroMemory(&GeneralAttributes, sizeof(NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES));
        RegistrationAttributes.Header.Type               = NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES;
        RegistrationAttributes.Header.Revision           = NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES_REVISION_1;
        RegistrationAttributes.Header.Size               = sizeof(NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES);
        RegistrationAttributes.MiniportAdapterContext    = (NDIS_HANDLE)pAdapter;
        RegistrationAttributes.AttributeFlags            = NDIS_MINIPORT_ATTRIBUTES_HARDWARE_DEVICE | NDIS_MINIPORT_ATTRIBUTES_BUS_MASTER | NDIS_MINIPORT_ATTRIBUTES_REGISTER_BUGCHECK_CALLBACK;
        RegistrationAttributes.CheckForHangTimeInSeconds = MAC_IS_ALIVE_POLL_SEC;
        RegistrationAttributes.InterfaceType             = NIC_INTERFACE_TYPE;
        if ((Status = NdisMSetMiniportAttributes(MiniportAdapterHandle, (PNDIS_MINIPORT_ADAPTER_ATTRIBUTES)&RegistrationAttributes)) != NDIS_STATUS_SUCCESS) {
            DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("NdisMSetMiniportAttributes() failed.");
            break;
        }
        (void)Acpi_Init(pAdapter, &pAdapter->ENETDev_bmACPISupportedFunctions);              // Initialize ACPI
        // Try to get MAC address from ACPI
        if (!NT_SUCCESS(Acpi_GetValue(pAdapter, IMX_ENET_DSM_FUNCTION_GET_MAC_ADDRESS_INDEX, pAdapter->FecMacAddress, sizeof(pAdapter->FecMacAddress)))) {
            DBG_ENET_DEV_PRINT_INFO("Acpi_GetValue(GET_MAC_ADDRESS) failed.");
        }  else {
            Status = STATUS_ACPI_INVALID_DATA;
            DBG_ENET_DEV_PRINT_INFO("ENET MAC address: %02X-%02X-%02X-%02X-%02X-%02X (from ACPI)", pAdapter->FecMacAddress[0], pAdapter->FecMacAddress[1], pAdapter->FecMacAddress[2], pAdapter->FecMacAddress[3], pAdapter->FecMacAddress[4], pAdapter->FecMacAddress[5]);
        }
        // Read the registry parameters
        if ((Status = NICReadRegParameters(pAdapter)) != NDIS_STATUS_SUCCESS) {
            DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("NICReadRegParameters() failed.");
            pAdapter->NdisInterruptHandle = NULL;  // SDV bug fix
            break;
        }
        #ifdef DBG
        if (!(pAdapter->FecMacAddress[0] || pAdapter->FecMacAddress[1] || pAdapter->FecMacAddress[2] || pAdapter->FecMacAddress[3] || pAdapter->FecMacAddress[4] || pAdapter->FecMacAddress[5])) {
            Status = NDIS_STATUS_FAILURE;
            DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS_AND_PARAMS("ENET MAC address is not valid"," %02X-%02X-%02X-%02X-%02X-%02X", pAdapter->FecMacAddress[0], pAdapter->FecMacAddress[1], pAdapter->FecMacAddress[2], pAdapter->FecMacAddress[3], pAdapter->FecMacAddress[4], pAdapter->FecMacAddress[5]);
            break;
        }
        #endif
        DBG_ENET_DEV_PRINT_INFO("ENET MAC address: %02X-%02X-%02X-%02X-%02X-%02X (used by device)", pAdapter->FecMacAddress[0], pAdapter->FecMacAddress[1], pAdapter->FecMacAddress[2], pAdapter->FecMacAddress[3], pAdapter->FecMacAddress[4], pAdapter->FecMacAddress[5]);
        // Set up generic attributes
        GeneralAttributes.Header.Type       = NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES;
        GeneralAttributes.Header.Revision   = NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES_REVISION_2;
        GeneralAttributes.Header.Size       = NDIS_SIZEOF_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES_REVISION_2;
        GeneralAttributes.MediaType         = NIC_MEDIA_TYPE;
        GeneralAttributes.MtuSize           = ETHER_FRAME_MAX_PAYLOAD_LENGTH;
        GeneralAttributes.MaxXmitLinkSpeed  = NIC_MEDIA_MAX_SPEED;
        GeneralAttributes.MaxRcvLinkSpeed   = NIC_MEDIA_MAX_SPEED;
        GeneralAttributes.XmitLinkSpeed     = NDIS_LINK_SPEED_UNKNOWN;
        GeneralAttributes.RcvLinkSpeed      = NDIS_LINK_SPEED_UNKNOWN;
        GeneralAttributes.MediaConnectState = MediaConnectStateUnknown;
        GeneralAttributes.MediaDuplexState  = MediaDuplexStateUnknown;
        GeneralAttributes.LookaheadSize     = ETHER_FRAME_MAX_PAYLOAD_LENGTH;

        NdisZeroMemory(&PowerManagementCapabilities, sizeof(PowerManagementCapabilities));
        PowerManagementCapabilities.Header.Type     = NDIS_OBJECT_TYPE_DEFAULT;
        PowerManagementCapabilities.Header.Revision = NDIS_PM_CAPABILITIES_REVISION_2;
        PowerManagementCapabilities.Header.Size     = NDIS_SIZEOF_NDIS_PM_CAPABILITIES_REVISION_2;
        GeneralAttributes.PowerManagementCapabilitiesEx = &PowerManagementCapabilities;

        GeneralAttributes.MacOptions              = NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA | NDIS_MAC_OPTION_TRANSFERS_NOT_PEND | NDIS_MAC_OPTION_NO_LOOPBACK;
        GeneralAttributes.SupportedPacketFilters  = NIC_SUPPORTED_FILTERS;
        GeneralAttributes.MaxMulticastListSize    = NIC_MAX_MCAST_LIST;
        GeneralAttributes.MacAddressLength        = ETH_LENGTH_OF_ADDRESS;
        NdisMoveMemory(GeneralAttributes.PermanentMacAddress, pAdapter->PermanentAddress, ETH_LENGTH_OF_ADDRESS);
        NdisMoveMemory(GeneralAttributes.CurrentMacAddress, pAdapter->CurrentAddress, ETH_LENGTH_OF_ADDRESS);

        GeneralAttributes.PhysicalMediumType     = NdisPhysicalMedium802_3;
        GeneralAttributes.RecvScaleCapabilities  = NULL;
        GeneralAttributes.AccessType             = NET_IF_ACCESS_BROADCAST;      // NET_IF_ACCESS_BROADCAST for a typical ethernet adapter
        GeneralAttributes.DirectionType          = NET_IF_DIRECTION_SENDRECEIVE; // NET_IF_DIRECTION_SENDRECEIVE for a typical ethernet adapter
        GeneralAttributes.ConnectionType         = NET_IF_CONNECTION_DEDICATED;  // NET_IF_CONNECTION_DEDICATED for a typical ethernet adapter
        GeneralAttributes.IfType                 = IF_TYPE_ETHERNET_CSMACD;      // IF_TYPE_ETHERNET_CSMACD for a typical ethernet adapter (regardless of speed)
        GeneralAttributes.IfConnectorPresent     = TRUE; // RFC 2665 TRUE if physical adapter
        GeneralAttributes.SupportedStatistics = NDIS_STATISTICS_RCV_DISCARDS_SUPPORTED |
                                                NDIS_STATISTICS_RCV_ERROR_SUPPORTED |
                                                NDIS_STATISTICS_BYTES_RCV_SUPPORTED |
                                                NDIS_STATISTICS_DIRECTED_FRAMES_RCV_SUPPORTED |
                                                NDIS_STATISTICS_MULTICAST_FRAMES_RCV_SUPPORTED |
                                                NDIS_STATISTICS_BROADCAST_FRAMES_RCV_SUPPORTED |
                                                NDIS_STATISTICS_BYTES_XMIT_SUPPORTED |
                                                NDIS_STATISTICS_DIRECTED_FRAMES_XMIT_SUPPORTED |
                                                NDIS_STATISTICS_MULTICAST_FRAMES_XMIT_SUPPORTED |
                                                NDIS_STATISTICS_BROADCAST_FRAMES_XMIT_SUPPORTED |
                                                NDIS_STATISTICS_XMIT_ERROR_SUPPORTED |
                                                NDIS_STATISTICS_XMIT_DISCARDS_SUPPORTED;
            //NDIS_STATISTICS_GEN_STATISTICS_SUPPORTED; // it is not in documentation
        GeneralAttributes.SupportedOidList       = ENETSupportedOids;
        GeneralAttributes.SupportedOidListLength = ENETSupportedOidsSize;

        if ((Status = NdisMSetMiniportAttributes(MiniportAdapterHandle, (PNDIS_MINIPORT_ADAPTER_ATTRIBUTES)&GeneralAttributes)) != NDIS_STATUS_SUCCESS) {
            DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("NdisMSetMiniportAttributes() failed.");
            break;
        }
        // Get HW resources
        NDIS_PHYSICAL_ADDRESS  ENETRegBase;
        ULONG                  ENETRegLength = 0;
        MP_MDIO_DEVICE_CFG     MDIODevCfg;
        BOOLEAN                GotInterrupt = FALSE;
        ENETRegBase.QuadPart = 0;
        MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress.QuadPart = 0;
        if (MiniportInitParameters->AllocatedResources) {
            for (index = 0; index < MiniportInitParameters->AllocatedResources->Count; index++) {
                pResDesc = &MiniportInitParameters->AllocatedResources->PartialDescriptors[index];
                switch (pResDesc->Type) {
                    case CmResourceTypeInterrupt:
                        DBG_CODE(if (!GotInterrupt) pAdapter->ENETDev_Irq = pResDesc->u.Interrupt.Level);
                        GotInterrupt = TRUE;
                        DBG_ENET_DEV_PRINT_INFO("InterruptLevel  = 0x%x", pResDesc->u.Interrupt.Level);
                        DBG_ENET_DEV_PRINT_INFO("InterruptVector = 0x%x", pResDesc->u.Interrupt.Vector);
                        break;
                    case CmResourceTypeMemory:
                        if (ENETRegBase.QuadPart == 0) {
                            ENETRegBase   = pResDesc->u.Memory.Start;
                            ENETRegLength = pResDesc->u.Memory.Length;
                        } else if (MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress.QuadPart == 0) {
                            MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress = pResDesc->u.Memory.Start;
                        }
                        break;
                    default:
                        break;
                }  // switch
            } // for
            if ((ENETRegBase.QuadPart == 0) || !GotInterrupt) {
                Status = NDIS_STATUS_RESOURCE_CONFLICT;
                DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("Interrupt vector or IO memory address not found.");
                break;
            }
            if (MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress.QuadPart == 0) {
                MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress = ENETRegBase;
                DBG_ENET_DEV_PRINT_INFO("MDIO bus controller address was not found. ENET controller address will be used.");
            }
        } else {
            Status = NDIS_ERROR_CODE_RESOURCE_CONFLICT;
            DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("MiniportInitParameters->AllocatedResources == NULL.");
            break;
        }
        DBG_CODE(pAdapter->ENETDev_RegsPhyAddress = ENETRegBase);
        // Get registers base address
        if ((Status = NdisMMapIoSpace((PVOID *)&pAdapter->ENETRegBase, MiniportAdapterHandle, ENETRegBase, ENETRegLength)) != NDIS_STATUS_SUCCESS) {
                DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("NdisMMapIoSpace() failed.");
            break;
        }
        IMX_MII_LINK_PROPERTIES_t LinkPropertiesToAdvertise;
        LinkPropertiesToAdvertise.R = IMX_MII_LINK_PROPERTIES_AUTO_NEGOTIATION | IMX_MII_LINK_PROPERTIES_PAUSE | IMX_MII_LINK_PROPERTIES_ASYMETRIC_PAUSE; /* This MAC supports both Pause Rx and Pause Tx */;
        switch (pAdapter->SpeedSelect) {
            case SPEED_HALF_DUPLEX_10M:
                LinkPropertiesToAdvertise.B.BASE_T_10_HD = 1;
                break;
            case SPEED_FULL_DUPLEX_10M:
                LinkPropertiesToAdvertise.B.BASE_T_10_FD = 1;
                break;
            case SPEED_HALF_DUPLEX_100M:
                LinkPropertiesToAdvertise.B.BASE_TX_100_HD = 1;
                break;
            case SPEED_FULL_DUPLEX_100M:
                LinkPropertiesToAdvertise.B.BASE_TX_100_FD = 1;
                break;
            case SPEED_HALF_DUPLEX_1G:
                LinkPropertiesToAdvertise.B.BASE_T_1000_HD = 1;
                break;
            case SPEED_FULL_DUPLEX_1G:
                LinkPropertiesToAdvertise.B.BASE_T_1000_FD = 1;
                break;
            default:
                LinkPropertiesToAdvertise.R |= IMX_MII_LINK_PROPERTIES_BASE_ALL_MASK;
                break;            
        }
        pAdapter->ENETDev_PHYDevice.PHYDev_LinkPropertiesToAdvertise.R = LinkPropertiesToAdvertise.R;
        pAdapter->ENETDev_PHYDevice.PHYDev_pEnetAdapter                = pAdapter;
        pAdapter->ENETDev_PHYDevice.PHYDev_pMDIODev                    = &pAdapter->ENETDev_MDIODevice;
        MDIODevCfg.MDIODevCfg_MIIFrameDoneCallback                     = PHY_FrameDoneCallback;
        MDIODevCfg.MDIODevCfg_Context                                  = (NDIS_HANDLE)&pAdapter->ENETDev_PHYDevice;
        MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress.QuadPart    = ENETRegBase.QuadPart;  // Suppose each ENET has its own MDIO bus
        MDIODevCfg.MDIODevCfg_MDIOBusController_InputClk_kHz           = 66000;                 // Default ENET/MDIO controller is 66 MHz
        MDIODevCfg.MDIODevCfg_PhyDevice_Address                        = 0;                     // Default PHY address is 0
        MDIODevCfg.MDIODevCfg_PhyDevice_InterfaceType                  = RGMII;                 // Default Interafce type is RGMII
        MDIODevCfg.MDIODevCfg_PhyDevice_MaxMDIOBusClock_kHz            = 2500;                  // Default PHY max clock is 2.5 MHz
        MDIODevCfg.MDIODevCfg_PhyDevice_MinSTAHoldTime_ns              = 10;                    // Default PHY STA hosl time is 10 ns
        MDIODevCfg.MDIODevCfg_PhyDevice_DisableFramePreamble           = FALSE;                 // Preamble is enabled by default
        // Try to get PHY specific values from ACPI
        if (NT_SUCCESS(Status = AcpiDevice_GetDevicePropertiesPackage(pAdapter))) {
            DBG_ENET_DEV_PRINT_INFO("Device Properties found in ACPI _DSD.XXX");
            AcpiDevice_GetIntegerPropertyValue(pAdapter, "MDIOBusController_InputClk_kHz" , (PULONG)&MDIODevCfg.MDIODevCfg_MDIOBusController_InputClk_kHz);
            AcpiDevice_GetIntegerPropertyValue(pAdapter, "PhyAddress"                     , (PULONG)&MDIODevCfg.MDIODevCfg_PhyDevice_Address);
            AcpiDevice_GetIntegerPropertyValue(pAdapter, "PhyInterafceType"               , (PULONG)&MDIODevCfg.MDIODevCfg_PhyDevice_InterfaceType);
            AcpiDevice_GetIntegerPropertyValue(pAdapter, "PhyMaxMDIOBusClock_kHz"         , (PULONG)&MDIODevCfg.MDIODevCfg_PhyDevice_MaxMDIOBusClock_kHz);
            AcpiDevice_GetIntegerPropertyValue(pAdapter, "PhyMinSTAHoldTime_ns"           , (PULONG)&MDIODevCfg.MDIODevCfg_PhyDevice_MinSTAHoldTime_ns);
            AcpiDevice_GetIntegerPropertyValue(pAdapter, "PhyDisablePreamble"             , (PULONG)&MDIODevCfg.MDIODevCfg_PhyDevice_DisableFramePreamble);
            AcpiDevice_GetIntegerPropertyValue(pAdapter, "MDIOBusController_Address"      , (PULONG)&MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress);
            AcpiDevice_GetCommad              (pAdapter, "ConfigCmds"                     , &pAdapter->ENETDev_PHYDevice.PHYDev_pInitCmd);
        } else {
            DBG_ENET_DEV_PRINT_INFO("Device Properties not found in ACPI (_DSD).");
        }
        if (pAdapter->pOutputBuffer) {
            ExFreePoolWithTag(pAdapter->pOutputBuffer, MP_TAG_ACPI);
        }
        DBG_ENET_DEV_PRINT_INFO("MDIO controller MDIO clock [kHz]= %d",           MDIODevCfg.MDIODevCfg_MDIOBusController_InputClk_kHz);
        DBG_ENET_DEV_PRINT_INFO("ENET controller physical address = 0x%08x_%08x", NdisGetPhysicalAddressHigh(ENETRegBase), NdisGetPhysicalAddressLow(ENETRegBase));
        DBG_ENET_DEV_PRINT_INFO("MDIO controller physical address = 0x%08x_%08x", NdisGetPhysicalAddressHigh(MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress), NdisGetPhysicalAddressLow(MDIODevCfg.MDIODevCfg_MDIOBusController_PhyAddress));
        DBG_ENET_DEV_PRINT_INFO("PHY device address = %d",                        MDIODevCfg.MDIODevCfg_PhyDevice_Address);
        DBG_ENET_DEV_PRINT_INFO("PHY device interface type = %s",                 Dbg_GetEnetPhyInterfaceTypeName(MDIODevCfg.MDIODevCfg_PhyDevice_InterfaceType));
        DBG_ENET_DEV_PRINT_INFO("PHY device max. MDIO clock [kHz]= %d",           MDIODevCfg.MDIODevCfg_PhyDevice_MaxMDIOBusClock_kHz);
        DBG_ENET_DEV_PRINT_INFO("PHY device min. STA hold time [ns]= %d",         MDIODevCfg.MDIODevCfg_PhyDevice_MinSTAHoldTime_ns);
        DBG_ENET_DEV_PRINT_INFO("PHY device disable preamble %s",                 MDIODevCfg.MDIODevCfg_PhyDevice_DisableFramePreamble?"YES":"NO");
        if ((Status = MDIODev_InitDevice(&MDIODriver, &MDIODevCfg, pAdapter->ENETDev_PHYDevice.PHYDev_pMDIODev)) != NDIS_STATUS_SUCCESS) {
            break;
        }
        // Allocate all other memory blocks including shared memory
        if ((Status = NICAllocAdapterMemory(pAdapter)) != NDIS_STATUS_SUCCESS)  {
            break;
        }
        EnetInit(pAdapter, MDIODevCfg.MDIODevCfg_PhyDevice_InterfaceType);
        NdisZeroMemory(&Interrupt, sizeof(NDIS_MINIPORT_INTERRUPT_CHARACTERISTICS));
        Interrupt.Header.Type             = NDIS_OBJECT_TYPE_MINIPORT_INTERRUPT;
        Interrupt.Header.Revision         = NDIS_MINIPORT_INTERRUPT_REVISION_1;
        Interrupt.Header.Size             = sizeof(NDIS_MINIPORT_INTERRUPT_CHARACTERISTICS);
        Interrupt.InterruptHandler        = EnetIsr;
        Interrupt.InterruptDpcHandler     = EnetDpc;
        if ((Status = NdisMRegisterInterruptEx(pAdapter->AdapterHandle, pAdapter, &Interrupt, &pAdapter->NdisInterruptHandle)) != NDIS_STATUS_SUCCESS) {
            DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("NdisMRegisterInterruptEx() failed.");
            break;
        }
        break;
    }
    if (Status != NDIS_STATUS_SUCCESS)  {
        DBG_ENET_DEV_PRINT_ERROR_WITH_STATUS("MpInitializeEx() failed. Driver is getting unloaded.");
        MpFreeAdapter(pAdapter);
    } else {
        Dbg_PrintENETDeviceSettings(pAdapter);
    }
    DBG_ENET_DEV_METHOD_END_WITH_STATUS(Status);
    return Status;
}

/*++
Routine Description:
    NDIS MiniportHaltEx callback handler.
    Halt handler is called when NDIS receives IRP_MN_STOP_DEVICE, IRP_MN_SUPRISE_REMOVE or IRP_MN_REMOVE_DEVICE requests from the PNP manager.
    NDIS does not call MiniportHaltEx if there are outstanding OID requests or send requests.
    NDIS submits no further requests for the affected device after NDIS calls MiniportHaltEx.
    NDIS calls MiniportPause at IRQL = PASSIVE_LEVEL.
Arguments:
    MiniportAdapterContext
        Adapter context address
    HaltAction
        The reason for halting the adapter
Return Value:
    None
--*/
_Use_decl_annotations_
void MpHaltEx(NDIS_HANDLE MiniportAdapterContext, NDIS_HALT_ACTION HaltAction)
{
    PMP_ADAPTER  pAdapter = (PMP_ADAPTER)MiniportAdapterContext;

    DBG_SM_METHOD_BEG();
    UNREFERENCED_PARAMETER(HaltAction);
    ASSERT(pAdapter->Tx_PendingNBs == 0);
    NdisResetEvent(&pAdapter->StateMachine.SM_EnetHaltedEvent);                                            // Set event to not-signaled state
    (void)SmSetState(pAdapter, SM_STATE_HALTED, MP_SM_NEXT_STATE_IMMEDIATELY, SM_CALLED_BY_NDIS);          // Switch to the HALTED state immediately
    if (!NdisWaitEvent(&pAdapter->StateMachine.SM_EnetHaltedEvent, MP_SM_WAIT_FOR_SM_EXIT_TIMEOUT_MSEC)) { // Wait until the state is changed and Enet device is not running
        DBG_ENET_DEV_PRINT_ERROR("Failed to stop State engine. Adapter context will not be released.");
    } else {
        MpFreeAdapter(pAdapter);  // Free the entire adapter object, including the shared memory structures.
    }
 //   DBG_HALT_EX_METHOD_END_SM();
}

/*++
Routine Description:
    NDIS MiniportPause callback handler.
    NDIS calls MiniportPause at IRQL = PASSIVE_LEVEL.
Arguments:
    MiniportAdapterContext
        Adapter context address
    MiniportPauseParameters
        Additional information about the pause operation
Return Value:
    NDIS_STATUS_PENDING
--*/
_Use_decl_annotations_
NDIS_STATUS MpPause(NDIS_HANDLE MiniportAdapterContext, PNDIS_MINIPORT_PAUSE_PARAMETERS MiniportPauseParameters)
{
    PMP_ADAPTER         pAdapter = (PMP_ADAPTER)MiniportAdapterContext;

    DBG_SM_METHOD_BEG();
    UNREFERENCED_PARAMETER(MiniportPauseParameters);
    (void)SmSetState(pAdapter, SM_STATE_PAUSING, MP_SM_NEXT_STATE_IMMEDIATELY, SM_CALLED_BY_NDIS);  // Switch to the PAUSING state immediately
    DBG_SM_METHOD_END_WITH_STATUS(NDIS_STATUS_PENDING);
    return NDIS_STATUS_PENDING;
}

/*++
Routine Description:
    NDIS MiniportRestart callback handler.
    NDIS calls MiniportRestart at IRQL = PASSIVE_LEVEL.
Arguments:
    MiniportAdapterContext
        Adapter context address
    MiniportRestartParameters
        Additional information about the restart operation
Return Value:
    NDIS_STATUS_PENDING
--*/
_Use_decl_annotations_
NDIS_STATUS MpRestart(NDIS_HANDLE MiniportAdapterContext, PNDIS_MINIPORT_RESTART_PARAMETERS MiniportRestartParameters)
{
    PMP_ADAPTER pAdapter = (PMP_ADAPTER)MiniportAdapterContext;

    DBG_SM_METHOD_BEG();
    UNREFERENCED_PARAMETER(MiniportRestartParameters);
    SmSetState(pAdapter, SM_STATE_RUNNING, MP_SM_NEXT_STATE_IMMEDIATELY, SM_CALLED_BY_NDIS);  // Switch to the RESTARTING state immediately
    DBG_SM_METHOD_END_WITH_STATUS(NDIS_STATUS_SUCCESS);
    return NDIS_STATUS_SUCCESS;
}

/*++
Routine Description:
    NDIS MiniportShutdownEx callback handler.
    MiniportShutdownEx runs at IRQL = PASSIVE_LEVEL  if ShutdownAction == NdisShutdownPowerOff
    MiniportShutdownEx runs at high IRQL             if ShutdownAction == NdisShutdownBugCheck
Arguments:
    MiniportAdapterContext
        Adapter context address
    ShutdownAction
        Additional information about the shutdown operation
Return Value:
    None
--*/
_Use_decl_annotations_
void MpShutdownEx(NDIS_HANDLE MiniportAdapterContext, NDIS_SHUTDOWN_ACTION ShutdownAction) {
    PMP_ADAPTER              pAdapter    = (PMP_ADAPTER)MiniportAdapterContext;

    DBG_SM_METHOD_BEG();
    if (ShutdownAction == NdisShutdownPowerOff) {
        EnetDeinit(pAdapter);
    } else {
        DBG_SM_PRINT_WARNING("Unexpected MpShutdownEx - status NdisShutdownBugCheck");
    }
    DBG_SM_METHOD_END();
}

/*
Routine Description:
    MiniportDevicePnPEvent handler
Argument:
    MiniportAdapterContext
        Our adapter context
    NetDevicePnPEventPtr
        The P&P event parameters
Return Value:
    None
*/
_Use_decl_annotations_
VOID MpDevicePnPEventNotify(NDIS_HANDLE MiniportAdapterContext, PNET_DEVICE_PNP_EVENT NetDevicePnPEvent)
{
    PMP_ADAPTER              pAdapter = (PMP_ADAPTER)MiniportAdapterContext;
    UNREFERENCED_PARAMETER(pAdapter);
    UNREFERENCED_PARAMETER(MiniportAdapterContext);
    UNREFERENCED_PARAMETER(NetDevicePnPEvent);
    DBG_SM_METHOD_BEG();
    #if DBG
    switch (NetDevicePnPEvent->DevicePnPEvent)  {
        case NdisDevicePnPEventQueryRemoved:
            DBG_SM_PRINT_INFO("PnP event: NdisDevicePnPEventQueryRemoved");
            break;
        case NdisDevicePnPEventRemoved:
            DBG_SM_PRINT_INFO("PnP event: NdisDevicePnPEventRemoved");
            break;
        case NdisDevicePnPEventSurpriseRemoved:
            DBG_SM_PRINT_INFO("PnP event: NdisDevicePnPEventSurpriseRemoved");
            break;
        case NdisDevicePnPEventQueryStopped:
            DBG_SM_PRINT_INFO("PnP event: NdisDevicePnPEventQueryStopped");
            break;
        case NdisDevicePnPEventStopped:
            DBG_SM_PRINT_INFO("PnP event: NdisDevicePnPEventStopped");
            break;
        case NdisDevicePnPEventPowerProfileChanged:
            DBG_SM_PRINT_INFO("PnP event: NdisDevicePnPEventPowerProfileChange");
            break;
        default:
            DBG_SM_PRINT_INFO("Unknown PnP event %x", NetDevicePnPEvent->DevicePnPEvent);
            break;
    }
    #endif
    DBG_SM_METHOD_END();
}

/*++
Routine Description:
    NDIS MiniportResetEx callback handler.
    NDIS calls MiniportResetEx at IRQL <= DISPATCH_LEVEL.
Arguments:
    AddressingReset
        To let NDIS know whether we need help from it with our reset
    MiniportAdapterContext
        Pointer to our adapter
Return Value:
    NDIS_STATUS_SUCCESS
    NDIS_STATUS_PENDING
    NDIS_STATUS_RESET_IN_PROGRESS
    NDIS_STATUS_HARD_ERRORS
--*/
_Use_decl_annotations_
NDIS_STATUS MpResetEx(NDIS_HANDLE  MiniportAdapterContext, PBOOLEAN AddressingReset)
{
    NDIS_STATUS         Status = NDIS_STATUS_SUCCESS;
    PMP_ADAPTER         pAdapter = (PMP_ADAPTER) MiniportAdapterContext;

    DBG_SM_METHOD_BEG();
    *AddressingReset = TRUE;                                                                       // Tell NDIS, to re-send the configuration parameters once reset is done.
    NdisAcquireSpinLock(&pAdapter->Dev_SpinLock);
    if (pAdapter->PendingNdisOperations.B.OP_RESET) {                                              // Reset pending?
        Status = NDIS_STATUS_RESET_IN_PROGRESS;                                                    // Yes, return pending status.
    } else {
        pAdapter->PendingNdisOperations.B.OP_RESET = 1;                                            // No, remember reset operation status.
        Status = NDIS_STATUS_PENDING;
    }
    NdisReleaseSpinLock(&pAdapter->Dev_SpinLock);
    if (Status == NDIS_STATUS_PENDING) {
        SmSetState(pAdapter, SM_STATE_RESET, MP_SM_NEXT_STATE_IMMEDIATELY, SM_CALLED_BY_NDIS);     // Switch to the RESET state immediately
    }
    DBG_SM_METHOD_END_WITH_STATUS(Status);
    return(Status);
}
