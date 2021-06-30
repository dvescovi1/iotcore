/* Copyright (c) Microsoft Corporation.
   Licensed under the MIT License. */

#include "pch.h"
#include <stdio.h>
#include <string>
#include <cfgmgr32.h>

#include <initguid.h>

#include "Public.h"

using namespace Concurrency;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

// attempt to read a memory address
// returns true on success
bool testMemoryAccess(void *ptr, long *value)
{
    bool success = true;
    
    __try {
        *value = *(long *)ptr;
    }
    __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
        EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {
        success = false;
    }
    return success;
}

std::wstring GetInterfacePath(const GUID& InterfaceGuid)
{
    ULONG length;
    CONFIGRET cr = CM_Get_Device_Interface_List_SizeW(
        &length,
        const_cast<GUID*>(&InterfaceGuid),
        nullptr,        // pDeviceID
        CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

    if (cr != CR_SUCCESS) {
        throw wexception::make(
            HRESULT_FROM_WIN32(CM_MapCrToWin32Err(cr, ERROR_NOT_FOUND)),
            L"Failed to get size of device interface list. (cr = 0x%x)",
            cr);
    }

    if (length < 2) {
        throw wexception::make(
            HRESULT_FROM_WIN32(CM_MapCrToWin32Err(cr, ERROR_NOT_FOUND)),
            L"The IMXVPUKM device was not found on this system. (cr = 0x%x)",
            cr);
    }

    std::unique_ptr<WCHAR[]> buf(new WCHAR[length]);
    cr = CM_Get_Device_Interface_ListW(
        const_cast<GUID*>(&InterfaceGuid),
        nullptr,        // pDeviceID
        buf.get(),
        length,
        CM_GET_DEVICE_INTERFACE_LIST_PRESENT);

    if (cr != CR_SUCCESS) {
        throw wexception::make(
            HRESULT_FROM_WIN32(CM_MapCrToWin32Err(cr, ERROR_NOT_FOUND)),
            L"Failed to get device interface list. (cr = 0x%x)",
            cr);
    }

    // Return the first string in the multistring
    return std::wstring(buf.get());
}

FileHandle OpenVpuHandle()
{
    auto interfacePath = GetInterfacePath(GUID_DEVINTERFACE_imxvpukm);

    printf("Opening device %S\n", interfacePath.c_str());

    FileHandle fileHandle(CreateFile(
        interfacePath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,          // dwShareMode
        nullptr,    // lpSecurityAttributes
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr));  // hTemplateFile
    
    if (!fileHandle.IsValid()) {
        if (GetLastError() == ERROR_ACCESS_DENIED) {
            // Try opening read-only
            fileHandle.Attach(CreateFile(
                interfacePath.c_str(),
                GENERIC_READ,
                0,          // dwShareMode
                nullptr,    // lpSecurityAttributes
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr));  // hTemplateFile

            if (fileHandle.IsValid()) {
                return fileHandle;
            }
        }

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"Failed to open a handle to the VPU device. "
            L"(hr = 0x%x, interfacePath = %s)",
            HRESULT_FROM_WIN32(GetLastError()),
            interfacePath.c_str());
    }

    return fileHandle;
}

void test()
{
    auto handle = OpenVpuHandle();
    int val;
    ULONG input;
    ULONG output;
    void *vpuMem = NULL;
    DWORD bytes;

    // VPU_MC_CORES test call
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_MC_CORES,
        nullptr,
        0,
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_MC_CORES failed. "
            L"(hr = 0x%x, bytes = %d)",
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_MC_CORES output = %d\n", output);
    }

    // VPU_CORE_ID with DWL_CLIENT_TYPE_HEVC_DEC (12)
    input = 12;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_CORE_ID,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_CORE_ID(%d) failed. "
            L"(hr = 0x%x, bytes = %d)",
            input, 
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_CORE_ID(%d) coreid = %d\n", input, output);
    }

    // VPU_CORE_ID with DWL_CLIENT_TYPE_VP9_DEC (11)
    input = 11;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_CORE_ID,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_CORE_ID(%d) failed. "
            L"(hr = 0x%x, bytes = %d)",
            input,
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_CORE_ID(%d) coreid = %d\n", input, output);
    }

    // VPU_CORE_ID with DWL_CLIENT_TYPE_H264_DEC (1)
    input = 1;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_CORE_ID,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_CORE_ID(%d) failed. "
            L"(hr = 0x%x, bytes = %d)",
            input,
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_CORE_ID(%d) coreid = %d\n", input, output);
    }

    // VPU_CORE_ID with DWL_CLIENT_TYPE_VP8_DEC (10)
    input = 10;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_CORE_ID,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_CORE_ID(%d) failed. "
            L"(hr = 0x%x, bytes = %d)",
            input,
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_CORE_ID(%d) coreid = %d\n", input, output);
    }

    // VPU_CORE_ID with DWL_CLIENT_TYPE_PP_DEC (4)
    input = 4;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_CORE_ID,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr)) {

        printf("IOCTL_VPU_CORE_ID(%d) correctly failed hr=0x%08x\n", input, HRESULT_FROM_WIN32(GetLastError()));
    }
    else
    {
        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_CORE_ID(%d) unexpectedly succeeded. "
            L"output = %d",
            output);
    }


    // VPU_HW_SIZE test call
    input = 0;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_HW_SIZE,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_HW_SIZE failed. "
            L"(hr = 0x%x, bytes = %d)",
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_HW_SIZE(%d) = %d\n", input, output);
    }

    // VPU_HW_SIZE test call
    input = 1;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_HW_SIZE,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_HW_SIZE failed. "
            L"(hr = 0x%x, bytes = %d)",
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_HW_SIZE(%d) = %d\n", input, output);
    }

    // VPU_HW_SIZE test call
    input = 2;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_HW_SIZE,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        printf("IOCTL_VPU_HW_SIZE(%d) correctly failed hr = 0x%08x\n", input, HRESULT_FROM_WIN32(GetLastError()));

    }
    else
    {
        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_HW_SIZE unexpectedly succeeded. "
            L"(size = %d)",
            output);
    }

    // VPU_HW_ADDR test call - maps memory into process space
    // now disabled
    input = 1;
    if (DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_HW_ADDR,
        &input,
        sizeof(input),
        &vpuMem,
        sizeof(void *),
        &bytes,
        nullptr)) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_HW_ADDR(%d) = %p, [0] = %lx unexpectedly succeeded\n",
            input, vpuMem, *(long *)vpuMem);
    }
    else
    {
        printf("IOCTL_VPU_HW_ADDR(%d) = correctly failed"
            "(hr = 0x%x)\n",
            input,
            HRESULT_FROM_WIN32(GetLastError()));
    }
#if 0
    // perform 2nd call to VPU_HW_ADDR, should return same virtual address.
    void *vpuMem2 = NULL;
    input = 1;
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_HW_ADDR,
        &input,
        sizeof(input),
        &vpuMem2,
        sizeof(void *),
        &bytes,
        nullptr) || (bytes != sizeof(void *))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_HW_ADDR failed. "
            L"(hr = 0x%x, bytes = %d)",
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else if (vpuMem != vpuMem2)
    {
        throw wexception::make(
            E_FAIL,
            L"IOCTL_VPU_HW_ADDR returned different pointer from 1st call %p, first call %p",
            vpuMem2,
            vpuMem);
    }
    else
    {
        printf("IOCTL_VPU_HW_ADDR(%d) = %p, [0] = %lx\n", input, vpuMem2, *(long *)vpuMem2);
    }
#endif
    // VPU_HW_ADDR test call - maps memory into process space
    void *vpuMem3 = NULL;
    input = 0;
    if (DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_HW_ADDR,
        &input,
        sizeof(input),
        &vpuMem3,
        sizeof(void *),
        &bytes,
        nullptr)) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_HW_ADDR(%d) = %p, [0] = %lx unexpectedly succeeded\n",
            input, vpuMem3, *(long *)vpuMem3);
    }
    else
    {
        printf("IOCTL_VPU_HW_ADDR(%d) = correctly failed"
            "(hr = 0x%x)\n",
            input,
            HRESULT_FROM_WIN32(GetLastError()));
    }

    // test VPU_ALLOC_MEM   VPU_MEM_UNCACHED, VPU_MEM_WRITE_COMBINE, VPU_MEM_CACHED
    IMXVPU_MEMORY buffers[VPU_MEM_CACHED + 1];
    for (int i = VPU_MEM_UNCACHED; i <= VPU_MEM_CACHED; i++)
    {
        IMXVPU_MEMORY &block = buffers[i];
        memset(&block, 0, sizeof(block));
        block.size = 8192;
        block.flags = i;
        void *virtAddress = NULL;

        if (!DeviceIoControl(
            handle.Get(),
            IOCTL_VPU_ALLOC_MEM,
            &block,
            sizeof(block),
            &block,
            sizeof(block),
            &bytes,
            nullptr) || (bytes != sizeof(block))) {

            throw wexception::make(
                HRESULT_FROM_WIN32(GetLastError()),
                L"IOCTL_VPU_ALLOC_MEM type %d failed. "
                L"(hr = 0x%x, bytes = %d)",
                block.flags,
                HRESULT_FROM_WIN32(GetLastError()),
                bytes);
        }
        else
        {
            long value = 0;

            if (testMemoryAccess(block.virtAddress, &value))
            {
                printf("IOCTL_VPU_ALLOC_MEM(%d, %d) VirtAddres = %p, physAddress = %llx, value=%d\n", block.size, block.flags, block.virtAddress, block.physAddress, value);
            }
            else
            {
                throw wexception::make(
                    E_FAIL,
                    L"IOCTL_VPU_ALLOC_MEM type %d succeeded but memory access failed. "
                    L"VirtAddres = %p, physAddress = %llx", block.flags, block.virtAddress, block.physAddress);
            }

            bytes = 0;
            if (!DeviceIoControl(
                handle.Get(),
                IOCTL_VPU_CACHE_FLUSH,
                &block.virtAddress,
                sizeof(void *),
                nullptr,
                0,
                &bytes,
                nullptr) || (bytes != 0)) {

                throw wexception::make(
                    HRESULT_FROM_WIN32(GetLastError()),
                    L"IOCTL_VPU_CACHE_FLUSH type %d failed. "
                    L"(hr = 0x%x, bytes = %d)",
                    block.flags,
                    HRESULT_FROM_WIN32(GetLastError()),
                    bytes);
            }

            bytes = 0;
            if (!DeviceIoControl(
                handle.Get(),
                IOCTL_VPU_CACHE_INVALIDATE,
                &block.virtAddress,
                sizeof(void *),
                nullptr,
                0,
                &bytes,
                nullptr) || (bytes != 0)) {

                throw wexception::make(
                    HRESULT_FROM_WIN32(GetLastError()),
                    L"IOCTL_VPU_CACHE_INVALIDATE type %d failed. "
                    L"(hr = 0x%x, bytes = %d)",
                    block.flags,
                    HRESULT_FROM_WIN32(GetLastError()),
                    bytes);
            }
        }
    }

    // test CACHE operations with invalid address
    void * testBlock = (void *)((BYTE *)buffers[1].virtAddress + sizeof(ULONG));
    bytes = 0;

    if (DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_CACHE_INVALIDATE,
        &testBlock,
        sizeof(testBlock),
        nullptr,
        0,
        &bytes,
        nullptr) || (bytes != 0)) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_CACHE_INVALIDATE failed on invalid virtual address. "
            L"(hr = 0x%x, bytes = %d)",
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }

    // test VPU_FREE_MEM
    for (int i = VPU_MEM_UNCACHED; i <= VPU_MEM_CACHED; i++)
    {
        IMXVPU_MEMORY &block = buffers[i];
        void *virtAddress = block.virtAddress;

        if (!DeviceIoControl(
            handle.Get(),
            IOCTL_VPU_FREE_MEM,
            &block,
            sizeof(block),
            nullptr,
            0,
            &bytes,
            nullptr) || (bytes != 0)) {

            throw wexception::make(
                HRESULT_FROM_WIN32(GetLastError()),
                L"IOCTL_VPU_FREE_MEM type=%d failed. "
                L"(hr = 0x%x, bytes = %d)",
                i,
                HRESULT_FROM_WIN32(GetLastError()),
                bytes);
        }
        else
        {
            long value = 0;

            if (!testMemoryAccess(virtAddress, &value))
            {
                printf("IOCTL_VPU_FREE_MEM type=%d - block freed\n", i);
            }
            else
            {
                throw wexception::make(
                    E_FAIL,
                    L"IOCTL_VPU_FREE_MEM type %d succeeded but memory still accessible. "
                    L"VirtAddress = %p, value = %d", i, virtAddress, value);
            }
        }
    }

    // VPU_PULL_REG test call
    IMXVPU_REGISTERS regs = { 0 };

    regs.coreid = 1;
    regs.regSize = 64 * sizeof(ULONG);
    
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_DEC_PULL_REG,
        &regs,
        sizeof(ULONG) * 2,
        &regs,
        sizeof(ULONG) * (2 + 64),
        &bytes,
        nullptr) || (bytes != (sizeof(ULONG) * (64 + 2)))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_DEC_PULL_REG failed. "
            L"(hr = 0x%x, bytes = %d)",
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_DEC_PULL_REG PullRegs - core = %d, size = %d, [0] = %lx\n", regs.coreid, regs.regSize, regs.regs[0]);
    }

    // VPU_PUSH_REG test call
    if (!DeviceIoControl(
        handle.Get(),
        IOCTL_VPU_DEC_PUSH_REG,
        &regs,
        sizeof(ULONG) * (2 + 64),
        nullptr,
        0,
        &bytes,
        nullptr) || (bytes != 0)) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_DEC_PUSH_REG failed. "
            L"(hr = 0x%x, bytes = %d)",
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }
    else
    {
        printf("IOCTL_VPU_DEC_PUSH_REG Push reg succeeded\n");
    }

    // purposefully leak a memory block and close vpu handle
    {
        IMXVPU_MEMORY block = { 0 };
        block.size = 8192;
        block.flags = VPU_MEM_CACHED;

        if (!DeviceIoControl(
            handle.Get(),
            IOCTL_VPU_ALLOC_MEM,
            &block,
            sizeof(block),
            &block,
            sizeof(block),
            &bytes,
            nullptr) || (bytes != sizeof(block))) {

            throw wexception::make(
                HRESULT_FROM_WIN32(GetLastError()),
                L"IOCTL_VPU_ALLOC_MEM failed. "
                L"(hr = 0x%x, bytes = %d)",
                HRESULT_FROM_WIN32(GetLastError()),
                bytes);
        }
        else
        {
            printf("IOCTL_VPU_ALLOC_MEM(%d) VirtAddres = %p, physAddress = %llx\n", block.size, block.virtAddress, block.physAddress);
        }

        //
        // Close File handle, all mapped memory should be unmapped.
        //
        handle.Close();

        // test VPU memory block unmapped
        bool failed;
        long value = 0x31415928;

        failed = testMemoryAccess(vpuMem, &value);
        if (failed)
        {
            throw wexception::make(
                E_FAIL,
                L"VPU hw mem hasn't been unmapped IOCTL_VPU_HW_ADDR = %p, [0] = %lx\n",
                vpuMem,
                value);
        }
        else
        {
            printf("VPU hw mem has been unmapped IOCTL_VPU_HW_ADDR = %p\n", vpuMem);
        }

        // test memory block
        value = 0x31415928;
        failed = testMemoryAccess(block.virtAddress, &value);
        if (failed)
        {
            throw wexception::make(
                E_FAIL,
                L"VPU buffer hasn't been unmapped IOCTL_VPU_ALLOC_MEM = %p, [0] = %lx\n",
                block.virtAddress,
                value);
        }
        else
        {
            printf("VPU buffer has been unmapped IOCTL_VPU_HW_ADDR = %p\n", block.virtAddress);
        }
    }

    // allocate new vpu block
    // purposefully leak a memory block and exit process
    {
        handle = OpenVpuHandle();

        IMXVPU_MEMORY block = { 0 };
        block.size = 8192;
        block.flags = VPU_MEM_CACHED;

        if (!DeviceIoControl(
            handle.Get(),
            IOCTL_VPU_ALLOC_MEM,
            &block,
            sizeof(block),
            &block,
            sizeof(block),
            &bytes,
            nullptr) || (bytes != sizeof(block))) {

            throw wexception::make(
                HRESULT_FROM_WIN32(GetLastError()),
                L"IOCTL_VPU_ALLOC_MEM failed. "
                L"(hr = 0x%x, bytes = %d)",
                HRESULT_FROM_WIN32(GetLastError()),
                bytes);
        }

        // test memory block
        long value = 0x31415928;
        if (testMemoryAccess(block.virtAddress, &value))
        {
            printf("IOCTL_VPU_ALLOC_MEM(%d, %d) VirtAddres = %p, physAddress = %llx, value=%d\n", block.size, block.flags, block.virtAddress, block.physAddress, value);
        }
        else
        {
            throw wexception::make(
                E_FAIL,
                L"IOCTL_VPU_ALLOC_MEM type %d succeeded but memory access failed. "
                L"VirtAddres = %p, physAddress = %llx", block.flags, block.virtAddress, block.physAddress);
        }

        // exit program, check in KD that file is closed and memory released
        // *(long *)0x0 = 0;
    }

    printf("All tests passed\n");
}

void TestReserveRelease()
{
    auto handle1 = OpenVpuHandle();
    auto handle2 = OpenVpuHandle();

    auto vpu1 = handle1.Get();
    auto vpu2 = handle2.Get();
    event doneEvent;

    ULONG input;
    ULONG output;
    ULONG bytes;
    task_group tasks;
    BOOL failed = FALSE;

    printf("Testing Reserve/Release\n");

    printf("Reserving H264 decoder\n");

    // Reserve the H264 decoder
    input = DWL_CLIENT_TYPE_H264_DEC;
    if (!DeviceIoControl(
        vpu1,
        IOCTL_VPU_DEC_RESERVE,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_DEC_RESERVE(%d) failed. "
            L"(hr = 0x%x, bytes = %d)",
            input,
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }

    printf("Running task to also reserve H264 decoder\n");

    // schedule a task to reserve the VPU
    tasks.run([&] {
        try
        {
            auto vpu2 = handle2.Get();

            ULONG in, out, b;
            in = DWL_CLIENT_TYPE_H264_DEC;

            printf("subtask: Reserving H264 decoder\n");

            if (!DeviceIoControl(
                vpu2,
                IOCTL_VPU_DEC_RESERVE,
                &in,
                sizeof(in),
                &out,
                sizeof(out),
                &b,
                nullptr) || (b != sizeof(out))) {

                failed = TRUE;
                throw wexception::make(
                    HRESULT_FROM_WIN32(GetLastError()),
                    L"IOCTL_VPU_DEC_RESERVE(%d) failed. "
                    L"(hr = 0x%x, bytes = %d)",
                    input,
                    HRESULT_FROM_WIN32(GetLastError()),
                    bytes);
            }

            printf("subtask: H264 decoder reserved\n");

            doneEvent.set();

            printf("subtask: releasing H264 decoder\n");

            if (!DeviceIoControl(
                vpu2,
                IOCTL_VPU_DEC_RELEASE,
                &out,
                sizeof(out),
                nullptr,
                0,
                &b,
                nullptr) || (b != 0)) {

                failed = TRUE;
                throw wexception::make(
                    HRESULT_FROM_WIN32(GetLastError()),
                    L"IOCTL_VPU_DEC_RELEASE(%d) failed. "
                    L"(hr = 0x%x, bytes = %d)",
                    input,
                    HRESULT_FROM_WIN32(GetLastError()),
                    bytes);
            }

            printf("subtask: H264 decoder released\n");
        }
        catch (wexception &err)
        {
            printf("subtask: execution failed HR=%08x\n"
                "%S\n",
                err.HResult(),
                err.wwhat());
        }
    });

    // wait 20 ms
    if (doneEvent.wait(20) == 0)
    {
        failed = TRUE;

        throw wexception::make(
            E_FAIL,
            L"done event is signaled before VPU released");
    }
    else
    {
        printf("Subtask is correctly blocked on IOCT_VPU_DEC_RESERVE\n");
    }

    printf("Releasing core %d\n", output);

    // release the VPU
    if (!DeviceIoControl(
        vpu1,
        IOCTL_VPU_DEC_RELEASE,
        &output,
        sizeof(output),
        nullptr,
        0,
        &bytes,
        nullptr) || (bytes != 0)) {

        failed = TRUE;
        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_DEC_RELEASE(%d) failed. "
            L"(hr = 0x%x, bytes = %d)",
            input,
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }

    // wait upto 20 ms, but should return immediately
    if (doneEvent.wait(20) != 0)
    {
        failed = TRUE;

        throw wexception::make(
            E_FAIL,
            L"done event was not signaled after VPU released");
    }
    else
    {
        printf("Subtask correctly signaled done event\n");
    }

    tasks.wait();
    printf("Finished TestReserveRelease\n");
}


void TestReserveFileClose()
{
    auto handle1 = OpenVpuHandle();
    auto handle2 = OpenVpuHandle();

    auto vpu1 = handle1.Get();
    auto vpu2 = handle2.Get();
    event doneEvent;

    ULONG input;
    ULONG output;
    ULONG bytes;
    task_group tasks;
    BOOL failed = FALSE;

    printf("Testing Reserve/FileClose\n");

    printf("Reserving H265 decoder\n");

    // Reserve the H264 decoder
    input = DWL_CLIENT_TYPE_HEVC_DEC;
    if (!DeviceIoControl(
        vpu1,
        IOCTL_VPU_DEC_RESERVE,
        &input,
        sizeof(input),
        &output,
        sizeof(output),
        &bytes,
        nullptr) || (bytes != sizeof(output))) {

        throw wexception::make(
            HRESULT_FROM_WIN32(GetLastError()),
            L"IOCTL_VPU_DEC_RESERVE(%d) failed. "
            L"(hr = 0x%x, bytes = %d)",
            input,
            HRESULT_FROM_WIN32(GetLastError()),
            bytes);
    }

    printf("Running task to also reserve H264 decoder\n");

    // schedule a task to reserve the VPU
    tasks.run([&] {
        try
        {
            auto vpu2 = handle2.Get();

            ULONG in, out, b;
            in = DWL_CLIENT_TYPE_HEVC_DEC;

            printf("subtask: Reserving H265 decoder\n");

            if (!DeviceIoControl(
                vpu2,
                IOCTL_VPU_DEC_RESERVE,
                &in,
                sizeof(in),
                &out,
                sizeof(out),
                &b,
                nullptr) || (b != sizeof(out))) {

                failed = TRUE;
                throw wexception::make(
                    HRESULT_FROM_WIN32(GetLastError()),
                    L"IOCTL_VPU_DEC_RESERVE(%d) failed. "
                    L"(hr = 0x%x, bytes = %d)",
                    input,
                    HRESULT_FROM_WIN32(GetLastError()),
                    bytes);
            }

            printf("subtask: H265 decoder reserved\n");

            doneEvent.set();

            printf("subtask: releasing H265 decoder\n");

            if (!DeviceIoControl(
                vpu2,
                IOCTL_VPU_DEC_RELEASE,
                &out,
                sizeof(out),
                nullptr,
                0,
                &b,
                nullptr) || (b != 0)) {

                failed = TRUE;
                throw wexception::make(
                    HRESULT_FROM_WIN32(GetLastError()),
                    L"IOCTL_VPU_DEC_RELEASE(%d) failed. "
                    L"(hr = 0x%x, bytes = %d)",
                    input,
                    HRESULT_FROM_WIN32(GetLastError()),
                    bytes);
            }

            printf("subtask: H265 decoder released\n");
        }
        catch (wexception &err)
        {
            printf("subtask: execution failed HR=%08x\n"
                "%S\n",
                err.HResult(),
                err.wwhat());
        }
    });

    // wait 20 ms
    if (doneEvent.wait(20) == 0)
    {
        failed = TRUE;

        throw wexception::make(
            E_FAIL,
            L"done event is signaled before VPU released");
    }
    else
    {
        printf("Subtask is correctly blocked on IOCT_VPU_DEC_RESERVE\n");
    }

    printf("Closing file without releasing core %d\n", output);

    handle1.Close();

    // wait upto 20 ms, but should return immediately
    if (doneEvent.wait(20) != 0)
    {
        failed = TRUE;

        throw wexception::make(
            E_FAIL,
            L"done event was not signaled after file closed");
    }
    else
    {
        printf("Subtask correctly signaled done event\n");
    }

    tasks.wait();
    printf("Finished TestReserve/FileClose\n");
}

int main()
{
    printf("VpuIoctlTest\n");

    try
    {
        // basic tests
        test();

        TestReserveRelease();

        TestReserveFileClose();
    }
    catch (wexception &err)
    {
        printf("Test execution failed HR=%08x\n"
            "%S\n",
            err.HResult(),
            err.wwhat());
        return -1;
    }

    return 0;
}
