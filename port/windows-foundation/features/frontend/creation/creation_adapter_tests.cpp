#include "creation_adapter.hpp"
#include "dynamic_text_bindings.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::frontend::creation;
int main() {
    assert(class_choices().size() == 3);
    assert(class_choices()[0].profile_token == initial_class_token);
    assert(class_choices()[1].profile_token == "RoguePlayerBase");
    assert(class_choices()[2].profile_token == "MagePlayerBase");
    assert(!find_class("warrior") && !find_class("KnightPlayerBase1"));
    CreationRequest request{"staged-id", "A", "KnightPlayerBase"};
    auto absent = stage_creation(request);
    assert(!absent.ok() && !absent.character && absent.status == CreationStatus::unavailable_service);
    unsigned calls = 0;
    // Deliberately arbitrary provider fixture values, not source balance claims.
    CompleteCreationService fixture = [&](const CreationRequest& r, const ClassChoice& c, std::string&) {
        ++calls;
        assert(c.character_row == 263);
        CharacterState state;
        state.id = r.character_id; state.name = r.player_name; state.class_id = r.class_token;
        state.stats = CharacterStats{1, 71, 80, 19, 24, 9, 8, 7};
        state.experience = 23; state.gold = 42;
        state.inventory = {{"owned-1", "test-item", 2}};
        state.equipment = {{"test-slot", "owned-1"}};
        state.skills = {{"test-skill", 3}}; state.unlocks = {"test-unlock"};
        return std::optional<CharacterState>(state);
    };
    auto staged = stage_creation(request, fixture);
    assert(staged.ok() && calls == 1);
    assert(staged.character->stats.health == 71 && staged.character->gold == 42);
    assert(staged.character->inventory[0].quantity == 2 && staged.character->skills[0].rank == 3);
    request.class_token = "warrior";
    assert(stage_creation(request, fixture).status == CreationStatus::invalid_request && calls == 1);
    request.class_token = "KnightPlayerBase"; request.player_name = "bad\nname";
    assert(stage_creation(request, fixture).status == CreationStatus::invalid_request && calls == 1);
    request.player_name = "\xd7\x90\xd7\x93\xd7\x9d";
    assert(stage_creation(request, fixture).ok());
    auto wrong = [&](const CreationRequest& r, const ClassChoice& c, std::string& e) {
        auto state = fixture(r, c, e); state->class_id = "MagePlayerBase"; return state;
    };
    assert(stage_creation(request, wrong).status == CreationStatus::invalid_projection);
    auto invalid = [&](const CreationRequest& r, const ClassChoice& c, std::string& e) {
        auto state = fixture(r, c, e); state->stats.health = 999; return state;
    };
    assert(stage_creation(request, invalid).status == CreationStatus::invalid_projection);
    auto error_with_value = [&](const CreationRequest& r, const ClassChoice& c, std::string& e) {
        auto state = fixture(r, c, e); e = "native item effects unavailable"; return state;
    };
    auto failed = stage_creation(request, error_with_value);
    assert(failed.status == CreationStatus::service_failure && !failed.character);
    auto throws = [](const CreationRequest&, const ClassChoice&, std::string&) -> std::optional<CharacterState> {
        throw std::runtime_error("native service failed");
    };
    assert(stage_creation(request, throws).status == CreationStatus::service_failure);
    const auto rogue = class_text_bindings("RoguePlayerBase");
    assert(rogue.ok() && rogue.fields.size() == 4);
    assert(rogue.fields[0].source_text == "Rogue");
    assert(rogue.fields[1].source_text.find("^2Archer^r") != std::string::npos);
    assert(rogue.fields[1].html_text.find("<font color=\"#9ADEFF\">Archer</font>") != std::string::npos);
    assert(rogue.fields[2].field_path == "menu_SelectClass/btn_Confirm/text");
    assert(rogue.fields[3].source_text == "Choose a class");
    assert(!class_text_bindings("RoguePlayerBase_Archer").ok());
    assert(source_font_colors()[1] == 0x9CFF9A && source_font_colors()[2] == 0x9ADEFF);
    assert(source_font_colors()[3] == 0xD49AFF && source_font_colors()[0] == 0xFFFFFF);
    assert(source_localized_html("^1MORE GAMES!^r").html == "<font color=\"#9CFF9A\">MORE GAMES!</font>");
    assert(source_localized_html("<P><FONT COLOR=\"#FF3300\">^2Archer^r</FONT></P>").html ==
           "<P><FONT COLOR=\"#FF3300\"><font color=\"#9ADEFF\">Archer</font></FONT></P>");
    assert(source_localized_html("a^nb|c^r").html == std::string("a\nb\x11") + "c</font>");
    assert(source_localized_html("^3x^r^0y^r").html == "<font color=\"#D49AFF\">x</font><font color=\"#FFFFFF\">y</font>");
    assert(!source_localized_html(std::string(65537, 'x')).ok());
    auto saved = *staged.character;
    saved.class_id = "RoguePlayerBase_Archer";
    assert(!saved_profile_text_bindings(saved).ok());
    auto metadata = [](const CharacterState& shared, std::string&) {
        // Presentation fixture only; level/name remain owned by SAME shared state.
        return std::optional<SavedProfilePresentation>(SavedProfilePresentation{
            shared.id, shared.class_id, "Archer", 3, "fixture-location", 0, "fixture-date", true, true});
    };
    auto profile = saved_profile_text_bindings(saved, metadata);
    assert(profile.ok() && profile.fields.size() == 9);
    assert(profile.fields[0].source_text == saved.name);
    assert(profile.fields[1].source_text == "Archer");
    assert(profile.fields[2].source_text == "LEVEL 1");
    auto sparse_metadata=[](const CharacterState& shared,std::string&){
        return std::optional<SavedProfilePresentation>(SavedProfilePresentation{
            shared.id,shared.class_id,"",0,"",0,"",false,false});
    };
    auto sparse_saved=saved;
    sparse_saved.class_id="KnightPlayerBase";
    const auto sparse=saved_profile_text_bindings(sparse_saved,sparse_metadata);
    assert(sparse.ok()&&sparse.fields[0].source_text==saved.name);
    assert(sparse.fields[1].field_path=="menu_MainMenu/PlayerInfos/player_class/text"&&
           sparse.fields[1].source_text=="Warrior");
    const auto sparse_level=std::find_if(sparse.fields.begin(),sparse.fields.end(),[](const auto& field){return field.field_path=="menu_MainMenu/PlayerInfos/Hud_Level/text";});
    assert(sparse_level!=sparse.fields.end()&&sparse_level->source_text=="LEVEL 1");
    for(const char* leaf:{"Hud_Act","Hud_Location","Last_Save","Last_Save_Infos","DifficultyTitle","Difficulty"}) {
        const auto empty=std::find_if(sparse.fields.begin(),sparse.fields.end(),[&](const auto& field){return field.field_path==std::string("menu_MainMenu/PlayerInfos/")+leaf+"/text";});
        assert(empty!=sparse.fields.end()&&empty->source_text.empty());
    }
    struct ProfileClassCase { const char* token; const char* label; };
    constexpr ProfileClassCase profile_classes[]={
        {"KnightPlayerBase","Warrior"},
        {"RoguePlayerBase","Rogue"},
        {"MagePlayerBase","Mage"}
    };
    for(const auto& expected:profile_classes){
        CharacterState selected_profile=*staged.character;
        selected_profile.id=std::string("selected-")+expected.token;
        selected_profile.name=std::string("profile-")+expected.label;
        selected_profile.class_id=expected.token;
        auto source_class_metadata=[&](const CharacterState& same,std::string&){
            return std::optional<SavedProfilePresentation>(SavedProfilePresentation{
                same.id,same.class_id,expected.label,0,"",0,"",false,false});
        };
        const auto projected=saved_profile_text_bindings(selected_profile,source_class_metadata);
        assert(projected.ok()&&projected.fields.size()==9);
        assert(projected.fields[0].source_text==selected_profile.name);
        assert(projected.fields[1].field_path=="menu_MainMenu/PlayerInfos/player_class/text");
        assert(projected.fields[1].source_text==expected.label);
        assert(projected.fields[2].source_text=="LEVEL 1");
        for(const auto& field:projected.fields){
            if(expected.token!=std::string_view("KnightPlayerBase"))
                assert(field.source_text!="Warrior");
        }
    }
    const auto missing_specialization_label=saved_profile_text_bindings(saved,sparse_metadata);
    assert(!missing_specialization_label.ok()&&missing_specialization_label.fields.empty());
    saved.stats.level = 17;
    assert(saved_profile_text_bindings(saved, metadata).fields[2].source_text == "LEVEL 17");
    auto wrong_metadata = [&](const CharacterState& shared, std::string& error) {
        auto p = metadata(shared, error); p->character_id = "another-profile"; return p;
    };
    assert(!saved_profile_text_bindings(saved, wrong_metadata).ok());
    std::cout << "PASS creation adapter: exact class order, service gates, shared state staging, dynamic original labels/colors, SAME saved name/level, invalid projections\n";
}
