// B076: ambient AnimatedDecor material clips, decor loading and the level PlayerLight reader.
// usage: ambient_decor_tests <assets root with data/scene + original-cache> <android extra root with data/3d/*>
#include "ambient_decor_v1.hpp"
#include "../level_lighting/level_light_v1.hpp"
#include "../../actor_definitions.hpp"
#include "../../assembled_level.hpp"
#include "../../../scene-materials/scene.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstring>
#include <chrono>

namespace f = dh::foundation;
namespace sd = dh::foundation::scene_decor;
namespace ll = dh::foundation::level_lighting;

static int failures = 0;
static void check(bool ok, const char* text) { if (!ok) { ++failures; std::cerr << "FAIL: " << text << '\n'; } }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

static std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), {}};
}

static void material_defaults(const std::vector<std::uint8_t>& source) {
    using namespace dh2;
    resources::BresView view{};scene::Scene decoded;std::string error;
    check(dh2_bres_open(&view,source.data(),source.size())==resources::BresError::ok,"caustics fixture BRES");
    check(scene::load(view,decoded,error),"local effect defaults load");
    const auto find=[&](const scene::Scene& s)->const scene::Material*{for(const auto& m:s.materials)if(m.id=="Standard_2")return &m;return nullptr;};
    const auto* material=find(decoded);
    check(material&&near(material->texture_matrix[0],0.25f)&&near(material->texture_matrix[5],0.25f),"caustics inherit the effect's 4x4 atlas matrix");
    if(!material)return;
    const auto word=[&](std::size_t p){return std::uint32_t(source[p])|(std::uint32_t(source[p+1])<<8)|(std::uint32_t(source[p+2])<<16)|(std::uint32_t(source[p+3])<<24);};
    const auto p=std::size_t(dh2_bres_library_item(&view,resources::Library::material,0)-source.data());
    auto changed=source;
    const auto put=[&](std::size_t p,std::uint32_t v){for(unsigned j=0;j<4;++j)changed[p+j]=std::uint8_t(v>>(8*j));};
    // Replace one irrelevant material boolean with an explicit identity matrix.
    // The real local effect stays at 0.25; an explicit identity must win.
    const auto param=word(p+20);
    const std::string name="texture-matrix";
    const auto nameOffset=std::uint32_t(changed.size());changed.insert(changed.end(),name.begin(),name.end());changed.push_back(0);
    while(changed.size()%4)changed.push_back(0);
    const auto data=std::uint32_t(changed.size());changed.resize(changed.size()+64,0);
    for(unsigned i=0;i<16;++i)if(i%5==0)put(data+i*4,0x3f800000);
    put(param,nameOffset);put(param+8,10);put(param+20,data);put(12,std::uint32_t(changed.size()));
    check(dh2_bres_open(&view,changed.data(),changed.size())==resources::BresError::ok&&scene::load(view,decoded,error),"explicit matrix override fixture load");
    material=find(decoded);check(material&&near(material->texture_matrix[0],1)&&near(material->texture_matrix[5],1),"explicit identity overrides nonidentity effect default");
    put(param+8,7);
    check(!scene::load(view,decoded,error)&&!error.empty()&&decoded.materials.empty(),"malformed matrix override fails without partial scene");
}

static void render_ownership(const std::filesystem::path& root,const std::vector<std::uint8_t>& torches) {
    const auto fixture=std::filesystem::temp_directory_path()/("dh-decor-owner-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(fixture);
    struct Cleanup{std::filesystem::path path;~Cleanup(){std::error_code ec;std::filesystem::remove_all(path,ec);}} cleanup{fixture};
    f::AssetCatalog source(root),assets(fixture);
    const auto virtual_xml=[&](const char* path,const std::string& text){std::ofstream out(fixture/path,std::ios::binary);out<<text;};
    std::ofstream(fixture/"torch.bdae",std::ios::binary).put(0);
    std::ofstream(fixture/"world.bdae",std::ios::binary).put(0);
    assets.set_virtual_file("torch.bdae",torches);
    assets.set_virtual_file("world.bdae",source.read("data/3d/modules/swamp/swamp.bdae"));
    virtual_xml("owner-test.mlx","<Level><GameObject name='module' gametype='Module' dae='world.bdae' xrefobject='_module_deadend_brdwalk_w_00' mvp='owner-test.mvp'/></Level>");
    virtual_xml("owner-test.mvp","<Module><GameObject name='torch' gametype='AnimatedDecor' dae='torch.bdae' xrefobject='_anim_swamp_torchsingle'/></Module>");
    f::OriginalScene preview,gameplay;std::vector<f::LevelModuleZone> zones;std::string error;
    check(f::load_level_with_module_zones(assets,"owner-test.mlx",preview,zones,error),"geometry preview retains static decor");
    check(f::load_level_with_module_zones(assets,"owner-test.mlx",gameplay,zones,error,{false}),"gameplay level excludes runtime decor");
    std::vector<f::ActorDefinition> defs;
    check(f::load_actor_definitions(assets,"owner-test.mlx",defs,error),"runtime decor declarations load");
    sd::AmbientDecorRuntimeV1 runtime;std::vector<std::string> notices;
    check(runtime.load(assets,defs,{},notices,error)&&runtime.instances().size()==1,"runtime owns one decor instance");
    if(runtime.instances().size()==1)check(preview.triangleCount==gameplay.triangleCount+runtime.instances()[0].scene->triangleCount,"preview/runtime ownership partitions the same geometry once");
    for(auto& d:defs)if(d.gametype=="AnimatedDecor")d.properties["activate_cond"]="QuestStarted";
    check(runtime.load(assets,defs,{},notices,error)&&runtime.instances().empty(),"conditional decor excluded from ambient owner");
    for(auto& d:defs)if(d.gametype=="AnimatedDecor"){d.properties["activate_cond"]="Invalid";d.properties["visible"]="0";}
    check(runtime.load(assets,defs,{},notices,error)&&runtime.instances().empty(),"hidden decor excluded from ambient owner");
    for(auto& d:defs)if(d.gametype=="AnimatedDecor")d.properties["visible"]="1";
    check(runtime.load(assets,defs,{"torch"},notices,error)&&runtime.instances().empty(),"script-owned decor excluded from ambient owner");
    check(runtime.load(assets,defs,{},notices,error)&&runtime.instances().size()==1,"Invalid sentinel leaves ordinary decor admitted");
}

// B115: native CTextureTransformEx stores channel87/88 in state0/1,
// then buildTextureTransform places them at matrix8/9. The actual atlas
// phases below are deliberately unequal, so a transposed traversal fails.
static void water_texture_transform(const std::filesystem::path& extra) {
    f::AssetCatalog assets(extra); sd::AmbientDecorRuntimeV1 runtime;
    f::ActorDefinition water; water.name="water"; water.gametype="AnimatedDecor";
    water.modelPath="data/3d/animateddecors/swamp_watercaustics.bdae";
    water.properties["xrefobject"]="_anim_animwater_corner_brdwalk_sw_";
    water.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    std::vector<std::string> notices; std::string error;
    check(runtime.load(assets,{water},{},notices,error)&&runtime.instances().size()==1,"source water loads one owner");
    if(runtime.instances().empty()) return;
    const auto colors=[&]{std::vector<std::array<float,4>> out;for(const auto& range:runtime.instances()[0].scene->mesh.ranges)out.push_back(range.material.color);return out;}();
    const auto vertices=runtime.instances()[0].scene->mesh.vertices.size();
    const auto triangles=runtime.instances()[0].scene->triangleCount;
    const auto verify=[&](float u,float v,const char* message){
        const auto& instance=runtime.instances()[0];
        check(instance.scene->mesh.vertices.size()==vertices&&instance.scene->triangleCount==triangles,"UV update preserves geometry multiplicity");
        for(std::size_t i=0;i<instance.scene->mesh.ranges.size();++i){
            const auto& material=instance.scene->mesh.ranges[i].material;
            check(near(material.uvOffset[0],u)&&near(material.uvOffset[1],v),message);
            check(material.color==colors[i],"texture transform does not animate RGBA");
            check(near(instance.scene->materials[i].textureMatrix[0],.25f)&&near(instance.scene->materials[i].textureMatrix[5],.25f),"water keeps source atlas scale");
        }
    };
    runtime.update(0);verify(0,0,"water initial native transform");
    runtime.update(.1);verify(.25f,0,"native water100ms U=.25 V=0");
    runtime.update(.5);verify(.5f,.25f,"native water600ms U=.5 V=.25");
    runtime.update(.9);verify(0,0,"water1500ms wraps to native initial transform");
    water.properties["xrefobject"]="missing-water-root";
    notices.clear();
    check(runtime.load(assets,{water},{},notices,error)&&runtime.instances().empty()&&!notices.empty(),"missing water root rejects instead of publishing stale geometry");
}

static void full_material_transport(const std::filesystem::path& extra) {
    f::AssetCatalog assets(extra);sd::AmbientDecorRuntimeV1 runtime;
    f::ActorDefinition entrance;entrance.name="entrance";entrance.gametype="AnimatedDecor";
    entrance.modelPath="data/3d/animateddecors/swamp_cave_entrance_troll.bdae";
    entrance.properties["xrefobject"]="_anim_caveentrance_troll_";
    entrance.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    std::vector<std::string> notices;std::string error;
    check(runtime.load(assets,{entrance},{},notices,error)&&runtime.instances().size()==1,"real entrance loads once for material transport");
    if(runtime.instances().empty())return;
    auto& scene=*runtime.instances()[0].scene;
    bool opaque=false,alpha=false;std::size_t lookups=0;
    runtime.bind_textures([&](const std::string& uri,const std::string& mask,bool blue){
        ++lookups;check(!uri.empty(),"real entrance texture URI retained");
        if(!mask.empty())check(blue,"all entrance L1 alpha variants use source blue channel");
        return mask.empty()&&uri=="env_swamp_spec.tga"?202u:101u;
    });
    for(std::size_t i=0;i<scene.materials.size();++i){const auto& source=scene.materials[i];const auto& range=scene.mesh.ranges[i];
        if(source.id=="Material__11610"){
            opaque=true;check(range.indexCount==380*3&&range.material.levelLit&&!range.material.transparent&&range.material.alphaReference==0,"authored opaque Sp flags and geometry retained");
            check(source.specularMap=="env_swamp_spec.tga"&&near(source.glossiness,10)&&range.material.specularTexture==202&&near(range.material.glossiness,10),"opaque entrance retains authored specular binding");
        }
        if(source.id=="Material__11611"){
            alpha=true;check(range.material.transparent&&source.specularMap=="env_swamp_spec.tga"&&range.material.specularTexture==202,"alpha Sp binding retained without claiming renderer alpha pass");
        }
    }
    check(opaque&&alpha&&lookups>scene.materials.size()&&runtime.instances().size()==1,"specular lookups add resources without adding geometry");
    for(const auto technique:{"L1_Vc_Al_Sp_----_----_----","L1_Vc_At_----_----_----_----","L1_Vc_Al_----_----_----_----"}){
        f::OriginalMaterial source;source.diffuse="d";source.alphaMap="a";source.effectFile="GL_Diffuse_L1_VC_iPhone.bdae";source.technique=technique;
        f::Material target;target.specularTexture=999;target.glossiness=10;
        f::bind_original_material_textures(source,target,[&](const std::string&,const std::string& mask,bool blue){check(mask=="a"&&blue,"L1 alpha/alpha-test technique requests blue");return 123u;});
        check(target.texture==123&&target.specularTexture==0&&target.glossiness==0,"non-Sp map branch clears stale specular state");
        source.effectFile="ProfileCOMMON";
        f::bind_original_material_textures(source,target,[&](const std::string&,const std::string&,bool blue){check(!blue,"generic alpha retains red channel policy");return 124u;});
    }
    f::OriginalMaterial source;source.diffuse="d";source.specularMap="missing";source.glossiness=10;
    f::Material target;target.texture=7;target.specularTexture=8;target.glossiness=9;
    try{f::bind_original_material_textures(source,target,[&](const std::string& uri,const std::string&,bool){if(uri=="missing")throw std::runtime_error("lookup failed");return 77u;});check(false,"throwing lookup unexpectedly succeeded");}catch(const std::runtime_error&){}
    check(target.texture==7&&target.specularTexture==8&&target.glossiness==9,"failed second lookup preserves previous material binding");
    f::bind_original_material_textures(source,target,[&](const std::string& uri,const std::string&,bool){return uri=="missing"?0u:77u;});
    check(target.texture==77&&target.specularTexture==0,"unavailable specular texture remains zero without fabricated binding");
}

int main(int argc, char** argv) {
    if (argc < 3) { std::cerr << "usage\n"; return 2; }
    const std::filesystem::path assets = argv[1], extra = argv[2];

    // Sampler: hold before/after, step (not linear) and linear branches.
    sd::AmbientTrack stepTrack;
    stepTrack.times = {0, 100, 200};
    stepTrack.values = {{0, 0, 0, 0}, {0.5f, 0, 0, 0}, {1.0f, 0, 0, 0}};
    check(near(sd::sample_ambient_track(stepTrack, -5)[0], 0.0f), "hold before first key");
    check(near(sd::sample_ambient_track(stepTrack, 99.9)[0], 0.0f), "step track holds previous key");
    check(near(sd::sample_ambient_track(stepTrack, 100)[0], 0.5f), "step track switches on the key time");
    check(near(sd::sample_ambient_track(stepTrack, 999)[0], 1.0f), "hold after last key");
    sd::AmbientTrack linearTrack = stepTrack;
    linearTrack.linear = true;
    check(near(sd::sample_ambient_track(linearTrack, 50)[0], 0.25f), "linear track interpolates");
    check(near(sd::sample_ambient_track(linearTrack, 150)[0], 0.75f), "linear track second segment");
    check(sd::sample_ambient_track(sd::AmbientTrack{}, 10)[0] == 0.0f, "empty track is zero");

    // Real Swamp libraries.
    const auto torches = read_file(extra / "data/3d/animateddecors/swamp_tikitorches.bdae");
    if (torches.empty()) { std::cerr << "SKIP decor libraries missing\n"; }
    else {
        material_defaults(read_file(extra / "data/3d/animateddecors/swamp_cave_watercaustics.bdae"));
        water_texture_transform(extra);
        render_ownership(assets,torches);
        full_material_transport(extra);
        sd::AmbientDecorModel model;
        std::string error;
        check(sd::parse_ambient_model(torches, model, error), "parse torches");
        check(model.endMs == 1066, "torch clip length 1066 ms");
        const sd::AmbientMaterialClip* fire = nullptr;
        for (const auto& clip : model.clips) if (clip.materialId == "torch_fire") fire = &clip;
        check(fire && fire->offsetU >= 0 && fire->offsetV >= 0 && fire->color < 0, "torch_fire has offsetU/offsetV only");
        if (fire) {
            const auto u = sd::sample_ambient_track(model.tracks[std::size_t(fire->offsetU)], 100);
            const auto v = sd::sample_ambient_track(model.tracks[std::size_t(fire->offsetV)], 300);
            check(near(u[0], 0.375f), "native torch_fire U100ms=.375 (channel87)");
            check(near(v[0], 0.25f), "native torch_fire V300ms=.25 (channel88)");
        }
        const auto cave = read_file(extra / "data/3d/animateddecors/swamp_caveentrance_effect.bdae");
        sd::AmbientDecorModel caveModel;
        check(sd::parse_ambient_model(cave, caveModel, error), "parse cave entrance effect");
        bool colour = false;
        for (const auto& clip : caveModel.clips) colour = colour || clip.color >= 0;
        check(colour, "cave entrance glow animates the material colour (type 86)");
        for (const auto& track : caveModel.tracks) for (const auto& value : track.values) check(value[0] <= 1.0f && value[3] <= 1.0f, "colour keys normalised to 0..1");
        // Dust-mote library has no clips and no static geometry: parses to an empty model.
        sd::AmbientDecorModel dust;
        check(sd::parse_ambient_model(read_file(extra / "data/3d/animateddecors/swamp_rayolight_dust.bdae"), dust, error) && dust.clips.empty(), "rayolight dust has no material clips");
        check(!sd::parse_ambient_model(std::vector<std::uint8_t>{1, 2, 3}, dust, error), "garbage rejected");
    }

    // Level PlayerLight from LevelConfig (data/3D/Light/swamp.lightset_xml).
    f::AssetCatalog catalog(assets);
    ll::LevelPlayerLightV1 light;
    std::string error;
    check(ll::read_level_player_light_v1(catalog, "data/scene/001_swamp.mlx", false, light, error) && light.present, "swamp player light read");
    check(near(light.attachedOffset[2], 200.0f) && near(light.attenuation[0], 0.265625f) && near(light.attenuation[1], 0.0f)
          && std::fabs(light.attenuation[2] - 0.132813e-6f) < 1e-10f, "swamp attenuation after InitPost unit conversion");
    check(near(light.ambient[0], 1.0f) && near(light.diffuse[2], 1.0f), "swamp light colours 255 -> 1");
    check(light.hasClearColor && near(light.clearColor[0], 0.0f), "swamp clear colour black");
    ll::LevelPlayerLightV1 fixedSet;
    check(ll::read_level_player_light_v1(catalog, "data/scene/001_swamp.mlx", true, fixedSet, error) && fixedSet.present
          && near(fixedSet.attenuation[0], 0.15625f), "ES1.1 fixed light set parsed (different attenuation)");
    ll::LevelPlayerLightV1 none;
    check(!ll::read_level_player_light_v1(catalog, "data/scene/003_darkwood.mlx", false, none, error) && !none.present && !error.empty(),
          "light set file absent from the package -> error, no light (renderer keeps the preview lighting)");
    ll::LevelPlayerLightV1 missing;
    check(!ll::read_level_player_light_v1(catalog, "data/scene/does_not_exist.mlx", false, missing, error) && !missing.present, "missing level rejected");

    if (failures) { std::cerr << failures << " check(s) failed\n"; return 1; }
    std::cout << "PASS ambient decor sampler, library parser and level light reader\n";
    return 0;
}
