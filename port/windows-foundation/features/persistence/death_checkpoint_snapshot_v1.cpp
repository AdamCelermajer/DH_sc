#include "death_checkpoint_snapshot_v1.hpp"

#include "../../character_state.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <set>

namespace dh::foundation::persistence::death_flow {
namespace {
constexpr std::size_t max_records = character_collection_limit;
constexpr std::size_t max_text = character_text_limit;
constexpr std::size_t max_qest = 16u * 1024u * 1024u;
constexpr std::size_t max_file = 64u * 1024u * 1024u;
constexpr std::uint8_t magic[4] = {'D','C','P','1'};

bool valid_text(const std::string& value, bool allow_empty = false) {
    if ((!allow_empty && value.empty()) || value.size() > max_text) return false;
    for (unsigned char ch : value) if (ch < 32 || ch == 127) return false;
    return true;
}
bool finite_transform(const Transform& t) {
    for (float value : t.position) if (!std::isfinite(value)) return false;
    for (float value : t.rotation) if (!std::isfinite(value)) return false;
    for (float value : t.scale) if (!std::isfinite(value)) return false;
    return true;
}
bool finite_position(const std::array<float,3>& p) {
    return std::all_of(p.begin(), p.end(), [](float v) { return std::isfinite(v); });
}
bool actor_family(bool (*fn)(void*, ActorId, SourceCheckpointObjectFamilyV1&,
                 bool&,std::string&),void* ctx,ActorId id,
                 SourceCheckpointObjectFamilyV1& family,bool& skip,std::string& error) {
    if(!fn){error="Source actor checkpoint family provider is required";return false;}
    if(!fn(ctx,id,family,skip,error)){if(error.empty())error="Source actor checkpoint family provider failed";return false;}
    return true;
}
bool object_family(bool (*fn)(void*, ObjectId, SourceCheckpointObjectFamilyV1&,
                  bool&,std::string&),void* ctx,ObjectId id,
                  SourceCheckpointObjectFamilyV1& family,bool& skip,std::string& error) {
    if(!fn){error="Source object checkpoint family provider is required";return false;}
    if(!fn(ctx,id,family,skip,error)){if(error.empty())error="Source object checkpoint family provider failed";return false;}
    return true;
}

class Writer {
public:
    std::vector<std::uint8_t> bytes;
    void u8(std::uint8_t v) { bytes.push_back(v); }
    void u32(std::uint32_t v) { for (unsigned i=0;i<4;++i) u8(std::uint8_t(v>>(i*8))); }
    void u64(std::uint64_t v) { for (unsigned i=0;i<8;++i) u8(std::uint8_t(v>>(i*8))); }
    void f32(float v) { std::uint32_t bits=0; std::memcpy(&bits,&v,4); u32(bits); }
    void text(const std::string& v) { u32(static_cast<std::uint32_t>(v.size())); bytes.insert(bytes.end(),v.begin(),v.end()); }
    void blob(const std::vector<std::uint8_t>& v) { u32(static_cast<std::uint32_t>(v.size())); bytes.insert(bytes.end(),v.begin(),v.end()); }
};
class Reader {
    const std::vector<std::uint8_t>& bytes_;
    std::size_t at_ = 0;
public:
    explicit Reader(const std::vector<std::uint8_t>& bytes) : bytes_(bytes) {}
    bool raw(void* out, std::size_t n) {
        if (n > bytes_.size()-at_) return false;
        std::memcpy(out,bytes_.data()+at_,n); at_+=n; return true;
    }
    bool u8(std::uint8_t& v) { return raw(&v,1); }
    bool u32(std::uint32_t& v) { v=0; for(unsigned i=0;i<4;++i){std::uint8_t b=0;if(!u8(b))return false;v|=std::uint32_t(b)<<(i*8);}return true; }
    bool u64(std::uint64_t& v) { v=0; for(unsigned i=0;i<8;++i){std::uint8_t b=0;if(!u8(b))return false;v|=std::uint64_t(b)<<(i*8);}return true; }
    bool f32(float& v) { std::uint32_t bits=0;if(!u32(bits))return false;std::memcpy(&v,&bits,4);return true; }
    bool text(std::string& v, std::size_t limit=max_text) {
        std::uint32_t n=0;if(!u32(n)||n>limit||n>bytes_.size()-at_)return false;
        v.assign(reinterpret_cast<const char*>(bytes_.data()+at_),n);at_+=n;return true;
    }
    bool blob(std::vector<std::uint8_t>& v, std::size_t limit=max_file) {
        std::uint32_t n=0;if(!u32(n)||n>limit||n>bytes_.size()-at_)return false;
        v.assign(bytes_.begin()+std::ptrdiff_t(at_),bytes_.begin()+std::ptrdiff_t(at_+n));at_+=n;return true;
    }
    std::size_t position() const noexcept { return at_; }
    bool finished() const noexcept { return at_==bytes_.size(); }
};
std::uint64_t checksum(const std::uint8_t* data, std::size_t size) {
    std::uint64_t h=14695981039346656037ull;
    for(std::size_t i=0;i<size;++i){h^=data[i];h*=1099511628211ull;}
    return h;
}
bool same_equipment(const std::vector<ActorEquipmentReference>& a,
                    const std::vector<ActorEquipmentReference>& b) {
    if(a.size()!=b.size())return false;
    for(std::size_t i=0;i<a.size();++i)
        if(a[i].slot!=b[i].slot||a[i].definition_id!=b[i].definition_id||a[i].item_instance_id!=b[i].item_instance_id)
            return false;
    return true;
}
void write_transform(Writer& w,const Transform& t) {
    for(float v:t.position)w.f32(v);for(float v:t.rotation)w.f32(v);for(float v:t.scale)w.f32(v);
}
bool read_transform(Reader& r,Transform& t) {
    for(float& v:t.position)if(!r.f32(v))return false;
    for(float& v:t.rotation)if(!r.f32(v))return false;
    for(float& v:t.scale)if(!r.f32(v))return false;
    return true;
}
void write_position(Writer& w,const std::array<float,3>& p){for(float v:p)w.f32(v);}
bool read_position(Reader& r,std::array<float,3>& p){for(float& v:p)if(!r.f32(v))return false;return true;}

bool same_player_protected_fields(const ActorState& a,const ActorState& b) {
    return a.id==b.id && a.definition_id==b.definition_id && a.class_id==b.class_id &&
        a.faction_id==b.faction_id && same_equipment(a.equipment,b.equipment) && a.attack_ids==b.attack_ids &&
        a.persistent_character_id==b.persistent_character_id && a.health==b.health &&
        a.max_health==b.max_health && a.resource==b.resource && a.max_resource==b.max_resource;
}
}

bool validate_death_checkpoint_snapshot_v1(const DeathCheckpointSnapshotV1& s,
                                           std::string& error) {
    auto fail=[&](const char* why){error=why;return false;};
    if(s.version!=1)return fail("Unsupported death checkpoint snapshot version");
    if(!valid_text(s.level_uri)||!valid_text(s.checkpoint_name))return fail("Death checkpoint level/name is invalid");
    if(s.controlled_actor_id==invalid_actor_id)return fail("Death checkpoint controlled actor ID is invalid");
    if(!finite_position(s.checkpoint_key_position)||!finite_position(s.revive_position))return fail("Death checkpoint position is nonfinite");
    if(s.actors.size()>max_records||s.objects.size()>max_records)return fail("Death checkpoint record count exceeds limit");
    if(s.qest.size()>max_qest||(!s.qest_present&&!s.qest.empty()))return fail("Death checkpoint QEST bytes are invalid");
    std::set<ActorId> actor_ids;
    for(const auto& a:s.actors){
        if(!a.id||!valid_text(a.definition_id)||!actor_ids.insert(a.id).second)return fail("Death checkpoint actor identity is invalid or duplicated");
        if(!finite_transform(a.transform))return fail("Death checkpoint actor transform is nonfinite");
        if(a.npc_vitals_present && (a.player || !std::isfinite(a.health)||!std::isfinite(a.max_health)||!std::isfinite(a.resource)||!std::isfinite(a.max_resource)))
            return fail("Death checkpoint NPC vitals are invalid");
    }
    std::set<ObjectId> object_ids;
    for(const auto& o:s.objects){
        if(!o.id||!valid_text(o.name)||!valid_text(o.visual.model,true)||!valid_text(o.visual.material,true)||!object_ids.insert(o.id).second)
            return fail("Death checkpoint object identity is invalid or duplicated");
        if(!finite_transform(o.transform)||o.state_components.size()>world_object_component_limit)
            return fail("Death checkpoint object transform/components are invalid");
        std::size_t total=0;
        for(const auto& entry:o.state_components){
            if(!valid_text(entry.first)||entry.second.size()>world_object_state_limit-total)
                return fail("Death checkpoint object component exceeds limit");
            total+=entry.second.size();
        }
    }
    error.clear();return true;
}

bool capture_death_checkpoint_snapshot_v1(
    const GameSave& game, const std::string& level_uri,
    const std::string& checkpoint_name,
    const std::array<float,3>& checkpoint_key_position,
    const std::array<float,3>& revive_position,
    const DeathCheckpointCaptureServicesV1& services,
    DeathCheckpointSnapshotV1& output, std::string& error) {
    if(!validate_game_save(game,error,true))return false; // checkpoint subset: see game_save.cpp checkpointUnboundVitals
    if(game.level_uri!=level_uri)return (error="GameSave is for a different loaded level",false);
    if(!services.actor_checkpoint_facts||!services.actor_source_state||!services.object_checkpoint_facts)
        return (error="Source checkpoint eligibility and Character-state providers are required",false);
    DeathCheckpointSnapshotV1 next;
    next.level_uri=level_uri;next.checkpoint_name=checkpoint_name;
    next.controlled_actor_id=game.controlled_actor_id;
    next.checkpoint_key_position=checkpoint_key_position;next.revive_position=revive_position;
    next.qest_present=!game.character.source_quest_progress_cqpg.empty();
    if(next.qest_present)next.qest=game.character.source_quest_progress_cqpg;
    try {
        for(const auto& p:game.actors){
            SourceCheckpointObjectFamilyV1 family=SourceCheckpointObjectFamilyV1::unknown;
            bool skipCheckpoint=false,eligible=false;
            if(!actor_family(services.actor_checkpoint_facts,services.context,p.actor.id,family,skipCheckpoint,error)||
               !source_checkpoint_object_eligible_v1(family,skipCheckpoint,eligible,error))return false;
            if(!eligible)continue;
            DeathCheckpointActorV1 a;
            a.id=p.actor.id;a.definition_id=p.actor.definition_id;
            a.player=p.traits.is_player||p.actor.id==game.controlled_actor_id;
            a.transform=p.actor.transform;
            if(!services.actor_source_state(services.context,a.id,a.source_cs_state,a.source_dead,error)){
                if(error.empty())error="Source Character serialized CS-state provider failed";return false;
            }
            if(!a.player){a.npc_vitals_present=true;a.health=p.actor.health;a.max_health=p.actor.max_health;
                a.resource=p.actor.resource;a.max_resource=p.actor.max_resource;}
            next.actors.push_back(std::move(a));
        }
        for(const auto& o:game.objects){
            SourceCheckpointObjectFamilyV1 family=SourceCheckpointObjectFamilyV1::unknown;
            bool skipCheckpoint=false,eligible=false;
            if(!object_family(services.object_checkpoint_facts,services.context,o.id,family,skipCheckpoint,error)||
               !source_checkpoint_object_eligible_v1(family,skipCheckpoint,eligible,error)){
                if(error.empty())error="Source object checkpoint eligibility provider failed";return false;
            }
            if(eligible)next.objects.push_back(o);
        }
    }catch(const std::exception& ex){error=ex.what();return false;}
    catch(...){error="Checkpoint snapshot provider threw";return false;}
    if(!validate_death_checkpoint_snapshot_v1(next,error))return false;
    output=std::move(next);error.clear();return true;
}

bool encode_death_checkpoint_snapshot_v1(const DeathCheckpointSnapshotV1& s,
                                         std::vector<std::uint8_t>& output,
                                         std::string& error) {
    if(!validate_death_checkpoint_snapshot_v1(s,error))return false;
    Writer w;
    w.bytes.insert(w.bytes.end(),std::begin(magic),std::end(magic));
    w.u32(s.version);w.text(s.level_uri);w.text(s.checkpoint_name);w.u64(s.controlled_actor_id);
    write_position(w,s.checkpoint_key_position);write_position(w,s.revive_position);
    w.u8(s.qest_present?1:0);w.blob(s.qest);
    w.u32(static_cast<std::uint32_t>(s.actors.size()));
    for(const auto& a:s.actors){
        w.u64(a.id);w.text(a.definition_id);w.u8(a.player?1:0);write_transform(w,a.transform);
        w.u32(a.source_cs_state);w.u8(a.source_dead?1:0);
        w.u8(a.npc_vitals_present?1:0);if(a.npc_vitals_present){w.f32(a.health);w.f32(a.max_health);w.f32(a.resource);w.f32(a.max_resource);}
    }
    w.u32(static_cast<std::uint32_t>(s.objects.size()));
    for(const auto& o:s.objects){
        w.u64(o.id);w.text(o.name);write_transform(w,o.transform);
        w.text(o.visual.model);w.text(o.visual.material);w.text(o.visual.animation);w.u8(o.visual.visible?1:0);
        w.u32(static_cast<std::uint32_t>(o.state_components.size()));
        for(const auto& c:o.state_components){w.text(c.first);w.blob(c.second);}
    }
    if(w.bytes.size()+8>max_file)return (error="Death checkpoint snapshot exceeds file limit",false);
    const auto hash=checksum(w.bytes.data(),w.bytes.size());w.u64(hash);
    output=std::move(w.bytes);error.clear();return true;
}

bool decode_death_checkpoint_snapshot_v1(const std::vector<std::uint8_t>& input,
                                         DeathCheckpointSnapshotV1& output,
                                         std::string& error) {
    if(input.size()<20||input.size()>max_file)return (error="Death checkpoint wire size is invalid",false);
    std::uint64_t expected=0;
    for(unsigned i=0;i<8;++i)expected|=std::uint64_t(input[input.size()-8+i])<<(i*8);
    if(checksum(input.data(),input.size()-8)!=expected)return (error="Death checkpoint checksum differs",false);
    std::vector<std::uint8_t> body(input.begin(),input.end()-8);
    Reader r(body);std::uint8_t gotMagic[4]{};
    if(!r.raw(gotMagic,4)||!std::equal(std::begin(magic),std::end(magic),gotMagic))return (error="Death checkpoint magic differs",false);
    DeathCheckpointSnapshotV1 next;
    std::uint8_t flag=0;std::uint32_t count=0;
    if(!r.u32(next.version)||!r.text(next.level_uri)||!r.text(next.checkpoint_name)||!r.u64(next.controlled_actor_id)||
       !read_position(r,next.checkpoint_key_position)||!read_position(r,next.revive_position)||!r.u8(flag)||flag>1||!r.blob(next.qest,max_qest))
        return (error="Death checkpoint header is truncated or invalid",false);
    next.qest_present=flag!=0;
    if(!r.u32(count)||count>max_records)return (error="Death checkpoint actor count is invalid",false);
    next.actors.reserve(count);
    for(std::uint32_t i=0;i<count;++i){
        DeathCheckpointActorV1 a;std::uint8_t b=0,dead=0,hasVitals=0;
        if(!r.u64(a.id)||!r.text(a.definition_id)||!r.u8(b)||b>1||!read_transform(r,a.transform)||
           !r.u32(a.source_cs_state)||!r.u8(dead)||dead>1)
            return (error="Death checkpoint actor record is truncated",false);
        a.player=b!=0;
        a.source_dead=dead!=0;
        if(!r.u8(hasVitals)||hasVitals>1)return (error="Death checkpoint actor vitals flag is invalid",false);
        a.npc_vitals_present=hasVitals!=0;
        if(a.npc_vitals_present&&(!r.f32(a.health)||!r.f32(a.max_health)||!r.f32(a.resource)||!r.f32(a.max_resource)))
            return (error="Death checkpoint actor vitals are truncated",false);
        next.actors.push_back(std::move(a));
    }
    if(!r.u32(count)||count>max_records)return (error="Death checkpoint object count is invalid",false);
    next.objects.reserve(count);
    for(std::uint32_t i=0;i<count;++i){
        WorldObject o;std::uint8_t visible=0;std::uint32_t components=0;
        if(!r.u64(o.id)||!r.text(o.name)||!read_transform(r,o.transform)||!r.text(o.visual.model)||
           !r.text(o.visual.material)||!r.text(o.visual.animation)||!r.u8(visible)||visible>1||
           !r.u32(components)||components>world_object_component_limit)
            return (error="Death checkpoint object record is truncated",false);
        o.visual.visible=visible!=0;
        for(std::uint32_t c=0;c<components;++c){std::string key;std::vector<std::uint8_t> bytes;
            if(!r.text(key)||!r.blob(bytes,world_object_state_limit)||!o.state_components.emplace(std::move(key),std::move(bytes)).second)
                return (error="Death checkpoint object component is invalid",false);
        }
        next.objects.push_back(std::move(o));
    }
    if(!r.finished())return (error="Death checkpoint has trailing data",false);
    if(!validate_death_checkpoint_snapshot_v1(next,error))return false;
    output=std::move(next);error.clear();return true;
}

bool restore_death_checkpoint_snapshot_v1(
    const DeathCheckpointSnapshotV1& snapshot, const std::string& expected_level_uri,
    PlayableActorWorld& world, CharacterState& character,
    const DeathCheckpointRestoreServicesV1& services,
    DeathCheckpointRestoreReceiptV1& receipt, std::string& error) {
    receipt={};
    if(!validate_death_checkpoint_snapshot_v1(snapshot,error))return false;
    if(snapshot.level_uri!=expected_level_uri)return (error="Death checkpoint belongs to a different active level",false);
    std::map<ActorId,const DeathCheckpointActorV1*> records;
    for(const auto& a:snapshot.actors)records.emplace(a.id,&a);
    std::vector<PersistedPlayableActor> stagedActors;
    stagedActors.reserve(world.actors().size());
    try {
        for(const auto& liveEntry:world.actors()){
            const auto id=liveEntry.first;
            const auto* combat=world.combat_properties(id);const auto* traits=world.traits(id);
            if(!combat||!traits)return (error="Live actor source bindings are unavailable",false);
            PersistedPlayableActor p{liveEntry.second,*combat,*traits};
            const auto it=records.find(id);
            if(it==records.end()){stagedActors.push_back(std::move(p));continue;}
            const auto& saved=*it->second;
            if(saved.definition_id!=p.actor.definition_id||saved.player!=(p.traits.is_player||id==snapshot.controlled_actor_id))
                return (error="Death checkpoint actor identity/role differs",false);
            bool alreadyLimbus=false,alreadyPreSpawn=false;
            std::optional<bool> canRespawn,physicalBodyPresent;
            if(!services.current_source_states||!services.apply_source_state)
                return (error="Source Character restore state providers are required",false);
            if(!services.current_source_states(services.context,id,alreadyLimbus,alreadyPreSpawn,
                    canRespawn,physicalBodyPresent,error)){
                if(error.empty())error="Current source Character state provider failed";return false;
            }
            SourceCharacterRestorePlanV1 plan;
            if(!source_character_restore_plan_v1(saved.source_cs_state,saved.source_dead,
                    alreadyLimbus,alreadyPreSpawn,canRespawn,physicalBodyPresent,plan,error))return false;
            ActorState candidate=p.actor;
            candidate.transform=saved.transform;
            if(saved.npc_vitals_present){candidate.health=saved.health;candidate.max_health=saved.max_health;
                candidate.resource=saved.resource;candidate.max_resource=saved.max_resource;}
            if(!services.apply_source_state(services.context,plan,candidate,error)){
                if(error.empty())error="Source Character restore projection failed";return false;
            }
            if(saved.player&&!same_player_protected_fields(p.actor,candidate))
                return (error="Source Character projection changed protected player profile/vital fields",false);
            if(candidate.id!=p.actor.id||candidate.definition_id!=p.actor.definition_id||
               candidate.class_id!=p.actor.class_id||candidate.faction_id!=p.actor.faction_id||
               !same_equipment(candidate.equipment,p.actor.equipment)||candidate.attack_ids!=p.actor.attack_ids||
               candidate.persistent_character_id!=p.actor.persistent_character_id)
                return (error="Source Character projection changed live actor identity/bindings",false);
            std::string actorValidation;
            if(!validate_actor_state(candidate,actorValidation)) {
                error="Death checkpoint staged actor validation failed actor="+std::to_string(id)+
                    " definition="+candidate.definition_id+" savedCS="+std::to_string(saved.source_cs_state)+
                    " savedDead="+(saved.source_dead?"1":"0")+" resultingDead="+(candidate.source_dead?"1":"0")+
                    " resultingLimbus="+(candidate.source_limbus?"1":"0")+" HP="+std::to_string(candidate.health)+
                    "/"+std::to_string(candidate.max_health)+": "+actorValidation;
                return false;
            }
            p.actor=std::move(candidate);
            stagedActors.push_back(std::move(p));
            ++receipt.actors_restored;
        }
    }catch(const std::exception& ex){error=ex.what();return false;}
    catch(...){error="Death checkpoint actor staging threw";return false;}
    for(const auto& saved:snapshot.actors)if(!world.find_actor(saved.id))++receipt.actors_missing_skipped;

    std::vector<WorldObject> stagedObjects;
    stagedObjects.reserve(world.objects().size());
    std::map<ObjectId,const WorldObject*> objectRecords;
    for(const auto& o:snapshot.objects)objectRecords.emplace(o.id,&o);
    for(const auto& liveEntry:world.objects()){
        WorldObject object=liveEntry.second;
        const auto found=objectRecords.find(object.id);
        if(found!=objectRecords.end()){
            const auto& saved=*found->second;
            if(saved.name!=object.name||saved.visual.model!=object.visual.model)
                return (error="Death checkpoint world object ID/name/model identity differs",false);
            object.transform=saved.transform;
            object.visual=saved.visual;
            object.state_components=saved.state_components;
            ++receipt.objects_restored;
        }
        stagedObjects.push_back(std::move(object));
    }
    for(const auto& saved:snapshot.objects)if(!world.find_object(saved.id))++receipt.objects_missing_skipped;

    CharacterState stagedCharacter=character;
    if(!services.initialize_source_qest)
        return (error="Source PlayerSavegame INITQUESTS checkpoint initializer is required",false);
    std::vector<std::uint8_t> initializedQest;
    if(!services.initialize_source_qest(services.context,initializedQest,error)){
        if(error.empty())error="Source PlayerSavegame INITQUESTS checkpoint initializer failed";return false;
    }
    stagedCharacter.source_quest_progress_cqpg=snapshot.qest_present?snapshot.qest:std::move(initializedQest);
    const auto validation=validate_character_state(stagedCharacter);
    if(!validation.ok())return (error=validation.errors.front(),false);
    if(!world.replace_state(stagedActors,std::move(stagedObjects),world.random_state(),error))return false;
    character=std::move(stagedCharacter);receipt.qest_restored=true;
    receipt.complete=true;error.clear();return true;
}

} // namespace dh::foundation::persistence::death_flow
