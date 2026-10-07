#include "canonical_cached_file_v1.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "fixed_declarations_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
using namespace dh2::loader;
static void require(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static assets::ZipAssetPackV1 pack(const char* path,const char* prefix){
    auto file=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);
    require(bool(*file),"cache unavailable");const auto length=file->tellg();require(length>=0,"cache length unavailable");
    assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(length);
    backing.read=[file](std::uint64_t at,void* dst,std::size_t n,std::string& e){
        file->clear();file->seekg(std::streamoff(at));file->read(static_cast<char*>(dst),std::streamsize(n));
        if(!*file){e="cache read failed";return false;}return true;
    };
    assets::ZipAssetPackV1 result;std::string error;
    require(result.mount(std::move(backing),prefix,error),error);return result;
}
struct Context {unsigned parses{},releases{},debug{};bool release_fails{};};
static CanonicalFileSourceServicesV1 source(const std::shared_ptr<Context>& context){
    return {[context](bool ok,std::string& e){++context->parses;if(!ok)e="parser failed";return ok;},
        [context](std::string& e){++context->releases;if(context->release_fails){e="fixture source release failed";return false;}return true;}};
}
static LevelFileWalkStepV1 finish(CanonicalCachedFileV1& file,const std::string& uri){
    for(unsigned count=0;count<1000;++count){const auto s=file.step(uri,"Level");if(s!=LevelFileWalkStepV1::pending)return s;}
    throw std::runtime_error("file dispatcher failed to terminate");
}
int main(int argc,char** argv){if(argc!=3)return 2;try{
    auto real=pack(argv[1],"com.gameloft.android.GAND.GloftD2SS/files/");
    auto fixtures=pack(argv[2],"com.gameloft.android.GAND.GloftD2SS/files/");std::string error;unsigned checks=0;
    world::CanonicalPropertyMapV1 properties({});
    world::CanonicalReceiverConstructionV1 construction;
    construction.unknown_type_debug=[](void* p,const char*,std::string&){++static_cast<Context*>(p)->debug;return true;};
    auto context=std::make_shared<Context>();construction.context=context.get();
    world::CanonicalClassReceiverBindingsV1 bindings(properties,construction);
    world::CanonicalObjectManagerV1 manager({});
    const std::string uri="data/scene/001_swamp.mlx";
    CanonicalCachedFileV1 candidate(real,manager,bindings.services(),source(context),{context},ObjectEntryRouteV1::level);
    require(finish(candidate,uri)==LevelFileWalkStepV1::failed,"unsupported SWAMP unexpectedly completed");
    require(candidate.attempts().size()==1,"SWAMP source traversed beyond first unavailable constructor");
    const auto& attempt=*candidate.attempts().front();const auto& entry=attempt.source().entry();
    require(entry.document.uri()==uri&&entry.element==1&&*entry.source().attribute("gametype")=="LevelConfig","incorrect SWAMP first failure");
    require(candidate.error().find("required actual registered class construction: LevelConfig")!=std::string::npos,candidate.error());
    require(attempt.factory_attempt()&&attempt.factory_attempt()->prefix()==world::CanonicalFactoryStageV1::empty,"factory failure prefix changed");
    require(manager.source_count50()==0&&context->parses==1&&context->releases==0,"failure mutated/published or released source");++checks;
    const auto original_error=candidate.error();
    require(finish(candidate,uri)==LevelFileWalkStepV1::failed&&candidate.error()==original_error&&context->parses==1,"failure replayed source callbacks");++checks;
    unsigned cleanup=0;
    require(!candidate.discard_after_owner_release({},error)&&candidate.attempts().size()==1,"source discarded without canonical cleanup");
    require(!candidate.discard_after_owner_release([&](std::string& e){++cleanup;e="fixture owner release failed";return false;},error)&&candidate.file().source(),"cleanup failure lost source");++checks;
    context->release_fails=true;
    require(!candidate.discard_after_owner_release([&](std::string&){++cleanup;return true;},error)&&cleanup==2&&candidate.attempts().size()==1,"source-release failure lost prefix");
    context->release_fails=false;
    require(candidate.discard_after_owner_release([&](std::string&){++cleanup;return true;},error)&&cleanup==2&&candidate.attempts().empty()&&candidate.discarded(),"cleanup retry replayed canonical owner release");
    require(candidate.step(uri,"Level")==LevelFileWalkStepV1::failed,"discarded occurrence restarted");++checks;
    {
        auto c=std::make_shared<Context>();world::CanonicalObjectManagerV1 m({});
        CanonicalCachedFileV1 filtered(real,m,bindings.services(),source(c),{c},ObjectEntryRouteV1::manager,std::string());
        require(finish(filtered,uri)==LevelFileWalkStepV1::complete&&filtered.attempts().size()==10&&m.source_count50()==0,"original empty-filter gate changed");
        require(c->parses==1&&c->releases==1,"successful source release count changed");
        require(finish(filtered,uri)==LevelFileWalkStepV1::complete&&c->parses==1&&c->releases==1,"completed occurrence restarted");++checks;
        require(filtered.step("other.mlx","Level")==LevelFileWalkStepV1::failed,"completed source identity changed");++checks;
    }
    {
        auto c=std::make_shared<Context>();world::CanonicalObjectManagerV1 m({});
        CanonicalCachedFileV1 pending(real,m,bindings.services(),source(c),{c},ObjectEntryRouteV1::level);
        require(pending.step(uri,"Level")==LevelFileWalkStepV1::pending&&pending.attempts().empty(),"initial source step changed");
        require(!pending.discard_after_owner_release([](std::string& e){e="fixture partial owner cleanup";return false;},error),"cleanup failure ignored");
        require(pending.step(uri,"Level")==LevelFileWalkStepV1::failed&&pending.attempts().empty(),"source delivery resumed during partial owner cleanup");++checks;
    }
    {
        auto c=std::make_shared<Context>();world::CanonicalObjectManagerV1 m({});
        CanonicalCachedFileV1 player(fixtures,m,bindings.services(),source(c),{c},ObjectEntryRouteV1::level);
        require(finish(player,"player.mlx")==LevelFileWalkStepV1::complete&&player.attempts().size()==1&&!player.attempts()[0]->factory_attempt()&&m.source_count50()==0,"Level Player exclusion lost");++checks;
        CanonicalCachedFileV1 malformed(fixtures,m,bindings.services(),source(c),{c},ObjectEntryRouteV1::level);
        require(finish(malformed,"malformed.mlx")==LevelFileWalkStepV1::failed&&malformed.attempts().empty(),"malformed XML constructed a receiver");++checks;
        CanonicalCachedFileV1 missing(fixtures,m,bindings.services(),source(c),{c},ObjectEntryRouteV1::level);
        require(finish(missing,"missing.mlx")==LevelFileWalkStepV1::failed&&missing.file().raw_source().empty(),"missing file reported ready");++checks;
        CanonicalCachedFileV1 nopin(fixtures,m,bindings.services(),source(c),{},ObjectEntryRouteV1::level);
        require(finish(nopin,"player.mlx")==LevelFileWalkStepV1::failed&&!nopin.file().active(),"unowned candidate acquired source");++checks;
    }
    FixedSourcesV1 sources;require(sources.prepare(real,"SWAMP",uri,error),error);
    FixedMapV1 map;require(map.prepare(real,sources.borrow(),error),error);
    FixedDeclarationsV1 declarations;require(declarations.prepare(map.borrow(),error),error);
    std::map<std::string,unsigned> counts;
    const auto retained=declarations.borrow();
    for(const auto& d:retained.declarations())++counts[*retained.element(d).attribute("gametype")];
    require(counts["Character"]==50&&counts["OpenableContainer"]==5&&map.borrow().modules().size()==9,"SWAMP source inventory drifted");++checks;
    std::cout<<"{\"validation\":\"PASS\",\"composition_checks\":"<<checks
        <<",\"first_failure\":{\"uri\":\"data/scene/001_swamp.mlx\",\"element\":1,\"type\":\"LevelConfig\",\"factory_address\":3411112,\"factory_prefix\":\"empty\",\"registered_count\":0},\"module_count\":9,\"declaration_count\":"<<declarations.borrow().declarations().size()<<",\"additional_required_classes\":[";
    bool comma=false;
    for(const auto& [name,count]:counts){if(name=="Character"||name=="OpenableContainer"||name=="AnimatedDecor")continue;
        const world::CanonicalFactoryEntryV1* found=nullptr;
        for(const auto& factory:world::canonical_factories_v1())if(name==factory.name)found=&factory;
        require(found,"source factory catalog entry missing");if(comma)std::cout<<',';comma=true;
        std::cout<<"{\"type\":\""<<name<<"\",\"occurrences\":"<<count<<",\"factory_address\":"<<found->original_address<<'}';
    }
    std::cout<<"],\"live_runtime_verified\":false,\"full_loader_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
