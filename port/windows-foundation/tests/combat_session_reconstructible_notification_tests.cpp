#include "../combat_session.hpp"
#include "../game_save.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
struct Trace {unsigned notifications=0,end_events=0;};
CombatSessionAnimationNotificationServices services_for(CombatSession& session,Trace& trace){
    CombatSessionAnimationNotificationServices services;
    services.notification=[&trace](ActorId,const dh2::character::AnimationEventRequest& request,
                                   std::int32_t& result,std::string&){
        ++trace.notifications;if(request.event==0x22u)++trace.end_events;result=0;return true;
    };
    services.state_event=[&session,&trace](ActorId actor,std::uint32_t event,
                                           const RetainedAnimationEvent*,std::string&){
        if(event==0x22u){
            ++trace.end_events;
            auto* live=session.actor(actor);
            if(!live||live->action!=CharacterAction::dead){return false;}
            // Model the already-integrated host's source End34 body-removal
            // witness. Native receiver ownership is separately main-tested.
            live->source_physical_present=false;
        }
        return true;
    };
    return services;
}
}

int main(int argc,char** argv){try{
    check(argc==3,"Supply original shared assets and private save path");
    AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase database;OriginalMeleeBindings melee;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(melee.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan player_plan;
    check(build_original_combat_visual_plan(assets,melee,"KnightPlayerBase",customization,
          "reconstructible-notification-player",player_plan,error),error);
    auto player_visual=player_plan.config;player_visual.motion_node_id="auto";
    player_visual.consume_root_motion=true;

    CombatSessionConfig config;config.diagnosticRngSeed=17;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=player_visual;
    CombatSessionProfile player;player.action={"AttackStatic",0,{0,1}};player.initialIdle={"Idle",0,{0}};
    player.damageMarkerNames={"attack_mainhand"};player.customization=customization;player.propertyOptions={256,true};
    config.profiles.emplace(config.playerProfileId,player);
    CombatSessionProfile lizard;lizard.action={"Attack",0,{0,1}};lizard.initialIdle={"Idle",0,{0}};
    lizard.reaction=CombatSessionChoice{"Injured",0,{0}};lizard.death=CombatSessionChoice{"Died",0,{0}};
    lizard.damageMarkerNames={"attack_mainhand"};lizard.customization=customization;
    lizard.motionRoot="auto";lizard.propertyOptions={std::nullopt,true};lizard.retainedPhaseClock=true;
    OriginalAttackSelection attack;attack.state="Attack";attack.group_path={0};lizard.sequenceAction=attack;
    config.profiles.emplace("Swamp_LizadMan_Type1",lizard);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="reconstructible-notification-lizard";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,17,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual rendered_player;CombatSession session;
    check(session.initialize(assets,database,melee,config,rendered_player,population,
          {0,0,0},customization,error),error);

    Trace legacy_trace;auto legacy=services_for(session,legacy_trace);
    session.set_animation_notification_services(legacy);
    check(!session.validate_lifecycle_checkpoint(error),
          "Legacy notification setter incorrectly marked callbacks reconstructible");
    session.clear_animation_notification_services();

    auto* player_actor=session.actor(1);auto* lizard_actor=session.actor(2);
    check(player_actor&&lizard_actor,"Same-session actors unavailable");
    player_actor->persistent_character_id="reconstructible-notification-character";
    player_actor->source_physical_present=true;lizard_actor->source_physical_present=true;
    bool dirty=true;unsigned validations=0;
    const auto make_validator=[&](){
        const auto captured_lease=session.actor_binding_lease();
        return [&,captured_lease](std::string& problem){
            ++validations;
            if(dirty){problem="fixture physical-presence witness is dirty";return false;}
            const auto current_lease=session.actor_binding_lease();
            const auto old_owner=captured_lease.lock();const auto current_owner=current_lease.lock();
            if(!old_owner||!current_owner||old_owner.owner_before(current_owner)||
               current_owner.owner_before(old_owner)){
                problem="fixture validator is attached to a stale actor lease";return false;
            }
            const auto* current_player=session.actor(1);const auto* dead=session.actor(2);
            if(!current_player||!current_player->source_physical_present||
               !dead||!dead->source_physical_present){problem="fixture actor body-presence fact is unknown";return false;}
            problem.clear();return true;
        };
    };
    Trace rejected_trace;
    check(!session.bind_reconstructible_animation_notifications(
              services_for(session,rejected_trace),make_validator(),error)&&
          error.find("dirty")!=std::string::npos,
          "Dirty presence-fact validator admitted reconstructible callback binding");
    check(session.validate_lifecycle_checkpoint(error),
          "Rejected typed bind left notification callbacks installed or the Session dirty");
    dirty=false;Trace trace;
    check(session.bind_reconstructible_animation_notifications(
              services_for(session,trace),make_validator(),error),error);
    check(session.validate_lifecycle_checkpoint(error),error);
    session.set_animation_notification_services(services_for(session,legacy_trace));
    check(!session.validate_lifecycle_checkpoint(error),
          "Legacy setter inherited a prior reconstructible checkpoint grant");
    check(session.bind_reconstructible_animation_notifications(
              services_for(session,trace),make_validator(),error),error);
    dirty=true;
    check(!session.validate_lifecycle_checkpoint(error)&&error.find("dirty")!=std::string::npos,
          "Changed physical-presence witness was not revalidated before checkpoint");
    Trace rejected_rebind_trace;
    check(!session.bind_reconstructible_animation_notifications(
              services_for(session,rejected_rebind_trace),make_validator(),error)&&
          error.find("dirty")!=std::string::npos,
          "Dirty replacement validator unexpectedly replaced the valid typed consumer");
    bool rejected_teardown_entered=false;
    check(!session.detach_for_restore([&](std::string&){rejected_teardown_entered=true;return true;},error)&&
          !rejected_teardown_entered&&error.find("dirty")!=std::string::npos,
          "Invalid checkpoint witness reached external teardown");
    dirty=false;
    check(session.validate_lifecycle_checkpoint(error),error);
    Trace throwing_rebind_trace;
    check(!session.bind_reconstructible_animation_notifications(
              services_for(session,throwing_rebind_trace),
              [](std::string&)->bool{throw std::runtime_error("fixture validator exception");},error)&&
          error.find("fixture validator exception")!=std::string::npos&&
          session.validate_lifecycle_checkpoint(error),
          "Throwing replacement validator did not preserve the previous valid typed consumer");

    // Genuine source lethal result: End34 only after the retained Died clip's
    // authored completion, then the typed callback updates the persisted fact.
    OriginalMeleeDamageProvider formula;
    check(formula.bind_actor(1,*session.world()->combat_properties(1),error)&&
          formula.bind_actor(2,*session.world()->combat_properties(2),error),error);
    auto* dead=session.actor(2);check(dead&&dead->alive(),"Actual source Lizard did not start alive");
    CombatSessionSourceHit hit;hit.attacker=1;hit.target=2;hit.binding_lease=session.actor_binding_lease();
    hit.source_id="typed-notification-lethal";hit.marker_name="death_direct";hit.mask=0x20080000u;
    hit.category=-1;hit.element=-1;hit.direct_amount=static_cast<std::int32_t>(std::ceil(dead->health*256.0f));
    hit.generation=1;DamageEvent receipt;
    check(session.apply_source_result(hit,receipt,error)&&receipt.target_died&&dead->action==CharacterAction::dead,error);
    const auto* pose=session.retained_actor_pose(2);check(pose,"Retained death pose unavailable");
    const auto clip=pose->slots()[pose->current_slot()].clip_id;
    const auto start=pose->slots()[pose->current_slot()].timeline.start_ms;
    const auto end=pose->slots()[pose->current_slot()].timeline.end_ms;
    const auto rate=pose->slots()[pose->current_slot()].timeline.scale;
    check(clip.find("/Died/")!=std::string::npos&&end>start&&rate>0&&!pose->current_ended(),
          "Actual Died source clip was not retained for completion");
    InputActions input;double wall=0;bool ended=false;
    const double limit=double(end-start)/1000.0/rate+0.5;
    for(unsigned frame=0;frame<1000&&wall<limit;++frame){
        check(session.update(0.016,input,{0,0,0},0,error),error);wall+=0.016;
        if(session.retained_actor_pose(2)->current_ended()){ended=true;break;}
    }
    check(ended&&trace.end_events==2&&dead->source_physical_present==std::optional<bool>(false),
          "Typed callback did not deliver exactly one source End34 and publish body absence");
    check(session.validate_lifecycle_checkpoint(error),error);

    CharacterState character=make_default_character("reconstructible-notification-character","Test","warrior");
    const std::filesystem::path save_path(argv[2]);GameSave captured;
    check(capture_game_save("original/notification-checkpoint",1,character,*session.world(),captured,error),error);
    check(captured.version==3&&captured.actors[1].actor.source_physical_present==std::optional<bool>(false),
          "Typed End34 body-absence witness did not produce a v3 checkpoint");
    check(save_game(save_path,captured,error),error);GameSave loaded;
    check(load_game(save_path,loaded,error),error);
    const auto ends_before_detach=trace.end_events;
    const auto old_lease=session.actor_binding_lease();
    const auto serial_before_teardown=session.update_serial();
    const auto validations_before_teardown=validations;
    bool failed_teardown_called=false;
    std::string nested_error;
    check(!session.detach_for_restore([&](std::string& teardown_error){
        failed_teardown_called=true;
        check(validations==validations_before_teardown+1,
              "Typed checkpoint validator did not run before external teardown");
        check(!old_lease.expired(),"Old actor lease expired before external teardown completed");
        const auto nested_serial=session.update_serial();
        check(!session.update(0.016,input,{0,0,0},0,nested_error)&&
              session.update_serial()==nested_serial,
              "Nested update was admitted during external restore teardown");
        check(!session.validate_lifecycle_checkpoint(nested_error),
              "Nested checkpoint validation was admitted during external restore teardown");
        check(!session.detach_for_restore([](std::string&){return true;},nested_error),
              "Nested detach was admitted during external restore teardown");
        check(!session.bind_reconstructible_animation_notifications(
                  services_for(session,trace),make_validator(),nested_error),
              "Nested typed binding was admitted during external restore teardown");
        std::int32_t nested_result=0;
        check(!session.deliver_original_animation_notification(1,{},nested_result,nested_error),
              "Nested animation notification was admitted during external restore teardown");
        bool replacement_threw=false;
        try{session.set_animation_notification_services(services_for(session,trace));}
        catch(const std::logic_error&){replacement_threw=true;}
        check(replacement_threw,"Nested legacy callback replacement was admitted during teardown");
        bool clearing_threw=false;
        try{session.clear_animation_notification_services();}
        catch(const std::logic_error&){clearing_threw=true;}
        check(clearing_threw,"Nested callback clearing was admitted during teardown");
        const CombatSession::MotionPhaseHandler phase=[](ActorState&,std::uint64_t,std::uint32_t,
                const std::vector<CombatSessionMotionSample>&,std::string&){return true;};
        check(!session.set_motion_phase_handler(phase,nested_error)&&
              !session.clear_motion_phase_handler(nested_error),
              "Motion phase mutation was admitted during external teardown");
        teardown_error="fixture external retirement failed after reaching its prefix";
        return false;
    },error)&&failed_teardown_called&&
          error.find("reaching its prefix")!=std::string::npos,
          "Failed external teardown did not preserve its failure prefix (called="+
          std::to_string(failed_teardown_called)+", error="+error+")");
    check(!old_lease.expired()&&
          session.update_serial()==serial_before_teardown&&
          session.validate_lifecycle_checkpoint(error),
          "Failed external teardown detached the Session, retired its lease, or left it unusable");
    check(session.actor(2)->source_physical_present==std::optional<bool>(false),
          "Failed external teardown rolled back the already-reached body-presence prefix");

    const auto validations_before_success=validations;
    bool successful_teardown_called=false;
    check(session.detach_for_restore([&](std::string& teardown_error){
        successful_teardown_called=true;
        check(validations==validations_before_success+1,
              "Successful teardown did not revalidate before retiring external state");
        check(!old_lease.expired(),
              "Successful teardown retired Session ownership before callback completion");
        check(trace.end_events==ends_before_detach,
              "Successful teardown replayed a source End34 notification");
        teardown_error.clear();return true;
    },error),error);
    check(successful_teardown_called&&old_lease.expired(),
          "Successful teardown did not retire the old Session lease after callback completion");
    check(restore_game_save(loaded,"original/notification-checkpoint",*session.world(),character,error),error);
    check(session.rebind_after_restore(error),error);
    check(session.actor(2)->source_physical_present==std::optional<bool>(false),
          "Restored Session lost the saved absent-body fact");
    check(!session.update(0.016,input,{0,0,0},0,error),
          "Restored Session updated with a dropped reconstructible notification callback");
    check(trace.end_events==ends_before_detach,"Restore replayed source End34 without a fresh callback");
    session.clear_animation_notification_services();
    check(!session.update(0.016,input,{0,0,0},0,error),
          "Clearing callbacks incorrectly removed the restore fresh-binding requirement");
    Trace legacy_after_restore;
    session.set_animation_notification_services(services_for(session,legacy_after_restore));
    check(!session.update(0.016,input,{0,0,0},0,error),
          "Legacy callback setter incorrectly removed the restore fresh-binding requirement");
    session.clear_animation_notification_services();
    auto fresh_lease=session.actor_binding_lease();
    check(!fresh_lease.expired(),"Restored Session did not publish a fresh actor binding lease");
    dirty=false;
    check(session.bind_reconstructible_animation_notifications(
              services_for(session,trace),make_validator(),error),error);
    check(old_lease.expired(),"Old actor lease became valid after fresh bind");
    for(unsigned frame=0;frame<120;++frame)check(session.update(0.016,input,{0,0,0},0,error),error);
    check(trace.end_events==ends_before_detach&&
          session.actor(2)->source_physical_present==std::optional<bool>(false),
          "Restored terminal corpse replayed End34 or regained body presence");
    check(validations>=7,"Checkpoint validator was not re-run across binding/save/failed+successful detach/restore");

    std::cout<<"PASS legacy callbacks remain volatile; typed End34 binding validates presence witness, v3 saves false body fact, detach/rebind drops callback, fresh lease resumes without End34 replay\n";
    return 0;
}catch(const std::exception& exception){std::cerr<<"FAIL: "<<exception.what()<<'\n';return 1;}}
