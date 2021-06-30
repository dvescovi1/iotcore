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

/*++
Routine Description:
    This routine removes item from the list of items.
Arguments:
    ppListHead  - Pointer to the "list head" pointer.
    ppListHead  - Pointer to removed item.
Return Value:
    none
--*/
void MDIO_RemoveFromList(_Inout_ PVOID * ppListHead, _In_ PVOID pListItem) {
    PVOID pCurrentItem;

    if ((pCurrentItem = *ppListHead) == pListItem) {            // First item in the list?
        *ppListHead = *(PVOID *)pListItem;                      // Yes, list head.
    } else {
        while (pCurrentItem != NULL) {                          // Remove MDIO bus device from the list.
            if (*(PVOID*)pCurrentItem == pListItem) {           // Item found?
                *(PVOID*)pCurrentItem = *(PVOID *)pListItem;    // Yes, remove it from the list.
                break;                                          // Stop loop.
            } else {
                pCurrentItem = *(PVOID *)pCurrentItem;          // Get next item.
            }
        }
    }
    return;
}

/*++
Routine Description:
    MDIO bus main thread. The thread contains a single loop waiting for one of these thee events: Kill event, Start transfer event and Timer event.
    switch(event)
        case KillEvent:
            Terminate thread;
        case TimerEvent:
            Transfer done? 
                No, restart timer (MDIO_TransferTimeout_ms).
                Yes, find new MDIO device ready to transfer a frame (round-robin priority).
                New frame found?
                    Yes, Start transfer (See below).
                    No, Stop MDIO clock.
        case StartTransfer:
            Write frame command (MMFR register).
            Start MDIO clock (MMCR register).
            Start timer (MDIO_TransferTimeout_ms).
Arguments:
    pMDIOBus - MDIO bus data structure address.
Return Value:
    none
--*/
_Function_class_(KSTART_ROUTINE)
VOID MDIOBus_Main(_In_ MP_MDIO_BUS *pMDIOBus)
{
    #define  MDIO_Events_Count 3

    MP_MDIO_DEVICE*         pMDIODev;
    MP_MDIO_DEVICE*         pMDIONextDev;
    MP_MDIO_FRAME*          pActiveFrame, Frame;
    volatile CSP_ENET_REGS* MDIORegs = pMDIOBus->MDIOBus_pRegBase;
    NTSTATUS                Status;
    PVOID                   MII_Events[MDIO_Events_Count];
    LARGE_INTEGER           DueTime = { .QuadPart = -MDIO_TransferTimeout_ms * (10 * 1000) };

    C_ASSERT(MDIO_Events_Count <= THREAD_WAIT_OBJECTS);

    DBG_MDIO_BUS_METHOD_BEG();

    MII_Events[0] = (PVOID)&pMDIOBus->MDIOBus_KillEvent;
    MII_Events[1] = (PVOID)&pMDIOBus->MDIOBus_Timer;
    MII_Events[2] = (PVOID)&pMDIOBus->MDIOBus_StartTransferEvent;

    KeInitializeTimerEx(&pMDIOBus->MDIOBus_Timer, SynchronizationTimer);                                      // Initialize one-shot auto-reset timer
    do {
        Status = KeWaitForMultipleObjects(MDIO_Events_Count, MII_Events, WaitAny, Executive, KernelMode, FALSE, NULL, NULL);
        if (!NT_SUCCESS(Status)) {
            DBG_MDIO_BUS_PRINT_ERROR_WITH_STATUS("KeWaitForMultipleObjects() failed.");
            break;
        } else if (Status == STATUS_WAIT_0) {
            DBG_MDIO_BUS_PRINT_INFO("KIll enevnt signaled.");
            break;
        } else {
            // Timer has expirad or new command has been queued
            NdisAcquireSpinLock(&pMDIOBus->MDIOBus_BusSpinLock);                                              // Acquire MDIO bus spin lock.
            if (MDIORegs->EIR.U & ENET_EIR_MII_MASK) {                                                        // Transfer done?
                MDIORegs->EIR.U = ENET_EIR_MII_MASK;                                                          // Clear Transfer done flag.
                if ((pMDIODev = pMDIOBus->MDIOBus_pActiveDevice) != NULL) {                                   // Is there a device waiting for transfer end?
                    Frame.MIIFunction = NULL;                                                                 
                    NdisAcquireSpinLock(&pMDIODev->MDIODev_DeviceSpinLock);                                   // Yes, acquire MDIO device frame list spin lock.
                    if ((pActiveFrame = pMDIODev->MDIODev_pPendingFrameListHead) != NULL) {                   // Is there a frame waiting for transfer end?
                        DBG_MDIO_DEV_CMD_PRINT_TRACE("Transfer done. Frame: 0x%p.", pActiveFrame);
                        pMDIODev->MDIODev_pPendingFrameListHead = pActiveFrame->MDIOFrm_pNextFrame;           // Update pending frame list head.
                        pActiveFrame->MDIOFrm_pNextFrame = pMDIODev->MDIODev_pFreeFrameListHead;              // Return frame to the free frame list.
                        pMDIODev->MDIODev_pFreeFrameListHead = pActiveFrame;                                  
                        Frame = *pActiveFrame;                                                                // Create local copy of active frame.
                    } else {
                        DBG_MDIO_DEV_CMD_PRINT_TRACE("Transfer done. No frame is active.");
                    }
                    pMDIOBus->MDIOBus_TransferInProgress = FALSE;                                             // Remember that no transfer is pending now.
                    NdisReleaseSpinLock(&pMDIODev->MDIODev_DeviceSpinLock);                                   // Release MDIO device spin lock.
                    DBG_MDIO_DEV_CMD_PRINT_INFO("Frame done. MMFR: 0x%08X (ST=%d, OP=%s, PHYADR=%d, REG=0x%02X)", MDIORegs->MMFR.U, \
                        (MDIORegs->MMFR.U >> 30) & 0x03, \
                        (((MDIORegs->MMFR.U >> 28) & 0x03)==1)?"WRITE":((((MDIORegs->MMFR.U >> 28) & 0x03)==2)?"READ":"UNKNOWN"), \
                        (MDIORegs->MMFR.U >> 23) & 0x1F, \
                        (MDIORegs->MMFR.U >> 18) & 0x1F);
                    if (pMDIODev->MDIODev_MIIFrameDoneCallback) {
                        (pMDIODev->MDIODev_MIIFrameDoneCallback)((MDIORegs->MMFR.U & ENET_MMFR_RA_MASK) >> ENET_MMFR_RA_SHIFT, (UINT16)MDIORegs->MMFR.U, pMDIODev->MDIODev_Context);
                    }                    
                    if (Frame.MIIFunction) {                                                                  // Callback required?
                        DBG_MDIO_DEV_CMD_PRINT_TRACE("Invoking callback. Frame: 0x%p", pActiveFrame);
                        (*Frame.MIIFunction)(MDIORegs->MMFR.U, (NDIS_HANDLE)pMDIODev->MDIODev_Context);       // Yes, call the callback.
                    }
                    pMDIOBus->MDIOBus_pActiveDevice = pMDIOBus->MDIOBus_pActiveDevice->MDIODev_pNextDevice;   // Mark next device in the list as a candiadte to send new frame.
                } else {
                    DBG_MDIO_BUS_PRINT_WARNING("NO device is waiting for transfer done !!!.");
                }
            }
            if (!pMDIOBus->MDIOBus_TransferInProgress) {                                                      // If no transfer is pending, try to start new transfer.
                if ((pMDIODev = pMDIOBus->MDIOBus_pActiveDevice) == NULL) {                                   // No device selected for transfer?
                    pMDIODev = pMDIOBus->MDIOBus_pFirstDevice;                                                // Select first device in the list.
                }
                if (pMDIODev) {
                    pMDIONextDev = pMDIODev;
                    do {
                        NdisAcquireSpinLock(&pMDIODev->MDIODev_DeviceSpinLock);                               // Acquire MDIO device spin lock.
                        if ((pActiveFrame = pMDIODev->MDIODev_pPendingFrameListHead) != NULL) {               // Get new frame from pending frame list.
                            DBG_MDIO_DEV_CMD_PRINT_TRACE("Starting new frame 0x%p.", pActiveFrame);
                            pMDIOBus->MDIOBus_pRegBase->MMFR.U = pActiveFrame->MDIOFrm_MMFRRegVal;            // Write frame register.
                            pMDIOBus->MDIOBus_pRegBase->MSCR.U = pMDIODev->MDIODev_MSCR.U;                    // Write speed and control register.
                            pMDIOBus->MDIOBus_pActiveDevice = pMDIODev;                                       // Mark current device as active device.
                            pMDIOBus->MDIOBus_TransferInProgress = TRUE;                                      // Remember that transfer is in progress.
                            NdisReleaseSpinLock(&pMDIODev->MDIODev_DeviceSpinLock);                           // Release MDIO device frame list spin lock.
                            break;                                                                            
                        }                                                                                     
                        NdisReleaseSpinLock(&pMDIODev->MDIODev_DeviceSpinLock);                               // Release MDIO device spin lock.
                        if ((pMDIODev = pMDIODev->MDIODev_pNextDevice) == NULL) {                             // Current device has no frame ready to sent, select next device.
                            pMDIODev = pMDIOBus->MDIOBus_pFirstDevice;                                        
                        }                                                                                     
                    } while (pMDIODev != pMDIONextDev);                                                       // Stop if no device has frame ready to transfer.
                }                                                                                             
            } else {
                DBG_MDIO_BUS_PRINT_WARNING("Previous transfer is still in progress, consider timer period increase.");
            }
            if (pMDIOBus->MDIOBus_TransferInProgress) {                                                       
                KeSetTimerEx(&pMDIOBus->MDIOBus_Timer, DueTime, 0, NULL);                                     // Restart timer.
            }  else {
                pMDIOBus->MDIOBus_pRegBase->MSCR.U = 0;                                                       // Write speed and control register.
                DBG_MDIO_DEV_CODE(pMDIODev = NULL);
                DBG_MDIO_BUS_PRINT_TRACE("No transfer request, stopping MCIO clock.");
            }
            NdisReleaseSpinLock(&pMDIOBus->MDIOBus_BusSpinLock);                                              // Release MDIO bus spin lock.
        }                                                                                                     
    } while (1);                                                                                              
    KeCancelTimer(&pMDIOBus->MDIOBus_Timer);
    DBG_MDIO_BUS_METHOD_END_WITH_STATUS(Status);
    PsTerminateSystemThread(STATUS_SUCCESS);
}

/*++
Routine Description:
    Starts MDIO bus controller main thread.
Arguments:
    pMDIOBus - MDIO bus data structure address.
Return Value:
    See PsCreateSystemThread() return values.
--*/
NTSTATUS MDIOBus_Start(_In_ MP_MDIO_BUS *pMDIOBus)
{
    NTSTATUS              ntStatus;
    HANDLE                hThread;
    OBJECT_ATTRIBUTES     oa;

    DBG_MDIO_BUS_METHOD_BEG();
    do {
        KeInitializeEvent((PVOID)&pMDIOBus->MDIOBus_KillEvent, NotificationEvent, FALSE);
        KeInitializeEvent((PVOID)&pMDIOBus->MDIOBus_StartTransferEvent, SynchronizationEvent, FALSE);
        InitializeObjectAttributes(&oa, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
        if (!NT_SUCCESS(ntStatus = PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, &oa, NULL, NULL, (PKSTART_ROUTINE)MDIOBus_Main, pMDIOBus))) {
            break;
        }
        ObReferenceObjectByHandle(hThread, THREAD_ALL_ACCESS, NULL, KernelMode, (PVOID*)&pMDIOBus->MDIOBus_ThreadHandle, NULL);
        ZwClose(hThread);
    } while (0);
    DBG_MDIO_BUS_METHOD_END();
    return ntStatus;
}

/*++
Routine Description:
    Stops MDIO bus controller main thread.
Arguments:
    pMDIOBus - MDIO bus data structure address.
Return Value:
    none
--*/
VOID MDIOBus_Stop(_In_ MP_MDIO_BUS *pMDIOBus)
{
    DBG_MDIO_BUS_METHOD_BEG();
    if (pMDIOBus->MDIOBus_ThreadHandle) {
        DBG_MDIO_BUS_PRINT_INFO("Setting Exit event");
        KeSetEvent(&pMDIOBus->MDIOBus_KillEvent, 0, FALSE);
        DBG_MDIO_BUS_PRINT_INFO("Waiting for MDIOBus_Main() exit");
        KeWaitForSingleObject(pMDIOBus->MDIOBus_ThreadHandle, Executive, KernelMode, FALSE, NULL);
        ObDereferenceObject(pMDIOBus->MDIOBus_ThreadHandle);
    }
    DBG_MDIO_BUS_METHOD_END();
}

/*++
Routine Description:
    Attached Enet PHY device to the existing MDIO bus.
Arguments:
    pMDIOBus - MDIO bus data structure address.
    pMDIODev - MDIO Enet PHY device data structure address.
Return Value:
    NDIS_ERROR_CODE_RESOURCE_CONFLICT or STATUS_SUCCESS
--*/
NTSTATUS MDIOBus_AddDevice(_In_ MP_MDIO_BUS *pMDIOBus, _In_ MP_MDIO_DEVICE *pMDIODev)
{
    NTSTATUS        Status = STATUS_SUCCESS;
    MP_MDIO_DEVICE *ptmpMDIODev;

    DBG_MDIO_DEV_METHOD_BEG();
    NdisAcquireSpinLock(&pMDIOBus->MDIOBus_BusSpinLock);                                                         // Acquire MDIO bus spin lock.
    Dbg_PrintMDIOBusInfo(pMDIOBus, "  ");
    ptmpMDIODev = pMDIOBus->MDIOBus_pFirstDevice;
    while (ptmpMDIODev) {
        if (ptmpMDIODev->MDIODev_PhyDevice_Address == pMDIODev->MDIODev_PhyDevice_Address) {
            Status = NDIS_STATUS_RESOURCE_CONFLICT;
            DBG_MDIO_DEV_PRINT_ERROR_WITH_STATUS_AND_PARAMS("ENET PHY was not added to the MDIO bus.", "ENET PHY address %d already used by other device.", pMDIODev->MDIODev_PhyDevice_Address);
            break;
        }
        ptmpMDIODev = ptmpMDIODev->MDIODev_pNextDevice;
    }
    if (!ptmpMDIODev) {
        pMDIODev->MDIODev_pNextDevice = pMDIOBus->MDIOBus_pFirstDevice;                                          // Add MDIO device to the begin of device queue.
        pMDIOBus->MDIOBus_pFirstDevice = pMDIODev;
        Dbg_PrintMDIOBusInfo(pMDIOBus, "  ");
    };
    NdisReleaseSpinLock(&pMDIOBus->MDIOBus_BusSpinLock);                                                         // Release MDIO bus spin lock.
    DBG_MDIO_DEV_METHOD_END_WITH_STATUS(Status);
    return(Status);
}

/*++
Routine Description:
    Deattached Enet PHY device to the existing MDIO bus.
Arguments:
    pMDIOBus - MDIO bus data structure address.
    pMDIODev - MDIO Enet PHY device data structure address.
Return Value:
    none
--*/
void MDIOBus_RemoveDevice(_In_ MP_MDIO_BUS *pMDIOBus, _In_ MP_MDIO_DEVICE *pMDIODev)
{
    DBG_MDIO_DEV_METHOD_BEG();
    NdisAcquireSpinLock(&pMDIOBus->MDIOBus_BusSpinLock);                                                         // Acquire MDIO bus spin lock.
    Dbg_PrintMDIOBusInfo(pMDIOBus, "  ");
    MDIO_RemoveFromList((PVOID *)&pMDIOBus->MDIOBus_pFirstDevice, pMDIODev);                                     // Remove MDIO device from device queue.
    if (pMDIOBus->MDIOBus_pActiveDevice == pMDIODev) {
        pMDIOBus->MDIOBus_pActiveDevice = NULL;
        pMDIOBus->MDIOBus_TransferInProgress = FALSE;
        KeSetEvent(&pMDIOBus->MDIOBus_StartTransferEvent, 0, FALSE);                                             // Restart MDIO bus.
    }
    Dbg_PrintMDIOBusInfo(pMDIOBus, "  ");
    pMDIODev->MDIODev_pBus = NULL;
    NdisReleaseSpinLock(&pMDIOBus->MDIOBus_BusSpinLock);                                                         // Release MDIO bus spin lock.
    DBG_MDIO_DEV_METHOD_END();
}

/*++
Routine Description:
    Returns handle to the MDIO bus. (If MDIO bus doesnt exist it cretes and starts new MDIO bus instance).
Arguments:
    pMDIODrv    - MDIO driver data structure address.
    ppMDIOBus   - Address of MDIO bus data structure pointer.
    MDIORegBase - MDIO controller physical address.
Return Value:
    STATUS_SUCCESS or STATUS_INSUFFICIENT_RESOURCES
--*/
NTSTATUS MDIOBus_Open(_In_ MP_MDIO_DRIVER *pMDIODrv, _Out_ MP_MDIO_BUS **ppMDIOBus, _In_ NDIS_PHYSICAL_ADDRESS MDIORegBase)
{
    MP_MDIO_BUS* pMDIOBus = NULL;
    NTSTATUS     Status = STATUS_SUCCESS;

    DBG_MDIO_BUS_METHOD_BEG_WITH_PARAMS("MDIORegBase: 0x%08x_%08x", NdisGetPhysicalAddressHigh(MDIORegBase), NdisGetPhysicalAddressLow(MDIORegBase));
    do {
        // Find existing or create new MDIO bus instance
        NdisAcquireSpinLock(&pMDIODrv->MDIODrv_DriverSpinLock);                                                  // Acquire MDIO driver spin lock.
        pMDIOBus = pMDIODrv->MDIODrv_pFirstBus;                                                                  // Get list head.
        while (pMDIOBus != NULL) {                                                                               // Try to find MDIO bus controller .
            if (pMDIOBus->MDIOBus_RegsPhyAddress.QuadPart == MDIORegBase.QuadPart) {                             // MDIO bus controller already initialized?
                DBG_MDIO_BUS_PRINT_INFO("MDIO bus found.");
                NdisInterlockedIncrement(&pMDIOBus->MDIOBus_ReferenceCount);                                     // Increment reference counter.
                break;
            } else {
                pMDIOBus = pMDIOBus->MDIOBus_pNextBus;                                                           // Get next MDIO bus controller.
            }
        }
        NdisReleaseSpinLock(&pMDIODrv->MDIODrv_DriverSpinLock);                                                  // Release MDIO driver spin lock.
        if (pMDIOBus == NULL) {                                                                                  // MDIO bus controller not found? Create new instance of MDIO bus controller.
            if ((pMDIOBus = (PMP_MDIO_BUS)ExAllocatePoolWithTag(NonPagedPoolNx, sizeof(MP_MDIO_BUS), MP_TAG_MDIO_DRV)) == NULL) {
                Status = STATUS_INSUFFICIENT_RESOURCES;
                DBG_MDIO_BUS_PRINT_ERROR_WITH_STATUS("ExAllocatePoolWithTag() failed.");
                break;
            }
            NdisZeroMemory(pMDIOBus, sizeof(MP_MDIO_BUS));                                                       // Clean up the memory block.
            #if DBG
            RtlStringCbPrintfA(pMDIOBus->MDIOBus_DeviceName, sizeof(pMDIOBus->MDIOBus_DeviceName) - 1, "MDIO_%d", pMDIODrv->MDIODrv_BusCounter++);
            #endif
            if ((pMDIOBus->MDIOBus_pRegBase = (CSP_ENET_REGS *)MmMapIoSpaceEx(MDIORegBase, 1024, PAGE_READWRITE | PAGE_NOCACHE)) == NULL) {
                Status = STATUS_INSUFFICIENT_RESOURCES;                                                          // Set error code.
                DBG_MDIO_BUS_PRINT_ERROR_WITH_STATUS("MmMapIoSpaceEx(MDIOPhyBase) failed.");
                ExFreePoolWithTag(pMDIOBus, MP_TAG_MDIO_DRV);                                                    // Release memory.
                break;                                                                                           // Stop initialization.
            }
            pMDIOBus->MDIOBus_ReferenceCount = 1;                                                                // Set reference counter.
            NdisAllocateSpinLock(&pMDIOBus->MDIOBus_BusSpinLock);                                                // Initialize MDIO bus spin lock.
            pMDIOBus->MDIOBus_pMDIODriver = pMDIODrv;                                                            // Remember MDIO driver context structure address.
            pMDIOBus->MDIOBus_RegsPhyAddress = MDIORegBase;                                                      // Remember MDIO bus controller physical address.
            if ((Status = MDIOBus_Start(pMDIOBus)) != STATUS_SUCCESS) {                                          // Start MDIO bus controller thread.
                MmUnmapIoSpace((PVOID)pMDIOBus->MDIOBus_pRegBase, 1024);                                         // Unmap MDIO peripheral register.
                ExFreePoolWithTag(pMDIOBus, MP_TAG_MDIO_DRV);                                                    // Free MDIO bus device structure.
                pMDIOBus = NULL;
                break;                                                                                           // Stop initialization.
            }
            NdisAcquireSpinLock(&pMDIODrv->MDIODrv_DriverSpinLock);                                              // Acquire MDIO driver spin lock.
            pMDIOBus->MDIOBus_pNextBus = pMDIODrv->MDIODrv_pFirstBus;                                            // Add MDIO bus controller to the begin of the MDIO bus controller list.
            pMDIODrv->MDIODrv_pFirstBus = pMDIOBus;
            NdisReleaseSpinLock(&pMDIODrv->MDIODrv_DriverSpinLock);                                              // Release MDIO driver spin lock.
            DBG_MDIO_BUS_PRINT_INFO("New MDIO bus created.");
        }
    } while (0);
    *ppMDIOBus = pMDIOBus;
    DBG_MDIO_BUS_METHOD_END_WITH_STATUS(Status);
    return Status;
}

/*++
Routine Description:
    Closes handle to the MDIO bus. (If last hanlde is closed MDIO bus instance is removed).
Arguments:
    pMDIODrv    - MDIO driver data structure address.
    ppMDIOBus   - Address of MDIO bus data structure pointer.
Return Value:
    none
--*/
void MDIOBus_Close(_In_ MP_MDIO_BUS *pMDIOBus)
{
    MP_MDIO_DRIVER *pMDIODrv = pMDIOBus->MDIOBus_pMDIODriver;
    LONG            ReferenceCount;

    DBG_MDIO_BUS_METHOD_BEG();
    ReferenceCount = NdisInterlockedDecrement(&pMDIOBus->MDIOBus_ReferenceCount);                // Decrement MDIO bus reference counter.
    if (ReferenceCount == 0) {                                                                   // Last reference closed?
        DBG_MDIO_BUS_PRINT_INFO("Bus reference count = 0, Removing MDIO bus.");
        MDIOBus_Stop(pMDIOBus);                                                                  // Stop MDIO bus thread.
        NdisAcquireSpinLock(&pMDIODrv->MDIODrv_DriverSpinLock);                                  // Acquire MDIO driver spin lock.
        MDIO_RemoveFromList((PVOID *)&pMDIODrv->MDIODrv_pFirstBus, pMDIOBus);                    // Remove MDIO bus from list.
        NdisReleaseSpinLock(&pMDIODrv->MDIODrv_DriverSpinLock);                                  // Release MDIO driver spin lock.
        MmUnmapIoSpace((PVOID)pMDIOBus->MDIOBus_pRegBase, 1024);                                 // Unmap MDIO peripheral register.
        DBG_MDIO_BUS_METHOD_END();
        ExFreePoolWithTag(pMDIOBus, MP_TAG_MDIO_DRV);                                            // Free MDIO bus device structure.
    } else {                                                                                     
        DBG_MDIO_BUS_PRINT_INFO("Bus reference count = %d", ReferenceCount);                     
        DBG_MDIO_BUS_METHOD_END();
    }
}

/*++
Routine Description:
    Initialises MDIO device and attaches it to the MDIO bus.
    As soon as this method returns the MDIODev_QueueCommand() method can be used to queue sequences of MII reads and MII writes.
    If MDIO bus device doesnt exixts it is created together with system worker thread. This thread periodicaly tests MII transfer done flags
    and calls callback (if requsted by the command) and starts new transfer (if there is some in the queue).

Arguments:
    pMDIODrv       - Pointer to the MDIO driver data structure.
    pMDIODevCfg    - Pointer to the MDIO device configuration structure.
    pMDIODev       - Pointer to the MDIO device data structure.
Return Value:
    STATUS_SUCCESS, STATUS_INSUFFICIENT_RESOURCES or NDIS_STATUS_FAILURE
--*/
_Use_decl_annotations_
NTSTATUS MDIODev_InitDevice(MP_MDIO_DRIVER *pMDIODrv, MP_MDIO_DEVICE_CFG *pMDIODevCfg, MP_MDIO_DEVICE *pMDIODev) {
    NTSTATUS Status = STATUS_SUCCESS;
    ULONG MDIODev_PhyDevice_MDIOBusClock_kHz, MDIODev_PhyDevice_STAHoldTime_ns, i, MII_Div, MII_Hold, tmpTime;

    DBG_MDIO_DEV_METHOD_BEG_WITH_PARAMS("MDIO bus controller physical address: 0x%08X_%08X, Phy: %d", NdisGetPhysicalAddressHigh(pMDIODevCfg->MDIODevCfg_MDIOBusController_PhyAddress), NdisGetPhysicalAddressLow(pMDIODevCfg->MDIODevCfg_MDIOBusController_PhyAddress), pMDIODevCfg->MDIODevCfg_PhyDevice_Address);
    do {
        NdisAllocateSpinLock(&pMDIODev->MDIODev_DeviceSpinLock);                                                 // Initialize device spin lock.
        // Initilaize MDIO frame lists.
        pMDIODev->MDIODev_pPendingFrameListHead = NULL;                                                          // Initialize pending frame list head pointer.
        pMDIODev->MDIODev_pPendingFrameListTail = NULL;                                                          // Initialize pending frame list tail pointer.
        pMDIODev->MDIODev_pFreeFrameListHead = &pMDIODev->MDIODev_FrameListArray[0];                             // Initialize free frame list head pointer.
        for (i = 0; i < MDIO_FRAME_LIST_SIZE - 1; i++) {                                                         // Initialize free frame list.
            pMDIODev->MDIODev_FrameListArray[i].MDIOFrm_pNextFrame = &pMDIODev->MDIODev_FrameListArray[i + 1];
        }
        pMDIODev->MDIODev_FrameListArray[MDIO_FRAME_LIST_SIZE - 1].MDIOFrm_pNextFrame = NULL;                          // Initialize the last item of free frame list.
        pMDIODev->MDIODev_PhyDevice_Address = BIT_FIELD_VAL(ENET_MMFR_PA, pMDIODevCfg->MDIODevCfg_PhyDevice_Address);  // Remember ENET PHY device address.
        pMDIODev->MDIODev_PhyDevice_InterfaceType = pMDIODevCfg->MDIODevCfg_PhyDevice_InterfaceType;                   // Remember ENET PHY Interface type
        // Compute MII speed divider.
        for (MII_Div = 0; MII_Div <= 63; MII_Div++) {
            MDIODev_PhyDevice_MDIOBusClock_kHz = pMDIODevCfg->MDIODevCfg_MDIOBusController_InputClk_kHz /((MII_Div +1)<<1);
            if (MDIODev_PhyDevice_MDIOBusClock_kHz <= pMDIODevCfg->MDIODevCfg_PhyDevice_MaxMDIOBusClock_kHz) {
                break;
            }
        }
        if (MDIODev_PhyDevice_MDIOBusClock_kHz > pMDIODevCfg->MDIODevCfg_PhyDevice_MaxMDIOBusClock_kHz) {
            Status = NDIS_STATUS_FAILURE;
            DBG_MDIO_DEV_PRINT_ERROR_WITH_STATUS_AND_PARAMS("MDIO clock frequecny is out of range.", "Computed frequency is %d kHz . It must be <= %d kHz.", MDIODev_PhyDevice_MDIOBusClock_kHz, pMDIODevCfg->MDIODevCfg_PhyDevice_MaxMDIOBusClock_kHz);
            break;
        }
        // Compute hold time
        tmpTime = 1000000/pMDIODevCfg->MDIODevCfg_MDIOBusController_InputClk_kHz;
        MDIODev_PhyDevice_STAHoldTime_ns = tmpTime;
        for (MII_Hold = 1; MII_Hold <= 8; MII_Hold++) {
            if (MDIODev_PhyDevice_STAHoldTime_ns >= pMDIODevCfg->MDIODevCfg_PhyDevice_MinSTAHoldTime_ns) {
                break;
            }
            MDIODev_PhyDevice_STAHoldTime_ns += tmpTime;
        }
        if (MDIODev_PhyDevice_STAHoldTime_ns < pMDIODevCfg->MDIODevCfg_PhyDevice_MinSTAHoldTime_ns) {
            Status = NDIS_STATUS_FAILURE;
            DBG_MDIO_DEV_PRINT_ERROR_WITH_STATUS_AND_PARAMS("MDIO hold time is out of range.", "Computed MDIO hold time %d ns is lower then required hold time %d ns.", MDIODev_PhyDevice_STAHoldTime_ns, pMDIODevCfg->MDIODevCfg_PhyDevice_MinSTAHoldTime_ns);
            break;
        }
        pMDIODev->MDIODev_MSCR.U = BIT_FIELD_VAL(ENET_MSCR_MII_SPEED, MII_Div) | BIT_FIELD_VAL(ENET_MSCR_HOLDTIME, MII_Hold) | BIT_FIELD_VAL(ENET_MSCR_DIS_PRE, pMDIODevCfg->MDIODevCfg_PhyDevice_DisableFramePreamble? 1:0);
        DBG_MDIO_DEV_PRINT_INFO("MDIO clock frequency: %d kHz {%d kHz / ((%d + 1) * 2)}, Hold Time: %d ns {%d * %d ns }, Preamble %s.", MDIODev_PhyDevice_MDIOBusClock_kHz, pMDIODevCfg->MDIODevCfg_MDIOBusController_InputClk_kHz, MII_Div, MDIODev_PhyDevice_STAHoldTime_ns, MII_Hold, tmpTime, pMDIODevCfg->MDIODevCfg_PhyDevice_DisableFramePreamble? "Enabled":"Disabled");
        DBG_CODE(pMDIODev->MDIODev_PhyDevice_MDIOBusClock_kHz     = MDIODev_PhyDevice_MDIOBusClock_kHz);
        DBG_CODE(pMDIODev->MDIODev_PhyDevice_STAHoldTime_ns       = MDIODev_PhyDevice_STAHoldTime_ns);
        DBG_CODE(pMDIODev->MDIODev_PhyDevice_DisableFramePreamble = pMDIODevCfg->MDIODevCfg_PhyDevice_DisableFramePreamble);        
        pMDIODev->MDIODev_Context = pMDIODevCfg->MDIODevCfg_Context;
        pMDIODev->MDIODev_MIIFrameDoneCallback = pMDIODevCfg->MDIODevCfg_MIIFrameDoneCallback;
        // Get MDIO bus handle.
        if ((Status = MDIOBus_Open(pMDIODrv, &pMDIODev->MDIODev_pBus, pMDIODevCfg->MDIODevCfg_MDIOBusController_PhyAddress)) != STATUS_SUCCESS) {
            DBG_MDIO_DEV_PRINT_ERROR_WITH_STATUS("MDIOBus_Open() failed!");
            break;
        }
        // Attache device to the MDIO bus.
        DBG_CODE(RtlStringCbPrintfA(pMDIODev->MDIODev_DeviceName, sizeof(pMDIODev->MDIODev_DeviceName) - 1, "%s_%d", pMDIODev->MDIODev_pBus->MDIOBus_DeviceName, pMDIODevCfg->MDIODevCfg_PhyDevice_Address));
        if ((Status = MDIOBus_AddDevice(pMDIODev->MDIODev_pBus, pMDIODev)) != STATUS_SUCCESS) {
            DBG_MDIO_DEV_PRINT_ERROR_WITH_STATUS("MDIOBus_AddDevice() failed!");
            MDIOBus_Close(pMDIODev->MDIODev_pBus);
            pMDIODev->MDIODev_pBus = NULL;
            break;
        }
    } while (0);
    DBG_MDIO_DEV_METHOD_END_WITH_STATUS(Status);
    return Status;
}

/*++
Routine Description:
    Unconfigures Enet PHY.
Arguments:
    pMDIODrv       - MDIO driver data structure address.
    pMDIODev       - MDIO Enet PHY device data structure address.
Return Value:
    none
--*/
_Use_decl_annotations_
void MDIODev_DeinitDevice(MP_MDIO_DEVICE *pMDIODev) {
    MP_MDIO_BUS *pMDIOBus = pMDIODev->MDIODev_pBus;

    DBG_MDIO_DEV_METHOD_BEG();
    if (pMDIOBus) {                                                         // Check if MDIO device is initialized
        MDIOBus_RemoveDevice(pMDIOBus, pMDIODev);
        MDIOBus_Close(pMDIOBus);
        pMDIODev->MDIODev_pBus = NULL;   
    }
    DBG_MDIO_DEV_METHOD_END();
    return;
}

/*++
Routine Description:
    This function will call MDIODev_QueueFrame to queue all the requested MII management
    commands to the sending list.
Arguments:
    pAdapter  - Points to a structure which contains the status and control information for the ENET controller and MII 
    pCmd      - Points to a ENET_PHY_CMD array which specifies the MII management commands and the parsing functions
Return Value:
    None
--*/
_Use_decl_annotations_
NTSTATUS MDIODev_QueueCommand(MP_MDIO_DEVICE* pMDIODev, PENET_PHY_CMD pCmd)
{
    NTSTATUS        Status = NDIS_STATUS_SUCCESS;
    MP_MDIO_FRAME*  pMDIOFrame = NULL;
    MP_MDIO_FRAME*  pMDIOCmdFrameListHead;
    MP_MDIO_FRAME*  pMDIOCmdLastFrame = NULL;
    MP_MDIO_BUS*    pMDIOBus = pMDIODev->MDIODev_pBus;
    UINT            i;

    DBG_MDIO_DEV_CMD_METHOD_BEG();
    NdisAcquireSpinLock(&pMDIODev->MDIODev_DeviceSpinLock);                                              // Acquire MDIO device spin lock.
    pMDIOCmdFrameListHead = pMDIODev->MDIODev_pFreeFrameListHead;                                        
    for (i = 0; (pCmd + i)->MIIData != ENET_MII_END; i++) {                                              
        if ((pMDIOFrame = pMDIODev->MDIODev_pFreeFrameListHead) != NULL) {                               // Is there an item in the list?
            pMDIODev->MDIODev_pFreeFrameListHead = pMDIOFrame->MDIOFrm_pNextFrame;                       // Yes, get item and update list head .
            pMDIOFrame->MDIOFrm_MMFRRegVal = (pCmd + i)->MIIData | pMDIODev->MDIODev_PhyDevice_Address;  // Copy requested MMFR register content.
            pMDIOFrame->MIIFunction        = (pCmd + i)->MIIFunct;                                       // Copy callback functio address.
            pMDIOFrame->MDIOFrm_pNextFrame = NULL;                                                       // Suppose no other command.
            if (pMDIOCmdLastFrame != NULL) {                                                             // Add frame to the tail of list.
                pMDIOCmdLastFrame->MDIOFrm_pNextFrame = pMDIOFrame;
            }
            pMDIOCmdLastFrame = pMDIOFrame;                                                              // 
            DBG_MDIO_DEV_CMD_PRINT_TRACE("Frame 0x%p queued", pMDIOFrame);
/*
            DBG_MDIO_DEV_CMD_PRINT_INFO("Frame queued. MMFR: 0x%08X (ST=%d, OP=%s, PHYADR=%d, REG=0x%02X), Callback: %s", pMDIOFrame->MDIOFrm_MMFRRegVal, \
                (pMDIOFrame->MDIOFrm_MMFRRegVal >> 30) & 0x03, \
                (((pMDIOFrame->MDIOFrm_MMFRRegVal >> 28) & 0x03)==1)?"WRITE":((((pMDIOFrame->MDIOFrm_MMFRRegVal >> 28) & 0x03)==2)?"READ":"UNKNOWN"), \
                (pMDIOFrame->MDIOFrm_MMFRRegVal >> 23) & 0x1F, \
                (pMDIOFrame->MDIOFrm_MMFRRegVal >> 18) & 0x1F, \
                Dbg_GetMIICallbackName(pMDIOFrame->MIIFunction));
*/
        }  else {                                                                                        // There are no free items in the list.
            Status = NDIS_STATUS_RESOURCES;                                                              // Stop the loop and return error code.
            DBG_MDIO_DEV_CMD_PRINT_ERROR_WITH_STATUS("Failed to get free Frame descriptor. Increase MDIO_FRAME_LIST_SIZE constant.");
            break;
        }
    }
    if (pMDIOFrame != NULL) {                                                                            // A frame added?
        pMDIODev->MDIODev_CmdPending = TRUE;                                                             
        if (pMDIODev->MDIODev_pPendingFrameListHead == NULL) {                                           // Pending list empty?
            pMDIODev->MDIODev_pPendingFrameListHead = pMDIOCmdFrameListHead;                             // Add frame list as the first item.
            pMDIODev->MDIODev_pPendingFrameListTail = pMDIOFrame;                                        
            KeSetEvent(&pMDIOBus->MDIOBus_StartTransferEvent, 0, FALSE);                                 
        } else {                                                                                         
            pMDIODev->MDIODev_pPendingFrameListTail->MDIOFrm_pNextFrame = pMDIOCmdFrameListHead;         // Append frame to the tail of the list.
            pMDIODev->MDIODev_pPendingFrameListTail = pMDIOFrame;                                        
        }                                                                                                
    }                                                                                                    
    NdisReleaseSpinLock(&pMDIODev->MDIODev_DeviceSpinLock);                                              // Release MDIO device spin lock.
    DBG_MDIO_DEV_CMD_METHOD_END_WITH_STATUS(Status);
    return Status;
}
