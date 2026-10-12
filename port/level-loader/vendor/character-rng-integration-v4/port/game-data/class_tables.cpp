#include "class_tables.hpp"
#include "properties.hpp"
#include <algorithm>
#include <cstring>
#include <set>
#include <stdexcept>
namespace {
std::int32_t signed_bits(std::uint32_t x){std::int32_t result;std::memcpy(&result,&x,4);return result;}
std::int32_t add(std::int32_t a,std::int32_t b){return signed_bits(std::uint32_t(a)+std::uint32_t(b));}
std::int32_t mul(std::int32_t a,std::int32_t b){return signed_bits(std::uint32_t(a)*std::uint32_t(b));}
std::int32_t asr8(std::int32_t a){auto bits=std::uint32_t(a);return signed_bits((bits>>8)|((bits&0x80000000u)?0xff000000u:0));}
bool prop(std::int32_t p){return p>=0&&p<224;}
}
extern "C" unsigned dh2_class_formula(std::int32_t d,std::int32_t type,std::int32_t a,std::int32_t b,std::int32_t c,std::int32_t* sheet,const std::int32_t* buff){
 if(type==0)return 1;
 if(type==8)return 2;
 if(type<0||type>9||type==3)return 3;
 if(!sheet||!prop(d))return 4;
 switch(type){
 case 1:if(!prop(b))return 4;sheet[d]=add(a==-666?sheet[d]:a,mul(c,asr8(buff?buff[b]:sheet[b])));break;
 case 2:sheet[d]=sheet[d]<a?a:(sheet[d]<b?sheet[d]:b);break;
 case 4:if(!prop(a))return 4;sheet[d]=add(sheet[d],sheet[a]);break;
 case 5:if(!prop(a))return 4;sheet[d]=asr8(mul(sheet[d],sheet[a]));break;
 case 6:if(!prop(a))return 4;sheet[d]=add(asr8(mul(buff?buff[d]:sheet[d],sheet[a])),a);break;
 case 7:sheet[d]=add(sheet[d],a);break;
 case 9:sheet[d]=a;break;
 }return 0;
}
namespace {
unsigned apply_rows(const dh2::data::ClassRow* table,std::uint32_t count,std::int32_t id,std::int32_t* sheet,const std::int32_t* buff,std::int32_t* stack,unsigned depth,unsigned& budget,dh2::data::PropertyView* owner=nullptr){
 if(id<0||std::uint32_t(id)>=count)return 0;
 if(depth>=32)return 5;
 for(unsigned i=0;i<depth;++i)if(stack[i]==id)return 5;
 stack[depth]=id;const auto& row=table[id];if(row.count>10000||(!row.data&&row.count))return 4;
 for(unsigned i=0;i<row.count;++i){
  if(++budget>100000)return 6;
  const auto& f=row.data[i];unsigned action=0;
  if(f.type==1&&owner){
   if(!prop(f.destination)||!prop(f.p2))return 4;
   const auto base=f.p1==-666?sheet[f.destination]:f.p1;std::int32_t source=0;
   if(dh2_property_resolve(owner,f.p2,&source))return 4;
   sheet[f.destination]=add(base,mul(f.p3,asr8(source)));
  }else action=dh2_class_formula(f.destination,f.type,f.p1,f.p2,f.p3,sheet,buff);
  if(action==4)return 4;
  if(action==1)for(auto child:{f.p1,f.p2,f.p3}){auto error=apply_rows(table,count,child,sheet,buff,stack,depth+1,budget,owner);if(error)return error;}
  if(action==2)break;
 }return 0;
}
}
extern "C" unsigned dh2_class_apply(const dh2::data::ClassRow* table,std::uint32_t count,std::int32_t id,std::int32_t* sheet,const std::int32_t* buff){
 if(!table||!sheet||count>10000)return 4;
 std::int32_t stack[32];unsigned budget=0;return apply_rows(table,count,id,sheet,buff,stack,0,budget);
}
extern "C" unsigned dh2_class_apply_to_base(const dh2::data::ClassRow* table,std::uint32_t count,std::int32_t id,std::int32_t* base,dh2::data::PropertyView* owner){
 if(!table||!base||!owner||owner->base!=base||count>10000||dh2_property_validate(owner))return 4;
 std::int32_t stack[32];unsigned budget=0;return apply_rows(table,count,id,base,nullptr,stack,0,budget,owner);
}
extern "C" unsigned dh2_class_recalc_base(const dh2::data::ClassRow* table,std::uint32_t count,std::int32_t* base,dh2::data::PropertyView* owner){
 if(!base)return 4;
 auto status=dh2_class_apply_to_base(table,count,base[26],base,owner);if(status)return status;
 for(unsigned p=0;p<224;++p){std::int32_t result;if(dh2_property_resolve(owner,p,&result))return 4;}return 0;
}
namespace dh2::data {
namespace {
const std::vector<std::vector<std::string>> expected_schema={
 {"DestProp","Formula","Param1","Param2","Param3"},{"list_entries"},
 {"DestProp","Formula","Base","ScaleProp","PerProp"},{"DestProp","Formula","Value","UNUSED0","UNUSED1"},
 {"DestProp","Formula","Value","NotUsed1","NotUsed2"},{"DestProp","Formula","Value","NotUsed1","NotUsed2"},
 {"DestProp","Formula","Stat1","Stat2","Stat3"},{"DestProp","Formula","Min","Max","NotUsed"},
 {"DestProp","Formula","Base","ScaleProp","PerProp"},{"DestProp","Formula","PropToAdd","NotUsed","NotUsed2"},
 {"DestProp","Formula","ScaleProp","NotUsed","NotUsed2"},{"DestProp","Formula","ScaleProp","NotUsed","NotUsed2"},
 {"DestProp","Formula","IsSpecialAttack","IsSpecialAttackState","NotUsed1"}};
struct Reader{
 Bytes bytes;std::size_t offset=0,budget=0;
 explicit Reader(Bytes b):bytes(b){if(!b.data||b.size<4||b.size>8*1024*1024)throw std::runtime_error("Class table input outside limit");}
 void require(std::size_t n){if(offset>bytes.size||n>bytes.size-offset)throw std::runtime_error("Truncated class table");}
 std::uint32_t word(){require(4);auto p=bytes.data+offset;offset+=4;return p[0]|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
 std::int32_t integer(){return signed_bits(word());}
 unsigned count(){auto n=word();if(n>10000||budget+n>100000)throw std::runtime_error("Class count outside limit");budget+=n;return n;}
 std::vector<std::string> strings(){auto n=count();std::vector<std::string> result;std::set<std::string> seen;
  for(unsigned i=0;i<n;++i){auto size=word();if(!size||size>4096)throw std::runtime_error("Class identifier length outside limit");require(size);std::string s(reinterpret_cast<const char*>(bytes.data+offset),size);offset+=size;
   if(std::any_of(s.begin(),s.end(),[](unsigned char c){return c<32||c>126;})||!seen.insert(s).second)throw std::runtime_error("Invalid or duplicate class identifier");
   result.push_back(std::move(s));
  }return result;
 }
 void end(){if(offset!=bytes.size)throw std::runtime_error("Unexpected class table suffix");}
};
void validate(const ClassFormula& f,std::size_t count){
 if(f.type==0){for(auto ref:{f.p1,f.p2,f.p3})if(ref< -1||(ref>=0&&std::size_t(ref)>=count))throw std::runtime_error("Class group reference outside table");return;}
 if(f.type<0||f.type>9||f.type==3||f.type==8)return;
 if(!prop(f.destination))throw std::runtime_error("Class destination outside property sheet");
 if((f.type==1&&!prop(f.p2))||((f.type==4||f.type==5||f.type==6)&&!prop(f.p1)))throw std::runtime_error("Class source outside property sheet");
}
}
bool load_classes(Bytes records,Bytes names,Bytes schema,ClassTables& out,std::string& error){
 out={};error.clear();try{
  Reader data(records),keys(names),layout(schema);ClassTables next;next.names=keys.strings();keys.end();
  for(const auto& expected:expected_schema)if(layout.strings()!=expected)throw std::runtime_error("Class schema differs");
  layout.end();
  auto count=data.count();if(!count||count!=next.names.size())throw std::runtime_error("Class table dimensions differ");next.rows.resize(count);
  for(auto& row:next.rows){auto n=data.count();data.require(std::size_t(n)*20);row.reserve(n);for(unsigned i=0;i<n;++i){ClassFormula f;f.destination=data.integer();f.type=data.integer();f.p1=data.integer();f.p2=data.integer();f.p3=data.integer();validate(f,count);row.push_back(f);}}
  data.end();next.data_consumed=data.offset;out=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool apply_class(const ClassTables& table,std::int32_t id,PropertySheet& sheet,std::string& error,const PropertySheet* buff){
 error.clear();try{if(table.rows.empty()||table.rows.size()>10000)throw std::runtime_error("Class execution table outside limit");auto next=sheet;PropertySheet saved_buff;if(buff){saved_buff=*buff;buff=&saved_buff;}std::vector<ClassRow> views;views.reserve(table.rows.size());
  for(const auto& row:table.rows){if(row.size()>10000)throw std::runtime_error("Class execution row outside limit");for(const auto& f:row)validate(f,table.rows.size());views.push_back({row.data(),std::uint32_t(row.size())});}
  if(dh2_class_apply(views.data(),views.size(),id,next.data(),buff?buff->data():nullptr))throw std::runtime_error("Invalid, recursive or excessive class execution");
  sheet=next;return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool apply_class_uncached(const ClassTables& table,std::int32_t id,const PropertyRules& rules,PropertyState& state,std::string& error){
 error.clear();try{
  if(table.rows.empty()||table.rows.size()>10000)throw std::runtime_error("Class execution table outside limit");
  auto next=state;auto owner=property_view(rules,next);std::vector<ClassRow> views;views.reserve(table.rows.size());
  for(const auto& row:table.rows){if(row.size()>10000)throw std::runtime_error("Class execution row outside limit");for(const auto& f:row)validate(f,table.rows.size());views.push_back({row.data(),std::uint32_t(row.size())});}
  if(dh2_class_apply_to_base(views.data(),views.size(),id,next.base.data(),&owner))throw std::runtime_error("Invalid, recursive or excessive uncached class execution");
  state=next;return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
