// glitch::video::CMaterialRendererManager. The manager IS the SIDedCollection<CMaterialRenderer> of the
// original (manager +0x00..+0x27, ctor base 0x5d7db0 / 0x5d7de4), so the port derives from the shared
// glitch::core::detail::SIDedCollection instance instead of standing in for it: the entry vector at
// +0x18..+0x20 is the collection's entries, and the two lookups below are the original's own get(id).
// Still not modelled: the technique, render-pass, name-map and parameter tables (the SSharedString
// red-black trees), which the rest of the class needs.
//
// The two lookups that were the batch:
//   0x5dd0e4 getMaterialInstance(uint16_t, bool)   the cached CMaterial of a renderer, or a new one
//   0x5d9af0 createMaterialInstance(E_MATERIAL_TYPE)  createMaterialRenderer, then allocate the material
//   0x5d7d40 SIDedCollection<CMaterialRenderer>::get, the inline lookup the callers write out by hand
//
// Layout decision for the renderer id (open question 1 of NOTES.md "dep-material-renderer-manager layout"):
//   The binary keeps the id at node +0x22 (getId 0x5d98c4 reads +34, createMR 0x5d8b28 writes +32). The
//   shared template puts it at SIdValue +0x1c (node +0x34) instead, because its SIdValue's id follows the
//   Props record at that record's size. For this collection the template's own layout wins over the
//   evidence: the port keeps the shared template unchanged and leaves the id at its template offset. That is
//   safe because NOTHING the port reconstructs here reads the id back: get(id) (both lookups below and
//   0x5d7d40 itself) indexes the entry vector and never touches the mapped record's id, and neither insert
//   (0x5dc548) nor getId (0x5d98c4) nor rename is reconstructed yet. Whichever worker reconstructs
//   insert/getId must therefore move the id to node +0x22 in the template (or give this collection its own
//   SIdValue); doing it here would change every other SIDedCollection instance.
#pragma once

#include <cstdint>

#include "glitch-core-io/core/sided_collection.hpp"
#include "glitch-shared/reference_counted.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/material_renderer.hpp"
#include "glitch-video/tb/renderpass_srenderstate.hpp"

namespace glitch::video {

class IVideoDriver;
class CMaterial;
class CMaterialRendererManager;
enum E_MATERIAL_TYPE : int;

namespace detail::materialrenderermanager {
// Props record of the renderer collection (SIdValue derives from it; +0x00 of the node's mapped record).
// Evidence: the SIdValue ctor 0x5d7e34 writes +0x00 = 0, +0x04 = -1 and +0x08 = 17, and getMaterialInstance
// 0x5dd0e4 reads node +0x18 (this +0x00) and createMaterialRenderer 0x5d8b28 writes node +0x20 (this +0x08).
struct SProperties {
    boost::intrusive_ptr<CMaterial> material;  // +0x00 (node +0x18): the cached material instance, null when none
    uint32_t unk04 = 0xffffffffu;              // +0x04 (node +0x1c): -1 after construction, meaning not recovered
    uint16_t materialType = 17;                // +0x08 (node +0x20): E_MATERIAL_TYPE index of the renderer

    // 0x5d7c44 _ZN6glitch5video6detail23materialrenderermanager11SProperties8onRemoveEPvt
    // The hook the collection's remove (0x5d9f74) runs before it erases the entry: this record's material type
    // decides which slot of the manager's renderer-id table is reset to 0xffff, and only types inside the table's
    // 17 entries do anything (the listing is LDRH R3,[R0,#8]; CMP R3,#0x10; ADDLS R3,R1,R3,LSL#1;
    // MOVLS R2,#0xFFFFFFFF; STRHLS R2,[R3,#0x2C]). `owner` is the CMaterialRendererManager and `id`, the entry
    // being removed, is not read. The parameters are void*/uint16_t because that is the original's mangled
    // signature (onRemove(void*, unsigned short)).
    void onRemove(void* owner, uint16_t id);
};
}  // namespace detail::materialrenderermanager

// The manager's collection instance: the CMaterialRenderer entry vector, stride 8 (value +0x00, name-map node
// iterator +0x04). 0x5d7db0 / 0x5d7de4 are the collection constructors of this instance, 0x5d9ab0 its destructor.
using CMaterialRendererCollection = core::detail::SIDedCollection<boost::intrusive_ptr<CMaterialRenderer>, uint16_t, false,
                                                                   detail::materialrenderermanager::SProperties,
                                                                   core::detail::sidedcollection::SValueTraits>;

class CMaterialRendererManager : public CMaterialRendererCollection {
public:
    // Temporary id, passed by value as one word in the original.
    struct STemporaryID {
        uint32_t value = 0;
    };

    // The state of the technique being defined (SCreationState in the original). Its list heads
    // (+0x08..+0x34) are not modelled; only the name and the in-progress word are kept.
    class SCreationState {
    public:
        // One pink-bind record (the list node payload at node +0x08 in the original).
        struct SPinkBind {
            STemporaryID id;     // +0x08 (node +0x08)
            uint8_t flag = 0;    // +0x0c (node +0x0c), counter byte minus one
            uint16_t value = 0;  // +0x0e (node +0x0e), the ushort argument
        };
        // List node: next at +0x00, prev at +0x04, record at +0x08 (16 bytes in this port).
        struct SPinkBindNode {
            SPinkBindNode* next = nullptr;  // +0x00
            SPinkBindNode* prev = nullptr;  // +0x04
            SPinkBind data;                 // +0x08
        };

        // 0x5d7c5c. Partial: the constructor sets the name and the word at +0x04, links the pink sentinel at
        // +0x28/+0x2c and the pending-pass sentinel at +0x10/+0x14 to themselves (with the two cursors at +0x30
        // and +0x18) and stores the pink counter 0 at +0x24. Its remaining stores belong to list heads this
        // class does not model yet (+0x08/+0x0c, +0x1c/+0x20, +0x34..+0x40 and the zero words +0x44..+0x54).
        explicit SCreationState(char* name);
        ~SCreationState();
        SCreationState(const SCreationState&) = delete;
        SCreationState& operator=(const SCreationState&) = delete;

        // 0x5d9788. Adds a pink bind. Fills the node under the cursor when one exists, otherwise
        // appends a new node at the tail and moves the cursor to the end sentinel.
        void addPinkBind(STemporaryID id, uint16_t value);

        // One pending render pass: the 52-byte SRenderPass record of the original (see
        // material_renderer_manager_layout.hpp, "SRenderPass").
        struct SRenderPass {
            uint32_t stateWords[8] = {};                  // +0x00..+0x1f, the 8 words copied from the state argument
                                                          // (the renderpass::SRenderState block, see tb/renderpass_srenderstate.hpp)
            glitch::IReferenceCounted* shader = nullptr;  // +0x20, counted
            int* bindings = nullptr;                      // +0x24, 8 bytes per entry, process-buffer block
            uint32_t unknown28 = 0;                       // +0x28, meaning unknown
            uint16_t unknown2c = 0;                       // +0x2c, meaning unknown
            uint16_t unknown2e = 0;                       // +0x2e, meaning unknown
            uint8_t flag30 = 1;                           // +0x30, one when written by addRenderPass
        };
        // Pending pass list node: next at +0x00, prev at +0x04, record at +0x08.
        struct SPassNode {
            SPassNode* next = nullptr;  // +0x00
            SPassNode* prev = nullptr;  // +0x04
            SRenderPass pass;           // +0x08
        };

        // 0x5d93e8. Partial: see the notes in glitch-video/src/tb_video-glitch_video_CMaterialRendererManager_SCreationS-0.cpp
        // (the shader binding count and the list sentinel setup are not reconstructed yet).
        void addRenderPass(glitch::IReferenceCounted* shader, const detail::renderpass::SRenderState& state);

        char* m_name = nullptr;      // +0x00
        uint32_t m_inProgress04 = 0; // +0x04, non-zero while a technique is open (isCreatingTechnique)
        // +0x24 byte, the pink counter. The record flag is this counter minus one. Other fields
        // of the original (+0x08..+0x23) are not modelled.
        uint8_t m_pinkCounter24 = 0;
        // +0x28 (next) and +0x2c (prev): the sentinel of the pink-bind list. The sentinel is the end
        // of the list. Initially both point at the sentinel itself.
        SPinkBindNode m_pinkSentinel;
        // +0x30: the cursor used by addPinkBind (points at a node or at the sentinel).
        SPinkBindNode* m_pinkCursor = nullptr;
        // +0x10 (next) and +0x14 (prev): sentinel of the pending pass list, linked to itself by the
        // constructor (0x5d7c5c writes +0x10 and +0x14 = &sentinel).
        SPassNode m_passSentinel;
        // +0x18: pass-list cursor (node reused by the next addRenderPass, or the last node written).
        SPassNode* m_passCursor = nullptr;
        // +0x54: running total of 8-byte binding entries of the open technique (addRenderPass adds to it).
        uint32_t m_bindingTotal = 0;
    };

    // 0x5d8f24 (C1) / 0x5d8fa0 (C2). Partial: the SIDedCollection base and the -1 fill of the
    // +0x2c table are not modelled.
    explicit CMaterialRendererManager(IVideoDriver* driver);

    // 0x5d7cf4: returns the id when a creation state is open and the id is not zero, otherwise 0.
    int32_t getTechnique(STemporaryID id) const;
    // 0x5d7d14: the same test and result as getTechnique.
    int32_t getParameterDef(STemporaryID id) const;

    // 0x5db15c. Partial: deletes the creation state; the process-buffer release is a plain delete.
    void clearCreationState();
    // 0x5dc058. True while a technique is open with its in-progress word set; with no open technique and a
    // non-null name the name is logged at level 3 and false is returned.
    bool isCreatingTechnique(const char* name) const;

    // 0x5d8b28: the id of the renderer of a built-in type (created on first use). Still declared only (needs the
    // video-side getStringsInternal, constructEffect and the DefaultEffects database); another worker owns its row.
    uint16_t createMaterialRenderer(E_MATERIAL_TYPE type);
    // 0x5d9af0 _ZN6glitch5video24CMaterialRendererManager22createMaterialInstanceENS0_15E_MATERIAL_TYPEE
    // Creates the renderer of that type when there is none yet, then allocates one material on it with a null
    // name and technique 0 (CMaterial::allocate 0x5cc0a0, which takes the renderer's default values). Null when
    // the entry of the resulting id is unset, i.e. for an id past the entry vector.
    boost::intrusive_ptr<CMaterial> createMaterialInstance(E_MATERIAL_TYPE type);
    // 0x5dd0e4 _ZN6glitch5video24CMaterialRendererManager19getMaterialInstanceEtb (ObjectManager::Draw3D passes
    // the renderer id and true). The renderer entry's cached material, allocated and cached on the first call;
    // `flag` selects CMaterial::reset(nullptr) on the hit path. Null for id 0xffff or an id past the vector.
    boost::intrusive_ptr<CMaterial> getMaterialInstance(uint16_t id, bool flag);
    // Port spelling of the inline lookup the original inlines at every call site (0x5d7d40 is the collection's
    // get, the out-of-line copy of it): entry value of the eight-byte entry at +0x18 + 8*id, null for an id past
    // the end (the collection's shared Invalid entry holds a null value). Callers keep this signature.
    boost::intrusive_ptr<CMaterialRenderer> getMaterialRenderer(uint16_t id) const;

    // The host object's own layout is the port's (base first, then these members); the offsets are the
    // ORIGINAL object's, kept for reference only and never used for host pointer arithmetic.
    IVideoDriver* m_driver = nullptr;               // +0x28
    SCreationState* m_creationState = nullptr;      // +0x90, open technique state (null when none)
    // +0x2c of the original: one uint16 renderer id per E_MATERIAL_TYPE, 17 entries, 0xffff when the type has no
    // renderer yet. The constructor fills all 17 with 0xffff (LC_API_MEMSET_0(this + 44, -1, 0x22) at 0x5d8f24),
    // createMaterialRenderer 0x5d8b28 writes entry type, and SProperties::onRemove 0x5d7c44 resets it.
    uint16_t m_materialTypeIds[17] = {};
};

}  // namespace glitch::video
