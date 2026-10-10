#include "npc_profile_binding.hpp"
#include "../../actor_definitions.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
using namespace dh::foundation;using namespace dh::foundation::encounters;
void require(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
int main(int argc,char**argv){try{
 require(argc==2,"Repository root required");std::filesystem::path repo(argv[1]);
 AssetCatalog assets(repo/".local-inputs/windows-shared-assets");
 AssetCatalog fixtures(repo/"port/windows-foundation/features/encounters/fixtures");
 const std::string table="original-cache/data/pydata/";std::string error;
 auto records=assets.read(table+"character_properties_pyarray.bin");
 auto names=assets.read(table+"character_properties_pyarraynames.bin");
 auto fields=assets.read(table+"character_properties_pystructnames.bin");
 dh2::data::CharacterTable characters;
 require(dh2::data::load_characters({records.data(),records.size()},{names.data(),names.size()},
 {fields.data(),fields.size()},characters,error),error);
 auto tr=assets.read(table+"character_templates_pyarray.bin");
 auto tn=assets.read(table+"character_templates_pyarraynames.bin");
 auto ts=fixtures.read("character_templates_pystructnames.bin");
 dh2::data::CharacterTemplateTableV78 templates;
 require(templates.load({tr.data(),tr.size()},{tn.data(),tn.size()},{ts.data(),ts.size()},error),error);
 std::vector<ActorDefinition> definitions;
 require(load_actor_definitions(assets,"original-cache/data/scene/001_swamp.mlx",definitions,error),error);
 // Seed is an explicit test transport. Production must borrow application RNG.
 dh2::data::LootRandom8V2 random{12345,0};unsigned symbolic=0;
 for(const auto& d:definitions){auto t=d.properties.find("char_template");
  if(d.gametype!="Character"||t==d.properties.end()||t->second.empty())continue;
  auto n=d.properties.find("charpropsname");const std::string name=n==d.properties.end()?"":n->second;
  std::int16_t cache=-1,template_cache=-1;NpcProfileSelection selected,again;
  require(select_npc_profile(characters,templates,t->second,name,cache,template_cache,random,selected,error),error);
  const auto calls=random.calls;require(select_npc_profile(characters,templates,t->second,name,cache,
      template_cache,random,again,error),error);
  require(random.calls==calls&&again.row==selected.row&&again.name==selected.name,"Cached NPC profile re-rolled");
  require(template_cache==templates.find(t->second.c_str()),"Actual template cache was not written");
  const auto& row=templates.rows().at(static_cast<std::size_t>(template_cache));
  require(std::find(row.selected_ids.begin(),row.selected_ids.end(),cache)!=row.selected_ids.end(),"Selected row outside original weighted array");
  ++symbolic;std::cout<<d.sourceId<<" -> "<<selected.name<<"\n";
 }
 require(symbolic==21,"Original opening symbolic template fixture count changed");
 std::int16_t missing=-1,missing_template=-1;NpcProfileSelection output;
 auto before=random.calls;require(!select_npc_profile(characters,templates,"unbound_fixture_template","",missing,
 missing_template,random,output,error)&&missing==-1&&random.calls==before,"Unknown template fabricated a profile");
 std::cout<<"PASS originalSymbolicActors="<<symbolic<<" originalTemplates="<<templates.rows().size()
 <<" rngCalls="<<random.calls<<" cachedNoReroll=true noAdmission=true\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}

