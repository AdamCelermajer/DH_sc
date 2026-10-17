#include "player_skill_tables_adapter.hpp"
#include <stdexcept>
#include <limits>

namespace dh2::player_skill_tables_adapter {
namespace {
constexpr std::size_t limit=4096,string_limit=1048576;
void validate(const std::vector<data::IntegerListRow>& lists,std::size_t minimum) {
    if(lists.size()<minimum || lists.size()>limit) throw std::invalid_argument("missing/big source fallback list table");
    for(const auto& row:lists) if(row.members.size()>limit) throw std::invalid_argument("big source list");
}
}
Tables::Tables(data::SkillTables&& skills,data::FaeryTables&& faeries)
    :skills_(std::move(skills)),faeries_(std::move(faeries)) {
    validate(skills_.skill_lists,4);validate(faeries_.faery_lists,1);
    if(skills_.skills.size()>limit || faeries_.faeries.size()>limit)
        throw std::invalid_argument("big source record table");
    for(const auto& row:skills_.skills)
        if(row.script_length<0 || row.skill_icon_length<0 || row.script.size()>string_limit ||
           row.skill_icon.size()>string_limit || std::size_t(row.script_length)!=row.script.size() ||
           std::size_t(row.skill_icon_length)!=row.skill_icon.size())
            throw std::invalid_argument("source skill string/length mismatch");
    for(const auto& list:faeries_.faery_lists)
        lists_.push_back({0,static_cast<std::int32_t>(list.members.size()),list.members.data()});
    for(const auto& row:faeries_.faeries) {
        if(row.spell_script_length<0 || row.spell_script.size()>string_limit ||
           std::size_t(row.spell_script_length)!=row.spell_script.size())
            throw std::invalid_argument("source faery string/length mismatch");
        rows_.push_back({{0,static_cast<std::uint32_t>(row.description),static_cast<std::uint32_t>(row.elemental),
            static_cast<std::uint32_t>(row.model_file),static_cast<std::uint32_t>(row.name_id),
            static_cast<std::uint32_t>(row.spell_script_length),0,static_cast<std::uint32_t>(row.spell_type),
            static_cast<std::uint32_t>(row.type)}});
        scripts_.push_back(reinterpret_cast<std::uintptr_t>(row.spell_script.c_str()));
    }
    source_faeries_={lists_.data(),static_cast<std::uint32_t>(lists_.size()),rows_.data(),static_cast<std::uint32_t>(rows_.size())};
    names_={rows_.data(),scripts_.data(),scripts_.size()};
}
std::shared_ptr<const Tables> Tables::create(data::SkillTables&& skills,data::FaeryTables&& faeries,std::string& error) {
    error.clear();try{return std::shared_ptr<const Tables>(new Tables(std::move(skills),std::move(faeries)));}
    catch(const std::exception& failure){error=failure.what();return {};}
}
std::uint32_t Tables::skill_list_id(std::int32_t raw)const {
    return raw>=0 && std::size_t(raw)<skills_.skill_lists.size()?static_cast<std::uint32_t>(raw):3;
}
std::uint32_t Tables::faery_list_id(std::int32_t raw)const {
    return raw>=0 && std::size_t(raw)<faeries_.faery_lists.size()?static_cast<std::uint32_t>(raw):0;
}
const data::IntegerListRow* Tables::skill_list(std::int32_t raw)const {return &skills_.skill_lists[skill_list_id(raw)];}
const data::IntegerListRow* Tables::faery_list(std::int32_t raw)const {return &faeries_.faery_lists[faery_list_id(raw)];}
const data::SkillRow* Tables::skill(std::int32_t raw,std::uint32_t slot)const {
    const auto* list=skill_list(raw);if(slot>=list->members.size()) return nullptr;
    const auto id=list->members[slot];
    return id>=0 && std::size_t(id)<skills_.skills.size()?&skills_.skills[id]:nullptr;
}
} // namespace dh2::player_skill_tables_adapter
