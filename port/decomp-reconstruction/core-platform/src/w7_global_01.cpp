#include "core-platform/w7_global_01.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "core-platform/memory.hpp"

namespace {

// Layout facts read from the 32-bit ARM listing. A node's child list sentinel is
// at byte 244, and each child carries its list link at byte 4. Valid only for
// that 4-byte-pointer layout; the Windows port needs the real ISceneNode layout.
constexpr std::size_t kChildListOffset = 244;
constexpr std::size_t kChildLinkOffset = 4;
// Byte offset 96 in the ARM vtable, which uses 4-byte entries: index 24.
constexpr std::size_t kSlot96Index = 96 / 4;
// Byte offset 284 of the node, holding the flag word that carries the 0x200 bit.
constexpr std::size_t kFlagsOffset = 284;
constexpr std::uint32_t kOnAnimateFlag = 0x200;

// Irrlicht-style assertion path shared by the glitch helpers. Level 2 aborts (the
// original writes to address 0), level 1 prints the message to stderr.
void ReportAssert(const char* expr, const char* file, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, file, line);
    }
}

constexpr const char kIrrutilsFile[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Data\\Collada\\irrutils.cpp";

struct ListLink {
    ListLink* next;  // word 0 of a link node
};

ListLink* ChildListHead(glitch::scene::ISceneNode* node) {
    return reinterpret_cast<ListLink*>(reinterpret_cast<char*>(node) + kChildListOffset);
}

glitch::scene::ISceneNode* ChildOf(ListLink* link) {
    if (link == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<glitch::scene::ISceneNode*>(reinterpret_cast<char*>(link) -
                                                         kChildLinkOffset);
}

// Calls the virtual at vtable slot 96 with (node, child). The method name is not recovered.
void CallSlot96(glitch::scene::ISceneNode* node, glitch::scene::ISceneNode* child) {
    using Fn = void (*)(glitch::scene::ISceneNode*, glitch::scene::ISceneNode*);
    void** vtable = *reinterpret_cast<void***>(node);
    reinterpret_cast<Fn>(vtable[kSlot96Index])(node, child);
}

}  // namespace

// 0x50e3d8 _Z14RecursiveCountPN6glitch5scene10ISceneNodeE
// Partial: the traversal matches the listing, but the child-list offsets (244, 4)
// are the 32-bit ARM layout.
int RecursiveCount(glitch::scene::ISceneNode* node) {
    ListLink* head = ChildListHead(node);
    ListLink* link = head->next;
    if (link == head) {
        return 1;
    }
    int count = 1;
    do {
        glitch::scene::ISceneNode* child = ChildOf(link);
        link = link->next;
        count += RecursiveCount(child);
    } while (link != head);
    return count;
}

// 0x50e46c _Z19setOnAnimateEnabledPN6glitch5scene10ISceneNodeEb
// Partial: the flag bit at byte 284 is the 32-bit ARM layout. The pseudocode
// returns the node argument; that return is treated as register residue, so this is void.
void setOnAnimateEnabled(glitch::scene::ISceneNode* node, bool enable) {
    auto* flags = reinterpret_cast<std::uint32_t*>(reinterpret_cast<char*>(node) + kFlagsOffset);
    if (enable) {
        *flags |= kOnAnimateFlag;
    } else {
        *flags &= ~kOnAnimateFlag;
    }
}

// 0x50e484 _Z22RecursiveSetBoolOnNodePN6glitch5scene10ISceneNodeEbPFvS2_bE
// Partial: the pseudocode calls the callback with no visible arguments. The
// callback is invoked as callback(node, enable) per the mangled signature. The
// child-list offsets are the 32-bit ARM layout.
void RecursiveSetBoolOnNode(glitch::scene::ISceneNode* node, bool enable,
                            void (*callback)(glitch::scene::ISceneNode*, bool)) {
    if (node == nullptr) {
        return;
    }
    callback(node, enable);
    ListLink* head = ChildListHead(node);
    for (ListLink* link = head->next; link != head;) {
        glitch::scene::ISceneNode* child = ChildOf(link);
        link = link->next;
        RecursiveSetBoolOnNode(child, enable, callback);
    }
}

// 0x50e4d0 _Z21NodeRemoveAllChildrenPN6glitch5scene10ISceneNodeE
// Partial: the vtable slot 96 (0x60) is not recovered as a removal. The ISceneNode
// vtable listing (decomp_vtable.py) names 0x60 as getUID() const, which takes no
// child argument, and removeChild is at 0x6c, so the call's dynamic type is needed
// to name it. The child-list offsets are ARM32 layout. The pseudocode returns the
// last call's result; this is void.
void NodeRemoveAllChildren(glitch::scene::ISceneNode* node) {
    ListLink* head = ChildListHead(node);
    for (ListLink* link = head->next; link != head;) {
        CallSlot96(node, ChildOf(link));
        link = link->next;
    }
}

// 0x50eb90 _Z15ChangeMaterialsPN6glitch5scene10ISceneNodeENS_5video15E_MATERIAL_TYPEE
// The listing contains only the ASSERT(false) path at irrutils.cpp:838. The
// symbol size (503) is larger than the function IDA delimits, so the material
// swap body is not present in this function.
void ChangeMaterials(glitch::scene::ISceneNode* /*node*/, glitch::video::E_MATERIAL_TYPE /*type*/) {
    ReportAssert("false", kIrrutilsFile, 838);
}

// 0x50ec18 _Z15ChangeMaterialsPN6glitch5scene10ISceneNodeENS_5video15E_MATERIAL_TYPEES4_
// Same as the one-material overload: the listing contains only the ASSERT(false)
// path, at irrutils.cpp:805.
void ChangeMaterials(glitch::scene::ISceneNode* /*node*/, glitch::video::E_MATERIAL_TYPE /*from*/,
                     glitch::video::E_MATERIAL_TYPE /*to*/) {
    ReportAssert("false", kIrrutilsFile, 805);
}

// 0x50eca0 _Z25DBG_TraverseAllGlitchNodePN6glitch5scene10ISceneNodeEPFvS2_EPFbS2_E
// Partial: the child-list offsets (244, 4) are the 32-bit ARM layout. The
// assertion path matches the listing at irrutils.cpp:656.
void DBG_TraverseAllGlitchNode(glitch::scene::ISceneNode* node,
                               void (*visit)(glitch::scene::ISceneNode*),
                               bool (*filter)(glitch::scene::ISceneNode*)) {
    if (visit == nullptr) {
        ReportAssert("callbackFunctor != 0", kIrrutilsFile, 656);
    }
    if (node == nullptr) {
        return;
    }
    if (filter == nullptr || filter(node) != false) {
        visit(node);
    }
    ListLink* head = ChildListHead(node);
    for (ListLink* link = head->next; link != head;) {
        glitch::scene::ISceneNode* child = ChildOf(link);
        link = link->next;
        DBG_TraverseAllGlitchNode(child, visit, filter);
    }
}
