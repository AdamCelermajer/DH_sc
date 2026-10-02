#include "../engine-resources/resources.hpp"
#include "../texture-assets/texture.hpp"
#include "scene_buffers.hpp"

#include <GLES2/gl2.h>
#include <android/log.h>
#include <jni.h>
#include <pthread.h>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <new>

namespace {
pthread_mutex_t guard = PTHREAD_MUTEX_INITIALIZER;
float* vertices = nullptr; // normalized world x, y, z, u, v per vertex
std::uint16_t* indices = nullptr;
std::uint32_t vertex_count = 0, index_count = 0;
bool character_z_up = false;
std::uint8_t* model_bytes = nullptr;
std::uint8_t* animation_bytes = nullptr;
dh2::resources::BresView model_image{};
dh2::pose::Clip animation_clip{};
bool animation_playing = false;
double animation_start = 0;
std::uint8_t* rgba = nullptr;
int texture_width = 0, texture_height = 0;
bool texture_dirty = false;
GLuint program = 0, texture = 0;
GLint position_loc = -1, uv_loc = -1, sampler_loc = -1, has_texture_loc = -1;
GLint aspect_loc = -1, yaw_loc = -1, pitch_loc = -1, zoom_loc = -1;
GLint z_up_loc = -1;
int screen_width = 1, screen_height = 1;
float yaw = 0.6f, pitch = 0.9f, zoom = 1.0f;

constexpr char vertex_shader[] =
    "attribute vec3 aPosition; attribute vec2 aUv; varying vec2 vUv;"
    "uniform float uAspect; uniform float uYaw; uniform float uPitch; uniform float uZoom; uniform float uZUp;"
    "void main(){"
    "float cy=cos(uYaw),sy=sin(uYaw),cp=cos(uPitch),sp=sin(uPitch);"
    "vec3 p=mix(aPosition,vec3(aPosition.x,aPosition.z,-aPosition.y),uZUp);"
    "vec3 turned=vec3(cy*p.x+sy*p.z,p.y,-sy*p.x+cy*p.z);"
    "vec3 viewed=vec3(turned.x,cp*turned.y-sp*turned.z,"
    "sp*turned.y+cp*turned.z);"
    "gl_Position=vec4(viewed.x*uAspect*uZoom,viewed.y*uZoom,"
    "-viewed.z*0.5,1.0);vUv=aUv;}";
constexpr char fragment_shader[] =
    "precision mediump float; varying vec2 vUv; uniform sampler2D uTexture;"
    "uniform float uHasTexture; void main(){"
    "vec4 c=texture2D(uTexture,vUv);"
    "gl_FragColor=mix(vec4(0.95,0.48,0.19,1.0),c,uHasTexture);}";

GLuint compile(GLenum kind, const char* source) {
    GLuint shader = glCreateShader(kind);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512]{};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        __android_log_print(ANDROID_LOG_ERROR, "DH2Source", "shader: %s", log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

jstring message(JNIEnv* env, const char* value) { return env->NewStringUTF(value); }

double monotonic_time() {
    timespec t{}; clock_gettime(CLOCK_MONOTONIC, &t);
    return double(t.tv_sec) + double(t.tv_nsec) / 1000000000.0;
}

bool update_pose_locked(std::int32_t time) {
    if (!model_bytes || !animation_clip.count) return false;
    dh2::viewer::SceneMesh frame{};
    if (dh2_viewer_scene_mesh_at(&frame, &model_image, &animation_clip, time)
        != dh2::viewer::SceneMeshError::ok) return false;
    std::free(vertices); std::free(indices);
    vertices = frame.vertices; indices = frame.indices;
    vertex_count = frame.vertex_count; index_count = frame.index_count;
    return true;
}

std::uint8_t* input_copy(JNIEnv* env, jbyteArray source, std::size_t& size) {
    size = source ? static_cast<std::size_t>(env->GetArrayLength(source)) : 0;
    if (!size || size > 32U * 1024U * 1024U) return nullptr;
    auto* copy = static_cast<std::uint8_t*>(std::malloc(size));
    if (copy) env->GetByteArrayRegion(source, 0, static_cast<jsize>(size),
                                      reinterpret_cast<jbyte*>(copy));
    return copy;
}
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_loadBres(JNIEnv* env, jclass, jbyteArray source) {
    std::size_t size = 0;
    auto* bytes = input_copy(env, source, size);
    if (!bytes) return message(env, "BRES rejected: empty, too large, or out of memory");
    dh2::resources::BresView image{};
    if (dh2_bres_open(&image, bytes, size) != dh2::resources::BresError::ok) {
        std::free(bytes);
        return message(env, "BRES rejected: invalid container");
    }
    dh2::viewer::SceneMesh scene_mesh{};
    const auto result = dh2_viewer_scene_mesh(&scene_mesh, &image);
    if (result != dh2::viewer::SceneMeshError::ok) {
        std::free(bytes);
        return message(env, "BRES scene rejected: unsupported, too large, or malformed static draw data");
    }
    pthread_mutex_lock(&guard);
    std::free(model_bytes); model_bytes = bytes; model_image = image;
    std::free(animation_bytes); animation_bytes = nullptr;
    animation_clip = {}; animation_playing = false;
    std::free(vertices); std::free(indices);
    vertices = scene_mesh.vertices; indices = scene_mesh.indices;
    vertex_count = scene_mesh.vertex_count; index_count = scene_mesh.index_count;
    character_z_up = scene_mesh.skin_joints != 0;
    const bool already_textured = rgba != nullptr;
    pthread_mutex_unlock(&guard);
    char status[224]{};
    std::snprintf(status, sizeof(status),
                  "%s: %u draws, %u vertices, %u indices, %u bones. First diffuse: %s. %s",
                  scene_mesh.skin_joints ? "Character pose" : "Static scene",
                  scene_mesh.draw_commands, scene_mesh.vertex_count, scene_mesh.index_count, scene_mesh.skin_joints,
                  scene_mesh.first_diffuse_texture[0]
                      ? scene_mesh.first_diffuse_texture : "unresolved",
                  already_textured ? "Rendering with imported texture." : "Import that texture to render it.");
    return message(env, status);
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_loadAnimation(JNIEnv* env, jclass, jbyteArray source) {
    std::size_t size = 0; auto* bytes = input_copy(env, source, size);
    if (!bytes) return message(env, "Animation rejected: empty, too large, or out of memory");
    auto* candidate = static_cast<dh2::pose::Clip*>(std::malloc(sizeof(dh2::pose::Clip)));
    if (!candidate) { std::free(bytes); return message(env, "Animation rejected: out of memory"); }
    new (candidate) dh2::pose::Clip{};
    dh2::resources::BresView image{};
    if (dh2_bres_open(&image, bytes, size) != dh2::resources::BresError::ok ||
        dh2_pose_clip_open(candidate, &image, 0) != dh2::pose::Error::ok ||
        candidate->end <= candidate->start) {
        std::free(candidate); std::free(bytes);
        return message(env, "Animation rejected: requires an unscaled float position/rotation/scale clip");
    }
    pthread_mutex_lock(&guard);
    dh2::viewer::SceneMesh test{};
    const bool valid = character_z_up && model_bytes &&
        dh2_viewer_scene_mesh_at(&test, &model_image, candidate, candidate->start)
            == dh2::viewer::SceneMeshError::ok;
    if (!valid) {
        pthread_mutex_unlock(&guard); std::free(candidate); std::free(bytes);
        return message(env, "Animation rejected: import a matching skinned character first");
    }
    std::free(animation_bytes); animation_bytes = bytes;
    animation_clip = *candidate; animation_playing = false;
    std::free(vertices); std::free(indices);
    vertices = test.vertices; indices = test.indices;
    vertex_count = test.vertex_count; index_count = test.index_count;
    const auto tracks = animation_clip.count;
    const auto duration = animation_clip.end - animation_clip.start;
    pthread_mutex_unlock(&guard); std::free(candidate);
    char status[160]{};
    std::snprintf(status, sizeof(status), "Animation preview: %u tracks, %d ms. Play or seek. Absolute-key preview; no gameplay.", tracks, duration);
    return message(env, status);
}

extern "C" JNIEXPORT jint JNICALL
Java_local_dh2_sourceviewer_MainActivity_animationDuration(JNIEnv*, jclass) {
    pthread_mutex_lock(&guard);
    const auto duration = animation_clip.count ? animation_clip.end - animation_clip.start : 0;
    pthread_mutex_unlock(&guard); return duration;
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_MainActivity_seekAnimation(JNIEnv*, jclass, jint milliseconds) {
    pthread_mutex_lock(&guard); animation_playing = false;
    if (animation_clip.count) {
        const auto duration = animation_clip.end - animation_clip.start;
        const auto time = milliseconds < 0 ? 0 : milliseconds > duration ? duration : milliseconds;
        if (!update_pose_locked(time + animation_clip.start))
            __android_log_print(ANDROID_LOG_ERROR, "DH2Source", "animation pose rejected");
    }
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_MainActivity_playAnimation(JNIEnv*, jclass, jboolean playing) {
    pthread_mutex_lock(&guard);
    animation_playing = playing && animation_clip.count;
    animation_start = monotonic_time();
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_loadTexture(JNIEnv* env, jclass, jbyteArray source) {
    std::size_t size = 0;
    auto* bytes = input_copy(env, source, size);
    if (!bytes) return message(env, "Texture rejected: empty, too large, or out of memory");
    dh2::textures::TextureView view{};
    bool ok = dh2_texture_open(&view, bytes, size) == dh2::textures::Error::ok
        && view.width <= 4096 && view.height <= 4096
        && (view.format == dh2::textures::Format::pvrtc_2bpp
            || view.format == dh2::textures::Format::pvrtc_4bpp);
    std::uint8_t* decoded = nullptr;
    if (ok) {
        const std::size_t length = std::size_t(view.width) * view.height * 4U;
        decoded = static_cast<std::uint8_t*>(std::malloc(length));
        ok = decoded && dh2_texture_decode_rgba8(decoded, length, view.width * 4U,
                                                    bytes, size) == dh2::textures::Error::ok;
    }
    std::free(bytes);
    if (!ok) {
        std::free(decoded);
        return message(env, "Texture rejected: expected supported BTEX/PVRTC1 data");
    }
    pthread_mutex_lock(&guard);
    std::free(rgba);
    rgba = decoded; texture_width = static_cast<int>(view.width);
    texture_height = static_cast<int>(view.height); texture_dirty = true;
    const bool mesh_loaded = vertices != nullptr;
    pthread_mutex_unlock(&guard);
    char result[128]{};
    std::snprintf(result, sizeof(result), "PVRTC texture decoded: %u x %u. %s",
                  view.width, view.height,
                  mesh_loaded ? "Rendering imported BRES scene." : "Import a matching BRES scene to display it.");
    return message(env, result);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_MainActivity_setView(JNIEnv*, jclass,
                                                 jfloat next_yaw, jfloat next_pitch,
                                                 jfloat next_zoom) {
    if (!std::isfinite(next_yaw) || !std::isfinite(next_pitch) ||
        !std::isfinite(next_zoom)) return;
    pthread_mutex_lock(&guard);
    yaw = next_yaw;
    pitch = next_pitch < 0.15f ? 0.15f : next_pitch > 1.5f ? 1.5f : next_pitch;
    zoom = next_zoom < 0.4f ? 0.4f : next_zoom > 4.0f ? 4.0f : next_zoom;
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_MainActivity_surfaceCreated(JNIEnv*, jclass) {
    GLuint vs = compile(GL_VERTEX_SHADER, vertex_shader);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fragment_shader);
    if (!vs || !fs) return;
    program = glCreateProgram();
    glAttachShader(program, vs); glAttachShader(program, fs); glLinkProgram(program);
    glDeleteShader(vs); glDeleteShader(fs);
    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) { glDeleteProgram(program); program = 0; return; }
    position_loc = glGetAttribLocation(program, "aPosition");
    uv_loc = glGetAttribLocation(program, "aUv");
    sampler_loc = glGetUniformLocation(program, "uTexture");
    has_texture_loc = glGetUniformLocation(program, "uHasTexture");
    aspect_loc = glGetUniformLocation(program, "uAspect");
    yaw_loc = glGetUniformLocation(program, "uYaw");
    pitch_loc = glGetUniformLocation(program, "uPitch");
    zoom_loc = glGetUniformLocation(program, "uZoom");
    z_up_loc = glGetUniformLocation(program, "uZUp");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    pthread_mutex_lock(&guard);
    texture_dirty = true;
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_MainActivity_surfaceChanged(JNIEnv*, jclass, jint width, jint height) {
    screen_width = width > 0 ? width : 1;
    screen_height = height > 0 ? height : 1;
    glViewport(0, 0, screen_width, screen_height);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_MainActivity_draw(JNIEnv*, jclass) {
    glClearColor(0.035f, 0.055f, 0.085f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!program) return;
    static const float placeholder[] = {
        -0.5f,-0.5f,0,0,1,  0.5f,-0.5f,0,1,1,
         0.5f, 0.5f,0,1,0, -0.5f, 0.5f,0,0,0
    };
    static const std::uint16_t placeholder_indices[] = {0,1,2,2,3,0};
    pthread_mutex_lock(&guard);
    if (animation_playing && animation_clip.count) {
        const auto duration = animation_clip.end - animation_clip.start;
        const auto elapsed = std::fmod((monotonic_time() - animation_start) * 1000.0, double(duration));
        if (!update_pose_locked(static_cast<std::int32_t>(elapsed) + animation_clip.start)) {
            animation_playing = false;
            __android_log_print(ANDROID_LOG_ERROR, "DH2Source", "animated frame rejected");
        }
    }
    if (texture_dirty && rgba) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
        texture_dirty = false;
    }
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(sampler_loc, 0);
    glUniform1f(has_texture_loc, rgba ? 1.0f : 0.0f);
    glUniform1f(aspect_loc, static_cast<float>(screen_height) / screen_width);
    glUniform1f(yaw_loc, yaw);
    glUniform1f(pitch_loc, pitch);
    glUniform1f(z_up_loc, character_z_up ? 1.0f : 0.0f);
    const float aspect_fit = screen_width < screen_height
        ? static_cast<float>(screen_width) / screen_height : 1.0f;
    glUniform1f(zoom_loc, 1.2f * aspect_fit * zoom);
    const float* points = vertices ? vertices : placeholder;
    const std::uint16_t* faces = indices ? indices : placeholder_indices;
    const auto count = indices ? index_count : 6U;
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), points);
    glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), points + 3);
    glEnableVertexAttribArray(position_loc); glEnableVertexAttribArray(uv_loc);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_SHORT, faces);
    glDisableVertexAttribArray(position_loc); glDisableVertexAttribArray(uv_loc);
    pthread_mutex_unlock(&guard);
}
