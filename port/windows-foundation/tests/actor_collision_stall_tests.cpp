// w5-collision harness: walks the Knight body (ActorMovement, approximate CollisionScene path) on the real
// Swamp collision from the shipped package from many start points, along 16 headings, and reports stalls:
// a heading is a stall when the body covers < 30% of the commanded distance while another heading from the
// same start covers >= 90% (a free direction exists). A stall is a wall stall when floor continues 30 units
// ahead of the stopped body; otherwise it is an edge of the floor the approximate collision knows about.
// Also checks a synthetic 45 degree wall (tangent slide). Body/policy values are the ones the shipped run
// logged for KnightPlayerBase (radius 65.44, height 344.56, development floor-edge policy 50/200/60).
#include "../actor_movement.hpp"
#include "../asset_catalog.hpp"
#include "../collision_scene.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool condition,const std::string& message){if(!condition)throw std::runtime_error(message);}
constexpr float pi=3.14159265358979323846f;
constexpr int headings=16;
constexpr int frames=40;
constexpr float speedPerFrame=14.0f; // Swamp run trace: ~14.1 units per frame at fixed step .016
ActorMovementConfig knightConfig(){
    ActorMovementConfig c;c.walkSpeed=speedPerFrame*60;c.runSpeed=c.walkSpeed;c.turnSpeedRadians=3;
    c.bodyRadius=65.4417f;c.bodyHeight=344.557f;c.maxStepUp=50;c.maxStepDown=200;c.maxSlopeDegrees=60;
    c.forwardAxis={0,-1,0};return c;
}
// Walks `frames` frames toward heading h from p; returns the horizontal distance covered and the end point.
float walk(const CollisionScene& scene,Vec3 p,int h,const ActorMovementConfig& config,Vec3* end=nullptr){
    ActorMovement m(config);m.setPosition(p,0);
    const float a=h*2*pi/headings;
    for(int f=0;f<frames;++f)m.apply_authored_root_motion({speedPerFrame*std::cos(a),speedPerFrame*std::sin(a),0},scene);
    if(end)*end=m.state().position;
    return std::hypot(m.state().position.x-p.x,m.state().position.y-p.y);
}
struct Census {int starts=0,grounded=0,stalls=0,wallStalls=0;};
Census census(const CollisionScene& scene,float x0,float x1,float y0,float y1,float step,bool print,const char* label){
    const auto config=knightConfig();Census out;
    const float cmd=speedPerFrame*frames;
    for(float y=y0;y<=y1;y+=step)for(float x=x0;x<=x1;x+=step){
        ++out.starts;
        ActorMovement probe(config);probe.setPosition({x,y,0},0);
        if(!probe.snap_to_floor(scene))continue;
        ++out.grounded;
        const Vec3 p=probe.state().position;
        float d[headings];float best=0;Vec3 end[headings];
        for(int h=0;h<headings;++h){d[h]=walk(scene,p,h,config,&end[h]);best=std::max(best,d[h]);}
        for(int h=0;h<headings;++h)
            if(d[h]<0.3f*cmd&&best>=0.9f*cmd){
                ++out.stalls;
                const float a=h*2*pi/headings;FloorHit ahead;
                const bool wall=scene.floor(end[h].x+30*std::cos(a),end[h].y+30*std::sin(a),end[h].z,ahead,50,200);
                if(wall)++out.wallStalls;
                if(print&&wall&&out.wallStalls<=12)std::printf("  %s wall stall start=(%.1f,%.1f,%.1f) heading=%d covered=%.1f of %.1f (best free %.1f)\n",label,p.x,p.y,p.z,h*360/headings,d[h],cmd,best);
            }
    }
    return out;
}
// Synthetic check: a vertical 45 degree wall on the line x = y. The body starts on the x>y side at
// (-100,-200) and walks (-10,0) per frame, i.e. into the wall with a tangent component along (-1,-1).
// The body must slide along the wall (tangent), not stop at the first contact.
void diagonal_wall_slides(){
    CollisionScene scene;
    scene.triangles.push_back({{-2000,-2000,0},{2000,-2000,0},{2000,2000,0},true});
    scene.triangles.push_back({{-2000,-2000,0},{2000,2000,0},{-2000,2000,0},true});
    scene.triangles.push_back({{-500,-500,0},{500,500,0},{500,500,400},false});
    scene.triangles.push_back({{-500,-500,0},{500,500,400},{-500,-500,400},false});
    auto config=knightConfig();config.bodyRadius=20;config.bodyHeight=100;
    ActorMovement m(config);m.setPosition({-100,-200,0},0);
    for(int f=0;f<60;++f)m.apply_authored_root_motion({-10,0,0},scene);
    const float dx=m.state().position.x+100,dy=m.state().position.y+200;
    const float along=std::hypot(dx,dy);
    const float expected=60*5*std::sqrt(2.0f); // tangent component 5 per axis per frame
    std::printf("diagonal wall: walk (-10,0) x60 moved (%.1f,%.1f) length %.1f; tangent slide expects about %.1f\n",dx,dy,along,expected);
    check(along>=0.8f*expected,"walk into a diagonal wall stopped instead of sliding along the tangent");
}
}

int main(int argc,char** argv){try{
    check(argc==2,"Supply the shipped package assets directory");
    AssetCatalog assets(argv[1]);CollisionScene swamp;std::string error;
    check(load_collision_level(assets,"data/scene/001_swamp.mlx",swamp,error),error);
    std::printf("Swamp approximate collision triangles=%zu floors=%zu\n",swamp.triangles.size(),swamp.floorInstances);
    const auto start=census(swamp,1000,1180,-320,-100,30,false,"start");
    const auto edge=census(swamp,260,340,40,120,10,true,"edge");
    std::printf("start area: starts=%d grounded=%d stalls=%d wallStalls=%d\n",start.starts,start.grounded,start.stalls,start.wallStalls);
    std::printf("platform edge: starts=%d grounded=%d stalls=%d wallStalls=%d\n",edge.starts,edge.grounded,edge.stalls,edge.wallStalls);
    diagonal_wall_slides();
    std::cout<<"actor collision stall harness passed\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
