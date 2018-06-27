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
/// This function calculates the (X,Y) address of each given plane. X is in bytes
/// and Y is in scanlines.
///
/// @param[in]  pTexInfo: ptr to ::GMM_TEXTURE_INFO
///
/////////////////////////////////////////////////////////////////////////////////////
void GmmLib::GmmTextureCalc::FillPlanarOffsetAddress(GMM_TEXTURE_INFO *pTexInfo)
{
    GMM_GFX_SIZE_T *pUOffsetX, *pUOffsetY;
    GMM_GFX_SIZE_T *pVOffsetX, *pVOffsetY;
    bool            UVPacked = false;
    uint32_t        Height;

#define SWAP_UV()              \
    {                          \
        GMM_GFX_SIZE_T *pTemp; \
                               \
        pTemp     = pUOffsetX; \
        pUOffsetX = pVOffsetX; \
        pVOffsetX = pTemp;     \
                               \
        pTemp     = pUOffsetY; \
        pUOffsetY = pVOffsetY; \
        pVOffsetY = pTemp;     \
    }

    __GMM_ASSERTPTR(pTexInfo, VOIDRETURN);
    __GMM_ASSERTPTR(((pTexInfo->TileMode < GMM_TILE_MODES) && (pTexInfo->TileMode >= TILE_NONE)), VOIDRETURN);
    GMM_DPF_ENTER;

    // GMM_PLANE_Y always at (0, 0)...
    pTexInfo->OffsetInfo.Plane.X[GMM_PLANE_Y] = 0;
    pTexInfo->OffsetInfo.Plane.Y[GMM_PLANE_Y] = 0;

    Height = pTexInfo->BaseHeight;
    if(pTexInfo->Flags.Gpu.__NonMsaaTileYCcs)
    {
        Height = __GMM_EXPAND_HEIGHT(this, Height, pTexInfo->Alignment.VAlign, pTexInfo);
        Height = ScaleTextureHeight(pTexInfo, Height);
        if(pTexInfo->Flags.Gpu.UnifiedAuxSurface)
        {
            pTexInfo->OffsetInfo.Plane.Y[GMM_PLANE_Y] = 0;
        }
    }

    // GMM_PLANE_U/V Planes...
    pUOffsetX = &pTexInfo->OffsetInfo.Plane.X[GMM_PLANE_U];
    pUOffsetY = &pTexInfo->OffsetInfo.Plane.Y[GMM_PLANE_U];
    pVOffsetX = &pTexInfo->OffsetInfo.Plane.X[GMM_PLANE_V];
    pVOffsetY = &pTexInfo->OffsetInfo.Plane.Y[GMM_PLANE_V];

    switch(pTexInfo->Format)
    {
        case GMM_FORMAT_IMC1:
            SWAP_UV(); // IMC1 = IMC3 with Swapped U/V
        case GMM_FORMAT_IMC3:
        case GMM_FORMAT_MFX_JPEG_YUV420: // Same as IMC3.
        // YYYYYYYY
        // YYYYYYYY
        // YYYYYYYY
        // YYYYYYYY
        // UUUU
        // UUUU
        // VVVV
        // VVVV
        case GMM_FORMAT_MFX_JPEG_YUV422V: // Similar to IMC3 but U/V are full width.
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // UUUUUUUU
            // UUUUUUUU
            // VVVVVVVV
            // VVVVVVVV
            {
                *pUOffsetX = 0;
                *pUOffsetY = GFX_ALIGN(pTexInfo->BaseHeight, GMM_IMCx_PLANE_ROW_ALIGNMENT);

                *pVOffsetX = 0;
                *pVOffsetY =
                GFX_ALIGN(pTexInfo->BaseHeight, GMM_IMCx_PLANE_ROW_ALIGNMENT) +
                GFX_ALIGN(GFX_CEIL_DIV(pTexInfo->BaseHeight, 2), GMM_IMCx_PLANE_ROW_ALIGNMENT);

                break;
            }
        case GMM_FORMAT_MFX_JPEG_YUV411R_TYPE: //Similar to IMC3 but U/V are quarther height and full width.
            //YYYYYYYY
            //YYYYYYYY
            //YYYYYYYY
            //YYYYYYYY
            //UUUUUUUU
            //VVVVVVVV
            {
                *pUOffsetX = 0;
                *pUOffsetY = GFX_ALIGN(pTexInfo->BaseHeight, GMM_IMCx_PLANE_ROW_ALIGNMENT);

                *pVOffsetX = 0;
                *pVOffsetY =
                GFX_ALIGN(pTexInfo->BaseHeight, GMM_IMCx_PLANE_ROW_ALIGNMENT) +
                GFX_ALIGN(GFX_CEIL_DIV(pTexInfo->BaseHeight, 4), GMM_IMCx_PLANE_ROW_ALIGNMENT);

                break;
            }
        case GMM_FORMAT_MFX_JPEG_YUV411: // Similar to IMC3 but U/V are quarter width and full height.
        // YYYYYYYY
        // YYYYYYYY
        // YYYYYYYY
        // YYYYYYYY
        // UU
        // UU
        // UU
        // UU
        // VV
        // VV
        // VV
        // VV
        case GMM_FORMAT_MFX_JPEG_YUV422H: // Similar to IMC3 but U/V are full height.
        // YYYYYYYY
        // YYYYYYYY
        // YYYYYYYY
        // YYYYYYYY
        // UUUU
        // UUUU
        // UUUU
        // UUUU
        // VVVV
        // VVVV
        // VVVV
        // VVVV
        case GMM_FORMAT_BGRP:
        case GMM_FORMAT_RGBP:
        case GMM_FORMAT_MFX_JPEG_YUV444: // Similar to IMC3 but U/V are full size.
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // UUUUUUUU
            // UUUUUUUU
            // UUUUUUUU
            // UUUUUUUU
            // VVVVVVVV
            // VVVVVVVV
            // VVVVVVVV
            // VVVVVVVV
            {
                *pUOffsetX = 0;
                *pUOffsetY = GFX_ALIGN(pTexInfo->BaseHeight, GMM_IMCx_PLANE_ROW_ALIGNMENT);

                *pVOffsetX = 0;
                *pVOffsetY = GFX_ALIGN(pTexInfo->BaseHeight, GMM_IMCx_PLANE_ROW_ALIGNMENT) * 2;

                break;
            }
        case GMM_FORMAT_IMC2:
            SWAP_UV(); // IMC2 = IMC4 with Swapped U/V
        case GMM_FORMAT_IMC4:
        {
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // UUUUVVVV
            // UUUUVVVV

            __GMM_ASSERT((pTexInfo->Pitch & 1) == 0);

            *pUOffsetX = 0;
            *pUOffsetY = GFX_ALIGN(pTexInfo->BaseHeight, GMM_IMCx_PLANE_ROW_ALIGNMENT);

            *pVOffsetX = pTexInfo->Pitch / 2;
            *pVOffsetY = GFX_ALIGN(pTexInfo->BaseHeight, GMM_IMCx_PLANE_ROW_ALIGNMENT);

            break;
        }
        case GMM_FORMAT_I420: // I420 = IYUV
        case GMM_FORMAT_IYUV:
            SWAP_UV(); // I420/IYUV = YV12 with Swapped U/V
        case GMM_FORMAT_YV12:
        case GMM_FORMAT_YVU9:
        {
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // VVVVVV..  <-- V and U planes follow the Y plane, as linear
            // ..UUUUUU      arrays--without respect to pitch.

            uint32_t YSize, YVSizeRShift, VSize, UOffset;
            uint32_t YSizeForUVPurposes, YSizeForUVPurposesDimensionalAlignment;

            YSize = GFX_ULONG_CAST(pTexInfo->Pitch) * pTexInfo->BaseHeight;

            // YVU9 has one U/V pixel for each 4x4 Y block.
            // The others have one U/V pixel for each 2x2 Y block.

            // YVU9 has a Y:V size ratio of 16 (4x4 --> 1).
            // The others have a ratio of 4 (2x2 --> 1).
            YVSizeRShift = (pTexInfo->Format != GMM_FORMAT_YVU9) ? 2 : 4;

            // If a Y plane isn't fully-aligned to its Y-->U/V block size, the
            // extra/unaligned Y pixels still need corresponding U/V pixels--So
            // for the purpose of computing the UVSize, we must consider a
            // dimensionally "rounded-up" YSize. (E.g. a 13x5 YVU9 Y plane would
            // require 4x2 U/V planes--the same UVSize as a fully-aligned 16x8 Y.)
            YSizeForUVPurposesDimensionalAlignment = (pTexInfo->Format != GMM_FORMAT_YVU9) ? 2 : 4;
            YSizeForUVPurposes =
            GFX_ALIGN(GFX_ULONG_CAST(pTexInfo->Pitch), YSizeForUVPurposesDimensionalAlignment) *
            GFX_ALIGN(pTexInfo->BaseHeight, YSizeForUVPurposesDimensionalAlignment);

            VSize   = (YSizeForUVPurposes >> YVSizeRShift);
            UOffset = YSize + VSize;

            *pVOffsetX = 0;
            *pVOffsetY = pTexInfo->BaseHeight;

            *pUOffsetX = UOffset % pTexInfo->Pitch;
            *pUOffsetY = UOffset / pTexInfo->Pitch;

            break;
        }
        case GMM_FORMAT_NV12:
        case GMM_FORMAT_NV21:
        case GMM_FORMAT_NV11:
        case GMM_FORMAT_P010:
        case GMM_FORMAT_P012:
        case GMM_FORMAT_P016:
        case GMM_FORMAT_P208:
        {
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // YYYYYYYY
            // [UV-Packing]
            *pUOffsetX = *pVOffsetX = 0;
            *pUOffsetY = *pVOffsetY = Height;

            UVPacked = true;
            break;
        }
        default:
        {
            GMM_ASSERTDPF(0, "Unknown Video Format U\n");
            break;
        }
    }

    if(((pTexInfo->Flags.Info.TiledYs || pTexInfo->Flags.Info.TiledYf) &&
        (pTexInfo->Flags.Info.StdSwizzle || UVPacked)) ||
       pTexInfo->Flags.Gpu.__NonMsaaTileYCcs)
    {
        GMM_GFX_SIZE_T TileHeight = pGmmGlobalContext->GetPlatformInfo().TileInfo[pTexInfo->TileMode].LogicalTileHeight;
        GMM_GFX_SIZE_T TileWidth  = pGmmGlobalContext->GetPlatformInfo().TileInfo[pTexInfo->TileMode].LogicalTileWidth;

        *pUOffsetX = GFX_ALIGN(*pUOffsetX, TileWidth);
        *pUOffsetY = GFX_ALIGN(*pUOffsetY, TileHeight);
        *pVOffsetX = GFX_ALIGN(*pVOffsetX, TileWidth);
        *pVOffsetY = GFX_ALIGN(*pVOffsetY, TileHeight);

        if(pTexInfo->Flags.Gpu.UnifiedAuxSurface && pTexInfo->Flags.Gpu.__NonMsaaTileYCcs)
        {
            *pUOffsetY += pTexInfo->OffsetInfo.Plane.Y[GMM_PLANE_Y];
            *pVOffsetY = *pUOffsetY;
        }
    }

    GMM_DPF_EXIT;

#undef SWAP_UV
}


/////////////////////////////////////////////////////////////////////////////////////
/// Sibling function of GmmLib::GmmTextureCalc::ExpandWidth. it returns the given
/// Width, as appropriately scaled by the MSAA NumSamples parameter and aligned to the
/// given UnitAlignment.
///
/// @param[in]  Height: Height of the surface
/// @param[in]  UnitAlignment: Unit alignment factor
/// @param[in]  NumSamples: No of MSAA samples
///
/// @return     scaled height
/////////////////////////////////////////////////////////////////////////////////////
uint32_t GmmLib::GmmTextureCalc::ExpandHeight(uint32_t Height, uint32_t UnitAlignment, uint32_t NumSamples)
{
    // Implemented as separate function (instead of as a single function with a
    // Width/Height parameter) so both functions can be later implemented without
    // branches, if need be.

    return (
    GmmLib::GmmTextureCalc::ExpandWidth(
    Height, UnitAlignment,
    (NumSamples == 2) ? 1 :                 // MSAA_2X: No height adjustment
    ((NumSamples == 8) ? 4 : NumSamples))); // <-- MSAA_8X:Height = MSAA_4X:Height.
}


/////////////////////////////////////////////////////////////////////////////////////
/// This function returns the given Width, as appropriately scaled by the MSAA
/// NumSamples parameter and aligned to the given UnitAlignment.
///
/// @param[in]  Width: Height of the surface
/// @param[in]  UnitAlignment: Unit alignment factor
/// @param[in]  NumSamples: No of MSAA samples
///
/// @return     scaled width
/////////////////////////////////////////////////////////////////////////////////////
uint32_t GmmLib::GmmTextureCalc::ExpandWidth(uint32_t Width, uint32_t UnitAlignment, uint32_t NumSamples)
{
    uint32_t ExpandedWidth;

    switch(NumSamples)
    {
        case 1:
            ExpandedWidth = Width;
            break;
        case 2: // Same as 4x...
        case 4:
            ExpandedWidth = GFX_CEIL_DIV(GFX_MAX(Width, 1), 2) * 4;
            break;
        case 8: // Same as 16x...
        case 16:
            ExpandedWidth = GFX_CEIL_DIV(GFX_MAX(Width, 1), 2) * 8;
            break;
        default:
            ExpandedWidth = Width;
            __GMM_ASSERT(0);
    }

    ExpandedWidth = GFX_MAX(ExpandedWidth, UnitAlignment);
    ExpandedWidth = GFX_ALIGN_NP2(ExpandedWidth, UnitAlignment);

    return (ExpandedWidth);
}


/////////////////////////////////////////////////////////////////////////////////////
/// This function calculates Mip Tail Start LOD using max mip tail dimensions and
/// populates pTexInfo->Alignment.MipTailStartLod
///
/// @param[in]  pTexInfo: ptr to ::GMM_TEXTURE_INFO
///
/////////////////////////////////////////////////////////////////////////////////////
void GmmLib::GmmTextureCalc::FindMipTailStartLod(GMM_TEXTURE_INFO *pTexInfo)
{
    GMM_DPF_ENTER;

    if(!(pTexInfo->Flags.Info.TiledYf || pTexInfo->Flags.Info.TiledYs) ||
       (pTexInfo->MaxLod == 0) ||
       (pTexInfo->Flags.Wa.DisablePackedMipTail))
    {
        // HW never ignores MipTailStartLod for Yf/Ys surfaces. If we do not
        // want a mip tail, we set MipTailStartLod to be greater than MaxLod.
        pTexInfo->Alignment.MipTailStartLod = GMM_TILED_RESOURCE_NO_MIP_TAIL;
    }
    else
    {
        uint32_t                 MipDepth, MipHeight, MipWidth, CompressWidth, CompressHeight, CompressDepth;
        uint32_t                 Level     = 0;
        const GMM_PLATFORM_INFO *pPlatform = GMM_OVERRIDE_PLATFORM_INFO(pTexInfo);

        MipDepth  = pTexInfo->Depth;
        MipHeight = pTexInfo->BaseHeight;
        MipWidth  = GFX_ULONG_CAST(pTexInfo->BaseWidth);

        //if compressed texture format, use compressed height, width
        GetCompressionBlockDimensions(pTexInfo->Format, &CompressWidth, &CompressHeight, &CompressDepth);

        if(GmmIsCompressed(pTexInfo->Format))
        {
            MipWidth  = GFX_CEIL_DIV(MipWidth, CompressWidth);
            MipHeight = GFX_CEIL_DIV(MipHeight, CompressHeight);
            MipDepth  = GFX_CEIL_DIV(MipDepth, CompressDepth);
        }

        while((Level < pTexInfo->MaxLod) &&
              (((pTexInfo->Type == RESOURCE_1D) &&
                !(MipWidth <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartWidth)) ||
               (((pTexInfo->Type == RESOURCE_2D) || (pTexInfo->Type == RESOURCE_CUBE)) &&
                !((MipWidth <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartWidth) &&
                  (MipHeight <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartHeight))) ||
               ((pTexInfo->Type == RESOURCE_3D) &&
                !((MipWidth <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartWidth) &&
                  (MipHeight <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartHeight) &&
                  (MipDepth <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartDepth)))))
        {
            Level++;

            MipWidth  = GFX_ULONG_CAST(GmmTexGetMipWidth(pTexInfo, Level));
            MipHeight = GmmTexGetMipHeight(pTexInfo, Level);
            MipDepth  = GmmTexGetMipDepth(pTexInfo, Level);

            MipWidth  = GFX_CEIL_DIV(MipWidth, CompressWidth);
            MipHeight = GFX_CEIL_DIV(MipHeight, CompressHeight);
            MipDepth  = GFX_CEIL_DIV(MipDepth, CompressDepth);
        }

        if(((pTexInfo->Type == RESOURCE_1D) &&
            (MipWidth <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartWidth)) ||
           (((pTexInfo->Type == RESOURCE_2D) || (pTexInfo->Type == RESOURCE_CUBE)) &&
            ((MipWidth <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartWidth) &&
             (MipHeight <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartHeight))) ||
           ((pTexInfo->Type == RESOURCE_3D) &&
            ((MipWidth <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartWidth) &&
             (MipHeight <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartHeight) &&
             (MipDepth <= pPlatform->TileInfo[pTexInfo->TileMode].MaxMipTailStartDepth))))
        {
            pTexInfo->Alignment.MipTailStartLod = Level;
        }
        else
        {
            pTexInfo->Alignment.MipTailStartLod = GMM_TILED_RESOURCE_NO_MIP_TAIL;
        }
    }

    GMM_DPF_EXIT;
}


/////////////////////////////////////////////////////////////////////////////////////
/// This function returns the height, width and depth of the compression block for a
/// given surface format.
///
/// @param[in]  Format: ::GMM_RESOURCE_FORMAT
/// @param[in]  pWidth: populates Width
/// @param[in]  pHeight: populates Height
/// @param[in]  pDepth: populates Depth
///
/////////////////////////////////////////////////////////////////////////////////////
void GmmLib::GmmTextureCalc::GetCompressionBlockDimensions(GMM_RESOURCE_FORMAT Format,
                                                           uint32_t *          pWidth,
                                                           uint32_t *          pHeight,
                                                           uint32_t *          pDepth)
{

    GMM_DPF_ENTER;
    __GMM_ASSERT(pWidth && pHeight && pDepth);

    if(pWidth && pHeight && pDepth)
    {
        if((Format > GMM_FORMAT_INVALID) && (Format < GMM_RESOURCE_FORMATS))
        {
            *pWidth  = pGmmGlobalContext->GetPlatformInfo().FormatTable[Format].Element.Width;
            *pHeight = pGmmGlobalContext->GetPlatformInfo().FormatTable[Format].Element.Height;
            *pDepth  = pGmmGlobalContext->GetPlatformInfo().FormatTable[Format].Element.Depth;
        }
        else
        {
            *pWidth  = 1;
            *pHeight = 1;
            *pDepth  = 1;
        }
    }
    GMM_DPF_EXIT;
}

/////////////////////////////////////////////////////////////////////////////////////
/// This function Convert from d3d tile (64KB) to h/w tile
///
/// @param[in]  pTexInfo: ::GMM_TEXTURE_INFO
/// @param[in/out]  pColFactor: populates Width
/// @param[in/out]  pRowFactor: populates Height
/// @param[out]  true on Success else false
///
/////////////////////////////////////////////////////////////////////////////////////
bool GmmLib::GmmTextureCalc::GmmGetD3DToHwTileConversion(GMM_TEXTURE_INFO *pTexInfo,
                                                         uint32_t *        pColFactor,
                                                         uint32_t *        pRowFactor)
{
    uint32_t i   = 0;
    uint32_t Bpp = pTexInfo->BitsPerPixel;

    // check for  unsupported bpp
    if(!(Bpp == 8 || Bpp == 16 || Bpp == 32 || Bpp == 64 || Bpp == 128))
    {
        __GMM_ASSERT(false);
        goto EXIT_ERROR;
    }

    // for TileYS, no conversion
    if(pTexInfo->Flags.Info.TiledYs || pTexInfo->Flags.Info.Linear)
    {
        *pColFactor = 1;
        *pRowFactor = 1;
    }
    else if(pTexInfo->Flags.Info.TiledY)
    {
        // Logic for non-MSAA
        {
            //      Bpp = 8      => i = 0           , Bpp = 16 => i = 1, ...
            // Log2(Bpp = 8) = 3 => i = Log2(8) - 3.

            i           = __GmmLog2(Bpp) - 3;
            *pColFactor = __GmmTileYConversionTable[i][0];
            *pRowFactor = __GmmTileYConversionTable[i][1];
        }

        // Logic for MSAA
        if(pTexInfo->MSAA.NumSamples > 1)
        {

            // For MSAA, the DirectX tile dimensions change, using the table __GmmMSAAConversion.
            uint32_t W = __GmmMSAAConversion[__GmmLog2(pTexInfo->MSAA.NumSamples)][0];
            uint32_t H = __GmmMSAAConversion[__GmmLog2(pTexInfo->MSAA.NumSamples)][1];

            // For the new DirectX tile dimensions the new Col and Row conversion factors are:
            *pColFactor /= W;
            *pRowFactor /= H;
        }
    }
    else
    {
        // unsupported format.
        __GMM_ASSERT(false);
        goto EXIT_ERROR;
    }

    return true;

EXIT_ERROR:
    *pColFactor = 0;
    *pRowFactor = 0;
    return false;
}
