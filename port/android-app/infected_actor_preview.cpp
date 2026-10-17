#include "scene_buffers.hpp"
#include "../skin-payloads/skin.hpp"
#include "../texture-assets/texture.hpp"

#include <GLES2/gl2.h>
#include <android/log.h>
#include <jni.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <pthread.h>

namespace {
using dh2::viewer::SceneMesh;
constexpr const char* tag = "DH2InfectedActor";
constexpr unsigned frame_count = 20;
constexpr unsigned texture_count = 4;
constexpr std::size_t max_asset_bytes = 32U * 1024U * 1024U;
constexpr const char* texture_paths[texture_count] = {
    "data/3d/textures/atlas_dh2_game_objects_001.tga",
    "data/3d/textures/atlas_dh2_game_objects_001_alpha.tga",
    "data/3d/textures/atlas_dh2_game_objects_001_specular.tga",
    "data/3d/textures/atlas_skinned_characters_animdecor_gameobjects_002.tga"};
constexpr const char* controller_ids[6] = {
    "zombie02-mesh-skin", "_mesh_burned-mesh-skin", "blacksmith-mesh-skin",
    "_mesh_castle_worker02-mesh-skin", "_mesh_merchant_skinned_batch-mesh-skin",
    "_mesh_nun-mesh-skin"};

struct Texture {
    char path[256]{};
    std::uint8_t* pixels = nullptr;
    std::uint32_t width = 0, height = 0;
    GLuint gpu = 0;
};

pthread_mutex_t guard = PTHREAD_MUTEX_INITIALIZER;
SceneMesh frames[frame_count]{};
Texture textures[texture_count]{};
GLuint program = 0;
GLint position_attribute = -1, uv_attribute = -1;
GLint diffuse_sampler = -1, specular_sampler = -1, alpha_sampler = -1;
GLint has_diffuse_uniform = -1, has_specular_uniform = -1, has_alpha_uniform = -1;
GLint center_uniform = -1, scale_uniform = -1, yaw_uniform = -1;
GLint pitch_uniform = -1, zoom_uniform = -1, aspect_uniform = -1, color_uniform = -1;
int screen_width = 1, screen_height = 1, render_height = 1;
float center[3]{};
float scene_scale = 1.0f;
float camera_yaw = 0.0f, camera_pitch = 0.72f, camera_zoom = 1.0f;
std::int32_t clip_duration = 0;
double clip_started = 0;
unsigned visible_draws = 0;
int active_model = -1, active_clip = -1;
bool ready = false;

constexpr char vertex_shader[] =
    "attribute vec3 aPosition;attribute vec2 aUv;varying vec2 vUv;"
    "uniform vec3 uCenter;uniform float uScale;uniform float uYaw;"
    "uniform float uPitch;uniform float uZoom;uniform float uAspect;"
    "void main(){vec3 p=(aPosition-uCenter)*uScale;"
    "float cy=cos(uYaw),sy=sin(uYaw);float x=cy*p.x-sy*p.y;"
    "float y=sy*p.x+cy*p.y;float cp=cos(uPitch),sp=sin(uPitch);"
    "float screenY=y*sp+p.z*cp;float depth=y*cp-p.z*sp;"
    "gl_Position=vec4(x*uZoom*uAspect,screenY*uZoom,depth*0.01,1.0);vUv=aUv;}";
constexpr char fragment_shader[] =
    "precision mediump float;varying vec2 vUv;"
    "uniform sampler2D uDiffuse;uniform sampler2D uSpecular;uniform sampler2D uAlpha;"
    "uniform float uHasDiffuse;uniform float uHasSpecular;uniform float uHasAlpha;"
    "uniform vec4 uColor;void main(){"
    "vec4 base=mix(uColor,texture2D(uDiffuse,vUv),uHasDiffuse);"
    "float mask=1.0;if(uHasAlpha>0.5){mask=texture2D(uAlpha,vUv).a;"
    "if(mask<0.025)discard;}"
    "vec3 shine=texture2D(uSpecular,vUv).rgb;"
    "base.rgb=clamp(base.rgb*(1.0+0.08*uHasSpecular*shine)+"
    "0.025*uHasSpecular*shine,0.0,1.0);"
    "gl_FragColor=vec4(base.rgb,base.a*mask);}";

jstring make_string(JNIEnv* env, const char* message) {
    return env->NewStringUTF(message ? message : "Infected actor diagnostic preview failed.");
}

double monotonic_seconds() {
    timespec value{};
    clock_gettime(CLOCK_MONOTONIC, &value);
    return static_cast<double>(value.tv_sec) + static_cast<double>(value.tv_nsec) / 1.0e9;
}

GLuint compile_shader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    if (!shader) return 0;
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE) return shader;
    char log[768]{};
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    __android_log_print(ANDROID_LOG_ERROR, tag, "shader compile failed: %s", log);
    glDeleteShader(shader);
    return 0;
}

void forget_program() {
    program = 0;
    position_attribute = uv_attribute = -1;
    diffuse_sampler = specular_sampler = alpha_sampler = -1;
    has_diffuse_uniform = has_specular_uniform = has_alpha_uniform = -1;
    center_uniform = scale_uniform = yaw_uniform = -1;
    pitch_uniform = zoom_uniform = aspect_uniform = color_uniform = -1;
}

void clear_actor(bool delete_gl) {
    ready = false;
    for (auto& frame : frames) dh2_viewer_scene_mesh_free(&frame);
    for (auto& texture : textures) {
        if (delete_gl && texture.gpu) glDeleteTextures(1, &texture.gpu);
        std::free(texture.pixels);
        texture = {};
    }
    clip_duration = 0;
    clip_started = 0;
    visible_draws = 0;
    active_model = active_clip = -1;
}

bool copy_bytes(JNIEnv* env, jbyteArray input, std::uint8_t** output, std::size_t* size) {
    if (!env || !input || !output || !size) return false;
    const jsize length = env->GetArrayLength(input);
    if (length <= 0 || static_cast<std::size_t>(length) > max_asset_bytes) return false;
    auto* bytes = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(length)));
    if (!bytes) return false;
    env->GetByteArrayRegion(input, 0, length, reinterpret_cast<jbyte*>(bytes));
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        std::free(bytes);
        return false;
    }
    *output = bytes;
    *size = static_cast<std::size_t>(length);
    return true;
}

bool decode_texture(Texture* output, const char* path,
                    const std::uint8_t* bytes, std::size_t size) {
    if (!output || !path || !bytes || !size) return false;
    bool allowed = false;
    for (const auto* expected : texture_paths) allowed |= std::strcmp(path, expected) == 0;
    if (!allowed) return false;
    dh2::textures::TextureView view{};
    if (dh2_texture_open(&view, bytes, size) != dh2::textures::Error::ok ||
        !view.width || !view.height || view.width > 4096 || view.height > 4096 ||
        std::uint64_t(view.width) * view.height > 16U * 1024U * 1024U) return false;
    const auto bytes_count = std::size_t(view.width) * view.height * 4U;
    auto* rgba = static_cast<std::uint8_t*>(std::malloc(bytes_count));
    if (!rgba) return false;
    if (view.format == dh2::textures::Format::pvrtc_2bpp ||
        view.format == dh2::textures::Format::pvrtc_4bpp) {
        if (dh2_texture_decode_rgba8(rgba, bytes_count,
                std::size_t(view.width) * 4U, bytes, size) != dh2::textures::Error::ok) {
            std::free(rgba);
            return false;
        }
    } else if (view.format == dh2::textures::Format::bgra8 && view.payload &&
               view.payload_size == bytes_count) {
        for (std::size_t i = 0; i < bytes_count; i += 4) {
            rgba[i] = view.payload[i + 2];
            rgba[i + 1] = view.payload[i + 1];
            rgba[i + 2] = view.payload[i];
            rgba[i + 3] = view.payload[i + 3];
        }
    } else {
        std::free(rgba);
        return false;
    }
    if (std::strlen(path) >= sizeof(output->path)) { std::free(rgba); return false; }
    std::strcpy(output->path, path);
    output->pixels = rgba;
    output->width = view.width;
    output->height = view.height;
    return true;
}

bool upload_texture(Texture* texture) {
    if (!texture || !texture->pixels) return false;
    glGenTextures(1, &texture->gpu);
    if (!texture->gpu) return false;
    glBindTexture(GL_TEXTURE_2D, texture->gpu);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
    return glGetError() == GL_NO_ERROR;
}

void normalize_source_path(const char* input, char* output, std::size_t capacity) {
    if (!output || !capacity) return;
    output[0] = '\0';
    if (!input) return;
    constexpr char device_prefix[] = "q:/data/iphone/";
    constexpr char cache_prefix[] = "data/";
    if (std::strncmp(input, device_prefix, sizeof(device_prefix) - 1) == 0)
        std::snprintf(output, capacity, "data/%s", input + sizeof(device_prefix) - 1);
    else if (std::strncmp(input, cache_prefix, sizeof(cache_prefix) - 1) == 0)
        std::snprintf(output, capacity, "%s", input);
}

bool equal_ascii_folded(const char* left, const char* right) {
    if (!left || !right) return false;
    while (*left && *right) {
        char a = *left++, b = *right++;
        if (a >= 'A' && a <= 'Z') a = static_cast<char>(a + ('a' - 'A'));
        if (b >= 'A' && b <= 'Z') b = static_cast<char>(b + ('a' - 'A'));
        if (a != b) return false;
    }
    return *left == *right;
}

GLuint bound_texture(const SceneMesh& mesh,
                     const dh2::viewer::SceneDrawDescriptor& draw,
                     const char* parameter) {
    if (draw.first_texture > mesh.texture_reference_count ||
        draw.texture_count > mesh.texture_reference_count - draw.first_texture) return 0;
    for (std::uint32_t i = 0; i < draw.texture_count; ++i) {
        const auto& reference = mesh.texture_references[draw.first_texture + i];
        const bool direct_match = equal_ascii_folded(reference.parameter_id, parameter);
        // The source BDAE uses the stable `diffuse-sampler` material parameter
        // name for skinned characters; the renderer-facing semantic is Diffuse.
        const bool source_diffuse_alias = equal_ascii_folded(parameter, "Diffuse") &&
            equal_ascii_folded(reference.parameter_id, "diffuse-sampler");
        if (reference.image_index < 0 || (!direct_match && !source_diffuse_alias)) continue;
        char path[256]{};
        normalize_source_path(reference.source_path, path, sizeof(path));
        for (const auto& texture : textures)
            if (texture.gpu && std::strcmp(texture.path, path) == 0) return texture.gpu;
    }
    return 0;
}

bool texture_closure_complete(const SceneMesh& mesh) {
    for (std::uint32_t command = 0; command < mesh.draw_commands; ++command) {
        const auto& draw = mesh.draws[command];
        if (draw.first_texture > mesh.texture_reference_count ||
            draw.texture_count > mesh.texture_reference_count - draw.first_texture) return false;
        for (std::uint32_t i = 0; i < draw.texture_count; ++i) {
            const auto& reference = mesh.texture_references[draw.first_texture + i];
            if (reference.image_index < 0) continue;
            char path[256]{};
            normalize_source_path(reference.source_path, path, sizeof(path));
            bool found = false;
            for (const auto* expected : texture_paths) found |= std::strcmp(path, expected) == 0;
            if (!found) return false;
        }
    }
    return true;
}

void render_mesh(const SceneMesh& mesh) {
    if (!mesh.vertices || !mesh.indices || !mesh.draws || !mesh.draw_commands) return;
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), mesh.vertices);
    glVertexAttribPointer(uv_attribute, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), mesh.vertices + 3);
    glEnableVertexAttribArray(position_attribute);
    glEnableVertexAttribArray(uv_attribute);
    for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
        const auto& draw = mesh.draws[i];
        if (!draw.visible || draw.first_index > mesh.index_count ||
            draw.index_count > mesh.index_count - draw.first_index) continue;
        const GLuint diffuse = bound_texture(mesh, draw, "Diffuse");
        const GLuint specular = bound_texture(mesh, draw, "Specular");
        const GLuint alpha = bound_texture(mesh, draw, "AlphaMap");
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, diffuse);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, specular);
        glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, alpha);
        glUniform1f(has_diffuse_uniform, diffuse ? 1.0f : 0.0f);
        glUniform1f(has_specular_uniform, specular ? 1.0f : 0.0f);
        glUniform1f(has_alpha_uniform, alpha ? 1.0f : 0.0f);
        glUniform4f(color_uniform, 0.47f, 0.46f, 0.39f, 1.0f);
        glDepthMask(alpha ? GL_FALSE : GL_TRUE);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(draw.index_count),
                       GL_UNSIGNED_SHORT, mesh.indices + draw.first_index);
        if (alpha) glDepthMask(GL_TRUE);
    }
    glDepthMask(GL_TRUE);
    glDisableVertexAttribArray(position_attribute);
    glDisableVertexAttribArray(uv_attribute);
}

bool establish_bounds() {
    float minimum[3] = {INFINITY, INFINITY, INFINITY};
    float maximum[3] = {-INFINITY, -INFINITY, -INFINITY};
    visible_draws = 0;
    for (std::uint32_t frame = 0; frame < frame_count; ++frame) {
        const auto& mesh = frames[frame];
        for (std::uint32_t command = 0; command < mesh.draw_commands; ++command) {
            const auto& draw = mesh.draws[command];
            if (!draw.visible || draw.first_vertex > mesh.vertex_count ||
                draw.vertex_count > mesh.vertex_count - draw.first_vertex) continue;
            if (frame == 0) ++visible_draws;
            for (std::uint32_t i = 0; i < draw.vertex_count; ++i) {
                for (unsigned axis = 0; axis < 3; ++axis) {
                    const float value = mesh.vertices[std::size_t(draw.first_vertex + i) * 5 + axis];
                    minimum[axis] = std::min(minimum[axis], value);
                    maximum[axis] = std::max(maximum[axis], value);
                }
            }
        }
    }
    if (!visible_draws) return false;
    float extent = 0;
    for (unsigned axis = 0; axis < 3; ++axis) {
        center[axis] = (minimum[axis] + maximum[axis]) * .5f;
        extent = std::max(extent, maximum[axis] - minimum[axis]);
    }
    if (!std::isfinite(extent) || extent <= 0 || maximum[2] <= minimum[2]) return false;
    scene_scale = 1.2f / extent;
    return std::isfinite(scene_scale);
}

} // namespace

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_InfectedActorPreviewActivity_loadActor(
        JNIEnv* env, jclass, jbyteArray model_array, jbyteArray clip_array,
        jint model_index, jint clip_index, jobjectArray texture_path_array,
        jobjectArray texture_byte_arrays) {
    pthread_mutex_lock(&guard);
    clear_actor(true);
    auto fail = [env](const char* text) { return make_string(env, text); };
    if (!program) { pthread_mutex_unlock(&guard); return fail("GLES actor diagnostic renderer is unavailable."); }
    if (model_index < 0 || model_index >= 6 || clip_index < 0 || clip_index >= 3) {
        pthread_mutex_unlock(&guard); return fail("The selected source model or clip index is outside the pinned bundle.");
    }
    std::uint8_t* model_bytes = nullptr;
    std::uint8_t* clip_bytes = nullptr;
    std::size_t model_size = 0, clip_size = 0;
    if (!copy_bytes(env, model_array, &model_bytes, &model_size) ||
        !copy_bytes(env, clip_array, &clip_bytes, &clip_size)) {
        std::free(model_bytes); std::free(clip_bytes);
        pthread_mutex_unlock(&guard); return fail("A pinned actor model or animation file is missing or too large.");
    }
    dh2::resources::BresView model{}, animation{};
    dh2::skin::Skin skin{};
    dh2::pose::Clip clip{};
    const auto model_open = dh2_bres_open(&model, model_bytes, model_size);
    const auto animation_open = dh2_bres_open(&animation, clip_bytes, clip_size);
    const auto controller_count = model_open == dh2::resources::BresError::ok ?
        dh2_bres_library_count(&model, dh2::resources::Library::controller) : 0;
    const auto skin_open = model_open == dh2::resources::BresError::ok ?
        dh2_skin_open(&skin, &model, 0) : dh2::skin::Error::argument;
    const auto clip_open = animation_open == dh2::resources::BresError::ok ?
        dh2_pose_clip_open(&clip, &animation, 0) : dh2::pose::Error::argument;
    bool valid = model_open == dh2::resources::BresError::ok &&
        animation_open == dh2::resources::BresError::ok && controller_count == 1 &&
        skin_open == dh2::skin::Error::ok &&
        skin.id && std::strcmp(skin.id, controller_ids[model_index]) == 0 && skin.joints == 20 &&
        clip_open == dh2::pose::Error::ok && clip.end > clip.start;
    char failure[192] = "asset verification failed";
    if (!valid) {
        std::snprintf(failure, sizeof(failure),
            "preflight model=%u animation=%u controllers=%u skin=%u joints=%u clip=%u interval=%d..%d",
            static_cast<unsigned>(model_open), static_cast<unsigned>(animation_open),
            static_cast<unsigned>(controller_count), static_cast<unsigned>(skin_open),
            skin.joints, static_cast<unsigned>(clip_open), clip.start, clip.end);
    }
    const jsize path_count = texture_path_array ? env->GetArrayLength(texture_path_array) : 0;
    const jsize data_count = texture_byte_arrays ? env->GetArrayLength(texture_byte_arrays) : 0;
    valid = valid && path_count == static_cast<jsize>(texture_count) &&
        data_count == static_cast<jsize>(texture_count);
    for (unsigned i = 0; valid && i < texture_count; ++i) {
        auto path_string = static_cast<jstring>(env->GetObjectArrayElement(texture_path_array, i));
        auto byte_array = static_cast<jbyteArray>(env->GetObjectArrayElement(texture_byte_arrays, i));
        const char* path = path_string ? env->GetStringUTFChars(path_string, nullptr) : nullptr;
        std::uint8_t* bytes = nullptr;
        std::size_t size = 0;
        const bool path_ok = path && std::strcmp(path, texture_paths[i]) == 0;
        const bool bytes_ok = path_ok && copy_bytes(env, byte_array, &bytes, &size);
        const bool decoded = bytes_ok && decode_texture(&textures[i], path, bytes, size);
        const bool uploaded = decoded && upload_texture(&textures[i]);
        valid = path_ok && bytes_ok && decoded && uploaded;
        if (!valid) {
            std::snprintf(failure, sizeof(failure),
                "texture %u path=%d bytes=%d decode=%d upload=%d size=%u width=%u",
                i, path_ok, bytes_ok, decoded, uploaded,
                size > UINT32_MAX ? 0U : static_cast<unsigned>(size),
                bytes_ok ? static_cast<unsigned>(textures[i].width) : 0U);
        }
        if (bytes) std::free(bytes);
        if (path) env->ReleaseStringUTFChars(path_string, path);
        if (byte_array) env->DeleteLocalRef(byte_array);
        if (path_string) env->DeleteLocalRef(path_string);
    }
    if (valid) {
        for (unsigned i = 0; i < frame_count; ++i) {
            const auto time = clip.start + static_cast<std::int32_t>(
                std::int64_t(clip.end - clip.start - 1) * i / frame_count);
            const auto mesh_error = dh2_world_scene_skin_mesh_at(
                &frames[i], &model, &clip, time, controller_ids[model_index]);
            const bool mesh_shape_ok = frames[i].draw_commands && frames[i].skin_joints;
            const bool closure_ok = mesh_error == dh2::viewer::SceneMeshError::ok &&
                mesh_shape_ok && texture_closure_complete(frames[i]);
            if (mesh_error != dh2::viewer::SceneMeshError::ok || !mesh_shape_ok || !closure_ok) {
                std::snprintf(failure, sizeof(failure),
                    "mesh sample %u error=%u draws=%u skin_joints=%u textures=%u",
                    i, static_cast<unsigned>(mesh_error), frames[i].draw_commands,
                    frames[i].skin_joints, closure_ok);
                valid = false; break;
            }
        }
    }
    if (valid && !establish_bounds()) {
        std::snprintf(failure, sizeof(failure), "render bounds are empty or invalid");
        valid = false;
    }
    std::free(model_bytes);
    std::free(clip_bytes);
    if (!valid) {
        clear_actor(true);
        pthread_mutex_unlock(&guard);
        char message[256]{};
        std::snprintf(message, sizeof(message), "Actor preview validation failed: %s", failure);
        __android_log_print(ANDROID_LOG_ERROR, tag, "%s", message);
        return fail(message);
    }
    active_model = model_index;
    active_clip = clip_index;
    clip_duration = clip.end - clip.start;
    clip_started = monotonic_seconds();
    ready = true;
    char report[512]{};
    std::snprintf(report, sizeof(report),
        "Loaded model %d / clip %d · %u source draw records · %u sampled frames · %d ms. Diagnostic render only; no game AI, animation state machine, collision, activation, spawning, or playable level.",
        model_index + 1, clip_index + 1, visible_draws, frame_count, clip_duration);
    pthread_mutex_unlock(&guard);
    return make_string(env, report);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedActorPreviewActivity_setView(
        JNIEnv*, jclass, jfloat yaw, jfloat pitch, jfloat zoom) {
    pthread_mutex_lock(&guard);
    camera_yaw = yaw;
    camera_pitch = std::max(.25f, std::min(1.35f, pitch));
    camera_zoom = std::max(.5f, std::min(3.5f, zoom));
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedActorPreviewActivity_surfaceCreated(JNIEnv*, jclass) {
    pthread_mutex_lock(&guard);
    clear_actor(false);
    forget_program();
    while (glGetError() != GL_NO_ERROR) {}
    const GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    const GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
    if (vs && fs) {
        program = glCreateProgram();
        glAttachShader(program, vs); glAttachShader(program, fs); glLinkProgram(program);
        GLint linked = GL_FALSE; glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked != GL_TRUE) {
            char log[768]{}; glGetProgramInfoLog(program, sizeof(log), nullptr, log);
            __android_log_print(ANDROID_LOG_ERROR, tag, "program link failed: %s", log);
            glDeleteProgram(program); program = 0;
        }
    }
    if (vs) glDeleteShader(vs);
    if (fs) glDeleteShader(fs);
    if (program) {
        position_attribute = glGetAttribLocation(program, "aPosition");
        uv_attribute = glGetAttribLocation(program, "aUv");
        diffuse_sampler = glGetUniformLocation(program, "uDiffuse");
        specular_sampler = glGetUniformLocation(program, "uSpecular");
        alpha_sampler = glGetUniformLocation(program, "uAlpha");
        has_diffuse_uniform = glGetUniformLocation(program, "uHasDiffuse");
        has_specular_uniform = glGetUniformLocation(program, "uHasSpecular");
        has_alpha_uniform = glGetUniformLocation(program, "uHasAlpha");
        color_uniform = glGetUniformLocation(program, "uColor");
        center_uniform = glGetUniformLocation(program, "uCenter");
        scale_uniform = glGetUniformLocation(program, "uScale");
        yaw_uniform = glGetUniformLocation(program, "uYaw");
        pitch_uniform = glGetUniformLocation(program, "uPitch");
        zoom_uniform = glGetUniformLocation(program, "uZoom");
        aspect_uniform = glGetUniformLocation(program, "uAspect");
    }
    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedActorPreviewActivity_surfaceChanged(
        JNIEnv*, jclass, jint width, jint height) {
    screen_width = width > 0 ? width : 1;
    screen_height = height > 0 ? height : 1;
    render_height = std::max(1, std::min(screen_height,
        static_cast<int>(std::lround(screen_height * 0.60f))));
    glViewport(0, 0, screen_width, render_height);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedActorPreviewActivity_draw(JNIEnv*, jclass) {
    glClearColor(.025f, .038f, .052f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pthread_mutex_lock(&guard);
    if (program && ready) {
        const double clip_ms = std::fmod(
            std::max(0.0, monotonic_seconds() - clip_started) * 1000.0,
            static_cast<double>(clip_duration));
        const auto frame = std::min(frame_count - 1, static_cast<unsigned>(
            clip_ms * frame_count / clip_duration));
        glUseProgram(program);
        glUniform1i(diffuse_sampler, 0); glUniform1i(specular_sampler, 1);
        glUniform1i(alpha_sampler, 2);
        glUniform3f(center_uniform, center[0], center[1], center[2]);
        glUniform1f(scale_uniform, scene_scale); glUniform1f(yaw_uniform, camera_yaw);
        glUniform1f(pitch_uniform, camera_pitch); glUniform1f(zoom_uniform, camera_zoom);
        glUniform1f(aspect_uniform, static_cast<float>(render_height) / screen_width);
        render_mesh(frames[frame]);
    }
    pthread_mutex_unlock(&guard);
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        __android_log_print(ANDROID_LOG_ERROR, tag, "frame GLES error: 0x%x", error);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedActorPreviewActivity_destroyActorPreview(JNIEnv*, jclass) {
    pthread_mutex_lock(&guard);
    clear_actor(false);
    forget_program();
    pthread_mutex_unlock(&guard);
}
