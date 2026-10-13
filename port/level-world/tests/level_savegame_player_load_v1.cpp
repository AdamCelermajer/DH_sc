#include "../level_savegame_player_load_v1.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace load = dh2::level_savegame_player_load_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace factory = dh2::character_runtime_factory_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace aggro = dh2::character::aggro_search;
namespace save = dh2::level_savegame_owner_v1;

static void put_u32(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (unsigned i=0; i!=4; ++i) out.push_back(std::uint8_t(value>>(8*i)));
}
static bool write_payload(void* raw, std::uintptr_t,
                          std::vector<std::uint8_t>& out, std::string&) {
    out=*static_cast<const std::vector<std::uint8_t>*>(raw);
    return false;
}
struct PayloadSet {
    std::vector<std::uint8_t> player, other, door, trigger, container;
};
static bool write_payload_set(void* raw, std::uintptr_t identity,
                              std::vector<std::uint8_t>& out, std::string&) {
    const auto& payloads=*static_cast<const PayloadSet*>(raw);
    out=identity==0xabc?payloads.player:
        identity==0xddd?payloads.door:
        identity==0xeee?payloads.trigger:
        identity==0xfff?payloads.container:payloads.other;
    return false;
}

static std::vector<std::uint8_t> image(std::string key="PlayerCharacter_0",
                                      std::string name="PlayerCharacter_0",
                                      std::uint32_t type=1,
                                      std::size_t payload_size=59) {
    std::vector<std::uint8_t> payload;
    payload.reserve(payload_size);
    payload.insert(payload.end(), {0,0});
    put_u32(payload, 17); // GameObject +0x270
    put_u32(payload, 4); // Character SM state
    for (unsigned i=0; i!=24; ++i) payload.push_back(std::uint8_t(i+1));
    payload.push_back(1); // dead
    for (unsigned i=0; i!=24; ++i) payload.push_back(std::uint8_t(i+31));
    if (payload_size == 65) {
        put_u32(payload, 0x1234); payload.push_back(1); payload.push_back(0);
    }
    assert(payload.size()==payload_size);
    const bool player_key=key=="PlayerCharacter_0";
    const save::Object object{0xabc,name,key,static_cast<std::int32_t>(type),1,
                              std::uint8_t(player_key),std::uint8_t(player_key),
                              std::uint8_t(player_key),0};
    const save::SerializeServices serializer{&payload,&write_payload};
    std::vector<std::uint8_t> objs,info,out;
    save::Result result{}; std::string error;
    assert(save::serialize_objects(&object,1,0,&serializer,objs,&result,error)==
           save::Status::complete);
    assert(save::serialize_info_word(0x12345678,info,error));
    const save::Section sections[]={{"INFO",std::move(info)},{"OBJS",std::move(objs)}};
    assert(save::serialize_savegame(sections,2,out,error));
    return out;
}

static std::vector<std::uint8_t> image_with_faery_idle_row() {
    PayloadSet payloads;
    payloads.player={0,0};put_u32(payloads.player,17);put_u32(payloads.player,3);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(0);
    payloads.player.push_back(0);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(0);
    payloads.other={0,0};put_u32(payloads.other,77);put_u32(payloads.other,3);
    for(unsigned sheet=0;sheet!=2;++sheet) {
        if(sheet==0)payloads.other.insert(payloads.other.end(),{0x78,0x56,0x34,0x12});
        else payloads.other.insert(payloads.other.end(),{0xaa,0xbb,0xcc,0xdd});
        for(unsigned i=0;i!=224;++i)put_u32(payloads.other,1000*sheet+i);
    }
    payloads.other.insert(payloads.other.end(),24,0);
    payloads.other.push_back(0);
    payloads.other.insert(payloads.other.end(),24,0);
    assert(payloads.player.size()==59&&payloads.other.size()==1859);
    const save::Object objects[]={{0xabc,"PlayerCharacter_0","PlayerCharacter_0",1,1,1,1,1,0},
                                  {0xdef,"Faery_0","Faery_0",2,1,1,0,0,0}};
    const save::SerializeServices serializer{&payloads,&write_payload_set};
    std::vector<std::uint8_t> objs,info,out;save::Result result{};std::string error;
    assert(save::serialize_objects(objects,2,0,&serializer,objs,&result,error)==
           save::Status::complete&&result.saved_objects==2);
    assert(save::serialize_info_word(0x12345678,info,error));
    const save::Section sections[]={{"INFO",std::move(info)},{"OBJS",std::move(objs)}};
    assert(save::serialize_savegame(sections,2,out,error));
    return out;
}

// Faery_0 plus Npc_0: two non-Player Character rows that share the same
// Character::Serialize payload layout.
static std::vector<std::uint8_t> image_with_faery_and_npc_rows() {
    PayloadSet payloads;
    payloads.player={0,0};put_u32(payloads.player,17);put_u32(payloads.player,3);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(0);
    payloads.player.push_back(0);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(0);
    payloads.other={0,0};put_u32(payloads.other,77);put_u32(payloads.other,3);
    for(unsigned sheet=0;sheet!=2;++sheet) {
        if(sheet==0)payloads.other.insert(payloads.other.end(),{0x78,0x56,0x34,0x12});
        else payloads.other.insert(payloads.other.end(),{0xaa,0xbb,0xcc,0xdd});
        for(unsigned i=0;i!=224;++i)put_u32(payloads.other,1000*sheet+i);
    }
    payloads.other.insert(payloads.other.end(),24,0);
    payloads.other.push_back(0);
    payloads.other.insert(payloads.other.end(),24,0);
    assert(payloads.other.size()==1859);
    const save::Object objects[]={{0xabc,"PlayerCharacter_0","PlayerCharacter_0",1,1,1,1,1,0},
                                  {0xdef,"Faery_0","Faery_0",2,1,1,0,0,0},
                                  {0x333,"Npc_0","Npc_0",2,1,1,0,0,0}};
    const save::SerializeServices serializer{&payloads,&write_payload_set};
    std::vector<std::uint8_t> objs,info,out;save::Result result{};std::string error;
    assert(save::serialize_objects(objects,3,0,&serializer,objs,&result,error)==
           save::Status::complete&&result.saved_objects==3);
    assert(save::serialize_info_word(0x12345678,info,error));
    const save::Section sections[]={{"INFO",std::move(info)},{"OBJS",std::move(objs)}};
    assert(save::serialize_savegame(sections,2,out,error));
    return out;
}

static std::vector<std::uint8_t> image_with_unapplied_row() {
    PayloadSet payloads;
    payloads.player={0,0}; put_u32(payloads.player,17); put_u32(payloads.player,4);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(std::uint8_t(i+1));
    payloads.player.push_back(0);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(std::uint8_t(i+31));
    payloads.other={0xaa,0xbb,0xcc};
    const save::Object objects[]={{0xabc,"PlayerCharacter_0","PlayerCharacter_0",1,1,1,1,1,0},
                                  {0xdef,"Chest_0","Chest_0",0,1,0,0,0,0}};
    const save::SerializeServices serializer{&payloads,&write_payload_set};
    std::vector<std::uint8_t> objs,info,out; save::Result result{}; std::string error;
    assert(save::serialize_objects(objects,2,0,&serializer,objs,&result,error)==
           save::Status::complete && result.saved_objects==2);
    assert(save::serialize_info_word(0x12345678,info,error));
    const save::Section sections[]={{"INFO",std::move(info)},{"OBJS",std::move(objs)}};
    assert(save::serialize_savegame(sections,2,out,error));
    return out;
}

static std::vector<std::uint8_t> image_with_door_row() {
    PayloadSet payloads;
    payloads.player={0,0}; put_u32(payloads.player,17); put_u32(payloads.player,4);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(std::uint8_t(i+1));
    payloads.player.push_back(0);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(std::uint8_t(i+31));
    payloads.door={1,0}; put_u32(payloads.door,77); put_u32(payloads.door,2);
    const save::Object objects[]={{0xabc,"PlayerCharacter_0","PlayerCharacter_0",1,1,1,1,1,0},
                                  {0xddd,"Door_0","Door_0",3,1,0,0,0,0}};
    const save::SerializeServices serializer{&payloads,&write_payload_set};
    std::vector<std::uint8_t> objs,info,out; save::Result result{}; std::string error;
    assert(save::serialize_objects(objects,2,0,&serializer,objs,&result,error)==
           save::Status::complete && result.saved_objects==2);
    assert(save::serialize_info_word(0x12345678,info,error));
    const save::Section sections[]={{"INFO",std::move(info)},{"OBJS",std::move(objs)}};
    assert(save::serialize_savegame(sections,2,out,error));
    return out;
}

static std::vector<std::uint8_t> image_with_source_row(
        std::uintptr_t identity, const char* name, std::int32_t type,
        std::vector<std::uint8_t> payload) {
    PayloadSet payloads;
    payloads.player={0,0};put_u32(payloads.player,17);put_u32(payloads.player,4);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(std::uint8_t(i+1));
    payloads.player.push_back(0);
    for(unsigned i=0;i!=24;++i)payloads.player.push_back(std::uint8_t(i+31));
    if(identity==0xeee)payloads.trigger=std::move(payload);
    else payloads.container=std::move(payload);
    const save::Object objects[]={{0xabc,"PlayerCharacter_0","PlayerCharacter_0",1,1,1,1,1,0},
                                  {identity,name,name,type,1,0,0,0,0}};
    const save::SerializeServices serializer{&payloads,&write_payload_set};
    std::vector<std::uint8_t> objs,info,out;save::Result result{};std::string error;
    assert(save::serialize_objects(objects,2,0,&serializer,objs,&result,error)==
           save::Status::complete&&result.saved_objects==2);
    assert(save::serialize_info_word(0x12345678,info,error));
    const save::Section sections[]={{"INFO",std::move(info)},{"OBJS",std::move(objs)}};
    assert(save::serialize_savegame(sections,2,out,error));
    return out;
}

struct Fixture {
    std::uintptr_t component_tokens[factory::component_count]{};
    unsigned prepares{}, commits{};
    std::uintptr_t generic_player_identity{}, faery_identity{};
    std::uintptr_t door_identity{}, trigger_identity{}, container_identity{};
    std::uintptr_t npc_identity{};
    load::PlayerState applied{};
    unsigned character_prepares{}, character_commits{};
    load::FactoryDeserializeState restored_character{};
    load::SourceNoncharacterDeserializeState restored_noncharacter{};
    unsigned noncharacter_prepares{}, noncharacter_commits{};
    bool fail_prepare{}, fail_commit{};
    static int component(void* raw, ctor::Component id, ctor::Identity identity,
                         factory::ComponentStorage::Slot* slot, std::string&) {
        auto& f=*static_cast<Fixture*>(raw); slot->component=id;
        slot->canonical_owner=&f.component_tokens[static_cast<unsigned>(id)];
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
    // Returns true on failure, like FactsServices::read_object_facts.
    static bool object_facts(void* raw, const manager::GameObject& live,
                             const factory::Record* record, save::Object* out,
                             std::string&) {
        auto& f=*static_cast<Fixture*>(raw);
        if (!out || !live.identity)
            return true;
        if (!record && live.identity==f.generic_player_identity) {
            out->identity=live.identity; out->name="PlayerCharacter_0";
            out->manager_key="PlayerCharacter_0"; out->type_word=1; out->enabled=1;
            out->is_character=1; out->is_player=1; out->is_local_player=1;
            return false;
        }
        if (!record && live.identity==f.door_identity) {
            out->identity=live.identity; out->name="Door_0";
            out->manager_key="Door_0"; out->type_word=3; out->enabled=1;
            return false;
        }
        if (!record && (live.identity==f.trigger_identity||
                        live.identity==f.container_identity)) {
            out->identity=live.identity;
            out->name=live.name;out->manager_key=live.name;
            out->type_word=live.module_id_64;out->enabled=1;
            return false;
        }
        if (!record && f.npc_identity && live.identity==f.npc_identity) {
            out->identity=live.identity; out->name="Npc_0";
            out->manager_key="Npc_0"; out->type_word=2; out->enabled=1;
            out->is_character=1; out->is_player=0; out->is_local_player=0;
            return false;
        }
        if (!record || record->game_object.identity!=live.identity) return true;
        out->identity=live.identity; out->enabled=1;
        if(live.source_handle==10) {
            out->name="Faery_0";out->manager_key="Faery_0";out->type_word=2;
            out->is_character=1;out->is_player=0;out->is_local_player=0;
        } else {
            out->name="PlayerCharacter_0";out->manager_key="PlayerCharacter_0";
            out->type_word=1;out->is_character=1;out->is_player=1;
            out->is_local_player=1;
        }
        return false;
    }
    static bool prepare(void* raw, const factory::Record& record,
                        const load::PlayerState&, std::string& error) {
        auto& f=*static_cast<Fixture*>(raw); ++f.prepares;
        if (!record.object_registered || !record.character.identity) return true;
        if (f.fail_prepare) { error="fixture preflight rejection"; return true; }
        return false;
    }
    static bool commit(void* raw, const factory::Record& record,
                       const load::PlayerState& state, std::string& error) {
        auto& f=*static_cast<Fixture*>(raw); ++f.commits;
        if (!record.character.identity) return true;
        if (f.fail_commit) { error="fixture atomic commit rejection"; return true; }
        f.applied=state; return false;
    }
    static bool supports_character(void* raw,const factory::Record& record,
                              const load::FactoryDeserializeState& state,
                              bool* supported,
                              std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);
        if(!supported){error="missing support output";return true;}
        const std::array<std::uint8_t,4> expected_saved_header{0x78,0x56,0x34,0x12};
        const std::array<std::uint8_t,4> expected_resolved_header{0xaa,0xbb,0xcc,0xdd};
        if(state.properties.saved[0]!=0||state.properties.saved[223]!=223||
           state.properties.resolved[0]!=1000||state.properties.resolved[223]!=1223||
           state.properties.saved_header!=expected_saved_header||
           state.properties.resolved_header!=expected_resolved_header) {
            error="property sheet decode mismatch";return true;
        }
        *supported=record.character.identity==f.faery_identity&&
            state.game_object_word_270==77&&state.state_machine_state==3&&
            state.dead==0&&state.position[0]==0&&
            state.properties.saved_header==expected_saved_header&&
            state.properties.resolved_header==expected_resolved_header;
        return false;
    }
    static bool prepare_character(void* raw,const factory::Record& record,
                             const load::FactoryDeserializeState&,
                             std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);++f.character_prepares;
        if(record.character.identity!=f.faery_identity){error="wrong Faery";return true;}
        return false;
    }
    static bool commit_character(void* raw,const factory::Record& record,
                            const load::FactoryDeserializeState& state,
                            std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);++f.character_commits;
        if(record.character.identity!=f.faery_identity){error="wrong Faery";return true;}
        f.restored_character=state;
        return false;
    }
    static bool canonical_character(void* raw, std::uintptr_t identity) {
        return identity && identity==static_cast<Fixture*>(raw)->generic_player_identity;
    }
    static bool canonical_player(void* raw, std::uintptr_t identity) {
        return canonical_character(raw,identity);
    }
    static bool classify_noncharacter(void* raw,const manager::GameObject& live,
        const save::Object& object,load::SourceNoncharacterKind* kind,
        bool* supported,std::string& error) {
        const auto& f=*static_cast<Fixture*>(raw);
        if(!kind||!supported) {error="fixture has no outputs";return true;}
        if(live.identity==f.door_identity&&object.type_word==3&&live.door.definition_ready)
            *kind=load::SourceNoncharacterKind::door;
        else if(live.identity==f.trigger_identity&&object.type_word==4)
            *kind=load::SourceNoncharacterKind::trigger;
        else if(live.identity==f.container_identity&&object.type_word==5)
            *kind=load::SourceNoncharacterKind::container;
        else {error="fixture did not resolve exact source row";return true;}
        *supported=true;return false;
    }
    static bool prepare_noncharacter(void* raw,const manager::GameObject& live,
        const load::SourceNoncharacterDeserializeState& state,std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);++f.noncharacter_prepares;
        if(live.identity==f.door_identity) {
            if(state.kind!=load::SourceNoncharacterKind::door||
               state.game_object.game_object_word_270!=77||state.door_state_3a8!=2) {
                error="fixture Door preflight mismatch";return true;
            }
        } else if(live.identity==f.trigger_identity) {
            if(state.kind!=load::SourceNoncharacterKind::trigger||
               state.game_object.game_object_word_270!=77||
               state.trigger.pending_3b8!=12||state.trigger.max_count_3b4!=19) {
                error="fixture Trigger preflight mismatch";return true;
            }
        } else if(live.identity==f.container_identity) {
            if(state.kind!=load::SourceNoncharacterKind::container||
               state.container.state_394!=4) {
                error="fixture Container preflight mismatch";return true;
            }
        } else {error="fixture source identity mismatch";return true;}
        return false;
    }
    static bool commit_noncharacter(void* raw,const manager::GameObject& live,
        const load::SourceNoncharacterDeserializeState& state,std::string& error) {
        auto& f=*static_cast<Fixture*>(raw);++f.noncharacter_commits;
        if(live.identity!=f.door_identity&&live.identity!=f.trigger_identity&&
           live.identity!=f.container_identity) {
            error="fixture source identity changed";return true;
        }
        f.restored_noncharacter=state;return false;
    }
    static bool prepare_identity(void* raw, std::uintptr_t identity,
                        const load::PlayerState&, std::string& error) {
        auto& f=*static_cast<Fixture*>(raw); ++f.prepares;
        if (!identity || identity!=f.generic_player_identity || f.fail_prepare) {
            error="fixture identity preflight rejection"; return true;
        }
        return false;
    }
    static bool commit_identity(void* raw, std::uintptr_t identity,
                       const load::PlayerState& state, std::string& error) {
        auto& f=*static_cast<Fixture*>(raw); ++f.commits;
        if (!identity || identity!=f.generic_player_identity || f.fail_commit) {
            error="fixture identity atomic commit rejection"; return true;
        }
        f.applied=state; return false;
    }
};

int main() {
    manager::Owner objects;
    Fixture fixture;
    factory::Owner characters(objects,{&fixture,&Fixture::enroll,&Fixture::remove_character});
    factory::Services factory_services{&fixture,&Fixture::component,&Fixture::associate,
        &Fixture::register_state,&Fixture::rollback};
    factory::Record* player=nullptr; factory::Result created{}; std::string error;
    assert(characters.create(9,{}, {},factory_services,&player,&created,error)==
           factory::Status::complete);
    assert(player && player->character.identity==player->game_object.identity);
    const auto facts=dh2::level_savegame_object_manager_v1::FactsServices{
        &fixture,&Fixture::object_facts,nullptr,nullptr};
    const load::CommitServices commit{&fixture,&Fixture::prepare,&Fixture::commit};
    load::Result result{};

    // Source loader resolves the manager key, not the row's display name.
    auto saved=image("PlayerCharacter_0","PlayerCharacter_01");
    assert(load::load_player(saved.data(),saved.size(),objects,characters,&facts,&commit,
                             &result,error)==load::Status::loaded_player_only);
    assert(result.sections==2 && result.object_rows==1 && result.player_rows==1 &&
           result.unapplied_object_rows==0 &&
           result.source_handle==9 && result.identity==player->character.identity &&
           result.payload_bytes==59 && fixture.prepares==1 && fixture.commits==1);
    assert(fixture.applied.game_object_word_270==17 &&
           fixture.applied.state_machine_state==4 && fixture.applied.dead==1 &&
           fixture.applied.position[0]==1 && fixture.applied.vector_1309[11]==54);
    const auto no_commit=load::load_player(saved.data(),saved.size(),objects,characters,
        &facts,nullptr,&result,error);
    assert(no_commit==load::Status::provider_unavailable && fixture.commits==1);

    // Fresh-process-shaped reopen: only bytes survive; a new canonical manager
    // and Character factory resolve the saved key to a different live identity.
    manager::Owner cold_objects;
    Fixture cold_fixture;
    factory::Owner cold_characters(cold_objects,
        {&cold_fixture,&Fixture::enroll,&Fixture::remove_character});
    factory::Services cold_factory_services{&cold_fixture,&Fixture::component,
        &Fixture::associate,&Fixture::register_state,&Fixture::rollback};
    factory::Record* cold_player=nullptr; factory::Result cold_created{};
    assert(cold_characters.create(9,{}, {},cold_factory_services,&cold_player,
        &cold_created,error)==factory::Status::complete);
    const auto cold_facts=dh2::level_savegame_object_manager_v1::FactsServices{
        &cold_fixture,&Fixture::object_facts,nullptr,nullptr};
    const load::CommitServices cold_commit{&cold_fixture,&Fixture::prepare,&Fixture::commit};
    const auto saved_identity=player->character.identity;
    assert(cold_player->character.identity!=saved_identity);
    assert(load::load_player(saved.data(),saved.size(),cold_objects,cold_characters,
        &cold_facts,&cold_commit,&result,error)==load::Status::loaded_player_only);
    assert(result.identity==cold_player->character.identity &&
        result.identity!=saved_identity && cold_fixture.commits==1 &&
        cold_fixture.applied.dead==1 && cold_fixture.applied.game_object_word_270==17);

    // A malformed envelope, unsupported row, absent key, wrong type, missing
    // provider, or preflight failure cannot reach the atomic mutation callback.
    auto malformed=saved; malformed.push_back(0xff);
    auto before=fixture.commits;
    assert(load::load_player(malformed.data(),malformed.size(),objects,characters,&facts,
        &commit,&result,error)==load::Status::malformed_save && fixture.commits==before);
    auto extra=image("OtherObject","PlayerCharacter_0");
    before=fixture.commits;
    assert(load::load_player(extra.data(),extra.size(),objects,characters,&facts,&commit,
        &result,error)==load::Status::unsupported_record && fixture.commits==before);
    auto absent=image("MissingPlayer");
    assert(load::load_player(absent.data(),absent.size(),objects,characters,&facts,&commit,
        &result,error)==load::Status::unsupported_record && fixture.commits==before);
    auto bad_type=image("PlayerCharacter_0","PlayerCharacter_0",2);
    assert(load::load_player(bad_type.data(),bad_type.size(),objects,characters,&facts,&commit,
        &result,error)==load::Status::identity_mismatch && fixture.commits==before);
    fixture.fail_prepare=true;
    assert(load::load_player(saved.data(),saved.size(),objects,characters,&facts,&commit,
        &result,error)==load::Status::provider_failed && fixture.commits==before);
    fixture.fail_prepare=false;
    fixture.fail_commit=true;
    const auto committed_dead=fixture.applied.dead;
    assert(load::load_player(saved.data(),saved.size(),objects,characters,&facts,&commit,
        &result,error)==load::Status::commit_failed && fixture.commits==before+1);
    assert(fixture.applied.dead==committed_dead);
    fixture.fail_commit=false;

    auto with_ai=image("PlayerCharacter_0","PlayerCharacter_0",1,65);
    const auto ai_index=static_cast<std::size_t>(ctor::Component::ai);
    assert(ai_index<player->components.slots.size());
    const auto ai_slot=player->components.slots[ai_index];
    player->components.slots[ai_index].constructed=false;
    player->components.slots[ai_index].canonical_owner=nullptr;
    assert(load::load_player(with_ai.data(),with_ai.size(),objects,characters,&facts,&commit,
        &result,error)==load::Status::provider_unavailable && fixture.commits==before+1);
    player->components.slots[ai_index]=ai_slot;

    // Source Character::Deserialize's non-player alive state3 path restores
    // saved/resolved property values and resolves to Idle. The same-Record
    // row remains protected because pose, dead, vectors and other fields remain.
    manager::Owner lifecycle_objects;
    Fixture lifecycle_fixture;
    factory::Owner lifecycle_characters(lifecycle_objects,
        {&lifecycle_fixture,&Fixture::enroll,&Fixture::remove_character});
    factory::Services lifecycle_factory_services{&lifecycle_fixture,
        &Fixture::component,&Fixture::associate,&Fixture::register_state,
        &Fixture::rollback};
    factory::Record* lifecycle_player=nullptr;factory::Record* lifecycle_faery=nullptr;
    factory::Result lifecycle_created{};
    assert(lifecycle_characters.create(9,{}, {},lifecycle_factory_services,
        &lifecycle_player,&lifecycle_created,error)==factory::Status::complete);
    assert(lifecycle_characters.create(10,{}, {},lifecycle_factory_services,
        &lifecycle_faery,&lifecycle_created,error)==factory::Status::complete);
    lifecycle_fixture.faery_identity=lifecycle_faery->character.identity;
    const auto lifecycle_facts=dh2::level_savegame_object_manager_v1::FactsServices{
        &lifecycle_fixture,&Fixture::object_facts,nullptr,nullptr};
    load::CommitServices lifecycle_commit{&lifecycle_fixture,&Fixture::prepare,
        &Fixture::commit};
    lifecycle_commit.supports_factory_character_restore=&Fixture::supports_character;
    lifecycle_commit.prepare_factory_character_restore=&Fixture::prepare_character;
    lifecycle_commit.commit_factory_character_restore=&Fixture::commit_character;
    const auto lifecycle_saved=image_with_faery_idle_row();
    assert(load::load_player(lifecycle_saved.data(),lifecycle_saved.size(),
        lifecycle_objects,lifecycle_characters,&lifecycle_facts,&lifecycle_commit,
        &result,error)==load::Status::loaded_player_only&&
        result.unapplied_object_rows==1);
    load::FactoryLifecycleResult lifecycle_result{};
    assert(load::restore_factory_character_rows(lifecycle_saved.data(),
        lifecycle_saved.size(),lifecycle_objects,lifecycle_characters,
        &lifecycle_facts,&lifecycle_commit,&lifecycle_result,error)==
        load::Status::loaded_player_only);
    assert(lifecycle_result.rows_examined==1&&
        lifecycle_result.rows_fully_applied==1&&
        lifecycle_result.rows_still_unapplied==0&&
        lifecycle_fixture.character_prepares==1&&
        lifecycle_fixture.character_commits==1&&
        lifecycle_fixture.restored_character.state_machine_state==3&&
        lifecycle_fixture.restored_character.position.size()==12&&
        lifecycle_fixture.restored_character.dead==0&&
        lifecycle_fixture.restored_character.vector_1474.size()==12&&
        lifecycle_fixture.restored_character.properties.resolved[223]==1223);

    // LevelSavegame::__LoadObjects 0x461e90 skips a row whose key resolves to no
    // object. A same-key Character without a factory Record (an NPC/Actor
    // Character created elsewhere) has no restore owner here: that row stays
    // unapplied and must not abort the Faery row restored next to it.
    manager::GameObject npc_object{};
    npc_object.identity=0x333;npc_object.source_handle=20;npc_object.name="Npc_0";
    manager::GameObject* npc_live=nullptr;
    assert(lifecycle_objects.add_object(20,npc_object,&npc_live)==manager::Status::ok&&
           npc_live&&!lifecycle_characters.find(20));
    lifecycle_fixture.npc_identity=0x333;
    const auto npc_saved=image_with_faery_and_npc_rows();
    load::FactoryLifecycleResult npc_result{};
    assert(load::restore_factory_character_rows(npc_saved.data(),npc_saved.size(),
        lifecycle_objects,lifecycle_characters,&lifecycle_facts,&lifecycle_commit,
        &npc_result,error)==load::Status::loaded_player_only);
    assert(npc_result.rows_examined==2&&npc_result.rows_fully_applied==1&&
        npc_result.rows_still_unapplied==1&&
        lifecycle_fixture.character_commits==2);

    // The production native Player is a canonical manager Character with no
    // factory Record. Resolve and atomically commit through that same identity.
    manager::Owner native_objects;
    Fixture native_fixture;
    native_fixture.generic_player_identity=0x778899;
    factory::Owner native_characters(native_objects,
        {&native_fixture,&Fixture::enroll,&Fixture::remove_character});
    manager::GameObject native_object{};
    native_object.identity=native_fixture.generic_player_identity;
    native_object.source_handle=3;
    native_object.name="PlayerCharacter_0";
    manager::GameObject* native_live=nullptr;
    assert(native_objects.add_object(3,native_object,&native_live)==manager::Status::ok &&
           native_live && native_live->identity==native_fixture.generic_player_identity);
    const auto native_facts=dh2::level_savegame_object_manager_v1::FactsServices{
        &native_fixture,&Fixture::object_facts,nullptr,nullptr,nullptr,
        &Fixture::canonical_character,&Fixture::canonical_player};
    const load::CommitServices native_commit{&native_fixture,nullptr,nullptr,
        &Fixture::prepare_identity,&Fixture::commit_identity};
    const auto direct_saved=image("PlayerCharacter_0","PlayerCharacter_0",1,65);
    assert(load::load_player(direct_saved.data(),direct_saved.size(),native_objects,
        native_characters,&native_facts,&native_commit,&result,error)==load::Status::loaded_player_only);
    assert(result.source_handle==3 && result.identity==native_fixture.generic_player_identity &&
           result.payload_bytes==65 && native_fixture.prepares==1 &&
           native_fixture.commits==1 && native_fixture.applied.has_group_info_tail &&
           result.unapplied_object_rows==0 &&
           native_fixture.applied.group_word_36==0x1234 &&
           native_fixture.applied.group_flag_40==1);
    const auto multi_saved=image_with_unapplied_row();
    assert(load::load_player(multi_saved.data(),multi_saved.size(),native_objects,
        native_characters,&native_facts,&native_commit,&result,error)==
        load::Status::loaded_player_only);
    assert(result.object_rows==2&&result.player_rows==1&&
           result.unapplied_object_rows==1&&native_fixture.commits==2);

    // The same LevelSave OBJS image restores a proven concrete Door row via
    // its exact GameObject prefix and Door enum, while unknown rows stay held.
    manager::Owner door_objects;
    Fixture door_fixture;
    factory::Owner door_characters(door_objects,
        {&door_fixture,&Fixture::enroll,&Fixture::remove_character});
    factory::Services door_factory_services{&door_fixture,&Fixture::component,
        &Fixture::associate,&Fixture::register_state,&Fixture::rollback};
    factory::Record* door_player=nullptr;factory::Result door_created{};
    assert(door_characters.create(9,{}, {},door_factory_services,&door_player,
        &door_created,error)==factory::Status::complete);
    door_fixture.door_identity=0xddd;
    manager::GameObject door_row{};door_row.identity=door_fixture.door_identity;
    door_row.source_handle=11;door_row.name="Door_0";door_row.module_id_64=3;
    door_row.door.definition_ready=1;
    manager::GameObject* door_live=nullptr;
    assert(door_objects.add_named_object(11,"Door_0",door_row,&door_live)==
        manager::Status::ok&&door_live==door_objects.find_by_identity(door_fixture.door_identity));
    const auto door_facts=dh2::level_savegame_object_manager_v1::FactsServices{
        &door_fixture,&Fixture::object_facts,nullptr,nullptr};
    load::CommitServices door_commit{&door_fixture,&Fixture::prepare,&Fixture::commit};
    door_commit.classify_source_noncharacter_restore=&Fixture::classify_noncharacter;
    door_commit.prepare_source_noncharacter_restore=&Fixture::prepare_noncharacter;
    door_commit.commit_source_noncharacter_restore=&Fixture::commit_noncharacter;
    const auto door_saved=image_with_door_row();
    assert(load::load_player(door_saved.data(),door_saved.size(),door_objects,
        door_characters,&door_facts,&door_commit,&result,error)==
        load::Status::loaded_player_only&&result.unapplied_object_rows==1);
    load::FactoryLifecycleResult door_result{};
    const auto door_load_status=load::restore_source_noncharacter_rows(
        door_saved.data(),door_saved.size(),door_objects,door_characters,&door_facts,
        &door_commit,&door_result,error);
    if(door_load_status!=load::Status::loaded_player_only)
        std::fprintf(stderr,"Door restore status=%u error=%s\n",
            static_cast<unsigned>(door_load_status),error.c_str());
    assert(door_load_status==load::Status::loaded_player_only);
    assert(door_result.rows_examined==1&&door_result.rows_fully_applied==1&&
        door_result.rows_still_unapplied==0&&door_fixture.noncharacter_prepares==1&&
        door_fixture.noncharacter_commits==1&&
        door_fixture.restored_noncharacter.game_object.object_base_flag_80==1&&
        door_fixture.restored_noncharacter.game_object.object_base_flag_8a==0&&
        door_fixture.restored_noncharacter.game_object.game_object_word_270==77&&
        door_fixture.restored_noncharacter.door_state_3a8==2);

    // Trigger::Deserialize inherits the exact six-byte GameObject prefix,
    // then reads pending +0x3b8 and max +0x3b4. Container's 7-byte layout is
    // likewise decoded before the production provider decides its Level guard.
    for (const bool trigger : {true,false}) {
        manager::Owner source_objects;
        Fixture source_fixture;
        factory::Owner source_characters(source_objects,
            {&source_fixture,&Fixture::enroll,&Fixture::remove_character});
        factory::Services source_factory_services{&source_fixture,
            &Fixture::component,&Fixture::associate,&Fixture::register_state,
            &Fixture::rollback};
        factory::Record* source_player=nullptr;factory::Result source_created{};
        assert(source_characters.create(9,{}, {},source_factory_services,
            &source_player,&source_created,error)==factory::Status::complete);
        const auto identity=trigger?std::uintptr_t{0xeee}:std::uintptr_t{0xfff};
        const char* key=trigger?"Exit_0":"Container_0";
        const auto type=trigger?4:5;
        if(trigger)source_fixture.trigger_identity=identity;
        else source_fixture.container_identity=identity;
        manager::GameObject source_row{};source_row.identity=identity;
        source_row.source_handle=12;source_row.name=key;
        source_row.module_id_64=type;
        manager::GameObject* source_live=nullptr;
        assert(source_objects.add_named_object(12,key,source_row,&source_live)==
               manager::Status::ok&&source_live);
        std::vector<std::uint8_t> payload{1,0};put_u32(payload,77);
        if(trigger){put_u32(payload,12);put_u32(payload,19);}
        else payload.push_back(4);
        const auto source_saved=image_with_source_row(identity,key,type,payload);
        const auto source_facts=dh2::level_savegame_object_manager_v1::FactsServices{
            &source_fixture,&Fixture::object_facts,nullptr,nullptr};
        load::CommitServices source_commit{&source_fixture,&Fixture::prepare,
            &Fixture::commit};
        source_commit.classify_source_noncharacter_restore=&Fixture::classify_noncharacter;
        source_commit.prepare_source_noncharacter_restore=&Fixture::prepare_noncharacter;
        source_commit.commit_source_noncharacter_restore=&Fixture::commit_noncharacter;
        load::FactoryLifecycleResult source_result{};
        const auto source_status=load::restore_source_noncharacter_rows(source_saved.data(),
            source_saved.size(),source_objects,source_characters,&source_facts,
            &source_commit,&source_result,error);
        if(source_status!=load::Status::loaded_player_only)
            std::fprintf(stderr,"Source row restore status=%u trigger=%u error=%s\n",
                static_cast<unsigned>(source_status),unsigned(trigger),error.c_str());
        assert(source_status==load::Status::loaded_player_only);
        assert(source_result.rows_examined==1&&source_result.rows_fully_applied==1&&
               source_result.rows_still_unapplied==0&&
               source_fixture.noncharacter_prepares==1&&
               source_fixture.noncharacter_commits==1);
        if(trigger)assert(source_fixture.restored_noncharacter.kind==
                load::SourceNoncharacterKind::trigger&&
            source_fixture.restored_noncharacter.trigger.pending_3b8==12&&
            source_fixture.restored_noncharacter.trigger.max_count_3b4==19);
        else assert(source_fixture.restored_noncharacter.kind==
                load::SourceNoncharacterKind::container&&
            source_fixture.restored_noncharacter.container.state_394==4);
    }
    std::puts("PASS level_savegame_player_load_v1 checks=14");
}
