// Slice 06: glitch::scene scene-graph bookkeeping (CSceneManager, ISceneNode,
// CTriangleSelector, SAnimatedMesh, SBatchConfig). See the header for the stand-in
// layouts. Bodies marked "partial" compile and do the part whose inputs are mapped;
// the omitted part is named in the comment above the function.

#include "glitch-scene-collada-gui/recon/slice_06.hpp"

#include <algorithm>
#include <cctype>

namespace glitch::collada::recon_06 {

namespace {

// strcasecmp semantics: true when the two strings are equal ignoring ASCII case.
bool equalsIgnoreCase(const std::string& a, const char* b) {
    std::size_t i = 0;
    for (; i < a.size() && b[i] != '\0'; ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return i == a.size() && b[i] == '\0';
}

// Shared shape of the three registries: grab the new entry, then append it.
template <typename Entry>
void appendGrabbed(std::vector<Entry*>& list, Entry* entry) {
    if (entry == nullptr) {
        return;
    }
    grab(entry);
    list.push_back(entry);
}

// Shared body of getSceneNodeFromName, getSceneNodeFromScopeID and getSceneNodeFromUID.
// The node's own key is compared first; on a mismatch the children are searched in
// order and the first hit is returned.
template <typename NodeT>
NodeT* findInTreeByKey(NodeT& self, const std::string SceneNode::*key, const char* wanted) {
    if (equalsIgnoreCase(self.*key, wanted)) {
        return &self;
    }
    for (SceneNode* child : self.children) {
        if (child == nullptr) {
            continue;
        }
        if (NodeT* found = findInTreeByKey(*child, key, wanted)) {
            return found;
        }
    }
    return nullptr;
}

}  // namespace

void grab(RefCounted* object) {
    if (object != nullptr) {
        ++object->referenceCount;
    }
}

void drop(RefCounted* object) {
    if (object != nullptr && --object->referenceCount == 0) {
        delete object;
    }
}

void grabMaterial(Material* material) {
    if (material != nullptr) {
        ++material->referenceCount;
    }
}

void dropMaterial(Material* material) {
    if (material != nullptr && --material->referenceCount == 0) {
        delete material;
    }
}

// ---------------------------------------------------------------- SBatchConfig

//@ 0x58f2a8
SBatchConfig& SBatchConfig::operator=(const SBatchConfig& other) {
    // Material reference: take the new one first, then release the old one.
    grabMaterial(other.material);
    Material* previous = material;
    material = other.material;
    dropMaterial(previous);

    for (int i = 0; i < 4; ++i) {
        words1[i] = other.words1[i];
        words2[i] = other.words2[i];
    }

    grab(other.extra);
    RefCounted* previousExtra = extra;
    extra = other.extra;
    drop(previousExtra);
    return *this;
}

// ----------------------------------------------------- STransparentNodeEntry

//@ 0x58d710
STransparentNodeEntry::~STransparentNodeEntry() {
    dropMaterial(material);
}

// ---------------------------------------------------------- CSceneManager

//@ 0x58a7f0
void addShadowReceiverTarget(SceneManager& self, RefCounted* target) {
    auto& targets = self.shadowReceiverTargets;
    if (std::find(targets.begin(), targets.end(), target) != targets.end()) {
        return;
    }
    grab(target);
    targets.push_back(target);
}

//@ 0x58ae20
void registerSceneNodeFactory(SceneManager& self, RefCounted* factory) {
    appendGrabbed(self.sceneNodeFactories, factory);
}

//@ 0x58d758
void registerSceneNodeAnimatorFactory(SceneManager& self, RefCounted* factory) {
    appendGrabbed(self.sceneNodeAnimatorFactories, factory);
}

//@ 0x58d8ac
void addExternalMeshLoader(SceneManager& self, RefCounted* loader) {
    appendGrabbed(self.externalMeshLoaders, loader);
}

//@ 0x58b350
//@ partial: the original calls a virtual slot (+0x68) on each queued node before the
//@ release; that slot is not mapped, so only the release and the queue reset are done.
void clearDeletionList(SceneManager& self) {
    for (SceneNode* node : self.deletionQueue) {
        drop(node);
    }
    self.deletionQueue.clear();
}

//@ 0x58b4e0
void addToDeletionQueue(SceneManager& self, SceneNode* node) {
    if (node == nullptr) {
        return;
    }
    grab(node);
    self.deletionQueue.push_back(node);
}

//@ 0x58b558
void collectAllNodes(SceneManager& self, SceneNode* node) {
    if (node == nullptr) {
        return;
    }
    // Empty, dummy, daen and scene-manager-root nodes are not render candidates.
    const std::uint32_t type = node->type;
    if (type != kNodeTypeEmpty && type != kNodeTypeDummy && type != kNodeTypeDaen &&
        type != kNodeTypeSceneManagerRoot) {
        self.renderNodes.push_back(node);
    }
    if (!node->animators.empty()) {
        self.animatedNodes.push_back(node);
    }
    for (SceneNode* child : node->children) {
        collectAllNodes(self, child);
    }
}

//@ 0x58b7ac
void collectAllNodes(SceneManager& self) {
    if (!self.collectPending) {
        return;
    }
    // The original resets both lists by moving their end pointer back to the begin.
    self.renderNodes.clear();
    self.animatedNodes.clear();
    collectAllNodes(self, self.root);
    self.collectPending = false;
}

//@ 0x58b9f0
//@ partial: Timer::getTime (reset path) and the per-animator and root update virtual
//@ calls are not mapped. The clock and the collection step are done.
void update(SceneManager& self, float elapsed, bool updateAnimators) {
    if (elapsed != kUpdateResetTime) {
        self.time += elapsed;
    }
    collectAllNodes(self);
    (void)updateAnimators;
}

//@ 0x58c3cc
void getSceneNodesFromType(SceneManager& self, std::uint32_t type, std::vector<SceneNode*>& out,
                           SceneNode* start) {
    SceneNode* node = start != nullptr ? start : self.root;
    if (node->type == type || type == kNodeTypeAny) {
        out.push_back(node);
    }
}

//@ 0x598dd4
void getSceneNodesFromType(SceneNode& self, std::uint32_t type, std::vector<SceneNode*>& out) {
    if (self.type == type || type == kNodeTypeAny) {
        out.push_back(&self);
    }
    for (SceneNode* child : self.children) {
        if (child != nullptr) {
            getSceneNodesFromType(*child, type, out);
        }
    }
}

// ----------------------------------------------------------- ISceneNode queries

//@ 0x596f6c
SceneNode* getSceneNodeFromType(SceneNode& self, std::uint32_t type) {
    if (self.type == type || type == kNodeTypeAny) {
        return &self;
    }
    for (SceneNode* child : self.children) {
        if (child == nullptr) {
            continue;
        }
        if (SceneNode* found = getSceneNodeFromType(*child, type)) {
            return found;
        }
    }
    return nullptr;
}

//@ 0x598398
const SceneNode* getSceneNodeFromScopeID(const SceneNode& self, const char* scope) {
    return findInTreeByKey(self, &SceneNode::scopeId, scope);
}

//@ 0x59840c
SceneNode* getSceneNodeFromScopeID(SceneNode& self, const char* scope) {
    return findInTreeByKey(self, &SceneNode::scopeId, scope);
}

//@ 0x598480
const SceneNode* getSceneNodeFromName(const SceneNode& self, const char* name) {
    return findInTreeByKey(self, &SceneNode::name, name);
}

//@ 0x5984f4
SceneNode* getSceneNodeFromName(SceneNode& self, const char* name) {
    return findInTreeByKey(self, &SceneNode::name, name);
}

//@ 0x598568
const SceneNode* getSceneNodeFromUID(const SceneNode& self, const char* uid) {
    return findInTreeByKey(self, &SceneNode::uid, uid);
}

//@ 0x5985e4
SceneNode* getSceneNodeFromUID(SceneNode& self, const char* uid) {
    return findInTreeByKey(self, &SceneNode::uid, uid);
}

// ----------------------------------------------------------- ISceneNode state

//@ 0x59725c
void setTriangleSelector(SceneNode& self, TriangleSelector* selector) {
    RefCounted* previous = self.triangleSelector;
    drop(previous);
    self.triangleSelector = selector;
    grab(selector);
}

//@ 0x597f50
void setName(SceneNode& self, const std::string& name) {
    self.name = name;
}

//@ 0x598a04
void setName(SceneNode& self, const char* name) {
    self.name = name;
}

//@ 0x597d58
void addBindedAnimator(SceneNode& self, SceneNodeAnimator* animator) {
    if (animator == nullptr) {
        return;
    }
    for (SceneNodeAnimator* existing : self.bindedAnimators) {
        if (existing == animator) {
            return;
        }
    }
    grab(animator);
    self.bindedAnimators.push_back(animator);
}

// ---------------------------------------------------------- SAnimatedMesh

//@ 0x599f18
RefCounted* getMesh(const SAnimatedMesh& self, int index) {
    if (self.meshes.empty()) {
        return nullptr;
    }
    RefCounted* mesh = self.meshes[static_cast<std::size_t>(index)];
    grab(mesh);
    return mesh;
}

}  // namespace glitch::collada::recon_06
