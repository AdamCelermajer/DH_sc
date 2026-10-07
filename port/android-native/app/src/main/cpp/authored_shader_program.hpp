#pragma once
#include <android/asset_manager.h>
#include <GLES2/gl2.h>
#include <array>
#include "../../../../../engine-resources/gpu_object_owner_v43.hpp"

namespace dh2::android_ui {
// Modern GL adapter for the byte-exact authored shaders. It does not implement
// GameSWF's display list, timeline, ActionScript or original blend-state owner.
struct Program {
    resources::GpuObjectOwnerV43 ownership_v43;
    GLuint name=0;
    Program()=default;
    Program(const Program&)=delete;
    Program& operator=(const Program&)=delete;
    Program(Program&& other)noexcept{*this=std::move(other);}
    Program& operator=(Program&& other)noexcept{
      if(this!=&other){ownership_v43=std::move(other.ownership_v43);name=std::exchange(other.name,0);
       position=other.position;uv=other.uv;color=other.color;matrix=other.matrix;diffuse=other.diffuse;sampler=other.sampler;}
      return *this;
    }
    GLint position=-1,uv=-1,color=-1,matrix=-1,diffuse=-1,sampler=-1;
};
Program create(AAssetManager*,bool premultiplied,resources::ResourceScopeV37 scope=resources::ResourceScopeV37::shader);
void release(Program&) noexcept;
void draw_quad(const Program&,GLuint texture,const std::array<float,16>&,
               const std::array<float,4>& color,const std::array<float,4>& diffuse);
// GPU readback validates the actual source programs' additive color and
// multiplicative tint/alpha behavior. Throws on compilation/state/pixel errors.
// Must run on a newly created GL context before any retained render state.
void validate_pixels(const Program& normal,const Program& premultiplied);
}
