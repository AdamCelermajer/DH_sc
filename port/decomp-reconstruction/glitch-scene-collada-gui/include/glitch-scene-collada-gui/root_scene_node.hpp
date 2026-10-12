// glitch::collada::CRootSceneNode: root of an instantiated COLLADA scene.
// It keeps several intrusive circular lists (particle systems, lights,
// morphing meshes, cameras) and a material list. Each list is a sentinel
// head followed by nodes of {next, prev, value}, matching the original
// three-word link records allocated with GlitchAlloc(12).
#pragma once

#include <cstdint>

namespace glitch::collada {

// Three-word link record used by the original intrusive lists
// (the original allocates 12 bytes per node).
template <typename T>
struct SIntrusiveLink {
    SIntrusiveLink* next = nullptr;  // +0x00
    SIntrusiveLink* prev = nullptr;  // +0x04
    T* value = nullptr;              // +0x08
};

// Circular list with a sentinel head. The original appends at the tail:
// the new node's next is the head, its prev is the old tail.
template <typename T>
struct SIntrusiveList {
    SIntrusiveLink<T> head;  // head.next = first, head.prev = last

    SIntrusiveList() { head.next = &head; head.prev = &head; }
    SIntrusiveList(const SIntrusiveList&) = delete;
    SIntrusiveList& operator=(const SIntrusiveList&) = delete;
    ~SIntrusiveList() {
        SIntrusiveLink<T>* n = head.next;
        while (n != &head) {
            SIntrusiveLink<T>* next = n->next;
            delete n;
            n = next;
        }
    }

    void pushBack(T* value) {
        auto* node = new SIntrusiveLink<T>();
        node->value = value;
        node->next = &head;
        node->prev = head.prev;
        head.prev->next = node;
        head.prev = node;
    }

    std::uint32_t count() const {
        std::uint32_t n = 0;
        for (const SIntrusiveLink<T>* p = head.next; p != &head; p = p->next) ++n;
        return n;
    }
};

struct CColladaUIDHolder {
    std::uint8_t pad_0000[0x20];
    std::uint32_t uid;  // +0x20
};

class CRootSceneNode {
public:
    // Original member offsets. Each SIntrusiveList here stands in for the
    // sentinel pair the original keeps at the listed offset.
    CColladaUIDHolder* uidOwner = nullptr;        // +0x14c
    SIntrusiveList<void> particleSystems;         // +0x158
    SIntrusiveList<void> morphingMeshes;          // +0x168
    SIntrusiveList<void> cameras;                 // +0x170
    SIntrusiveList<void> materials;               // +0x178
    SIntrusiveList<void> lights;                  // +0x188

    std::uint32_t getUID() const;                 // 0x65adc4
    void removeMaterial(const void* material);    // 0x65add4 (empty body)
    std::uint32_t getMaterialCount() const;       // 0x65ae38
    std::uint32_t getType() const;                // 0x65ae64
    void addParticleSystem(void* particleSystem); // 0x65b210
    void addLight(void* light);                   // 0x65b244
    void addMorphingMesh(void* morphingMesh);     // 0x65b278
    void addCamera(void* camera);                 // 0x65b2ac
};

}  // namespace glitch::collada
