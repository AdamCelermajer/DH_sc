#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::data {
struct DesignNames16 {const char* const* names;std::uint32_t count,reserved;};
struct DesignRegistration24 {const char* group;const DesignNames16* members;std::uint64_t reserved;};
struct DesignRegistry16 {const DesignRegistration24* registrations;std::uint32_t count,reserved;};
static_assert(sizeof(DesignNames16)==16&&sizeof(DesignRegistration24)==24&&sizeof(DesignRegistry16)==16);
// Owns copied registration keys; descriptors/names/pointer arrays remain borrowed
// from decoded tables. The view survives until the next registration/move.
class GameDesignTables {
 struct Entry {std::string group;const DesignNames16* members;};
 std::vector<Entry> entries_;
 std::vector<DesignRegistration24> projection_;
public:
 GameDesignTables()=default;
 GameDesignTables(const GameDesignTables&)=delete;
 GameDesignTables& operator=(const GameDesignTables&)=delete;
 GameDesignTables(GameDesignTables&&)=default;
 GameDesignTables& operator=(GameDesignTables&&)=default;
 bool register_table(const char* group,const DesignNames16*,std::string& error);
 DesignRegistry16 view() const;
};
}
// Exact strcmp/first match, source miss -1. 0 delivery,1 bounded malformed input.
// Strings are borrowed terminated C strings. No case folding or intern equality.
extern "C" int dh2_game_design_find(std::int32_t*,const dh2::data::DesignNames16*,const char*);
// Persistent script_design_bindings-compatible kind1 provider. Both source
// GetPyStruct and GetPyOID use this namespace. Last registration wins; kind0
// is a different backend and returns delivery failure here. Output is atomic
// on malformed delivery. Missing group/member is successful delivery of -1.
extern "C" int dh2_game_design_tables_lookup(void*,std::uint32_t,const char*,const char*,std::int32_t*);
