#include "canonical_source_adapter_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
using namespace dh2;
using namespace dh2::loader;
static void check(bool v,const std::string& e){if(!v)throw std::runtime_error(e);}
struct FixtureActor {
    target_providers::Handle16 handle{0,UINT32_MAX,0};
    std::uint32_t type{};std::uint8_t across{};std::int32_t room{-1};
    bool character{};const char* catalog{};std::string name,archetype;std::array<float,3> position{};
    std::shared_ptr<void> world_pin;
};
struct FixtureWorld {
    std::vector<std::string> calls;
    std::weak_ptr<FixtureActor> actor;
    std::string fail;
};
static FixtureWorld& fixture_world(void* p){return *static_cast<FixtureWorld*>(p);}
static bool call(void* p,const char* name,std::string& e){
    fixture_world(p).calls.emplace_back(name);
    if(fixture_world(p).fail==name){e=std::string("Fixture failure: ")+name;return false;}
    return true;
}
static bool set_name(void* p,const char* v,std::string&){static_cast<FixtureActor*>(p)->name=v;return true;}
static bool archetype(void* p,const char* v,std::string&){static_cast<FixtureActor*>(p)->archetype=v;return true;}
static bool as_character(void* p,std::uintptr_t& out,std::string&){auto& a=*static_cast<FixtureActor*>(p);out=a.character?reinterpret_cast<std::uintptr_t>(p):0;return true;}
template<class Borrow> static auto catalog_field(Borrow& b,const char** name,int)->decltype(b.class_name20=name,void()){b.class_name20=name;}
template<class Borrow> static void catalog_field(Borrow&,const char**,long){}
static world::CanonicalObjectBorrowV1 borrow(std::shared_ptr<FixtureActor> a){
    world::CanonicalObjectBorrowV1 result{reinterpret_cast<std::uintptr_t>(a.get()),a,&a->handle,&a->type,&a->across,&a->room,a.get(),set_name,archetype,as_character};
    catalog_field(result,&a->catalog,0);return result;
}
static bool construct(void* p,const world::CanonicalFactoryEntryV1& entry,const world::CanonicalSourceObjectRequestV1& request,world::CanonicalObjectBorrowV1& out,std::string& e){
    if(!call(p,"construct",e))return false;
    check(canonical_source_entry_v1(request)!=nullptr,"Receiver lost full original XML");
    auto a=std::make_shared<FixtureActor>();a->character=std::string(entry.name)=="Character";
    a->type=std::string(entry.name)=="Module"?5:0;
    // Fixtures explicitly pin the request; no production class is constructed.
    a->world_pin=std::const_pointer_cast<void>(request.source_lease);
    fixture_world(p).actor=a;out=borrow(std::move(a));return true;
}
static bool network(void* p,world::CanonicalObjectBorrowV1&,std::string& e){return call(p,"network",e);}
static bool properties(void* p,const world::CanonicalObjectBorrowV1&,std::string& e){return call(p,"properties",e);}
static bool set_template(void* p,const world::CanonicalObjectBorrowV1&,const char* value,std::string& e){check(value&&!*value,"Present-empty template changed");return call(p,"template",e);}
static bool defaults(void* p,const world::CanonicalObjectBorrowV1& object,std::string& e){if(!call(p,"defaults",e))return false;static_cast<FixtureActor*>(object.context)->position={1,2,3};return true;}
static bool overrides(void* p,const world::CanonicalObjectBorrowV1& object,const world::CanonicalSourceObjectRequestV1& request,std::string& e){
    if(!call(p,"overrides",e))return false;
    const auto* entry=canonical_source_entry_v1(request);check(entry,"Original receiver element unavailable");
    if(const auto* value=entry->source().attribute("position")){
        check(*value=="7,8,9","Authored position replaced by projected placement");
        static_cast<FixtureActor*>(object.context)->position={7,8,9};
    }
    check(!entry->source().attribute("activate_cond")||*entry->source().attribute("activate_cond")=="chapter_gate","Condition source changed");
    return true;
}
static bool post(void* p,const world::CanonicalObjectBorrowV1&,std::string& e){return call(p,"early_post",e);}
static bool is_game(void* p,const world::CanonicalObjectBorrowV1&,bool& out,std::string& e){out=true;return call(p,"is_game",e);}
static bool position(void* p,const world::CanonicalObjectBorrowV1& object,std::array<float,3>& out,std::string& e){out=static_cast<FixtureActor*>(object.context)->position;return call(p,"position",e);}
static bool set_position(void* p,const world::CanonicalObjectBorrowV1& object,const std::array<float,3>& value,bool source_update,std::string& e){check(source_update,"SetPosition source update flag lost");if(!call(p,"set_position",e))return false;static_cast<FixtureActor*>(object.context)->position=value;return true;}
static bool unknown(void* p,const char* type,std::string& e){check(std::string(type)=="UnknownFixture","Wrong unknown type");return call(p,"unknown_debug",e);}
static world::CanonicalClassServicesV1 services(FixtureWorld* p){return {p,construct,properties,set_template,defaults,overrides,post,is_game,position,set_position,unknown};}
static world::CanonicalObjectManagerServicesV1 registry_services(FixtureWorld* p){world::CanonicalObjectManagerServicesV1 s;s.context=p;s.assign_network_id=network;return s;}
static CanonicalSourceBindingV1 binding(std::shared_ptr<FixtureWorld> owner,const std::string& attributes,ObjectEntryRouteV1 route=ObjectEntryRouteV1::manager,std::optional<std::string> filter={},bool nested=false){
    XmlDocumentV1 document;std::string error;
    const auto text="<Level><GameObject "+attributes+">"+(nested?std::string("<Script value=\"hello &amp; world\"/>"):std::string())+"</GameObject></Level>";
    check(document.capture("fixture/original.mlx",{text.begin(),text.end()},error),error);
    CanonicalSourceBindingV1 result;
    check(prepare_canonical_source_binding_v1(document.borrow(),1,route,filter,{owner,900,-27,{100,200,300}},result,error),error);
    return result;
}
int main(){try{
    unsigned checks=0;std::string error;
    for(const auto& original:original_factories_v1()){
        const auto& canonical=world::canonical_factories_v1();
        auto i=std::find_if(canonical.begin(),canonical.end(),[&](const auto& v){return std::string(v.name)==original.gametype;});
        check(i!=canonical.end()&&i->original_address==original.original_address,"Factory catalog drift");
    }++checks;
    std::weak_ptr<FixtureWorld> weak;
    {
        auto owner=std::make_shared<FixtureWorld>();weak=owner;
        auto source=binding(owner,"gametype=\"Character\" name=\"original\" template=\"\" _templateName=\"Registered\" position=\"7,8,9\" activate_cond=\"chapter_gate\"",ObjectEntryRouteV1::manager,{},true);
        auto retained=source.request();source={};owner.reset();
        check(!weak.expired(),"Candidate context expired while XML request retained");
        check(retained.attribute(retained.source_context,retained.element,"template")&&!*retained.attribute(retained.source_context,retained.element,"template"),"Empty template lost");
        check(!retained.attribute(retained.source_context,retained.element,"missing"),"Missing attribute invented");
        check(!retained.attribute(retained.source_context,retained.element+1,"name"),"Wrong source element accepted");
        const auto* entry=canonical_source_entry_v1(retained);check(entry&&entry->source().children.size()==1,"Nested source lost");
        check(*entry->document.elements().at(entry->source().children[0]).attribute("value")=="hello & world","Entity/nested XML changed");
        check(retained.module_occurrence==900&&retained.runtime_module_id==-27&&retained.module_offset==std::array<float,3>{100,200,300},"Runtime module context replaced by diagnostic indices");
        auto foreign=retained;foreign.source_lease.reset();check(!canonical_source_entry_v1(foreign),"Unretained request accepted by receiver accessor");
        ++checks;
    }
    check(weak.expired(),"Source binding retention cycle leaked candidate owner");++checks;
    {
        auto owner=std::make_shared<FixtureWorld>();world::CanonicalObjectManagerV1 manager(registry_services(owner.get()));manager.begin_frame(UINT32_MAX);
        CanonicalBoundSourceAttemptV1 attempt(binding(owner,"gametype=\"Character\" name=\"original\" template=\"\" position=\"7,8,9\" activate_cond=\"chapter_gate\""));
        check(attempt.execute(manager,services(owner.get()),error),error);
        check(attempt.step()==CanonicalBoundSourceStepV1::source_complete,"Canonical prefix not complete");
        const auto& handle=attempt.factory_attempt()->handle();auto actor=owner->actor.lock();
        check(handle.key==0&&handle.frame==UINT32_MAX&&handle.cached==reinterpret_cast<std::uintptr_t>(actor.get()),"Canonical signed handle changed");
        check(manager.object(handle.key)->shared_handle==&actor->handle&&actor->room==-27,"Actual receiver handle/room not retained");
        check(actor->position==std::array<float,3>{107,208,309},"Offset used raw/source position instead of actual post-property position");
        check(owner->calls==std::vector<std::string>{"construct","network","properties","template","defaults","overrides","is_game","position","set_position"},"Canonical operation ordering changed");
        const auto count=owner->calls.size();check(!attempt.execute(manager,services(owner.get()),error)&&owner->calls.size()==count,"Failed candidate attempt delivered twice");++checks;
    }
    for(const std::string type:{"LevelConfig","LightPoint"}){
        auto owner=std::make_shared<FixtureWorld>();world::CanonicalObjectManagerV1 manager(registry_services(owner.get()));
        const bool light=type=="LightPoint";
        CanonicalBoundSourceAttemptV1 attempt(binding(owner,"gametype=\""+type+"\" name=\""+(light?"_prim_PlayerLight":"config")+"\""));
        check(attempt.execute(manager,services(owner.get()),error),error);auto actor=owner->actor.lock();
        check(std::count(owner->calls.begin(),owner->calls.end(),"early_post")==(!light?1:0),"LevelConfig early InitPost gate changed");
        if(light)check(actor->name=="_prim_PlayerLight_S"&&actor->room==-1&&actor->handle.key==0,"Special light requested room confused with source/registry identity");
        ++checks;
    }
    for(const std::string type:{"Player","UnknownFixture"}){
        auto owner=std::make_shared<FixtureWorld>();world::CanonicalObjectManagerV1 manager(registry_services(owner.get()));
        const bool player=type=="Player";CanonicalBoundSourceAttemptV1 attempt(binding(owner,"gametype=\""+type+"\" name=\"gate\"",player?ObjectEntryRouteV1::level:ObjectEntryRouteV1::manager));
        check(attempt.execute(manager,services(owner.get()),error),error);check(attempt.step()==CanonicalBoundSourceStepV1::original_source_skip&&manager.source_count50()==0,"Original gate constructed a fixture receiver");
        check(player?owner->calls.empty():owner->calls==std::vector<std::string>{"unknown_debug"},"Original skip/debug gate changed");++checks;
    }
    {
        auto owner=std::make_shared<FixtureWorld>();world::CanonicalObjectManagerV1 manager(registry_services(owner.get()));
        CanonicalBoundSourceAttemptV1 attempt(binding(owner,"name=\"unsafe\"",ObjectEntryRouteV1::level));
        check(!attempt.execute(manager,services(owner.get()),error)&&!attempt.factory_attempt()&&owner->calls.empty(),"Unsafe Level null type was treated as successful loading");++checks;
        CanonicalBoundSourceAttemptV1 filtered(binding(owner,"gametype=\"Character\" name=\"filtered\"",ObjectEntryRouteV1::manager,std::string()));
        check(filtered.execute(manager,services(owner.get()),error)&&filtered.step()==CanonicalBoundSourceStepV1::original_source_skip&&owner->calls.empty(),"Present-empty type filter ignored");++checks;
    }
    for(const std::string failing:{"construct","overrides","network"}){
        auto owner=std::make_shared<FixtureWorld>();owner->fail=failing;world::CanonicalObjectManagerV1 manager(registry_services(owner.get()));
        CanonicalBoundSourceAttemptV1 attempt(binding(owner,"gametype=\"Character\" name=\"failure\""));
        check(!attempt.execute(manager,services(owner.get()),error)&&attempt.step()==CanonicalBoundSourceStepV1::failed,"Provider failure accepted");
        const auto* actual=attempt.factory_attempt();check(actual&&actual->stage()==world::CanonicalFactoryStageV1::failed,"Canonical reached prefix lost");
        check(manager.source_count50()==(failing=="construct"?0u:1u),"Failure silently rolled back/committed canonical registry");
        check(failing=="construct"||manager.object(actual->handle().key)!=nullptr,"Failed canonical Add/property prefix lost registry receiver");++checks;
    }
    {
        auto owner=std::make_shared<FixtureWorld>();auto source=binding(owner,"gametype=\"Character\" name=\"retained\"");const auto before=source.request();
        check(!prepare_canonical_source_binding_v1({},0,ObjectEntryRouteV1::manager,{}, {owner},source,error)&&source.request().source_context==before.source_context,"Invalid input replaced existing source binding");
        check(!prepare_canonical_source_binding_v1(source.entry().document,1,ObjectEntryRouteV1::manager,{}, {owner,UINT32_MAX,9,{}},source,error),"Unreset level context accepted");
        check(!prepare_canonical_source_binding_v1(source.entry().document,1,ObjectEntryRouteV1::manager,{}, {owner,5,9,{NAN,0,0}},source,error),"Nonfinite canonical module position accepted");++checks;
    }
    std::cout<<"{\"validation\":\"PASS\",\"adapter_checks\":"<<checks<<",\"scope\":\"retained XML and canonical source-prefix receiver fixtures\",\"runtime_objects_verified\":false,\"full_loader_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
