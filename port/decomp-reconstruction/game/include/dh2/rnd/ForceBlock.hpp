// rnd::ForceBlock: a rule that forces one named block onto the next exit. Adds a single int after the
// Rule base. Original 32-bit size 0x8c (140).
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x48dd80 / 0x48ddbc, dtors 0x48d638 /
// 0x48d734, NewImpl 0x48cfcc, LoadFromXml 0x4912d0, Impl ctors 0x48cf10 / 0x48cff8, Impl dtors 0x48d434 /
// 0x48d550, Impl::FilterExits 0x48f47c).
#pragma once

#include <cstdint>

#include "dh2/rnd/Rule.hpp"

namespace rnd {

class RootRule;

class ForceBlock : public Rule {
public:
    class Impl;

    ForceBlock(RootRule& root, Rule* parent);                       // 0x48dd80 / 0x48ddbc
    ~ForceBlock() override;                                         // 0x48d638 / 0x48d734

    Rule::Impl* NewImpl(Rule::Impl* parent) const override;         // slot 0x08: 0x48cfcc
    void LoadFromXml(TiXmlNode* node) override;                     // slot 0x0c: 0x4912d0

    // Original layout. Rule occupies +0x00 .. +0x87.
    int32_t unk_88;                                                 // +0x88 set to 4 by the ctor
};

// Runtime node of a ForceBlock.
class ForceBlock::Impl : public Rule::Impl {
public:
    Impl(const ForceBlock& rule, Rule::Impl* parent);               // 0x48cf10 / 0x48cff8
    ~Impl() override;                                               // 0x48d434 / 0x48d550

    void FilterExits(std::vector<std::pair<const Exit*, ListElem>>& candidates,
                     ListRule*& list) override;                     // 0x48f47c
};

}  // namespace rnd
