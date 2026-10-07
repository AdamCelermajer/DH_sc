#pragma once
#include "localization.hpp"
#include "hud_text_format_v1.hpp"
namespace dh2::ui {
struct LocalizationArgumentV1;
struct HudTextEnvironmentV1 {
 LocalizationServices localization;
 void* application{};
 bool(*version)(void*,std::uint32_t limit,bool include_build,std::string&,std::string&){};
 bool(*title)(void*,std::uint32_t limit,std::string&,std::string&){};
};
// Versioned single owned StringManager cache. Derived from the frozen source
// reconstruction, adding raw integer lookup and complete parseEx without
// changing Localization or sharing a second incoherent cache with it.
class HudTextV1 {
 public:
 // Original common_text PyData array, names and schema, in serialized order.
 // Malformed input rejects atomically. Buffers are copied; no borrowed backing.
 bool load(LocalizationBytes records,LocalizationBytes names,LocalizationBytes schema,std::string&);
 bool switch_pack(std::int32_t pack,bool unload_old,std::string&); // -1..8
 bool preload(std::uint32_t pack,std::uint32_t sheet,bool force,const LocalizationServices&,std::string&);
 bool integer_string(std::int32_t,const LocalizationServices&,std::string&,bool& is_null,std::string&);
 bool parse_ex(const char*,const HudTextVariantV1*,std::size_t,const HudTextEnvironmentV1&,std::string&,bool& changed,std::string&);
  bool native_string(const std::string& symbol,const LocalizationServices&,LocalizationResult&,std::string&);
  bool parsed_string_v4(const std::string&,const std::vector<LocalizationArgumentV1>&,
                        const HudTextEnvironmentV1&,std::string&,std::string&);
 // Additive original StringManager getters for MenuBase localization. These
 // borrow THIS cache and preserve native_string/parseEx behavior unchanged.
 std::int32_t source_symbol_pack_v1()const noexcept{return 8;}
 bool source_string_index_v1(std::uint32_t sheet,std::uint32_t index,std::int32_t pack,
                             const LocalizationServices&,std::string&,std::string&);
 bool source_string_count_v1(std::uint32_t sheet,std::int32_t pack,std::int32_t&,std::string&)const;
 std::int32_t pack()const{return pack_;}
 const std::vector<std::string>& pack_names()const{return pack_names_;}
 const std::string& sheet_name(std::uint32_t pack,std::uint32_t sheet)const;
 const std::string& sheet_filename(std::uint32_t pack,std::uint32_t sheet)const;
 std::size_t loaded_sheets()const;
 private:
 struct Sheet {std::string filename,name;std::vector<std::string> strings;bool loaded{};};
 std::array<std::array<Sheet,37>,9> sheets_{};std::vector<std::string> pack_names_;
 std::int32_t pack_{-1};bool ready_{},busy_{};
 bool preload_impl(std::uint32_t,std::uint32_t,bool,const LocalizationServices&,std::string&);
 bool index(std::uint32_t sheet,std::uint32_t index,std::int32_t pack,const LocalizationServices&,std::string&,std::string&);
 bool id(std::uint32_t,const LocalizationServices&,std::string&,std::string&);
 bool defaults(const LocalizationServices&,std::string&);
};
} // namespace dh2::ui
