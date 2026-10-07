#include "../player_save_identity_authority_v29.hpp"
#include "../../game-data/ai.hpp"
#include "../../game-data/properties.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
using namespace dh2;
static unsigned checks;
static void check(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
static std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f),p);return {std::istreambuf_iterator<char>(f),{}};}
static data::Bytes view(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
struct Source {
 const data::AiTables& ai;data::PropertyState& properties;unsigned virtuals{},reads{},writes{};
 static bool player(void* raw,bool& out,std::string& e){auto& s=*static_cast<Source*>(raw);++s.virtuals;const auto* row=data::ai_props(s.ai,s.properties.resolved[1]);if(!row){e="Actual cache AI row unavailable";return false;}out=row->type==1;return true;}
};
int main(int argc,char** argv){try{
 check(argc==2,"Expected actual original cache data directory");const std::string path=argv[1];std::string error;
 auto a=read(path+"/character_properties_pyarray.bin"),b=read(path+"/character_properties_pyarraynames.bin"),c=read(path+"/character_properties_pystructnames.bin");
 data::CharacterTable table;check(data::load_characters(view(a),view(b),view(c),table,error),error);data::PropertyRules rules;check(data::load_property_rules(table,rules,error),error);
 const auto at=std::find(table.names.begin(),table.names.end(),"KnightPlayerBase");check(at!=table.names.end(),"Actual authored player fallback unavailable");const auto knight=std::int32_t(at-table.names.begin());
 a=read(path+"/ai_pyarray.bin");b=read(path+"/ai_pyarraynames.bin");c=read(path+"/ai_pystructnames.bin");data::AiTables ai;
 auto fa=read(path+"/ai_factions_pyarray.bin"),fb=read(path+"/ai_factions_pyarraynames.bin"),fc=read(path+"/ai_factions_pystructnames.bin");
 check(data::load_ai(view(a),view(b),view(c),view(fa),view(fb),view(fc),ai,error),error);
 a=read(path+"/character_classes_pyarray.bin");b=read(path+"/character_classes_pyarraynames.bin");c=read(path+"/character_classes_pystructnames.bin");data::ClassTables classes;
 check(data::load_classes(view(a),view(b),view(c),classes,error),error);
 data::PropertyState properties;data::reset_properties(rules,properties,&table.rows[std::size_t(knight)]);check(data::recalc_properties_with_class(classes,rules,properties,error),error);
 Source source{ai,properties};data::LootRandom8V2 random{};auto save=std::make_shared<data::PlayerSavegameV1>();save->set_character(0x100000001ULL);std::int16_t metadata=-1;
 data::PlayerSaveLoadServicesV1 reads;reads.owner=save;reads.invoke=[&](const auto&,auto&,std::string& e){++source.reads;e="Required actual positive save profile provider";return false;};
 data::PlayerSaveWriteServicesV1 writes;writes.owner=save;writes.invoke=[&](const auto&,auto&,std::string& e){++source.writes;e="Required actual positive save writer provider";return false;};
 player::PlayerSaveIdentityAuthorityV29 authority(save,save->character(),metadata,table,random,{&source,Source::player,nullptr,nullptr},reads,writes);
 check(&authority.receiver()==save.get()&&authority.load_owner()->save().character()==save->character(),"Save C1 adoption replaced the actual receiver");
 check(authority.set_slot(-1,error),error);std::int32_t id{};
 check(authority.safe_properties_id(id,error),error);check(id==knight&&metadata==knight&&save->class_id()==knight,"Source SG_Load(1)/fallback/cached metadata publication differs");
 check(source.virtuals==1&&source.reads==0&&!authority.load_owner()->profile().identity,"NULL profile branch fabricated a file");
 const auto before=properties;check(authority.save(error)&&source.writes==0,"NULL profile SG_Save must return before writer callbacks");
 check(authority.safe_properties_id(id,error)&&source.virtuals==1&&source.reads==0,"Cached property ID should be the source leaf");
 check(properties.base==before.base&&properties.saved==before.saved&&properties.resolved==before.resolved,"Metadata lookup changed property/gear authority");
 // A real nonnegative slot reaches the actual filename/profile source boundary.
 auto second=std::make_shared<data::PlayerSavegameV1>();second->set_character(0x100000002ULL);std::int16_t second_metadata=-1;
 player::PlayerSaveIdentityAuthorityV29 missing(second,second->character(),second_metadata,table,random,{&source,Source::player,nullptr,nullptr},reads,writes);
 check(missing.set_slot(0,error)&&!missing.safe_properties_id(id,error)&&source.reads==1,error);
 check(second_metadata==-1&&second->class_id()==-1&&error.find("positive save profile")!=std::string::npos,"Missing profile did not preserve source prefix");
 save->set_character(0x100000003ULL);check(!authority.save(error)&&!authority.load(1,error)&&!authority.safe_properties_id(id,error),"Changed actor identity was silently adopted");
 std::cout<<"PASS "<<checks<<" checks; actual KnightPlayerBase id="<<knight<<"; actual AI IsPlayer; same Save/profile/cache; zero-call NULL file branches; required positive profile failure\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
