#include "../app/src/main/cpp/native_menu_preview_physics_guard_v124.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
namespace {
void check(bool condition,const char* message){if(!condition){std::cerr<<message<<'\n';std::exit(1);}}
struct Fixture {bool ready{};bool readiness_ok{true};bool load_ok{true};unsigned ready_calls{},load_calls{};float bounds[4]{};};
bool ready(void* raw,bool& value,std::string& error){auto& f=*static_cast<Fixture*>(raw);++f.ready_calls;if(!f.readiness_ok){error="readiness failed";return false;}value=f.ready;error.clear();return true;}
bool load(void* raw,float x,float y,float right,float bottom,std::string& error){auto& f=*static_cast<Fixture*>(raw);++f.load_calls;f.bounds[0]=x;f.bounds[1]=y;f.bounds[2]=right;f.bounds[3]=bottom;if(!f.load_ok){error="load failed";return false;}f.ready=true;error.clear();return true;}
}
int main(){
 std::string error;Fixture f;
 auto ready_fn=[&](bool& v,std::string& e){return ready(&f,v,e);};
 auto load_fn=[&](float x,float y,float r,float b,std::string& e){return load(&f,x,y,r,b,e);};
 check(model_renderer::ensure_menu_preview_physics_v124(false,false,ready_fn,load_fn,error)&&f.ready_calls==0&&f.load_calls==0,"SetupCharacter null-scene source no-op must not synthesize PhysicalWorld");
 f.ready=true;check(model_renderer::ensure_menu_preview_physics_v124(true,false,ready_fn,load_fn,error)&&f.ready_calls==1&&f.load_calls==0,"existing same process backend must not be reloaded");
 f.ready=false;check(model_renderer::ensure_menu_preview_physics_v124(true,false,ready_fn,load_fn,error)&&f.ready_calls==2&&f.load_calls==1&&f.bounds[0]==0.f&&f.bounds[1]==0.f&&f.bounds[2]==1.f&&f.bounds[3]==1.f&&f.ready,"stale scene backend restores exact source menu bounds");
 f.ready=false;check(!model_renderer::ensure_menu_preview_physics_v124(true,true,ready_fn,load_fn,error)&&f.load_calls==1&&error.find("Character is retained")!=std::string::npos,"must never clear a retained Character body by reloading world");
 f.ready=false;f.readiness_ok=false;check(!model_renderer::ensure_menu_preview_physics_v124(true,false,ready_fn,load_fn,error)&&f.load_calls==1&&error=="readiness failed","readiness failure must propagate");
 f.readiness_ok=true;f.load_ok=false;check(!model_renderer::ensure_menu_preview_physics_v124(true,false,ready_fn,load_fn,error)&&f.load_calls==2&&error=="load failed","physical load failure must propagate");
 std::cout<<"menu preview PhysicalWorld guard: 6 cases passed\n";
}
