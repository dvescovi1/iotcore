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

ENET_PHY_CMD PHYCmd_GetConfig1000Cmd[] = {
    {MII_READ_COMMAND(MII_REG_BASET_1000_C), NULL},                        // Get media type (1000 Mbs) advertised to the remote partner.
    {MII_READ_COMMAND(MII_REG_BASET_1000_S), NULL},                        // Get media type (1000 Mbs) advertised by the remote partner.
    {MII_READ_COMMAND(MII_REG_C),            NULL},                        // Get Control register.
    {MII_READ_COMMAND(MII_REG_ANA),          NULL},                        // Get media type (10/100 Mbs) advertised to the remote partner.
    {MII_READ_COMMAND(MII_REG_LPA),          NULL},                        // Get media type (10/100 Mbs) advertised by the remote partner.
    {MII_READ_COMMAND(MII_REG_S),            PHYCmd_CmdDone_Callback},     // Get status register.
    {ENET_MII_END,                           NULL}
};

ENET_PHY_CMD PHYCmd_GetStatusRegsCmd[] = {
    {MII_READ_COMMAND(MII_REG_ES),           NULL},                        // Get media type (1000 Mbs) supported by the local device.
    {MII_READ_COMMAND(MII_REG_S),            PHYCmd_CmdDone_Callback},     // Get status register.
    {ENET_MII_END,                           NULL}
};

#define PHYCmd_GetConfig100Cmd  &(PHYCmd_GetConfig1000Cmd[2])
#define PHYCmd_GetStatusRegCmd  &(PHYCmd_GetStatusRegsCmd[1])

#define DBG_PHY_DEV_PRINT_LINK_PROPERTIES(_msg_, _LinkProperties_)  \
    DBG_PHY_DEV_PRINT_INFO(_msg_##" - AN:%d, PS:%d, AS_PS:%d, T_10_HD=%d, T_10_FD=%d, TX_100_HD=%d, TX_100_FD=%d, T_1000_HD=%d, T_1000_FD=%d", \
        _LinkProperties_.B.AUTO_NEGOTIATION, \
        _LinkProperties_.B.PAUSE, \
        _LinkProperties_.B.ASYMETRIC_PAUSE, \
        _LinkProperties_.B.BASE_T_10_HD, \
        _LinkProperties_.B.BASE_T_10_FD, \
        _LinkProperties_.B.BASE_TX_100_HD, \
        _LinkProperties_.B.BASE_TX_100_FD, \
        _LinkProperties_.B.BASE_T_1000_HD, \
        _LinkProperties_.B.BASE_T_1000_FD \
)

/*++
Routine Description:
    Frame done callback.
Argument:
    RegAddress - MII register address.
    RegVal     - MII registre value.
    Context    - Callback context.
--*/
void PHY_FrameDoneCallback(UINT RegAddress, UINT16 RegVal, NDIS_HANDLE Context)
 {
    if (RegAddress < 16) {
        ((UINT16 *)&((MP_PHY_DEVICE*)Context)->PHYDev_MIIRegs)[RegAddress] = RegVal;
    }
}

/*++
Routine Description:
    Command done callbac. Sets CmdPending variable to FALSE value.
Argument:
    RegVal                - Not used.
    MiniportAdapterHandle - Context parametr.
--*/
void PHYCmd_CmdDone_Callback(UINT RegVal, NDIS_HANDLE MiniportAdapterHandle) {
    UNREFERENCED_PARAMETER(RegVal);
    ((MP_PHY_DEVICE*)MiniportAdapterHandle)->PHYDev_pMDIODev->MDIODev_CmdPending = FALSE;
}

/*++
Routine Description:
    Returns PHY supported media type.
Argument:
    pMII - MII registers base address.
Return Value:
    Media type supported by the PHY.
--*/
static inline UINT32 MII_GetSupportedLinkProperties(MII_REG_t *pMII)
{
    IMX_MII_LINK_PROPERTIES_t  LinkProperties;

    LinkProperties.R = IMX_MII_LINK_PROPERTIES_PAUSE | IMX_MII_LINK_PROPERTIES_ASYMETRIC_PAUSE; // Local device supports both Pause Rx and Pause Tx.
    LinkProperties.B.AUTO_NEGOTIATION = pMII->S.B.AUTO_NEGOTIATION_ABILITY;                     // Local device supports Local device supports Auto negotiation ability.
    LinkProperties.B.BASE_T_10_HD     = pMII->S.B.BASE_T_10_FD;                                 // Local device supports 10BASE-T half duplex.
    LinkProperties.B.BASE_T_10_FD     = pMII->S.B.BASE_T_10_HD;                                 // Local device supports 10BASE-T full duplex.
    LinkProperties.B.BASE_TX_100_HD   = pMII->S.B.BASE_X_100_HD;                                // Local device supports 100BASE-TX half duplex.
    LinkProperties.B.BASE_TX_100_FD   = pMII->S.B.BASE_X_100_FD;                                // Local device supports 100BASE-TX full duplex.
    if (pMII->S.B.EXTENDED_STATUS) {
        LinkProperties.B.BASE_T_1000_HD = pMII->ES.B.BASE_T_1000_HD;                            // Local device supports 1000BASE-T half duplex.
        LinkProperties.B.BASE_T_1000_FD = pMII->ES.B.BASE_T_1000_FD;                            // Local device supports 1000BASE-T full duplex.
    }
    return LinkProperties.R;
}

/*++
Routine Description:
    Returns advertised media type.
Argument:
    pMII - MII registers base address.
Return Value:
    Media type advertised by the PHY
--*/
static inline UINT32 MII_GetAdvertisedLinkProperties(MII_REG_t *pMII)
{
    IMX_MII_LINK_PROPERTIES_t  LinkProperties;

    LinkProperties.R = 0;
    if (pMII->C.B.AUTO_NEGOTIATION_ENABLE) {
        // Auto negotiation is enabled, read advertised values from ANA and BASE_T_1000 registers.
        LinkProperties.B.AUTO_NEGOTIATION = 1;                                      // Local device has advertised Auto negotiation ability.
        LinkProperties.B.ASYMETRIC_PAUSE  = pMII->ANA.B.ASYMETRIC_PAUSE;            // Local device has advertised Asymetric pause.
        LinkProperties.B.PAUSE            = pMII->ANA.B.PAUSE;                      // Local device has advertised Pause.
        LinkProperties.B.BASE_TX_100_FD   = pMII->ANA.B.BASE_TX_100_FD;             // Local device has advertised 100BASE-TX full duplex.
        LinkProperties.B.BASE_TX_100_HD   = pMII->ANA.B.BASE_TX_100_HD;             // Local device has advertised 100BASE-TX half duplex.
        LinkProperties.B.BASE_T_10_FD     = pMII->ANA.B.BASE_T_10_HD;               // Local device has advertised 10BASE-T full duplex.
        LinkProperties.B.BASE_T_10_HD     = pMII->ANA.B.BASE_T_10_FD;               // Local device has advertised 10BASE-T half duplex.
        if (pMII->S.B.EXTENDED_STATUS) {
            LinkProperties.B.BASE_T_1000_FD = pMII->BASET_1000_C.B.BASE_T_1000_FD;  // Local device has advertised 1000BASE-T full duplex.
            LinkProperties.B.BASE_T_1000_HD = pMII->BASET_1000_C.B.BASE_T_1000_HD;  // Local device has advertised 1000BASE-T half duplex.
        }
    } else {
        // Auto negotiation is disabled, decode "advertised values" from Control register
        switch (pMII->C.R & (MII_REG_C_DUPLEX_MODE_MASK | MII_REG_C_SPEED_SELECTION_MSB_MASK | MII_REG_C_SPEED_SELECTION_LSB_MASK)) {
            case 0:
                LinkProperties.B.BASE_T_10_HD = 1;
                break;
            case MII_REG_C_DUPLEX_MODE_MASK:
                LinkProperties.B.BASE_T_10_FD = 1;
                break;
            case MII_REG_C_SPEED_SELECTION_LSB_MASK:
                LinkProperties.B.BASE_TX_100_HD = 1;
                break;
            case MII_REG_C_DUPLEX_MODE_MASK | MII_REG_C_SPEED_SELECTION_LSB_MASK:
                LinkProperties.B.BASE_TX_100_FD = 1;
                break;
            case MII_REG_C_SPEED_SELECTION_MSB_MASK:
                LinkProperties.B.BASE_T_1000_HD = 1;
                break;
            case MII_REG_C_DUPLEX_MODE_MASK | MII_REG_C_SPEED_SELECTION_MSB_MASK:
                LinkProperties.B.BASE_T_1000_FD = 1;
                break;
        }
    }
    return LinkProperties.R;
}

/*++
Routine Description:
    Returns link partner advertised media type.
Argument:
    pMII - MII registers base address
Return Value:
    Media type advertised by the link partner
--*/
static inline UINT32 MII_GetLinkPartnerLinkProperties(MII_REG_t *pMII)
{
    IMX_MII_LINK_PROPERTIES_t  LinkProperties;

    LinkProperties.R = 0;
    if (pMII->C.B.AUTO_NEGOTIATION_ENABLE) {
        // Auto negotiation is enabled, read Link partner supported modes from LPA and BASE_S_1000 registers.
        LinkProperties.B.AUTO_NEGOTIATION = 1;                                          // Link partner has advertised Auto negotiation ability.
        if (pMII->S.B.AUTO_NEGOTIATION_COMPLETE) {
            LinkProperties.B.ASYMETRIC_PAUSE  = pMII->LPA.B.ASYMETRIC_PAUSE;            // Link partner has advertised Asymetric pause.
            LinkProperties.B.PAUSE            = pMII->LPA.B.PAUSE;                      // Link partner has advertised pause frames.
            LinkProperties.B.BASE_T_10_HD     = pMII->LPA.B.BASE_T_10_FD;               // Link partner has advertised 10BASE-T half duplex.
            LinkProperties.B.BASE_T_10_FD     = pMII->LPA.B.BASE_T_10_HD;               // Link partner has advertised 10BASE-T full duplex.
            LinkProperties.B.BASE_TX_100_HD   = pMII->LPA.B.BASE_TX_100_HD;             // Link partner has advertised 100BASE-TX half duplex.
            LinkProperties.B.BASE_TX_100_FD   = pMII->LPA.B.BASE_TX_100_FD;             // Link partner has advertised 100BASE-TX full duplex.
            if (pMII->S.B.EXTENDED_STATUS) {
                LinkProperties.B.BASE_T_1000_HD = pMII->BASET_1000_S.B.BASE_T_1000_HD;  // Link partner has advertised 1000BASE-T half duplex.
                LinkProperties.B.BASE_T_1000_FD = pMII->BASET_1000_S.B.BASE_T_1000_FD;  // Link partner has advertised 1000BASE-T full duplex.
            }
        }
    } else {
        // Auto negotiation is disabled, suppose Link partner supports all modes.
        LinkProperties.R = IMX_MII_LINK_PROPERTIES_BASE_ALL_MASK;
    }
    return LinkProperties.R;
}

/*++
Routine Description:
    Decodes selected link speed and duplex mode.
Argument:
    pPHYDev - Pointer to Phy device structure.
--*/
void  MII_GetSelectedLinkProperties(MP_PHY_DEVICE *pPHYDev)
{
    IMX_MII_LINK_PROPERTIES_t  LinkProperties;
    UINT32                     LinkState = 0;

    LinkProperties.R = pPHYDev->PHYDev_SupportedLinkProperties.R & pPHYDev->PHYDev_AdvertisedLinkProperties.R  & pPHYDev->PHYDev_LinkPartnerLinkProperties.R;
    if (LinkProperties.B.BASE_T_1000_FD) {                             // 1000BASE-T full duplex.
        LinkState = IMX_MII_LINK_STATE_DUPLEX_MODE_SET(MediaDuplexStateFull) | IMX_MII_LINK_STATE_SPEED_MHZ_SET(1000) | IMX_MII_LINK_STATE_CONNECT_STATE_SET(MediaConnectStateConnected);
    } else if (LinkProperties.B.BASE_T_1000_HD) {                      // 1000BASE-T half duplex.
        LinkState = IMX_MII_LINK_STATE_DUPLEX_MODE_SET(MediaDuplexStateHalf) | IMX_MII_LINK_STATE_SPEED_MHZ_SET(1000) | IMX_MII_LINK_STATE_CONNECT_STATE_SET(MediaConnectStateConnected);
    } else if (LinkProperties.B.BASE_TX_100_FD) {                      // 100BASE-TX full duplex.
        LinkState = IMX_MII_LINK_STATE_DUPLEX_MODE_SET(MediaDuplexStateFull) | IMX_MII_LINK_STATE_SPEED_MHZ_SET(100) | IMX_MII_LINK_STATE_CONNECT_STATE_SET(MediaConnectStateConnected);
    } else if (LinkProperties.B.BASE_TX_100_HD) {                      // 100BASE-TX half duplex.
        LinkState = IMX_MII_LINK_STATE_DUPLEX_MODE_SET(MediaDuplexStateHalf) | IMX_MII_LINK_STATE_SPEED_MHZ_SET(100) | IMX_MII_LINK_STATE_CONNECT_STATE_SET(MediaConnectStateConnected);
    } else if (LinkProperties.B.BASE_T_10_FD) {                        // 10BASE-T full duplex.
        LinkState = IMX_MII_LINK_STATE_DUPLEX_MODE_SET(MediaDuplexStateFull) | IMX_MII_LINK_STATE_SPEED_MHZ_SET(10) | IMX_MII_LINK_STATE_CONNECT_STATE_SET(MediaConnectStateConnected);
    } else if (LinkProperties.B.BASE_T_10_HD) {                        // 10BASE-T half duplex.
        LinkState = IMX_MII_LINK_STATE_DUPLEX_MODE_SET(MediaDuplexStateHalf) | IMX_MII_LINK_STATE_SPEED_MHZ_SET(10) | IMX_MII_LINK_STATE_CONNECT_STATE_SET(MediaConnectStateConnected);
    } else {
        LinkState = IMX_MII_LINK_STATE_DUPLEX_MODE_SET(MediaDuplexStateUnknown) | IMX_MII_LINK_STATE_SPEED_MHZ_SET(0)  | IMX_MII_LINK_STATE_CONNECT_STATE_SET(MediaConnectStateDisconnected);
    }
    pPHYDev->PHYDev_LinkState.R = LinkState;
    DBG_PHY_DEV_PRINT_INFO("Selected Media type %d Mbs, %s duplex.", IMX_MII_LINK_STATE_SPEED_MHZ_GET(LinkState), (IMX_MII_LINK_STATE_DUPLEX_MODE_GET(LinkState)==MediaDuplexStateFull?"Full":(IMX_MII_LINK_STATE_DUPLEX_MODE_GET(LinkState)?"Half":"Unknown")));
    return;
}

/*++
Routine Description:
    Decodes selected link speed and duplex mode.
Argument:
    pPHYDev                   - Pointer to Phy device structure.
    LinkPropertiesToAdvertise - New link properties.
--*/
void MII_SetLinkPropertiesCmd(MP_PHY_DEVICE *pPHYDev, IMX_MII_LINK_PROPERTIES_t LinkPropertiesToAdvertise)
{
    MII_REG_t                   *pMII = &pPHYDev->PHYDev_MIIRegs;
    IMX_MII_LINK_PROPERTIES_t  LinkProperties;
    MII_REG_C_t                  C;
    MII_REG_ANA_t                ANA;
    MII_REG_BASET_1000_C_t       BASET_1000_C;
    ENET_PHY_CMD                 PHYCmdSetLinkProperties[4];
    int i = 0;

    DBG_PHY_DEV_METHOD_BEG();
    LinkProperties.R = pPHYDev->PHYDev_SupportedLinkProperties.R & LinkPropertiesToAdvertise.R;
    DBG_PHY_DEV_PRINT_LINK_PROPERTIES("NewLinkProperties", pPHYDev->PHYDev_SupportedLinkProperties);
    if (LinkProperties.B.AUTO_NEGOTIATION == 1) {
        // Auto negotiation is enabled, write advertised values to ANA and BASE_T_1000 registers.
        ANA.R = 1;                                                                    // Selector field = 802.3
        ANA.B.ASYMETRIC_PAUSE  = (UINT16)LinkProperties.B.ASYMETRIC_PAUSE;            // Asymmetric pause.
        ANA.B.PAUSE            = (UINT16)LinkProperties.B.PAUSE;                      // Pause.
        ANA.B.BASE_TX_100_FD   = (UINT16)LinkProperties.B.BASE_TX_100_FD;             // 100BASE-TX full duplex.
        ANA.B.BASE_TX_100_HD   = (UINT16)LinkProperties.B.BASE_TX_100_HD;             // 100BASE-TX half duplex.
        ANA.B.BASE_T_10_HD     = (UINT16)LinkProperties.B.BASE_T_10_FD;               // 10BASE-T full duplex.
        ANA.B.BASE_T_10_FD     = (UINT16)LinkProperties.B.BASE_T_10_HD;               // 10BASE-T half duplex.
        PHYCmdSetLinkProperties[i].MIIData    = MII_WRITE_COMMAND(MII_REG_ANA, ANA.R);
        PHYCmdSetLinkProperties[i++].MIIFunct = NULL;
        if (pMII->S.B.EXTENDED_STATUS) {
            BASET_1000_C.R = 0;
            BASET_1000_C.B.BASE_T_1000_FD = (UINT16)LinkProperties.B.BASE_T_1000_FD;  // 1000BASE-T full duplex.
            BASET_1000_C.B.BASE_T_1000_HD = (UINT16)LinkProperties.B.BASE_T_1000_HD;  // 1000BASE-T half duplex.
            PHYCmdSetLinkProperties[i].MIIData    = MII_WRITE_COMMAND(MII_REG_BASET_1000_C, BASET_1000_C.R);
            PHYCmdSetLinkProperties[i++].MIIFunct = NULL;
        }
        C.R = MII_REG_C_RESTART_AUTO_NEGOTIATION_MASK | MII_REG_C_AUTO_NEGOTIATION_ENABLE_MASK;
    } else {
        // Auto negotiation is disabled, write "advertised values" to the Control register.
        C.R = 0;
        if (LinkProperties.B.BASE_T_1000_FD) {                             // 1000BASE-T full duplex.
            C.R |= (UINT16)(MII_REG_C_DUPLEX_MODE_MASK | MII_REG_C_SPEED_SELECTION_MSB_MASK);
        } else if (LinkProperties.B.BASE_T_1000_HD) {                      // 1000BASE-T half duplex.
            C.R |= (UINT16)(MII_REG_C_SPEED_SELECTION_MSB_MASK);
        } else if (LinkProperties.B.BASE_TX_100_FD) {                      // 100BASE-TX full duplex.
            C.R |= (UINT16)(MII_REG_C_DUPLEX_MODE_MASK | MII_REG_C_SPEED_SELECTION_LSB_MASK);
        } else if (LinkProperties.B.BASE_TX_100_HD) {                      // 100BASE-TX half duplex.
            C.R |= (UINT16)(MII_REG_C_SPEED_SELECTION_LSB_MASK);
        } else if (LinkProperties.B.BASE_T_10_FD) {                        // 10BASE-T full duplex.
            C.R |= (UINT16)(MII_REG_C_DUPLEX_MODE_MASK);
        } else if (LinkProperties.B.BASE_T_10_HD) {                        // 10BASE-T half duplex.
            C.R |= (UINT16)(0);
        } else {
        }
    }
    PHYCmdSetLinkProperties[i].MIIData    = MII_WRITE_COMMAND(MII_REG_C, C.R);
    PHYCmdSetLinkProperties[i++].MIIFunct = NULL;
    PHYCmdSetLinkProperties[i].MIIData    = 0x00;
    MDIODev_QueueCommand(pPHYDev->PHYDev_pMDIODev, PHYCmdSetLinkProperties);
    DBG_PHY_DEV_METHOD_END();
    return;
}

/*++
Routine Description:
    MII state engine, Should be called periodically from a system thread.
Argument:
    MiniportAdapterHandle - Context parameter.
--*/
void MII_StateEngine(NDIS_HANDLE MiniportAdapterHandle)
{
    MP_PHY_DEVICE         *pPHYDev  = &((PMP_ADAPTER)MiniportAdapterHandle)->ENETDev_PHYDevice;
    MP_MDIO_DEVICE        *pMDIODev = pPHYDev->PHYDev_pMDIODev;
    MII_REG_t             *pMII     = &pPHYDev->PHYDev_MIIRegs;
    MP_PHY_DEVICE_STATE    PHYDev_NextState;
    ENET_PHY_CMD*          pNewCmd = PHYCmd_GetStatusRegCmd;    // Read S register.
    BOOLEAN                LinkStateChanged = FALSE;

    DBG_PHY_DEV_METHOD_BEG();
    if (!pMDIODev->MDIODev_CmdPending) {
        DBG_PHY_DEV_PRINT_INFO("Current state: %s", Dbg_GetPhyStateEngineStateName(pPHYDev->PHYDev_CurrentState));
        switch (pPHYDev->PHYDev_CurrentState) {
            case MP_PHY_STATE_NOT_INITIALIZED:
                // This is the first call of MII_StateEngine(), write initialisation sequence from ACPI.
                if (pPHYDev->PHYDev_pInitCmd) {
                    MDIODev_QueueCommand(pMDIODev, pPHYDev->PHYDev_pInitCmd);
                }
                pNewCmd = PHYCmd_GetStatusRegsCmd;             // Read S and ES registers.
                PHYDev_NextState = MP_PHY_STATE_INITIALIZED;   // Remember new state after initialisation sequence is done.
                break;
            case MP_PHY_STATE_INITIALIZED:
                // PHY initialisation sequence is done, read current state of registers required to get current media state.
                DBG_PHY_DEV_PRINT_TRACE("PHY was initialized.");
                pPHYDev->PHYDev_SupportedLinkProperties.R = MII_GetSupportedLinkProperties(pMII);          // Get speed, duplex, auto-nego ability and pause frames supported by the PHY/MAC
                pPHYDev->PHYDev_LinkPropertiesToAdvertise.R  &= pPHYDev->PHYDev_SupportedLinkProperties.R; // Mask speed, duplex and auto-nego ability required by the USER (.inf file) and not supported bu the PHY
                DBG_PHY_DEV_PRINT_LINK_PROPERTIES("SupportedLinkProperties   ", pPHYDev->PHYDev_SupportedLinkProperties);
                DBG_PHY_DEV_PRINT_LINK_PROPERTIES("LinkPropertiesToAdvertise ", pPHYDev->PHYDev_LinkPropertiesToAdvertise);
                if (pMII->S.B.EXTENDED_STATUS) {
                    pPHYDev->PHYDev_pGetLinkConfigCmd = PHYCmd_GetConfig1000Cmd;
                } else {
                    pPHYDev->PHYDev_pGetLinkConfigCmd = PHYCmd_GetConfig100Cmd;
                }
                if (pMII->S.B.LINK_STATUS) {                           // Link is UP, get current configuration and check if PHY restart is required
                    DBG_PHY_DEV_PRINT_TRACE("Link is UP, read current configuration and compare with requested configuration.");
                    pNewCmd = pPHYDev->PHYDev_pGetLinkConfigCmd;       // Read current link configuration from PHY.
                    PHYDev_NextState = MP_PHY_STATE_DEFAULT_CFG_RAED;  // Remember new state.
                }  else {
                    DBG_PHY_DEV_PRINT_TRACE("Link is DOWN, set default configuration.");
                    MII_SetLinkPropertiesCmd(pPHYDev, pPHYDev->PHYDev_LinkPropertiesToAdvertise);
                    PHYDev_NextState = MP_PHY_STATE_LINK_DOWN;
                }
                break;
            case MP_PHY_STATE_DEFAULT_CFG_RAED:
               if (pMII->S.B.LINK_STATUS) {            // UP
                   DBG_PHY_DEV_PRINT_TRACE("Link is UP, compare requested and already advertised link properties.");
                   pPHYDev->PHYDev_AdvertisedLinkProperties.R = MII_GetAdvertisedLinkProperties(pMII);       // Get speed, duplex and auto-nego ability already advertised by the PHY to the link partner
                   DBG_PHY_DEV_PRINT_LINK_PROPERTIES("AdvertisedLinkProperties  ", pPHYDev->PHYDev_AdvertisedLinkProperties);
                   if (pPHYDev->PHYDev_LinkPropertiesToAdvertise.R !=  pPHYDev->PHYDev_AdvertisedLinkProperties.R) {
                       // PHY (in Uboot) has advertised media properties that is disabled by the user in imxnetmini.inf file or is not supported by the PHY, reconfigure PHY.
                       DBG_PHY_DEV_PRINT_TRACE("Requested link properties != already advertised link properties. Set new link properties.");
                       MII_SetLinkPropertiesCmd(pPHYDev, pPHYDev->PHYDev_LinkPropertiesToAdvertise);
                       PHYDev_NextState = MP_PHY_STATE_LINK_DOWN;                                            // Remember new state after PHY re-configuration.
                   } else {
                       DBG_PHY_DEV_PRINT_TRACE("Requested link properties == already advertised link properties. Do not reconfigure PHY.");
                       PHYDev_NextState = MP_PHY_STATE_LINK_DOWN_TO_UP;
                   }
               }  else {
                   DBG_PHY_DEV_PRINT_TRACE("Link is DOWN, set default configuration.");
                   MII_SetLinkPropertiesCmd(pPHYDev, pPHYDev->PHYDev_LinkPropertiesToAdvertise);
                   PHYDev_NextState = MP_PHY_STATE_LINK_DOWN;
               }
                break;
            case MP_PHY_STATE_LINK_UP:
                if (pMII->S.B.LINK_STATUS) {            // UP -> UP
                    PHYDev_NextState = MP_PHY_STATE_LINK_UP;
                } else {                                // UP -> DOWN
                    pPHYDev->PHYDev_LinkPartnerLinkProperties.R = 0;
                    PHYDev_NextState = MP_PHY_STATE_LINK_DOWN;
                    LinkStateChanged = TRUE;
                }
                break;
            case MP_PHY_STATE_LINK_DOWN_TO_UP:
                if (pMII->S.B.LINK_STATUS) {            // UP -> UP
                    DBG_PHY_DEV_PRINT_TRACE("Link is up.");
                    pPHYDev->PHYDev_AdvertisedLinkProperties.R  = MII_GetAdvertisedLinkProperties(pMII);       // Get speed, duplex and auto-nego ability already advertised to the link partner.
                    pPHYDev->PHYDev_LinkPartnerLinkProperties.R = MII_GetLinkPartnerLinkProperties(pMII);      // Get speed, duplex and auto-nego ability already advertised by the link partner.
                    DBG_PHY_DEV_PRINT_LINK_PROPERTIES("AdvertisedLinkProperties  ", pPHYDev->PHYDev_AdvertisedLinkProperties);
                    DBG_PHY_DEV_PRINT_LINK_PROPERTIES("LinkPartnerLinkProperties ", pPHYDev->PHYDev_LinkPartnerLinkProperties);
                    PHYDev_NextState = MP_PHY_STATE_LINK_UP;
                    LinkStateChanged = TRUE;
                    if (pPHYDev->PHYDev_LinkPropertiesToAdvertise.R & IMX_MII_LINK_PROPERTIES_AUTO_NEGOTIATION) {                  // Is auto-negotiation enabled?
                        if (pPHYDev->PHYDev_LinkPropertiesToAdvertise.R & IMX_MII_LINK_PROPERTIES_BASE_T_1000_FD_MASK) {           // Is 1Gbps supported by local device?
                            IMX_MII_LINK_PROPERTIES_t tmpLinkPropToAdvertise;
                            if (pPHYDev->PHYDev_AdvertisedLinkProperties.R & pPHYDev->PHYDev_LinkPartnerLinkProperties.R & IMX_MII_LINK_PROPERTIES_BASE_T_1000_FD_MASK) {  // Was 1Gbps speed selected?
                                if (!(pPHYDev->PHYDev_LinkPartnerLinkProperties.R & IMX_MII_LINK_PROPERTIES_PAUSE)) {                                                      // Does link partner supports pause frame reception?
                                    DBG_PHY_DEV_PRINT_TRACE("Link is up, 1 Gbps speed is selected but link partner doesnt support pause frames, 1 Gbps will be disabled and auto-negotiation will be restarted.");
                                    tmpLinkPropToAdvertise.R = pPHYDev->PHYDev_LinkPropertiesToAdvertise.R & ~IMX_MII_LINK_PROPERTIES_BASE_T_1000_FD_MASK;
                                    MII_SetLinkPropertiesCmd(pPHYDev, tmpLinkPropToAdvertise);
                                    LinkStateChanged = FALSE;
                                    PHYDev_NextState = MP_PHY_STATE_LINK_DOWN;
                                }
                            } else {
                                if ((pPHYDev->PHYDev_LinkPartnerLinkProperties.R & (IMX_MII_LINK_PROPERTIES_BASE_T_1000_FD_MASK | IMX_MII_LINK_PROPERTIES_PAUSE)) ==
                                    (IMX_MII_LINK_PROPERTIES_BASE_T_1000_FD_MASK | IMX_MII_LINK_PROPERTIES_PAUSE)) {  // Does link partner supports 1Gbps and pause frame reception?
                                    DBG_PHY_DEV_PRINT_TRACE("Link is up, 1 Gbps speed is not selected but link partner Support pause frames, 1 Gbps will be enabled and auto-negotiation will be restarted.");
                                    tmpLinkPropToAdvertise.R = pPHYDev->PHYDev_LinkPropertiesToAdvertise.R | IMX_MII_LINK_PROPERTIES_BASE_T_1000_FD_MASK;
                                    MII_SetLinkPropertiesCmd(pPHYDev, tmpLinkPropToAdvertise);
                                    LinkStateChanged = FALSE;
                                    PHYDev_NextState = MP_PHY_STATE_LINK_DOWN;
                                }

                            }
                        }
                    }
                } else {                                // UP -> DOWN
                    PHYDev_NextState = MP_PHY_STATE_LINK_DOWN;
                }
                break;
            case MP_PHY_STATE_LINK_DOWN:
                if (pMII->S.B.LINK_STATUS) {                         // DOWN -> DOWN_TO_UP.
                    pNewCmd = pPHYDev->PHYDev_pGetLinkConfigCmd;     // Read current link configuration from PHY.
                    PHYDev_NextState = MP_PHY_STATE_LINK_DOWN_TO_UP; // Remember new state.
                } else {                                             // DOWN -> DOWN.
                    PHYDev_NextState = MP_PHY_STATE_LINK_DOWN;       // Do not change state.
                }
                break;
            default:
                PHYDev_NextState = MP_PHY_STATE_NOT_INITIALIZED;
                break;
        }
        DBG_PHY_DEV_PRINT_INFO("New state: %s", Dbg_GetPhyStateEngineStateName(PHYDev_NextState));
        pPHYDev->PHYDev_CurrentState = PHYDev_NextState;           // Remember new state.
        if (LinkStateChanged) {
            MII_GetSelectedLinkProperties(pPHYDev);                // Get current speed and duplex.
            EnetOnLinkStateChanged(pPHYDev);                       // Report new state.
        }
        MDIODev_QueueCommand(pMDIODev, pNewCmd);
    }
    DBG_PHY_DEV_METHOD_END();
}