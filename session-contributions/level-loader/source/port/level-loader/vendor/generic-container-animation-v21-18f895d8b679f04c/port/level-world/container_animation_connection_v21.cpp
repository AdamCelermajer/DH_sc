#include "container_animation_connection_v21.hpp"
#include <type_traits>
namespace dh2::world {
namespace {
bool get(const ContainerAnimatorProviderV21& p,std::shared_ptr<RetainedGenericAnimatorV21>& a,std::shared_ptr<RetainedGameObjectVisualV1>& v,std::string& e){
 if(!p||!p(a,v,e))return false;
 if(!a||!v||!v->ready()){e="Required SAME attached generic container animator/visual";return false;}return true;
}
auto flags(ContainerAnimatorProviderV21 p){return [p](std::uint32_t clear,std::uint32_t set,std::string& e){std::shared_ptr<RetainedGenericAnimatorV21>a;std::shared_ptr<RetainedGameObjectVisualV1>v;if(!get(p,a,v,e))return false;v->scene_flags()=(v->scene_flags()&~clear)|set;return true;};}
auto play(ContainerAnimatorProviderV21 p){return [p](const char* name,bool& accepted,std::string& e){std::shared_ptr<RetainedGenericAnimatorV21>a;std::shared_ptr<RetainedGameObjectVisualV1>v;if(!get(p,a,v,e))return false;return v->play(name,false,accepted,e);};}
template<class T>bool bind(std::shared_ptr<std::weak_ptr<T>> receiver,const ContainerAnimatorProviderV21& p,std::string& e,bool derived=true){
 auto core=receiver?receiver->lock():nullptr;if(!core){e="Required retained canonical container callback receiver";return false;}
 std::shared_ptr<RetainedGenericAnimatorV21>a;std::shared_ptr<RetainedGameObjectVisualV1>v;if(!get(p,a,v,e))return false;
 GenericAnimationCallbacksV21 f;
 // Weak receiver references avoid visual->callback->container->visual cycles.
 // Each delivery pins the actual canonical receiver throughout its source call.
 f.completion=[receiver](timeline::State& t,std::string& error){auto r=receiver?receiver->lock():nullptr;if(!r){error="Container completion receiver released";return false;}if constexpr(std::is_same_v<T,CanonicalOpenableContainerV1>)return r->receiver().animation_finished(t.loop!=0,error);else return r->animation_finished(t.loop!=0,error);};
 f.event=[receiver,derived](const animation::TriggeredEvent& event,std::string& error){(void)derived;auto r=receiver?receiver->lock():nullptr;if(!r){error="Container authored event receiver released";return false;}if constexpr(std::is_same_v<T,CanonicalOpenableContainerV1>)return r->receiver().animation_event(event.name,error);else return derived?r->animation_event(event.name,error):r->container_animation_event_v21(event.name,error);};
 return a->set_callbacks(std::move(f),e);
}
}
bool connect_openable_animation_v21(OpenableContainerServicesV1& s,std::shared_ptr<std::weak_ptr<CanonicalOpenableContainerV1>> receiver,ContainerAnimatorProviderV21 p,std::string& e){
 if(!p){e="Required attached Openable visual/controller provider";return false;}
 s.bind_timeline_callbacks=[receiver,p](std::string& error){return bind(receiver,p,error);};s.play_animation=play(p);s.scene_flags=flags(p);return true;
}
bool connect_destructible_animation_v21(DestructibleContainerServicesV16& s,std::shared_ptr<std::weak_ptr<CanonicalDestructibleContainerV16>> receiver,ContainerAnimatorProviderV21 p,std::string& e){
 if(!p){e="Required attached Destructible visual/controller provider";return false;}
 s.bind_callbacks=[receiver,p](bool derived,std::string& error){return bind(receiver,p,error,derived);};s.common.play_animation=play(p);s.common.scene_flags=flags(p);
 s.animation_count=[p](std::uint32_t& n,std::string& error){std::shared_ptr<RetainedGenericAnimatorV21>a;std::shared_ptr<RetainedGameObjectVisualV1>v;return get(p,a,v,error)&&a->count(n,error);};
 s.play_index=[p](std::uint32_t i,bool loop,std::string& error){std::shared_ptr<RetainedGenericAnimatorV21>a;std::shared_ptr<RetainedGameObjectVisualV1>v;if(!get(p,a,v,error))return false;bool accepted=false;if(!a->play_index(i,loop,accepted,error))return false;return true;};
 return true;
}
}
