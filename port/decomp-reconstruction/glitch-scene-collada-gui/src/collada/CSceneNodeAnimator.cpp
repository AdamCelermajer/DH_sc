// Bodies of glitch::collada::CSceneNodeAnimator (canonical class: collada/CSceneNodeAnimator.hpp). This file replaces
//   src/scene_node_animator.cpp (four accessors over a vector of target records without a base class) and
//   src/tb_scene-collada-gui-glitch_collada_CSceneNodeAnimator-0.cpp (setTarget, computeAnimationValues, applyAnimationValues,
//   all written as no-ops because the base class and the target record were not mapped).
//
// The comment before each function is `// 0xADDR symbol`. A function whose comment contains PARTIAL is ledgered partial.
#include "glitch-scene-collada-gui/collada/CSceneNodeAnimator.hpp"

#include <algorithm>

#include "glitch-scene-collada-gui/collada/CTimelineController.hpp"
#include "glitch-scene-collada-gui/collada/IAnimationTrackEx.hpp"

namespace glitch::collada {

// 0x65dbd8 _ZN6glitch7collada18CSceneNodeAnimatorC1ERKNS0_16CColladaDatabaseERNS0_21SLibraryAnimationClipsE and 0x65dda0 (C2)
// Copies the database handle (a use on the record), reads the streaming flag and the start / end of the animation from the
// database's table holder, creates the timeline controller over the clip library (clip 0 when the library has clips, the whole
// range otherwise) and installs it.
CSceneNodeAnimator::CSceneNodeAnimator(const CColladaDatabase& source, SLibraryAnimationClips& clipLibrary) {
    copyDatabase(database, source);       // +0x28 / +0x2c
    const SDatabaseTables& tables = *source.record->data->tables;
    clips = &clipLibrary;                 // +0x40
    animationFlag = tables.animationFlag != 0;  // +0x34

    boost::intrusive_ptr<CTimelineController> controller(new CTimelineController());
    if (clipLibrary.count != 0) {
        controller->clipTable = clips;    // controller +0x34
        if (clips != nullptr && clips->count != 0) {
            controller->setClip(0);
        } else {
            controller->rangeStartMs = 0;  // +0x10
            controller->rangeEndMs = 1;    // +0x14
        }
    } else {
        controller->setRange(tables.animationStart, tables.animationEnd, true);  // vptr+0x50
    }
    startTime = tables.animationStart;    // +0x38
    endTime = tables.animationEnd;        // +0x3c
    length = static_cast<std::uint32_t>(endTime - startTime);  // +0x14
    setTimelineCtrl(controller.get());    // takes a use; the handle drops the construction use
}

// 0x65dab0 _ZN6glitch7collada18CSceneNodeAnimatorD1Ev, 0x65db58 (D2); D0 0x65db3c is skipped.
// PARTIAL: removes the animation tracks (their applicators), drops the animation block (the streaming manager's CAnimationBlock
// is not reconstructed, so the pointer is never set), frees the target vector and drops the database handle (see dropDatabase).
CSceneNodeAnimator::~CSceneNodeAnimator() {
    removeAnimationTracks();
    dropDatabase(database);
}

// 0x65df4c _ZNK6glitch7collada18CSceneNodeAnimator19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// PARTIAL, not reconstructed: the body writes the attributes "BlendOutWeight", "Start", "End", "Length" (floats in seconds, from
// +0x30, +0x38, +0x3c and +0x14), "Interpolation Type" (an enum over getStringsInternal, from +0x0c), "Binded to" (the bound
// node's UID) and "Source" (the database's file name) through the IAttributes interface, which is only forward-declared.
void CSceneNodeAnimator::serializeAttributes(io::IAttributes*, io::SAttributeReadWriteOptions*) const {}

// 0x65d540 _ZN6glitch7collada18CSceneNodeAnimator21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Empty in the original.
void CSceneNodeAnimator::deserializeAttributes(io::IAttributes*, io::SAttributeReadWriteOptions*) {}

// 0x65d528 _ZN6glitch7collada18CSceneNodeAnimator11animateNodeEPNS_5scene10ISceneNodeEj
// Tail-calls applyAnimationValues (vptr+0x50).
void CSceneNodeAnimator::animateNode(scene::ISceneNode*, std::uint32_t timeMs) {
    applyAnimationValues(timeMs);
}

// 0x65d6b4 _ZN6glitch7collada18CSceneNodeAnimator10updateTimeEj
// Steps the clock only when the animator has targets (or the +0x50 word is set).
void CSceneNodeAnimator::updateTime(std::uint32_t timeMs) {
    if (!targets.empty() || unk_50 != 0) {
        ISceneNodeAnimator::updateTime(timeMs);
    }
}

// 0x65d544 _ZN6glitch7collada18CSceneNodeAnimator11createCloneEv
// Returns null.
scene::ISceneNodeAnimator* CSceneNodeAnimator::createClone() {
    return nullptr;
}

// 0x65d3f8 _ZN6glitch7collada18CSceneNodeAnimator17getAnimationTrackEi
// The channel array of the target's animation.
const SChannel* CSceneNodeAnimator::getAnimationTrack(std::int32_t index) {
    return targets[index].animation->channels;
}

// 0x65d3e8 _ZN6glitch7collada18CSceneNodeAnimator19getAnimationTrackExEi
IAnimationTrackEx* CSceneNodeAnimator::getAnimationTrackEx(std::int32_t index) {
    return targets[index].animation->trackEx;
}

// 0x65d408 _ZN6glitch7collada18CSceneNodeAnimator9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
// Stores the target, releases the old applicator (destructor slot vptr+0x04) and takes a clone (vptr+0x08) of the new one.
void CSceneNodeAnimator::setTarget(std::int32_t index, void* target, const animation_track::CApplicatorInfo* applicator) {
    STarget& record = targets[index];
    record.target = target;
    if (record.applicator != nullptr) {
        delete record.applicator;
        record.applicator = nullptr;
    }
    if (applicator != nullptr) {
        record.applicator = applicator->clone();
    }
}

// 0x65d480 _ZN6glitch7collada18CSceneNodeAnimator10getBindURIEi
// The name of the target's channel.
const char* CSceneNodeAnimator::getBindURI(std::int32_t index) {
    return targets[index].animation->channels->name;
}

// 0x65d494 _ZN6glitch7collada18CSceneNodeAnimator14getTargetCountEv
std::int32_t CSceneNodeAnimator::getTargetCount() {
    return static_cast<std::int32_t>(targets.size());
}

// 0x65d4a8 _ZN6glitch7collada18CSceneNodeAnimator13getTargetSizeEi
// The value size of the target's track (getAnimationTrackEx, vptr+0x58; then the track's getValueSize, vptr+0x08).
std::int32_t CSceneNodeAnimator::getTargetSize(std::int32_t index) {
    return static_cast<std::int32_t>(getAnimationTrackEx(index)->getValueSize());
}

// 0x65d4c8 _ZN6glitch7collada18CSceneNodeAnimator14getTargetsSizeEv
std::int32_t CSceneNodeAnimator::getTargetsSize() {
    std::int32_t total = 0;
    const std::int32_t count = static_cast<std::int32_t>(targets.size());
    for (std::int32_t i = 0; i < count; ++i) {
        total += static_cast<std::int32_t>(getAnimationTrackEx(i)->getValueSize());
    }
    return total;
}

// 0x65e0d8 _ZN6glitch7collada18CSceneNodeAnimator17addAnimationTrackEPNS0_10SAnimationE
// Appends a target record for the animation (no target, no applicator).
void CSceneNodeAnimator::addAnimationTrack(SAnimation* animation) {
    STarget record;
    record.animation = animation;
    targets.push_back(record);
}

// 0x65d54c _ZN6glitch7collada18CSceneNodeAnimator20removeAnimationTrackEPNS0_10SAnimationE
// Removes the first record of the animation, releasing its applicator.
void CSceneNodeAnimator::removeAnimationTrack(SAnimation* animation) {
    for (std::size_t i = 0; i < targets.size(); ++i) {
        if (targets[i].animation == animation) {
            if (targets[i].applicator != nullptr) {
                delete targets[i].applicator;
                targets[i].applicator = nullptr;
            }
            targets.erase(targets.begin() + static_cast<std::ptrdiff_t>(i));
            return;
        }
    }
}

// 0x65d628 _ZN6glitch7collada18CSceneNodeAnimator21removeAnimationTracksEv
// Releases the applicator of every record (the records stay).
void CSceneNodeAnimator::removeAnimationTracks() {
    for (STarget& record : targets) {
        if (record.applicator != nullptr) {
            delete record.applicator;
            record.applicator = nullptr;
        }
    }
}

// 0x65d768 _ZN6glitch7collada18CSceneNodeAnimator16getAnimationDataEi
// PARTIAL. Asks the timeline controller for its current clip (vptr+0x38), builds an SAnimationBlockSearchKey from the database, the
// clip and the time, asks CAnimationStreamingManager::getAnimationBlock (0x60c59c) for the block, keeps it at +0x54 and returns the
// output array at block -> holder -> +0x0c. The streaming manager's block registry is not reconstructed (see getAnimationBlock), so
// the held block, and with it the data, stays null.
const SAnimationOutput* CSceneNodeAnimator::getAnimationData(std::int32_t time) {
    scene::ITimelineController* controller = getTimelineCtrl();
    SAnimationBlockSearchKey key;
    copyDatabase(key.database, database);
    key.clip = controller != nullptr ? controller->getCurrentClipIndex() : 0;  // vptr+0x38
    key.time = time;
    if (CAnimationStreamingManager::Instance != nullptr) {  // the original dereferences it unconditionally
        CAnimationStreamingManager::Instance->getAnimationBlock(key, animationBlock);
    }
    dropDatabase(key.database);
    return animationBlock != nullptr ? animationBlock->dataHolder->buffer : nullptr;
}

namespace {

// The time the animators sample at: the controller's current time, or the animation start plus the time modulo the length
// (the original divides unsigned; a zero length is guarded here).
std::int32_t sampleTime(scene::ITimelineController* controller, std::int32_t startTime, std::uint32_t length,
                        std::uint32_t timeMs) {
    if (controller != nullptr) {
        return controller->currentTimeMs;
    }
    return startTime + static_cast<std::int32_t>(length != 0 ? timeMs % length : 0);
}

}  // namespace

// 0x65d80c _ZN6glitch7collada18CSceneNodeAnimator22computeAnimationValuesEj
// PARTIAL: with the animation data missing (see getAnimationData) nothing is read. For every target with a target object the value
// at the sample time is read into it through the accessor (getValue), with the shared key hint when the streaming flag is set.
void CSceneNodeAnimator::computeAnimationValues(std::uint32_t timeMs) {
    if (targets.empty() && unk_50 == 0) {
        return;
    }
    ISceneNodeAnimator::updateTime(timeMs);
    const std::int32_t time = sampleTime(getTimelineCtrl(), startTime, length, timeMs);
    const bool interpolate = interpolationType != 1;
    const SAnimationOutput* data = getAnimationData(time);
    if (data == nullptr) {
        return;  // PARTIAL guard: the original would read through the block data
    }
    SAnimationKeyCache cache;
    cache.enabled = animationFlag ? 1 : 0;
    for (STarget& record : targets) {
        if (record.target != nullptr) {
            SAnimationAccessor accessor(record.animation, data, &cache);
            std::int32_t& hint = animationFlag ? targets[0].keyIndex : record.keyIndex;
            accessor.getValue(time, record.target, hint, interpolate);
        }
    }
}

// 0x65d91c _ZN6glitch7collada18CSceneNodeAnimator20applyAnimationValuesEj
// PARTIAL: as computeAnimationValues, with applyValue (the record's applicator is passed) instead of getValue.
void CSceneNodeAnimator::applyAnimationValues(std::uint32_t timeMs) {
    if (targets.empty() && unk_50 == 0) {
        return;
    }
    ISceneNodeAnimator::updateTime(timeMs);
    const std::int32_t time = sampleTime(getTimelineCtrl(), startTime, length, timeMs);
    const bool interpolate = interpolationType != 1;
    const SAnimationOutput* data = getAnimationData(time);
    if (data == nullptr) {
        return;  // PARTIAL guard: the original would read through the block data
    }
    SAnimationKeyCache cache;
    cache.enabled = animationFlag ? 1 : 0;
    for (std::size_t i = 0; i < targets.size(); ++i) {
        STarget& record = targets[i];
        if (record.target != nullptr) {
            SAnimationAccessor accessor(record.animation, data, &cache);
            std::int32_t& hint = animationFlag ? targets[0].keyIndex : record.keyIndex;
            accessor.applyValue(time, record.target, record.applicator, hint, interpolate);
        }
    }
}

// 0x65da38 _ZN6glitch7collada18CSceneNodeAnimator17getAnimationValueEiiPv
// PARTIAL (the animation data): reads the value of one track at `time` with the key cache switched off.
void CSceneNodeAnimator::getAnimationValue(std::int32_t trackIndex, std::int32_t time, void* out) {
    STarget& record = targets[trackIndex];
    const SAnimationOutput* data = getAnimationData(time);
    if (data == nullptr) {
        return;  // PARTIAL guard: the original would read through the block data
    }
    SAnimationKeyCache cache;
    cache.enabled = 0;
    SAnimationAccessor accessor(record.animation, data, &cache);
    accessor.getValue(time, out, record.keyIndex, interpolationType != 1);
}

}  // namespace glitch::collada
