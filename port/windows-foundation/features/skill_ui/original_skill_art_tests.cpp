#include "original_skill_art.hpp"
#include "../../../game-data/skill_tables.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace dh::foundation;using namespace dh::foundation::skill_ui;
using Raw=std::vector<std::uint8_t>;
Raw read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error(p);return {std::istreambuf_iterator<char>(f),{}};}
void check_at(bool b,int line){if(!b)throw std::runtime_error("original skill art test failed line "+std::to_string(line));}
#define check(x) check_at((x),__LINE__)
int main(int argc,char**argv){try{check(argc==2);std::string root=argv[1],e;auto d=read(root+"/skills_pyarray.bin"),n=read(root+"/skills_pyarraynames.bin"),s=read(root+"/skills_pystructnames.bin");dh2::data::SkillTables t;check(t.load({d.data(),d.size()},{n.data(),n.size()},{s.data(),s.size()},e));auto b=t.borrow();unsigned icons=0,hits=0;
 check(original_skill_icon_states().size()==78&&original_skill_icon_placements().size()==105);
 for(const char* name:{"Knight","KnightBerserker","KnightPaladin","Rogue","RogueArcher","RogueAssassin","Mage","MageIllusionist","MageNecromancer"}){int list=b.list_index(name);unsigned cf=std::string(name).find("Knight")==0?0:std::string(name).find("Rogue")==0?1:2;int p=0;for(int id:b.lists()[list]){HudGeometry g;if(!append_original_skill_icon(cf,p,b.skills()[id].icon,g,e))throw std::runtime_error(e+" class="+name+" position="+std::to_string(p));for(const auto& batch:g.batches)for(const auto& v:batch.triangles)check(std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.u)&&std::isfinite(v.v));++p;++icons;}}
 for(unsigned cf=0;cf<3;++cf){bool slots[3]={};bool train=false;const auto& zones=original_skill_hit_zones(cf);check(zones.size()==36);for(const auto& z:zones){check(z.triangles.size()>=3);if(z.kind==HitKind::assign){check(z.position>=0&&z.position<3);slots[z.position]=true;}if(z.kind==HitKind::train)train=true;const auto&a=z.triangles[0];const auto&bb=z.triangles[1];const auto&c=z.triangles[2];auto hit=original_skill_hit(cf,(a.x+bb.x+c.x)/3,(a.y+bb.y+c.y)/3);if(!hit||hit->kind!=z.kind||hit->position!=z.position)throw std::runtime_error("hit mismatch "+z.path+" got="+(hit?hit->path:"none"));++hits;}check(slots[0]&&slots[1]&&slots[2]&&train);for(int slot=0;slot<3;++slot){HudGeometry g;check(append_original_skill_slot_icon(cf,slot,"blank",g,e));}}
 HudGeometry unchanged;unchanged.batches.push_back({"sentinel",0,{}});check(!append_original_skill_icon(0,0,"absent-original-label",unchanged,e)&&unchanged.batches.size()==1);check(!append_original_skill_icon(3,0,"blank",unchanged,e));check(!append_original_skill_icon(0,16,"blank",unchanged,e));check(!original_skill_hit(0,-100,-100));
 std::cout<<"{\"validation\":\"PASS\",\"actual_player_skill_icons\":"<<icons<<",\"source_hit_centroids\":"<<hits<<",\"original_icon_labels\":78}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}


