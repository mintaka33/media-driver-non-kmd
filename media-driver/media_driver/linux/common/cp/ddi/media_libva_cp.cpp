/*
* Copyright (c) 2015-2017, Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
//!
//! \file     media_libva_cp.cpp
//! \brief    DDI interface for content protection parameters and operations.
//! \details  Other components will call this interface for content protection  operations directly
//!

#include "media_libva_util.h"
#include "media_libva_decoder.h"
#include "media_libva_encoder.h"
#include "media_libva_vp.h"
#include "media_libva_cp.h"

static void DdiStubMessage()
{
    DDI_NORMALMESSAGE("This function is stubbed as CP is not enabled.");
}

DdiCpInterface::DdiCpInterface(MOS_CONTEXT& mosCtx)
{
    DdiStubMessage();
}

DdiCpInterface::~DdiCpInterface()
{
}

void DdiCpInterface::SetCpParams(uint32_t encryptionType, CodechalSetting *setting)
{
    DdiStubMessage();
}

VAStatus DdiCpInterface::EndPictureCenc(
    VADriverContextP vaDrvCtx,
    VAContextID      contextId)
{
    DdiStubMessage();
    return VA_STATUS_SUCCESS;
}

VAStatus DdiCpInterface::QueryCencStatus(
    VADriverContextP    ctx,
    VASurfaceStatus     *status)
{
    DdiStubMessage();
    return VA_STATUS_SUCCESS;
}

VAStatus DdiCpInterface::RenderCencPicture(
    VADriverContextP      vaDrvctx,
    VAContextID           contextId,
    DDI_MEDIA_BUFFER      *buf,
    void                  *data)
{
    DdiStubMessage();
    return VA_STATUS_SUCCESS;
}

VAStatus DdiCpInterface::CreateBuffer(
    VABufferType             type,
    DDI_MEDIA_BUFFER*        buffer,
    uint32_t                 size,
    uint32_t                 num_elements
)
{
    DdiStubMessage();
    return VA_STATUS_ERROR_UNSUPPORTED_BUFFERTYPE;
}

bool DdiCpInterface::CheckSupportedBufferForCodec(VABufferType type)
{
    DdiStubMessage();
    return false;
}

VAStatus DdiCpInterface::InitHdcp2Buffer(DDI_CODEC_COM_BUFFER_MGR* bufMgr)
{
    DdiStubMessage();
    return VA_STATUS_SUCCESS;
}

VAStatus DdiCpInterface::StatusReportForHdcp2Buffer(
    DDI_CODEC_COM_BUFFER_MGR*       bufMgr,
    void*               encodeStatusReport)
{
    DdiStubMessage();
    return VA_STATUS_SUCCESS;
}

void DdiCpInterface::FreeHdcp2Buffer(DDI_CODEC_COM_BUFFER_MGR* bufMgr)
{
    DdiStubMessage();
    return ;
}

MOS_STATUS DdiCpInterface::StoreCounterToStatusReport(
    PDDI_ENCODE_STATUS_REPORT_INFO info)
{
    DdiStubMessage();
    return MOS_STATUS_SUCCESS;
}

VAStatus DdiCpInterface::ParseCpParamsForEncode()
{
    DdiStubMessage();
    return VA_STATUS_SUCCESS;
}

void DdiCpInterface::SetHdcp2Enabled(int32_t flag)
{
    DdiStubMessage();
}

bool DdiCpInterface::IsHdcp2Enabled()
{
    DdiStubMessage();
    return false;
}

void DdiCpInterface::ResetCpContext()
{
    DdiStubMessage();
}

VAStatus DdiCpInterface::RenderPictureForVp(
    VADriverContextP      vaDrvCtx,
    VAContextID           vpCtxID,
    DDI_MEDIA_BUFFER      *buf,
    void                  *data)
{
    DdiStubMessage();
    return VA_STATUS_ERROR_UNSUPPORTED_BUFFERTYPE;
}

bool DdiCpInterface::CheckSupportedBufferForVp(VABufferType type)
{
    DdiStubMessage();
    return false;
}

VAStatus DdiCpInterface::CreateCencDecode(
    CodechalDecode              *decoder,
    PMOS_CONTEXT                osContext,
    CodechalSetting *           settings)
{
    DdiStubMessage();
    return VA_STATUS_SUCCESS;
}
