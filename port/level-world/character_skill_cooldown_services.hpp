#pragma once
#include "../adam-script-runtime/script_runtime.h"
#include <cstddef>
#include <cstdint>
namespace dh2::character_skill_cooldown_services {
struct Slot {std::uintptr_t instance;std::int32_t* timer_id_18;};
struct Services {
    void* context;
    std::uintptr_t character;
    // Original GetCharSkillList/GetCharFaeryList selected count, not slot count.
    // kind0 skills, kind1 spells; zero means real synchronous success.
    std::int32_t (*list_count)(void*,std::uintptr_t,std::uint32_t,std::uint32_t*);
    // Fresh source vector lookup. Null source slot returns {0,nullptr}.
    // Reject assertion-unsafe/out-of-backing indices; no modulo/fallback slot.
    std::int32_t (*slot)(void*,std::uintptr_t,std::uint32_t,std::uint32_t,Slot*);
    // Reached string/object Value::getNumber remains a genuine provider.
    // Native-width identities must not be narrowed/cast to a float here.
    std::int32_t (*number)(void*,const dh2_script_value*,float*);
};
// Borrow only: use the existing retained skill instance field18 and Character
// timer owner. This adapter does not start/update timers or own a VM/property.
// Captured services/character and argument backing remain live; count/backing
// cannot change. Elements/maps may mutate through callbacks. Captured slot and
// retired fields remain alive until synchronous return, including errors.
// Never overwrite controls/output or reenter same arguments/field during a
// callback. Independent invocations may nest. Errors preserve completed writes.
// Malformed returned field fails without diagnostic copying (it may alias the
// diagnostic buffer); callback failure still sets the required-service marker.
// Numeric conversion follows source f2uiz/f2iz saturation; type1 uses its raw
// source number field (source-value VM projection supplies canonical0/1).
// Missing providers/unsafe numeric slot domain return required-service marker.
// Source assert-recovery branches are excluded; no full496B caller-body claim.
int skill(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,
          std::uint32_t,std::uint32_t*,char*,std::size_t);
int spell(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,
          std::uint32_t,std::uint32_t*,char*,std::size_t);
}
