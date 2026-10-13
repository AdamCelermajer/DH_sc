// glitch::scene factory and loader interfaces that the scene manager holds: ISceneNodeFactory, ISceneNodeAnimatorFactory,
// IMeshLoader and IShadowReceiverTarget. Canonical header of the engine unification wave (u1-scene). They replace the
// empty-destructor stand-ins in scene_simple_accessors.hpp (classes with no base and no slots).
//
// Evidence (tools/decomp_vtable.py): each is a plain glitch::IReferenceCounted subclass (vtable header (0,0), (4,0)): slots
// +0x00 / +0x04 destructors, +0x08 onDelete, then the pure slots below, named by the implementations in
// CDefaultSceneNodeFactory (44 bytes), CDefaultSceneNodeAnimatorFactory (44 bytes) and CColladaBinaryFileLoader (32 bytes).
// IShadowReceiverTarget has no function in the queue; only its name (the element type of CSceneManager's target list) and the
// reference count are known.
#pragma once

#include <cstdint>

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-scene-collada-gui/scene/scene_types.hpp"

namespace glitch::video {
class ITexture;
}

namespace glitch::scene {

// 0x6b9cec (D1; empty), 0x6b9e44 (D0, skipped).
class ISceneNodeFactory : public glitch::IReferenceCounted {
public:
    ~ISceneNodeFactory() override;
    virtual ISceneNode* addSceneNode(E_SCENE_NODE_TYPE type, ISceneNode* parent) = 0;               // +0x0c
    virtual ISceneNode* addSceneNode(const char* typeName, ISceneNode* parent) = 0;                 // +0x10
    virtual std::int32_t getCreatableSceneNodeTypeCount() const = 0;                                 // +0x14
    virtual E_SCENE_NODE_TYPE getCreateableSceneNodeType(std::uint32_t index) const = 0;             // +0x18
    virtual const char* getCreateableSceneNodeTypeName(std::uint32_t index) const = 0;              // +0x1c
    virtual const char* getCreateableSceneNodeTypeName(E_SCENE_NODE_TYPE type) const = 0;            // +0x20
};

// 0x6b958c (D1; empty), 0x6b97c0 (D0, skipped).
class ISceneNodeAnimatorFactory : public glitch::IReferenceCounted {
public:
    ~ISceneNodeAnimatorFactory() override;
    virtual ISceneNodeAnimator* createSceneNodeAnimator(E_SCENE_NODE_ANIMATOR_TYPE type, ISceneNode* target) = 0;  // +0x0c
    virtual ISceneNodeAnimator* createSceneNodeAnimator(const char* typeName, ISceneNode* target) = 0;             // +0x10
    virtual std::uint32_t getCreatableSceneNodeAnimatorTypeCount() const = 0;                                      // +0x14
    virtual E_SCENE_NODE_ANIMATOR_TYPE getCreateableSceneNodeAnimatorType(std::uint32_t index) const = 0;          // +0x18
    virtual const char* getCreateableSceneNodeAnimatorTypeName(std::uint32_t index) const = 0;                     // +0x1c
    virtual const char* getCreateableSceneNodeAnimatorTypeName(E_SCENE_NODE_ANIMATOR_TYPE type) const = 0;         // +0x20
};

// 0x6b930c (D1; empty), 0x6b9578 (D0, skipped). The two slots are the ones CColladaBinaryFileLoader implements (+0x0c, +0x10); its +0x14 createScene
// is the loader's own.
class IMeshLoader : public glitch::IReferenceCounted {
public:
    ~IMeshLoader() override;
    virtual bool isALoadableFileExtension(const char* fileName) const = 0;                          // +0x0c
    virtual boost::intrusive_ptr<IAnimatedMesh> createMesh(io::IReadFile* file) = 0;                 // +0x10
};

// The target a shadow receiver texture or node registers with the scene manager (CSceneManager::addShadowReceiverTarget).
class IShadowReceiverTarget : public glitch::IReferenceCounted {
public:
    ~IShadowReceiverTarget() override;

    // +0x0c: the texture the target is registered for. Evidence: CSceneManager::removeShadowReceiverTarget (0x589754)
    // reads this word of each list entry and compares it with the texture argument (null matches null).
    glitch::video::ITexture* texture = nullptr;
};

}  // namespace glitch::scene
