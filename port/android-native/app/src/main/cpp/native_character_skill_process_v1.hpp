#pragma once

#include "native_ghost_skills.hpp"
#include "native_ghost_skill_update_v1.hpp"
#include "../../../../../../port/level-world/character_external_script_process_v1.hpp"
#include "../../../../../../port/level-world/character_runtime_factory_v1.hpp"
#include "../../../../../../port/level-world/ais_external_init_vcb.hpp"
#include "../../../../../../port/level-world/character_ai_initialization.hpp"
#include "../../../../../../port/level-world/character_ai_update_all_skills.hpp"

namespace dh2::native::character_skill_process_v1 {

namespace factory = dh2::character_runtime_factory_v1;
namespace process = dh2::character_external_script_process_v1;

// Renderer-owned facts needed by the existing Ghost skill runtime. The
// adapter borrows every pointer and the canonical Record's AIS Session; it
// owns no VM, vectors, skill instances, Character, or timer state.
struct Bindings {
    ghost_skills::Runtime* runtime{};
    character_ai_initialization::State* ai{};
    data::PropertyView* properties{};
    const data::SkillTables* skill_tables{};
    const data::FaeryTables* faery_tables{};
    std::string* script_path{};
    std::uint32_t assert_level{};
    const dh2_pycst_view* faery_constants{};
    debug_switches::Globals* debug_globals{};
    const debug_switches::Services* debug_services{};
    void* script_loader_context{};
    int (*load_script)(void*, monster_external_script::Session&,
                       const char*, std::size_t, bool*){};
};

struct Result {
    ghost_skills::Result preparation{};
    ghost_skill_update_v1::Result last_script_update{};
    character_ai_update_all_skills::Result update{};
};

class Adapter {
public:
    void bind(const Bindings& bindings) noexcept { bindings_ = bindings; }
    void services(process::SkillServices& output) noexcept;
    const Result& result() const noexcept { return result_; }
    const std::string& error() const noexcept { return error_; }

private:
    static int prepare(void*, factory::Record&, monster_external_script::Session&,
                       process::set_skills::Result*);
    static int update(void*, factory::Record&, monster_external_script::Session&,
                      process::update_skills::Result*);
    static int load_script(void*, const char*, std::size_t, bool*);
    static int call_script(void*, const char*, const dh2_script_value*, std::uint32_t);
    static int initialize_vcb(void*, std::uintptr_t);
    static int contains_alias(void*, ais_external_init_vcb::State*, const char*, bool*);
    static std::int32_t update_operation(
        void*, character_ai_update_all_skills::State*,
        const character_ai_update_all_skills::Request*,
        character_ai_update_all_skills::Response*);
    bool valid(factory::Record&, monster_external_script::Session&) const noexcept;

    Bindings bindings_{};
    factory::Record* active_record_{};
    monster_external_script::Session* active_session_{};
    Result result_{};
    std::string error_;
};

} // namespace dh2::native::character_skill_process_v1
