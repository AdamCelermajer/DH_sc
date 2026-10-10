#include "game_save.hpp"
#include "character_quest_blob.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <limits>
#include <set>
#include <stdexcept>
#include <tuple>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

namespace dh::foundation {
namespace {
constexpr std::size_t file_limit=64u*1024u*1024u;
constexpr unsigned char magic[]={'D','H','G','A','M','E',0,1};
struct Archive {
    std::vector<unsigned char> bytes;
    bool reading=false;std::size_t at=0;
    void word(std::uint64_t& value,unsigned width) {
        if(reading){
            if(at>bytes.size()||width>bytes.size()-at)throw std::runtime_error("Truncated game save");
            value=0;for(unsigned i=0;i<width;++i)value|=std::uint64_t(bytes[at++])<<(i*8);
        }else{
            if(bytes.size()>file_limit-width)throw std::runtime_error("Game save exceeds64MiB");
            for(unsigned i=0;i<width;++i)bytes.push_back(static_cast<unsigned char>(value>>(i*8)));
        }
    }
    void u32(std::uint32_t& value){std::uint64_t bits=value;word(bits,4);value=std::uint32_t(bits);}
    void u64(std::uint64_t& value){word(value,8);}
    void i32(std::int32_t& value){std::uint32_t bits;std::memcpy(&bits,&value,4);u32(bits);std::memcpy(&value,&bits,4);}
    void real(float& value){static_assert(sizeof(float)==4&&std::numeric_limits<float>::is_iec559);
        std::uint32_t bits;std::memcpy(&bits,&value,4);u32(bits);std::memcpy(&value,&bits,4);}
    void flag(bool& value){std::uint64_t bits=value?1:0;word(bits,1);if(bits>1)throw std::runtime_error("Invalid game save flag");value=bits!=0;}
    void optional_flag(std::optional<bool>& value){
        bool known=value.has_value();flag(known);
        if(!known){if(reading)value.reset();return;}
        bool bit=reading?false:*value;flag(bit);if(reading)value=bit;
    }
    void text(std::string& value){std::uint32_t size=std::uint32_t(value.size());
        if(!reading&&value.size()>character_text_limit)throw std::runtime_error("Game save string exceeds limit");
        u32(size);if(size>character_text_limit)throw std::runtime_error("Game save string exceeds limit");
        if(reading){if(at>bytes.size()||size>bytes.size()-at)throw std::runtime_error("Truncated game save text");
            value.assign(reinterpret_cast<const char*>(bytes.data()+at),size);at+=size;
        }else{if(bytes.size()>file_limit-size)throw std::runtime_error("Game save exceeds64MiB");bytes.insert(bytes.end(),value.begin(),value.end());}}
    void optional_text(std::optional<std::string>& value){bool present=value.has_value();flag(present);
        if(reading){if(present)value=std::string{};else value.reset();}if(present)text(*value);}
    template<class T,class Fn>void list(std::vector<T>& values,Fn fn){
        if(!reading&&values.size()>character_collection_limit)throw std::runtime_error("Game save collection exceeds limit");
        std::uint32_t count=std::uint32_t(values.size());u32(count);
        if(count>character_collection_limit)throw std::runtime_error("Game save collection exceeds limit");
        if(reading)values.resize(count);for(auto& item:values)fn(item);
    }
};
void character(Archive& a,CharacterState& c){
    auto schema=c.schema_version;
    a.u32(schema);
    if(a.reading){
        if(schema<1||schema>character_schema_version)throw std::runtime_error("Unsupported character schema version in game save");
        c.schema_version=character_schema_version;
    }else if(schema!=character_schema_version)throw std::runtime_error("Unsupported character schema version in game save");
    a.text(c.id);a.text(c.name);a.text(c.class_id);a.u32(c.stats.level);
    for(float* v:{&c.stats.health,&c.stats.max_health,&c.stats.resource,&c.stats.max_resource,
                  &c.stats.strength,&c.stats.dexterity,&c.stats.intelligence})a.real(*v);
    if(schema>=2){
        a.real(c.stats.endurance);a.real(c.stats.energy);a.flag(c.source_endurance_energy_known);
        a.u32(c.source_stat_points);a.u32(c.source_skill_points);a.flag(c.source_points_known);
    }
    a.u64(c.experience);a.u64(c.gold);
    a.list(c.inventory,[&](auto& i){a.text(i.instance_id);a.text(i.definition_id);a.u32(i.quantity);});
    a.list(c.equipment,[&](auto& i){
        a.text(i.slot);a.text(i.item_instance_id);
        if(schema>=2){a.i32(i.equipment_set);a.i32(i.source_slot);}
    });
    a.list(c.skills,[&](auto& s){a.text(s.id);a.u32(s.rank);});
    a.list(c.unlocks,[&](auto& u){a.text(u);});
    if(schema>=2){
        a.flag(c.source_skill_slots_known);
        a.list(c.skill_slots,[&](auto& s){a.u32(s.equipment_set);a.u32(s.slot);a.u32(s.saved_skill_row);});
        a.i32(c.source_faery_list_id);a.flag(c.source_faery_state_known);
        for(auto& difficulty:c.faery_by_difficulty){
            a.i32(difficulty.current_faery);
            for(auto& faery:difficulty.faeries){
                std::uint64_t state=faery.state,level=faery.level;
                a.word(state,1);a.word(level,2);
                if(a.reading){
                    faery.state=static_cast<std::uint8_t>(state);
                    faery.level=static_cast<std::uint16_t>(level);
                }
            }
        }
    }
    if(schema>=3){
        auto size=static_cast<std::uint32_t>(c.source_quest_progress_cqpg.size());a.u32(size);
        if(size>character_quest_blob_limit)throw std::runtime_error("Game save quest progress exceeds limit");
        if(a.reading){
            if(a.at>a.bytes.size()||size>a.bytes.size()-a.at)throw std::runtime_error("Truncated game save quest progress");
            c.source_quest_progress_cqpg.resize(size);
        }
        for(auto& byte:c.source_quest_progress_cqpg){std::uint64_t value=byte;a.word(value,1);if(a.reading)byte=static_cast<std::uint8_t>(value);}
    }
    if(schema>=4){
        // Same append-only tail as save_store.cpp (character.save schema v4).
        a.i32(c.current_difficulty);a.i32(c.unlocked_difficulty);
        a.flag(c.menu_metadata.known);a.u32(c.menu_metadata.save_time);
        for(auto& row:c.menu_metadata.level_row)a.i32(row);
        for(auto& act:c.menu_metadata.current_act)a.i32(act);
        auto visited=static_cast<std::uint32_t>(c.visited_modules.size());a.u32(visited);
        if(visited>character_visited_limit)throw std::runtime_error("Game save visited-module section exceeds limit");
        if(a.reading)c.visited_modules.resize(visited);
        for(auto& entry:c.visited_modules){
            a.text(entry.level_uri);a.u32(entry.module_id);
            std::uint64_t byte=entry.visited;a.word(byte,1);if(a.reading)entry.visited=static_cast<std::uint8_t>(byte);
        }
    }
}
void actor_record(Archive& a,PersistedPlayableActor& p){
    auto& s=p.actor;a.u64(s.id);a.text(s.definition_id);a.text(s.class_id);a.i32(s.faction_id);
    for(auto* array:{&s.transform.position,&s.transform.rotation,&s.transform.scale})for(float& v:*array)a.real(v);
    for(float* v:{&s.health,&s.max_health,&s.resource,&s.max_resource})a.real(*v);
    // Transient control state is never serialized; restored actors idle/dead.
    if(a.reading){s.action=s.health>0?CharacterAction::idle:CharacterAction::dead;s.action_elapsed_seconds=0;s.target_id=0;}
    a.list(s.equipment,[&](auto& e){a.text(e.slot);a.text(e.definition_id);a.optional_text(e.item_instance_id);});
    a.list(s.attack_ids,[&](auto& id){a.text(id);});a.optional_text(s.persistent_character_id);
    for(auto* sheet:{&p.combat.sheets.base,&p.combat.sheets.saved,&p.combat.sheets.gear,&p.combat.sheets.resolved})
        for(auto& value:*sheet)a.i32(value);
    auto& f=p.combat.facts;a.i32(f.main_damage_class);a.i32(f.off_damage_class);
    a.flag(f.two_hander);a.flag(f.dual_wield);a.flag(f.shield);
    if(a.reading){f.original_state=-1;f.combo_hits=0;}
    a.flag(p.traits.is_player);a.flag(p.traits.targetable);
    bool main=p.traits.main_item.has_value();a.flag(main);
    if(a.reading){if(main)p.traits.main_item=dh2::data::ItemRecord164{};else p.traits.main_item.reset();}
    if(main)for(auto& word:p.traits.main_item->words)a.i32(word);
}
void object_record(Archive& a,WorldObject& object){
    a.u64(object.id);a.text(object.name);
    for(auto* array:{&object.transform.position,&object.transform.rotation,&object.transform.scale})for(auto& value:*array)a.real(value);
    a.text(object.visual.model);a.text(object.visual.material);a.text(object.visual.animation);a.flag(object.visual.visible);
    std::uint32_t count=static_cast<std::uint32_t>(object.state_components.size());a.u32(count);
    if(count>world_object_component_limit)throw std::runtime_error("World object component count exceeds limit");
    if(a.reading)object.state_components.clear();
    auto iterator=object.state_components.begin();std::size_t total=0;
    for(std::uint32_t i=0;i<count;++i){
        std::string key=a.reading?std::string{}:iterator->first;a.text(key);
        std::vector<std::uint8_t> value=a.reading?std::vector<std::uint8_t>{}:iterator->second;
        auto size=static_cast<std::uint32_t>(value.size());a.u32(size);
        if(size>world_object_state_limit-total)throw std::runtime_error("World object persistent state exceeds limit");
        total+=size;
        if(a.reading)value.resize(size);
        for(auto& byte:value){std::uint64_t bits=byte;a.word(bits,1);if(a.reading)byte=static_cast<std::uint8_t>(bits);}
        if(a.reading){if(!object.state_components.emplace(std::move(key),std::move(value)).second)throw std::runtime_error("Duplicate world object component key");}
        else ++iterator;
    }
}
void snapshot(Archive& a,GameSave& g){a.u32(g.version);
    if(g.version<1||g.version>3)throw std::runtime_error("Unsupported game save version");
    character(a,g.character);a.text(g.level_uri);
    a.u64(g.controlled_actor_id);a.u32(g.random.seed);a.u32(g.random.calls);
    a.list(g.actors,[&](auto& actor){actor_record(a,actor);});
    if(g.version>=2)a.list(g.objects,[&](auto& object){object_record(a,object);});
    // Append in the already serialized actor order. Never change v1/v2 actor
    // bytes, infer presence from HP, or serialize physical receiver pointers.
    if(g.version>=3)for(auto& actor:g.actors)a.optional_flag(actor.actor.source_physical_present);
}
std::uint64_t checksum(const std::vector<unsigned char>& bytes){std::uint64_t hash=14695981039346656037ull;
    for(auto b:bytes){hash^=b;hash*=1099511628211ull;}return hash;}
bool text_valid(const std::string& text){if(text.empty()||text.size()>character_text_limit)return false;
    for(unsigned char c:text)if(c<32||c==127)return false;return true;}
std::int32_t raw_vital(float value){const double raw=std::round(double(value)*256.0);
    if(!std::isfinite(raw)||raw<INT32_MIN||raw>INT32_MAX)throw std::runtime_error("Live vital outside original signed256 range");
    return std::int32_t(raw);}
void normalize(PersistedPlayableActor& p){
    p.actor.action=p.actor.health>0?CharacterAction::idle:CharacterAction::dead;
    p.actor.action_elapsed_seconds=0;p.actor.target_id=0;p.combat.facts.original_state=-1;p.combat.facts.combo_hits=0;
    p.actor.source_flags520.reset();p.actor.source_movement_type.reset();p.actor.source_validate_boundary452.reset();
    p.actor.source_target_node180.reset();p.actor.source_target_position184.reset();
    for(const unsigned current:{36u,41u}){
        const auto desired=raw_vital(current==36?p.actor.health:p.actor.resource);
        // Visual-only actors have no combat actions or target admission. Their
        // source negative/unavailable vital cells project to zero for the live
        // registry; saving that projection must not overwrite source metadata.
        if(!p.traits.targetable&&p.actor.attack_ids.empty()&&desired==0&&p.combat.sheets.resolved[current]<0)continue;
        const auto bits=std::uint32_t(p.combat.sheets.saved[current])+std::uint32_t(desired)-
                        std::uint32_t(p.combat.sheets.resolved[current]);
        std::memcpy(&p.combat.sheets.saved[current],&bits,4);p.combat.sheets.resolved[current]=desired;
    }
}
bool checkpoint_binding_compatible(const PersistedPlayableActor& saved,const ActorState& current,
    const OriginalCombatProperties& combat,const PlayableActorTraits& traits,std::string& error){
    const auto fail=[&](const char* why){error="Saved actor "+std::to_string(current.id)+" binding incompatible: "+why;return false;};
    if(saved.actor.class_id!=current.class_id||saved.actor.faction_id!=current.faction_id||
       saved.actor.persistent_character_id!=current.persistent_character_id)
        return fail("class/faction/persistent identity changed");
    auto saved_attacks=saved.actor.attack_ids,current_attacks=current.attack_ids;
    std::sort(saved_attacks.begin(),saved_attacks.end());std::sort(current_attacks.begin(),current_attacks.end());
    if(saved_attacks!=current_attacks)return fail("authorized attack/action groups changed");
    using EquipmentKey=std::tuple<std::string,std::string,std::optional<std::string>>;
    const auto equipment_keys=[](const auto& items){std::vector<EquipmentKey> result;
        for(const auto& item:items)result.emplace_back(item.slot,item.definition_id,item.item_instance_id);
        std::sort(result.begin(),result.end());return result;};
    if(equipment_keys(saved.actor.equipment)!=equipment_keys(current.equipment))
        return fail("equipped source definitions/instances changed");
    if(saved.traits.is_player!=traits.is_player||saved.traits.targetable!=traits.targetable)
        return fail("player/targetable role changed");
    if(!traits.targetable&&current.attack_ids.empty())
        for(const unsigned index:{36u,38u,41u,43u})
            if((saved.combat.sheets.resolved[index]<0||combat.sheets.resolved[index]<0)&&
               saved.combat.sheets.resolved[index]!=combat.sheets.resolved[index])
                return fail("unavailable source vital metadata changed");
    if(saved.traits.main_item.has_value()!=traits.main_item.has_value()||
       (traits.main_item&&!std::equal(std::begin(saved.traits.main_item->words),
          std::end(saved.traits.main_item->words),std::begin(traits.main_item->words))))
        return fail("rendered main weapon record changed");
    const auto& old=saved.combat.facts;const auto& now=combat.facts;
    if(old.main_damage_class!=now.main_damage_class||old.off_damage_class!=now.off_damage_class||
       old.two_hander!=now.two_hander||old.dual_wield!=now.dual_wield||old.shield!=now.shield||
       saved.combat.sheets.gear!=combat.sheets.gear)
        return fail("equipped damage/defense/power binding changed");
    for(const unsigned index:{0u,1u,2u,3u,4u,26u})
        if(saved.combat.sheets.resolved[index]!=combat.sheets.resolved[index])
            return fail("original faction/AI/animation/model/class source changed");
    error.clear();return true;
}
}
bool validate_game_save(const GameSave& g,std::string& error){
    const auto fail=[&](const char* why){error=why;return false;};
    if(g.version<1||g.version>3)return fail("Unsupported game save version");
    if((g.version==1&&!g.objects.empty())||g.objects.size()>character_collection_limit)return fail("Invalid saved object version/count");
    const auto character_valid=validate_character_state(g.character);
    if(!character_valid.ok()){error=character_valid.errors.front();return false;}
    if(!text_valid(g.level_uri)||g.controlled_actor_id==0||g.actors.empty()||g.actors.size()>character_collection_limit)
        return fail("Invalid game save level/controlled actor/count");
    std::set<ActorId> ids;const ActorState* controlled=nullptr;
    for(const auto& p:g.actors){
        if(g.version<3&&p.actor.source_physical_present.has_value())
            return fail("Legacy game save cannot retain a known actor physical assignment");
        if(!ids.insert(p.actor.id).second)return fail("Duplicate saved actor ID");
        if(!validate_actor_state(p.actor,error))return false;
        if(p.actor.action_elapsed_seconds!=0||p.actor.target_id!=0||
           (p.actor.action!=CharacterAction::idle&&p.actor.action!=CharacterAction::dead))return fail("Saved actor has transient control state");
        if(p.actor.source_flags520||p.actor.source_movement_type||p.actor.source_validate_boundary452)return fail("Saved source motion cells must be reconstructed from canonical state");
        if(p.actor.source_target_node180||p.actor.source_target_position184)return fail("Saved source target node/cache must be reconstructed from the visual hierarchy");
        if(p.actor.faction_id!=p.combat.sheets.resolved[0])return fail("Saved actor faction/property mismatch");
        for(const auto value:{std::pair<float,unsigned>{p.actor.health,36},
             {p.actor.max_health,38},{p.actor.resource,41},{p.actor.max_resource,43}}){
            const double raw=std::round(double(value.first)*256.0);
            const auto source=p.combat.sheets.resolved[value.second];
            const bool unavailable=!p.traits.targetable&&p.actor.attack_ids.empty()&&source<0&&value.first==0;
            if(raw<INT32_MIN||raw>INT32_MAX||(!unavailable&&std::int32_t(raw)!=source))
                return fail("Saved live vital/original sheet mismatch");
        }
        const auto& f=p.combat.facts;
        if(f.main_damage_class< -1||f.main_damage_class>=141||f.off_damage_class< -1||f.off_damage_class>=141||
           f.combo_hits!=0||f.original_state!=-1)return fail("Invalid saved original combat facts");
        if(p.actor.id==g.controlled_actor_id)controlled=&p.actor;
    }
    for(const auto& object:g.objects){
        if(!ids.insert(object.id).second)return fail("Duplicate saved world ID");
        if(!validate_world_object(object,error))return false;
        if(!text_valid(object.name))return fail("Invalid saved world object identity");
        for(const auto* text:{&object.visual.model,&object.visual.material,&object.visual.animation})
            if(!text->empty()&&!text_valid(*text))return fail("Invalid saved world object visual");
    }
    if(!controlled)return fail("Controlled actor absent from save");
    if(!controlled->persistent_character_id||*controlled->persistent_character_id!=g.character.id)
        return fail("Controlled actor persistent identity differs");
    if(controlled->health!=g.character.stats.health||controlled->max_health!=g.character.stats.max_health||
       controlled->resource!=g.character.stats.resource||controlled->max_resource!=g.character.stats.max_resource)
        return fail("Character and controlled live vitals differ");
    error.clear();return true;
}
bool capture_game_save(const std::string& level,ActorId controlled,const CharacterState& character_state,
    const PlayableActorWorld& world,GameSave& output,std::string& error){try{
    GameSave next;next.level_uri=level;next.controlled_actor_id=controlled;next.character=character_state;next.random=world.random_state();
    for(const auto& entry:world.actors()){
        const auto* props=world.combat_properties(entry.first);const auto* traits=world.traits(entry.first);
        if(!props||!traits){error="Live actor lacks original bindings";return false;}
        PersistedPlayableActor record{entry.second,*props,*traits};normalize(record);next.actors.push_back(std::move(record));
    }
    for(const auto& entry:world.objects())next.objects.push_back(entry.second);
    if(!next.objects.empty())next.version=2;
    if(std::any_of(next.actors.begin(),next.actors.end(),[](const auto& p){
        return p.actor.source_physical_present.has_value();}))next.version=3;
    const auto* live=world.find_actor(controlled);if(!live){error="Controlled live actor absent";return false;}
    next.character.stats.health=live->health;next.character.stats.max_health=live->max_health;
    next.character.stats.resource=live->resource;next.character.stats.max_resource=live->max_resource;
    if(!validate_game_save(next,error))return false;output=std::move(next);return true;
}catch(const std::exception& ex){error=ex.what();return false;}}
bool restore_game_save(const GameSave& g,const std::string& expected,PlayableActorWorld& world,
    CharacterState& character_state,std::string& error){try{
    if(!validate_game_save(g,error))return false;
    if(g.level_uri!=expected){error="Saved level differs from loaded level";return false;}
    if(g.actors.size()!=world.actors().size()){error="Saved actor roster differs from loaded level";return false;}
    for(const auto& p:g.actors){const auto* live=world.find_actor(p.actor.id);
        if(!live||live->definition_id!=p.actor.definition_id){error="Saved actor ID/definition differs from loaded level";return false;}
        const auto* combat=world.combat_properties(p.actor.id);const auto* traits=world.traits(p.actor.id);
        if(!combat||!traits){error="Loaded actor original binding unavailable";return false;}
        if(!checkpoint_binding_compatible(p,*live,*combat,*traits,error))return false;
    }
    if(g.version>=2){
        if(g.objects.size()!=world.objects().size()){error="Saved object roster differs from loaded level";return false;}
        for(const auto& object:g.objects){const auto* live=world.find_object(object.id);
            if(!live||live->name!=object.name||live->visual.model!=object.visual.model||live->visual.material!=object.visual.material){
                error="Saved object ID/definition/visual differs from loaded level";return false;
            }
        }
    }
    auto next_character=g.character;
    // Old actor-only saves leave newly supported objects at their loaded state.
    // Version2 restores both subsets atomically; no callback/loot replay occurs.
    if(g.version>=2){if(!world.replace_state(g.actors,g.objects,g.random,error))return false;}
    else if(!world.replace_actors(g.actors,g.random,error))return false;
    character_state=std::move(next_character);error.clear();return true;
}catch(const std::exception& ex){error=ex.what();return false;}}
bool save_game(const std::filesystem::path& path,const GameSave& g,std::string& error){
    std::filesystem::path temporary;bool created=false;
    try{
        if(path.empty()||path.filename().empty())throw std::runtime_error("Game save filename empty");
        if(!validate_game_save(g,error))return false;
        Archive a;for(auto b:magic)a.bytes.push_back(b);auto copy=g;snapshot(a,copy);
        auto hash=checksum(a.bytes);a.u64(hash);
        static std::atomic<std::uint64_t> seq{0};temporary=path;
        temporary+=".tmp."+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count())+"."+std::to_string(seq++);
#ifdef _WIN32
        const auto handle=CreateFileW(temporary.c_str(),GENERIC_WRITE,0,nullptr,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,nullptr);
        if(handle==INVALID_HANDLE_VALUE)throw std::runtime_error("Cannot create game save temporary");created=true;
        DWORD written=0;const bool wrote=WriteFile(handle,a.bytes.data(),DWORD(a.bytes.size()),&written,nullptr)&&written==a.bytes.size();
        const bool flushed=wrote&&FlushFileBuffers(handle);const bool closed=CloseHandle(handle);
        if(!wrote||!flushed||!closed)throw std::runtime_error("Cannot write/flush game save temporary");
        if(!MoveFileExW(temporary.c_str(),path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))throw std::runtime_error("Cannot atomically replace game save");
#else
        if(std::filesystem::exists(temporary))throw std::runtime_error("Game save temporary exists");
        std::ofstream stream(temporary,std::ios::binary|std::ios::trunc);if(!stream)throw std::runtime_error("Cannot create game save temporary");created=true;
        stream.write(reinterpret_cast<const char*>(a.bytes.data()),std::streamsize(a.bytes.size()));stream.flush();
        if(!stream)throw std::runtime_error("Cannot write game save temporary");stream.close();if(!stream)throw std::runtime_error("Cannot close game save temporary");
        std::filesystem::rename(temporary,path);
#endif
        error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();}
    if(created){std::error_code ignored;std::filesystem::remove(temporary,ignored);}return false;
}
bool load_game(const std::filesystem::path& path,GameSave& output,std::string& error){try{
    std::ifstream stream(path,std::ios::binary|std::ios::ate);if(!stream)throw std::runtime_error("Cannot open game save");
    const auto length=stream.tellg();if(length<20||length>std::streamoff(file_limit))throw std::runtime_error("Game save size invalid");
    Archive a;a.reading=true;a.bytes.resize(std::size_t(length));stream.seekg(0);
    stream.read(reinterpret_cast<char*>(a.bytes.data()),std::streamsize(a.bytes.size()));if(!stream)throw std::runtime_error("Cannot read game save");
    a.at=a.bytes.size()-8;std::uint64_t expected=0;a.u64(expected);a.bytes.resize(a.bytes.size()-8);
    if(checksum(a.bytes)!=expected)throw std::runtime_error("Game save checksum differs");
    a.at=0;for(auto expected_byte:magic){std::uint64_t byte=0;a.word(byte,1);if(byte!=expected_byte)throw std::runtime_error("Not a live game save");}
    GameSave next;snapshot(a,next);if(a.at!=a.bytes.size())throw std::runtime_error("Game save trailing data");
    if(!validate_game_save(next,error))return false;output=std::move(next);error.clear();return true;
}catch(const std::exception& ex){error=ex.what();return false;}}
} // namespace dh::foundation
