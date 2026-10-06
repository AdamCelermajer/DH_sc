#include "retained_character_kill_borrow_v42.hpp"
namespace dh2::character {
bool retained_character_kill_borrow_v42(RetainedCharacterActorV1& actor,
 std::shared_ptr<void> receiver,data::AggroTable& outgoing,
 CharacterKillLiveBorrowV21& out,std::string& error){
 if(!receiver||!actor.object||!actor.object->identity||!actor.object->properties||
    !actor.object->life||!actor.session||!actor.controller||!actor.machine||
    !actor.kill_fields_v42().produced){
  error="Required SAME retained Character/session/controller and produced C1 or observed Kill fields";return false;
 }
 auto& object=*actor.object;auto& properties=actor.session->property_view();
 if(properties.resolved!=object.properties->resolved.data()||
    properties.saved!=object.properties->saved.data()||
    properties.base!=object.properties->base.data()||
    properties.gear!=object.properties->gear.data()||
    actor.machine->native_fsm().character!=object.identity||
    actor.shared_handle().cached!=object.identity||object.life->dead>1||
    outgoing.count>outgoing.capacity||(outgoing.count&&!outgoing.entries)){
  error="Required identical retained Character sheets/life/FSM/Handle/Aggro before Kill publication";return false;
 }
 const std::int32_t* oid{};const std::int16_t* property{};std::uintptr_t* tracked{};
 actor.kill_metadata_borrow_v23(oid,property,tracked);
 CharacterKillLiveBorrowV21 borrow;borrow.identity=object.identity;
 borrow.controller=actor.controller->identity();borrow.controllable_character=object.identity;
 borrow.receiver=std::move(receiver);borrow.properties=&properties;borrow.life=object.life.get();
 borrow.fields=&actor.kill_fields_v42();borrow.oid64=oid;borrow.property13c8=property;
 borrow.tracked14a4=tracked;borrow.outgoing=&outgoing;borrow.shared_handle=&actor.shared_handle();
 if(!borrow.controller){error="Required actual retained NPC controller identity";return false;}
 out=std::move(borrow);return true;
}
}
