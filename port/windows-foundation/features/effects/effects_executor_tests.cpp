#include "effects_executor.hpp"
#include "../../../level-world/character_authored_resource_v32.hpp"
#include "../../../level-world/character_authored_fx_forces_v4.hpp"
#include "../../../level-world/visual_fx_preload.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace dh2;
using namespace dh::foundation::effects;
using Raw = std::vector<std::uint8_t>;
static unsigned checks;
static void check(bool value, const std::string& message) {
    ++checks; if (!value) throw std::runtime_error(message);
}
static Raw read(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error("Missing actual asset: " + path);
    return {std::istreambuf_iterator<char>(stream), {}};
}
static std::string normalize(std::string path) {
    std::replace(path.begin(), path.end(), '\\', '/');
    std::transform(path.begin(), path.end(), path.begin(), [](unsigned char c) { return char(std::tolower(c)); });
    return path;
}
struct Services {
    std::map<std::string, std::string> assets;
    character::DebugSwitches* debug{};
    fx::DebugModules* modules{};
    character::DebugFileServices24 files{};
    scene::Scene live;
    unsigned reads{}, gates{};
    bool reenter{}, reentered{};
    EffectsExecutor* executor{};
    dh::foundation::MarkerOccurrence occurrence;
    ActorAttachment actor{0x100000001ull, 0, {113,227,331}, {.1f,.2f,.3f}};
    Services(const char* mapping) {
        std::ifstream stream(mapping); std::string line;
        while (std::getline(stream, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            auto split = line.find('\t'); check(split != std::string::npos, "asset mapping");
            assets.emplace(normalize(line.substr(0, split)), line.substr(split + 1));
        }
        files = {this, [](void*, const char*, std::uintptr_t* handle) { *handle=0; return 0; },
                      [](void*, std::uintptr_t) { return 0; }};
        debug = dh2_character_debug_create(); modules = dh2_fx_debug_modules_create(debug, &files);
        check(debug && modules, "actual native Debug/Module owners");
        scene::Node root; root.translation[0]=actor.target_position[0];
        root.translation[1]=actor.target_position[1]; root.translation[2]=actor.target_position[2];
        live.graph.push_back(root); std::string error; check(scene::update_world(live,error), error);
    }
    ~Services() { dh2_fx_debug_modules_destroy(modules); dh2_character_debug_destroy(debug); }
    static bool asset(void* p, const char* uri, Raw& bytes, std::string& error) {
        auto& s=*static_cast<Services*>(p); ++s.reads;
        auto it=s.assets.find(normalize(uri));
        if(it==s.assets.end()) { error=std::string("Unsupported missing exact original resource: ")+uri; return false; }
        try { bytes=read(it->second); return true; } catch(const std::exception& e) { error=e.what(); return false; }
    }
    static bool invoke(void* p, fx::MeshFxRequestV1& q, std::string& error) {
        auto& s=*static_cast<Services*>(p); using O=fx::MeshFxOperationV1;
        check(q.live_scene==&s.live, "same retained Scene");
        if(s.reenter && !s.reentered && s.executor) {
            s.reentered=true; std::string nested;
            check(s.executor->marker(s.actor,"fixture-retained-clip",s.occurrence,nested)==DispatchResult::duplicate,
                  "marker token committed before backend reentry");
        }
        if(q.operation==O::debug_load) return dh2_character_debug_load(s.debug,&s.files)==1;
        if(q.operation==O::module_enabled) return dh2_fx_debug_module_get(&q.result,s.modules,q.text)==1;
        if(q.operation==O::set_switch || q.operation==O::instance_switch)
            return dh2_character_debug_get(&q.result,s.debug,q.text,&s.files)==1;
        if(q.operation==O::floor_normal) { std::fill_n(q.point,3,0.f); return true; }
        if(q.identity!=s.actor.actor) { error="Unsupported fixture socket identity"; return false; }
        if(q.operation==O::anchor_position) std::copy_n(s.live.graph[0].translation,3,q.point);
        else if(q.operation==O::anchor_rotation) std::copy_n(s.actor.source_rotation,3,q.point);
        else if(q.operation==O::anchor_scale) std::copy_n(s.live.graph[0].scale,3,q.point);
        else if(q.operation==O::anchor_dead || q.operation==O::anchor_disabled || q.operation==O::anchor_stationary) q.result=0;
        else { error="Unsupported fixture service"; return false; }
        return true;
    }
};
static bool camera(void*, float* m, float* p, std::string&) {
    const float identity[16]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    std::copy_n(identity,16,m); p[0]=17;p[1]=29;p[2]=500; return true;
}
static bool driver(void*, std::uint32_t& value, std::string&) { value=8; return true; }
int main(int argc,char** argv) { try {
    check(argc==3,"usage: effects-test actual-table-dir exact-uri-mapping");
    Services services(argv[2]); data::EffectsTables tables; std::string error;
    std::string dir=argv[1];
    auto a=read(dir+"/effects_pyarray.bin"), b=read(dir+"/effects_pyarraynames.bin"),
         c=read(dir+"/effects_pystructnames.bin"), d=read(dir+"/effects_dictionary_pyarraynames.bin"),
         e=read(dir+"/effects_dictionary_pyarray.bin");
    auto bytes=[](const Raw& value) { return data::Bytes{value.data(),value.size()}; };
    check(tables.load(bytes(a),bytes(b),bytes(c),bytes(d),bytes(e),error),error);
    auto table=tables.borrow(); fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;
    fx::CharacterAuthoredResourceFactoryV32 resources({nullptr,camera,driver},forces.factory());
    unsigned resources_tested=0, mesh_draws=0, particle_draws=0, unsupported=0;
    int melee=-1;
    for(unsigned index=0;index<table.sets().size();++index) {
        const auto& row=table.sets()[index];
        if(row.type || row.steps.size()!=1 || row.steps[0].redir || !row.steps[0].subobject.empty()) continue;
        auto file=row.steps[0].file;
        if(file<0 || std::size_t(file)>=table.dictionary().values.size()) continue;
        const auto& uri=table.dictionary().values[file];
        if(!services.assets.count(normalize(uri))) continue;
        if(uri.find("swoosh_prince_1hand_combo_01")!=std::string::npos) melee=index;
        fx::CharacterMeshFxOwnerV4 manager(table,services.live,{&services,Services::asset},
                                         {&services,Services::invoke},resources.factory());
        check(manager.precache_libraries(error),error); EffectsExecutor executor(manager);
        std::uintptr_t id{}; bool played=executor.play_set(index,services.actor,true,false,&id,error);
        if(!played) {
            check(!error.empty(),"reached missing original continuation must be explicit");
            std::cout<<"REQUIRED | "<<uri<<" | "<<error<<std::endl;++unsupported;continue;
        }
        check(played&&id,uri+": play id="+std::to_string(id)+" "+error);
        bool visible=false;
        for(int ms=4000;ms<7000;ms+=32) {
            bool sampled=executor.scene_phase(ms,32,error)&&executor.manager_phase(32,error);
            check(sampled,uri+": frame "+std::to_string(ms)+" "+error);
            std::vector<fx::CharacterFxMeshDrawSourceV4> meshes;
            std::vector<fx::CharacterParticleDrawSourceV3> particles;
            const auto before=manager.views();
            check(executor.draw_sources(meshes,particles,error),error);
            check(manager.views().at(0).current_ms==before.at(0).current_ms,"draw borrows without tick");
            mesh_draws+=meshes.size();particle_draws+=particles.size();visible|=!meshes.empty()||!particles.empty();
        }
        check(visible,uri+": actual draw closure");
        auto view=manager.views().at(0);
        if(!view.pooled) { executor.release_actor(services.actor.actor);check(manager.views().at(0).state.anchor==0,"cleanup detaches source anchor");check(executor.stop(id,error),error); }
        else id=0;
        auto reads=services.reads; std::uintptr_t warm{};
        check(executor.play_set(index,services.actor,true,false,&warm,error)&&warm==view.identity&&services.reads==reads,"retained original pool reuse");
        check(executor.stop(warm,error)&&warm==0,error);++resources_tested;
        std::cout<<"PASS | "<<uri<<std::endl;
    }
    check(melee>=0 && resources_tested>=8 && mesh_draws>0 && particle_draws>0,"original melee/skill/blood domains exercised");
    fx::CharacterMeshFxOwnerV4 manager(table,services.live,{&services,Services::asset},
                                     {&services,Services::invoke},resources.factory());
    check(manager.precache_libraries(error),error); EffectsExecutor executor(manager);services.executor=&executor;
    services.occurrence.marker.name="fx_"+table.set_names().at(melee);services.occurrence.marker.index=4;
    services.occurrence.generation=7; services.reenter=true;
    check(executor.marker(services.actor,"fixture-retained-clip",services.occurrence,error)==DispatchResult::delivered,error);
    check(services.reentered,"synchronous source service reentry");
    check(executor.marker(services.actor,"fixture-retained-clip",services.occurrence,error)==DispatchResult::duplicate,"same marker not replayed");
    ++services.occurrence.generation;
    check(executor.marker(services.actor,"fixture-retained-clip",services.occurrence,error)==DispatchResult::delivered,"retained restart generation");
    dh2::data::AnimationStep step;step.fx=melee;step.anchor_fx=true;
    PhaseOccurrence phase{services.actor.actor,3,17,{0,2}};
    check(executor.phase(services.actor,phase,step,{},error)==DispatchResult::delivered,error);
    check(executor.phase(services.actor,phase,step,{},error)==DispatchResult::duplicate,"same phase once");
    step.swoosh=true;++phase.occurrence;
    check(executor.phase(services.actor,phase,step,{},error)==DispatchResult::required_failure&&error.find("Swoosh")!=std::string::npos,"missing original equipment gate explicit");
    ++phase.occurrence;
    StepServices gate{&services,[](void* p,const dh2::data::AnimationStep&,bool& result) {++static_cast<Services*>(p)->gates;result=false;return 0;}};
    check(executor.phase(services.actor,phase,step,gate,error)==DispatchResult::delivered&&services.gates==1,"actual supplied Swoosh gate");
    std::uintptr_t id=99;check(!executor.play_set(melee,services.actor,true,true,&id,error)&&id==0,"missing original socket explicit");
    check(!executor.manager_phase(-1,error),"negative App dt rejected");
    executor.release_actor(services.actor.actor);
    for(const auto& view:manager.views())check(view.state.anchor==0,"actor teardown clears retained anchor");
    std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_resources\":"<<resources_tested
             <<",\"mesh_draw_sources\":"<<mesh_draws<<",\"particle_draw_sources\":"<<particle_draws
             <<",\"unsupported_actual_resources\":"<<unsupported
             <<",\"live_renderer\":false,\"anchor_and_camera_fixture\":true}\n";
    return 0;
} catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;} }
