/*==============================================================================
Copyright(c) 2017 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files(the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and / or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
============================================================================*/
#pragma once

#include "GmmCommonExt.h"
#include "GmmInfo.h"

// Interfaces exported from  GMM Lib DLL
typedef struct _GmmExportEntries
{
#ifdef _WIN32
    GMM_STATUS          (GMM_STDCALL *pfnCreateSingletonContext)(const PLATFORM Platform,
                                                                const SKU_FEATURE_TABLE* pSkuTable,
                                                                const WA_TABLE* pWaTable,
                                                                const GT_SYSTEM_INFO* pGtSysInfo);
#else
    GMM_STATUS(GMM_STDCALL *pfnCreateSingletonContext)(const PLATFORM Platform,
                                                        const void* pSkuTable,
                                                        const void* pWaTable,
                                                        const void* pGtSysInfo);
#endif

    void                (GMM_STDCALL *pfnDestroySingletonContext)(void);

    GMM_CLIENT_CONTEXT* (GMM_STDCALL *pfnCreateClientContext)(GMM_CLIENT ClientType);
    void                (GMM_STDCALL *pfnDeleteClientContext)(GMM_CLIENT_CONTEXT *pGmmClientContext);
}GmmExportEntries;


#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////
/// Only function exported from GMM lib DLL.
/////////////////////////////////////////////////////////////////////////////////////
    GMM_LIB_API GMM_STATUS GMM_STDCALL OpenGmm(GmmExportEntries *pm_GmmFuncs);

#ifdef __cplusplus
}
#endif

#ifdef _WIN32
typedef GMM_STATUS (APIENTRY *pfnGmmEntry)(GmmExportEntries *);
#endif
