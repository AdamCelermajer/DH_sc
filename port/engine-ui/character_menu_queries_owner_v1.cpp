#include "character_menu_queries_owner_v1.hpp"
#include "character_menu_item_actions_v1.hpp"
#include "character_menu_save_actions_v1.hpp"
#include "character_menu_faery_actions_v1.hpp"
#include "item_text_varargs_v5.hpp"
#include "character_menu_inventory_order_v1.hpp"
#include "../level-world/player_equipment_queries_v1.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <limits>
#include <stdexcept>
namespace dh2::ui {
namespace {
std::int32_t signed_word(std::uint32_t w){std::int32_t s;std::memcpy(&s,&w,4);return s;}
std::int32_t add(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)+std::uint32_t(b));}
std::int32_t mul(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)*std::uint32_t(b));}
std::int32_t integer(double n){return std::isnan(n)?0:n>=2147483647.?INT32_MAX:n<=-2147483648.?INT32_MIN:std::int32_t(n);}
bool number(CharacterMenuCallV1& c,std::size_t i,double& out,std::string& e){if(i>=c.arguments.size()){e="Source callback argument projection truncated";return false;}const auto& v=c.arguments[i];if(v.kind==2){out=v.number;return true;}if(!c.number){e="Source AS numeric conversion provider required";return false;}return c.number(v,out,e);}
bool boolean(CharacterMenuCallV1& c,std::size_t i,bool& out,std::string& e){if(i>=c.arguments.size()){e="Source callback argument projection truncated";return false;}const auto& v=c.arguments[i];if(v.kind==1){out=v.boolean;return true;}if(v.kind==0){out=false;return true;}if(!c.boolean){e="Source AS boolean conversion provider required";return false;}return c.boolean(v,out,e);}
}
CharacterMenuValueV1 CharacterMenuValueV1::numeric(double n){CharacterMenuValueV1 v;v.kind=2;v.number=n;return v;}
CharacterMenuValueV1 CharacterMenuValueV1::flag(bool b){CharacterMenuValueV1 v;v.kind=1;v.boolean=b;return v;}
CharacterMenuValueV1 CharacterMenuValueV1::string(std::string s){CharacterMenuValueV1 v;v.kind=4;v.text=std::move(s);return v;}
CharacterMenuValueV1 CharacterMenuValueV1::reference(std::uintptr_t p){CharacterMenuValueV1 v;v.kind=5;v.object=p;return v;}
bool character_menu_item_equippable_v1(const data::ItemRecord164& item,const data::PropertySheet& sheet,std::int32_t saved_class,const data::CharacterTable& actors,bool online,bool remote,bool& out,std::string& e){
 if(online&&remote){out=true;return true;}auto required_class=saved_class;
 auto req=item.words[34];if(req>=1&&req<=9){const char* names[]{"KnightPlayerBase","KnightPlayerBase_Berserker","KnightPlayerBase_Paladin","RoguePlayerBase","RoguePlayerBase_Assassin","RoguePlayerBase_Archer","MagePlayerBase","MagePlayerBase_Necromancer","MagePlayerBase_Illusionist"};auto at=std::find(actors.names.begin(),actors.names.end(),names[req-1]);if(at==actors.names.end()){e="Source requirement class name unavailable in actual cache";return false;}required_class=std::int32_t(at-actors.names.begin());}
 if(sheet[19]<signed_word(std::uint32_t(item.words[29])<<8)){out=false;return true;}
 for(unsigned j=0;j<4;++j)if(add(sheet[149+j],sheet[153+j])<signed_word(std::uint32_t(item.words[30+j])<<8)){out=false;return true;}
 out=saved_class==required_class;return true;
}
CharacterMenuQueriesOwnerV1::CharacterMenuQueriesOwnerV1(CharacterMenuQueriesGraphV1 g):graph_(std::move(g)){if(!graph_.owner||!graph_.actions)throw std::invalid_argument("Menu queries require retained action/provider graph");}
bool CharacterMenuQueriesOwnerV1::player(std::int32_t index,bool remote,bool skills,std::uintptr_t& actor,std::string& e)const{
 if(!graph_.player){e="Source NativeGetPlayerChar provider required";return false;}if(!graph_.player(index,remote,actor,e))return false;if(!actor)return true;
 if(!graph_.actions->validate_graph(skills,e))return false;
 if(actor!=graph_.actions->bindings().equipment->inventory()->character()){e="Menu query selected a player outside the borrowed graph";return false;}return true;
}
bool CharacterMenuQueriesOwnerV1::write(CharacterMenuCallV1& c,std::uintptr_t object,const char* key,CharacterMenuValueV1 v,std::string& e)const{if(!object||!c.member){e="Source AS member receiver required";return false;}return c.member(object,key,v,e);}
bool CharacterMenuQueriesOwnerV1::item_name(const data::ItemInstanceV1& item,std::string& out,std::string& e)const{
 if(!graph_.font_palette){e="Genuine FontPalette owner required for colored inventory names";return false;}
 std::int32_t row=2;auto count=item.powers.size();if(count<=4){const char* keys[]{"zero","one","two","three","four"};auto& loc=graph_.text_environment.localization;std::uint32_t value;if(!loc.constant||!loc.constant(loc.context,"ItemPowerColor",keys[count],value,e))return false;row=signed_word(value);}
 std::uint32_t color;if(!graph_.font_palette->text_color(row,color,e))return false;char hexadecimal[9];std::snprintf(hexadecimal,sizeof(hexadecimal),"%X",color);out="<font color='#";out+=hexadecimal;out+="'>";out+=item.name;out+="</font>";return true;
}
bool CharacterMenuQueriesOwnerV1::auto_slot(std::uint32_t slot,std::string& e){
 auto& graph=graph_.actions->bindings();if(slot>=9||!graph_.item_equippable){e="Source automatic-slot requirement provider or slot unavailable";return false;}
 const auto& inventory=*graph.equipment->inventory();struct Entry{std::uint32_t index;const data::ItemInstanceV1* item;const data::ItemRecord164* row;};std::vector<Entry> candidates;
 for(unsigned j=0;j<inventory.items().size();++j){auto& cell=inventory.items()[j];if(!cell||!cell->item)continue;auto* row=data::item(inventory.table(),cell->item->id);if(!row){e="Source automatic-slot metadata unavailable";return false;}if(character_menu_slot_candidate_v1(*cell->item,row->record,graph.equipment->properties()->resolved,slot))candidates.push_back({j,cell->item.get(),&row->record});}
 std::sort(candidates.begin(),candidates.end(),[&](auto& a,auto& b){return character_menu_item_value_less_v1(*a.item,*a.row,*b.item,*b.row,graph.stats.actor_index);});
 for(auto& entry:candidates){bool available;if(!graph_.item_equippable(*entry.item,available,e))return false;if(!available)continue;bool equipped;if(!inventory.is_equipped(entry.index,equipped,e))return false;if(equipped)continue;return graph_.actions->equip(slot,entry.index,e);}
 // All caller paths reach this with an empty slot. The inventory operation is
 // a proven no-op; its Character wrapper delivers the required identical
 // gear-properties/requirements/Skin/vitals continuation on the same graph.
 auto* cell=inventory.equipment()[slot==1||slot==2?inventory.current_equipment():0][slot];if(cell){e="Source empty automatic-slot continuation projection mismatch";return false;}return graph_.actions->unequip(slot,e);
}
bool CharacterMenuQueriesOwnerV1::stats(CharacterMenuCallV1& c,std::uintptr_t actor,std::uintptr_t object,std::string& e){
 const auto& g=graph_.actions->bindings();auto& state=*g.equipment->properties();const auto* save=g.save;
 if(save&&save->character()!=actor){e="Stats save belongs to a different player";return false;}
 if(!graph_.text||!graph_.characters){e="Stats genuine character/localization owners required";return false;}
 data::CombatantView combat{};if(!g.equipment->combat_view(combat,e))return false;std::int32_t damage[2];for(unsigned j=0;j<2;++j)if(dh2_combat_bonus(&combat,j,damage+j)){e="Source damage bonus projection invalid";return false;}
 auto prop=[&](unsigned i){return state.resolved[i]>>8;};
 auto bonus=[&](unsigned first,unsigned dual){if(combat.main_damage_class<0)return std::int32_t(0);auto v=state.resolved[first+unsigned(combat.main_damage_class)];if(combat.dual_wield)v=add(v,state.resolved[dual]);return v>>8;};
 const auto attack=add(prop(50),bonus(51,58));const auto dmin=add(prop(79),damage[0]>>8),dmax=add(prop(80),damage[0]>>8),omin=add(prop(81),damage[1]>>8),omax=add(prop(82),damage[1]>>8);
 if(!write(c,object,"Name",CharacterMenuValueV1::string(save?save->name():""),e))return false;
 auto class_id=save?save->class_id():-1;std::int32_t string_id=state.resolved[5];if(class_id!=-1){if(class_id<0||std::size_t(class_id)>=graph_.characters->rows.size()){e="Source saved class row absent";return false;}string_id=graph_.characters->rows[class_id][5];}
 std::string label;bool null;if(!graph_.text->integer_string(string_id,graph_.text_environment.localization,label,null,e))return false;
 if(!write(c,object,"Class",CharacterMenuValueV1::string(null?"":label),e))return false;
 const double icon=class_id>=290&&class_id<=292?3.:class_id>=325&&class_id<=327?2.:1.;if(!write(c,object,"Icon",CharacterMenuValueV1::numeric(icon),e))return false;
 struct Entry{const char* key;unsigned property;};
 const Entry before[]={{"Level",19},{"HP",36},{"HP_Bonus",37},{"Max_HP",38},{"MP",41},{"MP_Bonus",42},{"Max_MP",43},{"XP",33},{"Max_XP",34},{"Stat_Strength",149},{"Stat_Dexterity",150},{"Stat_Endurance",151},{"Stat_Energy",152},{"Stat_Points",148}};
 for(auto& q:before)if(!write(c,object,q.key,CharacterMenuValueV1::numeric(prop(q.property)),e))return false;
 const auto crit=combat.main_damage_class<0?prop(63):add(prop(63),prop(64+unsigned(combat.main_damage_class)));
 if(!write(c,object,"Rating_Attack",CharacterMenuValueV1::numeric(attack),e)||!write(c,object,"Rating_Critical",CharacterMenuValueV1::numeric(crit),e))return false;
 const Entry defense[]={{"Rating_Defense",59},{"Rating_Dodge",60},{"Rating_Block",61},{"Resistance_Fire",74},{"Resistance_Earth",77},{"Resistance_Water",75},{"Resistance_Air",78},{"Resistance_Lightning",76}};
 for(auto& q:defense)if(!write(c,object,q.key,CharacterMenuValueV1::numeric(prop(q.property)),e))return false;
 if(!write(c,object,"Damage_Min_Main_Hand",CharacterMenuValueV1::numeric(dmin),e)||!write(c,object,"Damage_Max_Main_Hand",CharacterMenuValueV1::numeric(dmax),e))return false;
 const Entry elemental[]={{"Damage_Elemental_Min_Main_Hand",95},{"Damage_Elemental_Max_Main_Hand",96},{"Damage_Elemental_Type_Main_Hand",97}};
 for(auto& q:elemental)if(!write(c,object,q.key,CharacterMenuValueV1::numeric(prop(q.property)),e))return false;
 if(!write(c,object,"Damage_Min_Off_Hand",CharacterMenuValueV1::numeric(omin),e)||!write(c,object,"Damage_Max_Off_Hand",CharacterMenuValueV1::numeric(omax),e))return false;
 const Entry elements[]={{"Damage_Elemental_Min_Off_Hand",98},{"Damage_Elemental_Max_Off_Hand",99},{"Damage_Elemental_Type_Off_Hand",100},{"Damage_Fire_Min_Main_Hand",101},{"Damage_Fire_Max_Main_Hand",102},{"Damage_Water_Min_Main_Hand",105},{"Damage_Water_Max_Main_Hand",106},{"Damage_Lightning_Min_Main_Hand",109},{"Damage_Lightning_Max_Main_Hand",110},{"Damage_Air_Min_Main_Hand",117},{"Damage_Air_Max_Main_Hand",118},{"Damage_Earth_Min_Main_Hand",113},{"Damage_Earth_Max_Main_Hand",114}};
 for(auto& q:elements)if(!write(c,object,q.key,CharacterMenuValueV1::numeric(prop(q.property)),e))return false;
 auto& inv=*g.equipment->inventory();const auto set=unsigned(inv.current_equipment());auto row=[&](unsigned slot)->const data::ItemRecord164*{auto* cell=inv.equipment()[set][slot];auto* item=cell&&cell->item?data::item(inv.table(),cell->item->id):nullptr;return item?&item->record:nullptr;};
 player::EquipmentQueries12V1 weapons{};if(dh2_equipment_queries_v1(&weapons,row(1),row(2),0)){e="Source inventory weapon queries invalid";return false;}bool two;if(!inv.has_two_hander(false,two,e))return false;
 const char* keys[]{"IsWeaponTwoHanded","HasOffHandWeapon","HasStaff","HasBow"};bool flags[]{two,bool(weapons.flags&player::query_dual),bool(weapons.flags&player::query_staff),bool(weapons.flags&player::query_bow)};
 for(unsigned j=0;j<4;++j)if(!write(c,object,keys[j],CharacterMenuValueV1::flag(flags[j]),e))return false;
 const Entry tail[]={{"Menu_Average_Melee_To_Hit",207},{"Physical_Armor",71},{"Spell_Rating_Dodge",164},{"Menu_Melee_Damage_Reduction",209},{"Spell_Rating_Critical",165},{"Menu_Average_Spell_To_Hit",208},{"Spell_Damage_Bonus_Fire",166},{"Spell_Damage_Bonus_Earth",169},{"Spell_Damage_Bonus_Water",167},{"Spell_Damage_Bonus_Air",170},{"Spell_Damage_Bonus_Lightning",168},{"Regen_HP",39},{"Regen_MP",44},{"Leech_HP",132},{"Leech_MP",133},{"Special_Loot_Gold_Multiplier",195},{"Special_Loot_Magical_Chance",196},{"Stun_Resist_Chance",138}};
 for(auto& q:tail)if(!write(c,object,q.key,CharacterMenuValueV1::numeric(prop(q.property)),e))return false;
 c.result=CharacterMenuValueV1::reference(object);return true;
}
bool CharacterMenuQueriesOwnerV1::item_details(CharacterMenuCallV1& c,std::uintptr_t object,std::uint32_t index,std::string& e){
 const auto& g=graph_.actions->bindings();const auto& inv=*g.equipment->inventory();if(index>=inv.items().size()||!inv.items()[index]||!inv.items()[index]->item)return true;const auto& item=*inv.items()[index]->item;const auto* row=data::item(inv.table(),item.id);if(!row){e="Source item metadata absent";return false;}
 if(!graph_.text||!graph_.item_equippable){e="Item details require genuine text and requirement providers";return false;}
 auto& loc=graph_.text_environment.localization;std::uint32_t mult;if(!loc.constant||!loc.constant(loc.context,"CharacterDesign","TransmuteMultiplier",mult,e))return false;
 // Source _GetProperty reads cached property197 raw, then fixed-point products
 // wrap at each ARM mul. Ordinary player inventory has buy/sell factors0.
 const auto bonus=add(g.equipment->properties()->resolved[197],256);auto transmute=mul(signed_word(mult),mul(signed_word(std::uint32_t(item.value)<<8),bonus)>>8)>>16;transmute=std::max(1,transmute);
 std::int32_t values[]{item.value,1,1,transmute};std::string formatted[4];for(unsigned j=0;j<4;++j){data::ItemTextArgumentV5 a{float(values[j]),values[j],nullptr};struct Context{CharacterMenuQueriesOwnerV1* owner;std::string output;}ctx{this,{}};
  HudTextServicesV1 svc{&ctx,[](void* p,const HudTextRequestV1& q,HudTextResponseV1& r,std::string& e){auto& c=*static_cast<Context*>(p);auto& owner=*c.owner;auto& env=owner.graph_.text_environment;if(q.operation==hud_text_pack_v1){r.value=owner.graph_.text->pack();return true;}if(q.operation==hud_text_constant_v1){std::uint32_t v;if(!env.localization.constant||!env.localization.constant(env.localization.context,q.group,q.key,v,e))return false;r.value=signed_word(v);return true;}if(q.operation==hud_text_integer_string_v1){bool null;if(!owner.graph_.text->integer_string(q.value,env.localization,c.output,null,e))return false;r.text=null?nullptr:c.output.c_str();return true;}e="Item price formatter requires unrecovered application operation";return false;}};bool changed;if(!item_text_varargs_v5("^d",&a,1,svc,formatted[j],changed,e))return false;}
 bool equippable;if(!graph_.item_equippable(item,equippable,e))return false;
 const std::vector<data::ItemPowerInstanceV5>* powers=nullptr;if(!item.powers.empty()){if(!graph_.powers||!graph_.powers(item,powers,e)||!powers||powers->size()!=item.powers.size()){e="Actual item power presentation binding required";return false;}for(unsigned j=0;j<powers->size();++j)if((*powers)[j].id!=item.powers[j]){e="Item power presentation detached from inventory";return false;}}
 const char* numbers[]{"ItemValue","ItemBuyValue","ItemSellValue","ItemTransmuteValue"};for(unsigned j=0;j<4;++j)if(!write(c,object,numbers[j],CharacterMenuValueV1::numeric(values[j]),e))return false;
 const char* strings[]{"ItemValueString","ItemBuyValueString","ItemSellValueString","ItemTransmuteValueString"};for(unsigned j=0;j<4;++j)if(!write(c,object,strings[j],CharacterMenuValueV1::string(formatted[j]),e))return false;
 if(!write(c,object,"ItemEquippable",CharacterMenuValueV1::flag(equippable),e)||!write(c,object,"ItemReqsDesc",CharacterMenuValueV1::string(item.requirements),e)||!write(c,object,"ItemStatsDesc",CharacterMenuValueV1::string(item.description),e)||!write(c,object,"ItemMagics",CharacterMenuValueV1::numeric(item.powers.size()),e)||!write(c,object,"ItemIcon",CharacterMenuValueV1::string(row->icon_name),e)||!write(c,object,"IsStackable",CharacterMenuValueV1::flag(std::uint8_t(row->record.words[7])!=0),e))return false;
 if(powers)for(unsigned j=0;j<powers->size();++j){auto key="ItemPowers"+std::to_string(j)+"Desc";if(!write(c,object,key.c_str(),CharacterMenuValueV1::string((*powers)[j].description),e))return false;}
 return true;
}
struct CharacterMenuQueriesOwnerV1::SkillFrame {
 CharacterMenuQueriesOwnerV1& owner;CharacterMenuCallV1& call;std::string& error;
 std::deque<std::string> strings;std::vector<std::unique_ptr<std::vector<HudTextVariantV1>>> arguments;
 std::deque<HudInitSkill96> records;
 const char* pin(std::string s){strings.push_back(std::move(s));return strings.back().c_str();}
 std::vector<HudTextVariantV1>* args(std::uintptr_t id){for(auto& a:arguments)if(reinterpret_cast<std::uintptr_t>(a.get())==id)return a.get();return nullptr;}
 bool invoke(const HudInitRequest64& q,HudInitResponse32& r){
  auto& g=owner.graph_;auto& a=g.actions->bindings();using Op=HudInitOperation;
  auto argument=[&]()->const CharacterMenuValueV1*{if(q.index>=call.arguments.size()){error="Source skill argument projection truncated";return nullptr;}return &call.arguments[q.index];};
  switch(q.operation){
   case Op::argument_type:{auto* v=argument();if(!v)return false;r.value=v->kind;r.identity=v->object;return true;}
   case Op::argument_is_number:{auto* v=argument();if(!v)return false;r.value=v->kind==2;return true;}
   case Op::argument_number:return number(call,q.index,r.number,error);
   case Op::argument_boolean:{bool b;if(!boolean(call,q.index,b,error))return false;r.value=b;return true;}
   case Op::cast_object:{auto* v=argument();if(!v)return false;r.identity=v->kind==5?v->object:0;return true;}
   case Op::cast_array:{auto* v=argument();if(!v)return false;if(v->kind!=5){r.identity=0;return true;}if(!call.array){error="Source AS array cast provider required";return false;}if(!call.array(v->object,error))return false;r.identity=v->object;return true;}
   case Op::player:return owner.player(q.value,q.other!=0,true,r.identity,error);
   case Op::skill_id:r.value=a.save->skill_id(q.index);return true;
   case Op::character_skill:{auto* s=g.actions->skill_record(std::int32_t(q.index),error);if(!s)return false;HudInitSkill96 row{};row.display_properties=s->display_props.data();row.display_count=std::uint32_t(s->display_props.size());row.required_level=signed_word(s->scalar.words[8]);row.current_text=signed_word(s->scalar.words[12]);row.description_text=signed_word(s->scalar.words[13]);row.name_text=signed_word(s->scalar.words[16]);row.next_text=signed_word(s->scalar.words[17]);row.faery_dependent=std::uint8_t(s->scalar.words[6]);row.assignable=std::uint8_t(s->scalar.words[11]);row.icon=s->icon.c_str();row.identity=reinterpret_cast<std::uintptr_t>(s);records.push_back(row);r.identity=reinterpret_cast<std::uintptr_t>(&records.back());return true;}
   case Op::character_level:r.value=a.equipment->properties()->resolved[19]>>8;return true;
   case Op::skill_level:r.value=a.save->skill_level(q.index);return true;
   case Op::skill_slot:r.value=q.other?a.save->skill_slot(q.index):a.save->skill_in_slot(q.index);return true;
   case Op::unlocked_difficulty:r.value=a.save->unlocked_difficulty();return true;
   case Op::can_increment:{bool result;if(g.can_increment){if(!g.can_increment(q.subject,q.index,result,error))return false;}else if(!g.actions->can_increment(q.index,result,error))return false;r.value=result;return true;}
   case Op::current_faery:case Op::faery_level:{if(!g.difficulty){error="Source selected difficulty provider required";return false;}std::int32_t tier;if(!g.difficulty(tier,error))return false;if(tier<0||tier>2){error="Source difficulty assertion domain unsupported";return false;}r.value=q.operation==Op::current_faery?a.save->current_faery(tier):a.save->faery_level(q.index,tier);return true;}
   case Op::character_faery_offset:if(!g.faery_offset){error="Source faery character text offset provider required";return false;}return g.faery_offset(q.subject,q.index,r.value,error);
   case Op::constant:{if(!g.text_environment.localization.constant){error="Genuine design constant provider required";return false;}std::uint32_t v;if(!g.text_environment.localization.constant(g.text_environment.localization.context,q.text,q.name,v,error))return false;r.value=signed_word(v);return true;}
   case Op::string_symbol:{if(!g.text){error="Genuine localized text owner required";return false;}std::string text;bool null;if(!g.text->integer_string(q.value,g.text_environment.localization,text,null,error))return false;r.text=null?nullptr:pin(std::move(text));return true;}
   case Op::arguments_create:{auto out=std::make_unique<std::vector<HudTextVariantV1>>();r.identity=reinterpret_cast<std::uintptr_t>(out.get());arguments.push_back(std::move(out));return true;}
   case Op::arguments_append:{auto* out=args(q.subject);if(!out){error="Source VarArgs handle outside this invocation";return false;}out->push_back({float(q.number),q.value,nullptr});return true;}
   case Op::skill_info:{if(!g.temporary||!g.temporary_binding){error="Real V3 shared temporary property binding required";return false;}if(!g.temporary_binding(*a.skills,g.temporary,error))return false;float fraction=0;if(a.skills->info(q.index,std::uint32_t(q.value),&fraction)<0){error=a.skills->error();return false;}r.fraction=fraction;return true;}
   case Op::property:if(!g.temporary||q.index>=224){error="Source skill temporary property projection unavailable";return false;}r.value=(*g.temporary)[q.index];return true;
   case Op::parse_text:{auto* in=args(q.subject);if(!in||!g.text){error="Genuine source VarArgs/text owner required";return false;}std::string output;bool changed;if(!g.text->parse_ex(q.text,in->data(),in->size(),g.text_environment,output,changed,error))return false;r.text=pin(std::move(output));return true;}
   case Op::write_member:{CharacterMenuValueV1 value=q.type==3?CharacterMenuValueV1::string(q.text?q.text:""):q.type==1?CharacterMenuValueV1::flag(q.value!=0):CharacterMenuValueV1::numeric(q.number);return owner.write(call,q.object,q.name,std::move(value),error);}
   case Op::array_push:if(!call.append){error="Source AS array append provider required";return false;}return call.append(q.object,CharacterMenuValueV1::numeric(q.number),error);
   case Op::result_boolean:call.result=CharacterMenuValueV1::flag(q.value!=0);return true;
   case Op::result_number:call.result=CharacterMenuValueV1::numeric(q.number);return true;
   case Op::result_object:call.result=CharacterMenuValueV1::reference(q.object);return true;
   default:error="Operation outside source character menu query domain";return false;
  }
 }
 static int service(void* p,const HudInitRequest64* q,HudInitResponse32* r){return static_cast<SkillFrame*>(p)->invoke(*q,*r)?1:0;}
};
bool CharacterMenuQueriesOwnerV1::dispatch(const char* name,CharacterMenuCallV1& c,std::string& e){
 e.clear();if(!name){e="Null native callback name";return false;}auto equal=[&](const char* v){return !std::strcmp(name,v);};auto& g=graph_.actions->bindings();auto count=c.arguments.size();
 if(equal("NativeSaveGame")){
  double selected=0;if(count==1&&!number(c,0,selected,e))return false;
  std::uintptr_t actor;if(!player(integer(selected),false,false,actor,e))return false;if(!actor)return true;
  if(!graph_.save_actions||graph_.save_actions->bindings().actions!=graph_.actions){e="Required same-graph NativeSaveGame owner unavailable";return false;}
  return graph_.save_actions->save(e);
 }
 if(equal("NativeHUDSetActiveFaery")){
  if(count!=2||c.arguments[0].kind!=2||std::isnan(c.arguments[0].number)||c.arguments[1].kind!=2)return true;
  std::uintptr_t actor;if(!player(integer(c.arguments[1].number),false,false,actor,e))return false;if(!actor)return true;
  if(!graph_.faery_actions||graph_.faery_actions->bindings().actions!=graph_.actions){e="Required same-graph ChangeFaery owner unavailable";return false;}
  if(!graph_.faery_actions->set_active(std::uint32_t(integer(c.arguments[0].number)),e))return false;c.result={};return true;
 }
 if(equal("NativeInvTransmuteItem")||equal("NativeInvDropItem")){
  const bool transmute=equal("NativeInvTransmuteItem");double item_number,player_number=0;
  if(transmute){if(count!=2||c.arguments[0].kind!=2||std::isnan(c.arguments[0].number)||c.arguments[1].kind!=2)return true;item_number=c.arguments[0].number;player_number=c.arguments[1].number;}
  else if(!number(c,0,item_number,e))return false;
  std::uintptr_t actor;if(!player(integer(player_number),false,false,actor,e))return false;if(!actor)return true;
  if(!graph_.item_actions){e="Required source item-action owner unavailable";return false;}
  if(graph_.item_actions->bindings().inventory!=g.equipment->inventory()){e="Item-action owner is not the same authoritative menu inventory";return false;}
  if(transmute){std::int32_t ignored;if(!graph_.item_actions->transmute(std::uint32_t(integer(item_number)),false,ignored,e))return false;c.result={};return true;}
  return graph_.item_actions->drop(std::uint32_t(integer(item_number)),e);
 }
 if(equal("NativeGetSkillDetails")||equal("NativeSkillGetEquipedSkillsIDs")||equal("NativeHUDGetActiveFaery")){
  SkillFrame frame{*this,c,e};HudInitInput16 input{reinterpret_cast<std::uintptr_t>(&c),std::uint32_t(count),std::uint32_t(count)};HudInitServices16 services{&frame,SkillFrame::service};unsigned entry=equal("NativeGetSkillDetails")?1:equal("NativeSkillGetEquipedSkillsIDs")?0:2;auto status=dh2_ui_hud_initialization_v1(&input,entry,&services);if(status<0){if(e.empty())e="Source HUD query malformed projection";return false;}return true;
 }
 if(equal("NativeGetPlayerStats")){double n;if(count<2||!number(c,1,n,e))return false;std::uintptr_t actor;if(!player(integer(n),false,false,actor,e))return false;if(!actor){c.result=CharacterMenuValueV1::reference(c.arguments[0].kind==5?c.arguments[0].object:0);return true;}return stats(c,actor,c.arguments[0].kind==5?c.arguments[0].object:0,e);}
 if(equal("NativeInvGetItemDetails")){
  if(count<3){e="Source item details requires three argument slots";return false;}if(c.arguments[0].kind!=2||std::isnan(c.arguments[0].number)||c.arguments[1].kind!=5||c.arguments[2].kind!=2)return true;
  // Source optional merchant form reaches a different inventory/price owner.
  // Until genuinely bound it must reject, never silently use player prices.
  if(count==6){e="Source merchant item-details receiver requires its genuine inventory/trade owner";return false;}
  std::uintptr_t actor;if(!player(integer(c.arguments[2].number),false,false,actor,e))return false;if(!actor)return true;return item_details(c,c.arguments[1].object,std::uint32_t(integer(c.arguments[0].number)),e);
 }
 if(equal("NativeInvGetHasOffHandWeapon")||equal("NativeInvGetHasTwoHandedWeapon")){
  if(count!=1||c.arguments[0].kind!=2||std::isnan(c.arguments[0].number))return true;
  std::uintptr_t actor;if(!player(integer(c.arguments[0].number),false,false,actor,e))return false;
  bool value=false;if(actor){const auto& inv=*g.equipment->inventory();
   if(equal("NativeInvGetHasTwoHandedWeapon")){if(!inv.has_two_hander(false,value,e))return false;}
   else{auto row=[&](unsigned slot)->const data::ItemRecord164*{auto* cell=inv.equipment()[inv.current_equipment()][slot];auto* metadata=cell&&cell->item?data::item(inv.table(),cell->item->id):nullptr;return metadata?&metadata->record:nullptr;};player::EquipmentQueries12V1 facts{};if(dh2_equipment_queries_v1(&facts,row(1),row(2),0)){e="Source off-hand weapon projection invalid";return false;}value=bool(facts.flags&player::query_dual);}
  }c.result=CharacterMenuValueV1::flag(value);return true;
 }
 if(equal("NativeInvGetEquipedItem")){
  if(count!=3)return true;double slot_number,player_number;if(!number(c,0,slot_number,e)||!number(c,2,player_number,e))return false;
  const auto slot=integer(slot_number);std::uintptr_t actor;if(!player(integer(player_number),false,false,actor,e))return false;
  const data::OwnedItemSlotV4* cell=nullptr;if(actor&&slot>=0&&slot<9){const auto& inv=*g.equipment->inventory();cell=inv.equipment()[slot==1||slot==2?inv.current_equipment():0][slot];}
  if(!cell||!cell->item){c.result=CharacterMenuValueV1::flag(false);return true;}
  const auto object=c.arguments[1].kind==5?c.arguments[1].object:0;std::string name;if(!item_name(*cell->item,name,e))return false;
  const auto& items=g.equipment->inventory()->items();auto at=std::find_if(items.begin(),items.end(),[&](auto& p){return p.get()==cell;});if(at==items.end()){e="Equipped item detached from authoritative inventory";return false;}
  if(!write(c,object,"ItemName",CharacterMenuValueV1::string(std::move(name)),e)||!write(c,object,"ItemIndex",CharacterMenuValueV1::numeric(at-items.begin()),e)||!write(c,object,"ItemColor",CharacterMenuValueV1::numeric(cell->item->powers.size()),e))return false;
  c.result=CharacterMenuValueV1::flag(true);return true;
 }
 if(equal("NativeInvGetItemsListForSlot")){
  if(count!=3||c.arguments[0].kind!=2||std::isnan(c.arguments[0].number)||c.arguments[1].kind!=5||c.arguments[2].kind!=2)return true;
  if(!c.array||!c.array(c.arguments[1].object,e))return false;std::uintptr_t actor;if(!player(integer(c.arguments[2].number),false,false,actor,e))return false;if(!actor)return true;
  auto& loc=graph_.text_environment.localization;if(!loc.constant){e="Genuine EquipmentSlots constants required";return false;}std::uint32_t ids[5];const char* keys[]{"Count","LeftHand","RightHand","LeftHandRingFinger","RightHandRingFinger"};for(unsigned j=0;j<5;++j)if(!loc.constant(loc.context,"EquipmentSlots",keys[j],ids[j],e))return false;
  const auto slot=integer(c.arguments[0].number);const bool valuables=slot==signed_word(ids[0]);if(!valuables&&(slot<0||slot>=9)){e="Source item-list slot assertion outside actual equipment";return false;}
  const auto& inv=*g.equipment->inventory();struct Entry{const data::ItemInstanceV1* item;const data::ItemRecord164* row;std::uint32_t index;};std::vector<Entry> entries;
  for(unsigned j=0;j<inv.items().size();++j){auto& cell=inv.items()[j];if(!cell||!cell->item)continue;auto* item=data::item(inv.table(),cell->item->id);if(!item){e="Source inventory-list metadata absent";return false;}if(valuables?item->record.words[26]==-1:character_menu_slot_candidate_v1(*cell->item,item->record,g.equipment->properties()->resolved,std::uint32_t(slot)))entries.push_back({cell->item.get(),&item->record,j});}
  auto equipped=[&](unsigned s)->const data::ItemInstanceV1*{if(s>=9)return nullptr;auto* cell=inv.equipment()[s==1||s==2?inv.current_equipment():0][s];return cell?cell->item.get():nullptr;};
  unsigned first=std::uint32_t(slot),second=first;if(std::uint32_t(slot)==ids[1]||std::uint32_t(slot)==ids[2]){first=ids[1];second=ids[2];}else if(std::uint32_t(slot)==ids[3]||std::uint32_t(slot)==ids[4]){first=ids[3];second=ids[4];}
  if(!graph_.item_equippable){e="Genuine list requirement provider required";return false;}
  // Source comparator consults the same immutable design and freshly borrowed
  // item/equipment identities. No inventory or player property owner is copied.
  bool valid=true;std::sort(entries.begin(),entries.end(),[&](const Entry& a,const Entry& b){if(!valid)return false;bool aa,bb;if(!graph_.item_equippable(*a.item,aa,e)||!graph_.item_equippable(*b.item,bb,e)){valid=false;return false;}return character_menu_item_equipment_less_v1(*a.item,*a.row,*b.item,*b.row,g.stats.actor_index,aa,bb,a.item==equipped(first)||a.item==equipped(second),b.item==equipped(first)||b.item==equipped(second),a.item==equipped(std::uint32_t(slot)));});if(!valid)return false;
  if(!c.create_object||!c.append){e="Source AS row allocation and array append providers required";return false;}
  for(auto& entry:entries){std::string name;if(!item_name(*entry.item,name,e))return false;const auto other=first==std::uint32_t(slot)?second:first;const bool equipped_other=!valuables&&first!=second&&entry.item==equipped(other);std::uintptr_t object;if(!c.create_object(object,e))return false;
   if(!write(c,object,"ItemName",CharacterMenuValueV1::string(std::move(name)),e)||!write(c,object,"ItemIndex",CharacterMenuValueV1::numeric(entry.index),e))return false;
   bool available=true;if(!valuables&&!graph_.item_equippable(*entry.item,available,e))return false;
   if(!write(c,object,"ItemEquippable",CharacterMenuValueV1::flag(available),e)||!write(c,object,"ItemEquipped",CharacterMenuValueV1::flag(!valuables&&entry.item==equipped(std::uint32_t(slot))),e)||!write(c,object,"ItemEquippedOtherHand",CharacterMenuValueV1::flag(equipped_other),e)||!write(c,object,"ItemQuantity",CharacterMenuValueV1::string(valuables?std::to_string(entry.item->signed_quantity()):" "),e)||!c.append(c.arguments[1].object,CharacterMenuValueV1::reference(object),e))return false;
  }c.result=CharacterMenuValueV1::reference(c.arguments[1].object);return true;
 }
 if(equal("NativeHUDGetIsFaeryUnlocked")){
  if(count!=2||c.arguments[0].kind!=2||std::isnan(c.arguments[0].number)||c.arguments[1].kind!=2)return true;
  std::uintptr_t actor;if(!player(integer(c.arguments[1].number),false,true,actor,e))return false;if(!actor)return true;
  if(!graph_.difficulty){e="Source selected difficulty provider required";return false;}std::int32_t tier;if(!graph_.difficulty(tier,e))return false;
  auto id=std::uint32_t(integer(c.arguments[0].number));if(tier<0||tier>=3||id>=5||!g.save->faeries_initialized()[tier]){e="Source faery assertion outside initialized save rows";return false;}
  auto& debug=g.stats;if(!debug.debug_load||!debug.debug_query){e="Source faery unlock Debug providers required";return false;}if(!debug.debug_load(e))return false;bool unlocked;if(!debug.debug_query("UnlockAllFaeries",unlocked,e))return false;
  if(!unlocked)unlocked=g.save->faeries()[tier][id].state==1;c.result=CharacterMenuValueV1::flag(unlocked);return true;
 }
 if(equal("NativeInvAutoEquipSlot")){
  if(count!=2)return true;double slot_number,player_number;if(!number(c,0,slot_number,e)||!number(c,1,player_number,e))return false;std::uintptr_t actor;if(!player(integer(player_number),false,false,actor,e))return false;if(!actor)return true;auto first=integer(slot_number);
  if(first!=-1&&std::uint32_t(first)>=9)return true;
  if(first!=-1)return graph_.actions->unequip(std::uint32_t(first),e)&&auto_slot(std::uint32_t(first),e);
  for(unsigned slot=0;slot<9;++slot)if(!graph_.actions->unequip(slot,e))return false;
  for(unsigned cursor=9;cursor>0;--cursor)if(!auto_slot(cursor-1,e))return false;
  for(unsigned cursor=9;cursor>0;--cursor){auto slot=cursor-1;const auto& inv=*g.equipment->inventory();auto* cell=inv.equipment()[slot==1||slot==2?inv.current_equipment():0][slot];if(!cell&&slot!=1&&slot!=2&&!auto_slot(slot,e))return false;}return true;
 }
 if(equal("NativeSwapEquipment")){
  double index;if(!number(c,0,index,e))return false;std::uintptr_t actor;if(!player(integer(index),false,false,actor,e))return false;if(actor)return graph_.actions->swap(e);
  if(!g.swap_hud){e="Source swap HUD continuation required even for absent player";return false;}return g.swap_hud("DisplayRightHud",e)&&g.swap_hud("FillActionIcon",e);
 }
 unsigned arity=0;if(equal("NativeInvEquipItem")||equal("NativeEquipSkill"))arity=3;else if(equal("NativeInvUnequipItem")||equal("NativeStatsAssignPoint")||equal("NativeSkillsTrainSkill"))arity=2;else if(equal("NativeSkillsGetSkillPointsLeft")||equal("NativeInvGetPlayerGold"))arity=1;
 if(arity){if(count!=arity)return true;for(auto& v:c.arguments)if(v.kind!=2||std::isnan(v.number))return true;std::uintptr_t actor;bool requires_skills=equal("NativeEquipSkill")||equal("NativeSkillsTrainSkill")||equal("NativeSkillsGetSkillPointsLeft");if(!player(integer(c.arguments.back().number),false,requires_skills,actor,e))return false;if(!actor)return true;
  auto first=integer(c.arguments[0].number);if(equal("NativeInvEquipItem"))return graph_.actions->equip(std::uint32_t(first),std::uint32_t(integer(c.arguments[1].number)),e);
  if(equal("NativeInvUnequipItem"))return graph_.actions->unequip(std::uint32_t(first),e);
  if(equal("NativeStatsAssignPoint")){if(!graph_.actions->assign_stat(std::uint32_t(first),e))return false;c.result={};return true;}
  if(equal("NativeEquipSkill"))return graph_.actions->equip_skill(first,integer(c.arguments[1].number),e);
  std::int32_t result;if(equal("NativeSkillsTrainSkill")){if(!graph_.actions->train_skill(first,result,e))return false;c.result=CharacterMenuValueV1::numeric(result);return true;}
  if(equal("NativeSkillsGetSkillPointsLeft")){if(!graph_.actions->skill_points(result,e))return false;c.result=CharacterMenuValueV1::numeric(result);return true;}
  c.result=CharacterMenuValueV1::numeric(g.equipment->inventory()->gold());return true;
 }
 e="Source callback has no recovered native owner binding: ";e+=name;return false;
}
}
