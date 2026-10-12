// Element groups: vox::SegmentGroup (base), vox::SequentialGroup, vox::RandomGroup.
// Only the functions whose bodies are recovered are declared here. The
// constructors and the copy constructors take GroupInfos, which is not
// reconstructed yet.
#pragma once

#include <cstdint>
#include <vector>

namespace vox {

enum class GroupPeekMode : int {
    kNormal = 0,  // original: mode 0 checks the state words for an empty group
    kOther = 1,
};

// Layout: +0x00 vtable, +0x04 valid, +0x08 select mode, +0x0c..+0x20 six state words.
class SegmentGroup {
public:
    SegmentGroup() = default;
    virtual ~SegmentGroup();  // 0x881a74 D2, 0x881a78 D1

    int GetSelectMode() const { return select_mode_; }  // 0x881a7c
    bool IsValid() const { return valid_; }             // 0x881a84
    void SetState(const SegmentGroup& other);           // 0x881a8c

protected:
    bool valid_ = false;                  // +0x04
    int select_mode_ = 0;                 // +0x08
    std::uint32_t words_[6] = {};         // +0x0c, +0x10, +0x14, +0x18, +0x1c, +0x20
};

// +0x24 element vector (begin, end, capacity), +0x30 current index, +0x34 saved index.
// words_[0] (+0xc) and words_[1] (+0x10) are the reset values, words_[2] (+0x14)
// and words_[3] (+0x18) the current state, words_[4] and words_[5] the saved state.
class SequentialGroup : public SegmentGroup {
public:
    // Six-word state record, in the order GetState writes it.
    struct State {
        std::int32_t v[6];  // [0] +0x14, [1] +0x18, [2] +0x30, [3] +0x1c, [4] +0x20, [5] +0x34
    };

    SequentialGroup() = default;
    ~SequentialGroup() override;  // 0x882888 D1, 0x8828d0 D2, 0x882c68 deleting variant

    void GetState(State* state) const;                 // 0x881d30
    void SetState(const State* state);                 // 0x881dfc
    void SetState(const SequentialGroup& other);       // 0x881dd8
    int PeekAtNextGroupElement(GroupPeekMode mode) const;  // 0x881d64
    void Reset(int reset_mode);                        // 0x881da0
    void SetToPreviousState();                         // 0x881e30
    void AddElement(int value);                        // 0x88387c
    int GetGroupElementPosition();                     // 0x881cc0 (partial, see the .cpp)

private:
    std::vector<int> elements_;  // +0x24..+0x2c
    int index_ = 0;              // +0x30
    int prev_index_ = 0;         // +0x34
};

// +0x24 vector of element pointers (first word of each is its value), +0x44 current index.
class RandomGroupElement;
class RandomGroup : public SegmentGroup {
public:
    RandomGroup() = default;
    int PeekAtNextGroupElement(GroupPeekMode mode) const;  // 0x881c0c
    void AddElement(RandomGroupElement* element);          // 0x8836b0 (partial, see ledger)

private:
    std::vector<RandomGroupElement*> elements_;  // +0x24..+0x2c
    int current_ = -1;                           // +0x44
};

}  // namespace vox
