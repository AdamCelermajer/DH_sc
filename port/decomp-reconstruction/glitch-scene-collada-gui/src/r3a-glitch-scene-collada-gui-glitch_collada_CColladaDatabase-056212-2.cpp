// Track B redo batch r3a-glitch-scene-collada-gui-glitch_collada_CColladaDatabase-056212-2 (part 3 of 4).
// Owner: glitch::collada::CColladaDatabase. Visual-scene and node overloads by index and name, the animation and default-value
// lookups, and the scene/node/material overloads that load a file by name.
// Partial here: every overload that loads a file (CResFileManager::load / get with the DefaultFactory, which is not declared), the
// scene/node builders that need the root's virtual slots, the per-type default-value switch and the material factory path.
// The e2 file keeps the stub for constructScene(driver) (0x61b9e8) and constructNode(driver, SNode*, root) (0x61b2f4).

#include <cstring>

#include "glitch-scene-collada-gui/collada/CColladaDatabase.hpp"
#include "glitch-scene-collada-gui/collada_factory.hpp"

namespace glitch::collada {

// 0x61b988 glitch::collada::CColladaDatabase::constructVisualScene(glitch::video::IVideoDriver*, int, glitch::collada::CRootSceneNode*) const
CRootSceneNode* CColladaDatabase::constructVisualScene(video::IVideoDriver* driver, int index, CRootSceneNode* root) const {
    return constructVisualScene(driver, visualSceneAt(tables(), index), root);
}

// 0x61b9b8 glitch::collada::CColladaDatabase::constructVisualScene(glitch::video::IVideoDriver*, char const*, glitch::collada::CRootSceneNode*) const
CRootSceneNode* CColladaDatabase::constructVisualScene(video::IVideoDriver* driver, const char* name, CRootSceneNode* root) const {
    return constructVisualScene(driver, getVisualScene(tables(), name), root);
}

// 0x61babc glitch::collada::CColladaDatabase::constructScene(glitch::video::IVideoDriver*, bool) const
// PARTIAL: the scene is built by constructScene(driver) (0x61b9e8, a stub in the e2 file). When the flag is set and an animator is built, the
// original calls root slot +0x6c with it and drops the animator. The root slot is not named in root_scene_node.hpp, so the animator is
// not attached here.
CRootSceneNode* CColladaDatabase::constructScene(video::IVideoDriver* driver, bool flag) const {
    CRootSceneNode* root = constructScene(driver);
    if (root == nullptr || !flag) {
        return root;
    }
    return root;
}

// 0x61bb20 glitch::collada::CColladaDatabase::constructScene(glitch::video::IVideoDriver*, char const*, glitch::collada::CColladaFactory*)
// PARTIAL: loads `fileName` through CResFileManager::load and runs the scene build on a temporary database with the given factory (or
// the DefaultFactory, which is not declared). Null here.
CRootSceneNode* CColladaDatabase::constructScene(video::IVideoDriver* driver, const char* fileName, CColladaFactory* factory) {
    (void)driver;
    (void)fileName;
    (void)factory;
    return nullptr;
}

// 0x61bbd4 glitch::collada::CColladaDatabase::constructScene(glitch::video::IVideoDriver*, char const*, bool, glitch::collada::CColladaFactory*)
// PARTIAL: as 0x61bb20, with the animator flag. Null here.
CRootSceneNode* CColladaDatabase::constructScene(video::IVideoDriver* driver, const char* fileName, bool flag,
                                                 CColladaFactory* factory) {
    (void)driver;
    (void)fileName;
    (void)flag;
    (void)factory;
    return nullptr;
}

// 0x61bc8c glitch::collada::CColladaDatabase::constructScene(glitch::video::IVideoDriver*, glitch::io::IReadFile*, bool, bool, glitch::collada::CColladaFactory*)
// PARTIAL: as 0x61bb20 for a read file (CResFileManager::load with both flags). Null here.
CRootSceneNode* CColladaDatabase::constructScene(video::IVideoDriver* driver, glitch::io::IReadFile* file, bool flagA, bool flagB,
                                                 CColladaFactory* factory) {
    (void)driver;
    (void)file;
    (void)flagA;
    (void)flagB;
    (void)factory;
    return nullptr;
}

// 0x61bd54 glitch::collada::CColladaDatabase::constructScene(glitch::video::IVideoDriver*, glitch::io::IReadFile*, bool, glitch::collada::CColladaFactory*)
// PARTIAL: as 0x61bc8c. Null here.
CRootSceneNode* CColladaDatabase::constructScene(video::IVideoDriver* driver, glitch::io::IReadFile* file, bool flag,
                                                 CColladaFactory* factory) {
    (void)driver;
    (void)file;
    (void)flag;
    (void)factory;
    return nullptr;
}

// 0x61be14 glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*) const
// PARTIAL: the original returns null for a null node. Otherwise it creates a root with factory slot +0x54, builds the node with the
// three-argument constructNode (0x61b2f4, partial in the e2 file), attaches it through root slot +0x5c, runs onPostLoad and drops the
// node. The root slots are not named, so only the null case is reproduced.
CRootSceneNode* CColladaDatabase::constructNode(video::IVideoDriver* driver, SNode* node) const {
    (void)driver;
    if (node == nullptr) {
        return nullptr;
    }
    return nullptr;
}

// 0x61c1fc glitch::collada::CColladaDatabase::getBlendableAnimation(glitch::collada::SAnimation const*) const
SAnimation* CColladaDatabase::getBlendableAnimation(const SAnimation* animation) const {
    if (animation == nullptr) {
        return nullptr;
    }
    return getBlendableAnimation(animation->channels);
}

// 0x61c2f8 glitch::collada::CColladaDatabase::getDefaultValue(char const*, glitch::collada::SChannel::Type, void**, void*) const
// PARTIAL: the original dispatches on the channel type (the 944-byte switch) to write the default value through `out`. The switch is not
// transcribed in this pass. Returns false.
bool CColladaDatabase::getDefaultValue(const char* name, SChannel::Type type, void** out, void* flag) const {
    (void)name;
    (void)type;
    (void)out;
    (void)flag;
    return false;
}

// 0x61c6bc glitch::collada::CColladaDatabase::getDefaultValue(glitch::collada::SChannel const*, void**) const
// The original passes the channel's bind name (+0x04), type (+0x08) and flag word (+0x0c) to the overload above.
bool CColladaDatabase::getDefaultValue(const SChannel* channel, void** out) const {
    return getDefaultValue(channel->name, static_cast<SChannel::Type>(channel->type), out,
                           const_cast<void*>(static_cast<const void*>(&channel->flag)));
}

// 0x61c6ec glitch::collada::CColladaDatabase::getDefaultValue(glitch::collada::SAnimation const*, void**) const
bool CColladaDatabase::getDefaultValue(const SAnimation* animation, void** out) const {
    return getDefaultValue(animation->channels, out);
}

// 0x61c6f4 glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*) const
// The node is found by name (getNode) and built by the node overload above, which is partial.
CRootSceneNode* CColladaDatabase::constructNode(video::IVideoDriver* driver, const char* nodeName) const {
    return constructNode(driver, getNode(nodeName));
}

// 0x61c71c glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, bool, glitch::collada::CColladaFactory*)
// PARTIAL: loads the file and builds the named node on a temporary database (see 0x61bb20). Null here.
CRootSceneNode* CColladaDatabase::constructNode(video::IVideoDriver* driver, glitch::io::IReadFile* file, const char* nodeName,
                                                bool flag, CColladaFactory* factory) {
    (void)driver;
    (void)file;
    (void)nodeName;
    (void)flag;
    (void)factory;
    return nullptr;
}

// 0x61c7cc glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::io::IReadFile*, char const*, glitch::collada::CColladaFactory*)
// PARTIAL: as 0x61c71c without the flag. Null here.
CRootSceneNode* CColladaDatabase::constructNode(video::IVideoDriver* driver, glitch::io::IReadFile* file, const char* nodeName,
                                                CColladaFactory* factory) {
    (void)driver;
    (void)file;
    (void)nodeName;
    (void)factory;
    return nullptr;
}

// 0x61c878 glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, char const*, char const*, glitch::collada::CColladaFactory*)
// PARTIAL: as 0x61c7cc, with the file named by a string. Null here.
CRootSceneNode* CColladaDatabase::constructNode(video::IVideoDriver* driver, const char* fileName, const char* nodeName,
                                                CColladaFactory* factory) {
    (void)driver;
    (void)fileName;
    (void)nodeName;
    (void)factory;
    return nullptr;
}

// 0x61c91c glitch::collada::CColladaDatabase::getAnimation(char const*, glitch::collada::SChannel::Type, char const*) const
// The first animation whose first channel has the given type, bind URI (compared without case, the 'extra' word at channel +0x0c) and
// name (channel +0x04).
SAnimation* CColladaDatabase::getAnimation(const char* name, SChannel::Type type, const char* uri) const {
    const std::int32_t count = static_cast<std::int32_t>(tables().animations.size());
    for (std::int32_t i = 0; i < count; ++i) {
        SAnimation* animation = animationAt(tables(), i);
        const SChannel* channel = animation->channels;
#ifdef _WIN32
        const bool uriMatches = _stricmp(channel->extra, uri) == 0;
#else
        const bool uriMatches = strcasecmp(channel->extra, uri) == 0;
#endif
        if (channel->type == static_cast<std::uint32_t>(type) && uriMatches && std::strcmp(channel->name, name) == 0) {
            return animation;
        }
    }
    return nullptr;
}

// 0x61cca8 glitch::collada::CColladaDatabase::constructMaterial(glitch::IReferenceCounted**, glitch::video::IVideoDriver*, glitch::collada::SMaterial*, glitch::collada::CRootSceneNode*) const
// PARTIAL: the material is built through factory slot +0x20 (createMaterial). collada_factory.hpp returns a raw CMaterial* there, and the
// out-slot form of the binary (an intrusive_ptr written through the first argument) is not confirmed. The out slot is left null.
glitch::IReferenceCounted** CColladaDatabase::constructMaterial(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                                SMaterial* material, CRootSceneNode* root) const {
    (void)driver;
    (void)material;
    (void)root;
    *out = nullptr;
    return out;
}

// 0x61cefc glitch::collada::CColladaDatabase::constructMaterial(glitch::IReferenceCounted**, glitch::video::IVideoDriver*, char const*, glitch::collada::CRootSceneNode*) const
// The material is found by name and built by the overload above.
glitch::IReferenceCounted** CColladaDatabase::constructMaterial(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                                const char* name, CRootSceneNode* root) const {
    return constructMaterial(out, driver, getMaterial(tables(), name), root);
}

}  // namespace glitch::collada
