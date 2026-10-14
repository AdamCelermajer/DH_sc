// Bodies of glitch::collada::CAnimationSet, IAnimationSetTemplate, CAnimationSetTransformationTemplate and CDynamicAnimationSet
// (canonical classes: collada/CAnimationSet.hpp, collada/CDynamicAnimationSet.hpp). This file replaces
//   src/animation_set.cpp, src/tb_scene-collada-gui-glitch_collada_CAnimationSet-0.cpp (accessors over a stand-in struct),
//   src/tb_scene-collada-gui-glitch_collada_IAnimationSetTemplate-0.cpp,
//   src/tb_scene-collada-gui-glitch_collada_CAnimationSetTransformationTempla-0.cpp and the CAnimationSet / IAnimationSetTemplate /
//   CAnimationSetTransformationTemplate / CDynamicAnimationSet stubs of recon/slice_22.cpp, recon/slice_28.cpp and
//   collada_scene_accessors.cpp.
//
// The comment before each function is `// 0xADDR symbol`. A function whose comment contains PARTIAL is ledgered partial.
#include "glitch-scene-collada-gui/collada/CAnimationSet.hpp"
#include "glitch-scene-collada-gui/collada/CDynamicAnimationSet.hpp"

#include <cstdio>
#include <cstring>

#include "glitch-scene-collada-gui/collada/IAnimationTrackEx.hpp"
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_detail.hpp"

namespace glitch::collada {

namespace {

// The channel match of addAnimation: a compatible channel (CompatibilityTable row = the existing channel's type, bit = the new
// channel's type) of the same name; for type 14 the flag byte must match, for type 86 the string at +0x0c, otherwise the match
// is complete. Returns the index of the match or -1.
template <typename ChannelAt>
int findMatchingChannel(int count, const SChannel& channel, ChannelAt channelAt) {
    for (int i = 0; i < count; ++i) {
        const SChannel& existing = channelAt(i);
        if (!detail::channelTypeCompatible(existing.type, channel.type)) {
            continue;
        }
        if (std::strcmp(existing.name, channel.name) != 0) {
            continue;
        }
        if (channel.type == 14) {
            if (existing.flag == channel.flag) {
                return i;
            }
        } else if (channel.type != 86 || std::strcmp(existing.extra, channel.extra) == 0) {
            return i;
        }
    }
    return -1;
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------------
// IAnimationSetTemplate
// ---------------------------------------------------------------------------------------------------------------------

// 0x667588 _ZN6glitch7collada21IAnimationSetTemplateD1Ev, 0x66762c (D2); D0 0x667610 is skipped.
// Frees the record of every target (the targets themselves are not deleted by the original).
IAnimationSetTemplate::~IAnimationSetTemplate() {
    for (STemplateTarget* target : targets) {
        delete target->record;
        target->record = nullptr;
    }
}

// 0x667320 _ZN6glitch7collada21IAnimationSetTemplate10setUnAddedEv
// For each target: clear the added flag (+0x00) and the record pointer (+0x0c). Returns this.
IAnimationSetTemplate& IAnimationSetTemplate::setUnAdded() {
    for (STemplateTarget* target : targets) {
        target->added = 0;
        target->record = nullptr;
    }
    return *this;
}

// 0x667384 _ZN6glitch7collada21IAnimationSetTemplate11addChannelsEPSt6vector...
// PARTIAL. Reconstructed: the walk over the targets, the skip of targets already added, the record allocation, the kind copy, the
// record link and the added flag. Missing: the record's value04 (the original calls virtual slot +0x54 of the target's node and
// stores the result: the collada::CSceneNode stand-in has no virtuals), and, for the kinds 0x01 .. 0x0d, the
// CVirtualEx<...>::getInstance() calls of the matching track instantiation (their result is discarded; they only set up
// function-local statics). The two vector arguments are never read by the pseudocode.
void IAnimationSetTemplate::addChannels(std::vector<const SChannel*>*, std::vector<const CAnimationTrackEx*>*) {
    for (STemplateTarget* target : targets) {
        if (target->added) {
            continue;
        }
        STemplateRecord* record = new STemplateRecord();  // +0x00..+0x0f
        record->kind08 = target->kind;                    // record +0x08 = target +0x04
        target->record = record;                          // target +0x0c
        target->added = 1;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// CAnimationSetTransformationTemplate
// ---------------------------------------------------------------------------------------------------------------------

namespace {

// Allocates one 16-byte target record (the original uses operator new(0x10) and writes the flag byte, the kind word and the node).
STemplateTarget* newTarget(std::uint32_t kind, CSceneNode* node) {
    auto* target = new STemplateTarget;
    target->added = 0;
    target->kind = kind;
    target->node = node;
    return target;
}

}  // namespace

// 0x6e21c0 _ZN6glitch7collada35CAnimationSetTransformationTemplateC2EPKc, 0x6e21f4 (C1)
// The name argument is not stored.
CAnimationSetTransformationTemplate::CAnimationSetTransformationTemplate(const char*) {}

// 0x6e264c _ZN6glitch7collada35CAnimationSetTransformationTemplateC2EPNS0_10CSceneNodeE, 0x6e2690 (C1)
CAnimationSetTransformationTemplate::CAnimationSetTransformationTemplate(CSceneNode* node) {
    addTransformationTargets(node);
}

// 0x6e2830 _ZN6glitch7collada35CAnimationSetTransformationTemplateC2EPNS0_16CColladaDatabaseE, 0x6e28f0 (C1)
// Targets for every root node of every visual scene of the database.
CAnimationSetTransformationTemplate::CAnimationSetTransformationTemplate(CColladaDatabase* database) {
    const SDatabaseTables& tables = database->tables();
    for (std::size_t i = 0; i < tables.visualScenes.size(); ++i) {
        SVisualScene* scene = visualSceneAt(tables, static_cast<int>(i));
        for (std::uint32_t k = 0; k < scene->nodeCount; ++k) {
            addTransformationTargets(scene->nodes[k]);
        }
    }
}

// 0x6e2248 _ZN6glitch7collada35CAnimationSetTransformationTemplateD1Ev, 0x6e2298 (D2); D0 0x6e227c is skipped.
CAnimationSetTransformationTemplate::~CAnimationSetTransformationTemplate() = default;

// 0x6e22cc _ZN6glitch7collada35CAnimationSetTransformationTemplate16isAnimationExistEPKNS0_8SChannelE
// A target matches when its node's UID (vptr+0x54) equals the channel name and the channel type belongs to the target kind: types
// 10..13 match kind 10, types 5..9 kind 5, types 1..4 kind 1 (types above 13 match nothing). A match sets the target's added flag and
// returns 1.
int CAnimationSetTransformationTemplate::isAnimationExist(const SChannel* channel) {
    for (STemplateTarget* target : targets) {
        if (std::strcmp(channel->name, target->node->getUID()) != 0) {
            continue;
        }
        const std::uint32_t type = channel->type;
        if (type > 0xD) {
            continue;
        }
        const std::uint32_t bit = 1u << type;
        std::uint32_t accepted = 0;
        if ((bit & 0x3C00u) != 0) {
            accepted = 10;
        } else if ((bit & 0x3E0u) != 0) {
            accepted = 5;
        } else if ((bit & 0x1Eu) != 0) {
            accepted = 1;
        } else {
            continue;
        }
        if (target->kind == accepted) {
            target->added = 1;
            return 1;
        }
    }
    return 0;
}

// 0x6e23d0 _ZNK6glitch7collada35CAnimationSetTransformationTemplate15getDefaultValueEPKNS0_8SChannelEPPv
// PARTIAL: each target's node is asked for a default value in turn (CColladaDatabase::getDefaultValue on the database embedded at
// node +0x14c) and the first success returns 1; the collada::CSceneNode stand-in has no database member, so no default is found.
int CAnimationSetTransformationTemplate::getDefaultValue(const SChannel*, void**) const {
    return 0;
}

// 0x6e24e8 _ZN6glitch7collada35CAnimationSetTransformationTemplate24addTransformationTargetsEPNS0_10CSceneNodeE
// Pushes three targets for this node, of kinds 1, 5 and 10, in that order. PARTIAL: the original then walks the node's intrusive
// child list (sentinel at node +0xf4; each child pointer is the link word minus 4) and calls this overload for every child; the
// collada::CSceneNode stand-in has no child list, so the children are not visited.
void CAnimationSetTransformationTemplate::addTransformationTargets(CSceneNode* node) {
    targets.push_back(newTarget(1, node));
    targets.push_back(newTarget(5, node));
    targets.push_back(newTarget(10, node));
}

// 0x6e26d4 _ZN6glitch7collada35CAnimationSetTransformationTemplate24addTransformationTargetsERNS0_5SNodeE
// Pushes three targets of kinds 1, 5 and 10 (the original never stores a node word for these entries: operator new does not zero
// it, so the node is null here), then recurses over the children (count +0x38, base +0x3c, stride 0x50).
void CAnimationSetTransformationTemplate::addTransformationTargets(SNode& node) {
    targets.push_back(newTarget(1, nullptr));
    targets.push_back(newTarget(5, nullptr));
    targets.push_back(newTarget(10, nullptr));
    for (std::uint32_t i = 0; i < node.childCount; ++i) {
        addTransformationTargets(node.children[i]);
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// CAnimationSet
// ---------------------------------------------------------------------------------------------------------------------

// 0x65f2ac _ZN6glitch7collada13CAnimationSetD1Ev; D0 0x65f348 is skipped.
// Releases the template, frees the vectors and drops the database handles (the std::vector members do the freeing).
CAnimationSet::~CAnimationSet() {
    remAnimationTemplate();
    for (CColladaDatabase& database : databases) {
        dropDatabase(database);
    }
}

// 0x6601fc _ZN6glitch7collada13CAnimationSet12addAnimationEPKNS0_10SAnimationE
// PARTIAL: the original asks CColladaDatabase::getAnimationTrackEx(animation) (0x611ae0, which picks the track class from the
// channel type: not reconstructed) for the track; the track the loader stored at SAnimation +0x14 is used here.
int CAnimationSet::addAnimation(const SAnimation* animation) {
    const SChannel& channel = *animation->channels;
    const int match = findMatchingChannel(static_cast<int>(channels.size()), channel,
                                          [this](int i) -> const SChannel& { return *channels[i]; });
    if (match >= 0) {
        return match;
    }
    const auto* track = static_cast<const CAnimationTrackEx*>(animation->trackEx);
    if (track == nullptr) {
        return -1;
    }
    channels.push_back(&channel);
    tracks.push_back(track);
    return static_cast<int>(channels.size()) - 1;
}

// 0x65f004 _ZN6glitch7collada13CAnimationSet12remAnimationEPKNS0_10SAnimationE
int CAnimationSet::remAnimation(const SAnimation*) {
    return 0;
}

// 0x65f00c _ZN6glitch7collada13CAnimationSet10getChannelEj
const SChannel* CAnimationSet::getChannel(unsigned index) {
    return channels[index];
}

// 0x65f018 _ZNK6glitch7collada13CAnimationSet10getChannelEj
const SChannel* CAnimationSet::getChannel(unsigned index) const {
    return channels[index];
}

// 0x62db98 _ZN6glitch7collada13CAnimationSet20setMismatchBehaviorENS1_19E_MISMATCH_BEHAVIORE
CAnimationSet& CAnimationSet::setMismatchBehavior(E_MISMATCH_BEHAVIOR behavior) {
    mismatchBehavior = behavior;
    return *this;
}

// 0x65f024 _ZNK6glitch7collada13CAnimationSet17getAnimationTrackEi
const SChannel* CAnimationSet::getAnimationTrack(int index) const {
    return getChannel(static_cast<unsigned>(index));
}

// 0x65f038 _ZN6glitch7collada13CAnimationSet19getAnimationTrackExEi
const CAnimationTrackEx* CAnimationSet::getAnimationTrackEx(int index) {
    return tracks[index];
}

// 0x6600fc _ZN6glitch7collada13CAnimationSet19addAnimationLibraryEPKc
// PARTIAL: the original loads the file (CResFileManager::load, auto-unload switched off meanwhile), wraps the record in a
// CColladaDatabase over CColladaDatabase::DefaultFactory and calls the database overload (vptr+0x2c). The resource manager is not
// reconstructed, so the url never loads: the error line is printed and the last index is returned, as the original does on failure.
int CAnimationSet::addAnimationLibrary(const char* url) {
    std::printf("ERROR: CAnimationSet::addAnimationLibrary: Unable to add url: %s\n", url);
    return static_cast<int>(databases.size()) - 1;
}

// 0x6601d4 _ZN6glitch7collada13CAnimationSet19addAnimationLibraryERKNS0_16CColladaDatabaseE
// Appends the handle (a copy: the record gets a use) and returns its index.
int CAnimationSet::addAnimationLibrary(const CColladaDatabase& database) {
    CColladaDatabase copy;
    copyDatabase(copy, database);
    databases.push_back(copy);
    return static_cast<int>(databases.size()) - 1;
}

// 0x65f0d0 _ZN6glitch7collada13CAnimationSet19remAnimationLibraryERNS0_16CColladaDatabaseE
void CAnimationSet::remAnimationLibrary(CColladaDatabase&) {}

// 0x65f0d4 _ZN6glitch7collada13CAnimationSet19remAnimationLibraryEi
void CAnimationSet::remAnimationLibrary(int) {}

// 0x660710 _ZN6glitch7collada13CAnimationSet7compileEv
void CAnimationSet::compile() {
    if (animationTemplate != nullptr) {
        animationTemplate->setUnAdded();
    }
    // Collect the animations of every database (those the template does not claim are skipped).
    for (std::size_t d = 0; d < databases.size(); ++d) {
        CColladaDatabase& database = databases[d];
        const int animationCount = static_cast<int>(database.tables().animations.size());
        for (int k = 0; k < animationCount; ++k) {
            SAnimation* animation = animationAt(database.tables(), k);
            if (animationTemplate == nullptr || animationTemplate->isAnimationExist(animation->channels)) {
                addAnimation(animation);  // vptr+0x0c
            }
        }
    }
    if (animationTemplate != nullptr) {
        animationTemplate->addChannels(&channels, &tracks);
    }
    // Prune the channels that no database can drive: no blendable animation, no default value, no template default, and the
    // mismatch behaviour is 0.
    for (std::size_t d = 0; d < databases.size(); ++d) {
        CColladaDatabase& database = databases[d];
        for (std::size_t j = 0; j < channels.size(); ++j) {
            void* defaultValue = nullptr;
            if (database.getBlendableAnimation(channels[j]) == nullptr && !database.getDefaultValue(channels[j], &defaultValue)) {
                if ((animationTemplate == nullptr || !animationTemplate->getDefaultValue(channels[j], &defaultValue)) &&
                    mismatchBehavior == 0) {
                    channels.erase(channels.begin() + static_cast<std::ptrdiff_t>(j));
                    tracks.erase(tracks.begin() + static_cast<std::ptrdiff_t>(j));
                    --j;
                }
            }
        }
    }
    animatedTrackCount = static_cast<std::uint32_t>(channels.size());
    bindings.assign(databases.size() * animatedTrackCount, SBinding());
    std::size_t index = 0;
    for (std::size_t d = 0; d < databases.size(); ++d) {
        CColladaDatabase& database = databases[d];
        for (std::size_t j = 0; j < channels.size(); ++j, ++index) {
            SBinding& binding = bindings[index];
            SAnimation* blendable = database.getBlendableAnimation(channels[j]);
            void* defaultValue = nullptr;
            const bool hasDefault = database.getDefaultValue(channels[j], &defaultValue);
            binding.defaultValue = defaultValue;
            if (blendable != nullptr) {
                binding.kind = 2;
                binding.animation = blendable;
            } else {
                binding.kind = 1;
                if (!hasDefault && animationTemplate != nullptr) {
                    void* templateDefault = nullptr;
                    animationTemplate->getDefaultValue(channels[j], &templateDefault);
                    binding.defaultValue = templateDefault;
                }
            }
        }
    }
    CompileInternal();
}

// 0x65f044 _ZNK6glitch7collada13CAnimationSet17getAnimationCountEv
std::uint32_t CAnimationSet::getAnimationCount() const {
    return static_cast<std::uint32_t>(databases.size());
}

// 0x65f058 _ZNK6glitch7collada13CAnimationSet21getAnimatedTrackCountEv
std::uint32_t CAnimationSet::getAnimatedTrackCount() const {
    return animatedTrackCount;
}

// 0x65f060 _ZNK6glitch7collada13CAnimationSet18getAnimationLengthEj
std::uint32_t CAnimationSet::getAnimationLength(unsigned index) const {
    return index < lengths.size() ? static_cast<std::uint32_t>(lengths[index]) : 0u;
}

// 0x65f07c _ZNK6glitch7collada13CAnimationSet17getAnimationStartEj
std::uint32_t CAnimationSet::getAnimationStart(unsigned index) const {
    return index < starts.size() ? static_cast<std::uint32_t>(starts[index]) : 0u;
}

// 0x65f098 _ZNK6glitch7collada13CAnimationSet15getAnimationEndEj
std::uint32_t CAnimationSet::getAnimationEnd(unsigned index) const {
    return index < ends.size() ? static_cast<std::uint32_t>(ends[index]) : 0u;
}

// 0x65f0b4 _ZN6glitch7collada13CAnimationSet11getDatabaseEi
CColladaDatabase* CAnimationSet::getDatabase(int index) {
    return &databases[index];
}

// 0x65f0c0 _ZNK6glitch7collada13CAnimationSet10getBindingEi
const CAnimationSet::SBinding* CAnimationSet::getBinding(int index) const {
    return &bindings[index];
}

// 0x65f0d8 _ZN6glitch7collada13CAnimationSet17remAnimationTemplateEv
void CAnimationSet::remAnimationTemplate() {
    delete animationTemplate;
    animationTemplate = nullptr;
}

// 0x6600a4 _ZN6glitch7collada13CAnimationSet20addAnimationTemplateEPNS0_16CColladaDatabaseE
// The original overwrites the template pointer without releasing the old one.
void CAnimationSet::addAnimationTemplate(CColladaDatabase* database) {
    animationTemplate = new CAnimationSetTransformationTemplate(database);
}

// 0x6600d0 _ZN6glitch7collada13CAnimationSet20addAnimationTemplateEPNS0_10CSceneNodeE
void CAnimationSet::addAnimationTemplate(CSceneNode* node) {
    animationTemplate = new CAnimationSetTransformationTemplate(node);
}

// 0x6605ac _ZN6glitch7collada13CAnimationSet15CompileInternalEv
void CAnimationSet::CompileInternal() {
    starts.assign(databases.size(), 0);
    ends.assign(databases.size(), 0);
    lengths.assign(databases.size(), 0);
    for (std::size_t i = 0; i < databases.size(); ++i) {
        const SDatabaseTables& tables = databases[i].tables();
        starts[i] = tables.animationStart;
        ends[i] = tables.animationEnd;
        lengths[i] = ends[i] - starts[i];
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// CDynamicAnimationSet
// ---------------------------------------------------------------------------------------------------------------------

// 0x3648c4 _ZN6glitch7collada20CDynamicAnimationSetC1Ev
// The default factory (CColladaDatabase::DefaultFactory) is not modelled, so the default library keeps a null factory.
CDynamicAnimationSet::CDynamicAnimationSet() = default;

// 0x62e198 _ZN6glitch7collada20CDynamicAnimationSetD1Ev; D0 0x62e1e8 is skipped.
CDynamicAnimationSet::~CDynamicAnimationSet() {
    dropDatabase(defaultLibrary);
}

// 0x62dba0 _ZN6glitch7collada20CDynamicAnimationSet10getChannelEj
const SChannel* CDynamicAnimationSet::getChannel(unsigned index) {
    return &ownedChannels[index];
}

// 0x62dbac _ZNK6glitch7collada20CDynamicAnimationSet10getChannelEj
const SChannel* CDynamicAnimationSet::getChannel(unsigned index) const {
    return &ownedChannels[index];
}

// 0x62e8a8 _ZN6glitch7collada20CDynamicAnimationSet19addAnimationLibraryERKNS0_16CColladaDatabaseE
int CDynamicAnimationSet::addAnimationLibrary(const CColladaDatabase& database) {
    const int result = CAnimationSet::addAnimationLibrary(database);
    dirty = true;
    return result;
}

// 0x62dbb8 _ZN6glitch7collada20CDynamicAnimationSet16getDatabaseIndexERNS0_16CColladaDatabaseE
int CDynamicAnimationSet::getDatabaseIndex(CColladaDatabase& database) {
    for (std::size_t i = 0; i < databases.size(); ++i) {
        if (databases[i].record == database.record) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// 0x62fc90 _ZN6glitch7collada20CDynamicAnimationSet26setDefaultAnimationLibraryERKNS0_16CColladaDatabaseE
// The argument's record takes a use, then replaces the default library; the handle it displaces loses its own.
// The original copies the new pair over +0x68/+0x6c and then runs ~CColladaDatabase on the saved old pair,
// which is exactly copyDatabase() then dropDatabase() here.
void CDynamicAnimationSet::setDefaultAnimationLibrary(const CColladaDatabase& database) {
    CColladaDatabase previous;
    previous.record = defaultLibrary.record;
    previous.factory = defaultLibrary.factory;
    copyDatabase(defaultLibrary, database);
    dropDatabase(previous);
    dirty = true;
}

// 0x62fcf8 _ZN6glitch7collada20CDynamicAnimationSet26setDefaultAnimationLibraryEj
// An index past the end of the database list resets the default to the empty factory handle, releasing what it
// held (the original builds a default CColladaDatabase and runs ~CColladaDatabase on it, then copies the pair
// into +0x68/+0x6c). An index inside the list copies that database in, which takes its own use.
void CDynamicAnimationSet::setDefaultAnimationLibrary(unsigned index) {
    if (index >= databases.size()) {  // 0x62fd00..0x62fd20: (end - begin) >> 3
        CColladaDatabase previous;
        previous.record = defaultLibrary.record;
        previous.factory = defaultLibrary.factory;
        defaultLibrary.record = nullptr;                       // the original's outgoing default
        defaultLibrary.factory = &CColladaDatabase::DefaultFactory;
        dropDatabase(previous);                                // ~CColladaDatabase on the old handle
    }
    if (index < databases.size()) {
        copyDatabase(defaultLibrary, databases[index]);
    }
    dirty = true;
}

// 0x62fc58 _ZN6glitch7collada20CDynamicAnimationSet8clearSetEv
void CDynamicAnimationSet::clearSet() {
    for (CColladaDatabase& database : databases) {
        dropDatabase(database);
    }
    databases.clear();
    dirty = true;
}

// 0x62f54c _ZN6glitch7collada20CDynamicAnimationSet11clearTracksEv
void CDynamicAnimationSet::clearTracks() {
    ownedChannels.clear();
    tracks.clear();
    bindings.clear();
    dirty = true;
    animatedTrackCount = 0;
}

// 0x62f354 _ZN6glitch7collada20CDynamicAnimationSet12addAnimationEPKNS0_10SAnimationE
// As CAnimationSet::addAnimation, over the owned channels (copied by value). A negative channel type gives -1.
// PARTIAL: the track is the one the loader stored at SAnimation +0x14 (see CAnimationSet::addAnimation).
int CDynamicAnimationSet::addAnimation(const SAnimation* animation) {
    const SChannel& channel = *animation->channels;
    if (!ownedChannels.empty() && static_cast<std::int32_t>(channel.type) < 0) {
        return -1;
    }
    const int match = findMatchingChannel(static_cast<int>(ownedChannels.size()), channel,
                                          [this](int i) -> const SChannel& { return ownedChannels[i]; });
    if (match >= 0) {
        return match;
    }
    const auto* track = static_cast<const CAnimationTrackEx*>(animation->trackEx);
    if (track == nullptr) {
        return -1;
    }
    ownedChannels.push_back(channel);
    tracks.push_back(track);
    return static_cast<int>(ownedChannels.size()) - 1;
}

}  // namespace glitch::collada
