#include "playable_actor_world.hpp"
#include "asset_catalog.hpp"
#include "../level-world/character_attack_geometry.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace dh::foundation {
bool load_original_ai_tables(const AssetCatalog& assets,const std::string& root,
                             dh2::data::AiTables& output,std::string& error) {
    try {
        const auto read=[&](const char* name){return assets.read(std::filesystem::path(root)/name);};
        const auto a=read("ai_pyarray.bin"),an=read("ai_pyarraynames.bin"),af=read("ai_pystructnames.bin"),
                   f=read("ai_factions_pyarray.bin"),fn=read("ai_factions_pyarraynames.bin"),
                   ff=read("ai_factions_pystructnames.bin");
        const auto bytes=[](const auto& value){return dh2::data::Bytes{value.data(),value.size()};};
        dh2::data::AiTables next;
        if(!dh2::data::load_ai(bytes(a),bytes(an),bytes(af),bytes(f),bytes(fn),bytes(ff),next,error))return false;
        output=std::move(next);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
dh2::data::CombatRandom PlayableActorWorld::random_state() const noexcept {
    return lootRandomLoan_?dh2::data::CombatRandom{lootRandomLoan_->seed,lootRandomLoan_->calls}:random_;
}
void PlayableActorWorld::sync_random_from_loan() const noexcept {
    if(lootRandomLoan_)random_={lootRandomLoan_->seed,lootRandomLoan_->calls};
}
void PlayableActorWorld::sync_loan_from_random() const noexcept {
    if(lootRandomLoan_){lootRandomLoan_->seed=random_.seed;lootRandomLoan_->calls=random_.calls;}
}
bool PlayableActorWorld::with_loot_random(const LootRandomConsumer& consume,std::string& error) {
    error.clear();
    if(!consume||lootRandomLoan_){error="Loot RNG requires one non-nested scoped consumer";return false;}
    dh2::data::LootRandom8V2 loan{random_.seed,random_.calls};lootRandomLoan_=&loan;
    bool success=false;
    try{success=consume(loan,error);}
    catch(const std::exception& ex){error=ex.what();}
    catch(...){error="Loot RNG consumer threw an unknown exception";}
    random_={loan.seed,loan.calls};lootRandomLoan_=nullptr;
    if(success)error.clear();else if(error.empty())error="Loot RNG consumer failed";
    return success;
}
bool PlayableActorWorld::random_uniform(std::uint32_t bound,std::uint32_t& output,std::string& error) {
    if(bound>static_cast<std::uint32_t>(INT32_MAX)) {
        error="Source random range exceeds signed 32-bit admission";
        return false;
    }
    sync_random_from_loan();
    output=static_cast<std::uint32_t>(dh2_combat_random(&random_,static_cast<std::int32_t>(bound)));
    sync_loan_from_random();
    error.clear();
    return true;
}

PlayableActorWorld::PlayableActorWorld(dh2::data::AiTables tables,dh2::data::CombatRandom& random)
    :tables_(std::move(tables)),random_(random) {
    if(tables_.rows.size()<=8||tables_.factions.size()<=10)
        throw std::invalid_argument("Original AI/faction fallback rows unavailable");
}
bool PlayableActorWorld::make_binding(const OriginalCombatProperties& properties,
    PlayableActorTraits traits,Binding& output,std::string& error) const {
    using namespace dh2::character;
    CombatProperties896 props{};
    std::copy(properties.sheets.resolved.begin(),properties.sheets.resolved.end(),props.words);
    CombatItemRecord164 item{};
    if(traits.main_item) std::copy_n(traits.main_item->words,41,item.words);
    const CombatItemInstance4 instance{0};const CombatItemInstance4* reference=&instance;
    const CombatEquipSet8 set{traits.main_item?&reference:nullptr};
    const CombatInventory16 inventory{&set,1,0};
    std::vector<float> radii; radii.reserve(tables_.rows.size());
    for(const auto& ai:tables_.rows)radii.push_back(ai.melee_radius);
    float reach=0;
    if(dh2_attack_melee_radius(&reach,&props,&inventory,&item,1,radii.data(),
        std::uint32_t(radii.size()))||!std::isfinite(reach)||reach<0) {
        error="Original actor melee reach unavailable/invalid";return false;
    }
    output={std::move(traits),reach};error.clear();return true;
}
bool PlayableActorWorld::bind_actor(ActorState actor,OriginalCombatProperties properties,
    PlayableActorTraits traits,std::string& error) {
    if(actors_.count(actor.id)||objects_.count(actor.id)){error="World ID already bound";return false;}
    if(!validate_actor_state(actor,error))return false;
    if(actor.faction_id!=properties.sheets.resolved[0]){error="Actor faction differs from original property sheet";return false;}
    Binding binding;
    if(!make_binding(properties,std::move(traits),binding,error))return false;
    if(!damage_.bind_actor(actor.id,std::move(properties),error))return false;
    const ActorId id=actor.id;actors_.emplace(id,std::move(actor));bindings_.emplace(id,std::move(binding));
    error.clear();return true;
}
bool PlayableActorWorld::remove_actor(ActorId id) {
    if(!actors_.erase(id))return false;
    bindings_.erase(id);damage_.remove_actor(id);
    for(auto& entry:actors_)if(entry.second.target_id==id)entry.second.target_id=invalid_actor_id;
    return true;
}
bool PlayableActorWorld::bind_object(WorldObject object,std::string& error){
    if(actors_.count(object.id)||objects_.count(object.id)){error="World ID already bound";return false;}
    if(!validate_world_object(object,error))return false;
    const auto id=object.id;objects_.emplace(id,std::move(object));error.clear();return true;
}
const WorldObject* PlayableActorWorld::find_object(ObjectId id)const noexcept{
    const auto found=objects_.find(id);return found==objects_.end()?nullptr:&found->second;
}
WorldObject* PlayableActorWorld::find_object(ObjectId id)noexcept{
    const auto found=objects_.find(id);return found==objects_.end()?nullptr:&found->second;
}
bool PlayableActorWorld::remove_object(ObjectId id){return objects_.erase(id)!=0;}
bool PlayableActorWorld::set_object_transform(ObjectId id,const Transform& value){
    const auto found=objects_.find(id);if(found==objects_.end())return false;
    auto candidate=found->second;candidate.transform=value;std::string error;
    if(!validate_world_object(candidate,error))return false;
    found->second.transform=value;return true;
}
bool PlayableActorWorld::set_object_visual(ObjectId id,VisualReference value){
    const auto found=objects_.find(id);if(found==objects_.end())return false;
    found->second.visual=std::move(value);return true;
}
bool PlayableActorWorld::set_object_component(ObjectId id,std::string key,std::vector<std::uint8_t> value,std::string& error){
    const auto object=objects_.find(id);if(object==objects_.end()){error="World object is not bound";return false;}
    auto candidate=object->second;candidate.state_components.insert_or_assign(std::move(key),std::move(value));
    if(!validate_world_object(candidate,error))return false;
    // Replace just this record without invalidating other object borrows.
    object->second.state_components.swap(candidate.state_components);error.clear();return true;
}
void PlayableActorWorld::clear() {
    if(lootRandomLoan_)throw std::logic_error("Cannot clear actors during a scoped loot RNG consumer");
    for(const auto& actor:actors_)damage_.remove_actor(actor.first);
    actors_.clear();objects_.clear();bindings_.clear();resolutions_.clear();resolutionObserverErrors_.clear();
}
bool PlayableActorWorld::bind_source(std::string id,OriginalMeleeSource source,std::string& error) {
    return damage_.bind_source(std::move(id),source,error);
}
bool PlayableActorWorld::update_combat_properties(ActorId id,OriginalCombatProperties properties,
    PlayableActorTraits traits,std::string& error) {
    const auto actor=actors_.find(id);
    if(actor==actors_.end()){error="Actor not bound";return false;}
    Binding binding;if(!make_binding(properties,std::move(traits),binding,error))return false;
    const auto faction=properties.sheets.resolved[0];
    if(!damage_.bind_actor(id,std::move(properties),error))return false;
    bindings_.at(id)=std::move(binding);actor->second.faction_id=faction;
    error.clear();return true;
}
ActorState* PlayableActorWorld::find_actor(ActorId id) {
    auto found=actors_.find(id);return found==actors_.end()?nullptr:&found->second;
}
const ActorState* PlayableActorWorld::find_actor(ActorId id) const {
    auto found=actors_.find(id);return found==actors_.end()?nullptr:&found->second;
}
bool PlayableActorWorld::eligible_target(const ActorState& attacker,const ActorState& victim) const {
    const auto a=bindings_.find(attacker.id),d=bindings_.find(victim.id);
    const auto* live_a=find_actor(attacker.id);const auto* live_d=find_actor(victim.id);
    if(a==bindings_.end()||d==bindings_.end()||!live_a||!live_d||attacker.id==victim.id||
       !live_a->alive()||!live_d->alive()||live_a->action==CharacterAction::dead||
       live_d->action==CharacterAction::dead||!d->second.traits.targetable)return false;
    return dh2::data::ai_enemy(tables_,live_a->faction_id,live_d->faction_id,
        a->second.traits.is_player,d->second.traits.is_player);
}
float PlayableActorWorld::melee_reach(ActorId id) const noexcept {
    const auto found=bindings_.find(id);return found==bindings_.end()?0:found->second.melee_reach;
}
float PlayableActorWorld::target_radius(const ActorState& actor) const {return melee_reach(actor.id);}
bool PlayableActorWorld::original_melee_in_range(ActorId attacker,ActorId victim) const {
    const auto* a=find_actor(attacker);const auto* d=find_actor(victim);
    if(!a||!d)return false;
    const float radii[]={melee_reach(attacker),melee_reach(victim)};
    return dh2_attack_melee_distance(a->transform.position.data(),d->transform.position.data(),radii)==1;
}
bool PlayableActorWorld::resolve_damage(const std::string& source,const ActorState& attacker,
    const ActorState& victim,const std::string& marker,float& amount,std::string& error) const {
    std::optional<std::uint32_t> outcomes,source_mask;
    return resolve_damage_with_outcomes(source,attacker,victim,marker,amount,outcomes,source_mask,error);
}
bool PlayableActorWorld::resolve_damage_with_outcomes(const std::string& source,const ActorState& attacker,
    const ActorState& victim,const std::string& marker,float& amount,
    std::optional<std::uint32_t>& outcomes,std::optional<std::uint32_t>& source_mask,
    std::string& error) const {
    if(!eligible_target(attacker,victim)){error="Original target eligibility rejected";return false;}
    if(!original_melee_in_range(attacker.id,victim.id)){error="Original strict melee range rejected";return false;}
    PlayableCombatResolution event;event.attacker=attacker.id;event.victim=victim.id;
    event.source_id=source;event.marker_name=marker;
    sync_random_from_loan();bool resolved=false;
    try{resolved=damage_.resolve(source,attacker.id,victim.id,random_,event.melee,error);}
    catch(...){sync_loan_from_random();throw;}
    sync_loan_from_random();
    if(!resolved)return false;
    if(resolutionObserver_) {
        std::string presentationError;
        if(!resolutionObserver_(event,presentationError))
            resolutionObserverErrors_.push_back(presentationError.empty()?"Combat presentation observer failed":std::move(presentationError));
    }
    amount=event.melee.damage;outcomes=event.melee.original.outcomes;
    source_mask=event.melee.original.mask;
    resolutions_.push_back(std::move(event));error.clear();return true;
}
std::vector<PlayableCombatResolution> PlayableActorWorld::take_resolutions() {
    std::vector<PlayableCombatResolution> result;result.swap(resolutions_);return result;
}
bool PlayableActorWorld::resolve_source_result(const std::string& source,
    ActorId attacker,ActorId victim,const std::string& marker,std::uint32_t mask,
    std::int32_t category,std::int32_t element,std::int32_t direct_amount,
    OriginalMeleeResolution& output,std::string& error,
    const dh2::data::PropertySheet* attacker_formula_sheet) {
    const auto* a=find_actor(attacker);const auto* d=find_actor(victim);
    if(source.empty()||marker.empty()||!a||!d||!eligible_target(*a,*d)){
        error="Source result identity/target eligibility rejected";return false;
    }
    return calculate_source_result(source,attacker,victim,marker,mask,category,element,direct_amount,output,error,attacker_formula_sheet);
}
bool PlayableActorWorld::calculate_source_result(const std::string& source,
    ActorId attacker,ActorId victim,const std::string& marker,std::uint32_t mask,
    std::int32_t category,std::int32_t element,std::int32_t direct_amount,
    OriginalMeleeResolution& output,std::string& error,
    const dh2::data::PropertySheet* attacker_formula_sheet){
    if(source.empty()||marker.empty()||!find_actor(attacker)||!find_actor(victim)){
        error="Source calculation requires current character identities";return false;
    }
    PlayableCombatResolution event;event.attacker=attacker;event.victim=victim;
    event.source_id=source;event.marker_name=marker;
    sync_random_from_loan();bool resolved=false;
    try{resolved=damage_.resolve_result(attacker,victim,mask,category,element,
        direct_amount,random_,event.melee,error,attacker_formula_sheet);}catch(...){sync_loan_from_random();throw;}
    sync_loan_from_random();if(!resolved)return false;
    if(resolutionObserver_){std::string presentationError;
        bool observed=false;
        try{observed=resolutionObserver_(event,presentationError);}
        catch(const std::exception& ex){presentationError=ex.what();}
        catch(...){presentationError="Combat presentation observer threw";}
        if(!observed)
            resolutionObserverErrors_.push_back(presentationError.empty()?"Combat presentation observer failed":std::move(presentationError));
    }
    output=event.melee;resolutions_.push_back(std::move(event));error.clear();return true;
}
std::vector<std::string> PlayableActorWorld::take_resolution_observer_errors() {
    std::vector<std::string> result;result.swap(resolutionObserverErrors_);return result;
}
const OriginalCombatProperties* PlayableActorWorld::combat_properties(ActorId id) const noexcept {
    return damage_.properties(id);
}
const PlayableActorTraits* PlayableActorWorld::traits(ActorId id) const noexcept {
    const auto found=bindings_.find(id);return found==bindings_.end()?nullptr:&found->second.traits;
}
bool PlayableActorWorld::replace_actors(const std::vector<PersistedPlayableActor>& records,
    dh2::data::CombatRandom random,std::string& error) {
    std::vector<WorldObject> objects;objects.reserve(objects_.size());
    for(const auto& object:objects_)objects.push_back(object.second);
    return replace_state(records,std::move(objects),random,error);
}
bool PlayableActorWorld::replace_state(const std::vector<PersistedPlayableActor>& records,
    std::vector<WorldObject> objects,dh2::data::CombatRandom random,std::string& error) {
    if(lootRandomLoan_){error="Cannot replace actors/RNG during a scoped loot RNG consumer";return false;}
    try {
        if(records.size()>character_collection_limit||objects.size()>character_collection_limit){error="World restore count exceeds limit";return false;}
        auto temporary_random=random;PlayableActorWorld candidate(tables_,temporary_random);
        candidate.damage_=damage_;
        for(const auto& old:actors_)candidate.damage_.remove_actor(old.first);
        for(auto& object:objects)if(!candidate.bind_object(std::move(object),error))return false;
        for(const auto& record:records)
            if(!candidate.bind_actor(record.actor,record.combat,record.traits,error))return false;
        actors_.swap(candidate.actors_);bindings_.swap(candidate.bindings_);
        std::swap(damage_,candidate.damage_);resolutions_.clear();resolutionObserverErrors_.clear();random_=random;
        objects_.swap(candidate.objects_);
        error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
} // namespace dh::foundation
