// Copyright (c) Microsoft Corporation. All rights reserved.
// Copyright 2019 NXP
// Licensed under the MIT License.
//
//
// Module Name:
//
//  imxDodDevice.h
//
// Abstract:
//
//    This is IMXDOD device implementation
//
// Environment:
//
//    Kernel mode only.
//

#ifndef _IMXDODDEVICE_HPP_
#define  _IMXDODDEVICE_HPP_ 1

// MCU & board info length check value
#define IMX_MCU_TYPE_INFO_LENGTH    0xAAAA

// MCU and board type information:
#define IMX_GET_MCU_TYPE(x)         ((x >> 16) & 0xFFFF)
#define IMX_MCIM6QP_MCU_TYPE        0x0001
#define IMX_MCIM6Q_MCU_TYPE         0x0002
#define IMX_MCIM6SX_MCU_TYPE        0x0003
#define IMX_MCIM6DL_MCU_TYPE        0x0004
#define IMX_MCIM6UL_MCU_TYPE        0x0005
#define IMX_MCIM6ULL_MCU_TYPE       0x0006
#define IMX_MCIM7D_MCU_TYPE         0x0007
#define IMX_MCIM8MM_MCU_TYPE        0x0008
#define IMX_MCIM8MQ_MCU_TYPE        0x0009
#define IMX_MCU_TYPE_MAX_VALUE      IMX_MCIM8MQ_MCU_TYPE

#define IMX_GET_BOARD_TYPE(x)       (x & 0xFFFF)
#define IMX_SDB_BOARD_TYPE          0x0001
#define IMX_SDP_BOARD_TYPE          0x0002
#define IMX_EVK_BOARD_TYPE          0x0003
#define IMX_SABRE_BOARD_TYPE        0x0004

// MCU graphic device type information:
#define IMX_LCDIF_DEVICE_ID         0xDEF00001
#define IMX_IPU_DEVICE_ID           0xDEF00002
#define IMX_HDMI_PHY_ID             0xDEF00003
#define IMX_HDMI_CDN_ID             0xDEF00006
#define IMX_MIPI_DSI_ID             0xDEF00007

class IMXDOD_DEVICE {
public: // NONPAGED

    static DXGKDDI_RESET_DEVICE DdiResetDevice;
    static DXGKDDI_SYSTEM_DISPLAY_ENABLE DdiSystemDisplayEnable;
    static DXGKDDI_SYSTEM_DISPLAY_WRITE DdiSystemDisplayWrite;
    static DXGKDDISETPOWERCOMPONENTFSTATE DdiSetPowerComponentFState;
    static DXGKDDIPOWERRUNTIMECONTROLREQUEST DdiPowerRuntimeControlRequest;

    static DXGKDDI_INTERRUPT_ROUTINE DdiInterruptRoutine;
    static DXGKDDI_DPC_ROUTINE DdiDpcRoutine;

    __forceinline IMXDOD_DEVICE (const DEVICE_OBJECT* PhysicalDeviceObjectPtr) :
        physicalDeviceObjectPtr(PhysicalDeviceObjectPtr),
        dxgkInterface(),
        dxgkStartInfo(),
        dxgkDeviceInfo(),
        dxgkDisplayInfo(),
        dxgkVideoSignalInfo(),
        dxgkCurrentSourceMode(),
        ipuRegistersPtr(),
        frameBufferLength(0),
        biosFrameBufferPtr(),
        ipu1Conf(0)
    {}

private: // NONPAGED

    enum : ULONG { CHILD_COUNT = 1 };

    enum POWER_COMPONENT {
        POWER_COMPONENT_GPU3D,
        POWER_COMPONENT_IPU,
        POWER_COMPONENT_MONITOR,
        POWER_COMPONENT_COUNT,
    };

    IMXDOD_DEVICE(const IMXDOD_DEVICE&) = delete;
    IMXDOD_DEVICE& operator= (const IMXDOD_DEVICE&) = delete;

    void IpuOn();
    void IpuOff();

    void HdmiPhyOn();
    void HdmiPhyOff();

    void LcdifOn();
    void LcdifOff();

    void HdmiCdnGeneralReadRegister(UINT32 address, UINT32 *value);
    void HdmiCdnGeneralWriteRegister(UINT32 address, UINT32 value);
    void HdmiCdnOn();
    void HdmiCdnOff();

    void MipiDsiOn();
    void MipiDsiOff();

    __forceinline void writeIpuRegister(ULONG Offset, ULONG Value) const
    {
        WRITE_REGISTER_NOFENCE_ULONG(
            reinterpret_cast<ULONG*>(
                reinterpret_cast<char*>(this->ipuRegistersPtr) +
                Offset),
            Value);
    }

    __forceinline ULONG readIpuRegister(ULONG Offset) const
    {
        return READ_REGISTER_NOFENCE_ULONG(reinterpret_cast<ULONG*>(
            reinterpret_cast<char*>(this->ipuRegistersPtr) + Offset));
    }

    __forceinline void writeHdmiRegister(ULONG Offset, UCHAR Value) const
    {
        WRITE_REGISTER_NOFENCE_UCHAR(
            reinterpret_cast<UCHAR*>(
                reinterpret_cast<char*>(this->hdmiRegistersPtr) +
                Offset),
            Value);
    }

    __forceinline UCHAR readHdmiRegister(ULONG Offset) const
    {
        return READ_REGISTER_NOFENCE_UCHAR(reinterpret_cast<UCHAR*>(
            reinterpret_cast<char*>(this->hdmiRegistersPtr) + Offset));
    }

    __forceinline void writeLcdifRegister(ULONG Offset, ULONG Value) const
    {
        WRITE_REGISTER_NOFENCE_ULONG(
            reinterpret_cast<ULONG*>(
                reinterpret_cast<char*>(this->lcdifRegistersPtr) +
                Offset),
            Value);
    }

    __forceinline ULONG readLcdifRegister(ULONG Offset) const
    {
        return READ_REGISTER_NOFENCE_ULONG(reinterpret_cast<ULONG*>(
            reinterpret_cast<char*>(this->lcdifRegistersPtr) + Offset));
    }

    __forceinline void writeHdmiCdnRegister(ULONG Offset, ULONG Value) const
    {
        WRITE_REGISTER_NOFENCE_ULONG(
            reinterpret_cast<ULONG*>(
                reinterpret_cast<char*>(this->hdmiCdnRegistersPtr) +
                Offset),
            Value);
    }

    __forceinline ULONG readHdmiCdnRegister(ULONG Offset) const
    {
        return READ_REGISTER_NOFENCE_ULONG(reinterpret_cast<ULONG*>(
            reinterpret_cast<char*>(this->hdmiCdnRegistersPtr) + Offset));
    }

    __forceinline void writeMipiDsiRegister(ULONG Offset, ULONG Value) const
    {
        WRITE_REGISTER_NOFENCE_ULONG(
            reinterpret_cast<ULONG*>(
                reinterpret_cast<char*>(this->mipiDsiRegistersPtr) +
                Offset),
            Value);
    }

    __forceinline ULONG readMipiDsiRegister(ULONG Offset) const
    {
        return READ_REGISTER_NOFENCE_ULONG(reinterpret_cast<ULONG*>(
            reinterpret_cast<char*>(this->mipiDsiRegistersPtr) + Offset));
    }

    const DEVICE_OBJECT* const physicalDeviceObjectPtr;
    DXGKRNL_INTERFACE dxgkInterface;
    DXGK_START_INFO dxgkStartInfo;
    DXGK_DEVICE_INFO dxgkDeviceInfo;
    DXGK_DISPLAY_INFORMATION dxgkDisplayInfo;
    D3DKMDT_VIDEO_SIGNAL_INFO dxgkVideoSignalInfo;
    D3DKMDT_VIDPN_SOURCE_MODE dxgkCurrentSourceMode;

    LONGLONG McuBoardType;

    BOOLEAN ipuHdmiEnabled;
    PVOID ipuRegistersPtr;
    PVOID hdmiRegistersPtr;

    // LCDIF controller
    BOOLEAN lcdifEnabled;
    PVOID lcdifRegistersPtr;

    // LCDIF register saving
    ULONG CTRL;
    ULONG CTRL1;
    ULONG TRANSFER_COUNT;
    ULONG VDCTRL0;
    ULONG VDCTRL1;
    ULONG VDCTRL2;
    ULONG VDCTRL3;
    ULONG VDCTRL4;
    ULONG CUR_BUF;
    ULONG NEXT_BUF;

    SIZE_T frameBufferLength;
    VOID* biosFrameBufferPtr;       // must be freed with MmUnmapIoSpace

    // IPU register saving
    ULONG ipu1Conf;

    // MIPI_DSI controller
    BOOLEAN mipiDsiEnabled;
    PVOID mipiDsiRegistersPtr;
    // MIPI_DSI register saving
    ULONG DSI_CLKCTRL;

    // HDMI cadence
    BOOLEAN hdmiCdnEnabled;
    PVOID hdmiCdnRegistersPtr;

public: // PAGED

    static DXGKDDI_ADD_DEVICE DdiAddDevice;
    static DXGKDDI_REMOVE_DEVICE DdiRemoveDevice;
    static DXGKDDI_START_DEVICE DdiStartDevice;
    static DXGKDDI_STOP_DEVICE DdiStopDevice;
    static DXGKDDI_DISPATCH_IO_REQUEST DdiDispatchIoRequest;

    static DXGKDDI_QUERY_CHILD_RELATIONS DdiQueryChildRelations;
    static DXGKDDI_QUERY_CHILD_STATUS DdiQueryChildStatus;
    static DXGKDDI_QUERY_DEVICE_DESCRIPTOR DdiQueryDeviceDescriptor;
    static DXGKDDI_SET_POWER_STATE DdiSetPowerState;

    static DXGKDDI_QUERYADAPTERINFO DdiQueryAdapterInfo;
    static DXGKDDI_SETPOINTERPOSITION DdiSetPointerPosition;
    static DXGKDDI_SETPOINTERSHAPE DdiSetPointerShape;

    static DXGKDDI_ISSUPPORTEDVIDPN DdiIsSupportedVidPn;
    static DXGKDDI_RECOMMENDFUNCTIONALVIDPN DdiRecommendFunctionalVidPn;
    static DXGKDDI_ENUMVIDPNCOFUNCMODALITY DdiEnumVidPnCofuncModality;
    static DXGKDDI_SETVIDPNSOURCEVISIBILITY DdiSetVidPnSourceVisibility;
    static DXGKDDI_COMMITVIDPN DdiCommitVidPn;
    static DXGKDDI_UPDATEACTIVEVIDPNPRESENTPATH DdiUpdateActiveVidPnPresentPath;

    static DXGKDDI_RECOMMENDMONITORMODES DdiRecommendMonitorModes;
    static DXGKDDI_QUERYVIDPNHWCAPABILITY DdiQueryVidPnHWCapability;
    static DXGKDDI_PRESENTDISPLAYONLY DdiPresentDisplayOnly;

    static DXGKDDI_STOP_DEVICE_AND_RELEASE_POST_DISPLAY_OWNERSHIP
        DdiStopDeviceAndReleasePostDisplayOwnership;

private: // PAGED

    _IRQL_requires_(PASSIVE_LEVEL)
    static NTSTATUS SourceHasPinnedMode (
        D3DKMDT_HVIDPN VidPnHandle,
        const DXGK_VIDPN_INTERFACE* VidPnInterfacePtr,
        D3DKMDT_VIDEO_PRESENT_SOURCE_MODE_ID SourceId
        );

    _IRQL_requires_(PASSIVE_LEVEL)
    NTSTATUS CreateAndAssignSourceModeSet (
        D3DKMDT_HVIDPN VidPnHandle,
        const DXGK_VIDPN_INTERFACE* VidPnInterfacePtr,
        D3DKMDT_VIDEO_PRESENT_SOURCE_MODE_ID SourceId,
        D3DKMDT_VIDEO_PRESENT_TARGET_MODE_ID TargetId
        ) const;

    _IRQL_requires_(PASSIVE_LEVEL)
    static NTSTATUS TargetHasPinnedMode (
        D3DKMDT_HVIDPN VidPnHandle,
        const DXGK_VIDPN_INTERFACE* VidPnInterfacePtr,
        D3DKMDT_VIDEO_PRESENT_TARGET_MODE_ID TargetId
        );

    _IRQL_requires_(PASSIVE_LEVEL)
    NTSTATUS CreateAndAssignTargetModeSet (
        D3DKMDT_HVIDPN VidPnHandle,
        const DXGK_VIDPN_INTERFACE* VidPnInterfacePtr,
        D3DKMDT_VIDEO_PRESENT_SOURCE_MODE_ID SourceId,
        D3DKMDT_VIDEO_PRESENT_TARGET_MODE_ID TargetId
        ) const;

    _IRQL_requires_(PASSIVE_LEVEL)
    static NTSTATUS IsVidPnSourceModeFieldsValid (
        const D3DKMDT_VIDPN_SOURCE_MODE* SourceModePtr
        );

    _IRQL_requires_(PASSIVE_LEVEL)
    static NTSTATUS IsVidPnPathFieldsValid (
        const D3DKMDT_VIDPN_PRESENT_PATH* PathPtr
        );
};

#endif // _IMXDODDEVICE_HPP_
