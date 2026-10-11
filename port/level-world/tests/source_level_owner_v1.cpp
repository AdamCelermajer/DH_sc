#include "../source_level_owner_v1.hpp"
#include "../../game-data/player_savegame_v1.hpp"

#include <cstdio>
#include <cstdlib>

namespace {
const dh2::navigation::CollisionWorld* expected_floor_world{};
unsigned floor_query_calls{};
std::uint32_t floor_query_include_special{99};
int floor_query_result{1};
float floor_query_height{42.f};
}
extern "C" int dh2_test_floor_height(dh2::navigation::HeightHit* hit,
    const dh2::navigation::CollisionWorld* world, const float*, std::uint32_t special) {
    ++floor_query_calls;
    if (world != expected_floor_world) return -1;
    floor_query_include_special = special;
    if (floor_query_result == 1) hit->height = floor_query_height;
    else if (floor_query_result == 0) hit->height = -999.f; // caller must ignore miss output
    return floor_query_result;
}

namespace owner = dh2::source_level_owner_v1;
namespace quick = dh2::level_quick_save_v1;
namespace save = dh2::level_savegame_save_v1;
namespace {
void check(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "FAIL: %s\n", message); std::exit(1); }
}
struct Calls { unsigned position{}, gate{}, save{}, save_all{}; std::uint8_t current_gate{7};
    dh2::data::PlayerSavegameV1* canonical_save{}; const void* qest_owner{};
    std::uintptr_t level_savegame{}; std::string published_name;
    std::vector<std::uint8_t> published_image; };
int copy_position(void* raw, std::uintptr_t, std::uint32_t, std::uint32_t,
                  std::uint32_t, std::string&) {
    ++static_cast<Calls*>(raw)->position; return 0;
}
int write_gate(void* raw, std::uintptr_t, std::uint8_t value, std::string&) {
    auto& c = *static_cast<Calls*>(raw); ++c.gate; c.current_gate = value; return 0;
}
int save_level(void* raw, std::uintptr_t, std::string&) {
    ++static_cast<Calls*>(raw)->save; return 0;
}
int save_all(void* raw, std::uintptr_t identity, std::string&) {
    auto& c=*static_cast<Calls*>(raw);
    if(identity!=c.level_savegame||!c.canonical_save||!c.qest_owner||
       c.canonical_save->source_quest_log_b8().character_5c!=c.canonical_save->character())return 1;
    ++c.save_all; return 0;
}
bool publish_level(void* raw, const std::string& filename,
                   const std::vector<std::uint8_t>& image, std::string&) {
    auto& c=*static_cast<Calls*>(raw);
    c.published_name=filename;c.published_image=image;return true;
}
}

int main() {
    owner::Owner lifecycle;
    const auto object_manager_identity=
        reinterpret_cast<std::uintptr_t>(&lifecycle.object_manager());
    dh2::object_manager_runtime_owner_v1::GameObject menu_object{};
    menu_object.identity=0x22;
    dh2::object_manager_runtime_owner_v1::GameObject* retained_menu_object{};
    check(lifecycle.object_manager().add_named_object(2,"MainMenu",menu_object,
          &retained_menu_object)==dh2::object_manager_runtime_owner_v1::Status::ok &&
          retained_menu_object && lifecycle.object_manager().find_by_name("MainMenu")==retained_menu_object,
          "Application ObjectManager did not own an exact-name menu object");
    int projection{};
    check(!lifecycle.snapshot().pending_script_id_known,
          "empty Level owner fabricated a Level+0x148 value");
    check(lifecycle.begin_load(), "fresh load did not begin");
    check(lifecycle.request_level(23, 0, 0) && lifecycle.begin_source_load(),
          "selected Crypt row did not reach source load phase");
    check(lifecycle.snapshot().pending_script_id_known &&
          lifecycle.snapshot().pending_script_id == -1,
          "source Level constructor sentinel was not retained by the Level owner");
    check(lifecycle.publish_projection(&projection), "completed projection was not published");
    dh2::data::PlayerSavegameV1 canonical_save;canonical_save.set_character(0x400);
    canonical_save.set_slot(0);
    Calls calls{0,0,0,0,7,&canonical_save,&canonical_save.source_quest_log_b8(),0,{},{}};
    quick::Services quick_services{&calls, copy_position, write_gate, save_level};
    quick::State q{}; q.level=0x200; q.level_savegame=0x300; q.local_character=0x400;
    q.level_state=38; q.level_savegame_gate_39=7;
    q.character_word_168=1; q.character_word_160=2; q.character_word_164=3;
    quick::Result qr{}; std::string error;
    check(lifecycle.quick_save(&q, 1, &quick_services, &qr, error)==quick::Status::skipped &&
          calls.position==0, "projection was mistaken for a source save owner");
    int level_fields{};
    const owner::NativeLevelBinding binding{&level_fields, calls.qest_owner,
        reinterpret_cast<std::uintptr_t>(&canonical_save), canonical_save.character(),
        23, 0, 0, 38, 0, false, canonical_save.slot(), 5};
    check(lifecycle.bind_native_level(binding), "Crypt/Save/QEST source owners did not bind");
    const auto bound=lifecycle.snapshot();
    if(!(bound.source_gslevel && bound.source_level && bound.source_level_savegame &&
          bound.source_level_savegame!=reinterpret_cast<std::uintptr_t>(&canonical_save) &&
          bound.canonical_player_savegame==reinterpret_cast<std::uintptr_t>(&canonical_save) &&
          bound.level_fields==&level_fields && bound.quest_owner==calls.qest_owner &&
          bound.level_row==23 && bound.entry_point==0 && bound.difficulty==0 &&
          bound.source_save_slot==0 && lifecycle.level_savegame().constructed() &&
          lifecycle.level_savegame().filename()=="dh2_000_0_005_023_level.savegame" &&
          bound.pending_script_id_known && bound.pending_script_id==-1))
      std::fprintf(stderr,"bind identities gs=%zx lev=%zx lsg=%zx save=%zx fields=%p/%p q=%p/%p row=%d entry=%d diff=%d\n",std::size_t(bound.source_gslevel),std::size_t(bound.source_level),std::size_t(bound.source_level_savegame),std::size_t(bound.canonical_player_savegame),bound.level_fields,&level_fields,bound.quest_owner,calls.qest_owner,bound.level_row,bound.entry_point,bound.difficulty);
    check(bound.source_gslevel && bound.source_level && bound.source_level_savegame &&
          bound.source_level_savegame!=reinterpret_cast<std::uintptr_t>(&canonical_save) &&
          bound.canonical_player_savegame==reinterpret_cast<std::uintptr_t>(&canonical_save) &&
          bound.canonical_player_savegame==reinterpret_cast<std::uintptr_t>(&canonical_save) &&
          bound.level_fields==&level_fields && bound.quest_owner==calls.qest_owner &&
          bound.level_row==23 && bound.entry_point==0 && bound.difficulty==0 &&
          bound.source_save_slot==0 && lifecycle.level_savegame().constructed() &&
          lifecycle.level_savegame().filename()=="dh2_000_0_005_023_level.savegame" &&
          bound.pending_script_id_known && bound.pending_script_id==-1,
          "source identity did not retain actual level/save/QEST bindings");
    calls.level_savegame=bound.source_level_savegame;
    check(lifecycle.level_savegame().gate_39()==0 &&
          !lifecycle.sg_block_level(bound.source_level+1,1) &&
          lifecycle.sg_block_level(bound.source_level,1) &&
          lifecycle.level_savegame().gate_39()==1 &&
          !lifecycle.sg_block_level(bound.source_level,2),
          "SG_BlockLevel did not update only the bound Savegame gate byte");
    dh2::navigation::CollisionWorld floor_world{};
    expected_floor_world = &floor_world;
    check(!lifecycle.bind_floor_query(nullptr, dh2_test_floor_height),
          "missing borrowed collision world was accepted");
    check(lifecycle.bind_floor_query(&floor_world, dh2_test_floor_height),
          "selected Level floor query did not bind");
    float initial_position[3]{1.f, 2.f, 3.f};
    check(lifecycle.set_initial_position_height(bound.generation, initial_position) == 1 &&
          initial_position[2] == floor_query_height && floor_query_calls == 1 &&
          floor_query_include_special == 0,
          "floor query did not use filtered source semantics and apply the hit height");
    floor_query_result = 0;
    initial_position[2] = 17.f;
    check(lifecycle.set_initial_position_height(bound.generation, initial_position) == 0 &&
          initial_position[2] == 17.f,
          "floor miss changed the caller's original Z");
    check(lifecycle.set_initial_position_height(bound.generation + 1, initial_position) == -1 &&
          floor_query_calls == 2,
          "stale Character generation reached the floor provider");
    floor_query_result = 1;
    q.level=bound.source_level; q.level_savegame=bound.source_level_savegame;
    const auto quick_result=lifecycle.quick_save(&q, 1, &quick_services, &qr, error);
    if(quick_result!=quick::Status::saved)std::fprintf(stderr,"quick status=%u err=%s calls=%u/%u/%u gate=%u restored=%u\n",unsigned(quick_result),error.c_str(),calls.position,calls.gate,calls.save,calls.current_gate,qr.original_gate_restored);
    check(quick_result==quick::Status::saved &&
          calls.position==1 && calls.save==1 &&
          lifecycle.level_savegame().gate_39()==1 && qr.original_gate_restored,
          "QuickSave did not restore the bound LevelSavegame gate byte");
    check(lifecycle.sg_block_level(bound.source_level,0) &&
          lifecycle.level_savegame().gate_39()==0,
          "SG_BlockLevel(false) did not clear the current LevelSavegame gate");
    save::State s{}; s.savegame=bound.source_level_savegame;
    save::Services save_services{&calls, save_all}; save::Result sr{};
    check(lifecycle.save_level(&s, &save_services, &sr, error)==save::Status::saved &&
          calls.save_all==1, "LevelSavegame::Save did not use its bound source identity");
    dh2::level_savegame_owner_v1::Object disabled_menu{};
    disabled_menu.identity=0x22;disabled_menu.name="MainMenu";
    disabled_menu.manager_key="MainMenu";disabled_menu.enabled=0;
    dh2::level_savegame_owner_v1::SaveResult level_result{};
    const dh2::level_savegame_owner_v1::PublishServices publisher{&calls,publish_level};
    const dh2::level_savegame_save_v1::State source_save_state{
        bound.source_level_savegame,0,0,0,0};
    check(lifecycle.save_level_objects(&source_save_state,&disabled_menu,1,0,
          nullptr,&publisher,&sr,&level_result,error)==save::Status::saved &&
          calls.published_name=="dh2_000_0_000_023_level.savegame" &&
          level_result.object_rows==1 && level_result.saved_objects==0 &&
          calls.published_image.size()==28 && calls.published_image[0]==2 &&
          calls.published_image[8]=='I' && calls.published_image[9]=='N' &&
          calls.published_image[10]=='F' && calls.published_image[11]=='O' &&
          calls.published_image[12]==23 && calls.published_image[20]=='O' &&
          calls.published_image[21]=='B' && calls.published_image[22]=='J' &&
          calls.published_image[23]=='S' && calls.published_image[24]==0,
          "LevelSavegame Save did not publish exact INFO+OBJS through bound owner");
    const auto saved_image=lifecycle.level_savegame().last_image();
    check(lifecycle.suspend_projection(), "EGL projection suspend failed");
    check(reinterpret_cast<std::uintptr_t>(&lifecycle.object_manager())==object_manager_identity &&
          lifecycle.object_manager().find_by_name("MainMenu")==retained_menu_object,
          "EGL recreation replaced or cleared the Application ObjectManager");
    check(lifecycle.set_initial_position_height(bound.generation, initial_position) == -1,
          "suspended projection retained a usable floor query");
    check(lifecycle.quick_save(&q, 1, &quick_services, &qr, error)==quick::Status::skipped,
          "suspended projection allowed a save");
    check(lifecycle.begin_load() && lifecycle.request_level(23,0,0) &&
          lifecycle.begin_source_load() && lifecycle.publish_projection(&projection),
          "retained logical level did not rebind after EGL recreation");
    check(lifecycle.bind_native_level(binding), "retained Save/QEST owners did not rebind");
    check(lifecycle.level_savegame().last_image()==saved_image,
          "same Level Savegame bind discarded its last INFO/OBJS image");
    check(lifecycle.bind_floor_query(&floor_world, dh2_test_floor_height),
          "rebound selected Level floor query did not bind");
    dh2::object_manager_runtime_owner_v1::GameObject duplicate_name{};
    duplicate_name.identity=0x11;
    dh2::object_manager_runtime_owner_v1::GameObject duplicate_name_later{};
    duplicate_name_later.identity=0x99;
    dh2::object_manager_runtime_owner_v1::GameObject* lower_handle{};
    check(lifecycle.object_manager().add_named_object(-2,"SharedName",duplicate_name,
          &lower_handle)==dh2::object_manager_runtime_owner_v1::Status::ok &&
          lifecycle.object_manager().add_named_object(9,"SharedName",duplicate_name_later,
          &retained_menu_object)==dh2::object_manager_runtime_owner_v1::Status::ok &&
          lifecycle.object_manager().find_by_name("SharedName")==lower_handle,
          "exact-name lookup did not follow signed source-handle order");
    bool removed=false;
    check(lifecycle.object_manager().remove_object(-2,&removed)==
          dh2::object_manager_runtime_owner_v1::Status::ok && removed &&
          lifecycle.object_manager().find_by_name("SharedName")->source_handle==9,
          "exact-name lookup did not advance after source-handle removal");
    check(lifecycle.quick_save(&q, 1, &quick_services, &qr, error)==quick::Status::saved,
          "rebound projection lost its source save chain");
    lifecycle.begin_teardown();
    check(lifecycle.set_initial_position_height(bound.generation, initial_position) == -1,
          "teardown left a borrowed floor query usable");
    check(lifecycle.save_level(&s, &save_services, &sr, error)==save::Status::skipped,
          "tearing-down level allowed Savegame dispatch");
    check(!lifecycle.finish_teardown(),
          "source Level teardown discarded the manager before ObjectManager::Flush");
    check(!lifecycle.flush_object_manager_after_source_level_clear() &&
          lifecycle.object_manager().find_by_name("MainMenu")!=nullptr,
          "ObjectManager::Flush ran before source Level cache clear");
    check(lifecycle.mark_source_level_caches_cleared() &&
          !lifecycle.mark_source_level_caches_cleared() &&
          lifecycle.object_manager().find_by_name("MainMenu")!=nullptr,
          "source cache-clear boundary was not ordered before manager flush");
    check(lifecycle.flush_object_manager_after_source_level_clear() &&
          lifecycle.snapshot().object_manager_flushed &&
          lifecycle.snapshot().object_manager_flush_count==1 &&
          lifecycle.object_manager().empty() &&
          lifecycle.object_manager().find_by_name("MainMenu")==nullptr,
          "ObjectManager flush did not clear the single application owner after caches");
    check(!lifecycle.flush_object_manager_after_source_level_clear(),
          "duplicate source ObjectManager::Flush was accepted");
    check(lifecycle.finish_teardown(),"source lifecycle failed after ordered manager flush");
    check(lifecycle.snapshot().phase==owner::Phase::empty &&
          !lifecycle.level_savegame().constructed() &&
          lifecycle.snapshot().source_level==0 &&
          lifecycle.snapshot().object_manager_identity==object_manager_identity,
          "terminal teardown lost its Application ObjectManager identity");
    std::puts("PASS source_level_owner_v1 checks=34");
}
