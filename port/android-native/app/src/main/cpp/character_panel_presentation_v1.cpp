#include "character_panel_presentation_v1.hpp"
#include <cstring>
namespace dh2::android_ui {
namespace {
// Original AddPowerProperties jump table 0x3e3308, matching the frozen
// dh2_gear_power_v5 kernel. Attr is its 0..48 enum, not a property index.
constexpr unsigned targets[49][5]={
 {149,150,151,152,0},{149},{150},{151},{152},{38},{43},{95},{96},{97},
 {101},{102},{105},{106},{109},{110},{113},{114},{117},{118},
 {39},{44},{40},{45},{63},{60},{61},{71},{79,80},{50},{59},
 {74},{77},{75},{78},{76},{74,77,75,78,76},{132},{133},{165},{158},
 {166,169,167,170,168},{170},{167},{168},{169},{166},{195},{196}};
std::int32_t signed_word(std::uint32_t word){std::int32_t result;std::memcpy(&result,&word,4);return result;}
bool live_item(const model_renderer::PlayerGameplayBinding& p,const data::ItemInstanceV1& item,std::string& error){
 if(!p.active||!p.gear||!p.gear->ready()||!p.gear->inventory()){error="Power presentation requires the live inventory";return false;}
 for(const auto& cell:p.gear->inventory()->items())if(cell&&cell->item.get()==&item)return true;
 error="Power presentation item is detached from the live inventory";return false;
}
}
bool CharacterPanelPresentationV1::prepare(std::string& error){
 if(ready_)return true;
 auto table=[&](const char* prefix,auto load){
  std::vector<std::uint8_t> b,n,s;
  auto read=[&](const char* suffix,std::vector<std::uint8_t>& out){bool found=false;
   if(!assets_.read(std::string("data/pydata/")+prefix+suffix,found,out,error))return false;
   if(!found){error=std::string("Required presentation source table absent: ")+prefix+suffix;return false;}return true;};
  return read("_pyarray.bin",b)&&read("_pyarraynames.bin",n)&&read("_pystructnames.bin",s)&&
   load(data::Bytes{b.data(),b.size()},data::Bytes{n.data(),n.size()},data::Bytes{s.data(),s.size()},error);
 };
 if(!table("item_powers",[&](auto b,auto n,auto s,auto& e){return power_tables_.load(b,n,s,e);})||
    !table("faeries",[&](auto b,auto n,auto s,auto& e){return faery_tables_.load(b,n,s,e);}))return false;
 ready_=true;return true;
}
bool CharacterPanelPresentationV1::raw(ui::HudTextV1& text,const ui::HudTextEnvironmentV1& environment,
 std::int32_t id,std::string& result,std::string& error){
 bool is_null=false;if(!text.integer_string(id,environment.localization,result,is_null,error))return false;
 if(is_null){error="Original presentation StringManager returned a null pointer";return false;}return true;
}
bool CharacterPanelPresentationV1::describe(std::int32_t id,ui::HudTextV1& text,
 const ui::HudTextEnvironmentV1& environment,PowerPresentationV1& output,std::string& error){
 if(!prepare(error))return false;auto tables=power_tables_.borrow();
 if(id<0||std::size_t(id)>=tables.rows().size()||std::size_t(id)>=tables.names().size()){error="Actual item power ID outside source table";return false;}
 const auto& row=tables.rows()[std::size_t(id)];PowerPresentationV1 next;
 next.id=id;next.identifier=tables.names()[std::size_t(id)];next.palette=row.scalars.palette;
 next.special_effect=row.scalars.special_effect;next.description_id=row.scalars.description;next.authored_sorting_order=row.scalars.sorting_order;next.properties=row.properties;
 std::string format;if(!raw(text,environment,row.scalars.description,format,error))return false;
 if(row.properties.empty())next.description=format;
 else {
  std::vector<ui::HudTextVariantV1> arguments;arguments.reserve(row.properties.size());
  for(const auto& property:row.properties){auto value=property.value;
   arguments.push_back({static_cast<float>(value)*0.00390625f,value>>8,nullptr});}
  bool changed=false;if(!text.parse_ex(format.c_str(),arguments.data(),arguments.size(),environment,next.description,changed,error))return false;
 }
 output=std::move(next);return true;
}
bool CharacterPanelPresentationV1::power_details(const model_renderer::PlayerGameplayBinding& player,
 const data::ItemInstanceV1& item,ui::HudTextV1& text,const ui::HudTextEnvironmentV1& environment,
 std::vector<PowerPresentationV1>& output,std::string& error){
 if(!live_item(player,item,error))return false;
 std::vector<PowerPresentationV1> next;next.reserve(item.powers.size());
 for(auto id:item.powers){PowerPresentationV1 descriptor;if(!describe(id,text,environment,descriptor,error))return false;
  auto* characters=player.design.characters();
  for(const auto& property:descriptor.properties){
   PowerAttributePresentationV1 attribute;
   attribute.property_id=property.type;attribute.raw_value=property.value;attribute.flags=property.extra;
   attribute.source_number=static_cast<float>(property.value)*0.00390625f;
   if(property.type>=0&&property.type<49)for(unsigned destination:targets[property.type]){
    if(!destination)break;
    auto left=destination;
    if(property.type>=7&&property.type<=9)left+=3;
    else if((property.type>=10&&property.type<=19)||property.type==28)left+=2;
    if(!characters||destination>=characters->fields.size()||left>=characters->fields.size()){error="Power destination outside live CharacterProperties schema";return false;}
    attribute.right_property_ids.push_back(int(destination));attribute.left_property_ids.push_back(int(left));
    attribute.right_property_names.push_back(characters->fields[destination]);attribute.left_property_names.push_back(characters->fields[left]);
   }
   // Unknown Attr values are ignored by the source kernel, so retain the
   // enum/value and leave destinations empty instead of inventing an effect.
   if(attribute.right_property_names.size()==1)attribute.property_name=attribute.right_property_names.front();
   descriptor.attributes.push_back(std::move(attribute));
  }
  next.push_back(std::move(descriptor));
 }
 output=std::move(next);return true;
}
bool CharacterPanelPresentationV1::powers(const model_renderer::PlayerGameplayBinding& player,
 const data::ItemInstanceV1& item,ui::HudTextV1& text,const ui::HudTextEnvironmentV1& environment,
 const std::vector<data::ItemPowerInstanceV5>*& output,std::string& error){
 std::vector<PowerPresentationV1> descriptors;if(!power_details(player,item,text,environment,descriptors,error))return false;
 std::vector<data::ItemPowerInstanceV5> next;next.reserve(descriptors.size());
 // QueriesOwner reads only ID/description. This field reports authored row
 // metadata, not positional ItemPower state: AddPower preserves positional
 // sort words during swaps. Never use this derived query view for mutation.
 for(auto& d:descriptors)next.push_back({d.id,d.authored_sorting_order,std::move(d.description)});
 auto& view=views_[&item];view=std::move(next);output=&view;return true;
}
bool CharacterPanelPresentationV1::faery(const model_renderer::PlayerGameplayBinding& player,std::uint32_t slot,
 ui::HudTextV1& text,const ui::HudTextEnvironmentV1& environment,FaeryPresentationV1& output,std::string& error){
 if(!player.active||!player.properties||!player.properties->resolved||!prepare(error)){if(error.empty())error="Faery presentation requires the live player";return false;}
 auto table=faery_tables_.borrow();auto selected=player.properties->resolved[29];
 if(selected<0||std::size_t(selected)>=table.lists().size())selected=0;
 if(table.lists().empty()||slot>=table.lists()[std::size_t(selected)].size()){error="Faery slot outside actual selected source list";return false;}
 auto id=table.lists()[std::size_t(selected)][slot];
 if(id<0||std::size_t(id)>=table.faeries().size()||std::size_t(id)>=table.faery_names().size()){error="Faery source list points outside original rows";return false;}
 const auto& row=table.faeries()[std::size_t(id)];FaeryPresentationV1 next;
 next.slot=int(slot);next.table_id=id;next.identifier=table.faery_names()[std::size_t(id)];
 next.description_id=signed_word(row.scalar.words[1]);next.element=signed_word(row.scalar.words[2]);
 next.model_id=signed_word(row.scalar.words[3]);next.name_id=signed_word(row.scalar.words[4]);
 next.script=row.script;next.spell_type=signed_word(row.scalar.words[7]);next.type=signed_word(row.scalar.words[8]);
 if(!raw(text,environment,next.name_id,next.name,error)||!raw(text,environment,next.description_id,next.description,error))return false;
 output=std::move(next);return true;
}
}
