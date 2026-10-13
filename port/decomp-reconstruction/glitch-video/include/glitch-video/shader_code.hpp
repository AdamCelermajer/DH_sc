// glitch::video::IShaderCode, CGLSLShaderCode and the code-shader manager. Canonical definition of IShaderCode (engine
// unification wave u1-video); the stand-in in glitch-core-io (deps/glitch_core_objects.hpp) is replaced by this header.
// The shader-code collection is the canonical SIDedCollection instance (glitch-core-io/core/sided_collection.hpp, over
// glitch-shared/sid_collection.hpp); its getId keeps the 0xffff "not found" convention.
//
// IShaderCode layout (32-bit; constructors 0x6e2130 C1 / 0x6e2178 C2, size 0x20):
//   +0x00  vptr (vtable 0x98b208, five slots)
//   +0x04  reference count; the constructor stores 0 (a new code is wrapped with intrusive_ptr<IShaderCode>(p, true))
//   +0x08  std::string name (STLport, 0x18 bytes)
// Vtable: +0x00 ~IShaderCode (D1 0x6e207c, D2 0x6e20e4; D0 0x6e20c8)  +0x04 deleting destructor  +0x08 onDelete
//         +0x0c serializeAttributes(IAttributes*) const (pure)  +0x10 deserializeAttributes(IAttributes*) (pure)
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-core-io/core/sided_collection.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/video_types.hpp"

namespace glitch::video {

class IShaderCode : public glitch::IReferenceCounted {
public:
    explicit IShaderCode(const char* name);  // 0x6e2130 (C1), 0x6e2178 (C2)
    ~IShaderCode() override = default;       // 0x6e207c (D1), 0x6e20e4 (D2): frees the name

    virtual void serializeAttributes(io::IAttributes* attributes) const = 0;  // +0x0c
    virtual void deserializeAttributes(io::IAttributes* attributes) = 0;      // +0x10

    // +0x04 is IReferenceCounted::referenceCounter.
    std::string m_name;   // +0x08, std::string built from the name argument
};

class CProgrammableGLDriver;

// CGLSLShaderCode layout (constructors 0x6df690 / 0x6df6e4 driver only, 0x6df738 / 0x6df878 with source text; size 0x38):
//   +0x20  char** source lines (new[] copies of the strings)   +0x24  line count
//   +0x28  GL shader type (0x8b30 fragment, 0x8b31 vertex)     +0x2c  CProgrammableGLDriver*
//   +0x30  GL shader object name                               +0x34  uint8, compiled flag (0 after construction)
class CGLSLShaderCode : public IShaderCode {
public:
    // 0x6df690 (C1) / 0x6df6e4 (C2). The base is built with an empty name. The driver is kept at +0x2c; the GL shader
    // name (+0x30) and the compiled flag (+0x34) are zeroed.
    explicit CGLSLShaderCode(CProgrammableGLDriver* driver);
    void serializeAttributes(io::IAttributes* attributes) const override;  // 0x6df398, empty
    void deserializeAttributes(io::IAttributes* attributes) override;      // 0x6df39c, empty

private:
    char** m_sourceLines = nullptr;             // +0x20
    int32_t m_sourceLineCount = 0;              // +0x24
    uint32_t m_glShaderType = 0;                // +0x28
    CProgrammableGLDriver* m_driver = nullptr;  // +0x2c
    uint32_t m_unknown30 = 0;                   // +0x30, the GL shader object name
    uint8_t m_unknown34 = 0;                    // +0x34, compiled flag
};

// The shader-code collection of ICodeShaderManager (+0x54): SIDedCollection<intrusive_ptr<IShaderCode>, ushort, false,
// SEmptyProperties, SValueTraits>, the canonical instance. Its ctor and dtor are the collection's own, 0x6e0804 / 0x6e0818
// and 0x6e1758 / 0x6e1828 (glitch-shared/sid_collection.hpp). Used through getId(name) (0xffff when not found),
// insert(name, code, managed), get(id)->value and removeUnused() (removeAll(false)).
using ShaderCodeCollection =
    glitch::core::detail::SIDedCollection<boost::intrusive_ptr<IShaderCode>, uint16_t, false,
                                          glitch::core::detail::sidedcollection::SEmptyProperties,
                                          glitch::core::detail::sidedcollection::SValueTraits>;

// IShaderManager base is not reconstructed; only the code-collection part of
// the manager is modelled (+0x54 in the original).
class ICodeShaderManager {
public:
    ICodeShaderManager() = default;           // 0x6e0ca4 (C1), 0x6e0cf0 (C2)
    virtual ~ICodeShaderManager() = default;  // 0x6e176c (D1), 0x6e17d8 (D2)

    boost::intrusive_ptr<IShaderCode> getShaderCode(const char* name) const;  // 0x6e1b08
    void addShaderCode(const boost::intrusive_ptr<IShaderCode>& code);       // 0x6e1d70
    void removeUnusedShaderCodes();                                          // 0x6e2028
    void removeUnusedShaders();                                              // 0x6e2034

    // 0x6e0a3c. Partial: the original opens the additional config file through the driver,
    // which is not ported. The text (if loaded) replaces '^' with newline. Until a text is
    // loaded, m_additionalConfigLength stays -1 and the call is retried.
    void initAdditionalConfig(const char* name);

    // 0x6e0b64. Partial: returns std::string instead of a process-buffer char*. Parts are
    // joined in the original order (a, b, c, then the additional config text when loaded).
    // outLength (when not null) receives the total length.
    std::string makeShaderCodeName(const char* a, uint32_t aLength, const char* b, uint32_t bLength,
                                   const char* c, uint32_t cLength, uint32_t* outLength) const;
    // 0x6e0c10. Partial: same as the overload above; null b or c count as empty.
    std::string makeShaderCodeName(const char* a, const char* b, const char* c, uint32_t* outLength) const;

    ShaderCodeCollection m_shaderCodes;  // +0x54

    // Additional config text: +0x7c is the text pointer (null when not loaded) and +0x80 the
    // length, -1 until a text is loaded. Only the flag and the text are modelled here.
    std::string m_additionalConfig;               // +0x7c
    bool m_hasAdditionalConfig = false;           // +0x7c non-null
    int32_t m_additionalConfigLength = -1;        // +0x80
};

}  // namespace glitch::video
