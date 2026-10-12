// glitch::scene::ISceneNode small accessors. Only the members these accessors
// touch are mapped; the rest of the base layout (scene manager, transforms,
// name, material list, vtable) is not reconstructed yet. Offsets are kept as
// comments, and the code uses typed members only.
#pragma once

#include <cstdint>
#include <vector>

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

    // Mapped members (original offsets in comments).
    float absoluteTransform[16] = {};                     // +0x24
    std::vector<ISceneNode*> children;                    // +0xf0
    std::vector<ISceneNodeAnimator*> animators;           // +0xfc
    std::vector<ISceneNodeAnimator*> bindedAnimators;     // +0x104
    int id = 0;                                           // +0x10c
    std::uint32_t flags = 0;                              // +0x11c
    ITriangleSelector* triangleSelector = nullptr;        // +0x114
    void* gameData = nullptr;                             // +0x124
    float cameraOffset = 0.0f;                            // +0x128
    int renderingLayer = 0;                               // +0x12c
    std::uint32_t automaticCulling = 0;                   // +0x118
};

}  // namespace glitch::scene
