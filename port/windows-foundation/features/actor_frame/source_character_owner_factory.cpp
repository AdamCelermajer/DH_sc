#include "source_character_owner_factory.hpp"
#include <utility>

namespace dh::foundation::features {
bool SourceCharacterOwnerAliases::validate(std::string& e)const{
    if(!lifetime||lifetime->failed||!character||lifetime->actor.get()!=character||
       !character->object||character->object->identity!=identity||
       character->shared_handle().cached!=identity||
       lifetime->properties.get()!=properties||character->object->properties.get()!=properties||
       lifetime->life.get()!=life||character->object->life.get()!=life||
       &lifetime->view!=property_view||!properties||
       property_view->base!=properties->base.data()||property_view->saved!=properties->saved.data()||
       property_view->gear!=properties->gear.data()||property_view->resolved!=properties->resolved.data()||
       !inventory37c||lifetime->inventory37c!=inventory37c||!character->machine||
       fsm!=&character->machine->native_fsm()||fsm->character!=identity||
       visual_owner!=lifetime->visual.get()||!visual_owner||
       visual2d8!=&character->source_visual()||position160!=character->source_position160_v7()||
       player_skills!=lifetime->player_script_owner_v62.get()||save14e8!=lifetime->save.get()){
        e="Required unchanged SAME canonical Character/property/inventory/FSM/visual owner aliases";return false;
    }
    if(lifetime->save_fields&&*lifetime->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(save14e8)){
        e="Character14e8 does not alias SAME native Save owner";return false;
    }
    if(player_skills&&player_skills->native_savegame()!=save14e8){
        e="Player skill VM and Character14e8 must borrow SAME native Save";return false;
    }
    e.clear();return true;
}
SourceCharacterOwnerFactory::SourceCharacterOwnerFactory(
    std::shared_ptr<dh2::world::CanonicalCharacterCandidateFactoryV60> factory):factory_(std::move(factory)){}
bool SourceCharacterOwnerFactory::construct(const dh2::world::CanonicalFactoryEntryV1& entry,
    const dh2::world::CanonicalSourceObjectRequestV1& source,
    dh2::world::CanonicalClassReceiverV1& out,std::string& e){
    if(!factory_){e="Required existing world canonical Character factory";return false;}
    dh2::world::CanonicalClassReceiverV1 receiver;
    if(!factory_->construct(entry,source,receiver,e))return false; // native factory retains reached failure prefix
    const auto id=receiver.object.identity;
    auto record=factory_->find(id);
    if(!record||!receiver.source_init_final_v95){e="Required SAME constructor record and source Character virtual58";return false;}
    auto native_final=std::move(receiver.source_init_final_v95);
    // Retain only the delivery ledger with the callback, never a raw adapter
    // pointer. A receiver remains safe if the host releases this facade.
    receiver.source_init_final_v95=[receipts=final_deliveries_,id,record,native_final=std::move(native_final)](std::string& error){
        if(record->failed){error=record->error.empty()?"Failed source Character continuation cannot replay":record->error;return false;}
        if(!native_final(error)){
            // Native once1395 was already written before the tail. In
            // particular an external player_save_final callback can fail
            // without setting record.failed; never let a retry accept the
            // source once gate as proof that its prior tail completed.
            record->failed=true;record->error=error;receipts->erase(id);return false;
        }
        if(record->failed||!record->init_complete){error="Incomplete source Character final delivery";return false;}
        (*receipts)[id]=record;return true;
    };
    out=std::move(receiver);e.clear();return true;
}
bool SourceCharacterOwnerFactory::aliases(std::uintptr_t id,SourceCharacterOwnerAliases& out,std::string& e)const{
    auto r=factory_?factory_->find(id):nullptr;
    if(!r||!r->actor||!r->actor->object||!r->actor->machine){e="Required retained whole canonical Character constructor prefix";return false;}
    SourceCharacterOwnerAliases a;
    a.lifetime=r;a.identity=id;a.character=r->actor.get();a.properties=r->properties.get();
    a.property_view=&r->view;a.life=r->life.get();a.inventory37c=r->inventory37c;
    a.fsm=&r->actor->machine->native_fsm();a.visual_owner=r->visual.get();
    a.visual2d8=&r->actor->source_visual();a.position160=r->actor->source_position160_v7();
    a.player_skills=r->player_script_owner_v62.get();a.save14e8=r->save.get();
    if(!a.validate(e))return false;out=std::move(a);return true;
}
bool SourceCharacterOwnerFactory::borrow_constructed_prefix(std::uintptr_t id,SourceCharacterOwnerAliases& out,std::string& e)const{
    return aliases(id,out,e);
}
bool SourceCharacterOwnerFactory::borrow_completed_character(std::uintptr_t id,SourceCharacterOwnerAliases& out,std::string& e)const{
    SourceCharacterOwnerAliases a;if(!aliases(id,a,e))return false;
    auto receipt=final_deliveries_->find(id);
    if(!a.lifetime->init_complete||receipt==final_deliveries_->end()||receipt->second.lock()!=a.lifetime){
        e="Required actual completed Character InitPost and successful whole source InitFinal delivery";return false;
    }
    bool player{};if(!a.lifetime->is_player(player,e))return false;
    if(player&&(!a.player_skills||!a.player_skills->ready()||!a.save14e8||
       !a.lifetime->equipment||!a.lifetime->equipment->ready()||
       a.lifetime->equipment->inventory()!=a.inventory37c)){
        e="Required SAME completed player VM/Save/Gear inventory owner graph";return false;
    }
    out=std::move(a);e.clear();return true;
}
}
