#include <GLES2/gl2.h>
#include <android/log.h>
#include <map>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <exception>
#include "blood_texture_image_v1.hpp"
#include "texture_driver_fields_v1.hpp"
#include "texture_mipmap_v1.hpp"
#include "texture_unbind_v1.hpp"
#include "original_cache_assets_v1.hpp"
void check(const char*){}
struct AdapterCompileV5 {
 struct EffectGpuTextureV4 {GLuint texture{};dh2::textures::BloodTextureImageOwnerV1 image;dh2::textures::TextureParameterOwnerV1& owner(){return image.parameters();}};
 std::map<std::string,std::shared_ptr<EffectGpuTextureV4>> effect_gpu_textures_v4;
 dh2::textures::TextureDriverOptionsOwnerV1 effect_driver_options_v4;
 #include "renderer_effect_texture_v5.inc"
};
