#include "quest_text_resolver_v1.hpp"

namespace dh::foundation {
namespace {
constexpr std::int32_t source_none_string=1835016;
bool localized_if_present(const CharacterQuestTextV1& resolver,
    const CharacterState& character,std::int32_t id,
    std::optional<std::string>& out,std::string& error) {
    if(!resolver)return true;
    std::string text;
    if(!resolver(character,id,text,error))return false;
    out=std::move(text);return true;
}
}

bool bind_source_quest_text_resolver_v1(
    dh2::ui::HudTextV1& text,const dh2::ui::HudTextEnvironmentV1& environment,
    CharacterQuestTextV1& out,std::string& error) {
    CharacterQuestTextV1 staged=[&text,&environment](
        const CharacterState&,std::int32_t id,std::string& value,
        std::string& callback_error) {
        if(id<0){callback_error="Source Quest StringID resolver received a negative ID outside a Quest fallback branch";return false;}
        std::string localized;bool is_null=false;
        if(!text.integer_string(id,environment.localization,localized,is_null,callback_error))return false;
        if(is_null){callback_error="Source Quest StringID unexpectedly resolved to null";return false;}
        value=std::move(localized);callback_error.clear();return true;
    };
    out=std::move(staged);error.clear();return true;
}

bool resolve_source_quest_page_text_v1(
    const dh2::data::QuestDefinitionV51& definition,const CharacterState& character,
    const CharacterQuestTextV1& resolver,SourceQuestPageTextV1& out,
    std::string& error) {
    SourceQuestPageTextV1 staged;
    auto source_text=[&](std::int32_t id,std::optional<std::string>& value) {
        if(id<0||id==source_none_string){value="not specified";return true;}
        return localized_if_present(resolver,character,id,value,error);
    };
    if(!source_text(definition.text_fields[0],staged.title)||
       !source_text(definition.text_fields[1],staged.pre_description))return false;

    const auto objective_id=definition.text_fields[2];
    if(objective_id==source_none_string) {
        std::string joined;bool has_description=false,unavailable=false;
        for(const auto& objective:definition.objectives) {
            if(objective.description<=0)continue;
            std::optional<std::string> line;
            if(!localized_if_present(resolver,character,objective.description,line,error))return false;
            if(!line){unavailable=true;continue;}
            if(line->empty())continue;
            if(has_description)joined.push_back('\n');
            joined+=*line;has_description=true;
        }
        // No objective needs text, or the source list is empty: the native
        // ObjectiveList::GetDesc returns the empty std::string.
        if(!unavailable)staged.objective_description=std::move(joined);
    } else if(objective_id<0) {
        staged.objective_description="not specified";
    } else {
        if(!localized_if_present(resolver,character,objective_id,
                                 staged.objective_description,error))return false;
    }

    if(definition.text_fields[3]==source_none_string)
        staged.post_description="not specified";
    else
        staged.post_description=std::string{};

    out=std::move(staged);error.clear();return true;
}

} // namespace dh::foundation
