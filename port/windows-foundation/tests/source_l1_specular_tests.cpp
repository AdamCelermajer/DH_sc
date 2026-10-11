#include "../source_l1_specular.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool okay,const char* message){if(!okay)throw std::runtime_error(message);}
static bool near(float a,float b){return std::abs(a-b)<1e-5f;}
int main(){try{
    LevelPointLight light;light.position={0,0,200};light.attenuation={.265625f,0,.132813f/1000000};light.specular={1,.5f,.25f};
    std::array<float,4> out;
    check(source_l1_specular(light,{0,0,0},{0,0,1},{0,0,3132},256,out),"source near sample rejected");
    const float attenuation=1/(light.attenuation[0]+light.attenuation[2]*200*200);
    check(near(out[0],attenuation)&&near(out[1],attenuation*.5f)&&near(out[2],attenuation*.25f)&&out[3]==0,"source colored specular/headroom/alpha lost");
    const auto close=out;
    light.position.z=1000;
    check(source_l1_specular(light,{0,0,0},{0,0,1},{0,0,3132},256,out)&&out[0]<close[0],"authored distance attenuation lost");
    check(source_l1_specular(light,{0,0,0},{0,0,-1},{0,0,3132},10,out)&&out[0]==0,"backface light gate lost");
    light.position={0,0,200};
    check(source_l1_specular(light,{0,0,0},{0,0,.99f},{0,0,3132},256,out)&&near(out[0],attenuation*std::pow(.99f,256)),"authored exponent capped to128 or normal renormalized");
    const auto centered=out;
    check(source_l1_specular(light,{0,0,0},{0,0,.99f},{3132,0,0},256,out)&&out[0]<centered[0],"world eye half-vector ignored");
    Mat4 transform{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};transform[0]=2;transform[5]=3;transform[10]=4;Vec3 n;
    check(source_l1_world_normal(transform,{1,1,1},n)&&near(n.x,.5f)&&near(n.y,1.f/3)&&near(n.z,.25f),"inverse transpose normal mismatch");
    transform[10]=0;check(!source_l1_world_normal(transform,{0,0,1},n),"singular world inverse admitted");
    check(!source_l1_specular(light,light.position,{0,0,1},{0,0,3132},10,out),"undefined normalize admitted");
    light.specular={0,0,0};check(source_l1_specular(light,{0,0,0},{0,0,1},{0,0,3132},10,out)&&out[0]==0&&out[1]==0&&out[2]==0,"zero authored specular invented white");
    std::cout<<"PASS source L1 colored specular, attenuation, world eye, uncapped exponent, inverse transpose and undefined gates\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
