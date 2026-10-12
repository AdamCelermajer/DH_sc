#pragma once
#include "class_tables.hpp"
namespace dh2::data {
struct PropertyRules {PropertySheet defaults{},types{};};
// Buff groups are supplied in the original map's ascending key order. Sheets
// within a group retain deque insertion order. They are read-only snapshots.
struct PropertyBuffGroup {const std::int32_t* const* sheets=nullptr;std::uint32_t count=0;};
struct PropertyView {
 const std::int32_t *defaults=nullptr,*types=nullptr,*base=nullptr;
 std::int32_t* saved=nullptr;
 const std::int32_t* gear=nullptr;
 std::int32_t* resolved=nullptr;
 const PropertyBuffGroup* groups=nullptr;
 std::uint32_t group_count=0;
};
struct PropertyState {PropertySheet base{},saved{},gear{},resolved{};};
bool load_property_rules(const CharacterTable&,PropertyRules&,std::string&);
PropertyView property_view(const PropertyRules&,PropertyState&);
void reset_properties(const PropertyRules&,PropertyState&,const PropertySheet* base=nullptr);
// This resolves sheets already constructed by their respective producers.
// Gear calculation, buff creation/timers and uncached class application remain
// separate reconstruction steps. Full recalculation preserves runtime-only
// values (type 8), matching RecalcProperty's fallthrough behavior.
bool recalc_properties(const PropertyRules&,PropertyState&,std::string&);
bool apply_class_uncached(const ClassTables&,std::int32_t,const PropertyRules&,PropertyState&,std::string&);
bool recalc_properties_with_class(const ClassTables&,const PropertyRules&,PropertyState&,std::string&);
}
// Return 0 on valid invocation, 1 on invalid input. Failed calls leave sheets
// unchanged. Resolve returns the raw fixed-point/OID value through result.
extern "C" unsigned dh2_property_resolve(dh2::data::PropertyView*,std::int32_t,std::int32_t* result);
extern "C" unsigned dh2_property_set(dh2::data::PropertyView*,std::int32_t,std::int32_t);
extern "C" unsigned dh2_property_add(dh2::data::PropertyView*,std::int32_t,std::int32_t);
extern "C" unsigned dh2_property_set_to_sheet(dh2::data::PropertyView*,std::int32_t,std::int32_t,std::int32_t* sheet);
extern "C" unsigned dh2_property_set_int(dh2::data::PropertyView*,std::int32_t,std::int32_t);
extern "C" unsigned dh2_property_validate(const dh2::data::PropertyView*);
