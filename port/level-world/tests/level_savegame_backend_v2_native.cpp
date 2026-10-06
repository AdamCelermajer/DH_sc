#include "../level_savegame_writer_v2.hpp"
#include "../savegame_file_gate_v2.hpp"
#include "../reference/level-savegame-v2/save-all-gold.hpp"
#include "../reference/level-savegame-v2/save-objects-gold.hpp"
#include "../reference/level-savegame-v2/jobs-gold.hpp"
#include "../reference/level-savegame-v2/load-objects-gold.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
using namespace dh2::level;
static unsigned checks;static void check(bool x){++checks;if(!x){std::fprintf(stderr,"FAIL check %u\n",checks);std::abort();}}
struct Object {std::uint8_t checkpoint{1},disabled{};std::int32_t room{7};std::string gametype{"Monster"},name{"Mob"};bool character{},player{},local{true},online{},missing{};std::vector<std::uint8_t> body;std::string calls;unsigned loads{},consume{UINT32_MAX};};
static void call(Object& o,const char* s){if(!o.calls.empty())o.calls+=",";o.calls+=s;}
static bool character(void* p,bool& b,std::string&){auto& o=*static_cast<Object*>(p);call(o,"character");b=o.character;return true;}
static bool player(void* p,bool& b,std::string&){auto& o=*static_cast<Object*>(p);call(o,"player");b=o.player;return true;}
static bool save_body(void* p,SavegameStreamV2& s,std::string& e){auto& o=*static_cast<Object*>(p);return s.write({o.body.data(),o.body.size()},e);}
static bool load_body(void* p,SavegameStreamV2& s,std::string& e){auto& o=*static_cast<Object*>(p);std::size_t count=o.consume==UINT32_MAX?o.body.size():o.consume;std::vector<std::uint8_t>b(count);if(!s.read(b.data(),b.size(),e))return false;check(std::equal(b.begin(),b.end(),o.body.begin()));++o.loads;return true;}
static LevelSaveObjectBorrowV2 borrow(Object& o){return {&o,&o,&o.checkpoint,&o.gametype,&o.name,&o.room,&o.disabled,character,player,save_body,load_body};}
static bool first(void* p,std::uintptr_t& i,LevelSaveObjectBorrowV2& a,bool& found,std::string&){i=1;a=borrow(*static_cast<Object*>(p));found=true;return true;}
static bool next(void*,std::uintptr_t& i,LevelSaveObjectBorrowV2&,bool& found,std::string&){++i;found=false;return true;}
static bool online(void* p,bool& b,std::string&){b=static_cast<Object*>(p)->online;return true;}
static bool local(void* p,const void* id,bool& b,std::string&){auto& o=*static_cast<Object*>(p);check(id==&o);call(o,"local");b=o.local;return true;}
static bool by_name(void* p,const char* name,std::int32_t room,bool a,const char* b,LevelSaveObjectBorrowV2& out,std::string&){auto& o=*static_cast<Object*>(p);check(std::string(name)==o.name&&room==7&&!a&&!b);out=o.missing?LevelSaveObjectBorrowV2{}:borrow(o);return true;}
static bool get_player(void* p,std::int32_t i,bool create,LevelSaveObjectBorrowV2& out,std::string&){check(i==0&&create);auto& o=*static_cast<Object*>(p);out=o.missing?LevelSaveObjectBorrowV2{}:borrow(o);return true;}
static LevelSaveObjectsServicesV2 object_services(Object& o){return {&o,first,next,online,local,by_name,get_player};}
struct Files {struct Handle{std::string name;std::size_t pos{};} handle;std::map<std::string,std::vector<std::uint8_t>> disk;unsigned opens{},writes{},closes{},backups{},diagnostics{};bool refuse_write{};std::string trace;};
static void trace(Files& f,const std::string& s){if(!f.trace.empty())f.trace+=",";f.trace+=s;}
static bool read(void* p,const std::string& name,bool& found,std::vector<std::uint8_t>& b,std::string&){auto& f=*static_cast<Files*>(p);auto i=f.disk.find(name);found=i!=f.disk.end();if(found)b=i->second;return true;}
static bool backup(void* p,const std::string& a,const std::string& b,bool& result,std::string&){auto& f=*static_cast<Files*>(p);++f.backups;auto i=f.disk.find(a);result=i!=f.disk.end();if(result){f.disk.erase(b);f.disk[b]=std::move(i->second);f.disk.erase(i);}return true;}
static bool open(void* p,const std::string& name,void*& out,std::string&){auto& f=*static_cast<Files*>(p);++f.opens;trace(f,"open1");f.handle={name,0};f.disk[name].clear();out=&f.handle;return true;}
static bool write(void* p,void* h,dh2::data::Bytes b,std::uint64_t& n,std::string& e){auto& f=*static_cast<Files*>(p);check(h==&f.handle);++f.writes;if(f.refuse_write){e="declared platform refusal";return false;}std::uint64_t hash=14695981039346656037ull;for(std::size_t i=0;i<b.size;++i)hash=(hash^b.data[i])*1099511628211ull;trace(f,"write"+std::to_string(f.handle.pos)+":"+std::to_string(b.size)+":"+std::to_string(hash));auto& data=f.disk[f.handle.name];if(f.handle.pos+b.size>data.size())data.resize(f.handle.pos+b.size);if(b.size)std::memcpy(data.data()+f.handle.pos,b.data,b.size);f.handle.pos+=b.size;n=b.size;return true;}
static bool seek(void* p,void* h,std::uint64_t n,std::string&){auto& f=*static_cast<Files*>(p);check(h==&f.handle);trace(f,"seek"+std::to_string(n));f.handle.pos=n;return true;}
static bool close(void* p,void*& h,std::string&){auto& f=*static_cast<Files*>(p);check(h==&f.handle);++f.closes;trace(f,"close");h=nullptr;return true;}
static bool failure(void* p,const char*,std::string&){++static_cast<Files*>(p)->diagnostics;return true;}
static SavegameFileServicesV2 file_services(Files& f){return {&f,read,backup,open,write,seek,close,failure};}
static bool payload(void* p,SavegameStreamV2& s,std::string& e){auto& b=*static_cast<std::vector<std::uint8_t>*>(p);return s.write({b.data(),b.size()},e);}
struct LoadContext{Files* files;Object* object;};
static bool read_load(void* p,const std::string& n,bool& found,std::vector<std::uint8_t>& b,std::string& e){return read(static_cast<LoadContext*>(p)->files,n,found,b,e);}
static bool load_objects(void* p,dh2::data::Bytes b,std::uint64_t offset,LevelSavegameFieldsV1& fields,std::string& e){auto& c=*static_cast<LoadContext*>(p);SavegameStreamV2 stream(b);stream.seek(offset);return level_savegame_load_objects_v2(stream,fields,object_services(*c.object),e);}
int main(){std::string e;unsigned index=0;
 for(const auto& g:load_objects_gold_v2){Object o;o.name=g.name;o.missing=g.missing;o.consume=g.consume;o.body={0,1,2,3,4,5,6,7};SavegameStreamV2 s({g.bytes,g.byte_size});LevelSavegameFieldsV1 f;f.initializing38=g.initializing;check(level_savegame_load_objects_v2(s,f,object_services(o),e));check(s.tell()==g.cursor&&o.loads==g.loads);}
 for(const auto& g:save_jobs_gold_v2){std::vector<std::uint8_t>b(g.size);for(unsigned i=0;i<g.size;++i)b[i]=static_cast<std::uint8_t>(i*17+g.size);b[0]=2;b[1]=b[2]=b[3]=0;Files f;SavegameJobsOwnerV2 jobs(file_services(f));auto s=std::make_unique<SavegameStreamV2>(dh2::data::Bytes{b.data(),b.size()});check(jobs.add_write("save",std::move(s),e));unsigned updates=0;bool progress;do{check(jobs.update(progress,e));++updates;}while(progress);if(updates!=g.updates||f.trace!=g.trace)std::fprintf(stderr,"size%u updates%u/%u trace%s gold%s\n",g.size,updates,g.updates,f.trace.c_str(),g.trace);check(updates==g.updates&&f.trace==g.trace&&f.disk["save"]==b);}
 for(const auto& g:save_all_gold_v2){std::vector<std::uint8_t>body(g.size);for(unsigned j=0;j<g.size;++j)body[j]=static_cast<std::uint8_t>(j*31+index);std::vector<SavegameSectionWriterV2>writers={{{'O','B','J','S'},&body,payload},{{'I','N','F','O'},&body,payload}};std::unique_ptr<SavegameStreamV2> stream;check(savegame_build_frame_v2({},writers,stream,e));check(stream->bytes()==std::vector<std::uint8_t>(g.bytes,g.bytes+g.count));
  Files files;files.disk["save"]={0,0,0,0};SavegameJobsOwnerV2 jobs(file_services(files));check(jobs.add_backup("save",e));check(jobs.add_write("save",std::move(stream),e));check(jobs.pending()==2);bool progress;check(jobs.update(progress,e)&&progress&&files.backups==1&&files.disk["save.bak"]==std::vector<std::uint8_t>({0,0,0,0}));
  check(jobs.flush(nullptr,e)&&jobs.pending()==0);check(files.disk["save"]==std::vector<std::uint8_t>(g.bytes,g.bytes+g.count));check(files.opens==1&&files.closes==1&&files.diagnostics==0);++index;
 }
 for(const auto& g:save_objects_gold_v2){Object o;o.character=g.character;o.player=g.player;o.local=g.local;o.online=g.online;o.disabled=g.disabled;o.body.assign(g.body,g.body+g.body_size);SavegameStreamV2 stream;check(level_savegame_save_objects_v2(stream,object_services(o),e));check(stream.bytes()==std::vector<std::uint8_t>(g.bytes,g.bytes+g.byte_size));check(stream.tell_write()==g.cursor&&o.calls==g.calls);
  LevelSavegameFieldsV1 fields;fields.initializing38=0;stream.seek(0);check(level_savegame_load_objects_v2(stream,fields,object_services(o),e));check(o.loads==(g.byte_size>4?1u:0u));
 }
 Object o;o.body={11,22,33,44};Files files;LoadContext c{&files,&o};LevelSavegameCacheV1 cache({&c,read_load,nullptr,load_objects});check(cache.construct("level",false,e));int level;LevelSavegameFieldsV1 fields;fields.level8=&level;fields.row28=9;fields.initializing38=0;SavegameJobsOwnerV2 jobs(file_services(files));check(level_savegame_save_all_v2(cache,fields,object_services(o),jobs,e));check(jobs.pending()==2&&files.disk.empty());{auto p=cache.profile();check(p&&p.section("OBJS")->size==4&&p.bytes().size()>p.section("OBJS")->offset+4);}
 check(cache.load_section("INFO",LevelSavegameSectionV1::info,fields,e)&&fields.loaded_row2c==9);check(cache.load_section("OBJS",LevelSavegameSectionV1::objects,fields,e)&&o.loads==1);check(jobs.flush(nullptr,e));check(files.disk["level"]==cache.profile().bytes());
 Files crash;crash.disk["save"]={0,0,0,0};SavegameJobsOwnerV2 dying(file_services(crash));auto large=std::make_unique<SavegameStreamV2>();check(large->write_u32(0,e));std::vector<std::uint8_t> zeros(4096);check(large->write({zeros.data(),zeros.size()},e));check(dying.add_backup("save",e)&&dying.add_write("save",std::move(large),e));bool progress;check(dying.update(progress,e));check(dying.update(progress,e)&&dying.pending()==1);check(crash.disk["save"].size()==2048&&crash.disk["save"][0]==255);LevelSavegameCacheV1 fallback({&crash,read,nullptr,nullptr});check(fallback.construct("save",false,e)&&fallback.has_cached_file()&&fallback.profile().bytes()==std::vector<std::uint8_t>({0,0,0,0}));check(dying.flush(nullptr,e)&&crash.disk["save"][0]==0);
 Files refused;refused.refuse_write=true;SavegameJobsOwnerV2 failed(file_services(refused));auto one=std::make_unique<SavegameStreamV2>();check(one->write_u32(0,e));check(failed.add_write("save",std::move(one),e));check(!failed.update(progress,e)&&failed.failed());auto writes=refused.writes;check(!failed.update(progress,e)&&refused.writes==writes);
 check(failed.release(e)&&refused.closes==1&&failed.pending()==0);
 auto pinned=std::make_shared<Files>();std::weak_ptr<Files> weak=pinned;auto kept=std::make_unique<LevelSavegameCacheV1>(LevelSavegameCacheServicesV1{pinned.get(),read,nullptr,nullptr,pinned});check(kept->construct("lease",false,e));pinned.reset();check(!weak.expired());kept.reset();check(weak.expired());
 auto storage=std::make_shared<Files>();auto actual_files=file_services(*storage);actual_files.storage_lease=storage;auto global_jobs=std::make_shared<SavegameJobsOwnerV2>(actual_files);auto gate=std::make_shared<SavegameFileGateV2>(actual_files,global_jobs);auto queued=std::make_unique<SavegameStreamV2>();check(queued->write_u32(0,e));check(global_jobs->add_write("flush",std::move(queued),e));LevelSavegameCacheServicesV1 gated;check(gate->cache_services(gated,e));LevelSavegameCacheV1 gated_cache(gated);check(gated_cache.construct("flush",false,e)&&global_jobs->pending()==0&&gated_cache.has_cached_file());
 std::printf("LevelSavegame V2 original8 writer+32 objects / complete cache-job roundtrip PASS %u checks\n",checks);
}
