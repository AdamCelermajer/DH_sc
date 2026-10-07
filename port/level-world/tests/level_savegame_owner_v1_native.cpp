#include "../level_savegame_cache_v1.hpp"
#include "../level_savegame_runtime_v1.hpp"
#include "../reference/level-savegame-v1/constructor-gold.hpp"
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace dh2::level;
static unsigned checks;
static void check(bool x){++checks;if(!x){std::fprintf(stderr,"FAIL check %u\n",checks);std::abort();}}
struct Context {
 std::vector<std::uint8_t> primary,backup;
 bool found{},backup_found{},online{},hosting{};std::uint8_t flag{};
 int reads{},objects{},saves{},destroys{},fail_register{};
 std::vector<std::string> calls;
};
static bool read(void* p,const std::string& f,bool& found,std::vector<std::uint8_t>& bytes,std::string&){auto& c=*static_cast<Context*>(p);++c.reads;c.calls.push_back(f);bool bak=f.size()>=4&&f.substr(f.size()-4)==".bak";found=bak?c.backup_found:c.found;bytes=bak?c.backup:c.primary;return true;}
static bool objects(void* p,dh2::data::Bytes b,LevelSavegameFieldsV1& f,std::string&){auto& c=*static_cast<Context*>(p);++c.objects;check(f.level8!=nullptr&&b.size==1&&b.data[0]==42);return true;}
static bool construct(void* p,const std::string& n,bool raw,std::shared_ptr<void>& out,std::string& e){auto c=std::make_shared<LevelSavegameCacheV1>(LevelSavegameCacheServicesV1{p,read,objects});if(!c->construct(n,raw,e))return false;out=c;return true;}
static bool reg(void* p,void* cache,const char* n,LevelSavegameSectionV1 s,LevelSavegameFieldsV1& f,std::string& e){auto& c=*static_cast<Context*>(p);c.calls.push_back(n);if(c.fail_register==1+(s==LevelSavegameSectionV1::objects)){e="declared registration refusal";return false;}return static_cast<LevelSavegameCacheV1*>(cache)->register_section(n,s,f,e);}
static bool load(void*,void* cache,const char* n,LevelSavegameSectionV1 s,LevelSavegameFieldsV1& f,std::string& e){return static_cast<LevelSavegameCacheV1*>(cache)->load_section(n,s,f,e);}
static bool online(void* p,bool& v,std::string&){v=static_cast<Context*>(p)->online;return true;}
static bool hosting(void* p,bool& v,std::string&){v=static_cast<Context*>(p)->hosting;return true;}
static bool flag(void* p,std::uint8_t& v,std::string&){v=static_cast<Context*>(p)->flag;return true;}
static bool save(void* p,void*,std::string&){++static_cast<Context*>(p)->saves;return true;}
static bool destroy(void* p,void*,std::string&){++static_cast<Context*>(p)->destroys;return true;}
static LevelSavegameServicesV1 services(Context& c){return {&c,construct,reg,load,online,hosting,flag,save,destroy};}
static void u32(std::vector<std::uint8_t>& b,unsigned v){for(int i=0;i<4;++i)b.push_back(static_cast<std::uint8_t>(v>>(i*8)));}
static std::vector<std::uint8_t> profile(){std::vector<std::uint8_t>b;u32(b,2);u32(b,4);for(char x:std::string("INFO"))b.push_back(x);u32(b,433);u32(b,1);for(char x:std::string("OBJS"))b.push_back(x);b.push_back(42);return b;}
int main(){std::string e;int level;
 for(const auto& g:level_save_gold){Context c;LevelSavegameOwnerV1 owner(services(c));check(owner.construct({&level,g.seed,g.difficulty,g.row,g.mode,g.checkpoint},e));check(owner.current_filename()==g.filename);check(owner.fields().level8==&level&&owner.fields().mode0c==g.mode&&owner.fields().row28==g.row);check(owner.fields().loaded_row2c==-1&&owner.fields().field30==-1&&owner.fields().field34==-1&&owner.fields().initializing38==0&&owner.fields().inhibit_save39==0);check(c.reads==2&&c.calls[2]=="INFO"&&c.calls[3]=="OBJS");check(owner.load(e)&&c.objects==0);check(owner.release(e)&&c.destroys==1);check(!owner.construct({&level,0,0,0,0,false},e));}
 for(int which=0;which<6;++which){Context c;c.found=true;c.primary=profile();if(which==1){c.primary={255,255,255,255};c.backup_found=true;c.backup=profile();}if(which==2){c.primary={1,2,3};c.backup_found=true;c.backup=profile();}if(which==3){c.primary={255,255,255,255};c.backup_found=true;c.backup={255,255,255,255};}if(which==4){c.fail_register=2;}if(which==5){c.primary={2,0,0,0,1};}
  LevelSavegameOwnerV1 owner(services(c));bool ok=owner.construct({&level,114,1,7,0,false},e);check(ok==(which!=4&&which!=5));if(which==4){check(owner.phase()==4&&!owner.ready()&&owner.fields().initializing38==1&&owner.native_savegame()!=nullptr);check(owner.release(e));continue;}if(which==5){check(owner.phase()==2&&owner.native_savegame()==nullptr);continue;}
  check(owner.load(e));check(owner.fields().loaded_row2c==(which==3?-1:433));check(c.objects==(which==3?0:1));
  check(owner.save(e)&&c.saves==1);c.online=true;check(owner.save(e)&&c.saves==1);c.hosting=true;c.flag=1;check(owner.save(e)&&c.saves==1);c.flag=0;check(owner.save(e)&&c.saves==2);owner.fields().inhibit_save39=1;check(owner.save(e)&&c.saves==2);check(owner.release(e));
 }
 Context c;auto missing=services(c);missing.construct_cache=nullptr;LevelSavegameOwnerV1 unavailable(missing);check(!unavailable.construct({&level,1,0,0,0,false},e)&&unavailable.phase()==2);check(!unavailable.construct({&level,1,0,0,0,false},e));
 Context live;live.found=true;live.primary=profile();LevelSavegameRuntimeV1 runtime({{&live,read,objects},&live,online,hosting,flag,nullptr});
 check(!runtime.owner().ready()&&!runtime.cache());check(runtime.owner().construct({&level,114,1,7,0,false},e));check(runtime.cache()&&runtime.cache()->has_cached_file());check(runtime.owner().load(e)&&runtime.owner().fields().loaded_row2c==433&&live.objects==1);
 check(!runtime.owner().save(e)&&e.find("saveAll")!=std::string::npos);check(runtime.owner().release(e)&&!runtime.cache());
 std::printf("LevelSavegame original64 + source cache/lifetime/guard native PASS %u checks\n",checks);return 0;
}
