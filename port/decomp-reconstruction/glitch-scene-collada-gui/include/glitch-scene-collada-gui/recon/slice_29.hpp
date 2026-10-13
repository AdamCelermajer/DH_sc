// Slice 29 reconstruction (see src/recon/slice_29.cpp).
//
// Only the classes whose members the pseudocode confirms are declared here.
// CMeshConnectivity, SSilhouette and SShadowVolume are nested in the same
// namespace as their owner (glitch::scene::CShadowVolumeSceneNode in the
// original); the owner class itself has no canonical header yet, so
// it is declared empty. CMeshBuffer is the canonical scene/CMeshBuffer.hpp class. Offsets in the comments are the 32-bit ARM ones.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"

namespace glitch::scene::recon_29 {

// glitch::scene::CMeshConnectivity. The first word is the mesh buffer handle
// (refcounted; the count is the buffer's +0x04 word). The constructor zeroes the
// five words that follow; their roles are not confirmed.
class CMeshConnectivity {
public:
    explicit CMeshConnectivity(const boost::intrusive_ptr<const CMeshBuffer>& buffer);

    boost::intrusive_ptr<const CMeshBuffer> buffer;  // +0x00
    std::uint32_t reserved_0x04[5] = {};              // +0x04..+0x17 zeroed by the constructor; role not confirmed
};

// glitch::scene::CShadowVolumeSceneNode (only the nested types are mapped).
class CShadowVolumeSceneNode {
public:
    // glitch::scene::CShadowVolumeSceneNode::SSilhouette (its base is CMeshConnectivity).
    struct SSilhouette : CMeshConnectivity {
        explicit SSilhouette(const boost::intrusive_ptr<const CMeshBuffer>& buffer);

        void* data_0x18 = nullptr;          // +0x18 released with the allocator in the destructor
        std::uint32_t word_0x1c = 0;        // +0x1c zeroed by the constructor; role not confirmed
        std::uint32_t word_0x20 = 0;        // +0x20 zeroed by the constructor; role not confirmed
        void* array_0x24 = nullptr;         // +0x24 released with delete[] in the destructor
    };

    // glitch::scene::CShadowVolumeSceneNode::SShadowVolume. The copy and the
    // destructor are not rebuilt here (refcounted members with no helper type).
    struct SShadowVolume {
        SShadowVolume();

        SSilhouette* silhouette = nullptr;  // +0x00 owned; the destructor deletes it
        std::uint32_t word_0x04 = 0;        // +0x04 copied by the copy operations; role not confirmed
        std::uint32_t word_0x08 = 0;        // +0x08
        std::uint32_t word_0x0c = 0;        // +0x0c
        void* array_0x10 = nullptr;         // +0x10 released with delete[] in the destructor
        void* array_0x14 = nullptr;         // +0x14 released with delete[] in the destructor
        std::uint32_t word_0x18 = 0;        // +0x18
        std::uint32_t word_0x1c = 0;        // +0x1c
        void* ref_0x20 = nullptr;           // +0x20 refcounted vertex stream block (count at +0)
        void* ref_0x24 = nullptr;           // +0x24 refcounted (count at +0x04)
        void* ref_0x28 = nullptr;           // +0x28 refcounted (count at +0x04)
        void* ref_0x2c = nullptr;           // +0x2c refcounted (count at +0x04)
        std::uint32_t word_0x30 = 0;        // +0x30..+0x3f zeroed by the constructor; copied by the copy operations
        std::uint32_t word_0x34 = 0;
        std::uint32_t word_0x38 = 0;
        std::uint32_t word_0x3c = 0;
        std::uint16_t half_0x40 = 255;      // +0x40 set to 255 by the constructor; role not confirmed
        std::uint16_t half_0x42 = 6;        // +0x42 set to 6 by the constructor
        std::uint16_t half_0x44 = 0;        // +0x44 zeroed by the constructor
        std::uint16_t half_0x46 = 0xFFFF;   // +0x46 set to -1 by the constructor
        std::uint16_t half_0x48 = 0;        // +0x48 zeroed by the constructor
    };
};

}  // namespace glitch::scene::recon_29
