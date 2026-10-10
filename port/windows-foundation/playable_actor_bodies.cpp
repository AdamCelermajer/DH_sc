#include "playable_actor_bodies.hpp"
#include <exception>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cmath>
namespace dh::foundation {
std::function<bool(dh2::physical::ContactEvent,void*,unsigned,std::string&)>
original_actor_contact_callback(ActorId owner,OriginalActorContactBindings services) {
    return [owner,services=std::move(services)](dh2::physical::ContactEvent event,void* context,unsigned persist,std::string& error) {
        error.clear();
        if(event==dh2::physical::ContactEvent::result)return true; // Original46fb14: bx lr.
        if(!owner||!services.runtime_lease||!services.peer_owner) {error="Required actual POCharacter owner/peer contact borrow";return false;}
        std::uintptr_t peer=0;if(!services.peer_owner(context,peer,error))return false;
        if(!peer)return true;
        if(!services.handle_character) {error="Required original owner handle-to-Character contact conversion";return false;}
        std::uintptr_t character=0;
        const auto debug=[&]() {
            bool tracing=false;
            if(!services.debug_load||!services.debug_switch) {error="Required original POCharacter contact Debug prefix";return false;}
            if(!services.debug_load(error)||!services.debug_switch("isTracingPlayersCollision",tracing,error))return false;
            if(tracing) {bool ignored=false;if(!services.is_player) {error="Required original tracing IsPlayer query";return false;}if(!services.is_player(owner,ignored,error))return false;}
            return true;
        };
        if(event==dh2::physical::ContactEvent::persist) {
            if(!services.handle_character(owner,character,error)||!debug())return false;
        } else if(event==dh2::physical::ContactEvent::add||event==dh2::physical::ContactEvent::remove) {
            if(!debug()||!services.handle_character(owner,character,error))return false;
        } else {error="Unknown original physical contact event";return false;}
        if(!character)return true;
        if(!services.ai_events) {error="Required SAME Character contact CharAI/AIS/FSM event authority";return false;}
        dh2::character::AIEventState64* state=nullptr;dh2::character::AIEventServices24 delivery{};
        if(!services.ai_events(character,state,delivery,error))return false;
        if(!state||!state->owner||state->owner->owner!=character) {error="Contact CharAI receiver belongs to another Character";return false;}
        const std::uint32_t base=event==dh2::physical::ContactEvent::add?0x37:event==dh2::physical::ContactEvent::persist?0x39:0x3b;
        const dh2::character::AIEventPayload24 payload{peer,0,0,0};dh2::character::AIEventResult16 result{};
        const auto status=dh2_character_ai_event(&result,state,base+(persist?0:1),&payload,&delivery);
        if(status) {if(error.empty())error="Original Character contact RaiseAIEvent service failed: "+std::to_string(status);return false;}
        return true;
    };
}
namespace {
bool current_plan_definition(const OriginalActorBodyPlan& plan,ActorId id,const float* position,bool group_override,dh2::physical::CharacterBodyConfig& output,std::string& error) {
    dh2::physical::CharacterBodyInput input{};input.owner=reinterpret_cast<void*>(id);input.new_physical=&input;input.character_type=plan.character_type;input.is_player=plan.is_player;input.special_owner_byte=plan.physical_enabled&&!plan.circular;input.collision_group_override=group_override;
    input.absolute_bounds[0]=plan.bounds.absolute_box[0];input.absolute_bounds[1]=plan.bounds.absolute_box[1];input.absolute_bounds[2]=plan.bounds.absolute_box[3];input.absolute_bounds[3]=plan.bounds.absolute_box[4];input.position[0]=position[0];input.position[1]=position[1];
    if(dh2_character_body_config(&output,&input)) {error="Rebased source body definition rejected actual bounds/identity";return false;}
    return true;
}
}
struct PlayableActorBodies::Entry {
    ActorState* actor{};
    const OriginalCombatProperties* properties{};
    dh2::navigation::NavigationObject pf{};
    OriginalActorNavigationWorldBindings navigation_world{};
    bool navigation_world_attached=false;
    std::shared_ptr<OriginalActorNavigation> navigation;
    std::shared_ptr<OriginalActorPhysical> physical;
    dh2::physical::NativeWorld* world{};
};
PlayableActorBodies::PlayableActorBodies()=default;
PlayableActorBodies::~PlayableActorBodies(){std::string error;if(!clear(error))std::terminate();}
bool PlayableActorBodies::bind(ActorState& actor,const OriginalCombatProperties& properties,const OriginalActorBodyPlan& plan,OriginalActorPhysicalBindings services,std::string& error) {
    if(stepping_){error="Cannot bind actor bodies during physical Step delivery";return false;}
    if(!actor.id||entries_.count(actor.id)) {error="Playable body needs a fresh stable actor identity";return false;}
    if((services.identity&&services.identity!=actor.id)||(services.position160&&services.position160!=actor.transform.position.data())||services.properties||(services.readonly_properties&&services.readonly_properties!=&properties.sheets)) {error="Playable body fields must borrow SAME actor/property authority";return false;}
    if(!services.actor_lease||!services.world_lease||!services.data_lease||!services.world||!services.ai||!services.destination1a8||!services.attached2e0||!services.visual2d8||!services.static84||!services.is_player||!services.debug_switch||!services.filter||!services.contact) {error="Playable body requires actual world/actor/data/source slot/debug/filter/contact providers";return false;}
    auto ai_id=properties.sheets.resolved[1];if(ai_id<0||static_cast<std::size_t>(ai_id)>=services.ai->rows.size())ai_id=8;
    if(services.ai->rows.size()<=8||ai_id!=plan.ai_id||services.ai->rows[ai_id].type!=plan.character_type) {error="Body plan AI/type differs from SAME current resolved properties";return false;}
    auto entry=std::make_shared<Entry>();entry->actor=&actor;entry->properties=&properties;entry->world=services.world;
    services.identity=actor.id;services.position160=actor.transform.position.data();services.readonly_properties=&properties.sheets;
    const auto no_collisions=std::make_shared<bool>(false);const auto debug=services.debug_switch;
    services.debug_switch=[debug,no_collisions](const char* key,bool& out,std::string& e){if(!debug(key,out,e))return false;if(std::strcmp(key,"MP_NoCollisions")==0)*no_collisions=out;return true;};
    // PF constructor owns the sole new PFObject. InitPhysical occurs BEFORE
    // InitFinal; its actual source callback observes user==0 and returns.
    OriginalActorNavigationBindings nav;nav.actor_lease=services.actor_lease;nav.world_lease=services.world_lease;nav.identity=actor.id;nav.position160=actor.transform.position.data();nav.object=&entry->pf;nav.static84=services.static84;
    entry->navigation=std::make_shared<OriginalActorNavigation>(nav);
    if(!entry->navigation->construct_defaults(error))return false;
    services.update_pf=[navigation=entry->navigation](auto id,const auto& body,const auto& config,auto& e){return navigation->update_pf(id,body,config,e);};
    entry->physical=std::make_shared<OriginalActorPhysical>(std::move(services));
    if(!entry->physical->bind_bounds(plan.bounds,error))return false;
    // CreateShape/SetMass may synchronously filter overlapping bodies. Loan
    // the actual stable receiver identity before allocation, in this same
    // registry, so peers can resolve it without casts or a second owner map.
    // Keep the loan through failure cleanup; remove only after release succeeds.
    entries_.emplace(actor.id,entry);
    const auto rollback=[&](const std::string& reason){
        const auto original=reason;std::string cleanup;
        if(!entry->physical->release(cleanup)){
            error=original+"; body release failed: "+cleanup;return false;
        }
        entries_.erase(actor.id);error=original;return false;
    };
    if(!entry->physical->initialize(error))return rollback(error);
    // Reject plan/body divergence before publication; all filter/circular/type
    // data was independently reconstructed using current immutable properties.
    const auto& actual=entry->physical->definition();
    dh2::physical::CharacterBodyConfig expected{};
    if(!current_plan_definition(plan,actor.id,actor.transform.position.data(),*no_collisions,expected,error))return rollback(error);
    if(bool(actual.enabled)!=plan.physical_enabled || (actual.enabled&&(actual.shape.group_index!=(*no_collisions?-666:plan.group_index)||actual.shape.category_bits!=plan.category_bits||actual.shape.mask_bits!=plan.mask_bits||bool(actual.shape.kind==0)!=plan.circular||actual.radius!=expected.radius))) {
        std::ostringstream detail;detail<<std::setprecision(9)<<"Actual body source properties/debug policy differs from supplied body plan: enabled="<<actual.enabled<<"/"<<plan.physical_enabled<<", group="<<actual.shape.group_index<<"/"<<(*no_collisions?-666:plan.group_index)<<", category="<<actual.shape.category_bits<<"/"<<plan.category_bits<<", mask="<<actual.shape.mask_bits<<"/"<<plan.mask_bits<<", circle="<<(actual.shape.kind==0)<<"/"<<plan.circular<<", radiusPhysics="<<actual.radius<<"/currentExpected="<<expected.radius<<", oldPlanGame="<<plan.radius_game_units;
        return rollback(detail.str());
    }
    if(entry->pf.user!=0)return rollback("Fresh InitPhysical PF callback escaped original constructor-user0 policy");
    error.clear();return true;
}
bool PlayableActorBodies::set_position(ActorId id,const std::array<float,3>& position,bool destination,std::string& error) {
    const auto found=entries_.find(id);if(found==entries_.end()||found->second->actor->id!=id) {error="Required same registered live actor body";return false;}
    return found->second->physical->set_position(position,destination,error);
}
bool PlayableActorBodies::step_world(dh2::physical::NativeWorld& world,std::uint64_t frame,
    std::uint32_t dt,const CurrentActorLookup& lookup,bool& stepped,std::string& error){
    stepped=false;error.clear();
    if(stepping_||!frame||!lookup||!world.backend()||!world.cleanup_delivery_idle_v106()){
        error="Physical Step requires current frame, registry and idle loaded world";return false;
    }
    for(const auto& pair:entries_){const auto* current=lookup(pair.first);
        if(pair.second->world!=&world||current!=pair.second->actor||!current||current->id!=pair.first){
            error="Physical Step actor/world differs from current registered owner";return false;
        }
    }
    if(last_step_world_){
        if(last_step_world_!=&world||frame<last_step_frame_||(frame==last_step_frame_&&dt!=last_step_dt_)){
            error="Physical Step frame/world is stale or inconsistent";return false;
        }
        if(frame==last_step_frame_){error=last_step_error_;return last_step_ok_;}
    }
    // Commit the occurrence before reached contact callbacks. A failed callback
    // cannot replay the same world's already-delivered Begin/Persist prefix.
    last_step_world_=&world;last_step_frame_=frame;last_step_dt_=dt;
    last_step_ok_=false;last_step_error_="Physical Step did not complete";
    struct Scope{bool& value;~Scope(){value=false;}} scope{stepping_};stepping_=true;stepped=true;
    try{world.update(dt);last_step_ok_=true;last_step_error_.clear();return true;}
    catch(const std::exception& failure){error=failure.what();last_step_error_=error;return false;}
    catch(...){error="Physical Step consumer threw";last_step_error_=error;return false;}
}
bool PlayableActorBodies::reconcile_physics_position(ActorId id,const CurrentActorLookup& lookup,
    bool validating_floor,PhysicsPositionResult& output,std::string& error){
    error.clear();const auto found=entries_.find(id);
    if(stepping_||!lookup||found==entries_.end()){
        error="Physics position publication requires current registered actor outside Step";return false;
    }
    auto& entry=*found->second;const auto* current=lookup(id);
    if(current!=entry.actor||!current||current->id!=id||!entry.world->cleanup_delivery_idle_v106()){
        error="Physics position publication owner/world is stale or delivering callbacks";return false;
    }
    PhysicsPositionResult result;result.position=entry.actor->transform.position;
    const auto& native=entry.physical->native();
    if(!native.body){output=result;return true;}
    dh2::physical::NativeBodyObservation observed{};
    if(dh2_native_body_observe(&observed,&native)){error="Current native body observation failed";return false;}
    const float x=observed.position[0]*100.f,y=observed.position[1]*100.f;
    if(!std::isfinite(x)||!std::isfinite(y)){error="Native body position is nonfinite";return false;}
    result.body_present=true;result.sleeping=observed.sleeping!=0;
    if(!result.sleeping&&(std::fabs(x-result.position[0])>1.f||std::fabs(y-result.position[1])>1.f)){
        result.position[0]=x;result.position[1]=y;result.xy_changed=true;
        if(validating_floor){
            OriginalActorNavigationMoveResult admission;
            if(!entry.navigation_world_attached||!entry.navigation->validate_position(
                {x,y,result.position[2]},admission,error)){
                if(error.empty())error="Native position import requires actual initialized floor/PF world";return false;
            }
            result.floor_checked=true;result.floor_valid=admission.position_valid;
            result.position={admission.position.x,admission.position.y,admission.position.z};
        }
    }
    if(!entry.physical->set_position(result.position,false,error))return false;
    result.body_reseated=true;output=result;return true;
}
bool PlayableActorBodies::resolve_physical_actor(void* context,ActorId& output,std::string& error)const{
    if(!context){output=invalid_actor_id;error.clear();return true;}
    for(const auto& pair:entries_)if(context==pair.second->physical.get()){
        output=pair.first;error.clear();return true;
    }
    error="Physical peer context is not registered in this actor body pool";return false;
}
bool PlayableActorBodies::release(ActorId id,std::string& error) {
    if(stepping_){error="Cannot release actor bodies during physical Step delivery";return false;}
    const auto found=entries_.find(id);if(found==entries_.end()){error="Actor body identity not registered";return false;}
    if(!found->second->physical->release(error))return false;
    if(!found->second->navigation->update_pf(id,found->second->physical->native(),found->second->physical->definition(),error))return false;
    entries_.erase(found);error.clear();return true;
}
bool PlayableActorBodies::remove_physical(ActorId id,std::string& error) {
    if(stepping_){error="Cannot remove actor physical receivers during Step delivery";return false;}
    const auto found=entries_.find(id);if(found==entries_.end()){error="Actor body identity not registered";return false;}
    if(!found->second->physical->release(error))return false;
    return found->second->navigation->update_pf(id,found->second->physical->native(),found->second->physical->definition(),error);
}
bool PlayableActorBodies::initialize_physical(ActorId id,std::string& error) {
    if(stepping_){error="Cannot initialize actor physical receivers during Step delivery";return false;}
    const auto found=entries_.find(id);if(found==entries_.end()){error="Actor body identity not registered";return false;}
    return found->second->physical->initialize(error);
}
bool PlayableActorBodies::set_physical_filter_enabled(ActorId id,bool enabled,std::string& error) {
    if(stepping_){error="Cannot change actor physical filters during Step delivery";return false;}
    const auto found=entries_.find(id);if(found==entries_.end()){error="Actor body identity not registered";return false;}
    return found->second->physical->set_filter_enabled(enabled,error);
}
bool PlayableActorBodies::set_source_physical_filter(ActorId id,const CurrentActorLookup& lookup,
    std::int16_t group,std::uint16_t category,std::uint16_t mask,bool apply_secondary,
    std::string& error) {
    if(stepping_){error="Cannot change actor physical filters during Step delivery";return false;}
    const auto found=entries_.find(id);
    if(found==entries_.end()||!lookup){error="Source physical filter requires a registered current actor";return false;}
    const auto& entry=*found->second;const auto* actor=lookup(id);
    if(!actor||actor!=entry.actor||actor->id!=id){error="Source physical filter actor differs from current registered owner";return false;}
    if(!entry.physical||!entry.physical->native().body||
       entry.physical->actor_identity()!=id){
        error="Source physical filter requires the SAME assigned actor body";return false;
    }
    if(!entry.world||!entry.world->cleanup_delivery_idle_v106()){
        error="Source physical filter requires idle current world delivery";return false;
    }
    return entry.physical->set_source_filter(group,category,mask,apply_secondary,error);
}
bool PlayableActorBodies::reset_source_physical_filter(ActorId id,const CurrentActorLookup& lookup,
    std::string& error) {
    if(stepping_){error="Cannot change actor physical filters during Step delivery";return false;}
    const auto found=entries_.find(id);
    if(found==entries_.end()||!lookup){error="Source physical filter reset requires a registered current actor";return false;}
    const auto& entry=*found->second;const auto* actor=lookup(id);
    if(!actor||actor!=entry.actor||actor->id!=id){error="Source physical filter reset actor differs from current registered owner";return false;}
    if(!entry.physical||!entry.physical->native().body||
       entry.physical->actor_identity()!=id){
        error="Source physical filter reset requires the SAME assigned actor body";return false;
    }
    if(!entry.world||!entry.world->cleanup_delivery_idle_v106()){
        error="Source physical filter reset requires idle current world delivery";return false;
    }
    return entry.physical->reset_source_filter(error);
}
bool PlayableActorBodies::set_pinned(ActorId id,bool pinned,std::string& error) {
    if(stepping_){error="Cannot change actor body mass during Step delivery";return false;}
    const auto found=entries_.find(id);if(found==entries_.end()){error="Actor body identity not registered";return false;}
    return found->second->physical->set_pinned(pinned,error);
}
bool PlayableActorBodies::stop_physical(ActorId id,const CurrentActorLookup& lookup,
    bool& stopped,std::string& error) {
    stopped=false;
    if(stepping_){error="Cannot Stop actor physical receivers during physical Step delivery";return false;}
    const auto found=entries_.find(id);
    if(found==entries_.end()||!lookup){error="Physical Stop requires a registered current actor";return false;}
    const auto entry=found->second;
    const auto* actor=lookup(id);
    if(!actor||actor!=entry->actor||actor->id!=id){error="Physical Stop actor differs from current registered owner";return false;}
    if(!entry->world||!entry->world->cleanup_delivery_idle_v106()){
        error="Physical Stop requires idle current world delivery";return false;
    }
    OriginalActorSubobjectsBorrow borrow;
    if(!entry->physical->subobjects_borrow(borrow,error))return false;
    if(!borrow.native||!borrow.native->body){error.clear();return true;}
    // The existing source kernel preserves physical angle/mass/pin and performs
    // linear0 -> angular0 -> current gameXY SetXForm -> PutToSleep. No gameplay,
    // PF path, destination or heading field is replaced here.
    try{
        if(dh2_native_body_stop(borrow.native,actor->transform.position.data())!=0){
            error="Source physical Stop rejected the current actor position";return false;
        }
        stopped=true;error.clear();return true;
    }catch(const std::exception& failure){error=failure.what();return false;}
}
bool PlayableActorBodies::initialize_navigation(ActorId id,OriginalActorNavigationWorldBindings world,std::string& error) {
    const auto found=entries_.find(id);if(found==entries_.end()){error="Navigation actor body identity not registered";return false;}
    auto& entry=*found->second;OriginalTriggerActorBorrow actor;
    if(!entry.physical->actor_borrow(actor,error)||!entry.navigation->attach_world(world,entry.physical->relative_bounds(),actor.absolute12c,error)||!entry.navigation->initialize_pf(error))return false;
    if(!entry.navigation->update_pf(id,entry.physical->native(),entry.physical->definition(),error))return false;
    entry.navigation_world=std::move(world);entry.navigation_world_attached=true;error.clear();return true;
}
bool PlayableActorBodies::move_grounded(ActorId id,Vec3 current,Vec3 delta,OriginalActorNavigationMoveResult& output,std::string& error) {
    const auto found=entries_.find(id);if(found==entries_.end()){error="Grounded motion actor body identity not registered";return false;}
    auto& entry=*found->second;const float position[]{current.x,current.y,current.z};
    for(unsigned k=0;k<3;++k)if(position[k]!=entry.actor->transform.position[k]){error="Grounded motion must borrow SAME admitted ActorState position";return false;}
    // Original ValidateDirection belongs to UpdatePath's heading coordinator;
    // its bool is diagnostic, and it never vetoes authored root displacement.
    // RootSceneNode displacement reaches the independent position producer.
    const Vec3 requested{current.x+delta.x,current.y+delta.y,current.z+delta.z};
    OriginalActorNavigationMoveResult result;if(!entry.navigation->validate_position(requested,result,error))return false;
    result.direction_checked=false;result.direction_allowed=false;
    // Original position producer mutates point on rejection as well (cached PF
    // position for kind3). Its bool is evidence, not a transaction rollback.
    if(!entry.physical->set_position({result.position.x,result.position.y,result.position.z},false,error))return false;
    output=result;error.clear();return true;
}
bool PlayableActorBodies::update_path(ActorId id,OriginalActorPathBindings& source,dh2::navigation::ControllerResult& result,std::string& error) {
 const auto found=entries_.find(id);if(found==entries_.end()){error="Source path actor body identity not registered";return false;}
 return found->second->navigation->update_path(source,result,error);
}
bool PlayableActorBodies::update_manual_heading(ActorId id,const OriginalManualHeadingBindings& source,OriginalManualHeadingResult& result,std::string& error) {
 const auto found=entries_.find(id);if(found==entries_.end()){error="Manual heading actor body identity not registered";return false;}
 return found->second->navigation->update_manual_heading(source,result,error);
}
bool PlayableActorBodies::clear(std::string& error) {
    if(stepping_){error="Cannot clear actor body pool during Step delivery";return false;}
    while(!entries_.empty())if(!release(entries_.begin()->first,error))return false;
    last_step_world_=nullptr;last_step_frame_=0;last_step_dt_=0;
    last_step_ok_=true;last_step_error_.clear();
    error.clear();return true;
}
bool PlayableActorBodies::rebase_plan(OriginalActorBodyPlan& plan,ActorId id,const std::array<float,3>& position,std::string& error) {
    auto next=plan;if(!OriginalActorBounds::update_absolute(next.bounds,{position[0],position[1],position[2]},error))return false;
    dh2::physical::CharacterBodyConfig definition;if(!current_plan_definition(next,id,position.data(),false,definition,error))return false;
    if(bool(definition.enabled)!=next.physical_enabled||bool(definition.enabled&&definition.shape.kind==0)!=next.circular||definition.shape.group_index!=next.group_index||definition.shape.category_bits!=next.category_bits||definition.shape.mask_bits!=next.mask_bits) {error="Rebased source plan has inconsistent type/filter metadata";return false;}
    next.radius_game_units=next.circular?definition.radius*100.f:0.f;plan=std::move(next);error.clear();return true;
}
bool PlayableActorBodies::actor_borrow(std::uintptr_t id,OriginalTriggerActorBorrow& out,std::string& error)const {
    const auto found=entries_.find(id);if(found==entries_.end()){error="Actor body query identity not registered";return false;}
    return found->second->physical->actor_borrow(out,error);
}
bool PlayableActorBodies::physical_borrow(std::uintptr_t id,OriginalTriggerPhysicalBorrow& out,std::string& error)const {
    for(const auto& entry:entries_)if(id==reinterpret_cast<std::uintptr_t>(entry.second->physical.get()))return entry.second->physical->physical_borrow(id,out,error);
    error="Physical query receiver not registered";return false;
}
const OriginalActorPhysical* PlayableActorBodies::physical(ActorId id)const noexcept {const auto found=entries_.find(id);return found==entries_.end()?nullptr:found->second->physical.get();}
bool PlayableActorBodies::subobjects_borrow(ActorId id,OriginalActorSubobjectsBorrow& out,std::string& error){
 const auto found=entries_.find(id);if(found==entries_.end()||found->second->actor->id!=id){error="Subobjects actor body identity is not registered";return false;}
 return found->second->physical->subobjects_borrow(out,error);
}
const dh2::navigation::NavigationObject* PlayableActorBodies::navigation(ActorId id)const noexcept {const auto found=entries_.find(id);return found==entries_.end()?nullptr:&found->second->pf;}
bool PlayableActorBodies::navigation_world_binding(ActorId id,OriginalActorNavigationWorldBindings& output,std::string& error)const {
    const auto found=entries_.find(id);
    if(found==entries_.end()||!found->second->navigation_world_attached) {error="Actor PFObject has no retained initialized floor/obstacle binding";return false;}
    const auto& binding=found->second->navigation_world;
    if(!binding.floor_lease||!binding.obstacle_lease||!binding.floors||!binding.floors->sewn||!binding.obstacles) {error="Retained actor navigation geometry binding is incomplete";return false;}
    output=binding;error.clear();return true;
}
}
