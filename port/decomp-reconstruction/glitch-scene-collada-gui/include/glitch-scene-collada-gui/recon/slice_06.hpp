// Slice 06 of the glitch::scene reconstruction: scene-graph bookkeeping of
// CSceneManager, ISceneNode, CTriangleSelector, IMesh and SAnimatedMesh.
//
// The owning headers (scene_nodes.hpp, scene_node.hpp) map only accessor members
// and are read-only for this slice, so the state the bodies touch is modelled here
// with the same field order as the original. Offsets are kept as comments; the code
// uses typed members only. Free functions take the object explicitly.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::collada::recon_06 {

// Four-character type tag, packed the way the original compares it.
constexpr std::uint32_t fourcc(const char (&s)[5]) {
    return static_cast<std::uint32_t>(static_cast<std::uint8_t>(s[0])) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(s[1])) << 8) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(s[2])) << 16) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(s[3])) << 24);
}

inline constexpr std::uint32_t kNodeTypeEmpty = fourcc("emty");          // 0x79746d65
inline constexpr std::uint32_t kNodeTypeDummy = fourcc("dmmy");          // 0x796d6d64
inline constexpr std::uint32_t kNodeTypeDaen = fourcc("daen");           // 0x6e656164
inline constexpr std::uint32_t kNodeTypeSceneManagerRoot = fourcc("smgr");  // 0x72676d73
inline constexpr std::uint32_t kNodeTypeAny = fourcc("any_");            // 0x5f796e61, type query wildcard
inline constexpr std::uint32_t kNodeTypeUnknown = fourcc("unkn");        // 0x6e6b6e75

// Sentinel passed to CSceneManager::update to reset the clock from the timer.
inline constexpr float kUpdateResetTime = -123460.0f;

// Intrusive reference count. The original IReferenceCounted keeps the count at +0x04,
// after the vtable pointer.
class RefCounted {
public:
    virtual ~RefCounted() = default;
    int referenceCount = 1;  // +0x04
};

void grab(RefCounted* object);
void drop(RefCounted* object);

// Material word layout: the original keeps the count at +0x00 (CMaterial).
struct Material {
    int referenceCount = 1;  // +0x00
};
void grabMaterial(Material* material);
void dropMaterial(Material* material);

class SceneManager;
class TriangleSelector;

// Stand-in for ISceneNodeAnimator (count and vtable are not mapped here).
class SceneNodeAnimator : public RefCounted {};

// Stand-in for ISceneNode. Child, animator and binded-animator lists are kept as
// vectors in the same order as the original intrusive rings.
class SceneNode : public RefCounted {
public:
    std::uint32_t type = kNodeTypeUnknown;   // getType (virtual in the original)
    std::string name;                        // +0x0c
    std::string uid;                         // getUID (+0x20 in the original)
    std::string scopeId;                     // getScopeID
    SceneNode* parent = nullptr;             // +0xec
    SceneManager* sceneManager = nullptr;    // +0x110
    TriangleSelector* triangleSelector = nullptr;  // +0x114
    bool visible = true;                     // +0x120
    bool visibilityLatched = false;          // +0x121
    std::uint32_t flags = 0;                 // +0x11c, bit 0 visible, bit 11 shadow caster
    std::vector<SceneNode*> children;        // +0xf4 ring
    std::vector<SceneNodeAnimator*> animators;        // +0xfc ring, owning
    std::vector<SceneNodeAnimator*> bindedAnimators;  // +0x104 ring, owning
};

// Stand-in for CSceneManager. Vector members keep the original begin/end/cap order
// as comments; ring and flag members use the same offsets.
class SceneManager {
public:
    SceneNode* root = nullptr;                                   // +0x04
    std::vector<RefCounted*> shadowReceiverTargets;              // +0x90..+0x98
    std::vector<RefCounted*> externalMeshLoaders;                // +0xb4..+0xbc
    std::vector<SceneNode*> deletionQueue;                       // +0xc0..+0xc8
    std::vector<RefCounted*> sceneNodeFactories;                 // +0xcc..+0xd4
    std::vector<RefCounted*> sceneNodeAnimatorFactories;         // +0xd8..+0xe0
    std::vector<SceneNode*> renderNodes;                         // +0x270..+0x278 (list A)
    std::vector<SceneNode*> animatedNodes;                       // +0x27c..+0x284 (list B)
    bool collectPending = false;                                 // +0x288
    float time = 0.0f;                                           // +0x254
};

// Stand-in for CBatchSceneNode's batch configuration (SBatchConfig, 0x2c bytes).
struct SBatchConfig {
    Material* material = nullptr;     // +0x00 (ref count at +0x00 of the material)
    std::uint32_t words1[4] = {};     // +0x04..+0x10
    std::uint32_t words2[4] = {};     // +0x14..+0x20
    RefCounted* extra = nullptr;      // +0x28
    SBatchConfig& operator=(const SBatchConfig& other);
};

// Stand-in for SDefaultEndOfBatchCallback: the batch configurations keyed by id.
struct SDefaultEndOfBatchCallback {
    std::map<std::uint32_t, SBatchConfig> configs;  // +0x14 tree
};

// Stand-in for STransparentNodeEntry (only the material reference is used here).
struct STransparentNodeEntry {
    Material* material = nullptr;     // +0x08
    ~STransparentNodeEntry();
};

// Stand-in for CTriangleSelector (only the owned buffer at +0x0c is used here).
class TriangleSelector : public RefCounted {
public:
    const SceneNode* node = nullptr;  // +0x08
    void* ownedBuffer = nullptr;      // +0x0c
    bool flag = false;                // +0x18
};

// Stand-in for CAppendMeshBuffer: the byte counts of its vertex and index data.
class AppendMeshBuffer {
public:
    std::uint32_t vertexBytes = 0;    // +0x3c
    std::uint32_t indexBytes = 0;     // +0x44
};

// Stand-in for SAnimatedMesh: the mesh list is the vector at +0x20.
class SAnimatedMesh {
public:
    std::vector<RefCounted*> meshes;  // +0x20..+0x24
};

// ---- CSceneManager ----
void addShadowReceiverTarget(SceneManager& self, RefCounted* target);
void registerSceneNodeFactory(SceneManager& self, RefCounted* factory);
void registerSceneNodeAnimatorFactory(SceneManager& self, RefCounted* factory);
void addExternalMeshLoader(SceneManager& self, RefCounted* loader);
void clearDeletionList(SceneManager& self);
void addToDeletionQueue(SceneManager& self, SceneNode* node);
void collectAllNodes(SceneManager& self);
void collectAllNodes(SceneManager& self, SceneNode* node);
void update(SceneManager& self, float elapsed, bool updateAnimators);
void getSceneNodesFromType(SceneManager& self, std::uint32_t type,
                           std::vector<SceneNode*>& out, SceneNode* start);

// ---- ISceneNode ----
void getSceneNodesFromType(SceneNode& self, std::uint32_t type, std::vector<SceneNode*>& out);
SceneNode* getSceneNodeFromType(SceneNode& self, std::uint32_t type);
const SceneNode* getSceneNodeFromName(const SceneNode& self, const char* name);
SceneNode* getSceneNodeFromName(SceneNode& self, const char* name);
const SceneNode* getSceneNodeFromScopeID(const SceneNode& self, const char* scope);
SceneNode* getSceneNodeFromScopeID(SceneNode& self, const char* scope);
const SceneNode* getSceneNodeFromUID(const SceneNode& self, const char* uid);
SceneNode* getSceneNodeFromUID(SceneNode& self, const char* uid);
void setTriangleSelector(SceneNode& self, TriangleSelector* selector);
void setName(SceneNode& self, const std::string& name);
void setName(SceneNode& self, const char* name);
void addBindedAnimator(SceneNode& self, SceneNodeAnimator* animator);

// ---- SAnimatedMesh ----
RefCounted* getMesh(const SAnimatedMesh& self, int index);

// ---- CSceneManager, partial (see the cpp for the omitted part of each)
RefCounted* getMeshFromFile(SceneManager& self, const void* readFile, int loaderIndex);
RefCounted* getMeshFromName(SceneManager& self, const char* name);
void writeSceneNode(SceneManager& self, const SceneNode& node, void* xmlWriter, void* userData);
void saveScene(SceneManager& self, void* writeFile, void* userData);
void loadScene(SceneManager& self, const void* readFile, void* userData);
SceneManager* createNewSceneManager(SceneManager& self, bool copyRoot);
void releaseSceneManagerMembers(SceneManager& self);
void copyVertexData(AppendMeshBuffer& self, void* destination);
void copyIndexData(AppendMeshBuffer& self, void* destination);
void destroyEndOfBatchCallback(SDefaultEndOfBatchCallback& self);

// ---- CTriangleSelector, partial
void releaseTriangleSelector(TriangleSelector& self);
void constructTriangleSelectorFromBox(TriangleSelector& self, const core::aabbox3d<float>& box,
                                      const SceneNode* node, bool flag);
void constructTriangleSelectorFromNode(TriangleSelector& self, const SceneNode* node, bool flag);
void constructTriangleSelectorFromMesh(TriangleSelector& self, const RefCounted* mesh,
                                       const SceneNode* node, bool flag);
float getTrianglesInBox(TriangleSelector& self, int maxCount, int& outCount);
bool getTrianglesOnLine(TriangleSelector& self, int maxCount, int& outCount);

// ---- ISceneNode and IMeshSceneNode, partial
void setNodeHierarchyAsShadowCaster(SceneNode& self, bool enabled);
void setMeshHierarchyAsShadowCaster(SceneNode& self, bool enabled);
void onDelete(SceneNode& self);
void onUpdateTime(SceneNode& self, std::uint32_t time);
void notifyVisibilityChanged(SceneNode& self, bool visibleInParent);
void setVisible(SceneNode& self, bool visible);
void remove(SceneNode& self);
void setParent(SceneNode& self, SceneNode* parent);
void cloneMembers(SceneNode& self, const SceneNode& other);
void removeBindedAnimators(SceneNode& self);
void removeBindedAnimator(SceneNode& self, SceneNodeAnimator* animator);
void removeAnimators(SceneNode& self);
void removeAnimator(SceneNode& self, SceneNodeAnimator* animator);
void addAnimator(SceneNode& self, SceneNodeAnimator* animator);
void removeAll(SceneNode& self);
void logTree(const SceneNode& self, int depth, int maxDepth);
void releaseSceneNodeMembers(SceneNode& self);
void setRelativeTransformation(SceneNode& self, const void* transform);
void setAbsoluteTransformation(SceneNode& self, const void* transform);

// ---- SAnimatedMesh, partial
std::uint32_t getMeshBufferCount(const SAnimatedMesh& self);
RefCounted* getMeshBuffer(const SAnimatedMesh& self, unsigned index);

}  // namespace glitch::collada::recon_06
