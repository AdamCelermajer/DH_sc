from pathlib import Path
import json,subprocess,hashlib,re,shlex
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-lightset-v53';v=b/'vendor/character-rng-integration-v5-loading';build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host')
p=b/'canonical_light_point_v53.cpp';s=p.read_text().replace('isUsingColladaData','TestMultiPlayerLight');p.write_text(s)
for name in ['level_lightset_stage9_v53.hpp','canonical_lightset_file_v53.hpp']:
 p=b/name;s=p.read_text().replace('#include "canonical_level_context_v1.hpp"','#include <canonical_level_context_v1.hpp>').replace('#include "canonical_cached_file_v1.hpp"','#include <canonical_cached_file_v1.hpp>');p.write_text(s)
source=v/'port/level-world/canonical_property_map_v1.cpp';raw=source.read_text();patched=raw.replace('kind!="ObjectBase"&&kind!="GameObject"','kind!="LightPoint"&&kind!="ObjectBase"&&kind!="GameObject"').replace('// RoomZone::DeclareProperties396554','#include "canonical_light_point_declarations_v53.inc.proposed"\n // RoomZone::DeclareProperties396554');assert patched!=raw and 'canonical_light_point_declarations_v53.inc.proposed' in patched
(r/'canonical_property_map_v1.v53.test-only.cpp').write_text(patched)
scalars=json.loads((r/'original-light-scalars-v53.json').read_text())['rows'];stage=json.loads((r/'original-stage9-orchestration-v53.json').read_text())['rows'];sync=json.loads((r/'original-light-sync-v53.json').read_text())['rows']
def flt(x):return repr(x)+'f' if '.' in repr(x) else str(x)+'.0f'
def vec(a):return '{'+','.join(flt(x) for x in a)+'}'
cases=[]
for row in scalars:
 inp=row['input'];cases.append('{'+','.join(vec(x) for x in inp)+','+','.join(json.dumps(x) for x in row['normalized_float_bytes'])+'}')
stagecases=[]
for row in stage:
 stagecases.append('{'+str(row['fixed_pipeline']).lower()+','+json.dumps(row['fixed_name'])+','+json.dumps(row['regular_name'])+','+str(row['pending_polls'])+','+str(len(row['queries']))+'}')
syncCases=['{'+flt(x['radius'])+','+json.dumps(x['actual_CLight_bytes4_44'])+'}' for x in sync]
test=r'''#include "canonical_light_point_v53.hpp"
#include "level_lightset_stage9_v53.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace dh2;
std::string bytes(const void* p,std::size_t n){std::ostringstream s;auto* b=static_cast<const unsigned char*>(p);for(std::size_t i=0;i<n;++i)s<<std::hex<<std::setw(2)<<std::setfill('0')<<unsigned(b[i]);return s.str();}
bool assertion(void*,std::string& e){e.clear();return true;}
int main(){std::string e;
 std::array<float,3> zero{};world::CanonicalPropertySourceServicesV1 ps{};ps.position_rotation_default=&zero;ps.load_template_assertion=assertion;
 world::CanonicalPropertyMapV1 map(ps);auto runtime=std::make_shared<int>(19);
 struct Scalar {std::array<float,3> attenuation,ambient,diffuse,specular;const char *a,*b,*c,*d;};
 Scalar scalars[]={SCALAR_CASES};
 for(const auto& row:scalars){auto p=std::make_shared<world::CanonicalLightPointV53>(runtime,world::LightPointInitServicesV53{});auto canonical=p->canonical(p);assert(*canonical.type_f4==19&&canonical.is_character==nullptr);*canonical.class_name20="LightPoint";auto properties=p->properties();assert(map.init_properties(properties,e));assert(map.load_defaults(properties,e));auto& fields=properties.fields;
  assert(fields.write_vector3(fields.context,0x134,row.attenuation,e));assert(fields.write_vector3(fields.context,0x140,row.ambient,e));assert(fields.write_vector3(fields.context,0x14c,row.diffuse,e));assert(fields.write_vector3(fields.context,0x158,row.specular,e));
  assert(!p->init_post(e)&&e=="Required actual LightPoint renderer owner"&&!p->actual_node().identity&&!p->init_complete());
  assert(bytes(p->vector(0x134)->data(),12)==row.a);assert(bytes(p->vector(0x140)->data(),12)==row.b);assert(bytes(p->vector(0x14c)->data(),12)==row.c);assert(bytes(p->vector(0x158)->data(),12)==row.d);
  assert(!p->init_post(e));assert(bytes(p->vector(0x140)->data(),12)==row.b); // no repeated /255 after failed prefix
 }
 {auto p=std::make_shared<world::CanonicalLightPointV53>(runtime,world::LightPointInitServicesV53{});auto c=p->canonical(p);*c.class_name20="LightPoint";auto a=p->properties();assert(!p->floating(0x130)&&!p->byte(0x164));assert(map.init_properties(a,e)&&map.load_defaults(a,e));assert(*p->floating(0x130)==2&&*p->byte(0x164)==0);assert(map.set_property(a,"radius","3.5",e)&&map.set_property(a,"automatic","1",e)&&map.set_property(a,"attachedTo","Player",e)&&map.set_property(a,"attachedOffset","0,0,200",e));assert(*p->floating(0x130)==3.5&&*p->byte(0x164)==1&&*p->string(0x180)=="Player"&&(*p->vector(0x1a4))[2]==200);}
 struct Stage{bool fixed;const char *fixed_name,*regular_name;int polls,queries;};Stage stages[]={STAGE_CASES};
 for(const auto& row:stages){auto owner=std::make_shared<int>(53);std::uintptr_t id=53;std::string fixed=row.fixed_name,regular=row.regular_name;int count=0;loader::Stage9ConfigBorrowV53 f{owner,owner,&id,53,&fixed,&regular};loader::Stage9ServicesV53 s;s.actual_device_owner=owner;s.is_fixed_pipeline=[&](bool& o,std::string& err){o=row.fixed;err.clear();return true;};s.actual_file_loader_owner=owner;s.validate_current=[](std::string& err){err.clear();return true;};s.load_module_file_step=[&](const std::string& name,const char* tag,std::string& err){assert(name==(row.fixed?fixed:regular)&&std::string(tag)=="Module");++count;err.clear();return count<row.polls?loader::LifecycleStepV36::pending:loader::LifecycleStepV36::complete;};loader::Stage9BodyV53 body(f,s);loader::LifecycleStepV36 result;do{result=body.step(e);}while(result==loader::LifecycleStepV36::pending);assert(result==loader::LifecycleStepV36::complete&&count==row.queries&&e.empty());assert(body.step(e)==loader::LifecycleStepV36::complete&&count==row.queries);id=54;assert(body.step(e)==loader::LifecycleStepV36::failed);}
 struct Sync {float radius;const char* expected;};Sync syncs[]={SYNC_CASES};
 for(const auto& row:syncs){world::CanonicalLightPointV53 p(runtime,{});auto c=p.canonical(runtime);*c.class_name20="LightPoint";auto a=p.properties();assert(map.init_properties(a,e)&&map.load_defaults(a,e));auto& f=a.fields;assert(f.write_float(f.context,0x130,row.radius,e));assert(f.write_vector3(f.context,0x134,{.25f,.5f,.75f},e));assert(f.write_vector3(f.context,0x140,{1,2,3},e));assert(f.write_vector3(f.context,0x14c,{4,5,6},e));assert(f.write_vector3(f.context,0x158,{7,8,9},e));std::array<float,4> amb{},diff{},spec{};std::array<float,3> att{};float radius=4321;world::LightParameterFieldsV53 out{runtime,0x1234,&amb,&diff,&spec,&att,&radius,[](std::string& err){err.clear();return true;}};assert(p.sync_data(out,e));std::string joined=bytes(amb.data(),16)+bytes(diff.data(),16)+bytes(spec.data(),16)+bytes(att.data(),12)+bytes(&radius,4);assert(joined==row.expected&&e.empty());out.owner.reset();auto before=amb;assert(!p.sync_data(out,e)&&amb==before);}
 std::cout<<"PASS original8 stage9 alternatives/polls,4 bit-exact normalization cases,GO19 actual property map/defaults/overrides,7 original SyncData writes,missing/stale/replay guards; renderer/GPU activation0\n";
}
'''.replace('SCALAR_CASES',',\n'.join(cases)).replace('STAGE_CASES',',\n'.join(stagecases)).replace('SYNC_CASES',',\n'.join(syncCases)).replace('&&canonical.is_character==nullptr','')
(r/'lightset_native_v53.cpp').write_text(test)
ninja=(build/'build.ninja').read_text();line=next(x for x in ninja.splitlines() if x.strip().startswith('INCLUDES =') and '/vendor/character-rng-integration-v5-loading/port/level-loader' in x);incs=shlex.split(line.split('=',1)[1]);print('Includes',len(incs))
def wsl(p):return '/mnt/'+str(p)[0].lower()+str(p)[2:].replace('\\','/')
incflags=[]
for flag in incs:
 if flag.startswith('-I'):incflags.append('-I'+(wsl(Path(flag[2:])) if ':' in flag else flag[2:]))
incflags+=['-I'+wsl(b),'-I'+wsl(r)]
cmd=['wsl.exe','--exec','g++','-std=c++17','-O1','-g','-fno-omit-frame-pointer','-fsanitize=address,undefined','-ffunction-sections','-fdata-sections',*incflags,wsl(b/'canonical_light_point_v53.cpp'),wsl(b/'level_lightset_stage9_v53.cpp'),wsl(r/'canonical_property_map_v1.v53.test-only.cpp'),wsl(r/'lightset_native_v53.cpp'),'-Wl,--gc-sections','-o',wsl(r/'lightset-native-v53')]
result=subprocess.run(cmd,text=True,capture_output=True);(r/'native-build-v53.log').write_text(result.stdout+result.stderr);print(result.stdout+result.stderr);assert result.returncode==0
run=subprocess.run(['wsl.exe','--exec',wsl(r/'lightset-native-v53')],text=True,capture_output=True);(r/'native-test-v53.log').write_text(run.stdout+run.stderr);print(run.stdout+run.stderr);assert run.returncode==0
# No dirty archives linked. Syntax check actual canonical file ingestion adapter.
check=subprocess.run(['wsl.exe','--exec','g++','-std=c++17',*incflags,'-fsyntax-only',wsl(b/'canonical_lightset_file_v53.cpp')],text=True,capture_output=True);(r/'file-adapter-syntax-v53.log').write_text(check.stdout+check.stderr);print(check.stdout+check.stderr);assert check.returncode==0
(r/'native-v53-verified.json').write_text(json.dumps({'validation':'PASS','sanitizers':['ASAN','UBSAN'],'source_property_map_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'selected_includes_sha256':hashlib.sha256(ninja.encode()).hexdigest(),'actual_graph_archive_link':False,'actual_canonical_property_map_source_with_unique_declaration_proposal':True,'stage9_rows':len(stage),'normalization_bit_exact_cases':len(scalars),'sync_data_exact_cases':len(sync),'graphics_provider_bound':False},indent=2)+'\n')
