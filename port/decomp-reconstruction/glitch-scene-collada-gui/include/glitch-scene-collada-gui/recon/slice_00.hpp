// Slice 00 reconstruction: glitch::debugger::CTweakable, scene culling helpers,
// the synchronized animator blender, and the glitch::gui element/environment
// base functions. Types here are minimal stand-ins named after the originals.
// Field offsets are kept as comments; the layout is not ABI compatible with
// the original binary and only the recovered behaviour is meant to be kept.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::collada::recon_00 {

// ---------------------------------------------------------------------------
// glitch::debugger::CTweakable
// ---------------------------------------------------------------------------
class Tweakable;

class TweakableMapping {
public:
    // glitch::debugger::CTweakable::SMapping(E_ATTRIBUTE_TYPE, void*)
    // 0x32d088 _ZN6glitch8debugger10CTweakable8SMappingC1ENS_2io16E_ATTRIBUTE_TYPEEPv
    TweakableMapping() = default;
    TweakableMapping(int type, void* value);

    // 0x320294 _ZN6glitch8debugger10CTweakable8SMappingaSERKS2_
    TweakableMapping& operator=(const TweakableMapping& other);

    int type = 0;            // +0x00
    void* value = nullptr;   // +0x04
    std::string name;        // +0x08
    std::string text;        // +0x20
};

class Tweakable {
public:
    // 0x32cfc8 _ZN6glitch8debugger10CTweakableC2EPNS_7IDeviceE
    explicit Tweakable(void* device);
    // 0x32a088 / 0x404ab0 (D1 / D2 share one body)
    virtual ~Tweakable();

    // 0x31f420 _ZN6glitch8debugger10CTweakable10onSetValueERKSs (empty)
    virtual void onSetValue(const std::string& value);

    // 0x31f410 _ZNK6glitch8debugger10CTweakable15getInstanceNameEv
    int getInstanceName() const;
    // 0x31f418 _ZNK6glitch8debugger10CTweakable6getXMLEv
    // Original returns the address of the xml-name member (this+0x70).
    const std::string& getXML() const;

    // 0x32bb98 _ZN6glitch8debugger10CTweakable20registerVariableNameEPKc
    void registerVariableName(const char* name);
    // 0x32eeb8 _ZN6glitch8debugger10CTweakable16registerVariableEPKcRNS_5video6SColorE
    void registerVariable(const char* name, glitch::video::SColor& value);
    // 0x32ef4c _ZN6glitch8debugger10CTweakable16registerVariableEPKcRf
    void registerVariable(const char* name, float& value);
    // 0x32efe0 _ZN6glitch8debugger10CTweakable16registerVariableEPKcRi
    void registerVariable(const char* name, int& value);

    // 0x328994 _ZN6glitch8debugger10CTweakable11writeValuesEPNS_2io10IWriteFileE
    // Partial: XML writer is not part of this slice; writes nothing yet.
    void writeValues();
    // 0x32b3b4 _ZN6glitch8debugger10CTweakable8setValueERNS_2io11CAttributesEi
    // Partial: the 10-way type switch depends on CAttributes getters; not mapped.
    void* setValue(void* attributes, int index);
    // 0x32b650 _ZN6glitch8debugger10CTweakable9loadGroupERNS_2io11CAttributesEb.clone.25
    // Partial: attribute-group recursion; the attribute walker is not mapped.
    int loadGroup(void* attributes);
    // 0x32b6b8 _ZN6glitch8debugger10CTweakable7loadXMLEPKcb
    // Partial: file read and XML attribute reader are not mapped.
    void loadXML(const char* file, bool flag);

    // +0x04 original std::map<std::string, SMapping>
    std::map<std::string, TweakableMapping> mappings;
    // +0x1c original group name list (push_back target of registerVariableName)
    std::vector<std::string> names;
    // +0x54 root group name (assigned from the loaded XML)
    std::string rootGroup;
    // +0x68 instance name id (returned by getInstanceName)
    int instanceName = 0;
    // +0x6c device pointer (IDevice*)
    void* device = nullptr;
    // +0x70 xml file name (returned by getXML)
    std::string xmlName;
};

// glitch::ISharedObject<glitch::io::CFile>::drop() const
// 0x34f038 _ZNK6glitch13ISharedObjectINS_2io5CFileEE4dropEv
template <class T>
class ISharedObject {
public:
    void drop() const {
        if (refcount-- == 1) {
            delete static_cast<T*>(const_cast<ISharedObject*>(this));
        }
    }
    mutable std::int32_t refcount = 1;  // +0x00
};

// ---------------------------------------------------------------------------
// glitch::scene::SViewFrustum and culling helpers
// ---------------------------------------------------------------------------
struct FrustumPlane {
    float normal[3];  // +0x00 (nx, ny, nz)
    float d;          // +0x0c
};

class ViewFrustum {
public:
    // 0x35bec8 _ZNK6glitch5scene12SViewFrustum10intersectsERKNS_4core8aabbox3dIfEE
    bool intersects(const glitch::core::aabbox3d<float>& box) const;
    // 0x50cacc _ZNK6glitch5scene12SViewFrustum25intersectsWithoutBoxTest3ERKNS_4core8aabbox3dIfEE
    bool intersectsWithoutBoxTest3(const glitch::core::aabbox3d<float>& box) const;

    std::uint32_t header[3];        // +0x00 .. +0x0b (not used by these helpers)
    FrustumPlane planes[6];         // +0x0c, six planes of four floats each
    glitch::core::aabbox3d<float> box;  // +0x6c (frustum bounding box)
};

// ---------------------------------------------------------------------------
// glitch::collada::CSceneNodeAnimatorSynchronizedBlender / SynchronizedSet
// Segment animators are modelled through this stand-in interface. The slot
// numbers in the comments are the original vtable offsets.
// ---------------------------------------------------------------------------
class ISyncSegment {
public:
    virtual ~ISyncSegment() = default;
    virtual int getNumSegments() const = 0;                           // +8
    virtual int getSynchronizationLength(unsigned int at) const = 0;   // +12
    virtual int getSynchronizationTimestamp(unsigned int at) const = 0; // +20
    virtual int getSynchronizationSegment(unsigned int at) const = 0; // +28
    virtual int getSynchronizationPercentage() const = 0;             // +64
};

class SynchronizedBlender {
public:
    // 0x367870 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender14getNumSegmentsEv
    int getNumSegments() const;
    // 0x367898 ...getSynchronizationLengthEij
    int getSynchronizationLength(int index, unsigned int at) const;
    // 0x3678c4 ...getSynchronizationLengthEj (weighted sum, virtual slot +144)
    int getSynchronizationLength(unsigned int at) const;
    // 0x367948 ...getSynchronizationTimestampEij
    int getSynchronizationTimestamp(int index, unsigned int at) const;
    // 0x367974 ...getSynchronizationTimestampEj (weighted sum, virtual slot +152)
    int getSynchronizationTimestamp(unsigned int at) const;
    // 0x3679f8 ...getSynchronizationSegmentEii
    int getSynchronizationSegment(int index, int at) const;
    // 0x367a24 ...getSynchronizationSegmentEi (partial: slot +156 call has no arg in pseudo)
    int getSynchronizationSegment(int at) const;
    // 0x367a88 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender16normalizeWeightsEv
    float normalizeWeights();
    // 0x367b44 ...getSynchronizationPercentageEv
    int getSynchronizationPercentage() const;

    // +0x30 segments begin, +0x34 segments end (pointers to ISyncSegment)
    std::vector<ISyncSegment*> segments;
    // +0x3c weights begin, +0x40 weights end
    std::vector<float> weights;
    // +0xb2 weights normalized flag
    bool weightsNormalized = false;
};

class SynchronizedSet {
public:
    // 0x368e30 _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet25getSynchronizationSegmentEi
    unsigned int getSynchronizationSegment(int at) const;
    // 0x368ed4 _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet28getSynchronizationPercentageEv
    float getSynchronizationPercentage() const;

    // +0x6c count of segments; +0x7c accumulated weight (partial model)
    unsigned int segmentCount = 0;
    float weight = 0.0f;
    // Virtual hooks used by the set (slots +72 and +156 in the original).
    virtual ~SynchronizedSet() = default;
    virtual float getTotalLength() const;                 // +72
    virtual int getSegmentTimestamp(unsigned int i) const;  // +156
};

// ---------------------------------------------------------------------------
// glitch::gui::IGUIElement / CGUIEnvironment base functions
// The element list is kept as a std::vector instead of the original intrusive
// list. Virtual slots of the original are named in the comments.
// ---------------------------------------------------------------------------
class GuiEnvironment;

class GuiElement {
public:
    virtual ~GuiElement() = default;
    // +16: hit test used by getElementFromPoint
    virtual bool isPointInside(const glitch::core::position2d<int>& point) const;
    // +32 / +36
    virtual void draw();
    virtual void OnPostRender(unsigned int time);
    // +84 / +92 (identity and name accessors)
    virtual int getId() const;
    virtual const char* getName() const;
    // +8: event forwarding slot used by onEvent
    virtual int forwardEvent();

    // 0x534740 _ZN6glitch3gui11IGUIElement19setRelativePositionERKNS_4core4rectIiEE
    // Partial: scaled alignment fields (+0x78..) are not mapped yet.
    int setRelativePosition(const glitch::core::rect<int>& rectangle);
    // 0x534840 partial: alignment fields and parent-size scaling not mapped.
    float setAlignment(int horizontal, int vertical, int hEnd, int vEnd);
    // 0x534eb8
    void drawChildren();
    // 0x534ef8
    void postRenderChildren(unsigned int time);
    // 0x534e90
    int remove();
    // 0x534f40
    int move(glitch::core::position2d<int> delta);
    // 0x534fe8 partial: parent forwarding passes no event argument in pseudo.
    int onEvent();
    // 0x534dd0
    GuiElement* getElementFromPoint(const glitch::core::position2d<int>& point);
    // 0x53501c partial: recursive lookup uses slots +84 and +108 on children.
    GuiElement* getElementFromId(int id, bool recursive) const;
    // 0x535d10 partial: list unlink and reference drop are stand-ins.
    int removeChild(GuiElement* child);
    // 0x535d70 partial: list move-to-end; list node ops are stand-ins.
    int bringToFront(GuiElement* child);
    // 0x535dec partial: reference add and parent-rect propagation stand-ins.
    GuiElement* addChild(GuiElement* child);
    // 0x5362c8
    int setText(const wchar_t* text);
    // 0x534920 skipped in this pass (1112-byte layout recompute; see ledger).

    // +0x04 children (intrusive list in the original)
    std::vector<GuiElement*> children;
    // +0x24 parent element
    GuiElement* parent = nullptr;
    // +0x58 .. +0x64 relative rectangle (UpperLeftCorner, LowerRightCorner)
    glitch::core::rect<int> relative{};
    // +0x98 visible/enabled flag (byte)
    bool visible = true;
    // +0xa0 text (wide string)
    std::wstring text;
};

class GuiEnvironment {
public:
    virtual ~GuiEnvironment() = default;
    // 0x5353c8 partial: focus-change reference handling is a stand-in.
    int setFocus(GuiElement* element);
    // 0x5355a0 partial: focus drop uses the stand-in reference counter.
    int removeFocus(GuiElement* element);
    // 0x535658 partial: list clear and element drop stand-ins.
    void clear();
    // 0x535778 partial: skin reference counting stand-in.
    int setSkin(void* skin);
    // 0x5357d0 partial: virtual factory slot +224 is not mapped.
    void* getDefaultGUIElementFactory() const;
    // 0x535818 partial: element factory fan-out stand-in.
    int addGUIElement(const char* name, GuiElement* parentElement);
    // 0x535884 partial: save path uses writer slots not mapped.
    int saveGUI(const char* file, GuiElement* element);
    // 0x5358e0 partial: writer creation stand-in.
    int saveGUI(void* writeFile, GuiElement* element);
    // 0x535b58 partial: lookup walks children with slot +68 and +104.
    GuiElement* getElementById(int id, GuiElement* start);
    // 0x535c10
    GuiElement* getRootGUIElement();
    // 0x5364f4 partial: concrete element constructor not mapped; returns nullptr.
    GuiElement* addComboBox(const glitch::core::rect<int>& rectangle, GuiElement* parentElement, int id);
    // 0x536574 partial: constructor not mapped; returns nullptr.
    GuiElement* addInOutFader(const glitch::core::rect<int>* rectangle, GuiElement* parentElement, int id);
    // 0x536628 partial: constructor not mapped; returns nullptr.
    GuiElement* addToolBar(GuiElement* parentElement, int id);
    // 0x53669c partial: constructor not mapped; returns nullptr.
    GuiElement* addMenu(GuiElement* parentElement, int id);
    // 0x536724 partial: constructor not mapped; returns nullptr.
    GuiElement* addContextMenu(const glitch::core::rect<int>& rectangle, GuiElement* parentElement, int id);
    // 0x5367ac partial: constructor not mapped; returns nullptr.
    GuiElement* addTab(const glitch::core::rect<int>& rectangle, GuiElement* parentElement, int id);
    // 0x536810 partial: constructor not mapped; returns nullptr.
    GuiElement* addTabControl(const glitch::core::rect<int>& rectangle, GuiElement* parentElement,
                              bool fillBackground, bool border, int id);
    // 0x536880 partial: constructor not mapped; returns nullptr.
    GuiElement* addSpinBox(const wchar_t* text, const glitch::core::rect<int>& rectangle,
                           GuiElement* parentElement, int id);
    // 0x5368e8 partial: constructor not mapped; returns nullptr.
    GuiElement* addEditBox(const wchar_t* text, const glitch::core::rect<int>& rectangle, bool border,
                           GuiElement* parentElement, int id);
    // 0x536388 partial: read-file factory slot not mapped.
    int loadGUI(const char* file, GuiElement* element);
    // 0x53644c partial: lookup walks children; name compare via slot +92.
    GuiElement* getElementByName(const char* name, GuiElement* start);

    // +0x08 root element (embedded)
    GuiElement root;
    // +0x1b0 focus element, +0x1ac focus-reference element (stand-in names)
    GuiElement* focus = nullptr;
    GuiElement* focusReference = nullptr;
    // +0x1bc current skin (IGUISkin*)
    void* skin = nullptr;
};

// ---------------------------------------------------------------------------
// Small free-standing bodies
// ---------------------------------------------------------------------------

// glitch::scene::CBatchMeshExt<GameObject*>::saveSegmentExtraData(...) const
// 0x50cf04 _ZNK6glitch5scene13CBatchMeshExtIP10GameObjectE20saveSegmentExtraDataEPKvPNS_2io10IWriteFileENS_2os8E_ENDIANE
// Original is an empty body.
class BatchMeshExtExtraData {
public:
    void saveSegmentExtraData(const void* segment, void* writeFile, int endian) const;
};

// glitch::CIrrFactory (create/destroy helpers)
class IrrFactory {
public:
    // 0x534090 ~CIrrFactory D0 is a compiler deleting thunk; not rebuilt.
    // 0x533fb0 _ZN6glitch11CIrrFactory16createFileSystemEv
    // Partial: CFileSystem constructor is a stand-in, stores and retains it.
    IrrFactory* createFileSystem();
    // 0x533f28 _ZN6glitch11CIrrFactory20createGUIEnvironment...
    // Partial: the CGUIEnvironment constructor is a stand-in.
    GuiEnvironment* createGUIEnvironment(void* fileSystem, void* videoDriver, void* osOperator);

    // +0x00 file system (retained)
    void* fileSystem = nullptr;
};

}  // namespace glitch::collada::recon_00
