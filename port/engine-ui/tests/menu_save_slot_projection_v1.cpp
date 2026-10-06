#include "../menu_save_slot_projection_v1.hpp"
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <vector>
using namespace dh2::ui;
struct Probe { std::vector<std::uint32_t> ids; int constants=0,dates=0,reject=0; };
static bool constant(void* p,const char* group,const char* key,std::int32_t& value,std::string& e){
    auto& s=*static_cast<Probe*>(p);s.constants++;
    if(std::string(group)!="StrID"||std::string(key)!="GAMEPLAYMENUS_LEVEL"){e="wrong constant";return false;}
    if(s.reject==1){e="constant rejected";return false;}value=77;return true;
}
static bool string_id(void* p,std::uint32_t id,std::string& value,std::string& e){
    auto& s=*static_cast<Probe*>(p);s.ids.push_back(id);
    if(s.reject==2){e="text rejected";return false;}
    value=id==77?"LEVEL":("localized:"+std::to_string(id));return true;
}
static bool date(void* p,std::uint32_t raw,std::tm& value,std::string& e){
    auto& s=*static_cast<Probe*>(p);s.dates++;
    if(s.reject==3){e="date rejected";return false;}
    return menu_save_slot_local_date_v1(raw,value,e);
}
static void require(bool b){if(!b){std::fprintf(stderr,"FAIL\n");std::exit(1);}}
int main(){
    setenv("TZ","UTC",1);tzset();
    std::tm t{};std::string error;
    require(menu_save_slot_local_date_v1(0xffffffffu,t,error));
    require(t.tm_year==69&&t.tm_mon==11&&t.tm_mday==31&&t.tm_hour==23&&t.tm_min==59&&t.tm_sec==59);
    require(menu_save_slot_local_date_v1(0x80000000u,t,error));
    require(t.tm_year==1&&t.tm_mon==11&&t.tm_mday==13&&t.tm_hour==20&&t.tm_min==45&&t.tm_sec==52);
    dh2::data::CharacterTable chars;chars.rows.resize(3);
    dh2::data::LevelTables levels;levels.levels.resize(4);
    for(int i=0;i<3;++i)chars.rows[i][5]=101+i;
    for(int i=0;i<4;++i)levels.levels[i].scalar.words[9]=201+i;
    dh2::data::MenuProfileMetadataV1 profile;
    profile.slot=2;profile.name="Prince";profile.unlocked_difficulty=2;
    profile.location.levels={{-1,2,3}};profile.location.current_acts={{1,2,3}};
    profile.location.volatile_acts={{4,5,6}};profile.location.save_date=0;
    int cases=0;
    for(int row=0;row<3;++row)for(int difficulty=0;difficulty<3;++difficulty)
      for(int mode=0;mode<2;++mode)for(int language=0;language<8;++language)
      for(auto level: {std::numeric_limits<std::int32_t>::min(),1,std::numeric_limits<std::int32_t>::max()}){
        profile.character_row=row;profile.selected_difficulty=difficulty;profile.level=level;
        Probe p;MenuSaveSlotPresentationServicesV1 services{&p,constant,string_id,date};
        SwfFrontSaveSlotDetailsV1 out;
        require(project_menu_save_slot_v1(profile,chars,levels,-1,mode,language,services,out,error));
        const int location=difficulty==0?0:difficulty+1;
        require(p.ids==std::vector<std::uint32_t>({static_cast<std::uint32_t>(101+row),77,static_cast<std::uint32_t>(201+location)}));
        require(p.constants==1&&p.dates==1&&out.slot_id==2&&out.in_use&&out.player_name=="Prince");
        require(out.string_class_level=="localized:"+std::to_string(101+row)+" LEVEL "+std::to_string(level));
        require(out.current_act==(mode?4:1)+difficulty&&out.difficulty==difficulty&&out.difficulty_unlocked==2);
        const std::string expected=language==2?"01.01.     00:00":(language>=4&&language<=6?"01.01.     00:00":"01/01     00:00");
        require(out.last_save==expected);++cases;
      }
    profile.character_row=0;profile.selected_difficulty=99;
    Probe p;MenuSaveSlotPresentationServicesV1 services{&p,constant,string_id,date};SwfFrontSaveSlotDetailsV1 out;out.player_name="sentinel";
    require(project_menu_save_slot_v1(profile,chars,levels,1,false,0,services,out,error)&&out.difficulty==1);
    out.player_name="sentinel";
    require(!project_menu_save_slot_v1(profile,chars,levels,-1,false,0,services,out,error)&&out.player_name=="sentinel");
    profile.selected_difficulty=0;
    for(int reject=1;reject<4;++reject){p.reject=reject;require(!project_menu_save_slot_v1(profile,chars,levels,-1,false,0,services,out,error)&&out.player_name=="sentinel"&&!error.empty());}
    p.reject=0;profile.location.levels[0]=-2;
    require(!project_menu_save_slot_v1(profile,chars,levels,-1,false,0,services,out,error)&&out.player_name=="sentinel");
    profile.location.levels[0]=4;require(!project_menu_save_slot_v1(profile,chars,levels,-1,false,0,services,out,error));
    profile.location.levels[0]=0;profile.character_row=3;require(!project_menu_save_slot_v1(profile,chars,levels,-1,false,0,services,out,error));
    profile.character_row=0;services.local_date=nullptr;require(!project_menu_save_slot_v1(profile,chars,levels,-1,false,0,services,out,error));
    std::printf("PASS %d native presentation cases; signed ARM32 timestamps, difficulty/act selection, fallback, provider failures and atomic output\n",cases);
}
