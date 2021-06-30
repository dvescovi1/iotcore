/*
* Copyright 2018-2020 NXP
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

#include "imx_tcpci_driver.h"

#ifdef DBG
LARGE_INTEGER        DriverStartTime;
#endif
#ifdef DBG_MESSAGE_PRINTING

#undef MAKECASE
#undef MAKECASE1
#undef MAKEDEFAULT
#define MAKECASE(Value) case Value:  return #Value;
#define MAKECASE1(Value,Name) case Value:  return #Name;
#define MAKEDEFAULT(Message) default: return"!!! "Message" name unknown !!!";

const char* Dbg_GetIOCTLName(ULONG i) {
    switch (i) {
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_GET_STATUS)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_GET_CONTROL)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_CONTROL)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_TRANSMIT_BUFFER)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_RECEIVE_DETECT)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_CONFIG_STANDARD_OUTPUT)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_COMMAND)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_SET_MESSAGE_HEADER_INFO)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_ALTERNATE_MODE_ENTERED)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_ALTERNATE_MODE_EXITED)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_DISPLAYPORT_CONFIGURED)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_DISPLAYPORT_HPD_STATUS_CHANGED)
        MAKECASE(IOCTL_UCMTCPCI_PORT_CONTROLLER_DISPLAYPORT_DISPLAY_OUT_STATUS_CHANGED)
        MAKEDEFAULT("Unknown IOCTL")
    }
}

const char* Dbg_GetRegName(UINT32 i) {
    switch (i) {
        MAKECASE(TCPC_PHY_VENDOR_ID)
        MAKECASE(TCPC_PHY_PRODUCT_ID)
        MAKECASE(TCPC_PHY_DEVICE_ID)
        MAKECASE(TCPC_PHY_USBTYPEC_REV)
        MAKECASE(TCPC_PHY_USBPD_REV_VER)
        MAKECASE(TCPC_PHY_PD_INTERFACE_REV)
        MAKECASE(TCPC_PHY_ALERT)
        MAKECASE(TCPC_PHY_ALERT_MASK)
        MAKECASE(TCPC_PHY_POWER_STATUS_MASK)
        MAKECASE(TCPC_PHY_FAULT_STATUS_MASK)
        MAKECASE(TCPC_PHY_EXTENDED_STATUS_MASK)
        MAKECASE(TCPC_PHY_ALERT_EXTENDED_MASK)
        MAKECASE(TCPC_PHY_CONFIG_STANDARD_OUTPUT)
        MAKECASE(TCPC_PHY_TCPC_CONTROL)
        MAKECASE(TCPC_PHY_ROLE_CONTROL)
        MAKECASE(TCPC_PHY_FAULT_CONTROL)
        MAKECASE(TCPC_PHY_POWER_CONTROL)
        MAKECASE(TCPC_PHY_CC_STATUS)
        MAKECASE(TCPC_PHY_POWER_STATUS)
        MAKECASE(TCPC_PHY_FAULT_STATUS)
        MAKECASE(TCPC_PHY_EXTENDED_STATUS)
        MAKECASE(TCPC_PHY_ALERT_EXTENDED)
        MAKECASE(TCPC_PHY_COMMAND)
        MAKECASE(TCPC_PHY_DEVICE_CAPABILITIES_1)
        MAKECASE(TCPC_PHY_DEVICE_CAPABILITIES_2)
        MAKECASE(TCPC_PHY_STANDARD_INPUT_CAPABILITIES)
        MAKECASE(TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES)
        MAKECASE(TCPC_PHY_CONFIG_EXTENDED1)
        MAKECASE(TCPC_PHY_GENERIC_TIMER)
        MAKECASE(TCPC_PHY_MESSAGE_HEADER_INFO)
        MAKECASE(TCPC_PHY_RECEIVE_DETECT)
        MAKECASE(TCPC_PHY_RX_BUF_BYTE_X)
        MAKECASE(TCPC_PHY_TRANSMIT)
        MAKECASE(TCPC_PHY_TX_BUF_BYTE_X)
        MAKECASE(TCPC_PHY_VBUS_VOLTAGE)
        MAKECASE(TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD)
        MAKECASE(TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD)
        MAKECASE(TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG)
        MAKECASE(TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG)
        MAKECASE(TCPC_PHY_VBUS_HV_TARGET)
        MAKECASE(TCPC_PHY_EXT_CFG_ID)
        MAKECASE(TCPC_PHY_EXT_ALERT)
        MAKECASE(TCPC_PHY_EXT_ALERT_MASK)
        MAKECASE(TCPC_PHY_EXT_CONFIG)
        MAKECASE(TCPC_PHY_EXT_FAULT_CONFIG)
        MAKECASE(TCPC_PHY_EXT_CONTROL)
        MAKECASE(TCPC_PHY_EXT_STATUS)
        MAKECASE(TCPC_PHY_EXT_GPIO_CONFIG)
        MAKECASE(TCPC_PHY_EXT_GPIO_CONTROL)
        MAKECASE(TCPC_PHY_EXT_GPIO_ALERT_CONFIG)
        MAKECASE(TCPC_PHY_EXT_GPIO_STATUS)
        MAKECASE(TCPC_PHY_SOURCE_HIGH_VOLTAGE_MB4B_TIME)
        MAKECASE(TCPC_PHY_ADC_FILTER_CONTROL_1)
        MAKECASE(TCPC_PHY_ADC_FILTER_CONTROL_2)
        MAKECASE(TCPC_PHY_VCON_CONFIG)
        MAKECASE(TCPC_PHY_VCON_FAULT_DEBOUNCE)
        MAKECASE(TCPC_PHY_VCON_FAULT_RECOVERY)
        MAKECASE(TCPC_PHY_VCON_FAULT_ATTEMPTS)
        MAKEDEFAULT("Unknown reg name")
    }
}

void TCPC_PHY_DumpReg(TCPC_PHY_t *pRegs, UINT32 RegAddress, UINT32 params, ULONG Length) {
    char *pStr;
    switch (RegAddress) {
        case TCPC_PHY_ALERT:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value: 0x%04X (VDA=%d, AlrtExt=%d, ES=%d, BegMsg=%d, VbusSnkDcn=%d, RxBufOvf=%d, Flt=%d, VbusLo=%d, VbusHi=%d, TxMsgSucc=%d, TxMsgDacrd=%d, TxMsgFld=%d, HrdRst=%d, RxMsg=%d, PS=%d, CC=%d)", params?"Wr":"Rd", "ALERT", TCPC_PHY_ALERT, pRegs->ALERT.R,
                pRegs->ALERT.B.VENDOR_DEFINED_ALERT,
                pRegs->ALERT.B.ALERT_EXTENDED,
                pRegs->ALERT.B.EXTENDED_STATUS,
                pRegs->ALERT.B.BEGINNING_SOP_MESSAGE_STATUS,
                pRegs->ALERT.B.VBUS_SINK_DISCONNECT_DETECTED,
                pRegs->ALERT.B.RX_BUFFER_OVERFLOW,
                pRegs->ALERT.B.FAULT,
                pRegs->ALERT.B.VBUS_VOLTAGE_ALARM_LO,
                pRegs->ALERT.B.VBUS_VOLTAGE_ALARM_HI,
                pRegs->ALERT.B.TRANSMIT_SOP_MESSAGE_SUCCESSFUL,
                pRegs->ALERT.B.TRANSMIT_SOP_MESSAGE_DISCARDED,
                pRegs->ALERT.B.TRANSMIT_SOP_MESSAGE_FAILED,
                pRegs->ALERT.B.RECEIVED_HARD_RESET,
                pRegs->ALERT.B.RECEIVED_SOP_MESSAGE_STATUS,
                pRegs->ALERT.B.POWER_STATUS,
                pRegs->ALERT.B.CC_STATUS
            );
            break;           
        case TCPC_PHY_CONFIG_STANDARD_OUTPUT:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (HiImpdOuts=%d, DbgAccCncted#=%d, AudAccCncted#=%d, ActCblCncted=%d, MUXCtrl=%s, ConnPrsnt=%d, ConOrnt=%s)", params?"Wr":"Rd", "CONFIG_STANDARD_OUTPUT", TCPC_PHY_CONFIG_STANDARD_OUTPUT, pRegs->CONFIG_STANDARD_OUTPUT.R,
                pRegs->CONFIG_STANDARD_OUTPUT.B.HIGH_IMPEDANCE_OUTPUTS,
                pRegs->CONFIG_STANDARD_OUTPUT.B.DEBUG_ACCESSORY_CONNECTED,
                pRegs->CONFIG_STANDARD_OUTPUT.B.AUDIO_ACCESSORY_CONNECTED,
                pRegs->CONFIG_STANDARD_OUTPUT.B.ACTIVE_CABLE_CONNECTED,
                pRegs->CONFIG_STANDARD_OUTPUT.B.MUX_CONTROL== 0?"No conn":pRegs->CONFIG_STANDARD_OUTPUT.B.MUX_CONTROL==1? "USB3.1 Conctd":pRegs->CONFIG_STANDARD_OUTPUT.B.MUX_CONTROL==2?"DP AltMode – 4 lanes":"USB3.1 + Display Port Lanes 0 & 1",
                pRegs->CONFIG_STANDARD_OUTPUT.B.CONNECTION_PRESENT,
                pRegs->CONFIG_STANDARD_OUTPUT.B.CONNECTOR_ORIENTATION==0?"VCONN to CC2":"VCONN to CC1"
            );
            break;
        case TCPC_PHY_TCPC_CONTROL:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (EnSMBusPEC=%d, EnL4CAlrt=%d, EnWDT=%d, DbgAccCtrl=%s, I2CClkStrch %s, BIST=%d, PlgOrnt=%s)", params?"Wr":"Rd", "TCPC_CONTROL", TCPC_PHY_TCPC_CONTROL, pRegs->TCPC_CONTROL.R,
                pRegs->TCPC_CONTROL.B.ENABLE_SMBUS_PEC,
                pRegs->TCPC_CONTROL.B.ENABLE_LOOKING4CONNECTION_ALERT,
                pRegs->TCPC_CONTROL.B.ENABLE_WATCHDOG_TIMER,
                (pRegs->TCPC_CONTROL.B.DEBUG_ACCESSORY_CONTROL == 0)? "by TCPC":"by TCPM",
                (pRegs->TCPC_CONTROL.B.I2C_CLOCK_STRETCHING_CONTROL == 0)? "Dis":(pRegs->TCPC_CONTROL.B.I2C_CLOCK_STRETCHING_CONTROL == 1)? "Rsvd":(pRegs->TCPC_CONTROL.B.I2C_CLOCK_STRETCHING_CONTROL == 2)? "En":"En in no alrt pin",
                pRegs->TCPC_CONTROL.B.BIST_TEST_MODE,
                (pRegs->TCPC_CONTROL.B.PLUG_ORIENTATION == 0)? "VCONN to CC2":"VCONN to CC1"
            );
            break;
        case TCPC_PHY_ROLE_CONTROL:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (DRP=%s, RP_VALUE=%s, CC2=%s, CC1=%s)", params?"Wr":"Rd", "ROLE_CONTROL", TCPC_PHY_ROLE_CONTROL, pRegs->ROLE_CONTROL.R,
                (pRegs->ROLE_CONTROL.B.DRP == 0)? "Dis":"En",
                (pRegs->ROLE_CONTROL.B.RP_VALUE == 0)? "Rp default":(pRegs->ROLE_CONTROL.B.RP_VALUE == 1)? "Rp 1.5A":(pRegs->ROLE_CONTROL.B.RP_VALUE == 2)? "Rp 3.0A":"Reserved",
                (pRegs->ROLE_CONTROL.B.CC2 == 0)? "Ra":(pRegs->ROLE_CONTROL.B.CC2 == 1)? "Rp":(pRegs->ROLE_CONTROL.B.CC2 == 2)? "Rd":"Open",
                (pRegs->ROLE_CONTROL.B.CC1 == 0)? "Ra":(pRegs->ROLE_CONTROL.B.CC1 == 1)? "Rp":(pRegs->ROLE_CONTROL.B.CC1 == 2)? "Rd":"Open"
            );
            break;
        case TCPC_PHY_FAULT_CONTROL:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (ForceOffVBUSSigDis=%d, VBUSDschrgFltDetctTimerDis=%d, IntOrExtOCPDis=%d, IntOrExtOVPDis=%d, VConnOVCDis=%d)", params?"Wr":"Rd", "FAULT_CONTROL", TCPC_PHY_FAULT_CONTROL, pRegs->FAULT_CONTROL.R,
                pRegs->FAULT_CONTROL.B.FORCE_OFF_VBUS,
                pRegs->FAULT_CONTROL.B.VBUS_DISCHARGE_FAULT_DETECTION_TIMER,
                pRegs->FAULT_CONTROL.B.INTERNAL_OR_EXTERNAL_VBUS_OCP_FAULT,
                pRegs->FAULT_CONTROL.B.INTERNAL_OR_EXTERNAL_VBUS_OVP_FAULT,
                pRegs->FAULT_CONTROL.B.VCONN_OVER_CURRENT_FAULT
            );
            break;
        case TCPC_PHY_POWER_CONTROL:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (FRSEn=%d, VbusVltgMonDis=%d, VltgAlarmDis=%d, AutoDischrgVbusOnDiscnEn=%d, BldDichrgEn=%d, ForceDischrg=%d, VconPrwSupp=%d, VconEn=%d)", params?"Wr":"Rd", "POWER_CONTROL", TCPC_PHY_POWER_CONTROL, pRegs->POWER_CONTROL.R,
                pRegs->POWER_CONTROL.B.FAST_ROLE_SWAP_ENABLE,
                pRegs->POWER_CONTROL.B.VBUS_VOLTAGE_MONITOR,
                pRegs->POWER_CONTROL.B.DISABLE_VOLTAGE_ALARMS,
                pRegs->POWER_CONTROL.B.AUTO_DISCHARGE_DISCONNECT,
                pRegs->POWER_CONTROL.B.ENABLE_BLEED_DISCHARGE,
                pRegs->POWER_CONTROL.B.FORCE_DISCHARGE,
                pRegs->POWER_CONTROL.B.VCONN_POWER_SUPPORTED,
                pRegs->POWER_CONTROL.B.ENABLE_VCONN
            );
            break;
        case TCPC_PHY_CC_STATUS:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (L4C=%s, CN_RES=%s, CC2=%s, CC1=%s)", params?"Wr":"Rd", "CC_STATUS", TCPC_PHY_CC_STATUS, pRegs->CC_STATUS.R,
                (pRegs->CC_STATUS.B.LOOKING4CONNECTION == 0)? "No":"Yes",
                (pRegs->CC_STATUS.B.CONNECTRESULT == 0)? "presenting Rp":"presenting Rd",
                (pRegs->CC_STATUS.B.CC2_STATE == 0)? "SRC.Open/SNK.Open":(pRegs->CC_STATUS.B.CC2_STATE == 1)? "SRC.Ra/SNK.Default":(pRegs->CC_STATUS.B.CC2_STATE == 2)? "SRC.Rd/SNK.Power1.5":"reserved/SNK.Power3.0",
                (pRegs->CC_STATUS.B.CC1_STATE == 0)? "SRC.Open/SNK.Open":(pRegs->CC_STATUS.B.CC1_STATE == 1)? "SRC.Ra/SNK.Default":(pRegs->CC_STATUS.B.CC1_STATE == 2)? "SRC.Rd/SNK.Power1.5":"reserved/SNK.Power3.0"
            );
            break;
        case TCPC_PHY_POWER_STATUS:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (DACon=0%d, InitPnd=%d, VSrcHiVltg=%d, VSrcVbus=%d, VbusDetEn=%d, VbusPrsnt=%d, VconPrsnt=%d, SnkVbus=%d)", params?"Wr":"Rd", "POWER_STATUS", TCPC_PHY_POWER_STATUS, pRegs->POWER_STATUS.R,
                pRegs->POWER_STATUS.B.DEBUG_ACCESSORY_CONNECTED,
                pRegs->POWER_STATUS.B.TCPC_INITIALIZATION_STATUS,
                pRegs->POWER_STATUS.B.SOURCING_HIGH_VOLTAGE,
                pRegs->POWER_STATUS.B.SOURCING_VBUS,
                pRegs->POWER_STATUS.B.VBUS_DETECTION_ENABLED,
                pRegs->POWER_STATUS.B.VBUS_PRESENT,
                pRegs->POWER_STATUS.B.VCONN_PRESENT,
                pRegs->POWER_STATUS.B.SINKING_VBUS
            );
            break;
        case TCPC_PHY_COMMAND:
            switch (pRegs->COMMAND.R) {                
                case TCPC_PHY_COMMAND_CMD_WAKEI2C:                         pStr = "WAKEI2C";                     break;
                case TCPC_PHY_COMMAND_CMD_DISABLE_VBUS_DETECT:             pStr = "DISABLE_VBUS_DETECT";         break;
                case TCPC_PHY_COMMAND_CMD_ENABLE_VBUS_DETECT:              pStr = "ENABLE_VBUS_DETECT";          break;
                case TCPC_PHY_COMMAND_CMD_DISABLE_SINK_VBUS:               pStr = "DISABLE_SINK_VBUS";           break;
                case TCPC_PHY_COMMAND_CMD_SINK_VBUS:                       pStr = "SINK_VBUS";                   break;
                case TCPC_PHY_COMMAND_CMD_DISABLE_SOURCE_VBUS:             pStr = "DISABLE_SOURCE_VBUS";         break;
                case TCPC_PHY_COMMAND_CMD_SOURCE_VBUS_DEFAULT_VOLTAGE:     pStr = "SOURCE_VBUS_DEFAULT_VOLTAGE"; break;
                case TCPC_PHY_COMMAND_CMD_SOURCE_VBUS_HIGH_VOLTAGE:        pStr = "SOURCE_VBUS_HIGH_VOLTAGE";    break;
                case TCPC_PHY_COMMAND_CMD_LOOK_4_CONNECTION:               pStr = "LOOK_4_CONNECTION";           break;
                case TCPC_PHY_COMMAND_CMD_RX_ONE_MORE:                     pStr = "RX_ONE_MORE";                 break;
                case TCPC_PHY_COMMAND_CMD_SEND_FRSWAP_SIGNAL:              pStr = "SEND_FRSWAP_SIGNAL";          break;
                case TCPC_PHY_COMMAND_CMD_RESET_TRANSMIT_BUFFER:           pStr = "RESET_TRANSMIT_BUFFER";       break;        
                case TCPC_PHY_COMMAND_CMD_RESET_RECEIVE_BUFFER:            pStr = "RESET_RECEIVE_BUFFER";        break;
                case TCPC_PHY_COMMAND_CMD_I2C_IDLE:                        pStr = "I2C_IDLE";                    break;
                default:
                    pStr = "Unknown command"; break;
            }
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (%s)", params?"Wr":"Rd", "COMMAND", TCPC_PHY_COMMAND, pRegs->COMMAND.R, pStr);
            break;            
        case TCPC_PHY_DEVICE_CAPABILITIES_1:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value: 0x%04X (VbusHVT=%d, VbusOCP=%d, VbusOVP=%d, BldDischrg=%d, ForceDischrg=%d, VbusMeasrmnt=%d, SrcRes=%s, Roles=%s, SopDbg=%d, SrcVcon=%d, SnkVbus=%d, SrcHiVltgVbus=%d)", params?"Wr":"Rd", "DEVICE_CAPABILITIES_1", TCPC_PHY_DEVICE_CAPABILITIES_1, pRegs->DEVICE_CAPABILITIES_1.R,
                pRegs->DEVICE_CAPABILITIES_1.B.VBUS_HIGH_VOLTAGE_TARGET,
                pRegs->DEVICE_CAPABILITIES_1.B.VBUS_OCP_REPORTING,
                pRegs->DEVICE_CAPABILITIES_1.B.VBUS_OVP_REPORTING,
                pRegs->DEVICE_CAPABILITIES_1.B.BLEED_DISCHARGE,
                pRegs->DEVICE_CAPABILITIES_1.B.FORCE_DISCHARGE,
                pRegs->DEVICE_CAPABILITIES_1.B.VBUS_MEASUREMENT_AND_ALARM_CAPABLE,
                pRegs->DEVICE_CAPABILITIES_1.B.SOURCE_RESISTOR_SUPPORTED == 0? "Rp default only":pRegs->DEVICE_CAPABILITIES_1.B.SOURCE_RESISTOR_SUPPORTED == 1? "Rp 1.5A & def":pRegs->DEVICE_CAPABILITIES_1.B.SOURCE_RESISTOR_SUPPORTED == 2? "Rp 3.0A, 1.5A & def":"Reserved",
                pRegs->DEVICE_CAPABILITIES_1.B.POWER_ROLES_SUPPORTED == 0? "Src or Snk by TCPM":pRegs->DEVICE_CAPABILITIES_1.B.POWER_ROLES_SUPPORTED == 1? "Src only":pRegs->DEVICE_CAPABILITIES_1.B.POWER_ROLES_SUPPORTED == 2? "Snk only":pRegs->DEVICE_CAPABILITIES_1.B.POWER_ROLES_SUPPORTED == 3? "Snk with accessory":pRegs->DEVICE_CAPABILITIES_1.B.POWER_ROLES_SUPPORTED == 4? "DRP only":pRegs->DEVICE_CAPABILITIES_1.B.POWER_ROLES_SUPPORTED == 5? "rc, Snk, DRP Adapter/Cable":pRegs->DEVICE_CAPABILITIES_1.B.POWER_ROLES_SUPPORTED == 6? "Src, Snk, DRP":"Reserved",
                pRegs->DEVICE_CAPABILITIES_1.B.SOP_DBG_SOP_DBG_SUPPORT,
                pRegs->DEVICE_CAPABILITIES_1.B.SOURCE_VCONN,
                pRegs->DEVICE_CAPABILITIES_1.B.SINK_VBUS,
                pRegs->DEVICE_CAPABILITIES_1.B.SOURCE_HIGH_VOLTAGE_VBUS
                );
            break;
        case TCPC_PHY_DEVICE_CAPABILITIES_2:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value: 0x%04X (GenTimer=%d, LongMsg=%d, SMBusPEC=%d, SrcFRSwp=%d, SnkFRSwp=%d, WDT=%d, SnkDiscnDet=%d, StopDischThrld=%d, VBUSVltgAlarmLSB=%s, VCONNPwrSup=%s, VCONOvcFltCapbl=%d", params?"Wr":"Rd", "DEVICE_CAPABILITIES_2", TCPC_PHY_DEVICE_CAPABILITIES_2, pRegs->DEVICE_CAPABILITIES_2.R,
                pRegs->DEVICE_CAPABILITIES_2.B.GENERIC_TIMER,
                pRegs->DEVICE_CAPABILITIES_2.B.LONG_MESSAGE,
                pRegs->DEVICE_CAPABILITIES_2.B.SMBUS_PEC,
                pRegs->DEVICE_CAPABILITIES_2.B.SOURCE_FR_SWAP,
                pRegs->DEVICE_CAPABILITIES_2.B.SINK_FR_SWAP,
                pRegs->DEVICE_CAPABILITIES_2.B.WATCHDOG_TIMER,
                pRegs->DEVICE_CAPABILITIES_2.B.SINK_DISCONNECT_DETECTION,
                pRegs->DEVICE_CAPABILITIES_2.B.STOP_DISCHARGE_THRESHOLD,
                pRegs->DEVICE_CAPABILITIES_2.B.VBUS_VOLTAGE_ALARM_LSB==0?"25mV":pRegs->DEVICE_CAPABILITIES_2.B.VBUS_VOLTAGE_ALARM_LSB==1?"50mV":pRegs->DEVICE_CAPABILITIES_2.B.VBUS_VOLTAGE_ALARM_LSB==2?"100mV":"Reserved",
                pRegs->DEVICE_CAPABILITIES_2.B.VCONN_POWER_SUPPORTED==0?"1W":pRegs->DEVICE_CAPABILITIES_2.B.VCONN_POWER_SUPPORTED==1?"1.5W":pRegs->DEVICE_CAPABILITIES_2.B.VCONN_POWER_SUPPORTED==2?"2W":pRegs->DEVICE_CAPABILITIES_2.B.VCONN_POWER_SUPPORTED==3?"3W":pRegs->DEVICE_CAPABILITIES_2.B.VCONN_POWER_SUPPORTED==4?"4W":pRegs->DEVICE_CAPABILITIES_2.B.VCONN_POWER_SUPPORTED==5?"5W":pRegs->DEVICE_CAPABILITIES_2.B.VCONN_POWER_SUPPORTED==6?"6W":"Reserved",
                pRegs->DEVICE_CAPABILITIES_2.B.VCONN_OVERCURRENT_FAULT_CAPABLE
                );
            break;
        case TCPC_PHY_STANDARD_INPUT_CAPABILITIES:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (SrcFstRolSwp=%s, VBUSExtOvrVltgFlt=%d, VBUSExtOvrCrntFlt=%d, ForceOffVBUS=%d)", params?"Wr":"Rd", "STANDARD_INPUT_CAPABILITIES", TCPC_PHY_STANDARD_INPUT_CAPABILITIES, pRegs->STANDARD_INPUT_CAPABILITIES.R,
                pRegs->STANDARD_INPUT_CAPABILITIES.B.SOURCE_FAST_ROLE_SWAP==0?"no":pRegs->STANDARD_INPUT_CAPABILITIES.B.SOURCE_FAST_ROLE_SWAP==1?"input":pRegs->STANDARD_INPUT_CAPABILITIES.B.SOURCE_FAST_ROLE_SWAP==2?"In/Out":"Reserved",
                pRegs->STANDARD_INPUT_CAPABILITIES.B.VBUS_EXTERNAL_OVER_VOLTAGE_FAULT,
                pRegs->STANDARD_INPUT_CAPABILITIES.B.VBUS_EXTERNAL_OVER_CURRENT_FAULT,
                pRegs->STANDARD_INPUT_CAPABILITIES.B.FORCE_OFF_VBUS
            );
            break;
        case TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (VBUSSnkDscnDtctIndic=%d, DbgAccIndic=%d, VBUSPrsntMon=%d, AudioAdptAccIndic=%d, ActCbleIndic=%d, MUXCfgCtrl=%d, ConnectionPrsnt=%d, ConnectorOrient=%d)", params?"Wr":"Rd", "STANDARD_OUTPUT_CAPABILITIES", TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES, pRegs->STANDARD_OUTPUT_CAPABILITIES.R,
                pRegs->STANDARD_OUTPUT_CAPABILITIES.B.VBUS_SINK_DISCONNECT_DETECT_INDICATOR,
                pRegs->STANDARD_OUTPUT_CAPABILITIES.B.DEBUG_ACCESSORY_INDICATOR,
                pRegs->STANDARD_OUTPUT_CAPABILITIES.B.VBUS_PRESENT_MONITOR,
                pRegs->STANDARD_OUTPUT_CAPABILITIES.B.AUDIO_ADAPTER_ACCESSORY_INDICATOR,
                pRegs->STANDARD_OUTPUT_CAPABILITIES.B.ACTIVE_CABLE_INDICATOR,
                pRegs->STANDARD_OUTPUT_CAPABILITIES.B.MUX_CONFIGURATION_CONTROL,
                pRegs->STANDARD_OUTPUT_CAPABILITIES.B.CONNECTION_PRESENT,
                pRegs->STANDARD_OUTPUT_CAPABILITIES.B.CONNECTOR_ORIENTATION
                );
            break;

            
        case TCPC_PHY_EXT_CONTROL:
            DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X (BleedDischargeStrength=%d, VCONN Force Discharge=%s)", params?"Wr":"Rd", "EXT_CONTROL", TCPC_PHY_EXT_CONTROL, pRegs->EXT_CONTROL.R,
                pRegs->EXT_CONTROL.B.BLEED_DISCHARGE_STRENGTH,
                (pRegs->EXT_CONTROL.B.VCONN_FORCE_DISCHARGE == 0)? "dis":"ena"
            );
            break;
        default:
            if (Length == 1) {
                DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value:   0x%02X", params?"Wr":"Rd", Dbg_GetRegName(RegAddress), RegAddress, *(UINT8*)&(((UINT8*)pRegs)[RegAddress]));
            } else if (Length == 2) {
                DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value: 0x%04X", params?"Wr":"Rd", Dbg_GetRegName(RegAddress), RegAddress, *(UINT16*)&(((UINT8*)pRegs)[RegAddress]));
            } else {
                DBG_PRINT_REG("Reg%s: %-38s(0x%02X), Value: XXX,, Length: %d", params?"Wr":"Rd", Dbg_GetRegName(RegAddress), RegAddress, Length);
            }
            break;
    }
}

#define  GET_REGISTER(RegName) {TCPC_PHY_##RegName, (PVOID)&pDevContext->TCPI_PhyRegs.##RegName##, sizeof(TCPC_PHY_##RegName##_t)}

void TCPC_PHY_ReadAllRegs(PDEVICE_CONTEXT pDevContext) {
    REGISTER_ITEM RegsItems[] = {
        GET_REGISTER(VENDOR_ID),
        GET_REGISTER(PRODUCT_ID),
        GET_REGISTER(DEVICE_ID),
        GET_REGISTER(USBTYPEC_REV),
        GET_REGISTER(USBPD_REV_VER),
        GET_REGISTER(PD_INTERFACE_REV),
        GET_REGISTER(ALERT),
        GET_REGISTER(ALERT_MASK),
        GET_REGISTER(POWER_STATUS_MASK),
        GET_REGISTER(FAULT_STATUS_MASK),
        GET_REGISTER(EXTENDED_STATUS_MASK),
        GET_REGISTER(ALERT_EXTENDED_MASK),
        GET_REGISTER(CONFIG_STANDARD_OUTPUT),
        GET_REGISTER(TCPC_CONTROL),
        GET_REGISTER(ROLE_CONTROL),
        GET_REGISTER(FAULT_CONTROL),
        GET_REGISTER(POWER_CONTROL),
        GET_REGISTER(CC_STATUS),
        GET_REGISTER(POWER_STATUS),
        GET_REGISTER(FAULT_STATUS),
        GET_REGISTER(EXTENDED_STATUS),
        GET_REGISTER(ALERT_EXTENDED),
        GET_REGISTER(COMMAND),
        GET_REGISTER(DEVICE_CAPABILITIES_1),
        GET_REGISTER(DEVICE_CAPABILITIES_2),
        GET_REGISTER(STANDARD_INPUT_CAPABILITIES),
        GET_REGISTER(STANDARD_OUTPUT_CAPABILITIES),
        GET_REGISTER(CONFIG_EXTENDED1),
        GET_REGISTER(GENERIC_TIMER),
        GET_REGISTER(MESSAGE_HEADER_INFO),
        GET_REGISTER(RECEIVE_DETECT),
        GET_REGISTER(RX_BUF_BYTE_X),
        GET_REGISTER(TRANSMIT),
        GET_REGISTER(TX_BUF_BYTE_X),
        GET_REGISTER(VBUS_VOLTAGE),
        GET_REGISTER(VBUS_SINK_DISCONNECT_THRESHOLD),
        GET_REGISTER(VBUS_STOP_DISCHARGE_THRESHOLD),
        GET_REGISTER(VBUS_VOLTAGE_ALARM_HI_CFG),
        GET_REGISTER(VBUS_VOLTAGE_ALARM_LO_CFG),
        GET_REGISTER(VBUS_HV_TARGET),
        GET_REGISTER(EXT_CFG_ID),
        GET_REGISTER(EXT_ALERT),
        GET_REGISTER(EXT_ALERT_MASK),
        GET_REGISTER(EXT_CONFIG),
        GET_REGISTER(EXT_FAULT_CONFIG),
        GET_REGISTER(EXT_CONTROL),
        GET_REGISTER(EXT_STATUS),
        GET_REGISTER(EXT_GPIO_CONFIG),
        GET_REGISTER(EXT_GPIO_CONTROL),
        GET_REGISTER(EXT_GPIO_ALERT_CONFIG),
        GET_REGISTER(EXT_GPIO_STATUS),
        GET_REGISTER(SOURCE_HIGH_VOLTAGE_MB4B_TIME),
        GET_REGISTER(ADC_FILTER_CONTROL_1),
        GET_REGISTER(ADC_FILTER_CONTROL_2),
        GET_REGISTER(VCON_CONFIG),
        GET_REGISTER(VCON_FAULT_DEBOUNCE),
        GET_REGISTER(VCON_FAULT_RECOVERY),
        GET_REGISTER(VCON_FAULT_ATTEMPTS)
    };
 (void)I2C_ReadSynchronouslyMultiple(pDevContext, I2CRequestSourceClient, RegsItems, _countof(RegsItems));
}

#endif // DBG_MESSAGE_PRINTING