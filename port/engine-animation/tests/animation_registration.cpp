#include "../animation_registration.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace {
using namespace dh2::animation;
struct Fixture{RegistrationSet set;std::array<Player,128> resources;};
const Player* player(Fixture& f,std::uint64_t identity){return &f.resources[identity%f.resources.size()];}
[[maybe_unused]] void check(bool v,const char* message){if(!v)throw std::runtime_error(message);}
}
extern "C" {
void* dh2_registration_create(){return new Fixture;}
void dh2_registration_destroy(void* value){delete static_cast<Fixture*>(value);}
unsigned dh2_registration_append(void* value,int id,std::uint64_t identity,unsigned present){auto& f=*static_cast<Fixture*>(value);std::string error;return f.set.append(id,identity,present?player(f,identity):nullptr,error);}
unsigned dh2_registration_default(void* value,std::uint64_t identity,unsigned present){auto& f=*static_cast<Fixture*>(value);std::string error;return f.set.set_default(identity,present?player(f,identity):nullptr,error);}
void dh2_registration_refresh(void* value){static_cast<Fixture*>(value)->set.refresh_indices();}
int dh2_registration_lookup(void* value,int id){return static_cast<Fixture*>(value)->set.lookup(id);}
unsigned dh2_registration_count(void* value,unsigned entries){auto& s=static_cast<Fixture*>(value)->set;return static_cast<unsigned>(entries?s.entries().size():s.occurrences().size());}
unsigned dh2_registration_record(void* value,unsigned index,unsigned entry,unsigned* out){
 auto& f=*static_cast<Fixture*>(value);if(!out)return 0;int id,index_;std::uint64_t identity;
 if(entry){if(index>=f.set.entries().size())return 0;const auto& e=f.set.entries()[index];id=e.dictionary_id;index_=e.engine_index;identity=e.resource_identity;}
 else{if(index>=f.set.occurrences().size())return 0;const auto& o=f.set.occurrences()[index];id=o.dictionary_id;index_=o.engine_index;identity=o.resource_identity;if(o.player!=player(f,identity))return 0;}
 out[0]=static_cast<unsigned>(id);out[1]=static_cast<unsigned>(index_);out[2]=static_cast<unsigned>(identity);out[3]=static_cast<unsigned>(identity>>32);return 1;
}
std::uint64_t dh2_registration_default_identity(void* value){auto& f=*static_cast<Fixture*>(value);if(f.set.default_identity()&&f.set.default_player()!=player(f,f.set.default_identity()))return 0;return f.set.default_identity();}
}
#ifndef DH2_REGISTRATION_ORACLE
namespace{
struct Reader{std::vector<char> bytes;std::size_t at=0;explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing registration corpus");bytes.assign(std::istreambuf_iterator<char>(f),{});}unsigned word(){check(at+4<=bytes.size(),"Truncated registration corpus");unsigned v;std::memcpy(&v,bytes.data()+at,4);at+=4;return v;}std::uint64_t identity(){std::uint64_t lo=word();return lo|(std::uint64_t(word())<<32);}};
}
int main(int argc,char**argv){try{
 if(argc!=2)return 2;
 Reader reader(argv[1]);check(reader.word()==0x31524741,"Wrong registration corpus");unsigned cases=reader.word(),appends=0,lookups=0,entry_checks=0,guards=0;
 for(unsigned ci=0;ci<cases;++ci){Fixture f;std::string error;unsigned n=reader.word();
  for(unsigned i=0;i<n;++i){int id=static_cast<int>(reader.word());auto token=reader.identity();int expected=static_cast<int>(reader.word());check(f.set.append(id,token,player(f,token),error),error.c_str());check(f.set.lookup(id)==expected,"Initial source map index differs");++appends;++lookups;}
  auto inspect=[&]{unsigned count=reader.word();check(f.set.entries().size()==count,"Source map size differs");for(unsigned i=0;i<count;++i){const auto& e=f.set.entries()[i];check(e.dictionary_id==static_cast<int>(reader.word())&&e.engine_index==static_cast<int>(reader.word())&&e.resource_identity==reader.identity(),"Source map entry differs");++entry_checks;}};
  inspect();f.set.refresh_indices();inspect();auto token=reader.identity();check(f.set.set_default(token,player(f,token),error),error.c_str());check(f.set.default_identity()==token&&f.set.default_player()==player(f,token),"Default identity differs");
  auto inputs=f.set.compiled_inputs();check(inputs.size()==n,"Compiled occurrence count differs");for(unsigned i=0;i<n;++i){const auto& o=f.set.occurrences()[i];check(o.engine_index==static_cast<int>(i)&&inputs[i].id==static_cast<int>(i)&&inputs[i].player==o.player,"Occurrence input changed source index");}
  const auto saved=f.set.entries();const auto old_occurrences=f.set.occurrences();const auto old_default=f.set.default_identity();const auto* old_default_player=f.set.default_player();
  for(unsigned g=0;g<5;++g){bool ok=g==0?f.set.append(-1,1,player(f,1),error):g==1?f.set.append(1,0,player(f,1),error):g==2?f.set.append(1,1,nullptr,error):g==3?f.set.set_default(0,player(f,1),error):f.set.set_default(1,nullptr,error);check(!ok&&f.set.occurrences().size()==n&&f.set.entries().size()==saved.size()&&f.set.default_identity()==old_default&&f.set.default_player()==old_default_player,"Malformed registration not atomic");for(unsigned i=0;i<saved.size();++i)check(f.set.entries()[i].dictionary_id==saved[i].dictionary_id&&f.set.entries()[i].engine_index==saved[i].engine_index&&f.set.entries()[i].resource_identity==saved[i].resource_identity,"Rejected registration changed map");for(unsigned i=0;i<old_occurrences.size();++i)check(f.set.occurrences()[i].dictionary_id==old_occurrences[i].dictionary_id&&f.set.occurrences()[i].engine_index==old_occurrences[i].engine_index&&f.set.occurrences()[i].resource_identity==old_occurrences[i].resource_identity&&f.set.occurrences()[i].player==old_occurrences[i].player,"Rejected registration changed occurrence resource");++guards;}
 }
 Fixture full;std::string error;for(int i=0;i<1024;++i)check(full.set.append(i,1,player(full,1),error),error.c_str());check(!full.set.append(1024,1,player(full,1),error)&&full.set.occurrences().size()==1024&&full.set.lookup(1024)==-1,"Registration capacity not atomic");++guards;
 check(reader.at==reader.bytes.size(),"Trailing registration corpus");std::cout<<"{\"validation\":\"PASS\",\"cases\":"<<cases<<",\"appends\":"<<appends<<",\"lookups\":"<<lookups<<",\"entry_checks\":"<<entry_checks<<",\"atomic_rejections\":"<<guards<<",\"sanitizer_findings\":0}\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
