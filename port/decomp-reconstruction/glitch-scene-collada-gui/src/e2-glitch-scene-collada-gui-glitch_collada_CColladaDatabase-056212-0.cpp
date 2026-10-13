// Track B batch e2-glitch-scene-collada-gui-glitch_collada_CColladaDatabase-056212-0 (part 1 of 1).
// Owner: glitch::collada::CColladaDatabase. Layouts (SAnimation +0x10, SChannel, SNode, SVisualScene,
// SLight +0x0c) are declared in collada_database.hpp; the member declarations for this batch are there too.
#include <cstring>

#include "glitch-scene-collada-gui/collada_database.hpp"

namespace glitch::collada {

// 0x61ace8 glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, glitch::collada::SInstanceController*, glitch::collada::CRootSceneNode*, bool) const
// PARTIAL: stub. The original (a) builds the named controller through the overload
// constructController(IVideoDriver*, char const*, CRootSceneNode*) (another batch, not declared here);
// (b) when that object exists, resolves each material binding of the instance (count at +0x0c, bindings at
// +0x10, stride 0x3c) with getMaterial(driver, name, name2) or getMaterial(driver, word +0x08) (neither is
// declared here), creates the material and vertex-attribute map through the factory and the built
// object's vtable (slots +0x20 and +0x40, not named), and (c) runs the vtable slot +0x40 call with the flag.
// None of that is reconstructed, so the out slot is left null.
glitch::IReferenceCounted** CColladaDatabase::constructController(glitch::IReferenceCounted** out,
                                                                  video::IVideoDriver* driver,
                                                                  SInstanceController* instance,
                                                                  CRootSceneNode* root, bool flag) const {
    (void)driver;
    (void)instance;
    (void)root;
    (void)flag;
    *out = nullptr;
    return out;
}

// 0x61aeb8 glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, glitch::collada::SInstanceGeometry*, glitch::collada::CRootSceneNode*) const
// PARTIAL: stub. The original starts by clearing the out slot, as this does. It then builds the geometry
// through one of the named overloads (URL word at instance +0x00 with the "#" skipped, or the
// instance-less path, both in another batch), and for each material binding (count +0x0c, bindings +0x10,
// stride 0x3c) resolves the material and calls the factory's vertex-attribute map builder (factory slot +0x24)
// on the driver's own factory word (driver +0x04, its vtable is not named). None of that is reconstructed.
glitch::IReferenceCounted** CColladaDatabase::constructGeometry(glitch::IReferenceCounted** out,
                                                                video::IVideoDriver* driver,
                                                                SInstanceGeometry* instance,
                                                                CRootSceneNode* root) const {
    (void)driver;
    (void)instance;
    (void)root;
    *out = nullptr;
    return out;
}

// 0x61b2f4 glitch::collada::CColladaDatabase::constructNode(glitch::video::IVideoDriver*, glitch::collada::SNode*, glitch::collada::CRootSceneNode*) const
// PARTIAL: stub. The layout is known (SNode in collada_database.hpp) and the original does the following:
// the node is created by factory slot +0x40 (billboard, when SNode::billboard is set) or +0x3c (plain node);
// each object instance (instanceCount, stride 8) is built by constructCamera / constructLight / constructEmitter
// / constructGNPSEmitter / constructCoronas / constructForce / constructModularSkin / constructGeometry / constructController
// and attached with the node's vtable slot +0x5c; the transform is set through the node's slots +0xa4, +0x9c,
// +0x94 and +0x48, and the flag word +0x34 through slot +0x48 (slot names not mapped); the child nodes
// (childCount, children stride 0x50) are built recursively and attached. Missing: the emitter and GNPS and
// coronas and modular-skin overloads are not declared, the factory slots +0x48..+0x50 and +0x64..+0x68 are not
// named for the object kinds they build, and the node vtable slots are unnamed. Returns null.
void* CColladaDatabase::constructNode(video::IVideoDriver* driver, SNode* node, CRootSceneNode* root) const {
    (void)driver;
    (void)root;
    if (node == nullptr) {
        return nullptr;  // original: if (!a3) return 0
    }
    return nullptr;
}

// 0x61b9e8 glitch::collada::CColladaDatabase::constructScene(glitch::video::IVideoDriver*) const
// PARTIAL: stub. The original returns null when the record is missing (reproduced). Otherwise it creates
// the root with factory slot +0x54 (createScene), scans the scene object at holder +0xb8 (count at +0x00 of
// the scene object, base at +0x04 of 8-byte instances, kind 6 = visual scene) and calls constructVisualScene
// for each such entry, then CRootSceneNode::onPostLoad and resolveURLs on the root. The scene object is not
// modelled in SDatabaseTables (see getScene), and resolveURLs is not declared, so the root is not built.
CRootSceneNode* CColladaDatabase::constructScene(video::IVideoDriver* driver) const {
    (void)driver;
    if (record == nullptr) {
        return nullptr;  // original: if (!*this) return nullptr
    }
    return nullptr;
}

// 0x61c0c8 glitch::collada::CColladaDatabase::getAnimation(char const*, glitch::collada::SChannel::Type, unsigned char) const
// Walks the animation table (count = tables().animations.size()). Each animation's channel record (SAnimation
// +0x10) is tested by kind: kind 9 and kind 5 accept channel types 5 and 9 by name; kinds 87..91 (0x57..0x5b,
// and 256) accept channel types 87..91 by name; kinds 1..4 accept channel types 1..4 by name; kind 0 and kinds
// 6..8 and the other kinds above 9 need the channel type and flag to equal the arguments and then compare
// the name. The name test is strcmp on the channel's name (+0x04). The original dereferences the channel
// unconditionally; a null channel is skipped here.
SAnimation* CColladaDatabase::getAnimation(const char* name, SChannel::Type type, std::uint8_t flag) const {
    const std::uint32_t kind = static_cast<std::uint32_t>(type);
    const std::uint32_t count = static_cast<std::uint32_t>(tables().animations.size());
    for (std::uint32_t i = 0; i < count; ++i) {
        SAnimation* animation = animationAt(tables(), static_cast<int>(i));
        const SChannel* channel = animation->channels;
        if (channel == nullptr || name == nullptr || channel->name == nullptr) {
            continue;
        }
        const std::uint32_t channelType = channel->type;
        const bool sameKindAndFlag = channelType == kind && channel->flag == flag;
        bool kindMatches;
        if (kind == 9) {
            kindMatches = channelType == 5 || channelType == 9;
        } else if (kind > 9) {
            if (kind >= 0x57 && (kind <= 0x5b || kind == 256)) {
                kindMatches = channelType - 87u <= 4u;
            } else {
                kindMatches = sameKindAndFlag;
            }
        } else if (kind == 0) {
            kindMatches = sameKindAndFlag;
        } else if (kind <= 4) {
            kindMatches = channelType - 1u <= 3u;
        } else if (kind == 5) {
            kindMatches = channelType == 5 || channelType == 9;
        } else {
            kindMatches = sameKindAndFlag;  // kinds 6..8
        }
        if (kindMatches && std::strcmp(channel->name, name) == 0) {
            return animation;
        }
    }
    return nullptr;
}

// 0x61c1e0 glitch::collada::CColladaDatabase::getBlendableAnimation(glitch::collada::SChannel const*) const
// Null channel gives null. Otherwise the lookup uses the channel's name (+0x04), type (+0x08) and flag (+0x0c).
SAnimation* CColladaDatabase::getBlendableAnimation(const SChannel* channel) const {
    if (channel == nullptr) {
        return nullptr;
    }
    return getAnimation(channel->name, static_cast<SChannel::Type>(channel->type), channel->flag);
}

// 0x61c214 glitch::collada::CColladaDatabase::getNode(char const*, glitch::collada::SNode&) const
// The node itself is returned when its name (+0x00) equals the lookup name. Otherwise its children (count
// +0x38, base +0x3c, stride 0x50) are searched in order, and the first match is returned. A node with a
// null name never matches here; the original would call strcmp on it.
SNode* CColladaDatabase::getNode(const char* name, SNode& node) const {
    if (node.name != nullptr && std::strcmp(node.name, name) == 0) {
        return &node;
    }
    for (std::uint32_t i = 0; i < node.childCount; ++i) {
        if (SNode* found = getNode(name, node.children[i])) {
            return found;
        }
    }
    return nullptr;
}

// 0x61c290 glitch::collada::CColladaDatabase::getNode(char const*) const
// Searches the top-level nodes of visual scene 0 (the index getter visualSceneAt returns null when there
// are no visual scenes). The nodes are searched in order with getNode(name, node).
SNode* CColladaDatabase::getNode(const char* name) const {
    const SVisualScene* scene = visualSceneAt(tables(), 0);
    if (scene == nullptr) {
        return nullptr;
    }
    for (std::uint32_t i = 0; i < scene->nodeCount; ++i) {
        if (SNode* found = getNode(name, scene->nodes[i])) {
            return found;
        }
    }
    return nullptr;
}

// 0x61c2f8 glitch::collada::CColladaDatabase::getDefaultValue(char const*, glitch::collada::SChannel::Type, void**, void*) const
// Every path that does not produce a value stores null into *out and returns false, as the original's
// LABEL_3 does. The value is the address of a node field, looked up by name:
//   case 1..4   node translation (+0x0c)
//   case 5..9   node rotation (+0x18)
//   case 10, 11 node scale (+0x28)
//   case 12     scale word 1 (+0x2c)
//   case 13     scale word 2 (+0x30)
//   case 16     light block (SLight +0x0c)
//   case 20     node bool word (+0x34)
// Kinds 0, 15, 17..26, 71..85 and 87..91 and the default give null. PARTIAL:
//   - case 14 (controller value): only the controller lookup is reproduced. The value is controller +0x08,
//     then its array at +0x1c indexed by *flag. Neither the controller record nor the array is mapped, so
//     this path returns null even when the controller exists.
//   - case 86 (material parameters): the original logs "TODO - SetParam Default Values" for each parameter
//     whose key matches *flag, and logs an error when the material is missing. The log calls are not
//     reproduced. The return value (always null and false) is reproduced.
bool CColladaDatabase::getDefaultValue(const char* name, SChannel::Type type, void** out, void* flag) const {
    const std::uint32_t kind = static_cast<std::uint32_t>(type);
    const auto fail = [out]() {
        *out = nullptr;
        return false;
    };
    switch (kind) {
        case 0:
            // The original looks the node up here and discards it; the lookup has no side effect.
            return fail();
        case 1:
        case 2:
        case 3:
        case 4: {
            SNode* node = getNode(name);
            if (node == nullptr) {
                return fail();
            }
            *out = static_cast<void*>(node->translation);
            return true;
        }
        case 5:
        case 6:
        case 7:
        case 8:
        case 9: {
            SNode* node = getNode(name);
            if (node == nullptr) {
                return fail();
            }
            *out = static_cast<void*>(node->rotation);
            return true;
        }
        case 10:
        case 11: {
            SNode* node = getNode(name);
            if (node == nullptr) {
                return fail();
            }
            *out = static_cast<void*>(node->scale);
            return true;
        }
        case 12: {
            SNode* node = getNode(name);
            if (node == nullptr) {
                return fail();
            }
            *out = static_cast<void*>(&node->scale[1]);
            return true;
        }
        case 13: {
            SNode* node = getNode(name);
            if (node == nullptr) {
                return fail();
            }
            *out = static_cast<void*>(&node->scale[2]);
            return true;
        }
        case 14: {
            // PARTIAL: see the function comment. The controller value cannot be produced yet.
            (void)getController(tables(), name);
            return fail();
        }
        case 16: {
            SLight* light = getLight(tables(), name);
            if (light == nullptr) {
                return fail();
            }
            *out = static_cast<void*>(light->unk_0c);
            return true;
        }
        case 20: {
            SNode* node = getNode(name);
            if (node == nullptr) {
                return fail();
            }
            *out = static_cast<void*>(&node->unk_34);
            return true;
        }
        case 86: {
            // PARTIAL: the material lookup is reproduced; the parameter logs are not (see the function comment).
            (void)getMaterial(tables(), name);
            return fail();
        }
        default:
            // Kinds 15, 17..26, 71..85 and 87..91 produce no value in the original.
            (void)flag;
            return fail();
    }
}

// 0x61c6bc glitch::collada::CColladaDatabase::getDefaultValue(glitch::collada::SChannel const*, void**) const
// Forwards the channel's name (+0x04), type (+0x08) and, as the flag pointer, its flag byte (+0x0c).
// A null channel gives null (the original would dereference it).
bool CColladaDatabase::getDefaultValue(const SChannel* channel, void** out) const {
    if (channel == nullptr) {
        *out = nullptr;
        return false;
    }
    return getDefaultValue(channel->name, static_cast<SChannel::Type>(channel->type), out,
                           const_cast<std::uint8_t*>(&channel->flag));
}

}  // namespace glitch::collada
