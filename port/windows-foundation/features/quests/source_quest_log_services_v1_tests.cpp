#include "source_quest_log_services_v1.hpp"
#include "source_quest_page_v1.hpp"
#include "character_quest_page_v1.hpp"
#include "runtime_quest_menu_v1.hpp"
#include "runtime_quest_menu_art_v1.hpp"
#include "source_quest_menu_page_provider_v1.hpp"
#include "../character_menu/source_composition.hpp"
#include "../../../game-data/quest_persistence_v51.hpp"
#include "../../../script-runtime/script_constants.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>

using namespace dh::foundation;
namespace fs = std::filesystem;

static void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
static std::vector<std::uint8_t> read(const fs::path& path) {
    std::ifstream stream(path, std::ios::binary);
    check(bool(stream), path.string().c_str());
    return {std::istreambuf_iterator<char>(stream), {}};
}

struct Fixture {
    fs::path source_data;
    dh2_script_constants* constants{dh2_script_constants_create()};
    dh2::character::DebugSwitches* debug{dh2_character_debug_create()};
    std::map<std::uintptr_t, bool> leases;
    std::uintptr_t next_lease{1};
    unsigned debug_queries{};
    const CharacterState* current_player_profile{};
    ~Fixture() { dh2_character_debug_destroy(debug); dh2_script_constants_destroy(constants); }

    static int debug_open(void*, const char* name, std::uintptr_t* handle) {
        if (!name || std::strcmp(name, "DebugSwitches.savegame")) return -1;
        *handle = 0; // Original FileManager not-found branch.
        return 0;
    }
    static int debug_close(void*, std::uintptr_t) { return -1; }
    static bool text_open(void* raw, const char* uri, bool& found,
                          std::vector<std::uint8_t>& bytes,
                          std::uintptr_t& lease, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        if (!uri) { error = "missing source text URI"; return false; }
        const auto path = self.source_data / uri;
        std::ifstream stream(path, std::ios::binary);
        if (!stream) { found = false; lease = 0; error.clear(); return true; }
        bytes.assign(std::istreambuf_iterator<char>(stream), {});
        found = true; lease = self.next_lease++; self.leases.emplace(lease, true);
        error.clear(); return true;
    }
    static bool text_close(void* raw, std::uintptr_t lease, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        if (self.leases.erase(lease) != 1) { error = "invalid StringManager lease"; return false; }
        error.clear(); return true;
    }
    static bool debug_text(void* raw, const char* key, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw); ++self.debug_queries;
        if (dh2_character_debug_load(self.debug, &debug_files(self)) != 1) {
            error = "actual DebugSwitches load failed"; return false;
        }
        std::uint32_t ignored{};
        if (dh2_character_debug_get(&ignored, self.debug, key, &debug_files(self)) != 1) {
            error = "actual StringManager trace switch query failed"; return false;
        }
        error.clear(); return true;
    }
    static bool constant(void* raw, const char* group, const char* key,
                         std::uint32_t& value, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        std::int32_t signed_value{};
        if (dh2_script_constants_get(self.constants, group, key, &signed_value) != 0) {
            error = std::string("missing source constant ") + group + "." + key;
            return false;
        }
        std::memcpy(&value, &signed_value, sizeof value); error.clear(); return true;
    }
    static bool player_character(void* raw,std::uintptr_t& out,std::string& error){
        auto* self=static_cast<Fixture*>(raw);out=reinterpret_cast<std::uintptr_t>(self->current_player_profile);
        error.clear();return true;
    }
    static bool player_name(void* raw,std::uintptr_t actual,std::string& out,std::string& error){
        auto* self=static_cast<Fixture*>(raw);
        if(!self->current_player_profile||actual!=reinterpret_cast<std::uintptr_t>(self->current_player_profile)){
            error="Fixture StringManager player borrower changed";return false;
        }
        out=self->current_player_profile->name;error.clear();return true;
    }
    static dh2::character::DebugFileServices24& debug_files(Fixture& self) {
        static thread_local dh2::character::DebugFileServices24 files;
        files = {&self, &Fixture::debug_open, &Fixture::debug_close};
        return files;
    }
};

int main(int argc, char** argv) {
    try {
        if (argc != 6) return 2;
        Fixture fixture; check(fixture.constants && fixture.debug, "native owners unavailable");
        const fs::path quest_cache(argv[1]);
        fixture.source_data = fs::path(argv[2]);
        const fs::path assets(argv[3]);
        const fs::path authored_movie(argv[4]);
        const fs::path authored_actions(argv[5]);
        auto common_constants = read(fixture.source_data / "pydata/common_text_pycst.bin");
        auto quest_constants = read(assets / "data/v2quests_pycst.bin");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(fixture.constants, common_constants.data(),
              static_cast<std::uint32_t>(common_constants.size()), &reload) == 0,
              "source common-text constants load failed");
        check(dh2_script_constants_load(fixture.constants, quest_constants.data(),
              static_cast<std::uint32_t>(quest_constants.size()), &reload) == 0,
              "source quest constants load failed");

        auto quest_array = read(quest_cache / "v2quests_pyarray.bin");
        auto quest_names = read(quest_cache / "v2quests_pyarraynames.bin");
        auto tables = std::make_shared<dh2::data::QuestTablesPersistenceV51>();
        std::string error;
        check(tables->decode({quest_array.data(), quest_array.size()},
                             {quest_names.data(), quest_names.size()}, error), error.c_str());

        dh2::ui::HudTextV1 text;
        auto text_array = read(fixture.source_data / "pydata/common_text_pyarray.bin");
        auto text_names = read(fixture.source_data / "pydata/common_text_pyarraynames.bin");
        auto text_schema = read(fixture.source_data / "pydata/common_text_pystructnames.bin");
        check(text.load({text_array.data(), text_array.size()},
                        {text_names.data(), text_names.size()},
                        {text_schema.data(), text_schema.size()}, error), error.c_str());
        dh2::ui::HudTextEnvironmentV1 environment;
        environment.localization = {&fixture, &Fixture::text_open, &Fixture::text_close,
                                    &Fixture::debug_text, &Fixture::constant};
        environment.localization.player_character=&Fixture::player_character;
        environment.localization.player_name=&Fixture::player_name;
        check(text.switch_pack(0, false, error), error.c_str());

        auto& files = Fixture::debug_files(fixture);
        const dh2::character::DebugLevelBinding16 debug_binding{fixture.debug, &files};
        QuestLogFunctorV108 category;
        QuestLogTextV108 localized;
        std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constants;
        check(bind_source_quest_log_services_v1(debug_binding, text, environment,
              category, localized, constants, error), error.c_str());

        // The page adapter is only a typed bridge into the actual authored
        // movie/action surface. Keep source identity tied to the checked-in
        // SWF and its extracted action block; no replacement art is supplied.
        check(std::strcmp(SourceQuestPageV1::source_movie, "dqcharmenu_droid.swf") == 0 &&
              std::strcmp(SourceQuestPageV1::source_page, "menu_QuestLogSheetNEW") == 0 &&
              SourceQuestPageV1::source_sprite == 599,
              "Quest Page descriptor differs from authored source page");
        std::ifstream movie_stream(authored_movie, std::ios::binary);
        check(bool(movie_stream), "original authored Quest Log SWF is absent");
        std::ifstream action_stream(authored_actions, std::ios::binary);
        check(bool(action_stream), "authored Quest Log action extraction is absent");
        const std::string actions(std::istreambuf_iterator<char>(action_stream), {});
        for (const char* token : {"BLOCK root/sprite599 FRAME 0 OFFSET 00025b43",
                                  "NativeGetQuestDetails", "NativeGetQuestIDsInRange",
                                  "NativeSetCurrentQuest", "QuestTitle", "QuestID",
                                  "IsCurrent", "Assigned", "Completed",
                                  "MENU_MAIN_QUEST", "MENU_SIDE_QUEST"})
            check(actions.find(token) != std::string::npos,
                  "authored Quest Page action source is missing a required field/action");

        std::int32_t debug_priority{}, primary_priority{};
        check(constants("v2QuestPriority", "Debug", debug_priority, error), error.c_str());
        check(constants("v2QuestPriority", "Primary", primary_priority, error), error.c_str());
        check(debug_priority != primary_priority, "source priorities unexpectedly alias");
        check(!fixture.debug_queries, "binding performed eager DebugSwitches queries");

        const auto& original = tables->rows().at(0);
        std::string title; bool include{};
        check(localized(original.text_fields[0], title, error), error.c_str());
        check(!title.empty() && title != "#!WTF!#" && title != "#!SNL!#",
              "actual HudText StringID resolver returned no title");

        dh2::data::QuestDefinitionV51 active_definition = original;
        active_definition.priority = primary_priority;
        dh2::data::QuestPersistenceStateV51 active;
        active.definition = &active_definition; active.state = 6;
        check(category(active, QuestLogCategoryV108::active, include, error) && include,
              "native Active predicate omitted source non-debug state 6");
        active.state = 12;
        check(category(active, QuestLogCategoryV108::active, include, error) && include,
              "native Active predicate omitted source non-debug state 12");
        active.state = 13;
        check(category(active, QuestLogCategoryV108::active, include, error) && !include,
              "native Active predicate included source closed state 13");
        check(category(active, QuestLogCategoryV108::closed, include, error) && include,
              "native Closed predicate omitted source state above 12");
        active.state = 5;
        check(category(active, QuestLogCategoryV108::active, include, error) && !include,
              "native Active predicate included state below source active range");
        active_definition.priority = debug_priority; active.state = 8;
        check(category(active, QuestLogCategoryV108::active, include, error) && !include,
              "native default category admitted Debug priority");
        check(fixture.debug_queries > 0, "predicate did not query actual DebugSwitches owner");

        // Generic per-CharacterState progress has an explicit unknown default;
        // source fresh initialization uses authored definition.state values.
        CharacterState generic_character;
        generic_character.id = "same-generic-character";
        std::int32_t generic_active_row=-1;
        for(std::size_t i=0;i<tables->rows().size();++i)
            if(tables->rows()[i].priority!=debug_priority){generic_active_row=static_cast<std::int32_t>(i);break;}
        check(generic_active_row>=0,"source table has no non-debug row for generic fixture");
        CharacterQuestProgressV1 generic_progress;
        CharacterQuestProgressV1::BucketView bucket_view;
        check(generic_progress.bind_character(generic_character,error),error.c_str());
        check(generic_progress.bucket(generic_character,0,0,bucket_view,error) &&
              bucket_view.origin==CharacterQuestProgressV1::Origin::unknown && !bucket_view.states,
              "default generic Quest progress did not remain explicitly unknown");
        std::vector<std::uint8_t> unknown_codec;
        check(generic_progress.encode(*tables,unknown_codec,error),error.c_str());
        CharacterQuestProgressV1 unknown_round_trip;
        check(unknown_round_trip.decode(generic_character,*tables,unknown_codec,error),error.c_str());
        check(unknown_round_trip.bucket(generic_character,0,0,bucket_view,error) &&
              bucket_view.origin==CharacterQuestProgressV1::Origin::unknown && !bucket_view.states,
              "Quest codec turned unknown generic progress into initialized state");

        SourceCharacterQuestPageV1 unknown_page(generic_character,generic_progress,tables);
        CharacterQuestPageSnapshotV1 generic_snapshot;
        check(!unknown_page.refresh(0,0,CharacterQuestCategoryV1::assigned,
              {false,false,debug_priority},generic_snapshot,error),
              "generic Quest Page filtered an uninitialized/unknown source collection");
        RuntimeQuestMenuV1 unknown_runtime_menu(generic_character,generic_progress,tables,
            {false,false,debug_priority});
        check(unknown_runtime_menu.refresh(0,0,CharacterQuestCategoryV1::assigned,error),error.c_str());
        check(unknown_runtime_menu.frame().availability==RuntimeQuestMenuAvailabilityV1::unknown&&
              unknown_runtime_menu.frame().progress_origin==CharacterQuestProgressV1::Origin::unknown&&
              unknown_runtime_menu.frame().rows.empty()&&
              std::strcmp(unknown_runtime_menu.frame().art.movie,"dqcharmenu_droid.swf")==0,
              "runtime Quest menu fabricated a fresh list for unknown progress or lost source movie identity");
        check(generic_progress.initialize_fresh(generic_character,*tables,error),error.c_str());
        check(generic_progress.bucket(generic_character,0,0,bucket_view,error) &&
              bucket_view.origin==CharacterQuestProgressV1::Origin::fresh_source_initialized &&
              bucket_view.states && bucket_view.states->size()==tables->rows().size() &&
              bucket_view.current_quest==-1,
              "fresh source initialization failed to retain explicit original state provenance");
        for(std::uint32_t collection=0;collection<2;++collection)
            for(std::int32_t difficulty=0;difficulty<3;++difficulty){
                check(generic_progress.bucket(generic_character,collection,difficulty,bucket_view,error),error.c_str());
                check(bucket_view.origin==CharacterQuestProgressV1::Origin::fresh_source_initialized&&
                      bucket_view.states&&bucket_view.states->size()==tables->rows().size()&&
                      bucket_view.current_quest==-1,
                      "fresh source collection/difficulty is not fully initialized");
                for(std::size_t i=0;i<tables->rows().size();++i)
                    check(bucket_view.states->at(i)==tables->rows()[i].state,
                          "fresh generic progress did not use the original authored definition state");
            }

        // This is an explicit source-state producer update, not a local quest
        // transition simulation. It exercises exact row identity and page
        // filtering/currentquest mutation on the same generic character model.
        std::int32_t second_source_row=-1;
        for(std::size_t i=0;i<tables->rows().size();++i)
            if(std::int32_t(i)!=generic_active_row && tables->rows()[i].priority!=debug_priority){
                second_source_row=static_cast<std::int32_t>(i);break;
            }
        check(second_source_row>=0,"source table has no second non-debug row");
        std::int32_t third_source_row=-1;
        for(std::size_t i=0;i<tables->rows().size();++i)
            if(std::int32_t(i)!=generic_active_row && std::int32_t(i)!=second_source_row &&
               tables->rows()[i].priority!=debug_priority){
                third_source_row=static_cast<std::int32_t>(i);break;
            }
        check(third_source_row>=0,"source table has no third non-debug row for CQPG reload action");
        check(generic_progress.record_source_state(generic_character,{0,0,generic_active_row},8,error),error.c_str());
        check(generic_progress.record_source_state(generic_character,{0,0,second_source_row},13,error),error.c_str());
        CharacterQuestTextV1 string_id_resolver;
        check(bind_source_quest_text_resolver_v1(text,environment,string_id_resolver,error),error.c_str());
        CharacterQuestTextV1 generic_text=[&](const CharacterState& owner,std::int32_t id,
              std::string& value,std::string& callback_error){
            check(&owner==&generic_character,"generic Quest text query borrowed a different CharacterState");
            return string_id_resolver(owner,id,value,callback_error);
        };
        SourceCharacterQuestPageV1 generic_page(generic_character,generic_progress,tables,generic_text);
        CharacterQuestPageSnapshotV1 assigned_page,completed_page;
        const CharacterQuestLogPolicyV1 generic_policy{false,false,debug_priority};
        check(generic_page.refresh(0,0,CharacterQuestCategoryV1::assigned,generic_policy,assigned_page,error),error.c_str());
        check(generic_page.refresh(0,0,CharacterQuestCategoryV1::completed,generic_policy,completed_page,error),error.c_str());
        check(assigned_page.title_sorted&&completed_page.title_sorted&&
              std::is_sorted(assigned_page.rows.begin(),assigned_page.rows.end(),
                  [](const auto& a,const auto& b){return *a.title<*b.title;}),
              "generic Quest Page did not use original localized title order");
        const auto assigned_it=std::find_if(assigned_page.rows.begin(),assigned_page.rows.end(),
            [&](const auto& row){return row.id.row==generic_active_row;});
        const auto completed_it=std::find_if(completed_page.rows.begin(),completed_page.rows.end(),
            [&](const auto& row){return row.id.row==second_source_row;});
        check(assigned_it!=assigned_page.rows.end()&&assigned_it->state==8&&!assigned_it->current&&
              completed_it!=completed_page.rows.end()&&completed_it->state==13,
              "generic Quest Log did not filter source Active/Closed state ranges");
        CharacterQuestPageSelectionV1 generic_selection;
        check(generic_page.select(assigned_page,assigned_it->id,generic_selection,error)&&
              generic_selection.activation_visible&&
              generic_selection.details.title==assigned_it->title,
              "Quest Page selection did not borrow source-resolved title/details");

        RuntimeQuestMenuV1 runtime_menu(generic_character,generic_progress,tables,
                                        generic_policy,generic_text);
        check(runtime_menu.refresh(0,0,CharacterQuestCategoryV1::assigned,error),error.c_str());
        const auto& runtime_assigned=runtime_menu.frame();
        check(runtime_assigned.availability==RuntimeQuestMenuAvailabilityV1::ready&&
              runtime_assigned.progress_origin==CharacterQuestProgressV1::Origin::modified&&
              runtime_assigned.art.sprite==599&&
              std::strcmp(runtime_assigned.art.objective_detail_field,"QuestDesc/Description/text")==0&&
              std::strcmp(runtime_assigned.art.activate_hit_target,"btn_Activate/hitzone")==0&&
              runtime_assigned.rows.size()==assigned_page.rows.size()&&
              runtime_assigned.rows.front().id.row==assigned_page.rows.front().id.row&&
              runtime_assigned.rows.front().y_offset_swf_pixels==0&&
              (runtime_assigned.rows.size()<2||runtime_assigned.rows[1].y_offset_swf_pixels==50),
              "runtime Quest menu did not preserve source category/order/row geometry/progress provenance");
        check(runtime_menu.route_hit(RuntimeQuestMenuHitV1::quest_row_release,
                                     assigned_it->id,error),error.c_str());
        check(runtime_menu.frame().selection&&runtime_menu.frame().selection->activation_visible&&
              runtime_menu.frame().selection->details.title==assigned_it->title&&
              runtime_menu.frame().selection->details.objective_description.has_value(),
              "authored row onRelease route did not select exact source title/objective details");
        check(runtime_menu.route_hit(RuntimeQuestMenuHitV1::activate_release,
                                     assigned_it->id,error),error.c_str());
        const auto current_runtime=std::find_if(runtime_menu.frame().rows.begin(),runtime_menu.frame().rows.end(),
            [&](const auto& row){return row.id.row==assigned_it->id.row;});
        check(current_runtime!=runtime_menu.frame().rows.end()&&current_runtime->is_current,
              "authored Activate route did not publish currentquest through the same progress owner");
        check(runtime_menu.refresh(0,0,CharacterQuestCategoryV1::completed,error),error.c_str());
        check(!runtime_menu.frame().rows.empty()&&runtime_menu.route_hit(
              RuntimeQuestMenuHitV1::quest_row_release,completed_it->id,error),error.c_str());
        check(runtime_menu.frame().selection&&!runtime_menu.frame().selection->activation_visible,
              "source Completed row exposed the Assigned-only Activate action");

        // Install the Quest page through CharacterMenu's existing content
        // callback, then replay an unknown->loaded source-progress load on the
        // exact CharacterState/progress owner retained by the callback.
        auto callback_character=std::make_shared<CharacterState>();
        callback_character->id="quest-menu-callback-character";
        CharacterQuestProgressV1 callback_progress;
        check(callback_progress.bind_character(*callback_character,error),error.c_str());
        CharacterQuestProgressV1 saved_source_progress;
        CharacterState producer_character=*callback_character;
        check(saved_source_progress.initialize_fresh(producer_character,*tables,error),error.c_str());
        check(saved_source_progress.record_source_state(producer_character,
              {0,0,generic_active_row},8,error),error.c_str());
        check(saved_source_progress.record_source_state(producer_character,
              {0,0,second_source_row},13,error),error.c_str());
        check(saved_source_progress.record_source_state(producer_character,
              {0,0,third_source_row},8,error),error.c_str());
        std::vector<std::uint8_t> loaded_progress_bytes;
        check(saved_source_progress.encode(*tables,loaded_progress_bytes,error),error.c_str());
        auto callback_text=[&](const CharacterState& owner,std::int32_t id,
              std::string& value,std::string& callback_error){
            check(&owner==callback_character.get(),"CharacterMenu Quest callback resolved text against a different CharacterState");
            return string_id_resolver(owner,id,value,callback_error);
        };
        auto callback_runtime=std::make_shared<RuntimeQuestMenuV1>(
            *callback_character,callback_progress,tables,generic_policy,callback_text);
        auto owner_token=std::make_shared<int>(77);
        bool quest_page_active=false;
        unsigned previous_content_calls{};
        character_menu::SourceCompositionV1 source_composition(owner_token);
        character_menu::SourcePageProviderV1 existing_page;
        existing_page.owner=owner_token;
        existing_page.ready=[](std::string&){return true;};
        existing_page.append=[&](character_menu::Frame&,std::string&){++previous_content_calls;return true;};
        existing_page.release=[](float,float,std::string&){return true;};
        check(source_composition.register_page(character_menu::Tab::skills,
              std::move(existing_page),error),error.c_str());
        auto content_binding=std::make_shared<RuntimeQuestCharacterMenuBindingV1>(
            callback_runtime,callback_character,owner_token,[&]{return quest_page_active;},
            [&](const std::string& source_symbol,std::string& value,std::string& symbol_error){
                dh2::ui::LocalizationResult result;
                fixture.current_player_profile=callback_character.get();
                const bool ok=text.native_string(source_symbol,environment.localization,result,symbol_error);
                fixture.current_player_profile=nullptr;
                if(!ok)return false;
                if(!result.found){symbol_error="Original menu StringManager symbol is missing: "+source_symbol;return false;}
                value=std::move(result.text);return true;
            });
        character_menu::Bindings callback_bindings;
        callback_bindings.character=callback_character.get();
        check(source_composition.install_content(callback_bindings,error),error.c_str());
        check(content_binding->install_content(callback_bindings,error),error.c_str());
        check(content_binding->show(0,0,CharacterQuestCategoryV1::assigned,error),error.c_str());
        character_menu::Frame callback_frame;
        check(callback_bindings.content(character_menu::Tab::skills,callback_frame,error),error.c_str());
        check(callback_frame.art.batches.empty()&&previous_content_calls==1,
              "Quest page-active gate appended Quest art while the source page was not selected");
        quest_page_active=true;
        check(callback_bindings.content(character_menu::Tab::skills,callback_frame,error),error.c_str());
        check(!callback_frame.art.batches.empty()&&callback_runtime->frame().availability==RuntimeQuestMenuAvailabilityV1::unknown&&
              callback_runtime->frame().progress_origin==CharacterQuestProgressV1::Origin::unknown,
              "CharacterMenu content callback did not preserve unknown Quest progress");
        callback_character->source_quest_progress_cqpg=loaded_progress_bytes;
        check(content_binding->load_progress_from_character(error),error.c_str());
        check(content_binding->show(0,0,CharacterQuestCategoryV1::assigned,error),error.c_str());
        check(callback_bindings.content(character_menu::Tab::skills,callback_frame,error),error.c_str());
        check(callback_runtime->frame().availability==RuntimeQuestMenuAvailabilityV1::ready&&
              callback_runtime->frame().progress_origin==CharacterQuestProgressV1::Origin::loaded&&
              callback_runtime->frame().rows.size()==assigned_page.rows.size()+1,
              "CharacterMenu callback did not switch from unknown to loaded authored Assigned rows");
        check(std::any_of(callback_frame.art.batches.begin(),callback_frame.art.batches.end(),
              [](const auto& batch){return batch.role.find("btnQuests/d")!=std::string::npos;})&&
              std::any_of(callback_frame.text.begin(),callback_frame.text.end(),
              [&](const auto& text){return text.field.path.find("QuestName")!=std::string::npos&&
                                           text.value==*assigned_page.rows.front().title;}),
              "CharacterMenu callback did not append original row art and localized source title field");
        const auto callback_assigned=callback_runtime->frame().rows;
        const auto callback_target=std::find_if(callback_assigned.begin(),callback_assigned.end(),
            [&](const auto& row){return row.id.row==generic_active_row;});
        check(callback_target!=callback_assigned.end(),"loaded CharacterMenu Assigned rows omitted the source-active row");
        check(content_binding->route_hit(RuntimeQuestMenuHitV1::quest_row_release,
              callback_target->id,error),error.c_str());
        check(callback_bindings.content(character_menu::Tab::skills,callback_frame,error),error.c_str());
        check(callback_runtime->frame().selection&&callback_runtime->frame().selection->details.title==callback_target->title&&
              callback_runtime->frame().selection->details.objective_description.has_value(),
              "CharacterMenu row route did not return exact loaded title/objective details");
        check(std::any_of(callback_frame.text.begin(),callback_frame.text.end(),
              [&](const auto& text){return text.field.path.find("QuestDetailsText")!=std::string::npos&&
                                           text.value==*callback_target->title;}),
              "CharacterMenu selected detail title was not composed into its source text field");
        check(std::any_of(callback_frame.text.begin(),callback_frame.text.end(),
              [](const auto& text){return text.field.path.find("menu_title/txt_title")!=std::string::npos&&
                                          !text.value.empty();}),
              "Quest page title symbol did not resolve through the already-loaded source HudText StringManager");
        check(content_binding->show(0,0,CharacterQuestCategoryV1::completed,error),error.c_str());
        check(callback_bindings.content(character_menu::Tab::skills,callback_frame,error),error.c_str());
        check(!callback_runtime->frame().rows.empty(),
              "CharacterMenu content callback did not switch to the source Completed list");
        check(content_binding->route_hit(RuntimeQuestMenuHitV1::quest_row_release,
              callback_runtime->frame().rows.front().id,error),error.c_str());
        check(callback_runtime->frame().selection&&!callback_runtime->frame().selection->activation_visible,
              "Completed CharacterMenu detail exposed source Make Active action");
        check(content_binding->show(0,0,CharacterQuestCategoryV1::assigned,error),error.c_str());
        const auto activate_id=CharacterQuestIdV1{0,0,generic_active_row};
        check(content_binding->route_hit(RuntimeQuestMenuHitV1::activate_release,activate_id,error),error.c_str());
        const auto current_callback_row=std::find_if(callback_runtime->frame().rows.begin(),callback_runtime->frame().rows.end(),
            [&](const auto& row){return row.id.row==generic_active_row;});
        check(current_callback_row!=callback_runtime->frame().rows.end()&&current_callback_row->is_current&&
              previous_content_calls==5,
              "CharacterMenu Activate route did not update the same loaded currentquest owner or chain its prior callback");
        CharacterQuestProgressV1 persisted_callback_progress;
        check(persisted_callback_progress.decode(*callback_character,*tables,
              callback_character->source_quest_progress_cqpg,error),error.c_str());
        CharacterQuestStateV1 persisted_callback_state;
        check(persisted_callback_progress.query(*callback_character,activate_id,
              persisted_callback_state,error)&&persisted_callback_state.current_quest_row==generic_active_row,
              "CharacterMenu Activate did not encode currentquest back into the same CharacterState CQPG field");

        // Recreate the page/runtime cache from the persisted bytes on the SAME
        // current CharacterState owner, then exercise source selection and
        // Activate again. This proves the content provider reads the saved
        // CQPG field after reload and that the restored action writes its new
        // currentquest back to that same field.
        CharacterQuestProgressV1 reloaded_callback_progress;
        auto reloaded_callback_runtime=std::make_shared<RuntimeQuestMenuV1>(
            *callback_character,reloaded_callback_progress,tables,generic_policy,callback_text);
        auto reloaded_callback_binding=std::make_shared<RuntimeQuestCharacterMenuBindingV1>(
            reloaded_callback_runtime,callback_character,owner_token,[&]{return quest_page_active;},
            [&](const std::string& source_symbol,std::string& value,std::string& symbol_error){
                dh2::ui::LocalizationResult result;
                fixture.current_player_profile=callback_character.get();
                const bool ok=text.native_string(source_symbol,environment.localization,result,symbol_error);
                fixture.current_player_profile=nullptr;
                if(!ok)return false;
                if(!result.found){symbol_error="Original menu StringManager symbol is missing: "+source_symbol;return false;}
                value=std::move(result.text);return true;
            });
        character_menu::SourcePageProviderV1 quest_source_page;
        check(bind_source_quest_menu_page_provider_v1(reloaded_callback_binding,
              callback_character,owner_token,quest_source_page,error),error.c_str());
        check(source_composition.register_source_menu_page(
              SourceQuestMenuPageProviderV1::source_menu_symbol,quest_source_page,error),error.c_str());
        character_menu::SourceCompositionV1 foreign_source_composition(std::make_shared<int>(88));
        check(!foreign_source_composition.register_source_menu_page(
              SourceQuestMenuPageProviderV1::source_menu_symbol,quest_source_page,error)&&
              error.find("same selected-character owner")!=std::string::npos,
              "Quest page provider registered into a different canonical source owner");
        character_menu::Frame reloaded_frame;
        character_menu::MenuTextField prior_field;prior_field.path="pre-existing-pushed-menu-content";
        reloaded_frame.text.push_back({prior_field,"prior"});
        check(!source_composition.append_source_menu_page(
              SourceQuestMenuPageProviderV1::source_menu_symbol,reloaded_frame,error)&&
              reloaded_frame.text.size()==1,
              "unknown same-owner CQPG allowed Quest page append before the authored page was ready");
        check(!source_composition.release_source_menu_page(
              SourceQuestMenuPageProviderV1::source_menu_symbol,111.f,22.f,error),
              "unknown same-owner CQPG dispatched an authored Quest row release");
        check(reloaded_callback_binding->show(0,0,CharacterQuestCategoryV1::assigned,error),error.c_str());
        check(source_composition.source_menu_page_ready(
              SourceQuestMenuPageProviderV1::source_menu_symbol,error),error.c_str());
        check(source_composition.append_source_menu_page(
              SourceQuestMenuPageProviderV1::source_menu_symbol,reloaded_frame,error),error.c_str());
        check(reloaded_callback_runtime->frame().availability==RuntimeQuestMenuAvailabilityV1::ready&&
              reloaded_callback_runtime->frame().progress_origin==CharacterQuestProgressV1::Origin::loaded&&
              std::any_of(reloaded_callback_runtime->frame().rows.begin(),reloaded_callback_runtime->frame().rows.end(),
                  [&](const auto& row){return row.id.row==generic_active_row&&row.is_current;})&&
              std::any_of(reloaded_frame.text.begin(),reloaded_frame.text.end(),
                  [](const auto& field){return field.field.path.find("QuestName")!=std::string::npos;}),
              "exact-symbol Quest provider did not append restored current marker and localized row content");
        const auto reload_activate_id=CharacterQuestIdV1{0,0,third_source_row};
        const auto hit_row=std::find_if(reloaded_callback_runtime->frame().rows.begin(),
            reloaded_callback_runtime->frame().rows.end(),[&](const auto& row){
                return row.id.row==third_source_row;
            });
        check(hit_row!=reloaded_callback_runtime->frame().rows.end(),
              "reloaded Quest page omitted the exact authored row needed for hit projection");
        const auto& hit_art=original_runtime_quest_menu_art_v1();
        check(!hit_art.row_solids[0].empty()&&
              hit_art.row_solids[0].front().geometry.shape_id==106&&
              hit_art.row_solids[0].front().geometry.role=="btnQuests/d9"&&
              hit_art.row_solids[0].front().geometry.triangles.size()>=3,
              "source Quest row hit contour shape 106 was not retained by the page art provider");
        const auto& hit_triangle=hit_art.row_solids[0].front().geometry.triangles;
        const float local_x=(hit_triangle[0].x+hit_triangle[1].x+hit_triangle[2].x)/3.f;
        const float local_y=(hit_triangle[0].y+hit_triangle[1].y+hit_triangle[2].y)/3.f;
        const auto& row_parent=hit_art.row_parent_matrices[0];
        const float row_offset=float(hit_row->authored_row_index*reloaded_callback_runtime->frame().art.row_step_swf_pixels);
        const float row_stage_x=row_parent[0]*local_x+row_parent[2]*local_y+row_parent[4]+row_parent[2]*row_offset;
        const float row_stage_y=row_parent[1]*local_x+row_parent[3]*local_y+row_parent[5]+row_parent[3]*row_offset;
        SourceQuestMenuHitRouteV1 row_hit_probe;
        check(resolve_source_quest_menu_hit_v1(reloaded_callback_runtime->frame(),
              row_stage_x,row_stage_y,row_hit_probe,error)&&row_hit_probe.handled&&
              row_hit_probe.hit==RuntimeQuestMenuHitV1::quest_row_release&&
              row_hit_probe.quest.row==third_source_row,
              "source root-stage→sprite599→category→50px row transform missed its authored row contour");
        check(source_composition.release_source_menu_page(
              SourceQuestMenuPageProviderV1::source_menu_symbol,row_stage_x,row_stage_y,error),error.c_str());
        check(source_composition.append_source_menu_page(
              SourceQuestMenuPageProviderV1::source_menu_symbol,reloaded_frame,error),error.c_str());
        check(reloaded_callback_runtime->frame().selection&&
              reloaded_callback_runtime->frame().selection->details.objective_description.has_value(),
              "reloaded source row did not resolve details through the same CharacterState and StringManager owner");
        check(std::any_of(reloaded_frame.text.begin(),reloaded_frame.text.end(),
              [](const auto& field){return field.field.path.find("QuestDetailsText")!=std::string::npos;}),
              "source-coordinate row release did not append the selected source details field");
        SourceQuestMenuHitRouteV1 activate_hit_probe;
        check(resolve_source_quest_menu_hit_v1(reloaded_callback_runtime->frame(),385.f,285.f,
              activate_hit_probe,error)&&activate_hit_probe.handled&&
              activate_hit_probe.hit==RuntimeQuestMenuHitV1::activate_release&&
              activate_hit_probe.quest.row==third_source_row,
              "authored source Activate hitzone transform failed to route the selected quest");
        check(source_composition.release_source_menu_page(
              SourceQuestMenuPageProviderV1::source_menu_symbol,385.f,285.f,error),error.c_str());
        CharacterQuestProgressV1 post_reload_action_progress;
        check(post_reload_action_progress.decode(*callback_character,*tables,
              callback_character->source_quest_progress_cqpg,error),error.c_str());
        CharacterQuestStateV1 post_reload_action_state;
        check(post_reload_action_progress.query(*callback_character,reload_activate_id,
              post_reload_action_state,error)&&post_reload_action_state.state==8&&
              post_reload_action_state.current_quest_row==third_source_row&&
              std::any_of(reloaded_callback_runtime->frame().rows.begin(),reloaded_callback_runtime->frame().rows.end(),
                  [&](const auto& row){return row.id.row==third_source_row&&row.is_current;}),
              "same-owner reloaded Activate did not persist the new currentquest through CharacterState CQPG");

        // Compare every page field against the exact Quest getter source
        // branches across the actual 64 authored rows, including the
        // ObjectiveList newline-join fallback and literal source fallbacks.
        constexpr std::int32_t source_none_string=1835016;
        for(const auto& definition:tables->rows()){
            SourceQuestPageTextV1 actual_text;
            check(resolve_source_quest_page_text_v1(definition,generic_character,
                  generic_text,actual_text,error),error.c_str());
            auto expected_string=[&](std::int32_t id)->std::optional<std::string>{
                if(id<0||id==source_none_string)return std::string("not specified");
                std::string value;check(generic_text(generic_character,id,value,error),error.c_str());
                return value;
            };
            check(actual_text.title==expected_string(definition.text_fields[0])&&
                  actual_text.pre_description==expected_string(definition.text_fields[1]),
                  "Quest title/pre-description differs from source StringID/fallback rules");
            std::optional<std::string> expected_objective;
            if(definition.text_fields[2]==source_none_string){
                std::string joined;bool any{};
                for(const auto& objective:definition.objectives)if(objective.description>0){
                    std::string line;check(generic_text(generic_character,objective.description,line,error),error.c_str());
                    if(line.empty())continue;if(any)joined.push_back('\n');joined+=line;any=true;
                }
                expected_objective=std::move(joined);
            } else if(definition.text_fields[2]<0) {
                expected_objective="not specified";
            } else {
                expected_objective=expected_string(definition.text_fields[2]);
            }
            const std::optional<std::string> expected_post=definition.text_fields[3]==source_none_string
                ?std::optional<std::string>("not specified"):std::optional<std::string>("");
            check(actual_text.objective_description==expected_objective&&
                  actual_text.post_description==expected_post,
                  "Quest objective/post-description differs from source getter/fallback rules");
        }
        check(!generic_page.select(completed_page,assigned_it->id,generic_selection,error),
              "generic Quest details selected an ID outside the visible Closed list");
        check(generic_page.activate(assigned_page,assigned_it->id,generic_policy,error),error.c_str());
        CharacterQuestStateV1 generic_query;
        check(generic_progress.query(generic_character,assigned_it->id,generic_query,error)&&
              generic_query.state==8&&generic_query.current_quest_row==generic_active_row,
              "generic Make Active did not write/query the same source currentquest cell");

        std::vector<std::uint8_t> generic_codec;
        check(generic_progress.encode(*tables,generic_codec,error),error.c_str());
        CharacterQuestProgressV1 generic_loaded;
        check(generic_loaded.decode(generic_character,*tables,generic_codec,error),error.c_str());
        check(generic_loaded.bucket(generic_character,0,0,bucket_view,error)&&
              bucket_view.origin==CharacterQuestProgressV1::Origin::loaded&&
              bucket_view.current_quest==generic_active_row&&
              generic_loaded.query(generic_character,assigned_it->id,generic_query,error)&&
              generic_query.state==8&&generic_query.current_quest_row==generic_active_row,
              "generic Quest codec failed to preserve source state/currentquest and loaded provenance");

        auto save = std::make_shared<dh2::data::PlayerSavegameV1>();
        save->set_character(0x51a7);
        std::shared_ptr<dh2::character::CharacterMenuQuestsV51> quest_owner;
        check(construct_source_quest_owner(save, tables, quest_owner, error), error.c_str());
        OriginalQuestAdapter adapter(quest_owner);
        std::int32_t chosen_source_row = -1;
        for (std::size_t row = 0; row < tables->rows().size(); ++row) {
            dh2::data::QuestPersistenceStateV51* state{};
            check(adapter.resolve({0, 0, static_cast<std::int32_t>(row)}, state, error), error.c_str());
            if (state->definition->priority == debug_priority) continue;
            // The original source fixture initializes quests to their authored
            // startup states, which are all locked. Seed the exact recovered
            // Active state on the SAME native Quest receiver for this query
            // fixture; this does not emulate the native transition kernel.
            state->state = 6;
            chosen_source_row = static_cast<std::int32_t>(row);
            break;
        }
        check(chosen_source_row >= 0, "source fixture has no non-debug Quest row");
        SourceQuestServices source_services;
        source_services.local_quests = [quest_owner](auto& out, auto& callback_error) {
            out = quest_owner; callback_error.clear(); return true;
        };
        source_services.difficulty = [](std::uintptr_t, std::int32_t& out,
                                        std::string& callback_error) {
            out = 0; callback_error.clear(); return true;
        };
        source_services.online = [](bool& out, std::string& callback_error) {
            out = false; callback_error.clear(); return true;
        };
        source_services.constant = constants;
        auto page = std::make_shared<SourceQuestServiceBinding>(std::move(source_services));
        SourceQuestPageV1 quest_page(page, category, localized);
        SourceQuestPageSnapshotV1 snapshot;
        check(quest_page.refresh(true, snapshot, error), error.c_str());
        check(snapshot.multiplayer && std::is_sorted(snapshot.assigned.begin(), snapshot.assigned.end(),
              [](const auto& left, const auto& right) { return left.title < right.title; }),
              "authored Assigned list preserves source title order and multiplayer input");
        const auto chosen_it = std::find_if(snapshot.assigned.begin(), snapshot.assigned.end(),
            [&](const auto& row) { return row.id.row == chosen_source_row; });
        check(chosen_it != snapshot.assigned.end(), "native query omitted the seeded same-owner Active Quest");
        const auto chosen = chosen_it->id;
        SourceQuestPageSelectionV1 selection;
        check(quest_page.select(snapshot, SourceQuestLogListV1::assigned, chosen,
                               selection, error), error.c_str());
        check(selection.activate_visible &&
              selection.details.text_ids == tables->rows().at(std::size_t(chosen.row)).text_fields &&
              selection.details.text[0] == chosen_it->title,
              "authored row selection exposes same-owner localized details and Assigned action");
        check(quest_page.activate(snapshot, chosen, error), error.c_str());
        const auto& saved_progress = save->regular_quests_v45().progress().current_quest[0];
        check(saved_progress == chosen.row, "Quest Log activation writes the same PlayerSave currentquest");
        SourceQuestPageSnapshotV1 activated_snapshot;
        check(quest_page.refresh(true, activated_snapshot, error), error.c_str());
        check(std::count_if(activated_snapshot.assigned.begin(), activated_snapshot.assigned.end(),
              [&](const auto& row) { return row.id.row == chosen.row && row.current; }) == 1,
              "authored Quest Page current marker reads the same saved currentquest cell");
        check(!quest_page.select(snapshot, SourceQuestLogListV1::completed, chosen,
                                 selection, error),
              "source Completed list cannot select a row that is not in that authored list");
        check(fixture.leases.empty(), "HudText StringManager lease was not closed");
        std::cout << "PASS source-authored Quest Page; native GetQuestFunctor; same-owner list/details/currentquest; HudText StringID\n";
        return 0;
    } catch (const std::exception& exception) {
        std::fprintf(stderr, "%s\n", exception.what()); return 1;
    }
}
