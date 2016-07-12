#ifndef CGFWX_H
#define CGFWX_H

#ifdef __cplusplus
extern "C" {
#endif

//  Good, Fast Wavelet Codec "GFWX" v1.1 c implementation
//  ----------------------------------
//  August 1, 2016
//  Author: Graham Fyffe <fyffe@ict.usc.edu> or <gfyffe@gmail.com>
//  Website: www.gfwx.org
//  C porting made by Richer Archambaut richer.arc@gmail.com
//  Features:
//  - FAST
//  - compression ratio similar to JPEG 2000
//  - under 1000 lines of code, with no external libraries
//  - 100% lossless at max quality
//  - low quality looks interpolated instead of blocky
//  - progressive decoding with optional downsampling
//  - supports uint8_t, int8_t, uint16_t, int16_t
//  - supports 1 to 65536 interleaved channels
//  - supports 1 to 65536 non-interleaved layers
//  - optional Bayer mode to compress Bayer data more
//  - optional chroma downsampling, even in Bayer mode
//  - optional user-programmable color/channel transform
//  - optional slightly less fast mode to compress more
//  - imageData can be any class with a pointer-like interface
//  - thoroughly tested using several pictures of cats
//
//  GFWX is released under the 3-clause BSD license:
//
//  Copyright (c) 2015, University of Southern California. All rights reserved. Redistribution and use in source and binary forms,
//  with or without modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in
//     the documentation and/or other materials provided with the distribution.
//
//  3. Neither the name of the organization nor the names of its contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <math.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#if defined(_OPENMP)
#include <omp.h>
#endif

enum{
		QualityMax = 1024,		// compress with QualityMax for 100% lossless, or less than QualityMax for lossy
		ThreadIterations = 8,	// OMP settings tuned on my machine with large images
		BitDepthAuto = 0, BlockDefault = 7, BlockMax = 30,
		FilterLinear = 0, FilterCubic = 1, QuantizationScalar = 0, EncoderTurbo = 0, EncoderFast = 1, EncoderContextual = 2,
		IntentGeneric = 0, IntentMono = 1, IntentBayerRGGB = 2, IntentBayerBGGR = 3, IntentBayerGRBG = 4, IntentBayerGBRG = 5, IntentBayerGeneric = 6,
		IntentRGB = 7, IntentRGBA = 8, IntentRGBApremult = 9, IntentBGR = 10, IntentBGRA = 11, IntentBGRApremult = 12, IntentCMYK = 13,
		ResultOk = 0, ErrorOverflow = -1, ErrorMalformed = -2, ErrorTypeMismatch = -3
};

typedef struct header Header;	// use the empty constructor to fetch headers before decompressing, and use the parameterized constructor when compressing

Header * Header_New();
Header * HeaderNew(int sizex, int sizey, int layers, int channels, int bitDepth, int quality, int chromaScale, int blockSize, int filter, int quantization, int encoder, int intent);

	
typedef struct bits Bits;

Bits* Bits_New(uint32_t * buffer, uint32_t * bufferEnd);

uint32_t Bits_GetBits(Bits* ctx, int bits);

void Bits_PutBits(Bits* ctx, uint32_t x, int bits);

uint32_t Bits_GetZeros(Bits* ctx, uint32_t maxZeros);

void Bits_FlushWriteWord(Bits* ctx);

void Bits_FlushReadWord(Bits* ctx);

#ifdef __cplusplus
}
#endif

#endif