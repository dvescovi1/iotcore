/*
* This file is derived from Microsoft example available on https://github.com/microsoft/Windows-driver-samples/blob/master/usb/UcmTcpciCxClientSample
* Original file licensing:
* Copyright (c) 2015 Microsoft
* SPDX-License-Identifier: MS-PL
* NXP modifications are licensed under the same license
* Copyright 2020 NXP
*
*/

#include "imx_tcpci_driver.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, OnInterruptPassiveIsr)
#endif

#define RdReg(_Reg, _Buff) if (!NT_SUCCESS(ntStatus = I2C_ReadSynchronously (pDevContext, I2CRequestSourceAlertIsr, _Reg, &_Buff, sizeof(_Buff)))) {break;} 
#define WrReg(_Reg, _Buff) if (!NT_SUCCESS(ntStatus = I2C_WriteSynchronously(pDevContext, I2CRequestSourceAlertIsr, _Reg, &_Buff, sizeof(_Buff)))) {break;} 

/*++
Routine Description:
    Per the TCPCI spec, the port controller hardware will drive the Alert pin high when a hardware event occurs. This routine services such a hardware interrupt at PASSIVE_LEVEL.
    The routine determines if an interrupt is an alert from the port controller hardware; if so, it completes processing of the alert.
Arguments:
    Interrupt: A handle to a framework interrupt object.
    MessageID: If the device is using message-signaled interrupts (MSIs), this parameter is the message number that identifies the device's hardware interrupt message. Otherwise, this value is 0.
Return Value:
    TRUE if the function services the hardware interrupt. Otherwise, this function must return FALSE.

--*/
BOOLEAN OnInterruptPassiveIsr(_In_ WDFINTERRUPT PortControllerInterrupt, _In_ ULONG MessageID) {
    UNREFERENCED_PARAMETER(MessageID);
    PAGED_CODE();

    NTSTATUS                                ntStatus;
    PDEVICE_CONTEXT                         pDevContext;
    TCPC_PHY_ALERT_t                        AlertRegister;
    BOOLEAN                                 interruptRecognized = FALSE;
    int                                     NumAlertReports = 0;
    size_t                                  NumAlertsInReport;
    UCMTCPCI_PORT_CONTROLLER_ALERT_DATA     alertData;
    UCMTCPCI_PORT_CONTROLLER_RECEIVE_BUFFER receiveBuffer;
    UCMTCPCI_PORT_CONTROLLER_ALERT_DATA     hardwareAlerts[MAX_ALERTS];  // UcmTcpciCx expects the information on all of the alerts firing presently.


    pDevContext = DeviceGetContext(WdfInterruptGetDevice(PortControllerInterrupt));

    // Process the alerts as long as there are bits set in the alert register. Set a maximum number of alerts to process in this loop. If the hardware is messed up and we're unable
    // to quiesce the interrupt by writing to the alert register, then we don't want to be stuck in an infinite loop.
    while (NumAlertReports <= MAX_ALERTS_TO_PROCESS) {
        RdReg(TCPC_PHY_ALERT, AlertRegister);
        if (AlertRegister.R == 0) {  // If there are no bits set in the alert register, we should not service this interrupt.
            break;
        }      
        interruptRecognized = TRUE;         // Since there are bits set in the alert register, we can safely assume that the interrupt is ours to process.
        NumAlertsInReport = 0;
        do {
            if (AlertRegister.B.CC_STATUS == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertCCStatus;
                // We must read the CC status register and send the contents to UcmTcpciCx along with the CC status alert.
                RdReg(TCPC_PHY_CC_STATUS, alertData.CCStatus);
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.POWER_STATUS == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertPowerStatus;
                // We must read the power status register and send the contents to UcmTcpciCx along with the power status alert.
                RdReg(TCPC_PHY_POWER_STATUS, alertData.PowerStatus);
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.FAULT == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertFault;
                // We must read the fault status register and send the contents to UcmTcpciCx along with the fault alert.
                RdReg(TCPC_PHY_FAULT_STATUS, alertData.FaultStatus);
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
                // Clear FAULT_STATUS Register.
                // Mask reserved bit 7 in TCPCI Rev 1.0 Ver 1.0 only, see spec section 4.4.6.3
                alertData.FaultStatus.AsUInt8 &= 0x7F;
                WrReg(TCPC_PHY_FAULT_STATUS, alertData.FaultStatus);
            }
            if (AlertRegister.B.RECEIVED_SOP_MESSAGE_STATUS == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertReceiveSOPMessageStatus;
                // We must read the receive buffer register and send the contents to UcmTcpciCx along with the receive SOP alert.
                RdReg(TCPC_PHY_RX_BUF_BYTE_X, receiveBuffer);
                alertData.ReceiveBuffer = &receiveBuffer;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            // The remainder of the alert types do not require us to provide any extra information to UcmTcpciCx.
            if (AlertRegister.B.RECEIVED_HARD_RESET == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertReceivedHardReset;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.RX_BUFFER_OVERFLOW == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertRxBufferOverflow;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.TRANSMIT_SOP_MESSAGE_DISCARDED == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertTransmitSOPMessageDiscarded;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.TRANSMIT_SOP_MESSAGE_FAILED == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertTransmitSOPMessageFailed;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.TRANSMIT_SOP_MESSAGE_SUCCESSFUL == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertTransmitSOPMessageSuccessful;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.VBUS_SINK_DISCONNECT_DETECTED == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertVbusSinkDisconnectDetected;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.VBUS_VOLTAGE_ALARM_HI == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertVbusVoltageAlarmHi;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            if (AlertRegister.B.VBUS_VOLTAGE_ALARM_LO == 1) {
                UCMTCPCI_PORT_CONTROLLER_ALERT_DATA_INIT(&alertData);
                alertData.AlertType = UcmTcpciPortControllerAlertVbusVoltageAlarmLo;
                hardwareAlerts[NumAlertsInReport] = alertData;
                ++NumAlertsInReport;
            }
            WrReg(TCPC_PHY_ALERT, AlertRegister);
        } while (0);
        if (NT_SUCCESS(ntStatus)) {  
            UcmTcpciPortControllerAlert(pDevContext->PortController, hardwareAlerts, NumAlertsInReport);  
        } else {
            break;
        }
        ++NumAlertReports;
    }
    return interruptRecognized;
}
