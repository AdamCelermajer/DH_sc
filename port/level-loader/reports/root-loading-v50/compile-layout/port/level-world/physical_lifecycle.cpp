#include "physical_lifecycle.hpp"
namespace {
using namespace dh2::physical;
bool valid(const LifecycleBody* view,const TransformWorld* world,const MassCallbacks* cb){
 if(!view||!view->body||!view->body->state||view->body->reserved||view->body->state->flags>0xffffu||view->type>0xffffu||view->pinned>1||!world||!cb)return false;
 for(auto b:world->reserved)if(b)return false;
 return !world->first_shape||(cb->compute_mass&&cb->update_sweep_radius&&cb->refilter_proxy);
}
void apply_center_and_type(LifecycleBody& v,const float* center,const TransformWorld& w,const MassCallbacks& cb){
 auto& b=*v.body;auto& s=*b.state;b.local_center[0]=center[0];b.local_center[1]=center[1];
 const float xx=center[0]*b.rotation[0],xy=center[1]*b.rotation[2],rx=xx+xy;
 const float yx=center[0]*b.rotation[1],yy=center[1]*b.rotation[3],ry=yx+yy;
 b.center[0]=rx+s.position[0];b.center[1]=ry+s.position[1];b.previous_center[0]=b.center[0];b.previous_center[1]=b.center[1];
 for(auto* shape=w.first_shape;shape;shape=shape->next)cb.update_sweep_radius(w.context,shape->identity,b.local_center);
 const auto old=v.type;v.type=v.inverse_mass==0.f&&v.inverse_inertia==0.f?0u:1u;
 if(old!=v.type){const BodyTransform transform{{s.position[0],s.position[1]},{b.rotation[0],b.rotation[1],b.rotation[2],b.rotation[3]}};for(auto* shape=w.first_shape;shape;shape=shape->next)cb.refilter_proxy(w.context,shape->identity,w.broadphase,&transform);}
}
void mass(LifecycleBody& v,const MassData& m,const TransformWorld& w,const MassCallbacks& cb){
 if(w.locked)return;v.inverse_mass=v.inertia=v.inverse_inertia=0.f;v.mass=m.mass;
 if(m.mass>0.f)v.inverse_mass=1.f/m.mass;
 if(!(v.body->state->flags&0x40u)){v.inertia=m.inertia;if(v.inertia>0.f)v.inverse_inertia=1.f/v.inertia;}
 apply_center_and_type(v,m.center,w,cb);
}
void shapes(LifecycleBody& v,const TransformWorld& w,const MassCallbacks& cb){
 if(w.locked)return;v.mass=v.inverse_mass=v.inertia=v.inverse_inertia=0.f;float center[2]{0.f,0.f};
 for(auto* shape=w.first_shape;shape;shape=shape->next){MassData m{};cb.compute_mass(w.context,shape->identity,&m);v.mass=v.mass+m.mass;const float x=m.mass*m.center[0],y=m.mass*m.center[1];center[0]=center[0]+x;center[1]=center[1]+y;v.inertia=v.inertia+m.inertia;}
 if(v.mass>0.f){v.inverse_mass=1.f/v.mass;center[0]=center[0]*v.inverse_mass;center[1]=center[1]*v.inverse_mass;}
 if(v.inertia>0.f&&!(v.body->state->flags&0x40u)){const float xx=center[0]*center[0],yy=center[1]*center[1],squared=xx+yy,shift=squared*v.mass;v.inertia=v.inertia-shift;v.inverse_inertia=1.f/v.inertia;}
 else v.inertia=v.inverse_inertia=0.f;
 apply_center_and_type(v,center,w,cb);
}
}
extern "C" int dh2_physical_set_mass(LifecycleBody* v,const MassData* m,const TransformWorld* w,const MassCallbacks* cb){if(!valid(v,w,cb)||!m)return 1;const auto snapshot=*m;mass(*v,snapshot,*w,*cb);return 0;}
extern "C" int dh2_physical_mass_from_shapes(LifecycleBody* v,const TransformWorld* w,const MassCallbacks* cb){if(!valid(v,w,cb))return 1;shapes(*v,*w,*cb);return 0;}
extern "C" int dh2_physical_pin(LifecycleBody* v,const TransformWorld* w,const MassCallbacks* cb){if(!valid(v,w,cb))return 1;if(!v->pinned){v->pinned=1;const MassData zero{0.f,{v->body->local_center[0],v->body->local_center[1]},0.f};mass(*v,zero,*w,*cb);}return 0;}
extern "C" int dh2_physical_unpin(LifecycleBody* v,const TransformWorld* w,const MassCallbacks* cb){if(!valid(v,w,cb))return 1;if(v->pinned){v->pinned=0;shapes(*v,*w,*cb);dh2_physical_wake(v->body->state);}return 0;}
