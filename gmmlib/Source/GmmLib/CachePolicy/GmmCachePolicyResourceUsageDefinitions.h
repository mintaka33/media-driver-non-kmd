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


// Generic Usages

// KMD Usages
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_BATCH_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_COMP_FRAME_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_CONTEXT_SWITCH_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_CURSOR )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_DISPLAY_STATIC_IMG_FOR_SMOOTH_ROTATION_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_DUMMY_PAGE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_GDI_SURFACE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_GENERIC_KMD_RESOURCE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_GFX_RING )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_GTT_TRANSFER_REGION )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HW_CONTEXT )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_KMD_STAGING_SURFACE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MBM_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_NNDI_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OVERLAY_MBM )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_PRIMARY_SURFACE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SCREEN_PROTECTION_INTERMEDIATE_SURFACE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SHADOW_SURFACE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SM_SCRATCH_STATE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_STATE_MANAGER_KERNEL_STATE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_STATUS_PAGE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TIMER_PERF_QUEUE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_UNMAP_PAGING_RESERVED_GTT_DMA_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VSC_BATCH_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_WA_BATCH_BUFFER )
//
// 3D Usages
//
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_UMD_BATCH_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_BINDING_TABLE_POOL )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_CCS )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_CONSTANT_BUFFER_POOL )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_DEPTH_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_DISPLAYABLE_RENDER_TARGET )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_GEN9_UNENCRYPTED_DISPLAYABLE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_GATHER_POOL )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HEAP_SURFACE_STATE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HEAP_DYNAMIC_STATE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HEAP_GENERAL_STATE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HEAP_GENERAL_STATE_UC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HEAP_STATELESS_DATA_PORT )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HEAP_INDIRECT_OBJECT )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HEAP_INSTRUCTION )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HIZ )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_INDEX_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MCS )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_PUSH_CONSTANT_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_PULL_CONSTANT_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_QUERY )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_RENDER_TARGET )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SHADER_RESOURCE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_STAGING )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_STENCIL_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_STREAM_OUTPUT_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILE_POOL )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MOCS_62 )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_L3_EVICTION )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_L3_EVICTION_SPECIAL )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_CCS_MEDIA_WRITABLE )
//TODO: To substitute with Interface to set SCF once its available for UMDs
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SHADER_RESOURCE_LLC_BYPASS )

// Tiled Resource
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILED_CCS )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILED_DEPTH_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILED_HIZ )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILED_MCS )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILED_RENDER_TARGET )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILED_RENDER_TARGET_AND_SHADER_RESOURCE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILED_SHADER_RESOURCE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_TILED_UAV )

DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VERTEX_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OGL_WSTN_VERTEX_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_UAV )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_RENDER_TARGET_AND_SHADER_RESOURCE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_RENDER_TARGET_AND_SHADER_RESOURCE_PARTIALENCSURFACES )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_WDDM_HISTORY_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_CONTEXT_SAVE_RESTORE )

//
// CM USAGES
//
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_SurfaceState )
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_NO_L3_SurfaceState )
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_NO_LLC_ELLC_SurfaceState )
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_NO_LLC_SurfaceState )
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_NO_ELLC_SurfaceState )
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_NO_LLC_L3_SurfaceState )
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_NO_ELLC_L3_SurfaceState )
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_NO_CACHE_SurfaceState )
DEFINE_RESOURCE_USAGE( CM_RESOURCE_USAGE_L1_Enabled_SurfaceState )

//
// MP USAGES
//
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_BEGIN )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_DEFAULT )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_AGE3_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_EDRAM_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_EDRAM_AGE3_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_No_L3_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_No_LLC_L3_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_No_LLC_L3_AGE_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_No_LLC_eLLC_L3_AGE_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_PartialEnc_No_LLC_L3_AGE_SurfaceState )
DEFINE_RESOURCE_USAGE( MP_RESOURCE_USAGE_END )

// MHW - SFC USAGES
DEFINE_RESOURCE_USAGE( MHW_RESOURCE_USAGE_Sfc_CurrentOutputSurface )                                // SFC Output Surface
DEFINE_RESOURCE_USAGE( MHW_RESOURCE_USAGE_Sfc_CurrentOutputSurface_PartialEncSurface )              // SFC Output Surface for partial encrypted surfaces
DEFINE_RESOURCE_USAGE( MHW_RESOURCE_USAGE_Sfc_AvsLineBufferSurface )                                // SFC AVS Line buffer Surface
DEFINE_RESOURCE_USAGE( MHW_RESOURCE_USAGE_Sfc_IefLineBufferSurface )                                // SFC IEF Line buffer Surface

//
// CODEC USAGES
//
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_BEGIN_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_PRE_DEBLOCKING_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_PRE_DEBLOCKING_CODEC_PARTIALENCSURFACE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_POST_DEBLOCKING_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_ORIGINAL_UNCOMPRESSED_PICTURE_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_ORIGINAL_UNCOMPRESSED_PICTURE_DECODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_STREAMOUT_DATA_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_INTRA_ROWSTORE_SCRATCH_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_DEBLOCKINGFILTER_ROWSTORE_SCRATCH_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_REFERENCE_PICTURE_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MACROBLOCK_STATUS_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MFX_INDIRECT_BITSTREAM_OBJECT_DECODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MFX_INDIRECT_MV_OBJECT_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MFD_INDIRECT_IT_COEF_OBJECT_DECODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MFC_INDIRECT_PAKBASE_OBJECT_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_BSDMPC_ROWSTORE_SCRATCH_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MPR_ROWSTORE_SCRATCH_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_BITPLANE_READ_CODEC)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_AACSBIT_VECTOR_CODEC)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_DIRECTMV_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_CURR_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_REF_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_MV_DATA_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_HME_DOWNSAMPLED_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_HME_DOWNSAMPLED_ENCODE_DST )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ME_DISTORTION_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_BRC_ME_DISTORTION_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_PAK_OBJECT_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_FLATNESS_CHECK_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_MBENC_CURBE_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_MAD_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP8_BLOCK_MODE_COST_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP8_MB_MODE_COST_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP8_MBENC_OUTPUT_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP8_HISTOGRAM_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP8_L3_LLC_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MBDISABLE_SKIPMAP_CODEC)

DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MFX_STANDALONE_DEBLOCKING_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_MD_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_SAO_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_MV_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_STATUS_ERROR_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_LCU_ILDB_STREAMOUT_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP9_PROBABILITY_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP9_SEGMENT_ID_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP9_HVD_ROWSTORE_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VDENC_ROW_STORE_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VDENC_STREAMIN_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_MB_QP_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MACROBLOCK_ILDB_STREAM_OUT_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SSE_SRC_PIXEL_ROW_STORE_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SLICE_STATE_STREAM_OUT_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_CABAC_SYNTAX_STREAM_OUT_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_PRED_COL_STORE_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_PAK_IMAGESTATE_ENCODE)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_MBENC_BRC_ENCODE)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_MB_BRC_CONST_ENCODE)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_BRC_MB_QP_ENCODE)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_BRC_ROI_ENCODE)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_SLICE_MAP_ENCODE)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_WP_DOWNSAMPLED_ENCODE)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_VDENC_IMAGESTATE_ENCODE)
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_UNCACHED )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ELLC_ONLY )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ELLC_LLC_ONLY )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ELLC_LLC_L3 )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_BRC_HISTORY_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_SOFTWARE_SCOREBOARD_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ME_MV_DATA_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_MV_DISTORTION_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_4XME_DISTORTION_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_INTRA_DISTORTION_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_MB_STATS_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_PAK_STATS_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_PIC_STATE_READ_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_PIC_STATE_WRITE_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_COMBINED_ENC_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_BRC_CONSTANT_DATA_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_INTERMEDIATE_CU_RECORD_SURFACE_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_SCRATCH_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_LCU_LEVEL_DATA_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ENC_HISTORY_INPUT_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ENC_HISTORY_OUTPUT_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_DEBUG_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ENC_CONSTANT_TABLE_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ENC_CU_RECORD_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ENC_MV_TEMPORAL_BUFFER_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ENC_CU_PACKET_FOR_PAK_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ENC_BCOMBINED1_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SURFACE_ENC_BCOMBINED2_ENCODE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_FRAME_STATS_STREAMOUT_DATA_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_DEBLOCKINGFILTER_ROWSTORE_TILE_LINE_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_DEBLOCKINGFILTER_ROWSTORE_TILE_COLUMN_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_MD_TILE_LINE_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_MD_TILE_COLUMN_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_SAO_TILE_LINE_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HCP_SAO_TILE_COLUMN_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_VP9_PROBABILITY_COUNTER_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_HUC_VIRTUAL_ADDR_REGION_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_SIZE_STREAMOUT_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_COMPRESSED_HEADER_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_PROBABILITY_DELTA_BUFFER_CODEC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_END_CODEC )

// OCL Usages
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_BUFFER_CONST )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_BUFFER_CSR_UC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_BUFFER_CACHELINE_MISALIGNED )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_IMAGE )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_INLINE_CONST )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_INLINE_CONST_HDC )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_SCRATCH )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_PRIVATE_MEM )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_PRINTF_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_STATE_HEAP_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_SYSTEM_MEMORY_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_SYSTEM_MEMORY_BUFFER_CACHELINE_MISALIGNED )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_ISH_HEAP_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_TAG_MEMORY_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_TEXTURE_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_IMAGE_FROM_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_SELF_SNOOP_BUFFER )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_BUFFER_NO_LLC_CACHING )
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_OCL_IMAGE_NO_LLC_CACHING )

// Cross Adapter
DEFINE_RESOURCE_USAGE( GMM_RESOURCE_USAGE_XADAPTER_SHARED_RESOURCE )
