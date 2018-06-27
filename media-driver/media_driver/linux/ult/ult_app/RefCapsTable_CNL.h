/*
* Copyright (c) 2018, Intel Corporation
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
#ifndef __REF_CAPS_TABLE_CNL_H__
#define __REF_CAPS_TABLE_CNL_H__

#include "driver_loader.h"

std::vector<FeatureID> refFeatureIDTable_CNL = {
    { VAProfileNone                   , VAEntrypointVideoProc  },
    { VAProfileNone                   , VAEntrypointStats      },
    { VAProfileMPEG2Simple            , VAEntrypointVLD        },
    { VAProfileMPEG2Simple            , VAEntrypointEncSlice   },
    { VAProfileMPEG2Main              , VAEntrypointVLD        },
    { VAProfileMPEG2Main              , VAEntrypointEncSlice   },
    { VAProfileH264Main               , VAEntrypointVLD        },
    { VAProfileH264Main               , VAEntrypointEncSlice   },
    { VAProfileH264Main               , VAEntrypointEncSliceLP },
    { VAProfileH264High               , VAEntrypointVLD        },
    { VAProfileH264High               , VAEntrypointEncSlice   },
    { VAProfileH264High               , VAEntrypointEncSliceLP },
    { VAProfileH264ConstrainedBaseline, VAEntrypointVLD        },
    { VAProfileH264ConstrainedBaseline, VAEntrypointEncSlice   },
    { VAProfileH264ConstrainedBaseline, VAEntrypointEncSliceLP },
    { VAProfileVC1Simple              , VAEntrypointVLD        },
    { VAProfileVC1Main                , VAEntrypointVLD        },
    { VAProfileVC1Advanced            , VAEntrypointVLD        },
    { VAProfileJPEGBaseline           , VAEntrypointVLD        },
    { VAProfileJPEGBaseline           , VAEntrypointEncPicture },
    { VAProfileVP8Version0_3          , VAEntrypointVLD        },
    { VAProfileVP8Version0_3          , VAEntrypointEncSlice   },
    { VAProfileHEVCMain               , VAEntrypointVLD        },
    { VAProfileHEVCMain               , VAEntrypointEncSlice   },
    { VAProfileHEVCMain               , VAEntrypointEncSliceLP },
    { VAProfileHEVCMain10             , VAEntrypointVLD        },
    { VAProfileHEVCMain10             , VAEntrypointEncSlice   },
    { VAProfileHEVCMain10             , VAEntrypointEncSliceLP },
    { VAProfileVP9Profile0            , VAEntrypointVLD        },
    { VAProfileVP9Profile2            , VAEntrypointVLD        },
    { VAProfileVP9Profile0            , VAEntrypointEncSliceLP },
};

#endif // __REF_CAPS_TABLE_CNL_H__
