// Batch g1-game-app-rnd-8b06ae-7 (rnd, part 8 of 8): the rnd::Tile functions that the earlier rnd batches did not cover.
// The constructors and destructors, the child list (AddChild, RemoveChild), Print, the placement pair (TryPlaceTile,
// PlaceTile), the spawn and removal path (NewRoot, Spawn, TrySpawn, Unspawn, RemoveNeighbors), SetModuleMVXProperties
// and SaveAsModuleXML. Bodies follow the Hex-Rays pseudocode of libDungeonHunter2.so.
//
// Duplicate addresses: each C1/C2 or D1/D2 pair has one identical body, written once and listed under both addresses:
//   Tile::Tile(RandomGenerator&, Block&, Tile*, ListElem&)   0x4917f8, 0x491850
//   Tile::~Tile()                                            0x4914e8, 0x491500
//
// PARTIAL (the ledger notes repeat these):
//   - Tile::PlaceTile: the original returns the result of Block::PlaceInMap in r0. The header declares void, and the
//     test of batch -3 stubs the same void signature, so the result is dropped here. Every side effect is written.
//   - Tile::SetModuleMVXProperties: the path "<gameplay>/mvx/<name>.mvx" is built. The read of that file from the file
//     system at dword_99F73C+0x34 (IDevice +0x34, not declared in the tree) and the TinyXML parse are not reproduced
//     (the vox-audio TinyXML copy declares no TiXmlDocument::LoadFromBuffer and no TiXmlElement::Attribute), so no
//     property is set.
//   - Tile::TrySpawn: the offset of the opposite direction comes from rnd::Direction::sDirections through
//     rnd::DIRECTION_OPPOSITES. rnd::Direction is not declared in the tree (no dh2/rnd/Direction.hpp), so the function
//     returns null and spawns nothing.
//   - Tile::SaveAsModuleXML: the module element, its name, position, data paths and children are written. Its mvx
//     attributes come from SetModuleMVXProperties, which is partial.
//
// Storage: the original allocates tiles with operator new(0x90) and frees them with CustomFree in Unspawn; the port
// uses CustomAlloc with placement new and CustomFree, as the batch -3 factories do.
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <new>
#include <string>
#include <vector>

#include "core-platform/memory.hpp"
#include "dh2/Module.hpp"
#include "dh2/PropertyMap.hpp"
#include "dh2/rnd/Block.hpp"
#include "dh2/rnd/ListElem.hpp"
#include "dh2/rnd/ListRule.hpp"
#include "dh2/rnd/RandomGenerator.hpp"
#include "dh2/rnd/Tile.hpp"

namespace rnd {

// 0x4913d0 rnd::Tile::TryPlaceTile(int,int)
// The body is only "return 1" in the listing.
bool Tile::TryPlaceTile(int /*x*/, int /*y*/) {
    return true;
}

// 0x4913d8 rnd::Tile::PlaceTile(int,int,float)
// PARTIAL: the original returns the result of Block::PlaceInMap (vtable slot 0x08, called with the generator's tile map,
// this, x and y) in r0. The header and the batch -3 test declare this function as void, so that result is dropped.
// No caller in the tree reads it. The stores of x, y and z happen first, as in the original.
void Tile::PlaceTile(int x, int y, float z) {
    m_z = z;
    m_x = x;
    m_y = y;
    m_block->PlaceInMap(m_generator->m_tiles, this, x, y);
}

// 0x491428 rnd::Tile::AddChild(rnd::Tile*)
// The original does not check the slot count. Eight slots suffice: a block has at most eight sides, and each side
// spawns at most one child.
void Tile::AddChild(Tile* child) {
    m_children[m_childCount] = child;
    m_childCount = m_childCount + 1;
}

// 0x491440 rnd::Tile::RemoveChild(rnd::Tile*)
// Searches the children in order. When the child is found, the last child moves into its slot and the count drops by one.
// Nothing changes when the child is not present.
void Tile::RemoveChild(Tile* child) {
    const int32_t count = m_childCount;
    for (int32_t i = 0; i < count; ++i) {
        if (m_children[i] == child) {
            m_childCount = count - 1;
            m_children[i] = m_children[count - 1];
            return;
        }
    }
}

// 0x4914ac rnd::Tile::Print(void)
// The release body writes nothing for the node itself. The listing has no output call: it visits the children in order.
void Tile::Print() {
    for (int32_t i = 0; i < m_childCount; ++i) {
        m_children[i]->Print();
    }
}

// 0x4914e8 rnd::Tile::~Tile() (D1)
// 0x491500 rnd::Tile::~Tile() (D2, same body)
// The only work is ListElem::~ListElem on the copy at +0x34, which the implicit member destructor performs.
Tile::~Tile() {}

// 0x491518 rnd::Tile::SetModuleMVXProperties(Module*)
// PARTIAL. The original builds the path "<block gameplay>/mvx/<block name>.mvx" (the path is built here), then opens it on
// the file system at dword_99F73C+0x34 (IDevice +0x34, not declared in the tree), loads it into a TinyXML document,
// finds the "Module" then "GameObject" elements, and copies their attributes scale, xrefmax, xrefobject, dae, fog_color
// and is_solid into the module's PropertyMap (the module at +4). The read and the attribute copy are not reproduced.
void Tile::SetModuleMVXProperties(Module* module) {
    const std::string mvxPath = m_block->m_gameplay + "/mvx/" + m_block->m_name + ".mvx";
    (void)module;
    (void)mvxPath;
}

// 0x4917f8 rnd::Tile::Tile(rnd::RandomGenerator&,rnd::Block&,rnd::Tile*,rnd::ListElem&) (C1)
// 0x491850 rnd::Tile::Tile(rnd::RandomGenerator&,rnd::Block&,rnd::Tile*,rnd::ListElem&) (C2, same body)
// Stores the generator, the parent and the block, sets the child count to zero, and takes the block's name pointer
// (the Block::m_name c_str). Then the parent gets this tile as a child, and the element is copied into the tile.
// The original sets the name pointer after the child link, which has no effect on the other fields.
Tile::Tile(RandomGenerator& generator, Block& block, Tile* parent, ListElem& elem)
    : m_blockName(block.m_name.c_str()),
      m_generator(&generator),
      m_parent(parent),
      m_childCount(0),
      m_block(&block),
      m_listElem() {
    if (m_parent != nullptr) {
        m_parent->AddChild(this);
    }
    m_listElem = elem;
}

// 0x4918a8 rnd::Tile::Unspawn(void)
// Removes the tile from its parent, then from the block's map cell, then removes its descendants, then hands the element
// back to its list (when the element has an owner). The tile is destroyed and its memory freed.
void Tile::Unspawn() {
    if (m_parent != nullptr) {
        m_parent->RemoveChild(this);
    }
    m_block->RemoveFromMap(m_generator->m_tiles, this, m_x, m_y);
    RemoveNeighbors();
    if (m_listElem.m_owner != nullptr) {
        m_listElem.m_owner->Replace(m_listElem);
    }
    this->~Tile();
    CustomFree(this);
}

// 0x491928 rnd::Tile::RemoveNeighbors(void)
// Takes the children from the last slot down. Each child first loses its own children, then is unspawned. The count is
// lowered before the child is unspawned, so the unspawn's RemoveChild does not find it again.
void Tile::RemoveNeighbors() {
    for (int32_t i = m_childCount; i > 0; i = m_childCount) {
        m_childCount = i - 1;
        Tile* child = m_children[i - 1];
        child->RemoveNeighbors();
        child->Unspawn();
    }
}

// 0x49196c rnd::Tile::NewRoot(rnd::RandomGenerator&,rnd::Block&,rnd::ListElem&)
// Creates a tile with no parent. It is not placed in the map; the caller places it.
Tile* Tile::NewRoot(RandomGenerator& generator, Block& block, ListElem& elem) {
    void* memory = CustomAlloc(static_cast<unsigned int>(sizeof(Tile)));
    return new (memory) Tile(generator, block, nullptr, elem);
}

// 0x4919b0 rnd::Tile::Spawn(rnd::Block&,int,int,float,rnd::ListElem&)
// Creates a child of this tile for the block and the element. When the element's list is not a replacement list, the
// first candidate with the same gameplay and name is erased from that list. The new tile is then placed at (x, y, z).
Tile* Tile::Spawn(Block& block, int x, int y, float z, ListElem& elem) {
    void* memory = CustomAlloc(static_cast<unsigned int>(sizeof(Tile)));
    Tile* tile = new (memory) Tile(*m_generator, block, this, elem);
    ListRule* owner = elem.m_owner;
    if (owner != nullptr && !owner->m_replacement) {
        std::vector<ListElem>& candidates = owner->m_elems;
        for (auto it = candidates.begin(); it != candidates.end(); ++it) {
            if (it->m_gameplay == elem.m_gameplay && it->m_name == elem.m_name) {
                candidates.erase(it);
                break;
            }
        }
    }
    tile->PlaceTile(x, y, z);
    return tile;
}

// 0x491ab0 rnd::Tile::TrySpawn(rnd::Exit const*,rnd::Exit const*,rnd::Exit const*,rnd::ListElem&)
// PARTIAL. The original reads the direction of the "to" exit (its first word, the direction id), maps it through
// rnd::DIRECTION_OPPOSITES to an entry of rnd::Direction::sDirections, and takes that entry's x and y. It adds those
// to this tile's cell, adds the "from" exit's offset and height when given, and asks the exit's block whether the cell
// fits (vtable slot 0x10, FitsInMap). When it fits, it calls Spawn with the exit's block and the new cell and height.
// rnd::Direction is not declared in the tree, so nothing is computed and the function returns null.
Tile* Tile::TrySpawn(const Exit* /*unused*/, const Exit* /*from*/, const Exit* /*to*/, ListElem& /*elem*/) {
    return nullptr;
}

// 0x491d90 rnd::Tile::SaveAsModuleXML(TiXmlElement*,int)
// Writes this tile as a Module element under parent, then the children, and returns the index after the last one.
// PARTIAL: the mvx attributes come from SetModuleMVXProperties, which is partial. The original also stores the literal
// "Module" in a local that nothing reads; that dead store is not reproduced.
int Tile::SaveAsModuleXML(TiXmlElement* parent, int index) {
    // The original constructs the module with GO_IDS value 5 (the id passed in r1). The ObjectBase enum in this tree
    // names only GO_DOOR, so the value is written as a cast.
    Module module(static_cast<Module::GO_IDS>(5));
    PropertyMap& props = module;  // the PropertyMap base, at +4 of the object
    props.InitProperties();
    props.LoadDefaultProperties();

    // name = "<block name>_<index>", gametype = "Module"
    const std::string name = std::string(m_blockName) + "_" + std::to_string(index);
    props.SetProperty("name", name.c_str());
    props.SetProperty("gametype", "Module");

    // position = "x,y,z". The x and y are the cell's centre in world units; y is negated (the original flips the sign
    // bit of the block's unit height). The buffer is larger than the original's 92 bytes, so ordinary values are not cut.
    const Block& block = *m_block;
    const float cellX = (static_cast<float>(m_x) + static_cast<float>(static_cast<float>(block.m_blockWidth - 1) * 0.5f)) *
                        block.m_unitWidth;
    const float cellY = (static_cast<float>(m_y) + static_cast<float>(static_cast<float>(block.m_blockHeight - 1) * 0.5f)) *
                        -block.m_unitHeight;
    char position[160];
    std::snprintf(position, sizeof(position), "%f,%f,%f", static_cast<double>(cellX), static_cast<double>(cellY),
                  static_cast<double>(m_z));
    props.SetProperty("position", position);

    SetModuleMVXProperties(&module);

    // dataPath = "data/" + block visual + "/" + the block gameplay after "data/" (or from index 4 when "data/" is absent).
    // substr throws std::out_of_range when the offset is past the end, as the original's range check does.
    const std::string& gameplay = block.m_gameplay;
    const std::size_t found = gameplay.find("data/");
    const std::size_t skip = (found == std::string::npos) ? 4 : found + 5;
    const std::string dataPath = "data/" + block.m_visual + "/" + gameplay.substr(skip);

    // mgp and mvp come from the spawning element's gameplay and visual names, when those are set.
    if (!m_listElem.m_gameplay.empty()) {
        const std::string mgp = dataPath + "/mgp/" + m_listElem.m_gameplay;
        props.SetProperty("mgp", mgp.c_str());
    }
    if (!m_listElem.m_visual.empty()) {
        const std::string mvp = dataPath + "/mvp/" + m_listElem.m_visual;
        props.SetProperty("mvp", mvp.c_str());
    }
    props.SavePropertiesToXML(parent, nullptr);

    int next = index;
    for (int32_t i = 0; i < m_childCount; ++i) {
        next = m_children[i]->SaveAsModuleXML(parent, next + 1);
    }
    return next;
}

}  // namespace rnd
