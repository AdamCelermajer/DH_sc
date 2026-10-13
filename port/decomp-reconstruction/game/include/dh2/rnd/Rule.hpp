// rnd::Rule: a node of a random-level rule tree. Holds the root rule, its parent, the "exit" direction
// name and a 24-byte-entry vector. Rule::Impl is the per-generation runtime node, nested here.
// Original 32-bit size of Rule: 0x88 (136). Rule::Impl: 0x44 (68).
// Reconstructed from libDungeonHunter2.so pseudocode (ctor 0x48dc08 / 0x48dc90, dtors 0x48d58c /
// 0x48d7e8 / 0x48d860, Impl ctor 0x48bd60 / 0x48bebc, Impl dtors 0x48d1a0 / 0x48d2a8 / 0x48d2c4,
// GetRootRule 0x48bbf8, GetApp 0x48bcf4, GetBlock 0x48e274, NewRule 0x48de44, Unload 0x48bc94,
// LoadFromXml 0x490bdc).
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "dh2/rnd/ListElem.hpp"

class TiXmlNode;

namespace rnd {

class RootRule;
class RandomGenerator;
class Block;
class Direction;
class Exit;
class Tile;
class ListRule;

class Rule {
public:
    class Impl;

    Rule(RootRule& root, Rule* parent);                             // 0x48dc08 / 0x48dc90
    virtual ~Rule();                                                // 0x48d58c / 0x48d7e8 (vtable slots 0, 1)

    // Vtable slot 2 is pure: each rule kind builds its runtime node.
    virtual Impl* NewImpl(Impl* parent) const = 0;                  // slot 0x08
    virtual void LoadFromXml(TiXmlNode* node);                      // slot 0x0c: 0x490bdc
    virtual void Unload();                                          // slot 0x10: 0x48bc94
    virtual RootRule* GetRootRule() const;                          // slot 0x14: 0x48bbf8

    RandomGenerator* GetApp() const;                                // 0x48bcf4
    Block* GetBlock(const char* name) const;                        // 0x48e274
    static Rule* NewRule(TiXmlNode* node);                          // 0x48de44

    // Original layout. Offsets are the 32-bit offsets of the original object.
    RootRule* m_root;                                               // +0x04 the root rule (the RootRule itself for RootRule)
    Rule* m_parent;                                                 // +0x08
    int32_t unk_0c;                                                 // +0x0c zeroed by the ctor
    uint8_t pad_10[0x40];                                           // +0x10 .. +0x4f not written by the ctor
    std::string m_exit;                                             // +0x50 "exit" attribute; compared to north, east, south, west
    int32_t unk_68;                                                 // +0x68 zeroed by the ctor
    int32_t unk_6c;                                                 // +0x6c set to -1 by the ctor
    uint8_t unk_70[0x0c];                                           // +0x70 begin, end, cap of a 24-byte-entry vector
    int32_t unk_7c;                                                 // +0x7c zeroed by the ctor
    int32_t unk_80;                                                 // +0x80 set to 1 by the ctor
    int32_t unk_84;                                                 // +0x84 set to 1 by the ctor
};

// Per-generation node of a Rule. Holds the source rule, the parent node and the child list.
class Rule::Impl {
public:
    Impl(const Rule& rule, Impl* parent);                           // 0x48bd60 / 0x48bebc
    virtual ~Impl();                                                // 0x48d1a0 / 0x48d2a8 / 0x48d2c4

    RootRule* GetRootRule() const;                                  // 0x48bbec
    bool Step(Tile* tile, const Exit* from);                        // 0x48f954
    virtual int OneStep(Tile* tile, const Exit* from, const Exit* to);  // 0x490304
    virtual void FilterExits(std::vector<std::pair<const Exit*, ListElem>>& candidates,
                             ListRule*& list);                      // 0x48e690

    // Original layout. Offsets are the 32-bit offsets of the original object; the vptr at +0x00 is implicit.
    Rule* m_rule;                                                   // +0x04 the source rule
    Impl* m_parent;                                                 // +0x08
    int32_t m_childCount;                                           // +0x0c number of child nodes
    uint8_t pad_10[0x18];                                           // +0x10 .. +0x27 child-node slots (capacity not recovered)
    int32_t unk_28;                                                 // +0x28 zeroed by the ctor
    int32_t unk_2c;                                                 // +0x2c set to 1 by the ctor
    std::vector<const char*> m_names;                               // +0x30 names collected from the exit list
    const Direction* m_direction;                                   // +0x3c set from the exit name: north, east, south or west
    int32_t unk_40;                                                 // +0x40 zeroed by the ctor
};

}  // namespace rnd
