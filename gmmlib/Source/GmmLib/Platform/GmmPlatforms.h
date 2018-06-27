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

#ifdef __cplusplus
#include "../inc/External/Common/GmmMemAllocator.hpp"

namespace GmmLib {
    class NON_PAGED_SECTION PlatformInfo : public GmmMemAllocator
    {
    public:
        static int32_t RefCount;
        static int32_t OverrideRefCount;

    protected:
        GMM_PLATFORM_INFO Data;

    public:
        PlatformInfo(PLATFORM &Platform);
        virtual ~PlatformInfo()
        {
        }

        const GMM_PLATFORM_INFO& GetData()
        {
            return Data;
        }

        virtual void ApplyExtendedTexAlign(uint32_t CCSMode, ALIGNMENT& UnitAlign)
        {
            GMM_UNREFERENCED_PARAMETER(CCSMode);
            GMM_UNREFERENCED_PARAMETER(UnitAlign);
        }

        void SetDataSurfaceMaxSize(uint64_t Size)
        {
            Data.SurfaceMaxSize = Size;
        }
        void SetDataFBCRequiredStolenMemorySize(uint32_t Size)
        {
            Data.FBCRequiredStolenMemorySize = Size;
        }
        void SetDataNumberFenceRegisters(uint32_t Number)
        {
            Data.NumberFenceRegisters = Number;
        }

        static void IncrementRefCount()
        {
#if defined(__GMM_KMD__) || _WIN32
            InterlockedIncrement((LONG *)&RefCount);
#elif defined(__linux__)
            __sync_fetch_and_add(&RefCount, 1);
#endif
            //TODO[Android]
        }

        static int32_t DecrementRefCount()
        {
#if defined(__GMM_KMD__) || _WIN32
            return(InterlockedDecrement((LONG *)&RefCount));
#elif defined(__linux__)
            return(__sync_sub_and_fetch(&RefCount, 1));
#endif
            //TODO[Android]
        }

        static PlatformInfo* Create(PLATFORM Platform, bool Override);
    };
}
#define GMM_PLATFORM_INFO_CLASS GmmLib::PlatformInfo
#else
typedef struct PlatformInfo PlatformInfo;
#define GMM_PLATFORM_INFO_CLASS PlatformInfo
#endif

//***************************************************************************
//
//                      GMM_PLATFORM_INFO Internl API
//
//***************************************************************************
#define SET_TILE_MODE_INFO(Mode, _Width, _Height, _Depth, _MtsWidth, _MtsHeight, _MtsDepth) \
{                                                                                           \
    Data.TileInfo[Mode].LogicalTileWidth      = GMM_BYTES(_Width);                          \
    Data.TileInfo[Mode].LogicalTileHeight     = GMM_SCANLINES(_Height);                     \
    Data.TileInfo[Mode].LogicalTileDepth      = (_Depth);                                   \
    Data.TileInfo[Mode].LogicalSize           = (_Width) * (_Height) * (_Depth);            \
    Data.TileInfo[Mode].MaxPitch              = GMM_KBYTE(256);                             \
    Data.TileInfo[Mode].MaxMipTailStartWidth  = (_MtsWidth);                                \
    Data.TileInfo[Mode].MaxMipTailStartHeight = (_MtsHeight);                               \
    Data.TileInfo[Mode].MaxMipTailStartDepth  = (_MtsDepth);                                \
}

#if __cplusplus
extern "C" {
#endif

    const GMM_PLATFORM_INFO* GMM_STDCALL __GmmGetPlatformInfo();
    void GMM_STDCALL __SetFBCRequiredStolenMemorySize(uint32_t Size);
    void GMM_STDCALL __SetNumberFenceRegisters(uint32_t Number);

#if(defined(__GMM_KMD__) && (_DEBUG || _RELEASE_INTERNAL))
    const GMM_PLATFORM_INFO* GMM_STDCALL __GmmGetOverridePlatformInfo();
#endif

#if __cplusplus
}
#endif
