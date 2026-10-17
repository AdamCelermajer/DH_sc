// Reuse the verified canonical manager/online/nested internal-ID fixture.
// Its existing executable entry is renamed; only the new audit runs here.
#define main player_local_selection_existing_audit_main
#include "player_local_selection_v1.cpp"
#undef main
#include "../player_manager_friendly_v1.hpp"
namespace friendly=dh2::player_manager_friendly_v1;
namespace {
struct FriendlyFixture:Fixture {
    friendly::Services friendly_services{};
    explicit FriendlyFixture(const Row& row):Fixture(row) {
        span_count=static_cast<unsigned>(row[20]);
        queries.net_ids=[](void* context,const Registry* registry,const std::int32_t** ids,std::uint32_t* count) {
            auto& f=*static_cast<Fixture*>(context);require(registry==&f.registry,"same all-ID manager");
            *ids=f.span;*count=f.span_count;return f.note_raw(7,0,*count);
        };
        friendly_services={&queries,this,services.internal_id_670};
    }
    friendly::Result run_friendly() {
        friendly::Result result;friendly::Status status;
        if(row[0]==0) status=friendly::get_internal_id_by_friendly_id(&registry,&friendly_services,static_cast<int>(row[1]),static_cast<unsigned>(row[2]),&result);
        else if(row[0]==1) status=friendly::get_player(&registry,&friendly_services,static_cast<int>(row[1]),static_cast<unsigned>(row[2]),&result);
        else status=friendly::get_num_players(&registry,&friendly_services,&result);
        require(status==result.status,"friendly result status");return result;
    }
};
Row friendly_base(unsigned mode=0) {auto row=base(mode);row[20]=3;return row;}
void friendly_failures(unsigned& failed,unsigned& guards) {
    {friendly::SinglePlayerClassProjection p{};require(friendly::project_single_player_class(263,&p)==friendly::Status::complete&&p.visual_class==friendly::SinglePlayerVisualClass::warrior&&p.counts.warrior==1&&p.counts.rogue==0&&p.counts.mage==0,"Knight CharacterTable row projects warrior preview and loot class");++guards;}
    {friendly::SinglePlayerClassProjection p{};require(friendly::project_single_player_class(325,&p)==friendly::Status::complete&&p.visual_class==friendly::SinglePlayerVisualClass::rogue&&p.counts.warrior==0&&p.counts.rogue==1&&p.counts.mage==0,"Rogue CharacterTable row projects rogue preview and loot class");++guards;}
    {friendly::SinglePlayerClassProjection p{};require(friendly::project_single_player_class(290,&p)==friendly::Status::complete&&p.visual_class==friendly::SinglePlayerVisualClass::mage&&p.counts.warrior==0&&p.counts.rogue==0&&p.counts.mage==1,"Mage CharacterTable row projects mage preview and loot class");++guards;}
    for(const auto class_id:{77,94,120}){friendly::SinglePlayerClassProjection p{friendly::SinglePlayerVisualClass::rogue,{9,8,7}};require(friendly::project_single_player_class(class_id,&p)==friendly::Status::missing_projection&&p.visual_class==friendly::SinglePlayerVisualClass::rogue&&p.counts.mage==9&&p.counts.rogue==8&&p.counts.warrior==7,"source ClassID property is rejected without mutating projection");++guards;}
    {require(friendly::project_single_player_class(263,nullptr)==friendly::Status::invalid_argument,"class projection output is required");++guards;}
    {friendly::SinglePlayerClassCounts counts{9,9,9};require(friendly::project_single_player_class_counts(263,&counts)==friendly::Status::complete&&counts.warrior==1&&counts.rogue==0&&counts.mage==0,"Warrior source row projects one offline class count");++guards;}
    {friendly::SinglePlayerClassCounts counts{9,9,9};require(friendly::project_single_player_class_counts(325,&counts)==friendly::Status::complete&&counts.warrior==0&&counts.rogue==1&&counts.mage==0,"Rogue source row projects one offline class count");++guards;}
    {friendly::SinglePlayerClassCounts counts{9,9,9};require(friendly::project_single_player_class_counts(290,&counts)==friendly::Status::complete&&counts.warrior==0&&counts.rogue==0&&counts.mage==1,"Mage source row projects one offline class count");++guards;}
    {friendly::SinglePlayerClassCounts counts{9,9,9};require(friendly::project_single_player_class_counts(77,&counts)==friendly::Status::missing_projection&&counts.warrior==9&&counts.rogue==9&&counts.mage==9,"ClassID property is not a CharacterTable row");++guards;}
    {require(friendly::project_single_player_class_counts(263,nullptr)==friendly::Status::invalid_argument,"class-count output is required");++guards;}
    for(unsigned scenario=0;scenario<8;++scenario) {
        auto row=friendly_base(scenario%3);row[1]=scenario==7?-1:1;
        if(scenario>=3)row[3]=1;
        if(scenario==6)row[2]=1;
        FriendlyFixture baseline(row);auto result=baseline.run_friendly();
        require(result.status==friendly::Status::complete,"friendly failure baseline");
        for(unsigned step=0;step<baseline.attempts;++step)for(bool throwing:{false,true}) {
            FriendlyFixture f(row);if(throwing)f.throw_at=static_cast<int>(step);else f.fail_at=static_cast<int>(step);
            auto r=f.run_friendly();require(r.status==friendly::Status::provider_failed,"friendly explicit provider failure");
            require(f.trace.size()==step+1&&std::equal(f.trace.begin(),f.trace.end(),baseline.trace.begin()),"friendly completed provider prefix");
            ++failed;
        }
    }
    {FriendlyFixture f(friendly_base());f.friendly_services.internal_id_670=nullptr;auto r=f.run_friendly();require(r.status==friendly::Status::missing_provider&&f.trace.size()==1,"mandatory +670 field");++guards;}
    {FriendlyFixture f(friendly_base());f.friendly_services.queries=nullptr;auto r=f.run_friendly();require(r.status==friendly::Status::missing_provider&&f.trace.empty(),"missing online owner");++guards;}
    {FriendlyFixture f(friendly_base());f.registry.entries=nullptr;friendly::Result r;r.value=777;require(friendly::get_num_players(&f.registry,&f.friendly_services,&r)==friendly::Status::invalid_registry&&r.value==777&&f.trace.empty(),"invalid canonical registry");++guards;}
    {FriendlyFixture f(friendly_base());f.entries[0]=f.entries[1];friendly::Result r;require(friendly::get_player(&f.registry,&f.friendly_services,0,0,&r)==friendly::Status::invalid_registry&&f.trace.empty(),"canonical tree key order");++guards;}
    {FriendlyFixture f(friendly_base());auto* r=reinterpret_cast<friendly::Result*>(&f.registry);require(friendly::get_num_players(&f.registry,&f.friendly_services,r)==friendly::Status::invalid_argument&&f.trace.empty(),"output/registry alias");++guards;}
    {FriendlyFixture f(friendly_base());auto* r=reinterpret_cast<friendly::Result*>(&f.queries);require(friendly::get_player(&f.registry,&f.friendly_services,0,0,r)==friendly::Status::invalid_argument&&f.trace.empty(),"output/query alias");++guards;}
    {FriendlyFixture f(friendly_base());f.row[3]=1;f.span_count=4097;auto r=f.run_friendly();require(r.status==friendly::Status::missing_projection,"all-ID span bound");++guards;}
    {FriendlyFixture f(friendly_base());f.row[3]=1;f.span=nullptr;auto r=f.run_friendly();require(r.status==friendly::Status::missing_projection,"nonempty all-ID span needs backing");++guards;}
    {FriendlyFixture f(friendly_base());f.row[3]=1;f.row[17]=3;auto r=f.run_friendly();require(r.status==friendly::Status::missing_projection&&f.internal_calls==1,"matched captured index beyond shrunken span is native error");++guards;}
    {FriendlyFixture f(friendly_base());f.queries.acquire_matching=[](void*,dh2::player_locality_v1::Matching** p){*p=nullptr;return 0;};f.row[3]=1;auto r=f.run_friendly();require(r.status==friendly::Status::missing_projection,"missing Matching projection");++guards;}
    {FriendlyFixture f(friendly_base());f.queries.acquire_net_manager=[](void*,std::uintptr_t* p){*p=0;return 0;};f.row[3]=1;auto r=f.run_friendly();require(r.status==friendly::Status::missing_projection,"missing net manager projection");++guards;}
    {FriendlyFixture f(friendly_base());f.queries.internal_id_player=[](void*,const Registry*,int,unsigned,PlayerInfo** p){*p=nullptr;return 0;};f.row[3]=1;auto r=f.run_friendly();require(r.status==friendly::Status::missing_projection,"missing actual internal player");++guards;}
    {FriendlyFixture f(friendly_base(1));f.internal_words[1]=88;auto r=f.run_friendly();require(r.value==88&&r.player==&f.players[0],"friendly returns real +670 not map key");++guards;}
    {FriendlyFixture f(friendly_base());f.controlled.fill(0);f.row[1]=1;auto r=f.run_friendly();require(r.value==7,"friendly scan ignores local controller flag");++guards;}
    {FriendlyFixture f(friendly_base());f.row[3]=1;f.row[2]=0;auto r=f.run_friendly();require(r.value==2&&f.internal_calls==1,"network unfiltered ordinal still resolves internal player");++guards;}
}
}
int main(int argc,char** argv) {
    try {
        if(argc!=2)throw std::runtime_error("input rows required");
        std::ifstream input(argv[1]);if(!input)throw std::runtime_error("input open");
        std::vector<Row> rows;Row row{};while(input>>row[0]){for(unsigned i=1;i<row.size();++i)if(!(input>>row[i]))throw std::runtime_error("partial row");rows.push_back(row);}
        unsigned failures=0,guards=0;friendly_failures(failures,guards);
        std::cout<<"{\"validation\":\"PASS\",\"failure_prefix_cases\":"<<failures<<",\"guard_cases\":"<<guards<<",\"results\":[";
        bool comma=false;
        for(const auto& row:rows) {
            FriendlyFixture f(row);auto r=f.run_friendly();require(r.status==friendly::Status::complete,"friendly comparison complete");
            if(comma)std::cout<<',';
            comma=true;
            std::cout<<"{\"value\":"<<r.value<<",\"player\":"<<(r.player?r.player->identity:0)<<",\"trace\":";print_trace(f.trace);std::cout<<'}';
        }
        std::cout<<"],\"checks\":"<<checks<<'}'<<std::endl;return 0;
    } catch(const std::exception& e) {std::cerr<<e.what()<<std::endl;return 1;}
}
