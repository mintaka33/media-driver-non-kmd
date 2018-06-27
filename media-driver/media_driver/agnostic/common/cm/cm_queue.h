/*
* Copyright (c) 2017, Intel Corporation
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
//! \file      cm_queue.h
//! \brief     Contains CmQueue declarations.
//!

#ifndef MEDIADRIVER_AGNOSTIC_COMMON_CM_CMQUEUE_H_
#define MEDIADRIVER_AGNOSTIC_COMMON_CM_CMQUEUE_H_

#include "cm_def.h"

enum CM_FASTCOPY_OPTION
{
    CM_FASTCOPY_OPTION_NONBLOCKING = 0x00,
    CM_FASTCOPY_OPTION_BLOCKING = 0x01,
    CM_FASTCOPY_OPTION_DISABLE_TURBO_BOOST = 0x02
};

namespace CMRT_UMD
{
class CmTask;
class CmEvent;
class CmThreadSpace;
class CmThreadGroupSpace;
class CmVebox;
class CmSurface2D;

//!
//! \brief      CmQueue class for task queue management.
//! \details    The CmQueue object represents a CM task queue. Each task
//!             represented by a CmTask object has one or more kernels that
//!             can be run concurrently. Each kernel can run in multiple
//!             threads concurrently. The CmQueue is an in-order queue.
//!             Tasks get executed according to the order they get enqueued.
//!             The next task doesn't start execution until the current task
//!             finishes. Hence an output of one task can be assumed to be
//!             available for use as input for any subsequent task in the
//!             queue.Each Enqueue generates a CmEvent. Task with thread
//!             space should call Enqueue with a valid thread space pointer,
//!             while task with group specification should call
//!             EnqueueWithGroup with a valid pTGS group space pointer.
//!
class CmQueue
{
public:
    //!
    //! \brief   Enqueue a task for execution with per-task thread space.
    //! \details This function enqueues a task represented by the CmTask object.
    //!          The kernels in the CmTask object may be run concurrently.
    //!          Tasks get executed according to the order they get enqueued.
    //!          This is a non-blocking call. It returns immediately without waiting
    //!          for GPU to start or finish execution. A CmEvent is generated each time
    //!          a task is enqueued. The CmEvent can be used to check the status of task.
    //!          The generated event needs to be managed and released by user.
    //!          Since event is not useful in some cases, runtime provides the capability
    //!          to avoid generating event.
    //!          If thread space is valid, the dependency defined by thread space will be honored.
    //! \param   [in] task
    //!          pointer to task to submit
    //! \param   [in,out] event
    //!          reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!          its value returned by runtime is NULL.
    //! \param   [in] threadSpace
    //!          pointer to thread space which can define the thread dependency within the task.
    //!          This is a per task thread space. If this task has multiple kernels, each kernel
    //!          will have the thread space of same dimension, same dependency etc. If it is nullptr,
    //!          there is no thread dependency and the maximum thread space width will be asssumed
    //!          to calculate the coordinates for each thread. For each kernel , the per kernel thread space
    //!          that is defined by calling CmKernel::AssociateThreadSpace() overwrites the per task thread space.
    //! \retval  CM_SUCCESS if the task is successfully enqueued.
    //! \retval  CM_OUT_OF_HOST_MEMORY if out of host memory
    //! \retval  CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t Enqueue(CmTask *task,
                                      CmEvent *&event,
                                      const CmThreadSpace *threadSpace = nullptr) = 0;

    //!
    //! \brief    Destroy the CmEvent generated by Enqueue.
    //! \details  Destroy the event object previously generated by Enqueue.
    //!           The CmEvent object can be destroyed even before the corresponding task flushed or finished.
    //!           If this happens, there is no way the app can get the task status.
    //! \param    [in] event
    //!           reference to pointer to event
    //! \retval   CM_SUCCESS if event destroyed successfully
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t DestroyEvent(CmEvent *&event) = 0;

    //!
    //! \brief    Enqueue the task with thread group space.
    //! \details
    //! \param    [in] task
    //!           pointer to task to submit
    //! \param    [in,out] event
    //!           reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!           its value returned by runtime is NULL.
    //! \param    [in] threadGroupSpace
    //!           pointer to thread group space which defines the dimensions of the task.
    //!           threadGroupSpace  can not be NULL.
    //! \retval   CM_SUCCESS if the task is successfully enqueued.
    //! \retval   CM_INVALID_ARG_VALUE if input task is not valid
    //! \retval   CM_EXCEED_MAX_KERNEL_PER_ENQUEUE if the task's kernel number exceeds limitation.
    //! \retval   CM_INVALID_THREAD_GROUP_SPACE if the thread group space specification is invalid.
    //! \retval   CM_THREAD_ARG_NOT_ALLOWED if user has per thread arguments
    //!
    CM_RT_API virtual int32_t
    EnqueueWithGroup(CmTask *task,
                     CmEvent *&event,
                     const CmThreadGroupSpace *threadGroupSpace = nullptr) = 0;

    //!
    //! \brief    Enqueues the kernel to copy from system(CPU) memory to video(GPU) memory.
    //! \details  This function enqueues a task, which contains a pre-defined kernel to copy from host
    //!           system memory to video surface.
    //!           This is a non-blocking call. A CmEvent is generated each time a task is enqueued.
    //!           The CmEvent can be used to check the status.
    //!           The host memory sysMem must be 16-Byte aligned and surface's width in bytes must be 16-Byte aligned as well.
    //! \param    [in] surface
    //!           surface as copy destination, surface's width in bytes must be 16-Byte aligned
    //! \param    [in] sysMem
    //!           host memory as copy source, must be 16-Byte aligned
    //! \param    [in,out] event
    //!           reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!           its value returned by runtime is NULL.
    //! \retval   CM_SUCCESS if the task is successfully enqueued
    //! \retval   CM_GPUCOPY_INVALID_WIDTH if surface's width in bytes is not 16-Byte aligned
    //!           or more than CM_MAX_GPUCOPY_SURFACE_WIDTH_IN_BYTE.
    //! \retval   CM_GPUCOPY_INVALID_SYSMEM if sysMem is not 16-Byte aligned.
    //! \retval   CM_GPUCOPY_OUT_OF_RESOURCE if runtime runs out of resources
    //! \retval   CM_GPUCOPY_INVALID_SIZE if surface's height is more than CM_MAX_GPUCOPY_SURFACE_HEIGHT
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueCopyCPUToGPU(CmSurface2D *surface,
                                                  const unsigned char *sysMem,
                                                  CmEvent* &event) = 0;

    //!
    //! \brief    Enqueues the kernel to copy from video(GPU) memory to system(CPU) memory.
    //! \details  This function enqueues a task, which contains a pre-defined kernel to copy from surface to system memory.
    //!           This is a non-blocking call. A CmEvent is generated each time a task is enqueued.
    //!           The CmEvent can be used to check the status or other data regarding the task execution.
    //!           The host memory sysMem must be 16-Byte aligned and surface's width in bytes must be 16-Byte aligned as well.
    //! \param    [in] surface
    //!           surface as copy source, surface's width in bytes must be 16-Byte aligned
    //! \param    [in] sysMem
    //!           host memory as copy destination, must be 16-Byte aligned
    //! \param    [in,out] event
    //!           reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!           its value returned by runtime is NULL.
    //! \retval   CM_SUCCESS if the task is successfully enqueued
    //! \retval   CM_GPUCOPY_INVALID_WIDTH if surface's width in bytes is not 16-Byte aligned
    //!           or more than CM_MAX_GPUCOPY_SURFACE_WIDTH_IN_BYTE.
    //! \retval   CM_GPUCOPY_INVALID_SYSMEM if sysMem is not 16-Byte aligned.
    //! \retval   CM_GPUCOPY_OUT_OF_RESOURCE if runtime runs out of resources
    //! \retval   CM_GPUCOPY_INVALID_SIZE if surface's height is more than CM_MAX_GPUCOPY_SURFACE_HEIGHT
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueCopyGPUToCPU(CmSurface2D *surface,
                                                  unsigned char *sysMem,
                                                  CmEvent *&event) = 0;

    //!
    //! \brief    Enqueues the kernel to initialize a 2D surface.
    //! \details  This function enqueues a task, which contains a pre-defined kernel to initialize a surface 2d
    //!           This is a non-blocking call. A CmEvent is generated each time a task is enqueued.
    //!           The CmEvent can be used to check the status or other data regarding the task execution.
    //! \param    [in] surface
    //!           surface to initialize
    //! \param    [in] initValue
    //!           value to fill the surface
    //! \param    [in,out] event
    //!           reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!           its value returned by runtime is NULL.
    //! \retval   CM_SUCCESS if the task is successfully enqueued
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueInitSurface2D(CmSurface2D *surface,
                                                   const uint32_t initValue,
                                                   CmEvent *&event) = 0;

    //!
    //! \brief    Enqueue the kernel to copy memory between surfaces.
    //! \details  This function enqueues a task, which contains a pre-defined kernel to copy memory between surfaces.
    //!           This is a non-blocking call. A CmEvent is generated each time a task is enqueued.
    //!           The CmEvent can be used to check the status or other data regarding the task execution.
    //!           The input and output surfaces should have the same width, height and format.
    //! \param    [in] inputSurface
    //!           surface as copy source
    //! \param    [in] outputSurface
    //!           surface as copy destination
    //! \param    [in] option
    //!           If it is "CM_FASTCOPY_OPTION_NONBLOCKING", it returns immediately without waiting for GPU to start or finish.\n
    //!           If it is "CM_FASTCOPY_OPTION_BLOCKING", this function will return until copy is finished indeed.\n
    //!           If it is "CM_FASTCOPY_OPTION_DISABLE_TURBO_BOOST", mdf turbo boost is disabled.
    //! \param    [in,out] event
    //!           reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!           its value returned by runtime is NULL.
    //! \retval   CM_SUCCESS if the task is successfully enqueued
    //! \retval   CM_GPUCOPY_INVALID_SURFACES if the input and output surfaces have different
    //!           width, height and format.
    //! \retval   CM_GPUCOPY_INVALID_SIZE if surface's height is more than CM_MAX_GPUCOPY_SURFACE_HEIGHT
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueCopyGPUToGPU(CmSurface2D *outputSurface,
                                                  CmSurface2D *inputSurface,
                                                  uint32_t option,
                                                  CmEvent *&event) = 0;

    //!
    //! \brief    Enqueues the kernel to copy memory between host memories.
    //! \details  This function enqueues a task, which contains a pre-defined kernel to copy memory from src to dest memory.
    //!           Both dstSysMem and srcSysMem need to be 16-Byte aligned.  The maximum size is determined by sytem's memory
    //!           and it should be less than CM_MAX_1D_SURF_WIDTH bytes which is 1G bytes now. If the copy size is less than
    //!           1K bytes, the event will not be generated and it is a blocking call.
    //!           For the size larger than 1K bytes, this is a non-blocking call.
    //!           A CmEvent is generated to check the status or other data regarding the task execution.
    //!           To avoid generating event, user can set the event as CM_NO_EVENT and pass it to this function
    //! \param    [in] dstSysMem
    //!           destination memory, must be 16-Byte aligned
    //! \param    [in] srcSysMem
    //!           source memory, must be 16-Byte aligned
    //! \param    [in] size
    //!           size of memory to copy in bytes
    //! \param    [in] option
    //!           If it is "CM_FASTCOPY_OPTION_NONBLOCKING", it returns immediately without waiting for GPU to start or finish.\n
    //!           If it is "CM_FASTCOPY_OPTION_BLOCKING", this function will return until copy is finished indeed.\n
    //!           If it is "CM_FASTCOPY_OPTION_DISABLE_TURBO_BOOST", mdf turbo boost is disabled.
    //! \param    [in,out] event
    //!           reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!           its value returned by runtime is NULL.
    //! \retval   CM_SUCCESS if the task is successfully enqueued
    //! \retval   CM_GPUCOPY_INVALID_SYSMEM if dstSysMem or srcSysMem is not 16-Byte aligned.
    //! \retval   CM_GPUCOPY_OUT_OF_RESOURCE if runtime runs out of resources
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueCopyCPUToCPU(unsigned char *dstSysMem,
                                                  unsigned char *srcSysMem,
                                                  uint32_t size,
                                                  uint32_t option,
                                                  CmEvent *&event) = 0;

    //!
    //! \brief    Enqueue the kernel to copy memory from system memory to video memory with width and height stride.
    //! \details  This function enqueues a task, which contains a pre-defined kernel to copy from system memory to a surface.
    //!           Depending on user "opiton", this is a non-blocking or blocking call.
    //!           A CmEvent is generated each time a task is enqueued. The CmEvent can be used to check the status or other data
    //!           regarding the task execution. To avoid generating event, user can set the event as CM_NO_EVENT and pass it to
    //!           this function. The host memory sysMem's width stride must be 16-Byte aligned, and height stride has no any
    //!           alignment restriction.
    //! \param    [in] surface
    //!           surface as copy destination
    //! \param    [in] sysMem
    //!           system memory as copy source must be 16-Byte aligned
    //! \param    [in] widthStride
    //!           width stride of memory stored in host memory, in bytes, must be 16-Byte aligned
    //! \param    [in] heightStride
    //!           height stride of memory stored in host memory, in bytes.
    //! \param    [in] option
    //!           If it is "CM_FASTCOPY_OPTION_NONBLOCKING", it returns immediately without waiting for GPU to start or finish.\n
    //!           If it is "CM_FASTCOPY_OPTION_BLOCKING", this function will return until copy is finished indeed.\n
    //!           If it is "CM_FASTCOPY_OPTION_DISABLE_TURBO_BOOST", mdf turbo boost is disabled.
    //! \param    [in,out] event
    //!           reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!           its value returned by runtime is NULL.
    //! \retval   CM_SUCCESS if the task is successfully enqueued
    //! \retval   CM_GPUCOPY_INVALID_WIDTH if surface's width in bytes is not 16-Byte aligned
    //!           or more than CM_MAX_GPUCOPY_SURFACE_WIDTH_IN_BYTE.
    //! \retval   CM_GPUCOPY_INVALID_SYSMEM if sysMem is not 16-Byte aligned.
    //! \retval   CM_GPUCOPY_OUT_OF_RESOURCE if runtime runs out of resources
    //! \retval   CM_GPUCOPY_INVALID_SIZE if surface's height is more than CM_MAX_GPUCOPY_SURFACE_HEIGHT
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueCopyCPUToGPUFullStride(CmSurface2D *surface,
                                                            const unsigned char *sysMem,
                                                            const uint32_t widthStride,
                                                            const uint32_t heightStride,
                                                            const uint32_t option,
                                                            CmEvent *& event) = 0;

    //!
    //! \brief    Enqueue the kernel to copy memory from video memory to system memory with width and height stride.
    //! \details  This function enqueues a task, which contains a pre-defined kernel to copy from surface to system memory.
    //!           Depending on user "opiton", this is a non-blocking or blocking call.
    //!           A CmEvent is generated each time a task is enqueued. The CmEvent can be used to check the status or other data
    //!           regarding the task execution. To avoid generating event, user can set the event as CM_NO_EVENT and pass it to
    //!           this function. The host memory sysMem's width stride must be 16-Byte aligned, and height stride has no any
    //!           alignment restriction.
    //! \param    [in] surface
    //!           surface as copy source
    //! \param    [in] sysMem
    //!           system memory as copy destination, must be 16-Byte aligned
    //! \param    [in] widthStride
    //!           width stride of memory stored in host memory, in bytes, must be 16-Byte aligned
    //! \param    [in] heightStride
    //!           height stride of memory stored in host memory, in bytes,
    //! \param    [in] option
    //!           If it is "CM_FASTCOPY_OPTION_NONBLOCKING", it returns immediately without waiting for GPU to start or finish.\n
    //!           If it is "CM_FASTCOPY_OPTION_BLOCKING", this function will return until copy is finished indeed.\n
    //!           If it is "CM_FASTCOPY_OPTION_DISABLE_TURBO_BOOST", mdf turbo boost is disabled.
    //! \param    [in,out] event
    //!           reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!           its value returned by runtime is NULL.
    //! \retval   CM_SUCCESS if the task is successfully enqueued
    //! \retval   CM_GPUCOPY_INVALID_STRIDE if stride is not 16-Byte aligned or less than surface�s width in bytes.
    //! \retval   CM_GPUCOPY_INVALID_SYSMEM if sysMem is not 16-Byte aligned.
    //! \retval   CM_GPUCOPY_INVALID_SIZE if surface's height is more than CM_MAX_GPUCOPY_SURFACE_HEIGHT
    //! \retval   CM_GPUCOPY_OUT_OF_RESOURCE if runtime runs out of resources
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueCopyGPUToCPUFullStride(CmSurface2D *surface,
                                                            unsigned char *sysMem,
                                                            const uint32_t widthStride,
                                                            const uint32_t heightStride,
                                                            const uint32_t option,
                                                            CmEvent *& event) = 0;

    //!
    //! \brief   Enqueue a task for execution with hints.
    //! \details This API is designed to saturate the EUs when running a large dependency kernel.
    //!          At least two kernels must exist in the task. The ideal case is at least one large dependency kernel
    //!          running with smaller kernels. The idea is to get the smaller kernels for free during the time it already
    //!          takes to execute the large dependency kernel. Each task can have up to CAP_KERNEL_COUNT_PER_TASK kernels.
    //!          The 0th bit of the hints indicates to use media object or media walker. Currently, only media object is valid.
    //!          The next bits indicate whether the next kernel is in the same or different kernel group.
    //!          For example, if the 1th bit is set then the second kernel is in a different kernel group from the first kernel,
    //!          if it is not set it is in the same kernel group. The kernels are interleaved between different kernel groups
    //!          and run concurrently. Within a kernel group, the kernels are dispatched in order. The kernel groups are dispatched
    //!          to separate sub-slices. The assumption is made that the kernel groups are comparable in kernel execution time.
    //!          There can be no dependency between different kernels; all kernels in the task should be independent of one another.
    //!          Additionally, pKernel->AssociateThreadSpace(CmThreadSpace*& pTS) must be called for each kernel.
    //!          A CmEvent is generated  to check the status or other data regarding the task execution.
    //!          To avoid generating event, user can set the event as CM_NO_EVENT and pass it to this function.
    //! \param   [in] task
    //!          pointer to task to submit
    //! \param   [in,out] event
    //!          reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!          its value returned by runtime is NULL.
    //! \param   [in] hints
    //!          Hints about work load from host to driver.
    //! \retval  CM_SUCCESS if the task is successfully enqueued.
    //! \retval  CM_OUT_OF_HOST_MEMORY if out of host memory
    //! \retval  CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueWithHints(CmTask *task,
                                               CmEvent *&event,
                                               uint32_t hints = 0) = 0;

    //!
    //! \brief   Enqueue a vebox task to vebox engine.
    //! \details This call submits a VEBOX task to VEBOX engine for execution.
    //!          Before this function is called, user need call CmDevice::CreateVebox() to create a CmVebox object,
    //!          and call the APIs in CmVebox class to set up VEBOX state and surfaces.
    //! \param   [in] vebox
    //!          Pointer to a CmVebox object.
    //! \param   [in,out] event
    //!          reference to pointer of event generated. If it is set as CM_NO_EVENT,
    //!          its value returned by runtime is NULL.
    //! \retval  CM_SUCCESS if the task is successfully enqueued.
    //! \retval  CM_OUT_OF_HOST_MEMORY if out of host memory
    //! \retval  CM_INVALID_ARG_VALUE if input vebox is not valid
    //! \retval  CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t EnqueueVebox(CmVebox *vebox, CmEvent *&event) = 0;
};
};//namespace

#endif  // #ifndef MEDIADRIVER_AGNOSTIC_COMMON_CM_CMQUEUE_H_
