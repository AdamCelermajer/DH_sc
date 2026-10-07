#include "hud_initialization_v1.hpp"
#include <cmath>
#include <cstring>
#include <limits>
namespace dh2::ui {
const char* hud_init_member_name(HudInitMember m) noexcept {
 static const char* names[]={"SkillName","SkillDescription","SkillCurrLevel","SkillNextLevel","SkillAssignable","SkillIcon","SkillLevel","SkillAssignedToSlot","SkillUnlocked","Id","Upgraded","NumOptions","CurrentOption","OptionString"};
 auto i=static_cast<std::uint32_t>(m);return i<14?names[i]:nullptr;
}
namespace {
bool aligned(const void*p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
// Explicit source AEABI dependency projection: NaN zero, signed saturation.
std::int32_t integer(double x){if(std::isnan(x))return 0;if(x>=2147483647.)return std::numeric_limits<std::int32_t>::max();if(x<=-2147483648.)return std::numeric_limits<std::int32_t>::min();return static_cast<std::int32_t>(x);}
std::int32_t add(std::int32_t a,std::int32_t b){return static_cast<std::int32_t>(static_cast<std::uint32_t>(a)+static_cast<std::uint32_t>(b));}
struct Run {
 const HudInitInput16& input;const HudInitServices16& services;bool ok=true;
 HudInitResponse32 get(HudInitOperation op,std::uint32_t index=0,std::int32_t value=0,std::int32_t other=0,std::uint32_t type=0,std::uintptr_t subject=0,std::uintptr_t object=0,const char*text=nullptr,const char*name=nullptr,double number=0.){
  HudInitRequest64 q{op,index,value,other,type,0,subject,object,text,name,number};HudInitResponse32 r{};
  if(ok&&services.invoke(services.context,&q,&r)!=1)ok=false;return r;
 }
 HudInitResponse32 arg(HudInitOperation op,unsigned i){return get(op,i,0,0,0,input.call);}
 std::uintptr_t cast(unsigned i,bool array=false){return arg(array?HudInitOperation::cast_array:HudInitOperation::cast_object,i).identity;}
 std::int32_t num(unsigned i){return integer(arg(HudInitOperation::argument_number,i).number);}
 void write(std::uintptr_t actor,std::uintptr_t object,HudInitMember member,unsigned type,std::int32_t value=0,const char*text=nullptr){get(HudInitOperation::write_member,static_cast<unsigned>(member),value,0,type,actor,object,text,hud_init_member_name(member),static_cast<double>(value));}
 const char* symbol(std::int32_t id){return get(HudInitOperation::string_symbol,0,id).text;}
 std::int32_t constant(const char*group,const char*key){return get(HudInitOperation::constant,0,0,0,0,0,0,group,key).value;}
 const char* named(const char*group,const char*key){auto id=constant(group,key);if(!ok)return nullptr;return symbol(id);}
 const char* parse(const char*text,std::uintptr_t args){return get(HudInitOperation::parse_text,0,0,0,0,args,0,text).text;}
 void append(std::uintptr_t args,std::int32_t value,float fraction=0.){get(HudInitOperation::arguments_append,0,value,0,0,args,0,nullptr,nullptr,static_cast<double>(fraction));}
};
int equipped(Run&r){
 if(r.input.argument_count<2||r.input.argument_count>3)return 0;
 if(r.arg(HudInitOperation::argument_type,0).value!=5)return r.ok?0:-2;
 if(!r.arg(HudInitOperation::argument_is_number,1).value)return r.ok?0:-2; // actual is_number(), not truth conversion
 if(r.input.argument_count==3){auto t=r.arg(HudInitOperation::argument_type,2).value;if(t!=0&&t!=1)return r.ok?0:-2;}
 auto array=r.cast(0,true);if(!r.ok)return -2;
 // Source trusts the successful as_array cast after its initial object gate.
 if(!array)return -1;
 auto player_index=r.num(1);auto remote=r.input.argument_count==3?r.arg(HudInitOperation::argument_boolean,2).value:0;
 auto actor=r.get(HudInitOperation::player,0,player_index,remote).identity;
 if(!r.ok)return -2;if(!actor)return 0;
 for(unsigned i=0;i<3;++i){auto slot=r.get(HudInitOperation::skill_slot,i,0,0,0,actor).value;if(!r.ok)return -2;r.get(HudInitOperation::array_push,i,slot,0,2,actor,array,nullptr,nullptr,double(slot));if(!r.ok)return -2;}
 r.get(HudInitOperation::result_boolean,0,1,0,1,r.input.call);return r.ok?0:-2;
}
int faery(Run&r){
 auto index=r.num(0);std::uintptr_t object=0;
 if(r.input.argument_count==2){auto arg=r.arg(HudInitOperation::argument_type,1);if(arg.value==5)object=arg.identity;}
 if(!r.ok)return -2;auto actor=r.get(HudInitOperation::player,0,index,0).identity;if(!r.ok)return -2;if(!actor)return 0;
 auto id=r.get(HudInitOperation::current_faery,0,-1,0,0,actor).value;if(!r.ok)return -2;
 if(!object){r.get(HudInitOperation::result_number,0,id,0,2,r.input.call,0,nullptr,nullptr,double(id));return r.ok?0:-2;}
 r.write(actor,object,HudInitMember::faery_id,2,id);if(!r.ok)return -2;
 id=r.get(HudInitOperation::current_faery,0,-1,0,0,actor).value;if(!r.ok)return -2;
 auto level=r.get(HudInitOperation::faery_level,static_cast<unsigned>(id),-1,0,0,actor).value;if(!r.ok)return -2;
 r.write(actor,object,HudInitMember::faery_upgraded,1,level>0);if(!r.ok)return -2;
 r.get(HudInitOperation::result_object,0,0,0,5,r.input.call,object);return r.ok?0:-2;
}
int details(Run&r){
 auto raw=r.arg(HudInitOperation::argument_number,0).number;
 auto object=r.cast(1);
 auto player_index=r.num(2);auto remote=r.input.argument_count==4?r.arg(HudInitOperation::argument_boolean,3).value:0;
 if(!r.ok)return -2;auto actor=r.get(HudInitOperation::player,0,player_index,remote).identity;if(!r.ok)return -2;
 if(actor){
  auto args0=r.get(HudInitOperation::arguments_create,0).identity;auto args1=r.get(HudInitOperation::arguments_create,1).identity;if(!r.ok)return -2;
  auto index=integer(raw);r.get(HudInitOperation::skill_id,static_cast<unsigned>(index),0,0,0,actor);if(!r.ok)return -2;
  auto address=r.get(HudInitOperation::character_skill,static_cast<unsigned>(index),0,0,0,actor).identity;if(!r.ok)return -2;
  auto*s=reinterpret_cast<const HudInitSkill96*>(address);if(!aligned(s,alignof(HudInitSkill96))||s->reserved||s->reserved1||s->display_count>65536||(s->display_count&&!aligned(s->display_properties,alignof(std::int32_t))))return -1;
  // Source retains the exact row across callback mutation. Dynamic fields are
  // borrowed/pinned, and source scalar reads occur at their original positions.
  auto level=r.get(HudInitOperation::character_level,0,0,0,0,actor).value;if(!r.ok)return -2;
  const char* current="";const char*next="";std::int32_t unlocked=0;
  if(level<s->required_level){current=r.named("StrID","GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL");if(!r.ok)return -2;r.append(args0,s->required_level,static_cast<float>(s->required_level));}
  else {
   auto skill_level=r.get(HudInitOperation::skill_level,static_cast<unsigned>(index),0,0,0,actor).value;if(!r.ok)return -2;
   if(skill_level>0){auto id=s->current_text;
    if(s->faery_dependent&&id!=-1){auto fid=r.get(HudInitOperation::current_faery,0,-1,0,0,actor).value;if(!r.ok)return -2;id=add(id,r.get(HudInitOperation::character_faery_offset,static_cast<unsigned>(fid),0,0,0,actor).value);}
    if(!r.ok)return -2;if(id>=0)current=r.symbol(id);
   }else current=r.named("StrID","GAMEPLAYMENUS_NEEDS_SKILL_POINTS");
   if(!r.ok)return -2;auto max_level=r.constant("CharacterDesign","MaxSkillLevelBNormal");if(!r.ok)return -2;
   auto difficulty=r.get(HudInitOperation::unlocked_difficulty,0,0,0,0,actor).value;if(!r.ok)return -2;
   if(difficulty==1)max_level=r.constant("CharacterDesign","MaxSkillLevelCHard");
   else{difficulty=r.get(HudInitOperation::unlocked_difficulty,0,0,0,0,actor).value;if(!r.ok)return -2;if(difficulty==2)max_level=r.constant("CharacterDesign","MaxSkillLevelDVeryHard");}
   if(!r.ok)return -2;skill_level=r.get(HudInitOperation::skill_level,static_cast<unsigned>(index),0,0,0,actor).value;if(!r.ok)return -2;
   if(max_level<=skill_level)next=r.named("StrID","GAMEPLAYMENUS_MAX_SKILL_LEVEL");
   else if(!r.get(HudInitOperation::can_increment,static_cast<unsigned>(index),0,0,0,actor).value)next=r.named("StrID","GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING");
   else {auto id=s->next_text;if(s->faery_dependent&&id!=-1){auto fid=r.get(HudInitOperation::current_faery,0,-1,0,0,actor).value;if(!r.ok)return -2;id=add(id,r.get(HudInitOperation::character_faery_offset,static_cast<unsigned>(fid),0,0,0,actor).value);}if(!r.ok)return -2;if(id>=0)next=r.symbol(id);}
   if(!r.ok)return -2;
   for(unsigned n=0;n<2;++n){skill_level=r.get(HudInitOperation::skill_level,static_cast<unsigned>(index),0,0,0,actor).value;if(!r.ok)return -2;r.get(HudInitOperation::skill_info,static_cast<unsigned>(index),add(skill_level,static_cast<int>(n)),0,0,actor);if(!r.ok)return -2;
    for(unsigned p=0;p<s->display_count;++p){auto v=r.get(HudInitOperation::property,static_cast<unsigned>(s->display_properties[p]),0,0,0,actor).value;if(!r.ok)return -2;auto f=static_cast<float>(v)*(1.f/256.f);r.append(n?args1:args0,v>>8,f);if(!r.ok)return -2;}}
   unlocked=1;
  }
  if(!r.ok)return -2;current=r.parse(current,args0);if(!r.ok)return -2;next=r.parse(next,args1);if(!r.ok)return -2;
  const char*name="";if(s->name_text>=0)name=r.symbol(s->name_text);if(!r.ok)return -2;r.write(actor,object,HudInitMember::skill_name,3,0,name);
  const char*description="";if(r.ok&&s->description_text>=0)description=r.symbol(s->description_text);if(!r.ok)return -2;r.write(actor,object,HudInitMember::skill_description,3,0,description);
  r.write(actor,object,HudInitMember::skill_current,3,0,current);r.write(actor,object,HudInitMember::skill_next,3,0,next);
  r.write(actor,object,HudInitMember::skill_assignable,1,s->assignable);r.write(actor,object,HudInitMember::skill_icon,3,0,s->icon);if(!r.ok)return -2;
  auto skill_level=r.get(HudInitOperation::skill_level,static_cast<unsigned>(index),0,0,0,actor).value;if(!r.ok)return -2;r.write(actor,object,HudInitMember::skill_level,2,skill_level);if(!r.ok)return -2;
  auto slot=r.get(HudInitOperation::skill_slot,static_cast<unsigned>(index),0,1,0,actor).value;if(!r.ok)return -2;r.write(actor,object,HudInitMember::skill_slot,2,slot);r.write(actor,object,HudInitMember::skill_unlocked,1,unlocked);if(!r.ok)return -2;
 }
 r.get(HudInitOperation::result_object,0,0,0,5,r.input.call,object);return r.ok?0:-2;
}
int options(Run&r){
 const char* key=r.arg(HudInitOperation::argument_string,0).text;
 auto object=r.cast(1);if(!r.ok)return -2;if(!key)return -1;
 auto current=r.get(HudInitOperation::option_current,0,0,0,0,0,0,key).value;
 auto maximum=r.get(HudInitOperation::option_maximum,0,0,0,0,0,0,key).value;
 auto string_id=r.get(HudInitOperation::option_string_id,0,0,0,0,0,0,key).value;if(!r.ok)return -2;
 const char* text="";
 if(static_cast<std::uint32_t>(string_id)-static_cast<std::uint32_t>(current)!=0xffffffffu)text=r.symbol(string_id);
 if(!r.ok)return -2;
 if(object){
  auto override=r.get(HudInitOperation::language_override).value;if(!r.ok)return -2;
  if(override&&!std::strcmp(key,"Language"))maximum=5;
  r.write(0,object,HudInitMember::option_maximum,2,maximum);
  r.write(0,object,HudInitMember::option_current,2,current);
  // Source set_string(nullptr) yields an empty AS string; native adapters must
  // preserve that constructor behavior rather than forwarding a null buffer.
  r.write(0,object,HudInitMember::option_string,3,0,text?text:"");
  if(!r.ok)return -2;
  r.get(HudInitOperation::result_object,0,0,0,5,r.input.call,object);if(!r.ok)return -2;
 }
 auto override=r.get(HudInitOperation::language_override).value;if(!r.ok)return -2;
 if(override&&!std::strcmp(key,"Language"))r.get(HudInitOperation::publish_language,0,current);
 return r.ok?0:-2;
}
int ipod(Run&r){auto supported=r.get(HudInitOperation::platform_music_support).value;if(!r.ok)return -2;r.get(HudInitOperation::result_boolean,0,supported==1,0,1,r.input.call);return r.ok?0:-2;}
}
}
extern "C" int dh2_ui_hud_initialization_v1(const dh2::ui::HudInitInput16*input,std::uint32_t entry,const dh2::ui::HudInitServices16*services){
 using namespace dh2::ui;if(!aligned(input,alignof(HudInitInput16))||!aligned(services,alignof(HudInitServices16))||!services->invoke||entry>4||!input->call||input->available_arguments>4)return -1;
 if((entry==0&&input->argument_count>=2&&input->argument_count<=3&&input->available_arguments<input->argument_count)||(entry==1&&input->available_arguments<3)||(entry==2&&input->available_arguments<1)||(entry==1&&input->argument_count==4&&input->available_arguments<4)||(entry==2&&input->argument_count==2&&input->available_arguments<2))return -1;
 if(entry==3&&input->available_arguments<2)return -1;
 Run r{*input,*services};return entry==0?equipped(r):entry==1?details(r):entry==2?faery(r):entry==3?options(r):ipod(r);
}
