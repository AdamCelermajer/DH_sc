// SegmentGroup and SequentialGroup constructors from a GroupInfos record, and the
// native sub-decoder track-parameter accessor on a derived test object.
#include <cstdio>
#include <cstdlib>

#include "vox-audio/vox/groups.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_sub_decoder.hpp"

namespace vox {
struct SegmentGroupTestAccess {
    static std::uint32_t Word(const SegmentGroup& g, int i) { return g.words_[i]; }
    static bool Valid(const SegmentGroup& g) { return g.valid_; }
    static int Mode(const SegmentGroup& g) { return g.select_mode_; }
};
struct NativeSubDecoderTestAccess : public VoxNativeSubDecoder {
    NativeSubDecoderTestAccess(std::int16_t channels, std::int32_t word, std::int16_t bits) {
        channels_ = channels;
        field_0c_ = word;
        bits_ = bits;
    }
    using VoxNativeSubDecoder::MixMultipleSegments;
};
}  // namespace vox

namespace {
int g_failures = 0;
#define CHECK(cond)                                                       \
    do {                                                                  \
        if (!(cond)) {                                                    \
            std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__,   \
                         __LINE__, #cond);                                \
            ++g_failures;                                                 \
        }                                                                 \
    } while (0)
}  // namespace

int main() {
    vox::GroupInfos info;
    info.select_mode_ = 3;
    info.reset_value_ = 7;
    info.field_14_ = 9;

    // Reset flag 0 takes the reset value for the first word.
    vox::SequentialGroup seq0(&info, 0);
    CHECK(vox::SegmentGroupTestAccess::Valid(seq0));
    CHECK(vox::SegmentGroupTestAccess::Mode(seq0) == 3);
    CHECK(vox::SegmentGroupTestAccess::Word(seq0, 0) == 7);
    CHECK(vox::SegmentGroupTestAccess::Word(seq0, 1) == 9);
    CHECK(vox::SegmentGroupTestAccess::Word(seq0, 4) == 7);
    CHECK(vox::SegmentGroupTestAccess::Word(seq0, 5) == 9);

    // Any other reset flag uses 1 for the first word.
    vox::SegmentGroup seg1(&info, 5);
    CHECK(vox::SegmentGroupTestAccess::Word(seg1, 0) == 1);
    CHECK(vox::SegmentGroupTestAccess::Word(seg1, 2) == 1);
    CHECK(vox::SegmentGroupTestAccess::Word(seg1, 3) == 9);

    // Track parameters: channels, the word at +0x0c, bits per sample, and a zero.
    vox::NativeSubDecoderTestAccess sub(2, 12345, 16);
    vox::TrackParams params = sub.GetTrackParams();
    CHECK(params.channels == 2);
    CHECK(params.unknown_08 == 12345);
    CHECK(params.bits_per_sample == 16);
    CHECK(params.length_frames == 0);

    // MixMultipleSegments: the byte count divided by the frame size (2 channels, 2 bytes).
    CHECK(sub.MixMultipleSegments(nullptr, 400) == 100);

    // Clean frees the shared mixing buffer when it is set.
    vox::VoxNativeSubDecoder::s_pMixingBuffer = VoxAlloc(64);
    vox::VoxNativeSubDecoder::Clean();
    vox::VoxNativeSubDecoder::s_pMixingBuffer = nullptr;

    std::printf("group_ctor_test: %s\n", g_failures == 0 ? "ok" : "FAILED");
    return g_failures == 0 ? 0 : 1;
}
