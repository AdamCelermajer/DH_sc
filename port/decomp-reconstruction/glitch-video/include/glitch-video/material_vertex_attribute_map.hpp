// glitch::video::CMaterialVertexAttributeMap: one CVertexAttributeMap per parameter slot of a material
// renderer's current technique. Layout of the original object (constructors 0x5df7b8 / 0x5df2c0,
// 0x5df378 / 0x5df4b0, clone 0x5df464, set 0x5df814, totalMapCount 0x5df264):
//   +0x00  reference count (the constructors zero it; allocate and clone store 1)
//   +0x04  renderer (intrusive pointer; the renderer's count at its +0x00 is incremented)
//   +0x08  slot array, one CVertexAttributeMap pointer per slot, count = totalMapCount(renderer)
// The original allocates count + 2 words in one block (allocateBase). Here the slots are a std::vector.
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/vertex_attribute_map.hpp"

namespace glitch::video {

// One parameter block of a renderer: 52 bytes in the original (the element size of the block list).
struct SRendererParameterBlock {
    uint8_t bytes[52];
};

// One technique entry of a renderer: 12 bytes in the original.
struct STechniqueEntry {
    uint32_t field00 = 0;                             // +0x00, not identified
    uint8_t parameterCount = 0;                       // +0x04, number of parameters of the technique
    const SRendererParameterBlock* blocks = nullptr;  // +0x08, first parameter block of the technique
};

// Stand-in for glitch::video::CMaterialRenderer. The class is not reconstructed; only the words that
// CMaterialVertexAttributeMap reads are named, at the original offsets. Replace with the full class
// when it exists.
class CMaterialRenderer {
public:
    uint32_t m_refCount = 0;                              // +0x00, intrusive reference count
    uint8_t m_techniqueIndex10 = 0;                       // +0x10, 1-based technique index used by totalMapCount
    STechniqueEntry* m_techniques18 = nullptr;            // +0x18, technique table (12-byte entries)
    const SRendererParameterBlock* m_blocks1c = nullptr;  // +0x1c, base of the parameter blocks
};

class CMaterialVertexAttributeMap {
public:
    // 0x5df7b8 (C2) and 0x5df2c0 (C1): every slot starts empty. The renderer count is incremented.
    explicit CMaterialVertexAttributeMap(const boost::intrusive_ptr<CMaterialRenderer>& renderer);

    // 0x5df4b0 (C2) and 0x5df378 (C1): copy == true makes a new CVertexAttributeMap for each slot
    // (copy constructor from maps[i]); copy == false shares maps[i] and increments its count.
    // The original reads the slot count from totalMapCount(renderer).
    CMaterialVertexAttributeMap(const boost::intrusive_ptr<CMaterialRenderer>& renderer,
                                const boost::intrusive_ptr<CVertexAttributeMap>* maps, bool copy);

    // 0x5df264. The original is a member, but it reads only the renderer (passed as this): the byte at
    // +0x10 minus one selects the technique entry, its count byte is added to the number of 52-byte
    // parameter blocks between the renderer's block base and the entry's block list.
    static int32_t totalMapCount(const CMaterialRenderer& renderer);

    // 0x5df33c: new map with the renderer and empty slots, reference count 1. Stands in for
    // allocateBase (0x5df31c) plus the renderer-only constructor.
    static boost::intrusive_ptr<CMaterialVertexAttributeMap> allocate(
        const boost::intrusive_ptr<CMaterialRenderer>& renderer);

    // 0x5df464: new map with the same renderer and slots (copied or shared, see the second constructor),
    // reference count 1. Reads the source slot array as m_maps.data(); the count comes from the renderer.
    boost::intrusive_ptr<CMaterialVertexAttributeMap> clone(bool copy) const;

    // 0x5df814: stores map in the slot of (technique, parameterIndex). The slot index is the distance,
    // in 52-byte blocks, from the renderer's block base to the parameter block. The old map loses one
    // reference and is deleted when it was the last. Writes past the slot array are ignored here (the
    // original writes without a check).
    void set(uint8_t technique, uint8_t parameterIndex, const boost::intrusive_ptr<CVertexAttributeMap>& map);

    uint32_t m_refCount = 0;                                  // +0x00
    boost::intrusive_ptr<CMaterialRenderer> m_renderer;       // +0x04
    std::vector<boost::intrusive_ptr<CVertexAttributeMap>> m_maps;  // +0x08, one slot per totalMapCount
};

}  // namespace glitch::video
