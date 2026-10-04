// Reuses the complete source-gold service projection without changing its test.
#define main historical_manager_gold_main
#include "hud_manager.cpp"
#undef main
namespace {
struct Reentry:Fixture {
 unsigned trigger=0;bool entered=false;
 static int invoke(void*p,HudManagerState*s,const HudManagerRequest*q,HudManagerResponse*r){auto&t=*static_cast<Reentry*>(p);auto rc=Fixture::invoke(&t,s,q,r);if(rc==1&&unsigned(q->operation)==t.trigger&&!t.entered){t.entered=true;HudManagerServices services{&t,invoke};check(dh2_ui_hud_manager_v1(s,4,&services)==0,"Nested source SlowUpdate failed");}return rc;}
 int run_nested(){HudManagerServices services{this,invoke};return dh2_ui_hud_manager_v1(&state,cfg[0],&services);}
};
}
int main(int argc,char**argv){try{check(argc==2,"reentry gold argument");Reader r(argv[1]);check(r.get<unsigned>()==0x31525548,"reentry magic");auto count=r.get<unsigned>();unsigned calls=0,nested=0;
 for(unsigned n=0;n<count;++n){Reentry f;f.trigger=r.get<unsigned>();for(auto&v:f.cfg)v=r.get<unsigned>();std::array<unsigned,4>after;for(auto&v:after)v=r.get<unsigned>();auto num=r.get<unsigned>();std::vector<std::vector<unsigned char>> expected;for(unsigned i=0;i<num;++i)expected.push_back(r.bytes(r.get<unsigned>()));f.initialize();check(f.run_nested()==0&&f.entered,"Native reentry not delivered");check(f.calls==expected,"Native nested ordered callbacks differ from original");check(f.id(reinterpret_cast<std::uintptr_t>(f.state.cached_target))==after[0]&&f.state.initialized==after[1]&&unsigned(f.state.slow_ms)==after[2]&&f.state.render_fx==after[3],"Native nested after-state differs");++nested;calls+=num;}
 std::cout<<"{\"validation\":\"PASS\",\"cases\":"<<count<<",\"nested_same_manager_calls\":"<<nested<<",\"ordered_services\":"<<calls<<"}\n";return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
