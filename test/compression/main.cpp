#include <iomanip>
#include <fstream>
#include <sys/time.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "libgfwx/gfwx.hpp"

#define ULONGLONG unsigned long long

unsigned int filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

unsigned long long GetTickCount64(void)
{
    ULONGLONG ticks = 0;
#if defined(__linux__)
    struct timespec ts;

    if (!clock_gettime(CLOCK_MONOTONIC_RAW, &ts))
        ticks = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);

#elif defined(_WIN32)
    ticks = (ULONGLONG) GetTickCount();
#else
    /**
     * FIXME: this is relative to the Epoch time, and we
     * need to return a value relative to the system uptime.
     */
    struct timeval tv;

    if (!gettimeofday(&tv, NULL))
        ticks = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

#endif
    return ticks;
}

int main(int argc, char const * argv[])
{
    // load image at argv[1] using opencv
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);

    unsigned long long timer1, timer2;
    struct tm y2k = {0};
    double ms;
    double sec;

    // set up parameters for lossless GFWX
    int layers = 1;                               // just one image layer
    int channels = 3;                             // 3 interleaved channels
    int bitDepth = GFWX::BitDepthAuto;            // BitDepthAuto selects 8 or 16 based on type
    int quality = 128;                            // ranges from 1 (crappy) to 1024 (lossless)
    int chromaScale = 8;                          // chroma quality is divided by this number
    int blockSize = GFWX::BlockDefault;           // probably fine
    int filter = GFWX::FilterLinear;              // FilterLinear is usually best for lossless
    int quantization = GFWX::QuantizationScalar;  // only one choice here anyway
    int encoder = GFWX::EncoderTurbo;             // EncoderContextual compresses more than EncoderFast
    int intent = GFWX::IntentBGR;                 // opencv uses BGR instead of RGB
    int transform[] = GFWX_TRANSFORM_A710_BGR;    // handy predefined A710 transform (optional)

    // put the image dimensions and parameters into a header
    GFWX::Header header(image.size().width, image.size().height, layers, channels, bitDepth, quality,
        chromaScale, blockSize, filter, quantization, encoder, intent);

    // create a conservatively sized buffer to receive compressed bytes
    // (I've never seen an image take more than twice the original size, even random noise)
    std::vector<uchar> buffer(std::max((size_t)256, image.total() * image.elemSize() * 2));

    // compress the image into the byte buffer (the last two zeros are for optional metadata and size)
    timer1 = GetTickCount64();
    ptrdiff_t size = GFWX::compress(image.ptr(), header, &buffer[0], buffer.size(), transform, 0, 0);
    timer2 = GetTickCount64();

    ms = timer2 - timer1;

    sec = ms / 1000;

    printf (" %lf seconds to compress %d Bytes\n", sec, filesize(argv[1]));

    std::ofstream(argv[2], std::ios::binary).write((char*)&buffer[0], size);

    std::ifstream in(argv[2], std::ios::binary | std::ios::ate);
    std::vector<uchar> buffer2(in.tellg());
    in.seekg(0, std::ios::beg).read((char*)&buffer2[0], buffer2.size());

    // read the header first (with 0 pointer for image)
    GFWX::Header header2;
    ptrdiff_t result = GFWX::decompress((uchar*)0, header2, &buffer2[0], buffer2.size(), 0, true);
    if (result != GFWX::ResultOk)
        return (int)result;    // GFWX::ErrorMalformed for a bad file, or positive for truncated file

    // check for our expected image type (1 layer 8-bit BGR) or else convert it yourself
    if (header2.bitDepth != 8 || header2.channels != 3
        || header2.layers != 1 || header2.intent != GFWX::IntentBGR)
        return -1;

    // set up an opencv Mat to receive the image
    cv::Mat img = cv::Mat_<cv::Vec3b>(header2.sizey, header2.sizex);

    // decompress
    timer1 = GetTickCount64();
    result = GFWX::decompress(img.ptr(), header2, &buffer2[0], buffer2.size(), 0, false);
  	timer2 = GetTickCount64();

    ms = timer2 - timer1;

    sec = ms / 1000;

    if (result != GFWX::ResultOk)
        return (int)result;    // positive for truncated file, negative for error

    // write the result to a file at argv[3]
    cv::imwrite(argv[3], img);

    printf (" %lf seconds to decompress %d Bytes\n", sec, filesize(argv[2]));

    return 0;
}