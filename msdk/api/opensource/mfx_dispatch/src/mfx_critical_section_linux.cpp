// Copyright (c) 2017 Intel Corporation
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "mfx_critical_section.h"
#include <sched.h>

#define MFX_WAIT() sched_yield()

// static section of the file
namespace
{

enum
{
    MFX_SC_IS_FREE = 0,
    MFX_SC_IS_TAKEN = 1
};

} // namespace

namespace MFX
{

mfxU32 mfxInterlockedCas32(mfxCriticalSection *pCSection, mfxU32 value_to_exchange, mfxU32 value_to_compare)
{
    mfxU32 previous_value;

    asm volatile ("lock; cmpxchgl %1,%2"
                  : "=a" (previous_value)
                  : "r" (value_to_exchange), "m" (*pCSection), "0" (value_to_compare)
                  : "memory", "cc");
    return previous_value;
}

mfxU32 mfxInterlockedXchg32(mfxCriticalSection *pCSection, mfxU32 value)
{
    mfxU32 previous_value = value;

    asm volatile ("lock; xchgl %0,%1"
                  : "=r" (previous_value), "+m" (*pCSection)
                  : "0" (previous_value));
    return previous_value;
}

void mfxEnterCriticalSection(mfxCriticalSection *pCSection)
{
    while (MFX_SC_IS_TAKEN == mfxInterlockedCas32(pCSection,
                                                  MFX_SC_IS_TAKEN,
                                                  MFX_SC_IS_FREE))
    {
        MFX_WAIT();
    }
} // void mfxEnterCriticalSection(mfxCriticalSection *pCSection)

void mfxLeaveCriticalSection(mfxCriticalSection *pCSection)
{
    mfxInterlockedXchg32(pCSection, MFX_SC_IS_FREE);
} // void mfxLeaveCriticalSection(mfxCriticalSection *pCSection)

} // namespace MFX

