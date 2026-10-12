#include "glitch-video/null_shader.hpp"

namespace glitch::video {

// 0x6e0554 _ZN6glitch5video11CNullShader30releaseDriverSpecificResourcesEv
void CNullShader::releaseDriverSpecificResources() {}

// 0x6e0558 _ZNK6glitch5video11CNullShader10bindShaderEv
void CNullShader::bindShader() const {}

}  // namespace glitch::video
