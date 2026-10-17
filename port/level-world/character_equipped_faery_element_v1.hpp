#pragma once
#include "character_current_spell_v1.hpp"

namespace dh2::character_equipped_faery_element_v1 {
// Adam c3ae797 two-call algorithm, over the existing single save/table owner.
using SavedBindings=character_current_spell_v1::SavedBindings;
using FaeryRow=character_faery_selection::FaeryRow;
enum class Operation : std::uint32_t {selected_faery,faery_row};
struct Request {Operation operation;std::uint32_t id;std::int32_t difficulty;std::uintptr_t character;};
struct Response {std::int32_t selected;const FaeryRow* row;};
struct Services {
    void* context;
    // Zero is normal return. Row backing stays live through the signed word+8
    // read, after the callback. Errors/throws retain completed provider effects.
    int (*invoke)(void*,const Request*,Response*);
};
struct Result {std::uintptr_t character;std::uint32_t selected,calls,complete;std::int32_t element;Operation last_operation;};
enum class Status : std::int32_t {complete=1,invalid_argument=-1,provider_failed=-2};
Status query(std::uintptr_t,const Services*,Result*);
Services saved_services(SavedBindings*) noexcept;
struct Bindings {std::uintptr_t character;Services services;};
int equipped_faery_element_v1(void*,const dh2_script_value*,std::uint32_t,
                             dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t) noexcept;
// Arguments are ignored exactly. Full-width captured Character; one owning
// thread, no same-output reentry or owner retirement/rebinding. Distinct calls
// may nest. All borrowed save slots, tables/constants, provider context, returned
// rows and VM bindings stay live through synchronous return and VM close.
// Output/error storage must not alias opaque provider backing. Null saves yield
// source selection0 without reading difficulty; GetCharFaery remains mandatory.
}
