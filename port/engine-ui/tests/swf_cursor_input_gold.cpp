#include "swf_cursor_input.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <stdexcept>
using namespace dh2::ui;
using Bytes=std::vector<unsigned char>;
namespace {
const char*names[]{"root","btn_A","btnDelete","ordinary","btn_Legend","hitzone","btn_deadzone","disabled"};
const char*labels[]{"focus_out","focus_in","pressed","released","clicked"};
const char*methods[]{"on_focus_in","on_focus_out","on_clicked","onPress","onRelease","onReleaseOutside","onRollOver","onRollOut","onDragOver","onDragOut"};
void append(Bytes&out,const void*p,std::size_t n){auto*b=static_cast<const unsigned char*>(p);out.insert(out.end(),b,b+n);}
void word(Bytes&out,std::uint32_t n){append(out,&n,4);}
std::uint32_t read(const unsigned char*p){std::uint32_t n;std::memcpy(&n,p,4);return n;}
unsigned match(const char*name,const char*const*list,unsigned n){for(unsigned i=0;i<n;++i)if(!std::strcmp(name,list[i]))return i;throw std::runtime_error("Unexpected service name");}
float fixture_cos=1,fixture_sin=0;
struct Fixture {
 SwfInputState288 state{};std::uint32_t selection{},action{},labels_mask{};bool accepted{},depth{};
 unsigned hit{};std::array<SwfInputCharacter32,8> views{};std::array<std::array<float,6>,8> matrices{};
 std::array<std::uint32_t,9> references{};std::array<std::uint32_t,8> playing{};std::vector<std::uintptr_t> buttons;
 float raw_xy[2]{};std::int32_t raw_index{},mouse[3]{};std::vector<Bytes> trace;SwfInputServices16 services{this,invoke};
 Fixture(const unsigned char*p){auto u=[&](unsigned i){return read(p+4*i);};selection=u(9);action=u(6);accepted=u(7);hit=u(3);labels_mask=u(4);state.root=9;state.context=u(8)?1:0;state.native_receiver=1;state.flags=u(2);references.fill(1000);
  for(unsigned i=0;i<8;++i){views[i].name=names[i];views[i].is_sprite=p[64+4*i];views[i].visible=p[65+4*i];views[i].mouse9c=p[66+4*i];views[i].sprite_ea=p[67+4*i];playing[i]=(u(5)>>i)&1;std::memcpy(matrices[i].data(),p+96+24*i,24);}
  auto count=read(p+288);for(unsigned i=0;i<count;++i)buttons.push_back(read(p+292+4*i));
  for(unsigned i=0;i<4;++i){auto*slot=p+324+40*i;std::memcpy(&state.slots[i].cursor,slot,16);auto&v=state.slots[i];v.focus=read(slot+16);v.hover=read(slot+20);v.graphic=read(slot+24);v.pending=read(slot+28);v.pressed=read(slot+32);v.enabled=read(slot+36);}
 }
 Bytes event(const SwfEvent48&e){Bytes out;word(out,e.character);word(out,match(e.name,names,8));append(out,&e.kind,32);return out;}
 void note(unsigned op,std::uintptr_t ch){Bytes b;word(b,op);word(b,ch);trace.push_back(std::move(b));}
 static int invoke(void*p,SwfInputState288*,const SwfInputRequest64*q,SwfInputResponse56*out){auto&f=*static_cast<Fixture*>(p);auto ch=q->character;auto index=unsigned(ch?ch-1:0);auto op=unsigned(q->operation);
  if(op==1||op==2){f.note(op,ch);f.references[index]+=op==1?1:std::uint32_t(-1);}
  else if(op==3)out->character=&f.views[index];
  else if(op==4)std::memcpy(out->values,f.matrices[index].data(),24);
  else if(op==5){out->values[0]=q->values[0]*20-f.matrices[index][2];out->values[1]=q->values[1]*20-f.matrices[index][5];}
  else if(op==6){out->values[0]=q->values[0];out->values[1]=q->values[1];}
  else if(op==7){std::memcpy(f.raw_xy,q->values,8);f.raw_index=q->index;}
  else if(op==8){f.mouse[0]=static_cast<int>(q->values[0]);f.mouse[1]=static_cast<int>(q->values[1]);f.mouse[2]=q->integer;}
  else if(op==9)out->identity=1;
  else if(op==10){f.note(op,ch);out->identity=f.hit;}
  else if(op==11){Bytes b;word(b,op);word(b,ch);append(b,q->values,24);f.trace.push_back(std::move(b));}
  else if(op==12){f.note(op,ch);out->characters=f.buttons.data();out->count=f.buttons.size();}
  else if(op==13){out->result=0;if(f.views[index].is_sprite){Bytes b;word(b,op);word(b,ch);auto label=match(q->name,labels,5);word(b,label);f.trace.push_back(std::move(b));out->result=(f.labels_mask>>label)&1;if(out->result){Bytes b;word(b,21);word(b,ch);word(b,0);f.trace.push_back(std::move(b));f.playing[index]=0;}}}
  else if(op==14||op==15){Bytes b;word(b,op);auto event=f.event(*q->event);append(b,event.data(),event.size());f.trace.push_back(std::move(b));
   if(op==14)out->result=f.accepted;else{
    auto&e=*q->event;if(f.action==1&&(e.kind==3||e.kind==4||e.kind==6))e.consumed=1;
    if(f.action==2&&e.kind==6){e.kind=2;e.name=names[2];}
    if(f.action==3&&(e.kind==0||e.kind==8))f.state.flags^=64;
    if(f.action==4&&!f.depth&&e.kind==4){f.depth=true;if(dh2_ui_swf_reset_focus(&f.state,e.cursor,&f.selection,&f.services))throw std::runtime_error("Nested reset failed");f.depth=false;}
   }}
  else if(op==16){Bytes b;word(b,op);word(b,ch);word(b,match(q->name,methods,10));f.trace.push_back(std::move(b));}
  else if(op==17){f.note(op,ch);out->result=f.playing[index];}
  else if(op==18){Bytes b;word(b,op);word(b,q->integer);append(b,q->values,4);f.trace.push_back(std::move(b));}
  else throw std::runtime_error("Unexpected input operation");return 1;
 }
 Bytes snapshot(){Bytes out;append(out,raw_xy,8);word(out,raw_index);append(out,mouse,12);for(auto&s:state.slots){append(out,&s.cursor,16);for(auto n:{s.focus,s.hover,s.graphic,s.pending,s.pressed})word(out,n);word(out,s.enabled);}word(out,state.flags);word(out,selection);append(out,references.data(),36);append(out,playing.data(),32);return out;}
};
}
extern "C" float __wrap_cosf(float){return fixture_cos;}
extern "C" float __wrap_sinf(float){return fixture_sin;}
extern "C" void __wrap_sincosf(float,float*s,float*c){*s=fixture_sin;*c=fixture_cos;}
int main(int argc,char**argv){if(argc!=2)return 2;try{std::ifstream f(argv[1],std::ios::binary);Bytes bytes((std::istreambuf_iterator<char>(f)),{});if(bytes.size()<8||read(bytes.data())!=0x31494353)return 2;unsigned cases=read(bytes.data()+4),calls=0;std::size_t at=8;
 for(unsigned k=0;k<cases;++k){if(at+12>bytes.size())return 2;auto il=read(bytes.data()+at),ol=read(bytes.data()+at+4),ec=read(bytes.data()+at+8);at+=12;if(il!=484||ol!=260||at+il+ol>bytes.size())return 2;auto*raw=bytes.data()+at;Fixture row(raw);at+=il;Bytes expected(bytes.begin()+at,bytes.begin()+at+ol);at+=ol;std::vector<Bytes>trace;
  fixture_cos=1;fixture_sin=0;
  for(unsigned j=0;j<ec;++j){if(at+4>bytes.size())return 2;auto n=read(bytes.data()+at);at+=4;if(at+n>bytes.size())return 2;trace.emplace_back(bytes.begin()+at,bytes.begin()+at+n);if(read(bytes.data()+at)==11&&n==32){std::memcpy(&fixture_cos,bytes.data()+at+8,4);std::memcpy(&fixture_sin,bytes.data()+at+20,4);}at+=n;}
  unsigned op=read(raw),index=read(raw+4);SwfCursor16 next;std::memcpy(&next,raw+48,16);int status=op==0?dh2_ui_swf_update_cursor(&row.state,&next,index,&row.selection,&row.services):op==1?dh2_ui_swf_update_input(&row.state,static_cast<int>(read(raw+40)),index,&row.selection,&row.services):op==2?dh2_ui_swf_set_focus(&row.state,row.hit,index,&row.selection,&row.services):op==3?dh2_ui_swf_reset_focus(&row.state,index,&row.selection,&row.services):dh2_ui_swf_update_pending(&row.state,static_cast<int>(read(raw+40)),read(raw+44),&row.selection,&row.services);
  if(status||row.snapshot()!=expected||row.trace!=trace){std::cerr<<"Original input gold mismatch "<<k<<" status "<<status<<"\n";return 1;}calls+=ec;
 }if(at!=bytes.size())return 2;std::cout<<"{\"validation\":\"PASS\",\"original_gold_comparisons\":"<<cases<<",\"ordered_services\":"<<calls<<",\"libm_fixture_from_original_graphic_output\":true,\"mismatches\":0}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
