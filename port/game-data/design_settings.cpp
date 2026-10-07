#include "design_settings.hpp"
#include <cstring>
#include <stdexcept>
namespace {
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return an&&bn&&(x<=y?y-x<an:x-y<bn);}
std::uint32_t readword(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
struct Reader {
 dh2::data::Bytes bytes;std::size_t at=0;
 explicit Reader(dh2::data::Bytes b):bytes(b){if(!b.data||b.size>8u*1024u*1024u)throw std::runtime_error("DesignSettings input outside bounds");}
 void require(std::size_t n){if(n>bytes.size-at)throw std::runtime_error("Truncated DesignSettings stream");}
 std::uint32_t word(){require(4);auto w=readword(bytes.data+at);at+=4;return w;}
 std::vector<std::string> names(){auto n=word();if(n>65536)throw std::runtime_error("DesignSettings names outside bounds");std::vector<std::string> result;result.reserve(n);for(unsigned i=0;i<n;++i){auto size=word();if(size>1048576)throw std::runtime_error("DesignSettings name outside bounds");require(size);result.emplace_back(reinterpret_cast<const char*>(bytes.data+at),size);at+=size;}return result;}
};
const char* const fields[]={"AdditionalLeaderIconBonus","AggroTargetSwitchPct","AutoZoomRefPoint","AutoZoomStep","CombatMusicTrigger","CoopCamLimits_Bottom","CoopCamLimits_Sides","CoopCamLimits_Top","DefaultLevelWhenInvalid","DefaultOnlineMultiLevelHack","EnemyClickSensitivity","EnemySpottedAggro","ExtraMonsterDamageMultiplyerPerPlayer","ExtraMonsterHealthMultiplyerPerPlayer","LeaderIconBonus2Players","LeaderIconBonus3Players","LeaderIconBonus4Players","MiniMapZoomMaxLimit","MiniMapZoomMinLimit","NPCClickSensitivity","ObjectClickSensitivity","PlayerRotationSpeed","PlayerRunToWalkPercent","PlayerWalkToRunPercent","QuestMarker","QuestMarkerCompleted","QuestMarkerSecondary","QuestMarkerSecondaryCompleted","StateFeared","StatePoisonned","StateSlowed","StateStunned","TargetAttack","TargetMove","XP_LevelBonusScaleFactorPerLevelPct","XP_LevelMalusScaleFactorPerLevelPct","XP_LevelScaleFactorMaxPct","XP_LevelScaleFactorMinPct","XP_LevelScaleMaxLevelDiff","XP_MaxDistanceToReceiveXP","XP_PerPlayerReductionPct","ZoomMaxLimit","ZoomMinLimit"};
std::int32_t index(const std::vector<std::string>& names,const char* key){if(!key)return -1;for(std::size_t i=0;i<names.size();++i)if(!std::strcmp(names[i].c_str(),key))return static_cast<std::int32_t>(i);return -1;}
}
extern "C" unsigned dh2_design_settings_decode_record(dh2::data::DesignSettingsProjection176* out,std::uint32_t* used,const std::uint8_t* input,std::uint32_t size){
 if(!aligned(out)||!aligned(used)||!input||size<172||size>8u*1024u*1024u||overlap(out,sizeof(*out),used,4)||overlap(out,sizeof(*out),input,size)||overlap(used,4,input,size))return 1;
 dh2::data::DesignSettingsProjection176 next{};for(unsigned i=0;i<43;++i)next.words[i+1]=readword(input+i*4);*out=next;*used=172;return 0;
}
extern "C" unsigned dh2_design_settings_decode_table(dh2::data::DesignSettingsProjection176* out,std::uint32_t capacity,std::uint32_t* count,std::uint32_t* used,const std::uint8_t* input,std::uint32_t size){
 if(!aligned(count)||!aligned(used)||!input||size<4||size>8u*1024u*1024u||capacity>65536||(capacity&&!aligned(out)))return 1;
 const auto rowcount=readword(input);if(rowcount>65536||std::uint64_t(rowcount)*172+4>size)return 1;if(rowcount>capacity)return 2;
 const auto bytes=std::size_t(capacity)*sizeof(*out);
 if(overlap(count,4,used,4)||overlap(count,4,input,size)||overlap(used,4,input,size)||overlap(out,bytes,count,4)||overlap(out,bytes,used,4)||overlap(out,bytes,input,size))return 1;
 for(unsigned row=0;row<rowcount;++row){out[row].words[0]=0;for(unsigned i=0;i<43;++i)out[row].words[i+1]=readword(input+4+row*172+i*4);}*count=rowcount;*used=4+rowcount*172;return 0;
}
namespace dh2::data {
DesignSettingsKind design_settings_field_kind(std::uint32_t i){if(i>=43)return design_unknown;return i==4||i==8||i==9||(i>=24&&i<=33)?design_integer:design_float;}
struct DesignSettingsOwner::Snapshot {std::vector<DesignSettingsProjection176> rows;std::vector<std::string> names,fields;std::size_t records_used,names_used,schema_used;};
bool DesignSettingsOwner::load(Bytes records,Bytes names,Bytes schema,std::string& error){
 error.clear();if(snapshot_&&snapshot_.use_count()>1){error="DesignSettings borrowed snapshot is pinned";return false;}
 try{Reader r(records),n(names),s(schema);auto next=std::make_shared<Snapshot>();next->names=n.names();next->fields=s.names();if(next->fields.size()!=43)throw std::runtime_error("DesignSettings schema dimensions differ");for(unsigned i=0;i<43;++i)if(next->fields[i]!=::fields[i])throw std::runtime_error("DesignSettings source schema differs");auto count=r.word();if(count>65536||count!=next->names.size())throw std::runtime_error("DesignSettings table dimensions differ");r.require(std::size_t(count)*172);next->rows.resize(count);for(auto& row:next->rows){std::uint32_t used=0;if(dh2_design_settings_decode_record(&row,&used,records.data+r.at,static_cast<std::uint32_t>(records.size-r.at)))throw std::runtime_error("DesignSettings row rejected");r.at+=used;}next->records_used=r.at;next->names_used=n.at;next->schema_used=s.at;snapshot_=std::move(next);return true;}catch(const std::exception& e){error=e.what();return false;}
}
const std::vector<DesignSettingsProjection176>& DesignSettingsOwner::Borrow::rows()const{if(!snapshot_)throw std::logic_error("Missing DesignSettings snapshot");return snapshot_->rows;}
const std::vector<std::string>& DesignSettingsOwner::Borrow::row_names()const{if(!snapshot_)throw std::logic_error("Missing DesignSettings snapshot");return snapshot_->names;}
const std::vector<std::string>& DesignSettingsOwner::Borrow::fields()const{if(!snapshot_)throw std::logic_error("Missing DesignSettings snapshot");return snapshot_->fields;}
std::size_t DesignSettingsOwner::Borrow::records_consumed()const{return snapshot_?snapshot_->records_used:0;}
std::size_t DesignSettingsOwner::Borrow::names_consumed()const{return snapshot_?snapshot_->names_used:0;}
std::size_t DesignSettingsOwner::Borrow::schema_consumed()const{return snapshot_?snapshot_->schema_used:0;}
std::int32_t DesignSettingsOwner::Borrow::field_index(const char* key)const{return snapshot_?index(snapshot_->fields,key):-1;}
std::int32_t DesignSettingsOwner::Borrow::row_index(const char* key)const{return snapshot_?index(snapshot_->names,key):-1;}
const std::uint32_t* DesignSettingsOwner::Borrow::word(std::uint32_t row,std::uint32_t field)const{return snapshot_&&row<snapshot_->rows.size()&&field<43?&snapshot_->rows[row].words[field+1]:nullptr;}
}
