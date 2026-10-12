// Shared layout notes for glitch-scene-collada-gui, derived from the Hex-Rays
// pseudocode (recovered/native/hexrays-all). Offsets are the 32-bit ARM ones.
// Only facts that a constructor, accessor or ledgered body confirms are listed.
// Anything not listed is still unmapped. If the IDA C header export arrives,
// reconcile against it before relying on these.
#pragma once

namespace glitch::scene::layouts {

// glitch::scene::CBatchMesh
//   +0x00 vptr
//   +0x04 IMesh refcount word (zeroed by the constructor 0x578924, used by the
//         refcount idiom that getMeshBuffer uses on a buffer at +0x04)
//   +0x08..+0x1c zeroed words, not named here
//   +0x20 batches begin, +0x24 batches end. getMeshBufferCount is
//         (end - begin) / 20 (0x578c28), so an SBatch is 0x14 bytes
//         (setMaterial 0x57a2ac indexes 5 * i words)
//   +0x2c..+0x34 zeroed words, not named here
//   +0x38..+0x4f first aabbox3d<float>, min and max zeroed to -10 and +1 by the
//         constructor. Its role is not confirmed
//   +0x50..+0x67 second aabbox3d<float> (boundingBox in scene_nodes.hpp). The
//         constructor sets min to -10 and max to +1
//   +0x68 boundingBoxSource, +0x6c boundingBoxStamp (constructor sets -1),
//   +0x70 word set to 44 (0x2c) by the constructor, role not confirmed
//   +0x74 boundingBoxDirty byte (constructor sets 1)
//   +0x78 word zeroed by the constructor
//   +0x58..+0x5c: the constructor writes -10.0f into these as part of the second box
//
// SBatch (0x14 bytes)
//   +0x00 meshBuffer, +0x04 material, +0x08 vertexAttributeMap (pointer words,
//         the refcount is on the pointee at +0x04 per getMeshBuffer and setMaterial)
//   +0x0c, +0x0e, +0x10 16-bit fields zeroed by SBatch()
//
// glitch::gui::IGUIElement
//   +0x04 children list (std::list in the original). The IGUIToolBar constructor
//         0x55b190 links the list sentinel to a1+4 at +0x04 and +0x08. The node
//         layout beyond that is not confirmed.
//   +0x154 guiType (getTypeName 0x5350a4 indexes the name table with it)
//   Other IGUIElement fields are in gui_elements.hpp and are not re-derived here.

}  // namespace glitch::scene::layouts
