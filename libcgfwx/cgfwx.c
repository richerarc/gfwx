#include <cgfwx/cgfwx.h>

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
