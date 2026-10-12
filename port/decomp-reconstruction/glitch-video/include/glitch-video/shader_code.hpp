// glitch::video::IShaderCode and the code-shader manager. The shader-code
// collection is an SIDedCollection in the original; it is stood in for here by
// a name-to-index table, so lookups keep the 0xffff "not found" convention.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "glitch-video/deps/intrusive_ptr.hpp"

namespace glitch::io {
class IAttributes;
}  // namespace glitch::io

namespace glitch::video {

class IShaderCode {
public:
    explicit IShaderCode(const char* name);  // 0x6e2130 (C1), 0x6e2178 (C2)
    virtual ~IShaderCode() = default;        // 0x6e207c (D1), 0x6e20e4 (D2)

    std::string m_name;   // +0x08, std::string built from the name argument
    uint32_t m_refCount = 0;  // +0x04, set to 0 by the ctor
};

class CProgrammableGLDriver;

class CGLSLShaderCode : public IShaderCode {
public:
    // 0x6df690 (C1) / 0x6df6e4 (C2). Partial: the base is built with an empty name. The driver
    // is kept at +0x2c; the word at +0x30 and the byte at +0x34 are zeroed (not identified).
    explicit CGLSLShaderCode(CProgrammableGLDriver* driver);
    void serializeAttributes(io::IAttributes* attributes) const;  // 0x6df398, empty
    void deserializeAttributes(io::IAttributes* attributes);      // 0x6df39c, empty

private:
    CProgrammableGLDriver* m_driver = nullptr;  // +0x2c
    uint32_t m_unknown30 = 0;                   // +0x30, not identified
    uint8_t m_unknown34 = 0;                    // +0x34, not identified
};

// Stand-in for SIDedCollection<intrusive_ptr<IShaderCode>, uint16_t, ...>.
class CShaderCodeManager {
public:
    CShaderCodeManager() = default;  // 0x6e0804 / 0x6e0818
    ~CShaderCodeManager() = default; // 0x6e1758 / 0x6e1828

    static constexpr uint16_t kInvalidId = 0xffff;
    uint16_t getId(const char* name) const;
    void insert(const char* name, const boost::intrusive_ptr<IShaderCode>& code);
    boost::intrusive_ptr<IShaderCode> at(uint16_t id) const;
    void removeAll();

private:
    std::map<std::string, uint16_t> m_ids;
    std::vector<boost::intrusive_ptr<IShaderCode>> m_codes;
};

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

    CShaderCodeManager m_shaderCodes;  // +0x54
};

}  // namespace glitch::video
