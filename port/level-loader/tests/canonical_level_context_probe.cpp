#include "canonical_level_context_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
using namespace dh2::loader;
static void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static assets::ZipAssetPackV1 archive(const char* path){
    auto f=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);check(bool(*f),"cache unavailable");
    assets::ZipBackingV1 b;b.owner=f;b.bytes=std::uint64_t(f->tellg());
    b.read=[f](std::uint64_t at,void* out,std::size_t n,std::string& e){
        f->clear();f->seekg(std::streamoff(at));f->read(static_cast<char*>(out),std::streamsize(n));
        if(!*f){e="cache read failed";return false;}return true;
    };
    assets::ZipAssetPackV1 pack;std::string e;check(pack.mount(std::move(b),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);return pack;
}
static LevelPreparationV1::Borrow prepared(assets::ZipAssetPackV1 pack,const LevelSourceRequestV1& request){
    LevelPreparationV1 p(std::move(pack));std::string e;check(p.begin(request,e),e);
    for(unsigned i=0;i<30;++i){const auto step=p.step();if(step==LevelPreparationStepV1::source_ready)return p.latest_source();check(step==LevelPreparationStepV1::pending,p.error());}
    throw std::runtime_error("source preparation did not terminate");
}
struct GSLevelGlobalFixture {std::shared_ptr<CanonicalLevelContextV1> s_level;};
int main(int argc,char** argv){if(argc!=2)return 2;try{
    unsigned checks=0;std::string error;
    auto services=std::make_shared<int>(1);
    LevelSourceRequestV1 request{"SWAMP","data/scene/001_swamp.mlx"};
    std::shared_ptr<CanonicalLevelContextV1> first;
    check(CanonicalLevelContextV1::create(request,services,first,error),error);
    const auto fields=first->config_fields();
    check(first->identity()==reinterpret_cast<std::uintptr_t>(first.get())&&first->kill_level()->identity==first->identity()&&first->kill_level()->reserved==0,"Level identity diverged");
    check(*fields.config38==0&&*fields.music11c==-1&&*fields.safezone120==-1&&*fields.ambient124==-1&&first->source_word150()==0,"original constructor stores changed");++checks;
    auto globals=std::make_shared<GSLevelGlobalFixture>();globals->s_level=first;
    CanonicalGSLevelGlobalSlotV1 slot{globals,&globals->s_level};CanonicalCurrentLevelBorrowV1 current;
    check(borrow_current_canonical_level_v1(slot,current,error),error);
    check(current.identity()==first->identity()&&current.kill_level()==first->kill_level(),"GSLevel global returned another Level");++checks;
    check(&first->source_word150()==&first->kill_level()->loot_gate150,"gate150 copied into a second writable field");
    first->source_word150()=0x12345678u;
    check(current.kill_level()->loot_gate150==0x12345678u,"current borrow cached gate150");first->source_word150()=0;++checks;
    *fields.config38=0x5678;*fields.music11c=7;*fields.safezone120=11;*fields.ambient124=13;
    const auto again=current.level()->config_fields();
    check(again.level_owner.get()==first.get()&&again.config38==fields.config38&&*again.music11c==7&&*again.safezone120==11&&*again.ambient124==13,"config publication writes another Level");++checks;
    std::shared_ptr<CanonicalLevelContextV1> returned;
    auto other=request;other.identity="SWAMP_02";
    check(CanonicalLevelContextV1::create(other,services,returned,error),error);
    check(returned->identity()!=first->identity()&&returned->source_request().identity=="SWAMP_02"&&first->source_request().identity=="SWAMP","shared geometry collapsed visit identity");++checks;
    globals->s_level=returned;CanonicalCurrentLevelBorrowV1 second;
    check(borrow_current_canonical_level_v1(slot,second,error)&&second.identity()==returned->identity()&&current.identity()==first->identity(),"GSLevel global swap invalidated the previous borrow");++checks;
    globals->s_level.reset();CanonicalCurrentLevelBorrowV1 empty;
    check(borrow_current_canonical_level_v1(slot,empty,error)&&!empty&&empty.kill_level()==nullptr,"empty GSLevel global fabricated a Level");++checks;
    check(!borrow_current_canonical_level_v1({globals,nullptr},second,error)&&second.identity()==returned->identity(),"missing slot erased retained current-Level borrow");
    check(!borrow_current_canonical_level_v1({{},&globals->s_level},second,error)&&second.identity()==returned->identity(),"missing GSLevel global lease accepted");++checks;
    auto preserved=first;
    check(!CanonicalLevelContextV1::create({},services,first,error)&&first==preserved,"invalid selected source replaced Level");
    check(!CanonicalLevelContextV1::create(request,{},first,error)&&first==preserved,"unretained runtime service accepted");++checks;
    auto pack=archive(argv[1]);auto source=prepared(pack,request);
    check(first->retain_prepared_source(source,error)&&first->prepared_source().request().identity=="SWAMP",error);
    const auto saved_map=first->prepared_source().map().sources().identity();
    check(!returned->retain_prepared_source(source,error)&&!returned->prepared_source(),"different visit accepted the same source identity");
    check(!first->retain_prepared_source({},error)&&first->prepared_source().map().sources().identity()==saved_map,"invalid source replaced prepared Level data");++checks;
    std::weak_ptr<CanonicalLevelContextV1> weak;
    {
        std::shared_ptr<CanonicalLevelContextV1> owned;check(CanonicalLevelContextV1::create(request,services,owned,error),error);weak=owned;
        globals->s_level=owned;CanonicalCurrentLevelBorrowV1 borrow;check(borrow_current_canonical_level_v1(slot,borrow,error),error);
        owned.reset();globals->s_level.reset();check(!weak.expired(),"borrow failed to pin same Level through callbacks");
    }
    check(weak.expired(),"current-Level bridge introduced a retention cycle");++checks;
    check(!globals->s_level,"source preparation automatically published an active Level");++checks;
    // The returned borrow pins the genuine globals provider through callbacks.
    // This is a fixture lease check, not execution of original GSLevel Dtor.
    std::weak_ptr<GSLevelGlobalFixture> weak_globals;
    {
        auto temporary=std::make_shared<GSLevelGlobalFixture>();weak_globals=temporary;
        temporary->s_level=returned;CanonicalCurrentLevelBorrowV1 retained;
        {
            CanonicalGSLevelGlobalSlotV1 actual_global{temporary,&temporary->s_level};
            check(borrow_current_canonical_level_v1(actual_global,retained,error),error);
        }
        temporary.reset();check(!weak_globals.expired()&&retained.identity()==returned->identity(),"GSLevel globals provider not pinned through callbacks");
    }
    check(weak_globals.expired(),"GSLevel globals bridge introduced a retention cycle");++checks;
    std::cout<<"{\"validation\":\"PASS\",\"level_context_checks\":"<<checks<<",\"constructor_fields\":{\"config38\":0,\"music11c\":-1,\"safezone120\":-1,\"ambient124\":-1,\"word150\":0},\"single_writable_gate_verified\":true,\"full_level_constructor_verified\":false,\"globals_fixture\":true,\"gameplay_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
