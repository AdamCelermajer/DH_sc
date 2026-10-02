#include "../layers.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <vector>
static std::uint32_t seed=20261002;
static std::uint32_t next(){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static std::vector<std::uint8_t> read(const char *path){
    std::ifstream stream(path,std::ios::binary);
    return {(std::istreambuf_iterator<char>(stream)),{}};
}
int main(int argc,char **argv){
    assert(argc==4);
    const auto first=read(argv[1]),second=read(argv[2]),model=read(argv[3]);
    dh2::resources::BresView model_view{},first_view{};
    assert(dh2_bres_open(&model_view,model.data(),model.size())==dh2::resources::BresError::ok);
    assert(dh2_bres_open(&first_view,first.data(),first.size())==dh2::resources::BresError::ok);
    dh2::pose::Clip first_clip{};assert(dh2_pose_clip_open(&first_clip,&first_view,0)==dh2::pose::Error::ok);
    dh2::skin::Skin skin{};assert(dh2_skin_open(&skin,&model_view,0)==dh2::skin::Error::ok);
    dh2::scene::Scene scene{};assert(dh2_scene_open(&scene,&model_view)==dh2::scene::Error::ok);
    dh2::scene::Visual visual{};assert(dh2_scene_visual(&scene,0,&visual)==dh2::scene::Error::ok);
    for(int test=0;test<3000;++test){
        auto bytes=second;
        if(test%3==0)bytes.resize(next()%bytes.size());
        if(test%3==1)for(std::uint32_t i=0,n=1+next()%12;i<n;++i)bytes[next()%bytes.size()]=next();
        const auto before_bytes=bytes;
        dh2::resources::BresView view{};
        if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)continue;
        dh2::pose::Clip clip{};
        if(dh2_pose_clip_open(&clip,&view,0)!=dh2::pose::Error::ok)continue;
        dh2::layers::Layers layers{};layers.count=test%5==0?next()%10:2;
        layers.items[0]={&first_clip,static_cast<std::int32_t>(next()%10000)-1000,.5f};
        layers.items[1]={&clip,static_cast<std::int32_t>(next()%10000)-1000,.5f};
        if(test%7==0)layers.items[0].weight=std::numeric_limits<float>::quiet_NaN();
        dh2::math::Matrix4f palette[256];std::memset(palette,0xa5,sizeof(palette));
        unsigned char before[sizeof(palette)];std::memcpy(before,palette,sizeof(palette));
        const auto capacity=test%11==0?skin.joints-1:256;
        const auto result=dh2_layers_skin_palette(&layers,&skin,&visual,palette,capacity);
        if(result!=dh2::pose::Error::ok)assert(std::memcmp(before,palette,sizeof(palette))==0);
        assert(std::memcmp(before+skin.joints*sizeof(*palette),palette+skin.joints,
                           sizeof(palette)-skin.joints*sizeof(*palette))==0);
        assert(bytes==before_bytes);
    }
    std::puts("layers safety: 3000 second-clip corruption/truncation probes; checked layer/count/capacity/weight failures");
}
