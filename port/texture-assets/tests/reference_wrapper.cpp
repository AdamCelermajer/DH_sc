#include "PVRTDecompress.h"

// Build this against a separately obtained PowerVR Native_SDK decoder for
// independent pixel comparison. The SDK source itself is not vendored here.
extern "C" unsigned int dh2_reference_pvrtc(
    const void* source, unsigned int is_2bpp,
    unsigned int width, unsigned int height, unsigned char* output) {
    return pvr::PVRTDecompressPVRTC(source, is_2bpp, width, height, output);
}
