#include "../visual_skin_selection_v6.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <map>
using namespace dh2::skinning;
static unsigned checks;static void ck(bool b){if(!b)throw std::runtime_error("V6 selection check "+std::to_string(checks));++checks;}
using Bytes=std::vector<std::uint8_t>;static void w(Bytes& b,std::uint32_t v){for(unsigned j=0;j<4;++j)b.push_back(std::uint8_t(v>>(j*8)));}static void s(Bytes& b,const char* p){const auto n=p?std::strlen(p):0;w(b,n);if(n)b.insert(b.end(),p,p+n);}
struct Reader{Bytes bytes;unsigned at{};unsigned word(){ck(at+4<=bytes.size());unsigned v=0;for(unsigned j=0;j<4;++j)v|=unsigned(bytes[at++])<<(j*8);return v;}std::string string(){auto n=word();ck(n<=bytes.size()-at);std::string s(reinterpret_cast<const char*>(bytes.data()+at),n);at+=n;return s;}Bytes block(){auto n=word();ck(n<=bytes.size()-at);Bytes b(bytes.begin()+at,bytes.begin()+at+n);at+=n;return b;}};
struct Context {Bytes trace;bool success,found,mutation;std::uintptr_t old=0x10000000065ULL,newer=0x200000000caULL,parent=0x30000000033ULL,middle=0x4000000012fULL;std::map<std::uintptr_t,unsigned> refs{};unsigned delivered{},fail_at{};
 unsigned identity(std::uintptr_t p)const{return !p?0:p==old?101:p==newer?202:p==middle?303:0xffffffff;}
};
static bool invoke(void* p,VisualSelectionV6& state,const VisualRequestV6& q,std::uintptr_t* out){auto& c=*static_cast<Context*>(p);if(c.fail_at&&++c.delivered==c.fail_at)return false;switch(q.operation){
 case VisualOperationV6::construct_module:w(c.trace,1);s(c.trace,q.text);*out=c.success?c.newer:0;if(c.mutation)state.cells[0]={17,0,c.middle};break;
 case VisualOperationV6::retain:++c.refs[q.resource];break;
 case VisualOperationV6::release:w(c.trace,2);w(c.trace,c.identity(q.resource));--c.refs[q.resource];break;
 case VisualOperationV6::update_buffers:w(c.trace,3);w(c.trace,q.mode);break;
 case VisualOperationV6::visibility:w(c.trace,4);break;
 case VisualOperationV6::construct_weapon:w(c.trace,5);s(c.trace,q.text);w(c.trace,1);*out=c.success?c.newer:0;break;
 case VisualOperationV6::search:w(c.trace,6);s(c.trace,q.text);w(c.trace,0);*out=c.found?c.parent:0;break;
 case VisualOperationV6::detach:w(c.trace,7);w(c.trace,c.identity(q.resource));break;
 case VisualOperationV6::attach:w(c.trace,8);w(c.trace,c.identity(q.resource));break;
 }return true;}
int main(int argc,char** argv){try{ck(argc==2);std::ifstream f(argv[1],std::ios::binary);ck(bool(f));Reader r{{std::istreambuf_iterator<char>(f),{}}};ck(r.word()==0x364b5653);auto count=r.word();std::vector<std::string> names(count),defaults(count);std::vector<std::vector<std::string>> uris(count);std::vector<std::vector<VisualModuleV6>> modules(count);std::vector<VisualCategoryV6> catalog(count);std::vector<VisualCellV6> cells(count);
 for(unsigned i=0;i<count;++i){names[i]=r.string();defaults[i]=r.string();auto n=r.word();uris[i].resize(n);modules[i].resize(n);for(unsigned j=0;j<n;++j){uris[i][j]=r.string();modules[i][j]={uris[i][j].c_str(),0x100000000ULL+i*1000+j};}catalog[i]={names[i].c_str(),defaults[i].c_str(),modules[i].data(),n,0};}
 auto cases=r.word();unsigned requests=0;
 for(unsigned i=0;i<cases;++i){auto kind=r.word(),old=r.word(),present=r.word(),flags=r.word(),success=r.word(),update=r.word(),found=r.word(),slot=r.word(),mutation=r.word(),has=r.word(),expected=r.word();auto argument=r.string();auto trace=r.block();Context context{{},bool(success),bool(found),bool(mutation)};context.refs={{context.old,1},{context.newer,1},{context.middle,1}};for(auto& c:cells){std::memcpy(&c.id,&old,4);c.reserved=0;c.resource=0;}cells[0].resource=present?context.old:0;VisualSelectionV6 state{catalog.data(),cells.data(),count,flags,{present?context.old:0,present?context.old:0},0};VisualServicesV6 services{&context,invoke};int result=-99;
  if(kind==0)result=dh2_visual_category_v6(&state,argument.c_str());else if(kind==1)result=dh2_visual_module_uri_v6(&state,argument.c_str());else if(kind==2)result=dh2_visual_set_category_v6(&state,0,std::stoi(argument),update,&services);else result=dh2_visual_set_weapon_v6(&state,has?argument.c_str():nullptr,slot,update,&services);ck(std::uint32_t(result)==expected);ck(context.trace==trace);requests+=trace.size();
 }
 ck(r.at==r.bytes.size());Context context{{},true,true,false};VisualSelectionV6 state{catalog.data(),cells.data(),count,0,{0,0},0};VisualServicesV6 service{&context,invoke};for(auto& c:cells)c={-1,0,0};auto saved=cells;ck(dh2_visual_set_modular_v6(&state,-1,-1,nullptr)==0&&cells[0].id==-1);ck(dh2_visual_set_modular_v6(&state,0,0,nullptr)==-1);ck(dh2_visual_set_category_v6(&state,0,0,2,&service)==-1);ck(dh2_visual_set_modular_v6(&state,4,0,&service)==-1);ck(dh2_visual_set_modular_v6(&state,0,43,&service)==-1);ck(dh2_visual_set_weapon_v6(&state,"name",1,3,&service)==-1&&context.trace.empty());cells[0].reserved=1;ck(dh2_visual_set_modular_v6(&state,0,0,&service)==-1&&context.trace.empty());cells=saved;context.fail_at=1;ck(dh2_visual_set_modular_v6(&state,0,0,&service)==-2&&cells[0].id==-1&&cells[0].resource==0);
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<cases<<",\"trace_bytes\":"<<requests<<",\"guards\":8,\"checks\":"<<checks<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
