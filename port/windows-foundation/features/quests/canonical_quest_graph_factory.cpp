#include "canonical_quest_graph_factory.hpp"
namespace dh::foundation { namespace {
bool required(std::string& e,const char* leaf){if(e.empty())e=std::string("Required original canonical Quest graph: ")+leaf;return false;}
bool coherent(const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& r,std::uintptr_t id,std::string& e){
    return r&&r->actor&&r->actor->object&&r->actor->object->identity==id&&r->save_fields&&
        r->save_fields->character()==id&&r->save&&r->save->character()==id&&
        *r->save_fields->save_slot14e8()==reinterpret_cast<std::uintptr_t>(r->save.get())&&
        r->load&&&r->load->save()==r->save.get()&&r->quest_sync_owner?true:required(e,"PM-created Character/Save14e8/SaveLoad/sync owner");
}
}
bool bind_canonical_quest_character_services(SourceQuestServices& out,CanonicalQuestCharacterResolver resolve,std::string& e){
    if(!out.world_owner||!resolve)return required(e,"actual world and canonical Character resolver");
    out.character_quests=[resolve](auto id,auto& q,auto& error){q.reset();std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> r;
        if(!resolve(id,r,error)||!coherent(r,id,error))return false;
        q=r->profile_bootstrap?r->profile_bootstrap->quest_owner_v70():nullptr;
        return q&&q->save()==r->save?true:required(error,"SAME profile Quest owner");
    };
    out.difficulty=[resolve](auto id,auto& value,auto& error){std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> r;
        if(!resolve(id,r,error)||!coherent(r,id,error)||!r->services.difficulty_global)return required(error,"actual Character.Save difficulty global");
        value=r->services.difficulty_global->value();error.clear();return true;
    };
    out.synchronize_quests=[resolve](auto id,auto& error){std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> r;
        return resolve(id,r,error)&&coherent(r,id,error)&&r->quest_sync_owner->try_sync(r->services.quest_sync,error);
    };e.clear();return true;
}
bool prepare_canonical_quest_profile(const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& r,
    const std::shared_ptr<const dh2::data::QuestTablesPersistenceV51>& tables,
    dh2::character::CharacterProfileBootstrapInputsV59 input,
    std::shared_ptr<dh2::character::CharacterMenuQuestsV51>& out,std::string& e){
    if(out||!r||!r->actor||!r->actor->object||!coherent(r,r->actor->object->identity,e))return required(e,"fresh canonical source Character");
    if(r->profile_bootstrap||!tables||!tables->ready()||input.reads.quests||
       r->save->regular_quests_v45().initialized()||r->save->volatile_quests_v45().initialized())return required(e,"once-only actual profile/quest initializer order");
    if(input.save!=r->save||input.load!=r->load||input.character!=r->save->character()||
       input.source_save14e8!=r->save_fields->save_slot14e8()||!input.actual_source_cells_lease)return required(e,"SAME original profile bootstrap inputs");
    // Constructor only: the real Load2 initializer is the sole collection C1.
    auto quests=std::make_shared<dh2::character::CharacterMenuQuestsV51>(r->save,tables);
    input.reads.quests=quests;
    auto bootstrap=std::make_shared<dh2::character::CharacterProfileBootstrapV59>(std::move(input));
    r->profile_bootstrap=bootstrap; //original retained slot before reached masks
    if(!bootstrap->prepare(e))return false; //retains authentic interrupted prefix
    out=std::move(quests);e.clear();return true;
}
bool publish_canonical_quest_runtime(const std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>& r,
    const std::shared_ptr<SourceQuestServiceBinding>& binding,const CanonicalQuestNativeGraph& graph,std::string& e){
    if(!r||!r->actor||!r->actor->object||!coherent(r,r->actor->object->identity,e)||!r->profile_bootstrap||!binding)return required(e,"prepared SAME canonical Character/profile binding");
    auto q=r->profile_bootstrap->quest_owner_v70();
    if(!q||q->save()!=r->save||!graph.world_owner||!graph.level_owner||!graph.conditions||!graph.objectives||
       !graph.game_events194||!graph.game_events194->diagnostics().storage_load_complete||
       !graph.source_game_events194||*graph.source_game_events194!=reinterpret_cast<std::uintptr_t>(graph.game_events194.get())||
       !graph.level_events||!graph.level_identity||graph.level_events->identity()!=graph.level_identity)return required(e,"SAME completed Level194/dispatcher/Condition/Objective graph");
    auto services=graph.markers;services.conditions=graph.conditions;services.objectives=graph.objectives;
    if(!services.provider)return required(e,"actual quest marker service transport");
    // Runtime validates SAME manager ownership itself; a foreign runtime cannot
    // pass merely by supplying a matching separate Level194 pointer.
    if(!graph.objectives->attach_manager_v75(graph.game_events194,e))return false;
    return binding->publish(q,std::move(services),e);
}
}
