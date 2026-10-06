#define main prior_fx_cache_fixture_v32
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_melee_fx_manager_v8.cpp"
#undef main
#define main prior_actor_fixture_v32
#include "character_kill_live_v21_native.cpp"
#undef main
#pragma GCC diagnostic pop
#include "../character_loot_pickup_fx_v32.hpp"
#include "../canonical_point3d_globals_v1.hpp"
struct ActorSourceV32 {
 Fixture& f;std::uint32_t type{};
 static bool cast(void* p,std::uintptr_t& id,std::string&){id=static_cast<Actor*>(p)->id;return true;}
 static bool borrow(void* p,std::uintptr_t id,CharacterLootActorFieldsV31& out,std::string& e){auto& s=*static_cast<ActorSourceV32*>(p);auto* a=id==s.f.player->id?s.f.player.get():nullptr;if(!a){e="Required registered picker";return false;}out={};out.canonical.identity=id;out.canonical.lease=a->lease;out.canonical.shared_handle=&a->handle;out.canonical.type_f4=&s.type;out.canonical.context=a;out.canonical.as_character=cast;out.properties=&a->view;out.object_of_interest14a4=&a->tracked;out.receiver_lease=a->lease;return true;}
};
int main(int argc,char** argv){try{check(argc==2,"actual loot FX cache directory");std::string e;::Services services(argv[1]);data::EffectsTables tables;
 auto a=read(services.directory+"/effects_pyarray.bin"),b=read(services.directory+"/effects_pyarraynames.bin"),c=read(services.directory+"/effects_pystructnames.bin"),d=read(services.directory+"/effects_dictionary_pyarraynames.bin"),f=read(services.directory+"/effects_dictionary_pyarray.bin");auto bytes=[](const Raw& x){return data::Bytes{x.data(),x.size()};};check(tables.load(bytes(a),bytes(b),bytes(c),bytes(d),bytes(f),e),e);
 Fixture actors;ActorSourceV32 fields{actors};CharacterLootActorBindingV31 binding(*actors.world,actors.ai,{actors.lease,&fields,ActorSourceV32::borrow});
 actors.player->search.position[0]=113;actors.player->search.position[1]=227;actors.player->search.position[2]=331;
 fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;fx::CharacterAuthoredResourceFactoryV6 resources({nullptr,camera,driver},forces.factory());scene::Scene live;
 fx::CharacterMeshFxOwnerV4 owner(tables.borrow(),live,{&services,::Services::asset},{&services,::Services::invoke},resources.factory());check(owner.precache_libraries(e),e);
 world::CanonicalObjectManagerV1 manager({});world::CanonicalPropertyMapV1 props({nullptr,&world::canonical_vec3_origin_v1(),nullptr});physical::NativeWorld physics;navigation::CollisionWorld geometry{};navigation::ObstacleRegistry obstacles{};
 // No Item is spawned in this FX test. The borrowed Item owner is used only
 // to prove foreign Despawn rejection; positive145 remains predecessor test.
 WorldItemLiveOwnerV5 items(manager,props,physics,&geometry,&obstacles,{},{},{});
 CharacterLootPickupFxV32 pickup(owner,tables.borrow(),binding,items);LootInteractRequestV8 q{};q.operation=LootInteractOperationV8::loot_fx;q.character=actors.player->id;q.key="loot_orb_fx";LootInteractResponseV8 out;bool handled{};
 check(pickup.route(q,out,handled,e)&&handled,e);check(pickup.lookup_initialized()&&pickup.cached_set()==136,"actual name lookup resolves set136, not dictionary64");check(services.reads==1,"one actual resource load");auto states=owner.views();check(states.size()==1&&!states[0].state.anchor&&!states[0].state.fixed_rotation,"source NULL rotation/parent branch");check(states[0].state.position[0]==113&&states[0].state.position[2]==331,"same picker raw160");
 unsigned packets{},clouds{};for(int ms=0;ms<3000;ms+=16){check(owner.scene_frame(ms,16,e),e);check(owner.manager_frame(16,e),e);std::vector<fx::CharacterFxMeshDrawSourceV4> meshes;check(owner.mesh_draw_sources_v4(meshes,e),e);for(auto& m:meshes){fx::AuthoredFxGeometryPacketV7 packet;check(fx::authored_fx_geometry_packet_v7(m.part,m.material,packet,e),e);++packets;}std::vector<fx::CharacterParticleDrawSourceV3> particles;check(owner.particle_draw_sources_v3(particles,e),e);for(auto& p:particles){fx::AuthoredFxGeometryPacketV7 packet;check(fx::authored_fx_geometry_packet_v7(p.part,p.material,packet,e),e);++clouds;}}
 check(packets+clouds>0,"positive actual loot_orb draw packets");states=owner.views();check(states.size()==1&&states[0].finished&&states[0].pooled,"source completion returns resource to SAME pool");actors.player->search.position[0]=150;check(pickup.route(q,out,handled,e),e);check(services.reads==1&&owner.views()[0].state.position[0]==150,"warm reuse updates same raw160 without duplicate resource");
 q.operation=LootInteractOperationV8::despawn;q.object=0xdeadbeef;check(!pickup.route(q,out,handled,e)&&handled,"reject foreign Item despawn");q.operation=LootInteractOperationV8::show_text;check(pickup.route(q,out,handled,e)&&!handled,"other pickup services remain required outside FX");
 std::cout<<"PASS loot FX V32 checks="<<checks<<" mesh_packets="<<packets<<" cloud_packets="<<clouds<<"; actual cache resource/source NULL-rotation play/lifetime/pool/draw packets, source actor/camera/floor inputs fixtures; GPU/pickup award not claimed\n";return 0;
 }catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
