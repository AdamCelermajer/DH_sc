#include "level_preparation_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static loader::LevelPreparationStepV1 drain(loader::LevelPreparationV1& candidate) {
    for(unsigned i=0;i<20;++i) {
        const auto step=candidate.step();
        if(step!=loader::LevelPreparationStepV1::pending)return step;
    }throw std::runtime_error("Preparation exceeded stage domain");
}
int main(int argc,char** argv) {
    try {
        if(argc==6||argc==7) {
            std::string error;
            auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
            require(bool(*file),"Cache unavailable");const auto size=file->tellg();require(size>=0,"Cache size unavailable");
            assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(size);
            backing.read=[file](std::uint64_t at,void* dst,std::size_t n,std::string& e) {
                file->clear();file->seekg(std::streamoff(at));file->read(static_cast<char*>(dst),std::streamsize(n));
                if(!*file){e="Cache read failed";return false;}return true;
            };
            assets::ZipAssetPackV1 pack;
            require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
            loader::LevelPreparationV1 p(pack);
            const auto seed=std::stoul(argv[5]);require(seed<=UINT32_MAX,"Seed outside domain");
            const std::string kind=argv[4];require(kind=="fixed"||kind=="procedural","Invalid source kind");
            loader::LevelSourceRequestV1 request{argv[2],argv[3],kind=="fixed"?loader::LevelSourceKindV1::fixed:loader::LevelSourceKindV1::procedural,std::uint32_t(seed)};
            if(argc==7){require(std::string(argv[6])=="original","Invalid original mode");request.repair_known_references=false;request.allow_original_backup=false;}
            require(p.begin(request,error),error);
            const auto result=drain(p);
            if(p.failure()==loader::LevelPreparationFailureV1::original_no_layout) {
                std::cout<<"{\"validation\":\"NO_LAYOUT\",\"generated\":false}\n";return 0;
            }
            require(result==loader::LevelPreparationStepV1::source_ready,p.error());
            const auto prepared=p.latest_source();const auto& map=prepared.map();
            require(prepared.request().identity==argv[2]&&map.sources().identity()==argv[2],"Selected identity changed");
            std::cout<<"{\"validation\":\"PASS\",\"module_count\":"<<map.modules().size()
                <<",\"source_documents\":"<<map.sources().documents().size()<<",\"assets\":"<<map.assets().size()
                <<",\"scene_instances\":"<<map.instances().size()<<",\"declarations\":"<<prepared.declarations().declarations().size()
                <<",\"stages\":"<<p.completed_stages()<<",\"generated\":"<<(prepared.procedural_modules()?"true":"false")
                <<",\"backup_used\":"<<(prepared.resolution().backup_used?"true":"false")
                <<",\"reference_repairs\":"<<(prepared.procedural_modules()?prepared.procedural_modules()->reference_repairs.size():0)
                <<",\"runtime_objects_verified\":false,\"full_loader_verified\":false}\n";
            return 0;
        }
        require(argc==5,"Expected cache and inventory definitions for DESERT_CAVE_02, SWAMP_02, VOID_MAZE_03");std::string error;
        loader::LevelPreparationV1::Borrow retained,generated;
        auto reads=std::make_shared<std::uint64_t>(0);unsigned checks=0;
        {
            std::unique_ptr<loader::LevelPreparationV1> preparation;
            {
                auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
                require(bool(*file),"Cache unavailable");const auto size=file->tellg();require(size>=0,"Cache size unavailable");
                assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(size);
                backing.read=[file,reads](std::uint64_t at,void* dst,std::size_t n,std::string& e) {
                    ++*reads;file->clear();file->seekg(std::streamoff(at));file->read(static_cast<char*>(dst),std::streamsize(n));
                    if(!*file){e="Cache read failed";return false;}return true;
                };
                assets::ZipAssetPackV1 pack;
                require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
                preparation=std::make_unique<loader::LevelPreparationV1>(pack);
            }
            auto& p=*preparation;
            const loader::LevelSourceRequestV1 swamp{"SWAMP","001_swamp.mlx",loader::LevelSourceKindV1::fixed,0};
            require(!p.begin({"",""},error)&&p.stage()==loader::LevelPreparationStageV1::idle,"Invalid request mutated facade");++checks;
            require(p.begin(swamp,error),error);
            require(drain(p)==loader::LevelPreparationStepV1::source_ready,p.error());
            retained=p.latest_source();require(retained.request().identity=="SWAMP"&&retained.map().modules().size()==9,"Incomplete Swamp source");
            require(!retained.procedural_modules()&&p.completed_stages()==4,"Wrong fixed stage/provenance");++checks;
            const auto complete_reads=*reads;
            require(p.step()==loader::LevelPreparationStepV1::source_ready&&*reads==complete_reads,"Completion replayed acquisition");++checks;
            // Cancel at each boundary, including fully prepared but unpublished.
            for(unsigned stop=0;stop<4;++stop) {
                require(p.begin(swamp,error),error);
                for(unsigned step=0;step<stop;++step)require(p.step()==loader::LevelPreparationStepV1::pending,p.error());
                require(!p.begin({"OTHER","missing.mlx"},error),"Pending request replaced");
                require(p.latest_source().request().identity=="SWAMP","Pending source published");
                p.discard();const auto cancelled_reads=*reads;
                require(p.step()==loader::LevelPreparationStepV1::cancelled&&*reads==cancelled_reads,"Cancelled source replayed");
                require(p.latest_source().map().modules().size()==9,"Cancellation dropped prior source");++checks;
            }
            require(p.begin({"missing","__missing_candidate__.mlx"},error),error);
            require(drain(p)==loader::LevelPreparationStepV1::failed,"Missing source succeeded");
            const auto failed_reads=*reads;const auto failed_error=p.error();
            require(p.step()==loader::LevelPreparationStepV1::failed&&*reads==failed_reads&&p.error()==failed_error,"Failure replayed");
            require(!p.begin(swamp,error)&&p.latest_source().request().identity=="SWAMP","Failed candidate replaced");++checks;p.discard();
            loader::LevelSourceRequestV1 original_desert{"DESERT_CAVE_02",argv[2],loader::LevelSourceKindV1::procedural,0};original_desert.allow_original_backup=false;
            require(p.begin(original_desert,error),error);
            const auto no_layout=drain(p);
            require(no_layout==loader::LevelPreparationStepV1::failed,"Expected blocked candidate");
            require(p.failure()==loader::LevelPreparationFailureV1::original_no_layout,"No-layout result misclassified");
            require(p.latest_source().request().identity=="SWAMP","Generator failure published source");++checks;p.discard();
            loader::LevelSourceRequestV1 original_void{"VOID_MAZE_03",argv[4],loader::LevelSourceKindV1::procedural,1};original_void.repair_known_references=false;
            require(p.begin(original_void,error),error);
            require(drain(p)==loader::LevelPreparationStepV1::failed&&p.failure()==loader::LevelPreparationFailureV1::preparation,"Missing MGP succeeded");
            require(p.error().find("vm011_corner_voidmaze_ne_00_01.mgp")!=std::string::npos,"Missing dependency diagnostic lost");
            require(p.latest_source().request().identity=="SWAMP","Missing dependency published source");++checks;p.discard();
            require(p.begin({"SWAMP_02",argv[3],loader::LevelSourceKindV1::procedural,0},error),error);
            require(drain(p)==loader::LevelPreparationStepV1::source_ready,p.error());generated=p.latest_source();
            require(generated.request().identity=="SWAMP_02"&&generated.procedural_modules()&&generated.procedural_modules()->layout.source_owner,"Generated provenance lost");
            require(generated.map().modules().size()==11&&p.completed_stages()==11,"Generated stage/map mismatch");++checks;
            require(p.begin(swamp,error),error);require(drain(p)==loader::LevelPreparationStepV1::source_ready,p.error());++checks;
            require(retained.declarations().map().sources().documents().at(0).parsed(),"Older declaration source lost");
        }
        require(retained.map().sources().identity()=="SWAMP"&&retained.map().navigation().sewn,"Source owner lost after facade/cache teardown");++checks;
        require(generated.request().identity=="SWAMP_02"&&generated.map().sources().identity()=="SWAMP_02"&&generated.procedural_modules()->layout.seed==0,"Generated identity collapsed after teardown");++checks;
        for(const auto& declaration:retained.declarations().declarations())
            require(!retained.declarations().document(declaration).source().empty(),"Borrowed XML released");
        std::cout<<"{\"validation\":\"PASS\",\"lifecycle_checks\":"<<checks
            <<",\"modules\":"<<retained.map().modules().size()<<",\"declarations\":"<<retained.declarations().declarations().size()
            <<",\"runtime_objects_verified\":false,\"full_loader_verified\":false}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
