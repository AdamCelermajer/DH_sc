#include "class_preview_setup.hpp"
#include <algorithm>
#include <stdexcept>
namespace dh2::data {
bool class_preview_definitions(const CharacterTable& characters,const ClassTables& classes,
 const PropertyRules& rules,const LootTablesV2::Borrow& loot,const AnimationTables& animations,
 const Dictionary& clips,std::array<ClassPreviewDefinition,3>& output,std::string& error){
 try{
  if(!loot)throw std::runtime_error("Class preview loot owner unavailable");
  std::array<ClassPreviewDefinition,3> next;
  const char* names[]={"KnightPlayerBase","RoguePlayerBase","MagePlayerBase"};
  for(unsigned i=0;i<next.size();++i){auto& value=next[i];value.character=names[i];
   const auto row=std::find(characters.names.begin(),characters.names.end(),value.character);
   if(row==characters.names.end())throw std::runtime_error("Class preview character absent: "+value.character);
   value.row=std::int32_t(row-characters.names.begin());
   reset_properties(rules,value.properties,&characters.rows.at(value.row));
   if(!recalc_properties_with_class(classes,rules,value.properties,error))return false;
   value.loot=value.properties.resolved[9];value.animation_table=value.properties.resolved[2];
   if(value.loot<0||std::size_t(value.loot)>=loot.loots().size())throw std::runtime_error("Class preview starting loot absent");
   const auto& record=loot.loots()[value.loot];
   if(record.roll_type!=0||!record.random_entries.empty()||!record.sub_loots.empty())
    throw std::runtime_error("Class preview needs a random loot provider");
   for(const auto& entry:record.fixed_entries){
    // Only the original singleton, unpowered starting-item lists are
    // projected. Preserve order and duplicates (Rogue has two daggers).
    const std::int32_t plain[]={-1,0,-1,-1,0,0,0};
    if(!std::equal(std::begin(plain),std::end(plain),entry.words+1))
     throw std::runtime_error("Class preview needs a loot power provider");
    const auto list=entry.words[0];
    if(list<0||std::size_t(list)>=loot.item_lists().size())throw std::runtime_error("Class preview item list absent");
    const auto& items=loot.item_lists()[list];
    if(items.size()!=1||items[0].probability!=1||!items[0].quantity)
     throw std::runtime_error("Class preview needs item-list random selection");
    const auto& item=items[0];
    if(item.item<0||std::size_t(item.item)>=loot.items().rows.size())throw std::runtime_error("Class preview item absent");
    value.starting_items.push_back({item.item,item.quantity,loot.items().identifiers.at(item.item),loot.items().rows[item.item].name});
   }
   auto clip=[&](const char* state,bool looping){
    const auto* sequence=animation_state(animations,value.animation_table,state);
    if(!sequence||sequence->type!=0||sequence->steps.size()!=1||sequence->loop!=(looping?-1:0))
     throw std::runtime_error(std::string("Class preview animation sequence unsupported: ")+state);
    const auto& step=sequence->steps[0];const auto* path=animation_clip(step,clips);
    if(step.redir||step.speed!=1||!path||path->empty())throw std::runtime_error("Class preview animation resource absent");
    return *path;
   };
   value.idle_clip=clip("MenuIdle",true);value.select_clip=clip("MenuOnSelect",false);value.template_clip=clip("Template",false);
  }
  output=std::move(next);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
