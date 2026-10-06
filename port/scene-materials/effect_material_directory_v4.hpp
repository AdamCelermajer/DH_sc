#pragma once
#include "material_compare_v4.hpp"
#include "shader_program_collection_v4.hpp"
#include "effect_render_pass_v4.hpp"
namespace dh2::scene {
struct EffectMaterialValueBorrowV4 {
 const std::uint8_t* bytes{};std::size_t byte_size{};
 const std::uintptr_t* texture_identities{};std::size_t texture_count{};
 const std::uint8_t* const* matrices68{};std::size_t matrix_count{};
};
using EffectMaterialReadV4=bool(*)(void*,const ShaderUniformReflectionV4&,
 EffectMaterialValueBorrowV4&,std::string&);
// Retained immutable comparison snapshot from the same material's current
// values. Every local reflected uniform requires its genuine producer.
// Globals are excluded exactly as source sortParameters/active lists require.
class EffectMaterialDirectoryV4 {
 struct Storage;std::unique_ptr<Storage> storage_;
public:
 EffectMaterialDirectoryV4();~EffectMaterialDirectoryV4();
 bool refresh(const std::shared_ptr<ShaderProgramRecordV4>&,
 const EffectRenderPassV4&,void*,EffectMaterialReadV4,std::string&);
 const MaterialCompareViewV4* view()const noexcept;
};
}
