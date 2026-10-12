#include "level_preparation_v1.hpp"
#include "source_reference_repairs_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
static assets::ZipAssetPackV1 mount(const char* path,std::shared_ptr<bool> deny={}) {
    auto file=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);
    require(bool(*file),"Cache missing");auto size=file->tellg();require(size>=0,"Cache size missing");
    assets::ZipBackingV1 b;b.owner=file;b.bytes=std::uint64_t(size);
    b.read=[file,deny](std::uint64_t at,void* dst,std::size_t n,std::string& e){
        if(deny&&*deny){e="Injected backup read fault";return false;}
        file->clear();file->seekg(std::streamoff(at));file->read(static_cast<char*>(dst),std::streamsize(n));
        if(!*file){e="Read failed";return false;}return true;
    };
    assets::ZipAssetPackV1 p;std::string e;require(p.mount(std::move(b),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);return p;
}
static loader::LevelPreparationStepV1 drain(loader::LevelPreparationV1& p) {
    for(unsigned i=0;i<20;++i){auto step=p.step();if(step!=loader::LevelPreparationStepV1::pending)return step;}
    throw std::runtime_error("Stage loop");
}
int main(int argc,char** argv) {
    try {
        require(argc==4,"Expected real cache, missing-target fixture, authored-existing fixture");
        unsigned checks=0;std::string e;auto deny=std::make_shared<bool>(false);
        auto pack=mount(argv[1],deny);loader::LevelPreparationV1::Borrow backup,repaired;
        using namespace loader;
        const LevelSourceRequestV1 swamp{"SWAMP","001_swamp.mlx",LevelSourceKindV1::fixed,0};
        const LevelSourceRequestV1 icy{"ICY_CAVERN_02","027_icy_cavern_02.rule.xml",LevelSourceKindV1::procedural,0};
        {
            LevelPreparationV1 p(pack);require(p.begin(swamp,e),e);require(drain(p)==LevelPreparationStepV1::source_ready,p.error());
            for(unsigned stop=0;stop<10;++stop) {
                require(p.begin(icy,e),e);
                for(unsigned i=0;i<stop;++i)require(p.step()==LevelPreparationStepV1::pending,p.error());
                require(p.latest_source().request().identity=="SWAMP","Unpublished fallback replaced SWAMP");
                p.discard();require(p.step()==LevelPreparationStepV1::cancelled,"Cancelled fallback replayed");
                require(p.latest_source().request().identity=="SWAMP","Cancelled fallback replaced SWAMP");++checks;
            }
            require(p.begin(icy,e),e);
            for(unsigned i=0;i<6;++i)require(p.step()==LevelPreparationStepV1::pending,p.error());
            require(p.stage()==LevelPreparationStageV1::backup_sources,"No fallback stage after generator false");
            *deny=true;require(p.step()==LevelPreparationStepV1::failed,"Unreadable backup succeeded");
            *deny=false;require(p.failure()==LevelPreparationFailureV1::original_backup_unavailable,"Backup failure misclassified");
            require(p.error().find("x27_icy_cavern_02_BACKUP.mlx")!=std::string::npos,"Selected backup diagnostic missing");
            require(p.latest_source().request().identity=="SWAMP","Backup failure replaced prior map");
            auto cause=p.error();require(p.step()==LevelPreparationStepV1::failed&&p.error()==cause,"Failed backup retried itself");++checks;p.discard();
            require(p.begin(icy,e),e);require(drain(p)==LevelPreparationStepV1::source_ready,p.error());backup=p.latest_source();
            require(backup.request().definition==icy.definition&&backup.resolution().backup_used&&backup.resolution().original_no_layout,"Fallback provenance missing");
            require(backup.resolution().definition=="x27_icy_cavern_02_BACKUP.mlx"&&backup.map().modules().size()==13&&p.completed_stages()==10,"Wrong backup assembly");
            require(!backup.procedural_modules()&&!backup.attempted_layout().generated&&backup.attempted_layout().source_owner,"Failed original layout ownership missing");++checks;
            require(p.begin({"DESERT_CAVE_02","data/scene/017_red_desert_cave_02.rule.xml",LevelSourceKindV1::procedural,0},e),e);
            require(drain(p)==LevelPreparationStepV1::source_ready,p.error());
            require(p.latest_source().resolution().definition=="data/scene/x17_red_desert_cave_02_BACKUP.mlx"&&p.latest_source().map().modules().size()==10,"Qualified fallback selected incorrectly");++checks;
            require(p.begin({"VOID_MAZE_03","035_voidmaze_03.rule.xml",LevelSourceKindV1::procedural,1},e),e);
            require(drain(p)==LevelPreparationStepV1::source_ready,p.error());repaired=p.latest_source();
            require(repaired.procedural_modules()&&repaired.procedural_modules()->reference_repairs.size()==1&&repaired.map().modules().size()==7,"Void repair missing");
            const auto& repair=repaired.procedural_modules()->reference_repairs[0];
            require(repair.authored_uri.find("vm011_")!=std::string::npos&&repair.resolved_uri.find("/vm01_")!=std::string::npos,"Repair provenance lost");
            require(!repaired.resolution().backup_used&&repaired.attempted_layout().generated,"Repair incorrectly replaced generator");++checks;
            LevelSourceRequestV1 raw{"VOID_MAZE_03","035_voidmaze_03.rule.xml",LevelSourceKindV1::procedural,1};raw.repair_known_references=false;
            require(p.begin(raw,e),e);require(drain(p)==LevelPreparationStepV1::failed,"Original mode silently repaired references");
            require(p.latest_source().request().identity=="VOID_MAZE_03","Original-mode failure replaced prepared map");++checks;p.discard();
            auto raw_icy=icy;raw_icy.allow_original_backup=false;
            require(p.begin(raw_icy,e),e);require(drain(p)==LevelPreparationStepV1::failed&&p.failure()==LevelPreparationFailureV1::original_no_layout,"Original mode silently selected backup");++checks;p.discard();
        }
        require(backup.map().navigation().sewn&&backup.attempted_layout().source_owner&&repaired.procedural_modules()->reference_repairs.size()==1,"Provenance owner lost after facade destruction");++checks;
        ProceduralModulePlanV1 plan;ProceduralModuleV1 module;module.tile=4;
        module.overrides={{"mgp","data/iphone/3d/modules/void_maze/mgp/vm011_corner_voidmaze_ne_00_01.mgp"},{"mvp","data/iphone/3d/modules/void_maze/mvp/vm01_x_voidmaze_nswe_00"}};
        plan.modules.push_back(module);require(repair_procedural_references_v1(pack,plan,e),e);
        require(plan.reference_repairs.size()==2&&plan.modules[0].overrides.at("mvp").find(".mvp")!=std::string::npos,"Both documented repairs not applied");
        require(repair_procedural_references_v1(pack,plan,e)&&plan.reference_repairs.size()==2,"Repair reapplication duplicated provenance");++checks;
        ProceduralModulePlanV1 missing;missing.modules.push_back(module);
        require(!repair_procedural_references_v1(mount(argv[2]),missing,e),"Absent target silently accepted");
        require(missing.modules[0].overrides==module.overrides&&missing.reference_repairs.empty(),"Failed repair partially mutated plan");++checks;
        ProceduralModulePlanV1 authored;module.overrides.erase("mvp");authored.modules.push_back(module);
        require(repair_procedural_references_v1(mount(argv[3]),authored,e),e);
        require(authored.reference_repairs.empty()&&authored.modules[0].overrides==module.overrides,"Existing authored resource replaced");++checks;
        std::cout<<"{\"validation\":\"PASS\",\"recovery_checks\":"<<checks<<",\"backup_read_fault_retains_previous_map\":true,\"repair_rollback_verified\":true,\"original_mode_verified\":true,\"runtime_objects_verified\":false}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
