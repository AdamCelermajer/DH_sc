// rnd::EndPath: a terminal rule. It places nothing: its runtime node's OneStep returns 1 and ends the
// path. Original 32-bit size 0x88 (the Rule base; EndPath adds no fields in the recovered code).
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x48dd18 / 0x48dd4c, dtors 0x48d66c /
// 0x48d7ac, NewImpl 0x48c054, Impl ctors 0x48c018 / 0x48c080, Impl dtors 0x48d468 / 0x48d514, OneStep 0x48bd38).
#pragma once

#include "dh2/rnd/Rule.hpp"

namespace rnd {

class RootRule;

class EndPath : public Rule {
public:
    class Impl;

    EndPath(RootRule& root, Rule* parent);                          // 0x48dd18 / 0x48dd4c
    ~EndPath() override;                                            // 0x48d66c / 0x48d7ac

    Rule::Impl* NewImpl(Rule::Impl* parent) const override;         // slot 0x08: 0x48c054
    // LoadFromXml and Unload are inherited from Rule (vtable slots 0x0c and 0x10).
};

// Runtime node of an EndPath.
class EndPath::Impl : public Rule::Impl {
public:
    Impl(const EndPath& path, Rule::Impl* parent);                  // 0x48c018 / 0x48c080
    ~Impl() override;                                               // 0x48d468 / 0x48d514

    // Returns 1: an end path spawns nothing (Track A: OneStep 0x48bd38 returns 1).
    int OneStep(Tile* tile, const Exit* from, const Exit* to) override;  // 0x48bd38
};

}  // namespace rnd
