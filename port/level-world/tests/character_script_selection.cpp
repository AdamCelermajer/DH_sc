#include "../character_script_selection.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
std::uint32_t word(std::ifstream& input){std::uint32_t value;input.read(reinterpret_cast<char*>(&value),4);check(bool(input),"Truncated selector corpus");return value;}
std::vector<char> name(std::ifstream& input,std::uint32_t length){check(length<=4096,"Name length outside corpus limit");std::vector<char> value(length+1);input.read(value.data(),length);check(bool(input),"Truncated selector name");return value;}
struct Context {
 std::uintptr_t external=0xa123456789abcdefull,name=0;
 unsigned mutation=0;
 std::vector<std::array<std::uint32_t,3>> calls;
 unsigned token(std::uintptr_t value)const{if(!value)return 0;if(value==external)return 1;check(value==name,"Unknown retained filename identity");return 2;}
};
void construct(void* raw,ScriptSelectionState16* state,unsigned kind){
 auto& context=*static_cast<Context*>(raw);context.calls.push_back({kind,context.token(state->external_name),state->scripted});
 if(context.mutation){state->external_name=context.mutation==1?context.external:0;state->scripted=1-state->scripted;}
}
}
int main(int argc,char** argv){try{
 check(argc==2,"Usage: character_script_selection_audit selection-reference.bin");
 std::ifstream input(argv[1],std::ios::binary);check(word(input)==0x31435353,"Bad SSC1 magic");const auto count=word(input);unsigned callbacks=0,guards=0;
 for(unsigned index=0;index<count;++index){
  const auto operation=word(input),prior_external=word(input),prior_scripted=word(input),mutation=word(input);
  const auto expected_external=word(input),expected_scripted=word(input),name_length=word(input),owner_length=word(input);
  auto script=name(input,name_length),owner=name(input,owner_length);
  std::vector<std::array<std::uint32_t,3>> expected;const auto n=word(input);
  for(unsigned i=0;i<n;++i)expected.push_back({word(input),word(input),word(input)});
  Context context;context.name=reinterpret_cast<std::uintptr_t>(script.data());context.mutation=mutation;
  ScriptSelectionState16 state{prior_external?context.external:0,prior_scripted,0};const ScriptSelectionServices16 services{&context,construct};
  const ScriptCreationFacts24 facts{operation==2?0:name_length+1,0,script.data(),owner.data()};
  const auto result=operation==0?dh2_character_script_select(&state,script.data(),&services):dh2_character_script_create_step(&state,&facts,&services);
  check(result==1&&state.reserved==0&&context.token(state.external_name)==expected_external&&state.scripted==expected_scripted,"Original selection field stores differ");
  check(context.calls==expected,"Original synchronous factory order differs");callbacks+=context.calls.size();
 }
 check(input.peek()==std::char_traits<char>::eof(),"Trailing selector corpus bytes");
 Context context;ScriptSelectionState16 state{context.external,1,0};const ScriptSelectionServices16 services{&context,construct};const char valid[]="__player__";
 const ScriptCreationFacts24 facts{1,0,valid,nullptr};
 auto rejected=[&](int result){check(result==-1&&context.calls.empty(),"Invalid selector dispatched constructor");++guards;};
 rejected(dh2_character_script_select(nullptr,valid,&services));rejected(dh2_character_script_select(&state,nullptr,&services));rejected(dh2_character_script_select(&state,valid,nullptr));
 ScriptSelectionServices16 missing{&context,nullptr};rejected(dh2_character_script_select(&state,valid,&missing));
 rejected(dh2_character_script_create_step(&state,nullptr,&services));
 auto invalid=facts;invalid.reserved=1;rejected(dh2_character_script_create_step(&state,&invalid,&services));
 state.reserved=1;rejected(dh2_character_script_select(&state,valid,&services));state.reserved=0;
 state.scripted=2;rejected(dh2_character_script_select(&state,valid,&services));state.scripted=1;
 std::vector<char> unterminated(4097,'x');rejected(dh2_character_script_select(&state,unterminated.data(),&services));
 invalid=facts;invalid.script_length=0;rejected(dh2_character_script_create_step(&state,&invalid,&services));
 check(state.external_name==context.external&&state.scripted==1&&state.reserved==0,"Guard changed selector state");
 std::cout<<"{\"validation\":\"PASS\",\"original_selection_cases\":"<<count<<",\"factory_callbacks\":"<<callbacks
          <<",\"atomic_rejections\":"<<guards<<",\"filename_identity_above_4gib\":true,\"mismatches\":0,\"sanitizer_findings\":0}\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
