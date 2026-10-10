// Current production class Update/Show bodies are extracted by the runner.
// Target names come from the actual SWF assets, not an unconstrained find stub.
// Localization, GPU and movie invocation are explicit host transport leaves.
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>

namespace {
void check(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);}
constexpr int ANDROID_LOG_INFO=4;
template<class... Args> int __android_log_print(int,const char*,const char*,Args...){return 0;}
struct Evidence {
 std::set<std::string> paths;
 std::set<std::string> missing_paths;
 std::map<std::string,std::string> members;
 std::map<std::string,std::string> displayed_text;
 std::string selected,hook;
 unsigned registrations{},pane_draws{};
} evidence;
}
namespace dh2::ui {
struct SwfAsValue {
 std::string path,value;
 std::uintptr_t identity()const{return path.empty()?0:1;}
 static SwfAsValue text(const char* text){return {"",text};}
 static SwfAsValue boolean(bool value){return {"",value?"true":"false"};}
};
struct SwfDraw {};
struct SwfAsGraph {
 bool root_value(SwfAsValue& out,std::string& e){out={"_root",""};e.clear();return true;}
 bool find_target(const SwfAsValue&,const char* path,SwfAsValue& out,std::string& e){
  if(evidence.missing_paths.count(path)){out={};e="fixture target missing";return false;}
  out={evidence.paths.count(path)?path:"",""};e.clear();return true;
 }
 bool set_member(const SwfAsValue& field,const char* name,const SwfAsValue& value,bool& accepted,std::string& e){
  check(field.identity()!=0,"SetText reached absent authored field");
  evidence.members[field.path+"."+name]=value.value;
  // GameSWF accepts an arbitrary htmlText property on a sprite too. That
  // must not be confused with changing the EditText child which is drawn.
  if((std::string(name)=="htmlText"||std::string(name)=="text")&&evidence.displayed_text.count(field.path))
   evidence.displayed_text[field.path]=value.value;
  accepted=true;e.clear();return true;
 }
 bool invoke(const SwfAsValue& menu,const SwfAsValue&,const char* method,std::initializer_list<SwfAsValue> args,SwfAsValue&,bool& callable,std::string& e){check(menu.path=="menu_SelectClass"&&std::string(method)=="CurrentClass","Wrong class AS receiver");evidence.selected=args.begin()->value;callable=true;e.clear();return true;}
};
struct MenuMovieBorrowV58 {std::uintptr_t identity{};};
struct SwfMovie {
 void* context{};bool (*callback)(void*,const SwfDraw&,std::string&){};
 bool menu_action_script(void* context,bool (*apply)(void*,SwfAsGraph&,std::string&),std::string& e){SwfAsGraph graph;return apply(context,graph,e);}
 bool menu_display_callback(const char* path,void* raw,bool (*draw)(void*,const SwfDraw&,std::string&),std::string& e){
  const std::string key=path;check(key.rfind("_root.",0)==0&&evidence.paths.count(key.substr(6)),"Hook did not address the actual authored class pane");
  evidence.hook=path;++evidence.registrations;context=raw;callback=draw;e.clear();return true;
 }
};
}
namespace dh2::android_ui {
namespace ui=dh2::ui;
constexpr const char* tag="DH2Native";
struct FrontUiSessionV87 {
 struct Impl {
  std::string front_screen="main";
  std::shared_ptr<ui::SwfMovie> movie=std::make_shared<ui::SwfMovie>();
  int class_index{},class_applied_index=-1;
  std::uintptr_t class_left{},class_right{};
  bool process_class_select_active_v87{};
  static bool constant(void*,const char* group,const char* symbol,std::uint32_t& id,std::string& e){
   check(std::string(group)=="StrID","Wrong class string constant group");
   const char* names[]={"MENU_CLASS_00","MENU_KNIGHT_DESC","MENU_CLASS_01","MENU_ROGUE_DESC","MENU_CLASS_02","MENU_MAGE_DESC"};
   for(unsigned i=0;i<6;++i)if(symbol==std::string(names[i])){id=i;e.clear();return true;}
   e="Unexpected class symbol";return false;
  }
  bool text_id_v101(std::uint32_t id,std::string& out,std::string& e){
   const char* strings[]={"Warrior","Warrior description","Rogue","Rogue description","Mage","Mage description"};
   check(id<6,"Invalid source class text id");out=strings[id];e.clear();return true;
  }
  static bool class_pane(void*,const ui::SwfDraw&,std::string& e){++evidence.pane_draws;e.clear();return true;}
#include "class_screen_update_live_v1.inc"
 };
 std::shared_ptr<Impl> impl_=std::make_shared<Impl>();
 bool movie_slot_v93(std::uint32_t slot,ui::MenuMovieBorrowV58& out,std::string& e){out.identity=slot==2?202:0;e.clear();return true;}
 bool process_class_select_show_v87(std::uintptr_t,std::string&);
};
#include "class_screen_show_live_v1.inc"
}
int main(){try{
 std::string path;while(std::getline(std::cin,path)){
  if(!path.empty()&&path.back()=='\r')path.pop_back();
  if(path.rfind("EDITTEXT:",0)==0){const auto target=path.substr(9);evidence.displayed_text[target]=target=="menu_SelectClass.class_title.text"?"TITLE_14":"authoring placeholder";}
  else if(!path.empty())evidence.paths.insert(path);
 }
 const std::string description="menu_SelectClass.class_description.text";
 check(evidence.paths.count(description),"Asset lacks the authored class_description EditText receiver");
 check(!evidence.paths.count("menu_SelectClass.s_description.text"),"Test fixture unexpectedly contains stale IDA field path");
 dh2::android_ui::FrontUiSessionV87 front;std::string e;
 check(front.process_class_select_show_v87(202,e),e);
 check(front.impl_->process_class_select_active_v87&&evidence.registrations==1&&evidence.hook=="_root.menu_SelectClass.class_select","Name Confirm did not complete class Show/pane registration");
 check(evidence.displayed_text[description]=="Warrior description"&&evidence.displayed_text["menu_SelectClass.class_title.text"]=="Warrior","Class title EditText still displays TITLE_14 after parent-sprite assignment");
 check(evidence.members["menu_SelectClass.btn_left._visible"]=="false"&&evidence.members["menu_SelectClass.btn_right._visible"]=="true","Initial Warrior arrows not applied");
 check(front.impl_->movie->callback(front.impl_->movie->context,{},e)&&evidence.pane_draws==1,"Registered class pane cannot dispatch render transport");
 for(unsigned index=1;index<3;++index){
  front.impl_->class_index=index;dh2::ui::SwfAsGraph graph;
  check(dh2::android_ui::FrontUiSessionV87::Impl::update_class(front.impl_.get(),graph,e),e);
  const std::string name=index==1?"Rogue":"Mage";
  check(evidence.displayed_text[description]==name+" description"&&evidence.displayed_text["menu_SelectClass.class_title.text"]==name,"Selected class failed localized EditText update");
  check(evidence.selected==(index==1?"RoguePlayerBase":"MagePlayerBase"),"CurrentClass retained a different save class");
 }
 evidence.missing_paths.insert(description);dh2::ui::SwfAsGraph missing_graph;
 check(!dh2::android_ui::FrontUiSessionV87::Impl::update_class(front.impl_.get(),missing_graph,e),
  "Missing authored class-description field was accepted");
 check(e.find(description)!=std::string::npos&&e.find("fixture target missing")!=std::string::npos,
  "Missing authored field diagnostic lost its exact receiver path or provider cause");
 std::cout<<"PASS current class Show/Update with actual authored target names: Warrior/Rogue/Mage title/description, arrows, CurrentClass and class-pane registration/delivery\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
