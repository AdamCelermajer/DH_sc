#include "../level_savegame_object_manager_v1.hpp"

#include <cassert>
#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace bridge = dh2::level_savegame_object_manager_v1;
namespace save = dh2::level_savegame_owner_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace factory = dh2::character_runtime_factory_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace aggro = dh2::character::aggro_search;
namespace chars = dh2::character_level_save_serialize_v1;

static std::uint32_t read_u32(const std::vector<std::uint8_t>& bytes,
                              std::size_t& cursor) {
    assert(cursor <= bytes.size() && bytes.size() - cursor >= 4);
    std::uint32_t value=0;
    for (unsigned i=0; i!=4; ++i) value |= std::uint32_t(bytes[cursor++]) << (8u*i);
    return value;
}
static std::uint64_t read_u64(const std::vector<std::uint8_t>& bytes,
                              std::size_t& cursor) {
    assert(cursor <= bytes.size() && bytes.size() - cursor >= 8);
    std::uint64_t value=0;
    for (unsigned i=0; i!=8; ++i) value |= std::uint64_t(bytes[cursor++]) << (8u*i);
    return value;
}
static std::vector<std::uint8_t> first_character_payload(
    const std::vector<std::uint8_t>& image, std::uint32_t wanted=0) {
    std::size_t cursor=0;
    assert(read_u32(image,cursor)==2);
    bool found_objects=false;
    while (cursor < image.size()) {
        const auto section_size=read_u32(image,cursor);
        assert(image.size()-cursor >= 4 && section_size <= image.size()-cursor-4);
        const std::string tag(image.begin()+cursor,image.begin()+cursor+4);
        cursor+=4;
        const auto end=cursor+section_size;
        if (tag=="OBJS") {
            found_objects=true;
            const auto object_count=read_u32(image,cursor);
            assert(object_count>=1);
            std::vector<std::uint8_t> first_payload;
            for(std::uint32_t i=0;i<object_count;++i){
                const auto name_size=read_u32(image,cursor);
                assert(name_size && name_size <= end-cursor);
                cursor+=name_size;
                const auto key_size=read_u32(image,cursor);
                assert(key_size && key_size <= end-cursor);
                cursor+=key_size;
                (void)read_u32(image,cursor); // source type word
                const auto payload_size=read_u64(image,cursor);
                assert(payload_size <= end-cursor);
                if(i==wanted) first_payload.assign(image.begin()+cursor,
                                                     image.begin()+cursor+payload_size);
                cursor+=static_cast<std::size_t>(payload_size);
            }
            assert(cursor==end);
            assert(wanted<object_count);
            return first_payload;
        }
        cursor=end;
    }
    assert(found_objects);
    return {};
}

struct Fixture {
    std::uintptr_t component_tokens[factory::component_count]{};
    dh2::character_ai_initialization::State ai_state{};
    std::uint8_t player_position[12]{}, player_rotation[12]{}, player_1306[12]{}, player_1309[12]{};
    std::array<std::uint8_t,900> properties_primary{}, properties_secondary{};
    std::vector<std::uint8_t> published;
    std::uint32_t fact_reads{}, character_serializes{}, publishes{};
    std::uint32_t state_machine_state{3};
    std::uintptr_t generic_player_identity{};
    std::uintptr_t generic_character_identity{};
    bool generic_player_fact_character{true};
    bool force_unregistered_character{};
    std::uint8_t dead{};
    bool source_property_images{};
    bool classify_player{true}, facts_player{true}, facts_character{true};

    static int component(void* raw, ctor::Component id, ctor::Identity identity,
                         factory::ComponentStorage::Slot* slot, std::string&) {
        auto& f=*static_cast<Fixture*>(raw);
        slot->component=id;
        if(id==ctor::Component::ai){
            f.ai_state={};
            f.ai_state.identity=identity;
            f.ai_state.owner_04=identity;
            f.ai_state.group_info_identity=0;
            slot->canonical_owner=&f.ai_state;
        }else{
            slot->canonical_owner=&f.component_tokens[static_cast<std::size_t>(id)];
        }
        return identity ? 0 : 1;
    }
    static int associate(void*, ctor::Association, ctor::Identity identity,
                         const factory::ComponentStorage&, std::string&) { return identity?0:1; }
    static int register_state(void*, ctor::Identity identity, std::uint32_t,
                              std::string&) { return identity?0:1; }
    static void rollback(void*, ctor::Action, std::uint32_t, ctor::Identity,
                         factory::ComponentStorage&) noexcept {}
    static int enroll(void*, aggro::Character*, bool duplicate, bool* appended) {
        *appended=!duplicate; return duplicate?1:0;
    }
    static int remove_character(void*, aggro::Character*, std::size_t* removed) {
        *removed=1; return 0;
    }
    static bool facts(void* raw, const manager::GameObject& live,
                      const factory::Record* record, save::Object* output,
                      std::string&) {
        auto& f=*static_cast<Fixture*>(raw); ++f.fact_reads;
        if (!output || (record && record->game_object.identity!=live.identity)) return true;
        const bool generic_player=!record && f.generic_player_identity==live.identity;
        const bool generic_character=!record && (generic_player ||
            f.generic_character_identity==live.identity);
        output->identity=live.identity;
        output->name=live.name.empty()?(generic_player?"PlayerCharacter_0":"DirectCharacter"):live.name;
        output->manager_key=output->name;
        output->type_word=1;
        output->enabled=1;
        const bool admitted_generic_character=generic_character&&
            (!generic_player||f.generic_player_fact_character);
        output->is_character=(record?f.facts_character:
            (admitted_generic_character||f.force_unregistered_character))?1:0;
        output->is_player=(record?f.facts_player:
            generic_player&&admitted_generic_character)?1:0;
        output->is_local_player=(record||
            (generic_player&&admitted_generic_character))?1:0;
        output->source_flag_129=0;
        return false;
    }
    static bool read_view(void* raw, const factory::Record& record,
                          chars::View* view, std::string&) {
        auto& f=*static_cast<Fixture*>(raw); ++f.character_serializes;
        if (!view || !record.character.identity) return false;
        view->state_machine_state=f.state_machine_state; view->is_player=f.classify_player?1:0;
        view->position={f.player_position,sizeof f.player_position};
        view->rotation={f.player_rotation,sizeof f.player_rotation};
        view->dead=f.dead;
        if (!view->is_player && f.source_property_images) {
            view->properties_primary={f.properties_primary.data(),f.properties_primary.size()};
            view->properties_secondary={f.properties_secondary.data(),f.properties_secondary.size()};
        }
        view->vector_1306={f.player_1306,sizeof f.player_1306};
        view->vector_1309={f.player_1309,sizeof f.player_1309};
        view->has_group_info=0;
        return false;
    }
    static bool game_fields(void*, const factory::Record& record,
                            save::GameObjectFields* fields, std::string&) {
        if (!fields || !record.character.identity) return true;
        *fields={0,0,7}; return false;
    }
    static bool read_view_identity(void* raw, std::uintptr_t identity,
                                   chars::View* view, std::string&) {
        auto& f=*static_cast<Fixture*>(raw); ++f.character_serializes;
        if (!view || (identity!=f.generic_player_identity&&
                      identity!=f.generic_character_identity)) return true;
        const bool player=identity==f.generic_player_identity;
        view->state_machine_state=f.state_machine_state; view->is_player=player?1:0;
        view->position={f.player_position,sizeof f.player_position};
        view->rotation={f.player_rotation,sizeof f.player_rotation};
        view->dead=f.dead;
        if(!player){
            view->properties_primary={f.properties_primary.data(),f.properties_primary.size()};
            view->properties_secondary={f.properties_secondary.data(),f.properties_secondary.size()};
        }
        view->vector_1306={f.player_1306,sizeof f.player_1306};
        view->vector_1309={f.player_1309,sizeof f.player_1309};
        view->has_group_info=0;
        return false;
    }
    static bool game_fields_identity(void* raw, std::uintptr_t identity,
                                     save::GameObjectFields* fields, std::string&) {
        auto& f=*static_cast<Fixture*>(raw);
        if (!fields || (identity!=f.generic_player_identity&&
                        identity!=f.generic_character_identity)) return true;
        *fields={1,0,identity==f.generic_player_identity?13:19}; return false;
    }
    static bool canonical_character_identity(void* raw, std::uintptr_t identity) {
        const auto& f=*static_cast<Fixture*>(raw);
        return identity && (f.generic_player_identity==identity ||
                            f.generic_character_identity==identity);
    }
    static bool canonical_player_identity(void* raw, std::uintptr_t identity) {
        return static_cast<Fixture*>(raw)->generic_player_identity==identity && identity!=0;
    }
    static bool publish(void* raw, const std::string& filename,
                        const std::vector<std::uint8_t>& image, std::string&) {
        auto& f=*static_cast<Fixture*>(raw);
        if (filename!="dh2_000_0_000_000_level.savegame") return false;
        f.published=image; ++f.publishes; return true;
    }
};

int main() {
    manager::Owner objects;
    Fixture fixture;
    factory::Owner characters(objects,{&fixture,&Fixture::enroll,&Fixture::remove_character});
    const factory::Services factory_services{&fixture,&Fixture::component,&Fixture::associate,
        &Fixture::register_state,&Fixture::rollback};
    factory::Record* character=nullptr; factory::Result constructed{}; std::string error;
    assert(characters.create(7,{}, {},factory_services,&character,&constructed,error)==
           factory::Status::complete);
    assert(character && character->object_registered && character->character_listed);

    save::Owner level_save;
    assert(level_save.construct("dh2_000_0_000_000_level.savegame",0x1234,error));
    const chars::CanonicalServices character_services{&fixture,&Fixture::read_view,
                                                        &Fixture::game_fields};
    const bridge::FactsServices facts{&fixture,&Fixture::facts,nullptr,&character_services};
    const save::PublishServices publisher{&fixture,&Fixture::publish};
    bridge::Result result{};
    assert(bridge::save_all(level_save,objects,characters,0,&facts,&publisher,
                            &result,error)==bridge::Status::saved);
    assert(result.manager_rows==1 && result.character_rows==1 &&
           result.save.saved_objects==1 && fixture.fact_reads==1 &&
           fixture.character_serializes==1 && fixture.publishes==1);
    assert(result.save.bytes==fixture.published.size() && result.save.bytes>4);

    // Eligibility facts and Character::IsPlayer come from one canonical
    // Character; disagreement must not produce a profile/level save image.
    fixture.facts_player=false;
    assert(bridge::save_all(level_save,objects,characters,0,&facts,&publisher,
                            &result,error)==bridge::Status::owner_failed);
    assert(error.find("Character::IsPlayer disagrees with source OBJS eligibility facts")!=
           std::string::npos && fixture.publishes==1);
    fixture.facts_player=true;

    // NPC CharProperties raw ARM32 images remain unavailable: source facts may
    // classify a Character as non-player, but serialization must fail closed.
    fixture.classify_player=false;
    fixture.facts_player=false;
    fixture.character_serializes=0;
    assert(bridge::save_all(level_save,objects,characters,0,&facts,&publisher,
                            &result,error)==bridge::Status::owner_failed);
    assert(error.find("Character::Serialize fields do not match pinned source spans")!=
           std::string::npos && fixture.character_serializes==1 && fixture.publishes==1);

    // With the two source-sized CharacterProperties images supplied, the same
    // canonical factory row reaches OBJS. The source dead byte is at +3634 in
    // an NPC payload (6 GameObject + 4 state + 3600 properties + 24 position).
    fixture.source_property_images=true;
    fixture.dead=1;
    fixture.state_machine_state=12;
    assert(bridge::save_all(level_save,objects,characters,0,&facts,&publisher,
                            &result,error)==bridge::Status::saved);
    const auto enemy_payload=first_character_payload(fixture.published);
    assert(enemy_payload.size()==1859 && enemy_payload[1834]==1);
    std::size_t character_cursor=6;
    assert(read_u32(enemy_payload,character_cursor)==12); // state follows GameObject bytes
    assert(fixture.publishes==2);

    // Reuse the same canonical ObjectManager row and owner; a source-facts
    // provider must not relabel a factory Character as a generic object.
    fixture.classify_player=true;
    fixture.facts_player=true;
    fixture.facts_character=false;
    assert(bridge::save_all(level_save,objects,characters,0,&facts,&publisher,
                            &result,error)==bridge::Status::identity_mismatch);
    assert(error.find("source facts conflict with canonical factory/type identity")!=
           std::string::npos && fixture.publishes==2);
    fixture.facts_character=true;
    assert(bridge::save_all(level_save,objects,characters,0,&facts,&publisher,
                            &result,error)==bridge::Status::saved);
    assert(fixture.publishes==3);

    // The retained Player is canonical in the same ObjectManager but does
    // not own a CharacterRuntimeFactory Record. Serialize by its verified
    // Character identity through the existing Player owner callbacks.
    fixture.generic_player_identity=900;
    manager::GameObject player_object{};
    player_object.identity=fixture.generic_player_identity;
    player_object.source_handle=3;
    player_object.name="PlayerCharacter_0";
    manager::GameObject* stored_player=nullptr;
    assert(objects.add_object(3,player_object,&stored_player)==manager::Status::ok &&
           stored_player && stored_player->identity==fixture.generic_player_identity);
    const chars::Services identity_services{&fixture,&Fixture::read_view_identity,
                                             &Fixture::game_fields_identity};
    const bridge::FactsServices identity_facts{&fixture,&Fixture::facts,nullptr,
        &character_services,&identity_services,&Fixture::canonical_character_identity,
        &Fixture::canonical_player_identity};
    const auto serializes_before=fixture.character_serializes;
    assert(bridge::save_all(level_save,objects,characters,0,&identity_facts,&publisher,
                            &result,error)==bridge::Status::saved);
    assert(result.manager_rows==2 && result.character_rows==2 &&
           result.save.saved_objects==2 && fixture.character_serializes==serializes_before+2 &&
           fixture.publishes==4);
    const auto player_payload=first_character_payload(fixture.published);
    assert(player_payload.size()==59 && player_payload[0]==1 && player_payload[1]==0);
    std::size_t player_cursor=6;
    assert(read_u32(player_payload,player_cursor)==fixture.state_machine_state);

    // A direct source actor with no factory Record is admitted only through
    // the canonical identity callback and its own exact serialized fields.
    fixture.generic_character_identity=901;
    manager::GameObject direct_character{};
    direct_character.identity=fixture.generic_character_identity;
    direct_character.source_handle=9;
    direct_character.name="DirectCharacter";
    manager::GameObject* stored_direct_character=nullptr;
    assert(objects.add_object(9,direct_character,&stored_direct_character)==manager::Status::ok &&
           stored_direct_character && stored_direct_character->identity==fixture.generic_character_identity);
    fixture.source_property_images=true;
    fixture.properties_primary.fill(0xa1);
    fixture.properties_secondary.fill(0xb2);
    assert(bridge::save_all(level_save,objects,characters,0,&identity_facts,&publisher,
                            &result,error)==bridge::Status::saved);
    assert(result.manager_rows==3 && result.character_rows==3 &&
           result.save.saved_objects==3 && fixture.publishes==5);
    const auto direct_payload=first_character_payload(fixture.published,2);
    assert(direct_payload.size()==1859);
    std::size_t direct_cursor=6;
    assert(read_u32(direct_payload,direct_cursor)==fixture.state_machine_state);
    assert(direct_payload[direct_cursor]==0xa1 &&
           direct_payload[direct_cursor+900]==0xb2);

    // A missing Record is not permission to serialize arbitrary Character
    // rows; only a source-fact-classified local Player may use this path.
    fixture.generic_player_fact_character=false;
    assert(bridge::save_all(level_save,objects,characters,0,&identity_facts,&publisher,
                            &result,error)==bridge::Status::identity_mismatch);
    assert(error.find("canonical retained Player is misclassified as a non-Character ObjectManager row")!=
           std::string::npos && fixture.publishes==5);
    fixture.generic_player_identity=0;
    fixture.generic_player_fact_character=true;
    fixture.force_unregistered_character=true;
    assert(bridge::save_all(level_save,objects,characters,0,&identity_facts,&publisher,
                            &result,error)==bridge::Status::identity_mismatch);
    assert(error.find("Record-less Character OBJS row is not a canonical Character owner")!=
           std::string::npos && fixture.publishes==5);
    std::puts("PASS level_savegame_object_manager_v1 checks=10");
}
