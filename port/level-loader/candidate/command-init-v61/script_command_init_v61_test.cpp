#include "script_command_init_v61.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace dh2::loader;
// ALL engine leaves below are explicit transport/order fixtures. They are
// never evidence that real Main menus/FX/animator/effects work.
struct App{};struct Constants{};struct Menus{};struct Menu{};struct FX{};struct Objects{};struct Object{virtual ~Object()=default;};struct Animator{};struct Character:Object{Animator animator;};struct Handle{};
struct Types {using Application=App;using Constants=::Constants;using MenuManager=Menus;using Menu=::Menu;using FXManager=FX;using ObjectManager=Objects;using ObjectHandle=Handle;using Object=::Object;using Character=::Character;using Animator=::Animator;};
struct Harness {
 std::shared_ptr<App> app=std::make_shared<App>();std::shared_ptr<Constants> constants=std::make_shared<Constants>();std::shared_ptr<Menus> menus=std::make_shared<Menus>();std::shared_ptr<Menu> menu=std::make_shared<Menu>();std::shared_ptr<FX> fx=std::make_shared<FX>();std::shared_ptr<Objects> objects=std::make_shared<Objects>();std::shared_ptr<Handle> handle=std::make_shared<Handle>();std::shared_ptr<Character> character=std::make_shared<Character>();
 std::vector<std::string> trace;bool object_present=true,character_present=true,menu_present=true,foreign_menu=false,foreign_animator=false;int fail_dict=0;
 ScriptInitLeavesV61<Types> leaves(){ScriptInitLeavesV61<Types> l;l.application=app;l.actual_fx_singleton=fx;
  l.app_constants2c=[this](App& a,auto& out,std::string&){assert(&a==app.get());trace.push_back("constants2c");out={constants,constants.get()};return true;};
  l.get_constant=[this](Constants& c,const char* group,const char* key,int& out,std::string&){assert(&c==constants.get()&&std::string(group)=="DialogStyles"&&std::string(key)=="EnterLocationDialog");trace.push_back("constant");out=5;return true;};
  l.menu_get_instance=[this](App& a,auto& out,std::string&){assert(&a==app.get());trace.push_back("menus");out={menus,menus.get()};return true;};
  l.menu_by_name=[this](Menus& m,const char* name,auto& out,std::string&){assert(&m==menus.get());trace.push_back("menu:"+std::string(name));if(menu_present)out={menu,foreign_menu?reinterpret_cast<Menu*>(character.get()):menu.get()};return true;};
  l.fx_register_set_to_load=[this](FX& f,int id,std::string&){assert(&f==fx.get());trace.push_back("fx:"+std::to_string(id));return true;};
  l.app_objects38=[this](App& a,auto& out,std::string&){assert(&a==app.get());trace.push_back("objects38");out={objects,objects.get()};return true;};
  l.object_by_name=[this](Objects& o,const char* name,int id,bool flag,const char* filter,auto& out,std::string&){assert(&o==objects.get()&&id==-1&&!flag&&!filter);trace.push_back("lookup:"+std::string(name));out={handle,handle.get()};return true;};
  l.handle_get_object=[this](Handle& h,bool flag,auto& out,std::string&){assert(&h==handle.get()&&!flag);trace.push_back("get:false");if(object_present)out={std::shared_ptr<Object>(character),character.get()};return true;};
  l.handle_as_character=[this](Handle& h,auto& out,std::string&){assert(&h==handle.get());trace.push_back("as-character");if(character_present)out={character,character.get()};return true;};
  l.character_animator49c=[this](Character& c,auto& out,std::string&){assert(&c==character.get());trace.push_back("animator49c");out={{std::shared_ptr<Animator>(character,&character->animator),&character->animator},foreign_animator?nullptr:character.get()};return true;};
  l.animator_add_dict_to_set=[this](Animator& a,int id,std::string& error){assert(&a==&character->animator);trace.push_back("dict:"+std::to_string(id));if(id==fail_dict){error="required second dictionary body failed";return false;}return true;};return l;
 }
};
using Bytes=std::vector<std::uint8_t>;
std::shared_ptr<ScriptCommandDataV52> fixture_data(int kind,const std::string& text,int first,int second){
 auto bytes=std::make_shared<Bytes>();auto word=[&](unsigned v){for(int n=0;n<4;++n)bytes->push_back(std::uint8_t(v>>(8*n)));};auto string=[&](){for(auto b:text)bytes->push_back(std::uint8_t(b));};word(kind);
 if(kind==10){word(123);word(first);word(0);}else if(kind==22||kind==23){word(0);word(unsigned(text.size()));string();bytes->push_back(0);}else if(kind==20){word(first);word(0);word(0);word(0);word(unsigned(text.size()));string();bytes->push_back(0);}else if(kind==45){word(first);word(second);word(0);word(unsigned(text.size()));string();bytes->push_back(0);}else assert(false);
 std::string e;auto data=ScriptCommandDataV52::original_storage_factory(kind,e);assert(data);ScriptReaderV52 reader({bytes,bytes->data(),bytes->size(),true});assert(data->read(reader,e)&&reader.finished());return data;
}
std::shared_ptr<CanonicalScriptCommandV59> command(int kind,ScriptCommandBehaviorV59 b,const std::string& text="",int first=5,int second=22){std::string e;std::shared_ptr<CanonicalScriptCommandV59> c;assert(create_script_command_receiver_v59(kind,std::move(b),c,e));auto borrow=c->constructor_borrow();auto data=fixture_data(kind,text,first,second);*borrow.kind8=kind;*borrow.retained_data=data;*borrow.data_c=data->identity();return c;}
void close(const std::shared_ptr<CanonicalScriptCommandV59>& c){auto b=c->constructor_borrow();b.retained_data->reset();*b.data_c=0;std::string e;assert(c->release_base_storage(e));}
int main(){std::string e;unsigned cases=0;
 for(int style:{5,6}){Harness h;auto c=command(10,script_init_behavior_v61(h.leaves()),"",style);assert(c->init(e));assert(*c->constructor_borrow().skip4==(style==5));assert((h.trace==std::vector<std::string>{"constants2c","constant"}));assert(!c->execute(false,0,e));close(c);++cases;}
 for(int kind:{22,23})for(const auto& name:{std::string("HUD"),std::string("prefixHUDsuffix"),std::string("hud"),std::string("Dialogue")}){Harness h;auto leaves=h.leaves();if(name.find("HUD")!=std::string::npos)leaves.menu_by_name={};auto c=command(kind,script_init_behavior_v61(leaves),name);assert(c->init(e));std::uintptr_t ptr;assert(c->read_operand_pointer(16,ptr,e));if(name.find("HUD")!=std::string::npos){assert(ptr==0&&(h.trace==std::vector<std::string>{"menus"}));}else{assert(ptr==reinterpret_cast<std::uintptr_t>(h.menu.get())&&(ptr>>32)!=0);assert((h.trace==std::vector<std::string>{"menus","menu:"+name}));CheckedCommandMenuBorrowV59 borrow;assert(c->checked_menu_borrow(borrow,e)&&borrow.actual_menu_owner.get()==h.menu.get());}close(c);++cases;}
 {Harness h;h.menu_present=false;auto c=command(22,script_init_behavior_v61(h.leaves()),"MissingActualMenu");assert(c->init(e));std::uintptr_t ptr;assert(c->read_operand_pointer(16,ptr,e)&&ptr==0);close(c);++cases;}
 {Harness h;h.foreign_menu=true;auto c=command(23,script_init_behavior_v61(h.leaves()),"Dialogue");assert(!c->init(e));std::uintptr_t ptr;assert(c->read_operand_pointer(16,ptr,e)&&ptr==0);close(c);++cases;}
 {Harness h;auto leaves=h.leaves();leaves.menu_by_name={};auto c=command(22,script_init_behavior_v61(leaves),"Dialogue");assert(!c->init(e)&&(h.trace==std::vector<std::string>{"menus"}));assert(!c->init(e)&&h.trace.size()==1);close(c);++cases;}
 {Harness h;auto c=command(20,script_init_behavior_v61(h.leaves()),"",-1);assert(c->init(e)&&(h.trace==std::vector<std::string>{"fx:-1"}));close(c);++cases;}
 {Harness h;auto leaves=h.leaves();leaves.fx_register_set_to_load={};auto c=command(20,script_init_behavior_v61(leaves));assert(!c->init(e)&&h.trace.empty());close(c);++cases;}
 for(int branch=0;branch<3;++branch){Harness h;auto leaves=h.leaves();if(branch==0){h.object_present=false;leaves.handle_as_character={};leaves.character_animator49c={};leaves.animator_add_dict_to_set={};}if(branch==1){h.character_present=false;leaves.character_animator49c={};leaves.animator_add_dict_to_set={};}auto c=command(45,script_init_behavior_v61(leaves),"NamedActor",11,22);assert(c->init(e));auto expected=std::vector<std::string>{"objects38","lookup:NamedActor","get:false"};if(branch>0)expected.push_back("as-character");if(branch==2){expected.push_back("animator49c");expected.push_back("dict:11");expected.push_back("dict:22");}assert(h.trace==expected);unsigned opaque;assert(c->read_operand_word(16,4,opaque,e)&&opaque==0);assert(c->read_operand_word(20,4,opaque,e)&&opaque==0);assert(c->read_operand_word(24,4,opaque,e)&&opaque==0xffffffffu);close(c);++cases;}
 {Harness h;h.fail_dict=22;auto c=command(45,script_init_behavior_v61(h.leaves()),"NamedActor",11,22);assert(!c->init(e)&&!c->initialized());assert(h.trace.back()=="dict:22"&&h.trace[h.trace.size()-2]=="dict:11");auto prefix=h.trace;assert(!c->init(e)&&h.trace==prefix);close(c);++cases;}
 {Harness h;h.foreign_animator=true;auto c=command(45,script_init_behavior_v61(h.leaves()),"NamedActor",11,22);assert(!c->init(e)&&h.trace.back()=="animator49c");close(c);++cases;}
 {Harness h;auto leaves=h.leaves();leaves.get_constant={};auto c=command(10,script_init_behavior_v61(leaves));assert(!c->init(e)&&*c->constructor_borrow().skip4==0&&(h.trace==std::vector<std::string>{"constants2c"}));close(c);++cases;}
 {Harness h;auto c=command(10,script_init_behavior_v61(h.leaves()));h.app.reset();assert(!c->init(e)&&h.trace.empty());close(c);++cases;}
 {Harness h;auto b=script_init_behavior_v61(h.leaves());auto c=command(10,b);CheckedCommandBorrowV59 same;assert(c->checked_data_borrow(same,e));auto foreign=same;foreign.identity++;assert(!b.init(foreign,e)&&h.trace.empty());close(c);++cases;}
 std::cout<<"{\"validation\":\"PASS\",\"transport_order_fixture_cases\":"<<cases<<",\"actual_Main_engine_leaves_are_fixtures\":true,\"actual_Execute_calls\":0,\"actual_fx_or_animation_verified\":false,\"whole_level_ready\":false}\n";
}
