// glitch::collada accessors on the synchronized animator set, the skinned-mesh
// and timeline bases, and the animator base's length. Only the members these
// accessors touch are mapped.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/collada/CAnimationSet.hpp"
#include "glitch-scene-collada-gui/collada/CSceneNodeAnimatorSynchronizedSet.hpp"
#include "glitch-scene-collada-gui/collada/CTimelineController.hpp"
#include "glitch-scene-collada-gui/collada/ISceneNodeAnimator.hpp"

namespace glitch::scene {
class ISceneNode;
class CMeshBuffer;
}

namespace glitch::video {
class IVideoDriver;
}

namespace glitch::core {
template <typename T>
struct aabbox3d;
}

namespace glitch::io {
class IAttributes;
struct SAttributeReadWriteOptions;
}  // namespace glitch::io

namespace glitch::collada {

class SSkinBuffer;

// SSyncSegment and CSceneNodeAnimatorSynchronizedSet: canonical in collada/ISceneNodeAnimator.hpp and
// collada/CSceneNodeAnimatorSynchronizedSet.hpp.

class ISkinnedMesh {
public:
    // 0x669b64 _ZN6glitch7collada12ISkinnedMesh20setIsSkinningEnabledEb
    void setIsSkinningEnabled(bool enabled);

    bool skinningEnabled = false;  // +0x18
};

// STimelineClip, STimelineClipTable and CTimelineController: canonical definitions in collada/CTimelineController.hpp.

// collada::ISceneNodeAnimator: canonical in collada/ISceneNodeAnimator.hpp.

namespace detail {

// Hardware skin techniques. Only the attach hook is mapped: it stores the scene node
// the technique is attached to at +0x14. The rest of each class is not modelled yet.
class CColladaHardwareMatrixSkinTechnique {
public:
    // 0x66b940 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);
    // 0x66c5f4 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique18computeBoundingBoxEv
    // Returns the box by value. Partial: see the body in src/tb_scene-collada-gui-glitch_collada_detail_CColladaHardwareMatrixSkin-0.cpp.
    glitch::core::aabbox3d<float> computeBoundingBox();

    std::uint8_t pad_0000[0x14] = {};                   // +0x00..+0x13 (not mapped)
    glitch::scene::ISceneNode* attachedNode = nullptr;  // +0x14
};

class CColladaHardwareQuatSkinTechnique {
public:
    // 0x66d080 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);
    // 0x66df88 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique15preparePtrCacheEv
    // Declared only: the member body is not reconstructed yet (the free stub in recon/slice_23 is not it).
    void preparePtrCache();
    // 0x66e384 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique18computeBoundingBoxEv
    // Returns the box by value (sret). Partial: needs SSkin and SSkinCache layouts; see the .cpp.
    glitch::core::aabbox3d<float> computeBoundingBox();

    std::uint8_t pad_0000[0x14] = {};                   // +0x00..+0x13 (not mapped)
    glitch::scene::ISceneNode* attachedNode = nullptr;  // +0x14
};

class CColladaHardwareTextureSkinTechnique {
public:
    // 0x66e778 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);

    std::uint8_t pad_0000[0x14] = {};                   // +0x00..+0x13 (not mapped)
    glitch::scene::ISceneNode* attachedNode = nullptr;  // +0x14
};

// Software skin technique. Only the attach hook is mapped; its constructor (C1 and C2)
// also stores the skin and cache references, which are not reconstructed here.
class CColladaSoftwareSkinTechnique {
public:
    // 0x66f6c8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique6attachEPNS_5scene10ISceneNodeE
    void attach(glitch::scene::ISceneNode* node);

    // 0x66f8ec _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4initERNS0_11SSkinBufferEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverEb
    // Partial: stream setup and the process-buffer request are not reconstructed (see the .cpp).
    int init(glitch::collada::SSkinBuffer& skinBuffer, glitch::scene::CMeshBuffer* meshBuffer,
             glitch::video::IVideoDriver* driver, bool skipProcessBuffer);
    // 0x66fab4 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique15preparePtrCacheEv
    // Partial: the skin's joint-name array and the scene-node virtual slot are not mapped.
    void preparePtrCache();
    // 0x66fb84 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique17computeBoundingBoxEv
    // Partial: the receiver and argument roles disagree with the symbol (see the .cpp).
    void computeBoundingBox();
    // 0x66fe34 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique12prepareCacheEv
    // Partial: the SMatrix layout and the skin-cache matrix vector are not mapped.
    void prepareCache();
    // 0x66ff48 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique4skinERNS0_11SSkinBufferEPNS_5scene11CMeshBufferE
    // Partial: vertex-stream and index decoding need the SSkinBuffer and mesh-buffer layouts.
    int skin(glitch::collada::SSkinBuffer& skinBuffer, glitch::scene::CMeshBuffer* meshBuffer);

    std::uint8_t pad_0000[0x14] = {};                   // +0x00..+0x13 (not mapped)
    glitch::scene::ISceneNode* attachedNode = nullptr;  // +0x14
};

}  // namespace detail

// CAnimationStreamingManager: canonical definition in collada/CColladaDatabase.hpp.

// SAnimationTemplate -> STemplateTarget and CAnimationSetTransformationTemplate: canonical in collada/CAnimationSet.hpp.

}  // namespace glitch::collada
