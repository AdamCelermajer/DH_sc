#include "glitch-scene-collada-gui/collada_misc.hpp"

namespace glitch::collada {

// 0x368db4 _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet14getNumSegmentsEv
std::uint32_t CSceneNodeAnimatorSynchronizedSet::getNumSegments() const {
    return segmentCount;
}

// 0x368eac _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet13hasReachedEndEv
bool CSceneNodeAnimatorSynchronizedSet::hasReachedEnd() const {
    return reachedEnd;
}

// 0x368dc4 _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet24getSynchronizationLengthEj
std::uint32_t CSceneNodeAnimatorSynchronizedSet::getSynchronizationLength(unsigned index) const {
    return segments[index].length;
}

// 0x368dfc _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet27getSynchronizationTimestampEj
std::uint32_t CSceneNodeAnimatorSynchronizedSet::getSynchronizationTimestamp(unsigned index) const {
    return segments[index].timestamp;
}

// 0x669b64 _ZN6glitch7collada12ISkinnedMesh20setIsSkinningEnabledEb
void ISkinnedMesh::setIsSkinningEnabled(bool enabled) {
    skinningEnabled = enabled;
}

// 0x666c18 _ZNK6glitch7collada19CTimelineController8getScaleEv
float CTimelineController::getScale() const {
    return scale;
}

// 0x666cc4 _ZNK6glitch7collada19CTimelineController12getClipCountEv
std::int32_t CTimelineController::getClipCount() const {
    return clipTable ? clipTable->count : 0;
}

// 0x666cd4 _ZNK6glitch7collada19CTimelineController12getClipStartEi
std::uint32_t CTimelineController::getClipStart(int index) const {
    return clipTable->clips[index].start;
}

// 0x666cec _ZNK6glitch7collada19CTimelineController10getClipEndEi
std::uint32_t CTimelineController::getClipEnd(int index) const {
    return clipTable->clips[index].end;
}

// 0x666d84 _ZNK6glitch7collada19CTimelineController11getClipNameEi
const char* CTimelineController::getClipName(int index) const {
    return clipTable->clips[index].name;
}

// 0x667cc4 _ZNK6glitch7collada18ISceneNodeAnimator9getLengthEv
std::uint32_t ISceneNodeAnimator::getLength() const {
    return length;
}

// 0x368ebc _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet29getSynchronizationElapsedTimeEv
std::int32_t CSceneNodeAnimatorSynchronizedSet::getSynchronizationElapsedTime() const {
    return static_cast<std::int32_t>(elapsedTime);
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

}  // namespace detail

CAnimationStreamingManager* CAnimationStreamingManager::Instance = nullptr;

// 0x60b3cc _ZN6glitch7collada26CAnimationStreamingManagerC2Ev
// 0x60b410 _ZN6glitch7collada26CAnimationStreamingManagerC1Ev
CAnimationStreamingManager::CAnimationStreamingManager() {
    for (std::uint32_t& word : words) {
        word = 0;
    }
    Instance = this;
}

// 0x6e21c0 _ZN6glitch7collada35CAnimationSetTransformationTemplateC2EPKc
// 0x6e21f4 _ZN6glitch7collada35CAnimationSetTransformationTemplateC1EPKc
CAnimationSetTransformationTemplate::CAnimationSetTransformationTemplate(const char* /*name*/) {}

}  // namespace glitch::collada
