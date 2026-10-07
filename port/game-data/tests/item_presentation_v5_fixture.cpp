#include "../item_presentation_v5.hpp"
#include <cstring>
#include <algorithm>
using namespace dh2::data;
namespace {
struct Context{Item row;std::string name,material;std::vector<std::string> keys;std::int32_t class_oids[9]{};};
const Item* meta(void* p,const ItemInstanceV1&,std::string&){return &static_cast<Context*>(p)->row;}
bool invoke(void* p,ItemInstanceV1&,const ItemTextRequestV5& q,ItemTextResponseV5& r,std::string& out,std::string& e){auto& c=*static_cast<Context*>(p);if(q.operation==ItemTextOperationV5::constant){auto it=std::find(c.keys.begin(),c.keys.end(),q.key);if(it==c.keys.end()){c.keys.emplace_back(q.key);it=c.keys.end()-1;}r.value=100000+std::int32_t(it-c.keys.begin());return true;}
 if(q.operation==ItemTextOperationV5::class_name){constexpr int rows[]={263,264,265,325,327,326,290,292,291};auto it=std::find(std::begin(rows),std::end(rows),q.value);if(it==std::end(rows)){e="bad class row";return false;}r.value=c.class_oids[it-std::begin(rows)];return true;}
 if(q.operation==ItemTextOperationV5::integer_string){if(q.value==c.row.record.words[17])r.text=c.name;else if(q.value==c.row.record.words[18])r.text=c.material;else if(q.value>=100000&&std::size_t(q.value-100000)<c.keys.size()){auto& key=c.keys[std::size_t(q.value-100000)];r.text=key=="GLOBAL_LIST_SEPERATOR"?", ":key=="INGAME_REQUIREMENTS"?"Requires:":key=="INGAME_REQUIRES_CLASS"?"^s":key+"(^d)";}else r.text="OID"+std::to_string(q.value);return true;}
 if(q.operation==ItemTextOperationV5::parse_varargs){bool escape=false;unsigned arg=0;for(const char* t=q.input;*t;++t){if(!escape){if(*t=='^')escape=true;else out+=*t;}else{escape=false;if(*t=='d'){if(arg>=q.count){e="fixture missing argument";return false;}out+=std::to_string(q.arguments[arg++].integer);}else if(*t=='s'){if(arg>=q.count){e="fixture missing string argument";return false;}auto* text=q.arguments[arg++].text;if(text)out+=text;}else if(*t=='n')out+='\n';else if(*t=='#'||*t=='*'||*t=='^')out+=*t;}}return true;}
 e="Unmodeled ItemText fixture service";return false;
}
std::uint32_t read(const std::uint8_t*& p){std::uint32_t n;std::memcpy(&n,p,4);p+=4;return n;}
std::string text(const std::uint8_t*& p){auto n=read(p);std::string s(reinterpret_cast<const char*>(p),n);p+=n;return s;}
}
extern "C" std::uint32_t dh2_item_presentation_fixture_v5(const std::uint8_t* input,std::uint8_t* output){const auto* p=input;auto op=read(p);ItemInstanceV1 item;item.value=std::int32_t(read(p));Context ctx;std::memcpy(&ctx.row.record,p,164);p+=164;std::memcpy(ctx.class_oids,p,36);p+=36;ctx.name=text(p);ctx.material=text(p);ItemTextServicesV5 svc{&ctx,meta,invoke};std::string e;bool okay=op==0?item_update_name_v5(item,svc,e):op==1?item_update_stats_v5(item,svc,e):item_update_requirements_v5(item,svc,e);if(!okay)return UINT32_MAX;const auto& result=op==0?item.name:op==1?item.description:item.requirements;auto n=std::uint32_t(result.size());std::memcpy(output,result.data(),n);return n;}

