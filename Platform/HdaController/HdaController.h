/*
 * File: HdaController.h
 *
 * Copyright (c) 2018 John Davis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _EFI_HDA_CONTROLLER_H_
#define _EFI_HDA_CONTROLLER_H_

#include "AudioDxe.h"
#include "HdaCodecProtocol.h"

//
// Consumed protocols.
//
#include <Protocol/PciIo.h>
#include <IndustryStandard/Pci.h>

// Structure used for PCI class code parsing.
#pragma pack(1)
typedef struct {
    UINT8 ProgInterface;
    UINT8 SubClass;
    UINT8 Class;
} HDA_PCI_CLASSREG;
#pragma pack()

#define PCI_CLASS_MEDIA_HDA 0x3

#define HDA_MAX_CODECS 15

#define INTEL_VEN_ID 0x8086

#define PCI_HDA_TCSEL_OFFSET    0x44
#define PCI_HDA_TCSEL_TC0_MASK  0xF8
#define PCI_HDA_DEVC_OFFSET     0x78
#define PCI_HDA_DEVC_NOSNOOPEN  (1 < 11)

#define HDA_CORB_VERB(Cad, Nid, Verb) ((((UINT32)Cad) << 28) | (((UINT32)Nid) << 20) | (Verb & 0xFFFFF))

#define HDA_RIRB_RESP(Response)     ((UINT32)Response)
#define HDA_RIRB_CAD(Response)      ((Response >> 32) & 0xF)
#define HDA_RIRB_UNSOL(Response)    ((Response >> 36) & 0x1)

typedef struct {
    // PCI protocol.
    EFI_PCI_IO_PROTOCOL *PciIo;
    EFI_DEVICE_PATH_PROTOCOL *DevicePath;
    UINT64 OriginalPciAttributes;
    EFI_HANDLE ControllerHandle;
    EFI_DRIVER_BINDING_PROTOCOL *DriverBinding;

    // Whether or not 64-bit addressing is supported.
    BOOLEAN Buffer64BitSupported;

    // Command output buffer (CORB).
    UINT32 *CorbBuffer;
    UINT32 CorbEntryCount;
    VOID *CorbMapping;
    EFI_PHYSICAL_ADDRESS CorbPhysAddr;
    UINT16 CorbWritePointer;

    // Response input buffer (RIRB).
    UINT64 *RirbBuffer;
    UINT32 RirbEntryCount;
    VOID *RirbMapping;
    EFI_PHYSICAL_ADDRESS RirbPhysAddr;
    UINT16 RirbReadPointer;

    // Events.
    EFI_EVENT ResponsePollTimer;
    EFI_EVENT ExitBootServiceEvent;
} HDA_CONTROLLER_DEV;

#define HDA_CONTROLLER_PRIVATE_DATA_SIGNATURE SIGNATURE_32('H','d','a','C')

typedef struct {
    // Signature.
    UINTN Signature;

    // HDA Codec protocol and address.
    EFI_HDA_CODEC_PROTOCOL HdaCodec;
    UINT8 HdaCodecAddress;

    // HDA controller.
    HDA_CONTROLLER_DEV *HdaDev;
} HDA_CONTROLLER_PRIVATE_DATA;

#define HDA_CONTROLLER_PRIVATE_DATA_FROM_THIS(This) CR(This, HDA_CONTROLLER_PRIVATE_DATA, HdaCodec, HDA_CONTROLLER_PRIVATE_DATA_SIGNATURE)

EFI_STATUS
EFIAPI
HdaControllerCodecProtocolSendCommand(
    IN EFI_HDA_CODEC_PROTOCOL *This,
    IN UINT8 Node,
    IN UINT32 Verb,
    OUT UINT32 *Response);

EFI_STATUS
EFIAPI
HdaControllerCodecProtocolSendCommands(
    IN EFI_HDA_CODEC_PROTOCOL *This,
    IN UINT8 Node,
    IN EFI_HDA_CODEC_VERB_LIST *Verbs);

EFI_STATUS
EFIAPI
HdaControllerSendCommands(
    IN HDA_CONTROLLER_DEV *HdaDev,
    IN UINT8 CodecAddress,
    IN UINT8 Node,
    IN EFI_HDA_CODEC_VERB_LIST *Verbs);

VOID
HdaControllerResponsePollTimerHandler(
    IN EFI_EVENT Event,
    IN VOID *Context);

EFI_STATUS
EFIAPI
HdaControllerRegisterDriver(VOID);

#endif
