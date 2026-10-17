#include "../player_saved_level_states_v1.hpp"
#include "../level_tables.hpp"
#include "../world_map_tables.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <vector>
using namespace dh2::data;
namespace levels=dh2::data::player_saved_level_states_v1;
using Event=std::array<std::uint32_t,4>;
struct Fixture {
 std::vector<Event> events;
 std::array<std::vector<std::string>,2> names{{{"L0","L1","L2"},{"W0","W1","W2"}}};
 std::array<std::uint32_t,2> counts{{3,3}};
 std::int32_t mode=0;std::uint32_t mutation=0,attempts=0;
 int fail_at=-1,throw_at=-1;bool changed=false,reenter=false;
 levels::Runtime* runtime=nullptr;levels::Status nested=levels::Status::complete;
 PlayerSavegameV1 save;
 struct Captured {Fixture* fixture;unsigned table;};
 Captured captures[2]{{this,0},{this,1}};
 Fixture(){for(unsigned d=0;d<3;++d)for(unsigned t=0;t<2;++t){auto* a=t?save.source_world_map_states(d):save.source_level_states(d);a->words=static_cast<std::int32_t*>(std::malloc(12));a->count=3;a->memory.release=[](void*,void* p){std::free(p);};for(unsigned j=0;j<3;++j)a->words[j]=std::int32_t(100+t*50+d*10+j);}}
 bool step(){const int at=int(attempts++);if(at==throw_at)throw std::runtime_error("actual provider fixture failure");if(reenter){reenter=false;std::string error;levels::Result result;nested=runtime->set_state(SavedStateTableV1::levels,0,1,0,&result,error);}return at!=fail_at;}
 static bool count(void* raw,SavedStateTableV1 table,std::uint32_t* value,std::string&){auto& f=*static_cast<Fixture*>(raw);const unsigned t=unsigned(table);*value=f.counts[t];f.events.push_back({2,t,*value,0});return f.step();}
 static const char* name(const void* raw,std::uint32_t row){const auto& b=*static_cast<const Captured*>(raw);auto& f=*b.fixture;f.events.push_back({4,b.table,row,0});if(!f.step())return nullptr;if(f.mutation==1&&!f.changed&&b.table==0){f.counts[0]=1;f.changed=true;}return row<f.names[b.table].size()?f.names[b.table][row].c_str():nullptr;}
 static bool array(void* raw,SavedStateTableV1 table,levels::NameArray* value,std::string&){auto& f=*static_cast<Fixture*>(raw);const unsigned t=unsigned(table);f.events.push_back({3,t,0,0});*value={&f.captures[t],name};return f.step();}
 static bool assert_mode(void* raw,std::int32_t* value,std::string&){auto& f=*static_cast<Fixture*>(raw);*value=f.mode;f.events.push_back({5,std::uint32_t(*value),0,0});return f.step();}
 static bool log(void* raw,const levels::AssertRequest& q,std::string&){auto& f=*static_cast<Fixture*>(raw);f.events.push_back({6,unsigned(q.table)*4+unsigned(q.condition),q.source_caller,q.source_line});if(f.mutation==3)f.mode=2;return f.step();}
 levels::Services services(){return {this,count,array,assert_mode,log};}
 void project(std::FILE* file,levels::Status status,const levels::Result& r){
  const std::uint32_t result[]{status==levels::Status::complete?0u:1u,r.consumed,r.read_calls,r.string_reads,r.table_counts,r.name_arrays,r.name_comparisons,r.assert_mode_reads,r.assert_logs,r.stores,r.completed_entries};std::fwrite(result,sizeof(result),1,file);
  for(unsigned t=0;t<2;++t)for(unsigned d=0;d<3;++d){const auto* a=t?save.source_world_map_states(d):save.source_level_states(d);std::fwrite(a->words,4,3,file);}
  const auto count=std::uint32_t(events.size());std::fwrite(&count,4,1,file);for(const auto& e:events)std::fwrite(e.data(),sizeof(e),1,file);
 }
};
unsigned policies(){unsigned checks=0;std::string error;levels::Result result;
 const auto check=[&](bool ok){if(!ok)throw std::runtime_error("saved levels policy check failed at "+std::to_string(checks));++checks;};
 {Fixture f;auto s=f.services();s.count=nullptr;levels::Runtime r({&f.save,s});check(r.set_state(SavedStateTableV1::levels,0,1,0,&result,error)==levels::Status::failed);check(f.save.source_level_states(0)->words[0]==100);}
 {Fixture f;auto s=f.services();s.assert_mode=nullptr;levels::Runtime r({&f.save,s});check(r.set_state(SavedStateTableV1::levels,0,2,0,&result,error)==levels::Status::failed);check(result.assert_mode_reads==1&&!result.stores);}
 {Fixture f;auto s=f.services();s.log_assert=nullptr;f.mode=1;levels::Runtime r({&f.save,s});check(r.set_state(SavedStateTableV1::levels,0,2,0,&result,error)==levels::Status::failed);check(result.assert_logs==1&&!result.stores);}
 {Fixture f;levels::Runtime r({&f.save,f.services()});check(r.set_state(SavedStateTableV1::levels,0,2,0,&result,error)==levels::Status::complete);check(f.save.source_level_states(0)->words[0]==2);}
 {Fixture f;f.mode=1;levels::Runtime r({&f.save,f.services()});check(r.set_state(SavedStateTableV1::world_map,2,99,2,&result,error)==levels::Status::complete);check(f.save.source_world_map_states(2)->words[2]==99&&result.assert_logs==1);}
 {Fixture f;levels::Runtime r({&f.save,f.services()});for(int difficulty:{-1,3,100}){check(r.set_state(SavedStateTableV1::levels,0,1,difficulty,&result,error)==levels::Status::failed);check(!result.stores&&f.save.source_level_states(0)->words[0]==100);}}
 {Fixture f;f.mode=1;levels::Runtime r({&f.save,f.services()});for(int id:{-1,3,100}){check(r.set_state(SavedStateTableV1::levels,id,1,0,&result,error)==levels::Status::failed);check(!result.stores&&result.assert_logs==1);}}
 {Fixture f;f.counts[0]=65537;levels::Runtime r({&f.save,f.services()});check(r.set_state(SavedStateTableV1::levels,0,1,0,&result,error)==levels::Status::failed);check(!result.stores);}
 {Fixture f;f.reenter=true;levels::Runtime r({&f.save,f.services()});f.runtime=&r;check(r.set_state(SavedStateTableV1::levels,0,1,0,&result,error)==levels::Status::complete);check(f.nested==levels::Status::busy&&result.stores==1);}
 const std::vector<std::uint8_t> valid{1,0,0,0,3,0,0,0,'L','0',0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 {Fixture f;levels::Runtime r({&f.save,f.services()});check(r.load({valid.data(),valid.size()},&result,error)==levels::Status::complete);check(result.stores==1&&result.completed_entries==1);}
 for(std::size_t n=0;n<valid.size();++n){Fixture f;levels::Runtime r({&f.save,f.services()});check(r.load({valid.data(),n},&result,error)==levels::Status::failed);check(result.consumed<=n);check(f.save.source_level_states(0)->words[0]==(n>=15?1:100)&&result.stores==(n>=15?1u:0u));}
 {Fixture f;levels::Runtime r({&f.save,{}});const std::uint8_t zero[24]{};check(r.load({zero,sizeof(zero)},&result,error)==levels::Status::complete);check(result.read_calls==6&&!result.table_counts&&!result.stores);}
 {Fixture f;auto s=f.services();s.names=nullptr;levels::Runtime r({&f.save,s});check(r.load({valid.data(),valid.size()},&result,error)==levels::Status::failed);check(result.consumed==11&&!result.stores);}
 {Fixture f;levels::Runtime r({&f.save,f.services()});const auto before=result;check(r.load({reinterpret_cast<const std::uint8_t*>(f.save.source_level_states(0)->words),12},&result,error)==levels::Status::invalid_argument);check(std::memcmp(&before,&result,sizeof(result))==0);}
 for(int malformed:{0,1048577}){Fixture f;levels::Runtime r({&f.save,f.services()});std::uint32_t raw[2]{1,std::uint32_t(malformed)};check(r.load({reinterpret_cast<const std::uint8_t*>(raw),8},&result,error)==levels::Status::failed);check(result.consumed==8&&!result.table_counts&&!result.stores);}
 {Fixture f;levels::Runtime r({&f.save,f.services()});auto bad=valid;bad[10]='X';check(r.load({bad.data(),bad.size()},&result,error)==levels::Status::failed);check(result.consumed==11&&!result.stores);}
 {Fixture f;levels::Runtime r({&f.save,f.services()});const std::uint8_t bytes[]{1,0,1,0};check(r.load({bytes,4},&result,error)==levels::Status::failed);check(result.consumed==4&&result.declared_entries==65537);}
 {Fixture f;LevelTables table;WorldMapTables world;LevelDeclaration l{};l.name="L0";table.levels.push_back(l);WorldMapLocation w{};w.name="W0";world.locations.push_back(w);const std::int32_t mode=0;levels::TableBindings b{&table,&world,&mode};levels::Runtime r({&f.save,levels::table_services(b)});check(r.load({valid.data(),valid.size()},&result,error)==levels::Status::complete);check(result.stores==1&&f.save.source_level_states(0)->words[0]==1);check(r.set_state(SavedStateTableV1::world_map,0,2,2,&result,error)==levels::Status::complete);check(f.save.source_world_map_states(2)->words[0]==2);b.world_map=nullptr;check(r.set_state(SavedStateTableV1::world_map,0,2,2,&result,error)==levels::Status::failed);check(!result.stores);}
 for(int at=0;at<4;++at)for(bool throwing:{false,true}){Fixture f;if(throwing)f.throw_at=at;else f.fail_at=at;levels::Runtime r({&f.save,f.services()});check(r.load({valid.data(),valid.size()},&result,error)==levels::Status::failed);check(!result.stores&&f.save.source_level_states(0)->words[0]==100);}
 {Fixture f;levels::Runtime r({&f.save,f.services()});auto before=result;check(r.load({valid.data(),valid.size()},nullptr,error)==levels::Status::invalid_argument);check(r.set_state(SavedStateTableV1(3),0,1,0,&result,error)==levels::Status::invalid_argument);check(std::memcmp(&before,&result,sizeof(result))==0);}
 return checks;
}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;
 std::FILE* input=std::fopen(argv[1],"rb");std::FILE* output=std::fopen(argv[2],"wb");if(!input||!output)return 3;
 std::uint32_t count;if(std::fread(&count,4,1,input)!=1)return 4;
 for(std::uint32_t i=0;i<count;++i){std::uint32_t row[10];if(std::fread(row,sizeof(row),1,input)!=1)return 5;std::vector<std::uint8_t> payload(row[9]);if(!payload.empty()&&std::fread(payload.data(),1,payload.size(),input)!=payload.size())return 6;
  Fixture f;f.counts={row[5],row[6]};f.mode=std::int32_t(row[7]);f.mutation=row[8];levels::Runtime runtime({&f.save,f.services()});f.runtime=&runtime;levels::Result result;std::string error;
  const auto status=row[0]?runtime.set_state(SavedStateTableV1(row[1]),std::int32_t(row[2]),std::int32_t(row[3]),std::int32_t(row[4]),&result,error):runtime.load({payload.data(),payload.size()},&result,error);f.project(output,status,result);
 }
 std::fclose(input);std::fclose(output);std::printf("%u\n",policies());return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 9;}}
