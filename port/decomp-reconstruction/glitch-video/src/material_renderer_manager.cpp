#include "glitch-video/material_renderer_manager.hpp"

#include <cstring>

#include "glitch-video/material.hpp"

namespace glitch::video {

namespace {
// The SEntry the two lookups share: the entry at the id, or the collection's shared Invalid entry when the id
// is past the end (get(id), 0x5d7d40 / the inline copy). The value of the Invalid entry is null, so reading it
// is the same observable result as the original's "null for an id past the end". A null pointer is never
// returned by the template's get(id); the check is kept so the helpers are safe to read.
template <class Manager>
auto* rendererEntry(Manager& manager, uint16_t id) {
    auto* entry = manager.get(id);
    return (entry != nullptr && entry->value) ? entry : nullptr;
}
}  // namespace

// Port spelling of the collection's out-of-line get (0x5d7d40, the copy of the lookup the callers inline).
// id >= (end - begin) >> 3 gives the collection's shared Invalid entry, whose value is null; the original
// hands that entry out and the callers read its (null) value, which is the same observable result as the
// port's null result. The port reads the value through the shared template's get(id) (see the header for
// where this differs from the binary and why the difference cannot be observed here).
boost::intrusive_ptr<CMaterialRenderer> CMaterialRendererManager::getMaterialRenderer(uint16_t id) const {
    const SEntry* entry = rendererEntry(*this, id);
    if (entry == nullptr) {
        return boost::intrusive_ptr<CMaterialRenderer>();
    }
    return entry->value;
}

// 0x5d7c5c _ZN6glitch5video24CMaterialRendererManager14SCreationStateC1EPc
// Partial: the list heads point at the sentinel (set here as in the original). The other list
// heads at +0x08..+0x23 (technique and parameter lists) are not modelled.
CMaterialRendererManager::SCreationState::SCreationState(char* name) : m_name(name) {
    m_pinkSentinel.next = &m_pinkSentinel;
    m_pinkSentinel.prev = &m_pinkSentinel;
    m_pinkCursor = &m_pinkSentinel;
}

// Frees the pink-bind nodes. The original destructor is not reconstructed; this only releases
// the nodes that addPinkBind allocates.
CMaterialRendererManager::SCreationState::~SCreationState() {
    SPinkBindNode* node = m_pinkSentinel.next;
    while (node != &m_pinkSentinel) {
        SPinkBindNode* next = node->next;
        delete node;
        node = next;
    }
}

// 0x5d9788 _ZN6glitch5video24CMaterialRendererManager14SCreationState11addPinkBindENS1_12STemporaryIDEt
void CMaterialRendererManager::SCreationState::addPinkBind(STemporaryID id, uint16_t value) {
    const uint8_t flag = static_cast<uint8_t>(m_pinkCounter24 - 1);
    if (m_pinkCursor == &m_pinkSentinel) {
        // The cursor is at the end: append a node at the tail.
        SPinkBindNode* node = new SPinkBindNode();
        node->data.flag = flag;
        node->data.value = value;
        node->data.id = id;
        SPinkBindNode* tail = m_pinkSentinel.prev;
        node->next = &m_pinkSentinel;
        node->prev = tail;
        tail->next = node;
        m_pinkSentinel.prev = node;
        m_pinkCursor = &m_pinkSentinel;
    } else {
        // Reuse the node under the cursor and advance the cursor.
        SPinkBindNode* node = m_pinkCursor;
        node->data.id = id;
        node->data.value = value;
        node->data.flag = flag;
        m_pinkCursor = node->next;
    }
}

// 0x5d8f24 _ZN6glitch5video24CMaterialRendererManagerC1EPNS0_12IVideoDriverE
// 0x5d8fa0 _ZN6glitch5video24CMaterialRendererManagerC2EPNS0_12IVideoDriverE
// Partial: the SIDedCollection base constructor and the memset of the id table (0xff, 0x22 bytes)
// are not reproduced.
CMaterialRendererManager::CMaterialRendererManager(IVideoDriver* driver) : m_driver(driver) {}

// 0x5d7cf4 _ZNK6glitch5video24CMaterialRendererManager12getTechniqueENS1_12STemporaryIDE
int32_t CMaterialRendererManager::getTechnique(STemporaryID id) const {
    return (m_creationState != nullptr && id.value != 0) ? static_cast<int32_t>(id.value) : 0;
}

// 0x5d7d14 _ZNK6glitch5video24CMaterialRendererManager15getParameterDefENS1_12STemporaryIDE
int32_t CMaterialRendererManager::getParameterDef(STemporaryID id) const {
    return (m_creationState != nullptr && id.value != 0) ? static_cast<int32_t>(id.value) : 0;
}

// 0x5dd0e4 _ZN6glitch5video24CMaterialRendererManager19getMaterialInstanceEtb
// id 0xffff is not looked up at all, so the result is null. Otherwise the entry of the id is taken and its
// value (the renderer) is used to allocate a material on the miss path: CMaterial::allocate(renderer, nullptr, 0)
// -- a null name and technique 0, i.e. the default-state overload 0x5cc0a0 -- and the result is cached in the
// record the collection maps the id to (entry node +0x18, the +0x00 field of SProperties). On the hit path the
// cached material is returned, and `flag` (the bool of the mangled name) selects CMaterial::reset(nullptr).
// The original reaches the record as entry->iterator->second (node +0x18) read as a CMaterial**;
// getProperties(id) is the template's spelling of exactly that address (0x5d7e20 returns node +24).
boost::intrusive_ptr<CMaterial> CMaterialRendererManager::getMaterialInstance(uint16_t id, bool flag) {
    if (id == 0xffff) {
        return boost::intrusive_ptr<CMaterial>();
    }
    // The entry the cache lives in. For an id past the end this is the collection's Invalid entry, whose value
    // is null: the original then reads the mapped record of that shared entry (getProperties has no range
    // check) and would store the new material in it. The port skips that undefined store and returns null,
    // which is the result the caller sees either way; see the note in the header.
    SEntry* entry = rendererEntry(*this, id);
    if (entry == nullptr) {
        return boost::intrusive_ptr<CMaterial>();
    }
    detail::materialrenderermanager::SProperties* const props = getProperties(id);  // 0x5d7e20: node +0x18, the record itself
    if (props->material) {
        if (flag) {
            props->material->reset(nullptr);  // 0x5cbc3c
        }
        return props->material;
    }
    boost::intrusive_ptr<CMaterial> material =
        CMaterial::allocate(entry->value, nullptr, 0);  // 0x5cc0a0, null name and technique 0
    props->material = material;
    return material;
}

// 0x5d9af0 _ZN6glitch5video24CMaterialRendererManager22createMaterialInstanceENS0_15E_MATERIAL_TYPEE
// The renderer of the type is created on first use (createMaterialRenderer 0x5d8b28, out of scope here), its
// entry is taken (out-of-range ids reach the collection's Invalid entry, whose value is null, as in the
// original), and one material is allocated on it with a null name and technique 0. Unlike getMaterialInstance
// this allocates every time; it does not touch the record's cached material.
boost::intrusive_ptr<CMaterial> CMaterialRendererManager::createMaterialInstance(E_MATERIAL_TYPE type) {
    const uint16_t id = createMaterialRenderer(type);
    // The renderer of the entry: null for an id past the end, which allocate accepts (the original passes the
    // value of the collection's Invalid entry, also null).
    SEntry* entry = rendererEntry(*this, id);
    return CMaterial::allocate(entry != nullptr ? entry->value : boost::intrusive_ptr<CMaterialRenderer>(),
                               nullptr, 0);  // 0x5cc0a0, null name and technique 0
}

// 0x5db15c _ZN6glitch5video24CMaterialRendererManager18clearCreationStateEv
// Partial: the original runs the state destructor and releases the process buffer.
void CMaterialRendererManager::clearCreationState() {
    delete m_creationState;
    m_creationState = nullptr;
}

// 0x5dc058 _ZNK6glitch5video24CMaterialRendererManager19isCreatingTechniqueEPKc
// Partial: when no technique is open and a name is given, the original logs the name at level 3;
// the logger is not ported.
bool CMaterialRendererManager::isCreatingTechnique(const char* /*name*/) const {
    if (m_creationState == nullptr) {
        return false;
    }
    return m_creationState->m_inProgress04 != 0;
}

}  // namespace glitch::video
