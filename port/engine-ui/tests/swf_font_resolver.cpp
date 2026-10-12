#include "../swf_font_resolver.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh2::ui;
static void check(bool b,const char* e){if(!b)throw std::runtime_error(e);}
struct Call {std::uint32_t kind;std::uint64_t value;std::string text;bool operator==(const Call& b)const{return kind==b.kind&&value==b.value&&text==b.text;}};
struct Case {std::uint32_t language,bold,italic,exists,rewrite,found;std::string name,path;std::vector<Call> calls;};
template<class T>static T read(std::ifstream& f){T v{};check(bool(f.read(reinterpret_cast<char*>(&v),sizeof v)),"truncated corpus");return v;}
static std::string str(std::ifstream& f){auto n=read<std::uint32_t>(f);check(n<4096,"oversized corpus text");std::string s(n,0);check(bool(f.read(s.data(),n)),"truncated corpus string");return s;}
struct Context {
    const Case* row{};std::vector<Call> calls;unsigned failure{},bad_rewrite{},reentry{};bool nested{};unsigned nested_calls{};
    static int invoke(void* p,FontResolveRequest40* r){auto& c=*static_cast<Context*>(p);check(r&&r->reserved==0,"malformed request");std::string text=r->text?r->text:"";std::uint64_t value=0;
        if(r->kind==FontResolveService::language){r->value=c.row->language;value=r->value;}
        if(r->kind==FontResolveService::rewrite_path){if(c.bad_rewrite){std::memset(r->buffer,'x',r->capacity);}else{text=(c.row->rewrite?"rewritten:":"")+text;check(text.size()<r->capacity,"bad rewrite capacity");std::memcpy(r->buffer,text.c_str(),text.size()+1);}}
        if(r->kind==FontResolveService::open_read)r->value=c.row->exists?0x1234:0;
        if(r->kind==FontResolveService::close_read)value=r->value;
        c.calls.push_back({std::uint32_t(r->kind),value,text});
        if(c.reentry&&!c.nested&&r->kind==FontResolveService::debug_load){
            c.nested=true;char bytes[4096];FontResolveOutput32 out{bytes,sizeof bytes,0,0,0};FontResolveInput24 in{"_sans","unread-prefix/",1,1};FontResolveServices16 services{p,invoke};
            check(dh2_swf_font_resolve(&out,&in,&services)==0&&out.found==1&&std::string(bytes)=="#/system/fonts/DroidSans.ttf","nested resolve failed");++c.nested_calls;c.nested=false;
        }
        return c.failure==std::uint32_t(r->kind)?-7:0;
    }
};
int main(int argc,char**argv){try{
    if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);check(bool(f),"missing corpus");check(read<std::uint32_t>(f)==0x31535246,"wrong corpus magic");auto n=read<std::uint32_t>(f);check(n==1152,"wrong corpus count");unsigned checks=0,requests=0,guards=0,prefixes=0;
    Case final;
    for(unsigned i=0;i<n;++i){Case row;row.language=read<std::uint32_t>(f);row.bold=read<std::uint32_t>(f);row.italic=read<std::uint32_t>(f);row.exists=read<std::uint32_t>(f);row.rewrite=read<std::uint32_t>(f);row.name=str(f);row.path=str(f);row.found=read<std::uint32_t>(f);auto nc=read<std::uint32_t>(f);for(unsigned j=0;j<nc;++j){Call call;call.kind=read<std::uint32_t>(f);call.value=read<std::uint64_t>(f);call.text=str(f);row.calls.push_back(std::move(call));}
        char bytes[4096];std::memset(bytes,0xa5,sizeof bytes);FontResolveOutput32 out{bytes,sizeof bytes,123,123,0};FontResolveInput24 in{row.name.c_str(),"source-root/",row.bold,row.italic};Context c{&row};FontResolveServices16 s{&c,Context::invoke};
        check(dh2_swf_font_resolve(&out,&in,&s)==0&&out.found==row.found&&out.length==row.path.size()&&std::string(bytes)==row.path&&c.calls==row.calls,"source corpus mismatch");++checks;requests+=c.calls.size();final=row;
    }
    check(f.peek()==std::char_traits<char>::eof(),"trailing corpus bytes");
    Case row{0,0,0,1,0,1,"Fontin SmallCaps","source-root/data/Fontin SmallCaps.ttf",{}};Context c{&row};FontResolveServices16 s{&c,Context::invoke};FontResolveInput24 in{row.name.c_str(),"source-root/",0,0};char bytes[4096];FontResolveOutput32 out{bytes,sizeof bytes,123,123,0};
    for(unsigned fail=1;fail<=6;++fail){c.calls.clear();c.failure=fail;std::memset(bytes,0xa5,sizeof bytes);out.length=123;out.found=123;check(dh2_swf_font_resolve(&out,&in,&s)==-2&&out.length==123&&out.found==123&&std::all_of(bytes,bytes+sizeof bytes,[](unsigned char x){return x==0xa5;}),"failure prefix mutated output");check(c.calls.back().kind==fail,"wrong failure prefix");++prefixes;}
    c.failure=0;c.calls.clear();c.bad_rewrite=1;check(dh2_swf_font_resolve(&out,&in,&s)==-2&&c.calls.back().kind==4,"unterminated rewrite accepted");++guards;c.bad_rewrite=0;
    for(unsigned which=0;which<7;++which){auto bad=in;auto dest=out;auto service=s;char nonterminated[4096];std::memset(nonterminated,'a',sizeof nonterminated);if(which==0)bad.name=nullptr;if(which==1)bad.base_path=nullptr;if(which==2)bad.bold=2;if(which==3)bad.italic=2;if(which==4)dest.reserved=1;if(which==5)service.invoke=nullptr;if(which==6)bad.name=nonterminated;c.calls.clear();check(dh2_swf_font_resolve(&dest,&bad,&service)==-1&&c.calls.empty(),"malformed input reached service");++guards;}
    c.calls.clear();out.capacity=1;check(dh2_swf_font_resolve(&out,&in,&s)==-1&&bytes[0]==char(0xa5)&&out.length==123,"short output was partially written");++guards;out.capacity=sizeof bytes;
    c.calls.clear();c.reentry=1;check(dh2_swf_font_resolve(&out,&in,&s)==0&&std::string(bytes)==row.path&&c.nested_calls==1&&c.calls.size()==8,"synchronous reentry failed");++checks;
    std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<n<<",\"checks\":"<<checks<<",\"ordered_services\":"<<requests<<",\"failure_prefixes\":"<<prefixes<<",\"atomic_guards\":"<<guards<<",\"nested_calls\":"<<c.nested_calls<<"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
