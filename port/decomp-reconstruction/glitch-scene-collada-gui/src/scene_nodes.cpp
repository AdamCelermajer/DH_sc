#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x599798 _ZN6glitch5scene18ISceneNodeAnimator10updateTimeEj
void ISceneNodeAnimator::updateTime(std::uint32_t /*time*/) {}

// 0x59979c _ZNK6glitch5scene18ISceneNodeAnimator22isEventReceiverEnabledEv
bool ISceneNodeAnimator::isEventReceiverEnabled() const {
    return false;
}

// 0x5997ac _ZN6glitch5scene18ISceneNodeAnimator7onEventERKNS_6SEventE
bool ISceneNodeAnimator::onEvent(const glitch::SEvent& /*event*/) {
    return false;
}

// 0x5997b4 _ZNK6glitch5scene18ISceneNodeAnimator7getTypeEv
std::uint32_t ISceneNodeAnimator::getType() const {
    return 10;
}

// 0x5997bc _ZN6glitch5scene18ISceneNodeAnimator6onBindEPNS0_10ISceneNodeE
void ISceneNodeAnimator::onBind(ISceneNode* /*node*/) {}

// 0x5997c0 _ZN6glitch5scene18ISceneNodeAnimator8onUnbindEPNS0_10ISceneNodeE
void ISceneNodeAnimator::onUnbind(ISceneNode* /*node*/) {}

// 0x599878 _ZNK6glitch5scene18ISceneNodeAnimator9getLengthEv
std::uint32_t ISceneNodeAnimator::getLength() const {
    return 0;
}

// 0x599880 _ZN6glitch5scene18ISceneNodeAnimator22computeAnimationValuesEj
void ISceneNodeAnimator::computeAnimationValues(std::uint32_t /*time*/) {}

// 0x599884 _ZN6glitch5scene18ISceneNodeAnimator20applyAnimationValuesEj
void ISceneNodeAnimator::applyAnimationValues(std::uint32_t /*time*/) {}

// 0x35bea4 _ZNK6glitch5scene15CEmptySceneNode7getTypeEv
std::uint32_t CEmptySceneNode::getType() const {
    return 0x79746d65;  // "emty"
}

// 0x5839ac _ZN6glitch5scene15CEmptySceneNode6renderEPv
void CEmptySceneNode::render(void* /*unused*/) {}

// 0x35beb0 _ZNK6glitch5scene16ICameraSceneNode13getTargetNodeEv
ISceneNode* ICameraSceneNode::getTargetNode() const {
    return nullptr;
}

// 0x35beb8 _ZN6glitch5scene16ICameraSceneNode13getTargetNodeEv
ISceneNode* ICameraSceneNode::getTargetNode() {
    return nullptr;
}

// 0x581f34 _ZNK6glitch5scene16CCameraSceneNode7getTypeEv
std::uint32_t CCameraSceneNode::getType() const {
    return 0x5f6d6163;  // "cam_"
}

// 0x581f40 _ZN6glitch5scene16CCameraSceneNode23setInputReceiverEnabledEb
void CCameraSceneNode::setInputReceiverEnabled(bool enabled) {
    inputReceiverEnabled = enabled;
}

// 0x581f48 _ZNK6glitch5scene16CCameraSceneNode22isInputReceiverEnabledEv
bool CCameraSceneNode::isInputReceiverEnabled() const {
    return inputReceiverEnabled;
}

// 0x582130 _ZNK6glitch5scene16CCameraSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CCameraSceneNode::getBoundingBox() const {
    return boundingBox;
}

// 0x583bec _ZNK6glitch5scene15CLightSceneNode7getTypeEv
std::uint32_t CLightSceneNode::getType() const {
    return 0x7468676c;  // "lght"
}

// 0x584a98 _ZNK6glitch5scene14CMeshSceneNode7getTypeEv
std::uint32_t CMeshSceneNode::getType() const {
    return 0x6873656d;  // "mesh"
}

// 0x588fdc _ZNK6glitch5scene21CSceneManagerRootNode7getTypeEv
std::uint32_t CSceneManagerRootNode::getType() const {
    return 0x72676d73;  // "smgr"
}

// 0x5890a8 _ZN6glitch5scene13CSceneManager23notifyVisibilityChangedEv
void CSceneManager::notifyVisibilityChanged() {
    visibilityChanged = true;
}

// 0x5890b4 _ZN6glitch5scene13CSceneManager22notifyHierarchyChangedEv
void CSceneManager::notifyHierarchyChanged() {
    hierarchyChanged = true;
    notifyVisibilityChanged();
}

// 0x589558 _ZN6glitch5scene13CSceneManager16createMeshWriterENS0_17EMESH_WRITER_TYPEE
IMeshWriter* CSceneManager::createMeshWriter(std::uint32_t /*writerType*/) {
    return nullptr;
}

// 0x580a2c _ZNK6glitch5scene19CBillboardSceneNode7getTypeEv
std::uint32_t CBillboardSceneNode::getType() const {
    return 0x6c6c6962;  // "bill"
}

// 0x580a98 _ZNK6glitch5scene19CBillboardSceneNode14getBoundingBoxEv
const core::aabbox3d<float>& CBillboardSceneNode::getBoundingBox() const {
    return boundingBox;
}

// 0x580b34 _ZNK6glitch5scene19CBillboardSceneNode16getMaterialCountEv
std::uint32_t CBillboardSceneNode::getMaterialCount() const {
    return 1;
}

// 0x580b3c _ZNK6glitch5scene19CBillboardSceneNode7getSizeEv
const core::dimension2d<float>& CBillboardSceneNode::getSize() const {
    return size;
}

// 0x50cd24 _ZN6glitch5scene10CBatchMesh4initEv
void CBatchMesh::init() {}

// 0x50cd28 _ZNK6glitch5scene10CBatchMesh20saveSegmentExtraDataEPKvPNS_2io10IWriteFileEb
void CBatchMesh::saveSegmentExtraData(const void* /*data*/, glitch::io::IWriteFile* /*file*/,
                                      bool /*flag*/) const {}

// 0x50cd10 _ZNK6glitch5scene10CBatchMesh13getIndexRangeERjS2_PKNS1_8SSegmentE
void CBatchMesh::getIndexRange(std::uint32_t& first, std::uint32_t& second,
                               const SSegment* segment) const {
    first = segment->indexBegin;
    second = segment->indexEnd;
}

// 0x50cd2c _ZNK6glitch5scene15CBatchSceneNode7getTypeEv
std::uint32_t CBatchSceneNode::getType() const {
    return 0x6e736162;  // "basn"
}

// 0x50cd38 _ZN6glitch5scene15CBatchSceneNode20preRegisterSceneNodeEv
void CBatchSceneNode::preRegisterSceneNode() {}

// 0x50cd3c _ZN6glitch5scene15CBatchSceneNode16isSegmentVisibleERKNS0_10CBatchMesh8SSegmentE
bool CBatchSceneNode::isSegmentVisible(const CBatchMesh::SSegment& segment) {
    return segment.visible != 0;
}

// 0x596c6c _ZNK6glitch5scene5IMesh15getUserPropertyEv
const void* IMesh::getUserProperty() const {
    return nullptr;
}

// 0x666be4 _ZN6glitch5scene15ITimeController6updateEi
void ITimeController::update(int /*time*/) {}

// 0x666bfc _ZN6glitch5scene19ITimelineController6jumpToEi
void ITimelineController::jumpTo(int frame) {
    currentFrame = frame;
}

// 0x666c04 _ZNK6glitch5scene19ITimelineController7getLoopEv
bool ITimelineController::getLoop() const {
    return false;
}

// 0x666c0c _ZN6glitch5scene19ITimelineController8setScaleEf
void ITimelineController::setScale(float /*scale*/) {}

// 0x666c10 _ZNK6glitch5scene19ITimelineController8getScaleEv
float ITimelineController::getScale() const {
    return 1.0f;
}

}  // namespace glitch::scene
