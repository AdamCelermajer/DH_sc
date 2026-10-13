// Batch g1-game-world-AssetManager-cbdbfa-0 (part 1 of 1): the AssetManager singleton, its map values, the texture and
// scene-node maps and the asset walk. Declarations come from dh2/AssetManager.hpp. A PARTIAL body names what it waits on.
//
// Device path. The original reaches the scene manager and the texture manager through the Application singleton's
// m_device (s_inst +0x10, the global dword_99F73C): the device's +0x1c is the SceneManager and the device's +0x10 is the
// video driver whose +0xe0 is the CTextureManager (asm of 0x50a4c4, 0x50a504, 0x50a540 and 0x50bdb4). IDevice declares
// neither +0x10 nor +0x1c, so every body that needs them is PARTIAL (layout requests in the ledger notes).
//
// Map walks. The original walks its red-black trees by hand. The C++ walks use std::map iterators, which visit the same
// nodes in the same order. Where a walk erases the node it is standing on, the iterator is moved on first and the key is
// copied before the erase, as the original moves its successor before the call.

#include <cstddef>
#include <string>

#include "dh2/AssetManager.hpp"
#include "glitch-core-io/core/reference_counted.hpp"
// ITexture is complete through ISceneNode.hpp, which pulls in the glitch_core_objects.hpp stand-in. glitch-video/texture.hpp
// cannot be included in the same unit: it redefines glitch::video::ITexture. Only its IReferenceCounted base is used here.
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

namespace {

// File-scope flag toggled by AssetManager::ToggleWireFrame (a byte in the original, read and written with LDRB/STRB).
// Its initial value is not confirmed from the binary; false is assumed.
bool s_wireFrame = false;

}  // namespace

// 0x50a474 AssetManager::SceneNode::~SceneNode()   (complete object destructor, D1)
// Drops the scene node through its IReferenceCounted base (virtual base offset read from the vtable in the original).
AssetManager::SceneNode::~SceneNode() {
    if (m_sceneNode != nullptr) {
        static_cast<glitch::IReferenceCounted*>(m_sceneNode)->drop();
    }
}

// 0x50a4c4 AssetManager::ToggleWireFrame()
// PARTIAL: the toggle of s_wireFrame is written. The original then calls SetWireFrame(sm->rootNode, s_wireFrame) (a tail
// call), where sm is the SceneManager at IDevice +0x1c and rootNode is CSceneManager +0x04. IDevice declares no +0x1c
// (layout request), so the call is not written.
void AssetManager::ToggleWireFrame() {
    s_wireFrame = !s_wireFrame;
}

// 0x50a504 AssetManager::loadSceneNode(char const*, char const*, bool, int)
// PARTIAL: the original calls SceneManager::LoadScene(sm, first, second, false, second != nullptr) and returns the node.
// sm is the SceneManager at IDevice +0x1c (asm: LDR R0,[R0,#0x1C] after s_inst +0x10), which IDevice does not declare
// (layout request). The flag and option arguments are not read by the original: the call passes false and the
// second-name test instead. The node is null here.
glitch::scene::ISceneNode* AssetManager::loadSceneNode(const char* first, const char* second, bool flag, int option) {
    (void)first;
    (void)second;
    (void)flag;
    (void)option;
    return nullptr;
}

// 0x50a540 AssetManager::preloadSceneNode(char const*, char const*, bool, int)
// PARTIAL: the original calls SceneManager::PreloadScene(sm, first, second) (a tail call). sm is the SceneManager at
// IDevice +0x1c (layout request), so the call is not written. The flag and option arguments are not read.
void AssetManager::preloadSceneNode(const char* first, const char* second, bool flag, int option) {
    (void)first;
    (void)second;
    (void)flag;
    (void)option;
}

// 0x50a564 AssetManager::GetAssetManager()
// The original inlines the constructor of the function-local static: each map header points at itself and holds no
// node, and the cursors are null. That is the state the implicit default constructor of AssetManager gives. The guard
// variable, the release and the atexit registration of ~AssetManager are the compiler's for a function-local static.
AssetManager* AssetManager::GetAssetManager() {
    static AssetManager assetManager;
    return &assetManager;
}

// 0x50a70c AssetManager::Texture::~Texture()   (complete object destructor, D1)
// Releases the stored texture through its IReferenceCounted base.
AssetManager::Texture::~Texture() {
    if (m_texture != nullptr) {
        m_texture->drop();
    }
}

// 0x50a74c AssetManager::Texture::~Texture()   (deleting destructor, D0)
// skipped: the compiler generates the deleting destructor from the virtual ~Texture() declared in dh2/AssetManager.hpp
// (the D1 body above, then CustomFree(this) in the original, which is delete this here).

// 0x50a794 AssetManager::SceneNode::~SceneNode()   (deleting destructor, D0)
// skipped: the compiler generates the deleting destructor from the virtual ~SceneNode() declared in dh2/AssetManager.hpp
// (the D1 body above, then CustomFree(this) in the original, which is delete this here).

// 0x50aa4c AssetManager::~AssetManager()
// The original erases the three maps in reverse declaration order (loadedScenes at +0x38, textures at +0x18, scene nodes
// at +0x00) and resets each header to empty. Erasing a Texture or SceneNode value runs its destructor, which releases the
// stored object. The cursors are not reset by the original and are not used after the destructor.
AssetManager::~AssetManager() {
    m_loadedScenes.clear();
    m_textures.clear();
    m_sceneNodes.clear();
}

// 0x50addc AssetManager::getNextAsset(std::string&)
// Walks the textures first, then the scene nodes. Each call writes one name and moves the cursor past it:
//   "Texture: <name>" for a texture entry; when the texture walk ends, the scene-node cursor starts at the first node;
//   "Scene node: <name>" plus " Ref #: <count>" when the node holds a scene node, for a scene-node entry.
// Returns 1 when a name was written and 0 when nothing is left to report (no texture left and the scene-node cursor at
// its end). The texture entries carry no reference count in the name.
int AssetManager::getNextAsset(std::string& name) {
    if (m_textureCursor == m_textures.end()) {
        if (m_sceneNodeCursor == m_sceneNodes.end()) {
            return 0;
        }
        name = "Scene node: ";
        name += m_sceneNodeCursor->first;
        const glitch::scene::ISceneNode* node = m_sceneNodeCursor->second.m_sceneNode;
        if (node != nullptr) {
            name += " Ref #: ";
            name += std::to_string(static_cast<const glitch::IReferenceCounted*>(node)->referenceCounter);
        }
        ++m_sceneNodeCursor;
        return 1;
    }

    name = "Texture: ";
    name += m_textureCursor->first;
    ++m_textureCursor;
    if (m_textureCursor == m_textures.end()) {
        m_sceneNodeCursor = m_sceneNodes.begin();
    }
    return 1;
}

// 0x50b020 AssetManager::getFirstAsset(std::string&)
// Starts the walk: the texture cursor goes to the first texture and the scene-node cursor to the end sentinel, so the
// scene nodes are only reached once the textures are exhausted (and then from their first entry).
int AssetManager::getFirstAsset(std::string& name) {
    m_textureCursor = m_textures.begin();
    m_sceneNodeCursor = m_sceneNodes.end();
    return getNextAsset(name);
}

// 0x50bdb4 AssetManager::loadTexture(char const*, int)
// PARTIAL. Returns the texture stored under name with one use for the caller and lowers the entry's flags level to the
// argument when it is smaller. The cached path (the entry already holds a texture) is written with the original's use
// counts: one use taken and dropped, two added for the entry and the result, and the local use released. The load path
// (no texture stored yet) calls CTextureManager::getTexture(name, nullptr) on the manager at IDevice +0x10 -> IVideoDriver
// +0xe0 (asm of 0x50bdb4: LDR R2,[R0,#0x10]; LDR R7,[R2,#0xE0]). IDevice does not declare +0x10 (layout request), so that
// path returns null and stores nothing. A fresh entry is value-initialised here (flags 0, texture null); the Texture
// constructor is not in this batch, so its initial flags value is not confirmed.
boost::intrusive_ptr<glitch::video::ITexture> AssetManager::loadTexture(const char* name, int flags) {
    boost::intrusive_ptr<glitch::video::ITexture> result;
    Texture& entry = m_textures[name];
    glitch::IReferenceCounted* stored = entry.m_texture;
    if (stored != nullptr) {
        ++stored->referenceCounter;     // use taken for the lookup...
        stored->drop();                 // ...and dropped again (the pair cancels)
        stored->referenceCounter += 2;  // the entry's use and the result's use are added together
        stored->drop();                 // the local use is released at the end of the original
        result.reset(static_cast<glitch::video::ITexture*>(stored));  // the result takes over the use left
    }
    // PARTIAL: the load path (no stored texture) is not written, see the note above.

    if (flags < entry.unk_04) {
        entry.unk_04 = flags;
    }
    return result;
}

// 0x50bef0 AssetManager::preloadTexture(char const*, int)
// PARTIAL: the body is loadTexture followed by the release of its result, which the temporary's destructor does. It
// inherits the partial load path of loadTexture (0x50bdb4).
void AssetManager::preloadTexture(const char* name, int flags) {
    (void)loadTexture(name, flags);
}

// 0x50bf28 AssetManager::dropTexture(char const*)
// Looks the name up (inserting an empty entry when it is absent, as the original's operator[] does). A texture whose count
// is above one after the lookup's use is only given back. Otherwise the original removes the texture from the manager and
// erases the entry.
// PARTIAL: CTextureManager::removeTexture is not called (the manager is at IDevice +0x10 -> +0xe0, layout request). The
// erase is written. The release of the caller's use, which the original's removeTexture makes by clearing its pointer,
// is not written, so this branch leaves that one use on the texture. The branch is reached only when the stored
// texture's count is zero before the lookup's use.
void AssetManager::dropTexture(const char* name) {
    const std::string key(name);
    Texture& entry = m_textures[key];
    glitch::IReferenceCounted* texture = entry.m_texture;
    if (texture == nullptr) {
        return;
    }
    ++texture->referenceCounter;
    if (static_cast<unsigned int>(texture->referenceCounter) > 1u) {
        texture->drop();
        return;
    }
    // PARTIAL: removeTexture(manager, &texture) is not called, see the note above.
    const auto it = m_textures.find(key);
    if (it != m_textures.end()) {
        m_textures.erase(it);
    }
}

// 0x50c02c AssetManager::clearTexture(int)
// Visits every texture in key order. An entry is dropped (dropTexture by name) when the argument is at most the entry's
// flags level. The successor is taken before the drop, since the drop can erase the entry.
// PARTIAL: each drop goes through dropTexture(const char*), which is partial (see 0x50bf28).
void AssetManager::clearTexture(int flags) {
    for (auto it = m_textures.begin(); it != m_textures.end();) {
        const auto current = it++;
        if (flags <= current->second.unk_04) {
            dropTexture(current->first.c_str());
        }
    }
}

// 0x50c124 AssetManager::dropTexture(boost::intrusive_ptr<glitch::video::ITexture>)
// Visits the textures in key order and stops at the first entry whose stored texture is the argument (the null argument
// matches a null entry). That entry is dropped by name. The original takes a use on each stored texture and drops it at
// once; the pair leaves the count unchanged for a stored texture, and that is kept here.
// PARTIAL: the matching entry is dropped through dropTexture(const char*), which is partial (see 0x50bf28).
void AssetManager::dropTexture(boost::intrusive_ptr<glitch::video::ITexture> texture) {
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it) {
        glitch::IReferenceCounted* stored = it->second.m_texture;
        if (stored != nullptr) {
            ++stored->referenceCounter;
            stored->drop();
        }
        if (stored == texture.get()) {
            dropTexture(it->first.c_str());
            return;
        }
    }
}

// 0x50c8a0 AssetManager::dropSceneNode(std::string const&)
// Erases the named entry when it holds no scene node, or when its scene node's reference count is at most one.
// The original looks the entry up with operator[] (inserting an empty one when absent), then finds and erases it. The
// insert and the erase cancel for an absent name, which is the same here.
void AssetManager::dropSceneNode(const std::string& name) {
    const glitch::scene::ISceneNode* node = m_sceneNodes[name].m_sceneNode;
    const bool erase =
        node == nullptr ||
        static_cast<unsigned int>(static_cast<const glitch::IReferenceCounted*>(node)->referenceCounter) <= 1u;
    if (erase) {
        const auto it = m_sceneNodes.find(name);
        if (it != m_sceneNodes.end()) {
            m_sceneNodes.erase(it);
        }
    }
}

// 0x50c918 AssetManager::clearSceneNode(int)
// Visits every scene node in key order. An entry is dropped (dropSceneNode by name) when the argument is at most the
// entry's flags level. The successor is taken before the drop, as in clearTexture.
void AssetManager::clearSceneNode(int flags) {
    for (auto it = m_sceneNodes.begin(); it != m_sceneNodes.end();) {
        const auto current = it++;
        if (flags <= current->second.unk_04) {
            dropSceneNode(current->first);
        }
    }
}

// 0x50ca0c AssetManager::clear(int)
// Clears the scene nodes, then the textures, with the same argument.
// PARTIAL: the texture half is clearTexture (see 0x50c02c), which is partial.
void AssetManager::clear(int flags) {
    clearSceneNode(flags);
    clearTexture(flags);
}

// 0x50ca2c AssetManager::dropSceneNode(char const*, char const*)
// Joins the scene name and the node name with '#' and drops the entry of that name.
void AssetManager::dropSceneNode(const char* sceneName, const char* nodeName) {
    std::string name(sceneName);
    name += '#';
    name += nodeName;
    dropSceneNode(name);
}
