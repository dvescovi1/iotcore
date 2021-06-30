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

#ifndef _MP_ENET_PHY_H
#define _MP_ENET_PHY_H

#define IMX_MII_LINK_PROPERTIES_BASE_T_10_HD_MASK     0x00000001
#define IMX_MII_LINK_PROPERTIES_BASE_T_10_FD_MASK     0x00000002
#define IMX_MII_LINK_PROPERTIES_BASE_TX_100_HD_MASK   0x00000004
#define IMX_MII_LINK_PROPERTIES_BASE_TX_100_FD_MASK   0x00000008
#define IMX_MII_LINK_PROPERTIES_BASE_T_1000_HD_MASK   0x00000010
#define IMX_MII_LINK_PROPERTIES_BASE_T_1000_FD_MASK   0x00000020
#define IMX_MII_LINK_PROPERTIES_BASE_ALL_MASK         0x0000003F
#define IMX_MII_LINK_PROPERTIES_ASYMETRIC_PAUSE       0x20000000
#define IMX_MII_LINK_PROPERTIES_PAUSE                 0x40000000
#define IMX_MII_LINK_PROPERTIES_AUTO_NEGOTIATION      0x80000000

typedef union IMX_MII_LINK_PROPERTIES_union_t {
    UINT32 R;
    struct {
        UINT32 BASE_T_10_HD         :  1;  /* Bit  0 - 10BASE-T half duplex. */
        UINT32 BASE_T_10_FD         :  1;  /* Bit  1 - 10BASE-T full duplex. */
        UINT32 BASE_TX_100_HD       :  1;  /* Bit  2 - 100BASE-TX half duplex. */
        UINT32 BASE_TX_100_FD       :  1;  /* Bit  3 - 100BASE-TX full duplex. */
        UINT32 BASE_T_1000_HD       :  1;  /* Bit  4 - 1000BASE-T half duplex. */
        UINT32 BASE_T_1000_FD       :  1;  /* Bit  5 - 1000BASE-T full duplex. */
        UINT32 Reserved             : 23;  
        UINT32 ASYMETRIC_PAUSE      :  1;  /* Bit 29 - Asymentric pause. */
        UINT32 PAUSE                :  1;  /* Bit 30 - Pause. */
        UINT32 AUTO_NEGOTIATION     :  1;  /* Bit 31 - Auto negoatiation. */
    } B;
} IMX_MII_LINK_PROPERTIES_t;

typedef enum {
    MP_PHY_STATE_NOT_INITIALIZED = 0,
    MP_PHY_STATE_INITIALIZED,
    MP_PHY_STATE_DEFAULT_CFG_RAED,
    MP_PHY_STATE_LINK_UP,
    MP_PHY_STATE_LINK_DOWN_TO_UP,
    MP_PHY_STATE_LINK_DOWN
} MP_PHY_DEVICE_STATE;

typedef struct IMX_MII_LINK_STATE_union_t {
    UINT32 R;
    struct {
        UINT32 Speed_MHz     : 16;  /* Bits   0 - 15. */
        UINT32 Dulex_Mode    :  2;  /* Bits  16 - 17. */
        UINT32 Reserved      :  8;  /* Bits  18 - 23. */
        UINT32 Connect_State :  2;  /* Bits  24 - 25. */
    } B;
} IMX_MII_LINK_STATE_t;

#define IMX_MII_LINK_STATE_SPEED_MHZ_MASK         0x0000FFFF
#define IMX_MII_LINK_STATE_SPEED_MHZ_SHIFT        0
#define IMX_MII_LINK_STATE_SPEED_MHZ_SET(x)       ((UINT32)(((UINT32)(x) << IMX_MII_LINK_STATE_SPEED_MHZ_SHIFT) & IMX_MII_LINK_STATE_SPEED_MHZ_MASK))
#define IMX_MII_LINK_STATE_SPEED_MHZ_GET(x)       ((UINT32)(((UINT32)(x) & IMX_MII_LINK_STATE_SPEED_MHZ_MASK) >> IMX_MII_LINK_STATE_SPEED_MHZ_SHIFT))
#define IMX_MII_LINK_STATE_DULPEX_MODE_MASK       0x00030000
#define IMX_MII_LINK_STATE_DUPLEX_MODE_SHIFT      16
#define IMX_MII_LINK_STATE_DUPLEX_MODE_SET(x)     ((UINT32)(((UINT32)(x) << IMX_MII_LINK_STATE_DUPLEX_MODE_SHIFT) & IMX_MII_LINK_STATE_DULPEX_MODE_MASK))
#define IMX_MII_LINK_STATE_DUPLEX_MODE_GET(x)     ((UINT32)(((UINT32)(x) & IMX_MII_LINK_STATE_DULPEX_MODE_MASK) >> IMX_MII_LINK_STATE_DUPLEX_MODE_SHIFT))

#define IMX_MII_LINK_STATE_CONNECT_STATE_MASK     0x03000000
#define IMX_MII_LINK_STATE_CONNECT_STATE_SHIFT    24
#define IMX_MII_LINK_STATE_CONNECT_STATE_SET(x)   ((UINT32)(((UINT32)(x) << IMX_MII_LINK_STATE_CONNECT_STATE_SHIFT) & IMX_MII_LINK_STATE_CONNECT_STATE_MASK))
#define IMX_MII_LINK_STATE_CONNECT_STATE_GET(x)   ((UINT32)(((UINT32)(x) & IMX_MII_LINK_STATE_CONNECT_STATE_MASK) >> IMX_MII_LINK_STATE_CONNECT_STATE_SHIFT))


// The Enet Phy device driver structure
typedef struct _MP_PHY_DEVICE {
    struct _MP_ADAPTER*           PHYDev_pEnetAdapter;                            // ENET device data address.
    MP_MDIO_DEVICE *              PHYDev_pMDIODev;                                // MDIO device data address.
    MP_PHY_DEVICE_STATE           PHYDev_CurrentState;                            // Current state engine state.
    ENET_PHY_CMD*                 PHYDev_pInitCmd;                                // Must be got from ACPI.
    ENET_PHY_CMD*                 PHYDev_pGetConfigCmd;                           // Get configuration command pointer (10/100 Mbs or 1000 Mbs).
    ENET_PHY_CMD*                 PHYDev_pGetLinkConfigCmd;                       // Get link configuration command pointer (10/100 Mbs or 1000 Mbs).
    MII_REG_t                     PHYDev_MIIRegs;                                 // Copy of PHY device MII registers.
    IMX_MII_LINK_PROPERTIES_t     PHYDev_LinkPropertiesToAdvertise;               // Link properties requested by the user (read from windows registry).
    IMX_MII_LINK_PROPERTIES_t     PHYDev_SupportedLinkProperties;                 // Link properties suppoprted by the PHY (read from MII_S and MII_ES registers).
    IMX_MII_LINK_PROPERTIES_t     PHYDev_AdvertisedLinkProperties;                // Link properties advertised to the Link partner.
    IMX_MII_LINK_PROPERTIES_t     PHYDev_LinkPartnerLinkProperties;               // Link properties reported by the Link partner.
    IMX_MII_LINK_STATE_t          PHYDev_LinkState;                               // Current link state.
#if DBG                       
    char                          PHYDev_DeviceName[MAX_DEVICE_NAME + 1];         // MDIO bus device name.
#endif                        
} MP_PHY_DEVICE, *PMP_PHY_DEVICE;

void PHY_FrameDoneCallback   (UINT RegAddress, UINT16 RegVal, NDIS_HANDLE Context);
void PHYCmd_CmdDone_Callback (UINT RegVal, NDIS_HANDLE MiniportAdapterHandle);
void MII_StateEngine         (NDIS_HANDLE MiniportAdapterHandle);

#endif // _MP_ENET_PHY_H
