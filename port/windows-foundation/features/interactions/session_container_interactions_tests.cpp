#include "session_openable_interaction_v1.hpp"
#include "session_destructible_interaction_v1.hpp"
#include "world_object_container_state_v1.hpp"
#include "../loot/runtime_world_item_interaction_v1.hpp"
#include "../../../game-data/loot_power_creation_v7.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include "../../../level-world/tests/openable_container_real_cache_fixture_v1.hpp"
#include "../../../level-world/tests/destructible_data_fixture_v16.inc"
#include "../../../engine-animation/events.hpp"
#include "../../../engine-animation/animation.hpp"
#include "../../../engine-resources/resources.hpp"
#include "../../../scene-materials/scene.hpp"

namespace {
std::vector<std::uint8_t> read(const std::string& root, const char* file) {
    std::ifstream input(std::filesystem::path(root) / file, std::ios::binary);
    assert(input);
    return {std::istreambuf_iterator<char>(input), {}};
}
dh2::data::Bytes view(const std::vector<std::uint8_t>& bytes) {
    return {bytes.data(), bytes.size()};
}
struct EntryQueryFixture { unsigned calls{}; };
bool entry_service(void* context, const dh2::data::LootEntryRequestV8& request,
                   std::int32_t& value, std::string& error) {
    ++static_cast<EntryQueryFixture*>(context)->calls;
    if (request.operation == dh2::data::LootEntryOperationV8::debug_load ||
        request.operation == dh2::data::LootEntryOperationV8::debug_query) {
        value = 0;
        return true;
    }
    if (request.operation == dh2::data::LootEntryOperationV8::mage_count ||
        request.operation == dh2::data::LootEntryOperationV8::rogue_count ||
        request.operation == dh2::data::LootEntryOperationV8::warrior_count) {
        // Explicit fixture value; production injects the same-session
        // PlayerManager count service, never this default.
        value = 0;
        return true;
    }
    error = "Fixture does not replace original PlayerManager entry services";
    return false;
}
struct Sink {
    std::vector<dh::foundation::interactions::SourceContainerDropItemV1> drops;
    static bool drop(void* context,
                     const dh::foundation::interactions::SourceContainerDropItemV1& item,
                     std::string& error) {
        auto& self = *static_cast<Sink*>(context);
        if (!item.source_actor || !item.opener_actor || !item.source_definition ||
            (!item.source_state && !item.source_object) || !item.selected.entry || !item.selected.item ||
            !item.selected.quantity) {
            error = "Drop sink received an incomplete original source selection";
            return false;
        }
        self.drops.push_back(item);
        return true;
    }
};
struct WorldDropSink {
    dh::foundation::loot::RuntimeWorldItemAdapterV1* store{};
    dh2::data::LootRandom8V2* rng{};
    std::vector<dh::foundation::interactions::SourceContainerDropItemV1> drops;
    std::vector<dh::foundation::loot::RuntimeWorldItemIdV1> world_ids;
    static bool drop(void* context,
        const dh::foundation::interactions::SourceContainerDropItemV1& item,
        std::string& error) {
        auto& self=*static_cast<WorldDropSink*>(context);
        if(!self.store||(!item.source_state&&!item.source_object)||!item.selected.item||!item.selected.entry||
           !item.selected.quantity){error="same-session world-item store sink is incomplete";return false;}
        dh::foundation::loot::RuntimeWorldItemRecordV1 record;
        record.source_actor=item.source_actor;
        record.killer_actor=item.opener_actor;
        record.loot_table=item.loot_table;
        record.item_id=item.selected.id;
        record.quantity=item.selected.quantity;
        record.authored_item=item.selected.item;
        record.authored_entry=item.selected.entry;
        if(record.authored_item->record.words[22]==13){
            if(!self.rng){error="source gold valuation needs the shared loot RNG";return false;}
            std::int32_t value{};
            if(dh2_loot_item_value_v7(&value,self.rng,&record.authored_item->record,nullptr,0,0)!=0){
                error="source gold value kernel rejected authored row";return false;
            }
            record.resolved_gold_value=value;
        }
        dh::foundation::loot::RuntimeWorldItemIdV1 identity{};
        if(item.source_object){
            if(item.source_state||!item.source_definition||
               item.source_object->transform.position!=item.source_position){
                error="neutral object drop must preserve its exact source identity and transform";
                return false;
            }
            if(!self.store->publish_source_object_drop(record,*item.source_definition,
                *item.source_object,identity,error))return false;
        }else if(!self.store->publish_death_drop(record,*item.source_state,identity,error))return false;
        self.drops.push_back(item);
        self.world_ids.push_back(identity);
        return true;
    }
};
struct CharacterOwner {
    dh::foundation::ActorId id{dh::foundation::invalid_actor_id};
    std::shared_ptr<dh::foundation::CharacterState> state;
    static bool resolve(void* context,dh::foundation::ActorId id,
        std::shared_ptr<dh::foundation::CharacterState>& out,std::string& error) {
        auto& self=*static_cast<CharacterOwner*>(context);
        if(id!=self.id||!self.state){error="test same-session CharacterState missing";return false;}
        out=self.state;return true;
    }
};
struct SessionActors {
    const void* session{};
    std::shared_ptr<const void> lease;
    std::map<dh::foundation::ActorId, dh::foundation::ActorDefinition> definitions;
    std::map<dh::foundation::ActorId, dh::foundation::ActorState> states;
    std::map<dh::foundation::ActorId, dh::foundation::WorldObject> objects;
    dh2::world::OpenableContainerFieldsV1 openable;
    dh2::world::OpenableContainerFieldsV1 object_openable;
    dh::foundation::interactions::SessionDestructibleFieldsV1 destructible;
    std::uint64_t lifecycle{1};
    static bool resolve(void* context, const void* session, dh::foundation::ActorId id,
                        dh::foundation::interactions::SessionContainerActorBorrowV1& out,
                        std::string& error) {
        auto& self = *static_cast<SessionActors*>(context);
        if (session != self.session) { error = "foreign session"; return false; }
        const auto definition = self.definitions.find(id);
        const auto state = self.states.find(id);
        const auto object = self.objects.find(id);
        if (definition == self.definitions.end() ||
            (state == self.states.end() && object == self.objects.end())) {
            error = "ActorId absent from source session";
            return false;
        }
        out.session_identity = self.session;
        out.actor_id = id;
        out.definition = &definition->second;
        out.state = state == self.states.end() ? nullptr : &state->second;
        out.object = object == self.objects.end() ? nullptr : &object->second;
        out.openable_state = id == 42 ? &self.object_openable :
            (id == 41 ? &self.openable : nullptr);
        out.destructible_state = id == 51 ? &self.destructible : nullptr;
        out.session_lease = self.lease;
        out.binding_lifecycle = self.lifecycle;
        return true;
    }
};
struct VisualCallbacks {
    dh::foundation::interactions::SessionContainerEventCallbackV1 event;
    dh::foundation::interactions::SessionContainerCompletionCallbackV1 finished;
    unsigned binds{}, clips{}, flags{};
    dh::foundation::ActorId expected_actor{41};
    static bool bind(void* context, const void* session, dh::foundation::ActorId actor,
        dh::foundation::interactions::SessionContainerEventCallbackV1 event,
        dh::foundation::interactions::SessionContainerCompletionCallbackV1 finished,
        std::string& error) {
        auto& self = *static_cast<VisualCallbacks*>(context);
        if (!session || actor != self.expected_actor || !event || !finished) {
            error = "invalid same-session retained visual binding";
            return false;
        }
        self.event = std::move(event);
        self.finished = std::move(finished);
        ++self.binds;
        return true;
    }
    static bool play(void* context, const void* session, dh::foundation::ActorId actor,
        const char* clip, bool& accepted, std::string& error) {
        auto& self = *static_cast<VisualCallbacks*>(context);
        if (!session || actor != self.expected_actor || !clip) { error = "invalid same-session clip"; return false; }
        ++self.clips;
        accepted = std::string(clip) == "activate" || std::string(clip) == "idleactive";
        return true;
    }
    static bool scene(void* context, const void* session, dh::foundation::ActorId actor,
        std::uint32_t, std::uint32_t, std::string& error) {
        auto& self = *static_cast<VisualCallbacks*>(context);
        if (!session || actor != self.expected_actor) { error = "invalid same-session scene flags"; return false; }
        ++self.flags;
        return true;
    }
};
struct BreakableCallbacks {
    std::vector<std::uint8_t>* bytes{};
    dh2::resources::BresView* bres{};
    std::function<bool(dh::foundation::ActorId,
        const dh::foundation::RetainedAnimationEvent&, std::string&)> event;
    bool script_loaded{};
    unsigned quest_calls{}, open_calls{};
    static bool visual_asset(void* raw, const void*, dh::foundation::ActorId,
        std::int32_t visual, std::string& error) {
        auto& self=*static_cast<BreakableCallbacks*>(raw);
        if (visual!=70 || !self.bytes || !self.bres) {
            error="breakable source visual id mismatch"; return false;
        }
        return dh2_bres_open(self.bres,self.bytes->data(),self.bytes->size())==
            dh2::resources::BresError::ok;
    }
    static bool load_script(void* raw, const void*, dh::foundation::ActorId,
        const char* script,const char* directory,std::string& error) {
        auto& self=*static_cast<BreakableCallbacks*>(raw);
        if (!script || std::string(script)!="moth_spawn_container" || !directory ||
            std::string(directory)!="data/scripts/objects/") {
            error="breakable source script declaration mismatch";return false;
        }
        self.script_loaded=true;return true;
    }
    static bool has_visual(void*,const void*,dh::foundation::ActorId,bool& yes,std::string&) {
        yes=true;return true;
    }
    static bool bind(void* raw,const void*,dh::foundation::ActorId actor,
        dh::foundation::interactions::SessionContainerEventCallbackV1 callback,
        dh::foundation::interactions::SessionContainerCompletionCallbackV1,std::string& error) {
        auto& self=*static_cast<BreakableCallbacks*>(raw);
        if(actor!=51||!callback){error="breakable retained event callback missing";return false;}
        self.event=std::move(callback);return true;
    }
    static bool animation_count(void* raw,const void*,dh::foundation::ActorId,
        std::uint32_t& count,std::string&) {
        const auto& self=*static_cast<BreakableCallbacks*>(raw);
        count=dh2_bres_library_count(self.bres,dh2::resources::Library::animation_clip);
        return true;
    }
    static bool quest(void* raw,const void*,dh::foundation::ActorId source,
        dh::foundation::ActorId actor,std::int32_t row,std::string& error) {
        auto& self=*static_cast<BreakableCallbacks*>(raw);
        if(source!=51||actor!=dh::foundation::invalid_actor_id||row!=30){
            error="breakable DestroyGameObject payload mismatch";return false;
        }
        ++self.quest_calls;return true;
    }
    static bool has_script(void*,const void*,dh::foundation::ActorId,bool& yes,std::string&) {
        yes=true;return true;
    }
    static bool script_call(void* raw,const void*,dh::foundation::ActorId source,
        const char* method,dh::foundation::ActorId opener,const char*,std::string& error) {
        auto& self=*static_cast<BreakableCallbacks*>(raw);
        if(source!=51||!method||std::string(method)!="OnOpen"||opener!=7){
            error="breakable OnOpen invocation mismatch";return false;
        }
        ++self.open_calls;return true;
    }
};
struct EventObservation { std::string name; std::int32_t lag{}; };
struct RoomMapping {
    std::map<dh::foundation::ActorId,std::int32_t> rooms;
    static bool resolve(void* raw,const dh::foundation::ActorDefinition& definition,
        std::int32_t& room,std::string& error){
        auto& self=*static_cast<RoomMapping*>(raw);
        const auto found=self.rooms.find(definition.stableId);
        if(found==self.rooms.end()){error="fixture room absent";return false;}
        room=found->second;error.clear();return true;
    }
};
void capture_event(const dh2::animation::TriggeredEvent* event, void* context) {
    auto& out = *static_cast<EventObservation*>(context);
    out.name = event->name;
    out.lag = event->lag_ms;
}
}

int main(int argc, char** argv) {
    assert(argc == 2);
    const std::string root = argv[1];
    auto records = read(root, "loot_table_pyarray.bin");
    auto names = read(root, "loot_table_pyarraynames.bin");
    auto schema = read(root, "loot_table_pystructnames.bin");
    auto item_power = read(root, "item_powers_pyarray.bin");
    auto item_power_names = read(root, "item_powers_pyarraynames.bin");
    auto item_power_schema = read(root, "item_powers_pystructnames.bin");
    auto monopoly = read(root, "item_powers_monopoly_pyarray.bin");
    auto monopoly_names = read(root, "item_powers_monopoly_pyarraynames.bin");
    auto monopoly_schema = read(root, "item_powers_monopoly_pystructnames.bin");
    auto quantity = read(root, "num_prob_records_v7.bin");

    std::string error;
    dh2::data::LootTablesV2 table_owner;
    assert(table_owner.load(view(records), view(names), view(schema), error));
    dh2::data::LootPowerInputsV7 power_input{
        view(item_power), view(item_power_names), view(item_power_schema),
        view(monopoly), view(monopoly_names), view(monopoly_schema),
        view(quantity), view(names), view(schema)};
    dh2::data::ItemPowerTablesV5 power_definition;
    assert(power_definition.load(power_input.powers, power_input.power_names,
                                 power_input.power_schema, error));
    dh2::data::LootPowerResourcesV7 power_owner;
    assert(power_owner.load(power_input, power_definition.borrow(), error));

    const auto tables = table_owner.borrow();
    const auto loot_name = std::find(tables.loot_names().begin(), tables.loot_names().end(),
                                     "Barrel_Level_01");
    assert(loot_name != tables.loot_names().end());
    const auto table_id = static_cast<std::int32_t>(loot_name - tables.loot_names().begin());
    const auto& authored = tables.loots().at(static_cast<std::size_t>(table_id));
    assert(authored.sub_loots.size() == 3);
    assert(authored.sub_loots[0] == 124 && authored.sub_loots[1] == 124 && authored.sub_loots[2] == 124);

    dh::foundation::ActorDefinition definition;
    definition.stableId = 41;
    definition.name = "Swamp_Normal_DestructibleBarrel";
    dh::foundation::ActorState state;
    state.id = 41;
    state.definition_id = definition.name;
    Sink sink;
    dh2::data::LootRandom8V2 rng{};
    EntryQueryFixture entry_queries;
    dh::foundation::interactions::SourceContainerLootServicesV1 services;
    services.tables = tables;
    services.powers = power_owner.borrow();
    services.entry = {&entry_queries, entry_service};
    services.gameplay_rng = &rng;
    services.context = &sink;
    services.drop_item = Sink::drop;
    dh::foundation::interactions::SourceContainerLootReceiptV1 receipt;
    std::unique_ptr<dh::foundation::interactions::SourceContainerLootV1> drops;
    bool selected_any = false;
    for (std::uint32_t seed = 1; seed < 10000 && !selected_any; ++seed) {
        sink.drops.clear();
        rng = {seed, 0};
        drops = std::make_unique<dh::foundation::interactions::SourceContainerLootV1>();
        assert(drops->drop(41, 1, definition, state, 7, table_id, -1, false,
                           services, receipt, error));
        selected_any = receipt.selected_items != 0;
    }
    assert(selected_any);
    assert(receipt.state == dh::foundation::interactions::SourceContainerLootStateV1::completed);
    assert(receipt.selected_items == sink.drops.size());
    assert(receipt.delivered_items == sink.drops.size());
    assert(entry_queries.calls > 0);
    for (const auto& item : sink.drops) {
        assert(item.source_actor == 41 && item.opener_actor == 7 &&
               item.loot_table == table_id && item.fixed_powers == -1 && !item.source_flag);
        assert(item.source_definition == &definition && item.source_state == &state);
    }
    const auto rng_calls = rng.calls;
    const auto delivered = receipt.delivered_items;
    assert(!drops->drop(41, 1, definition, state, 7, table_id, -1, false,
                        services, receipt, error));
    assert(error == "Container loot already attempted for same ActorId lifecycle");
    assert(rng.calls == rng_calls && sink.drops.size() == delivered);

    // Neutral source objects use their authored ObjectId/transform directly;
    // this route never fabricates an ActorState or combat sheet.
    dh::foundation::WorldObject neutral_source;
    neutral_source.id=42;
    neutral_source.name="Swamp_Normal_DestructibleBarrel";
    neutral_source.transform.position={12.5f,-3.0f,8.25f};
    auto neutral_definition=definition;
    neutral_definition.stableId=42;
    dh::foundation::interactions::SourceContainerLootV1 neutral_loot;
    dh::foundation::interactions::SourceContainerLootReceiptV1 neutral_receipt;
    bool neutral_selected=false;
    for(std::uint32_t seed=1;seed<10000&&!neutral_selected;++seed){
        sink.drops.clear(); rng={seed,0};
        dh::foundation::interactions::SourceContainerLootV1 candidate;
        assert(candidate.drop(42,3,neutral_definition,neutral_source,7,table_id,-1,false,
            services,neutral_receipt,error));
        neutral_selected=neutral_receipt.selected_items!=0;
        if(neutral_selected){
            assert(!sink.drops.empty());
            for(const auto& item:sink.drops){
                assert(item.source_object==&neutral_source&&!item.source_state);
                assert(item.source_position==neutral_source.transform.position);
            }
            assert(!candidate.drop(42,3,neutral_definition,neutral_source,7,table_id,-1,false,
                services,neutral_receipt,error));
            assert(error=="Container loot already attempted for same ActorId lifecycle");
        }
    }
    assert(neutral_selected);

    // The component's wire payload matches the seven bytes emitted by the
    // original container Save methods and preserves source fields on state writes.
    using namespace dh::foundation::interactions;
    SourceContainerObjsFieldsV1 source_fields{1,0,0x12345678,4};
    std::vector<std::uint8_t> encoded;
    assert(encode_source_container_objs_v1(source_fields,encoded,error));
    assert((encoded==std::vector<std::uint8_t>{1,0,0x78,0x56,0x34,0x12,4}));
    SourceContainerObjsFieldsV1 decoded;
    assert(decode_source_container_objs_v1(encoded,decoded,error));
    assert(decoded.visible80==1&&decoded.enabled8a==0&&decoded.archetype270==0x12345678&&
           decoded.state394==4);
    dh::foundation::WorldObject saved_object;
    saved_object.id=42;saved_object.name=neutral_source.name;
    assert(bind_source_container_objs_v1(saved_object,source_fields,error));
    assert(write_source_container_state394_v1(saved_object,2,error));
    assert(read_source_container_objs_v1(saved_object,decoded,error));
    assert(decoded.visible80==1&&decoded.enabled8a==0&&decoded.archetype270==0x12345678&&
           decoded.state394==2);
    assert(source_container_restore_visual_v1(2)==SourceContainerRestoreVisualV1::idle);
    assert(source_container_restore_visual_v1(4)==SourceContainerRestoreVisualV1::idleactive);
    assert(source_container_restore_visual_v1(3)==SourceContainerRestoreVisualV1::none);
    auto malformed=encoded;malformed.pop_back();
    assert(!decode_source_container_objs_v1(malformed,decoded,error));
    dh::foundation::ActorDefinition saved_definition;
    saved_definition.stableId=42;saved_definition.name="Swamp_Normal_Chest";
    saved_definition.gametype="OpenableContainer";
    RoomMapping room_mapping;room_mapping.rooms[42]=11;
    SourceObjectSaveKeyV1 save_key;
    assert(source_object_save_key_v1(saved_definition,11,save_key,error));
    dh::foundation::ActorId rebound{};
    assert(source_object_id_for_save_key_v1({saved_definition},save_key,&room_mapping,
        RoomMapping::resolve,rebound,error)&&rebound==42);
    auto ambiguous=saved_definition;ambiguous.stableId=43;
    room_mapping.rooms[43]=11;
    assert(!source_object_id_for_save_key_v1({saved_definition,ambiguous},save_key,
        &room_mapping,RoomMapping::resolve,rebound,error));

    // Run the source OpenableContainerOwner inside a same-session ActorId
    // resolver, then feed it the marker parsed from the real chest BRES.
    dh2::world::OpenableContainerTableV1 openable_table;
    assert(openable_table.load(source_openable_records, sizeof source_openable_records,
        source_openable_names, sizeof source_openable_names, error));
    std::int32_t openable_row_id{};
    dh2::world::OpenableContainerRowV1 openable_row;
    assert(openable_table.resolve("Swamp_Normal_Chest", openable_row_id, openable_row, error));
    assert(openable_row.loot >= 0);
    SessionActors actors;
    actors.session = &actors;
    actors.lease = std::make_shared<int>(1);
    auto& container_definition = actors.definitions[41];
    container_definition.stableId = 41;
    container_definition.name = "Swamp_Normal_Chest";
    container_definition.properties["data_desc"] = "Swamp_Normal_Chest";
    auto& container_state = actors.states[41];
    container_state.id = 41;
    container_state.definition_id = container_definition.name;
    actors.openable.data_desc = container_definition.properties["data_desc"];
    auto& neutral_chest_definition=actors.definitions[42];
    neutral_chest_definition.stableId=42;
    neutral_chest_definition.name="Swamp_Normal_Chest";
    neutral_chest_definition.properties["data_desc"]="Swamp_Normal_Chest";
    auto& neutral_object=actors.objects[42];
    neutral_object.id=42;
    neutral_object.name=neutral_chest_definition.name;
    neutral_object.transform.position={-7013.42f,12982.703f,250.0f};
    dh::foundation::interactions::SourceContainerObjsFieldsV1 neutral_fields{1,1,0,2};
    assert(dh::foundation::interactions::bind_source_container_objs_v1(
        neutral_object,neutral_fields,error));
    actors.object_openable.data_desc=neutral_chest_definition.properties["data_desc"];
    auto& opener_definition = actors.definitions[7];
    opener_definition.stableId = 7;
    opener_definition.name = "Player";
    auto& opener_state = actors.states[7];
    opener_state.id = 7;
    opener_state.definition_id = opener_definition.name;

    dh::foundation::ActorDefinition probe_chest_definition;
    probe_chest_definition.stableId=410; probe_chest_definition.name="Swamp_Normal_Chest";
    dh::foundation::ActorState probe_chest_state;
    probe_chest_state.id=410; probe_chest_state.definition_id=probe_chest_definition.name;
    dh::foundation::ActorDefinition probe_breakable_definition;
    probe_breakable_definition.stableId=510; probe_breakable_definition.name="Swamp_Normal_DestructibleBarrel";
    dh::foundation::ActorState probe_breakable_state;
    probe_breakable_state.id=510; probe_breakable_state.definition_id=probe_breakable_definition.name;
    const auto source_pickup_supported=[](const dh2::data::Item* item){
        return item&&item->record.words[3]!=13&&item->record.words[22]!=13&&
            item->record.words[3]!=14&&item->record.words[26]==-1;
    };
    std::map<std::int32_t,std::size_t> chest_item_counts;
    bool chest_generic_pickup_seen=false;
    for(std::uint32_t seed=1;seed<=4096;++seed){
        dh2::data::LootRandom8V2 chest_rng{seed,0};
        EntryQueryFixture chest_entry;
        auto chest_services=services;
        chest_services.entry={&chest_entry,entry_service};
        chest_services.gameplay_rng=&chest_rng;
        Sink chest_sink;chest_services.context=&chest_sink;chest_services.drop_item=Sink::drop;
        dh::foundation::interactions::SourceContainerLootV1 chest_owner;
        dh::foundation::interactions::SourceContainerLootReceiptV1 chest_receipt;
        if(!chest_owner.drop(410,1,probe_chest_definition,probe_chest_state,7,
                openable_row.loot,-1,false,chest_services,chest_receipt,error))continue;
        for(const auto& outcome:chest_sink.drops){
            ++chest_item_counts[outcome.selected.id];
            chest_generic_pickup_seen|=source_pickup_supported(outcome.selected.item);
        }
    }
    assert(!chest_item_counts.empty() && !chest_generic_pickup_seen);
    std::uint32_t integrated_seed{};
    for(std::uint32_t seed=1;seed<100000&&!integrated_seed;++seed){
        dh2::data::LootRandom8V2 probe_rng{seed,0};
        EntryQueryFixture probe_entry;
        auto probe_services=services;
        probe_services.entry={&probe_entry,entry_service};
        probe_services.gameplay_rng=&probe_rng;
        Sink probe_sink; probe_services.context=&probe_sink; probe_services.drop_item=Sink::drop;
        dh::foundation::interactions::SourceContainerLootV1 probe_owner;
        dh::foundation::interactions::SourceContainerLootReceiptV1 probe_receipt;
        if(!probe_owner.drop(410,1,probe_chest_definition,probe_chest_state,7,openable_row.loot,-1,false,
                probe_services,probe_receipt,error))continue;
        for(const auto& x:probe_sink.drops)++chest_item_counts[x.selected.id];
        probe_sink.drops.clear();
        if(!probe_owner.drop(510,1,probe_breakable_definition,probe_breakable_state,7,table_id,-1,false,
                probe_services,probe_receipt,error))continue;
        const bool breakable_has_generic_pickup=std::any_of(probe_sink.drops.begin(),probe_sink.drops.end(),
            [&](const auto& drop){return source_pickup_supported(drop.selected.item);});
        if(breakable_has_generic_pickup)integrated_seed=seed;
    }
    assert(integrated_seed);
    auto item_store=std::make_shared<dh::foundation::loot::RuntimeWorldItemAdapterV1>(tables);
    auto session_sink=std::make_shared<WorldDropSink>();
    session_sink->store=item_store.get();
    auto session_rng=std::make_shared<dh2::data::LootRandom8V2>(dh2::data::LootRandom8V2{integrated_seed,0});
    session_sink->rng=session_rng.get();
    auto session_loot = services;
    session_loot.context = session_sink.get();
    session_loot.gameplay_rng = session_rng.get();
    session_loot.drop_item = WorldDropSink::drop;
    VisualCallbacks visual;
    dh::foundation::interactions::SessionOpenableInteractionServicesV1 runtime_services;
    runtime_services.session_identity = actors.session;
    runtime_services.session_lease = actors.lease;
    runtime_services.actor_context = &actors;
    runtime_services.resolve_actor = SessionActors::resolve;
    runtime_services.visual_context = &visual;
    runtime_services.bind_retained_visual = VisualCallbacks::bind;
    runtime_services.play_retained_clip = VisualCallbacks::play;
    runtime_services.retained_scene_flags = VisualCallbacks::scene;
    runtime_services.loot = session_loot;
    runtime_services.source.spawn_roll_and_probability = [](std::int32_t& roll,
        std::int32_t& probability, std::string&) { roll = 0; probability = 100; return true; };
    runtime_services.source.resolve_row = [&](const std::string& name, std::int32_t& id,
        dh2::world::OpenableContainerRowV1& row, std::string& e) {
        return openable_table.resolve(name, id, row, e);
    };
    runtime_services.source.visual_asset = [&](std::int32_t id, std::string&) {
        return id == openable_row.visual;
    };
    runtime_services.source.game_object_init_post = [](std::string&) { return true; };
    runtime_services.source.meet_condition = [](bool& meets, std::string&) { meets = true; return true; };
    runtime_services.source.has_visual = [](bool& has, std::string&) { has = true; return true; };
    runtime_services.source.apply_mesh_box = [](std::string&) { return true; };
    runtime_services.source.precache_complete_source_v42 = [](std::string&) { return true; };
    runtime_services.source.load_object_script = [](const char*, const char* directory,
        std::string& e) { if (!directory || std::string(directory) != "data/scripts/objects/") {
            e = "wrong source object script directory"; return false; } return true; };
    runtime_services.source.detach_physical = [](std::string&) { return true; };
    runtime_services.source.play_sound_3d = [](std::int32_t, std::string&) { return true; };
    runtime_services.source.source_on_interact = [](std::string&) { return true; };
    runtime_services.source.has_script = [](bool& has, std::string&) { has = false; return true; };
    runtime_services.interaction.current_level = [&](std::uintptr_t& level, std::string&) {
        level = reinterpret_cast<std::uintptr_t>(actors.session); return true;
    };
    runtime_services.interaction.local_player_hosting = [](bool& hosting, std::string&) {
        hosting = false; return true;
    };
    runtime_services.interaction.handle_as_character = [](std::uintptr_t, std::uintptr_t& character,
        std::string&) { character = 0; return true; };
    std::string session_error;
    auto neutral_runtime_services=runtime_services;
    auto session_owner = dh::foundation::interactions::SessionOpenableInteractionV1::create(
        std::move(runtime_services), session_error);
    assert(session_owner && session_owner->init_post(41, session_error));
    assert(visual.binds == 1 && actors.openable.data374 == openable_row_id);
    assert(session_owner->interact(41, 7, session_error));
    assert(actors.openable.state394 == 3 && session_sink->drops.empty());

    const auto chest_path = ".local-inputs/publication/checkpoint/reference/openable-container-v1/cache/go_chest_swamp.bdae";
    auto chest_bytes = read(".", chest_path);
    dh2::resources::BresView chest_bres{};
    assert(dh2_bres_open(&chest_bres, chest_bytes.data(), chest_bytes.size()) ==
           dh2::resources::BresError::ok);
    dh2::animation::EventTrack chest_events;
    assert(chest_events.load(chest_bres, error));
    const auto opened_ms = dh2_events_time(&chest_events.view(), "opened");
    assert(opened_ms == 233);
    dh2::animation::EventCursor event_cursor;
    EventObservation observed;
    assert(dh2_events_update(&chest_events.view(), &event_cursor, opened_ms - 1,
        opened_ms + 1, 166, 1266, capture_event, &observed));
    assert(observed.name == "opened");
    CharacterOwner current_character;
    current_character.id=7;
    current_character.state=std::make_shared<dh::foundation::CharacterState>(
        dh::foundation::make_default_character("interaction-player","Interaction Player","warrior"));
    dh::foundation::loot::RuntimeWorldItemInteractionServicesV1 pickup_services{
        &current_character,CharacterOwner::resolve};
    dh::foundation::loot::RuntimeWorldItemInteractionV1 pickup_interaction;
    auto pickup_source_range=[&](std::size_t begin,std::size_t end,dh::foundation::ActorId source){
        std::size_t picked_count=0;
        for(std::size_t i=begin;i<end;++i){
            const auto world_id=session_sink->world_ids.at(i);
            dh::foundation::loot::RuntimeWorldItemEntryV1 entry;
            assert(item_store->inspect(world_id,entry,session_error));
            const auto expected_table=source==41?openable_row.loot:table_id;
            if(entry.source_outcome.source_actor!=source||entry.source_outcome.loot_table!=expected_table)
                throw std::runtime_error("world item source/table mismatch: actor="+
                    std::to_string(entry.source_outcome.source_actor)+" table="+
                    std::to_string(entry.source_outcome.loot_table)+" expected actor="+
                    std::to_string(source)+" table="+std::to_string(expected_table));
            const auto pickup_type=entry.authored_item->record.words[3];
            dh::foundation::loot::RuntimeWorldItemSourceInteractV1 request{7,world_id};
            dh::foundation::loot::RuntimeWorldItemInteractionReceiptV1 pickup_receipt;
            const auto before=item_store->size();
            if(!source_pickup_supported(entry.authored_item)){
                assert(!pickup_interaction.dispatch_live_player(7,true,&opener_state,request,
                    pickup_services,*item_store,pickup_receipt,session_error));
                if(pickup_type==13||entry.authored_item->record.words[22]==13)
                    assert(session_error=="Source gold-token valuation/AddGold owner is unavailable");
                else if(pickup_type==14)
                    assert(session_error=="Source potion-capacity/consumption pickup owner is unavailable");
                else if(entry.authored_item->record.words[26]!=-1)
                    assert(session_error=="Source equippable pickup requires native power/Gear/transmute owners");
                else assert(false);
                assert(item_store->size()==before);
                continue;
            }
            assert(pickup_type!=13&&pickup_type!=14&&entry.authored_item->record.words[26]==-1);
            const auto items_before_pickup=item_store->size();
            const auto inventory_before=current_character.state->inventory;
            assert(pickup_interaction.dispatch_live_player(7,true,&opener_state,request,
                pickup_services,*item_store,pickup_receipt,session_error));
            assert(pickup_receipt.pickup.completed&&pickup_receipt.player==7&&
                   pickup_receipt.item==world_id&&item_store->size()+1==items_before_pickup);
            const auto committed=current_character.state->inventory;
            assert(!pickup_interaction.dispatch_live_player(7,true,&opener_state,request,
                pickup_services,*item_store,pickup_receipt,session_error));
            assert(item_store->size()+1==items_before_pickup&&
                   current_character.state->inventory.size()==committed.size());
            for(std::size_t n=0;n<committed.size();++n)
                assert(current_character.state->inventory[n].instance_id==committed[n].instance_id&&
                       current_character.state->inventory[n].definition_id==committed[n].definition_id&&
                       current_character.state->inventory[n].quantity==committed[n].quantity);
            ++picked_count;
        }
        return picked_count;
    };
    const auto chest_store_begin=session_sink->world_ids.size();
    dh::foundation::RetainedAnimationEvent retained_event;
    retained_event.name = observed.name;
    retained_event.clip_id = "activate";
    retained_event.slot = 0;
    retained_event.wall_timestamp_ms = static_cast<std::uint32_t>(opened_ms + 1);
    retained_event.generation = 1;
    retained_event.lag_ms = observed.lag;
    // Exercise the same source chest flow using only the neutral WorldObject
    // plus its seven-byte persistent component (no ActorState or HP sheet).
    VisualCallbacks neutral_visual;
    neutral_visual.expected_actor=42;
    auto neutral_sink=std::make_shared<WorldDropSink>();
    neutral_sink->store=item_store.get();
    auto neutral_rng=std::make_shared<dh2::data::LootRandom8V2>(
        dh2::data::LootRandom8V2{integrated_seed,0});
    neutral_sink->rng=neutral_rng.get();
    neutral_runtime_services.visual_context=&neutral_visual;
    neutral_runtime_services.loot.context=neutral_sink.get();
    neutral_runtime_services.loot.gameplay_rng=neutral_rng.get();
    neutral_runtime_services.loot.drop_item=WorldDropSink::drop;
    auto neutral_owner=dh::foundation::interactions::SessionOpenableInteractionV1::create(
        std::move(neutral_runtime_services),session_error);
    assert(neutral_owner&&neutral_owner->init_post(42,session_error));
    assert(neutral_owner->interact(42,7,session_error));
    assert(actors.object_openable.state394==3);
    assert(dh::foundation::interactions::read_source_container_objs_v1(
        neutral_object,neutral_fields,session_error)&&neutral_fields.state394==3);
    const auto neutral_store_begin=neutral_sink->world_ids.size();
    assert(neutral_visual.event&&neutral_visual.event(42,retained_event,session_error));
    const auto neutral_drops=neutral_sink->drops.size();
    assert(neutral_drops>0&&neutral_sink->world_ids.size()==neutral_store_begin+neutral_drops);
    for(std::size_t i=neutral_store_begin;i<neutral_sink->world_ids.size();++i){
        dh::foundation::loot::RuntimeWorldItemEntryV1 stored;
        assert(item_store->inspect(neutral_sink->world_ids[i],stored,session_error));
        assert(stored.source_outcome.source_actor==42&&stored.source_outcome.killer_actor==7);
        assert(stored.source_position==neutral_object.transform.position);
    }
    std::vector<dh::foundation::loot::RuntimeWorldItemRenderV1> neutral_packets;
    assert(item_store->render_items(neutral_packets,session_error));
    for(std::size_t i=neutral_store_begin;i<neutral_sink->world_ids.size();++i){
        const auto packet=std::find_if(neutral_packets.begin(),neutral_packets.end(),
            [&](const auto& row){return row.identity==neutral_sink->world_ids[i];});
        assert(packet!=neutral_packets.end()&&packet->position==neutral_object.transform.position);
    }
    dh::foundation::loot::RuntimeWorldItemEntryV1 neutral_first_entry;
    assert(item_store->inspect(neutral_sink->world_ids[neutral_store_begin],
        neutral_first_entry,session_error));
    auto mismatched_definition=neutral_chest_definition;
    mismatched_definition.stableId=99;
    dh::foundation::loot::RuntimeWorldItemIdV1 rejected_identity{};
    const auto neutral_store_size=item_store->size();
    assert(!item_store->publish_source_object_drop(neutral_first_entry.source_outcome,
        mismatched_definition,neutral_object,rejected_identity,session_error));
    assert(item_store->size()==neutral_store_size&&rejected_identity==
        dh::foundation::loot::invalid_runtime_world_item_v1);
    assert(neutral_visual.event(42,retained_event,session_error));
    assert(neutral_sink->drops.size()==neutral_drops&&
           neutral_sink->world_ids.size()==neutral_store_begin+neutral_drops);
    assert(neutral_visual.finished&&neutral_visual.finished(42,1,false,session_error));
    assert(actors.object_openable.state394==4);
    assert(dh::foundation::interactions::read_source_container_objs_v1(
        neutral_object,neutral_fields,session_error)&&neutral_fields.state394==4);
    const auto object_store_after_completion=item_store->size();
    assert(neutral_visual.event(42,retained_event,session_error));
    assert(item_store->size()==object_store_after_completion);
    auto restored_neutral_object=neutral_object;
    assert(dh::foundation::interactions::read_source_container_objs_v1(
        restored_neutral_object,neutral_fields,session_error)&&neutral_fields.state394==4);
    assert(dh::foundation::interactions::source_container_restore_visual_v1(
        neutral_fields.state394)==
        dh::foundation::interactions::SourceContainerRestoreVisualV1::idleactive);
    assert(item_store->size()==object_store_after_completion);
    // Model a fresh retained visual generation after loading state4. The
    // source IsInteractive gate and restored idleactive state suppress both
    // renewed input (including its quest prefix) and stale `opened` markers.
    const auto neutral_rng_calls_after_restore=neutral_rng->calls;
    assert(neutral_owner->interact(42,7,session_error));
    auto restored_generation_event=retained_event;
    restored_generation_event.generation=2;
    assert(neutral_visual.event(42,restored_generation_event,session_error));
    assert(item_store->size()==object_store_after_completion&&
        neutral_sink->drops.size()==neutral_drops&&
        neutral_rng->calls==neutral_rng_calls_after_restore&&actors.object_openable.state394==4);
    const auto item_count_after_neutral=item_store->size();
    assert(visual.event && visual.event(41, retained_event, session_error));
    assert(!session_sink->drops.empty());
    const auto drops_after_opened = session_sink->drops.size();
    const auto calls_after_opened = session_rng->calls;
    assert(visual.event(41, retained_event, session_error));
    assert(session_sink->drops.size() == drops_after_opened &&
           session_rng->calls == calls_after_opened);
    assert(visual.finished && visual.finished(41, 1, false, session_error));
    assert(actors.openable.state394 == 4);
    const auto chest_store_end=session_sink->world_ids.size();
    assert(chest_store_end>chest_store_begin);
    const auto chest_picked=pickup_source_range(chest_store_begin,chest_store_end,41);
    assert(chest_picked==0&&item_store->size()==item_count_after_neutral+
        (chest_store_end-chest_store_begin));

    // The actual source destructible rows resolve the authored normal barrel,
    // its table9 loot, visual70, and moth script. The exact recovered BDAE is
    // consumed from the isolated source fixture below; its real markers and
    // clips drive the same-session path without guessed timing.
    auto destructible_table = std::make_shared<dh2::world::DestructibleContainerTableV16>();
    assert(destructible_table->load(destructible_group0_records_bin, sizeof destructible_group0_records_bin,
        destructible_group0_names_bin, sizeof destructible_group0_names_bin, error));
    const auto barrel_id = destructible_table->data_id("Swamp_Normal_DestructibleBarrel");
    assert(barrel_id == 30);
    const auto* barrel = destructible_table->row(barrel_id);
    assert(barrel && barrel->visual() == 70 && barrel->loot() == table_id &&
           barrel->script30 == "moth_spawn_container");
    SessionActors breakable_actors;
    breakable_actors.session = &breakable_actors;
    breakable_actors.lease = std::make_shared<int>(2);
    auto& breakable_definition = breakable_actors.definitions[51];
    breakable_definition.stableId = 51;
    breakable_definition.name = "Swamp_Normal_DestructibleBarrel";
    breakable_definition.properties["data_desc"] = breakable_definition.name;
    auto& breakable_state = breakable_actors.states[51];
    breakable_state.id = 51;
    breakable_state.definition_id = breakable_definition.name;
    breakable_actors.destructible.data_desc = breakable_definition.name;
    dh::foundation::interactions::SessionDestructibleInteractionServicesV1 breakable_services;
    breakable_services.session_identity = breakable_actors.session;
    breakable_services.session_lease = breakable_actors.lease;
    breakable_services.actor_context = &breakable_actors;
    breakable_services.resolve_actor = SessionActors::resolve;
    breakable_services.table = destructible_table;
    auto breakable_bres_bytes = read(".",
        "port/windows-foundation/features/interactions/test-assets/go_swamp_urn_breakable.bdae");
    dh2::resources::BresView breakable_bres{};
    assert(dh2_bres_open(&breakable_bres, breakable_bres_bytes.data(),
        breakable_bres_bytes.size()) == dh2::resources::BresError::ok);
    dh2::scene::Scene breakable_scene;
    assert(dh2::scene::load(breakable_bres, breakable_scene, error));
    dh2::animation::Player breakable_source_player;
    assert(breakable_source_player.load(breakable_bres_bytes.data(),
        breakable_bres_bytes.size(), breakable_scene, error,
        dh2::animation::MissingTargets::ignore));
    assert(breakable_source_player.track_count() > 0);
    dh2::animation::EventTrack breakable_events;
    assert(breakable_events.load(breakable_bres, error));
    BreakableCallbacks breakable_callbacks;
    breakable_callbacks.bytes=&breakable_bres_bytes;
    breakable_callbacks.bres=&breakable_bres;
    breakable_services.visual_context=&breakable_callbacks;
    breakable_services.visual_asset = BreakableCallbacks::visual_asset;
    breakable_services.load_object_script = BreakableCallbacks::load_script;
    breakable_services.has_visual = BreakableCallbacks::has_visual;
    breakable_services.bind_retained_visual = BreakableCallbacks::bind;
    breakable_services.animation_count = BreakableCallbacks::animation_count;
    breakable_services.raise_destroy_quest = BreakableCallbacks::quest;
    breakable_services.loot = session_loot;
    breakable_services.script_call = BreakableCallbacks::script_call;
    breakable_services.has_script = BreakableCallbacks::has_script;
    auto breakable = dh::foundation::interactions::SessionDestructibleInteractionV1::create(
        std::move(breakable_services), error);
    assert(breakable);
    assert(breakable->initialize(51, error));
    assert(breakable_callbacks.script_loaded && breakable_callbacks.event &&
           breakable_actors.destructible.initialized);
    // Model the opener retained by source interact before its animation marker.
    breakable_actors.destructible.opener = 7;
    const auto opened_breakable_ms = dh2_events_time(&breakable_events.view(), "opened");
    assert(opened_breakable_ms >= 0);
    std::string breakable_activate_name;
    std::int32_t breakable_activate_start=-1,breakable_activate_end=-1;
    std::vector<std::string> breakable_clip_names;
    for(std::uint32_t i=0;i<dh2_bres_library_count(&breakable_bres,
            dh2::resources::Library::animation_clip);++i){
        const auto* record=dh2_bres_library_item(&breakable_bres,
            dh2::resources::Library::animation_clip,static_cast<std::int32_t>(i));
        assert(record);
        std::uint32_t name_offset{};
        std::memcpy(&name_offset,record,sizeof(name_offset));
        assert(name_offset<breakable_bres.size);
        const auto* name=reinterpret_cast<const char*>(breakable_bres.bytes+name_offset);
        const auto* end=static_cast<const char*>(std::memchr(name,0,breakable_bres.size-name_offset));
        assert(end);
        std::string clip(name,end);
        breakable_clip_names.push_back(clip);
        if(clip=="activate"){
            std::memcpy(&breakable_activate_start,record+4,sizeof(breakable_activate_start));
            std::memcpy(&breakable_activate_end,record+8,sizeof(breakable_activate_end));
        }
    }
    assert(breakable_activate_start>=0&&breakable_activate_end>=breakable_activate_start&&
           opened_breakable_ms>=breakable_activate_start&&opened_breakable_ms<=breakable_activate_end);
    assert(std::find(breakable_clip_names.begin(),breakable_clip_names.end(),"idle")!=breakable_clip_names.end());
    dh::foundation::RetainedAnimationEvent breakable_marker;
    breakable_marker.name = "opened";
    breakable_marker.clip_id = "activate";
    breakable_marker.wall_timestamp_ms = static_cast<std::uint32_t>(opened_breakable_ms);
    breakable_marker.generation = 1;
    const auto breakable_store_begin=session_sink->world_ids.size();
    if (!breakable_callbacks.event(51, breakable_marker, error)) throw std::runtime_error(error);
    assert(breakable_callbacks.quest_calls == 1 && breakable_callbacks.open_calls == 1);
    assert(breakable_callbacks.event(51, breakable_marker, error));
    assert(breakable_callbacks.quest_calls == 1 && breakable_callbacks.open_calls == 1);
    const auto breakable_store_end=session_sink->world_ids.size();
    assert(breakable_store_end>breakable_store_begin);
    const auto breakable_picked=pickup_source_range(breakable_store_begin,breakable_store_end,51);
    assert(breakable_picked>0);

    // Exercise the authored type13 gold path using the source value kernel and
    // same Loot RNG before transferring through the same world-item store.
    dh::foundation::ActorDefinition gold_source_definition;
    gold_source_definition.stableId=61;
    gold_source_definition.name="Swamp_Normal_DestructibleBarrel";
    dh::foundation::ActorState gold_source_state;
    gold_source_state.id=61;
    gold_source_state.definition_id=gold_source_definition.name;
    Sink gold_selection;
    dh::foundation::interactions::SourceContainerDropItemV1 selected_gold;
    dh2::data::LootRandom8V2 selected_gold_rng{};
    bool found_source_gold=false;
    for(std::uint32_t seed=1;seed<10000&&!found_source_gold;++seed){
        dh2::data::LootRandom8V2 gold_rng{seed,0};
        EntryQueryFixture gold_entry;
        auto gold_services=session_loot;
        gold_services.context=&gold_selection;
        gold_services.gameplay_rng=&gold_rng;
        gold_services.drop_item=Sink::drop;
        gold_selection.drops.clear();
        dh::foundation::interactions::SourceContainerLootV1 gold_owner;
        dh::foundation::interactions::SourceContainerLootReceiptV1 gold_receipt;
        assert(gold_owner.drop(61,1,gold_source_definition,gold_source_state,7,table_id,-1,false,
            gold_services,gold_receipt,error));
        const auto outcome=std::find_if(gold_selection.drops.begin(),gold_selection.drops.end(),
            [](const auto& item){return item.selected.item->record.words[3]==13||
                                       item.selected.item->record.words[22]==13;});
        if(outcome!=gold_selection.drops.end()){
            selected_gold=*outcome;
            selected_gold_rng=gold_rng;
            std::int32_t resolved{};
            assert(dh2_loot_item_value_v7(&resolved,&gold_rng,
                &outcome->selected.item->record,nullptr,0,0)==0);
            found_source_gold=true;
        }
    }
    assert(found_source_gold&&selected_gold.selected.id==418);
    const auto gold_store_begin=session_sink->world_ids.size();
    session_sink->rng=&selected_gold_rng;
    assert(WorldDropSink::drop(session_sink.get(),selected_gold,error));
    assert(session_sink->world_ids.size()==gold_store_begin+1);
    const auto gold_world_id=session_sink->world_ids.back();
    const auto store_before_gold_pickup=item_store->size();
    dh::foundation::loot::RuntimeWorldItemInteractionReceiptV1 gold_receipt;
    assert(pickup_interaction.dispatch_live_player(7,true,&opener_state,
        {7,gold_world_id},pickup_services,*item_store,gold_receipt,session_error));
    assert(gold_receipt.pickup.quantity>0&&item_store->size()+1==store_before_gold_pickup);

    std::cout << "PASS same-session ActorId/definition/state source loot; actual table "
              << table_id << " forwards " << sink.drops.size()
              << " source-selected rows with shared RNG and blocks duplicate lifecycle delivery; "
              << "actual chest marker drove ActorState and object-only Openable routes exactly once; "
              << "neutral OBJS component persisted state2->3->4 and exact WorldObject position; "
              << "table " << openable_row.loot << " sampled 4096 seeds across "
              << chest_item_counts.size() << " original ItemTable rows with no generic pickup branch; "
              << "its exact source drops were published and rejected by existing Gear gates; "
              << "exact extracted breakable BDAE supplied its authored opened event to the "
              << "same-session Destructible owner (" << chest_picked << "/" << breakable_picked
              << " ordinary supported drops picked once; source gold item418 value/pickup owner verified; source bank tracks " << breakable_source_player.track_count()
              << "; opened " << opened_breakable_ms << " ms; activate "
              << breakable_activate_start << "–" << breakable_activate_end << " ms; clips ";
    for(std::size_t i=0;i<breakable_clip_names.size();++i)
        std::cout<<(i?",":"")<<breakable_clip_names[i];
    std::cout<<")\n";
}
