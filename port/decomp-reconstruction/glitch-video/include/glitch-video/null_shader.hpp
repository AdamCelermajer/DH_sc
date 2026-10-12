// glitch::video::CNullShader: shader stub used by the null driver. Only the
// two empty hooks are reconstructed here.
#pragma once

namespace glitch::video {

class CNullShader {
public:
    void releaseDriverSpecificResources();  // 0x6e0554
    void bindShader() const;                // 0x6e0558
};

}  // namespace glitch::video
