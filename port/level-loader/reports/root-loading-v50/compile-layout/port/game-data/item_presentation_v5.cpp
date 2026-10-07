#include "item_presentation_v5.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::data {namespace {
bool fail(std::string& e,const char* p){e=p;return false;}
const Item* metadata(const ItemTextServicesV5& s,const ItemInstanceV1& i,std::string& e){if(!s.metadata){e="Item metadata provider missing";return nullptr;}auto* r=s.metadata(s.context,i,e);if(!r&&e.empty())e="Item metadata unavailable";return r;}
bool call(const ItemTextServicesV5& s,ItemInstanceV1& i,const ItemTextRequestV5& q,ItemTextResponseV5& r,std::string& out,std::string& e){if(!s.invoke)return fail(e,"Required Item localization/formatter service missing");if(!s.invoke(s.context,i,q,r,out,e)){if(e.empty())e="Required Item text service failed";return false;}if(r.text.find('\0')!=std::string::npos||r.text.size()>1024*1024||out.size()>1024*1024)return fail(e,"Item text provider exceeded native bounds");return true;}
bool string(const ItemTextServicesV5& s,ItemInstanceV1& i,std::int32_t id,std::string& out,std::string& e){ItemTextResponseV5 r;std::string unused;if(!call(s,i,{ItemTextOperationV5::integer_string,id},r,unused,e))return false;out=std::move(r.text);return true;}
bool constant(const ItemTextServicesV5& s,ItemInstanceV1& i,const char* key,std::string& out,std::string& e){ItemTextResponseV5 r;std::string unused;if(!call(s,i,{ItemTextOperationV5::constant,0,"StrID",key},r,unused,e))return false;return string(s,i,r.value,out,e);}
bool parse(const ItemTextServicesV5& s,ItemInstanceV1& i,const std::string& text,const ItemTextArgumentV5* args,std::uint32_t n,std::string& out,std::string& e,ItemTextOperationV5 op=ItemTextOperationV5::parse_varargs){ItemTextResponseV5 r;return call(s,i,{op,0,nullptr,nullptr,text.c_str(),args,n},r,out,e);}
std::int32_t asr8(std::int32_t x){std::uint32_t u;std::memcpy(&u,&x,4);u=(u>>8)|(x<0?0xff000000u:0);std::memcpy(&x,&u,4);return x;}
bool stats_part(ItemInstanceV1& i,const ItemTextServicesV5& s,const char* key,unsigned word,unsigned n,std::string& e){std::string text;if(!constant(s,i,key,text,e))return false;ItemTextArgumentV5 a[2];for(unsigned j=0;j<n;++j){auto* m=metadata(s,i,e);if(!m)return false;a[j].integer=asr8(m->record.words[word+j]);}return parse(s,i,text,a,n,i.description,e);}
}
bool item_update_name_v5(ItemInstanceV1& i,const ItemTextServicesV5& s,std::string& e){
 e.clear();auto* m=metadata(s,i,e);if(!m)return false;const auto nameid=m->record.words[17],materialid=m->record.words[18];i.name.clear();if(nameid==-1)return true;
 std::string name;if(!string(s,i,nameid,name,e))return false;std::size_t token=name.find("[f]");unsigned choice=1;if(token==std::string::npos){token=name.find("[fs]");choice=3;if(token==std::string::npos){token=name.find("[s]");choice=2;if(token==std::string::npos)choice=0;}}
 const bool has_material=materialid!=-1;std::string material,selected;bool suffix=false;
 if(has_material){if(!string(s,i,materialid,material,e))return false;auto join=material.find("[a]");suffix=join!=std::string::npos&&join!=0;auto limit=suffix?join:material.size();std::int64_t indices[5]={-1,0,0,0,static_cast<std::int64_t>(limit)};
  for(unsigned j=1;j<4;++j){auto previous=indices[j-1];if(previous!=0&&std::uint64_t(previous+1)<material.size()){auto p=material.find('#',std::size_t(previous+1));indices[j]=p==std::string::npos?0:static_cast<std::int64_t>(p);}}
  if(indices[choice]!=0&&indices[choice+1]!=0){auto begin=indices[choice]+1;auto count=indices[choice+1]-indices[choice]-1;if(begin<0||count<0||std::uint64_t(begin)>material.size()||std::uint64_t(count)>material.size()-std::uint64_t(begin))return fail(e,"Source material substring outside valid domain");selected=material.substr(std::size_t(begin),std::size_t(count));}
  else selected=material.substr(0,limit);
 }
 std::string formatted;ItemTextArgumentV5 a;a.integer=i.value;if(!parse(s,i,name,&a,1,formatted,e))return false;std::string prefix=formatted.substr(0,token==std::string::npos?formatted.size():token);
 if(has_material&&!suffix)i.name=selected+" ";i.name+=prefix;if(has_material&&suffix)i.name+=" "+selected;return true;
}
bool item_update_stats_v5(ItemInstanceV1& i,const ItemTextServicesV5& s,std::string& e){e.clear();i.description.clear();auto* m=metadata(s,i,e);if(!m)return false;auto t=std::uint32_t(m->record.words[22]);if(t>12)return true;if((1u<<t)&0x1780)return stats_part(i,s,"GAMEPLAYMENUS_ARMOR_RATING",35,1,e);if(t==6){if(!stats_part(i,s,"GAMEPLAYMENUS_ARMOR_RATING",35,1,e))return false;i.description+=' ';return stats_part(i,s,"GAMEPLAYMENUS_BLOCK_RATING",36,1,e);}if(t<=5)return stats_part(i,s,"GAMEPLAYMENUS_DAMAGE_DESC",35,2,e);return true;}
bool item_update_requirements_v5(ItemInstanceV1& i,const ItemTextServicesV5& s,std::string& e){
 e.clear();i.requirements.clear();bool any=false;for(unsigned j=29;j<=34;++j){auto* m=metadata(s,i,e);if(!m)return false;if(m->record.words[j]){any=true;break;}}if(!any)return true;auto* m=metadata(s,i,e);if(!m)return false;if(m->record.words[22]==14)return true;
 std::string prefix,delimiter;if(!constant(s,i,"INGAME_REQUIREMENTS",prefix,e))return false;i.requirements=prefix;if(!constant(s,i,"GLOBAL_LIST_SEPERATOR",delimiter,e))return false;bool added=false;
 const char* keys[]={"INGAME_REQUIRES_LEVEL","INGAME_REQUIRES_STRENGTH","INGAME_REQUIRES_DEXTERITY","INGAME_REQUIRES_ENDURANCE","INGAME_REQUIRES_ENERGY","INGAME_REQUIRES_CLASS"};
 for(unsigned j=0;j<6;++j){m=metadata(s,i,e);if(!m)return false;if(!m->record.words[29+j])continue;if(added)i.requirements+=delimiter;std::string text;if(!constant(s,i,keys[j],text,e))return false;ItemTextArgumentV5 a;std::string classname;m=metadata(s,i,e);if(!m)return false;
  if(j<5)a.integer=m->record.words[29+j];else{auto cls=(std::uint32_t(m->record.words[34])-1u);if(cls>=9)return fail(e,"Source invalid ClassReq assertion domain unsupported");constexpr int rows[]={263,264,265,325,327,326,290,292,291};ItemTextResponseV5 r;std::string unused;if(!call(s,i,{ItemTextOperationV5::class_name,rows[cls]},r,unused,e)||!string(s,i,r.value,classname,e))return false;a.text=classname.c_str();}
  if(!parse(s,i,text,&a,1,i.requirements,e))return false;added=true;
 }return true;
}
const std::vector<ItemPowerInstanceV5>* ItemPresentationOwnerV5::powers(const ItemInstanceV1& i)const noexcept{auto it=powers_.find(const_cast<ItemInstanceV1*>(&i));return it==powers_.end()?nullptr:&it->second;}
bool ItemPresentationOwnerV5::forget(ItemInstanceV1& i,std::string& e)noexcept{if(running_){e="Destructive ItemPower reentry unsupported";return false;}powers_.erase(&i);return true;}
bool ItemPresentationOwnerV5::add_power(ItemInstanceV1& i,std::int32_t id,std::int32_t mode,const ItemTextServicesV5& s,std::string& e){
 e.clear();if(running_)return fail(e,"Reentrant ItemPower mutation unsupported");if(!tables_||id<0||std::size_t(id)>=tables_.rows().size())return fail(e,"Source invalid ItemPower assertion domain unsupported");auto& state=powers_[&i];if(state.size()!=i.powers.size())return fail(e,"ItemPower owner not synchronized to actual item IDs");for(std::size_t j=0;j<state.size();++j)if(state[j].id!=i.powers[j])return fail(e,"ItemPower item IDs were changed outside owner");
 struct Guard{bool& b;Guard(bool& x):b(x){b=true;}~Guard(){b=false;}}guard(running_);
 if(mode==1||mode==2){auto next=std::uint64_t(std::uint32_t(id))+(mode==1?1:2);if(next>=tables_.names().size())return fail(e,"Source ItemPower suffix lookahead outside table");const char* suffix=mode==1?"_Hard":"_VeryHard";if(tables_.names()[std::size_t(next)]==tables_.names()[std::size_t(id)]+suffix)id=static_cast<std::int32_t>(next);}
 const auto& row=tables_.rows()[std::size_t(id)];state.push_back({id,row.scalars.sorting_order,{}});i.powers.push_back(id);std::string text;if(!string(s,i,row.scalars.description,text,e))return false;
 if(row.properties.empty())state.back().description=text;else{std::vector<ItemTextArgumentV5> args;args.reserve(row.properties.size());for(const auto& p:row.properties)args.push_back({static_cast<float>(p.value)*0.00390625f,asr8(p.value),nullptr});if(!parse(s,i,text,args.data(),std::uint32_t(args.size()),state.back().description,e,ItemTextOperationV5::parse_ex))return false;}
 for(std::size_t j=state.size()-1;j>0&&state[j-1].sorting_order>state[j].sorting_order;--j){std::swap(state[j-1].id,state[j].id);std::swap(state[j-1].description,state[j].description);std::swap(i.powers[j-1],i.powers[j]);}return true;
}
}




