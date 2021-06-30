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

#ifndef _MII_REG_IO_MAP_H_
#define _MII_REG_IO_MAP_H_

/* MII_REG registers, offset from base address */
#define MII_REG_C             0x0000  /* Control register offset */
#define MII_REG_S             0x0001  /* Status register offset */
#define MII_REG_PHYID1        0x0002  /* PHY Identifier offset */
#define MII_REG_PHYID2        0x0003  /* PHY Identifier offset */
#define MII_REG_ANA           0x0004  /* Auto-Negotiation advertisement register offset */
#define MII_REG_LPA           0x0005  /* Auto-Negotiation Link Partner ability register offset */
#define MII_REG_ANE           0x0006  /* Auto-Negotiation expansion register offset */
#define MII_REG_ANNPT         0x0007  /* Auto-Negotiation Next Page transmit register offset */
#define MII_REG_ANLPRNP       0x0008  /* Auto-Negotiation Link Partner Received Next Page register offset */
#define MII_REG_BASET_1000_C  0x0009  /* 1000BASE-T Control Registers offset */
#define MII_REG_BASET_1000_S  0x000A  /* 1000BASE-T Status Registers offset */
#define MII_REG_MMD_ACR       0x000D  /* MMD Access Control Register offset */
#define MII_REG_MMD_AADR      0x000E  /* Access Address Data Register offset */
#define MII_REG_ES            0x000F  /* Extended status register offset */

/*
 * MII_REG_C register bits *
 */
#define MII_REG_C_RESET_MASK                                   0x00008000
#define MII_REG_C_RESET_SHIFT                                  15
#define MII_REG_C_LOOPBACK_MASK                                0x00004000
#define MII_REG_C_LOOPBACK_SHIFT                               14
#define MII_REG_C_SPEED_SELECTION_LSB_MASK                     0x00002000
#define MII_REG_C_SPEED_SELECTION_LSB_SHIFT                    13
#define MII_REG_C_AUTO_NEGOTIATION_ENABLE_MASK                 0x00001000
#define MII_REG_C_AUTO_NEGOTIATION_ENABLE_SHIFT                12
#define MII_REG_C_POWER_DOWN_MASK                              0x00000800
#define MII_REG_C_POWER_DOWN_SHIFT                             11
#define MII_REG_C_ISOLATE_MASK                                 0x00000400
#define MII_REG_C_ISOLATE_SHIFT                                10
#define MII_REG_C_RESTART_AUTO_NEGOTIATION_MASK                0x00000200
#define MII_REG_C_RESTART_AUTO_NEGOTIATION_SHIFT               9
#define MII_REG_C_DUPLEX_MODE_MASK                             0x00000100
#define MII_REG_C_DUPLEX_MODE_SHIFT                            8
#define MII_REG_C_COLLISION_TEST_MASK                          0x00000080
#define MII_REG_C_COLLISION_TEST_SHIFT                         7
#define MII_REG_C_SPEED_SELECTION_MSB_MASK                     0x00000040
#define MII_REG_C_SPEED_SELECTION_MSB_SHIFT                    6
#define MII_REG_C_UNIDIRECTIONAL_ENABLE_MASK                   0x00000020
#define MII_REG_C_UNIDIRECTIONAL_ENABLE_SHIFT                  5

/*
 * MII_REG_S register bits *
 */
#define MII_REG_S_BASE_T4_100_MASK                             0x00008000
#define MII_REG_S_BASE_T4_100_SHIFT                            15
#define MII_REG_S_BASE_X_100_FD_MASK                           0x00004000
#define MII_REG_S_BASE_X_100_FD_SHIFT                          14
#define MII_REG_S_BASE_X_100_HD_MASK                           0x00002000
#define MII_REG_S_BASE_X_100_HD_SHIFT                          13
#define MII_REG_S_BASE_T_10_FD_MASK                            0x00001000
#define MII_REG_S_BASE_T_10_FD_SHIFT                           12
#define MII_REG_S_BASE_T_10_HD_MASK                            0x00000800
#define MII_REG_S_BASE_T_10_HD_SHIFT                           11
#define MII_REG_S_BASE_T2_100_FD_MASK                          0x00000400
#define MII_REG_S_BASE_T2_100_FD_SHIFT                         10
#define MII_REG_S_BASE_T2_100_HD_MASK                          0x00000200
#define MII_REG_S_BASE_T2_100_HD_SHIFT                         9
#define MII_REG_S_EXTENDED_STATUS_MASK                         0x00000100
#define MII_REG_S_EXTENDED_STATUS_SHIFT                        8
#define MII_REG_S_UNIDIRECTIONAL_ABILITY_MASK                  0x00000080
#define MII_REG_S_UNIDIRECTIONAL_ABILITY_SHIFT                 7
#define MII_REG_S_MF_PREAMBLE_SUPPRESSION_MASK                 0x00000040
#define MII_REG_S_MF_PREAMBLE_SUPPRESSION_SHIFT                6
#define MII_REG_S_AUTO_NEGOTIATION_COMPLETE_MASK               0x00000020
#define MII_REG_S_AUTO_NEGOTIATION_COMPLETE_SHIFT              5
#define MII_REG_S_REMOTE_FAULT_MASK                            0x00000010
#define MII_REG_S_REMOTE_FAULT_SHIFT                           4
#define MII_REG_S_AUTO_NEGOTIATION_ABILITY_MASK                0x00000008
#define MII_REG_S_AUTO_NEGOTIATION_ABILITY_SHIFT               3
#define MII_REG_S_LINK_STATUS_MASK                             0x00000004
#define MII_REG_S_LINK_STATUS_SHIFT                            2
#define MII_REG_S_JABBER_DETECT_MASK                           0x00000002
#define MII_REG_S_JABBER_DETECT_SHIFT                          1
#define MII_REG_S_EXTENDED_CAPABILITY_MASK                     0x00000001
#define MII_REG_S_EXTENDED_CAPABILITY_SHIFT                    0

/*
 * MII_REG_PHYID1 register bits *
 */
#define MII_REG_PHYID1_OUI_MSB_MASK                            0x0000FFFF
#define MII_REG_PHYID1_OUI_MSB_SHIFT                           0

/*
 * MII_REG_PHYID2 register bits *
 */
#define MII_REG_PHYID2_OUI_LSB_MASK                            0x0000FC00
#define MII_REG_PHYID2_OUI_LSB_SHIFT                           10
#define MII_REG_PHYID2_MODEL_NUMBER_MASK                       0x000003F0
#define MII_REG_PHYID2_MODEL_NUMBER_SHIFT                      4
#define MII_REG_PHYID2_REVISION_NUMBER_MASK                    0x0000000F
#define MII_REG_PHYID2_REVISION_NUMBER_SHIFT                   0

/*
 * MII_REG_ANA register bits *
 */
#define MII_REG_ANA_NEXT_PAGE_MASK                             0x00008000
#define MII_REG_ANA_NEXT_PAGE_SHIFT                            15
#define MII_REG_ANA_REMOTE_FAULT_MASK                          0x00002000
#define MII_REG_ANA_REMOTE_FAULT_SHIFT                         13
#define MII_REG_ANA_EXTENDED_NEXT_PAGE_MASK                    0x00001000
#define MII_REG_ANA_EXTENDED_NEXT_PAGE_SHIFT                   12
#define MII_REG_ANA_ASYMETRIC_PAUSE_MASK                       0x00000800
#define MII_REG_ANA_ASYMETRIC_PAUSE_SHIFT                      11
#define MII_REG_ANA_PAUSE_MASK                                 0x00000400
#define MII_REG_ANA_PAUSE_SHIFT                                10
#define MII_REG_ANA_BASE_T4_100_MASK                           0x00000200
#define MII_REG_ANA_BASE_T4_100_SHIFT                          9
#define MII_REG_ANA_BASE_TX_100_FD_MASK                        0x00000100
#define MII_REG_ANA_BASE_TX_100_FD_SHIFT                       8
#define MII_REG_ANA_BASE_TX_100_HD_MASK                        0x00000080
#define MII_REG_ANA_BASE_TX_100_HD_SHIFT                       7
#define MII_REG_ANA_BASE_T_10_FD_MASK                          0x00000040
#define MII_REG_ANA_BASE_T_10_FD_SHIFT                         6
#define MII_REG_ANA_BASE_T_10_HD_MASK                          0x00000020
#define MII_REG_ANA_BASE_T_10_HD_SHIFT                         5
#define MII_REG_ANA_SELECTOR_FIELD_MASK                        0x0000001F
#define MII_REG_ANA_SELECTOR_FIELD_SHIFT                       0

/*
 * MII_REG_LPA register bits *
 */
#define MII_REG_LPA_NEXT_PAGE_MASK                             0x00008000
#define MII_REG_LPA_NEXT_PAGE_SHIFT                            15
#define MII_REG_LPA_ACK_MASK                                   0x00004000
#define MII_REG_LPA_ACK_SHIFT                                  14
#define MII_REG_LPA_REMOTE_FAULT_MASK                          0x00002000
#define MII_REG_LPA_REMOTE_FAULT_SHIFT                         13
#define MII_REG_LPA_XNP_MASK                                   0x00001000
#define MII_REG_LPA_XNP_SHIFT                                  12
#define MII_REG_LPA_ASYMETRIC_PAUSE_MASK                       0x00000800
#define MII_REG_LPA_ASYMETRIC_PAUSE_SHIFT                      11
#define MII_REG_LPA_PAUSE_MASK                                 0x00000400
#define MII_REG_LPA_PAUSE_SHIFT                                10
#define MII_REG_LPA_BASE_T4_100_MASK                           0x00000200
#define MII_REG_LPA_BASE_T4_100_SHIFT                          9
#define MII_REG_LPA_BASE_TX_100_FD_MASK                        0x00000100
#define MII_REG_LPA_BASE_TX_100_FD_SHIFT                       8
#define MII_REG_LPA_BASE_TX_100_HD_MASK                        0x00000080
#define MII_REG_LPA_BASE_TX_100_HD_SHIFT                       7
#define MII_REG_LPA_BASE_T_10_FD_MASK                          0x00000040
#define MII_REG_LPA_BASE_T_10_FD_SHIFT                         6
#define MII_REG_LPA_BASE_T_10_HD_MASK                          0x00000020
#define MII_REG_LPA_BASE_T_10_HD_SHIFT                         5
#define MII_REG_LPA_SELECTOR_FIELD_MASK                        0x0000001F
#define MII_REG_LPA_SELECTOR_FIELD_SHIFT                       0

/*
 * MII_REG_ANE register bits *
 */
#define MII_REG_ANE_RECEIVE_NEXT_PAGE_LOCATION_ABLE_MASK       0x00000040
#define MII_REG_ANE_RECEIVE_NEXT_PAGE_LOCATION_ABLE_SHIFT      6
#define MII_REG_ANE_RECEIVED_NEXT_PAGE_STORAGE_LOCATION_MASK   0x00000020
#define MII_REG_ANE_RECEIVED_NEXT_PAGE_STORAGE_LOCATION_SHIFT  5
#define MII_REG_ANE_PARALLEL_DETECTION_FAULT_MASK              0x00000010
#define MII_REG_ANE_PARALLEL_DETECTION_FAULT_SHIFT             4
#define MII_REG_ANE_LINK_PARTNER_NEXT_PAGE_ABLE_MASK           0x00000008
#define MII_REG_ANE_LINK_PARTNER_NEXT_PAGE_ABLE_SHIFT          3
#define MII_REG_ANE_NEXT_PAGE_ABLE_MASK                        0x00000004
#define MII_REG_ANE_NEXT_PAGE_ABLE_SHIFT                       2
#define MII_REG_ANE_PAGE_RECEIVED_MASK                         0x00000002
#define MII_REG_ANE_PAGE_RECEIVED_SHIFT                        1
#define MII_REG_ANE_LINK_PARTNER_AUTO_NEGOTIATION_ABLE_MASK    0x00000001
#define MII_REG_ANE_LINK_PARTNER_AUTO_NEGOTIATION_ABLE_SHIFT   0

/*
 * MII_REG_ANNPT register bits *
 */
#define MII_REG_ANNPT_NP_MASK                                  0x00008000
#define MII_REG_ANNPT_NP_SHIFT                                 15
#define MII_REG_ANNPT_MP_MASK                                  0x00002000
#define MII_REG_ANNPT_MP_SHIFT                                 13
#define MII_REG_ANNPT_ACK2_MASK                                0x00001000
#define MII_REG_ANNPT_ACK2_SHIFT                               12
#define MII_REG_ANNPT_TG_MASK                                  0x00000800
#define MII_REG_ANNPT_TG_SHIFT                                 11
#define MII_REG_ANNPT_CODE_MASK                                0x000007FF
#define MII_REG_ANNPT_CODE_SHIFT                               0

/*
 * MII_REG_ANLPRNP register bits *
 */
#define MII_REG_ANLPRNP_NP_MASK                                0x00008000
#define MII_REG_ANLPRNP_NP_SHIFT                               15
#define MII_REG_ANLPRNP_ACK_MASK                               0x00004000
#define MII_REG_ANLPRNP_ACK_SHIFT                              14
#define MII_REG_ANLPRNP_MP_MASK                                0x00002000
#define MII_REG_ANLPRNP_MP_SHIFT                               13
#define MII_REG_ANLPRNP_ACK2_MASK                              0x00001000
#define MII_REG_ANLPRNP_ACK2_SHIFT                             12
#define MII_REG_ANLPRNP_TG_MASK                                0x00000800
#define MII_REG_ANLPRNP_TG_SHIFT                               11
#define MII_REG_ANLPRNP_CODE_MASK                              0x000007FF
#define MII_REG_ANLPRNP_CODE_SHIFT                             0

/*
 * MII_REG_BASET_1000_C register bits *
 */
#define MII_REG_BASET_1000_C_TEST_MODE_MASK                    0x0000E000
#define MII_REG_BASET_1000_C_TEST_MODE_SHIFT                   13
#define MII_REG_BASET_1000_C_MS_MANUAL_CONFIG_MASK             0x00001000
#define MII_REG_BASET_1000_C_MS_MANUAL_CONFIG_SHIFT            12
#define MII_REG_BASET_1000_C_MS_CONFIG_VALUE_MASK              0x00000800
#define MII_REG_BASET_1000_C_MS_CONFIG_VALUE_SHIFT             11
#define MII_REG_BASET_1000_C_PORT_TYPE_MASK                    0x00000400
#define MII_REG_BASET_1000_C_PORT_TYPE_SHIFT                   10
#define MII_REG_BASET_1000_C_BASE_T_1000_FD_MASK               0x00000200
#define MII_REG_BASET_1000_C_BASE_T_1000_FD_SHIFT              9
#define MII_REG_BASET_1000_C_BASE_T_1000_HD_MASK               0x00000100
#define MII_REG_BASET_1000_C_BASE_T_1000_HD_SHIFT              8

/*
 * MII_REG_BASET_1000_S register bits *
 */
#define MII_REG_BASET_1000_S_MS_CONFIG_FAULT_MASK              0x00008000
#define MII_REG_BASET_1000_S_MS_CONFIG_FAULT_SHIFT             15
#define MII_REG_BASET_1000_S_MS_CONFIG_RESOLUTION_MASK         0x00004000
#define MII_REG_BASET_1000_S_MS_CONFIG_RESOLUTION_SHIFT        14
#define MII_REG_BASET_1000_S_LOCAL_RECEIVER_STATUS_MASK        0x00002000
#define MII_REG_BASET_1000_S_LOCAL_RECEIVER_STATUS_SHIFT       13
#define MII_REG_BASET_1000_S_REMOTE_RECEIVER_STATUS_MASK       0x00001000
#define MII_REG_BASET_1000_S_REMOTE_RECEIVER_STATUS_SHIFT      12
#define MII_REG_BASET_1000_S_BASE_T_1000_FD_MASK               0x00000800
#define MII_REG_BASET_1000_S_BASE_T_1000_FD_SHIFT              11
#define MII_REG_BASET_1000_S_BASE_T_1000_HD_MASK               0x00000400
#define MII_REG_BASET_1000_S_BASE_T_1000_HD_SHIFT              10
#define MII_REG_BASET_1000_S_IDLE_ERROR_COUNT_MASK             0x000000FF
#define MII_REG_BASET_1000_S_IDLE_ERROR_COUNT_SHIFT            0

/*
 * MII_REG_ES register bits *
 */
#define MII_REG_ES_BASE_X_1000_FD_MASK                         0x00008000
#define MII_REG_ES_BASE_X_1000_FD_SHIFT                        15
#define MII_REG_ES_BASE_X_1000_HD_MASK                         0x00004000
#define MII_REG_ES_BASE_X_1000_HD_SHIFT                        14
#define MII_REG_ES_BASE_T_1000_FD_MASK                         0x00002000
#define MII_REG_ES_BASE_T_1000_FD_SHIFT                        13
#define MII_REG_ES_BASE_T_1000_HD_MASK                         0x00001000
#define MII_REG_ES_BASE_T_1000_HD_SHIFT                        12

/*
 * MII_REG_C register bits definition *
 */
typedef union MII_REG_C_union_t {
    UINT16 R;
    struct {
        UINT16 Reserved_4                :  5;  /* Reserved */
        UINT16 UNIDIRECTIONAL_ENABLE     :  1;  /* Enables TX regardless of whether the PHY has determined that a valid link has been established. */
        UINT16 SPEED_SELECTION_MSB       :  1;  /* Manual speed selection - MSB bit. */
        UINT16 COLLISION_TEST            :  1;  /* Enables the COL signal test. */
        UINT16 DUPLEX_MODE               :  1;  /* Manual duplex selection. */
        UINT16 RESTART_AUTO_NEGOTIATION  :  1;  /* Forces the PHY to restart the Auto-Negotiation process. */
        UINT16 ISOLATE                   :  1;  /* Forces the PHY to electrically isolate its data paths from the MII or GMII. */
        UINT16 POWER_DOWN                :  1;  /* Places the PHY in a low-power consumption state. */
        UINT16 AUTO_NEGOTIATION_ENABLE   :  1;  /* Enables Auto-Negotiation process. */
        UINT16 SPEED_SELECTION_LSB       :  1;  /* Manual speed selection - LSB bit. */
        UINT16 LOOPBACK                  :  1;  /* Enables Loopback. */
        UINT16 RESET                     :  1;  /* Set the Status and Control registers to their default states. */
    } B;
} MII_REG_C_t;

/*
 * MII_REG_S register bits definition *
 */
typedef union MII_REG_S_union_t {
    UINT16 R;
    struct {
        UINT16 EXTENDED_CAPABILITY        :  1;  /* Extended capability. */
        UINT16 JABBER_DETECT              :  1;  /* Jabber detect. */
        UINT16 LINK_STATUS                :  1;  /* Link status. */
        UINT16 AUTO_NEGOTIATION_ABILITY   :  1;  /* Auto-Negotiation ability. */
        UINT16 REMOTE_FAULT               :  1;  /* Remote Fault. */
        UINT16 AUTO_NEGOTIATION_COMPLETE  :  1;  /* Auto-Negotiation complete. */
        UINT16 MF_PREAMBLE_SUPPRESSION    :  1;  /* MF preamble suppression ability. */
        UINT16 UNIDIRECTIONAL_ABILITY     :  1;  /* Unidirectional ability. */
        UINT16 EXTENDED_STATUS            :  1;  /* Extended status. */
        UINT16 BASE_T2_100_HD             :  1;  /* 100BASE-T2 half duplex ability. */
        UINT16 BASE_T2_100_FD             :  1;  /* 100BASE-T2 full duplex ability. */
        UINT16 BASE_T_10_HD               :  1;  /* 10 Mb/s half duplex ability. */
        UINT16 BASE_T_10_FD               :  1;  /* 10 Mb/s full duplex ability. */
        UINT16 BASE_X_100_HD              :  1;  /* 100BASE-X Half duplex ability. */
        UINT16 BASE_X_100_FD              :  1;  /* 100BASE-X full duplex ability. */
        UINT16 BASE_T4_100                :  1;  /* 100BASE-T4 ability. */
    } B;
} MII_REG_S_t;

/*
 * MII_REG_PHYID1 register bits definition *
 */
typedef union MII_REG_PHYID1_union_t {
    UINT16 R;
    struct {
        UINT16 OUI_MSB  : 16;  /* Organizationally Unique Identifier, bits [18:3]. */
    } B;
} MII_REG_PHYID1_t;

/*
 * MII_REG_PHYID2 register bits definition *
 */
typedef union MII_REG_PHYID2_union_t {
    UINT16 R;
    struct {
        UINT16 REVISION_NUMBER  :  4;  /* Revision number. */
        UINT16 MODEL_NUMBER     :  6;  /* Manufacturers model number. */
        UINT16 OUI_LSB          :  6;  /* Organizationally Unique Identifier, bits [19:24]. */
    } B;
} MII_REG_PHYID2_t;

/*
 * MII_REG_ANA register bits definition *
 */
typedef union MII_REG_ANA_union_t {
    UINT16 R;
    struct {
        UINT16 SELECTOR_FIELD      :  5;  /* Selector field. */
        UINT16 BASE_T_10_HD        :  1;  /* Advertise 10BASE-T half duplex capability. */
        UINT16 BASE_T_10_FD        :  1;  /* Advertise 10BASE-T full duplex capability. */
        UINT16 BASE_TX_100_HD      :  1;  /* Advertise 100BASE-TX half duplex capability. */
        UINT16 BASE_TX_100_FD      :  1;  /* Advertise 100BASE-TX full duplex capability. */
        UINT16 BASE_T4_100         :  1;  /* Advertise 100BASE-T4 capability. */
        UINT16 PAUSE               :  1;  /* Advertise pause capability. */
        UINT16 ASYMETRIC_PAUSE     :  1;  /* Advertise asymmetric pause capability. */
        UINT16 EXTENDED_NEXT_PAGE  :  1;  /* Advertise extended next page. */
        UINT16 REMOTE_FAULT        :  1;  /* Report remote fault. */
        UINT16 Reserved_14         :  1;  /* Reserved */
        UINT16 NEXT_PAGE           :  1;  /* Advertise next page. */
    } B;
} MII_REG_ANA_t;

/*
 * MII_REG_LPA register bits definition *
 */
typedef union MII_REG_LPA_union_t {
    UINT16 R;
    struct {
        UINT16 SELECTOR_FIELD   :  5;  /* Link partner Selector field. */
        UINT16 BASE_T_10_HD     :  1;  /* Link partner 10BASE-T half duplex capability. */
        UINT16 BASE_T_10_FD     :  1;  /* Link partner 10BASE-T full duplex capability. */
        UINT16 BASE_TX_100_HD   :  1;  /* Link partner 100BASE-TX half duplex capability. */
        UINT16 BASE_TX_100_FD   :  1;  /* Link partner 100BASE-TX full duplex capability. */
        UINT16 BASE_T4_100      :  1;  /* Link partner  100BASE-T4 capability. */
        UINT16 PAUSE            :  1;  /* Link partner pause capability. */
        UINT16 ASYMETRIC_PAUSE  :  1;  /* Link partner asymmetric pause capability. */
        UINT16 Reserved_12      :  1;  /* Reserved */
        UINT16 REMOTE_FAULT     :  1;  /* Link partner fault. */
        UINT16 ACK              :  1;  /* Link partner link code word acknowledgement */
        UINT16 NEXT_PAGE        :  1;  /* Link partner next page ability */
    } B;
} MII_REG_LPA_t;

/*
 * MII_REG_ANE register bits definition *
 */
typedef union MII_REG_ANE_union_t {
    UINT16 R;
    struct {
        UINT16 LINK_PARTNER_AUTO_NEGOTIATION_ABLE   :  1;  /* Link Partner Auto-Negotiation ability. */
        UINT16 PAGE_RECEIVED                        :  1;  /* Page Received. */
        UINT16 NEXT_PAGE_ABLE                       :  1;  /* Local device Next Page ability. */
        UINT16 LINK_PARTNER_NEXT_PAGE_ABLE          :  1;  /* Link Partner Next Page ability. */
        UINT16 PARALLEL_DETECTION_FAULT             :  1;  /* Parallel Detection Fault. */
        UINT16 RECEIVED_NEXT_PAGE_STORAGE_LOCATION  :  1;  /* Receive Next Page Storage Location. */
        UINT16 RECEIVE_NEXT_PAGE_LOCATION_ABLE      :  1;  /* Receive Next Page Location ability. */
        UINT16 Reserved_7                           :  9;  /* Reserved */
    } B;
} MII_REG_ANE_t;

/*
 * MII_REG_ANNPT register bits definition *
 */
typedef union MII_REG_ANNPT_union_t {
    UINT16 R;
    struct {
        UINT16 CODE         : 11;  /* Message/Unformatted Code field */
        UINT16 TG           :  1;  /* Toggle */
        UINT16 ACK2         :  1;  /* Acknowledge 2 */
        UINT16 MP           :  1;  /* Message Page */
        UINT16 Reserved_14  :  1;  /* Reserved */
        UINT16 NP           :  1;  /* Next Page */
    } B;
} MII_REG_ANNPT_t;

/*
 * MII_REG_ANLPRNP register bits definition *
 */
typedef union MII_REG_ANLPRNP_union_t {
    UINT16 R;
    struct {
        UINT16 CODE  : 11;  /* Message/Unformatted Code field */
        UINT16 TG    :  1;  /* Toggle */
        UINT16 ACK2  :  1;  /* Acknowledge 2 */
        UINT16 MP    :  1;  /* Message Page */
        UINT16 ACK   :  1;  /* Acknowledge */
        UINT16 NP    :  1;  /* Next Page */
    } B;
} MII_REG_ANLPRNP_t;

/*
 * MII_REG_BASET_1000_C register bits definition *
 */
typedef union MII_REG_BASET_1000_C_union_t {
    UINT16 R;
    struct {
        UINT16 Reserved_7        :  8;  /* Reserved */
        UINT16 BASE_T_1000_HD    :  1;  /* Advertise 1000BASE-T half duplex capability. */
        UINT16 BASE_T_1000_FD    :  1;  /* Advertise 1000BASE-T full duplex capability. */
        UINT16 PORT_TYPE         :  1;  /* Preference device if auto configuration is enabled MASTER (multiport device) or as SLAVE (single-port device). */
        UINT16 MS_CONFIG_VALUE   :  1;  /* MASTER-SLAVE Configuration Value */
        UINT16 MS_MANUAL_CONFIG  :  1;  /* MASTER-SLAVE Manual Configuration Enable */
        UINT16 TEST_MODE         :  3;  /* Transmitter test mode */
    } B;
} MII_REG_BASET_1000_C_t;

/*
 * MII_REG_BASET_1000_S register bits definition *
 */
typedef union MII_REG_BASET_1000_S_union_t {
    UINT16 R;
    struct {
        UINT16 IDLE_ERROR_COUNT        :  8;  /* Idle Error Count. */
        UINT16 Reserved_8              :  2;  /* Reserved */
        UINT16 BASE_T_1000_HD          :  1;  /* Link partner 1000BASE-T half duplex capability. */
        UINT16 BASE_T_1000_FD          :  1;  /* Link partner 1000BASE-T full duplex capability. */
        UINT16 REMOTE_RECEIVER_STATUS  :  1;  /* Remote Receiver Status */
        UINT16 LOCAL_RECEIVER_STATUS   :  1;  /* Local Receiver Status */
        UINT16 MS_CONFIG_RESOLUTION    :  1;  /* MASTER-SLAVE Configuration Resolution */
        UINT16 MS_CONFIG_FAULT         :  1;  /* MASTER-SLAVE configuration fault */
    } B;
} MII_REG_BASET_1000_S_t;

/*
 * MII_REG_MMD_ACR register bits definition *
 */
typedef union MII_REG_MMD_ACR_union_t {
    UINT16  R;
} MII_REG_MMD_ACR_t;

/*
 * MII_REG_MMD_AADR register bits definition *
 */
typedef union MII_REG_MMD_AADR_union_t {
    UINT16  R;
} MII_REG_MMD_AADR_t;

/*
 * MII_REG_ES register bits definition *
 */
typedef union MII_REG_ES_union_t {
    UINT16 R;
    struct {
        UINT16 Reserved_11     : 12;  /* Reserved */
        UINT16 BASE_T_1000_HD  :  1;  /* 1000BASE-T half duplex ability. */
        UINT16 BASE_T_1000_FD  :  1;  /* 1000BASE-T full duplex ability. */
        UINT16 BASE_X_1000_HD  :  1;  /* 1000BASE-X half duplex ability. */
        UINT16 BASE_X_1000_FD  :  1;  /* 1000BASE-X full duplex ability. */
    } B;
} MII_REG_ES_t;

typedef struct MII_REG_struct_t {
    MII_REG_C_t               C;                     /* 0x00000000 Control register */
    MII_REG_S_t               S;                     /* 0x00000001 Status register */
    MII_REG_PHYID1_t          PHYID1;                /* 0x00000002 PHY Identifier */
    MII_REG_PHYID2_t          PHYID2;                /* 0x00000003 PHY Identifier */
    MII_REG_ANA_t             ANA;                   /* 0x00000004 Auto-Negotiation advertisement register */
    MII_REG_LPA_t             LPA;                   /* 0x00000005 Auto-Negotiation Link Partner ability register */
    MII_REG_ANE_t             ANE;                   /* 0x00000006 Auto-Negotiation expansion register */
    MII_REG_ANNPT_t           ANNPT;                 /* 0x00000007 Auto-Negotiation Next Page transmit register */
    MII_REG_ANLPRNP_t         ANLPRNP;               /* 0x00000008 Auto-Negotiation Link Partner Received Next Page register */
    MII_REG_BASET_1000_C_t    BASET_1000_C;          /* 0x00000009 1000BASE-T Control Registers */
    MII_REG_BASET_1000_S_t    BASET_1000_S;          /* 0x0000000A 1000BASE-T Status Registers */
    UINT16                    Reserved_11[2];        /* 0x0000000B Reserved */
    MII_REG_MMD_ACR_t         MMD_ACR;               /* 0x0000000D MMD Access Control Register */
    MII_REG_MMD_AADR_t        MMD_AADR;              /* 0x0000000E Access Address Data Register */
    MII_REG_ES_t              ES;                    /* 0x0000000F Extended status register */
} MII_REG_t;

#endif /* _MII_REG_IO_MAP_H_ */
