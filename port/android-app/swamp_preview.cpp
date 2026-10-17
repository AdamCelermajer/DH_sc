#include "scene_buffers.hpp"
#include "swamp_render_policy.hpp"
#include "../world-data/world.hpp"
#include "../world-data/world_scene.hpp"
#include "../scene-payloads/scene.hpp"
#include "../texture-assets/texture.hpp"
#include "../animation-pose/pose.hpp"
#include "../navigation/navigation.hpp"
#include "../swamp-movement/movement.hpp"

#include <android/log.h>
#include <GLES2/gl2.h>
#include <jni.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <time.h>

namespace {
constexpr const char* tag = "DH2Swamp";
dh2::viewer::SceneMesh mesh{};
dh2::viewer::SceneMesh player_mesh{};
float center[3]{};
float scene_scale = 1.0f;
std::uint8_t* pixels = nullptr;
int pixel_width = 0, pixel_height = 0;
std::uint8_t* alpha_pixels = nullptr;
int alpha_pixel_width = 0, alpha_pixel_height = 0;
std::uint8_t* player_pixels = nullptr;
int player_pixel_width = 0, player_pixel_height = 0;
std::uint8_t* player_bres_bytes = nullptr;
std::uint8_t* idle_bres_bytes = nullptr;
std::uint8_t* walk_bres_bytes = nullptr;
dh2::resources::BresView player_bres{};
dh2::pose::Clip idle_clip{};
dh2::pose::Clip walk_clip{};
dh2::navigation::Navigation navigation{};
float player_spawn[3]{};
float player_position[3]{};
float player_heading = 0.0f;
bool player_ready = false;
bool player_walking = false;
bool last_pose_walking = false;
bool player_draw_logged = false;
double idle_epoch = 0.0;
std::int32_t last_pose_time = -1;
GLuint* draw_textures = nullptr;
std::uint8_t* draw_alpha_maps = nullptr;
std::uint32_t draw_texture_count = 0;
GLuint program = 0, swamp_texture = 0, alpha_texture = 0, player_texture = 0;
GLint pos_loc = -1, uv_loc = -1, sampler_loc = -1, alpha_sampler_loc = -1,
      has_texture_loc = -1, has_alpha_map_loc = -1,
      character_sampler_loc = -1, use_character_texture_loc = -1,
      solid_color_loc = -1, aspect_loc = -1, camera_loc = -1, zoom_loc = -1;
int screen_width = 1, screen_height = 1;
constexpr float player_move_speed = 30.0f;
constexpr float view_zoom = 4.2f;
constexpr std::uint32_t player_path_mask = dh2::movement::baseline_object_path_mask;
pthread_mutex_t guard = PTHREAD_MUTEX_INITIALIZER;

constexpr char vs_source[] =
    "attribute vec3 aPosition;attribute vec2 aUv;varying vec2 vUv;"
    "uniform float uAspect;uniform vec3 uCamera;uniform float uZoom;"
    "void main(){vec3 d=aPosition-uCamera;"
    // A following isometric view projects source-world X and Y onto distinct
    // screen diagonals, while source Z remains screen-up.
    "float sx=(d.x-d.y)*0.70710678;"
    "float sy=(d.x+d.y)*0.35355339+d.z*0.9;"
    "float depth=(d.x+d.y)*0.025-d.z*0.01;"
    "gl_Position=vec4(sx*uZoom*uAspect,sy*uZoom,depth,1.0);vUv=aUv;}";
constexpr char fs_source[] =
    "precision mediump float;varying vec2 vUv;uniform sampler2D uTexture;"
    "uniform sampler2D uAlphaMap;uniform sampler2D uCharacterTexture;"
    "uniform float uUseCharacterTexture;uniform float uHasAlphaMap;"
    "uniform float uHasTexture;uniform vec4 uSolidColor;"
    "void main(){vec4 sampled=mix(texture2D(uTexture,vUv),"
    "texture2D(uCharacterTexture,vUv),uUseCharacterTexture);"
    "vec4 base=mix(uSolidColor,sampled,uHasTexture);"
    // AlphaMap is a per-material source sampler. This preview applies its
    // decoded alpha channel without changing diffuse RGB.
    "float mask=1.0;if(uHasAlphaMap>0.5){mask=texture2D(uAlphaMap,vUv).a;"
    "if(mask<=0.0)discard;}gl_FragColor=vec4(base.rgb,base.a*mask);}";
// Uniform is separate so placement is transparent and adjustable from data.
GLuint shader(GLenum type, const char* source) {
    GLuint s = glCreateShader(type); glShaderSource(s, 1, &source, nullptr); glCompileShader(s);
    GLint ok = GL_FALSE; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) { char log[768]{}; glGetShaderInfoLog(s, sizeof log, nullptr, log);
        __android_log_print(ANDROID_LOG_ERROR, tag, "shader compile: %s", log);
        glDeleteShader(s); return 0; }
    return s;
}
GLuint make_texture(const std::uint8_t* data, int width, int height) {
    GLuint id = 0; glGenTextures(1, &id); glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        __android_log_print(ANDROID_LOG_ERROR, tag, "glTexImage2D failed: 0x%x (%dx%d)", error, width, height);
    return id;
}
struct Bytes { std::uint8_t* data; std::size_t size; };
bool get_bytes(JNIEnv* env, jbyteArray array, Bytes& output) {
    if (!array) return false;
    const auto length = env->GetArrayLength(array);
    if (length <= 0 || length > 32 * 1024 * 1024) return false;
    output.data = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(length)));
    if (!output.data) return false;
    output.size = static_cast<std::size_t>(length);
    env->GetByteArrayRegion(array, 0, length, reinterpret_cast<jbyte*>(output.data));
    if (env->ExceptionCheck()) { std::free(output.data); output = {}; return false; }
    return true;
}
bool equal_ascii_case(const char* a, const char* b) {
    if (!a || !b) return false;
    while (*a && *b) {
        char x = *a++, y = *b++;
        if (x >= 'A' && x <= 'Z') x = static_cast<char>(x + ('a' - 'A'));
        if (y >= 'A' && y <= 'Z') y = static_cast<char>(y + ('a' - 'A'));
        if (x != y) return false;
    }
    return *a == *b;
}
bool is_diffuse_id(const char* id) {
    return equal_ascii_case(id, "Diffuse") || equal_ascii_case(id, "diffuse-sampler");
}
int diffuse_binding(const dh2::viewer::SceneDrawDescriptor& draw) {
    bool sampler = false;
    for (std::uint32_t n = 0; n < draw.texture_count; ++n) {
        const auto& ref = mesh.texture_references[draw.first_texture + n];
        if (!is_diffuse_id(ref.parameter_id)) continue;
        sampler = true;
        // The original BRES path is q:/data/iphone/3d/textures/... . Apply
        // only the source-documented device prefix alias before exact compare.
        const char* path = ref.source_path;
        constexpr char source_prefix[] = "q:/data/iphone/3d/";
        if (std::strncmp(path, source_prefix, sizeof(source_prefix) - 1) == 0)
            path += sizeof(source_prefix) - 1;
        if (ref.image_index < 0) continue;
        if (equal_ascii_case(path, "textures/env_swamp.tga")) return 1;
    }
    return sampler ? -1 : 0;
}
int alpha_map_binding(const dh2::viewer::SceneDrawDescriptor& draw) {
    for (std::uint32_t n = 0; n < draw.texture_count; ++n) {
        const auto& ref = mesh.texture_references[draw.first_texture + n];
        if (!equal_ascii_case(ref.parameter_id, "AlphaMap")) continue;
        if (ref.image_index < 0) continue;
        const char* path = ref.source_path;
        constexpr char source_prefix[] = "q:/data/iphone/3d/";
        if (std::strncmp(path, source_prefix, sizeof(source_prefix) - 1) == 0)
            path += sizeof(source_prefix) - 1;
        return equal_ascii_case(path, "textures/pvr2_env_swamp_alpha.tga")
            ? 1 : -1;
    }
    return 0;
}
void drop_mesh() {
    dh2_viewer_scene_mesh_free(&mesh); std::free(draw_textures);
    std::free(draw_alpha_maps);
    draw_textures = nullptr; draw_alpha_maps = nullptr; draw_texture_count = 0;
}
jstring result(JNIEnv* env, const char* message) { return env->NewStringUTF(message); }
double monotonic_seconds() {
    timespec t{};
    clock_gettime(CLOCK_MONOTONIC, &t);
    return double(t.tv_sec) + double(t.tv_nsec) / 1000000000.0;
}
void remove_animation_root_and_place(dh2::viewer::SceneMesh* pose,
                                    const dh2::pose::Clip* clip,
                                    std::int32_t milliseconds) {
    float root_origin[4]{}, root_position[4]{};
    if (pose && clip) {
        for (std::uint32_t i = 0; i < clip->count; ++i) {
            const auto& track = clip->tracks[i];
            const char* target = dh2_animation_target(&track);
            if (!target || std::strcmp(target, "Bip01-node") != 0 ||
                dh2_animation_type(&track, 0) != 1) continue;
            if (dh2_pose_sample(clip, i, clip->start, root_origin) == dh2::pose::Error::ok &&
                dh2_pose_sample(clip, i, milliseconds, root_position) == dh2::pose::Error::ok)
                break;
            std::memset(root_origin, 0, sizeof(root_origin));
            std::memset(root_position, 0, sizeof(root_position));
            break;
        }
    }
    const float yaw_cos = std::cos(player_heading);
    const float yaw_sin = std::sin(player_heading);
    for (std::uint32_t i = 0; pose && i < pose->vertex_count; ++i) {
        float* v = pose->vertices + std::size_t(i) * 5;
        // The animation's Bip01 translation drives actor motion in the source
        // engine. The preview controller owns world translation, so remove
        // that root delta before applying the controller position once.
        const float local_x = v[0] - (root_position[0] - root_origin[0]);
        const float local_y = v[1] - (root_position[1] - root_origin[1]);
        const float local_z = v[2] - (root_position[2] - root_origin[2]);
        const float rotated_x = yaw_cos * local_x - yaw_sin * local_y;
        const float rotated_y = yaw_sin * local_x + yaw_cos * local_y;
        v[0] = (player_position[0] + rotated_x - center[0]) * scene_scale;
        v[1] = (player_position[1] + rotated_y - center[1]) * scene_scale;
        v[2] = (player_position[2] + local_z - center[2]) * scene_scale;
    }
}

bool player_pose_at(const dh2::pose::Clip& clip, std::int32_t milliseconds) {
    if (!player_ready || !player_bres.bytes || !clip.count) return false;
    dh2::viewer::SceneMesh next{};
    if (dh2_world_scene_mesh_at(&next, &player_bres, &clip, milliseconds) !=
            dh2::viewer::SceneMeshError::ok || !next.skin_joints || !next.draw_commands) {
        dh2_viewer_scene_mesh_free(&next);
        return false;
    }
    remove_animation_root_and_place(&next, &clip, milliseconds);
    dh2_viewer_scene_mesh_free(&player_mesh);
    player_mesh = next;
    last_pose_time = milliseconds;
    last_pose_walking = player_walking;
    return true;
}
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_loadSwamp(JNIEnv* env, jclass,
        jbyteArray bres_array, jbyteArray mlx_array, jbyteArray texture_array,
        jbyteArray alpha_array, jbyteArray spawn_array, jbyteArray hero_array,
        jbyteArray hero_texture_array, jbyteArray idle_array, jbyteArray walk_array) {
    Bytes bres_bytes{}, mlx_bytes{}, texture_bytes{}, alpha_bytes{}, spawn_bytes{},
          hero_bytes{}, hero_texture_bytes{}, idle_bytes{}, walk_bytes{};
    Bytes* inputs[] = {&bres_bytes, &mlx_bytes, &texture_bytes, &alpha_bytes,
                       &spawn_bytes, &hero_bytes, &hero_texture_bytes, &idle_bytes,
                       &walk_bytes};
    jbyteArray arrays[] = {bres_array, mlx_array, texture_array, alpha_array,
                           spawn_array, hero_array, hero_texture_array, idle_array,
                           walk_array};
    bool read_ok = true;
    for (std::size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
        if (!get_bytes(env, arrays[i], *inputs[i])) { read_ok = false; break; }
    if (!read_ok) {
        for (auto* input : inputs) std::free(input->data);
        return result(env, "SWAMP/player source assets could not be read from the APK.");
    }
    const auto fail = [&](const char* message) -> jstring {
        for (auto* input : inputs) std::free(input->data);
        return result(env, message);
    };
    dh2::world::SourceLevel level{}; dh2::world::Diagnostic world_diag{};
    if (dh2_world_import_level(&level, "SWAMP", "data/scene/001_swamp.mlx",
            mlx_bytes.data, mlx_bytes.size, &world_diag) != dh2::world::Error::ok) {
        return fail(world_diag.message);
    }
    if (level.module_count == 0 || std::strcmp(level.modules[0].cache_dae,
                                               "data/3d/modules/swamp/swamp.bdae")) {
        dh2_world_free(&level); return fail("SWAMP module zero does not match the bundled BRES.");
    }
    if (dh2_world_import_module_objects(&level, 0, dh2::world::RecordKind::mgp,
            level.modules[0].cache_mgp, spawn_bytes.data, spawn_bytes.size, &world_diag)
            != dh2::world::Error::ok) {
        dh2_world_free(&level);
        return fail(world_diag.message[0] ? world_diag.message : "SWAMP module-zero MGP could not be imported.");
    }
    const dh2::world::Object* entry_point = nullptr;
    for (std::uint32_t i = 0; i < level.entity_count; ++i) {
        const auto& object = level.entities[i];
        const char* id = dh2_world_field(&object, "entrypointID");
        if (object.module_index == 0 && object.kind == dh2::world::RecordKind::mgp &&
            object.gametype && std::strcmp(object.gametype, "SpawnPoint") == 0 &&
            id && std::strcmp(id, "0") == 0) {
            if (entry_point) {
                dh2_world_free(&level);
                return fail("SWAMP module zero has multiple entrypointID 0 spawn points.");
            }
            entry_point = &object;
        }
    }
    if (!entry_point) {
        dh2_world_free(&level);
        return fail("SWAMP module-zero entrypointID 0 SpawnPoint was not found.");
    }
    dh2::resources::BresView bres{};
    dh2::scene_payload::Scene scene{};
    dh2::world::ModuleBinding binding{};
    std::uint32_t records[65536]{}; std::uint32_t record_count = 0;
    dh2::math::Matrix4f correction{};
    auto world_result = dh2_bres_open(&bres, bres_bytes.data, bres_bytes.size);
    if (world_result != dh2::resources::BresError::ok ||
        dh2_scene_open(&scene, &bres) != dh2::scene_payload::Error::ok ||
        dh2_world_bind_module(&binding, &level.modules[0], &scene, &world_diag) != dh2::world::Error::ok ||
        dh2_world_module_records(records, 65536, &record_count, &binding, &scene, &world_diag) != dh2::world::Error::ok ||
        dh2_world_placement_matrix(&correction, &binding, &world_diag) != dh2::world::Error::ok) {
        dh2_world_free(&level);
        return fail(world_diag.message[0] ? world_diag.message : "SWAMP BRES/module binding failed.");
    }
    dh2::viewer::SceneMesh candidate{};
    if (dh2_world_scene_mesh_nodes(&candidate, &bres, records, record_count, &correction)
            != dh2::viewer::SceneMeshError::ok) {
        dh2_world_free(&level); return fail("SWAMP module geometry could not be assembled.");
    }
    dh2::resources::BresView hero_image{}, idle_image{}, walk_image{};
    dh2::pose::Clip hero_idle{}, hero_walk{};
    dh2::viewer::SceneMesh posed_player{};
    if (dh2_bres_open(&hero_image, hero_bytes.data, hero_bytes.size) != dh2::resources::BresError::ok ||
        dh2_bres_open(&idle_image, idle_bytes.data, idle_bytes.size) != dh2::resources::BresError::ok ||
        dh2_bres_open(&walk_image, walk_bytes.data, walk_bytes.size) != dh2::resources::BresError::ok ||
        dh2_pose_clip_open(&hero_idle, &idle_image, 0) != dh2::pose::Error::ok ||
        dh2_pose_clip_open(&hero_walk, &walk_image, 0) != dh2::pose::Error::ok ||
        dh2_world_scene_mesh_at(&posed_player, &hero_image, &hero_idle, hero_idle.start) !=
            dh2::viewer::SceneMeshError::ok || !posed_player.skin_joints) {
        dh2_viewer_scene_mesh_free(&posed_player); dh2_viewer_scene_mesh_free(&candidate);
        dh2_world_free(&level);
        return fail("Bundled warrior BRES or source idle/walk clips could not produce a skinned pose.");
    }
    dh2::textures::TextureView hero_texture_view{};
    if (dh2_texture_open(&hero_texture_view, hero_texture_bytes.data, hero_texture_bytes.size) !=
            dh2::textures::Error::ok ||
        (hero_texture_view.format != dh2::textures::Format::pvrtc_2bpp &&
         hero_texture_view.format != dh2::textures::Format::pvrtc_4bpp) ||
        hero_texture_view.width > 4096 || hero_texture_view.height > 4096) {
        dh2_viewer_scene_mesh_free(&posed_player); dh2_viewer_scene_mesh_free(&candidate);
        dh2_world_free(&level);
        return fail("Bundled warrior diffuse is not a supported PVRTC texture.");
    }
    const std::size_t hero_rgba_size = std::size_t(hero_texture_view.width) * hero_texture_view.height * 4;
    auto* decoded_hero = static_cast<std::uint8_t*>(std::malloc(hero_rgba_size));
    if (!decoded_hero || dh2_texture_decode_rgba8(decoded_hero, hero_rgba_size,
            hero_texture_view.width * 4, hero_texture_bytes.data, hero_texture_bytes.size) !=
            dh2::textures::Error::ok) {
        std::free(decoded_hero); dh2_viewer_scene_mesh_free(&posed_player);
        dh2_viewer_scene_mesh_free(&candidate); dh2_world_free(&level);
        return fail("Bundled warrior diffuse could not be decoded.");
    }
    dh2::textures::TextureView texture_view{};
    if (dh2_texture_open(&texture_view, texture_bytes.data, texture_bytes.size) != dh2::textures::Error::ok ||
        (texture_view.format != dh2::textures::Format::pvrtc_2bpp &&
         texture_view.format != dh2::textures::Format::pvrtc_4bpp) ||
        texture_view.width > 4096 || texture_view.height > 4096) {
        std::free(decoded_hero); dh2_viewer_scene_mesh_free(&posed_player);
        dh2_viewer_scene_mesh_free(&candidate); dh2_world_free(&level);
        return fail("Bundled SWAMP diffuse is not a supported PVRTC texture.");
    }
    const std::size_t rgba_size = std::size_t(texture_view.width) * texture_view.height * 4;
    auto* decoded = static_cast<std::uint8_t*>(std::malloc(rgba_size));
    if (!decoded || dh2_texture_decode_rgba8(decoded, rgba_size, texture_view.width * 4,
            texture_bytes.data, texture_bytes.size) != dh2::textures::Error::ok) {
        std::free(decoded); std::free(decoded_hero);
        dh2_viewer_scene_mesh_free(&posed_player); dh2_viewer_scene_mesh_free(&candidate); dh2_world_free(&level);
        return fail("Bundled SWAMP diffuse could not be decoded.");
    }
    dh2::textures::TextureView alpha_view{};
    if (dh2_texture_open(&alpha_view, alpha_bytes.data, alpha_bytes.size) != dh2::textures::Error::ok ||
        alpha_view.format != dh2::textures::Format::pvrtc_2bpp ||
        alpha_view.alpha_mask != 1 ||
        alpha_view.width != texture_view.width || alpha_view.height != texture_view.height) {
        std::free(decoded); std::free(decoded_hero);
        dh2_viewer_scene_mesh_free(&posed_player); dh2_viewer_scene_mesh_free(&candidate); dh2_world_free(&level);
        return fail("Bundled SWAMP AlphaMap is not the expected PVRTC 2bpp alpha channel.");
    }
    const std::size_t alpha_rgba_size = std::size_t(alpha_view.width) * alpha_view.height * 4;
    auto* decoded_alpha = static_cast<std::uint8_t*>(std::malloc(alpha_rgba_size));
    if (!decoded_alpha || dh2_texture_decode_rgba8(decoded_alpha, alpha_rgba_size,
            alpha_view.width * 4, alpha_bytes.data, alpha_bytes.size) != dh2::textures::Error::ok) {
        std::free(decoded_alpha); std::free(decoded); std::free(decoded_hero);
        dh2_viewer_scene_mesh_free(&posed_player);
        dh2_viewer_scene_mesh_free(&candidate); dh2_world_free(&level);
        return fail("Bundled SWAMP AlphaMap could not be decoded.");
    }
    float minv[3] = {INFINITY, INFINITY, INFINITY}, maxv[3] = {-INFINITY, -INFINITY, -INFINITY};
    for (std::uint32_t i = 0; i < candidate.vertex_count; ++i)
        for (int axis = 0; axis < 3; ++axis) {
            const float value = candidate.vertices[5 * i + axis];
            if (value < minv[axis]) minv[axis] = value;
            if (value > maxv[axis]) maxv[axis] = value;
        }
    float span = 0;
    for (int axis = 0; axis < 3; ++axis) { center[axis] = (minv[axis] + maxv[axis]) * .5f;
        if (maxv[axis] - minv[axis] > span) span = maxv[axis] - minv[axis]; }
    scene_scale = span > 0 ? 2.2f / span : 1.0f;
    dh2::navigation::Navigation candidate_navigation{};
    dh2::navigation::Diagnostic nav_diag{};
    if (dh2_nav_build_swamp(&candidate_navigation, &level, &scene, &nav_diag) !=
            dh2::navigation::Error::ok) {
        dh2_viewer_scene_mesh_free(&posed_player);
        dh2_viewer_scene_mesh_free(&candidate);
        dh2_world_free(&level);
        return fail(nav_diag.message[0] ? nav_diag.message :
                    "SWAMP module-zero floor height data could not be built.");
    }
    // Center and scale the source world mesh. Keep this exact frame transform
    // so the SpawnPoint and warrior use the identical module-zero coordinates.
    for (std::uint32_t i = 0; i < candidate.vertex_count; ++i)
        for (int axis = 0; axis < 3; ++axis)
            candidate.vertices[5 * i + axis] =
                (candidate.vertices[5 * i + axis] - center[axis]) * scene_scale;
    for (int axis = 0; axis < 3; ++axis) player_spawn[axis] = entry_point->world_position[axis];
    dh2_world_free(&level);
    std::free(bres_bytes.data); bres_bytes = {};
    std::free(mlx_bytes.data); mlx_bytes = {};
    std::free(texture_bytes.data); texture_bytes = {};
    std::free(alpha_bytes.data); alpha_bytes = {};
    std::free(spawn_bytes.data); spawn_bytes = {};
    std::free(hero_texture_bytes.data); hero_texture_bytes = {};

    pthread_mutex_lock(&guard);
    dh2_nav_free(&navigation);
    navigation = candidate_navigation;
    candidate_navigation = {};
    player_ready = false;
    player_walking = false;
    drop_mesh();
    mesh = candidate;
    draw_textures = static_cast<GLuint*>(std::calloc(mesh.draw_commands, sizeof(GLuint)));
    draw_alpha_maps = static_cast<std::uint8_t*>(std::calloc(mesh.draw_commands, sizeof(std::uint8_t)));
    draw_texture_count = draw_textures && draw_alpha_maps ? mesh.draw_commands : 0;
    if (!draw_texture_count) {
        drop_mesh(); std::free(decoded); std::free(decoded_alpha); std::free(decoded_hero);
        dh2_viewer_scene_mesh_free(&posed_player);
        pthread_mutex_unlock(&guard);
        return result(env, "SWAMP material bindings could not be allocated.");
    }
    std::uint32_t diffuse_count = 0, alpha_count = 0;
    for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
        const int binding = diffuse_binding(mesh.draws[i]);
        const int alpha_binding = alpha_map_binding(mesh.draws[i]);
        if (binding < 0) {
            drop_mesh(); std::free(decoded);
            std::free(decoded_alpha);
            std::free(decoded_hero); dh2_viewer_scene_mesh_free(&posed_player);
            pthread_mutex_unlock(&guard);
            return result(env, "A SWAMP diffuse sampler could not be resolved to the bundled source texture.");
        }
        if (alpha_binding < 0) {
            drop_mesh(); std::free(decoded); std::free(decoded_alpha);
            std::free(decoded_hero); dh2_viewer_scene_mesh_free(&posed_player);
            pthread_mutex_unlock(&guard);
            return result(env, "A SWAMP AlphaMap sampler could not be resolved to the bundled source texture.");
        }
        draw_textures[i] = static_cast<GLuint>(binding);
        draw_alpha_maps[i] = static_cast<std::uint8_t>(alpha_binding);
        diffuse_count += binding ? 1U : 0U;
        alpha_count += alpha_binding ? 1U : 0U;
        if (!binding) {
            const auto& draw = mesh.draws[i];
            __android_log_print(ANDROID_LOG_INFO, tag,
                "solid fallback draw[%u] visible=%u node=%s material=%s name=%s geometry=%s samplers=%u",
                i, draw.visible, draw.node_id, draw.material_id, draw.material_name, draw.geometry_id,
                draw.texture_count);
        }
    }
    std::free(pixels); pixels = decoded;
    pixel_width = static_cast<int>(texture_view.width);
    pixel_height = static_cast<int>(texture_view.height);
    std::free(alpha_pixels); alpha_pixels = decoded_alpha;
    alpha_pixel_width = static_cast<int>(alpha_view.width);
    alpha_pixel_height = static_cast<int>(alpha_view.height);
    if (swamp_texture) glDeleteTextures(1, &swamp_texture);
    if (alpha_texture) glDeleteTextures(1, &alpha_texture);
    if (player_texture) glDeleteTextures(1, &player_texture);
    swamp_texture = make_texture(pixels, pixel_width, pixel_height);
    alpha_texture = make_texture(alpha_pixels, alpha_pixel_width, alpha_pixel_height);
    player_pixels = decoded_hero;
    player_pixel_width = static_cast<int>(hero_texture_view.width);
    player_pixel_height = static_cast<int>(hero_texture_view.height);
    player_texture = make_texture(player_pixels, player_pixel_width, player_pixel_height);
    dh2_viewer_scene_mesh_free(&player_mesh);
    std::free(player_bres_bytes); player_bres_bytes = hero_bytes.data;
    hero_bytes = {};
    std::free(idle_bres_bytes); idle_bres_bytes = idle_bytes.data;
    idle_bytes = {};
    std::free(walk_bres_bytes); walk_bres_bytes = walk_bytes.data;
    walk_bytes = {};
    player_bres = hero_image;
    idle_clip = hero_idle;
    walk_clip = hero_walk;
    for (int axis = 0; axis < 3; ++axis) player_position[axis] = player_spawn[axis];
    player_heading = 0.0f;
    player_walking = false;
    player_ready = true;
    remove_animation_root_and_place(&posed_player, &idle_clip, idle_clip.start);
    player_mesh = posed_player;
    posed_player = {};
    float actor_min[3] = {INFINITY, INFINITY, INFINITY};
    float actor_max[3] = {-INFINITY, -INFINITY, -INFINITY};
    for (std::uint32_t i = 0; i < player_mesh.vertex_count; ++i)
        for (int axis = 0; axis < 3; ++axis) {
            const float value = player_mesh.vertices[std::size_t(i) * 5 + axis];
            actor_min[axis] = std::min(actor_min[axis], value);
            actor_max[axis] = std::max(actor_max[axis], value);
        }
    __android_log_print(ANDROID_LOG_INFO, tag,
        "source player loaded: draws=%u vertices=%u indices=%u bones=%u texture=%dx%d, spawn=(%.3f,%.3f,%.3f), view-center=(%.3f,%.3f,%.3f) scale=%.8f, actor-bounds=(%.3f..%.3f,%.3f..%.3f,%.3f..%.3f), idle=%d..%d walk=%d..%d",
        player_mesh.draw_commands, player_mesh.vertex_count, player_mesh.index_count,
        player_mesh.skin_joints, player_pixel_width, player_pixel_height,
        player_spawn[0], player_spawn[1], player_spawn[2],
        center[0], center[1], center[2], scene_scale,
        actor_min[0], actor_max[0], actor_min[1], actor_max[1], actor_min[2], actor_max[2],
        hero_idle.start, hero_idle.end, hero_walk.start, hero_walk.end);
    idle_epoch = monotonic_seconds();
    last_pose_time = hero_idle.start;
    last_pose_walking = false;
    __android_log_print(ANDROID_LOG_INFO, tag, "module0: %u draws/%u verts, diffuse=%u, AlphaMap=%u, refs=%u",
        mesh.draw_commands, mesh.vertex_count, diffuse_count, alpha_count, mesh.texture_reference_count);
    std::uint32_t visible_diagnostic_draws = 0, omitted_diagnostic_draws = 0;
    for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
        const auto& draw = mesh.draws[i];
        if (!draw.visible) continue;
        if (dh2::viewer::omit_unresolved_swamp_draw(draw.node_id, draw.material_id))
            ++omitted_diagnostic_draws;
        else
            ++visible_diagnostic_draws;
    }
    char message[384]{};
    std::snprintf(message, sizeof message, "SWAMP module 0 · %u drawn diagnostics / %u source draws · %u unresolved omitted · %u vertices · %u diffuse · %u AlphaMap. Source entry (%.2f, %.3f, %.1f) · idle/walk clips ready. Player path mask 2; tagged module-zero floors available. Swept wall/actor collision is not implemented.",
        visible_diagnostic_draws, mesh.draw_commands, omitted_diagnostic_draws,
        mesh.vertex_count, diffuse_count, alpha_count,
        player_spawn[0], player_spawn[1], player_spawn[2]);
    pthread_mutex_unlock(&guard);
    return result(env, message);
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_stepPlayer(JNIEnv* env, jclass,
        jfloat stick_x, jfloat stick_y, jfloat dt_seconds) {
    char message[320]{};
    pthread_mutex_lock(&guard);
    if (!player_ready || !navigation.triangles || !navigation.surface_count) {
        std::snprintf(message, sizeof(message), "MOVEMENT UNAVAILABLE · SWAMP module-zero data is not ready.");
        pthread_mutex_unlock(&guard);
        return result(env, message);
    }

    dh2::movement::Input input{{player_position[0], player_position[1], player_position[2]},
        player_heading, stick_x, stick_y, dt_seconds};
    dh2::movement::Output output{};
    const auto step_error = dh2_swamp_movement_step(&navigation, &input,
        player_move_speed, &output);
    if (step_error == dh2::movement::Error::ok && output.floor_sample_valid) {
        for (int axis = 0; axis < 3; ++axis) player_position[axis] = output.position[axis];
        player_heading = output.heading_radians;
    }
    const bool walking = step_error == dh2::movement::Error::ok && output.moved;
    if (walking != player_walking) {
        player_walking = walking;
        idle_epoch = monotonic_seconds();
        last_pose_time = -1;
        __android_log_print(ANDROID_LOG_INFO, tag,
            "source movement animation=%s at (%.3f,%.3f,%.3f)",
            player_walking ? "walk" : "idle", player_position[0],
            player_position[1], player_position[2]);
    }

    dh2::navigation::FloorHit floor{};
    bool found = false;
    auto query_error = dh2_nav_query_actor_floor(&navigation, player_position[0],
        player_position[1], player_position[2],
        dh2::movement::native_floor_vertical_tolerance, dh2::movement::edge_tolerance,
        player_path_mask, &floor, &found);
    if (found && floor.vertical_distance >= dh2::movement::native_floor_vertical_tolerance)
        found = false;
    bool module_zero = false;
    dh2::navigation::Surface surface{};
    if (query_error == dh2::navigation::Error::ok && found) {
        module_zero = dh2_nav_surface(&navigation, floor.surface_index, &surface) ==
            dh2::navigation::Error::ok && surface.module_index == 0;
    }
    if (step_error == dh2::movement::Error::no_module_zero_floor) {
        std::snprintf(message, sizeof(message),
            "NO PATH-ELIGIBLE MODULE-0 FLOOR · candidate rejected; position held at X %.2f Y %.2f Z %.2f · player path mask 0x%X · wall/actor collision unavailable.",
            player_position[0], player_position[1], player_position[2], player_path_mask);
    } else if (step_error != dh2::movement::Error::ok) {
        std::snprintf(message, sizeof(message),
            "MOVEMENT INPUT REJECTED · source position held · player path mask 0x%X · no wall/actor collision.",
            player_path_mask);
    } else if (!module_zero) {
        std::snprintf(message, sizeof(message),
            "NO PATH-ELIGIBLE MODULE-0 FLOOR · position X %.2f Y %.2f Z %.2f · player path mask 0x%X.",
            player_position[0], player_position[1], player_position[2], player_path_mask);
    } else {
        const char* type_name = floor.floor_type_tag_present
            ? floor.floor_type_tag : surface.source_node_name;
        std::snprintf(message, sizeof(message),
            "SOURCE XYZ · X %.2f Y %.2f Z %.2f · yaw %.2f rad · %s · floor %.2f · %s flags=0x%08X · player mask=0x%X; wall/actor collision unavailable.",
            player_position[0], player_position[1], player_position[2], player_heading,
            player_walking ? "WALK" : "IDLE", floor.height,
            type_name ? type_name : "unknown", floor.floor_type_flags, player_path_mask);
    }
    pthread_mutex_unlock(&guard);
    return result(env, message);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_destroySwampPreview(JNIEnv*, jclass) {
    pthread_mutex_lock(&guard);
    drop_mesh();
    dh2_viewer_scene_mesh_free(&player_mesh);
    dh2_nav_free(&navigation);
    std::free(pixels); pixels = nullptr; pixel_width = pixel_height = 0;
    std::free(alpha_pixels); alpha_pixels = nullptr; alpha_pixel_width = alpha_pixel_height = 0;
    std::free(player_pixels); player_pixels = nullptr; player_pixel_width = player_pixel_height = 0;
    std::free(player_bres_bytes); player_bres_bytes = nullptr;
    std::free(idle_bres_bytes); idle_bres_bytes = nullptr;
    std::free(walk_bres_bytes); walk_bres_bytes = nullptr;
    player_bres = {}; idle_clip = {}; walk_clip = {};
    player_ready = player_walking = false;
    player_draw_logged = false; last_pose_time = -1;
    swamp_texture = alpha_texture = player_texture = 0;
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_surfaceCreated(JNIEnv*, jclass) {
    GLuint vs = shader(GL_VERTEX_SHADER, vs_source), fs = shader(GL_FRAGMENT_SHADER, fs_source);
    if (!vs || !fs) return;
    program = glCreateProgram(); glAttachShader(program, vs); glAttachShader(program, fs);
    glLinkProgram(program); glDeleteShader(vs); glDeleteShader(fs);
    GLint ok = GL_FALSE; glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) { char log[768]{}; glGetProgramInfoLog(program, sizeof log, nullptr, log);
        __android_log_print(ANDROID_LOG_ERROR, tag, "program link: %s", log); glDeleteProgram(program); program = 0; return; }
    pos_loc = glGetAttribLocation(program, "aPosition"); uv_loc = glGetAttribLocation(program, "aUv");
    sampler_loc = glGetUniformLocation(program, "uTexture"); aspect_loc = glGetUniformLocation(program, "uAspect");
    alpha_sampler_loc = glGetUniformLocation(program, "uAlphaMap");
    character_sampler_loc = glGetUniformLocation(program, "uCharacterTexture");
    use_character_texture_loc = glGetUniformLocation(program, "uUseCharacterTexture");
    has_texture_loc = glGetUniformLocation(program, "uHasTexture");
    has_alpha_map_loc = glGetUniformLocation(program, "uHasAlphaMap");
    solid_color_loc = glGetUniformLocation(program, "uSolidColor");
    camera_loc = glGetUniformLocation(program, "uCamera");
    zoom_loc = glGetUniformLocation(program, "uZoom");
    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    pthread_mutex_lock(&guard);
    swamp_texture = pixels ? make_texture(pixels, pixel_width, pixel_height) : 0;
    alpha_texture = alpha_pixels
        ? make_texture(alpha_pixels, alpha_pixel_width, alpha_pixel_height) : 0;
    player_texture = player_pixels
        ? make_texture(player_pixels, player_pixel_width, player_pixel_height) : 0;
    pthread_mutex_unlock(&guard);
    __android_log_print(ANDROID_LOG_INFO, tag, "GLES preview pipeline ready");
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_surfaceChanged(JNIEnv*, jclass, jint width, jint height) {
    screen_width = width > 0 ? width : 1; screen_height = height > 0 ? height : 1;
    glViewport(0, 0, screen_width, screen_height);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_SwampPreviewActivity_draw(JNIEnv*, jclass) {
    glClearColor(.025f,.04f,.065f,1); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!program) return;
    glUseProgram(program); glUniform1f(aspect_loc, static_cast<float>(screen_height)/screen_width);
    glUniform1i(sampler_loc, 0); glUniform1i(alpha_sampler_loc, 1);
    glUniform1i(character_sampler_loc, 2);
    glActiveTexture(GL_TEXTURE0);
    pthread_mutex_lock(&guard);
    glUniform3f(camera_loc,
        (player_position[0] - center[0]) * scene_scale,
        (player_position[1] - center[1]) * scene_scale,
        (player_position[2] - center[2]) * scene_scale);
    glUniform1f(zoom_loc, view_zoom);
    const dh2::pose::Clip& active_clip = player_walking ? walk_clip : idle_clip;
    if (player_ready && active_clip.count && active_clip.end > active_clip.start) {
        const auto duration = active_clip.end - active_clip.start;
        const auto elapsed = static_cast<std::int64_t>((monotonic_seconds() - idle_epoch) * 1000.0);
        const auto time = active_clip.start + static_cast<std::int32_t>(elapsed % duration);
        if ((time != last_pose_time || player_walking != last_pose_walking) &&
            !player_pose_at(active_clip, time))
            __android_log_print(ANDROID_LOG_ERROR, tag,
                "source %s pose update rejected at %d ms",
                player_walking ? "walk" : "idle", time);
    }
    if (mesh.vertices && mesh.draws && draw_textures && draw_alpha_maps &&
        draw_texture_count == mesh.draw_commands) {
        glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), mesh.vertices);
        glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), mesh.vertices + 3);
        glEnableVertexAttribArray(pos_loc); glEnableVertexAttribArray(uv_loc);
        for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
            const auto& d = mesh.draws[i];
            if (!d.visible) continue;
            if (dh2::viewer::omit_unresolved_swamp_draw(d.node_id, d.material_id)) continue;
            const bool textured = draw_textures[i] != 0;
            const bool alpha_mapped = draw_alpha_maps[i] != 0;
            const bool additive =
                dh2::viewer::swamp_material_uses_additive_one_one(d.material_id);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, alpha_texture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, swamp_texture);
            glUniform1f(has_texture_loc, textured ? 1.0f : 0.0f);
            glUniform1f(has_alpha_map_loc, alpha_mapped ? 1.0f : 0.0f);
            glUniform1f(use_character_texture_loc, 0.0f);
            glUniform4f(solid_color_loc, 0.31f, 0.39f, 0.29f, 0.82f);
            if (additive) glBlendFunc(GL_ONE, GL_ONE);
            else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(dh2::viewer::swamp_draw_writes_depth(d.material_id,
                                                              alpha_mapped)
                ? GL_TRUE : GL_FALSE);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(d.index_count), GL_UNSIGNED_SHORT,
                           mesh.indices + d.first_index);
            glDepthMask(GL_TRUE);
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisableVertexAttribArray(pos_loc); glDisableVertexAttribArray(uv_loc);
    }
    if (player_ready && player_mesh.vertices && player_mesh.draw_commands && player_texture) {
        // Keep depth testing against the module's opaque geometry. The
        // Alpha-mapped materials do not write depth, so they cannot hide the
        // source-pose overlay by themselves.
        glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, player_texture);
        glUniform1f(has_texture_loc, 1.0f);
        glUniform1f(has_alpha_map_loc, 0.0f);
        glUniform1f(use_character_texture_loc, 1.0f);
        glUniform4f(solid_color_loc, 1, 1, 1, 1);
        glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), player_mesh.vertices);
        glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), player_mesh.vertices + 3);
        glEnableVertexAttribArray(pos_loc); glEnableVertexAttribArray(uv_loc);
        for (std::uint32_t i = 0; i < player_mesh.draw_commands; ++i) {
            const auto& d = player_mesh.draws[i];
            // append_first_skin creates a synthetic draw for the controller
            // fallback; it has no scene visibility bit to inherit.
            if (!d.visible && !player_mesh.skin_joints) continue;
            if (!player_draw_logged) {
                __android_log_print(ANDROID_LOG_INFO, tag,
                    "source player draw: visible=%u skin=%u count=%u first=(%.5f,%.5f,%.5f) texture=%u sampler=%d flag=%d",
                    d.visible, player_mesh.skin_joints, d.index_count,
                    player_mesh.vertices[0], player_mesh.vertices[1], player_mesh.vertices[2],
                    player_texture, character_sampler_loc, use_character_texture_loc);
                player_draw_logged = true;
            }
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(d.index_count), GL_UNSIGNED_SHORT,
                           player_mesh.indices + d.first_index);
        }
        glDisableVertexAttribArray(pos_loc); glDisableVertexAttribArray(uv_loc);
    }
    pthread_mutex_unlock(&guard);
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        __android_log_print(ANDROID_LOG_ERROR, tag, "frame GL error: 0x%x", error);
}
