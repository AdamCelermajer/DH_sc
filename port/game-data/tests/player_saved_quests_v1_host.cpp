#include "../player_saved_quests_v1.hpp"
#include "../quest_runtime_fields_v1.hpp"
#include <cstdio>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace dh2::data;
namespace saved=dh2::data::player_saved_quests_v1;
namespace scalar=dh2::data::quest_runtime_fields_v1;
struct Fixture {
 std::int32_t row[19]{};
 PlayerSavegameV1 save;
 quest_savegame_v1::QuestSavegame& first=save.source_quest_log_b8();
 quest_savegame_v1::QuestSavegame& second=save.source_quest_log_118();
 saved::StreamRef stream{0x10005000};
 std::vector<std::unique_ptr<scalar::Record>> records;
 std::vector<std::array<std::uintptr_t,2>> owners;
 std::vector<std::array<scalar::ActionRef,2>> actions;
 std::vector<std::uint8_t> bytes;std::uint32_t cursor=0,attempts=0;
 std::vector<std::array<std::uint32_t,8>> events;
 saved::Runtime* runtime=nullptr;bool mutated=false,reentry_seen=false;
 std::uint32_t current_log=0,current_d=0,current_index=0;
 Fixture(){owners.resize(19);actions.resize(19);for(unsigned i=0;i<19;++i){records.emplace_back(new scalar::Record(0x10008000+i*0x100));auto& q=*records.back();q.state_0=std::int32_t(0x60000000+i);q.difficulty_10=std::int32_t((i%9)/3);q.byte_64=std::uint8_t(i%2);for(unsigned a=0;a<2;++a){actions[i][a]={0x1000b000+i*0x100+a*0x40,&owners[i][a]};}q.action_18=&actions[i][0];q.action_1c=&actions[i][1];}}
 auto& log(unsigned which){return which?second:first;}
 unsigned id(const quest_savegame_v1::QuestRef* ref){for(unsigned i=0;i<records.size();++i)if(&records[i]->ref==ref)return i;throw std::runtime_error("real factory QuestRef missing");}
 void init(){for(unsigned l=0;l<2;++l)for(unsigned d=0;d<3;++d){auto& q=log(l);for(int j=0;j<row[6+l*3+d];++j){const auto i=l*9+d*3+unsigned(j);q.quests[d].push_back((std::uint32_t(row[12])&(1u<<i))?nullptr:&records[i]->ref);}q.word_2c[d]=0x11000000+std::int32_t(l*16+d);q.word_38[d]=0x22000000+std::int32_t(l*16+d);q.word_44[d]=0x33000000+std::int32_t(l*16+d);q.word_50[d]=0x44000000+std::int32_t(l*16+d);}}
 bool event(unsigned op,unsigned target=0,unsigned value=0,unsigned flag=0){events.push_back({op,current_log,current_d,current_index,target,value,cursor,flag});++attempts;if(row[14]==5&&attempts==1&&runtime){saved::Result nested;nested.service_calls=77;std::string error="keep";reentry_seen=runtime->load(&nested,error)==saved::Status::busy&&nested.service_calls==77&&error=="keep";}return row[15]>0&&attempts==std::uint32_t(row[15]);}
 bool read(void* dest,bool failed){if(failed&&row[16]){if(cursor+2>bytes.size())return false;std::memcpy(dest,bytes.data()+cursor,2);cursor+=2;return false;}if(failed)return false;if(cursor+4>bytes.size())return false;std::memcpy(dest,bytes.data()+cursor,4);cursor+=4;return true;}
 static std::int32_t leaf(void* context,const scalar::Request& request,scalar::Response*){
  auto& f=*static_cast<Fixture*>(context);const auto i=f.id(&request.quest->ref);bool failed=false;
  if(request.operation==scalar::Operation::read_stream_word){if(request.stream!=&f.stream||request.destination!=&request.quest->state_0)return 1;failed=f.event(8,i);if(!f.read(request.destination,failed))return 1;}
  else if(request.operation==scalar::Operation::action_virtual){if(request.stream!=&f.stream||request.offset!=0x28)return 1;const auto a=request.target==f.actions[i][0].identity?0u:1u;if(request.target!=f.actions[i][a].identity)return 1;failed=f.event(9,i*2+a,0x28);}
  else if(request.operation==scalar::Operation::load_objectives){if(request.stream!=&f.stream||request.target!=request.quest->ref.identity+0x2c)return 1;failed=f.event(10,i);if(!failed&&f.row[14]==2&&!f.mutated){f.log(f.current_log).quests[f.current_d][0]=&f.records[18]->ref;f.mutated=true;}if(!failed&&f.row[14]==4)request.quest->state_0=3;}
  else return 1;
  if(failed&&f.row[17])throw std::runtime_error("leaf observing provider failure");
  return failed?1:0;
 }
 static std::int32_t invoke(void* context,const saved::Request& request,saved::Reply& reply,std::string&){
  auto& f=*static_cast<Fixture*>(context);if(request.stream!=&f.stream)return 1;f.current_log=request.log==&f.second?1u:0u;f.current_d=std::uint32_t(request.difficulty);f.current_index=std::uint32_t(request.index);bool failed=false;
  switch(request.operation){
   case saved::Operation::tell:if(request.virtual_slot!=0x24)return 1;failed=f.event(1,0,0,0);reply.position=(std::uint64_t(std::uint32_t(f.row[13]))<<32)|f.cursor;break;
   case saved::Operation::seek:if(request.virtual_slot!=0x20)return 1;failed=f.event(2,0,std::uint32_t(request.offset),std::uint32_t(request.offset>>32));if(!failed){if(request.offset>f.bytes.size())return 1;f.cursor=std::uint32_t(request.offset);}break;
   case saved::Operation::read_unsigned:case saved::Operation::read_signed:{unsigned role=0;if(request.source_caller==0x46c3a0)role=1;else if(request.source_caller==0x46c518)role=2;else if(request.source_caller==0x46c530)role=3;else if(request.source_caller==0x46c53c)role=4;failed=f.event(request.operation==saved::Operation::read_unsigned?3u:4u,role);if(!f.read(request.destination,failed))return 1;if(f.row[14]==1&&!f.mutated&&role==0){f.log(f.current_log).quests[f.current_d].resize(1);f.mutated=true;}if(f.row[14]==3&&role==4)f.log(f.current_log).word_44[f.current_d]^=0x55;break;}
   case saved::Operation::quest_data:{const auto i=f.id(request.quest);failed=f.event(5,i,0,request.flag);if(!failed){scalar::Runtime real(*f.records[i],{&f,leaf});scalar::Result result;return real.load_quest_data(f.stream,request.flag,&result)==scalar::Status::complete?0:1;}break;}
   case saved::Operation::assert_mode:failed=f.event(6,0,std::uint32_t(f.row[5]));reply.word=f.row[5];break;
   case saved::Operation::log_assert:failed=f.event(7,0,0xc2);break;
   default:return 1;
  }
  if(failed&&f.row[17])throw std::runtime_error("observing provider failure");
  return failed?1:0;
 }
 saved::Status run(saved::Result& result,std::string& error){saved::Runtime actual({&save,&first,&second,&stream,{this,invoke}});runtime=&actual;saved::Status s;switch(row[0]){case 0:s=actual.load(&result,error);break;case 1:s=actual.load_quests(std::uint32_t(row[1]),&result,error);break;case 2:s=actual.unpack_quests(std::uint32_t(row[1]),row[2],std::uint32_t(row[4]),&result,error);break;default:s=actual.unpack_quest(std::uint32_t(row[1]),row[3],row[2],std::uint32_t(row[4]),&result,error);break;}runtime=nullptr;return s;}
 std::vector<std::uint32_t> projection(saved::Status status){std::vector<std::uint32_t> out{status==saved::Status::complete?0u:1u,cursor};for(unsigned l=0;l<2;++l){auto& q=log(l);for(const auto* a:{&q.word_2c,&q.word_38,&q.word_44,&q.word_50})for(auto w:*a)out.push_back(std::uint32_t(w));for(unsigned d=0;d<3;++d)out.push_back(std::uint32_t(q.quests[d].size()));}for(const auto& q:records){out.push_back(std::uint32_t(q->state_0));out.push_back(q->byte_64);}out.push_back(std::uint32_t(events.size()));for(const auto& e:events)out.insert(out.end(),e.begin(),e.end());return out;}
};
unsigned policies(){unsigned checks=0;auto check=[&](bool x){if(!x)throw std::runtime_error("QEST native policy "+std::to_string(checks));++checks;};Fixture f;f.row[0]=2;f.row[15]=-1;f.init();f.bytes.resize(16);saved::Runtime r({&f.save,&f.first,&f.second,&f.stream,{&f,Fixture::invoke}});f.runtime=&r;saved::Result result;std::string error;
 check(r.unpack_quests(0,0,0,&result,error)==saved::Status::complete);check(result.copied_word50==1&&f.cursor==16);
 auto before=result;check(r.load(nullptr,error)==saved::Status::invalid_argument);check(r.load_quests(2,&result,error)==saved::Status::invalid_argument);check(std::memcmp(&before,&result,sizeof(result))==0);
 check(r.unpack_quests(0,-1,0,&result,error)==saved::Status::failed);check(r.unpack_quests(0,3,0,&result,error)==saved::Status::failed);
 check(r.unpack_quest(0,77,0,255,&result,error)==saved::Status::failed);check(result.quest_calls==0);
 saved::Runtime missing({&f.save,&f.first,&f.second,&f.stream,{}});check(missing.load(&result,error)==saved::Status::failed&&result.service_calls==0);
 bool rejected=false;try{saved::Runtime wrong({&f.save,&f.second,&f.first,&f.stream,{&f,Fixture::invoke}});}catch(const std::invalid_argument&){rejected=true;}check(rejected);
 quest_savegame_v1::QuestSavegame other_first(f.save.quest_log_b8_act_words_v1());
 rejected=false;try{saved::Runtime wrong({&f.save,&other_first,&f.second,&f.stream,{&f,Fixture::invoke}});}catch(const std::invalid_argument&){rejected=true;}check(rejected);
 check(&f.first==&f.save.source_quest_log_b8()&&&f.second==&f.save.source_quest_log_118());
 Fixture nested;nested.row[0]=2;nested.row[14]=5;nested.row[15]=-1;nested.bytes.resize(16);nested.init();check(nested.run(result,error)==saved::Status::complete&&nested.reentry_seen);
 check(r.load(reinterpret_cast<saved::Result*>(&f.save),error)==saved::Status::invalid_argument);check(r.load_quests(0,reinterpret_cast<saved::Result*>(f.first.word_44.data()),error)==saved::Status::invalid_argument);
 check(r.load(reinterpret_cast<saved::Result*>(&r),error)==saved::Status::invalid_argument);check(r.load(reinterpret_cast<saved::Result*>(&error),error)==saved::Status::invalid_argument);
 f.first.quests[0].push_back(&f.records[0]->ref);
 check(r.load(reinterpret_cast<saved::Result*>(&f.records[0]->fields),error)==saved::Status::invalid_argument);check(r.load(reinterpret_cast<saved::Result*>(&f.records[0]->ref),error)==saved::Status::invalid_argument);
 f.stream.identity=0;check(r.load(&result,error)==saved::Status::invalid_argument);f.stream.identity=0x10005000;
 auto& old=f.first.word_44;old[0]=123;Fixture tail;tail.row[0]=2;tail.row[15]=4;tail.bytes={0,0,0,0,3,0,0,0,4,0,0,0,5,0,0,0};tail.init();check(tail.run(result,error)==saved::Status::failed);check(tail.first.word_2c[0]==3&&tail.first.word_38[0]==4&&tail.first.word_44[0]==0x33000000&&tail.first.word_50[0]==0x44000000);
 tail.cursor=tail.attempts=0;tail.events.clear();tail.row[16]=1;check(tail.run(result,error)==saved::Status::failed);check(tail.first.word_44[0]==0x33000005&&tail.first.word_50[0]==0x44000000);
 tail.cursor=tail.attempts=0;tail.events.clear();tail.row[15]=-1;check(tail.run(result,error)==saved::Status::complete);check(tail.first.word_44[0]==5&&tail.first.word_50[0]==5);
 for(unsigned n=0;n<16;++n){Fixture cut;cut.row[0]=2;cut.row[15]=-1;cut.bytes.resize(n);cut.init();check(cut.run(result,error)==saved::Status::failed);check(cut.cursor<=n);check(result.copied_word50==0);}
 return checks;}
int main(int argc,char** argv){try{if(argc!=3)return 2;auto* input=std::fopen(argv[1],"rb");auto* output=std::fopen(argv[2],"wb");if(!input||!output)return 3;std::uint32_t count;if(std::fread(&count,4,1,input)!=1)return 4;for(unsigned k=0;k<count;++k){Fixture f;if(std::fread(f.row,4,19,input)!=19)return 5;std::uint32_t n;if(std::fread(&n,4,1,input)!=1)return 6;f.bytes.resize(n);if(n&&std::fread(f.bytes.data(),1,n,input)!=n)return 7;f.init();saved::Result result;std::string error;const auto status=f.run(result,error);const auto fields=f.projection(status);const auto length=std::uint32_t(fields.size());std::fwrite(&length,4,1,output);std::fwrite(fields.data(),4,fields.size(),output);}std::fclose(input);std::fclose(output);std::printf("%u\n",policies());return 0;}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 9;}}
