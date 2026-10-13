// rnd::Path: a rule that grows a chain of tiles one exit at a time. Its runtime node, Path::Impl,
// adds exits that pass a size gate (see FilterExits and AddExit). Original 32-bit size: at least 0x8e
// (the ctor writes +0x8c and +0x8d); the tail is not recovered.
// Reconstructed from libDungeonHunter2.so pseudocode (ctor 0x48ddf8 / 0x48dfac, dtors 0x48d604 /
// 0x48d770, NewImpl 0x48c210, LoadFromXml 0x49130c, Impl ctors 0x48c130 / 0x48c23c, Impl dtors
// 0x48d400 / 0x48d49c, IsDeadEnd 0x48bd0c, AddExit 0x48f05c, FilterExits 0x48f25c, OneStep 0x48fd64).
#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "dh2/rnd/ListElem.hpp"
#include "dh2/rnd/Rule.hpp"

namespace rnd {

class RootRule;
class Exit;

class Path : public Rule {
public:
    class Impl;

    Path(RootRule& root, Rule* parent);                             // 0x48ddf8 / 0x48dfac
    ~Path() override;                                               // 0x48d604 / 0x48d770

    Rule::Impl* NewImpl(Rule::Impl* parent) const override;         // slot 0x08: 0x48c210
    void LoadFromXml(TiXmlNode* node) override;                     // slot 0x0c: 0x49130c

    // Original layout. Rule occupies +0x00 .. +0x87.
    int32_t unk_88;                                                 // +0x88 set to 4 by the ctor
    bool unk_8c;                                                    // +0x8c set to 1 by the ctor
    bool m_straightOnly;                                            // +0x8d set to 0 by the ctor (Track A: Rule+141)
};

// Runtime node of a Path. Exits are kept only when the destination block passes the size gate.
class Path::Impl : public Rule::Impl {
public:
    Impl(const Path& path, Rule::Impl* parent);                     // 0x48c130 / 0x48c23c
    ~Impl() override;                                               // 0x48d400 / 0x48d49c

    bool IsDeadEnd() const;                                         // 0x48bd0c
    void AddExit(std::vector<std::pair<const Exit*, ListElem>>& candidates,
                 const Exit* exit, ListElem elem);                  // 0x48f05c (signature inferred)
    void FilterExits(std::vector<std::pair<const Exit*, ListElem>>& candidates,
                     ListRule*& list) override;                     // 0x48f25c
    int OneStep(Tile* tile, const Exit* from, const Exit* to) override;  // 0x48fd64

    // Path::Impl adds no members that the recovered code touches; its tail is not recovered.
};

}  // namespace rnd
