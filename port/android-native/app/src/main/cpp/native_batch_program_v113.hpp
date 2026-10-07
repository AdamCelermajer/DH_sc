#pragma once
#include <GLES2/gl2.h>
#include <shader_sources.hpp>
#include <shader_reflection_v4.hpp>
#include <native_batch_compiler_v111.hpp>
namespace model_renderer {
struct NativeBatchAttributeBindingV113 {
 std::uint32_t source_slot{};GLint location{};std::uint32_t components{};
};
struct NativeBatchUniformValueV113 {
 const float* floats{};const std::int32_t* integers{};std::size_t elements{};
 bool skip{}; //actual source commitLightParameter(NULL/unsupportedsemantic) returns without GL upload.
};
using NativeBatchUniformReadV113=std::function<bool(const dh2::scene::ShaderUniformReflectionV4&,NativeBatchUniformValueV113&,std::string&)>;
//Actual linked original shader, reflected uniforms and source stream mapping.
//The existing source geometry transport owns all buffers/textures and Scene.
class NativeBatchProgramV113 {
 GLuint program_{};
 std::shared_ptr<dh2::resources::ContextResourceBudgetV37> budget_;
 dh2::resources::ResourceTokenV37 program_token_;
 std::uint64_t generation_{};
 std::vector<dh2::scene::ShaderUniformReflectionV4> uniforms_;
 std::vector<NativeBatchAttributeBindingV113> attributes_;
public:
 ~NativeBatchProgramV113(){release(true);}
 bool initialize(const dh2::scene::EffectRenderPassV4&,const dh2::scene::ShaderSourcePack&,
  std::uint32_t actual_driver_flags,const std::string* actual_shader_config,
  std::shared_ptr<dh2::resources::ContextResourceBudgetV37>,std::string&);
 bool bind_uniforms(const NativeBatchUniformReadV113&,std::string&)const;
 const auto& uniforms()const noexcept{return uniforms_;}
 const auto& attributes()const noexcept{return attributes_;}
 GLuint program()const noexcept{return program_;}
 void release(bool context_lost)noexcept;
};
struct NativeBatchGpuStateV113 {
 std::shared_ptr<NativeBatchProgramV113> program;
 std::array<GLuint,18> attributes{};
 std::array<std::uint32_t,18> widths{};
 std::shared_ptr<const dh2::world::NativeBatchMaterialValuesV113> material;
 std::shared_ptr<dh2::world::NativeMaterialLightsV113> lights;
 std::map<std::string,std::vector<std::int32_t>> sampler_units;
 std::vector<GLuint> sampler_textures;
};
}
