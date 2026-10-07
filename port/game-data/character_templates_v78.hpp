#pragma once
#include "data.hpp"
#include <memory>
#include <string>
#include <vector>
namespace dh2::data {
struct CharacterTemplateRowV78 {
 std::vector<std::int32_t> char_info;
 //Consumer projection for original strh13c8. Full IDs remain above for the
 //source precache producer; this immutable view owns no actor/cache state.
 std::vector<std::int16_t> selected_ids;
};
class CharacterTemplateTableV78 final {
 std::vector<CharacterTemplateRowV78> rows_;
 std::vector<std::string> names_;
 bool ready_{};
public:
 bool load(Bytes records,Bytes names,Bytes schema,std::string&);
 bool ready()const noexcept{return ready_;}
 const auto& rows()const noexcept{return rows_;}
 const auto& names()const noexcept{return names_;}
 std::int32_t find(const char*)const noexcept;
 bool preset(const std::string&,const std::int16_t*&,std::uint32_t&,std::string&)const;
 //Whole SafeGetCharPropsTemplateId3b36ec. Empty CString leaves source13ca
 //unchanged; a nonempty name performs actual ordered lookup and strh13ca.
 bool safe_template_id(const std::string&,std::int16_t& same13ca,std::int32_t&,std::string&)const;
};
}
