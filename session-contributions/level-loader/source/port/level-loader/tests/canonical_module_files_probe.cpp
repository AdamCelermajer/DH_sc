#include "canonical_module_files_v1.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "fixed_sources_v1.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;using namespace dh2::loader;
static void check(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
static assets::ZipAssetPackV1 pack(const char* path){
 auto f=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);check(bool(*f),"cache unavailable");
 assets::ZipBackingV1 b;b.owner=f;b.bytes=std::uint64_t(f->tellg());
 b.read=[f](std::uint64_t at,void* out,std::size_t n,std::string& e){f->clear();f->seekg(std::streamoff(at));f->read(static_cast<char*>(out),std::streamsize(n));if(!*f){e="cache read failure";return false;}return true;};
 assets::ZipAssetPackV1 p;std::string e;check(p.mount(std::move(b),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);return p;
}
struct Context {unsigned parses{},releases{};bool release_fails{};};
static CanonicalFileSourceServicesV1 source(std::shared_ptr<Context> c){return {
 [c](bool ok,std::string& e){++c->parses;if(!ok)e="declared parse failure";return ok;},
 [c](std::string& e){++c->releases;if(c->release_fails){e="declared source release failure";return false;}return true;}};}
static std::shared_ptr<CanonicalLevelContextV1> level(std::shared_ptr<Context> c){
 std::shared_ptr<CanonicalLevelContextV1> p;std::string e;check(CanonicalLevelContextV1::create({"SWAMP","data/scene/001_swamp.mlx"},c,p,e),e);return p;
}
static std::shared_ptr<CanonicalModuleFilesV1> relay(assets::ZipAssetPackV1 p,world::CanonicalObjectManagerV1& m,
 world::CanonicalClassServicesV1 classes,std::shared_ptr<Context> c,std::shared_ptr<CanonicalLevelContextV1> l){
 std::shared_ptr<CanonicalModuleFilesV1> out;std::string e;check(CanonicalModuleFilesV1::create(std::move(p),m,classes,source(c),std::move(l),c,out,e),e);return out;
}
int main(int argc,char** argv){if(argc!=3)return 2;try{
 auto original=pack(argv[1]),fixtures=pack(argv[2]);unsigned checks=0;std::string error;
 world::CanonicalPropertyMapV1 properties({});world::CanonicalReceiverConstructionV1 construction;
 construction.unknown_type_debug=[](void*,const char*,std::string&){return true;};
 world::CanonicalClassReceiverBindingsV1 bindings(properties,construction);
 world::CanonicalObjectManagerV1 manager({});auto c=std::make_shared<Context>();auto same=level(c);
 auto r=relay(fixtures,manager,bindings.services(),c,same);auto saved=r;
 check(!CanonicalModuleFilesV1::create({},manager,{},source(c),same,c,r,error)&&r==saved,"invalid source replaced relay");
 check(!CanonicalModuleFilesV1::create(fixtures,manager,{},source(c),{},c,r,error)&&r==saved,"missing SAME Level accepted");++checks;
 auto fields=same->module_load_fields();auto native=r->load_borrow();
 check(native.owner&&native.object_module_id18c==fields.object_module_id18c&&native.module_offset160==fields.module_offset160,"Module borrowed other Level fields");++checks;
 world::ModuleXmlFieldsV1 xml;xml.mgp378="player.mgp";xml.mvp390="player.mgp";const float position[]{11,22,33};
 check(r->begin_module(23,error)&&world::module_load_v1(xml,401,position,{},native,error),error);
 check(r->file_count()==2&&c->parses==2&&c->releases==2&&manager.source_count50()==0,"same URI occurrences collapsed or gates constructed fake actors");
 for(std::size_t i=0;i<2;++i){auto& a=r->file(i).attempts();check(a.size()==1&&a[0]->step()==CanonicalBoundSourceStepV1::original_source_skip&&!a[0]->factory_attempt(),"original Player exclusion changed");}
 ++checks;
 auto first=r->file(0).attempts()[0]->source().request(),second=r->file(1).attempts()[0]->source().request();
 check(first.source_lease.get()!=second.source_lease.get()&&first.module_occurrence==23&&first.runtime_module_id==401&&first.module_offset==std::array<float,3>{11,22,33},"source request collapsed occurrence or used diagnostic ID as runtime ID");
 check(*fields.object_module_id18c==-1&&fields.module_offset160[2]==0,"source Module did not reset SAME Level after both files");++checks;
 const float next_position[]{-2,-0.f,1};xml.mvp390.clear();
 check(r->begin_module(24,error)&&world::module_load_v1(xml,402,next_position,{},native,error),error);
 auto next=r->file(2).attempts()[0]->source().request();
 check(r->file_count()==3&&next.module_occurrence==24&&next.runtime_module_id==402&&std::signbit(next.module_offset[1]),"later Module reused previous context or lost float bits");++checks;
 unsigned owners=0;
 check(r->discard_after_owner_release([&](std::string&){++owners;return true;},error)&&owners==1&&r->file_count()==0&&r->discarded(),error);
 check(r->discard_after_owner_release([&](std::string&){++owners;return true;},error)&&owners==1&&!r->load_borrow().owner,"completed discard replayed owner or published new borrow");
 // Earlier borrow remains a lifetime pin, even after source journals release.
 check(native.object_module_id18c==fields.object_module_id18c&&*native.object_module_id18c==-1,"retained Module field borrow dangled after discard");++checks;
 {
  FixedSourcesV1 input;check(input.prepare(original,"SWAMP","data/scene/001_swamp.mlx",error),error);auto retained=input.borrow();
  auto gameplay=retained.module_links().front().gameplay;check(gameplay!=no_source_v1,"original first Module lacks gameplay file");
  const auto uri=retained.documents().at(gameplay).uri();auto actual_c=std::make_shared<Context>();auto actual_level=level(actual_c);
  world::CanonicalObjectManagerV1 actual_manager({});auto actual=relay(original,actual_manager,bindings.services(),actual_c,actual_level);
  check(actual->begin_module(0,error),error);auto borrow=actual->load_borrow();world::ModuleXmlFieldsV1 selected;selected.mgp378=uri;
  check(!world::module_load_v1(selected,77,position,{},borrow,error),"original SWAMP unsupported constructor silently completed");
  check(actual->file_count()==1&&!actual->file(0).attempts().empty()&&actual_manager.source_count50()==0,"original registered failure lost source prefix");
  const auto& failed=*actual->file(0).attempts().back();auto request=failed.source().request();
  check(failed.factory_attempt()&&failed.factory_attempt()->prefix()==world::CanonicalFactoryStageV1::empty&&request.runtime_module_id==77&&request.module_occurrence==0&&request.module_offset==std::array<float,3>{11,22,33},"real MGP factory prefix lost SAME Level context");
  const auto diagnostic=error;bool loaded=true;
  check(!borrow.load_file(uri,"Module",loaded,error)&&!loaded&&error==diagnostic&&actual_c->parses==1,"failed real source replayed");
  check(*borrow.object_module_id18c==77&&borrow.module_offset160[2]==33,"source failure fabricated Module reset");++checks;
  std::cout<<"real_first_failure="<<*failed.source().entry().source().attribute("gametype")<<" uri="<<uri<<'\n';
 }
 {
  auto ctx=std::make_shared<Context>();auto owner=level(ctx);world::CanonicalObjectManagerV1 m({});auto pending=relay(fixtures,m,bindings.services(),ctx,owner);
  check(pending->begin_module(0,error),error);auto b=pending->load_borrow();*b.object_module_id18c=9;bool loaded=true;
  check(b.load_file("player.mgp","Module",loaded,error)&&!loaded&&pending->file_count()==1,"source did not yield initial pending");
  check(!b.load_file("other.mgp","Module",loaded,error)&&pending->file_count()==1&&ctx->parses==1,"pending URI switched or replayed");++checks;
 }
 {
  auto ctx=std::make_shared<Context>();auto owner=level(ctx);world::CanonicalObjectManagerV1 m({});auto pending=relay(fixtures,m,bindings.services(),ctx,owner);
  check(pending->begin_module(0,error),error);auto b=pending->load_borrow();bool loaded=true;
  check(b.load_file("player.mgp","Module",loaded,error)&&!loaded,error);b.module_offset160[0]=-0.f;
  check(!b.load_file("player.mgp","Module",loaded,error)&&ctx->parses==1&&pending->file_count()==1,"pending SAME Level float-bit mutation ignored");++checks;
 }
 {
  auto ctx=std::make_shared<Context>();auto owner=level(ctx);world::CanonicalObjectManagerV1 m({});auto pending=relay(fixtures,m,bindings.services(),ctx,owner);
  check(pending->begin_module(0,error),error);auto b=pending->load_borrow();bool loaded=true;
  check(b.load_file("player.mgp","Module",loaded,error)&&!loaded,error);
  check(!pending->begin_module(1,error)&&pending->file_count()==1,"pending module interleaved");++checks;
  unsigned releases=0;ctx->release_fails=true;
  check(!pending->discard_after_owner_release({},error)&&pending->file_count()==1,"discard without owner release lost XML");
  check(!pending->discard_after_owner_release([&](std::string&){++releases;return true;},error)&&releases==1&&pending->file_count()==1,"failed XML release lost journal");
  check(!b.load_file("player.mgp","Module",loaded,error),"source resumed after release request");ctx->release_fails=false;
  check(pending->discard_after_owner_release([&](std::string&){++releases;return true;},error)&&releases==1&&ctx->releases==2&&pending->file_count()==0,"cleanup retry replayed released owner");++checks;
 }
 {
  auto ctx=std::make_shared<Context>();auto owner=level(ctx);world::CanonicalObjectManagerV1 m({});std::weak_ptr<CanonicalModuleFilesV1> weak;
  world::ModuleLevelLoadBorrowV1 held;
  {auto live=relay(fixtures,m,bindings.services(),ctx,owner);weak=live;held=live->load_borrow();live.reset();check(!weak.expired(),"borrow failed to pin relay");}
  held={};check(weak.expired(),"Module callback retained itself");++checks;
 }
 {
  auto ctx=std::make_shared<Context>();auto owner=level(ctx);world::CanonicalObjectManagerV1 m({});std::shared_ptr<CanonicalModuleFilesV1> reentrant;bool rejected=false;
  auto endpoints=source(ctx);endpoints.parse_result=[&](bool ok,std::string&){bool loaded=true;std::string inner;rejected=!reentrant->load_file("player.mgp","Module",loaded,inner)&&!loaded;return ok;};
  check(CanonicalModuleFilesV1::create(fixtures,m,bindings.services(),endpoints,owner,ctx,reentrant,error),error);
  check(reentrant->begin_module(0,error),error);bool loaded=true;
  check(!reentrant->load_file("player.mgp","Module",loaded,error)&&rejected&&reentrant->file_count()==1,"reentrant source delivery was not latched");++checks;
 }
 {
  auto ctx=std::make_shared<Context>();auto owner=level(ctx);world::CanonicalObjectManagerV1 m({});std::shared_ptr<CanonicalModuleFilesV1> cleanup;
  auto endpoints=source(ctx);bool source_rejected=false,owner_rejected=false;unsigned cleanup_calls=0;
  endpoints.release_load_state=[&](std::string&){++ctx->releases;std::string inner;source_rejected=!cleanup->discard_after_owner_release([&](std::string&){++cleanup_calls;return true;},inner);return true;};
  check(CanonicalModuleFilesV1::create(fixtures,m,bindings.services(),endpoints,owner,ctx,cleanup,error),error);
  check(cleanup->begin_module(0,error),error);bool loaded=true;
  check(cleanup->load_file("player.mgp","Module",loaded,error)&&!loaded,error);
  check(cleanup->discard_after_owner_release([&](std::string&){++cleanup_calls;std::string inner;owner_rejected=!cleanup->discard_after_owner_release([&](std::string&){++cleanup_calls;return true;},inner);return true;},error),error);
  check(owner_rejected&&source_rejected&&cleanup_calls==1&&ctx->releases==1&&cleanup->discarded()&&cleanup->file_count()==0,"recursive cleanup repeated released owner or source callback");++checks;
 }
 std::cout<<"{\"validation\":\"PASS\",\"module_file_checks\":"<<checks<<",\"positive_source_gate_fixtures\":true,\"class_construction_verified\":false,\"full_loader_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
