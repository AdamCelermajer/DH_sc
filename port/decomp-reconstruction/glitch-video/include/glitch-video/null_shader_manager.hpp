// glitch::video::CNullShaderManager: creates CNullShader objects for the null
// driver. The IShaderManager base and its SIDedCollection are stood in for by
// a name table here (see shader_code.hpp for the same approach).
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/null_shader.hpp"

namespace glitch::video {

class CNullDriver;

class CNullShaderManager {
public:
    explicit CNullShaderManager(CNullDriver* driver);  // base ctor not modelled
    ~CNullShaderManager() = default;                   // 0x5b9b80 (D1), 0x5b9cf0 (D0 base)

    // 0x5b9bb4: a new unnamed shader, the name argument is not used.
    boost::intrusive_ptr<CNullShader> createEmptyShader(const char* name);
    // 0x5b9bf4: returns the existing shader for the name or creates one.
    boost::intrusive_ptr<CNullShader> createShader(const char* name);
    void removeUnusedShaders();  // 0x5b9cec

    CNullDriver* m_driver = nullptr;  // +0x2c
    uint16_t m_nextShaderId = 0;      // +0x28, id given to the next named shader
    std::map<std::string, uint16_t> m_ids;
    std::vector<boost::intrusive_ptr<CNullShader>> m_shaders;
};

}  // namespace glitch::video
