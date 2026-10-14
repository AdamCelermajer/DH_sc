// Declarations for the global_02 reconstruction slice (w7_global_02.cpp).
#pragma once

namespace glitch {
namespace scene {
class ISceneNode;
class IMesh;
}
}  // namespace glitch

// 0x50f778 _Z18disableCullingModePN6glitch5scene10ISceneNodeE
int disableCullingMode(glitch::scene::ISceneNode* node);

// Target of the disableCullingMode tail branch. Declared here only; its body is
// ledgered under another category (_Z14setCullingModePN6glitch5scene10ISceneNodeEi).
int setCullingMode(glitch::scene::ISceneNode* node, int mode);

// 0x52f0e8 getState (unmangled, exported with C linkage)
extern "C" int getState();

// 0x50fe78 _Z16FlushMeshBuffersPN6glitch5scene5IMeshEbbb (core-platform/src/w7_flush_mesh_buffers.cpp)
void FlushMeshBuffers(glitch::scene::IMesh* mesh, bool vertices, bool indices, bool force);
