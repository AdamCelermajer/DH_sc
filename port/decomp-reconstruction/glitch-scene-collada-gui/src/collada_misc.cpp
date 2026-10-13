#include "glitch-scene-collada-gui/collada_misc.hpp"
#include "glitch-scene-collada-gui/collada/CColladaDatabase.hpp"

namespace glitch::collada {

// 0x669b64 _ZN6glitch7collada12ISkinnedMesh20setIsSkinningEnabledEb
void ISkinnedMesh::setIsSkinningEnabled(bool enabled) {
    skinningEnabled = enabled;
}

namespace detail {

// 0x66b940 _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique6attachEPNS_5scene10ISceneNodeE
void CColladaHardwareMatrixSkinTechnique::attach(glitch::scene::ISceneNode* node) {
    attachedNode = node;
}

// 0x66d080 _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique6attachEPNS_5scene10ISceneNodeE
void CColladaHardwareQuatSkinTechnique::attach(glitch::scene::ISceneNode* node) {
    attachedNode = node;
}

// 0x66e778 _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique6attachEPNS_5scene10ISceneNodeE
void CColladaHardwareTextureSkinTechnique::attach(glitch::scene::ISceneNode* node) {
    attachedNode = node;
}

// 0x66f6c8 _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique6attachEPNS_5scene10ISceneNodeE
void CColladaSoftwareSkinTechnique::attach(glitch::scene::ISceneNode* node) {
    attachedNode = node;
}

}  // namespace detail

CAnimationStreamingManager* CAnimationStreamingManager::Instance = nullptr;

// 0x60c59c _ZN6glitch7collada26CAnimationStreamingManager17getAnimationBlockERNS0_24SAnimationBlockSearchKeyERN5boost13intrusive_ptrINS0_15CAnimationBlockEEE
// PARTIAL: the original first asks the held block for the matching neighbour (CAnimationBlock::getBlock 0x60b350), then the manager's
// registry (0x60c220: a lower-bound search over the sorted block vector, a new CAnimationBlock built from the key when none matches),
// and replaces the held block by the one found. The registry search and the block constructors are not reconstructed (stand-ins in
// recon/slice_07.cpp), so no block is found and the held block is left unchanged.
void CAnimationStreamingManager::getAnimationBlock(SAnimationBlockSearchKey&, CAnimationBlock*&) {}

// 0x60b3cc _ZN6glitch7collada26CAnimationStreamingManagerC2Ev
// 0x60b410 _ZN6glitch7collada26CAnimationStreamingManagerC1Ev
// Zeroes the eight words (the member initialisers) and publishes the object through the static Instance pointer.
CAnimationStreamingManager::CAnimationStreamingManager() {
    Instance = this;
}

}  // namespace glitch::collada
