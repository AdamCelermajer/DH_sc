#include "trophy_manager_owner_v1.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace dh2::trophies {namespace {
struct Reader {data::Bytes b;std::size_t at{};bool good=true;
 std::uint32_t word(){if(!good||at>b.size||b.size-at<4){good=false;return 0;}auto*p=b.data+at;at+=4;return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::vector<std::string> strings(){auto n=word();if(n>65536){good=false;return {};}std::vector<std::string> out;for(unsigned i=0;i<n&&good;++i){auto z=word();if(z>1048576||at>b.size||z>b.size-at){good=false;break;}out.emplace_back(reinterpret_cast<const char*>(b.data+at),z);at+=z;}return out;}
};
std::int32_t signed_word(std::uint32_t v){std::int32_t out;std::memcpy(&out,&v,4);return out;}
}
std::shared_ptr<const TrophyCatalogV1> TrophyCatalogV1::load(data::Bytes rows,data::Bytes names,data::Bytes schema,std::string& error){
 error.clear();try{if(!rows.data||!names.data||!schema.data)throw std::runtime_error("Required authored trophy tables");
 auto next=std::make_shared<TrophyCatalogV1>();Reader r{rows},n{names},s{schema};auto count=r.word();if(count>65536)throw std::runtime_error("Trophy table count outside valid source span");
 next->names_=n.strings();auto fields=s.strings();if(!n.good||!s.good||next->names_.size()!=count||fields!=std::vector<std::string>{"Desc","GLIndex","GLLive","Grade","Label","Name","Type"})throw std::runtime_error("Authored TrophyTable names/schema mismatch");
 next->rows_.reserve(count);for(unsigned i=0;i<count;++i){TrophyRowV1 row{};std::int32_t* fields[]={&row.desc,&row.gl_index,&row.gl_live,&row.grade,&row.label,&row.name,&row.type};for(auto p:fields)*p=signed_word(r.word());next->rows_.push_back(row);}if(!r.good)throw std::runtime_error("Truncated source TrophyTable");return next;
 }catch(const std::exception& e){error=e.what();return {};}}
std::int32_t TrophyCatalogV1::index(const char* name)const noexcept{if(!name)return -1;for(std::size_t i=0;i<names_.size();++i)if(!std::strcmp(names_[i].c_str(),name))return int(i);return -1;}
TrophyManagerOwnerV1::TrophyManagerOwnerV1(std::shared_ptr<const TrophyCatalogV1> c,character::DebugSwitches*d,const character::DebugFileServices24*f,TrophyServicesV1 s):catalog_(std::move(c)),debug_(d),files_(f),services_(s){}
int TrophyManagerOwnerV1::tracing(){std::uint32_t ignored;if(!debug_||!files_||dh2_character_debug_load(debug_,files_)!=1||dh2_character_debug_get(&ignored,debug_,"isTracingTrophies",files_)!=1){error_="Required original trophy Debug delivery";return -2;}return 0;}
int TrophyManagerOwnerV1::ask(const TrophyRequestV1&q,TrophyResponseV1&r){r={};if(!services_.invoke||services_.invoke(services_.context,&q,&r)){error_="Required trophy service "+std::to_string(q.service);return -2;}return 0;}
std::unique_ptr<TrophyManagerOwnerV1> TrophyManagerOwnerV1::create(std::shared_ptr<const TrophyCatalogV1> c,character::DebugSwitches*d,const character::DebugFileServices24*f,TrophyServicesV1 s,std::string&error){
 error.clear();if(!c){error="Required original TrophyTable owner";return {};}
 auto out=std::unique_ptr<TrophyManagerOwnerV1>(new TrophyManagerOwnerV1(std::move(c),d,f,s));out->data_.reserve(out->catalog_->rows().size());
 for(const auto&r:out->catalog_->rows()){TrophyDataV1 row{int(out->data_.size()),r.name,r.desc,0,r.type,r.grade,r.label,r.gl_live,r.gl_index};if(out->tracing()){error=out->error_;return {};}out->data_.push_back(row);}out->initialized_=true;return out;
}
const TrophyDataV1* TrophyManagerOwnerV1::get(std::int32_t id)const noexcept{for(const auto&r:data_)if(r.id==id)return &r;return nullptr;}
bool TrophyManagerOwnerV1::is_unlocked(std::int32_t id)const noexcept{auto*r=get(id);return r&&r->unlocked;}
bool TrophyManagerOwnerV1::is_unlocking(std::int32_t id)const noexcept{return std::find(unlocking_.begin(),unlocking_.end(),id)!=unlocking_.end();}
int TrophyManagerOwnerV1::unlock(std::int32_t id){error_.clear();if(id<0||is_unlocked(id)||is_unlocking(id))return 0;if(!get(id)){error_="Source missing trophy data/assertion continuation";return -2;}unlocking_.push_back(id);return unlocked_callback(id);}
int TrophyManagerOwnerV1::unlocked_callback(std::int32_t id){
 // Source callback removes from a COPY of the unlocking vector. The original
 // retained vector is deliberately unchanged, including required-failure prefix.
 if(!is_unlocking(id))return 0;TrophyDataV1* row=nullptr;for(auto&candidate:data_)if(candidate.id==id){row=&candidate;break;}if(!row)return 0;
 for(unsigned i=0;i<3;++i)if(tracing())return -2;row->unlocked=1;
 AchievementMessageV1 message{"","",row->type,row->grade,row->label};TrophyResponseV1 response;
 if(row->name!=-1){if(ask({resolve_text_v1,row->name,nullptr,nullptr},response))return -2;message.name=response.text;}
 if(row->desc!=-1){if(ask({resolve_text_v1,row->desc,nullptr,nullptr},response))return -2;message.desc=response.text;}
 if(ask({application_in_game_v1,0,nullptr,nullptr},response))return -2;
 if(response.boolean&&ask({queue_message_v1,0,&message,nullptr},response))return -2;
 return save();
}
int TrophyManagerOwnerV1::bitmap(std::array<std::uint32_t,4>&out)const noexcept{out={};for(std::size_t i=0;i<data_.size();++i)if(data_[i].unlocked){if(i>127)return -2;out[i>>5]|=1u<<(i&31);}return 0;}
int TrophyManagerOwnerV1::save(){std::array<std::uint32_t,4> bits;if(bitmap(bits)){error_="Source trophy bitmap assertion outside128 rows";return -2;}TrophyResponseV1 response;return ask({save_bitmap_v1,0,nullptr,&bits},response);}
int TrophyManagerOwnerV1::load_bitmap(const std::array<std::uint32_t,4>&bits){if(data_.size()>128){error_="Source trophy bitmap assertion outside128 rows";return -2;}const auto captured=bits;for(std::size_t i=0;i<data_.size();++i)if(captured[i>>5]&(1u<<(i&31))){auto status=unlock(int(i));if(status)return status;}return 0;}
TrophyNativeBindingsV1::TrophyNativeBindingsV1(TrophyManagerOwnerV1&owner):owner_(owner){for(const auto&name:owner.catalog().names())names_.push_back(name.c_str());}
int TrophyNativeBindingsV1::skill_ai(const character::skills::SkillAIRequest32V3*q,character::skills::SkillAIResponse32V3*r){
 using namespace character::skills;if(!q||!r||!owner_.initialized())return -1;
 if(q->operation==skill_ai_trophy_manager_v3){r->identity=reinterpret_cast<std::uintptr_t>(&owner_);return 0;}
 if(q->operation==skill_ai_trophy_catalog_v3){r->names=names_.data();r->count=names_.size();return 0;}
 if(q->operation==skill_ai_unlock_v3){if(q->subject!=reinterpret_cast<std::uintptr_t>(&owner_))return -1;return owner_.unlock(signed_word(q->index));}return -1;
}
int TrophyNativeBindingsV1::hit(const character::HitRequest32*q,std::uintptr_t*out){
 using namespace character::skills;if(!q||!out||!owner_.initialized())return -1;
 if(q->service==hit_trophy_manager_v6){*out=reinterpret_cast<std::uintptr_t>(&owner_);return 0;}
 if(q->service==hit_trophy_index_v6){*out=std::uint32_t(owner_.catalog().index(q->name));return 0;}
 if(q->service==hit_unlock_v6){if(q->subject!=reinterpret_cast<std::uintptr_t>(&owner_))return -1;return owner_.unlock(signed_word(q->force));}return -1;
}
int TrophyNativeBindingsV1::unlock_named(const char*name){return owner_.unlock(owner_.catalog().index(name));}
}
