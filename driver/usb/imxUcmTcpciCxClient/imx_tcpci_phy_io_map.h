/*
* Copyright 2020 NXP
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

#ifndef _TCPC_PHY_IO_MAP_H_
#define _TCPC_PHY_IO_MAP_H_

/* TCPC_PHY registers, offset from base address */
#define TCPC_PHY_VENDOR_ID                       0x0000  /* VENDOR_ID Register offset */
#define TCPC_PHY_PRODUCT_ID                      0x0002  /* PRODUCT_ID Register offset */
#define TCPC_PHY_DEVICE_ID                       0x0004  /* DEVICE_ID Register offset */
#define TCPC_PHY_USBTYPEC_REV                    0x0006  /* USBTYPEC_REV Register offset */
#define TCPC_PHY_USBPD_REV_VER                   0x0008  /* USBPD_REV_VER Register offset */
#define TCPC_PHY_PD_INTERFACE_REV                0x000A  /* PD_INTERFACE_REV Register offset */
#define TCPC_PHY_ALERT                           0x0010  /* ALERT Register offset */
#define TCPC_PHY_ALERT_MASK                      0x0012  /* ALERT_MASK Register offset */
#define TCPC_PHY_POWER_STATUS_MASK               0x0014  /* POWER_STATUS_MASK Register offset */
#define TCPC_PHY_FAULT_STATUS_MASK               0x0015  /* FAULT_STATUS_MASK Register offset */
#define TCPC_PHY_EXTENDED_STATUS_MASK            0x0016  /* EXTENDED_STATUS_MASK Register offset */
#define TCPC_PHY_ALERT_EXTENDED_MASK             0x0017  /* ALERT_EXTENDED_MASK Register offset */
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT          0x0018  /* CONFIG_STANDARD_OUTPUT Register offset */
#define TCPC_PHY_TCPC_CONTROL                    0x0019  /* TCPC_CONTROL Register offset */
#define TCPC_PHY_ROLE_CONTROL                    0x001A  /* ROLE_CONTROL Register offset */
#define TCPC_PHY_FAULT_CONTROL                   0x001B  /* FAULT_CONTROL Register offset */
#define TCPC_PHY_POWER_CONTROL                   0x001C  /* POWER_CONTROL Register offset */
#define TCPC_PHY_CC_STATUS                       0x001D  /* CC_STATUS Register offset */
#define TCPC_PHY_POWER_STATUS                    0x001E  /* POWER_STATUS Register offset */
#define TCPC_PHY_FAULT_STATUS                    0x001F  /* FAULT_STATUS Register offset */
#define TCPC_PHY_EXTENDED_STATUS                 0x0020  /* EXTENDED_STATUS Register offset */
#define TCPC_PHY_ALERT_EXTENDED                  0x0021  /* ALERT_EXTENDED Register offset */
#define TCPC_PHY_COMMAND                         0x0023  /* COMMAND Register offset */
#define TCPC_PHY_DEVICE_CAPABILITIES_1           0x0024  /* DEVICE_CAPABILITIES_1 Register offset */
#define TCPC_PHY_DEVICE_CAPABILITIES_2           0x0026  /* DEVICE_CAPABILITIES_2 Register offset */
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES     0x0028  /* STANDARD_INPUT_CAPABILITIES Register offset */
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES    0x0029  /* STANDARD_OUTPUT_CAPABILITIES Register offset */
#define TCPC_PHY_CONFIG_EXTENDED1                0x002A  /* CONFIG_EXTENDED1 Register offset */
#define TCPC_PHY_GENERIC_TIMER                   0x002C  /* GENERIC_TIMER Register offset */
#define TCPC_PHY_MESSAGE_HEADER_INFO             0x002E  /* MESSAGE_HEADER_INFO Register offset */
#define TCPC_PHY_RECEIVE_DETECT                  0x002F  /* RECEIVE_DETECT Register offset */
#define TCPC_PHY_READABLE_BYTE_COUNT             0x0030  /* READABLE_BYTE_COUNT Register offset */
#define TCPC_PHY_RX_BUF_FRAME_TYPE               0x0030  /* RX_BUF_FRAME_TYPE Register offset */
#define TCPC_PHY_RX_BUF_BYTE_X                   0x0030  /* RX_BUF_BYTE_x Register offset */
#define TCPC_PHY_TRANSMIT                        0x0050  /* TRANSMIT Register offset */
#define TCPC_PHY_I2C_WRITE_BYTE_COUNT            0x0051  /* I2C_WRITE_BYTE_COUNT Register offset */
#define TCPC_PHY_TX_BUF_BYTE_X                   0x0051  /* TX_BUF_BYTE_x Register offset */
#define TCPC_PHY_VBUS_VOLTAGE                    0x0070  /* VBUS_VOLTAGE Register offset */
#define TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD  0x0072  /* VBUS_SINK_DISCONNECT_THRESHOLD Register offset */
#define TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD   0x0074  /* VBUS_STOP_DISCHARGE_THRESHOLD Register offset */
#define TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG       0x0076  /* VBUS_VOLTAGE_ALARM_HI_CFG Register offset */
#define TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG       0x0078  /* VBUS_VOLTAGE_ALARM_LO_CFG Register offset */
#define TCPC_PHY_VBUS_HV_TARGET                  0x007A  /* VBUS_HV_TARGET Register offset */
#define TCPC_PHY_EXT_CFG_ID                      0x0080  /* For NXP internal use only offset */
#define TCPC_PHY_EXT_ALERT                       0x0082  /* EXT ALERT Register offset */
#define TCPC_PHY_EXT_ALERT_MASK                  0x0084  /* EXT ALERT MASK Register offset */
#define TCPC_PHY_EXT_CONFIG                      0x0086  /* EXT CONFIG Register offset */
#define TCPC_PHY_EXT_FAULT_CONFIG                0x0088  /* EXT FAULT CONFIG Register offset */
#define TCPC_PHY_EXT_CONTROL                     0x008E  /* Bleed Discharge Strength (Only applied on write to POWER_CONTROL) offset */
#define TCPC_PHY_EXT_STATUS                      0x0090  /* EXT STATUS Register offset */
#define TCPC_PHY_EXT_GPIO_CONFIG                 0x0092  /* EXT_GPIO_CONFIG Register offset */
#define TCPC_PHY_EXT_GPIO_CONTROL                0x0093  /* EXT_GPIO_CONFIG Register offset */
#define TCPC_PHY_EXT_GPIO_ALERT_CONFIG           0x0094  /* EXT_GPIO_ALERT_CONFIG Register offset */
#define TCPC_PHY_EXT_GPIO_STATUS                 0x0096  /* EXT_GPIO_CONFIG Register offset */
#define TCPC_PHY_SOURCE_HIGH_VOLTAGE_MB4B_TIME   0x0097  /* SOURCE_HIGH_VOLTAGE_MB4B_TIME Register offset */
#define TCPC_PHY_ADC_FILTER_CONTROL_1            0x009A  /* ADC_FILTER_CONTROL_1 Register offset */
#define TCPC_PHY_ADC_FILTER_CONTROL_2            0x009B  /* ADC_FILTER_CONTROL_2 Register offset */
#define TCPC_PHY_VCON_CONFIG                     0x009C  /* VCON_CONFIG Register offset */
#define TCPC_PHY_VCON_FAULT_DEBOUNCE             0x009D  /* VCON_FAULT_DEBOUNCE Register offset */
#define TCPC_PHY_VCON_FAULT_RECOVERY             0x009E  /* VCON_FAULT_RECOVERY Register offset */
#define TCPC_PHY_VCON_FAULT_ATTEMPTS             0x009F  /* VCON_FAULT_ATTEMPTS Register offset */

/*
* TCPC_PHY_VENDOR_ID register bits *
*/
#define TCPC_PHY_VENDOR_ID_VID_MASK                                                                            0x0000FFFF
#define TCPC_PHY_VENDOR_ID_VID_SHIFT                                                                           0

/*
* TCPC_PHY_PRODUCT_ID register bits *
*/
#define TCPC_PHY_PRODUCT_ID_PID_MASK                                                                           0x0000FFFF
#define TCPC_PHY_PRODUCT_ID_PID_SHIFT                                                                          0

/*
* TCPC_PHY_DEVICE_ID register bits *
*/
#define TCPC_PHY_DEVICE_ID_BCD_DEVICE_MASK                                                                     0x0000FFFF
#define TCPC_PHY_DEVICE_ID_BCD_DEVICE_SHIFT                                                                    0

/*
* TCPC_PHY_USBTYPEC_REV register bits *
*/
#define TCPC_PHY_USBTYPEC_REV_BCD_USBTYPEC_RELEASE_MASK                                                        0x000000FF
#define TCPC_PHY_USBTYPEC_REV_BCD_USBTYPEC_RELEASE_SHIFT                                                       0

/*
* TCPC_PHY_USBPD_REV_VER register bits *
*/
#define TCPC_PHY_USBPD_REV_VER_BCD_USBPD_REVISION_MASK                                                         0x0000FF00
#define TCPC_PHY_USBPD_REV_VER_BCD_USBPD_REVISION_SHIFT                                                        8
#define TCPC_PHY_USBPD_REV_VER_BCD_USBPD_VERSION_MASK                                                          0x000000FF
#define TCPC_PHY_USBPD_REV_VER_BCD_USBPD_VERSION_SHIFT                                                         0

/*
* TCPC_PHY_PD_INTERFACE_REV register bits *
*/
#define TCPC_PHY_PD_INTERFACE_REV_BCD_USB_PD_INTER_BLOCK_SPECIFICATION_REVISION_MASK                           0x0000FF00
#define TCPC_PHY_PD_INTERFACE_REV_BCD_USB_PD_INTER_BLOCK_SPECIFICATION_REVISION_SHIFT                          8
#define TCPC_PHY_PD_INTERFACE_REV_BCD_USB_PD_INTER_BLOCK_SPECIFICATION_VERSION_MASK                            0x000000FF
#define TCPC_PHY_PD_INTERFACE_REV_BCD_USB_PD_INTER_BLOCK_SPECIFICATION_VERSION_SHIFT                           0

/*
* TCPC_PHY_ALERT register bits *
*/
#define TCPC_PHY_ALERT_VENDOR_DEFINED_ALERT_MASK                                                               0x00008000
#define TCPC_PHY_ALERT_VENDOR_DEFINED_ALERT_SHIFT                                                              15
#define TCPC_PHY_ALERT_ALERT_EXTENDED_MASK                                                                     0x00004000
#define TCPC_PHY_ALERT_ALERT_EXTENDED_SHIFT                                                                    14
#define TCPC_PHY_ALERT_EXTENDED_STATUS_MASK                                                                    0x00002000
#define TCPC_PHY_ALERT_EXTENDED_STATUS_SHIFT                                                                   13
#define TCPC_PHY_ALERT_BEGINNING_SOP_MESSAGE_STATUS_MASK                                                       0x00001000
#define TCPC_PHY_ALERT_BEGINNING_SOP_MESSAGE_STATUS_SHIFT                                                      12
#define TCPC_PHY_ALERT_VBUS_SINK_DISCONNECT_DETECTED_MASK                                                      0x00000800
#define TCPC_PHY_ALERT_VBUS_SINK_DISCONNECT_DETECTED_SHIFT                                                     11
#define TCPC_PHY_ALERT_RX_BUFFER_OVERFLOW_MASK                                                                 0x00000400
#define TCPC_PHY_ALERT_RX_BUFFER_OVERFLOW_SHIFT                                                                10
#define TCPC_PHY_ALERT_FAULT_MASK                                                                              0x00000200
#define TCPC_PHY_ALERT_FAULT_SHIFT                                                                             9
#define TCPC_PHY_ALERT_VBUS_VOLTAGE_ALARM_LO_MASK                                                              0x00000100
#define TCPC_PHY_ALERT_VBUS_VOLTAGE_ALARM_LO_SHIFT                                                             8
#define TCPC_PHY_ALERT_VBUS_VOLTAGE_ALARM_HI_MASK                                                              0x00000080
#define TCPC_PHY_ALERT_VBUS_VOLTAGE_ALARM_HI_SHIFT                                                             7
#define TCPC_PHY_ALERT_TRANSMIT_SOP_MESSAGE_SUCCESSFUL_MASK                                                    0x00000040
#define TCPC_PHY_ALERT_TRANSMIT_SOP_MESSAGE_SUCCESSFUL_SHIFT                                                   6
#define TCPC_PHY_ALERT_TRANSMIT_SOP_MESSAGE_DISCARDED_MASK                                                     0x00000020
#define TCPC_PHY_ALERT_TRANSMIT_SOP_MESSAGE_DISCARDED_SHIFT                                                    5
#define TCPC_PHY_ALERT_TRANSMIT_SOP_MESSAGE_FAILED_MASK                                                        0x00000010
#define TCPC_PHY_ALERT_TRANSMIT_SOP_MESSAGE_FAILED_SHIFT                                                       4
#define TCPC_PHY_ALERT_RECEIVED_HARD_RESET_MASK                                                                0x00000008
#define TCPC_PHY_ALERT_RECEIVED_HARD_RESET_SHIFT                                                               3
#define TCPC_PHY_ALERT_RECEIVED_SOP_MESSAGE_STATUS_MASK                                                        0x00000004
#define TCPC_PHY_ALERT_RECEIVED_SOP_MESSAGE_STATUS_SHIFT                                                       2
#define TCPC_PHY_ALERT_POWER_STATUS_MASK                                                                       0x00000002
#define TCPC_PHY_ALERT_POWER_STATUS_SHIFT                                                                      1
#define TCPC_PHY_ALERT_CC_STATUS_MASK                                                                          0x00000001
#define TCPC_PHY_ALERT_CC_STATUS_SHIFT                                                                         0

/*
* TCPC_PHY_ALERT_MASK register bits *
*/
#define TCPC_PHY_ALERT_MASK_VENDOR_DEFINED_ALERT_INT_MASK_MASK                                                 0x00008000
#define TCPC_PHY_ALERT_MASK_VENDOR_DEFINED_ALERT_INT_MASK_SHIFT                                                15
#define TCPC_PHY_ALERT_MASK_ALERT_EXTENDED_INT_MASK_MASK                                                       0x00004000
#define TCPC_PHY_ALERT_MASK_ALERT_EXTENDED_INT_MASK_SHIFT                                                      14
#define TCPC_PHY_ALERT_MASK_EXTENDED_STATUS_INT_MASK_MASK                                                      0x00002000
#define TCPC_PHY_ALERT_MASK_EXTENDED_STATUS_INT_MASK_SHIFT                                                     13
#define TCPC_PHY_ALERT_MASK_BEGINNING_SOP_MESSAGE_STATUS_INT_MASK_MASK                                         0x00001000
#define TCPC_PHY_ALERT_MASK_BEGINNING_SOP_MESSAGE_STATUS_INT_MASK_SHIFT                                        12
#define TCPC_PHY_ALERT_MASK_VBUS_SINK_DISCONNECT_DETECTED_INT_MASK_MASK                                        0x00000800
#define TCPC_PHY_ALERT_MASK_VBUS_SINK_DISCONNECT_DETECTED_INT_MASK_SHIFT                                       11
#define TCPC_PHY_ALERT_MASK_RX_BUFFER_OVERFLOW_INT_MASK_MASK                                                   0x00000400
#define TCPC_PHY_ALERT_MASK_RX_BUFFER_OVERFLOW_INT_MASK_SHIFT                                                  10
#define TCPC_PHY_ALERT_MASK_FAULT_INT_MASK_MASK                                                                0x00000200
#define TCPC_PHY_ALERT_MASK_FAULT_INT_MASK_SHIFT                                                               9
#define TCPC_PHY_ALERT_MASK_VBUS_VOLTAGE_ALARM_LO_INT_MASK_MASK                                                0x00000100
#define TCPC_PHY_ALERT_MASK_VBUS_VOLTAGE_ALARM_LO_INT_MASK_SHIFT                                               8
#define TCPC_PHY_ALERT_MASK_VBUS_VOLTAGE_ALARM_HI_INT_MASK_MASK                                                0x00000080
#define TCPC_PHY_ALERT_MASK_VBUS_VOLTAGE_ALARM_HI_INT_MASK_SHIFT                                               7
#define TCPC_PHY_ALERT_MASK_TRANSMIT_SOP_MESSAGE_SUCCESSFUL_INT_MASK_MASK                                      0x00000040
#define TCPC_PHY_ALERT_MASK_TRANSMIT_SOP_MESSAGE_SUCCESSFUL_INT_MASK_SHIFT                                     6
#define TCPC_PHY_ALERT_MASK_TRANSMIT_SOP_MESSAGE_DISCARDED_INT_MASK_MASK                                       0x00000020
#define TCPC_PHY_ALERT_MASK_TRANSMIT_SOP_MESSAGE_DISCARDED_INT_MASK_SHIFT                                      5
#define TCPC_PHY_ALERT_MASK_TRANSMIT_SOP_MESSAGE_FAILED_INT_MASK_MASK                                          0x00000010
#define TCPC_PHY_ALERT_MASK_TRANSMIT_SOP_MESSAGE_FAILED_INT_MASK_SHIFT                                         4
#define TCPC_PHY_ALERT_MASK_RECEIVED_HARD_RESET_INT_MASK_MASK                                                  0x00000008
#define TCPC_PHY_ALERT_MASK_RECEIVED_HARD_RESET_INT_MASK_SHIFT                                                 3
#define TCPC_PHY_ALERT_MASK_RECEIVED_SOP_MESSAGE_STATUS_INT_MASK_MASK                                          0x00000004
#define TCPC_PHY_ALERT_MASK_RECEIVED_SOP_MESSAGE_STATUS_INT_MASK_SHIFT                                         2
#define TCPC_PHY_ALERT_MASK_POWER_STATUS_INT_MASK_MASK                                                         0x00000002
#define TCPC_PHY_ALERT_MASK_POWER_STATUS_INT_MASK_SHIFT                                                        1
#define TCPC_PHY_ALERT_MASK_CC_STATUS_INT_MASK_MASK                                                            0x00000001
#define TCPC_PHY_ALERT_MASK_CC_STATUS_INT_MASK_SHIFT                                                           0

/*
* TCPC_PHY_POWER_STATUS_MASK register bits *
*/
#define TCPC_PHY_POWER_STATUS_MASK_DEBUG_ACCESSORY_CONNECTED_STATUS_INT_MASK_MASK                              0x00000080
#define TCPC_PHY_POWER_STATUS_MASK_DEBUG_ACCESSORY_CONNECTED_STATUS_INT_MASK_SHIFT                             7
#define TCPC_PHY_POWER_STATUS_MASK_TCPC_INITIALIZATION_STATUS_INT_MASK_MASK                                    0x00000040
#define TCPC_PHY_POWER_STATUS_MASK_TCPC_INITIALIZATION_STATUS_INT_MASK_SHIFT                                   6
#define TCPC_PHY_POWER_STATUS_MASK_SOURCING_HIGH_VOLTAGE_STATUS_INT_MASK_MASK                                  0x00000020
#define TCPC_PHY_POWER_STATUS_MASK_SOURCING_HIGH_VOLTAGE_STATUS_INT_MASK_SHIFT                                 5
#define TCPC_PHY_POWER_STATUS_MASK_SOURCING_VBUS_STATUS_INT_MASK_MASK                                          0x00000010
#define TCPC_PHY_POWER_STATUS_MASK_SOURCING_VBUS_STATUS_INT_MASK_SHIFT                                         4
#define TCPC_PHY_POWER_STATUS_MASK_VBUS_DETECTION_STATUS_INT_MASK_MASK                                         0x00000008
#define TCPC_PHY_POWER_STATUS_MASK_VBUS_DETECTION_STATUS_INT_MASK_SHIFT                                        3
#define TCPC_PHY_POWER_STATUS_MASK_VBUS_PRESENT_STATUS_INT_MASK_MASK                                           0x00000004
#define TCPC_PHY_POWER_STATUS_MASK_VBUS_PRESENT_STATUS_INT_MASK_SHIFT                                          2
#define TCPC_PHY_POWER_STATUS_MASK_VCONN_PRESENT_STATUS_INT_MASK_MASK                                          0x00000002
#define TCPC_PHY_POWER_STATUS_MASK_VCONN_PRESENT_STATUS_INT_MASK_SHIFT                                         1
#define TCPC_PHY_POWER_STATUS_MASK_SINKING_VBUS_STATUS_INT_MASK_MASK                                           0x00000001
#define TCPC_PHY_POWER_STATUS_MASK_SINKING_VBUS_STATUS_INT_MASK_SHIFT                                          0

/*
* TCPC_PHY_FAULT_STATUS_MASK register bits *
*/
#define TCPC_PHY_FAULT_STATUS_MASK_ALLREGISTERSRESETTODEFAULT_INT_MASK_MASK                                    0x00000080
#define TCPC_PHY_FAULT_STATUS_MASK_ALLREGISTERSRESETTODEFAULT_INT_MASK_SHIFT                                   7
#define TCPC_PHY_FAULT_STATUS_MASK_FORCE_OFF_VBUS_INT_MASK_MASK                                                0x00000040
#define TCPC_PHY_FAULT_STATUS_MASK_FORCE_OFF_VBUS_INT_MASK_SHIFT                                               6
#define TCPC_PHY_FAULT_STATUS_MASK_AUTO_DISCHARGE_FAILED_INT_MASK_MASK                                         0x00000020
#define TCPC_PHY_FAULT_STATUS_MASK_AUTO_DISCHARGE_FAILED_INT_MASK_SHIFT                                        5
#define TCPC_PHY_FAULT_STATUS_MASK_FORCE_DISCHARGE_FAILED_INT_MASK_MASK                                        0x00000010
#define TCPC_PHY_FAULT_STATUS_MASK_FORCE_DISCHARGE_FAILED_INT_MASK_SHIFT                                       4
#define TCPC_PHY_FAULT_STATUS_MASK_INTERNAL_OR_EXTERNAL_OCP_VBUS_OVER_CURRENT_PROTECTION_FAULT_INT_MASK_MASK   0x00000008
#define TCPC_PHY_FAULT_STATUS_MASK_INTERNAL_OR_EXTERNAL_OCP_VBUS_OVER_CURRENT_PROTECTION_FAULT_INT_MASK_SHIFT  3
#define TCPC_PHY_FAULT_STATUS_MASK_INTERNAL_OR_EXTERNAL_OVP_VBUS_OVER_VOLTAGE_PROTECTION_FAULT_INT_MASK_MASK   0x00000004
#define TCPC_PHY_FAULT_STATUS_MASK_INTERNAL_OR_EXTERNAL_OVP_VBUS_OVER_VOLTAGE_PROTECTION_FAULT_INT_MASK_SHIFT  2
#define TCPC_PHY_FAULT_STATUS_MASK_VCONN_OVER_CURRENT_FAULT_INT_MASK_MASK                                      0x00000002
#define TCPC_PHY_FAULT_STATUS_MASK_VCONN_OVER_CURRENT_FAULT_INT_MASK_SHIFT                                     1
#define TCPC_PHY_FAULT_STATUS_MASK_I2C_INTERFACE_ERROR_INT_MASK_MASK                                           0x00000001
#define TCPC_PHY_FAULT_STATUS_MASK_I2C_INTERFACE_ERROR_INT_MASK_SHIFT                                          0

/*
* TCPC_PHY_EXTENDED_STATUS_MASK register bits *
*/
#define TCPC_PHY_EXTENDED_STATUS_MASK_VSAFE0V_INT_MASK_MASK                                                    0x00000001
#define TCPC_PHY_EXTENDED_STATUS_MASK_VSAFE0V_INT_MASK_SHIFT                                                   0

/*
* TCPC_PHY_ALERT_EXTENDED_MASK register bits *
*/
#define TCPC_PHY_ALERT_EXTENDED_MASK_TIMER_EXPIRED_INT_MASK_MASK                                               0x00000004
#define TCPC_PHY_ALERT_EXTENDED_MASK_TIMER_EXPIRED_INT_MASK_SHIFT                                              2
#define TCPC_PHY_ALERT_EXTENDED_MASK_SOURCE_FAST_ROLE_SWAP_INT_MASK_MASK                                       0x00000002
#define TCPC_PHY_ALERT_EXTENDED_MASK_SOURCE_FAST_ROLE_SWAP_INT_MASK_SHIFT                                      1
#define TCPC_PHY_ALERT_EXTENDED_MASK_SINK_FAST_ROLE_SWAP_INT_MASK_MASK                                         0x00000001
#define TCPC_PHY_ALERT_EXTENDED_MASK_SINK_FAST_ROLE_SWAP_INT_MASK_SHIFT                                        0

/*
* TCPC_PHY_CONFIG_STANDARD_OUTPUT register bits *
*/
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_HIGH_IMPEDANCE_OUTPUTS_MASK                                            0x00000080
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_HIGH_IMPEDANCE_OUTPUTS_SHIFT                                           7
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_DEBUG_ACCESSORY_CONNECTED_MASK                                         0x00000040
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_DEBUG_ACCESSORY_CONNECTED_SHIFT                                        6
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_AUDIO_ACCESSORY_CONNECTED_MASK                                         0x00000020
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_AUDIO_ACCESSORY_CONNECTED_SHIFT                                        5
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_ACTIVE_CABLE_CONNECTED_MASK                                            0x00000010
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_ACTIVE_CABLE_CONNECTED_SHIFT                                           4
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_MUX_CONTROL_MASK                                                       0x0000000C
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_MUX_CONTROL_SHIFT                                                      2
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_CONNECTION_PRESENT_MASK                                                0x00000002
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_CONNECTION_PRESENT_SHIFT                                               1
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_CONNECTOR_ORIENTATION_MASK                                             0x00000001
#define TCPC_PHY_CONFIG_STANDARD_OUTPUT_CONNECTOR_ORIENTATION_SHIFT                                            0

/*
* TCPC_PHY_TCPC_CONTROL register bits *
*/
#define TCPC_PHY_TCPC_CONTROL_ENABLE_SMBUS_PEC_MASK                                                            0x00000080
#define TCPC_PHY_TCPC_CONTROL_ENABLE_SMBUS_PEC_SHIFT                                                           7
#define TCPC_PHY_TCPC_CONTROL_ENABLE_LOOKING4CONNECTION_ALERT_MASK                                             0x00000040
#define TCPC_PHY_TCPC_CONTROL_ENABLE_LOOKING4CONNECTION_ALERT_SHIFT                                            6
#define TCPC_PHY_TCPC_CONTROL_ENABLE_WATCHDOG_TIMER_MASK                                                       0x00000020
#define TCPC_PHY_TCPC_CONTROL_ENABLE_WATCHDOG_TIMER_SHIFT                                                      5
#define TCPC_PHY_TCPC_CONTROL_DEBUG_ACCESSORY_CONTROL_MASK                                                     0x00000010
#define TCPC_PHY_TCPC_CONTROL_DEBUG_ACCESSORY_CONTROL_SHIFT                                                    4
#define TCPC_PHY_TCPC_CONTROL_I2C_CLOCK_STRETCHING_CONTROL_MASK                                                0x0000000C
#define TCPC_PHY_TCPC_CONTROL_I2C_CLOCK_STRETCHING_CONTROL_SHIFT                                               2
#define TCPC_PHY_TCPC_CONTROL_BIST_TEST_MODE_MASK                                                              0x00000002
#define TCPC_PHY_TCPC_CONTROL_BIST_TEST_MODE_SHIFT                                                             1
#define TCPC_PHY_TCPC_CONTROL_PLUG_ORIENTATION_MASK                                                            0x00000001
#define TCPC_PHY_TCPC_CONTROL_PLUG_ORIENTATION_SHIFT                                                           0

/*
* TCPC_PHY_ROLE_CONTROL register bits *
*/
#define TCPC_PHY_ROLE_CONTROL_DRP_MASK                                                                         0x00000040
#define TCPC_PHY_ROLE_CONTROL_DRP_SHIFT                                                                        6
#define TCPC_PHY_ROLE_CONTROL_DRP_DISABLED                                                                     0
#define TCPC_PHY_ROLE_CONTROL_DRP_ENABLED                                                                      1
#define TCPC_PHY_ROLE_CONTROL_RP_VALUE_MASK                                                                    0x00000030
#define TCPC_PHY_ROLE_CONTROL_RP_VALUE_SHIFT                                                                   4
#define TCPC_PHY_ROLE_CONTROL_RP_VALUE_RP_DEFAULT                                                              0
#define TCPC_PHY_ROLE_CONTROL_RP_VALUE_RP_1_5A                                                                 1
#define TCPC_PHY_ROLE_CONTROL_RP_VALUE_RP_3_0A                                                                 2
#define TCPC_PHY_ROLE_CONTROL_RP_VALUE_RESERVED                                                                3
#define TCPC_PHY_ROLE_CONTROL_CC2_MASK                                                                         0x0000000C
#define TCPC_PHY_ROLE_CONTROL_CC2_SHIFT                                                                        2
#define TCPC_PHY_ROLE_CONTROL_CC2_RA                                                                           0
#define TCPC_PHY_ROLE_CONTROL_CC2_RP                                                                           1
#define TCPC_PHY_ROLE_CONTROL_CC2_RD                                                                           2
#define TCPC_PHY_ROLE_CONTROL_CC2_OPEN                                                                         3
#define TCPC_PHY_ROLE_CONTROL_CC1_MASK                                                                         0x00000003
#define TCPC_PHY_ROLE_CONTROL_CC1_SHIFT                                                                        0
#define TCPC_PHY_ROLE_CONTROL_CC1_RA                                                                           0
#define TCPC_PHY_ROLE_CONTROL_CC1_RP                                                                           1
#define TCPC_PHY_ROLE_CONTROL_CC1_RD                                                                           2
#define TCPC_PHY_ROLE_CONTROL_CC1_OPEN                                                                         3

/*
* TCPC_PHY_FAULT_CONTROL register bits *
*/
#define TCPC_PHY_FAULT_CONTROL_FORCE_OFF_VBUS_MASK                                                             0x00000010
#define TCPC_PHY_FAULT_CONTROL_FORCE_OFF_VBUS_SHIFT                                                            4
#define TCPC_PHY_FAULT_CONTROL_VBUS_DISCHARGE_FAULT_DETECTION_TIMER_MASK                                       0x00000008
#define TCPC_PHY_FAULT_CONTROL_VBUS_DISCHARGE_FAULT_DETECTION_TIMER_SHIFT                                      3
#define TCPC_PHY_FAULT_CONTROL_INTERNAL_OR_EXTERNAL_VBUS_OCP_FAULT_MASK                                        0x00000004
#define TCPC_PHY_FAULT_CONTROL_INTERNAL_OR_EXTERNAL_VBUS_OCP_FAULT_SHIFT                                       2
#define TCPC_PHY_FAULT_CONTROL_INTERNAL_OR_EXTERNAL_VBUS_OVP_FAULT_MASK                                        0x00000002
#define TCPC_PHY_FAULT_CONTROL_INTERNAL_OR_EXTERNAL_VBUS_OVP_FAULT_SHIFT                                       1
#define TCPC_PHY_FAULT_CONTROL_VCONN_OVER_CURRENT_FAULT_MASK                                                   0x00000001
#define TCPC_PHY_FAULT_CONTROL_VCONN_OVER_CURRENT_FAULT_SHIFT                                                  0

/*
* TCPC_PHY_POWER_CONTROL register bits *
*/
#define TCPC_PHY_POWER_CONTROL_FAST_ROLE_SWAP_ENABLE_MASK                                                      0x00000080
#define TCPC_PHY_POWER_CONTROL_FAST_ROLE_SWAP_ENABLE_SHIFT                                                     7
#define TCPC_PHY_POWER_CONTROL_VBUS_VOLTAGE_MONITOR_MASK                                                       0x00000040
#define TCPC_PHY_POWER_CONTROL_VBUS_VOLTAGE_MONITOR_SHIFT                                                      6
#define TCPC_PHY_POWER_CONTROL_DISABLE_VOLTAGE_ALARMS_MASK                                                     0x00000020
#define TCPC_PHY_POWER_CONTROL_DISABLE_VOLTAGE_ALARMS_SHIFT                                                    5
#define TCPC_PHY_POWER_CONTROL_AUTO_DISCHARGE_DISCONNECT_MASK                                                  0x00000010
#define TCPC_PHY_POWER_CONTROL_AUTO_DISCHARGE_DISCONNECT_SHIFT                                                 4
#define TCPC_PHY_POWER_CONTROL_ENABLE_BLEED_DISCHARGE_MASK                                                     0x00000008
#define TCPC_PHY_POWER_CONTROL_ENABLE_BLEED_DISCHARGE_SHIFT                                                    3
#define TCPC_PHY_POWER_CONTROL_FORCE_DISCHARGE_MASK                                                            0x00000004
#define TCPC_PHY_POWER_CONTROL_FORCE_DISCHARGE_SHIFT                                                           2
#define TCPC_PHY_POWER_CONTROL_VCONN_POWER_SUPPORTED_MASK                                                      0x00000002
#define TCPC_PHY_POWER_CONTROL_VCONN_POWER_SUPPORTED_SHIFT                                                     1
#define TCPC_PHY_POWER_CONTROL_ENABLE_VCONN_MASK                                                               0x00000001
#define TCPC_PHY_POWER_CONTROL_ENABLE_VCONN_SHIFT                                                              0

/*
* TCPC_PHY_CC_STATUS register bits *
*/
#define TCPC_PHY_CC_STATUS_LOOKING4CONNECTION_MASK                                                             0x00000020
#define TCPC_PHY_CC_STATUS_LOOKING4CONNECTION_SHIFT                                                            5
#define TCPC_PHY_CC_STATUS_CONNECTRESULT_MASK                                                                  0x00000010
#define TCPC_PHY_CC_STATUS_CONNECTRESULT_SHIFT                                                                 4
#define TCPC_PHY_CC_STATUS_CC2_STATE_MASK                                                                      0x0000000C
#define TCPC_PHY_CC_STATUS_CC2_STATE_SHIFT                                                                     2
#define TCPC_PHY_CC_STATUS_CC1_STATE_MASK                                                                      0x00000003
#define TCPC_PHY_CC_STATUS_CC1_STATE_SHIFT                                                                     0

/*
* TCPC_PHY_POWER_STATUS register bits *
*/
#define TCPC_PHY_POWER_STATUS_DEBUG_ACCESSORY_CONNECTED_MASK                                                   0x00000080
#define TCPC_PHY_POWER_STATUS_DEBUG_ACCESSORY_CONNECTED_SHIFT                                                  7
#define TCPC_PHY_POWER_STATUS_TCPC_INITIALIZATION_STATUS_MASK                                                  0x00000040
#define TCPC_PHY_POWER_STATUS_TCPC_INITIALIZATION_STATUS_SHIFT                                                 6
#define TCPC_PHY_POWER_STATUS_SOURCING_HIGH_VOLTAGE_MASK                                                       0x00000020
#define TCPC_PHY_POWER_STATUS_SOURCING_HIGH_VOLTAGE_SHIFT                                                      5
#define TCPC_PHY_POWER_STATUS_SOURCING_VBUS_MASK                                                               0x00000010
#define TCPC_PHY_POWER_STATUS_SOURCING_VBUS_SHIFT                                                              4
#define TCPC_PHY_POWER_STATUS_VBUS_DETECTION_ENABLED_MASK                                                      0x00000008
#define TCPC_PHY_POWER_STATUS_VBUS_DETECTION_ENABLED_SHIFT                                                     3
#define TCPC_PHY_POWER_STATUS_VBUS_PRESENT_MASK                                                                0x00000004
#define TCPC_PHY_POWER_STATUS_VBUS_PRESENT_SHIFT                                                               2
#define TCPC_PHY_POWER_STATUS_VCONN_PRESENT_MASK                                                               0x00000002
#define TCPC_PHY_POWER_STATUS_VCONN_PRESENT_SHIFT                                                              1
#define TCPC_PHY_POWER_STATUS_SINKING_VBUS_MASK                                                                0x00000001
#define TCPC_PHY_POWER_STATUS_SINKING_VBUS_SHIFT                                                               0

/*
* TCPC_PHY_FAULT_STATUS register bits *
*/
#define TCPC_PHY_FAULT_STATUS_ALL_REGISTERS_RESET_TO_DEFAULT_MASK                                              0x00000080
#define TCPC_PHY_FAULT_STATUS_ALL_REGISTERS_RESET_TO_DEFAULT_SHIFT                                             7
#define TCPC_PHY_FAULT_STATUS_FORCE_OFF_VBUS_MASK                                                              0x00000040
#define TCPC_PHY_FAULT_STATUS_FORCE_OFF_VBUS_SHIFT                                                             6
#define TCPC_PHY_FAULT_STATUS_AUTO_DISCHARGE_FAILED_MASK                                                       0x00000020
#define TCPC_PHY_FAULT_STATUS_AUTO_DISCHARGE_FAILED_SHIFT                                                      5
#define TCPC_PHY_FAULT_STATUS_FORCE_DISCHARGE_FAILED_MASK                                                      0x00000010
#define TCPC_PHY_FAULT_STATUS_FORCE_DISCHARGE_FAILED_SHIFT                                                     4
#define TCPC_PHY_FAULT_STATUS_INTERNAL_OR_EXTERNAL_VBUS_OCP_FAULT_MASK                                         0x00000008
#define TCPC_PHY_FAULT_STATUS_INTERNAL_OR_EXTERNAL_VBUS_OCP_FAULT_SHIFT                                        3
#define TCPC_PHY_FAULT_STATUS_INTERNAL_OR_EXTERNAL_VBUS_OVP_FAULT_MASK                                         0x00000004
#define TCPC_PHY_FAULT_STATUS_INTERNAL_OR_EXTERNAL_VBUS_OVP_FAULT_SHIFT                                        2
#define TCPC_PHY_FAULT_STATUS_VCONN_OVER_CURRENT_FAULT_MASK                                                    0x00000002
#define TCPC_PHY_FAULT_STATUS_VCONN_OVER_CURRENT_FAULT_SHIFT                                                   1
#define TCPC_PHY_FAULT_STATUS_I2C_INTERFACE_ERROR_MASK                                                         0x00000001
#define TCPC_PHY_FAULT_STATUS_I2C_INTERFACE_ERROR_SHIFT                                                        0

/*
* TCPC_PHY_EXTENDED_STATUS register bits *
*/
#define TCPC_PHY_EXTENDED_STATUS_VSAFE0V_MASK                                                                  0x00000001
#define TCPC_PHY_EXTENDED_STATUS_VSAFE0V_SHIFT                                                                 0

/*
* TCPC_PHY_ALERT_EXTENDED register bits *
*/
#define TCPC_PHY_ALERT_EXTENDED_TIMER_EXPIRED_MASK                                                             0x00000004
#define TCPC_PHY_ALERT_EXTENDED_TIMER_EXPIRED_SHIFT                                                            2
#define TCPC_PHY_ALERT_EXTENDED_SOURCE_FAST_ROLE_SWAP_MASK                                                     0x00000002
#define TCPC_PHY_ALERT_EXTENDED_SOURCE_FAST_ROLE_SWAP_SHIFT                                                    1
#define TCPC_PHY_ALERT_EXTENDED_SINK_FAST_ROLE_SWAP_MASK                                                       0x00000001
#define TCPC_PHY_ALERT_EXTENDED_SINK_FAST_ROLE_SWAP_SHIFT                                                      0

/*
* TCPC_PHY_COMMAND register bits *
*/
#define TCPC_PHY_COMMAND_CMD_MASK                                                                              0x000000FF
#define TCPC_PHY_COMMAND_CMD_SHIFT                                                                             0
#define TCPC_PHY_COMMAND_CMD_WAKEI2C                                                                           17
#define TCPC_PHY_COMMAND_CMD_DISABLE_VBUS_DETECT                                                               34
#define TCPC_PHY_COMMAND_CMD_ENABLE_VBUS_DETECT                                                                51
#define TCPC_PHY_COMMAND_CMD_DISABLE_SINK_VBUS                                                                 68
#define TCPC_PHY_COMMAND_CMD_SINK_VBUS                                                                         85
#define TCPC_PHY_COMMAND_CMD_DISABLE_SOURCE_VBUS                                                               102
#define TCPC_PHY_COMMAND_CMD_SOURCE_VBUS_DEFAULT_VOLTAGE                                                       119
#define TCPC_PHY_COMMAND_CMD_SOURCE_VBUS_HIGH_VOLTAGE                                                          136
#define TCPC_PHY_COMMAND_CMD_LOOK_4_CONNECTION                                                                 153
#define TCPC_PHY_COMMAND_CMD_RX_ONE_MORE                                                                       170
#define TCPC_PHY_COMMAND_CMD_SEND_FRSWAP_SIGNAL                                                                204
#define TCPC_PHY_COMMAND_CMD_RESET_TRANSMIT_BUFFER                                                             221
#define TCPC_PHY_COMMAND_CMD_RESET_RECEIVE_BUFFER                                                              238
#define TCPC_PHY_COMMAND_CMD_I2C_IDLE                                                                          255

/*
* TCPC_PHY_DEVICE_CAPABILITIES_1 register bits *
*/
#define TCPC_PHY_DEVICE_CAPABILITIES_1_VBUS_HIGH_VOLTAGE_TARGET_MASK                                           0x00008000
#define TCPC_PHY_DEVICE_CAPABILITIES_1_VBUS_HIGH_VOLTAGE_TARGET_SHIFT                                          15
#define TCPC_PHY_DEVICE_CAPABILITIES_1_VBUS_OCP_REPORTING_MASK                                                 0x00004000
#define TCPC_PHY_DEVICE_CAPABILITIES_1_VBUS_OCP_REPORTING_SHIFT                                                14
#define TCPC_PHY_DEVICE_CAPABILITIES_1_VBUS_OVP_REPORTING_MASK                                                 0x00002000
#define TCPC_PHY_DEVICE_CAPABILITIES_1_VBUS_OVP_REPORTING_SHIFT                                                13
#define TCPC_PHY_DEVICE_CAPABILITIES_1_BLEED_DISCHARGE_MASK                                                    0x00001000
#define TCPC_PHY_DEVICE_CAPABILITIES_1_BLEED_DISCHARGE_SHIFT                                                   12
#define TCPC_PHY_DEVICE_CAPABILITIES_1_FORCE_DISCHARGE_MASK                                                    0x00000800
#define TCPC_PHY_DEVICE_CAPABILITIES_1_FORCE_DISCHARGE_SHIFT                                                   11
#define TCPC_PHY_DEVICE_CAPABILITIES_1_VBUS_MEASUREMENT_AND_ALARM_CAPABLE_MASK                                 0x00000400
#define TCPC_PHY_DEVICE_CAPABILITIES_1_VBUS_MEASUREMENT_AND_ALARM_CAPABLE_SHIFT                                10
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOURCE_RESISTOR_SUPPORTED_MASK                                          0x00000300
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOURCE_RESISTOR_SUPPORTED_SHIFT                                         8
#define TCPC_PHY_DEVICE_CAPABILITIES_1_POWER_ROLES_SUPPORTED_MASK                                              0x000000E0
#define TCPC_PHY_DEVICE_CAPABILITIES_1_POWER_ROLES_SUPPORTED_SHIFT                                             5
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOP_DBG_SOP_DBG_SUPPORT_MASK                                            0x00000010
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOP_DBG_SOP_DBG_SUPPORT_SHIFT                                           4
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOURCE_VCONN_MASK                                                       0x00000008
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOURCE_VCONN_SHIFT                                                      3
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SINK_VBUS_MASK                                                          0x00000004
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SINK_VBUS_SHIFT                                                         2
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOURCE_HIGH_VOLTAGE_VBUS_MASK                                           0x00000002
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOURCE_HIGH_VOLTAGE_VBUS_SHIFT                                          1
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOURCE_VBUS_MASK                                                        0x00000001
#define TCPC_PHY_DEVICE_CAPABILITIES_1_SOURCE_VBUS_SHIFT                                                       0

/*
* TCPC_PHY_DEVICE_CAPABILITIES_2 register bits *
*/
#define TCPC_PHY_DEVICE_CAPABILITIES_2_GENERIC_TIMER_MASK                                                      0x00002000
#define TCPC_PHY_DEVICE_CAPABILITIES_2_GENERIC_TIMER_SHIFT                                                     13
#define TCPC_PHY_DEVICE_CAPABILITIES_2_LONG_MESSAGE_MASK                                                       0x00001000
#define TCPC_PHY_DEVICE_CAPABILITIES_2_LONG_MESSAGE_SHIFT                                                      12
#define TCPC_PHY_DEVICE_CAPABILITIES_2_SMBUS_PEC_MASK                                                          0x00000800
#define TCPC_PHY_DEVICE_CAPABILITIES_2_SMBUS_PEC_SHIFT                                                         11
#define TCPC_PHY_DEVICE_CAPABILITIES_2_SOURCE_FR_SWAP_MASK                                                     0x00000400
#define TCPC_PHY_DEVICE_CAPABILITIES_2_SOURCE_FR_SWAP_SHIFT                                                    10
#define TCPC_PHY_DEVICE_CAPABILITIES_2_SINK_FR_SWAP_MASK                                                       0x00000200
#define TCPC_PHY_DEVICE_CAPABILITIES_2_SINK_FR_SWAP_SHIFT                                                      9
#define TCPC_PHY_DEVICE_CAPABILITIES_2_WATCHDOG_TIMER_MASK                                                     0x00000100
#define TCPC_PHY_DEVICE_CAPABILITIES_2_WATCHDOG_TIMER_SHIFT                                                    8
#define TCPC_PHY_DEVICE_CAPABILITIES_2_SINK_DISCONNECT_DETECTION_MASK                                          0x00000080
#define TCPC_PHY_DEVICE_CAPABILITIES_2_SINK_DISCONNECT_DETECTION_SHIFT                                         7
#define TCPC_PHY_DEVICE_CAPABILITIES_2_STOP_DISCHARGE_THRESHOLD_MASK                                           0x00000040
#define TCPC_PHY_DEVICE_CAPABILITIES_2_STOP_DISCHARGE_THRESHOLD_SHIFT                                          6
#define TCPC_PHY_DEVICE_CAPABILITIES_2_VBUS_VOLTAGE_ALARM_LSB_MASK                                             0x00000030
#define TCPC_PHY_DEVICE_CAPABILITIES_2_VBUS_VOLTAGE_ALARM_LSB_SHIFT                                            4
#define TCPC_PHY_DEVICE_CAPABILITIES_2_VCONN_POWER_SUPPORTED_MASK                                              0x0000000E
#define TCPC_PHY_DEVICE_CAPABILITIES_2_VCONN_POWER_SUPPORTED_SHIFT                                             1
#define TCPC_PHY_DEVICE_CAPABILITIES_2_VCONN_OVERCURRENT_FAULT_CAPABLE_MASK                                    0x00000001
#define TCPC_PHY_DEVICE_CAPABILITIES_2_VCONN_OVERCURRENT_FAULT_CAPABLE_SHIFT                                   0

/*
* TCPC_PHY_STANDARD_INPUT_CAPABILITIES register bits *
*/
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES_SOURCE_FAST_ROLE_SWAP_MASK                                        0x00000018
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES_SOURCE_FAST_ROLE_SWAP_SHIFT                                       3
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES_VBUS_EXTERNAL_OVER_VOLTAGE_FAULT_MASK                             0x00000004
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES_VBUS_EXTERNAL_OVER_VOLTAGE_FAULT_SHIFT                            2
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES_VBUS_EXTERNAL_OVER_CURRENT_FAULT_MASK                             0x00000002
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES_VBUS_EXTERNAL_OVER_CURRENT_FAULT_SHIFT                            1
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES_FORCE_OFF_VBUS_MASK                                               0x00000001
#define TCPC_PHY_STANDARD_INPUT_CAPABILITIES_FORCE_OFF_VBUS_SHIFT                                              0

/*
* TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES register bits *
*/
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_VBUS_SINK_DISCONNECT_DETECT_INDICATOR_MASK                       0x00000080
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_VBUS_SINK_DISCONNECT_DETECT_INDICATOR_SHIFT                      7
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_DEBUG_ACCESSORY_INDICATOR_MASK                                   0x00000040
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_DEBUG_ACCESSORY_INDICATOR_SHIFT                                  6
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_VBUS_PRESENT_MONITOR_MASK                                        0x00000020
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_VBUS_PRESENT_MONITOR_SHIFT                                       5
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_AUDIO_ADAPTER_ACCESSORY_INDICATOR_MASK                           0x00000010
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_AUDIO_ADAPTER_ACCESSORY_INDICATOR_SHIFT                          4
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_ACTIVE_CABLE_INDICATOR_MASK                                      0x00000008
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_ACTIVE_CABLE_INDICATOR_SHIFT                                     3
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_MUX_CONFIGURATION_CONTROL_MASK                                   0x00000004
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_MUX_CONFIGURATION_CONTROL_SHIFT                                  2
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_CONNECTION_PRESENT_MASK                                          0x00000002
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_CONNECTION_PRESENT_SHIFT                                         1
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_CONNECTOR_ORIENTATION_MASK                                       0x00000001
#define TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_CONNECTOR_ORIENTATION_SHIFT                                      0

/*
* TCPC_PHY_CONFIG_EXTENDED1 register bits *
*/
#define TCPC_PHY_CONFIG_EXTENDED1_FR_SWAP_BIDIRECTIONAL_PIN_MASK                                               0x00000080
#define TCPC_PHY_CONFIG_EXTENDED1_FR_SWAP_BIDIRECTIONAL_PIN_SHIFT                                              7
#define TCPC_PHY_CONFIG_EXTENDED1_STANDARD_INPUT_SOURCE_FR_SWAP_MASK                                           0x00000040
#define TCPC_PHY_CONFIG_EXTENDED1_STANDARD_INPUT_SOURCE_FR_SWAP_SHIFT                                          6

/*
* TCPC_PHY_MESSAGE_HEADER_INFO register bits *
*/
#define TCPC_PHY_MESSAGE_HEADER_INFO_XXX_MASK                                                                  0x00000080
#define TCPC_PHY_MESSAGE_HEADER_INFO_XXX_SHIFT                                                                 7

/*
* TCPC_PHY_RECEIVE_DETECT register bits *
*/
#define TCPC_PHY_RECEIVE_DETECT_XXX_MASK                                                                       0x00000080
#define TCPC_PHY_RECEIVE_DETECT_XXX_SHIFT                                                                      7

/*
* TCPC_PHY_READABLE_BYTE_COUNT register bits *
*/
#define TCPC_PHY_READABLE_BYTE_COUNT_XXX_MASK                                                                  0x00000080
#define TCPC_PHY_READABLE_BYTE_COUNT_XXX_SHIFT                                                                 7

/*
* TCPC_PHY_RX_BUF_FRAME_TYPE register bits *
*/
#define TCPC_PHY_RX_BUF_FRAME_TYPE_XXX_MASK                                                                    0x00000080
#define TCPC_PHY_RX_BUF_FRAME_TYPE_XXX_SHIFT                                                                   7

/*
* TCPC_PHY_RX_BUF_BYTE_X register bits *
*/
#define TCPC_PHY_RX_BUF_BYTE_X_XXX_MASK                                                                        0x00000080
#define TCPC_PHY_RX_BUF_BYTE_X_XXX_SHIFT                                                                       7

/*
* TCPC_PHY_TRANSMIT register bits *
*/
#define TCPC_PHY_TRANSMIT_XXX_MASK                                                                             0x00000080
#define TCPC_PHY_TRANSMIT_XXX_SHIFT                                                                            7

/*
* TCPC_PHY_I2C_WRITE_BYTE_COUNT register bits *
*/
#define TCPC_PHY_I2C_WRITE_BYTE_COUNT_XXX_MASK                                                                 0x00000080
#define TCPC_PHY_I2C_WRITE_BYTE_COUNT_XXX_SHIFT                                                                7

/*
* TCPC_PHY_TX_BUF_BYTE_X register bits *
*/
#define TCPC_PHY_TX_BUF_BYTE_X_XXX_MASK                                                                        0x00000080
#define TCPC_PHY_TX_BUF_BYTE_X_XXX_SHIFT                                                                       7

/*
* TCPC_PHY_VBUS_VOLTAGE register bits *
*/
#define TCPC_PHY_VBUS_VOLTAGE_SCALE_FACTOR_MASK                                                                0x00000C00
#define TCPC_PHY_VBUS_VOLTAGE_SCALE_FACTOR_SHIFT                                                               10
#define TCPC_PHY_VBUS_VOLTAGE_VBUS_VOLTAGE_MEASUREMENT_MASK                                                    0x000003FF
#define TCPC_PHY_VBUS_VOLTAGE_VBUS_VOLTAGE_MEASUREMENT_SHIFT                                                   0

/*
* TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD register bits *
*/
#define TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD_VOLTAGE_TRIP_POINT_MASK                                        0x000003FF
#define TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD_VOLTAGE_TRIP_POINT_SHIFT                                       0

/*
* TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD register bits *
*/
#define TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD_VOLTAGE_TRIP_POINT_MASK                                         0x000003FF
#define TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD_VOLTAGE_TRIP_POINT_SHIFT                                        0

/*
* TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG register bits *
*/
#define TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG_VOLTAGE_TRIP_POINT_MASK                                             0x000003FF
#define TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG_VOLTAGE_TRIP_POINT_SHIFT                                            0

/*
* TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG register bits *
*/
#define TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG_VOLTAGE_TRIP_POINT_MASK                                             0x000003FF
#define TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG_VOLTAGE_TRIP_POINT_SHIFT                                            0

/*
* TCPC_PHY_EXT_CONTROL register bits *
*/
#define TCPC_PHY_EXT_CONTROL_BLEED_DISCHARGE_STRENGTH_MASK                                                     0x000000F0
#define TCPC_PHY_EXT_CONTROL_BLEED_DISCHARGE_STRENGTH_SHIFT                                                    4
#define TCPC_PHY_EXT_CONTROL_VCONN_FORCE_DISCHARGE_MASK                                                        0x00000001
#define TCPC_PHY_EXT_CONTROL_VCONN_FORCE_DISCHARGE_SHIFT                                                       0

/*
* TCPC_PHY_EXT_GPIO_CONFIG register bits *
*/
#define TCPC_PHY_EXT_GPIO_CONFIG_FRS_EN_MASK                                                                   0x00000040
#define TCPC_PHY_EXT_GPIO_CONFIG_FRS_EN_SHIFT                                                                  6
#define TCPC_PHY_EXT_GPIO_CONFIG_EN_SRC_MASK                                                                   0x00000020
#define TCPC_PHY_EXT_GPIO_CONFIG_EN_SRC_SHIFT                                                                  5
#define TCPC_PHY_EXT_GPIO_CONFIG_EN_SNK1_MASK                                                                  0x00000010
#define TCPC_PHY_EXT_GPIO_CONFIG_EN_SNK1_SHIFT                                                                 4
#define TCPC_PHY_EXT_GPIO_CONFIG_IILIM_5V_VBUS_MASK                                                            0x00000008
#define TCPC_PHY_EXT_GPIO_CONFIG_IILIM_5V_VBUS_SHIFT                                                           3

/*
* TCPC_PHY_EXT_GPIO_CONTROL register bits *
*/
#define TCPC_PHY_EXT_GPIO_CONTROL_FRS_EN_MASK                                                                  0x00000040
#define TCPC_PHY_EXT_GPIO_CONTROL_FRS_EN_SHIFT                                                                 6
#define TCPC_PHY_EXT_GPIO_CONTROL_EN_SRC_MASK                                                                  0x00000020
#define TCPC_PHY_EXT_GPIO_CONTROL_EN_SRC_SHIFT                                                                 5
#define TCPC_PHY_EXT_GPIO_CONTROL_EN_SNK1_MASK                                                                 0x00000010
#define TCPC_PHY_EXT_GPIO_CONTROL_EN_SNK1_SHIFT                                                                4
#define TCPC_PHY_EXT_GPIO_CONTROL_IILIM_5V_VBUS_MASK                                                           0x00000008
#define TCPC_PHY_EXT_GPIO_CONTROL_IILIM_5V_VBUS_SHIFT                                                          3

/*
* TCPC_PHY_EXT_GPIO_ALERT_CONFIG register bits *
*/
#define TCPC_PHY_EXT_GPIO_ALERT_CONFIG_IILIM_5V_VBUS_MASK                                                      0x00000038
#define TCPC_PHY_EXT_GPIO_ALERT_CONFIG_IILIM_5V_VBUS_SHIFT                                                     3

/*
* TCPC_PHY_EXT_GPIO_STATUS register bits *
*/
#define TCPC_PHY_EXT_GPIO_STATUS_FRS_EN_MASK                                                                   0x00000040
#define TCPC_PHY_EXT_GPIO_STATUS_FRS_EN_SHIFT                                                                  6
#define TCPC_PHY_EXT_GPIO_STATUS_IILIM_5V_VBUS_MASK                                                            0x00000008
#define TCPC_PHY_EXT_GPIO_STATUS_IILIM_5V_VBUS_SHIFT                                                           3

/*
* TCPC_PHY_VENDOR_ID register bits definition *
*/
typedef union TCPC_PHY_VENDOR_ID_union_t {
    UINT16 R;
    struct {
        UINT16 VID  : 16;  /* A unique 16-bit unsigned integer assigned by the USB-IF to the Vendor. */
    } B;
} TCPC_PHY_VENDOR_ID_t;

/*
* TCPC_PHY_PRODUCT_ID register bits definition *
*/
typedef union TCPC_PHY_PRODUCT_ID_union_t {
    UINT16 R;
    struct {
        UINT16 PID  : 16;  /* A unique 16-bit unsigned integer assigned uniquely by the Vendor to identify the TCPC. */
    } B;
} TCPC_PHY_PRODUCT_ID_t;

/*
* TCPC_PHY_DEVICE_ID register bits definition *
*/
typedef union TCPC_PHY_DEVICE_ID_union_t {
    UINT16 R;
    struct {
        UINT16 BCD_DEVICE  : 16;  /* A unique 16-bit unsigned integer assigned by the Vendor to identify the version of the TCPC. */
    } B;
} TCPC_PHY_DEVICE_ID_t;

/*
* TCPC_PHY_USBTYPEC_REV register bits definition *
*/
typedef union TCPC_PHY_USBTYPEC_REV_union_t {
    UINT16 R;
    struct {
        UINT16 BCD_USBTYPEC_RELEASE  :  8;  /* USB Type-C Cable and Connector Specification Revision */
        UINT16 Reserved_8            :  8;  /* Reserved */
    } B;
} TCPC_PHY_USBTYPEC_REV_t;

/*
* TCPC_PHY_USBPD_REV_VER register bits definition *
*/
typedef union TCPC_PHY_USBPD_REV_VER_union_t {
    UINT16 R;
    struct {
        UINT16 BCD_USBPD_VERSION   :  8;  /* USB PD Specification Version */
        UINT16 BCD_USBPD_REVISION  :  8;  /* USB PD Specification Revision */
    } B;
} TCPC_PHY_USBPD_REV_VER_t;

/*
* TCPC_PHY_PD_INTERFACE_REV register bits definition *
*/
typedef union TCPC_PHY_PD_INTERFACE_REV_union_t {
    UINT16 R;
    struct {
        UINT16 BCD_USB_PD_INTER_BLOCK_SPECIFICATION_VERSION   :  8;  /* USB-Port Controller Specification Version */
        UINT16 BCD_USB_PD_INTER_BLOCK_SPECIFICATION_REVISION  :  8;  /* USB-Port Controller Specification Revision */
    } B;
} TCPC_PHY_PD_INTERFACE_REV_t;

/*
* TCPC_PHY_ALERT register bits definition *
*/
typedef union TCPC_PHY_ALERT_union_t {
    UINT16 R;
    struct {
        UINT16 CC_STATUS                        :  1;  /* CC Status. */
        UINT16 POWER_STATUS                     :  1;  /* Power Status. */
        UINT16 RECEIVED_SOP_MESSAGE_STATUS      :  1;  /* Received SOP* Message Status. */
        UINT16 RECEIVED_HARD_RESET              :  1;  /* Received Hard Reset. */
        UINT16 TRANSMIT_SOP_MESSAGE_FAILED      :  1;  /* Transmit SOP* Message Failed. */
        UINT16 TRANSMIT_SOP_MESSAGE_DISCARDED   :  1;  /* Transmit SOP* Message Discarded. */
        UINT16 TRANSMIT_SOP_MESSAGE_SUCCESSFUL  :  1;  /* Transmit SOP* Message Successfulw. */
        UINT16 VBUS_VOLTAGE_ALARM_HI            :  1;  /* VBUS Voltage Alarm Hi. */
        UINT16 VBUS_VOLTAGE_ALARM_LO            :  1;  /* VBUS Voltage Alarm Lo. */
        UINT16 FAULT                            :  1;  /* Fault. */
        UINT16 RX_BUFFER_OVERFLOW               :  1;  /* Rx Buffer Overflow. */
        UINT16 VBUS_SINK_DISCONNECT_DETECTED    :  1;  /* VBUS Sink Disconnect Detected. */
        UINT16 BEGINNING_SOP_MESSAGE_STATUS     :  1;  /* Beginning SOP* Message Status. */
        UINT16 EXTENDED_STATUS                  :  1;  /* Extended Status. */
        UINT16 ALERT_EXTENDED                   :  1;  /* Alert Extended. */
        UINT16 VENDOR_DEFINED_ALERT             :  1;  /* Vendor Defined Alert. */
    } B;
} TCPC_PHY_ALERT_t;

/*
* TCPC_PHY_ALERT_MASK register bits definition *
*/
typedef union TCPC_PHY_ALERT_MASK_union_t {
    UINT16 R;
    struct {
        UINT16 CC_STATUS_INT_MASK                        :  1;  /* CC Status Interrupt Mask. */
        UINT16 POWER_STATUS_INT_MASK                     :  1;  /* Power Status Interrupt Mask. */
        UINT16 RECEIVED_SOP_MESSAGE_STATUS_INT_MASK      :  1;  /* Received SOP* Message Status Interrupt Mask. */
        UINT16 RECEIVED_HARD_RESET_INT_MASK              :  1;  /* Received Hard Reset Interrupt Mask. */
        UINT16 TRANSMIT_SOP_MESSAGE_FAILED_INT_MASK      :  1;  /* Transmit SOP* Message Failed Interrupt Mask. */
        UINT16 TRANSMIT_SOP_MESSAGE_DISCARDED_INT_MASK   :  1;  /* Transmit SOP* Message Discarded Interrupt Mask. */
        UINT16 TRANSMIT_SOP_MESSAGE_SUCCESSFUL_INT_MASK  :  1;  /* Transmit SOP* Message Successfulw Interrupt Mask. */
        UINT16 VBUS_VOLTAGE_ALARM_HI_INT_MASK            :  1;  /* VBUS Voltage Alarm Hi Interrupt Mask. */
        UINT16 VBUS_VOLTAGE_ALARM_LO_INT_MASK            :  1;  /* VBUS Voltage Alarm Lo Interrupt Mask. */
        UINT16 FAULT_INT_MASK                            :  1;  /* Fault. */
        UINT16 RX_BUFFER_OVERFLOW_INT_MASK               :  1;  /* Rx Buffer Overflow Interrupt Mask. */
        UINT16 VBUS_SINK_DISCONNECT_DETECTED_INT_MASK    :  1;  /* VBUS Sink Disconnect Detected Interrupt Mask. */
        UINT16 BEGINNING_SOP_MESSAGE_STATUS_INT_MASK     :  1;  /* Beginning SOP* Message Status Interrupt Mask. */
        UINT16 EXTENDED_STATUS_INT_MASK                  :  1;  /* Extended Status Interrupt Mask. */
        UINT16 ALERT_EXTENDED_INT_MASK                   :  1;  /* Alert Extended Interrupt Mask. */
        UINT16 VENDOR_DEFINED_ALERT_INT_MASK             :  1;  /* Vendor Defined Alert Interrupt Mask. */
    } B;
} TCPC_PHY_ALERT_MASK_t;

/*
* TCPC_PHY_POWER_STATUS_MASK register bits definition *
*/
typedef union TCPC_PHY_POWER_STATUS_MASK_union_t {
    UINT8 R;
    struct {
        UINT8 SINKING_VBUS_STATUS_INT_MASK               :  1;  /* Sinking VBUS Status Interrupt Mask. */
        UINT8 VCONN_PRESENT_STATUS_INT_MASK              :  1;  /* VCONN Present Status Interrupt Mask. */
        UINT8 VBUS_PRESENT_STATUS_INT_MASK               :  1;  /* VBUS Present Status Interrupt Mask. */
        UINT8 VBUS_DETECTION_STATUS_INT_MASK             :  1;  /* VBUS Detection Status Interrupt Mask. */
        UINT8 SOURCING_VBUS_STATUS_INT_MASK              :  1;  /* Sourcing VBUS Status Status Interrupt Mask. */
        UINT8 SOURCING_HIGH_VOLTAGE_STATUS_INT_MASK      :  1;  /* Sourcing High Voltage Status Interrupt Mask. */
        UINT8 TCPC_INITIALIZATION_STATUS_INT_MASK        :  1;  /* TCPC Initialization Status Interrupt Mask. */
        UINT8 DEBUG_ACCESSORY_CONNECTED_STATUS_INT_MASK  :  1;  /* Debug Accessory Connected Status Interrupt Mask. */
    } B;
} TCPC_PHY_POWER_STATUS_MASK_t;

/*
* TCPC_PHY_FAULT_STATUS_MASK register bits definition *
*/
typedef union TCPC_PHY_FAULT_STATUS_MASK_union_t {
    UINT8 R;
    struct {
        UINT8 I2C_INTERFACE_ERROR_INT_MASK                                          :  1;  /* I2C Interface Error Interrupt Mask. */
        UINT8 VCONN_OVER_CURRENT_FAULT_INT_MASK                                     :  1;  /* Vconn Over Current Fault Interrupt Mask. */
        UINT8 INTERNAL_OR_EXTERNAL_OVP_VBUS_OVER_VOLTAGE_PROTECTION_FAULT_INT_MASK  :  1;  /* Internal or External OVP VBUS Over Voltage Protection Fault Interrupt Mask. */
        UINT8 INTERNAL_OR_EXTERNAL_OCP_VBUS_OVER_CURRENT_PROTECTION_FAULT_INT_MASK  :  1;  /* Internal or External OCP VBUS Over Current Protection Fault Interrupt Mask. */
        UINT8 FORCE_DISCHARGE_FAILED_INT_MASK                                       :  1;  /* Force Discharge Failed Interrupt Mask. */
        UINT8 AUTO_DISCHARGE_FAILED_INT_MASK                                        :  1;  /* Auto Discharge Failed Interrupt Mask. */
        UINT8 FORCE_OFF_VBUS_INT_MASK                                               :  1;  /* Force Off VBUS Interrupt Mask. */
        UINT8 ALLREGISTERSRESETTODEFAULT_INT_MASK                                   :  1;  /* AllRegistersResetToDefault Interrupt Mask. */
    } B;
} TCPC_PHY_FAULT_STATUS_MASK_t;

/*
* TCPC_PHY_EXTENDED_STATUS_MASK register bits definition *
*/
typedef union TCPC_PHY_EXTENDED_STATUS_MASK_union_t {
    UINT8 R;
    struct {
        UINT8 VSAFE0V_INT_MASK  :  1;  /* vSafe0V Interrupt Mask. */
        UINT8 Reserved_1        :  7;  /* Reserved */
    } B;
} TCPC_PHY_EXTENDED_STATUS_MASK_t;

/*
* TCPC_PHY_ALERT_EXTENDED_MASK register bits definition *
*/
typedef union TCPC_PHY_ALERT_EXTENDED_MASK_union_t {
    UINT8 R;
    struct {
        UINT8 SINK_FAST_ROLE_SWAP_INT_MASK    :  1;  /* Sink Fast Role Swap Interrupt Mask. */
        UINT8 SOURCE_FAST_ROLE_SWAP_INT_MASK  :  1;  /* Source Fast Role Swap Interrupt Mask. */
        UINT8 TIMER_EXPIRED_INT_MASK          :  1;  /* Timer Expired Interrupt Mask. */
        UINT8 Reserved_3                      :  5;  /* Reserved */
    } B;
} TCPC_PHY_ALERT_EXTENDED_MASK_t;

/*
* TCPC_PHY_CONFIG_STANDARD_OUTPUT register bits definition *
*/
typedef union TCPC_PHY_CONFIG_STANDARD_OUTPUT_union_t {
    UINT8 R;
    struct {
        UINT8 CONNECTOR_ORIENTATION      :  1;  /* Connector Orientation */
        UINT8 CONNECTION_PRESENT         :  1;  /* Connection Present */
        UINT8 MUX_CONTROL                :  2;  /* MUX Control */
        UINT8 ACTIVE_CABLE_CONNECTED     :  1;  /* Active Cable Connected */
        UINT8 AUDIO_ACCESSORY_CONNECTED  :  1;  /* Audio Accessory Connected# */
        UINT8 DEBUG_ACCESSORY_CONNECTED  :  1;  /* Debug Accessory Connected#. */
        UINT8 HIGH_IMPEDANCE_OUTPUTS     :  1;  /* High Impedance outputs. */
    } B;
} TCPC_PHY_CONFIG_STANDARD_OUTPUT_t;

/*
* TCPC_PHY_TCPC_CONTROL register bits definition *
*/
typedef union TCPC_PHY_TCPC_CONTROL_union_t {
    UINT8 R;
    struct {
        UINT8 PLUG_ORIENTATION                 :  1;  /* Plug Orientation */
        UINT8 BIST_TEST_MODE                   :  1;  /* BIST Test Mode */
        UINT8 I2C_CLOCK_STRETCHING_CONTROL     :  2;  /* Clock Stretching Control */
        UINT8 DEBUG_ACCESSORY_CONTROL          :  1;  /* Debug Accessory Control */
        UINT8 ENABLE_WATCHDOG_TIMER            :  1;  /* Enable Watchdog Timer */
        UINT8 ENABLE_LOOKING4CONNECTION_ALERT  :  1;  /* x */
        UINT8 ENABLE_SMBUS_PEC                 :  1;  /* Enable SMBus PEC */
    } B;
} TCPC_PHY_TCPC_CONTROL_t;

/*
* TCPC_PHY_ROLE_CONTROL register bits definition *
*/
typedef union TCPC_PHY_ROLE_CONTROL_union_t {
    UINT8 R;
    struct {
        UINT8 CC1         :  2;  /* CC1 */
        UINT8 CC2         :  2;  /* CC2 */
        UINT8 RP_VALUE    :  2;  /* Rp Value */
        UINT8 DRP         :  1;  /* DRP */
        UINT8 Reserved_7  :  1;  /* Reserved */
    } B;
} TCPC_PHY_ROLE_CONTROL_t;

/*
* TCPC_PHY_FAULT_CONTROL register bits definition *
*/
typedef union TCPC_PHY_FAULT_CONTROL_union_t {
    UINT8 R;
    struct {
        UINT8 VCONN_OVER_CURRENT_FAULT              :  1;  /* VCONN Over Current Fault */
        UINT8 INTERNAL_OR_EXTERNAL_VBUS_OVP_FAULT   :  1;  /* Internal or External OVP VBUS Over Volatge Protection Fault */
        UINT8 INTERNAL_OR_EXTERNAL_VBUS_OCP_FAULT   :  1;  /* Internal or External OCP VBUS Over Current Protection Fault */
        UINT8 VBUS_DISCHARGE_FAULT_DETECTION_TIMER  :  1;  /* VBUS Discharge Fault Detection Timer */
        UINT8 FORCE_OFF_VBUS                        :  1;  /* Force Off VBUS */
        UINT8 Reserved_5                            :  3;  /* Reserved */
    } B;
} TCPC_PHY_FAULT_CONTROL_t;

/*
* TCPC_PHY_POWER_CONTROL register bits definition *
*/
typedef union TCPC_PHY_POWER_CONTROL_union_t {
    UINT8 R;
    struct {
        UINT8 ENABLE_VCONN               :  1;  /* Enable VCONN */
        UINT8 VCONN_POWER_SUPPORTED      :  1;  /* VCONN Power Supported */
        UINT8 FORCE_DISCHARGE            :  1;  /* Force Discharge */
        UINT8 ENABLE_BLEED_DISCHARGE     :  1;  /* Enable Bleed Discharge */
        UINT8 AUTO_DISCHARGE_DISCONNECT  :  1;  /* Auto Discharge Disconnect */
        UINT8 DISABLE_VOLTAGE_ALARMS     :  1;  /* Disable Voltage Alarms */
        UINT8 VBUS_VOLTAGE_MONITOR       :  1;  /* VBUS_VOLTAGE Monitor */
        UINT8 FAST_ROLE_SWAP_ENABLE      :  1;  /* Fast Role Swap Enable */
    } B;
} TCPC_PHY_POWER_CONTROL_t;

/*
* TCPC_PHY_CC_STATUS register bits definition *
*/
typedef union TCPC_PHY_CC_STATUS_union_t {
    UINT8 R;
    struct {
        UINT8 CC1_STATE           :  2;  /* CC1 State */
        UINT8 CC2_STATE           :  2;  /* CC2 State */
        UINT8 CONNECTRESULT       :  1;  /* ConnectResult */
        UINT8 LOOKING4CONNECTION  :  1;  /* Looking4Connection */
        UINT8 Reserved_6          :  2;  /* Reserved */
    } B;
} TCPC_PHY_CC_STATUS_t;

/*
* TCPC_PHY_POWER_STATUS register bits definition *
*/
typedef union TCPC_PHY_POWER_STATUS_union_t {
    UINT8 R;
    struct {
        UINT8 SINKING_VBUS                :  1;  /* Sinking VBUS */
        UINT8 VCONN_PRESENT               :  1;  /* VCONN Present */
        UINT8 VBUS_PRESENT                :  1;  /* VBUS Present */
        UINT8 VBUS_DETECTION_ENABLED      :  1;  /* VBUS Detection Enabled */
        UINT8 SOURCING_VBUS               :  1;  /* Sourcing VBUS */
        UINT8 SOURCING_HIGH_VOLTAGE       :  1;  /* Sourcing High Voltage */
        UINT8 TCPC_INITIALIZATION_STATUS  :  1;  /* TCPC Initialization Status */
        UINT8 DEBUG_ACCESSORY_CONNECTED   :  1;  /* Debug Accessory Connected */
    } B;
} TCPC_PHY_POWER_STATUS_t;

/*
* TCPC_PHY_FAULT_STATUS register bits definition *
*/
typedef union TCPC_PHY_FAULT_STATUS_union_t {
    UINT8 R;
    struct {
        UINT8 I2C_INTERFACE_ERROR                  :  1;  /* I2C Interface Error */
        UINT8 VCONN_OVER_CURRENT_FAULT             :  1;  /* VCONN Over Current Fault */
        UINT8 INTERNAL_OR_EXTERNAL_VBUS_OVP_FAULT  :  1;  /* Internal or External OVP VBUS Over Voltage Protection Fault */
        UINT8 INTERNAL_OR_EXTERNAL_VBUS_OCP_FAULT  :  1;  /* Internal or External OCP VBUS Over Current Protection Fault */
        UINT8 FORCE_DISCHARGE_FAILED               :  1;  /* Force Discharge Failed */
        UINT8 AUTO_DISCHARGE_FAILED                :  1;  /* Auto Discharge Failed */
        UINT8 FORCE_OFF_VBUS                       :  1;  /* Force Off VBUS */
        UINT8 ALL_REGISTERS_RESET_TO_DEFAULT       :  1;  /* AllRegistersResetToDefault */
    } B;
} TCPC_PHY_FAULT_STATUS_t;

/*
* TCPC_PHY_EXTENDED_STATUS register bits definition *
*/
typedef union TCPC_PHY_EXTENDED_STATUS_union_t {
    UINT8 R;
    struct {
        UINT8 VSAFE0V     :  1;  /* vSafe0V */
        UINT8 Reserved_1  :  7;  /* Reserved */
    } B;
} TCPC_PHY_EXTENDED_STATUS_t;

/*
* TCPC_PHY_ALERT_EXTENDED register bits definition *
*/
typedef union TCPC_PHY_ALERT_EXTENDED_union_t {
    UINT8 R;
    struct {
        UINT8 SINK_FAST_ROLE_SWAP    :  1;  /* Sink Fast Role Swap */
        UINT8 SOURCE_FAST_ROLE_SWAP  :  1;  /* Source Fast Role Swap */
        UINT8 TIMER_EXPIRED          :  1;  /* Timer Expired */
        UINT8 Reserved_3             :  5;  /* Reserved */
    } B;
} TCPC_PHY_ALERT_EXTENDED_t;

/*
* TCPC_PHY_COMMAND register bits definition *
*/
typedef union TCPC_PHY_COMMAND_union_t {
    UINT8 R;
    struct {
        UINT8 CMD  :  8;  /* Command */
    } B;
} TCPC_PHY_COMMAND_t;

/*
* TCPC_PHY_DEVICE_CAPABILITIES_1 register bits definition *
*/
typedef union TCPC_PHY_DEVICE_CAPABILITIES_1_union_t {
    UINT16 R;
    struct {
        UINT16 SOURCE_VBUS                         :  1;  /* Source VBUS */
        UINT16 SOURCE_HIGH_VOLTAGE_VBUS            :  1;  /* Source High Voltage VBUS */
        UINT16 SINK_VBUS                           :  1;  /* Sink VBUS */
        UINT16 SOURCE_VCONN                        :  1;  /* Source VCONN */
        UINT16 SOP_DBG_SOP_DBG_SUPPORT             :  1;  /* SOP_DBG/SOP_DBG Support */
        UINT16 POWER_ROLES_SUPPORTED               :  3;  /* Power Roles Supported */
        UINT16 SOURCE_RESISTOR_SUPPORTED           :  2;  /* Source Resistor Supported */
        UINT16 VBUS_MEASUREMENT_AND_ALARM_CAPABLE  :  1;  /* VBUS Measurement and Alarm Capable */
        UINT16 FORCE_DISCHARGE                     :  1;  /* Force Discharge */
        UINT16 BLEED_DISCHARGE                     :  1;  /* Bleed Discharge */
        UINT16 VBUS_OVP_REPORTING                  :  1;  /* VBUS OVP Reporting */
        UINT16 VBUS_OCP_REPORTING                  :  1;  /* VBUS OCP Reporting */
        UINT16 VBUS_HIGH_VOLTAGE_TARGET            :  1;  /* VBUS High Voltage Target */
    } B;
} TCPC_PHY_DEVICE_CAPABILITIES_1_t;

/*
* TCPC_PHY_DEVICE_CAPABILITIES_2 register bits definition *
*/
typedef union TCPC_PHY_DEVICE_CAPABILITIES_2_union_t {
    UINT16 R;
    struct {
        UINT16 VCONN_OVERCURRENT_FAULT_CAPABLE  :  1;  /* VCONN Overcurrent Fault Capable */
        UINT16 VCONN_POWER_SUPPORTED            :  3;  /* VCONN Power Supported */
        UINT16 VBUS_VOLTAGE_ALARM_LSB           :  2;  /* VBUS Voltage Alarm LSB */
        UINT16 STOP_DISCHARGE_THRESHOLD         :  1;  /* Stop Discharge Threshold */
        UINT16 SINK_DISCONNECT_DETECTION        :  1;  /* Sink Disconnect Detection */
        UINT16 WATCHDOG_TIMER                   :  1;  /* Watchdog Timer */
        UINT16 SINK_FR_SWAP                     :  1;  /* Sink FR Swap */
        UINT16 SOURCE_FR_SWAP                   :  1;  /* Source FR Swap */
        UINT16 SMBUS_PEC                        :  1;  /* SMBus PEC */
        UINT16 LONG_MESSAGE                     :  1;  /* Long Message */
        UINT16 GENERIC_TIMER                    :  1;  /* Generic Timer */
        UINT16 Reserved_14                      :  2;  /* Reserved */
    } B;
} TCPC_PHY_DEVICE_CAPABILITIES_2_t;

/*
* TCPC_PHY_STANDARD_INPUT_CAPABILITIES register bits definition *
*/
typedef union TCPC_PHY_STANDARD_INPUT_CAPABILITIES_union_t {
    UINT8 R;
    struct {
        UINT8 FORCE_OFF_VBUS                    :  1;  /* Force Off VBUS */
        UINT8 VBUS_EXTERNAL_OVER_CURRENT_FAULT  :  1;  /* VBUS External Over Current Fault */
        UINT8 VBUS_EXTERNAL_OVER_VOLTAGE_FAULT  :  1;  /* VBUS External Over Voltage Fault */
        UINT8 SOURCE_FAST_ROLE_SWAP             :  2;  /* Source Fast Role Swap */
        UINT8 Reserved_5                        :  3;  /* Reserved */
    } B;
} TCPC_PHY_STANDARD_INPUT_CAPABILITIES_t;

/*
* TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES register bits definition *
*/
typedef union TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_union_t {
    UINT8 R;
    struct {
        UINT8 CONNECTOR_ORIENTATION                  :  1;  /* Connector Orientation */
        UINT8 CONNECTION_PRESENT                     :  1;  /* Connection Present */
        UINT8 MUX_CONFIGURATION_CONTROL              :  1;  /* MUX Configuration Control */
        UINT8 ACTIVE_CABLE_INDICATOR                 :  1;  /* Active Cable Indicator */
        UINT8 AUDIO_ADAPTER_ACCESSORY_INDICATOR      :  1;  /* Audio Adapter Accessory Indicator */
        UINT8 VBUS_PRESENT_MONITOR                   :  1;  /* VBUS Present Monitor */
        UINT8 DEBUG_ACCESSORY_INDICATOR              :  1;  /* Debug Accessory Indicator */
        UINT8 VBUS_SINK_DISCONNECT_DETECT_INDICATOR  :  1;  /* VBUS Sink Disconnect Detect Indicator */
    } B;
} TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_t;

/*
* TCPC_PHY_CONFIG_EXTENDED1 register bits definition *
*/
typedef union TCPC_PHY_CONFIG_EXTENDED1_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_5                     :  6;  /* Reserved */
        UINT8 STANDARD_INPUT_SOURCE_FR_SWAP  :  1;  /* Standard Input Source FR Swap */
        UINT8 FR_SWAP_BIDIRECTIONAL_PIN      :  1;  /* FR Swap Bidirectional Pin */
    } B;
} TCPC_PHY_CONFIG_EXTENDED1_t;

/*
* TCPC_PHY_GENERIC_TIMER register bits definition *
*/
typedef union TCPC_PHY_GENERIC_TIMER_union_t {
    UINT16  R;
} TCPC_PHY_GENERIC_TIMER_t;

/*
* TCPC_PHY_MESSAGE_HEADER_INFO register bits definition *
*/
typedef union TCPC_PHY_MESSAGE_HEADER_INFO_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_6  :  7;  /* Reserved */
        UINT8 XXX         :  1;  /* x */
    } B;
} TCPC_PHY_MESSAGE_HEADER_INFO_t;

/*
* TCPC_PHY_RECEIVE_DETECT register bits definition *
*/
typedef union TCPC_PHY_RECEIVE_DETECT_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_6  :  7;  /* Reserved */
        UINT8 XXX         :  1;  /* x */
    } B;
} TCPC_PHY_RECEIVE_DETECT_t;

/*
* TCPC_PHY_READABLE_BYTE_COUNT register bits definition *
*/
typedef union TCPC_PHY_READABLE_BYTE_COUNT_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_6  :  7;  /* Reserved */
        UINT8 XXX         :  1;  /* x */
    } B;
} TCPC_PHY_READABLE_BYTE_COUNT_t;

/*
* TCPC_PHY_RX_BUF_FRAME_TYPE register bits definition *
*/
typedef union TCPC_PHY_RX_BUF_FRAME_TYPE_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_6  :  7;  /* Reserved */
        UINT8 XXX         :  1;  /* x */
    } B;
} TCPC_PHY_RX_BUF_FRAME_TYPE_t;

/*
* TCPC_PHY_RX_BUF_BYTE_X register bits definition *
*/
typedef union TCPC_PHY_RX_BUF_BYTE_X_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_6  :  7;  /* Reserved */
        UINT8 XXX         :  1;  /* x */
    } B;
} TCPC_PHY_RX_BUF_BYTE_X_t;

/*
* TCPC_PHY_TRANSMIT register bits definition *
*/
typedef union TCPC_PHY_TRANSMIT_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_6  :  7;  /* Reserved */
        UINT8 XXX         :  1;  /* x */
    } B;
} TCPC_PHY_TRANSMIT_t;

/*
* TCPC_PHY_I2C_WRITE_BYTE_COUNT register bits definition *
*/
typedef union TCPC_PHY_I2C_WRITE_BYTE_COUNT_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_6  :  7;  /* Reserved */
        UINT8 XXX         :  1;  /* x */
    } B;
} TCPC_PHY_I2C_WRITE_BYTE_COUNT_t;

/*
* TCPC_PHY_TX_BUF_BYTE_X register bits definition *
*/
typedef union TCPC_PHY_TX_BUF_BYTE_X_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_6  :  7;  /* Reserved */
        UINT8 XXX         :  1;  /* x */
    } B;
} TCPC_PHY_TX_BUF_BYTE_X_t;

/*
* TCPC_PHY_VBUS_VOLTAGE register bits definition *
*/
typedef union TCPC_PHY_VBUS_VOLTAGE_union_t {
    UINT16 R;
    struct {
        UINT16 VBUS_VOLTAGE_MEASUREMENT  : 10;  /* VBUS voltage measurement */
        UINT16 SCALE_FACTOR              :  2;  /* Scale Factor */
        UINT16 Reserved_12               :  4;  /* Reserved */
    } B;
} TCPC_PHY_VBUS_VOLTAGE_t;

/*
* TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD register bits definition *
*/
typedef union TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD_union_t {
    UINT16 R;
    struct {
        UINT16 VOLTAGE_TRIP_POINT  : 10;  /* Voltage trip point */
        UINT16 Reserved_10         :  6;  /* Reserved */
    } B;
} TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD_t;

/*
* TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD register bits definition *
*/
typedef union TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD_union_t {
    UINT16 R;
    struct {
        UINT16 VOLTAGE_TRIP_POINT  : 10;  /* Voltage trip point */
        UINT16 Reserved_10         :  6;  /* Reserved */
    } B;
} TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD_t;

/*
* TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG register bits definition *
*/
typedef union TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG_union_t {
    UINT16 R;
    struct {
        UINT16 VOLTAGE_TRIP_POINT  : 10;  /* Voltage trip point */
        UINT16 Reserved_10         :  6;  /* Reserved */
    } B;
} TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG_t;

/*
* TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG register bits definition *
*/
typedef union TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG_union_t {
    UINT16 R;
    struct {
        UINT16 VOLTAGE_TRIP_POINT  : 10;  /* Voltage trip point */
        UINT16 Reserved_10         :  6;  /* Reserved */
    } B;
} TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG_t;

/*
* TCPC_PHY_VBUS_HV_TARGET register bits definition *
*/
typedef union TCPC_PHY_VBUS_HV_TARGET_union_t {
    UINT16  R;
} TCPC_PHY_VBUS_HV_TARGET_t;

/*
* TCPC_PHY_EXT_CFG_ID register bits definition *
*/
typedef union TCPC_PHY_EXT_CFG_ID_union_t {
    UINT16  R;
} TCPC_PHY_EXT_CFG_ID_t;

/*
* TCPC_PHY_EXT_ALERT register bits definition *
*/
typedef union TCPC_PHY_EXT_ALERT_union_t {
    UINT16  R;
} TCPC_PHY_EXT_ALERT_t;

/*
* TCPC_PHY_EXT_ALERT_MASK register bits definition *
*/
typedef union TCPC_PHY_EXT_ALERT_MASK_union_t {
    UINT16  R;
} TCPC_PHY_EXT_ALERT_MASK_t;

/*
* TCPC_PHY_EXT_CONFIG register bits definition *
*/
typedef union TCPC_PHY_EXT_CONFIG_union_t {
    UINT16  R;
} TCPC_PHY_EXT_CONFIG_t;

/*
* TCPC_PHY_EXT_FAULT_CONFIG register bits definition *
*/
typedef union TCPC_PHY_EXT_FAULT_CONFIG_union_t {
    UINT16  R;
} TCPC_PHY_EXT_FAULT_CONFIG_t;

/*
* TCPC_PHY_EXT_CONTROL register bits definition *
*/
typedef union TCPC_PHY_EXT_CONTROL_union_t {
    UINT8 R;
    struct {
        UINT8 VCONN_FORCE_DISCHARGE     :  1;  /* VCONN Force Discharge */
        UINT8 Reserved_1                :  3;  /* Reserved */
        UINT8 BLEED_DISCHARGE_STRENGTH  :  4;  /* FRS_EN */
    } B;
} TCPC_PHY_EXT_CONTROL_t;

/*
* TCPC_PHY_EXT_STATUS register bits definition *
*/
typedef union TCPC_PHY_EXT_STATUS_union_t {
    UINT16  R;
} TCPC_PHY_EXT_STATUS_t;

/*
* TCPC_PHY_EXT_GPIO_CONFIG register bits definition *
*/
typedef union TCPC_PHY_EXT_GPIO_CONFIG_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_2     :  3;  /* Reserved */
        UINT8 IILIM_5V_VBUS  :  1;  /* IILIM_5V_VBUS */
        UINT8 EN_SNK1        :  1;  /* EN_SNK1 */
        UINT8 EN_SRC         :  1;  /* EN_SRC */
        UINT8 FRS_EN         :  1;  /* FRS_EN */
        UINT8 Reserved_7     :  1;  /* Reserved */
    } B;
} TCPC_PHY_EXT_GPIO_CONFIG_t;

/*
* TCPC_PHY_EXT_GPIO_CONTROL register bits definition *
*/
typedef union TCPC_PHY_EXT_GPIO_CONTROL_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_2     :  3;  /* Reserved */
        UINT8 IILIM_5V_VBUS  :  1;  /* IILIM_5V_VBUS */
        UINT8 EN_SNK1        :  1;  /* EN_SNK1 */
        UINT8 EN_SRC         :  1;  /* EN_SRC */
        UINT8 FRS_EN         :  1;  /* FRS_EN */
        UINT8 Reserved_7     :  1;  /* Reserved */
    } B;
} TCPC_PHY_EXT_GPIO_CONTROL_t;

/*
* TCPC_PHY_EXT_GPIO_ALERT_CONFIG register bits definition *
*/
typedef union TCPC_PHY_EXT_GPIO_ALERT_CONFIG_union_t {
    UINT16 R;
    struct {
        UINT16 Reserved_2     :  3;  /* Reserved */
        UINT16 IILIM_5V_VBUS  :  3;  /* IILIM_5V_VBUS */
        UINT16 Reserved_6     : 10;  /* Reserved */
    } B;
} TCPC_PHY_EXT_GPIO_ALERT_CONFIG_t;

/*
* TCPC_PHY_EXT_GPIO_STATUS register bits definition *
*/
typedef union TCPC_PHY_EXT_GPIO_STATUS_union_t {
    UINT8 R;
    struct {
        UINT8 Reserved_2     :  3;  /* Reserved */
        UINT8 IILIM_5V_VBUS  :  1;  /* IILIM_5V_VBUS */
        UINT8 Reserved_4     :  2;  /* Reserved */
        UINT8 FRS_EN         :  1;  /* FRS_EN */
        UINT8 Reserved_7     :  1;  /* Reserved */
    } B;
} TCPC_PHY_EXT_GPIO_STATUS_t;

/*
* TCPC_PHY_SOURCE_HIGH_VOLTAGE_MB4B_TIME register bits definition *
*/
typedef union TCPC_PHY_SOURCE_HIGH_VOLTAGE_MB4B_TIME_union_t {
    UINT8  R;
} TCPC_PHY_SOURCE_HIGH_VOLTAGE_MB4B_TIME_t;

/*
* TCPC_PHY_ADC_FILTER_CONTROL_1 register bits definition *
*/
typedef union TCPC_PHY_ADC_FILTER_CONTROL_1_union_t {
    UINT8  R;
} TCPC_PHY_ADC_FILTER_CONTROL_1_t;

/*
* TCPC_PHY_ADC_FILTER_CONTROL_2 register bits definition *
*/
typedef union TCPC_PHY_ADC_FILTER_CONTROL_2_union_t {
    UINT8  R;
} TCPC_PHY_ADC_FILTER_CONTROL_2_t;

/*
* TCPC_PHY_VCON_CONFIG register bits definition *
*/
typedef union TCPC_PHY_VCON_CONFIG_union_t {
    UINT8  R;
} TCPC_PHY_VCON_CONFIG_t;

/*
* TCPC_PHY_VCON_FAULT_DEBOUNCE register bits definition *
*/
typedef union TCPC_PHY_VCON_FAULT_DEBOUNCE_union_t {
    UINT8  R;
} TCPC_PHY_VCON_FAULT_DEBOUNCE_t;

/*
* TCPC_PHY_VCON_FAULT_RECOVERY register bits definition *
*/
typedef union TCPC_PHY_VCON_FAULT_RECOVERY_union_t {
    UINT8  R;
} TCPC_PHY_VCON_FAULT_RECOVERY_t;

/*
* TCPC_PHY_VCON_FAULT_ATTEMPTS register bits definition *
*/
typedef union TCPC_PHY_VCON_FAULT_ATTEMPTS_union_t {
    UINT8  R;
} TCPC_PHY_VCON_FAULT_ATTEMPTS_t;

typedef struct TCPC_PHY_struct_t {
    TCPC_PHY_VENDOR_ID_t                         VENDOR_ID;                                /* 0x00000000 VENDOR_ID Register */
    TCPC_PHY_PRODUCT_ID_t                        PRODUCT_ID;                               /* 0x00000002 PRODUCT_ID Register */
    TCPC_PHY_DEVICE_ID_t                         DEVICE_ID;                                /* 0x00000004 DEVICE_ID Register */
    TCPC_PHY_USBTYPEC_REV_t                      USBTYPEC_REV;                             /* 0x00000006 USBTYPEC_REV Register */
    TCPC_PHY_USBPD_REV_VER_t                     USBPD_REV_VER;                            /* 0x00000008 USBPD_REV_VER Register */
    TCPC_PHY_PD_INTERFACE_REV_t                  PD_INTERFACE_REV;                         /* 0x0000000A PD_INTERFACE_REV Register */
    UINT8                                        Reserved_12[4];                           /* 0x0000000C Reserved */
    TCPC_PHY_ALERT_t                             ALERT;                                    /* 0x00000010 ALERT Register */
    TCPC_PHY_ALERT_MASK_t                        ALERT_MASK;                               /* 0x00000012 ALERT_MASK Register */
    TCPC_PHY_POWER_STATUS_MASK_t                 POWER_STATUS_MASK;                        /* 0x00000014 POWER_STATUS_MASK Register */
    TCPC_PHY_FAULT_STATUS_MASK_t                 FAULT_STATUS_MASK;                        /* 0x00000015 FAULT_STATUS_MASK Register */
    TCPC_PHY_EXTENDED_STATUS_MASK_t              EXTENDED_STATUS_MASK;                     /* 0x00000016 EXTENDED_STATUS_MASK Register */
    TCPC_PHY_ALERT_EXTENDED_MASK_t               ALERT_EXTENDED_MASK;                      /* 0x00000017 ALERT_EXTENDED_MASK Register */
    TCPC_PHY_CONFIG_STANDARD_OUTPUT_t            CONFIG_STANDARD_OUTPUT;                   /* 0x00000018 CONFIG_STANDARD_OUTPUT Register */
    TCPC_PHY_TCPC_CONTROL_t                      TCPC_CONTROL;                             /* 0x00000019 TCPC_CONTROL Register */
    TCPC_PHY_ROLE_CONTROL_t                      ROLE_CONTROL;                             /* 0x0000001A ROLE_CONTROL Register */
    TCPC_PHY_FAULT_CONTROL_t                     FAULT_CONTROL;                            /* 0x0000001B FAULT_CONTROL Register */
    TCPC_PHY_POWER_CONTROL_t                     POWER_CONTROL;                            /* 0x0000001C POWER_CONTROL Register */
    TCPC_PHY_CC_STATUS_t                         CC_STATUS;                                /* 0x0000001D CC_STATUS Register */
    TCPC_PHY_POWER_STATUS_t                      POWER_STATUS;                             /* 0x0000001E POWER_STATUS Register */
    TCPC_PHY_FAULT_STATUS_t                      FAULT_STATUS;                             /* 0x0000001F FAULT_STATUS Register */
    TCPC_PHY_EXTENDED_STATUS_t                   EXTENDED_STATUS;                          /* 0x00000020 EXTENDED_STATUS Register */
    TCPC_PHY_ALERT_EXTENDED_t                    ALERT_EXTENDED;                           /* 0x00000021 ALERT_EXTENDED Register */
    UINT8                                        Reserved_34[1];                           /* 0x00000022 Reserved */
    TCPC_PHY_COMMAND_t                           COMMAND;                                  /* 0x00000023 COMMAND Register */
    TCPC_PHY_DEVICE_CAPABILITIES_1_t             DEVICE_CAPABILITIES_1;                    /* 0x00000024 DEVICE_CAPABILITIES_1 Register */
    TCPC_PHY_DEVICE_CAPABILITIES_2_t             DEVICE_CAPABILITIES_2;                    /* 0x00000026 DEVICE_CAPABILITIES_2 Register */
    TCPC_PHY_STANDARD_INPUT_CAPABILITIES_t       STANDARD_INPUT_CAPABILITIES;              /* 0x00000028 STANDARD_INPUT_CAPABILITIES Register */
    TCPC_PHY_STANDARD_OUTPUT_CAPABILITIES_t      STANDARD_OUTPUT_CAPABILITIES;             /* 0x00000029 STANDARD_OUTPUT_CAPABILITIES Register */
    TCPC_PHY_CONFIG_EXTENDED1_t                  CONFIG_EXTENDED1;                         /* 0x0000002A CONFIG_EXTENDED1 Register */
    UINT8                                        Reserved_43[1];                           /* 0x0000002B Reserved */
    TCPC_PHY_GENERIC_TIMER_t                     GENERIC_TIMER;                            /* 0x0000002C GENERIC_TIMER Register */
    TCPC_PHY_MESSAGE_HEADER_INFO_t               MESSAGE_HEADER_INFO;                      /* 0x0000002E MESSAGE_HEADER_INFO Register */
    TCPC_PHY_RECEIVE_DETECT_t                    RECEIVE_DETECT;                           /* 0x0000002F RECEIVE_DETECT Register */
    union {
        TCPC_PHY_READABLE_BYTE_COUNT_t           READABLE_BYTE_COUNT;                      /* 0x00000030 READABLE_BYTE_COUNT Register */
        TCPC_PHY_RX_BUF_FRAME_TYPE_t             RX_BUF_FRAME_TYPE;                        /* 0x00000030 RX_BUF_FRAME_TYPE Register */
        TCPC_PHY_RX_BUF_BYTE_X_t                 RX_BUF_BYTE_X;                            /* 0x00000030 RX_BUF_BYTE_x Register */
    };
    UINT8                                        Reserved_49[31];                          /* 0x00000031 Reserved */
    TCPC_PHY_TRANSMIT_t                          TRANSMIT;                                 /* 0x00000050 TRANSMIT Register */
    union {
        TCPC_PHY_I2C_WRITE_BYTE_COUNT_t          I2C_WRITE_BYTE_COUNT;                     /* 0x00000051 I2C_WRITE_BYTE_COUNT Register */
        TCPC_PHY_TX_BUF_BYTE_X_t                 TX_BUF_BYTE_X;                            /* 0x00000051 TX_BUF_BYTE_x Register */
    };
    UINT8                                        Reserved_82[30];                          /* 0x00000052 Reserved */
    TCPC_PHY_VBUS_VOLTAGE_t                      VBUS_VOLTAGE;                             /* 0x00000070 VBUS_VOLTAGE Register */
    TCPC_PHY_VBUS_SINK_DISCONNECT_THRESHOLD_t    VBUS_SINK_DISCONNECT_THRESHOLD;           /* 0x00000072 VBUS_SINK_DISCONNECT_THRESHOLD Register */
    TCPC_PHY_VBUS_STOP_DISCHARGE_THRESHOLD_t     VBUS_STOP_DISCHARGE_THRESHOLD;            /* 0x00000074 VBUS_STOP_DISCHARGE_THRESHOLD Register */
    TCPC_PHY_VBUS_VOLTAGE_ALARM_HI_CFG_t         VBUS_VOLTAGE_ALARM_HI_CFG;                /* 0x00000076 VBUS_VOLTAGE_ALARM_HI_CFG Register */
    TCPC_PHY_VBUS_VOLTAGE_ALARM_LO_CFG_t         VBUS_VOLTAGE_ALARM_LO_CFG;                /* 0x00000078 VBUS_VOLTAGE_ALARM_LO_CFG Register */
    TCPC_PHY_VBUS_HV_TARGET_t                    VBUS_HV_TARGET;                           /* 0x0000007A VBUS_HV_TARGET Register */
    UINT8                                        Reserved_124[4];                          /* 0x0000007C Reserved */
    TCPC_PHY_EXT_CFG_ID_t                        EXT_CFG_ID;                               /* 0x00000080 For NXP internal use only */
    TCPC_PHY_EXT_ALERT_t                         EXT_ALERT;                                /* 0x00000082 EXT ALERT Register */
    TCPC_PHY_EXT_ALERT_MASK_t                    EXT_ALERT_MASK;                           /* 0x00000084 EXT ALERT MASK Register */
    TCPC_PHY_EXT_CONFIG_t                        EXT_CONFIG;                               /* 0x00000086 EXT CONFIG Register */
    TCPC_PHY_EXT_FAULT_CONFIG_t                  EXT_FAULT_CONFIG;                         /* 0x00000088 EXT FAULT CONFIG Register */
    UINT8                                        Reserved_138[4];                          /* 0x0000008A Reserved */
    TCPC_PHY_EXT_CONTROL_t                       EXT_CONTROL;                              /* 0x0000008E Bleed Discharge Strength (Only applied on write to POWER_CONTROL) */
    UINT8                                        Reserved_143[1];                          /* 0x0000008F Reserved */
    TCPC_PHY_EXT_STATUS_t                        EXT_STATUS;                               /* 0x00000090 EXT STATUS Register */
    TCPC_PHY_EXT_GPIO_CONFIG_t                   EXT_GPIO_CONFIG;                          /* 0x00000092 EXT_GPIO_CONFIG Register */
    TCPC_PHY_EXT_GPIO_CONTROL_t                  EXT_GPIO_CONTROL;                         /* 0x00000093 EXT_GPIO_CONFIG Register */
    TCPC_PHY_EXT_GPIO_ALERT_CONFIG_t             EXT_GPIO_ALERT_CONFIG;                    /* 0x00000094 EXT_GPIO_ALERT_CONFIG Register */
    TCPC_PHY_EXT_GPIO_STATUS_t                   EXT_GPIO_STATUS;                          /* 0x00000096 EXT_GPIO_CONFIG Register */
    TCPC_PHY_SOURCE_HIGH_VOLTAGE_MB4B_TIME_t     SOURCE_HIGH_VOLTAGE_MB4B_TIME;            /* 0x00000097 SOURCE_HIGH_VOLTAGE_MB4B_TIME Register */
    UINT8                                        Reserved_152[2];                          /* 0x00000098 Reserved */
    TCPC_PHY_ADC_FILTER_CONTROL_1_t              ADC_FILTER_CONTROL_1;                     /* 0x0000009A ADC_FILTER_CONTROL_1 Register */
    TCPC_PHY_ADC_FILTER_CONTROL_2_t              ADC_FILTER_CONTROL_2;                     /* 0x0000009B ADC_FILTER_CONTROL_2 Register */
    TCPC_PHY_VCON_CONFIG_t                       VCON_CONFIG;                              /* 0x0000009C VCON_CONFIG Register */
    TCPC_PHY_VCON_FAULT_DEBOUNCE_t               VCON_FAULT_DEBOUNCE;                      /* 0x0000009D VCON_FAULT_DEBOUNCE Register */
    TCPC_PHY_VCON_FAULT_RECOVERY_t               VCON_FAULT_RECOVERY;                      /* 0x0000009E VCON_FAULT_RECOVERY Register */
    TCPC_PHY_VCON_FAULT_ATTEMPTS_t               VCON_FAULT_ATTEMPTS;                      /* 0x0000009F VCON_FAULT_ATTEMPTS Register */
} TCPC_PHY_t;

#endif /* _TCPC_PHY_IO_MAP_H_ */
