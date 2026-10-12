#include "generic_actor_visual_input_v37.hpp"
#include <algorithm>
namespace dh2::loader {
bool actor_visual_producer_v37(const std::shared_ptr<world::CanonicalOpenableGraphV21>& graph,
 ActorVisualProducerV37& out,std::string& e){
 if(!graph){e="Required actual canonical Container graph";return false;}
 ActorVisualProducerV37 p;p.receiver_owner=graph;p.receiver_identity=graph->receiver().base().identity();
 p.visual2d8=graph->receiver().base().pointer(0x2d8);p.visual=graph->visual();
 if(!p.visual2d8){e="Required SAME Container visual2d8 field";return false;}
 out=std::move(p);e.clear();return true;
}
bool actor_visual_producer_v37(const std::shared_ptr<character::RetainedCharacterActorV1>& actor,
 std::shared_ptr<world::RetainedGameObjectVisualV1> visual,ActorVisualProducerV37& out,std::string& e){
 if(!actor){e="Required actual canonical Character receiver";return false;}
 ActorVisualProducerV37 p;p.receiver_owner=actor;p.receiver_identity=actor->canonical(actor).identity;
 p.visual2d8=&actor->source_visual();p.visual=std::move(visual);p.character_receiver=true;
 p.character_animation=actor->animation.get();out=std::move(p);e.clear();return true;
}
bool GenericActorVisualInputV37::validate(std::int32_t key,const world::CanonicalObjectBorrowV1& object,
 const ActorVisualProducerV37& producer,std::string& e)const{
 if(!services_.registry_owner||!services_.manager||!services_.roots||!services_.lookup){e="Required actual actor registry/root/provider owner";return false;}
 const auto* current=services_.manager->object(key);
 if(!current||!current->lease||!current->identity||!current->shared_handle||current->shared_handle->key!=key||
    current->identity!=object.identity||current->lease!=object.lease||current->shared_handle!=object.shared_handle){
  e="Required live SAME canonical actor handle/receiver lease";return false;
 }
 if(!producer.receiver_owner||producer.receiver_identity!=object.identity||!producer.visual2d8){e="Required SAME canonical receiver visual2d8 producer";return false;}
 const auto published=*producer.visual2d8;
 if(!published){if(producer.visual){e="Unpublished actor Visual supplied by foreign/unfinished owner";return false;}return true;}
 if(!producer.visual||published!=reinterpret_cast<std::uintptr_t>(producer.visual.get())){e="Required actual published actor Visual lookup";return false;}
 if(!producer.visual->ready()||!producer.visual->root_identity()){e="Required initialized SAME actor Visual/root";return false;}
 if(producer.visual->root_game_object()!=object.identity){e="Actor Visual root204 belongs to another canonical receiver";return false;}
 const auto roots=services_.roots->roots();
 if(std::find(roots.begin(),roots.end(),producer.visual->root_identity())==roots.end()){e="Required SAME registered actor Visual root";return false;}
 const auto& bres=producer.visual->bres();const auto& scene=producer.visual->scene();
 if(!bres.bytes||!bres.size||producer.visual->node_flags().size()!=scene.graph.size()||producer.visual->source_mesh_render_enabled_v40().size()!=scene.instances.size()){e="Required live actor BRES/scene visibility producers";return false;}
 for(const auto& skin:producer.visual->skinned_meshes())if(skin.instance>=scene.instances.size()||skin.positions.size()!=skin.source_positions.size()||skin.positions.size()!=skin.skin.influences.size()){
  e="Required SAME actor skin/position producer domain";return false;
 }
 e.clear();return true;
}
bool GenericActorVisualInputV37::consume(std::int32_t key,
 const std::function<bool(const ActorVisualReadV37&,std::string&)>& consumer,
 ActorVisualConsumeResultV37& out,std::string& e){
 if(consuming_){e="Actor Visual consume reentered";return false;}
 if(!services_.registry_owner||!services_.manager||!services_.roots||!services_.lookup){e="Required actual actor registry/root/provider owner";return false;}
 const auto* registered=services_.manager->object(key);
 if(!registered){e="Actor key is absent from canonical registry";return false;}
 const auto object=*registered;
 struct Guard{bool& flag;explicit Guard(bool& v):flag(v){flag=true;}~Guard(){flag=false;}} guard(consuming_);
 ActorVisualProducerV37 producer;if(!services_.lookup(object,producer,e)||!validate(key,object,producer,e))return false;
 ActorVisualConsumeResultV37 result;result.character_receiver=producer.character_receiver;
 result.character_animation_present=producer.character_animation!=nullptr;
 if(!*producer.visual2d8){out=result;e.clear();return true;}
 if(!consumer){e="Required synchronous actor Visual consumer";return false;}
 auto& visual=*producer.visual;
 const ActorVisualReadV37 read{object,visual.bres(),visual.scene(),visual.node_flags(),visual.source_mesh_render_enabled_v40(),visual.skinned_meshes(),
   reinterpret_cast<std::uintptr_t>(&visual),visual.root_identity(),visual.scene_flags(),producer.character_animation};
 if(!consumer(read,e))return false;
 if(!validate(key,object,producer,e))return false;
 result.disposition=ActorVisualConsumptionV37::consumed_visual;result.instances=read.scene.instances.size();result.skinned_meshes=read.skinned_meshes.size();
 out=result;e.clear();return true;
}
}
