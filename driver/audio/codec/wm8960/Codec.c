/* Copyright (c) Microsoft Corporation. All rights reserved.
   Copyright 2019 NXP
   Licensed under the MIT License.

Module Name:

    Codec.c - Sends commands to codec via I2c.

Environment:

    User-mode Driver Framework 2

*/

#include "Driver.h"
#include "Codec.h"
#include <spb.h>
#include "codec.tmh"

#define CODEC_I2C_ADDR  (0x1a)

typedef struct {
    DWORD MsSleepTime;
    UCHAR CommandBytes[2];
} CODEC_COMMAND, *PCODEC_COMMAND;

NTSTATUS
CodecSendCommands(
    _In_ PDEVICE_CONTEXT DeviceContext,
    _In_reads_(NumCommands) CODEC_COMMAND CodecCommand[],
    USHORT NumCommands
    );

NTSTATUS
CodecSendCommand(
    _In_ PDEVICE_CONTEXT DeviceContext,
    _In_ PCODEC_COMMAND CodecCommand
    );

NTSTATUS
CodecInitializeHeadphoneOutJack(
    _In_ PDEVICE_CONTEXT DeviceContext
    )
{
    CODEC_COMMAND Commands[] =
    {
        #define WM8960_PACK_ADDR_DATA(addr, data)       .CommandBytes[0] = (((addr) << 1) | (((data) >> 8) & 1)), .CommandBytes[1] = ((data) & 0xFF)
        {.MsSleepTime = 1 , WM8960_PACK_ADDR_DATA(0xF,0)},     // R15 - Codec reset
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0xA,0)},     // Mute L DAC
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0xB,0x100)}, // Mute R DAC
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x28,0)},    // Speaker L mute
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x29,0x100)},// Speaker R mute
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x4,0x4)},   // Clocking 256*Fs , Sysclk = MCLK / 2 = 11.288 MHz
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x8,0x1C0)}, // Configure Class D amp Sysclk / 16
        {.MsSleepTime = 1 , WM8960_PACK_ADDR_DATA(0x19,0x0)},  // Disable power
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x1C,0x9C)}, // Anti pop enable
        {.MsSleepTime = 1 , WM8960_PACK_ADDR_DATA(0x19,0xBE)}, // Enable power
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x19,0xFE)}, // Enable VREF
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x33,0x9B)}, // Configure D amp DC and AC Gain
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x2F,0x3C)}, // Enable input mixers and PGA
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x22,0x100)},// Left output mixer path
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x25,0x100)},// Right output mixer path
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x2,0x80)},  // Enable zero crossing
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x3,0x80)},  // Enable zero crossing
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0xA,0xFF)},  // Left DAC volume
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0xB,0x1FF)}, // Right DAC volume
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x2,0xF9)},  // Unmute L volume
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x3,0x1F9)}, // Unmute R volume
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x5,0x0)},   // Unmute DAC
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x20,0x188)},// ADC L signal path
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x21,0x148)},// ADC R signal path
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x0,0x157)}, // Left input volume unmute
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x1,0x157)}, // Right input volume unmute
        {.MsSleepTime = 1 , WM8960_PACK_ADDR_DATA(0x17,0x1C1)},// Enable slow clock for volume and jack detect
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x30,0xF)},  // 0.65*AVDD, temp sensor enabled, JD3 for headphone detect
        // Headphone detect has to be configurable via ACPI because JDx pins differs per board
        //{.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x18,0x40)}, // Enable headphone detect
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x31,0xF7)}, // Enable speaker outputs
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x28,0x80)}, // Enable speaker zero crossing
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x29,0x80)}, // Enable speaker zero crossing
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x28,0xF9)}, // Set L speaker volume
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x29,0x1F9)},// Set R speaker volume
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x1A,0x1F8)},// Enable All except PLL and OUT3
        {.MsSleepTime = 1 , WM8960_PACK_ADDR_DATA(0x7,0xE)},   // I2S format, 32 bits
        {.MsSleepTime = 0 , WM8960_PACK_ADDR_DATA(0x9,0x40)},  // ADC uses DAC LRCK pin
    };

    NTSTATUS status;

    status = CodecSendCommands(DeviceContext, Commands, ARRAYSIZE(Commands));

    return status;
}

NTSTATUS
CodecSendCommand(
    _In_ PDEVICE_CONTEXT DeviceContext,
    _In_ PCODEC_COMMAND CodecCommand
    )
{
    NTSTATUS status;

    ULONG_PTR bytesWritten;
    
    WDF_MEMORY_DESCRIPTOR MemDescriptor;

    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&MemDescriptor,&CodecCommand->CommandBytes[0], sizeof(CodecCommand->CommandBytes));

    status = WdfIoTargetSendWriteSynchronously(DeviceContext->I2cTarget, NULL, &MemDescriptor, 0, NULL, &bytesWritten);
    
    if (CodecCommand->MsSleepTime != 0)
    {
        Sleep(CodecCommand->MsSleepTime);
    }

    return status;
}

NTSTATUS
CodecSendCommands(
    _In_ PDEVICE_CONTEXT DeviceContext,
    _In_reads_(NumCommands) CODEC_COMMAND CodecCommand[],
    USHORT NumCommands
    )
{
    ULONG i;
    NTSTATUS status;

    for (i = 0; i < NumCommands; i++)
    {
        status = CodecSendCommand(DeviceContext, &CodecCommand[i]);

        if (!NT_SUCCESS(status))
        {
            return status;
        }
    }
    return STATUS_SUCCESS;
}
