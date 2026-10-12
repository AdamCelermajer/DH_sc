// vox::DecoderNative containers and vox::DecoderNativeCursor accessors that are
// reconstructed (HasData, GetRewindLimit, SetLoop, the pending state list and the
// container builders). The sub-decoder is a PCM object placed in VoxAlloc storage, since
// the cursor frees it with VoxFree.
#include <cstdio>
#include <map>
#include <new>
#include <string>

#include "vox-audio/vox/decoder_native.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_sub_decoder.hpp"

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

namespace vox {

struct DecoderNativeTestAccess {
    static bool NeedsParse(const DecoderNative& d) { return d.needs_parse_; }
    static std::size_t SegmentCues(const DecoderNative& d) { return d.segment_cues_.size(); }
    static std::size_t Transitions(const DecoderNative& d) { return d.transitions_.size(); }
    static int SegmentsCount(const DecoderNative& d) { return d.segments_count_; }
    static int RulesCount(const DecoderNative& d) { return d.rules_count_; }
    static int StatesCount(const DecoderNative& d) { return d.states_count_; }
};

struct DecoderNativeCursorTestAccess {
    static void SetFrames(DecoderNativeCursor& c, int before, int current, int mark) {
        c.frames_before_ = before;
        c.frames_current_ = current;
        c.frames_mark_ = mark;
    }
    static void PushIndex(DecoderNativeCursor& c, std::int32_t index) {
        c.state_indices_.push_back(index);
    }
    static void SetStateNames(DecoderNativeCursor& c, const std::map<std::string, int>* names) {
        c.state_names_ = names;
    }
    static void SetSubDecoder(DecoderNativeCursor& c, VoxNativeSubDecoder* sub) {
        c.sub_decoder_ = sub;
    }
    static int StateIndex(DecoderNativeCursor& c) { return c.GetStateIndex(); }
};

}  // namespace vox

namespace {

// PCM sub-decoder with the has-data flag exposed.
class TestSubDecoder : public vox::VoxNativeSubDecoderPCM {
public:
    void SetHasData(bool on) { has_data_ = on; }
};

}  // namespace

int main() {
    using vox::DecoderNative;
    using vox::DecoderNativeCursor;
    using vox::DecoderNativeCursorTestAccess;
    using vox::DecoderNativeTestAccess;

    // Construction and the simple accessors.
    {
        DecoderNative d;
        CHECK(d.GetType() == 4);
        CHECK(d.GetParam() == nullptr);
        CHECK(DecoderNativeTestAccess::NeedsParse(d));
        CHECK(DecoderNativeTestAccess::StatesCount(d) == 0);
    }

    // Container builders set the count and the storage.
    {
        DecoderNative d;
        CHECK(d.CreateStatesContainer(4) != nullptr);
        CHECK(DecoderNativeTestAccess::StatesCount(d) == 4);

        d.CreateTransitionsContainer(3);
        CHECK(DecoderNativeTestAccess::Transitions(d) == 3);

        CHECK(d.CreateSegmentsInfoContainers(2, 8) != nullptr);
        CHECK(DecoderNativeTestAccess::SegmentsCount(d) == 2);
        CHECK(DecoderNativeTestAccess::SegmentCues(d) == 2);

        CHECK(d.CreateTransitionRulesContainer(2, 4) != nullptr);
        CHECK(DecoderNativeTestAccess::RulesCount(d) == 2);
        // The destructor frees all of the arrays above.
    }

    // The factory returns a constructed DecoderNative in VoxAlloc storage.
    {
        void* mem = vox::DecoderNativeFactory(nullptr);
        auto* d = static_cast<DecoderNative*>(mem);
        CHECK(d != nullptr && d->GetType() == 4);
        d->~DecoderNative();
        VoxFree(mem);
    }

    // Cursor on a decoder with no stream: the parse fails and the cursor stays empty.
    {
        DecoderNative d;
        DecoderNativeCursor c(&d, nullptr);
        CHECK(!c.HasData());
        CHECK(c.GetRewindLimit() == 0);
        CHECK(DecoderNativeCursorTestAccess::StateIndex(c) == -1);
        c.SetLoop(true);
        CHECK(DecoderNativeTestAccess::NeedsParse(d));
    }

    // Rewind limit: current past the mark gives 0, the mark past the earlier frames gives
    // the earlier frame count, otherwise the current position.
    {
        DecoderNative d;
        DecoderNativeCursor c(&d, nullptr);
        DecoderNativeCursorTestAccess::SetFrames(c, 10, 5, 3);
        CHECK(c.GetRewindLimit() == 0);
        DecoderNativeCursorTestAccess::SetFrames(c, 4, 3, 5);
        CHECK(c.GetRewindLimit() == 4);
        DecoderNativeCursorTestAccess::SetFrames(c, 10, 3, 5);
        CHECK(c.GetRewindLimit() == 3);
    }

    // Pending state list: pop order is newest first, then -1 when empty.
    {
        DecoderNative d;
        DecoderNativeCursor c(&d, nullptr);
        DecoderNativeCursorTestAccess::PushIndex(c, 1);
        DecoderNativeCursorTestAccess::PushIndex(c, 2);
        CHECK(DecoderNativeCursorTestAccess::StateIndex(c) == 2);
        CHECK(DecoderNativeCursorTestAccess::StateIndex(c) == 1);
        CHECK(DecoderNativeCursorTestAccess::StateIndex(c) == -1);
    }

    // SetInteractiveMusicState appends the named index and ignores unknown names.
    {
        DecoderNative d;
        DecoderNativeCursor c(&d, nullptr);
        std::map<std::string, int> names = {{"intro", 7}, {"loop", 9}};
        DecoderNativeCursorTestAccess::SetStateNames(c, &names);
        c.SetInteractiveMusicState("loop");
        c.SetInteractiveMusicState("missing");
        CHECK(DecoderNativeCursorTestAccess::StateIndex(c) == 9);
        CHECK(DecoderNativeCursorTestAccess::StateIndex(c) == -1);
        DecoderNativeCursorTestAccess::SetStateNames(c, nullptr);
    }

    // HasData follows the sub-decoder. The cursor frees the sub-decoder on destruction.
    {
        DecoderNative d;
        DecoderNativeCursor c(&d, nullptr);
        void* mem = VoxAlloc(sizeof(TestSubDecoder));
        auto* sub = new (mem) TestSubDecoder();
        sub->SetHasData(true);
        DecoderNativeCursorTestAccess::SetSubDecoder(c, sub);
        CHECK(c.HasData());
        sub->SetHasData(false);
        CHECK(!c.HasData());
        sub->SetHasData(true);
    }

    // The factory path of CreateNewCursor and DestroyCursor.
    {
        DecoderNative d;
        vox::DecoderCursorInterface* cursor = d.CreateNewCursor(nullptr);
        CHECK(cursor != nullptr);
        d.DestroyCursor(cursor);
    }

    std::printf("decoder_native_test: %s\n", g_failures == 0 ? "ok" : "FAILED");
    return g_failures == 0 ? 0 : 1;
}
