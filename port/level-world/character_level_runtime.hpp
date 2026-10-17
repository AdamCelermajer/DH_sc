#pragma once
#include "character_script_set_level.hpp"
#include "character_regeneration.hpp"
#include "character_init_hp_mp.hpp"
#include "debug_switches_runtime.hpp"
#include "../game-data/properties.hpp"
extern "C" {
#include "../pydata-constants/constants.h"
}
#include <map>
#include <memory>
#include <string>

namespace dh2::character_level_runtime {
struct DesignBinding {std::uintptr_t manager;const dh2_pycst_view* data;};
struct Storage {
    std::uintptr_t character,properties;
    std::int32_t* base;data::PropertyView* view;
    const data::ClassRow* classes;std::uint32_t class_count;
    const DesignBinding* designs;std::uint32_t design_count;
    debug_switches::Globals* debug_globals;
    const debug_switches::Services* debug_services;
};
struct NumberServices {
    void* context;
    // Actual fresh argument0 binary32 bits; zero success. No pre-shifting or
    // interpretation as a semantic Level. The original Lua ToFixed supplies it.
    std::int32_t (*read_number)(void*,std::uintptr_t argument,std::uint32_t* bits);
};
enum class Status : std::int32_t {
    complete,invalid_argument,busy,number_unavailable,number_failed,
    unsupported_numeric,design_missing,class_failed,debug_failed,
    property_failed,allocation_failed,source_failed
};
struct Result {
    character_script_set_level::Result level;
    character_regeneration::Result hp,mp;
    std::uint32_t class_result,number_reads,design_reads,hp_attempted,mp_attempted;
    character_init_hp_mp::Result init;
};
class Runtime {
public:
    Runtime()=default;
    Runtime(const Runtime&)=delete;
    Runtime& operator=(const Runtime&)=delete;
    Status set_level(const Storage*,const character_script_set_level::Arguments*,
                     const character_script_set_level::Globals*,const NumberServices*,Result*);
    // Convenience for the VM's already supplied fixed-point float callback.
    // Its two source reads return the same retained value. Full set_level above
    // supports genuinely fresh argument reads when the native caller has them.
    Status set_level_fixed(const Storage*,const character_script_set_level::Globals*,float raw,Result*);
    // Source _InitHpMp: restore HP then MP with -1 using the existing live
    // regeneration, property, Debug and file providers. No Level/class update.
    Status initialize_hp_mp(const Storage*,Result*);
    std::size_t retained_strings()const{return strings_.size();}
private:
    struct Context;
    static std::int32_t level_operation(void*,character_script_set_level::Character*,const character_script_set_level::Request*,std::uint32_t*);
    static std::int32_t regeneration_operation(void*,const character_regeneration::Request*,character_regeneration::Reply*);
    bool busy_=false;
    std::map<std::uintptr_t,std::unique_ptr<std::string>> strings_;
};
// Native adapter composition over already reconstructed original callers and
// arithmetic/class/property kernels: no additional original caller-body claim.
// Numeric helper accepts finite binary32 in [-2^31,2^31), truncating toward zero;
// other helper inputs reject explicitly before the source Level store.
// Storage/Argument/Global/Services/Result controls remain live and disjoint. The
// borrowed base/view, 224-word sheets, class rows/formulas, design bindings/view/
// bytes, debug owners and provider context remain live on one owning thread.
// Source identity and backing/control addresses cannot change during the call;
// sheet values, arguments and selected application/design/debug owners may.
// Class views must be retained separately from renderer load_scene's local table.
// Direct C class recalculation mutates live base/resolved storage; it is not the
// cached copy/commit wrapper. Failures retain the source Level store and all prior
// class/debug/property effects. No extra regeneration, rollback or cleanup.
// String backing left after failed source query is owned by this Runtime until
// its destruction; do not destroy an active Runtime or reenter it. Independent
// Runtime owners may nest. Debug FS/save effects use actual Runtime providers;
// successful no-op IO is not valid. Runtime does not invent host or level facts.
} // namespace dh2::character_level_runtime
