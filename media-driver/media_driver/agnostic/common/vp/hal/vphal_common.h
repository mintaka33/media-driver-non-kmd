/*
* Copyright (c) 2009-2017, Intel Corporation
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
//! \file     vphal_common.h
//! \brief    clarify common utilities for vphal
//! \details  clarify common utilities for vphal including:
//!           some marcro, enum, union, structure, function
//!
#ifndef __VPHAL_COMMON_H__
#define __VPHAL_COMMON_H__

#if EMUL || VPHAL_LIB

#include "support.h"

#endif  // EMUL || VPHAL_LIB

#include "mos_os.h"

#ifdef __cplusplus
extern "C" {
#endif

//!
//! \def DBG_TEXT(txt)
//! Allow certain \a txt fields to be present only on DEBUG builds.
//!
#if _DEBUG
#define DBG_TEXT(txt) txt
#else
#define DBG_TEXT(txt) ""
#endif

#ifndef ABS
//!
//! \def ABS( x )
//! Calcualte absolute value of \a x.
//!
#define ABS(a)      (((a) < 0) ? (-(a)) : (a))
#endif

//!
//! \def OUT_OF_BOUNDS(a, min, max)
//! Calcualte if \a a out the range of  [\a min, \a max].
//!
#define OUT_OF_BOUNDS(a, min, max) ( ((a) < (min)) || ((a) > (max)) )

//!
//! \def WITHIN_BOUNDS(a, min, max)
//! Calcualte if \a a within the range of  [\a min, \a max].
//!
#define WITHIN_BOUNDS(a, min, max) ( ((a) >= (min)) && ((a) <= (max)) )

//!
//! \def VPHAL_ABS(x)
//! Calcualte the Abslute value of \a x.
//!
#define VPHAL_ABS(x)               (((x) > 0) ? (x) : -(x))

#define VPHAL_PI                   3.14159265358979324f //!< Definition the const pi

//!
//! \def SAME_SIZE_RECT(rect1, rect2)
//! Compare if the size of two rectangles is the same
//!
#define SAME_SIZE_RECT(rect1, rect2)                                            \
    (((rect1).right  - (rect1).left == (rect2).right  - (rect2).left) &&        \
     ((rect1).bottom - (rect1).top  == (rect2).bottom - (rect2).top))

//!
//! \def RECT1_EQUALS_RECT2(rect1, rect2)
//! Compare if two rectangles has the same coordinate
//!
#define RECT1_EQUALS_RECT2(rect1, rect2)                                        \
    (((rect1).left  == (rect2).left)  && ((rect1).top    == (rect2).top) &&     \
     ((rect1).right == (rect2).right) && ((rect1).bottom == (rect2).bottom))

//!
//! \def RECT1_CONTAINS_RECT2(rect1, rect2)
//! Compare if rectangle \a rect1 contains rectangle \a rect2 in coordinate
//!
#define RECT1_CONTAINS_RECT2(rect1, rect2)                                      \
    (((rect1).left  <= (rect2).left)  && ((rect1).top    <= (rect2).top) &&     \
     ((rect1).right >= (rect2).right) && ((rect1).bottom >= (rect2).bottom))

//!
//! \def RECT1_OUTSIDE_RECT2(rect1, rect2)
//! Compare if the rectangle \a rect1 is outside the rectangle \a rect2 at least partly in coordinate
//!
#define RECT1_OUTSIDE_RECT2(rect1, rect2)                                       \
    (((rect1).left  >= (rect2).right) || ((rect1).top    >= (rect2).bottom) ||  \
     ((rect1).right <= (rect2).left)  || ((rect1).bottom <= (rect2).top))

//!
// \def DEGREE_TO_RADIAN(degree)
// Convert a degree value \a degree to radian.
//!
#define DEGREE_TO_RADIAN(degree)   (degree) * (VPHAL_PI) / 180
#define AVS_TBL_COEF_PREC         6           //!< Table coef precision (after decimal point
#define SAME_SAMPLE_THRESHOLD     1000        //!< When checking whether 2 timestamps are the same, leave room for some error

//!
//! \def MEDIA_IS_HDCONTENT(dwWidth, dwHeight)
//! Determine if the size of content is HD
//!
#define MEDIA_SDCONTENT_MAX_WIDTH        720
#define MEDIA_SDCONTENT_MAX_PAL_HEIGHT   576
#define MEDIA_SDCONTENT_MAX_SW_WIDTH     768
#define MEDIA_IS_HDCONTENT(dwWidth, dwHeight)    ((dwWidth > MEDIA_SDCONTENT_MAX_SW_WIDTH) || (dwHeight > MEDIA_SDCONTENT_MAX_PAL_HEIGHT))

//! \brief  Surface cache attributes
//!
#define VPHAL_SET_SURF_MEMOBJCTL(VpField, GmmUsageEnum)                                                         \
    {                                                                                                           \
        Usage = GmmUsageEnum;                                                                                   \
        MemObjCtrl = pOsInterface->pfnCachePolicyGetMemoryObject(Usage);                                        \
        VpField = MemObjCtrl.DwordValue;                                                                        \
    }

#define VPHAL_MEMORY_OBJECT_CONTROL     uint32_t

typedef struct _VPHAL_COMPOSITE_CACHE_CNTL
{
    bool                           bL3CachingEnabled;
    VPHAL_MEMORY_OBJECT_CONTROL    PrimaryInputSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL    InputSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL    TargetSurfMemObjCtl;
} VPHAL_COMPOSITE_CACHE_CNTL, *PVPHAL_COMPOSITE_CACHE_CNTL;

typedef struct _VPHAL_DNDI_CACHE_CNTL
{
    bool                            bL3CachingEnabled;
    VPHAL_MEMORY_OBJECT_CONTROL     CurrentInputSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     PreviousInputSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     STMMInputSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     STMMOutputSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     DnOutSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     CurrentOutputSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     StatisticsOutputSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     LaceOrAceOrRgbHistogramSurfCtrl;
    VPHAL_MEMORY_OBJECT_CONTROL     AlphaOrVignetteSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     SkinScoreSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     LaceLookUpTablesSurfMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL     Vebox3DLookUpTablesSurfMemObjCtl;
} VPHAL_DNDI_CACHE_CNTL, *PVPHAL_DNDI_CACHE_CNTL;

typedef struct _VPHAL_LACE_CACHE_CNTL
{
    // LACE would reuse the bL3CachingEnabled of VEBOX's cache control
    VPHAL_MEMORY_OBJECT_CONTROL    FrameHistogramSurfaceMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL    AggregatedHistogramSurfaceMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL    StdStatisticsSurfaceMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL    PwlfInSurfaceMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL    PwlfOutSurfaceMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL    WeitCoefSurfaceMemObjCtl;
    VPHAL_MEMORY_OBJECT_CONTROL    GlobalToneMappingCurveLUTSurfaceMemObjCtl;
} VPHAL_LACE_CACHE_CNTL, *PVPHAL_LACE_CACHE_CNTL;

//!
//! \brief  Feature specific cache control settings
//!
typedef struct _VPHAL_RENDER_CACHE_CNTL
{
    // Input
    bool                        bDnDi;
    bool                        bCompositing;
    bool                        bLace;

    // Output
    VPHAL_DNDI_CACHE_CNTL        DnDi;
    VPHAL_COMPOSITE_CACHE_CNTL   Composite;
    VPHAL_LACE_CACHE_CNTL        Lace;
} VPHAL_RENDER_CACHE_CNTL, *PVPHAL_RENDER_CACHE_CNTL;

//!
//! \brief Vphal Rotation Mode enum
//!
typedef enum _VPHAL_ROTATION
{
    VPHAL_ROTATION_IDENTITY               = 0,   //!< Rotation 0 degrees
    VPHAL_ROTATION_90                        ,   //!< Rotation 90 degrees
    VPHAL_ROTATION_180                       ,   //!< Rotation 180 degrees
    VPHAL_ROTATION_270                       ,   //!< Rotation 270 degrees
    VPHAL_MIRROR_HORIZONTAL                  ,   //!< Horizontal Mirror
    VPHAL_MIRROR_VERTICAL                    ,   //!< Vertical Mirror
    VPHAL_ROTATE_90_MIRROR_VERTICAL          ,   //!< 90 + V Mirror
    VPHAL_ROTATE_90_MIRROR_HORIZONTAL            //!< 90 + H Mirror
} VPHAL_ROTATION;

//!
//! \brief Vphal Palette Type Mode enum
//!
typedef enum _VPHAL_PALETTE_TYPE
{
    VPHAL_PALETTE_NONE    = 0,
    VPHAL_PALETTE_YCbCr_8,                    //!< 8-bit depth - AYUV palette
    VPHAL_PALETTE_ARGB_8,                     //!< 8-bit depth - ARGB palette
    VPHAL_PALETTE_AVYU_8                      //!< 8-bit depth - YUY2 palette
} VPHAL_PALETTE_TYPE;

//!
//! \brief Vphal Chroma Siting enum
//!
typedef enum _VPHAL_CHROMA_SITING
{
    CHROMA_SITING_NONE          = 0,
    CHROMA_SITING_HORZ_LEFT     = 1 << 0,
    CHROMA_SITING_HORZ_CENTER   = 1 << 1,
    CHROMA_SITING_HORZ_RIGHT    = 1 << 2,
    CHROMA_SITING_VERT_TOP      = 1 << 4,
    CHROMA_SITING_VERT_CENTER   = 1 << 5,
    CHROMA_SITING_VERT_BOTTOM   = 1 << 6,
} VPHAL_CHROMA_SITING;

//!
//! \brief Vphal Output chroma configuration enum
//!
typedef enum _VPHAL_CHROMA_SUBSAMPLING
{
    CHROMA_SUBSAMPLING_TOP_CENTER       = 0,
    CHROMA_SUBSAMPLING_CENTER_CENTER,
    CHROMA_SUBSAMPLING_BOTTOM_CENTER,
    CHROMA_SUBSAMPLING_TOP_LEFT,
    CHROMA_SUBSAMPLING_CENTER_LEFT,
    CHROMA_SUBSAMPLING_BOTTOM_LEFT
} VPHAL_CHROMA_SUBSAMPLING;

//!
//! \brief Vphal Gamma Values configuration enum
//!
typedef enum _VPHAL_GAMMA_VALUE
{
    GAMMA_1P0 = 0,
    GAMMA_2P2,
    GAMMA_2P6
} VPHAL_GAMMA_VALUE;

typedef enum _VPHAL_DP_ROTATION_MODE
{
    VPHAL_DP_ROTATION_NV12_AVG            = 0,   //!< nv12 -> yuy2 by chroma average
    VPHAL_DP_ROTATION_NV12_NV12              ,   //!< nv12 -> nv12
    VPHAL_DP_ROTATION_NV12_REP               ,   //!< nv12 -> yuy2 by chroma repeat
    VPHAL_DP_ROTATION_NV12_YUY2_NOT_SET          //!< nv12 -> yuy2 by chroma average or repeat, decided by scaling mode
} VPHAL_DP_ROTATION_MODE;

//!
//! \brief Surface types enum
//!        IMPORTANT : SurfaceType_Layer[] must be updated to match this enum type
//!
typedef enum _VPHAL_SURFACE_TYPE
{
    SURF_NONE = 0,
    SURF_IN_BACKGROUND,
    SURF_IN_PRIMARY,
    SURF_IN_SUBSTREAM,
    SURF_IN_REFERENCE,
    SURF_OUT_RENDERTARGET,
    SURF_TYPE_COUNT                 //!< Keep this line at the end
} VPHAL_SURFACE_TYPE;
C_ASSERT(SURF_TYPE_COUNT == 6);     //!< When adding, update assert & vphal_solo_scenario.cpp

//!
//! \brief Color Spaces enum
//!
typedef enum _VPHAL_CSPACE
{
    CSpace_None     = -5        ,   //!< Unidentified
    CSpace_Source   = -4        ,   //!< Current source Color Space

    // Groups of Color Spaces
    CSpace_RGB      = -3        ,   //!< sRGB
    CSpace_YUV      = -2        ,   //!< YUV BT601 or BT709 - non xvYCC
    CSpace_Gray     = -1        ,   //!< Gray scale image with only Y component
    CSpace_Any      =  0        ,   //!< Any

    // Specific Color Spaces
    CSpace_sRGB                 ,   //!< RGB - sRGB       -   RGB[0,255]
    CSpace_stRGB                ,   //!< RGB - stRGB      -   RGB[16,235]
    CSpace_BT601                ,   //!< YUV BT.601 Y[16,235] UV[16,240]
    CSpace_BT601_FullRange      ,   //!< YUV BT.601 Y[0,255]  UV[-128,+127]
    CSpace_BT709                ,   //!< YUV BT.709 Y[16,235] UV[16,240]
    CSpace_BT709_FullRange      ,   //!< YUV BT.709 Y[0,255]  UV[-128,+127]
    CSpace_xvYCC601             ,   //!< xvYCC 601 Y[16,235]  UV[16,240]
    CSpace_xvYCC709             ,   //!< xvYCC 709 Y[16,235]  UV[16,240]
    CSpace_BT601Gray            ,   //!< BT.601 Y[16,235]
    CSpace_BT601Gray_FullRange  ,   //!< BT.601 Y[0,255]
    CSpace_BT2020               ,   //!< BT.2020 YUV Limited Range 10bit Y[64, 940] UV[64, 960]
    CSpace_BT2020_FullRange     ,   //!< BT.2020 YUV Full Range 10bit [0, 1023]
    CSpace_BT2020_RGB           ,   //!< BT.2020 RGB Full Range 10bit [0, 1023]
    CSpace_BT2020_stRGB         ,   //!< BT.2020 RGB Studio Range 10bit [64, 940]
    CSpace_Count                    //!< Keep this at the end
} VPHAL_CSPACE;
C_ASSERT(CSpace_Count == 15);       //!< When adding, update assert & vphal_solo_scenario.cpp

//!
//! \def IS_COLOR_SPACE_BT2020_YUV(_a)
//! Check if the color space is BT2020 YUV
//!
#define IS_COLOR_SPACE_BT2020_YUV(_a)          (_a == CSpace_BT2020           || \
                                                _a == CSpace_BT2020_FullRange)

//!
//! \def IS_COLOR_SPACE_BT2020_RGB(_a)
//! Check if the color space is BT2020 RGB
//!
#define IS_COLOR_SPACE_BT2020_RGB(_a)          (_a == CSpace_BT2020_RGB       || \
                                                _a == CSpace_BT2020_stRGB)

//!
//! \def IS_COLOR_SPACE_BT2020(_a)
//! Check if the color space is BT2020
//!
#define IS_COLOR_SPACE_BT2020(_a)              (IS_COLOR_SPACE_BT2020_YUV(_a) || \
                                                IS_COLOR_SPACE_BT2020_RGB(_a))

//!
//! \def IS_RGB_LIMITED_RANGE(_a)
//! Check if RGB limited range
//!
#define IS_RGB_LIMITED_RANGE(_a)               (_a == CSpace_stRGB       || \
                                                _a == CSpace_BT2020_stRGB)

//!
//! \def IS_RGB_FULL_RANGE(_a)
//! Check if RGB full range
//!
#define IS_RGB_FULL_RANGE(_a)                  (_a == CSpace_sRGB       || \
                                                _a == CSpace_BT2020_sRGB)

//!
//! \def IS_YUV_LIMITED_RANGE(_a)
//! Check if YUV limited range
//!
#define IS_YUV_LIMITED_RANGE(_a)               (_a == CSpace_BT601       || \
                                                _a == CSpace_BT709       || \
                                                _a == CSpace_BT601Gray   || \
                                                _a == CSpace_BT2020)

//!
//! \def IS_YUV_FULL_RANGE(_a)
//! Check if YUV full range
//!
#define IS_YUV_FULL_RANGE(_a)                  (_a == CSpace_BT601_FullRange       || \
                                                _a == CSpace_BT709_FullRange       || \
                                                _a == CSpace_BT601Gray_FullRange   || \
                                                _a == CSpace_BT2020_FullRange)

//!
//! \brief Sample Type enum
//!
typedef enum _VPHAL_SAMPLE_TYPE
{
    SAMPLE_PROGRESSIVE,
    SAMPLE_SINGLE_TOP_FIELD,
    SAMPLE_SINGLE_BOTTOM_FIELD,
    SAMPLE_INTERLEAVED_EVEN_FIRST_TOP_FIELD,
    SAMPLE_INTERLEAVED_EVEN_FIRST_BOTTOM_FIELD,
    SAMPLE_INTERLEAVED_ODD_FIRST_TOP_FIELD,
    SAMPLE_INTERLEAVED_ODD_FIRST_BOTTOM_FIELD,
    SAMPLE_INVALID
} VPHAL_SAMPLE_TYPE;
C_ASSERT(SAMPLE_INVALID == 7);      //!< When adding, update assert & vphal_solo_scenario.cpp

//!
//! \brief DI Mode enum
//!
typedef enum _VPHAL_DI_MODE
{
    DI_MODE_BOB,
    DI_MODE_ADI
} VPHAL_DI_MODE;
C_ASSERT(DI_MODE_ADI == 1);         //!< When adding, update assert & vphal_solo_scenario.cpp

//!
//! \brief Blend Type enum
//!
typedef enum _VPHAL_BLEND_TYPE
{
    BLEND_NONE = 0,
    BLEND_SOURCE,
    BLEND_PARTIAL,
    BLEND_CONSTANT,
    BLEND_CONSTANT_SOURCE,
    BLEND_CONSTANT_PARTIAL
} VPHAL_BLEND_TYPE;
C_ASSERT(BLEND_CONSTANT == 3);      //!< When adding, update assert & vphal_solo_scenario.cpp

//!
//! \brief Scaling Mode enum
//!
typedef enum _VPHAL_SCALING_MODE
{
    VPHAL_SCALING_NEAREST,
    VPHAL_SCALING_BILINEAR,
    VPHAL_SCALING_AVS
} VPHAL_SCALING_MODE;
C_ASSERT(VPHAL_SCALING_AVS == 2);   //!< When adding, update assert & vphal_solo_scenario.cpp

typedef enum _VPHAL_SCALING_PREFERENCE
{
    VPHAL_SCALING_PREFER_SFC,          // SFC is default
    VPHAL_SCALING_PREFER_COMP,         // For Gen9/10, Composition AVS get better quality than SFC AVS
    VPHAL_SCALING_PREFER_SFC_FOR_VEBOX // if VEBOX is required, use VEBOX + SFC, otherwise use Composistion
} VPHAL_SCALING_PREFERENCE;
C_ASSERT(VPHAL_SCALING_PREFER_SFC_FOR_VEBOX == 2);   //!< When adding, update assert & vphal_solo_scenario.cpp

//!
//! \brief Gamut Mode enum
//!
typedef enum _VPHAL_GAMUT_MODE
{
    GAMUT_MODE_NONE,
    GAMUT_MODE_BASIC,
    GAMUT_MODE_ADVANCED
} VPHAL_GAMUT_MODE;
C_ASSERT(GAMUT_MODE_ADVANCED == 2); //!< When adding, update assert & vphal_solo_scenario.cpp

//!
//! \brief Composition Report Mode enum
//!
typedef enum _VPHAL_COMPOSITION_REPORT_MODE
{
    VPHAL_NO_COMPOSITION,                        //!< No composition , Error Reporting cases
    VPHAL_INPLACE_COMPOSITION,                   //!< Inplace Composition
    VPHAL_LEGACY_COMPOSITION                     //!< Legacy Composition
} VPHAL_COMPOSITION_REPORT_MODE;
//!
//! \brief DI Report Mode enum
//!
typedef enum _VPHAL_DI_REPORT_MODE
{
    VPHAL_DI_REPORT_PROGRESSIVE,                        //!< Progressive output
    VPHAL_DI_REPORT_BOB,                                //!< True BOB output
    VPHAL_DI_REPORT_ADI_BOB,                            //!< DNDI BOB output
    VPHAL_DI_REPORT_ADI,                                //!< ADI output
    VPHAL_DI_REPORT_FMD                                 //!< FMD output
} VPHAL_DI_REPORT_MODE;

//!
//! \brief Colorpack enum
//!
typedef enum _VPHAL_COLORPACK
{
    VPHAL_COLORPACK_420,
    VPHAL_COLORPACK_422,
    VPHAL_COLORPACK_444,
    VPHAL_COLORPACK_UNKNOWN
} VPHAL_COLORPACK, *PVPHAL_COLORPACK;

//!
//! \brief   Output Pipe Mode enum
//! \details The output pipe that writes the RenderTarget surface
//!
typedef enum _VPHAL_OUTPUT_PIPE_MODE
{
    VPHAL_OUTPUT_PIPE_MODE_INVALID = -1,                //!< None output pipe selected. This is an invalid state
    VPHAL_OUTPUT_PIPE_MODE_COMP    = 0,                 //!< Composition output pipe. RenderTarget will be written by Composition
    VPHAL_OUTPUT_PIPE_MODE_SFC     = 1,                 //!< SFC output pipe. RenderTarget will be written by SFC
    VPHAL_OUTPUT_PIPE_MODE_VEBOX   = 2                  //!< Vebox output pipe. RenderTarget will be written by Vebox
} VPHAL_OUTPUT_PIPE_MODE, *PVPHAL_OUTPUT_PIPE_MODE;

//!
//! \def SET_VPHAL_OUTPUT_PIPE(_a, _Pipe)
//! Set the output pipe
//!
#define SET_VPHAL_OUTPUT_PIPE(_a, _Pipe)              (_a->OutputPipe =  _Pipe)

//!
//! \def IS_VPHAL_OUTPUT_PIPE_INVALID(_a)
//! Sheck if the output pipe is invalid
//!
#define IS_VPHAL_OUTPUT_PIPE_INVALID(_a)              (_a->OutputPipe == VPHAL_OUTPUT_PIPE_MODE_INVALID)

//!
//! \def IS_VPHAL_OUTPUT_PIPE_COMP(_a)
//! Check if the output pipe is Composition
//!
#define IS_VPHAL_OUTPUT_PIPE_COMP(_a)                 (_a->OutputPipe == VPHAL_OUTPUT_PIPE_MODE_COMP)

//!
//! \def IS_VPHAL_OUTPUT_PIPE_SFC(_a)
//! Check if the output pipe is SFC
//!
#define IS_VPHAL_OUTPUT_PIPE_SFC(_a)                  (_a->OutputPipe == VPHAL_OUTPUT_PIPE_MODE_SFC)

//!
//! \def IS_VPHAL_OUTPUT_PIPE_VEBOX(_a)
//! Check if the output pipe is Vebox
//!
#define IS_VPHAL_OUTPUT_PIPE_VEBOX(_a)                (_a->OutputPipe == VPHAL_OUTPUT_PIPE_MODE_VEBOX)

//!
//! \def SET_VPHAL_COMPONENT(_a, _Component)
//! Set the Component
//!
#define SET_VPHAL_COMPONENT(_a, _Component)           (_a->Component  =  _Component)                     // Set the Component

//!
//! \def SET_VPHAL_MMC_STATE(_a, _bEnableMMC)
//! Set the Component
//!
#define SET_VPHAL_MMC_STATE(_a, _bEnableMMC)          (_a->bEnableMMC =  _bEnableMMC)                    // Set the Component

//!
//! Union   VPHAL_COLOR_SAMPLE_8
//! \brief  Vphal Color Sample 8 bit
//!
typedef union _VPHAL_COLOR_SAMPLE_8
{
    // ARGB
    struct
    {
        uint8_t    B;
        uint8_t    G;
        uint8_t    R;
        uint8_t    A;
    };

    // AVYU (YUY2 samples)
    struct
    {
        uint8_t    U;
        uint8_t    Y;
        uint8_t    V;
        uint8_t    a;
    };

    // AYCbCr (NV12 samples)
    struct
    {
        uint8_t    Cr;      //!< V
        uint8_t    Cb;      //!< U
        uint8_t    YY;      //!< Y
        uint8_t    Alpha;
    };

    uint32_t       dwValue;
} VPHAL_COLOR_SAMPLE_8, *PVPHAL_COLOR_SAMPLE_8;

//!
//! Union   VPHAL_COLOR_SAMPLE_16
//! \brief  Vphal Color Sample 16 bit
//!
typedef union _VPHAL_COLOR_SAMPLE_16
{
    // ARGB
    struct
    {
        int16_t      B;
        int16_t      G;
        int16_t      R;
        int16_t      A;
    };

    // AVYU
    struct
    {
        int16_t      U;
        int16_t      Y;
        int16_t      V;
        int16_t      a;
    };

    // AYCbCr
    struct
    {
        int16_t      Cr;      //!< V
        int16_t      Cb;      //!< U
        int16_t      YY;      //!< Y
        int16_t      Alpha;
    };

    uint32_t   Value;
} VPHAL_COLOR_SAMPLE_16, *PVPHAL_COLOR_SAMPLE_16;

//!
//! Structure VPHAL_PLANE_OFFSET
//! \brief  Vphal Plane OffSet
//!
typedef struct _VPHAL_PLANE_OFFSET
{
    int    iLockSurfaceOffset;//!< Locked surface offset
    int    iSurfaceOffset;    //!< Plane surface offset
    int    iXOffset;          //!< X offset - horizontal offset in pixels
    int    iYOffset;          //!< Y offset - vertical offset in pixels
} VPHAL_PLANE_OFFSET, *PVPHAL_PLANE_OFFSET;

//!
//! Structure VPHAL_PALETTE
//! \brief    Vphal Palette
//! \details  Palette entries:
//!           - 0 : empty palette
//!           - -1 : palette allocated but not loaded
//!           - >0 : palette in use
//!
typedef struct _VPHAL_PALETTE
{
    VPHAL_PALETTE_TYPE      PaletteType;        //!< Type of palette
    VPHAL_CSPACE            ColorSpace;         //!< ColorSpace of Palette
    bool                    bHasAlpha;          //!< Alpha is valid
    int32_t                 iTotalEntries;      //!< Palette entries allocated
    int32_t                 iNumEntries;        //!< Palette entries in use
    union
    {
        PVPHAL_COLOR_SAMPLE_8   pPalette8;
        PVPHAL_COLOR_SAMPLE_16  pPalette16;
    };
} VPHAL_PALETTE, *PVPHAL_PALETTE;

//!
//! Union   VPHAL_HALF_PRECISION_FLOAT
//! \brief  Vphal half precision float type
//!
typedef union _VPHAL_HALF_PRECISION_FLOAT
{
    struct
    {
        uint16_t      Mantissa : 10;
        uint16_t      Exponent : 5;
        uint16_t      Sign     : 1;
    };

    uint16_t value;
} VPHAL_HALF_PRECISION_FLOAT, PVPHAL_HALF_PRECISION_FLOAT;

//!
//! \brief    Performs Color Space Convert for Sample 8 bit
//! \details  Performs Color Space Convert from Src Color Spase to Dst Color Spase
//! \param    [out] pOutput
//!           Pointer to VPHAL_COLOR_SAMPLE_8
//! \param    [in] pInput
//!           Pointer to VPHAL_COLOR_SAMPLE_8
//! \param    [in] srcCspace
//!           Source Color Space 
//! \param    [in] dstCspace
//!           Dest Color Space 
//! \return   bool
//!           Return true if successful, otherwise false
//!
bool VpHal_CSC_8(
    VPHAL_COLOR_SAMPLE_8    *pOutput,
    VPHAL_COLOR_SAMPLE_8    *pInput,
    VPHAL_CSPACE            srcCspace,
    VPHAL_CSPACE            dstCspace);

//!
//! \brief
//! \details  Get CSC matrix in a form usable by Vebox, SFC and IECP kernels
//! \param    [in] SrcCspace
//!           Source Cspace
//! \param    [in] DstCspace
//!           Destination Cspace
//! \param    [out] pfCscCoeff
//!           [3x3] Coefficients matrix
//! \param    [out] pfCscInOffset
//!           [3x1] Input Offset matrix
//! \param    [out] pfCscOutOffset
//!           [3x1] Output Offset matrix
//! \return   void
//!
void VpHal_GetCscMatrix(
    VPHAL_CSPACE                        SrcCspace,
    VPHAL_CSPACE                        DstCspace,
    float*                              pfCscCoeff,
    float*                              pfCscInOffset,
    float*                              pfCscOutOffset);

//!
//! \brief    sinc
//! \details  Calculate sinc(x)
//! \param    [in] x
//!           float
//! \return   float
//!           sinc(x)
//!
float VpHal_Sinc(float x);

//!
//! \brief    Lanczos
//! \details  Calculate lanczos(x)
//!           Basic formula is:  lanczos(x)= VpHal_Sinc(x) * VpHal_Sinc(x / fLanczosT)
//! \param    [in] x
//!           float
//! \param    [in] dwNumEntries
//!           dword
//! \param    [in] fLanczosT
//! 
//! \return   float
//!           lanczos(x)
//!
float VpHal_Lanczos(
    float        x,
    uint32_t    dwNumEntries,
    float        fLanczosT);

typedef struct VPHAL_SURFACE           *PVPHAL_SURFACE;
typedef struct VPHAL_GET_SURFACE_INFO  *PVPHAL_GET_SURFACE_INFO;

//!
//! \brief    Get Surface Info from OsResource
//! \details  Update surface info in PVPHAL_SURFACE based on allocated OsResource
//! \param    [in] pOsInterface
//!           Pointer to MOS_INTERFACE
//! \param    [in] pInfo
//!           Pointer to VPHAL_GET_SURFACE_INFO
//! \param    [in,out] pSurface
//!           Pointer to VPHAL_SURFACE
//! \return   MOS_STATUS
//!           Return MOS_STATUS_SUCCESS if successful, otherwise failed
//!
MOS_STATUS VpHal_GetSurfaceInfo(
    PMOS_INTERFACE           pOsInterface,
    PVPHAL_GET_SURFACE_INFO  pInfo,
    PVPHAL_SURFACE           pSurface);

//!
//! \brief    Allocates the Surface
//! \details  Allocates the Surface
//!           - if the surface is not already allocated OR
//!           - resource dimenisions OR format changed
//! \param    [in] pOsInterface
//!           Pointer to MOS_INTERFACE
//! \param    [in,out] pSurface
//!           Pointer to VPHAL_SURFACE
//! \param    [in] pSurfaceName
//!           Pointer to surface name
//! \param    [in] Format
//!           Expected MOS_FORMAT
//! \param    [in] DefaultResType
//!           Expected Resource Type
//! \param    [in] DefaultTileType
//!           Expected Surface Tile Type
//! \param    [in] dwWidth
//!           Expected Surface Width
//! \param    [in] dwHeight
//!           Expected Surface Height
//! \param    [in] bCompressed
//!           Compressed surface or not
//! \param    [in] CompressionMode
//!           Compression Mode
//! \param    [out] pbAllocated
//!           true if allocated, false for not
//! \return   MOS_STATUS
//!           MOS_STATUS_SUCCESS if success. Error code otherwise
//!
MOS_STATUS VpHal_ReAllocateSurface(
    PMOS_INTERFACE          pOsInterface,                                       // [in]    Pointer to OS Interface
    PVPHAL_SURFACE          pSurface,                                           // [in/out]Pointer to surface
    PCCHAR                  pSurfaceName,                                       // [in]    Pointer to surface name
    MOS_FORMAT              Format,                                             // [in]    Surface Format
    MOS_GFXRES_TYPE         DefaultResType,                                     // [in]    Default Resource Type to use if resource has not be allocated yet
    MOS_TILE_TYPE           DefaultTileType,                                    // [in]    Default Resource Tile Type to use if resource has not be allocated yet
    uint32_t                dwWidth,                                            // [in]    Resource Width
    uint32_t                dwHeight,                                           // [in]    Resource Height
    bool                    pbCompressed,                                       // [in]    Flag indaicated reource is compressed or not
    MOS_RESOURCE_MMC_MODE   CompressionMode,                                    // [in]    Compression mode
    bool*                   pbAllocated);                                       // [out]   Flag indicating new allocation

//!
//! \brief    Reads the Surface contents and copy to the Dst Buffer
//! \details  Reads the Surface contents and copy to the Dst Buffer
//!           - 1 lock surface
//!           - 2 copy surface data to pDst
//!           - 3 unlock surface
//! \param    [in] pOsInterface
//!           Pointer to MOS_INTERFACE
//! \param    [in] pSurface
//!           Pointer to VPHAL_SURFACE
//! \param    [in] uBpp
//!           bit per pixel of surface contents
//! \param    [out] pDst
//!           output buffer to store Surface contents
//! \return   MOS_STATUS
//!           MOS_STATUS_SUCCESS if success. Error code otherwise
//!
MOS_STATUS VpHal_ReadSurface (
    PMOS_INTERFACE              pOsInterface,
    PVPHAL_SURFACE              pSurface,
    uint32_t                    uBpp,
    uint8_t*                    pDst);

//!
//! \brief    Copy Data from input Buffer to the Surface contents
//! \details  Copy Data from input Buffer to the Surface contents
//!           - 1 lock surface
//!           - 2 copy data from pSrc to Surface
//!           - 3 unlock surface
//! \param    [in] pOsInterface
//!           Pointer to MOS_INTERFACE
//! \param    [out] pSurface
//!           Pointer to VPHAL_SURFACE
//! \param    [in] uBpp
//!           bit per pixel of input buffer
//! \param    [in] pSrc
//!           Input buffer to store Surface contents
//! \return   MOS_STATUS
//!           MOS_STATUS_SUCCESS if success. Error code otherwise
//!
MOS_STATUS VpHal_WriteSurface (
    PMOS_INTERFACE              pOsInterface,
    PVPHAL_SURFACE              pSurface,
    uint32_t                    uBpp,
    const uint8_t*              pSrc);

//!
//! \brief    Get the color pack type of a surface
//! \details  Map mos surface format to color pack format and return.
//!           For unknown format return VPHAL_COLORPACK_UNKNOWN
//! \param    [in] Format
//!           MOS_FORMAT of a surface
//! \return   VPHAL_COLORPACK
//!           Color pack type of the surface
//!
VPHAL_COLORPACK VpHal_GetSurfaceColorPack (
    MOS_FORMAT                  Format);

//!
//! \brief    Decide whether Chroma up sampling is needed
//! \param    [in] pSource
//!           Pointer to Source Surface
//! \param    [in] pTarget
//!           Pointer to Target Surface
//! \return   bool
//!           Return true if Chroma up sampling is needed, otherwise false
//!
bool VpHal_IsChromaUpSamplingNeeded(
    PVPHAL_SURFACE          pSource,
    PVPHAL_SURFACE          pTarget);

//!
//! \brief    Decide whether Chroma down sampling is needed
//! \param    [in] pSource
//!           Pointer to Source Surface
//! \param    [in] pTarget
//!           Pointer to Target Surface
//! \return   bool
//!           Return true if Chroma down sampling is needed, otherwise false
//!
bool VpHal_IsChromaDownSamplingNeeded(
    PVPHAL_SURFACE          pSource,
    PVPHAL_SURFACE          pTarget);

//! \brief    Get the bit depth of a surface
//! \details  Get bit depth of input mos surface format and return.
//!           For unknown format return 0
//! \param    [in] Format
//!           MOS_FORMAT of a surface
//! \return   uint32_t
//!           Bit depth of the surface
//!
uint32_t VpHal_GetSurfaceBitDepth(
    MOS_FORMAT      Format);

//!
//! \brief      Get the scale ratio
//! \details    Get the scale ratio from input surface to output surface
//! \param      [in] pSource
//!             Pointer to input Surface
//! \param      [in] pTarget
//!             Pointer to output Surface
//! \param      [out] pfScaleX
//!             Pointer to scaling ratio x
//! \param      [out] pfScaleY
//!             Pointer to scaling ratio y
//! \return     void
//!
void VpHal_GetScalingRatio(
    PVPHAL_SURFACE              pSource,
    PVPHAL_SURFACE              pTarget,
    float*                      pfScaleX,
    float*                      pfScaleY);

//! \brief    Transfer float type to half precision float type
//! \details  Transfer float type to half precision float (16bit) type
//! \param    [in] fInput
//!           input FP32 number
//! \return   uint16_t
//!           half precision float value in bit
//!
uint16_t VpHal_FloatToHalfFloat(
    float     fInput);

//!
//! \brief    Initial the Type/TileType fields in Alloc Params structure
//! \details  Initial the Type/TileType fields in Alloc Params structure
//!           - Use the last type from GMM resource
//! \param    [in, out] pAllocParams
//!           Pointer to MOS_ALLOC_GFXRES_PARAMS
//! \param    [in] pSurface
//!           Pointer to VPHAL_SURFACE
//! \param    [in] DefaultResType
//!           Expected Resource Type
//! \param    [in] DefaultTileType
//!           Expected Surface Tile Type
//!
void VpHal_AllocParamsInitType(
    PMOS_ALLOC_GFXRES_PARAMS    pAllocParams,
    PVPHAL_SURFACE              pSurface,
    MOS_GFXRES_TYPE             DefaultResType,
    MOS_TILE_TYPE               DefaultTileType);

#ifdef __cplusplus
}
#endif

#endif  // __VPHAL_COMMON_H__
