#include <cgfwx/cgfwx.h>

//////////// ---------- Header ---------- ////////////

struct header{
		int sizex;
        int sizey;
        int layers;
        int channels;
        int bitDepth;
        int quality;
        int chromaScale;
        int blockSize;
        int filter;
        int quantization;
        int encoder;
        int intent;
        int version;
        int isSigned;
};

Header * Header_New(){
    Header *ctx = malloc(sizeof(Header));

    if (!ctx)
        return NULL;
    
    return ctx;
}

Header * HeaderNew(int sizex, int sizey, int layers, int channels, int bitDepth, int quality,
             int chromaScale, int blockSize, int filter, int quantization, int encoder, int intent){
    Header *ctx = malloc(sizeof(Header));

    if (!ctx)
        return NULL;
    
    ctx->sizex = sizex;
    ctx->sizey = sizey;
    ctx->layers = layers;
    ctx->channels = channels;
    ctx->bitDepth = bitDepth;
    ctx->quality = quality;
    ctx->chromaScale = chromaScale;
    ctx->blockSize = blockSize;
    ctx->filter = filter;
    ctx->quantization = quantization;
    ctx->encoder = encoder;
    ctx->intent = intent;

    return ctx;
}


//////////// ---------- Bits ---------- ////////////

struct bits{
		uint32_t * buffer, * bufferEnd;
		uint32_t writeCache;
		int indexBits;	// -1 indicates buffer overflow
};

Bits* Bits_New(uint32_t * buffer, uint32_t * bufferEnd){
    Bits *ctx = malloc(sizeof(Bits));

    if(!ctx)
        return NULL;

    ctx->buffer = buffer;
    ctx->bufferEnd = bufferEnd;
    ctx->indexBits = 0;
    ctx->writeCache = 0;

    return ctx;
}

uint32_t Bits_GetBits(Bits* ctx, int bits){
    uint32_t x;
    int newBits = ctx->indexBits + bits;
	if (ctx->buffer == ctx->bufferEnd)
		return ctx->indexBits = -1;        // signify overflow
	x = *ctx->buffer << ctx->indexBits;
	if (newBits >= 32) {
		++ctx->buffer;
		if ((newBits -= 32) > 0) {
			if (ctx->buffer == ctx->bufferEnd)
				return ctx->indexBits = -1;        // signify overflow
			x |= *ctx->buffer >> (32 - ctx->indexBits);
		}
	}
	ctx->indexBits = newBits;
	return x >> (32 - bits);
}

void Bits_PutBits(Bits* ctx, uint32_t x, int bits){
    int newBits = ctx->indexBits + bits;
	if (ctx->buffer == ctx->bufferEnd){
		newBits = -1;        // signify overflow
    }
	else if (newBits < 32){
		ctx->writeCache <<= bits;
        ctx->writeCache |= x;
    }
	else if (bits == 32 && newBits == 32) {
		newBits = 0;
		*(ctx->buffer++) = x;
	}
	else {
		newBits -= 32;
		*(ctx->buffer++) = (ctx->writeCache << (bits - newBits)) | (x >> newBits);
		ctx->writeCache = x;
	}
	ctx->indexBits = newBits;
}

uint32_t Bits_GetZeros(Bits* ctx, uint32_t maxZeros){
    int newBits = ctx->indexBits;
	if (ctx->buffer == ctx->bufferEnd)
		return ctx->indexBits = -1;        // signify overflow
	uint32_t b = *ctx->buffer;
	uint32_t x = 0;
	while (1) {
		if (newBits == 31) {
			++ctx->buffer;
			if ((b & 1u) || (++x == maxZeros)) {
				ctx->indexBits = 0;
				return x;
			}
			if (ctx->buffer == ctx->bufferEnd)
				return ctx->indexBits = -1;        // signify overflow
			b = *ctx->buffer;
			newBits = 0;
			continue;
		}
		if (((b << newBits) & (1u << 31)) || (++x == maxZeros)) {
			ctx->indexBits = newBits + 1;
			return x;
		}
		++newBits;
	}
}

void Bits_FlushWriteWord(Bits* ctx){
    Bits_PutBits(ctx, 0, (32 - ctx->indexBits) % 32);
}

void Bits_FlushReadWord(Bits* ctx){
    if (ctx->indexBits <= 0)
		return;
	++ctx->buffer;
	ctx->indexBits = 0;
}