#include "../data.hpp"
#include "../class_preview_setup.hpp"
#include "../class_tables.hpp"
#include "../properties.hpp"
#include "../loot_tables_v2.hpp"
#include "../animation_tables.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace dh2::data;
using Raw=std::vector<std::uint8_t>;
Raw read(const std::string& folder,const std::string& file){std::ifstream f(folder+"/"+file,std::ios::binary);if(!f)throw std::runtime_error(file);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::string folder=argv[1],e;
 auto c=read(folder,"character_properties_pyarray.bin"),cn=read(folder,"character_properties_pyarraynames.bin"),cf=read(folder,"character_properties_pystructnames.bin");CharacterTable characters;
 if(!load_characters({c.data(),c.size()},{cn.data(),cn.size()},{cf.data(),cf.size()},characters,e))throw std::runtime_error(e);
 auto k=read(folder,"character_classes_pyarray.bin"),kn=read(folder,"character_classes_pyarraynames.bin"),kf=read(folder,"character_classes_pystructnames.bin");ClassTables classes;PropertyRules rules;
 if(!load_classes({k.data(),k.size()},{kn.data(),kn.size()},{kf.data(),kf.size()},classes,e)||!load_property_rules(characters,rules,e))throw std::runtime_error(e);
 auto l=read(folder,"loot_table_pyarray.bin"),ln=read(folder,"loot_table_pyarraynames.bin"),lf=read(folder,"loot_table_pystructnames.bin");LootTablesV2 owner;
 ItemTable item_debug;if(!load_items({l.data(),l.size()},{ln.data(),ln.size()},{lf.data(),lf.size()},item_debug,e))throw std::runtime_error(e);
 std::cout<<"ITEMDATA "<<item_debug.data_consumed<<" records bytes "<<l.size()<<" names "<<ln.size()<<" schema "<<lf.size()<<"\n";
 if(!owner.load({l.data(),l.size()},{ln.data(),ln.size()},{lf.data(),lf.size()},e))throw std::runtime_error(e);auto loot=owner.borrow();
 auto a=read(folder,"animations_pyarray.bin"),an=read(folder,"animations_pyarraynames.bin"),af=read(folder,"animations_pystructnames.bin"),d=read(folder,"animations_dictionary_pyarray.bin"),dn=read(folder,"animations_dictionary_pyarraynames.bin");Dictionary clips;AnimationTables anims;
 if(!load_dictionary({dn.data(),dn.size()},{d.data(),d.size()},clips,e)||!load_animation_tables({a.data(),a.size()},{an.data(),an.size()},{af.data(),af.size()},clips,anims,e))throw std::runtime_error(e);

 std::array<ClassPreviewDefinition,3> definitions;
 auto check=[](bool value){if(!value)throw std::runtime_error("Class definition regression failed");};
 check(class_preview_definitions(characters,classes,rules,loot,anims,clips,definitions,e));
 const int rows[]={263,325,290},tables[]={48,50,49},loots[]={165,213,174};
 const std::vector<int> expected[]={{1079,1073,1076,664,925},{1081,1075,1078,370,370,925},{1080,1074,1077,1025,925}};
 for(unsigned i=0;i<3;++i){const auto& d=definitions[i];
  check(d.row==rows[i]&&d.animation_table==tables[i]&&d.loot==loots[i]);
  check(d.starting_items.size()==expected[i].size());
  for(unsigned j=0;j<expected[i].size();++j)check(d.starting_items[j].item==expected[i][j]&&d.starting_items[j].quantity==(j+1==expected[i].size()?5:1));
  check(d.idle_clip.find("menu_idle_")!=std::string::npos&&d.select_clip.find("_02.bdae")!=std::string::npos&&d.template_clip.find("prince_template_anim.bdae")!=std::string::npos);
 }
 check(definitions[1].starting_items[3].module=="MC_RWeapon_Dagger_01"&&definitions[1].starting_items[4].module=="MC_RWeapon_Dagger_01");
 // Failure preserves the previous output, including its independently owned
 // strings. Missing animation data cannot publish a half-built class list.
 auto saved=definitions;AnimationTables absent;
 check(!class_preview_definitions(characters,classes,rules,loot,absent,clips,definitions,e));
 for(unsigned i=0;i<3;++i)check(definitions[i].idle_clip==saved[i].idle_clip&&definitions[i].starting_items.size()==saved[i].starting_items.size());
 auto original=l;l.resize(229059); // last variable ItemTypeList is truncated
 check(!owner.load({l.data(),l.size()},{ln.data(),ln.size()},{lf.data(),lf.size()},e));
 check(loot.loots().size()==339&&loot.consumed()==282872);
 loot={};check(!owner.load({l.data(),l.size()},{ln.data(),ln.size()},{lf.data(),lf.size()},e));
 loot=owner.borrow();check(loot.loots().size()==339);
 std::fill(original.begin(),original.end(),0); // owner data does not alias cache
 check(definitions[0].starting_items[0].module=="MC_Torso_default_warrior"&&loot.items().identifiers[1079]=="StartingSuit");
 std::cout<<"CLASS_DEFINITIONS PASS | classes 3 | starting entries 16 | duplicate Rogue daggers preserved | atomic failure and truncated ItemTypeList checked\n";
 for(const char* name:{"KnightPlayerBase","RoguePlayerBase","MagePlayerBase"}){
  auto it=std::find(characters.names.begin(),characters.names.end(),name);if(it==characters.names.end())throw std::runtime_error(name);
  PropertyState state;reset_properties(rules,state,&characters.rows.at(it-characters.names.begin()));if(!recalc_properties_with_class(classes,rules,state,e))throw std::runtime_error(e);
  std::cout<<"CLASS "<<name<<" row "<<it-characters.names.begin()<<"\n";
  for(int p:{0,1,2,9,12,13,14,26})std::cout<<"PROPERTY "<<p<<" "<<characters.fields[p]<<" base "<<state.base[p]<<" resolved "<<state.resolved[p]<<"\n";
  const int id=state.resolved[9];const auto& row=loot.loots().at(id);std::cout<<"LOOT "<<id<<" "<<loot.loot_names()[id]<<" type "<<row.roll_type<<" random "<<row.random_entries.size()<<" fixed "<<row.fixed_entries.size()<<" sub "<<row.sub_loots.size()<<"\n";
  for(const auto& entry:row.fixed_entries){std::cout<<"LOOTENTRY";for(int word:entry.words)std::cout<<" "<<word;std::cout<<"\n";
   for(const auto& item:loot.item_lists().at(entry.words[0])){
    const auto& definition=loot.items().rows.at(item.item);std::cout<<"ITEM "<<item.item<<" "<<loot.items().identifiers[item.item]<<" name "<<definition.name<<" qty "<<int(item.quantity)<<" prob "<<item.probability<<" type "<<item_type(definition)<<" words";
    for(int v:definition.record.words)std::cout<<" "<<v;std::cout<<"\n";
   }
  }
  for(const char* n:{"MenuIdle","MenuOnSelect","Template"}){
   const auto* seq=animation_state(anims,state.resolved[2],n);if(!seq)throw std::runtime_error(n);std::cout<<"ANIM "<<n<<" type "<<seq->type<<" loop "<<seq->loop<<"\n";
   for(const auto& step:seq->steps){auto* path=animation_clip(step,clips);std::cout<<"STEP "<<step.anim<<" redir "<<step.redir<<" speed "<<step.speed<<" path "<<(path?*path:"")<<"\n";}
  }
 }
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
