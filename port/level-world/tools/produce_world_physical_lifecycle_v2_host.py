"""Compose stable original NPC host fixture with production peer/lifecycle owners."""
from pathlib import Path
root=Path(__file__).resolve().parents[1]
s=(root/'tests/character_world_npc_physical_v1_host.cpp').read_text()
s=s.replace('#include <map>','#include <map>\n#include "../character_world_npc_physical_lifecycle_v2.hpp"\n#include "../character_world_peer_properties_v1.hpp"\n#include "../character_world_physical_character_v1.hpp"')
s=s.replace('std::map<void*,std::uintptr_t> owners;','CharacterWorldPhysicalPeersV1* peers{};')
a=s.index(' static bool peer(void* p,void* context,');b=s.index(' static int update_pf',a)
s=s[:a]+''' static bool peer(void* p,void* context,std::uintptr_t& owner,std::string& error){return static_cast<PhysicalFixture*>(p)->peers->owner(context,owner,error);}
 static bool enabled(void* p,std::uintptr_t id,std::uint8_t& value,std::string& error){auto& f=*static_cast<PhysicalFixture*>(p);++f.reads;if(!f.enabled_available){error="explicit unavailable byte80 test";return false;}return f.peers->enabled(id,value,error);}
'''+s[b:]
s=s.replace('std::unique_ptr<CharacterWorldNpcPhysicalV1> physical;','CharacterWorldNpcPhysicalV1* physical{};\n std::unique_ptr<CharacterWorldNpcPhysicalLifecycleV2> graph;std::uint8_t flat{};')
s=s.replace('dh2::navigation::NavigationObject pf;','dh2::actor::RuntimeState runtime{};')
s=s.replace('std::unique_ptr<CharacterWorldNpcObjectV1> object_lifecycle;','CharacterWorldNpcObjectV1* object_lifecycle{};')
s=s.replace('n.pf','n.runtime.object').replace('n->pf','n->runtime.object')
s=s.replace('n.id=0x200000001ull+index;', '''unsigned source_index=0;for(;source_index<(dact.size()-16)/256;++source_index){const auto* row=dact.data()+16+source_index*256;unsigned room;std::memcpy(&room,row+4,4);if(room==keys[index].room&&!std::strcmp(reinterpret_cast<const char*>(row+8),keys[index].name.c_str()))break;}check(source_index<(dact.size()-16)/256);n.id=0x100000002ull+source_index;''')
s=s.replace('n.id,int(index+1),&n,Npc::refresh','n.id,int(n.id-0x100000000ull),&n,Npc::refresh')
s=s.replace('PhysicalFixture physical_fixture;','CharacterWorldPhysicalPeersV1 production_peers(world);PhysicalFixture physical_fixture;physical_fixture.peers=&production_peers;')
a=s.index(' for(unsigned index=0;index<actors.size();++index){auto& n=*actors[index];unsigned model=');b=s.index(' check(obstacles.count==11);',a)
s=s[:a]+''' for(unsigned index=0;index<actors.size();++index){auto& n=*actors[index];unsigned model=keys[index].character.find("Skeleton")!=std::string::npos?0:keys[index].character.find("Ghost")!=std::string::npos?2:1;
  check(!dh2_nav_object_defaults(&n.runtime.object));
  check(property_loader.initialize(keys[index].room,keys[index].name,keys[index].character,n.object_fields));
  n.scene=*models[model].complete_scene();check(n.visual.bind(n.scene,error));
  WorldNpcPhysicalGraphBorrowV2 b{world,physical_world,production_peers,models[model],*n.owner,*n.session,*n.object,n.ai,*n.controller->command_state(0),n.collisions,globals,n.object_fields,n.runtime,crypt_world.native_floor->collision_world,obstacles,*d.ai(),n.final_fields,spawn_random,network,n.visual,n.scene,n.light_field,n.node,n.flat,*debug,files};
  WorldNpcPhysicalGraphServicesV2 service{};service.collision={&clock,Clock::get,nullptr,nullptr,nullptr};
  // Current source Idle/domain initialization reaches no object method: the
  // already-enabled InitFinal gate does not invent visibility/Stop callbacks.
  n.graph=std::make_unique<CharacterWorldNpcPhysicalLifecycleV2>(b,service);
  n.physical=&n.graph->physical_owner();n.object_lifecycle=&n.graph->object_owner();physical_fixture.pf[n.id]=n.object_lifecycle;
  dh2::physical::NpcBodyRequest q{};q.properties=&n.session->property_view();q.ai=d.ai();
  for(unsigned at=0;at<(dact.size()-16)/256;++at){const auto* row=dact.data()+16+256*at;if(!std::strcmp(reinterpret_cast<const char*>(row+8),keys[index].name.c_str())){std::memcpy(q.rotation_degrees,row+212,12);break;}}
  check(n.graph->initialize(q,keys[index].name.c_str(),""));check(n.physical->phase()==4&&n.physical->native().body);
  check(n.runtime.object.motion.floor!=~0u&&n.runtime.object.user==n.id);
  const auto& projection=n.physical->projection();for(unsigned k=0;k<6;++k){check(n.runtime.subobjects.local_bounds[k]==projection.bounds.relative_box[k]);check(n.runtime.subobjects.absolute_bounds[k]==projection.bounds.absolute_box[k]);}
  check(n.runtime.rotation.heading_angle==projection.visual.rotation_radians[2]);
 }
'''+s[b:]
# V2 uses its production byte80 provider, so remove only the old wrapper's
# artificial missing-byte toggle and retain a real source-field negative test.
s=s.replace('physical_fixture.enabled_available=false;bool failed=false;', 'a.object_fields.visible_written=false;bool failed=false;').replace('physical_fixture.enabled_available=true;','a.object_fields.visible_written=true;')
s=s.replace('check(n->physical->release());check(n->object_lifecycle->update_pf());','check(n->graph->close());')
s=s.replace('n->physical.reset();','n->physical=nullptr;n->graph.reset();')
s=s.replace('physical_fixture.owners[failed.world_object().context]=n.id;','check(production_peers.add({&failed.world_object(),&failed.native(),n.id,&n.object_fields,nullptr}));')
s=s.replace('check(failed.release());','check(failed.release());check(production_peers.remove(failed.world_object().context));')
s=s.replace(' check(obstacles.count==11);', '''
 std::vector<dh2::objects::Record> actual_records;
 for(unsigned index=0;index<(dact.size()-16)/256;++index){const auto* row=dact.data()+16+256*index;dh2::objects::Record r;std::memcpy(&r.kind,row,4);std::memcpy(&r.room,row+4,4);r.name=reinterpret_cast<const char*>(row+8);r.model=reinterpret_cast<const char*>(row+136);std::memcpy(r.position.data(),row+200,12);std::memcpy(r.rotation_degrees.data(),row+212,12);std::memcpy(r.scale.data(),row+224,12);actual_records.push_back(r);}
 CharacterWorldPeerPropertiesV1 decor_properties;auto props=file((root+"/port/level-world/reference/character-world-physical-peers-v1/crypt01-decor-properties.bin").c_str());check(decor_properties.load(props.data(),props.size(),digest,actual_records));
 struct DecorPeer {std::uintptr_t id{},node{};std::uint32_t type{};WorldNpcObjectFieldsV1 fields;dh2::physical::NativeBody body{};dh2::target_providers::Handle16 handle{};std::unique_ptr<WorldPhysicalBaseActorV1> base;std::unique_ptr<CharacterWorldPhysicalReceiverV1> receiver;};
 std::vector<std::unique_ptr<DecorPeer>> decor_peers;unsigned decor_bodies=0;
 for(unsigned index=0;index<actual_records.size();++index){const auto& r=actual_records[index];if(r.kind!=2)continue;auto peer=std::make_unique<DecorPeer>();auto& p=*peer;p.id=0x100000002ull+index;check(decor_properties.initialize_decor(index,p.fields,p.type)&&p.type==0x14&&p.fields.static84==1&&p.fields.visible80==1);
  p.base=std::make_unique<WorldPhysicalBaseActorV1>(p.id,r.position.data(),p.fields,nullptr,p.node);check(world.add(p.base->registration(int(p.id-0x100000000ull),p.handle))==0);
  auto model_bytes=file((root+"/port/android-native/app/src/main/assets/actors/"+r.model).c_str());dh2::resources::BresView v{};check(dh2_bres_open(&v,model_bytes.data(),model_bytes.size())==dh2::resources::BresError::ok);dh2::physical::DecorSceneMarker marker;check(dh2::physical::decor_scene_marker(v,marker,error));
  if(marker.found){dh2::physical::DecorSceneInput placement{};std::copy(r.position.begin(),r.position.end(),placement.position);std::copy(r.rotation_degrees.begin(),r.rotation_degrees.end(),placement.rotation_degrees);std::copy(r.scale.begin(),r.scale.end(),placement.scale);std::copy_n(marker.bounds,6,placement.marker_bounds);std::copy_n(marker.parent_scale,3,placement.marker_parent_scale);dh2::physical::DecorSceneOutput out{};check(dh2_decor_scene(&out,&placement)==0);dh2::physical::DecorBodyInput input{};input.owner=reinterpret_cast<void*>(p.id);input.new_physical=&p.body;input.visual_present=input.colbox_found=1;std::copy_n(out.mesh_box,6,input.mesh_box);std::copy(r.position.begin(),r.position.end(),input.position);dh2::physical::DecorBodyConfig definition{};check(dh2_decor_body_config(&definition,&input)==0);
   p.receiver=std::make_unique<CharacterWorldPhysicalReceiverV1>(production_peers,p.body,p.id,WorldPhysicalReceiverV1::base_physical);check(production_peers.add({&p.receiver->world_object(),&p.body,p.id,&p.fields,&p.type}));p.body={physical_world.create_character(definition.physical,&p.receiver->world_object()),definition.physical.radius,definition.physical.pinned};check(p.body.body);++decor_bodies;
  }decor_peers.push_back(std::move(peer));
 }
 check(decor_peers.size()==84&&decor_bodies>0);
 auto& npc=*actors.front();npc.owner->state().current=3;
 for(const auto& p:decor_peers)if(p->body.body){auto* a=npc.physical->native().body->GetShapeList();auto* b=p->body.body->GetShapeList();const auto af=a->GetFilterData(),bf=b->GetFilterData();dh2::physical::WorldShape x{&npc.physical->world_object(),{af.groupIndex,af.categoryBits,af.maskBits,1}},y{&p->receiver->world_object(),{bf.groupIndex,bf.categoryBits,bf.maskBits,1}};std::uintptr_t owner{};check(production_peers.owner(y.owner->context,owner,error)&&owner==p->id);std::uint8_t enabled{};check(production_peers.enabled(owner,enabled,error)&&enabled==1);unsigned type{};check(production_peers.type(owner,type,error)&&type==20);
  dh2::physical::WorldContact contact{{x,y},{0,0}};check(dh2_physical_world_contact(&contact,0)==0&&globals.collisions==1);check(dh2_physical_world_contact(&contact,2)==0&&globals.collisions==0);p->fields.visible80=0;check(dh2_physical_world_should_collide(&x,&y)==0);p->fields.visible80=1;check(!production_peers.remove(y.owner->context));
 }
 check(obstacles.count==11);''')
s=s.replace(' auto& npc=*actors.front();npc.owner->state().current=3;', '''
 struct PlayerPeer {std::uintptr_t id=0x100000001ull,node{};WorldNpcObjectFieldsV1 fields;dh2::physical::NativeBody body{};dh2::target_providers::Handle16 handle{};dh2::target_search::Object48 search{};sk::SkillTargetCharacterV6 character{};std::shared_ptr<dh2::data::PropertyState> properties;dh2::data::CombatActorState life{};CharacterStateOwner machine{id};const dh2::scene::Scene* pose{};
  static int refresh(void* p,sk::WorldTargetActorBorrowV1* out){auto& s=*static_cast<PlayerPeer*>(p);s.search.identity=s.id;s.character.identity=s.id;s.character.resolved=s.properties->resolved.data();s.character.name="PlayerCharacterPrince";*out={};out->identity=s.id;out->search=&s.search;out->character=&s.character;out->life=&s.life;out->target_node=&s.node;out->position=s.search.position;out->scene=s.pose;return 0;}
 } player_peer;player_peer.properties=player;check(character_object_default_properties_v1(player_peer.fields,error));std::copy(crypt_world.spawn.begin(),crypt_world.spawn.end(),player_peer.search.position);
 check(world.add({player_peer.id,1,&player_peer,PlayerPeer::refresh,&player_peer.machine.state(),&player_peer.handle})==0);
 CharacterWorldPhysicalCharacterV1 player_events({world,player_peer.id,player_peer.machine.native_fsm(),*debug,files,nullptr,nullptr});CharacterWorldPhysicalReceiverV1 player_receiver(production_peers,player_peer.body,player_peer.id,WorldPhysicalReceiverV1::character,player_events.services());check(production_peers.add({&player_receiver.world_object(),&player_peer.body,player_peer.id,&player_peer.fields,nullptr}));
 // Borrow existing genuine source player body definition producer; this host
 // deliberately leaves the real FSM at constructor-null (GetInteger=-1) and player AIS
 // unavailable. No fabricated Idle/AI success is used to enable contacts.
 auto prince_bytes=file((root+"/port/android-native/app/src/main/assets/models/prince_modular.bdae").c_str());dh2::resources::BresView prince_view{};dh2::scene::Scene prince_pose;check(dh2_bres_open(&prince_view,prince_bytes.data(),prince_bytes.size())==dh2::resources::BresError::ok&&dh2::scene::load(prince_view,prince_pose,error));dh2::physical::ObjectVisualTransformV1 tr{};float scale[3],angles[3]{};check(dh2_character_visual_scale(scale,player->base.data()+12)==0&&dh2_object_visual_transform_v1(&tr,player_peer.search.position,angles,scale)==0);dh2::physical::CharacterOwnerBounds prince_bounds{};check(character_npc_visual_bounds_v1(prince_view,prince_pose,tr.root_matrix,player_peer.search.position,player->resolved[16],0,prince_bounds,error));player_peer.pose=&prince_pose;auto queries=world.targets().query_services();std::uintptr_t is_player{};dh2::target_providers::Request24 player_query{dh2::target_providers::virtual_player,0,player_peer.id,0};check(queries.invoke(queries.context,&player_query,&is_player)==0&&is_player);
 dh2::physical::CharacterBodyInput player_input{};player_input.owner=reinterpret_cast<void*>(player_peer.id);player_input.new_physical=&player_peer.body;player_input.character_type=d.ai()->rows[player->resolved[1]].type;player_input.is_player=unsigned(is_player);player_input.special_owner_byte=player_peer.fields.static84;player_input.absolute_bounds[0]=prince_bounds.absolute_box[0];player_input.absolute_bounds[1]=prince_bounds.absolute_box[1];player_input.absolute_bounds[2]=prince_bounds.absolute_box[3];player_input.absolute_bounds[3]=prince_bounds.absolute_box[4];std::copy_n(player_peer.search.position,2,player_input.position);dh2::physical::CharacterBodyConfig player_config{};check(dh2_character_body_config(&player_config,&player_input)==0);player_peer.body={physical_world.create_character(player_config,&player_receiver.world_object()),player_config.radius,player_config.pinned};check(player_peer.body.body);
 bool player_allowed=true;check(player_events.filter(1,player_allowed,error)&&player_allowed);check(!player_events.event(dh2::physical::ContactEvent::add,actors.front()->id,1,error)&&!error.empty());
 auto& npc=*actors.front();npc.owner->state().current=3;''')
s=s.replace('check(production_peers.clear());check(physical_world.backend()', 'physical_world.destroy(player_peer.body.body);check(production_peers.remove(player_receiver.world_object().context));check(world.remove(player_peer.id)==0);check(production_peers.clear());check(physical_world.backend()')
s=s.replace('actual_decor_world_objects\\\":84','canonical_player_peer\\\":true,\\\"player_constructor_FSM_absent\\\":true,\\\"player_AIS_required\\\":true,\\\"actual_decor_world_objects\\\":84')
s=s.replace('check(physical_world.backend()->GetBodyCount()==1);check(obstacles.count==0', '''for(auto& p:decor_peers){if(p->body.body){physical_world.destroy(p->body.body);check(production_peers.remove(p->receiver->world_object().context));}check(world.remove(p->id)==0);}check(production_peers.clear());check(physical_world.backend()->GetBodyCount()==1);check(obstacles.count==0''')
s=s.replace('actual_BRES_models\\\":3','actual_BRES_models\\\":3,\\\"actual_decor_world_objects\\\":84,\\\"actual_decor_bodies\\\":'+ '"<<decor_bodies<<"')
s=s.replace('source_collision_clock_fixture\\\":true','source_collision_clock_fixture\\\":true,\\\"production_peer_registry\\\":true,\\\"physical_lifecycle_v2\\\":true')
s=s.replace('for(auto& n:actors){check(n->graph->close());','physical_world.destroy(player_peer.body.body);check(production_peers.remove(player_receiver.world_object().context));check(world.remove(player_peer.id)==0);\n for(auto& n:actors){check(n->graph->close());')
s=s.replace('#include "../character_world_physical_character_v1.hpp"','#include "../character_world_physical_character_v1.hpp"\n#undef check\n#define check(expression) do { if (!(expression)) { std::cerr << "physical graph check failed at line " << __LINE__ << ": " << #expression << std::endl; std::abort(); } ++checks; } while (0)')
(root/'tests/character_world_physical_lifecycle_v2_host.cpp').write_text(s)
print('Wrote production peer/lifecycle graph fixture')
