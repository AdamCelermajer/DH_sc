#pragma once
#include "shader_reflection_v4.hpp"
#include "effect_render_pass_v4.hpp"
#include <map>
#include <memory>
namespace dh2::scene {
struct ShaderProgramRecordV4 {
 std::uint16_t collection_id{};
 std::shared_ptr<void> same_program;
 std::vector<ShaderUniformReflectionV4> parameters;
 std::uint32_t global_count{};
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(same_program.get());}
};
using ShaderProgramCreateV4=bool(*)(void*,std::uint16_t,const std::string&,
 std::shared_ptr<void>&,std::vector<ShaderUniformReflectionV4>&,std::string&);
std::string effect_program_cache_name_v4(const EffectRenderPassV4&);
// Retained source name collection for the supported append-only creation path.
// Same source program pass key retrieves ONE GL owner shared by every resource.
// Actual factory creates/links/introspects the program before publication.
class ShaderProgramCollectionV4 {
 std::map<std::string,std::shared_ptr<ShaderProgramRecordV4>> records_;
 std::uint16_t next_{},count_{};
public:
 bool get_or_create(const std::string& actual_cache_name,void* context,
 ShaderProgramCreateV4,std::shared_ptr<ShaderProgramRecordV4>&,std::string&);
 std::uint16_t next_id()const noexcept{return next_;}
 std::uint16_t count()const noexcept{return count_;}
};
}
