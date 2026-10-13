// glitch::scene node, animator and manager classes. The canonical definitions of the classes below moved to the scene/ headers
// of the engine unification wave (u1-scene); this header includes them, so existing includes keep working, and still holds the
// classes that have no canonical header yet (CLightSceneNode, CBillboardSceneNode). Those two are the partial stand-ins of the
// earlier waves: they have no ISceneNode base and map only the members their accessors touch.
//
//   ISceneNodeAnimator            -> scene/ISceneNodeAnimator.hpp
//   ITimeController, ITimelineController -> scene/ITimelineController.hpp
//   ICameraSceneNode              -> scene/ICameraSceneNode.hpp
//   CCameraSceneNode, SViewFrustum -> scene/CCameraSceneNode.hpp, scene/SViewFrustum.hpp
//   CEmptySceneNode, CSceneManagerRootNode -> scene/CEmptySceneNode.hpp
//   CMeshSceneNode                -> scene/CMeshSceneNode.hpp
//   CSceneManager                 -> scene/CSceneManager.hpp
//   IMesh                         -> scene/IMesh.hpp
//   CBatchMesh, CBatchSceneNode   -> scene/CBatchMesh.hpp, scene/CBatchSceneNode.hpp
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/scene/CBatchMesh.hpp"
#include "glitch-scene-collada-gui/scene/CBatchSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CCameraSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CEmptySceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CMeshSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CSceneManager.hpp"
#include "glitch-scene-collada-gui/scene/ICameraSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/IMesh.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNodeAnimator.hpp"
#include "glitch-scene-collada-gui/scene/ITimelineController.hpp"
#include "glitch-scene-collada-gui/scene/SViewFrustum.hpp"
#include "glitch-scene-collada-gui/scene_node.hpp"

namespace glitch::io {
class IWriteFile;
}
namespace glitch::video {
class CLight;
}

namespace glitch::scene {

class CLightSceneNode {
public:
    // 0x583bec _ZNK6glitch5scene15CLightSceneNode7getTypeEv (tag "lght")
    std::uint32_t getType() const;
    // 0x583c18 _ZN6glitch5scene15CLightSceneNode13doLightRecalcEv
    // Partial: needs glitch::video::CLight fields at +0x40 (float range, FLT_MAX means
    // no culling box) and +0x58 (uint16 count). CLight has no definition in this category.
    void doLightRecalc();
    // 0x583cf0 _ZNK6glitch5scene15CLightSceneNode14getBoundingBoxEv
    // Partial: compares recalcCount with light->+0x58, which is not reachable here.
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x583d18 _ZN6glitch5scene15CLightSceneNode6renderEPv
    // Partial: the driver is read from the scene manager at +0x110 (+0x14 of it), and that
    // type is not mapped. The call is IVideoDriver::addDynamicLight(const intrusive_ptr<CLight>&).
    int render(void* arg);
    // 0x583d34 _ZN6glitch5scene15CLightSceneNode19onRegisterSceneNodeEv
    // Partial: calls vtable slot +36 of the scene manager at +0x110. The slot's target
    // and the manager's class are not identified.
    int onRegisterSceneNode();

    // Layout (original offsets). The base class is not reconstructed yet, so only these
    // words are mapped here.
    boost::intrusive_ptr<glitch::video::CLight> light;  // +0x134 (word 77), passed by address to addDynamicLight
    std::uint32_t recalcCount = 0;                      // +0x138 (word 78), last light count seen by doLightRecalc
    core::aabbox3d<float> boundingBox;                  // +0x13c..+0x154 (words 79..84), min then max
};

class CBillboardSceneNode {
public:
    // 0x580a2c _ZNK6glitch5scene19CBillboardSceneNode7getTypeEv (tag "bill")
    std::uint32_t getType() const;
    // 0x580a98 _ZNK6glitch5scene19CBillboardSceneNode14getBoundingBoxEv
    const core::aabbox3d<float>& getBoundingBox() const;
    // 0x580b34 _ZNK6glitch5scene19CBillboardSceneNode16getMaterialCountEv (returns 1)
    std::uint32_t getMaterialCount() const;
    // 0x580b3c _ZNK6glitch5scene19CBillboardSceneNode7getSizeEv
    const core::dimension2d<float>& getSize() const;

    core::dimension2d<float> size;           // +0x134
    core::aabbox3d<float> boundingBox;       // +0x13c
};


// CBatchMesh and CBatchSceneNode: canonical definitions in scene/CBatchMesh.hpp and scene/CBatchSceneNode.hpp.

}  // namespace glitch::scene
