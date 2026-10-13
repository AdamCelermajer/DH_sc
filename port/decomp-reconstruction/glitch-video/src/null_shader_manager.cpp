#include "glitch-video/null_shader_manager.hpp"

#include "glitch-video/null_driver.hpp"

namespace glitch::video {

CNullShaderManager::CNullShaderManager(CNullDriver* driver) : m_driver(driver) {}

// 0x5b9bb4 _ZN6glitch5video18CNullShaderManager17createEmptyShaderEPKc
boost::intrusive_ptr<CNullShader> CNullShaderManager::createEmptyShader(const char*) {
    return boost::intrusive_ptr<CNullShader>(new CNullShader(m_driver), true);  // a new shader has count 0
}

// 0x5b9bf4 _ZN6glitch5video18CNullShaderManager12createShaderEPKc
// The original looks the name up in its collection and returns the stored
// shader when present. Otherwise it creates a shader with the next id and
// registers it through IShaderManager::addShader (base not modelled, so the
// registration is done inline here). Partial: base behaviour is approximated.
boost::intrusive_ptr<CNullShader> CNullShaderManager::createShader(const char* name) {
    auto it = m_ids.find(name);
    if (it != m_ids.end()) {
        return m_shaders[it->second];
    }
    uint16_t id = m_nextShaderId;
    auto shader = boost::intrusive_ptr<CNullShader>(new CNullShader(id, name, m_driver), true);  // a new shader has count 0
    m_ids[name] = static_cast<uint16_t>(m_shaders.size());
    m_shaders.push_back(shader);
    return shader;
}

// 0x5b9cec _ZN6glitch5video18CNullShaderManager19removeUnusedShadersEv
// The original calls SIDedCollection::removeAll(false) on the shader collection.
void CNullShaderManager::removeUnusedShaders() {
    m_ids.clear();
    m_shaders.clear();
}

}  // namespace glitch::video
