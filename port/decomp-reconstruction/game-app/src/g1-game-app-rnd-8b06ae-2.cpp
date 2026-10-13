// Batch g1-game-app-rnd-8b06ae-2 (rnd, part 3 of 8): MgxBlock, Block, RPElem, RoomPool, Rule, RootRule, Path and EndPath.
// Bodies follow the Hex-Rays pseudocode of libDungeonHunter2.so.
//
// Duplicate addresses: the Block constructors 0x48ab34 and 0x48ac24, and the MgxBlock constructors 0x48ad14 and 0x48b108,
// have identical bodies (the C1 and C2 variants). Each pair is one C++ definition, listed under both addresses.
//
// PARTIAL (the ledger notes repeat these):
//   - Block::Block: the constructor stores &unk_956B24 into each side's unk_14. That object is not a C string (its first
//     word is 4, then the pointer 0x8d4de0), so the field is left null here.
//   - MgxBlock::LoadFromXmlStream: needs the size (IFileStream vtable +0x08) and read (vtable +0x18) of the stream, neither
//     declared in dh2 yet, and the TinyXML document and handle calls. It returns false.
//   - MgxBlock::FromFilename: opens the .mgx file through the file system at dword_99F73C+0x34 (glitch IDevice +0x34), which
//     is not declared in this tree (the same gap as DebugSwitches::load). OpenMgxFile returns null, so the function returns
//     nullptr. The name, gameplay and visual fields and the failed-load path follow the original's order.
//
// Assert behaviour (PlaceInMap, RemoveFromMap): gAssertLevel 2 stores through null in the original (abort here), level 1
// prints the ASSERT line for rndBlock.cpp (298 or 320), and level 0 is silent. The cell loop goes on after a level 0 or 1
// failure, as in the original.
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#include "dh2/rnd/EndPath.hpp"
#include "dh2/rnd/MgxBlock.hpp"
#include "dh2/rnd/Path.hpp"
#include "dh2/rnd/RPElem.hpp"
#include "dh2/rnd/RoomPool.hpp"
#include "dh2/rnd/RootRule.hpp"
#include "dh2/rnd/Rule.hpp"

// Global assert level. core-platform/memory.hpp declares it too; it is not included here, as in the other game-app sources.
extern int gAssertLevel;

namespace {

// Source path printed by the original's ASSERT in rndBlock.cpp.
constexpr const char kRndBlockCppPath[] = "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\random\\rndBlock.cpp";

// The original's ASSERT(false) on a cell that is not in the expected state. Level 2 stores through null to crash, which is
// reproduced as abort(). Level 1 prints the line. Level 0 does nothing.
void ReportBlockCellAssert(int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "false", kRndBlockCppPath, line);
    }
}

// PARTIAL: the original opens `path` through the file system at dword_99F73C+0x34. That interface is not declared in this
// tree, so nothing is opened here.
IFileStream* OpenMgxFile(const char* path) {
    (void)path;
    return nullptr;
}

// PARTIAL: closes a stream from OpenMgxFile through the same file system object. Never reached while OpenMgxFile returns null.
void CloseMgxFile(IFileStream** stream) {
    (void)stream;
}

// The original searches the name from its end for ".mgx" and removes the last match with a four-character replace (a
// std::search over reverse iterators, then a replace with ""). That is rfind followed by erase.
void StripMgxExtension(std::string& name) {
    const std::size_t pos = name.rfind(".mgx");
    if (pos != std::string::npos) {
        name.erase(pos, 4);
    }
}

}  // namespace

namespace rnd {

// 0x48aa84 rnd::MgxBlock::LoadFromXmlStream(IFileStream*)
// PARTIAL: the original reads the whole stream into a buffer (stored at +0x9c0, the document at +0x9c4), parses it with
// TiXmlDocument::LoadFromBuffer, takes the "Module" child, and returns LoadFromXml(handle) (0x48a90c, not in this batch).
// The stream's size and read methods are not declared in this tree, so the body returns false. Waits on the glitch
// IFileStream declaration.
bool MgxBlock::LoadFromXmlStream(IFileStream* stream) {
    (void)stream;
    return false;
}

// 0x48ab34 rnd::Block::Block(void)
// 0x48ac24 rnd::Block::Block(void)  (same body; one definition serves both)
// PARTIAL: side unk_14 is left null. The original stores &unk_956B24 there (see the file header).
Block::Block() {
    unk_4c = 2960.0f;
    unk_50 = 2960.0f;
    m_width = 1;
    m_height = 1;
    unk_5c = 0;
    for (Side& side : m_sides) {
        side.unk_14 = nullptr;  // PARTIAL: the original stores &unk_956B24 here
        side.unk_1c = nullptr;
        side.unk_20 = nullptr;
        side.unk_24 = nullptr;
    }
}

// 0x48ad14 rnd::MgxBlock::MgxBlock(void)
// 0x48b108 rnd::MgxBlock::MgxBlock(void)  (same body; one definition serves both)
MgxBlock::MgxBlock() {
    unk_9c0 = 0;
    unk_9c4 = 0;
}

// 0x48ad54 rnd::MgxBlock::FromFilename(char const*,char const*,char const*)
// PARTIAL: OpenMgxFile opens nothing yet (see the file header), so this returns nullptr today. The rest follows the
// original: the path "<gameplay>/mgx/<filename>" (a 512-byte buffer), the name with ".mgx" removed, gameplay and visual
// set, and the block deleted when its XML does not load. The stream is closed after the load.
MgxBlock* MgxBlock::FromFilename(const char* gameplay, const char* visual, const char* filename) {
    char path[512];
    std::snprintf(path, sizeof(path), "%s/mgx/%s", gameplay, filename);
    IFileStream* stream = OpenMgxFile(path);
    if (stream == nullptr) {
        return nullptr;
    }
    MgxBlock* block = new MgxBlock();
    block->m_name = filename;
    StripMgxExtension(block->m_name);
    block->m_gameplay = gameplay;
    block->m_visual = visual;
    if (!block->LoadFromXmlStream(stream)) {
        delete block;
        block = nullptr;
    }
    CloseMgxFile(&stream);
    return block;
}

// 0x48b85c rnd::MgxBlock::FitsInMap(Array2d<rnd::Tile*>&,int,int)
// True when every cell of the m_width by m_height footprint at (x, y) is empty. Array2d grows to cover the cells.
bool MgxBlock::FitsInMap(Array2d<Tile*>& map, int x, int y) {
    for (int dx = 0; dx < m_width; ++dx) {
        for (int dy = 0; dy < m_height; ++dy) {
            if (map(x + dx, y + dy) != nullptr) {
                return false;
            }
        }
    }
    return true;
}

// 0x48b8f0 rnd::MgxBlock::RemoveFromMap(Array2d<rnd::Tile*>&,rnd::Tile*,int,int)
// Clears each footprint cell that holds `tile`. A cell that holds something else is an assert (see the file header).
// Returns 1 in the original.
bool MgxBlock::RemoveFromMap(Array2d<Tile*>& map, Tile* tile, int x, int y) {
    for (int dy = 0; dy < m_height; ++dy) {
        for (int dx = 0; dx < m_width; ++dx) {
            Tile*& cell = map(x + dx, y + dy);
            if (cell == tile) {
                cell = nullptr;
            } else {
                ReportBlockCellAssert(320);
            }
        }
    }
    return true;
}

// 0x48ba58 rnd::MgxBlock::PlaceInMap(Array2d<rnd::Tile*>&,rnd::Tile*,int,int)
// Stores `tile` in each empty footprint cell. An occupied cell is an assert (see the file header). Returns 1 in the original.
bool MgxBlock::PlaceInMap(Array2d<Tile*>& map, Tile* tile, int x, int y) {
    for (int dy = 0; dy < m_height; ++dy) {
        for (int dx = 0; dx < m_width; ++dx) {
            Tile*& cell = map(x + dx, y + dy);
            if (cell == nullptr) {
                cell = tile;
            } else {
                ReportBlockCellAssert(298);
            }
        }
    }
    return true;
}

// 0x48bbec rnd::Rule::Impl::GetRootRule(void) const
RootRule* Rule::Impl::GetRootRule() const {
    return m_rule->m_root;
}

// 0x48bbf8 rnd::Rule::GetRootRule(void) const
RootRule* Rule::GetRootRule() const {
    return m_root;
}

// 0x48bc00 rnd::RPElem::FillSizes(int,int)
// Stores the first size at +0x0c and +0x14, and max(a, b) at +0x10. Does nothing when either size is negative.
void RPElem::FillSizes(int a, int b) {
    if ((a | b) >= 0) {
        unk_10 = (b < a) ? a : b;
        unk_14 = a;
        unk_0c = a;
    }
}

// 0x48bc20 rnd::RoomPool::Find(unsigned long)
// The first element whose key (+0x04) equals `key`, or null. Elements are checked in order, as in the original.
RPElem* RoomPool::Find(unsigned long key) {
    for (RPElem& elem : m_elems) {
        if (elem.unk_04 == key) {
            return &elem;
        }
    }
    return nullptr;
}

// 0x48bc94 rnd::Rule::Unload(void)
// Deletes the child rules from the last slot back to the first, each through its virtual destructor, then clears the 16
// child slots. unk_0c is the child count. Only the first unk_0c slots are read; the original reads them the same way.
void Rule::Unload() {
    while (unk_0c > 0) {
        --unk_0c;
        Rule* child = m_children[unk_0c];
        if (child != nullptr) {
            delete child;
        }
    }
    for (Rule*& slot : m_children) {
        slot = nullptr;
    }
}

// 0x48bcf4 rnd::Rule::GetApp(void) const
RandomGenerator* Rule::GetApp() const {
    return m_root->m_generator;
}

// 0x48bd00 rnd::RootRule::Unload(void)
// Empty in the original (a single BX LR). The root rule owns nothing that Rule::Unload would release.
void RootRule::Unload() {}

// 0x48bd04 rnd::RootRule::Impl::FilterExits(std::vector<std::pair<rnd::Exit const*,rnd::ListElem>>&,rnd::ListRule*&)
// Empty in the original (a single BX LR). The arguments are not used.
void RootRule::Impl::FilterExits(std::vector<std::pair<const Exit*, ListElem>>& /*candidates*/, ListRule*& /*list*/) {}

// 0x48bd08 rnd::RootRule::Impl::WriteDebuggingInfo(void)
// Empty in the original (a single BX LR).
void RootRule::Impl::WriteDebuggingInfo() {}

// 0x48bd0c rnd::Path::Impl::IsDeadEnd(void) const
// A dead end when Impl unk_28 + 1 equals unk_2c and the source rule has no children (Rule::unk_0c == 0). The two Impl fields
// are not named yet (see the header).
bool Path::Impl::IsDeadEnd() const {
    return unk_28 + 1 == unk_2c && m_rule->unk_0c == 0;
}

// 0x48bd38 rnd::EndPath::Impl::OneStep(rnd::Tile*,rnd::Exit const*,rnd::Exit const*)
// Returns 1: an end path places nothing and ends the path.
int EndPath::Impl::OneStep(Tile* /*tile*/, const Exit* /*from*/, const Exit* /*to*/) {
    return 1;
}

}  // namespace rnd
