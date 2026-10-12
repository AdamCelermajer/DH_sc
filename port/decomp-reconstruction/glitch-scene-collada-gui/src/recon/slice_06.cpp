// Slice 06: glitch::scene scene-graph bookkeeping (CSceneManager, ISceneNode,
// CTriangleSelector, SAnimatedMesh, SBatchConfig). See the header for the stand-in
// layouts. Bodies marked "partial" compile and do the part whose inputs are mapped;
// the omitted part is named in the comment above the function.

#include "glitch-scene-collada-gui/recon/slice_06.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>

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
    if (node != nullptr && (node->type == type || type == kNodeTypeAny)) {
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


// ================================================================ partial bodies
// Each body below is partial: the omitted calls are named in the comment above it.

//@ 0x58bdb0
//@ partial: the file-based loader path needs IReadFile and the mesh loader interfaces.
RefCounted* getMeshFromFile(SceneManager& self, const void* readFile, int loaderIndex) {
    (void)self;
    (void)readFile;
    (void)loaderIndex;
    return nullptr;
}

//@ 0x58e824
//@ partial: name-based lookup needs the file system interface; returns null.
RefCounted* getMeshFromName(SceneManager& self, const char* name) {
    (void)self;
    (void)name;
    return nullptr;
}

//@ 0x58cdd4
//@ partial: the XML writer and the per-node attribute output are not mapped.
void writeSceneNode(SceneManager& self, const SceneNode& node, void* xmlWriter, void* userData) {
    (void)self;
    (void)node;
    (void)xmlWriter;
    (void)userData;
}

//@ 0x58d36c
//@ partial: scene serialisation needs the writer interface; nothing is written.
void saveScene(SceneManager& self, void* writeFile, void* userData) {
    (void)self;
    (void)writeFile;
    (void)userData;
}

//@ 0x58e768
//@ partial: scene deserialisation needs the reader interface; nothing is read.
void loadScene(SceneManager& self, const void* readFile, void* userData) {
    (void)self;
    (void)readFile;
    (void)userData;
}

//@ 0x58dd50
//@ partial: the CSceneManager constructor (0x58.. range) is not mapped; the new manager
//@ is default-constructed here and the root copy is not made.
SceneManager* createNewSceneManager(SceneManager& self, bool copyRoot) {
    (void)self;
    (void)copyRoot;
    return new SceneManager();
}

//@ 0x58eb70
//@ 0x58ef1c
//@ partial: the member teardown of CSceneManager (reference releases of the factories,
//@ loaders, receivers and deletion queue) is not reconstructed in this pass.
void releaseSceneManagerMembers(SceneManager& self) {
    (void)self;
}

//@ 0x58c2c4
//@ partial: IBuffer::map and the release of the mapping are not mapped; no bytes are copied.
void copyVertexData(AppendMeshBuffer& self, void* destination) {
    (void)self;
    (void)destination;
}

//@ 0x58c348
//@ partial: IBuffer::map and the release of the mapping are not mapped; no bytes are copied.
void copyIndexData(AppendMeshBuffer& self, void* destination) {
    (void)self;
    (void)destination;
}

//@ 0x58fa04
//@ partial: the batch tree is cleared and each configuration released; the vtable reset of
//@ the base is not modelled.
void destroyEndOfBatchCallback(SDefaultEndOfBatchCallback& self) {
    for (auto& entry : self.configs) {
        dropMaterial(entry.second.material);
        drop(entry.second.extra);
    }
    self.configs.clear();
}

// ------------------------------------------------------- CTriangleSelector

//@ 0x590f70
//@ partial: releases the owned buffer; the vtable reset of the base is not modelled.
void releaseTriangleSelector(TriangleSelector& self) {
    std::free(self.ownedBuffer);
    self.ownedBuffer = nullptr;
}

//@ 0x5919a0
//@ 0x591b24
//@ partial: stores the node and flag; the box, matrix and triangle-table defaults are not
//@ mapped beyond the member initialisers, so only the node and flag are set here.
void constructTriangleSelectorFromBox(TriangleSelector& self, const core::aabbox3d<float>& box,
                                      const SceneNode* node, bool flag) {
    (void)box;
    self.node = node;
    self.flag = flag;
}

//@ 0x591a60
//@ 0x591be4
//@ partial: stores the node and flag; triangle table and transform defaults not mapped.
void constructTriangleSelectorFromNode(TriangleSelector& self, const SceneNode* node, bool flag) {
    self.node = node;
    self.flag = flag;
}

//@ 0x595ec8
//@ partial: the mesh triangle table is not built; stores the node and flag only.
void constructTriangleSelectorFromMesh(TriangleSelector& self, const RefCounted* mesh,
                                       const SceneNode* node, bool flag) {
    (void)mesh;
    self.node = node;
    self.flag = flag;
}

//@ 0x591ca8
//@ partial: TestWithBox is not mapped; reports no triangles.
float getTrianglesInBox(TriangleSelector& self, int maxCount, int& outCount) {
    (void)self;
    (void)maxCount;
    outCount = 0;
    return 0.0f;
}

//@ 0x591cf0
//@ partial: TestWithLine is not mapped; reports no triangles and no hit.
bool getTrianglesOnLine(TriangleSelector& self, int maxCount, int& outCount) {
    (void)self;
    (void)maxCount;
    outCount = 0;
    return false;
}

// ------------------------------------------------------------- ISceneNode

//@ 0x597500
//@ partial: the original forwards to each child through a virtual slot (+0xe4) that is not
//@ mapped; only the flag of this node is left unchanged here.
void setNodeHierarchyAsShadowCaster(SceneNode& self, bool enabled) {
    (void)self;
    (void)enabled;
}

//@ 0x596cc8
//@ partial: sets the shadow-caster bit (0x800) on the mesh node, then forwards to the
//@ hierarchy call above.
void setMeshHierarchyAsShadowCaster(SceneNode& self, bool enabled) {
    if (enabled) {
        self.flags |= 0x800u;
    } else {
        self.flags &= ~0x800u;
    }
    setNodeHierarchyAsShadowCaster(self, enabled);
}

//@ 0x596ce0
//@ partial: the original calls two virtual slots (+0x80 and +0x74) that are not mapped.
void onDelete(SceneNode& self) {
    (void)self;
}

//@ 0x596d10
//@ partial: the animator update loop (virtual slot +0x14 per animator) is not mapped; only
//@ the gate on flags 0x201 is reproduced.
void onUpdateTime(SceneNode& self, std::uint32_t time) {
    if ((self.flags & 0x201u) == 0x201u) {
        (void)time;
    }
}

//@ 0x596e34
//@ partial: the visibility latch and the visible bit are updated; the child notification
//@ (virtual slot +0xec) is not mapped.
void notifyVisibilityChanged(SceneNode& self, bool visibleInParent) {
    const bool wasVisible = (self.flags & 1u) != 0;
    self.visibilityLatched = visibleInParent;
    if (self.visible && visibleInParent) {
        self.flags |= 1u;
    } else {
        self.flags &= ~1u;
    }
    (void)wasVisible;
}

//@ 0x596ec4
//@ partial: the visible bit is updated; the child notification (virtual slot +0xec) is not mapped.
void setVisible(SceneNode& self, bool visible) {
    if (self.visible == visible) {
        return;
    }
    self.visible = visible;
    if (visible && self.visibilityLatched) {
        self.flags |= 1u;
    } else {
        self.flags &= ~1u;
    }
}

//@ 0x59706c
//@ partial: forwards to the parent through virtual slot +0x60, not mapped; no-op here.
void remove(SceneNode& self) {
    (void)self;
}

//@ 0x5971e0
//@ partial: the virtual hook (+0x68) before the change and the scene-manager update are not
//@ mapped; the parent link and the change flag (0x40) are set.
void setParent(SceneNode& self, SceneNode* parent) {
    grab(&self);
    self.parent = parent;
    self.flags |= 0x40u;
    drop(&self);
}

//@ 0x597dcc
//@ partial: copies the name, the flags and the scene-manager reset; the transform blocks and
//@ the virtual clone of the base (+0xb4) are not mapped.
void cloneMembers(SceneNode& self, const SceneNode& other) {
    self.name = other.name;
    self.flags = other.flags;
    self.sceneManager = nullptr;
}

//@ 0x597f68
//@ partial: releases and clears the binded animators; the per-animator detach hook (+0x2c)
//@ is not mapped.
void removeBindedAnimators(SceneNode& self) {
    for (SceneNodeAnimator* animator : self.bindedAnimators) {
        drop(animator);
    }
    self.bindedAnimators.clear();
}

//@ 0x597fe4
//@ partial: the detach hook (+0x2c) on the animator is not mapped; releases and erases it.
void removeBindedAnimator(SceneNode& self, SceneNodeAnimator* animator) {
    auto& list = self.bindedAnimators;
    auto it = std::find(list.begin(), list.end(), animator);
    if (it == list.end()) {
        return;
    }
    drop(*it);
    list.erase(it);
}

//@ 0x598658
//@ partial: releases and clears the animators; the per-animator detach hook (+0x2c) and the
//@ scene-manager hierarchy notification are not mapped.
void removeAnimators(SceneNode& self) {
    for (SceneNodeAnimator* animator : self.animators) {
        drop(animator);
    }
    self.animators.clear();
}

//@ 0x5986e8
//@ partial: the detach hook (+0x2c) and the scene-manager hierarchy notification are not mapped.
void removeAnimator(SceneNode& self, SceneNodeAnimator* animator) {
    auto& list = self.animators;
    auto it = std::find(list.begin(), list.end(), animator);
    if (it == list.end()) {
        return;
    }
    drop(*it);
    list.erase(it);
}

//@ 0x598770
//@ partial: the attach hook (+0x28) and the scene-manager hierarchy notification are not mapped.
void addAnimator(SceneNode& self, SceneNodeAnimator* animator) {
    if (animator == nullptr) {
        return;
    }
    grab(animator);
    self.animators.push_back(animator);
}

//@ 0x5987e8
//@ partial: releases and clears the children; the scene-manager hierarchy notification is not mapped.
void removeAll(SceneNode& self) {
    for (SceneNode* child : self.children) {
        drop(child);
    }
    self.children.clear();
}

//@ 0x598a2c
//@ partial: the line print (uid, name, scope and type tag) through os::Printer is not mapped;
//@ the recursion over the children is reproduced.
void logTree(const SceneNode& self, int depth, int maxDepth) {
    const bool printHere = !(maxDepth != 0 && depth >= maxDepth);
    if (printHere) {
        for (const SceneNode* child : self.children) {
            if (child != nullptr) {
                logTree(*child, depth + 1, maxDepth);
            }
        }
    }
}

//@ 0x598b84
//@ 0x598cbc
//@ partial: the base teardown of ISceneNode (children and animators) is not reconstructed in
//@ this pass; the vtable reset is not modelled.
void releaseSceneNodeMembers(SceneNode& self) {
    (void)self;
}

//@ 0x59901c
//@ partial: the translation, rotation and scale decomposition (CMatrix4 and quaternion ops) is
//@ not mapped; the transform blocks are not written.
void setRelativeTransformation(SceneNode& self, const void* transform) {
    (void)self;
    (void)transform;
}

//@ 0x599444
//@ partial: the matrix inverse and product through the parent (CMatrix4) are not mapped.
void setAbsoluteTransformation(SceneNode& self, const void* transform) {
    (void)self;
    (void)transform;
}

// ----------------------------------------------------------- SAnimatedMesh

//@ 0x599f90
//@ partial: the buffer count comes from the first mesh's virtual slot (+0x10), not mapped; 0 here.
std::uint32_t getMeshBufferCount(const SAnimatedMesh& self) {
    (void)self;
    return 0;
}

//@ 0x599fc4
//@ partial: the buffer fetch goes through the first mesh's virtual slot (+0x14), not mapped; null here.
RefCounted* getMeshBuffer(const SAnimatedMesh& self, unsigned index) {
    (void)self;
    (void)index;
    return nullptr;
}

}  // namespace glitch::collada::recon_06
