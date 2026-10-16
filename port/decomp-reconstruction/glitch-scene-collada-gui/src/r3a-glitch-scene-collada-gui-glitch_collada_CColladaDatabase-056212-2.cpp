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

// 0x61c1fc glitch::collada::CColladaDatabase::getBlendableAnimation(glitch::collada::SAnimation const*) const
SAnimation* CColladaDatabase::getBlendableAnimation(const SAnimation* animation) const {
    if (animation == nullptr) {
        return nullptr;
    }
    return getBlendableAnimation(animation->channels);
}

// 0x61c6ec glitch::collada::CColladaDatabase::getDefaultValue(glitch::collada::SAnimation const*, void**) const
bool CColladaDatabase::getDefaultValue(const SAnimation* animation, void** out) const {
    return getDefaultValue(animation->channels, out);
}

// 0x61c91c glitch::collada::CColladaDatabase::getAnimation(char const*, glitch::collada::SChannel::Type, char const*) const
// The first animation whose first channel has the given type, bind URI (compared without case, the 'extra' word at channel +0x0c) and
// name (channel +0x04).
SAnimation* CColladaDatabase::getAnimation(const char* name, SChannel::Type type, const char* uri) const {
    const std::int32_t count = static_cast<std::int32_t>(tables().animations.size());
    for (std::int32_t i = 0; i < count; ++i) {
        SAnimation* animation = animationAt(tables(), i);
        const SChannel* channel = animation->channels;
        // Host-found (guard-page run of the new-game path): the bind URI was compared before the channel type was looked at, so a channel of another
        // type, whose 'extra' word is not a string pointer (0xff here), was passed to _stricmp. The type is tested first, as the && chain reads.
        if (channel->type != static_cast<std::uint32_t>(type)) {
            continue;
        }
#ifdef _WIN32
        const bool uriMatches = _stricmp(channel->extra, uri) == 0;
#else
        const bool uriMatches = strcasecmp(channel->extra, uri) == 0;
#endif
        if (uriMatches && std::strcmp(channel->name, name) == 0) {
            return animation;
        }
    }
    return nullptr;
}

}  // namespace glitch::collada
