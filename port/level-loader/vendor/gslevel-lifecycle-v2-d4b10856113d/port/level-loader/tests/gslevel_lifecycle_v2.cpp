#include "../gslevel_lifecycle_v2.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace dh2::loader;
struct Level {int id;};
struct Fixture {
 GSLevelFieldsV2<Level> fields;
 std::shared_ptr<Level> current,first=std::make_shared<Level>(Level{1}),second=std::make_shared<Level>(Level{2});
 std::vector<std::string> events;
 bool menu{},online{},contains{},mutate_allocate{},mutate_proxy{};
 int mode{},unload_mutation{},close_mutation{},fail_event{-1};
 bool add(const std::string& value,std::string& error){events.push_back(value);if(int(events.size())==fail_event){error="declared missing provider";return false;}return true;}
 void mutate(int value){if(value==1)fields.level34.reset();else if(value==2)fields.level34=second;else if(value==3)current=second;}
 GSLevelServicesV2<Level> services(){
  GSLevelServicesV2<Level> s;
  s.flush_animation_sets=[this](std::string& e){return add("flush",e);};
  s.allocate_level=[this](std::shared_ptr<void>& out,std::string& e){
   if(!add("allocate:428:0",e))return false;out=first;
   if(mutate_allocate){fields.arguments.name18="changed";fields.arguments.level1c=-7;fields.arguments.word20=0xfedcba98u;fields.arguments.byte2c=127;}
   return true;
  };
  s.construct_level=[this](const std::shared_ptr<void>& storage,const GSLevelArgumentsV2& a,std::shared_ptr<Level>& out,std::string& e){
   std::ostringstream v;v<<"construct:"<<a.name18<<":"<<a.level1c<<":"<<a.word20<<":"<<a.word24<<":"<<a.word28<<":"<<unsigned(a.byte2c)<<":"<<unsigned(a.byte2d)<<":"<<a.word30<<":"<<a.word40;
   if(!add(v.str(),e))return false;out=std::static_pointer_cast<Level>(storage);return true;
  };
  s.get_menu=[this](const char* name,std::uintptr_t& out,std::string& e){if(!add(std::string("menu:")+name,e))return false;out=menu?10:0;return true;};
  s.online_byte5=[this](std::uint8_t& out,std::string& e){if(!add("online",e))return false;out=online;return true;};
  s.online_state34=[this](std::int32_t& out,std::string& e){if(!add("online_state",e))return false;out=mode;return true;};
  s.overlay_contains_menu=[this](std::uintptr_t m,bool& out,std::string& e){if(!add("overlay:"+std::to_string(m),e))return false;out=contains;return true;};
  s.push_menu=[this](std::uintptr_t m,std::string& e){return add("push:"+std::to_string(m),e);};
  s.menu_render_fx=[](std::uintptr_t,std::uintptr_t& out,std::string&){out=20;return true;};
  s.check_menu_weak_proxy=[this](std::uintptr_t,std::string& e){return add("weak_proxy",e);};
  s.menu_character=[this](std::uintptr_t,std::uintptr_t& out,std::string&){out=mutate_proxy?31:30;return true;};
  s.invoke_as_no_arguments=[this](std::uintptr_t fx,std::uintptr_t character,const char* name,std::string& e){return add("callback:"+std::to_string(fx)+":"+std::to_string(character)+":"+name+":0",e);};
  s.unload_level=[this](const std::shared_ptr<Level>& level,std::string& e){if(!add("unload:"+std::to_string(level->id),e))return false;mutate(unload_mutation);return true;};
  s.menu_virtual10=[this](std::uintptr_t,std::string& e){if(!add("menu_close",e))return false;mutate(close_mutation);return true;};
  s.destroy_level=[this](const std::shared_ptr<Level>& level,std::string& e){return add("destroy:"+std::to_string(level->id),e);};
  return s;
 }
 Fixture(){fields.arguments={"SWAMP",5,6,7,8,1,0,9,10};fields.loading38=77;fields.active3c=88;}
};
static void check(bool ok,const char* error){if(!ok)throw std::runtime_error(error);}
int main(int argc,char** argv){try{
 if(argc==2&&std::string(argv[1])=="failures"){
  unsigned checks=0;
  for(int fail=1;fail<=8;++fail){Fixture f;f.menu=true;f.fail_event=fail;GSLevelLifecycleV2<Level> owner(f.fields,f.current,f.services());
   check(!owner.construct(),"required failure accepted");auto count=f.events.size();check(!owner.construct()&&f.events.size()==count,"constructor prefix replayed");++checks;
   if(fail<=3)check(!f.current&&f.fields.loading38==77,"pre-constructor failure published");else check(f.current==f.first&&f.fields.level34==f.first,"published prefix rolled back");++checks;
  }
  Fixture f;f.fields.level34=f.first;f.current=f.first;auto s=f.services();s.unload_level={};GSLevelLifecycleV2<Level> absent(f.fields,f.current,std::move(s));
  check(!absent.destroy()&&f.current==f.first&&f.fields.level34==f.first,"missing unload silently cleared level");++checks;
  auto n=f.events.size();check(!absent.destroy()&&f.events.size()==n,"destructor prefix replayed");++checks;
  Fixture reentry;reentry.menu=true;auto sr=reentry.services();GSLevelLifecycleV2<Level>* ptr{};sr.push_menu=[&](std::uintptr_t,std::string& e){check(!ptr->destroy(),"reentrant destructor accepted");return reentry.add("push:10",e);};
  GSLevelLifecycleV2<Level> reentered(reentry.fields,reentry.current,std::move(sr));ptr=&reentered;check(reentered.construct()&&reentry.current==reentry.first&&reentered.error().empty(),"outer constructor broken by reentry");++checks;
  for(int mismatch=0;mismatch<3;++mismatch){Fixture bad;auto bs=bad.services();
   bs.construct_level=[&](const std::shared_ptr<void>&,const GSLevelArgumentsV2&,std::shared_ptr<Level>& out,std::string&){
    if(mismatch==0)out.reset();else if(mismatch==1)out=bad.second;else out=std::shared_ptr<Level>(bad.first.get(),[](Level*){});return true;
   };
   GSLevelLifecycleV2<Level> wrong(bad.fields,bad.current,std::move(bs));check(!wrong.construct()&&!bad.current&&!bad.fields.level34,"different Level allocation or control block accepted");++checks;
  }
  std::cout<<"PASS failures "<<checks<<"\n";return 0;
 }
 check(argc==10,"expected scenario args");Fixture f;
 f.menu=std::stoi(argv[2]);f.online=std::stoi(argv[3]);f.mode=std::stoi(argv[4]);f.contains=std::stoi(argv[5]);f.mutate_allocate=std::stoi(argv[6]);f.mutate_proxy=std::stoi(argv[7]);f.unload_mutation=std::stoi(argv[8]);f.close_mutation=std::stoi(argv[9]);
 if(std::string(argv[1])=="D"){f.fields.level34=f.mutate_allocate?nullptr:f.first;f.current=f.first;}
 GSLevelLifecycleV2<Level> owner(f.fields,f.current,f.services());
 bool okay=std::string(argv[1])=="C"?owner.construct():owner.destroy();check(okay,owner.error().c_str());
 std::cout<<"{\"events\":[";for(std::size_t i=0;i<f.events.size();++i){if(i)std::cout<<",";std::cout<<"\""<<f.events[i]<<"\"";}
 std::cout<<"],\"field34\":"<<(f.fields.level34?f.fields.level34->id:0)<<",\"global\":"<<(f.current?f.current->id:0)<<",\"loading38\":"<<f.fields.loading38<<",\"active3c\":"<<unsigned(f.fields.active3c)<<"}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
