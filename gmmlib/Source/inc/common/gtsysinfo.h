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

// This header file describes the GT system info data structure and associated
// types and constants. It is for use by both producers and consumers of GT
// system info on all OS.

#ifndef __GT_SYS_INFO_H__
#define __GT_SYS_INFO_H__

#include <stdbool.h>
#include <stdint.h>

#pragma pack(push,1)

// Maximums which bound all supported GT
#define GT_MAX_SLICE                   (4)
#define GT_MAX_SUBSLICE_PER_SLICE      (8)
#define GT_MAX_VDBOX                   (2)
#define GT_MAX_VEBOX                   (2)

typedef struct GT_SUBSLICE_INFO
{
    bool                Enabled;            // determine if this SS is enabled.
    uint32_t            EuEnabledCount;     // Total Count of EU enabled on this SubSlice
    uint32_t            EuEnabledMask;      // Mask of EUs enabled on this SubSlice
} GT_SUBSLICE_INFO;

typedef struct GT_SLICE_INFO
{
    bool                 Enabled;                                    // determine if this slice is enabled.
    GT_SUBSLICE_INFO     SubSliceInfo[GT_MAX_SUBSLICE_PER_SLICE];    // SS details that belong to this slice.
    uint32_t             SubSliceEnabledCount;                       // No. of SS enabled in this slice
} GT_SLICE_INFO;

typedef struct GT_VEBOX_INFO
{
    union VEBoxInstances
    {
        struct VEBitStruct
        {
            uint32_t    VEBox0Enabled : 1;      // To determine if VEBox0 is enabled
            uint32_t    VEBox1Enabled : 1;      // To determine if VEBox1 is enabled
            uint32_t    Reserved      : 30;     // Reserved bits
        } Bits;

        uint32_t VEBoxEnableMask;               // Union for all VEBox instances. It can be used to know if any of the VEBOX is enabled.

    } Instances;

    union
    {
        struct
        {
            uint32_t    VEBox0   : 1;      // Set if VEBox0 supports SFC
            uint32_t    VEBox1   : 2;      // Set if VEBox1 supports SFC
            uint32_t    Reserved : 30;     // Reserved bits
        }SfcSupportedBits;

        uint32_t Value;

    } SFCSupport;                               // VEBOX support of Scalar & Format Converter;

    uint32_t NumberOfVEBoxEnabled;              // Number of bits set among bit 0-3 of VEBoxEnableMask; used on CNL

    bool     IsValid;                           // flag to check if VEBoxInfo is valid.

} GT_VEBOX_INFO;

typedef struct GT_VDBOX_INFO
{
    union VDBoxInstances
    {
        struct VDBitStruct
        {
            uint32_t    VDBox0Enabled : 1;      // To determine if VDBox0 is enabled
            uint32_t    VDBox1Enabled : 1;      // To determine if VDBox1 is enabled
            uint32_t    Reserved      : 30;     // Reserved bits
        } Bits;

        uint32_t VDBoxEnableMask;               // Union for all VDBox instances. It can be used to know if any of the VDBOX is enabled.

    } Instances;

    union
    {
        struct
        {
            uint32_t    VDBox0   : 1;      // Set if VDBox0 supports SFC
            uint32_t    VDBox1   : 1;      // Set if VDBox1 supports SFC
            uint32_t    Reserved : 30;     // Reserved bits
        }SfcSupportedBits;

        uint32_t Value;

    } SFCSupport;                               // VDBOX support of Scalar & Format Converter;

    uint32_t NumberOfVDBoxEnabled;              // Number of bits set among bit 0-7 of VDBoxEnableMask;

    bool     IsValid;                            // flag to check if VDBoxInfo is valid.

} GT_VDBOX_INFO;

typedef struct GT_SQIDI_INFO
{
	uint32_t        NumberofSQIDI;                        // Total no. of enabled SQIDIs
	uint32_t        NumberofDoorbellPerSQIDI;             // Total no. of doorbells available per SQIDI unit
}GT_SQIDI_INFO;

typedef struct GT_SYSTEM_INFO
{
    // These fields should always hold valid values
    uint32_t        EUCount;                        // Total no. of enabled EUs
    uint32_t        ThreadCount;                    // total no of system threads available
    uint32_t        SliceCount;                     // Total no. of enabled slices
    uint32_t        SubSliceCount;                  // Total no. of enabled subslices
    uint64_t        L3CacheSizeInKb;                // Total L3 cache size in kilo bytes
    uint64_t        LLCCacheSizeInKb;               // Total LLC cache size in kilo bytes
    uint64_t        EdramSizeInKb;                  // Total EDRAM size in kilo bytes
    uint32_t        L3BankCount;                    // Total L3 banks across all slices. This is not bank count per slice. 
    uint32_t        MaxFillRate;                    // Fillrate with Alphablend (in Pix/Clk)
    uint32_t        EuCountPerPoolMax;              // Max EU count per pool
    uint32_t        EuCountPerPoolMin;              // Min EU count per pool

    uint32_t        TotalVsThreads;                 // Total threads in VS
    uint32_t        TotalHsThreads;                 // Total threads in HS
    uint32_t        TotalDsThreads;                 // Total threads in DS
    uint32_t        TotalGsThreads;                 // Total threads in GS
    uint32_t        TotalPsThreadsWindowerRange;    // Total threads in PS Windower Range

    // Note: The CSR size requirement is not clear at this moment. Till then the driver will set
    // the maximum size that should be sufficient for all platform SKUs. 
    uint32_t        CsrSizeInMb;                    // Total size that driver needs to allocate for CSR.

    /*------------------------------------*/
    // Below fields are required for proper allocation of scratch/private space for threads execution.
    // Threads scratch space has to be allocated based on native die config. So allocation has to be
    // done even for unfused or non-enabled slices/subslices/EUs. Since H/W doesn't provide us a way to know
    // about the native die config S/W will allocate based on max EU/S/SS.
    uint32_t        MaxEuPerSubSlice;               // Max available EUs per sub-slice. 
    uint32_t        MaxSlicesSupported;             // Max slices this platfrom can have.
    uint32_t        MaxSubSlicesSupported;          // Max total sub-slices this platform can have (not per slice)
    /*------------------------------------*/

    // Flag to determine if hashing is enabled. If enabled then one of the L3 banks will be disabled.
    // As a result 'L3BankCount' will be reduced by 1 bank during system info derivation routine.
    // Note: Only expected only in CNL (limited SKUs).
    bool           IsL3HashModeEnabled;

    // VEBox/VDBox info
    GT_VDBOX_INFO   VDBoxInfo;                      // VDBoxInfo provides details(enabled/disabled) of all VDBox instances.
    GT_VEBOX_INFO   VEBoxInfo;                      // VEBoxInfo provides details(enabled/disabled) of all VEBox instances.
    
    // SliceInfo provides the detailed breakdown of the Slice/Subslice/EU configuration. It is useful
    // for various WA that depend on the specific SSEU components enabled or disabled, but it is not
    // considered critically important to driver function at this time and may not be validly populated
    // on all platforms. IsDynamicallyPopulated indicates if SliceInfo has been validly populated.
    // IsDynamicallyPopulated only applies to SliceInfo.
    // TODO: Rename IsDynamicallyPopulated to something like SliceInfoIsValid to help clarify its
    // purpose. At the moment we are constrained by USC not to make any changes to the GT System
    // Info interface which require USC changes. USC currently references IsDynamicallyPopulated.
    GT_SLICE_INFO   SliceInfo[GT_MAX_SLICE];
    bool            IsDynamicallyPopulated;

	//SqidiInfo provides the detailed information for number of SQIDIs supported in GT.
	//It also provides total no. of doorbells available per SQIDI unit.
    GT_SQIDI_INFO   SqidiInfo;

    uint32_t        ReservedCCSWays;                // Reserved CCS ways provides value of reserved L3 ways for CCS when CCS is enabled.
	                                                // This is a hardcoded value as suggested by HW. No MMIO read is needed for same.

} GT_SYSTEM_INFO, *PGT_SYSTEM_INFO;


#pragma pack(pop)

#endif //__GT_SYS_INFO_H__
