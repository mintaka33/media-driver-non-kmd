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


#include "Internal/Common/GmmLibInc.h"

/////////////////////////////////////////////////////////////////////////////////////
/// Copies parameters or sets flags based on info sent by the client.
///
/// @param[in]  CreateParams: Flags which specify what sort of resource to create
/// @return     false if encountered invalid param. true otherwise.
/////////////////////////////////////////////////////////////////////////////////////
bool GmmLib::GmmResourceInfoCommon::CopyClientParams(GMM_RESCREATE_PARAMS &CreateParams)
{
    Surf.Type                      = CreateParams.Type;
    Surf.Format                    = CreateParams.Format;
    Surf.BaseWidth                 = CreateParams.BaseWidth64;
    Surf.BaseHeight                = CreateParams.BaseHeight;
    Surf.Depth                     = CreateParams.Depth;
    Surf.MaxLod                    = CreateParams.MaxLod;
    Surf.ArraySize                 = CreateParams.ArraySize;
    Surf.Flags                     = CreateParams.Flags;
    Surf.MSAA                      = CreateParams.MSAA;
    Surf.Alignment.BaseAlignment   = CreateParams.BaseAlignment;
    Surf.CachePolicy.Usage         = CreateParams.Usage;
    Surf.MSAA.NumSamples           = GFX_MAX(Surf.MSAA.NumSamples, 1);
    Surf.MaximumRenamingListLength = CreateParams.MaximumRenamingListLength;
    Surf.OverridePitch             = CreateParams.OverridePitch;

    Surf.Flags.Info.__PreWddm2SVM =
    Surf.Flags.Info.SVM &&
    !(pGmmGlobalContext->GetSkuTable().FtrWddm2GpuMmu ||
      pGmmGlobalContext->GetSkuTable().FtrWddm2Svm);

#if !__GMM_KMD__ && _WIN32
    if(pGmmGlobalContext->GetWaTable().WaLLCCachingUnsupported)
    {
        Surf.Flags.Info.GttMapType = (CreateParams.Flags.Info.Cacheable) ?
                                     GMM_GTT_CACHETYPE_VLV_SNOOPED :
                                     GMM_GTT_CACHETYPE_UNCACHED;
    }
#endif

#if(_DEBUG || _RELEASE_INTERNAL)
    Surf.Platform = pGmmGlobalContext->GetPlatformInfo().Platform;
#endif

    if((CreateParams.Format > GMM_FORMAT_INVALID) &&
       (CreateParams.Format < GMM_RESOURCE_FORMATS))
    {
        Surf.BitsPerPixel = pGmmGlobalContext->GetPlatformInfo().FormatTable[CreateParams.Format].Element.BitsPer;
    }
    else
    {
        GMM_ASSERTDPF(0, "Format Error");
        return false;
    }

    if((GFX_GET_CURRENT_RENDERCORE(pGmmGlobalContext->GetPlatformInfo().Platform) <= IGFX_GEN10_CORE))
    {
        if(Surf.Flags.Gpu.MCS)
        {
            Surf.Flags.Gpu.CCS = Surf.Flags.Gpu.MCS;
        }
        Surf.Flags.Info.RenderCompressed = Surf.Flags.Info.MediaCompressed = 0;
    }

    // Moderate down displayable flags if input parameters are not conducive.
    // Reject non displayable tiling modes
    if(Surf.Flags.Gpu.FlipChain || Surf.Flags.Gpu.Overlay)
    {
        if(Surf.Flags.Info.TiledY && !pGmmGlobalContext->GetSkuTable().FtrDisplayYTiling)
        {
            if(Surf.Flags.Gpu.FlipChainPreferred)
            {
                Surf.Flags.Gpu.Overlay   = 0;
                Surf.Flags.Gpu.FlipChain = 0;
            }
            else
            {
                GMM_ASSERTDPF(0, "Y-Tiling mode not displayable");
                //return false;
            }
        }
    }

    // Promote tiling options if caller does not provide any.
    // X/Y/W/L are tiling formats, and Yf/Ys are modifiers to the internal
    // ordering for Y and L macro-formats.
    if(!(Surf.Flags.Info.TiledY || Surf.Flags.Info.TiledX ||
         Surf.Flags.Info.TiledW || Surf.Flags.Info.Linear))
    {
        // Propose Y unless this conflicts with .FtrDisplayYTiling and .FlipChain/.Overlay
        if(Surf.Flags.Gpu.Overlay || Surf.Flags.Gpu.FlipChain)
        {
            Surf.Flags.Info.TiledY = pGmmGlobalContext->GetSkuTable().FtrDisplayYTiling;
        }
        else
        {
            Surf.Flags.Info.TiledY = 1;
        }
        // Propose L+X by default.
        Surf.Flags.Info.TiledX = 1;
        Surf.Flags.Info.Linear = 1;
    }

    // Convert Any Pseudo Creation Params to Actual...
    GMM_TEXTURE_CALC *pTextureCalc = GMM_OVERRIDE_TEXTURE_CALC(&Surf);
    if(Surf.Flags.Gpu.UnifiedAuxSurface)
    {
        AuxSurf = Surf;

        if(Surf.Flags.Gpu.Depth && Surf.Flags.Gpu.CCS) //Depth + HiZ+CCS
        {
            //GMM_ASSERTDPF(Surf.Flags.Gpu.HiZ, "Lossless Z compression supported when Depth+HiZ+CCS is unified");
            AuxSecSurf                           = Surf;
            Surf.Flags.Gpu.HiZ                   = 0; //Its depth buffer, so clear HiZ
            AuxSecSurf.Flags.Gpu.HiZ             = 0;
            AuxSurf.Flags.Gpu.IndirectClearColor = 0; //Clear Depth flags from HiZ, contained with separate/legacy HiZ when Depth isn't compressible.
            AuxSurf.Flags.Gpu.CCS                = 0;
            AuxSurf.Type                         = (AuxSurf.Flags.Gpu.HiZ) ? AuxSurf.Type : RESOURCE_INVALID;
            AuxSurf.Flags.Info.RenderCompressed = AuxSurf.Flags.Info.MediaCompressed = 0;
        }
        else if(Surf.Flags.Gpu.SeparateStencil && Surf.Flags.Gpu.CCS) //Stencil compression
        {
            AuxSurf.Flags.Gpu.SeparateStencil = 0;
            Surf.Flags.Gpu.CCS                = 0;
            if(GMM_SUCCESS != pTextureCalc->PreProcessTexSpecialCases(&Surf))
            {
                return false;
            }
            Surf.Flags.Gpu.CCS = 1;
        }
        else if(Surf.MSAA.NumSamples > 1 && Surf.Flags.Gpu.CCS) //MSAA+MCS+CCS
        {
            GMM_ASSERTDPF(Surf.Flags.Gpu.MCS, "Lossless MSAA supported when MSAA+MCS+CCS is unified");
            AuxSecSurf                          = Surf;
            AuxSecSurf.Flags.Gpu.MCS            = 0;
            AuxSurf.Flags.Gpu.CCS               = 0;
            AuxSurf.Flags.Info.RenderCompressed = AuxSurf.Flags.Info.MediaCompressed = 0;
        }

        if(GMM_SUCCESS != pTextureCalc->PreProcessTexSpecialCases(&AuxSurf))
        {
            return false;
        }
        if(AuxSecSurf.Type != RESOURCE_INVALID &&
           GMM_SUCCESS != pTextureCalc->PreProcessTexSpecialCases(&AuxSecSurf))
        {
            return false;
        }
    }
    else
    {
        if(GMM_SUCCESS != pTextureCalc->PreProcessTexSpecialCases(&Surf))
        {
            return false;
        }
    }

    RotateInfo = CreateParams.RotateInfo;

#ifdef __GMM_KMD__
    if(Surf.Flags.Gpu.S3d)
    {
        Surf.S3d = CreateParams.S3d;
    }
#endif

    return true;
}


/////////////////////////////////////////////////////////////////////////////////////
/// Validates the parameters passed in by clients to make sure they do not
/// conflict or ask for unsupporting combinations/features.
///
/// @return     1 is validation passed. 0 otherwise.
/////////////////////////////////////////////////////////////////////////////////////
uint8_t GMM_STDCALL GmmLib::GmmResourceInfoCommon::ValidateParams()
{
    __GMM_BUFFER_TYPE              Restrictions             = {0};
    const __GMM_PLATFORM_RESOURCE *pPlatformResource        = NULL;
    bool                           AllowMaxWidthViolations  = false;
    bool                           AllowMaxHeightViolations = false;

    GMM_DPF_ENTER;

    __GMM_ASSERTPTR(pGmmGlobalContext, 0);

#if(defined(__GMM_KMD__) && (_DEBUG || _RELEASE_INTERNAL))
    //KMD Debug and Release Internal Drivers only
    //if GMM_TEXTURE_INFO's platform type != native platform
    //then initialize the OverridePlatformInfo.

    //If GMM_TEXTURE_INFO's platform type == IGFX_UNKNOWN_CORE then it means that UMD driver is
    //in "Release" version and passes all zeros in Surf.Platform
    if(GFX_GET_CURRENT_RENDERCORE(Surf.Platform) == IGFX_UNKNOWN_CORE)
    {
        Surf.Platform = pGmmGlobalContext->GetPlatformInfo().Platform;
        // If this is a unified surface then make sure the AUX surface has the same platform info
        if(Surf.Flags.Gpu.UnifiedAuxSurface)
        {
            AuxSurf.Platform    = Surf.Platform;
            AuxSecSurf.Platform = Surf.Platform;
        }
    }

    if(pGmmGlobalContext->GetPlatformInfoObj() != NULL &&
       (GFX_GET_CURRENT_RENDERCORE(Surf.Platform) !=
        GFX_GET_CURRENT_RENDERCORE(pGmmGlobalContext->GetPlatformInfo().Platform)) &&
       (pGmmGlobalContext->GetOverridePlatformInfoObj() == NULL ||
        (GFX_GET_CURRENT_RENDERCORE(Surf.Platform) !=
         GFX_GET_CURRENT_RENDERCORE(pGmmGlobalContext->GetOverridePlatformInfo().Platform))))
    {
        //Ensure override is a future platform.
        __GMM_ASSERT(GFX_GET_CURRENT_RENDERCORE(Surf.Platform) >
                     GFX_GET_CURRENT_RENDERCORE(pGmmGlobalContext->GetPlatformInfo().Platform));

        pGmmGlobalContext->SetOverridePlatformInfoObj(GmmLib::PlatformInfo::Create(Surf.Platform, true));

        if(pGmmGlobalContext->GetOverrideTextureCalc())
        {
            delete(pGmmGlobalContext->GetOverrideTextureCalc());
            pGmmGlobalContext->SetOverrideTextureCalc(NULL);
        }

        pGmmGlobalContext->SetOverrideTextureCalc(GmmLib::GmmTextureCalc::Create(Surf.Platform, true));
    }
#endif

    pPlatformResource = GMM_OVERRIDE_PLATFORM_INFO(&Surf);

    __GMM_ASSERT(!(
    Surf.Flags.Gpu.Query &&
    !Surf.Flags.Info.Cacheable)); // Why query not set as cacheable? If not cacheable, what keeps from stolen memory (because STORE_DWORD/PIPE_CONTROL/etc. targets can't be in stolen)?

    if(Surf.Flags.Gpu.HiZ)
    {
        // HiZ checked in PreProcessTexSpecialCases before legal HZ_Width/Height expansion.
        AllowMaxWidthViolations  = true;
        AllowMaxHeightViolations = true;
    }

    // check bpp
    if(((Surf.BitsPerPixel < 8) || (Surf.BitsPerPixel % GMM_BITS(8))) &&
       !(Surf.BitsPerPixel == 1))
    {
        GMM_ASSERTDPF(0, "Invalid BitsPerPixel!");
        goto ERROR_CASE;
    }

    // Check at least one tile pref set.
    // Yf/Ys checked explicitly here, require one of Y or Linear depending on resource type (e.g 1D-->Linear)
    // that TODO: could be automatically promoted.
    if((Surf.Flags.Info.Linear == 0) &&
       (Surf.Flags.Info.TiledW == 0) &&
       (Surf.Flags.Info.TiledX == 0) &&
       (Surf.Flags.Info.TiledY == 0) &&
       (Surf.Flags.Info.TiledYf == 0) &&
       (Surf.Flags.Info.TiledYs == 0))
    {
        GMM_ASSERTDPF(0, "No Tile or Linear preference specified!");
        goto ERROR_CASE;
    }

    if(!__CanSupportStdTiling(Surf))
    {
        GMM_ASSERTDPF(0, "Invalid TileYf/TileYs usage!");
        goto ERROR_CASE;
    }

    if(Surf.TileMode < TILE_NONE || Surf.TileMode >= GMM_TILE_MODES)
    {
        GMM_ASSERTDPF(0, "Invalid TileMode usage!");
        goto ERROR_CASE;
    }

    if(!((Surf.Format > GMM_FORMAT_INVALID) &&
         (Surf.Format < GMM_RESOURCE_FORMATS)))
    {
        GMM_ASSERTDPF(0, "Invalid Resource Format!");
        goto ERROR_CASE;
    }

    // Check resource format is supported on running platform
    if(IsPresentableformat() == false)
    {
        GMM_ASSERTDPF(0, "Bad Format!");
        goto ERROR_CASE;
    }

    if((Surf.Type == RESOURCE_PRIMARY) &&
       !Surf.Flags.Gpu.FlipChain)
    {
        GMM_ASSERTDPF(0, "Primary does not have FlipChain flag set.");
        goto ERROR_CASE;
    }

    if(Surf.Flags.Gpu.Overlay &&
       Surf.Flags.Gpu.FlipChain)
    {
        GMM_ASSERTDPF(0, "Overlay and FlipChain flags set. S3D logic may fail.");
        goto ERROR_CASE;
    }

    if((GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) < IGFX_GEN8_CORE) &&
       Surf.Flags.Info.TiledW)
    {
        GMM_ASSERTDPF(0, "Flag not supported on this platform.");
        goto ERROR_CASE;
    }

    if((GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) < IGFX_GEN9_CORE) &&
#if(_DEBUG || _RELEASE_INTERNAL)
       !pGmmGlobalContext->GetWaTable().WaDisregardPlatformChecks &&
#endif
       Surf.Flags.Gpu.MMC)
    {
        GMM_ASSERTDPF(0, "Flag not supported on this platform.");
        goto ERROR_CASE;
    }

    if(Surf.Flags.Gpu.MMC && //For Media Memory Compression --
       ((!(Surf.Flags.Info.TiledY || Surf.Flags.Info.TiledYs) &&
         (GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) <= IGFX_GEN10_CORE)) ||
        (GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) <= IGFX_GEN10_CORE &&
         Surf.ArraySize > GMM_MAX_MMC_INDEX)))
    {
        GMM_ASSERTDPF(0, "Invalid flag or array size!");
        goto ERROR_CASE;
    }

    //GMM asserts that ExistingSysMem allocation (whose malloc is outside GmmLib) are either
    //SVM Buffer, Index Buffer, Vertex Buffers, Render Target, Texture
    if(Surf.Flags.Info.ExistingSysMem &&
       !Surf.ExistingSysMem.IsGmmAllocated &&
       !Surf.ExistingSysMem.IsPageAligned)
    {
        if(!(Surf.Flags.Gpu.RenderTarget ||
             Surf.Flags.Gpu.Vertex ||
             Surf.Flags.Gpu.Index ||
             Surf.Flags.Gpu.Texture ||
             Surf.Flags.Info.Linear ||
             Surf.Flags.Info.SVM))
        {
            GMM_ASSERTDPF(0, "Flag not supported by ExistingSysMem alloc!");
            goto ERROR_CASE;
        }

        if(Surf.Type == RESOURCE_3D ||
           Surf.Type == RESOURCE_CUBE ||
           Surf.MaxLod ||
           GmmIsYUVPlanar(Surf.Format))
        {
            GMM_ASSERTDPF(0, "Invalid ExistingSysMem resource Type/Format");
            goto ERROR_CASE;
        }
    }

    GetRestrictions(Restrictions);

    // Check array size to make sure it meets HW limits
    if((Surf.ArraySize > Restrictions.MaxArraySize) &&
       ((RESOURCE_1D == Surf.Type) ||
        (RESOURCE_2D == Surf.Type) ||
        (RESOURCE_CUBE == Surf.Type)))
    {
        GMM_ASSERTDPF(0, "Invalid array size!");
        goto ERROR_CASE;
    }

    // Check dimensions to make sure it meets HW max limits
    if(((Surf.BaseHeight > Restrictions.MaxHeight) && !AllowMaxHeightViolations) ||
       ((Surf.BaseWidth > Restrictions.MaxWidth) && !AllowMaxWidthViolations) ||
       (Surf.Depth > Restrictions.MaxDepth)) // Any reason why MaxDepth != 1 for Tex2D
    {
        GMM_ASSERTDPF(0, "Invalid Dimension. Greater than max!");
        goto ERROR_CASE;
    }

    // Check width to make sure it meets Yx requirements
    if(((Surf.Format == GMM_FORMAT_Y8_UNORM_VA) && (Surf.BaseWidth % 4)) ||
       ((Surf.Format == GMM_FORMAT_Y16_UNORM) && (Surf.BaseWidth % 2)) ||
       ((Surf.Format == GMM_FORMAT_Y1_UNORM) && (Surf.BaseWidth % 32)))
    {
        GMM_ASSERTDPF(0, "Invalid width!");
        goto ERROR_CASE;
    }

    // Check dimensions to make sure it meets HW min limits
    if((Surf.BaseHeight < Restrictions.MinHeight) ||
       (Surf.BaseWidth < Restrictions.MinWidth) ||
       (Surf.Depth < Restrictions.MinDepth))
    {
        GMM_ASSERTDPF(0, "Invalid Dimension. Less than min!");
        goto ERROR_CASE;
    }

    // check mip map
    if(Surf.MaxLod > pPlatformResource->MaxLod)
    {
        GMM_ASSERTDPF(0, "Invalid mip map chain specified!");
        goto ERROR_CASE;
    }

    // MIPs are not supported for tiled Yf/Ys planar surfaces
    if((Surf.MaxLod) &&
       GmmIsPlanar(Surf.Format) &&
       (Surf.Flags.Info.TiledYf || Surf.Flags.Info.TiledYs))
    {
        GMM_ASSERTDPF(0, "Invalid mip map chain specified!");
        goto ERROR_CASE;
    }

    // check depth buffer tilings
    if((Surf.Flags.Gpu.Depth == 1) &&
       (Surf.Flags.Info.TiledX == 1))
    {
        GMM_ASSERTDPF(0, "Invalid Tiling for Depth Buffer!");
        goto ERROR_CASE;
    }

#if DBG
    // Check if stencil buffer gpu flag is set w/ other flags
    if(Surf.Flags.Gpu.SeparateStencil == 1)
    {
        GMM_RESOURCE_FLAG OnlySeparateStencilGpuFlag;

        memset(&OnlySeparateStencilGpuFlag.Gpu, 0, sizeof(OnlySeparateStencilGpuFlag.Gpu));
        OnlySeparateStencilGpuFlag.Gpu.SeparateStencil = 1;

        if(memcmp(&Surf.Flags.Gpu, &OnlySeparateStencilGpuFlag.Gpu, sizeof(OnlySeparateStencilGpuFlag.Gpu)))
        {
            GMM_ASSERTDPF(0,
                          "Should not set w/ other flags b/c vertical alignment is "
                          "unique for separate stencil and incompatible w/ other "
                          "usages such as render target.");
            goto ERROR_CASE;
        }
    }
#endif

    // MSAA Restrictions
    if((Surf.MSAA.NumSamples > 1) &&
       !( //--- Legitimate MSAA Case ------------------------------------------
       (Surf.Type == RESOURCE_2D) &&
       (Surf.Flags.Gpu.Depth ||
        Surf.Flags.Gpu.HiZ ||
        Surf.Flags.Gpu.RenderTarget ||
        Surf.Flags.Gpu.SeparateStencil) &&
       // Single-LOD (pre-SKL)...
       ((GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) >= IGFX_GEN9_CORE) ||
        (Surf.MaxLod == 0)) &&
       // TileYF cannot be MSAA'ed (pre-Gen10)...
       ((GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) >= IGFX_GEN10_CORE) ||
        !Surf.Flags.Info.TiledYf) &&
       // Non-Compressed/YUV...
       !GmmIsCompressed(Surf.Format) &&
       !GmmIsYUVPacked(Surf.Format) &&
       !GmmIsPlanar(Surf.Format) &&
       // Supported Sample Count for Platform...
       (((GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) >= IGFX_GEN7_CORE) &&
         ((Surf.MSAA.NumSamples == 4) || (Surf.MSAA.NumSamples == 8))) ||
        ((GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) >= IGFX_GEN8_CORE) &&
         ((Surf.MSAA.NumSamples == 2))) ||
        ((GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) >= IGFX_GEN9_CORE) &&
         ((Surf.MSAA.NumSamples == 16))))))
    {
        GMM_ASSERTDPF(0, "Invalid MSAA usage!");
        goto ERROR_CASE;
    }

    // CCS Restrictions
    if(Surf.Flags.Gpu.CCS)
    {
        if(!(                                                           //--- Legitimate CCS Case ----------------------------------------
           ((Surf.Type >= RESOURCE_2D && Surf.Type <= RESOURCE_BUFFER) && //Not supported: 1D; Supported: Buffer, 2D, 3D, cube, Arrays, mip-maps, MSAA, Depth/Stencil
            (Surf.Type <= RESOURCE_CUBE &&
              GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) >= IGFX_GEN8_CORE &&
              GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) <= IGFX_GEN10_CORE)
             ) ||
           ((GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) < IGFX_GEN8_CORE) &&
            Surf.Type == RESOURCE_2D && Surf.MaxLod == 0)))
        {
            GMM_ASSERTDPF(0, "Invalid CCS usage!");
            goto ERROR_CASE;
        }

        if(Surf.Flags.Info.RenderCompressed && Surf.Flags.Info.MediaCompressed)
        {
            GMM_ASSERTDPF(0, "Invalid CCS usage - can't be both RC and MC!");
            goto ERROR_CASE;
        }
    }

    // UnifiedAuxSurface Restrictions
    if((Surf.Flags.Gpu.UnifiedAuxSurface) &&
       !( //--- Legitimate UnifiedAuxSurface Case ------------------------------------------
       Surf.Flags.Gpu.CCS &&
       (Surf.MSAA.NumSamples <= 1 && (Surf.Flags.Gpu.RenderTarget || Surf.Flags.Gpu.Texture))
       ))
    {
        GMM_ASSERTDPF(0, "Invalid UnifiedAuxSurface usage!");
        goto ERROR_CASE;
    }

    // IndirectClearColor Restrictions
    if((Surf.Flags.Gpu.IndirectClearColor) &&
       !( //--- Legitimate IndirectClearColor Case ------------------------------------------
       (GFX_GET_CURRENT_RENDERCORE(pPlatformResource->Platform) >= IGFX_GEN9_CORE) &&
        Surf.Flags.Gpu.UnifiedAuxSurface
       ))
    {
        GMM_ASSERTDPF(0, "Invalid IndirectClearColor usage!");
        goto ERROR_CASE;
    }

    // check 2D, 3D & Cubemap dimensions
    switch(Surf.Type)
    {
        case RESOURCE_1D:
        case RESOURCE_2D:
        case RESOURCE_3D:
        {
            if(!Surf.BaseWidth || !Surf.BaseHeight)
            {
                GMM_ASSERTDPF(0, "Width or Height is 0!");
                goto ERROR_CASE;
            }
            break;
        }
        case RESOURCE_CUBE:
        {
            if(!Surf.BaseWidth || !Surf.BaseHeight)
            {
                GMM_ASSERTDPF(0, "Cubemap Dimensions invalid!");
                goto ERROR_CASE;
            }
            if(Surf.BaseWidth != Surf.BaseHeight)
            {
                GMM_ASSERTDPF(0, "Cubemap Dimensions invalid (width != Height)!");
                goto ERROR_CASE;
            }
            break;
        }
        case RESOURCE_SCRATCH:
        case RESOURCE_BUFFER:
        {
            if(Surf.BaseHeight != 1)
            {
                GMM_ASSERTDPF(0, "Linear surface height not 1!");
                goto ERROR_CASE;
            }
            break;
        }
        default:
            break;
    }

    GMM_DPF_EXIT;
    return 1;

ERROR_CASE:
    __GMM_ASSERT(0);
    return 0;
}

//=============================================================================
//
// Function: GetDisplayCompressionSupport
//
// Desc: Returns true if display hw supports lossless render/media decompression
//       else returns false.
//       Umds can call it to decide if full resolve is required
//
// Parameters:
//      See function arguments.
//
// Returns:
//      uint8_t
//-----------------------------------------------------------------------------
uint8_t GMM_STDCALL GmmLib::GmmResourceInfoCommon::GetDisplayCompressionSupport()
{
    uint8_t             ComprSupported = 0;
    GMM_RESOURCE_FORMAT Format         = Surf.Format;

    if(Surf.Flags.Gpu.UnifiedAuxSurface)
    {
        bool IsSupportedRGB64_16_16_16_16 = false;
        bool IsSupportedRGB32_8_8_8_8     = false;
        bool IsSupportedRGB32_2_10_10_10  = false;
        bool IsSupportedMediaFormats      = false;

        switch(Format) //RGB64 16:16 : 16 : 16 FP16
        {
            case GMM_FORMAT_R16G16B16A16_FLOAT:
            case GMM_FORMAT_R16G16B16X16_FLOAT:
                IsSupportedRGB64_16_16_16_16 = true;
            default:
                break;
        }

        switch(Format) //RGB32 8 : 8 : 8 : 8
        {
            case GMM_FORMAT_B8G8R8A8_UNORM:
            case GMM_FORMAT_R8G8B8A8_UNORM:
            case GMM_FORMAT_B8G8R8X8_UNORM:
            case GMM_FORMAT_R8G8B8X8_UNORM:
            case GMM_FORMAT_R8G8B8A8_UNORM_SRGB:
            case GMM_FORMAT_B8X8_UNORM_G8R8_SNORM:
            case GMM_FORMAT_X8B8_UNORM_G8R8_SNORM:
            case GMM_FORMAT_A8X8_UNORM_G8R8_SNORM:
            case GMM_FORMAT_B8G8R8A8_UNORM_SRGB:
            case GMM_FORMAT_B8G8R8X8_UNORM_SRGB:
            case GMM_FORMAT_R8G8B8A8_SINT:
            case GMM_FORMAT_R8G8B8A8_SNORM:
            case GMM_FORMAT_R8G8B8A8_SSCALED:
            case GMM_FORMAT_R8G8B8A8_UINT:
            case GMM_FORMAT_R8G8B8A8_USCALED:
            case GMM_FORMAT_R8G8B8X8_UNORM_SRGB:
                IsSupportedRGB32_8_8_8_8 = true;
            default:
                break;
        }

        switch(Format) //RGB32 2 : 10 : 10 : 10
        {
            case GMM_FORMAT_B10G10R10X2_UNORM:
            case GMM_FORMAT_R10G10B10A2_UNORM:
            case GMM_FORMAT_B10G10R10A2_UNORM:
            case GMM_FORMAT_B10G10R10A2_SINT:
            case GMM_FORMAT_B10G10R10A2_SNORM:
            case GMM_FORMAT_B10G10R10A2_SSCALED:
            case GMM_FORMAT_B10G10R10A2_UINT:
            case GMM_FORMAT_B10G10R10A2_UNORM_SRGB:
            case GMM_FORMAT_B10G10R10A2_USCALED:
            case GMM_FORMAT_R10G10B10_SNORM_A2_UNORM:
            case GMM_FORMAT_R10G10B10A2_SINT:
            case GMM_FORMAT_R10G10B10A2_SNORM:
            case GMM_FORMAT_R10G10B10A2_SSCALED:
            case GMM_FORMAT_R10G10B10A2_UINT:
            case GMM_FORMAT_R10G10B10A2_UNORM_SRGB:
            case GMM_FORMAT_R10G10B10A2_USCALED:
            case GMM_FORMAT_R10G10B10X2_USCALED:
            case GMM_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
                IsSupportedRGB32_2_10_10_10 = true;
            default:
                break;
        }

        switch(Format)
        {
            case GMM_FORMAT_YUY2: //YUV422 8 bpc
            case GMM_FORMAT_NV12: //YUV420 - NV12
            case GMM_FORMAT_P010: //YUV420 - P0xx
            case GMM_FORMAT_P016:
            case GMM_FORMAT_Y216:
            case GMM_FORMAT_Y416:
                IsSupportedMediaFormats = true; //YUV444 - Y412, Y416
            default:
                break;
        }

        bool IsRenderCompressed = false;
        bool IsMediaCompressed  = false;

        if(IsSupportedRGB32_8_8_8_8 || //RGB32 8 : 8 : 8 : 8
            (GFX_GET_CURRENT_DISPLAYCORE(pGmmGlobalContext->GetPlatformInfo().Platform) >= IGFX_GEN10_CORE &&
            IsSupportedRGB32_2_10_10_10)) //RGB32 2 : 10 : 10 : 10))
        {
            IsRenderCompressed = true;
        }

        ComprSupported = IsRenderCompressed || IsMediaCompressed;
    }

    return ComprSupported;
}
