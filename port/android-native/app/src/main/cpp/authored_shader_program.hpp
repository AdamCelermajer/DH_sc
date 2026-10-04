#pragma once
#include <android/asset_manager.h>
#include <GLES2/gl2.h>
#include <array>

namespace dh2::android_ui {
// Modern GL adapter for the byte-exact authored shaders. It does not implement
// GameSWF's display list, timeline, ActionScript or original blend-state owner.
struct Program {
    GLuint name=0;
    GLint position=-1,uv=-1,color=-1,matrix=-1,diffuse=-1,sampler=-1;
};
Program create(AAssetManager*,bool premultiplied);
void release(Program&) noexcept;
void draw_quad(const Program&,GLuint texture,const std::array<float,16>&,
               const std::array<float,4>& color,const std::array<float,4>& diffuse);
// GPU readback validates the actual source programs' additive color and
// multiplicative tint/alpha behavior. Throws on compilation/state/pixel errors.
// Must run on a newly created GL context before any retained render state.
void validate_pixels(const Program& normal,const Program& premultiplied);
}
