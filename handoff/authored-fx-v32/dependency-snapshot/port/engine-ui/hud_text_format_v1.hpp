#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::ui {
// Source VarArgs::Variant stores float at word0, integer at word1 and string
// pointer at word2. parseEx numeric directives read word0; ^s reads word2.
struct HudTextVariantV1 {float number{};std::int32_t integer{};const char* text{};};
enum HudTextOperationV1:std::uint32_t {
 hud_text_constant_v1=1,hud_text_integer_string_v1=2,
 hud_text_version_v1=3,hud_text_title_v1=4,hud_text_pack_v1=5
};
struct HudTextRequestV1 {std::uint32_t operation{},limit{};std::int32_t value{};std::uint32_t flag{};const char* group{};const char* key{};};
struct HudTextResponseV1 {std::int32_t value{};const char* text{};};
struct HudTextServicesV1 {void* context{};bool(*invoke)(void*,const HudTextRequestV1&,HudTextResponseV1&,std::string&){};};
// Complete parseEx control/arithmetic domain, with explicit localized defaults,
// application version/title and fresh current-pack providers. Appends to output.
// Returned changed flag is source's pipe flag, not "formatting occurred".
// Malformed bounded input rejects before services; a required provider failure
// preserves any already-appended source prefix. Output UTF transform runs last.
bool hud_text_parse_ex_v1(const char* input,const HudTextVariantV1* values,
 std::size_t count,const HudTextServicesV1&,std::string& output,bool& changed,
 std::string& error);
} // namespace dh2::ui
