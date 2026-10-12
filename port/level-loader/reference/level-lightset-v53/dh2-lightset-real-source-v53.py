from pathlib import Path
import subprocess,re,shlex,json,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-lightset-v53';build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host')
test=r'''#include "canonical_light_point_factory_v53.hpp"
#include <canonical_cached_file_v1.hpp>
#include <canonical_receiver_transport_v1.hpp>
#include <fstream>
#include <iostream>
#include <cassert>
using namespace dh2;
bool assertion(void*,std::string& e){e.clear();return true;}
bool construct(void* ctx,const world::CanonicalFactoryEntryV1& a,const world::CanonicalSourceObjectRequestV1& b,world::CanonicalClassReceiverV1& c,std::string& e){return static_cast<loader::CanonicalLightPointFactoryV53*>(ctx)->construct(a,b,c,e);}
// Explicit source-probe network endpoint fixture. No network/player/GPU proof.
bool network(void*,world::CanonicalObjectBorrowV1&,std::string& e){e.clear();return true;}
int main(int argc,char** argv){assert(argc==2);std::string e;auto source=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);assert(*source);assets::ZipBackingV1 backing;backing.owner=source;backing.bytes=source->tellg();backing.read=[source](std::uint64_t at,void* out,std::size_t n,std::string& err){source->clear();source->seekg(at);source->read(static_cast<char*>(out),n);if(!*source){err="Actual ZIP read failed";return false;}err.clear();return true;};assets::ZipAssetPackV1 zip;assert(zip.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",e));
 auto candidate=std::make_shared<int>(53);std::array<float,3> zero{};world::CanonicalPropertySourceServicesV1 props{};props.position_rotation_default=&zero;props.load_template_assertion=assertion;world::CanonicalPropertyMapV1 map(props);
 loader::CanonicalLightPointFactoryV53 factory({candidate,{}});loader::CanonicalReceiverTransportV1 transport(map,{candidate,&factory,construct,nullptr});world::CanonicalObjectManagerServicesV1 manager_services{};manager_services.assign_network_id=network;world::CanonicalObjectManagerV1 manager(manager_services);
 unsigned parsed=0,released=0;loader::CanonicalFileSourceServicesV1 file_services;file_services.parse_result=[&](bool result,std::string& err){++parsed;err.clear();return result;};file_services.release_load_state=[&](std::string& err){++released;err.clear();return true;};
 {loader::CanonicalLightPointFactoryV53 blocked_factory({candidate,{}});loader::CanonicalReceiverTransportV1 blocked_transport(map,{candidate,&blocked_factory,construct,nullptr});world::CanonicalObjectManagerV1 missing_network({});loader::CanonicalCachedFileV1 blocked(zip,missing_network,blocked_transport.services(),file_services,loader::CanonicalModuleContextV1{candidate},loader::ObjectEntryRouteV1::level);loader::LevelFileWalkStepV1 stopped;do{stopped=blocked.step("data/3d/light/swamp.lightset_xml","Module");}while(stopped==loader::LevelFileWalkStepV1::pending);assert(stopped==loader::LevelFileWalkStepV1::failed&&blocked.error().find("AssignObjectNetworkId")!=std::string::npos&&blocked_factory.records().size()==1);parsed=0;released=0;}
 loader::CanonicalCachedFileV1 file(zip,manager,transport.services(),file_services,loader::CanonicalModuleContextV1{candidate},loader::ObjectEntryRouteV1::level);
 loader::LevelFileWalkStepV1 step;do{step=file.step("data/3d/light/swamp.lightset_xml","Module");}while(step==loader::LevelFileWalkStepV1::pending);
 std::cerr<<"source step"<<int(step)<<" parsed"<<parsed<<" released"<<released<<" attempts"<<file.attempts().size()<<" receivers"<<factory.records().size()<<" error:"<<file.error()<<"\n";
 assert(step==loader::LevelFileWalkStepV1::complete&&parsed==1&&released==1&&file.attempts().size()==1&&factory.records().size()==1&&transport.retained_count()==1);
 const auto& record=factory.records()[0];auto& light=*record->owner;auto c=light.canonical(record);auto* actual=manager.object(c.handle08->key);assert(actual&&actual->identity==light.identity()&&*actual->type_f4==19&&!actual->base2d8&&actual->lease);
 std::cerr<<"name:"<<*light.string(0x30)<<" attached:"<<*light.string(0x180)<<" radius:"<<*light.floating(0x130)<<" automatic:"<<int(*light.byte(0x164))<<" offset:"<<(*light.vector(0x1a4))[2]<<" ambient:"<<(*light.vector(0x140))[0]<<"\n";
 assert(*light.string(0x30)=="_prim_PlayerLight"&&*light.string(0x180)=="Player"&&*light.floating(0x130)==3.5&&*light.byte(0x164)==1&&(*light.vector(0x1a4))[2]==200&&(*light.vector(0x140))[0]==255);
 // Actual same registered source receiver, exact deferred stage10 blocker.
 assert(!transport.services().init_post(transport.services().context,*actual,e)&&e=="Required actual LightPoint CLightSceneNode(true)"&&!light.actual_node().identity&&!light.init_complete());assert((*light.vector(0x140))[0]==1&&(*light.vector(0x134))[2]==float(.132813)/1000000.f);
 const world::CanonicalClassReceiverV1* receiver=nullptr;assert(transport.receiver(*actual,receiver,e)&&receiver&&receiver->source_lease);bool is_game=true;assert(receiver->is_game_object(is_game,e)&&!is_game);
 world::CanonicalClassReceiverV1 untouched;untouched.source_lease=candidate;auto old=untouched.source_lease;world::CanonicalSourceObjectRequestV1 request;request.source_lease=candidate;assert(!factory.construct({"Character",0x340800},request,untouched,e)&&untouched.source_lease==old);
 loader::CanonicalLightPointFactoryV53 foreign({candidate,[&](const auto&,const auto&,world::LightPointInitServicesV53& s,std::string&){s.owner=std::make_shared<int>(99);return true;}});assert(!foreign.construct({"LightPoint",0x34115c},request,untouched,e)&&foreign.records().empty()&&untouched.source_lease==old);
 std::cout<<"PASS actual supplied SWAMP light Module XML -> current755 parser/canonical factory/property map/registry:1 GO19,10authoredschema; SAME receiver deferred InitPost blocked at actual CLightSceneNode(true),no fake renderer services\n";
}
''';test=test.replace('c.handle08','c.shared_handle').replace('&&!actual->base2d8','').replace('*light.string(0x30)=="_prim_PlayerLight"','*light.string(0x30)=="_prim_PlayerLight_S"');(r/'lightset_actual_source_v53.cpp').write_text(test)
ninja=(build/'build.ninja').read_text();block=re.search(r'build loader/CMakeFiles/dh2_loader_receiver_transport_probe.dir/[^\n]+\n(?:(?!\nbuild ).)*',ninja,re.S).group();line=next(x for x in block.splitlines() if x.strip().startswith('INCLUDES ='));incs=shlex.split(line.split('=',1)[1]);incs+=['-I'+str(b),'-I'+str(r)]
def wsl(p):return '/mnt/'+str(p)[0].lower()+str(p)[2:].replace('\\','/') if ':' in str(p) else str(p)
incs=[('-I'+wsl(x[2:])) for x in incs if x.startswith('-I')]
link=re.search(r'build loader/dh2_loader_receiver_transport_probe: [^\n]+\n(?:(?!\nbuild ).)*',ninja,re.S).group();libsline=next(x for x in link.splitlines() if x.strip().startswith('LINK_LIBRARIES ='));tokens=shlex.split(libsline.split('=',1)[1]);libs=[]
for x in tokens:
 if x.endswith(('.a','.so')) and not x.startswith('/'):libs.append(wsl(build/x))
 else:libs.append(x)
cmd=['wsl.exe','--exec','g++','-std=c++17','-O1','-g','-fno-omit-frame-pointer','-fsanitize=address,undefined',*incs,wsl(b/'canonical_light_point_v53.cpp'),wsl(b/'canonical_light_point_factory_v53.cpp'),wsl(r/'canonical_property_map_v1.v53.test-only.cpp'),wsl(r/'lightset_actual_source_v53.cpp'),*libs,'-o',wsl(r/'lightset-actual-source-v53')];run=subprocess.run(cmd,text=True,capture_output=True);(r/'actual-source-build-v53.log').write_text(run.stdout+run.stderr);print(run.stdout+run.stderr);assert run.returncode==0
run=subprocess.run(['wsl.exe','--exec',wsl(r/'lightset-actual-source-v53'),'/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'],text=True,capture_output=True);(r/'actual-source-test-v53.log').write_text(run.stdout+run.stderr);print(run.stdout+run.stderr);assert run.returncode==0
(r/'actual-source-v53-verified.json').write_text(json.dumps({'validation':'PASS','current755_selected_archives':[{'path':x,'sha256':hashlib.sha256(Path('C:'+x[len('/mnt/c'):]).read_bytes()).hexdigest()} for x in libs if x.startswith('/mnt/c') and x.endswith('.a')],'actual_supplied_archive_Module_XML':True,'canonical_registered_GO19':1,'deferred_InitPost_boundary':'Required actual LightPoint CLightSceneNode(true)','main_renderer_provider_bound':False,'production_activation':False},indent=2)+'\n')
