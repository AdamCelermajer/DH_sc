// Slice 25 of the glitch::gui and glitch::scene reconstruction: CGUIContextMenu,
// IGUIElement tab handling, CGUIEditBox, CAppendMeshBuffer, CMeshBuffer, CMesh,
// CDummyTransformationSceneNode, the default scene-node factories and the Collada
// binary loader.
//
// Member offsets come from the layout headers under layouts/ (32-bit ARM). Members the
// layouts do not declare are not invented: those bodies are marked partial in
// ledger_parts/slice_25.csv and call the unattached helpers below.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/layouts/gui.hpp"
#include "glitch-scene-collada-gui/layouts/scene.hpp"

namespace glitch::gui {
class IGUIFont;
class IGUIEnvironment;
}  // namespace glitch::gui

namespace glitch::collada::recon_25 {

// Event words read by CGUIContextMenu::onEvent, sendClick and CGUIEditBox::processMouse.
// glitch::SEvent is not in the layouts, so this stand-in names the offsets that the
// bodies test (+0 type, +8 caller, +12 element, +16 GUI event type, +20 mouse event type).
struct SEventStand {
    std::uint32_t type = 0;               // +0x00 (0 GUI event, 1 mouse event)
    std::uint32_t pad_0x04 = 0;           // +0x04
    union {                               // +0x08: GUI caller, or mouse X for a mouse event
        glitch::gui::IGUIElement* caller;
        std::int32_t mouseX;
    };
    union {                               // +0x0c: GUI element, or mouse Y for a mouse event
        glitch::gui::IGUIElement* element;
        std::int32_t mouseY;
    };
    std::uint32_t guiEventType = 0;       // +0x10
    std::uint32_t mouseEventType = 0;     // +0x14
};

// glitch::gui::CGUIContextMenu::SItem. The layouts keep the item vector as void*
// begin/end words, so the items are read through itemsBegin and itemsEnd below.
// Field names follow the upstream Irrlicht SItem; the offsets are the 32-bit ones.
struct SContextMenuItem {
    std::wstring text;                        // +0x00..+0x47 string storage
    bool isSeparator = false;                 // +0x48 (set to text == nullptr by addItem)
    bool enabled = true;                      // +0x49 (addItem argument)
    bool checked = false;                     // +0x4a (addItem argument)
    bool autoChecking = false;                // +0x4b (not set by addItem)
    std::uint32_t width = 0;                  // +0x4c dimension2d<u32>.Width
    std::uint32_t height = 0;                 // +0x50 dimension2d<u32>.Height
    std::int32_t posY = 0;                    // +0x54
    glitch::gui::CGUIContextMenu* subMenu = nullptr;  // +0x58
    std::int32_t commandId = 0;               // +0x5c
};

// Item vector accessors over the layout's void* words (the items are stored in the
// layout's begin/end pair; no capacity or growth is modelled here).
inline SContextMenuItem* itemsBegin(glitch::gui::CGUIContextMenu* m) {
    return static_cast<SContextMenuItem*>(m->itemsBegin);
}
inline SContextMenuItem* itemsEnd(glitch::gui::CGUIContextMenu* m) {
    return static_cast<SContextMenuItem*>(m->itemsEnd);
}
inline std::size_t itemCount(const glitch::gui::CGUIContextMenu* m) {
    return static_cast<std::size_t>(static_cast<const SContextMenuItem*>(m->itemsEnd) -
                                    static_cast<const SContextMenuItem*>(m->itemsBegin));
}

// Calls through virtual slots or members the layouts do not declare. Each stub returns the
// neutral value so the rest of a body compiles; the rows that use them are marked partial.
namespace unattached {
// IGUIElement vtable slot +0x08 (onEvent), not declared on glitch::gui::IGUIElement.
int onEvent(glitch::gui::IGUIElement* target, const SEventStand& ev);
// glitch::os::Timer::getTime, not declared in any layout.
std::uint32_t timerNow();
// IReferenceCounted::drop on an object whose layout is not declared.
void dropRef(void* object);
}  // namespace unattached

// glitch::gui::IGUIElement
void IGUIElement_setTabOrder(glitch::gui::IGUIElement* self, std::int32_t order);                                  // 0x6b02e8
bool IGUIElement_getNextElement(glitch::gui::IGUIElement* self, bool tabGroup, glitch::gui::IGUIElement*& next,
                                glitch::gui::IGUIElement*& first, bool includeHidden);  // 0x6b0194

// glitch::gui::CGUIContextMenu
void CGUIContextMenu_setVisible(glitch::gui::CGUIContextMenu* self, bool visible);                                  // 0x6adc3c
bool CGUIContextMenu_hasOpenSubMenu(const glitch::gui::CGUIContextMenu* self);                                      // 0x6ad620
void CGUIContextMenu_closeAllSubMenus(glitch::gui::CGUIContextMenu* self);                                          // 0x6ad6c4
void CGUIContextMenu_setItemText(glitch::gui::CGUIContextMenu* self, std::uint32_t index, const wchar_t* text);     // 0x6ad8cc
std::uint32_t CGUIContextMenu_sendClick(glitch::gui::CGUIContextMenu* self, const glitch::core::position2d<std::int32_t>& p);  // 0x6ad02c
void CGUIContextMenu_recalculateSize(glitch::gui::CGUIContextMenu* self);                                           // 0x6ad260
void CGUIContextMenu_setEventParent(glitch::gui::CGUIContextMenu* self, glitch::gui::IGUIElement* parent);                  // 0x6ad58c
bool CGUIContextMenu_highlight(glitch::gui::CGUIContextMenu* self, const glitch::core::position2d<std::int32_t>& p, bool canOpenSubMenu);  // 0x6ad9e8
void CGUIContextMenu_removeAllItems(glitch::gui::CGUIContextMenu* self);                                            // 0x6ae480
void CGUIContextMenu_removeItem(glitch::gui::CGUIContextMenu* self, std::uint32_t index);                           // 0x6ae7bc
std::int32_t CGUIContextMenu_addItem(glitch::gui::CGUIContextMenu* self, const wchar_t* text, std::int32_t commandId,
                                     bool enabled, bool hasSubMenu, bool checked);                          // 0x6aea9c
void CGUIContextMenu_draw(glitch::gui::CGUIContextMenu* self);                                                      // 0x6aec08
int CGUIContextMenu_onEvent(glitch::gui::CGUIContextMenu* self, const SEventStand& ev);                             // 0x6ae1f4
void CGUIContextMenu_dtor_complete(glitch::gui::CGUIContextMenu* self);                                             // 0x6ae858
void IGUIContextMenu_ctor(glitch::gui::IGUIElement* self, glitch::gui::IGUIElement* parent, std::int32_t id, const glitch::core::rect<std::int32_t>& rect);  // 0x6adcf0
void CGUIContextMenu_ctor(glitch::gui::CGUIContextMenu* self, glitch::gui::IGUIElement* parent, std::int32_t id,
                          const glitch::core::rect<std::int32_t>& rect, bool drawBackground, bool moveOnScreen);   // 0x6adfb8
void CGUIContextMenu_ctor_alt(glitch::gui::CGUIContextMenu* self, glitch::gui::IGUIElement* parent, std::int32_t id,
                              const glitch::core::rect<std::int32_t>& rect, bool drawBackground, bool moveOnScreen);  // 0x6ae100

// glitch::gui::CGUIEditBox. The class has no layout in layouts/, so every body is partial.
// Its state is not modelled; the bodies only route the calls that the pseudocode makes.
// glitch::gui::IGUIEditBox base constructor (0x6b0394); base state only, see the source.
void IGUIEditBox_ctor(glitch::gui::IGUIElement* self, glitch::gui::IGUIEnvironment* env, glitch::gui::IGUIElement* parent,
                      std::int32_t id, const glitch::core::rect<std::int32_t>& rect);                          // 0x6b0394
// glitch::gui::CGUIEditBox constructors (0x6b2608 and 0x6b28c8, same demangled name).
void CGUIEditBox_ctor(glitch::gui::IGUIElement* self, const wchar_t* text, bool border, glitch::gui::IGUIEnvironment* env,
                      glitch::gui::IGUIElement* parent, std::int32_t id, const glitch::core::rect<std::int32_t>& rect);  // 0x6b2608
void CGUIEditBox_ctor_alt(glitch::gui::IGUIElement* self, const wchar_t* text, bool border, glitch::gui::IGUIEnvironment* env,
                          glitch::gui::IGUIElement* parent, std::int32_t id, const glitch::core::rect<std::int32_t>& rect);  // 0x6b28c8
// glitch::gui::CGUIEditBox::~CGUIEditBox (complete, 0x6b0bac). Stubbed; see the source.
void CGUIEditBox_dtor_complete(glitch::gui::IGUIElement* self);                                             // 0x6b0bac
void CGUIEditBox_setTextRect(glitch::gui::IGUIElement* self, std::int32_t line);                                    // 0x6afafc
std::int32_t CGUIEditBox_getCursorPos(glitch::gui::IGUIElement* self, std::int32_t x, std::int32_t y);             // 0x6afd18
void CGUIEditBox_getTextDimension(glitch::gui::IGUIElement* self, glitch::core::dimension2d<std::int32_t>& out);            // 0x6afe9c
std::int32_t CGUIEditBox_getLineFromPos(glitch::gui::IGUIElement* self, std::int32_t pos);                          // 0x6affcc
void CGUIEditBox_setPasswordBox(glitch::gui::IGUIElement* self, bool passwordBox, wchar_t passwordChar);            // 0x6b0844
void CGUIEditBox_breakText(glitch::gui::IGUIElement* self);                                                         // 0x6b0f8c
void CGUIEditBox_setText(glitch::gui::IGUIElement* self, const wchar_t* text);                                      // 0x6b15c8
void CGUIEditBox_updateAbsolutePosition(glitch::gui::IGUIElement* self);                                            // 0x6b160c
void CGUIEditBox_setOverrideFont(glitch::gui::IGUIElement* self, glitch::gui::IGUIFont* font);                              // 0x6b162c
void CGUIEditBox_inputChar(glitch::gui::IGUIElement* self, wchar_t ch);                                             // 0x6b1668
void CGUIEditBox_setMax(glitch::gui::IGUIElement* self, std::uint32_t maxChars);                                    // 0x6b185c
void CGUIEditBox_draw(glitch::gui::IGUIElement* self);                                                              // 0x6b18e0
void CGUIEditBox_calculateScrollPos(glitch::gui::IGUIElement* self);                                                // 0x6b246c
bool CGUIEditBox_processMouse(glitch::gui::IGUIElement* self, const SEventStand& ev);                               // 0x6b2b3c
bool CGUIEditBox_processKey(glitch::gui::IGUIElement* self, const SEventStand& ev);                                 // 0x6b2cc0
bool CGUIEditBox_onEvent(glitch::gui::IGUIElement* self, const SEventStand& ev);                                    // 0x6b3d70

// glitch::scene. CAppendMeshBuffer, CColladaBinaryFileLoader, the factories and
// CDummyTransformationSceneNode have no layout in layouts/scene.hpp; their bodies are partial.
void CAppendMeshBuffer_adjustStride(void* self, std::uint16_t stride);                                      // 0x6b8834
void CAppendMeshBuffer_configureStream(void* self, std::uint8_t streamIndex, std::int32_t attribute,
                                       std::int32_t valueType, std::uint16_t componentCount);              // 0x6b8950
void CAppendMeshBuffer_endAppend(void* self);                                                               // 0x6b8aa0
void CAppendMeshBuffer_resize(void* self, std::uint32_t vertexCount, std::uint32_t indexCount);            // 0x6b8b74
void CAppendMeshBuffer_reset(void* self);                                                                   // 0x6b8bb4
void CAppendMeshBuffer_dtor_complete(void* self);                                                           // 0x6b8c2c
void CAppendMeshBuffer_ctor(void* self, std::uint32_t vertexCount, std::uint32_t indexCount,
                            void* driver, std::int32_t usage, std::uint32_t stride);                        // 0x6b8cb4
void CAppendMeshBuffer_ctor_alt(void* self, std::uint32_t vertexCount, std::uint32_t indexCount,
                                void* driver, std::int32_t usage, std::uint32_t stride);                    // 0x6b8fe0
void CMeshBuffer_ctor(glitch::scene::CMeshBuffer* self, std::uint32_t streamCount, const void* primitiveStream,
                      bool flag);                                                                           // 0x6b88a8
void CMeshBuffer_onDelete(glitch::scene::CMeshBuffer* self);                                                        // 0x6bc6d8
void CMeshBuffer_dtor_complete(glitch::scene::CMeshBuffer* self);                                                   // 0x6bca38
void* CMeshBuffer_clone(glitch::scene::CMeshBuffer* self, void* sourceStreams, void* driver);                       // 0x6bcb90
void CColladaBinaryFileLoader_dtor_complete(void* self);                                                    // 0x6b93b0
void CColladaBinaryFileLoader_dtor_complete_alt(void* self);                                                // 0x6b93f0
int CColladaBinaryFileLoader_createScene(void* self, void* file);                                           // 0x6b9430
void* CColladaBinaryFileLoader_createMesh(void* self, void* file, void* driverFile);                        // 0x6b9458
void CDefaultSceneNodeAnimatorFactory_ctor(void* self, glitch::scene::CSceneManager* manager, void* cursorControl); // 0x6b9590
void CDefaultSceneNodeAnimatorFactory_ctor_alt(void* self, glitch::scene::CSceneManager* manager,
                                               void* cursorControl);                                        // 0x6b95dc
void CDefaultSceneNodeAnimatorFactory_dtor_complete(void* self);                                            // 0x6b9628
void CDefaultSceneNodeAnimatorFactory_dtor_complete_alt(void* self);                                        // 0x6b9668
std::int32_t CDefaultSceneNodeAnimatorFactory_getTypeFromName(void* self, const char* name);                // 0x6b9720
void* CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator_byName(void* self, const char* name,
                                                                     void* sceneNode);                      // 0x6b9778
void* CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator(void* self, std::int32_t type,
                                                               void* sceneNode);                            // 0x6b9928
std::int32_t CDefaultSceneNodeFactory_getCreateableSceneNodeTypeName(void* self, std::int32_t type);       // 0x6b9da0
std::int32_t CDefaultSceneNodeFactory_getTypeFromName(void* self, const char* name);                        // 0x6b9e58
void* CDefaultSceneNodeFactory_addSceneNode_byName(void* self, const char* name, void* parent);           // 0x6b9f04
void* CDefaultSceneNodeFactory_addSceneNode(void* self, std::int32_t type, void* parent);                   // 0x6b9f30
void CDefaultSceneNodeFactory_SSceneNodeTypePair_ctor(void* self, std::int32_t type, const char* name);     // 0x6ba774
void CDefaultSceneNodeFactory_dtor_complete(void* self);                                                    // 0x6ba8f8
void CDefaultSceneNodeFactory_ctor(void* self, glitch::scene::CSceneManager* manager, void* cursorControl,
                                   void* fileSystem);                                                       // 0x6baae8
void CDefaultSceneNodeFactory_ctor_alt(void* self, glitch::scene::CSceneManager* manager, void* cursorControl,
                                       void* fileSystem);                                                   // 0x6bb008
void* CDummyTransformationSceneNode_getRelativeTransformationMatrix(void* self);                             // 0x6bb540
void IDummyTransformationSceneNode_dtor_complete(void* self);                                               // 0x6bb568
void CDummyTransformationSceneNode_dtor_complete(void* self);                                               // 0x6bb5d4
void CDummyTransformationSceneNode_setRelativeTransformationMatrix(void* self, const float* matrix);        // 0x6bb65c
void IDummyTransformationSceneNode_ctor(void* self, const void* parent, std::int32_t id);                   // 0x6bb6f0
void CDummyTransformationSceneNode_ctor(void* self, std::int32_t id);                                       // 0x6bb77c
void* CDummyTransformationSceneNode_clone(void* self);                                                      // 0x6bb858
void CDummyTransformationSceneNode_ctor_alt(void* self, const void* parent, std::int32_t id);               // 0x6bb940
void CMesh_SBuffer_assign(glitch::scene::CMeshSBuffer* self, const glitch::scene::CMeshSBuffer* other);                     // 0x6bbcc4
void CMesh_dtor_complete(glitch::scene::CMesh* self);                                                               // 0x6bc298
void CMesh_recalculateBoundingBox(glitch::scene::CMesh* self);                                                      // 0x6bc2cc
void* CMesh_clone(glitch::scene::CMesh* self, void* source);                                                        // 0x6bc3e4
void* CMeshCache_getMeshByIndex(void* self, std::uint32_t index);                                           // 0x6be5b0
bool CMeshCache_isMeshLoaded(void* self, const char* name);                                                 // 0x6be6d4

}  // namespace glitch::collada::recon_25
