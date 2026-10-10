#include "session_container_retained_visual_v1.hpp"
#include "session_openable_interaction_v1.hpp"
#include "session_destructible_interaction_v1.hpp"
#include "session_container_modern_drop_v1.hpp"
#include "session_authored_container_enrollment_v1.hpp"
#include "session_container_modern_openable_v1.hpp"
#include "session_container_admitted_openable_v1.hpp"
#include "session_authored_openable_scene_v1.hpp"
#include "session_admitted_destructible_v1.hpp"
#include "session_source_object_admission_v1.hpp"
#include "world_object_container_state_v1.hpp"
#include "../loot/runtime_world_item_adapter_v1.hpp"
#include "../loot/runtime_world_item_interaction_v1.hpp"
#include "source_world_item_drop_render_v1.hpp"
#include "source_world_item_drop_material_v1.hpp"
#include "../../game_save.hpp"
#include "../../actor_definitions.hpp"
#include "../../texture_loader.hpp"
#include "../../../game-data/loot_power_creation_v7.hpp"
#include "../../../game-data/loot_audiovisual_v8.hpp"
#include "../../../level-world/tests/openable_container_real_cache_fixture_v1.hpp"
#include "../../../level-world/tests/destructible_data_fixture_v16.inc"
#include "../../embedded_scene_clips.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::interactions;

namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
bool fail(std::string& error,const char* message){error=message;return false;}
std::vector<std::uint8_t> read_bytes(const std::string& root,const char* path){
    std::ifstream input(std::filesystem::path(root)/path,std::ios::binary);
    check(bool(input),std::string("Missing source bytes: ")+path);
    return {std::istreambuf_iterator<char>(input),{}};
}
dh2::data::Bytes view(const std::vector<std::uint8_t>& bytes){return {bytes.data(),bytes.size()};}
std::vector<std::uint8_t> duplicate_openable_row_name(const std::uint8_t* bytes,
    std::size_t size,const std::string& from,const std::string& to){
    check(bytes&&size>=4,"Openable row-name fixture is truncated");
    auto read_u32=[&](std::size_t at){
        check(size-at>=4,"Openable row-name length is truncated");
        return std::uint32_t(bytes[at])|(std::uint32_t(bytes[at+1])<<8)|
            (std::uint32_t(bytes[at+2])<<16)|(std::uint32_t(bytes[at+3])<<24);
    };
    const auto count=read_u32(0);std::size_t at=4;
    std::vector<std::string> names;names.reserve(count);
    for(std::uint32_t i=0;i<count;++i){
        const auto length=read_u32(at);at+=4;
        check(length<=size-at,"Openable row-name bytes exceed fixture");
        names.emplace_back(reinterpret_cast<const char*>(bytes+at),length);at+=length;
    }
    check(at==size,"Openable row-name fixture has trailing bytes");
    bool changed=false;
    for(auto& name:names)if(name==from){name=to;changed=true;break;}
    check(changed,"Requested Openable row-name duplicate source not found");
    std::vector<std::uint8_t> result;
    auto append_u32=[&](std::uint32_t value){for(int i=0;i<4;++i)
        result.push_back(static_cast<std::uint8_t>(value>>(8*i)));};
    append_u32(count);
    for(const auto& name:names){append_u32(static_cast<std::uint32_t>(name.size()));
        result.insert(result.end(),name.begin(),name.end());}
    return result;
}
bool same_owner(const std::shared_ptr<const void>& a,const std::shared_ptr<const void>& b){
    return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
struct EntryServices { std::size_t calls{}; };
struct CpuTextureLeases {
    std::uint32_t next{1};
    std::map<std::uint32_t,TextureImage> images;
    static bool upload(void* raw,const TextureImage& image,std::uint32_t& id,std::string& error){
        auto& self=*static_cast<CpuTextureLeases*>(raw);
        if(!image.width||!image.height||image.rgba.size()!=std::size_t(image.width)*image.height*4){
            error="Source material fixture received invalid decoded texture pixels";return false;
        }
        id=self.next++;self.images.emplace(id,image);error.clear();return true;
    }
    static void release(void* raw,std::uint32_t id){static_cast<CpuTextureLeases*>(raw)->images.erase(id);}
};
struct ProbeDropSink {
    dh2::data::LootRandom8V2* rng{};
    std::size_t items{};
    static bool publish(void* raw,const SourceContainerDropItemV1& item,std::string& error){
        auto& self=*static_cast<ProbeDropSink*>(raw);
        if(!item.selected.item||!item.selected.entry||!item.selected.quantity){
            error="Source seed probe received an incomplete selected row";return false;
        }
        if(item.selected.item->record.words[22]==13){
            std::int32_t value{};
            if(!self.rng||dh2_loot_item_value_v7(&value,self.rng,
                &item.selected.item->record,nullptr,0,0)!=0){
                error="Source seed probe could not consume original gold value";return false;
            }
        }
        ++self.items;error.clear();return true;
    }
};
bool entry_service(void* raw,const dh2::data::LootEntryRequestV8& request,
                   std::int32_t& value,std::string& error){
    ++static_cast<EntryServices*>(raw)->calls;
    switch(request.operation){
    case dh2::data::LootEntryOperationV8::debug_load:
    case dh2::data::LootEntryOperationV8::debug_query:
    case dh2::data::LootEntryOperationV8::mage_count:
    case dh2::data::LootEntryOperationV8::rogue_count:
    case dh2::data::LootEntryOperationV8::warrior_count:
    case dh2::data::LootEntryOperationV8::assertion:value=0;error.clear();return true;
    }
    error="Entry fixture does not provide this source PlayerManager operation";return false;
}
struct Resolver {
    CombatSession* session{};
    SessionContainerRetainedVisualV1* visual{};
    const ActorDefinition* chest{};
    const ActorDefinition* urn{};
    ActorDefinition player_definition;
    dh2::world::OpenableContainerFieldsV1 openable;
    SessionDestructibleFieldsV1 destructible;
    static bool resolve(void* raw,const void* identity,ActorId id,
                        SessionContainerActorBorrowV1& out,std::string& error){
        auto& self=*static_cast<Resolver*>(raw);
        if(identity!=self.session||!self.session||!self.visual)
            return fail(error,"Container resolver received foreign Session");
        auto lease=self.session->actor_binding_lease().lock();
        if(!same_owner(lease,self.visual->session_lease()))
            return fail(error,"Container resolver has expired/replaced Session lease");
        out={};out.session_identity=self.session;out.actor_id=id;
        out.session_lease=lease;out.binding_lifecycle=self.visual->binding_lifecycle();
        if(id==self.chest->stableId){
            auto* object=self.session->world()->find_object(id);
            if(!object)return fail(error,"Authored chest WorldObject missing from current Session");
            out.definition=self.chest;out.object=object;out.openable_state=&self.openable;
        }else if(self.urn&&id==self.urn->stableId){
            auto* object=self.session->world()->find_object(id);
            if(!object)return fail(error,"Authored urn WorldObject missing from current Session");
            out.definition=self.urn;out.object=object;out.destructible_state=&self.destructible;
        }else if(id==1){
            auto* actor=self.session->actor(id);
            if(!actor)return fail(error,"Same current player ActorState absent");
            out.definition=&self.player_definition;out.state=actor;
        }else return fail(error,"Unknown container or opener ObjectId");
        error.clear();return true;
    }
private:
    static bool fail(std::string& error,const char* message){error=message;return false;}
};
struct OpenableScenePolicyContext {
    SessionContainerModernOpenablePolicyV1 base;
    CombatSession* session{};
    std::shared_ptr<Resolver> resolver_lifetime;
};
bool make_openable_scene_policy(void* raw,const ActorDefinition& definition,
    const dh2::world::OpenableContainerRowV1& row,
    SessionContainerModernOpenablePolicyV1& output,std::string& error){
    auto& context=*static_cast<OpenableScenePolicyContext*>(raw);
    if(!context.session||row.visual!=47||row.sound!=33||!row.keep_physics){
        error="Scene fixture received an unexpected original Openable row";return false;
    }
    output=context.base;
    const auto id=definition.stableId;
    auto* session=context.session;
    output.source.has_visual=[session,id](bool& has,std::string& e){
        has=session->retained_object_visual_borrow(id)!=nullptr;e.clear();return true;
    };
    error.clear();return true;
}
bool validate_openable_scene_visual(void*,const ActorDefinition&,
    const dh2::world::OpenableContainerRowV1& row,const WorldObject& object,
    std::string& error){
    if(row.visual!=47||object.visual.model!="go_chest_swamp.bdae"){
        error="WorldObject visual does not match authored GameObjectDict row47";return false;
    }
    error.clear();return true;
}
struct BreakableFixture {
    Resolver* resolver{};
    SessionContainerRetainedVisualV1* visual{};
    std::uint32_t clips{};
    unsigned quest_calls{},script_loads{},on_open_calls{},audio_calls{},physical_calls{},
        source_interact_calls{},stat_calls{};
    RetainedAnimationEvent opened_event;
    bool saw_opened_event{};
    static bool bind_callbacks(void* raw,const void* identity,ActorId id,
        SessionContainerEventCallbackV1 event,SessionContainerCompletionCallbackV1 finished,
        std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(!event)return fail(error,"Urn callback fixture received no source event callback");
        SessionContainerEventCallbackV1 observed=[&self,event=std::move(event)](
            ActorId source,const RetainedAnimationEvent& value,std::string& e){
            if(value.name=="opened"){
                self.opened_event=value;self.saw_opened_event=true;
            }
            return event(source,value,e);
        };
        return self.visual&&SessionContainerRetainedVisualV1::bind_callbacks(
            self.visual,identity,id,std::move(observed),std::move(finished),error);
    }
    static bool play_clip(void* raw,const void* identity,ActorId id,const char* clip,
                          bool& accepted,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        return self.visual&&SessionContainerRetainedVisualV1::play_clip(
            self.visual,identity,id,clip,accepted,error);
    }
    static bool scene_flags(void* raw,const void* identity,ActorId id,std::uint32_t clear,
                            std::uint32_t set,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        return self.visual&&SessionContainerRetainedVisualV1::scene_flags(
            self.visual,identity,id,clear,set,error);
    }
    static bool visual_asset(void* raw,const void* identity,ActorId id,std::int32_t visual,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||!self.resolver->urn||id!=self.resolver->urn->stableId||visual!=70)
            return fail(error,"Urn visual does not match authored destructible row30 visual70");
        error.clear();return true;
    }
    static bool validate_visual_row(void* raw,const ActorDefinition& definition,
        const dh2::world::DestructibleContainerRowV16& row,const WorldObject& object,
        std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(!self.resolver||definition.stableId!=self.resolver->urn->stableId||
           row.visual()!=70||object.id!=definition.stableId||
           object.name!=definition.name||object.visual.model!="go_swamp_urn_breakable.bdae")
            return fail(error,"Urn WorldObject differs from exact authored Destructible visual row70");
        error.clear();return true;
    }
    static bool has_visual(void* raw,const void* identity,ActorId id,bool& value,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||id!=self.resolver->urn->stableId)
            return fail(error,"Urn retained visual query identity mismatch");
        value=self.resolver->session->retained_object_visual_borrow(id)!=nullptr;error.clear();return true;
    }
    static bool animation_count(void* raw,const void* identity,ActorId id,std::uint32_t& count,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||id!=self.resolver->urn->stableId)
            return fail(error,"Urn clip count identity mismatch");
        count=self.clips;error.clear();return true;
    }
    static bool load_script(void* raw,const void* identity,ActorId id,const char* script,const char* dir,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||id!=self.resolver->urn->stableId||!script||
           std::string(script)!="moth_spawn_container"||!dir||std::string(dir)!="data/scripts/objects/")
            return fail(error,"Urn script request differs from authored row30");
        ++self.script_loads;error.clear();return true;
    }
    static bool raise_destroy(void* raw,const void* identity,ActorId source,ActorId actor,
                              std::int32_t data,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||source!=self.resolver->urn->stableId||data!=30||
           (actor!=1&&actor!=invalid_actor_id))
            return fail(error,"Urn DestroyGameObject payload differs from authored row30/session");
        ++self.quest_calls;error.clear();return true;
    }
    static bool has_script(void* raw,const void*,ActorId,bool& value,std::string& error){
        (void)raw;value=true;error.clear();return true;
    }
    static bool script_call(void* raw,const void* identity,ActorId source,const char* method,
                            ActorId opener,const char* event,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||source!=self.resolver->urn->stableId||!method||
           std::string(method)!="OnOpen"||opener!=1||event)
            return fail(error,"Urn OnOpen dispatch payload differs from source request");
        ++self.on_open_calls;error.clear();return true;
    }
    static bool detach(void* raw,const void* identity,ActorId id,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||id!=self.resolver->urn->stableId)
            return fail(error,"Urn physical detach identity mismatch");
        ++self.physical_calls;error.clear();return true;
    }
    static bool sound(void* raw,const void* identity,ActorId id,std::int32_t,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||id!=self.resolver->urn->stableId)
            return fail(error,"Urn authored sound identity mismatch");
        ++self.audio_calls;error.clear();return true;
    }
    static bool source_interact(void* raw,const void* identity,ActorId id,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||id!=self.resolver->urn->stableId)
            return fail(error,"Urn source-interact identity mismatch");
        ++self.source_interact_calls;error.clear();return true;
    }
    static bool as_character(void* raw,const void* identity,ActorId id,bool& value,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||id!=1)
            return fail(error,"Urn opener CharacterState identity mismatch");
        value=self.resolver->session->actor(id)!=nullptr;error.clear();return true;
    }
    static bool increment_stat(void* raw,const void* identity,ActorId id,std::int32_t stat,
                               std::int32_t amount,std::string& error){
        auto& self=*static_cast<BreakableFixture*>(raw);
        if(identity!=self.resolver->session||id!=1||stat!=217||amount!=1)
            return fail(error,"Urn authored breakable stat request mismatch");
        ++self.stat_calls;error.clear();return true;
    }
    static bool get_stat(void*,const void*,ActorId,std::int32_t stat,std::int32_t& value,std::string& error){
        if(stat!=217)return fail(error,"Urn breakable stat id mismatch");
        value=1;error.clear();return true;
    }
    static bool is_local(void*,const void*,ActorId,bool& local,std::string& error){
        local=false;error.clear();return true;
    }
    static bool trophy_id(void*,const char* name,std::int32_t& id,std::string& error){
        if(!name||std::string(name)!="destroy_200_breakables")
            return fail(error,"Urn trophy lookup differs from authored source name");
        id=200;error.clear();return true;
    }
    static bool unlock_trophy(void*,std::int32_t id,std::string& error){
        if(id!=200)return fail(error,"Urn trophy unlock received a foreign trophy ID");
        error.clear();return true;
    }
};
struct BreakableProviderLifetime {
    std::shared_ptr<Resolver> resolver;
    std::shared_ptr<BreakableFixture> fixture;
};
struct StoreSink {
    loot::RuntimeWorldItemAdapterV1* store{};
    dh2::data::LootRandom8V2* rng{};
    std::vector<loot::RuntimeWorldItemIdV1> identities;
    std::vector<SourceContainerDropItemV1> source_records;
    static bool publish(void* raw,const SourceContainerDropItemV1& item,std::string& error){
        auto& self=*static_cast<StoreSink*>(raw);
        if(!self.store||!item.source_object||item.source_state||!item.source_definition||
           item.source_position!=item.source_object->transform.position||!item.selected.item||
           !item.selected.entry||!item.selected.quantity){
            error="Object drop lacks exact same-world source record/transform";return false;
        }
        loot::RuntimeWorldItemRecordV1 record;
        record.source_actor=item.source_actor;record.killer_actor=item.opener_actor;
        record.loot_table=item.loot_table;record.item_id=item.selected.id;
        record.quantity=item.selected.quantity;record.authored_item=item.selected.item;
        record.authored_entry=item.selected.entry;
        if(record.authored_item->record.words[22]==13){
            if(!self.rng)return fail(error,"Source gold requires caller-owned shared Loot RNG");
            std::int32_t value{};
            if(dh2_loot_item_value_v7(&value,self.rng,&record.authored_item->record,nullptr,0,0)!=0)
                return fail(error,"Original GoldStack value kernel rejected source record");
            record.resolved_gold_value=value;
        }
        loot::RuntimeWorldItemIdV1 id{};
        if(!self.store->publish_source_object_drop(record,*item.source_definition,
             *item.source_object,id,error))return false;
        self.identities.push_back(id);self.source_records.push_back(item);error.clear();return true;
    }
private:
    static bool fail(std::string& error,const char* message){error=message;return false;}
};
bool fixture_gold_bonus(void*,ActorId opener,std::int32_t& bonus,std::string& error){
    if(opener!=1){error="Fixture gold bonus expected the actual same-session opener";return false;}
    bonus=0;error.clear();return true;
}
bool script_load(const char*,const char* directory,std::string& error){
    if(!directory||std::string(directory)!="data/scripts/objects/"){
        error="Openable source script directory differs";return false;
    }
    error.clear();return true;
}
bool script_call_missing(const char*,std::uintptr_t,const char*,std::string& error){
    error="Original ObjectScript runtime is not part of this visual bridge test";return false;
}
}

int main(int argc,char** argv){try{
    check(argc==7||argc==8,"Supply shared source assets, current loot cache, object assets, ItemAudioVisual cache, itemdrop assets, source effect assets, and optional source RNG seed");
    AssetCatalog assets(argv[1]),object_assets(argv[3]);std::string error;
    OriginalPropertyDatabase database;OriginalMeleeBindings melee;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(melee.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan visual_plan;
    check(build_original_combat_visual_plan(assets,melee,"KnightPlayerBase",customization,
        "container-live-player",visual_plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=argc>7?std::stoul(argv[7]):6;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=visual_plan.config;
    CombatSessionProfile player_policy;player_policy.action={"AttackStatic",0,{0,1}};
    player_policy.initialIdle={"Idle",0,{0}};player_policy.damageMarkerNames={"attack_mainhand"};
    player_policy.propertyOptions={256,true};config.profiles.emplace("KnightPlayerBase",player_policy);
    CharacterVisual player_visual;ActorPopulation population;CombatSession session;
    check(session.initialize(assets,database,melee,config,player_visual,population,
        {0,0,0},customization,error),error);

    const std::string level="original-cache/data/scene/001_swamp.mlx";
    std::vector<ActorDefinition> definitions;check(load_actor_definitions(assets,level,definitions,error),error);
    std::vector<const ActorDefinition*> scene_openables;
    for(const auto& definition:definitions)
        if(definition.gametype=="OpenableContainer")scene_openables.push_back(&definition);
    check(scene_openables.size()==5,"Actual 001_swamp Openable declaration count changed");
    std::map<std::string,std::size_t> openable_desc_counts;
    for(const auto* definition:scene_openables){
        const auto desc=definition->properties.find("data_desc");
        check(desc!=definition->properties.end(),"Authored Openable placement has no data_desc");
        ++openable_desc_counts[desc->second];
    }
    check(openable_desc_counts["Swamp_Normal_Chest"]==4&&
          openable_desc_counts["SwampCave_Normal_Chest"]==1,
          "Actual 001_swamp Openable placements no longer map to four normal + one cave chest");
    constexpr ActorId chest_id=4308955945491066525ull;
    const auto found=std::find_if(definitions.begin(),definitions.end(),
        [](const auto& definition){return definition.stableId==chest_id;});
    check(found!=definitions.end()&&found->properties.at("data_desc")=="Swamp_Normal_Chest",
          "Actual 001_swamp chest definition/data_desc missing");
    constexpr ActorId urn_id=17396591008448001070ull;
    const auto urn_found=std::find_if(definitions.begin(),definitions.end(),
        [](const auto& definition){return definition.stableId==urn_id;});
    check(urn_found!=definitions.end()&&urn_found->properties.at("data_desc")==
          "Swamp_Normal_DestructibleBarrel","Actual 001_swamp destructible definition/data_desc missing");
    WorldObject chest;chest.id=chest_id;chest.name=found->name;
    chest.visual.model="go_chest_swamp.bdae";
    chest.transform.position={found->placement[12],found->placement[13],found->placement[14]};
    SourceContainerObjsFieldsV1 saved_fields{1,1,0,2};
    check(bind_source_container_objs_v1(chest,saved_fields,error),error);
    const auto with_session_spawn_random=[&session](
        const SessionSourceSpawnRandomConsumerV1& consume,std::string& e){
        if(!consume){e="Empty fixture source RNG consumer";return false;}
        return session.world()->with_loot_random([&](dh2::data::LootRandom8V2& channel0,std::string& nested){
            return consume(channel0,nullptr,nested);
        },e);
    };
    const auto rejected_definition=std::find_if(definitions.begin(),definitions.end(),
        [&](const auto& definition){return definition.gametype=="OpenableContainer"&&
            definition.stableId!=chest_id&&definition.properties.count("data_desc");});
    check(rejected_definition!=definitions.end(),"No second authored Openable candidate for admission rejection cases");
    const auto no_rng_admission=[&](SessionSourceObjectAdmissionRequestV1 request,const char* message){
        SessionSourceObjectAdmissionReceiptV1 receipt;
        const auto before=session.world()->random_state();
        std::string rejection;
        check(!admit_session_source_object_v1(session,std::move(request),receipt,rejection),message);
        check(session.world()->random_state().calls==before.calls,
            "Malformed/colliding candidate consumed source RNG before admission preflight");
    };
    SessionSourceObjectAdmissionRequestV1 malformed_id_request;
    malformed_id_request.definition=&*found;malformed_id_request.candidate=chest;
    malformed_id_request.candidate.id=invalid_object_id;
    no_rng_admission(std::move(malformed_id_request),"Invalid candidate ObjectId passed admission preflight");
    SessionSourceObjectAdmissionRequestV1 malformed_transform_request;
    malformed_transform_request.definition=&*found;malformed_transform_request.candidate=chest;
    malformed_transform_request.candidate.transform.rotation[1]=std::numeric_limits<float>::infinity();
    no_rng_admission(std::move(malformed_transform_request),"Non-finite candidate transform passed admission preflight");
    auto colliding_definition=*found;colliding_definition.stableId=session.player_id();
    WorldObject colliding_candidate=chest;colliding_candidate.id=colliding_definition.stableId;
    colliding_candidate.name=colliding_definition.name;
    SessionSourceObjectAdmissionRequestV1 actor_collision_request;
    actor_collision_request.definition=&colliding_definition;actor_collision_request.candidate=std::move(colliding_candidate);
    no_rng_admission(std::move(actor_collision_request),"Source ObjectId collision with a current Character ActorId was admitted");

    // Exercise a typed provider that refreshes the Session binding only after
    // the World RNG loan has returned. The source roll prefix remains consumed,
    // but the pre-loan lease cannot authorize publication into the new binding.
    WorldObject renewed_candidate;renewed_candidate.id=rejected_definition->stableId;
    renewed_candidate.name=rejected_definition->name;renewed_candidate.visual.model="go_chest_swamp.bdae";
    renewed_candidate.transform.position={rejected_definition->placement[12],
        rejected_definition->placement[13],rejected_definition->placement[14]};
    check(bind_source_container_objs_v1(renewed_candidate,saved_fields,error),error);
    SessionSourceObjectAdmissionRequestV1 renewed_request;
    renewed_request.definition=&*rejected_definition;renewed_request.candidate=renewed_candidate;
    renewed_request.random_owner=session.actor_binding_lease().lock();
    std::size_t renewal_online_calls=0;
    renewed_request.online_byte5=[&](bool& online,std::string& e){
        ++renewal_online_calls;online=false;e.clear();return true;
    };
    renewed_request.with_spawn_random=[&session](const SessionSourceSpawnRandomConsumerV1& consume,std::string& e){
        if(!consume){e="Empty binding-renewal source RNG consumer";return false;}
        if(!session.world()->with_loot_random([&](dh2::data::LootRandom8V2& channel0,std::string& nested){
            return consume(channel0,nullptr,nested);
        },e))return false;
        session.detach_for_restore();
        return session.rebind_after_restore(e);
    };
    const auto renewal_calls_before=session.world()->random_state().calls;
    SessionSourceObjectAdmissionReceiptV1 renewed_receipt;
    check(!admit_session_source_object_v1(session,std::move(renewed_request),renewed_receipt,error)&&
          error.find("binding lease changed")!=std::string::npos&&renewal_online_calls==1&&
          session.world()->random_state().calls==renewal_calls_before+1&&
          !session.world()->find_object(rejected_definition->stableId),
          "Admission accepted a successful roll after its typed provider replaced the Session binding");

    SessionSourceObjectAdmissionReceiptV1 chest_admission;
    std::size_t chest_online_calls=0;
    WorldObject urn;urn.id=urn_id;urn.name=urn_found->name;
    urn.visual.model="go_swamp_urn_breakable.bdae";
    urn.transform.position={urn_found->placement[12],urn_found->placement[13],urn_found->placement[14]};
    check(bind_source_container_objs_v1(urn,{1,1,0,2},error),error);
    SessionSourceObjectAdmissionRequestV1 urn_admission_request;
    urn_admission_request.definition=&*urn_found;urn_admission_request.candidate=urn;
    urn_admission_request.random_owner=session.actor_binding_lease().lock();
    urn_admission_request.with_spawn_random=with_session_spawn_random;
    urn_admission_request.online_byte5=[](bool& online,std::string& e){online=false;e.clear();return true;};
    SessionSourceObjectAdmissionReceiptV1 urn_admission;
    check(admit_session_source_object_v1(session,std::move(urn_admission_request),
        urn_admission,error),error);
    check(urn_admission.admitted&&urn_admission.online_provider_evaluated&&
        urn_admission.prior_admission.object_id==urn_id&&
        session.world()->find_object(urn_id)&&
        !session.retained_object_visual_borrow(urn_id),
        "Exact authored urn admission must precede retained visual enrollment");
    auto visual_owner=std::make_shared<SessionContainerRetainedVisualV1>(session);
    auto& visual=*visual_owner;
    check(visual.session_lease()&&visual.binding_lifecycle()==1,error);

    const std::string cache=argv[2];
    auto loot_bytes=read_bytes(cache,"loot_table_pyarray.bin");
    auto loot_names=read_bytes(cache,"loot_table_pyarraynames.bin");
    auto loot_schema=read_bytes(cache,"loot_table_pystructnames.bin");
    auto power_bytes=read_bytes(cache,"item_powers_pyarray.bin");
    auto power_names=read_bytes(cache,"item_powers_pyarraynames.bin");
    auto power_schema=read_bytes(cache,"item_powers_pystructnames.bin");
    auto monopoly=read_bytes(cache,"item_powers_monopoly_pyarray.bin");
    auto monopoly_names=read_bytes(cache,"item_powers_monopoly_pyarraynames.bin");
    auto monopoly_schema=read_bytes(cache,"item_powers_monopoly_pystructnames.bin");
    auto quantity=read_bytes(cache,"num_prob_records_v7.bin");
    dh2::data::LootTablesV2 loot_tables;
    check(loot_tables.load(view(loot_bytes),view(loot_names),view(loot_schema),error),error);
    auto av_records=read_bytes(argv[4],"loot_audiovisual_pyarray.bin");
    auto av_names=read_bytes(argv[4],"loot_audiovisual_pyarraynames.bin");
    auto av_schema=read_bytes(argv[4],"loot_audiovisual_pystructnames.bin");
    dh2::data::LootAudioVisualV8 audiovisual_owner;
    check(audiovisual_owner.load(view(av_records),view(av_names),view(av_schema),error),error);
    const auto audiovisual=audiovisual_owner.borrow();
    dh2::data::LootPowerInputsV7 power_input{view(power_bytes),view(power_names),
        view(power_schema),view(monopoly),view(monopoly_names),view(monopoly_schema),
        view(quantity),view(loot_names),view(loot_schema)};
    dh2::data::ItemPowerTablesV5 item_powers;
    check(item_powers.load(power_input.powers,power_input.power_names,
        power_input.power_schema,error),error);
    dh2::data::LootPowerResourcesV7 powers;
    check(powers.load(power_input,item_powers.borrow(),error),error);
    const auto tables=loot_tables.borrow();
    loot::RuntimeWorldItemAdapterV1 store(tables);
    EntryServices entries;
    SessionContainerModernDropInputsV1 modern_inputs;
    modern_inputs.tables=tables;modern_inputs.powers=powers.borrow();
    modern_inputs.entry={&entries,entry_service};modern_inputs.store=&store;
    modern_inputs.gold_bonus256=fixture_gold_bonus;
    std::shared_ptr<SessionContainerModernDropV1> modern_drop;
    check(SessionContainerModernDropV1::create(session,std::move(modern_inputs),
        modern_drop,error),error);
    SourceContainerLootServicesV1 loot_services;
    check(modern_drop->services(loot_services,error),error);
    check(loot_services.owner.get()==modern_drop.get()&&
          loot_services.with_gameplay_rng&&loot_services.drop_item_with_rng,
          "Modern drop service did not pin its same-session RNG/store callback owner");

    dh2::world::OpenableContainerTableV1 openable_table;
    check(openable_table.load(source_openable_records,sizeof source_openable_records,
        source_openable_names,sizeof source_openable_names,error),error);
    std::int32_t chest_row_id{};dh2::world::OpenableContainerRowV1 chest_row;
    check(openable_table.resolve("Swamp_Normal_Chest",chest_row_id,chest_row,error),error);
    check(chest_row_id==58&&chest_row.visual==47&&chest_row.loot==227&&
          chest_row.sound==33&&chest_row.keep_physics,
          "Actual source Openable row68 did not resolve visual47/loot227/sound33/KeepPhysics");
    std::int32_t cave_chest_row_id{};dh2::world::OpenableContainerRowV1 cave_chest_row;
    check(openable_table.resolve("SwampCave_Normal_Chest",cave_chest_row_id,cave_chest_row,error),error);
    check(cave_chest_row_id==55&&cave_chest_row.visual==47&&cave_chest_row.loot==223&&
          cave_chest_row.sound==33&&cave_chest_row.keep_physics,
          "Actual cave chest row55 did not resolve visual47/loot223/sound33/KeepPhysics");
    auto resolver_owner=std::make_shared<Resolver>();
    auto& resolver=*resolver_owner;
    resolver.session=&session;resolver.visual=&visual;resolver.chest=&*found;
    resolver.urn=&*urn_found;
    resolver.player_definition.stableId=1;
    resolver.player_definition.name=session.actor(1)->definition_id;
    resolver.player_definition.gametype="Character";
    resolver.openable.data_desc="Swamp_Normal_Chest";
    resolver.destructible.data_desc="Swamp_Normal_DestructibleBarrel";
    SessionContainerActorBorrowV1 urn_borrow;
    check(Resolver::resolve(&resolver,&session,urn_id,urn_borrow,error),error);
    check(urn_borrow.actor_id==urn_id&&urn_borrow.definition==&*urn_found&&urn_borrow.object&&
        urn_borrow.destructible_state==&resolver.destructible&&urn_borrow.binding_lifecycle==1&&
        same_owner(urn_borrow.session_lease,visual.session_lease()),
        "Urn resolver did not return exact current WorldObject/definition/state/lease");

    SessionOpenableInteractionServicesV1 source;
    source.session_identity=&session;source.session_lease=visual.session_lease();
    source.actor_context=&resolver;source.resolve_actor=Resolver::resolve;
    source.visual_context=&visual;
    source.bind_retained_visual=SessionContainerRetainedVisualV1::bind_callbacks;
    source.play_retained_clip=SessionContainerRetainedVisualV1::play_clip;
    source.retained_scene_flags=SessionContainerRetainedVisualV1::scene_flags;
    source.loot=loot_services;
    source.source.spawn_roll_and_probability=[](std::int32_t& roll,std::int32_t& probability,std::string& e){roll=0;probability=100;e.clear();return true;};
    source.source.resolve_row=[&](const std::string& name,std::int32_t& id,
        dh2::world::OpenableContainerRowV1& row,std::string& e){return openable_table.resolve(name,id,row,e);};
    source.source.visual_asset=[&](std::int32_t id,std::string& e){
        if(id!=chest_row.visual){e="Wrong authored Openable visual row";return false;}e.clear();return true;};
    source.source.game_object_init_post=[](std::string& e){e.clear();return true;};
    source.source.meet_condition=[](bool& meets,std::string& e){meets=true;e.clear();return true;};
    source.source.has_visual=[&](bool& has,std::string& e){has=session.retained_object_visual_borrow(chest_id)!=nullptr;e.clear();return true;};
    source.source.apply_mesh_box=[](std::string& e){e.clear();return true;};
    source.source.create_attach_po_decor=[](std::string& e){e.clear();return true;};
    source.source.precache_complete_source_v42=[](std::string& e){e.clear();return true;};
    source.source.load_object_script=script_load;
    source.source.detach_physical=[](std::string& e){e.clear();return true;};
    source.source.play_sound_3d=[](std::int32_t,std::string& e){e.clear();return true;};
    source.source.source_on_interact=[](std::string& e){e.clear();return true;};
    source.source.has_script=[&](bool& has,std::string& e){has=!chest_row.script.empty();e.clear();return true;};
    source.source.script_call=script_call_missing;
    source.interaction.current_level=[](std::uintptr_t& level_id,std::string& e){level_id=1;e.clear();return true;};
    source.interaction.assert_missing_level=[](std::string& e){e="Unexpected missing Level in source Openable fixture";return false;};
    source.interaction.local_player_hosting=[](bool& hosting,std::string& e){hosting=false;e.clear();return true;};
    source.interaction.constant=[](const char*,const char*,std::int32_t&,std::string& e){e="Unexpected hosted quest constant lookup";return false;};
    source.interaction.room64=[](std::int32_t&,std::string& e){e="Unexpected source room64 lookup";return false;};
    source.interaction.raise_async=[](std::uintptr_t,const dh2::world::OpenableContainerQuestEventV2&,std::string& e){e="Unexpected source Quest EventManager raise";return false;};
    source.interaction.handle_as_character=[](std::uintptr_t,std::uintptr_t& character,std::string& e){character=0;e.clear();return true;};
    source.interaction.is_player=[](std::uintptr_t,bool& player,std::string& e){player=false;e.clear();return true;};
    source.interaction.props_add_int=[](std::uintptr_t,std::int32_t,std::int32_t,std::string& e){e="Unexpected source property increment";return false;};
    source.interaction.props_get_int=[](std::uintptr_t,std::int32_t,bool,std::int32_t&,std::string& e){e="Unexpected source property read";return false;};
    source.interaction.is_local_player=[](std::uintptr_t,bool& local,std::string& e){local=false;e.clear();return true;};
    source.interaction.trophy_name_index=[](const char*,std::int32_t&,std::string& e){e="Unexpected source trophy lookup";return false;};
    source.interaction.unlock_trophy=[](std::int32_t,std::string& e){e="Unexpected source trophy unlock";return false;};
    auto resolve_source_openable_row=source.source.resolve_row;
    SessionContainerModernOpenablePolicyV1 openable_policy;
    openable_policy.authored_table=std::make_shared<dh2::world::OpenableContainerTableV1>(openable_table);
    openable_policy.prior_admission=chest_admission.prior_admission;
    openable_policy.other_actor_context=&resolver;
    openable_policy.resolve_other_actor=Resolver::resolve;
    openable_policy.source_fields=resolver.openable;
    openable_policy.source=std::move(source.source);
    openable_policy.interaction=std::move(source.interaction);
    auto scene_policy_base=openable_policy;
    SessionSourceObjectAdmissionRequestV1 missing_provider_request;
    missing_provider_request.definition=&*found;missing_provider_request.candidate=chest;
    missing_provider_request.random_owner=session.actor_binding_lease().lock();
    missing_provider_request.with_spawn_random=with_session_spawn_random;
    missing_provider_request.online_byte5=[&](bool& online,std::string& e){++chest_online_calls;online=false;e.clear();return true;};
    auto missing_provider_policy=openable_policy;missing_provider_policy.resolve_other_actor=nullptr;
    SessionContainerAdmittedOpenableResultV1 missing_provider_result;
    const auto missing_provider_calls=session.world()->random_state().calls;
    check(!admit_and_bind_session_openable_v1(session,std::move(missing_provider_request),
        object_assets,visual,modern_drop,std::move(missing_provider_policy),
        missing_provider_result,error)&&!missing_provider_result.openable&&
        chest_online_calls==0&&
        session.world()->random_state().calls==missing_provider_calls&&
        !session.world()->find_object(chest_id)&&!session.retained_object_visual_borrow(chest_id),
        "Missing opener resolver must fail composition before admission RNG or visual/object publication");

    SessionSourceObjectAdmissionRequestV1 chest_admission_request;
    chest_admission_request.definition=&*found;chest_admission_request.candidate=chest;
    chest_admission_request.network_id108=-1;chest_admission_request.probability274=100;
    chest_admission_request.random_owner=session.actor_binding_lease().lock();
    chest_admission_request.with_spawn_random=with_session_spawn_random;
    chest_admission_request.online_byte5=[&](bool& online,std::string& e){
        ++chest_online_calls;online=false;e.clear();return true;
    };
    auto missing_admission=openable_policy;
    auto rejected_composed_policy=openable_policy;
    missing_admission.prior_admission={};
    const auto composed_chest_calls=session.world()->random_state().calls;
    check(admit_session_source_object_v1(session,std::move(chest_admission_request),
        chest_admission,error),error);
    check(chest_admission.admitted&&chest_online_calls==1&&
        chest_admission.probability==100&&chest_admission.data_id_ec==-1&&
        chest_admission.network_id108==-1&&!chest_admission.player_handle&&
        !chest_admission.online&&chest_admission.roll==-2&&
        chest_admission.cached_roll270==-2&&
        session.world()->random_state().calls==composed_chest_calls+1&&
        session.world()->find_object(chest_id)&&!session.retained_object_visual_borrow(chest_id),
        "Authored chest admission must publish the exact object before visual/callback binding");
    auto table_owner=std::make_shared<dh2::world::OpenableContainerTableV1>(openable_table);
    auto scene_policy_context=std::make_shared<OpenableScenePolicyContext>();
    scene_policy_context->base=scene_policy_base;
    scene_policy_context->session=&session;
    scene_policy_context->resolver_lifetime=resolver_owner;
    const std::vector<SessionSourceObjectAdmissionReceiptV1> scene_admissions{chest_admission};
    SessionAuthoredOpenableSceneProvidersV1 scene_providers;
    scene_providers.owner=scene_policy_context;
    scene_providers.context=scene_policy_context.get();
    scene_providers.make_policy=make_openable_scene_policy;
    scene_providers.validate_visual_row=validate_openable_scene_visual;
    scene_providers.source_row_names=source_openable_names;
    scene_providers.source_row_names_size=sizeof source_openable_names;
    std::shared_ptr<SessionAuthoredOpenableSceneV1> openable_scene;
    check(!bind_session_authored_openable_scene_v1(session,definitions,{},object_assets,
        visual_owner,modern_drop,table_owner,scene_providers,openable_scene,error)&&
        !openable_scene&&!session.retained_object_visual_borrow(chest_id)&&
        session.world()->find_object(chest_id),
        "Authored scene binder accepted current objects without caller admission receipts");
    auto missing_quest_context=std::make_shared<OpenableScenePolicyContext>(*scene_policy_context);
    missing_quest_context->base.interaction.raise_async=nullptr;
    auto missing_quest_providers=scene_providers;
    missing_quest_providers.owner=missing_quest_context;
    missing_quest_providers.context=missing_quest_context.get();
    check(!bind_session_authored_openable_scene_v1(session,definitions,scene_admissions,
        object_assets,visual_owner,modern_drop,table_owner,missing_quest_providers,
        openable_scene,error)&&!openable_scene&&
        !session.retained_object_visual_borrow(chest_id),
        "Missing typed same-Level quest provider partially enrolled an authored Openable scene");
    auto duplicate_admissions=scene_admissions;
    duplicate_admissions.push_back(chest_admission);
    check(!bind_session_authored_openable_scene_v1(session,definitions,duplicate_admissions,
        object_assets,visual_owner,modern_drop,table_owner,scene_providers,
        openable_scene,error)&&!openable_scene,
        "Authored scene binder accepted duplicate prior-admission receipts");
    auto duplicate_row_names=duplicate_openable_row_name(source_openable_names,
        sizeof source_openable_names,"SwampCave_Normal_Chest","Swamp_Normal_Chest");
    auto ambiguous_row_providers=scene_providers;
    ambiguous_row_providers.source_row_names=duplicate_row_names.data();
    ambiguous_row_providers.source_row_names_size=duplicate_row_names.size();
    check(!bind_session_authored_openable_scene_v1(session,definitions,scene_admissions,
        object_assets,visual_owner,modern_drop,table_owner,ambiguous_row_providers,
        openable_scene,error)&&!openable_scene&&
        !session.retained_object_visual_borrow(chest_id),
        "Authored scene binder accepted an ambiguous duplicate source data_desc row name");
    check(bind_session_authored_openable_scene_v1(session,definitions,scene_admissions,
        object_assets,visual_owner,modern_drop,table_owner,scene_providers,
        openable_scene,error),error);
    check(openable_scene&&openable_scene->object_ids().size()==1&&
        session.retained_object_visual_borrow(chest_id),
        "One-entry authored scene binder did not retain the exact admitted source chest placement");

    // Exercise the production-callable current-Level candidate path on a
    // second real decoded chest declaration. The model and transform are
    // derived from its exact source row/placement; only original GameObject
    // runtime fields/providers remain explicit caller inputs.
    const auto second_chest=std::find_if(scene_openables.begin(),scene_openables.end(),
        [&](const auto* candidate){return candidate->stableId!=chest_id&&
            candidate->stableId!=rejected_definition->stableId&&
            candidate->properties.at("data_desc")=="Swamp_Normal_Chest"&&
            !session.world()->find_object(candidate->stableId)&&
            !session.retained_object_visual_borrow(candidate->stableId);});
    check(second_chest!=scene_openables.end(),"No second actual authored chest for enrollment composition");
    WorldObject enrolled_chest;
    enrolled_chest.id=(*second_chest)->stableId;enrolled_chest.name=(*second_chest)->name;
    enrolled_chest.visual.model="go_chest_swamp.bdae";
    enrolled_chest.transform.position={(*second_chest)->placement[12],
        (*second_chest)->placement[13],(*second_chest)->placement[14]};
    check(bind_source_container_objs_v1(enrolled_chest,saved_fields,error),error);
    check(!session.world()->find_object(enrolled_chest.id)&&
        !session.retained_object_visual_borrow(enrolled_chest.id),
        "Second chest fixture must start unpublished and have no retained visual");
    SessionSourceObjectAdmissionRequestV1 enrollment_request;
    enrollment_request.definition=*second_chest;
    enrollment_request.candidate=std::move(enrolled_chest);
    enrollment_request.random_owner=session.actor_binding_lease().lock();
    enrollment_request.with_spawn_random=with_session_spawn_random;
    enrollment_request.online_byte5=[](bool& online,std::string& e){online=false;e.clear();return true;};
    auto malformed_enrollment=enrollment_request;
    malformed_enrollment.candidate.transform.position[0]+=1.0f;
    auto malformed_policy=scene_policy_base;
    const auto malformed_enrollment_rng=session.world()->random_state().calls;
    SessionAuthoredContainerEnrollmentResultV1 malformed_enrollment_result;
    check(!enroll_session_authored_container_v1(session,std::move(malformed_enrollment),
        object_assets,visual_owner,modern_drop,std::move(malformed_policy),{}, {},
        malformed_enrollment_result,error)&&!malformed_enrollment_result.admission.admitted&&
        session.world()->random_state().calls==malformed_enrollment_rng&&
        !session.world()->find_object((*second_chest)->stableId)&&
        !session.retained_object_visual_borrow((*second_chest)->stableId),
        "Misplaced authored candidate reached source RNG or published object/visual state");
    auto enrollment_policy=scene_policy_base;
    SessionAuthoredContainerEnrollmentResultV1 enrollment;
    check(enroll_session_authored_container_v1(session,std::move(enrollment_request),
        object_assets,visual_owner,modern_drop,std::move(enrollment_policy),{}, {},
        enrollment,error),error);
    check(enrollment.admission.admitted&&enrollment.openable&&
        !enrollment.destructible&&enrollment.admission.prior_admission.object_id==(*second_chest)->stableId&&
        session.world()->find_object((*second_chest)->stableId)&&
        session.retained_object_visual_borrow((*second_chest)->stableId),
        "Current-Level authored chest enrollment did not publish only after admission and retain its same-Session owner");
    std::shared_ptr<SessionContainerModernOpenableV1> owner;
    check(openable_scene->find(chest_id,owner,error)&&owner&&owner->source_id()==chest_id,
        "Authored scene binder did not expose its exact chest interaction owner");
    WorldObject rejected_composed_candidate;rejected_composed_candidate.id=rejected_definition->stableId;
    rejected_composed_candidate.name=rejected_definition->name;
    rejected_composed_candidate.visual.model="go_chest_swamp.bdae";
    rejected_composed_candidate.transform.position={rejected_definition->placement[12],
        rejected_definition->placement[13],rejected_definition->placement[14]};
    check(bind_source_container_objs_v1(rejected_composed_candidate,saved_fields,error),error);
    SessionSourceObjectAdmissionRequestV1 rejected_composed_request;
    rejected_composed_request.definition=&*rejected_definition;
    rejected_composed_request.candidate=std::move(rejected_composed_candidate);
    rejected_composed_request.probability274=0;
    rejected_composed_request.random_owner=session.actor_binding_lease().lock();
    rejected_composed_request.with_spawn_random=with_session_spawn_random;
    std::size_t rejected_composed_online_calls=0;
    rejected_composed_request.online_byte5=[&](bool& online,std::string& e){
        ++rejected_composed_online_calls;online=false;e.clear();return true;
    };
    SessionAuthoredContainerEnrollmentResultV1 rejected_composed;
    const auto reject_composed_calls=session.world()->random_state().calls;
    check(enroll_session_authored_container_v1(session,std::move(rejected_composed_request),
        object_assets,visual_owner,modern_drop,std::move(rejected_composed_policy),{}, {},
        rejected_composed,error),error);
    check(!rejected_composed.admission.admitted&&!rejected_composed.openable&&
        !rejected_composed.destructible&&
        rejected_composed.admission.candidate_hidden&&
        rejected_composed.admission.candidate_deleted&&
        rejected_composed.admission.marked_for_deletion,
        "Composed source rejection did not preserve the original hide/Delete/Mark receipt");
    check(!session.world()->find_object(rejected_definition->stableId)&&
        !session.retained_object_visual_borrow(rejected_definition->stableId)&&
        rejected_composed_online_calls==1&&
        session.world()->random_state().calls==reject_composed_calls+1,
        "Composed source rejection must consume its source draw and publish no object, visual, or callbacks");
    std::shared_ptr<SessionContainerModernOpenableV1> rejected_admission;
    check(!SessionContainerModernOpenableV1::create(session,*found,object_assets,
        visual,modern_drop,std::move(missing_admission),rejected_admission,error)&&
        !rejected_admission&&error.find("prior-admission receipt")!=std::string::npos,
        "Modern Openable inferred source admission from WorldObject presence alone: "+error);
    WorldObject* current_chest{};
    check(modern_drop->current_object(*found,current_chest,error)&&
          current_chest==session.world()->find_object(chest_id),
          "Modern composition did not resolve the actual admitted chest WorldObject");
    auto wrong_object_definition=*urn_found;wrong_object_definition.stableId=chest_id;
    check(!modern_drop->current_object(wrong_object_definition,current_chest,error),
          "Modern composition accepted a mismatched stable source ObjectId");
    check(owner&&owner->source_id()==chest_id,error);
    check(owner->source_fields().data_desc=="Swamp_Normal_Chest"&&
          owner->source_fields().data374==chest_row_id&&
          owner->source_fields().state394==2&&owner->source_fields().key_name.empty()&&
          !owner->source_fields().death_reset&&owner->source_fields().key_consume&&
          owner->source_fields().key_qty==1&&owner->source_fields().key_id710==-1&&
          owner->source_fields().opener398==0,
          "Modern admitted chest did not derive original Openable row/constructor fields and saved state");
    auto mismatched_definition=*urn_found;mismatched_definition.stableId=chest_id;
    check(!visual.bind_authored_object(mismatched_definition,object_assets,error),
        "Authored source definition mismatch was accepted for a current WorldObject");

    auto destructible_table=std::make_shared<dh2::world::DestructibleContainerTableV16>();
    check(destructible_table->load(destructible_group0_records_bin,sizeof destructible_group0_records_bin,
        destructible_group0_names_bin,sizeof destructible_group0_names_bin,error),error);
    const auto source_breakable_id=destructible_table->data_id("Swamp_Normal_DestructibleBarrel");
    const auto* source_breakable=destructible_table->row(source_breakable_id);
    check(source_breakable&&source_breakable_id==30&&source_breakable->loot()==9,
        "Actual destructible source row30/table9 unavailable for same-RNG fixture");
    const auto urn_bytes=read_bytes(argv[3],"go_swamp_urn_breakable.bdae");
    std::vector<EmbeddedSceneClip> urn_clips;
    check(decode_embedded_scene_clips(urn_bytes.data(),urn_bytes.size(),urn_clips,error),error);
    check(urn_clips.size()==3&&urn_clips[0].name=="activate"&&urn_clips[1].name=="idle"&&
        urn_clips[2].name=="idleactive","Recovered urn clip order/count differs from exact BDAE");
    auto breakable_fixture=std::make_shared<BreakableFixture>();
    breakable_fixture->resolver=&resolver;
    breakable_fixture->visual=&visual;breakable_fixture->clips=static_cast<std::uint32_t>(urn_clips.size());
    SessionDestructibleInteractionServicesV1 breakable_services;
    breakable_services.session_identity=&session;breakable_services.session_lease=visual.session_lease();
    breakable_services.actor_context=&resolver;breakable_services.resolve_actor=Resolver::resolve;
    breakable_services.table=destructible_table;
    breakable_services.visual_context=breakable_fixture.get();
    breakable_services.visual_asset=BreakableFixture::visual_asset;
    breakable_services.has_visual=BreakableFixture::has_visual;
    breakable_services.animation_count=BreakableFixture::animation_count;
    breakable_services.bind_retained_visual=BreakableFixture::bind_callbacks;
    breakable_services.play_retained_clip=BreakableFixture::play_clip;
    breakable_services.retained_scene_flags=BreakableFixture::scene_flags;
    breakable_services.detach_physical=BreakableFixture::detach;
    breakable_services.play_sound_3d=BreakableFixture::sound;
    breakable_services.source_on_interact=BreakableFixture::source_interact;
    breakable_services.load_object_script=BreakableFixture::load_script;
    breakable_services.raise_destroy_quest=BreakableFixture::raise_destroy;
    breakable_services.has_script=BreakableFixture::has_script;
    breakable_services.script_call=BreakableFixture::script_call;
    breakable_services.as_character=BreakableFixture::as_character;
    breakable_services.increment_stat=BreakableFixture::increment_stat;
    breakable_services.get_stat=BreakableFixture::get_stat;
    breakable_services.is_local_player=BreakableFixture::is_local;
    breakable_services.trophy_id=BreakableFixture::trophy_id;
    breakable_services.unlock_trophy=BreakableFixture::unlock_trophy;
    breakable_services.loot=loot_services;
    SessionContainerActorBorrowV1 service_borrow;
    check(breakable_services.resolve_actor(breakable_services.actor_context,
        breakable_services.session_identity,urn_id,service_borrow,error),error);
    check(service_borrow.session_identity==breakable_services.session_identity&&
        service_borrow.actor_id==urn_id&&service_borrow.definition->stableId==urn_id&&
        service_borrow.object&&service_borrow.destructible_state&&service_borrow.binding_lifecycle&&
        same_owner(service_borrow.session_lease,breakable_services.session_lease),
        "Destructible services did not retain matching current object/session inputs");
    auto breakable_provider_lifetime=std::make_shared<BreakableProviderLifetime>();
    breakable_provider_lifetime->resolver=resolver_owner;
    breakable_provider_lifetime->fixture=breakable_fixture;
    SessionAdmittedDestructibleProvidersV1 breakable_providers;
    breakable_providers.owner=breakable_provider_lifetime;
    breakable_providers.context=breakable_fixture.get();
    breakable_providers.validate_visual_row=BreakableFixture::validate_visual_row;
    auto missing_breakable_provider=breakable_services;
    missing_breakable_provider.raise_destroy_quest=nullptr;
    std::shared_ptr<SessionAdmittedDestructibleV1> breakable;
    check(!SessionAdmittedDestructibleV1::bind(session,*urn_found,{},object_assets,
        visual_owner,missing_breakable_provider,breakable_providers,breakable,error)&&
        !breakable&&!session.retained_object_visual_borrow(urn_id),
        "Destructible binder accepted a missing source quest endpoint or partially bound visual");
    check(!SessionAdmittedDestructibleV1::bind(session,*urn_found,urn_admission,
        object_assets,visual_owner,missing_breakable_provider,breakable_providers,
        breakable,error)&&!breakable&&!session.retained_object_visual_borrow(urn_id),
        "Destructible binder accepted missing DestroyGameObject endpoint");
    auto missing_script_provider=breakable_services;
    missing_script_provider.script_call=nullptr;
    check(!SessionAdmittedDestructibleV1::bind(session,*urn_found,urn_admission,
        object_assets,visual_owner,missing_script_provider,breakable_providers,
        breakable,error)&&!breakable&&!session.retained_object_visual_borrow(urn_id),
        "Destructible binder accepted missing source OnOpen script endpoint");
    auto missing_urn_rng=breakable_services;
    missing_urn_rng.loot.gameplay_rng=nullptr;
    missing_urn_rng.loot.with_gameplay_rng=nullptr;
    check(!SessionAdmittedDestructibleV1::bind(session,*urn_found,urn_admission,
        object_assets,visual_owner,missing_urn_rng,breakable_providers,
        breakable,error)&&!breakable&&!session.retained_object_visual_borrow(urn_id)&&
        error.find("source loot RNG and drop-store endpoints")!=std::string::npos,
        "Urn binder must reject a missing shared LootRandom8 loan before visual enrollment");
    auto missing_urn_store=breakable_services;
    missing_urn_store.loot.drop_item={};
    missing_urn_store.loot.drop_item_with_rng={};
    check(!SessionAdmittedDestructibleV1::bind(session,*urn_found,urn_admission,
        object_assets,visual_owner,missing_urn_store,breakable_providers,
        breakable,error)&&!breakable&&!session.retained_object_visual_borrow(urn_id)&&
        error.find("source loot RNG and drop-store endpoints")!=std::string::npos,
        "Urn binder must reject a missing same-store sink before visual enrollment");
    auto mismatched_urn_admission=urn_admission;
    mismatched_urn_admission.prior_admission.object_id=chest_id;
    check(!SessionAdmittedDestructibleV1::bind(session,*urn_found,mismatched_urn_admission,
        object_assets,visual_owner,breakable_services,breakable_providers,
        breakable,error)&&!breakable&&!session.retained_object_visual_borrow(urn_id),
        "Destructible binder accepted admission receipt for a different ObjectId");
    // Rebuild the actual decoded urn candidate and exercise the new one-call
    // current-Level enrollment path after the lower-level negative receipt and
    // provider checks above. The source admission owner is the only publisher.
    check(session.world()->remove_object(urn_id),
        "Could not reset the isolated urn candidate before enrollment-path test");
    WorldObject enrolled_urn;enrolled_urn.id=urn_id;enrolled_urn.name=urn_found->name;
    enrolled_urn.visual.model="go_swamp_urn_breakable.bdae";
    enrolled_urn.transform.position={urn_found->placement[12],urn_found->placement[13],
        urn_found->placement[14]};
    check(bind_source_container_objs_v1(enrolled_urn,{1,1,0,2},error),error);
    SessionSourceObjectAdmissionRequestV1 urn_enrollment_request;
    urn_enrollment_request.definition=&*urn_found;
    urn_enrollment_request.candidate=std::move(enrolled_urn);
    urn_enrollment_request.random_owner=session.actor_binding_lease().lock();
    urn_enrollment_request.with_spawn_random=with_session_spawn_random;
    urn_enrollment_request.online_byte5=[](bool& online,std::string& e){online=false;e.clear();return true;};
    SessionAuthoredContainerEnrollmentResultV1 urn_enrollment;
    check(enroll_session_authored_container_v1(session,std::move(urn_enrollment_request),
        object_assets,visual_owner,modern_drop,{},breakable_services,breakable_providers,
        urn_enrollment,error),error);
    urn_admission=urn_enrollment.admission;
    breakable=urn_enrollment.destructible;
    check(breakable&&breakable->source_id()==urn_id&&
        session.retained_object_visual_borrow(urn_id)&&
        breakable_fixture->script_loads==1&&resolver.destructible.stages==0,
        "Recovered urn must bind its exact prior-admitted visual/source owner with no staged hit");
    check(openable_scene->interact(chest_id,1,error),error);
    const auto enrolled_store_before=store.size();
    check(enrollment.openable->interact(1,error),error);
    SourceContainerObjsFieldsV1 after_input;
    check(read_source_container_objs_v1(*session.world()->find_object(chest_id),after_input,error),error);
    check(after_input.state394==3,"Actual chest input did not persist state3/activate");

    InputActions input;
    const auto rng_calls_before_chest=session.world()->random_state().calls;
    check(session.update(.25,input,{0,0,0},0,error),error);
    check(session.world()->random_state().calls>rng_calls_before_chest,
          "Container table selection did not borrow the same persisted Session RNG stream");
    check(store.size()>0,"Actual retained chest BRES opened event did not publish source loot");
    check(store.size()>enrolled_store_before,
          "Enrolled second source chest did not route its retained opened marker through the same-session drop/store owner");
    const auto chest_position=session.world()->find_object(chest_id)->transform.position;
    std::vector<loot::RuntimeWorldItemRenderV1> packets;
    check(store.render_items(packets,error),error);
    check(packets.size()==store.size(),"Existing store render enumeration differs from chest drops");
    std::size_t chest_drop_count{};
    for(std::size_t i=0;i<packets.size();++i){
        loot::RuntimeWorldItemEntryV1 entry;check(store.inspect(packets[i].identity,entry,error),error);
        if(entry.source_outcome.source_actor==chest_id){
            ++chest_drop_count;
            check(entry.source_outcome.killer_actor==1&&entry.source_position==chest_position&&
                  packets[i].position==chest_position,
                  "Chest drop lost actual opener identity or authored transform");
        }
    }
    check(chest_drop_count>0,"Same-store entries do not identify the authored chest as source");
    check(session.update(1.8,input,{0,0,0},0,error),error);
    SourceContainerObjsFieldsV1 completed;
    check(read_source_container_objs_v1(*session.world()->find_object(chest_id),completed,error),error);
    check(completed.state394==4&&source_container_restore_visual_v1(completed.state394)==
        SourceContainerRestoreVisualV1::idleactive,"Authored activation did not finish to state4");
    check(breakable->interact(1,error),error);
    const auto urn_rng_calls_before=session.world()->random_state().calls;
    check(session.update(.30,input,{0,0,0},0,error),error);
    check(breakable_fixture->on_open_calls==1&&breakable_fixture->quest_calls==2,
        "Recovered urn source damage/opened marker did not dispatch the two original quest sites and one OnOpen");
    check(breakable_fixture->saw_opened_event,
        "Recovered BDAE timeline did not deliver its exact opened event to the source owner");
    check(session.world()->random_state().calls>urn_rng_calls_before,
        "Authored urn row30/table9 opened marker did not advance the current Session LootRandom8 stream");
    const auto urn_drop_count_before_duplicate=store.size();
    check(breakable->animation_event(breakable_fixture->opened_event,error),error);
    check(store.size()==urn_drop_count_before_duplicate&&breakable_fixture->quest_calls==2&&
        breakable_fixture->on_open_calls==1,
        "Duplicate authored urn opened marker replayed source quest, loot, or OnOpen");
    const auto urn_position=session.world()->find_object(urn_id)->transform.position;
    bool found_urn_drop=false;
    std::size_t urn_drop_records{};
    check(store.render_items(packets,error),error);
    for(const auto& packet:packets){
        loot::RuntimeWorldItemEntryV1 entry;check(store.inspect(packet.identity,entry,error),error);
        if(entry.source_outcome.source_actor==urn_id){
            found_urn_drop=true;
            ++urn_drop_records;
            check(entry.source_outcome.loot_table==9,
                "Authored urn row30 drop lost its stored source loot-table identity 9");
            check(entry.source_outcome.killer_actor==1&&entry.source_position==urn_position,
                "Urn breakable drop lost its exact same-world source transform");
        }
    }
    check(urn_drop_records>0,"Authored urn row30/table9 did not publish a source record into the same store");
    if(!found_urn_drop){
        std::string detail="Recovered urn opened marker did not publish through the existing item store; store="+
            std::to_string(store.size())+
            " urn_id="+std::to_string(urn_id)+" quest="+
            std::to_string(breakable_fixture->quest_calls)+" open="+
            std::to_string(breakable_fixture->on_open_calls)+" audio="+
            std::to_string(breakable_fixture->audio_calls)+" state="+
            std::to_string(resolver.destructible.state394);
        for(const auto& packet:packets){
            loot::RuntimeWorldItemEntryV1 entry;
            if(store.inspect(packet.identity,entry,error))
                detail+=" "+std::to_string(entry.source_outcome.source_actor)+"/"+
                    std::to_string(entry.source_outcome.loot_table);
        }
        check(false,detail);
    }
    check(session.update(1.7,input,{0,0,0},0,error),error);
    SourceContainerObjsFieldsV1 urn_completed;
    check(read_source_container_objs_v1(*session.world()->find_object(urn_id),urn_completed,error),error);
    check(urn_completed.state394==4,"Urn source animation completion did not persist state4");
    AssetCatalog itemdrop_assets(argv[5]),effect_assets(argv[6]);
    CpuTextureLeases texture_leases;
    SourceWorldItemDropTextureServicesV1 texture_services;
    texture_services.upload=[&](const TextureImage& image,std::uint32_t& texture,std::string& e){
        return CpuTextureLeases::upload(&texture_leases,image,texture,e);
    };
    texture_services.release=[&](std::uint32_t texture){CpuTextureLeases::release(&texture_leases,texture);};
    SourceWorldItemDropMaterialBindingsV1 material_bindings(itemdrop_assets,
        std::move(texture_services),&effect_assets);
    std::size_t material_calls{},render_submissions{};
    std::shared_ptr<const SourceWorldItemDropRenderFrameV1> source_frame;
    SourceWorldItemDropRenderServicesV1 render_services;
    render_services.material=[&](const SourceWorldItemDropMaterialV1& request,
            Material& output,std::string& e){
        check(request.authored_material&&request.authored_scene_material&&request.source_image&&
            request.resource_uri==source_item_model_resource_uri_v1,
            "Drop renderer lost exact itemdrops source material/scene lease");
        if(request.visual_uri=="root_itemdrop_Gold_01")
            check(request.authored_scene_material->effect_file=="GL_Diffuse_L1_VC_iPhone.bdae"&&
                request.authored_scene_material->effect_uri=="#Multilight-fx"&&
                request.authored_scene_material->gles2_technique=="L1_Vc_Al_Sp_----_----_----",
                "Gold drop material differed from its exact source effect technique");
        ++material_calls;return material_bindings.bind(request,output,e);
    };
    render_services.submit=[&](std::shared_ptr<const SourceWorldItemDropRenderFrameV1> frame,
            std::string&){++render_submissions;source_frame=std::move(frame);return true;};
    std::unique_ptr<SourceWorldItemDropRenderV1> drop_renderer;
    check(SourceWorldItemDropRenderV1::load(itemdrop_assets,store,audiovisual,
        std::move(render_services),drop_renderer,error),error);
    std::shared_ptr<const SourceWorldItemDropRenderFrameV1> prepared;
    check(drop_renderer->prepare(prepared,error),error);
    check(prepared&&prepared->renderer_ready&&material_calls>0&&!texture_leases.images.empty(),
        "Same-store Potion/Gold source geometry did not acquire original material/pass/texture leases");
    const auto urn_drop=std::find_if(packets.begin(),packets.end(),[&](const auto& packet){
        loot::RuntimeWorldItemEntryV1 entry;return store.inspect(packet.identity,entry,error)&&
            entry.source_outcome.source_actor==urn_id;
    });
    check(urn_drop!=packets.end(),"Source renderer fixture lost the exact urn drop ID");
    loot::RuntimeWorldItemEntryV1 urn_drop_entry;
    check(store.inspect(urn_drop->identity,urn_drop_entry,error),error);
    const auto urn_item_id=urn_drop_entry.source_outcome.item_id;
    const auto urn_draw=std::find_if(prepared->draws.begin(),prepared->draws.end(),[&](const auto& draw){
        return draw.identity==urn_drop->identity;
    });
    check(urn_item_id==418||urn_item_id==925,"Barrel_Level_01 selected unsupported unexpected item");
    check(urn_draw!=prepared->draws.end()&&urn_draw->source_pass_ready&&urn_draw->mesh&&
        urn_draw->source_record.source_actor==urn_id&&urn_draw->source_position==urn_position&&
        urn_draw->source_retention,
        "Same-store urn drop did not resolve to source Potion/Gold draw packet and exact transform");
    check(drop_renderer->submit(error)&&render_submissions==1&&source_frame&&
        source_frame->renderer_ready,"Source itemdrop frame did not reach caller submission endpoint");
    const auto drops_before_restore=store.size();
    auto character=make_default_character("container-live-player","Container Live Player","warrior");
    session.actor(1)->persistent_character_id=character.id;GameSave save;
    check(capture_game_save(level,1,character,*session.world(),save,error)&&save.version==2,error);
    session.detach_for_restore();
    check(restore_game_save(save,level,*session.world(),character,error)&&session.rebind_after_restore(error),error);
    SourceContainerLootServicesV1 stale_services;
    check(!modern_drop->services(stale_services,error)&&!stale_services.with_gameplay_rng,
          "Pre-restore modern drop binding survived a replaced Session lease");
    check(visual.refresh_after_restore(error),error);
    check(visual.restore_authored_object(*found,object_assets,error),error);
    check(visual.restore_authored_object(*urn_found,object_assets,error),error);
    check(!owner->interact(1,error),
          "Pre-restore modern openable binding survived a replaced Session lease");
    SessionContainerModernDropInputsV1 restored_drop_inputs;
    restored_drop_inputs.tables=tables;restored_drop_inputs.powers=powers.borrow();
    restored_drop_inputs.entry={&entries,entry_service};restored_drop_inputs.store=&store;
    restored_drop_inputs.gold_bonus256=fixture_gold_bonus;
    std::shared_ptr<SessionContainerModernDropV1> restored_drop;
    check(SessionContainerModernDropV1::create(session,std::move(restored_drop_inputs),
        restored_drop,error),error);
    SessionContainerModernOpenablePolicyV1 restored_policy;
    restored_policy.other_actor_context=&resolver;
    restored_policy.resolve_other_actor=Resolver::resolve;
    restored_policy.source_fields=resolver.openable;
    restored_policy.authored_table=std::make_shared<dh2::world::OpenableContainerTableV1>(openable_table);
    restored_policy.prior_admission.session_identity=&session;
    restored_policy.prior_admission.session_lease=session.actor_binding_lease().lock();
    restored_policy.prior_admission.admission_owner=std::make_shared<int>(2);
    restored_policy.prior_admission.object_id=chest_id;
    restored_policy.prior_admission.definition_name=found->name;
    restored_policy.prior_admission.data_desc="Swamp_Normal_Chest";
    restored_policy.source.resolve_row=resolve_source_openable_row;
    std::shared_ptr<SessionContainerModernOpenableV1> restored_owner;
    check(SessionContainerModernOpenableV1::create(session,*found,object_assets,
        visual,restored_drop,std::move(restored_policy),restored_owner,error),error);
    check(restored_owner->initialize_admitted(error),error);
    const auto drops_after_restore=store.size();
    check(restored_owner->interact(1,error),error);
    RetainedAnimationEvent stale_opened;stale_opened.name="opened";
    check(restored_owner->animation_event(stale_opened,error),error);
    check(store.size()==drops_after_restore,
        "State4 silent Openable rebind replayed input, opened marker, or loot");
    check(session.update(.25,input,{0,0,0},0,error),error);
    check(store.size()==drops_before_restore,"Silent state4 restore replayed chest loot/open event");
    SourceContainerObjsFieldsV1 restored;
    check(read_source_container_objs_v1(*session.world()->find_object(chest_id),restored,error),error);
    check(restored.state394==4,"GameSave restore lost source state4");
    SourceContainerObjsFieldsV1 urn_restored;
    check(read_source_container_objs_v1(*session.world()->find_object(urn_id),urn_restored,error),error);
    check(urn_restored.state394==4,"GameSave restore lost urn source state4");
    check(breakable_fixture->stat_calls==1,"Urn source breakable interaction did not update its Character stat once");
    check(!breakable->interact(1,error)&&error.find("stale Session lease")!=std::string::npos,
        "Pre-restore admitted Destructible binding survived a replaced Session lease");
    std::cout<<"PASS SessionAuthoredOpenableSceneV1 collects the admitted chest; enroll_session_authored_container_v1 admits and opens a second decoded chest and admits/binds the decoded urn. Their source markers route drops through the same-session RNG/store; misplaced candidates and missing/duplicate receipts/providers reject before publication. Silent state4 GameSave restore rejects replay and stale bindings reject use. Production candidate construction and online/current-Level/quest/audio/physical/script/stat providers remain caller gaps; Lua _Summon execution is not claimed\n";
    return 0;
}catch(const std::exception& exception){std::cerr<<exception.what()<<'\n';return 1;}}
