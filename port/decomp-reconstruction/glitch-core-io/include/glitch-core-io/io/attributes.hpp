// glitch::io::IAttribute (interface subset) and glitch::io::CAttributes
// (group tree of attributes, with name lookup and indexed access).
// Reconstructed from libDungeonHunter2.so.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

namespace detail {
// Zeroed 16 values with the diagonal set to 1, as the original fills it.
inline glitch::core::CMatrix4<float> identityMatrix() {
    glitch::core::CMatrix4<float> m{};
    m.M[0] = m.M[5] = m.M[10] = m.M[15] = 1.f;
    return m;
}
}  // namespace detail

// Interface subset. Virtual slot numbers from the vtable are noted; the base
// implementations of the setters reconstructed so far are empty.
class IAttribute : public glitch::IReferenceCounted {
public:
    // +0x1c (c_str of the glitch string at +0x08): attribute name.
    std::string name;
    // +0x20: flag passed to the constructors; the io attribute code never reads it.
    bool flag = false;

    ~IAttribute() override = default;

    virtual int getInt() const { return 0; }                              // +0x0c
    virtual glitch::video::SColorf getColorf() const { return {1.f, 1.f, 1.f, 1.f}; }  // +0x14
    virtual glitch::core::vector3d<float> getVector3d() const { return {}; }            // +0x30
    virtual glitch::core::vector4d<float> getVector4d() const { return {}; }            // +0x34
    virtual glitch::core::position2d<std::int32_t> getPosition() const { return {}; }   // +0x38
    virtual glitch::core::rect<std::int32_t> getRect() const { return {}; }             // +0x3c
    virtual glitch::core::vector2d<float> getVector2d() const { return {}; }            // +0x4c
    virtual glitch::core::vector2d<std::int32_t> getVector2di() const { return {}; }    // +0x50
    virtual glitch::core::vector3d<std::int32_t> getVector3di() const { return {}; }    // +0x54
    virtual glitch::core::vector4d<std::int32_t> getVector4di() const { return {}; }    // +0x58
    virtual glitch::video::SColor getColor() const { return {0xffffffffu}; }  // 0x31d5f8
    virtual float getFloat() const { return 0.f; }                        // +0x10
    virtual bool getBool() const { return false; }                        // +0x28
    virtual void getBinary(void*, int) const {}                           // +0x2c
    virtual int getEnum() const { return 0; }                             // +0x80
    virtual void* getUserPointer() const { return nullptr; }             // 0x31d81c
    virtual std::string getString() const;                                // +0x1c (0x326268)
    virtual std::wstring getStringW() const;                              // +0x20 (0x32097c)
    virtual glitch::core::CMatrix4<float> getMatrix() const;              // +0x44 (0x326588)
    virtual void setInt(int) {}                                           // +0x88
    virtual void setFloat(float) {}                                       // +0x8c
    virtual void setString(const char*) {}                                // +0x90
    virtual std::vector<std::wstring> getArray() const { return {}; }      // +0x24
    virtual void setArray(const std::vector<std::wstring>&) {}            // +0x98
    virtual void setColor(glitch::video::SColorf) {}                      // +0x9c
    virtual void setColor(glitch::video::SColor) {}                       // +0xa0
    virtual void setBool(bool) {}                                         // +0xa4
    virtual void setBinary(void*, int) {}                                 // +0xa8
    virtual void setVector2d(glitch::core::vector2d<float>) {}            // +0xac
    virtual void setVector2d(glitch::core::vector2d<std::int32_t>) {}     // +0x11c
    virtual void setVector3d(const glitch::core::vector3d<float>&) {}     // +0xb0
    virtual void setVector4d(const glitch::core::vector4d<float>&) {}     // +0xb4
    virtual void setPosition(glitch::core::position2d<std::int32_t>) {}   // +0xb8
    virtual void setVector2di(glitch::core::vector2d<std::int32_t>) {}    // +0xcc
    virtual void setVector3di(const glitch::core::vector3d<std::int32_t>&) {}  // +0xd0
    virtual void setVector4di(const glitch::core::vector4d<std::int32_t>&) {}  // +0xd4
    virtual void setUserPointer(void*) {}                                 // +0xf4
    virtual void setEnum(const char*, const char* const*) {}              // +0xf8
    // Type name (L"unknown" here; the base default is not recovered from the binary).
    virtual const wchar_t* getTypeString() const { return L"unknown"; }  // +0x108
    virtual int getType() const { return 0; }                             // +0x104
    // Base defaults from the original bodies (0x31d690 .. 0x31d7dc).
    virtual glitch::core::quaternion getQuaternion() const { return {0.f, 0.f, 0.f, 1.f}; }  // 0x31d690
    virtual glitch::core::triangle3d<float> getTriangle() const { return {}; }               // 0x31d6ac
    virtual glitch::core::line2d<float> getLine2d() const { return {{0.f, 0.f}, {1.f, 1.f}}; }   // 0x31d724
    virtual glitch::core::line2d<std::int32_t> getLine2di() const { return {{0, 0}, {1, 1}}; }  // 0x31d740
    virtual glitch::core::line3d<float> getLine3d() const { return {{0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}}; }  // 0x31d75c
    virtual glitch::core::line3d<std::int32_t> getLine3di() const { return {{0, 0, 0}, {1, 1, 1}}; }     // 0x31d780
    virtual glitch::core::dimension2d<std::int32_t> getDimension2d() const { return {}; }    // 0x31d7a4
    virtual glitch::core::aabbox3d<float> getBBox() const { return {{-1.f, -1.f, -1.f}, {1.f, 1.f, 1.f}}; }  // 0x31d7b4
    virtual glitch::core::plane3d<float> getPlane() const { return {{0.f, 1.f, 0.f}, -0.f}; }  // 0x31d7dc
    virtual void setQuaternion(const glitch::core::quaternion&) {}                          // 0x31d870
    virtual void setTriangle(const glitch::core::triangle3d<float>&) {}                     // 0x31d888
    virtual void setLine2d(glitch::core::line2d<float>) {}                                  // 0x31d898
    virtual void setLine2d(glitch::core::line2d<std::int32_t>) {}                           // 0x31d89c
    virtual void setLine3d(glitch::core::line3d<float>) {}                                  // 0x31d8a0
    virtual void setLine3d(glitch::core::line3d<std::int32_t>) {}                           // 0x31d8a4
    virtual void setDimension2d(glitch::core::dimension2d<std::int32_t>) {}                 // 0x31d8a8
    virtual void setBBox(glitch::core::aabbox3d<float>) {}                                  // 0x31d8b4
    virtual void setPlane(glitch::core::plane3d<float>) {}                                  // 0x31d8b8
    virtual void setString(const wchar_t* value);  // 0x326d20                                               // +0x94
    virtual void setRect(glitch::core::rect<std::int32_t>) {}                               // 0x31d86c
    // boost::intrusive_ptr accessors. The base returns null and ignores the value.
    virtual boost::intrusive_ptr<glitch::video::ITexture> getTexture() const;  // 0x31d7fc (+0x78)
    virtual boost::intrusive_ptr<glitch::video::CLight> getLight() const;      // 0x31d808 (+0x7c)
    virtual void setTexture(const boost::intrusive_ptr<glitch::video::ITexture>&) {}  // 0x31d8c4
    virtual void setLight(boost::intrusive_ptr<glitch::video::CLight>) {}             // 0x31d8c8
    virtual void setMatrix(const glitch::core::CMatrix4<float>&) {}                         // 0x31d884
};

class CAttributes : public glitch::IReferenceCounted {
public:
    // glitch::io::CAttributes::SContext: an attribute group. 0x3c bytes in the original:
    // +0x00 vptr, +0x04 refcount, +0x08 name (glitch string), +0x20 attribute
    // vector (begin/end/capacity), +0x2c child group vector, +0x38 parent group.
    class SContext : public glitch::IReferenceCounted {
    public:
        explicit SContext(const std::string& groupName);      // 0x562b10 (C1), 0x562c80 (C2)
        ~SContext() override;                                  // 0x5643a8 (D1), 0x5644f8 (D2)

        // Returns the direct child with this name, or nullptr. 0x562964
        SContext* hasContext(const char* groupName) const;
        // Returns the child with this name, creating it when create is set. 0x563690
        SContext* getContext(const char* groupName, bool create);
        // Drops every child group and attribute and empties both vectors. 0x563a54
        void clear();

        std::string name;                       // +0x08
        std::vector<IAttribute*> attributes;    // +0x20
        std::vector<SContext*> children;        // +0x2c
        SContext* parent = nullptr;             // +0x38
    };

    // +0x58: driver the container was created for (reference counted).
    explicit CAttributes(glitch::video::IVideoDriver* driver);   // 0x562b78 (C1), 0x562bfc (C2)
    ~CAttributes() override;                                      // 0x564418 (D1), 0x564488 (D2)

    // +0x08: the root group, embedded as in the original. Its attribute vector is at +0x28.
    SContext root{""};
    // +0x44: current group. The original also keeps +0x48 = &group->attributes; that
    // pointer is not stored here, the accessor below returns the same vector.
    SContext* group = &root;
    // +0x4c..+0x54 are three words the constructor zeroes; their role is not recovered.
    glitch::video::IVideoDriver* driver = nullptr;               // +0x58

    std::vector<IAttribute*>& currentAttributes() { return group->attributes; }
    const std::vector<IAttribute*>& currentAttributes() const { return group->attributes; }

    int getAttributeCount() const;                    // 0x5607e4
    const char* getAttributeName(int index) const;    // 0x5607fc
    int getAttributeType(int index) const;            // 0x560820
    bool getBool(int index) const;                    // 0x5608a0
    int getInt(int index) const;                      // 0x5608dc
    float getFloat(int index) const;                  // 0x560918
    int getEnum(int index) const;                     // 0x560d1c
    void getBinaryData(int index, void* data, int size) const;  // 0x560ce4

    glitch::video::SColorf getColorf(int index) const;                         // 0x5609e8
    glitch::core::vector2d<float> getVector2d(int index) const;                // 0x560a44
    glitch::core::vector3d<float> getVector3d(int index) const;                // 0x560a94
    glitch::core::vector4d<float> getVector4d(int index) const;                // 0x560ae8
    glitch::core::vector2d<std::int32_t> getVector2di(int index) const;        // 0x560b40
    glitch::core::vector3d<std::int32_t> getVector3di(int index) const;        // 0x560b90
    glitch::core::vector4d<std::int32_t> getVector4di(int index) const;        // 0x560be4
    glitch::core::position2d<std::int32_t> getPosition2d(int index) const;     // 0x560c3c
    glitch::core::rect<std::int32_t> getRect(int index) const;                 // 0x560c8c
    void setAttribute(int index, glitch::video::SColorf value);                // 0x560f0c

    void setAttribute(int index, const char* value);                       // 0x560db0
    void setAttribute(int index, bool value);                              // 0x560e20
    void setAttribute(int index, int value);                               // 0x560e58
    void setAttribute(int index, float value);                             // 0x560e90
    void setAttribute(int index, glitch::video::SColor value);             // 0x560ec8
    void setAttribute(int index, const glitch::core::vector2d<float>& value);  // 0x560f6c
    void setAttribute(int index, const glitch::core::vector3d<float>& value);  // 0x560fb8
    void setAttribute(int index, const glitch::core::vector4d<float>& value);  // 0x560ff0
    void setAttribute(int index, const glitch::core::vector2d<std::int32_t>& value);  // 0x561028
    void setAttribute(int index, const glitch::core::vector3d<std::int32_t>& value);  // 0x56106c
    void setAttribute(int index, const glitch::core::vector4d<std::int32_t>& value);  // 0x5610a4
    void setAttribute(int index, glitch::core::position2d<std::int32_t> value);       // 0x5610dc
    void setAttribute(int index, void* data, int size);                    // 0x56117c
    void setAttribute(int index, const char* value, const char* const* choices);  // 0x5611b4
    void setAttribute(int index, void* userPointer);                        // 0x56189c
    void setAttribute(int index, const wchar_t* value);                     // 0x560de8
    void setAttribute(int index, glitch::core::rect<std::int32_t> value);  // 0x561120
    void setAttribute(int index, boost::intrusive_ptr<glitch::video::ITexture> value);  // 0x5611ec
    void setAttribute(int index, boost::intrusive_ptr<glitch::video::CLight> value);    // 0x561aa0
    void setAttribute(int index, glitch::core::quaternion value);          // 0x5612ac
    void setAttribute(int index, const glitch::core::aabbox3d<float>& value);      // 0x5613b0
    void setAttribute(int index, const glitch::core::plane3d<float>& value);       // 0x5614b0
    void setAttribute(int index, const glitch::core::triangle3d<float>& value);    // 0x5615d4
    void setAttribute(int index, const glitch::core::line2d<float>& value);        // 0x5616e4
    void setAttribute(int index, const glitch::core::line3d<float>& value);        // 0x5617e4
    void setAttribute(int index, const glitch::core::CMatrix4<float>& value);      // 0x563b74
    const wchar_t* getAttributeTypeString(int index) const;                         // 0x56085c
    const wchar_t* getAttributeTypeString(const char* name) const;                 // 0x562388
    glitch::video::SColor getColor(int index) const;                           // 0x560954
    glitch::core::quaternion getQuaternion(int index) const;                   // 0x561224
    glitch::core::aabbox3d<float> getBox3d(int index) const;                   // 0x561314
    glitch::core::plane3d<float> getPlane3d(int index) const;                  // 0x561424
    glitch::core::triangle3d<float> getTriangle3d(int index) const;            // 0x561514
    glitch::core::line2d<float> getLine2d(int index) const;                    // 0x561660
    glitch::core::line3d<float> getLine3d(int index) const;                    // 0x561748
    void* getUserPointer(int index) const;                                     // 0x561858
    std::vector<std::wstring> getArray(int index) const;                       // 0x563308
    glitch::core::CMatrix4<float> getMatrix(int index) const;                  // 0x565464
    std::string getString(int index) const;                                    // 0x564240
    std::wstring getStringW(int index) const;                                  // 0x561950
    boost::intrusive_ptr<glitch::video::ITexture> getTexture(int index) const;  // 0x56074c
    boost::intrusive_ptr<glitch::video::CLight> getLight(int index) const;      // 0x560798
    boost::intrusive_ptr<glitch::video::ITexture> getTexture(const char* name) const;  // 0x56241c
    boost::intrusive_ptr<glitch::video::CLight> getLight(const char* name) const;      // 0x5623e0

    // Group handling (0x564314 .. 0x5618f0).
    void clear();                                     // 0x563b30
    void push(const char* groupName);                 // 0x564314
    void push(int groupIndex);                        // 0x564568: group named "group<index>"
    void push(const char* groupName, int groupIndex); // 0x564644
    void pop();                                       // 0x5618d8
    void setGroup(unsigned int index);                // 0x561908
    const std::string& getGroupName() const;          // 0x561924
    int getGroupsCount() const;                       // 0x5618f0

    // Typed attributes appended to the current group (no check for an existing name).
    void addInt(const char* name, int value, bool flag);                       // 0x568b1c
    void addFloat(const char* name, float value, bool flag);                  // 0x56891c
    void addBool(const char* name, bool value, bool flag);                    // 0x56871c
    void addUserPointer(const char* name, void* value, bool flag);            // 0x568038
    void addRect(const char* name, glitch::core::rect<std::int32_t> value, bool flag);  // 0x568ce0
    void addVector2d(const char* name, const glitch::core::vector2d<float>& value, bool flag);       // 0x56a1a0
    void addVector3d(const char* name, const glitch::core::vector3d<float>& value, bool flag);       // 0x56a030
    void addVector4d(const char* name, const glitch::core::vector4d<float>& value, bool flag);       // 0x569ebc
    void addVector2di(const char* name, const glitch::core::vector2d<std::int32_t>& value, bool flag);  // 0x5692c8
    void addVector3di(const char* name, const glitch::core::vector3d<std::int32_t>& value, bool flag);  // 0x569158
    void addVector4di(const char* name, const glitch::core::vector4d<std::int32_t>& value, bool flag);  // 0x568fe4
    void addPosition2d(const char* name, glitch::core::position2d<std::int32_t> value, bool flag);   // 0x568e64
    void addLine2d(const char* name, glitch::core::line2d<float> value, bool flag);                  // 0x569604
    void addLine3d(const char* name, glitch::core::line3d<float> value, bool flag);                  // 0x56944c
    void addTriangle3d(const char* name, glitch::core::triangle3d<float> value, bool flag);          // 0x5697ac
    void addBox3d(const char* name, glitch::core::aabbox3d<float> value, bool flag);                 // 0x569b3c
    void addPlane3d(const char* name, glitch::core::plane3d<float> value, bool flag);                // 0x569998
    void addQuaternion(const char* name, glitch::core::quaternion value, bool flag);                 // 0x569d00
    void addColorf(const char* name, glitch::video::SColorf value, bool flag);                       // 0x56a31c
    void addColor(const char* name, glitch::video::SColor value, bool flag);                         // 0x56a528
    void addStringAsColor(const char* name, const wchar_t* value, bool flag);                        // 0x56a3ec
    // String-valued adds: each creates the typed attribute with its default value, appends it
    // to the current group, then sets the value from the wide string through setString (+0x94).
    void addStringAsUserPointer(const char* name, const wchar_t* value, bool flag);  // 0x567f24
    void addStringAsBool(const char* name, const wchar_t* value, bool flag);         // 0x568608
    void addStringAsFloat(const char* name, const wchar_t* value, bool flag);        // 0x568808
    void addStringAsInt(const char* name, const wchar_t* value, bool flag);          // 0x568a08
    void addStringAsRect(const char* name, const wchar_t* value, bool flag);         // 0x568c08
    void addStringAsPosition2d(const char* name, const wchar_t* value, bool flag);   // 0x568d94
    void addStringAsVector4di(const char* name, const wchar_t* value, bool flag);    // 0x568f0c
    void addStringAsVector3di(const char* name, const wchar_t* value, bool flag);    // 0x569084
    void addStringAsVector2di(const char* name, const wchar_t* value, bool flag);    // 0x5691f8
    void addStringAsLine3d(const char* name, const wchar_t* value, bool flag);       // 0x569368
    void addStringAsLine2d(const char* name, const wchar_t* value, bool flag);       // 0x569528
    void addStringAsTriangle3d(const char* name, const wchar_t* value, bool flag);   // 0x5696c0
    void addStringAsPlane3d(const char* name, const wchar_t* value, bool flag);      // 0x5698b8
    void addStringAsBox3d(const char* name, const wchar_t* value, bool flag);        // 0x569a54
    void addStringAsQuaternion(const char* name, const wchar_t* value, bool flag);   // 0x569c18
    void addStringAsVector4d(const char* name, const wchar_t* value, bool flag);     // 0x569de4
    void addStringAsVector3d(const char* name, const wchar_t* value, bool flag);     // 0x569f5c
    void addStringAsVector2d(const char* name, const wchar_t* value, bool flag);     // 0x56a0d0
    void addStringAsColorf(const char* name, const wchar_t* value, bool flag);       // 0x56a240
    void addStringAsMatrix(const char* name, const wchar_t* value, bool flag);       // 0x56a638
    void addStringAsTexture(const char* name, const wchar_t* value, bool flag);      // 0x5682c0
    void addString(const char* name, const wchar_t* value, bool flag);               // 0x56a798
    void addMatrix(const char* name, const glitch::core::CMatrix4<float>& value, bool flag);         // 0x56a70c

    // By name: set the existing attribute, or append a new one with flag false.
    void setAttribute(const char* name, int value);                           // 0x56b11c
    void setAttribute(const char* name, float value);                         // 0x56b07c
    void setAttribute(const char* name, bool value);                          // 0x56b1bc
    void setAttribute(const char* name, void* value);                         // 0x56ad30
    void setAttribute(const char* name, glitch::core::rect<std::int32_t> value);  // 0x56b878
    void setAttribute(const char* name, const glitch::core::vector2d<float>& value);       // 0x5659b4
    void setAttribute(const char* name, const glitch::core::vector2d<std::int32_t>& value);  // 0x567a9c
    void setAttribute(const char* name, const glitch::core::vector3d<float>& value);       // 0x56b7b8
    void setAttribute(const char* name, const glitch::core::vector4d<float>& value);       // 0x56b6f8
    void setAttribute(const char* name, const glitch::core::vector3d<std::int32_t>& value);  // 0x56b638
    void setAttribute(const char* name, const glitch::core::vector4d<std::int32_t>& value);  // 0x56b578
    void setAttribute(const char* name, glitch::core::position2d<std::int32_t> value);      // 0x56b978
    void setAttribute(const char* name, glitch::core::line2d<float> value);                 // 0x56bcc4
    void setAttribute(const char* name, const glitch::core::plane3d<float>& value);         // 0x56bf4c
    void setAttribute(const char* name, glitch::core::quaternion value);                    // 0x56add0
    void setAttribute(const char* name, const glitch::core::CMatrix4<float>& value);        // 0x56aee4
    void setAttribute(const char* name, glitch::video::SColor value);                       // 0x56abe8
    void setAttribute(const char* name, glitch::video::SColorf value);                      // 0x56b25c

    // Name lookup in the current group (0x5628f4 .. 0x562374).
    unsigned int findAttribute(const char* name) const;   // 0x5628f4: index, or 0xffffffff
    IAttribute* getAttributeP(const char* name) const;     // 0x561fa0
    bool existsAttribute(const char* name) const;          // 0x562374
    int getAttributeType(const char* name) const;          // 0x5623b8: 0x1e when missing

    bool getBool(const char* name) const;                  // 0x562888
    int getInt(const char* name) const;                    // 0x562860
    float getFloat(const char* name) const;                // 0x562838
    int getEnum(const char* name) const;                   // 0x5624c4
    void getBinaryData(const char* name, void* data, int size) const;  // 0x5624ec
    glitch::video::SColorf getColorf(const char* name) const;     // 0x562768
    glitch::video::SColor getColor(const char* name) const;       // 0x5627c0
    glitch::core::vector2d<float> getVector2d(const char* name) const;   // 0x562694
    glitch::core::vector3d<float> getVector3d(const char* name) const;   // 0x562644
    glitch::core::vector4d<float> getVector4d(const char* name) const;   // 0x5625f0
    glitch::core::vector2d<std::int32_t> getVector2di(const char* name) const;  // 0x5625b0
    glitch::core::vector3d<std::int32_t> getVector3di(const char* name) const;  // 0x56256c
    glitch::core::vector4d<std::int32_t> getVector4di(const char* name) const;  // 0x56251c
    glitch::core::position2d<std::int32_t> getPosition2d(const char* name) const;  // 0x562728
    glitch::core::rect<std::int32_t> getRect(const char* name) const;     // 0x5626d8
    glitch::core::quaternion getQuaternion(const char* name) const;       // 0x5622fc
    glitch::core::aabbox3d<float> getBox3d(const char* name) const;       // 0x562270
    glitch::core::plane3d<float> getPlane3d(const char* name) const;      // 0x5621f4
    glitch::core::triangle3d<float> getTriangle3d(const char* name) const; // 0x562144
    glitch::core::line2d<float> getLine2d(const char* name) const;        // 0x5620d0
    glitch::core::line3d<float> getLine3d(const char* name) const;        // 0x562044
    void* getUserPointer(const char* name) const;                         // 0x56201c
    std::vector<std::wstring> getArray(const char* name) const;           // 0x5628b0
    glitch::core::CMatrix4<float> getMatrix(const char* name) const;      // 0x567e48
    std::string getString(const char* name) const;                        // 0x564768
    void getString(const char* name, char* out) const;                    // 0x564924
    std::wstring getStringW(const char* name) const;                      // 0x5637c0
    void getStringW(const char* name, wchar_t* out) const;                // 0x563858
};

}  // namespace glitch::io
