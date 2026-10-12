#include "../edit_text_display_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace dh2::ui;using namespace edit_text_display_v1;
using Blob=std::vector<unsigned char>;
unsigned word(const Blob&b,std::size_t&p){if(p+4>b.size())throw std::runtime_error("truncated gold");unsigned x;std::memcpy(&x,b.data()+p,4);p+=4;return x;}
float real(unsigned x){float f;std::memcpy(&f,&x,4);return f;}
void append(Blob&b,unsigned x){auto p=b.size();b.resize(p+4);std::memcpy(b.data()+p,&x,4);}
void floating(Blob&b,float f){unsigned x;std::memcpy(&x,&f,4);append(b,x);}
Blob take(const Blob&b,std::size_t&p,std::size_t n){if(p+n>b.size())throw std::runtime_error("truncated gold");Blob r(b.begin()+p,b.begin()+p+n);p+=n;return r;}
std::shared_ptr<text_filter_v1::Effect> effect(const Blob&b,std::size_t&p){auto e=std::make_shared<text_filter_v1::Effect>();auto n=word(b,p);for(unsigned i=0;i<n;++i){auto bytes=take(b,p,44);text_filter_v1::Filter f;std::copy(bytes.begin(),bytes.end(),f.bytes.begin());f.defined.fill(1);e->filters.push_back(f);}return e;}
int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("usage: edit_text_display_v1 display-gold.bin");std::ifstream in(argv[1],std::ios::binary);Blob b((std::istreambuf_iterator<char>(in)),{});std::size_t p{};if(word(b,p)!=0x31584454)throw std::runtime_error("bad gold");auto n=word(b,p);unsigned grid{},guards{},trigcalls{};
 for(unsigned c=0;c<n;++c){unsigned cfg[16];for(auto&x:cfg)x=word(b,p);State q;q.owner=std::make_shared<Owner>();q.owner->effect=effect(b,p);auto parent=std::make_shared<Owner>();parent->effect=effect(b,p);q.owner->parent=parent;
  auto expected=take(b,p,word(b,p));std::size_t ep{};auto expected_events=take(expected,ep,word(expected,ep));auto expected_grid=word(expected,ep);auto trig=take(expected,ep,word(expected,ep));if(ep!=expected.size())throw std::runtime_error("bad result");
  auto flags=cfg[0];q.border=flags&32;q.grid_fit=flags&64;q.focus=flags&128;q.callback_present=flags&256;for(unsigned i=0;i<4;++i)q.rectangle[i]=real(cfg[i+8]);q.background=cfg[12];q.xcursor=real(cfg[13]);q.ycursor=real(cfg[14]);q.text_height=real(cfg[15]);
  Policy policy{bool(flags&1),bool(flags&2),bool(flags&4),bool(flags&16),real(cfg[1])};Blob events,seen;std::size_t tp{};Services s;
  s.policy=[&](auto& out,auto&){out=policy;return true;};s.renderer_present=[&](bool&out,auto&){out=flags&512;return true;};s.world_matrix=[&](auto&out,auto&){for(unsigned i=0;i<6;++i)out[i]=real(cfg[i+2]);return true;};
  s.renderer=[&](const Command&v,auto&){if(v.kind==Command::grid_fit){grid=v.enabled;return true;}append(events,v.kind);
   if(v.kind==Command::matrix)for(auto f:v.transform)floating(events,f);
   else if(v.kind==Command::fill_color||v.kind==Command::line_color)append(events,v.rgba);
   else if(v.kind==Command::mesh||v.kind==Command::line){append(events,v.xy.size()/2);for(float f:v.xy)floating(events,f);}
   else if(v.kind==Command::line_width)floating(events,v.width);
   else if(v.kind==Command::cache_set)append(events,v.enabled);return true;};
  s.enqueue=[&](auto&){append(events,21);return true;};s.cache_validate=[&](bool&out,auto&){append(events,20);out=flags&8;return true;};s.display_callback=[&](auto&){append(events,22);return true;};
  s.records=[&](const auto&ctx,bool supplied,auto&){append(events,23);append(events,supplied);for(float f:ctx.matrix)floating(events,f);append(events,bool(ctx.override_rgba));append(events,ctx.override_rgba.value_or(0));append(events,ctx.argument0);append(events,ctx.argument1);append(events,ctx.argument2);return true;};
  auto import=[&](unsigned kind,float f,float&out){auto k=word(trig,tp),arg=word(trig,tp),result=word(trig,tp);unsigned input;std::memcpy(&input,&f,4);if(k!=kind||arg!=input)throw std::runtime_error("trig request mismatch");append(seen,k);append(seen,arg);append(seen,result);out=real(result);++trigcalls;return true;};
  s.cosine=[&](float f,float&out,auto&){return import(0,f,out);};s.sine=[&](float f,float&out,auto&){return import(1,f,out);};std::string error;
  if(!display(q,s,error))throw std::runtime_error(error);
  if(events!=expected_events||grid!=expected_grid||seen!=trig){std::size_t mismatch{};while(mismatch<events.size()&&mismatch<expected_events.size()&&events[mismatch]==expected_events[mismatch])++mismatch;throw std::runtime_error("case "+std::to_string(c)+" trace mismatch byte "+std::to_string(mismatch)+" sizes "+std::to_string(events.size())+"/"+std::to_string(expected_events.size()));}
  // Failure preserves its completed source prefix: no unavailable cache,
  // root queue, or required renderer operation is accepted as success.
  if(c==0){auto missing=s;missing.policy={};if(display(q,missing,error))throw std::runtime_error("missing policy accepted");++guards;
   policy.buffering=true;policy.flushing=false;missing=s;missing.enqueue={};if(display(q,missing,error))throw std::runtime_error("missing queue accepted");++guards;
   policy.buffering=false;policy.render_cache=true;missing=s;missing.cache_validate={};if(display(q,missing,error))throw std::runtime_error("missing cache accepted");++guards;
  }
 }
 if(p!=b.size())throw std::runtime_error("trailing gold");std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<n<<",\"ordered_trig_calls\":"<<trigcalls<<",\"required_provider_guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
