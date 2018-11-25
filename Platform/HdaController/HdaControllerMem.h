/*
 * File: HdaControllerMem.h
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

#ifndef EFI_HDA_CONTROLLER_MEM_H_
#define EFI_HDA_CONTROLLER_MEM_H_

#include "HdaController.h"

#define HDA_CORB_ENTRY_SIZE sizeof(UINT32)
#define HDA_RIRB_ENTRY_SIZE sizeof(UINT64)

HDA_CONTROLLER_DEV *HdaControllerAllocDevice(
    IN EFI_PCI_IO_PROTOCOL *PciIo,
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN UINT64 OriginalPciAttributes);

EFI_STATUS
EFIAPI
HdaControllerAllocBuffers(
    HDA_CONTROLLER_DEV *HdaDev);

EFI_STATUS
EFIAPI
HdaControllerEnableBuffers(
    HDA_CONTROLLER_DEV *HdaDev);

#endif
