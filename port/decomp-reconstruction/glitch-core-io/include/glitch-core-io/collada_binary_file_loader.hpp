// glitch::scene::CColladaBinaryFileLoader: loader for the binary Collada container. Only the
// constructor is recovered so far (two listings: C1 and C2, one definition). Layout of the
// original, 32-bit offsets kept as comments:
//   +0x00  vtable (not reconstructed; the port has no virtual overrides for this class)
//   +0x04  reference count (IReferenceCounted, starts at 1)
//   +0x08  scene manager (not owned)
//   +0x0c  file system (intrusive reference, added on construction)
#pragma once

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-core-io/deps/glitch_core.hpp"
#include "glitch-core-io/io/interfaces.hpp"

namespace glitch::scene {

class CSceneManager;

class CColladaBinaryFileLoader : public glitch::IReferenceCounted {
public:
    // 0x6b9310 (C2) and 0x6b9360 (C1): stores the manager and takes a reference on the file system.
    CColladaBinaryFileLoader(CSceneManager* manager, const boost::intrusive_ptr<glitch::io::IFileSystem>& fs);

    CSceneManager* sceneManager = nullptr;                     // +0x08
    boost::intrusive_ptr<glitch::io::IFileSystem> fileSystem;  // +0x0c
};

}  // namespace glitch::scene
