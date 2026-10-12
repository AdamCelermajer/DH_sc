#include "character_buffs.hpp"
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
unsigned checks=0,frames=0,guards=0,expiries=0;
void check(bool value){++checks;if(!value)throw std::runtime_error("buff check "+std::to_string(checks)+" frame "+std::to_string(frames));}
unsigned word(std::istream&f){unsigned v=0;f.read(reinterpret_cast<char*>(&v),4);check(bool(f));return v;}
int signed_bits(unsigned v){int i=0;std::memcpy(&i,&v,4);return i;}
std::string text(std::istream&f){auto n=word(f);check(n<4096);std::string s(n,'\0');f.read(s.data(),n);check(bool(f));return s;}
template<class T>void read(std::istream&f,T&v){f.read(reinterpret_cast<char*>(&v),sizeof(v));check(bool(f));}
using Trace=std::vector<std::vector<unsigned>>;
struct Fixture;
Fixture* current=nullptr;
struct Fixture {
 dh2::data::PropertyRules rules;std::array<dh2::data::PropertySheet,4> sheets;dh2::data::PropertyView view{};
 std::array<Timer32,512> slots{};TimerStore32 timers{slots.data(),0,512,0x123456789abcdef0u,0,0};TimerServices32 timer_services{};BuffServices16 services{this,invoke};BuffBindings32 bindings{};BuffOwner* owner=nullptr;
 std::map<std::uintptr_t,unsigned> serials;std::vector<unsigned> timer_refs;unsigned next_serial=0;Trace trace;std::map<std::uintptr_t,int> fxids;int failure=-1;bool reenter=false;bool classify=false;const std::vector<dh2::data::ClassRow>* classes=nullptr;
 explicit Fixture(const dh2::data::PropertyRules&r):rules(r){for(auto&s:sheets)s=rules.defaults;view={rules.defaults.data(),rules.types.data(),sheets[0].data(),sheets[1].data(),sheets[2].data(),sheets[3].data(),nullptr,0};timer_services.context=this;timer_services.expired=expired;bindings={&view,&timers,&timer_services,&services};owner=dh2_character_buffs_create(&bindings);check(owner);}
 unsigned serial(std::uintptr_t p){if(!p)return 0;auto found=serials.find(p);if(found!=serials.end())return found->second;return serials[p]=++next_serial;}
 static int invoke(void* context,dh2::data::PropertyView* view,const BuffRequest32*r,std::uintptr_t*out){auto&f=*static_cast<Fixture*>(context);check(view==&f.view&&r->character==f.timers.owner);if(int(r->service)==f.failure)return 0;if(f.reenter){BuffResult24 invalid{99,88,77,66,55};auto original=invalid;check(dh2_character_buff_add(&invalid,f.owner,987,1,1,1,-1,"reentrant")==-1);check(std::memcmp(&invalid,&original,sizeof(invalid))==0);check(dh2_character_buffs_destroy(f.owner)==-1);}
  switch(r->service){
  case buff_fx_load:f.trace.push_back({3,unsigned(r->id)});*out=0x80000000u+(unsigned(r->id)&0xffffu);f.fxids[*out]=r->id;break;
  case buff_fx_release:f.trace.push_back({4,unsigned(f.fxids.count(r->subject)?f.fxids[r->subject]:-1)});break;
  case buff_fx_object:f.trace.push_back({5,unsigned(f.fxids.at(r->subject))});*out=r->subject;break;
  case buff_fx_enable:f.trace.push_back({6,unsigned(f.fxids.at(r->subject)),unsigned(r->index),r->enabled});break;
  case buff_recalculate:{f.trace.push_back({7});dh2::data::ClassRow empty{};auto* rows=f.classes?f.classes->data():&empty;auto count=f.classes?unsigned(f.classes->size()):1u;check(dh2_class_recalc_base(rows,count,f.sheets[0].data(),view)==0);break;}
  default:check(false);
  }return 1;
 }
 static void expired(void* context,std::uintptr_t owner,int event,Timer32*t){auto&f=*static_cast<Fixture*>(context);check(owner==f.timers.owner&&event==0x36&&!t->active);++expiries;BuffResult24 out;check(dh2_character_buff_expired(&out,f.owner,t)==1);}
};
std::string origin(void*p){Dl_info d{};check(dladdr(p,&d));return d.dli_fname;}
template<class Fn>Fn next(const char*name){auto p=dlsym(RTLD_NEXT,name);check(p);return reinterpret_cast<Fn>(p);}
}
// Interpose only to observe calls; every operation immediately executes the
// genuine central shared-library TimerStore. No timer behavior is replaced.
extern "C" std::int32_t dh2_character_timer_start(TimerStore32*s,unsigned duration,int repeat,int event,std::uintptr_t ref,const TimerServices32*services){using Fn=decltype(&dh2_character_timer_start);static auto fn=next<Fn>("dh2_character_timer_start");unsigned serial=0;if(current&&s==&current->timers){serial=current->serial(ref);current->trace.push_back({0,duration,unsigned(repeat),unsigned(event),serial});}auto id=fn(s,duration,repeat,event,ref,services);if(current&&s==&current->timers&&id>=0){if(current->timer_refs.size()<=unsigned(id))current->timer_refs.resize(id+1);current->timer_refs[id]=serial;}return id;}
extern "C" int dh2_character_timer_stop(TimerStore32*s,unsigned id){using Fn=decltype(&dh2_character_timer_stop);static auto fn=next<Fn>("dh2_character_timer_stop");if(current&&s==&current->timers)current->trace.push_back({1,id});return fn(s,id);}
extern "C" int dh2_character_timer_time_left(unsigned*elapsed,unsigned*duration,const TimerStore32*s,unsigned id){using Fn=decltype(&dh2_character_timer_time_left);static auto fn=next<Fn>("dh2_character_timer_time_left");if(current&&s==&current->timers)current->trace.push_back({2,id});return fn(elapsed,duration,s,id);}
int main(int argc,char**argv){try{
 check(argc==3);std::ifstream f(argv[1],std::ios::binary);check(word(f)==0x31465542u);auto count=word(f);dh2::data::PropertyRules rules;read(f,rules.defaults);read(f,rules.types);Fixture fixture(rules);current=&fixture;
 for(frames=0;frames<count;++frames){auto op=word(f);std::vector<unsigned>params(word(f));for(auto&v:params)v=word(f);std::vector<std::string>names(word(f));for(auto&s:names)s=text(f);BuffResult24 out;fixture.trace.clear();int status=0;
  switch(op){
  case 0:status=dh2_character_buff_add(&out,fixture.owner,signed_bits(params[0]),params[1],signed_bits(params[2]),params[3],signed_bits(params[4]),names[0].c_str());break;
  case 1:{std::uintptr_t p=params[1]?0xdeadu:0;for(auto&entry:fixture.serials)if(entry.second==params[1])p=entry.first;status=dh2_character_buff_delete(&out,fixture.owner,signed_bits(params[0]),p);break;}
  case 2:{std::uintptr_t p=0;for(auto&entry:fixture.serials)if(entry.second==params[0])p=entry.first;Timer32 timer;timer.id=params[1];timer.user_ref=p;status=dh2_character_buff_expired(&out,fixture.owner,&timer);break;}
  case 3:for(unsigned i=0;i<params.size();++i)fixture.slots[i].elapsed_ms=params[i];status=1;break;
  case 4:status=dh2_character_buffs_remove_all(&out,fixture.owner);break;
  case 5:{std::vector<const char*>buffs,fx;for(unsigned i=0;i<names.size();++i)(i<params[3]?buffs:fx).push_back(names[i].c_str());BuffDictionary16 b{buffs.data(),unsigned(buffs.size()),0},x{fx.data(),unsigned(fx.size()),0};status=dh2_character_buff_add_dot(&out,fixture.owner,signed_bits(params[0]),signed_bits(params[1]),signed_bits(params[2]),&b,&x);out.instance=0;break;}
  case 6:status=dh2_character_buffs_destroy(fixture.owner);fixture.owner=nullptr;break;
  default:check(false);
  }check(status==1);auto result=word(f),decls=word(f),instances=word(f),timers=word(f);check(decls==dh2_character_buffs_declarations(fixture.owner)&&instances==dh2_character_buffs_count(fixture.owner));std::set<std::uintptr_t> alive;
  for(unsigned i=0;i<instances;++i){BuffSnapshot48 snap;check(dh2_character_buff_snapshot(&snap,fixture.owner,i)==1);alive.insert(snap.instance);check(fixture.serial(snap.instance)==word(f));check(unsigned(snap.id)==word(f));check(snap.strength==word(f));check(unsigned(snap.timer_id)==word(f));auto name=text(f);check(name==snap.name);check(unsigned(fixture.fxids.count(snap.fx)?fixture.fxids.at(snap.fx):-1)==word(f));dh2::data::PropertySheet expected;read(f,expected);check(std::memcmp(expected.data(),snap.sheet,896)==0);}
  check((out.instance?fixture.serial(out.instance):0)==result);for(auto&s:fixture.sheets){dh2::data::PropertySheet expected;read(f,expected);check(s==expected);}check(timers==fixture.timers.count);
  for(unsigned i=0;i<timers;++i){auto&t=fixture.slots[i];check(t.active==word(f));check(t.elapsed_ms==word(f));check(t.duration_ms==word(f));check(unsigned(t.repeat)==word(f));check(unsigned(t.event)==word(f));check(fixture.timer_refs[i]==word(f));}
  Trace expected(word(f));for(auto&row:expected){row.resize(word(f));for(auto&v:row)v=word(f);}check(fixture.trace==expected);for(auto it=fixture.serials.begin();it!=fixture.serials.end();)if(!alive.count(it->first))it=fixture.serials.erase(it);else++it;
 }check(f.peek()==std::char_traits<char>::eof());current=nullptr;
 // Actual TimerStore one-shot expiry and independent resolved DoT channels.
 Fixture live(rules);current=&live;const char*ids[]={"unused","AUTO_DOT_01_FIRE"};BuffDictionary16 dictionary{ids,2,0};BuffResult24 result;for(int e=-1;e<=4;++e)check(dh2_character_buff_add_dot(&result,live.owner,10+e+1,257+e,e,&dictionary,&dictionary)==1);check(dh2_character_buffs_count(live.owner)==6);for(int e=-1;e<=4;++e)check(live.sheets[3][127+e]==257+e);check(dh2_character_timers_update(&live.timers,10,0,&live.timer_services)==1);check(expiries==1&&dh2_character_buffs_count(live.owner)==5);check(live.sheets[3][126]==rules.defaults[126]);check(dh2_character_timers_update(&live.timers,100,0,&live.timer_services)==1);check(expiries==6&&!dh2_character_buffs_count(live.owner));for(unsigned p=126;p<132;++p)check(live.sheets[3][p]==rules.defaults[p]);
 // Source stronger scan changes multiple metadata strengths while resetting
 // only the last candidate sheet. Invalid stale TimeLeft is explicit failure.
 check(dh2_character_buff_add(&result,live.owner,900,0,2,1,-1,"a")==1);auto a=result.instance;check(dh2_character_buff_add(&result,live.owner,900,100,2,1,-1,"b")==1);check(dh2_character_buff_add(&result,live.owner,900,100,2,1,-1,"c")==-2);BuffSnapshot48 snap;check(dh2_character_buff_snapshot(&snap,live.owner,0)==1&&snap.instance==a&&snap.timer_id==-1);check(dh2_character_buffs_remove_all(&result,live.owner)==1);
 // Atomic malformed calls preserve output, owner, timers and property words.
 for(unsigned i=0;i<9;++i){BuffResult24 untouched{99,88,77,66,55},before=untouched;auto sheets=live.sheets;auto slots=live.slots;int bad=0;if(i==0)bad=dh2_character_buff_add(&untouched,live.owner,1,1,1,1,-1,nullptr);else if(i==1)bad=dh2_character_buff_expired(&untouched,live.owner,nullptr);else if(i==2){Timer32 stale;stale.user_ref=0xdead;bad=dh2_character_buff_expired(&untouched,live.owner,&stale);}else if(i==3)bad=dh2_character_buff_add_dot(&untouched,live.owner,0,1,0,&dictionary,&dictionary);else if(i==4)bad=dh2_character_buff_add_dot(&untouched,live.owner,1,-1,0,&dictionary,&dictionary);else if(i==5)bad=dh2_character_buff_add_dot(&untouched,live.owner,1,1,5,&dictionary,&dictionary);else if(i==6)bad=dh2_character_buff_add_dot(&untouched,live.owner,1,1,0,nullptr,&dictionary);else if(i==7){live.timers.reserved=1;bad=dh2_character_buff_add(&untouched,live.owner,1,1,1,1,-1,"bad");live.timers.reserved=0;}else bad=dh2_character_buff_add(&untouched,nullptr,1,1,1,1,-1,"bad");check(bad==-1&&std::memcmp(&untouched,&before,sizeof(before))==0&&sheets==live.sheets&&std::memcmp(slots.data(),live.slots.data(),sizeof(slots))==0);++guards;}
 // Required services fail at their real prefix, with no accepted replacement.
 live.failure=buff_fx_load;check(dh2_character_buff_add(&result,live.owner,50,1,1,1,7,"load")==-2);check(dh2_character_buffs_declarations(live.owner)==1&&!dh2_character_buffs_count(live.owner));live.failure=-1;live.reenter=true;check(dh2_character_buff_add(&result,live.owner,50,1,1,1,7,"load")==1);live.failure=buff_recalculate;check(dh2_character_buff_delete(&result,live.owner,50,0xdeadu)==-2&&!dh2_character_buffs_count(live.owner));live.failure=-1;live.reenter=false;check(dh2_character_buffs_remove_all(&result,live.owner)==1);
 // Real authored class table, through the actual central data DSO.
 auto load=[&](const std::string&name){std::ifstream input(std::string(argv[2])+"/"+name,std::ios::binary);check(bool(input));return std::vector<unsigned char>(std::istreambuf_iterator<char>(input),{});};auto records=load("character_classes_pyarray.bin"),names=load("character_classes_pyarraynames.bin"),schema=load("character_classes_pystructnames.bin");dh2::data::ClassTables table;std::string error;check(dh2::data::load_classes({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},table,error));std::vector<dh2::data::ClassRow>rows;for(auto&row:table.rows)rows.push_back({row.data(),unsigned(row.size())});live.classes=&rows;live.sheets[0][26]=0;check(dh2_character_buff_add(&result,live.owner,51,20,1,1,-1,"class")==1);check(dh2_character_buff_delete(&result,live.owner,51,result.instance)==1);
 // D1 does not stop active timers or recalculate; explicit lifetime contract.
 check(dh2_character_buff_add(&result,live.owner,52,100,1,1,8,"dtor")==1);live.trace.clear();live.reenter=true;auto active=live.slots[0].active;check(dh2_character_buffs_destroy(live.owner)==1);live.owner=nullptr;check(live.slots[0].active==active&&live.trace==Trace{{4,8}});current=nullptr;
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"checks\":"<<checks<<",\"atomic_guards\":"<<guards<<",\"real_timer_expiries\":"<<expiries<<",\"module_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_buff_add))<<"\",\"world_library\":\""<<origin(reinterpret_cast<void*>(&dh2_character_timers_update))<<"\",\"data_library\":\""<<origin(reinterpret_cast<void*>(&dh2_class_recalc_base))<<"\"}\n";return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
