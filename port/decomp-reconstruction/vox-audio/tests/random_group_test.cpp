// RandomGroup selection sequence, exclusion window, state round-trip, and the copy
// constructors of SegmentGroup and SequentialGroup.
#include <cstddef>
#include <cstdio>

#include "vox-audio/vox/groups.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {
struct RandomGroupTestAccess {
    static void SetCounts(RandomGroup& g, int count, int total) {
        g.element_count_ = count;
        g.total_weight_ = total;
    }
    static int Exclusion(const RandomGroup& g) { return g.exclusion_window_; }
    static void SetExclusion(RandomGroup& g, int window) { g.exclusion_window_ = window; }
    static std::size_t VectorSize(const RandomGroup& g) { return g.elements_.size(); }
    static int TotalWeight(const RandomGroup& g) { return g.total_weight_; }
    static int RemovedCount(const RandomGroup& g) {
        int n = 0;
        for (const RandomGroupNode* p = g.removed_.next; p != &g.removed_; p = p->next) {
            ++n;
        }
        return n;
    }
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
    // SegmentGroup copy sets valid from a default (invalid) source and copies the select mode.
    vox::GroupInfos info;
    info.select_mode_ = 3;
    info.reset_value_ = 7;
    info.field_14_ = 9;
    vox::SegmentGroup empty;
    vox::SegmentGroup empty_copy(empty);
    CHECK(empty_copy.IsValid());
    vox::SegmentGroup seg(&info, 0);
    vox::SegmentGroup seg_copy(seg);
    CHECK(seg_copy.GetSelectMode() == 3);

    // SequentialGroup copy keeps the element order and the current index.
    vox::SequentialGroup seq(&info, 0);
    seq.AddElement(5);
    seq.AddElement(6);
    vox::SequentialGroup seq_copy(seq);
    CHECK(seq_copy.PeekAtNextGroupElement(vox::GroupPeekMode::kOther) == 5);

    // Weights {0, 0, 1}: the only selectable element is index 2 whatever the roll is.
    vox::GroupInfos rinfo;
    rinfo.select_mode_ = 0;
    rinfo.field_0c_ = -1;
    rinfo.reset_value_ = 2;
    rinfo.field_14_ = 2;
    vox::RandomGroup rg(&rinfo, 0);
    CHECK(rg.IsValid());
    // AddElement copies each record, so the test records can be locals.
    vox::RandomGroupElement e10{10, 0};
    vox::RandomGroupElement e20{20, 0};
    vox::RandomGroupElement e30{30, 1};
    rg.AddElement(&e10);
    rg.AddElement(&e20);
    rg.AddElement(&e30);
    // The raw window is -1, so each AddElement counts one for the exclusion window (asm 0x8836b0).
    CHECK(vox::RandomGroupTestAccess::Exclusion(rg) == 3);
    // The scenario below was written for an exclusion window of 0.
    vox::RandomGroupTestAccess::SetExclusion(rg, 0);
    vox::RandomGroupTestAccess::SetCounts(rg, 3, 1);
    CHECK(rg.GetActiveElementIndex() == 2);
    CHECK(rg.Reset(0) == 2);

    // Cycle of 3 with the state words at 2: 30, 30, then the cycle is used up.
    CHECK(rg.GetGroupElementPosition() == 30);
    CHECK(rg.GetGroupElementPosition() == 30);
    CHECK(rg.GetGroupElementPosition() == -1);

    // Previous state brings back the remaining count of the last step.
    rg.SetToPreviousState();
    CHECK(rg.GetGroupElementPosition() == 30);

    // State round-trip into another group keeps the scalar fields.
    vox::RandomGroupState st{};
    rg.GetState(&st);
    vox::RandomGroup other(&rinfo, 0);
    other.SetState(&st);
    vox::RandomGroupState st2{};
    other.GetState(&st2);
    CHECK(st2.cycles == st.cycles);
    CHECK(st2.remaining == st.remaining);
    CHECK(st2.current == st.current);
    CHECK(st2.total_weight == st.total_weight);
    CHECK(st2.cycle_remaining == st.cycle_remaining);

    // Copy constructor copies the element records and the total weight.
    vox::RandomGroup rg_copy(rg);
    CHECK(vox::RandomGroupTestAccess::VectorSize(rg_copy) == 3);
    CHECK(vox::RandomGroupTestAccess::TotalWeight(rg_copy) == 1);
    CHECK(vox::RandomGroupTestAccess::RemovedCount(rg_copy) == 0);

    // Exclusion window 1 with equal weights: one element is held back at a time.
    vox::GroupInfos winfo;
    winfo.select_mode_ = 0;
    winfo.field_0c_ = 1;
    winfo.reset_value_ = 2;
    winfo.field_14_ = 2;
    vox::RandomGroup wg(&winfo, 0);
    vox::RandomGroupElement w10{10, 1};
    vox::RandomGroupElement w20{20, 1};
    wg.AddElement(&w10);
    wg.AddElement(&w20);
    vox::RandomGroupTestAccess::SetCounts(wg, 2, 2);
    wg.Reset(0);
    const int first = wg.GetGroupElementPosition();
    CHECK(first == 10 || first == 20);
    CHECK(vox::RandomGroupTestAccess::VectorSize(wg) == 1);
    CHECK(vox::RandomGroupTestAccess::RemovedCount(wg) == 1);
    CHECK(vox::RandomGroupTestAccess::TotalWeight(wg) == 1);
    const int second = wg.GetGroupElementPosition();
    CHECK(second == 10 || second == 20);
    CHECK(vox::RandomGroupTestAccess::VectorSize(wg) == 1);
    CHECK(vox::RandomGroupTestAccess::RemovedCount(wg) == 1);
    CHECK(vox::RandomGroupTestAccess::TotalWeight(wg) == 1);

    if (g_failures == 0) {
        std::printf("random_group_test: all checks passed\n");
        return 0;
    }
    std::printf("random_group_test: %d failures\n", g_failures);
    return 1;
}
