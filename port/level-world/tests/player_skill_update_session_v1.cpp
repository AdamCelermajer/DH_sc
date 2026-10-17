// Reuse the real-cache Session fixture without changing its existing gate.
#define main player_skill_session_v1_existing_main
#include "player_skill_session_v1.cpp"
#undef main

#include "../player_skill_update_session_v1.hpp"
#include <bit>

namespace update_session = dh2::player_skill_update_session_v1;

namespace {
std::filesystem::path cache_path;
std::filesystem::path temp_path;

void set_mode(Fixture& f, int mode) {
    dh2_script_value arg{};
    arg.type = DH2_SCRIPT_NUMBER;
    arg.number = static_cast<float>(mode);
    Return output;
    check(f.call("set_skill_update_mode", {arg}, output) == 0,
          "could not set skill update mode");
    check(output.count == 0, "mode setter returned values");
}

const char* skill_overlay = R"lua(
RegisterSkill(function()
  update_count = (update_count or 0) + 1
  if skill_update_mode == 1 then return "one" end
  if skill_update_mode == 2 then error("ordinary update error") end
  if skill_update_mode == 3 then GetCurrentSkillInfo__() end
  if skill_update_mode == 4 then return "left", false, nil, "right" end
end, function() return true end, function() end, function() end, function() end)
)lua";

void install_skill_overlays(Fixture& f) {
    for (const auto& row : f.catalogue.tables->skills().skills) {
        if (row.script_length > 0 && !row.script.empty())
            f.overlay(("data/scripts/skills/" + row.script + ".luac").c_str(),
                      skill_overlay);
    }
    for (const auto& row : f.catalogue.tables->faeries().faeries) {
        if (row.spell_script_length > 0 && !row.spell_script.empty())
            f.overlay(("data/scripts/skills/" + row.spell_script + ".luac").c_str(),
                      skill_overlay);
    }
    f.overlay("fixture/update_helpers",
              "function set_skill_update_mode(v) skill_update_mode=v end; "
              "function get_skill_update_count() return update_count or 0 end");
}

void load_helpers(Fixture& f) {
    s::LoadResult load{};
    check(f.load("fixture/update_helpers", load) == 0 && load.source_success,
          "update helper chunk failed");
}

std::uint32_t counter(Fixture& f) {
    Return output;
    check(f.call("get_skill_update_count", {}, output) == 0 &&
          output.count == 1 && output.type == 3, "update counter query failed");
    return static_cast<std::uint32_t>(output.number);
}

void run() {
    Catalogue catalogue(cache_path);

    // The real source preparation produces retained Arguments with the exact
    // script name and loop-slot value consumed by the update bridge.
    Fixture f(cache_path, temp_path / "protocol", catalogue, "KnightPlayerBase");
    f.common();
    install_skill_overlays(f);
    p::source::Result prepared{};
    check(f.owner->prepare(&prepared) == p::source::Status::complete,
          "real Knight preparation failed");
    check(prepared.script_allocations == 13 && f.owner->slots(p::source::List::skill).size() == 16 &&
          f.owner->slots(p::source::List::faery).size() == 5,
          "real Knight source vectors changed");
    const auto skill_id = f.owner->slots(p::source::List::skill).front();
    const auto* instance = f.owner->instance(skill_id);
    const auto* arguments = f.owner->instance_arguments(skill_id);
    check(instance && arguments && arguments->values.size() == 2 &&
          arguments->values[0].type == p::Value::Type::string &&
          arguments->values[0].text == instance->script_name &&
          arguments->values[1].type == p::Value::Type::integer &&
          arguments->values[1].word == 0,
          "preparation did not retain the source SetSkill arguments");
    load_helpers(f);

    std::int32_t machine = 6;
    // A valid VM for another Character cannot be paired with this owner. Fail
    // before running Lua or touching either Character's prepared instances.
    std::string identity_error;
    auto foreign = p::Owner::create(catalogue.tables,
        {CHAR + 1, AIS, &f.view, 0}, f.session->preparation_services(), identity_error);
    check(bool(foreign), "foreign preparation fixture rejected");
    const auto identity_calls = f.native_names.size();
    bool identity_rejected = false;
    try {
        update_session::Runtime invalid(*f.session, *foreign, AIS, CHAR + 1, machine);
    } catch (const std::invalid_argument&) {
        identity_rejected = true;
    }
    check(identity_rejected && f.native_names.size() == identity_calls && counter(f) == 0,
          "cross-Character Session/owner pairing was accepted or called Lua");
    update_session::Runtime runtime(*f.session, *f.owner, AIS, CHAR, machine);
    update_session::Result result{};
    std::string error;
    const auto native_before = f.native_names.size();
    check(runtime.update(result, error) == 0 &&
          result.source.decision == dh2::character_ai_update_all_skills::Decision::skipped_while_using_skill &&
          result.callbacks == 0 && counter(f) == 0 && f.native_names.size() == native_before,
          "UsingSkill did not skip before callbacks");

    machine = 7;
    check(runtime.update(result, error) == 0 &&
          result.source.decision == dh2::character_ai_update_all_skills::Decision::skipped_while_casting &&
          result.callbacks == 0 && counter(f) == 0 && f.native_names.size() == native_before,
          "Casting did not skip before callbacks");

    machine = 0;
    set_mode(f, 0); // SetSkill and OnSkillUpdate both return zero values.
    check(runtime.update(result, error) == 0 && result.callbacks == 13 &&
          result.lua_errors == 0 && runtime.retained_failed_returns() == 0 &&
          counter(f) == 13, "zero-return skill update did not complete");

    set_mode(f, 1); // Each update returns one string, which is owned then released.
    check(runtime.update(result, error) == 0 && result.callbacks == 13 &&
          result.lua_errors == 0 && runtime.retained_failed_returns() == 0 &&
          counter(f) == 26, "one-return skill update ownership failed");

    // Return one value from SetSkill itself. The source caller must erase its
    // full nonempty return range before running OnSkillUpdate.
    f.overlay("fixture/wrap_set_skill",
              "source_set_skill=SetSkill; "
              "SetSkill=function(name,id) last_set_name=name; last_set_id=id; "
              "source_set_skill(name,id); return 'set-result' end; "
              "function get_last_set_name() return last_set_name end; "
              "function get_last_set_id() return last_set_id end");
    s::LoadResult wrapped{};
    check(f.load("fixture/wrap_set_skill", wrapped) == 0 && wrapped.source_success,
          "SetSkill result wrapper failed to load");
    set_mode(f, 0);
    check(runtime.update(result, error) == 0 && result.callbacks == 13 &&
          result.lua_errors == 0 && runtime.retained_failed_returns() == 0 &&
          counter(f) == 39, "one-return SetSkill erase/update path failed");
    const auto last_faery_id=f.owner->slots(p::source::List::faery).back();
    const auto* last_faery=f.owner->instance(last_faery_id);
    const auto* faery_args=f.owner->instance_arguments(last_faery_id);
    check(last_faery && faery_args && faery_args->values.size()==2,
          "last source faery Arguments missing");
    Return observed_name,observed_slot;
    check(f.call("get_last_set_name",{},observed_name)==0 && observed_name.count==1 &&
          observed_name.type==4 && observed_name.text==last_faery->script_name &&
          f.call("get_last_set_id",{},observed_slot,0)==0 && observed_slot.count==1 &&
          observed_slot.type==3,
          "update bridge did not pass the stored faery Arguments");
    float expected_slot=0.0f;
    if(faery_args->values[1].type==p::Value::Type::number)
        std::memcpy(&expected_slot,&faery_args->values[1].word,sizeof(expected_slot));
    else if(faery_args->values[1].type==p::Value::Type::integer){
        std::int32_t integer=0;std::memcpy(&integer,&faery_args->values[1].word,sizeof(integer));
        expected_slot=static_cast<float>(integer);
    }else check(false,"unexpected source faery argument type");
    check(observed_slot.number==expected_slot,"stored source slot argument changed");

    // All source returns are retained and the complete SetSkill vector is
    // erased before one update call per instance; callback effects never replay.
    f.overlay("fixture/multi_set_skill",
              "SetSkill=function(name,id) source_set_skill(name,id); return 'left',false,nil,'right' end");
    check(f.load("fixture/multi_set_skill", wrapped)==0 && wrapped.source_success,
          "multiple SetSkill fixture load failed");
    set_mode(f,4);const auto before_multiple=counter(f);
    check(runtime.update(result,error)==0 && result.callbacks==13 && result.lua_errors==0 &&
          runtime.retained_failed_returns()==0 && counter(f)==before_multiple+13,
          "multiple source ReturnValues were rejected, leaked or replayed");

    // An ordinary SetSkill Lua error follows the source error-code branch:
    // the update callback is skipped and its ReturnValues object is destroyed.
    f.overlay("fixture/wrap_set_skill_error",
              "SetSkill=function(name,id) source_set_skill(name,id); "
              "error('ordinary SetSkill error') end");
    check(f.load("fixture/wrap_set_skill_error", wrapped) == 0 && wrapped.source_success,
          "SetSkill error wrapper failed to load");
    set_mode(f, 0);
    const auto before_error = counter(f);
    check(runtime.update(result, error) == 0 && result.callbacks == 13 &&
          result.lua_errors == 13 && runtime.retained_failed_returns() == 0 &&
          counter(f) == before_error,
          "ordinary SetSkill error did not skip updates and destroy returns");

    // Restore the original SetSkill and make OnSkillUpdate itself error. The
    // source caller ignores its error word but still destroys ReturnValues.
    f.overlay("fixture/restore_set_skill", "SetSkill=source_set_skill; return");
    check(f.load("fixture/restore_set_skill", wrapped) == 0 && wrapped.source_success,
          "SetSkill restore chunk failed");
    set_mode(f, 2);
    const auto before_update_error = counter(f);
    check(runtime.update(result, error) == 0 && result.callbacks == 13 &&
          result.lua_errors == 13 && runtime.retained_failed_returns() == 0 &&
          counter(f) == before_update_error + 13,
          "ordinary OnSkillUpdate error skipped source destructor path");

    // A required native failure stops on the first real callback and retains
    // its ReturnValues resource. No slot or callback is silently retried.
    set_mode(f, 3);
    const auto attempts_before = f.native_names.size();
    check(runtime.update(result, error) < 0 &&
          result.status == dh2::character_ai_update_all_skills::Status::service_failed &&
          result.callbacks == 0 && runtime.retained_failed_returns() == 1 &&
          f.native_names.size() == attempts_before + 1 &&
          f.native_names.back() == "GetCurrentSkillInfo__",
          "required provider failure was not retained at its first source call");
    machine = 6;
    const auto failed_calls = f.native_names.size();
    check(runtime.update(result, error) == 0 && result.callbacks == 0 &&
          runtime.retained_failed_returns() == 1 && f.native_names.size() == failed_calls,
          "failed ReturnValues/callback was replayed through the FSM skip");

    // CharAI::UpdateSkills uses this same prepared character/VM and updates
    // only the Save-selected faery. A foreign Save must fail before Lua.
    dh2::data::PlayerSavegameV1 save;
    save.set_character(CHAR);
    save.initialize_faeries();
    dh2::player_skill_update_session_v1::SelectedFaeryResult selected{};
    const auto before_foreign_save = f.native_names.size();
    dh2::data::PlayerSavegameV1 foreign_save;
    foreign_save.set_character(CHAR + 1);
    foreign_save.initialize_faeries();
    check(runtime.update_current_faery(foreign_save, 0, selected, error) < 0 &&
          f.native_names.size() == before_foreign_save,
          "selected-faery update accepted a foreign Save identity");

    machine = 0;
    set_mode(f, 0);
    const auto before_selected = counter(f);
    const auto selected_status = runtime.update_current_faery(save, 0, selected, error);
    check(selected_status == 0 &&
          selected.status == dh2::character_ai_update_skills::Status::complete &&
          selected.source.decision == dh2::character_ai_update_skills::Decision::updated &&
          selected.source.faery_index == 0 && selected.source.faery_updated == 1 &&
          selected.source.script_updates == 1 && selected.callbacks == 1 &&
          counter(f) == before_selected + 1,
          "selected faery did not use its exact prepared script and VM");

    // A required native failure through the selected path retains its exact
    // ReturnValues owner and stops without retrying or inventing cleanup.
    set_mode(f, 3);
    const auto selected_failure_calls = f.native_names.size();
    check(runtime.update_current_faery(save, 0, selected, error) < 0 &&
          selected.status == dh2::character_ai_update_skills::Status::service_failed &&
          selected.callbacks == 0 && runtime.retained_failed_returns() == 2 &&
          f.native_names.size() == selected_failure_calls + 1 &&
          f.native_names.back() == "GetCurrentSkillInfo__",
          "selected-faery required provider failure was not retained at source boundary");

    std::cout << "{\"validation\":\"PASS\",\"source_slots\":21,"
                 "\"cross_character_guard\":true,"
                 "\"fsm_skips\":2,\"zero_return_updates\":13,"
                 "\"one_return_updates\":13,\"one_return_set_skill_updates\":13,"
                 "\"multi_return_updates\":13,\"multi_return_set_skill_updates\":13,"
                 "\"ordinary_set_skill_errors\":13,"
                 "\"ordinary_update_errors\":13,\"required_failures\":1,"
                 "\"selected_faery_updates\":1,\"selected_required_failures\":1,"
                 "\"retained_failed_resources\":" << runtime.retained_failed_returns()
              << ",\"real_cache_preparation\":true,\"native_player_wiring\":false}\n";
}
} // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: player_skill_update_session_v1 <cache-root> <temp-root>\n";
        return 2;
    }
    // Avoid platform-specific argv sharing with the included legacy test's
    // renamed main and keep the focused runner's contract explicit.
    try {
        cache_path = std::filesystem::path(argv[1]);
        temp_path = std::filesystem::path(argv[2]);
        run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
