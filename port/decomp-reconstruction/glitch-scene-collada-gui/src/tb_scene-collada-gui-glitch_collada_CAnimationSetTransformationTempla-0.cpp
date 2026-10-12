// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CAnimationSetTransformationTempla-0.
// Owner: glitch::collada::CAnimationSetTransformationTemplate (declared in
// glitch-scene-collada-gui/collada_misc.hpp, which also holds the SAnimationTemplate target record).
//
// Reconstructed here: the three target pushes of both addTransformationTargets overloads.
// Partial: isAnimationExist, getDefaultValue, and the child walks of both addTransformationTargets
// overloads. Their missing inputs are named next to each body.

#include "glitch-scene-collada-gui/collada_misc.hpp"

namespace glitch::collada {

namespace {

// Target kind accepted for a channel type index. Bits 10..13 of the original mask accept target
// type 10, bits 5..9 accept 5, bits 1..4 accept 1; channel types above 13 accept nothing. The
// result is 0 when no target kind is accepted. Used by the isAnimationExist match once SChannel
// is mapped.
[[maybe_unused]] std::uint32_t acceptedTargetType(std::uint32_t channelType) {
    if (channelType > 0xD) {
        return 0;
    }
    const std::uint32_t bit = 1u << channelType;
    if ((bit & 0x3C00u) != 0) {
        return 10;
    }
    if ((bit & 0x3E0u) != 0) {
        return 5;
    }
    if ((bit & 0x1Eu) != 0) {
        return 1;
    }
    return 0;
}

// Scene-node name getter (vtable slot +0x54 on the target node). Not mapped in this category,
// so no name is returned yet and no target is matched.
[[maybe_unused]] const char* targetNodeName(const SAnimationTemplate& /*target*/) {
    return nullptr;
}

// Default-value lookup for one target node: CColladaDatabase::getDefaultValue(SChannel const*,
// void**) on the database embedded at node +0x14c. Neither the overload nor the node field is
// mapped, so no default is found yet.
bool nodeDefaultValue(const CSceneNode* /*node*/, const SChannel* /*channel*/, void** /*out*/) {
    return false;
}

// Allocates one 16-byte target record (the original uses operator new(0x10) and writes the flag
// byte, the type word and the node word).
SAnimationTemplate* newTarget(std::uint32_t type, CSceneNode* node) {
    auto* target = new SAnimationTemplate;
    target->flag = 0;
    target->type = type;
    target->node = node;
    return target;
}

}  // namespace

// 0x6e22cc glitch::collada::CAnimationSetTransformationTemplate::isAnimationExist(glitch::collada::SChannel const*)
// Partial: the original walks the targets. For each one it compares the scene node's name with
// the channel name (SChannel +0x04, strcmp equality), then tests the channel type index (+0x08)
// against the target type with acceptedTargetType. On a match it sets the target flag to 1 and
// returns 1; otherwise it returns 0. SChannel has no mapped layout in this category, and the name
// getter (targetNodeName) is not mapped, so every target is treated as unmatched.
int CAnimationSetTransformationTemplate::isAnimationExist(const SChannel* channel) {
    (void)channel;
    for (SAnimationTemplate* target : targets) {
        const char* name = targetNodeName(*target);
        if (name == nullptr) {
            continue;
        }
        // Unreachable until targetNodeName and the SChannel fields are mapped.
    }
    return 0;
}

// 0x6e23d0 glitch::collada::CAnimationSetTransformationTemplate::getDefaultValue(glitch::collada::SChannel const*, void**) const
// Reconstructed control flow: an empty target list returns 0; otherwise each target's node is
// asked in turn, and the first success returns 1. Partial: the per-node lookup (nodeDefaultValue)
// is not mapped, so the result is always 0.
int CAnimationSetTransformationTemplate::getDefaultValue(const SChannel* channel, void** out) const {
    for (const SAnimationTemplate* target : targets) {
        if (nodeDefaultValue(target->node, channel, out)) {
            return 1;
        }
    }
    return 0;
}

// 0x6e24e8 glitch::collada::CAnimationSetTransformationTemplate::addTransformationTargets(glitch::collada::CSceneNode*)
// Pushes three targets for this node, of types 1, 5 and 10, in that order. Partial: the original
// then walks the node's intrusive child list (sentinel at node +0xf4; each child pointer is the
// link word minus 4) and calls this overload for every child. The +0xf4 list is not mapped
// (layouts/scene.hpp leaves it unconfirmed), so the children are not visited.
void CAnimationSetTransformationTemplate::addTransformationTargets(CSceneNode* node) {
    targets.push_back(newTarget(1, node));
    targets.push_back(newTarget(5, node));
    targets.push_back(newTarget(10, node));
    // Child walk over node +0xf4: not reconstructed (see above).
}

// 0x6e26d4 glitch::collada::CAnimationSetTransformationTemplate::addTransformationTargets(glitch::collada::SNode&)
// Pushes three targets of types 1, 5 and 10. The original never stores a node word for these
// entries (operator new does not zero it), so the node is null here. Partial: the original then
// calls this overload for each of the SNode's children (count at +0x38, child base at +0x3c, stride
// 0x50). The SNode loader struct is not mapped, so the child loop is not reconstructed.
void CAnimationSetTransformationTemplate::addTransformationTargets(SNode& node) {
    (void)node;
    targets.push_back(newTarget(1, nullptr));
    targets.push_back(newTarget(5, nullptr));
    targets.push_back(newTarget(10, nullptr));
    // Child loop over SNode +0x38 / +0x3c: not reconstructed (see above).
}

}  // namespace glitch::collada
