#include "../character_sneaking_tables.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
using namespace dh2::character::sneaking;
using dh2::data::SkillTables;using Raw=std::vector<std::uint8_t>;
unsigned checks=0,cases=0,calls=0,guards=0,active_calls=0,pre_calls=0;
void verify(bool ok,int line){++checks;if(!ok)throw std::runtime_error("Sneaking adapter audit line "+std::to_string(line));}
#define check(x) verify((x),__LINE__)
Raw file(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f));return Raw(std::istreambuf_iterator<char>(f),{});}
void word(Raw& b,std::uint32_t v){for(unsigned i=0;i<4;++i)b.push_back(static_cast<std::uint8_t>(v>>(i*8)));}
void text(Raw& b,const Raw& s){word(b,s.size());b.insert(b.end(),s.begin(),s.end());}
dh2::data::Bytes bytes(const Raw& r){return {r.data(),r.size()};}
struct Fixture {
 std::array<std::int32_t,224> props{};std::array<std::uintptr_t,64> scripts{};
 Character48 character{};AI24 ai{};Services16 service{this,invoke};
 std::unique_ptr<SkillTables>* owner=nullptr;std::vector<Request24> trace;
 unsigned player=0,active=1;int fail=-1,mode=0;
 Fixture(const SneakingTables& t){character={0x100000001ULL,{props.data(),224,0},&t.view(),&ai,0xa5,{}};ai={&character,scripts.data(),64,0};for(unsigned i=0;i<64;++i)scripts[i]=0x200000001ULL+i;props[198]=1;}
 static int invoke(void* context,const Request24* r,std::uint32_t* out){auto& f=*static_cast<Fixture*>(context);check(!r->reserved);f.trace.push_back(*r);++calls;
  if(static_cast<int>(r->operation)==f.fail)return 1;
  if(r->operation==is_player){*out=f.player;if(f.owner)f.owner->reset();return 0;}
  if(r->operation==delete_buff){check(r->argument==0x92);if(f.mode==1)f.props[198]=0;*out=0;return 0;}
  if(r->operation==skill_check_active){++active_calls;check(r->receiver==f.scripts[r->index]);if(f.mode==2)f.scripts[r->index]=0x300000007ULL;*out=f.active;return 0;}
  if(r->operation==skill_pre){++pre_calls;check(r->receiver==f.scripts[r->index]);*out=0;return 0;}
  return 1;
 }
 void reset(){trace.clear();player=0;active=1;fail=-1;mode=0;character.changed415=0xa5;props[198]=1;}
};
void equal(const SneakingTables& t){const auto& view=t.view();const auto& source=t.source();check(view.list_count==source.lists().size()&&view.skill_count==source.skills().size());check(!view.reserved0&&!view.reserved1);
 for(unsigned i=0;i<view.list_count;++i){check(view.lists[i].ids==source.lists()[i].data()&&view.lists[i].count==source.lists()[i].size()&&!view.lists[i].reserved);}
 for(unsigned i=0;i<view.skill_count;++i)for(unsigned j=0;j<19;++j)check(view.skills[i].words[j]==source.skills()[i].scalar.words[j]);
}
int main(int argc,char** argv){try{
 check(argc==4);static_assert(!std::is_copy_constructible_v<SneakingTables>&&!std::is_move_constructible_v<SneakingTables>);
 auto records=file(argv[1]),names=file(argv[2]),schema=file(argv[3]);
 auto owner=std::make_unique<SkillTables>();std::string error;check(owner->load(bytes(records),bytes(names),bytes(schema),error));SneakingTables t(owner->borrow());equal(t);check(t.view().list_count==36&&t.view().skill_count==127);
 for(const auto& r:t.source().skills())check(!(r.scalar.words[7]&0x2000000));
 check(!owner->load(bytes(records),bytes(names),bytes(schema),error)&&error.find("pinned")!=std::string::npos);++guards;
 Fixture f(t);f.owner=&owner;f.props[28]=14;check(!dh2_character_cancel_sneaking(&f.character,&f.service)&&!owner);check(f.trace.size()==1&&f.character.changed415==0xa5);
 auto saved_skills=t.source().skills();auto saved_lists=t.source().lists();auto saved_names=t.source().skill_names();
 std::fill(records.begin(),records.end(),0xff);std::fill(names.begin(),names.end(),0xff);records.clear();records.shrink_to_fit();names.clear();names.shrink_to_fit();equal(t);
 check(t.source().lists()==saved_lists&&t.source().skill_names()==saved_names);
 for(unsigned i=0;i<saved_skills.size();++i){check(t.source().skills()[i].script==saved_skills[i].script);check(t.source().skills()[i].icon==saved_skills[i].icon);check(t.source().skills()[i].display_props==saved_skills[i].display_props);}
 std::vector<std::int32_t> indices={std::numeric_limits<std::int32_t>::min(),-1};for(int i=0;i<=36;++i)indices.push_back(i);indices.push_back(std::numeric_limits<std::int32_t>::max());
 for(auto tree:indices){for(unsigned player=0;player<2;++player){f.reset();f.player=player;f.props[28]=tree;check(!dh2_character_cancel_sneaking(&f.character,&f.service));check(f.trace.size()==1+player&&f.character.changed415==(player?1:0xa5));++cases;}
  auto selected=tree<0||tree>=36?3:tree;const auto& ids=t.source().lists()[selected];
  for(unsigned slot=0;slot<ids.size();++slot)for(unsigned active=0;active<2;++active){f.reset();f.props[28]=tree;f.active=active;f.mode=2;check(!dh2_character_cancel_skill(&f.ai,slot,&f.service));auto type=t.source().skills()[ids[slot]].scalar.words[18];check(f.trace.size()==(type==1?1+active:0));if(type==1&&active)check(f.trace.back().receiver==0x300000007ULL);f.scripts[slot]=0x200000001ULL+slot;++cases;}
 }
 for(auto sneak:{std::numeric_limits<std::int32_t>::min(),-1,0,1,std::numeric_limits<std::int32_t>::max()}){f.reset();f.props[28]=-1;f.props[198]=sneak;check(!dh2_character_cancel_sneaking(&f.character,&f.service)&&f.trace.size()==1);++cases;}
 f.reset();f.player=1;f.fail=delete_buff;check(dh2_character_cancel_sneaking(&f.character,&f.service)==2&&f.character.changed415==0xa5);++guards;
 f.reset();f.player=1;f.mode=1;check(!dh2_character_cancel_sneaking(&f.character,&f.service)&&f.props[198]==0&&f.character.changed415==1);++cases;
 f.reset();check(dh2_character_cancel_skill(&f.ai,64,&f.service)==1&&f.trace.empty());++guards;
 f.reset();f.scripts[0]=0;f.ai.owner=nullptr;check(!dh2_character_cancel_skill(&f.ai,0,&f.service)&&f.trace.empty());++cases;f.ai.owner=&f.character;
 // Synthetic authored values exercise the owned adapter, not cache Sneak defaults.
 // Four lists preserve signed invalid references, fallback index3, and order.
 Raw synthetic;word(synthetic,4);word(synthetic,1);word(synthetic,0xffffffff);word(synthetic,1);word(synthetic,0x80000000);word(synthetic,1);word(synthetic,1);word(synthetic,2);word(synthetic,1);word(synthetic,0);word(synthetic,2);
 for(unsigned i=0;i<2;++i){word(synthetic,0x80000000u+i);synthetic.push_back(255);word(synthetic,2);word(synthetic,0xffffffff);word(synthetic,0x80000000);word(synthetic,7);synthetic.push_back(128);word(synthetic,i?0:0x02000000);word(synthetic,5);text(synthetic,{'s',0,'x'});synthetic.push_back(254);word(synthetic,9);word(synthetic,10);text(synthetic,{'i',0,'y'});word(synthetic,11);word(synthetic,12);word(synthetic,1);}
 Raw synthetic_names;word(synthetic_names,4);for(unsigned i=0;i<4;++i)text(synthetic_names,{'L',0,static_cast<std::uint8_t>('0'+i)});word(synthetic_names,2);text(synthetic_names,{'S',0,'a'});text(synthetic_names,{'S',0,'b'});
 SkillTables synthetic_owner;check(synthetic_owner.load(bytes(synthetic),bytes(synthetic_names),bytes(schema),error));SneakingTables st(synthetic_owner.borrow());equal(st);check(st.source().list_index("L")==0&&st.source().skill_index("S")==0);check(st.source().skills()[0].script==std::string("s\0x",3)&&st.source().skills()[0].icon==std::string("i\0y",3));check(st.source().skills()[0].display_props==std::vector<std::int32_t>({-1,std::numeric_limits<std::int32_t>::min()}));check(st.view().skills[0].words[2]==255&&st.view().skills[0].words[6]==128&&st.view().skills[0].words[11]==254);
 Fixture sf(st);for(auto tree:{-1,3,4,std::numeric_limits<std::int32_t>::min(),std::numeric_limits<std::int32_t>::max()}){sf.reset();sf.props[28]=tree;sf.mode=2;check(!dh2_character_cancel_sneaking(&sf.character,&sf.service));check(sf.trace.size()==3&&sf.trace[1].index==1&&sf.trace[2].receiver==0x300000007ULL);sf.scripts[1]=0x200000002ULL;++cases;}
 for(auto tree:{0,1}){sf.reset();sf.props[28]=tree;check(dh2_character_cancel_sneaking(&sf.character,&sf.service)==1&&sf.trace.size()==1);++guards;}
 sf.reset();sf.props[28]=2;check(!dh2_character_cancel_sneaking(&sf.character,&sf.service)&&sf.trace.size()==1);++cases;
 for(unsigned active=0;active<2;++active){sf.reset();sf.props[28]=3;sf.active=active;check(!dh2_character_cancel_skill(&sf.ai,1,&sf.service)&&sf.trace.size()==1+active);++cases;}
 sf.reset();sf.props[28]=3;sf.fail=skill_pre;check(dh2_character_cancel_skill(&sf.ai,1,&sf.service)==2&&sf.trace.size()==2);++guards;
 std::fill(schema.begin(),schema.end(),0xff);schema.clear();schema.shrink_to_fit();std::fill(synthetic.begin(),synthetic.end(),0xff);synthetic.clear();synthetic.shrink_to_fit();synthetic_names.clear();synthetic_names.shrink_to_fit();equal(st);equal(t);check(t.source().skill_fields().size()==15&&t.source().skill_fields().back()=="Type"&&st.source().skills()[0].script==std::string("s\0x",3));
 bool rejected=false;try{SneakingTables missing{SkillTables::Borrow{}};}catch(const std::invalid_argument&){rejected=true;}check(rejected);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"cases\":"<<cases<<",\"ordered_calls\":"<<calls<<",\"active_calls\":"<<active_calls<<",\"pre_calls\":"<<pre_calls<<",\"guards\":"<<guards<<",\"actual_lists\":36,\"actual_skills\":127,\"pinned_after_loader_and_inputs_destroyed\":true,\"full_DelBuff\":false,\"full_skill_VM\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
