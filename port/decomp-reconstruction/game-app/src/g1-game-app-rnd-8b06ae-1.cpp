// Batch g1-game-app-rnd-8b06ae-1 (owner rnd, part 2 of 8): random-level file lists, rule and block loading,
// block and exit geometry, and the rnd destructors. Bodies follow the Hex-Rays pseudocode of libDungeonHunter2.so.
//
// Status summary (the ledger rows carry the detail):
//   reconstructed: TryIsZero, Exit::GetBlockUnitPosition, Exit::Exit (C1 and C2 are one definition).
//   partial:       GetFiles, LoadBlocks, LoadRuleFile, SaveModularLevelToStream, SaveModularLevelXml,
//                  IsStraight, GetFlag, Block::LinkToOtherBlocks, Block::~Block (both addresses),
//                  MgxBlock::~MgxBlock (both addresses), Exit::LoadFromXml, MgxBlock::LoadFromXml.
//   skipped:       the two deleting destructors (Block and MgxBlock, D0 variants).
//
// What blocks the rest (not declared in the tree yet):
//   - The device file system (IDevice +0x34, read through the glitch file interface with the binary's vtable slots)
//     is needed by GetFiles, LoadRuleFile and SaveModularLevelXml.
//   - rnd::Direction (sDirections and DIRECTION_OPPOSITES) is needed by IsStraight, GetFlag, LinkToOtherBlocks and
//     Exit::LoadFromXml.
//   - The Block side record is the rnd::Exit record: Exit::LoadFromXml and the Exit constructor run on the address of
//     Block::m_sides[k] (+0x60 + 300 k), and the link table lives at side +0x28 (count) and +0x2c (pointers). The
//     header does not declare that relation or the names vector (side +0x1c, a std::vector<std::string>).
//   - The TinyXML surface in the tree (TiXmlHandle::Child, ToElement, Attribute) is partial, so the XML loaders stay stubs.
#include "dh2/rnd/RandomGenerator.hpp"

#include <cmath>
#include <cstddef>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "dh2/Point3D.hpp"
#include "dh2/lstr.hpp"
#include "dh2/rnd/Block.hpp"
#include "dh2/rnd/Exit.hpp"
#include "dh2/rnd/MgxBlock.hpp"

#if defined(_WIN32)
#define DH2_STRCASECMP _stricmp
#else
#include <strings.h>
#define DH2_STRCASECMP strcasecmp
#endif

namespace rnd {

namespace {

// Case-insensitive equality through the platform's strcasecmp, as the original's LC_API_STRCASECMP does.
bool IgnoreCaseEquals(const char* lhs, const char* rhs) {
    return DH2_STRCASECMP(lhs, rhs) == 0;
}

// PARTIAL stub. The original opens `path` on the device file system (IDevice +0x34, not declared in the tree), reads
// its size and then its bytes, closes it and writes a NUL after the text. Until that file system is declared, the text
// stays empty and the call reports false.
bool ReadFileListText(const char* path, std::string& text) {
    (void)path;
    text.clear();
    return false;
}

}  // namespace

// Splits a list file into lines at each '\n'. For each newline at index pos, the original takes text[0, pos - 1) as the
// line, so the character before the newline is dropped as well (list files carry CRLF line ends and the '\r' goes).
// When the newline is the first character, pos - 1 wraps to the largest size and substr returns the whole rest, as the
// original's 0xFFFFFFFF length does. The remainder after the newline becomes the next input. Text after the last
// newline is not added.
void SplitFileListText(std::string text, std::vector<std::string>& files) {
    std::size_t pos = text.find('\n');
    while (pos != std::string::npos) {
        files.push_back(text.substr(0, pos - 1));
        text = text.substr(pos + 1);
        pos = text.find('\n');
    }
}

// 0x489aa4 rnd::TryIsZero(char const*)
// True when the text is "0" or empty (case-insensitive compare, as the original).
bool TryIsZero(const char* text) {
    return IgnoreCaseEquals(text, "0") || IgnoreCaseEquals(text, "");
}

// 0x488904 rnd::RandomGenerator::GetFiles(char const*, std::vector<std::string, std::allocator<std::string>>&)
// PARTIAL. The line split is rebuilt (SplitFileListText, tested). The read of the list file is a stub
// (ReadFileListText), so no names are produced in this build. The original returns a garbage register value,
// which the header keeps as void.
void RandomGenerator::GetFiles(const char* directory, std::vector<std::string>& files) {
    std::string text;
    ReadFileListText(directory, text);
    SplitFileListText(text, files);
}

// 0x488b84 rnd::RandomGenerator::SaveModularLevelToStream(StreamBuffer&)
// PARTIAL. Not rebuilt. The original builds a TinyXML document (declaration "1.0" / "utf-8", two comment nodes with the
// generator banner and the source file name from unk_174, a "Level" element with the level config properties at
// m_levelConfig +0x04 and the root tile saved through Tile::SaveAsModuleXML(tile, level, 0) when m_rootTile is set),
// prints it with a TiXmlPrinter whose line break is "\r\n", and writes the printed bytes into the stream. The
// TiXmlDocument, TiXmlDeclaration and TiXmlComment surface and the StreamBuffer write are not declared in the tree,
// and Tile::SaveAsModuleXML takes a third argument that Tile.hpp does not declare. Nothing is written here.
void RandomGenerator::SaveModularLevelToStream(StreamBuffer& stream) {
    (void)stream;
}

// 0x488e50 rnd::RandomGenerator::SaveModularLevelXml(void)
// PARTIAL. Not rebuilt. The original writes the stream from SaveModularLevelToStream to a file named after the rule
// file: the path m_sceneDir + unk_174 has its ".xml" (found with find) replaced by "_<n>.mlx", where <n> is an unsigned
// long written by the ostringstream. The value of <n> is not visible in the Hex-Rays listing (the put-number call has
// no argument shown), so the name cannot be formed. The file is then opened for writing on the device file system
// (not declared) and the name after the last '/' is returned. Returns an empty name in this build.
std::string RandomGenerator::SaveModularLevelXml() {
    return std::string();
}

// 0x4892ec rnd::RandomGenerator::LoadBlocks(void)
// PARTIAL. The block list is read from "<unk_144>/mgx/mgxlist.txt" through GetFiles, whose read is a stub, so no .mgx
// name is found in this build. The rest is rebuilt: each name that contains ".mgx" is loaded with
// MgxBlock::FromFilename(folder, target, name) and inserted under the block's name (insert_unique, so a name that is
// already present keeps its first block), then every block links to the others.
bool RandomGenerator::LoadBlocks() {
    const std::string listPath = unk_144 + "/mgx/mgxlist.txt";
    std::vector<std::string> names;
    GetFiles(listPath.c_str(), names);
    for (const std::string& name : names) {
        if (name.find(".mgx") == std::string::npos) {
            continue;
        }
        MgxBlock* block = MgxBlock::FromFilename(unk_144.c_str(), unk_12c.c_str(), name.c_str());
        if (block != nullptr) {
            m_blocks.insert(std::make_pair(block->m_name.c_str(), static_cast<Block*>(block)));
        }
    }
    for (auto& entry : m_blocks) {
        entry.second->LinkToOtherBlocks(m_blocks);
    }
    return true;
}

// 0x489508 rnd::RandomGenerator::LoadRuleFile(char const*)
// PARTIAL. Not rebuilt past the first step. The original keeps the rule path in unk_174, splits it at the last
// separator (rfind with a character whose value is not visible in the listing, so the split is not rebuilt here),
// opens the file through the device file system (not declared), parses it into m_xmlDocument, reads the "rules"
// element, the "target" and "folder" attributes (unk_12c and unk_144), LoadBlocks, LoadListRules, LoadRoomPools, the
// RootRule and the room pool sizes, and returns whether all of them succeeded. The stub returns false.
bool RandomGenerator::LoadRuleFile(const char* path) {
    (void)path;
    return false;
}

// 0x489988 rnd::Block::IsStraight(void)
// PARTIAL. Not rebuilt. The original is true when the side count (+0x5c) is 2 and the direction of side 0 equals
// sDirections[DIRECTION_OPPOSITES[direction of side 1]]. rnd::Direction is not declared in the tree.
bool Block::IsStraight() const {
    return false;
}

// 0x4899e4 rnd::Block::GetFlag(void)
// PARTIAL. Not rebuilt. The original ORs 1 << (direction id of side i) over the first m_count sides (+0x5c, stride
// 300 bytes), where the id is the first int of the Direction that each side points to. rnd::Direction is not declared.
int Block::GetFlag() const {
    return 0;
}

// 0x489aec rnd::Block::LinkToOtherBlocks(std::map<char const*, rnd::Block*, lstr> const&)
// PARTIAL. Not rebuilt. For each side of this block and each side of every other block in the map, the original
// appends the other side's record to this side's link table when the two names vectors (side +0x1c, strings compared
// by content) match and the direction of the first side equals the opposite direction of the second. That needs
// rnd::Direction and the link table fields (side +0x28 count, +0x2c pointers), which are not declared.
void Block::LinkToOtherBlocks(const std::map<const char*, Block*, lstr>& blocks) {
    (void)blocks;
}

// 0x489ddc rnd::Exit::GetBlockUnitPosition(Point3D<float>&, rnd::Block*)
// Converts a position in block units to the grid cell it sits on. The unit width and height are unk_4c and unk_50
// (MgxBlock::LoadFromXml "unit_width" and "unit_height"); m_width (+0x54) and m_height (+0x58) are the block size in
// units. The cell is the ceiling of the offset from the block's near edge, minus one, and never negative. The
// original's return value is the new m_y, which the header keeps as void.
void Exit::GetBlockUnitPosition(Point3D<float>& position, Block* block) {
    const float unitWidth = block->unk_4c;   // +0x4c
    const float unitHeight = block->unk_50;  // +0x50
    const float halfWidth = (static_cast<float>(-block->m_width) * unitWidth) * 0.5f;    // +0x54
    const float cellX = std::ceil((position.X - halfWidth) / unitWidth) - 1.0f;
    const int x = static_cast<int>(cellX);
    m_x = x < 0 ? 0 : x;                                                                  // +0x08

    const float halfHeight = (static_cast<float>(block->m_height) * unitHeight) * 0.5f;  // +0x58
    const float cellY = std::ceil((halfHeight - position.Y) / unitHeight) - 1.0f;
    const int y = static_cast<int>(cellY);
    m_y = y < 0 ? 0 : y;                                                                  // +0x0c
}

// 0x48a380 / 0x48a3d4 rnd::Exit::Exit(rnd::Direction const&, rnd::Block*, int, int, float, int, std::vector<std::string>&)
// C1 and C2 are one definition. The block name pointer is the owning block's name (+0x18 of the block, the start of
// its name string). The names vector is copied. unk_28 (+0x28) is zeroed; in a block side record that word is the link
// count.
Exit::Exit(const Direction& direction, Block* block, int x, int y, float weight, int flags,
           std::vector<std::string>& names)
    : unk_00(block->m_name.c_str()),
      m_block(block),
      m_x(x),
      m_y(y),
      unk_10(weight),
      m_direction(&direction),
      unk_18(flags),
      m_names(names),
      unk_28(0) {}

// 0x48a5d0 rnd::Exit::LoadFromXml(TiXmlHandle, rnd::Block*, int)
// PARTIAL, not defined in this file. The original reads the "linktype" attribute (spaces removed, split at a
// separator character whose value is not visible in the listing), skips the exit when the last name is "0" or
// empty (TryIsZero), finds the "direction" attribute among the four rnd::Direction entries (or the entry after
// them), parses "position" into two floats and places the exit with GetBlockUnitPosition. The declaration takes
// TiXmlHandle by value, and the tree has no complete global TiXmlHandle (the copy in vox-audio is in namespace
// vox::tixml), so the definition cannot be written yet. rnd::Direction and StrToObj are also not declared.

// 0x48a90c rnd::MgxBlock::LoadFromXml(TiXmlHandle&)
// PARTIAL. Not rebuilt. The original reads "unit_width" and "unit_height" (doubles) into unk_4c and unk_50, and
// "block_width" and "block_height" (ints) into m_width and m_height, then walks the children: each child whose
// "gametype" is "link" (case-insensitive) is loaded into side m_count (the side record is the rnd::Exit record) and m_count
// grows by one when that load succeeds. The Side-to-Exit relation and the TinyXML attribute reads are not declared, so
// nothing is read here and the call returns false.
bool MgxBlock::LoadFromXml(TiXmlHandle& handle) {
    (void)handle;
    return false;
}

// 0x489f58 / 0x489fd8 rnd::Block::~Block()
// PARTIAL. The original destroys the names vector (std::vector<std::string>) of each of the eight side records, from
// m_sides[7] down to m_sides[0], then m_visual, m_gameplay and m_name. Block.hpp declares the side vector fields as
// void*, so the vectors are not released here. The string members are released by their own destructors.
// The C1 and C2 destructors are one definition. The deleting variant at 0x489fbc is skipped (see the ledger).
Block::~Block() {}

// 0x48a03c / 0x48a0b4 rnd::MgxBlock::~MgxBlock()
// PARTIAL. The original sets the MgxBlock vtable, releases the object at +0x9c4 through its vtable slot +4 when it is
// non-null, frees the buffer at +0x9c0, and then runs Block::~Block. Both fields are int32 in MgxBlock.hpp, so neither
// is released here. The deleting variant at 0x48a098 is skipped (see the ledger).
MgxBlock::~MgxBlock() {}

}  // namespace rnd
