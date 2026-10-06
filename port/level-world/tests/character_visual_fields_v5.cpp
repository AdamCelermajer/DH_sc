#include "../canonical_character_family_v4.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include "../character_scene.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <iostream>
using namespace dh2;
static std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Required cache "+p);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::string path=argv[1],error;std::vector<std::vector<std::uint8_t>> raw;
 auto bytes=[&](const std::string& name){raw.push_back(read(path+"/"+name));auto& b=raw.back();return data::Bytes{b.data(),b.size()};};
 raw.reserve(32);character::GameDesignInputs256 input{};character::GameDesignTableInput48* tables[]{&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};const char* prefixes[]{"character_properties","character_classes","ai","ai_factions","levels"};
 for(unsigned i=0;i<5;++i)*tables[i]={bytes(std::string(prefixes[i])+"_pyarray.bin"),bytes(std::string(prefixes[i])+"_pyarraynames.bin"),bytes(std::string(prefixes[i])+"_pystructnames.bin")};
 character::CharacterGameDesign design;if(!design.initialize(input,error))throw std::runtime_error(error);
 data::Dictionary models;if(!data::load_dictionary(bytes("character_models_dictionary_pyarraynames.bin"),bytes("character_models_dictionary_pyarray.bin"),models,error))throw std::runtime_error(error);
 data::LootTablesV2 loot;if(!loot.load(bytes("loot_table_pyarray.bin"),bytes("loot_table_pyarraynames.bin"),bytes("loot_table_pystructnames.bin"),error))throw std::runtime_error(error);
 auto world=std::make_shared<int>(1);data::LootRandom8V2 rng{};world::CanonicalCharacterFamilyServicesV4 services;services.world=world;services.design=&design;services.models=&models;services.loot_tables=&loot;services.loot_random=&rng;
 // Spawn/debug are fixtures; the intentional reached Visual InitPost boundary
 // must fail, not impersonate a successfully initialized/renderable actor.
 services.remaining_init=[](world::CanonicalCharacterRecordV4&,const character::NpcInitPostRequestV1& q,character::NpcInitPostResponseV1& r,std::string& e){if(q.source_entry==0x38bd64){r.value=-2;return true;}if(q.source_entry==0x337888||q.source_entry==0x337a88)return true;e="Required genuine whole Visual InitPost";return false;};
 world::CanonicalCharacterFamilyFactoryV4 factory(services);world::CanonicalPropertyMapV1 map({nullptr,&world::canonical_vec3_origin_v1(),nullptr});auto table=design.borrow();unsigned families=0,priest=0;
 for(std::size_t index=2;index<table.characters()->rows.size();++index){data::PropertyState p;data::reset_properties(*table.rules(),p,&table.characters()->rows[index]);if(!data::recalc_properties_with_class(*table.classes(),*table.rules(),p,error))throw std::runtime_error(error);auto aiid=p.resolved[1];if(aiid<0||static_cast<std::size_t>(aiid)>=table.ai()->rows.size())aiid=8;auto type=table.ai()->rows[aiid].type;if(type==1||type==3)continue;
  const auto& desc=table.characters()->names[index];std::map<std::string,std::string> attrs{{"name",desc}};world::CanonicalSourceObjectRequestV1 source;source.source_lease=world;source.source_context=&attrs;source.attribute=[](void* p,std::uint32_t,const char* key)->const char*{auto& a=*static_cast<std::map<std::string,std::string>*>(p);auto i=a.find(key);return i==a.end()?nullptr:i->second.c_str();};
  world::CanonicalClassReceiverV1 receiver;assert(factory.construct({"Character",0x340800},source,receiver,error));auto fields=receiver.properties();assert(map.init_properties(fields,error)&&map.load_defaults(fields,error));assert(map.set_property(fields,"charpropsname",desc.c_str(),error));assert(receiver.object.set_name(receiver.object.context,desc.c_str(),error));
  assert(!receiver.init_post(error)&&error=="Required genuine whole Visual InitPost");auto record=factory.find(receiver.object.identity);assert(record&&record->init->last_entry()==0x38be5c&&*record->fields.called1394==1&&*record->fields.properties_id13c8==static_cast<std::int16_t>(index));
  assert(record->properties->base==p.base&&record->properties->resolved==p.resolved);assert(record->inventory->inventory().properties()==record->properties&&record->actor->object->properties==record->properties&&record->actor->object->life==record->life);assert(record->inventory->inventory().character()==receiver.object.identity&&record->inventory->inventory().items().empty());
  auto model=record->actor->source_string(0x290);auto modelid=record->properties->resolved[3];if(modelid>=0&&static_cast<std::size_t>(modelid)<models.values.size())assert(model&&*model==models.values[modelid]);
  // Real table/property/actor storage; PF is explicitly an ordering observer,
  // not a claim of navigation initialization or collision-body construction.
  auto actor=record->actor;world::GameObjectVisualFieldBorrowV5 borrow;unsigned pf=0;
  assert(actor->visual_fields_v5(record,[&](std::string&){assert(actor->source_bounds_ready);++pf;return true;},borrow,error));
  assert(borrow.identity==receiver.object.identity&&borrow.position160==actor->object->position.data()&&borrow.rotation16c==actor->runtime.rotation.rotation&&borrow.static84==actor->source_bool_field(0x84));
  assert(fields.fields.write_vector3(fields.fields.context,0x160,{13.f,-7.f,9.f},error));
  assert(borrow.position160[0]==13.f&&borrow.position160[1]==-7.f);
  assert(fields.fields.write_vector3(fields.fields.context,0x16c,{0.2f,0.4f,0.6f},error));
  assert(borrow.rotation16c[1]==0.4f);
  assert(fields.fields.write_vector3(fields.fields.context,0x120,{2.f,3.f,4.f},error));
  assert(borrow.scale120[0]==2.f&&borrow.scale120[2]==4.f);
  const float box[6]{-20.f,-10.f,0.f,20.f,10.f,80.f};
  physical::CharacterOwnerBoundsInput expected_input{};std::copy_n(box,6,expected_input.mesh_box);std::copy_n(borrow.position160,3,expected_input.position);expected_input.collision_scale=record->properties->resolved[16];expected_input.previous_flat=actor->source_bounds_flat;
  physical::CharacterOwnerBounds expected{};assert(!dh2_character_owner_bounds(&expected,&expected_input));
  assert(borrow.apply_mesh_box(box,error)&&pf==1);
  for(unsigned n=0;n<6;++n){assert(actor->runtime.subobjects.local_bounds[n]==expected.relative_box[n]);assert(actor->runtime.subobjects.absolute_bounds[n]==expected.absolute_box[n]);}
  assert(actor->source_bool_field(0x15c)==&actor->source_bounds_flat);
  assert(fields.fields.write_bool(fields.fields.context,0x15c,1,error)&&actor->source_bounds_flat==1);
  world::GameObjectVisualFieldBorrowV5 missing;assert(actor->visual_fields_v5(record,{},missing,error));
  actor->source_bounds_ready=false;assert(!missing.apply_mesh_box(box,error)&&actor->source_bounds_ready&&error=="Required SAME Character UpdatePFObject after bounds prefix");
  assert(!receiver.init_post(error));if(desc.find("Priest")!=std::string::npos){++priest;std::cout<<"actual row "<<index<<" "<<desc<<" AItype "<<type<<" model "<<(model?*model:"")<<"\n";}++families;
 }
 assert(families>100&&priest>0&&rng.calls==0);std::cout<<"Character visual fields V5 native PASS "<<families<<" actual NPC rows; same constructor/sheets/inventory/FSM/model prefix; intentional Visual InitPost boundary, no whole level claim\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
