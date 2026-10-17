#pragma once
#include "../game-data/skill_tables.hpp"
#include "character_ai_set_skills_and_spells.hpp"
#include <memory>

namespace dh2::player_skill_tables_adapter {
// Retains our existing decoded tables. Source scalar words normalize only
// pointer/vtable slots; strings and lists stay in their real owned storage.
class Tables {
public:
    Tables(const Tables&)=delete;Tables& operator=(const Tables&)=delete;
    Tables(Tables&&)=delete;Tables& operator=(Tables&&)=delete;
    static std::shared_ptr<const Tables> create(data::SkillTables&&,data::FaeryTables&&,std::string&);
    const data::SkillTables& skills()const{return skills_;}
    const data::FaeryTables& faeries()const{return faeries_;}
    std::uint32_t skill_list_id(std::int32_t raw)const;
    std::uint32_t faery_list_id(std::int32_t raw)const;
    const data::IntegerListRow* skill_list(std::int32_t raw)const;
    const data::IntegerListRow* faery_list(std::int32_t raw)const;
    const data::SkillRow* skill(std::int32_t raw,std::uint32_t slot)const;
    const character_faery_selection::Tables& source_faeries()const{return source_faeries_;}
    const character_ai_set_skills_and_spells::FaeryBinding::FullWidthScriptNames& faery_names()const{return names_;}
private:
    Tables(data::SkillTables&&,data::FaeryTables&&);
    data::SkillTables skills_;data::FaeryTables faeries_;
    std::vector<character_faery_selection::FaeryListRow> lists_;
    std::vector<character_faery_selection::FaeryRow> rows_;
    std::vector<std::uintptr_t> scripts_;
    character_faery_selection::Tables source_faeries_{};
    character_ai_set_skills_and_spells::FaeryBinding::FullWidthScriptNames names_{};
};
// Storage adapter, no new complete original body. Selection uses the exact
// source signed selector/fallback (Skill3, Faery0); unsafe references fail.
} // namespace dh2::player_skill_tables_adapter
