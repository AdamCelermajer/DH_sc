// glitch::scene::ISceneNode small accessors. Only the members these accessors
// touch are mapped; the rest of the base layout (scene manager, transforms,
// name, material list, vtable) is not reconstructed yet. Offsets are kept as
// comments, and the code uses typed members only.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::video {
class CMaterial;
}

namespace glitch::scene {

class ISceneNode;
class ISceneNodeAnimator;
class ITriangleSelector;

// Bit 0 of this word is the visible flag (see visibility_ex.hpp); bit 7 is the
// debug-object flag.
inline constexpr std::uint32_t kSceneNodeDebugObjectBit = 0x80;

class ISceneNode {
public:
    // 0x35a8e0 _ZNK6glitch5scene10ISceneNode25getAbsoluteTransformationEv
    // Returns the address of the absolute transform member (+0x24, 16 floats).
    const float* getAbsoluteTransformation() const;

    // 0x596e24 _ZNK6glitch5scene10ISceneNode20getRenderVertexCountEPv
    std::uint32_t getRenderVertexCount(void* /*unused*/) const;

    // 0x596f5c _ZNK6glitch5scene10ISceneNode5getIDEv
    int getID() const;
    // 0x596f64 _ZN6glitch5scene10ISceneNode5setIDEi
    void setID(int id);

    // 0x596fe4 _ZNK6glitch5scene10ISceneNode6getUIDEv
    const char* getUID() const;
    // 0x596ff4 _ZNK6glitch5scene10ISceneNode10getScopeIDEv
    const char* getScopeID() const;

    // 0x597094 _ZNK6glitch5scene10ISceneNode12getAnimatorsEv
    const std::vector<ISceneNodeAnimator*>& getAnimators() const;
    // 0x59709c _ZNK6glitch5scene10ISceneNode18getBindedAnimatorsEv
    const std::vector<ISceneNodeAnimator*>& getBindedAnimators() const;

    // 0x5970b0 _ZNK6glitch5scene10ISceneNode16getMaterialCountEv
    std::uint32_t getMaterialCount() const;
    // 0x5970a4 _ZNK6glitch5scene10ISceneNode11getMaterialEj
    video::CMaterial* getMaterial(unsigned index) const;

    // 0x5971c8 _ZNK6glitch5scene10ISceneNode11getChildrenEv
    const std::vector<ISceneNode*>& getChildren() const;

    // 0x5971d0 _ZNK6glitch5scene10ISceneNode15getUserPropertyEv
    const void* getUserProperty() const;
    // 0x5971d8 _ZNK6glitch5scene10ISceneNode18getUserPropertyStrEv
    const void* getUserPropertyStr() const;

    // 0x597254 _ZNK6glitch5scene10ISceneNode19getTriangleSelectorEv
    ITriangleSelector* getTriangleSelector() const;

    // 0x597298 _ZNK6glitch5scene10ISceneNode7getTypeEv
    // Original returns the constant 0x6e6b6e75 ("unkn").
    std::uint32_t getType() const;

    // 0x5974cc _ZN6glitch5scene10ISceneNode11setGameDataEPv
    void setGameData(void* data);
    // 0x5974d4 _ZNK6glitch5scene10ISceneNode11getGameDataEv
    void* getGameData() const;

    // 0x5974dc _ZN6glitch5scene10ISceneNode15setCameraOffsetEf
    void setCameraOffset(float offset);
    // 0x5974e4 _ZNK6glitch5scene10ISceneNode15getCameraOffsetEv
    float getCameraOffset() const;

    // 0x5974ec _ZN6glitch5scene10ISceneNode17setRenderingLayerEi
    void setRenderingLayer(int layer);
    // 0x5974f4 _ZNK6glitch5scene10ISceneNode17getRenderingLayerEv
    int getRenderingLayer() const;

    // 0x59719c _ZN6glitch5scene10ISceneNode19setAutomaticCullingENS0_14E_CULLING_TYPEE
    // E_CULLING_TYPE is a 32-bit enum; the value is stored as given.
    void setAutomaticCulling(std::uint32_t culling);

    // 0x5971bc _ZNK6glitch5scene10ISceneNode13isDebugObjectEv
    bool isDebugObject() const;
    // 0x5971a4 _ZN6glitch5scene10ISceneNode16setIsDebugObjectEb
    void setIsDebugObject(bool debug);

    // 0x5974c4 _ZN6glitch5scene10ISceneNode5cloneEv
    // The base implementation returns null; derived nodes override it.
    ISceneNode* clone();

    // Empty bodies in the original (a single return).
    // 0x5970b8 _ZN6glitch5scene10ISceneNode21notifyMaterialChangedEv
    void notifyMaterialChanged();
    // 0x596ec0 _ZN6glitch5scene10ISceneNode21onChangedSceneManagerEv
    void onChangedSceneManager();
    // 0x5974fc _ZN6glitch5scene10ISceneNode17onMaterialChangedEv
    void onMaterialChanged();
    // 0x597544 _ZN6glitch5scene10ISceneNode14resetTransformEb
    void resetTransform(bool keepTransform);

    // Transform, hierarchy and animation bodies (batch e1-glitch-scene-collada-gui-glitch_scene_ISceneNode-0).
    // The original methods are virtual (vtable slots noted per method); this class has no vptr yet,
    // so the declarations are non-virtual.
    // 0x596d08 glitch::scene::ISceneNode::onRegisterSceneNode() (slot 0x1c)
    int onRegisterSceneNode();
    // 0x596d6c glitch::scene::ISceneNode::onAnimate(unsigned int) (slot 0x20)
    void onAnimate(unsigned int timeMs);
    // 0x596e2c glitch::scene::ISceneNode::getName() const (slot 0x30)
    int getName() const;
    // 0x597004 glitch::scene::ISceneNode::removeChild(glitch::scene::ISceneNode*) (slot 0x6c)
    int removeChild(ISceneNode* child);
    // 0x5970bc glitch::scene::ISceneNode::getScale() const (slot 0x9c)
    const core::vector3d<float>& getScale() const;
    // 0x5970c4 glitch::scene::ISceneNode::setScale(glitch::core::vector3d<float> const&) (slot 0xa0)
    void setScale(const core::vector3d<float>& value);
    // 0x5970ec glitch::scene::ISceneNode::getRotation() const (slot 0xa4)
    const core::quaternion& getRotation() const;
    // 0x5970f4 glitch::scene::ISceneNode::setRotation(glitch::core::quaternion const&) (slot 0xa8)
    void setRotation(const core::quaternion& value);
    // 0x597124 glitch::scene::ISceneNode::getPosition() const (slot 0xac)
    const core::vector3d<float>& getPosition() const;
    // 0x59712c glitch::scene::ISceneNode::setPosition(glitch::core::vector3d<float> const&) (slot 0xb0)
    void setPosition(const core::vector3d<float>& value);
    // 0x597154 glitch::scene::ISceneNode::setPosition(float, float, float)
    void setPosition(float x, float y, float z);
    // 0x597180 glitch::scene::ISceneNode::getAbsolutePosition() const
    core::vector3d<float> getAbsolutePosition() const;
    // 0x597290 glitch::scene::ISceneNode::getParent() const
    ISceneNode* getParent() const;
    // 0x59770c glitch::scene::ISceneNode::getTransformedBoundingBox() const (slot 0x40)
    const core::aabbox3d<float>& getTransformedBoundingBox() const;
    // 0x597c60 glitch::scene::ISceneNode::updateAbsolutePosition(bool) (slot 0xc4)
    void updateAbsolutePosition(bool includeChildren);
    // 0x598864 glitch::scene::ISceneNode::addChild(glitch::scene::ISceneNode*) (slot 0x68)
    ISceneNode* addChild(ISceneNode* child);
    // 0x598908 glitch::scene::ISceneNode::getRelativeTransformation() const (slot 0x4c)
    const core::CMatrix4<float>& getRelativeTransformation() const;

    // Mapped members (original offsets in comments).
    float absoluteTransform[16] = {};                     // +0x24
    std::vector<ISceneNode*> children;                    // +0xf0
    std::vector<ISceneNodeAnimator*> animators;           // +0xfc
    std::vector<ISceneNodeAnimator*> bindedAnimators;     // +0x104
    int id = 0;                                           // +0x10c
    mutable std::uint32_t flags = 0;                      // +0x11c (mutable: getRelativeTransformation updates it)
    ITriangleSelector* triangleSelector = nullptr;        // +0x114
    void* gameData = nullptr;                             // +0x124
    float cameraOffset = 0.0f;                            // +0x128
    int renderingLayer = 0;                               // +0x12c
    std::uint32_t automaticCulling = 0;                   // +0x118

    // Members for the transform and hierarchy bodies. Type of the word at +0x20 is not confirmed.
    std::int32_t nameWord = 0;                            // +0x20
    core::vector3d<float> position{};                     // +0xac
    core::quaternion rotation{};                          // +0xb8
    core::vector3d<float> scale{};                        // +0xc8
    mutable core::aabbox3d<float> transformedBoundingBox{};  // +0xd4 (24 bytes)
    ISceneNode* parent = nullptr;                         // +0xec
    // 65-byte CMatrix4 (16 floats, then the flag byte at +0xa8); the relative transform
    // of the original, updated in place by getRelativeTransformation.
    mutable core::CMatrix4<float> relativeTransform{};    // +0x68
};

}  // namespace glitch::scene
