#include "player_equipment_queries_v1.hpp"
namespace {
bool aligned(const void* p,std::size_t n){auto a=reinterpret_cast<std::uintptr_t>(p);return p&&a%4==0&&a<=UINTPTR_MAX-n;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return b&&x<y+bn&&y<x+an;}
std::int32_t asr8(std::int32_t v){return v>=0?v/256:-std::int32_t((std::uint64_t(-std::int64_t(v))+255)/256);}
}
extern "C" int dh2_equipment_requirements_v1(std::int32_t* out,const dh2::player::EquipmentRequirements32V1* f,const dh2::data::ItemRecord164* row) noexcept {
 if(!aligned(out,sizeof(*out))||!aligned(f,sizeof(*f))||f->present>1||(f->present&&!aligned(row,sizeof(*row)))||overlap(out,sizeof(*out),f,sizeof(*f))||(f->present&&overlap(out,sizeof(*out),row,sizeof(*row))))return -1;
 std::int32_t result=1;if(!(f->online&&f->remote)&&f->present)for(unsigned i=0;i<5;++i)if(row->words[29+i]>asr8(f->cached[i])){result=0;break;}*out=result;return 0;
}
extern "C" int dh2_equipment_queries_v1(dh2::player::EquipmentQueries12V1* out,const dh2::data::ItemRecord164* main,const dh2::data::ItemRecord164* off,std::int32_t flag) noexcept {
 using namespace dh2::player;if(!aligned(out,sizeof(*out))||(main&&!aligned(main,sizeof(*main)))||(off&&!aligned(off,sizeof(*off)))||overlap(out,sizeof(*out),main,sizeof(*main))||overlap(out,sizeof(*out),off,sizeof(*off)))return -1;EquipmentQueries12V1 q{-1,-1,0};
 if(main){q.main_category=main->words[37];q.flags|=query_main;if(q.main_category==4)q.flags|=query_bow;if(q.main_category==5)q.flags|=query_staff;if(main->words[26]==-4){q.flags|=query_two_raw;auto type=std::uint32_t(main->words[22]);if(type-4<=1||flag==0)q.flags|=query_two_effective;}}
 if(off){q.off_category=off->words[37];q.flags|=off->words[22]==6?query_shield:query_dual;}*out=q;return 0;
}
