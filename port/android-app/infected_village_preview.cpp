#include "infected_village_scene.hpp"
#include "infected_village_render_policy.hpp"
#include "../texture-assets/texture.hpp"

#include <GLES2/gl2.h>
#include <android/log.h>
#include <jni.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>

namespace {
constexpr const char* tag = "DH2InfectedVillage";
constexpr std::uint32_t max_textures = 8;
constexpr std::size_t max_asset_bytes = 32U * 1024U * 1024U;
constexpr const char* allowed_texture_paths[] = {
    "data/3d/textures/env_infectedvillage.tga",
    "data/3d/textures/env_infectedvillage_spec.tga",
    "data/3d/textures/pvr2_env_infectedvillage_alpha.tga"};

struct Texture {
    char path[256];
    std::uint8_t* pixels;
    std::uint32_t width, height;
    GLuint gpu;
};

dh2::infectedpreview::Preview preview{};
Texture textures[max_textures]{};
std::uint32_t texture_count = 0;
GLuint program = 0;
GLint position_attribute = -1, uv_attribute = -1;
GLint diffuse_sampler = -1, specular_sampler = -1, alpha_sampler = -1;
GLint has_diffuse_uniform = -1, has_specular_uniform = -1, has_alpha_uniform = -1;
GLint center_uniform = -1, scale_uniform = -1, yaw_uniform = -1;
GLint pitch_uniform = -1, zoom_uniform = -1, aspect_uniform = -1;
GLint color_uniform = -1;
int screen_width = 1, screen_height = 1;
float center[3]{};
float scene_scale = 1.0f;
float camera_yaw = 0.0f, camera_pitch = 0.72f, camera_zoom = 1.5f;
bool scene_ready = false;
pthread_mutex_t guard = PTHREAD_MUTEX_INITIALIZER;

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
    // A restrained static tint uses the checked specular image without
    // pretending to reproduce the unavailable original effect/pass shader.
    "vec3 shine=texture2D(uSpecular,vUv).rgb;"
    "base.rgb=clamp(base.rgb*(1.0+0.08*uHasSpecular*shine)+"
    "0.025*uHasSpecular*shine,0.0,1.0);"
    "gl_FragColor=vec4(base.rgb,base.a*mask);}";

void free_textures() {
    for (std::uint32_t i = 0; i < texture_count; ++i) {
        if (textures[i].gpu) glDeleteTextures(1, &textures[i].gpu);
        std::free(textures[i].pixels);
        textures[i] = {};
    }
    texture_count = 0;
}

void clear_scene() {
    free_textures();
    dh2::infectedpreview::free(&preview);
    scene_ready = false;
}

// Drop CPU-owned scene state without issuing GL deletes. This is required
// after Activity teardown or EGL context loss because old GLuints belong to
// a context that may no longer be current. The Activity retains its checked
// APK bytes and reloads/reuploads them after surfaceCreated.
void discard_scene_without_gl() {
    for (std::uint32_t i = 0; i < texture_count; ++i) {
        std::free(textures[i].pixels);
        textures[i] = {};
    }
    texture_count = 0;
    dh2::infectedpreview::free(&preview);
    scene_ready = false;
}

void forget_gl_program() {
    program = 0;
    position_attribute = uv_attribute = -1;
    diffuse_sampler = specular_sampler = alpha_sampler = -1;
    has_diffuse_uniform = has_specular_uniform = has_alpha_uniform = -1;
    center_uniform = scale_uniform = yaw_uniform = -1;
    pitch_uniform = zoom_uniform = aspect_uniform = color_uniform = -1;
}

jstring make_string(JNIEnv* env, const char* text) {
    return env->NewStringUTF(text ? text : "Static source preview failed.");
}

GLuint compile_shader(GLenum kind, const char* source) {
    GLuint shader = glCreateShader(kind);
    if (!shader) return 0;
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char log[768]{};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        __android_log_print(ANDROID_LOG_ERROR, tag, "shader compile failed: %s", log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool copy_java_bytes(JNIEnv* env, jbyteArray array, std::uint8_t** data,
                     std::size_t* size) {
    if (!array || !data || !size) return false;
    const jsize length = env->GetArrayLength(array);
    if (length <= 0 || static_cast<std::size_t>(length) > max_asset_bytes) return false;
    auto* copy = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(length)));
    if (!copy) return false;
    env->GetByteArrayRegion(array, 0, length, reinterpret_cast<jbyte*>(copy));
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        std::free(copy);
        return false;
    }
    *data = copy;
    *size = static_cast<std::size_t>(length);
    return true;
}

void free_bytes(std::uint8_t** data, std::size_t count) {
    if (!data) return;
    for (std::size_t i = 0; i < count; ++i) {
        std::free(data[i]);
        data[i] = nullptr;
    }
}

bool allowed_texture(const char* path) {
    for (const auto* allowed : allowed_texture_paths)
        if (path && std::strcmp(path, allowed) == 0) return true;
    return false;
}

bool decode_texture(Texture* target, const char* path,
                    const std::uint8_t* bytes, std::size_t size) {
    if (!target || !path || !bytes || !size || !allowed_texture(path)) return false;
    dh2::textures::TextureView view{};
    if (dh2_texture_open(&view, bytes, size) != dh2::textures::Error::ok ||
        !view.width || !view.height ||
        std::uint64_t(view.width) * view.height > 16U * 1024U * 1024U)
        return false;
    const std::size_t rgba_size = std::size_t(view.width) * view.height * 4U;
    auto* pixels = static_cast<std::uint8_t*>(std::malloc(rgba_size));
    if (!pixels) return false;
    if (view.format == dh2::textures::Format::pvrtc_2bpp ||
        view.format == dh2::textures::Format::pvrtc_4bpp) {
        if (dh2_texture_decode_rgba8(pixels, rgba_size,
                std::size_t(view.width) * 4U, bytes, size) != dh2::textures::Error::ok) {
            std::free(pixels);
            return false;
        }
    } else if (view.format == dh2::textures::Format::bgra8 &&
               view.payload && view.payload_size == rgba_size) {
        for (std::size_t i = 0; i < rgba_size; i += 4) {
            pixels[i] = view.payload[i + 2];
            pixels[i + 1] = view.payload[i + 1];
            pixels[i + 2] = view.payload[i];
            pixels[i + 3] = view.payload[i + 3];
        }
    } else {
        std::free(pixels);
        return false;
    }
    std::snprintf(target->path, sizeof(target->path), "%s", path);
    target->pixels = pixels;
    target->width = view.width;
    target->height = view.height;
    return true;
}

GLuint upload_texture(Texture* texture) {
    if (!texture || !texture->pixels) return 0;
    GLuint id = 0;
    glGenTextures(1, &id);
    if (!id) {
        __android_log_print(ANDROID_LOG_ERROR, tag,
            "texture allocation failed for %s", texture->path);
        return 0;
    }
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        __android_log_print(ANDROID_LOG_ERROR, tag,
            "texture upload failed for %s: GLES error 0x%x", texture->path, error);
        glDeleteTextures(1, &id);
        return 0;
    }
    return id;
}

void normalize_source_path(const char* input, char* output, std::size_t capacity) {
    if (!output || !capacity) return;
    output[0] = '\0';
    if (!input) return;
    constexpr char device_prefix[] = "q:/data/iphone/";
    constexpr char cache_prefix[] = "data/";
    if (std::strncmp(input, device_prefix, sizeof(device_prefix) - 1) == 0) {
        std::snprintf(output, capacity, "data/%s", input + sizeof(device_prefix) - 1);
    } else if (std::strncmp(input, cache_prefix, sizeof(cache_prefix) - 1) == 0) {
        std::snprintf(output, capacity, "%s", input);
    }
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

GLuint bound_texture(const dh2::viewer::SceneMesh& mesh,
                     const dh2::viewer::SceneDrawDescriptor& draw,
                     const char* parameter) {
    if (draw.first_texture > mesh.texture_reference_count ||
        draw.texture_count > mesh.texture_reference_count - draw.first_texture) return 0;
    for (std::uint32_t i = 0; i < draw.texture_count; ++i) {
        const auto& reference = mesh.texture_references[draw.first_texture + i];
        if (reference.image_index < 0 ||
            !equal_ascii_folded(reference.parameter_id, parameter)) continue;
        char source_path[256]{};
        normalize_source_path(reference.source_path, source_path, sizeof(source_path));
        for (std::uint32_t texture = 0; texture < texture_count; ++texture)
            if (std::strcmp(source_path, textures[texture].path) == 0)
                return textures[texture].gpu;
    }
    return 0;
}

void establish_bounds() {
    float minimum[3] = {INFINITY, INFINITY, INFINITY};
    float maximum[3] = {-INFINITY, -INFINITY, -INFINITY};
    for (const auto& mesh : preview.modules) {
        for (std::uint32_t command = 0; command < mesh.draw_commands; ++command) {
            const auto& draw = mesh.draws[command];
            if (!draw.visible || dh2::infectedpreview::omit_diagnostic_draw(
                    draw.node_id, draw.material_id) ||
                draw.first_vertex > mesh.vertex_count ||
                draw.vertex_count > mesh.vertex_count - draw.first_vertex) continue;
            for (std::uint32_t i = 0; i < draw.vertex_count; ++i) {
                for (unsigned axis = 0; axis < 3; ++axis) {
                    const float value = mesh.vertices[
                        std::size_t(draw.first_vertex + i) * 5 + axis];
                    minimum[axis] = std::min(minimum[axis], value);
                    maximum[axis] = std::max(maximum[axis], value);
                }
            }
        }
    }
    float extent = 0.0f;
    for (unsigned axis = 0; axis < 3; ++axis) {
        center[axis] = (minimum[axis] + maximum[axis]) * 0.5f;
        extent = std::max(extent, maximum[axis] - minimum[axis]);
    }
    scene_scale = extent > 0.0f && std::isfinite(extent) ? 1.7f / extent : 1.0f;
}

void render_mesh(const dh2::viewer::SceneMesh& mesh) {
    if (!mesh.vertices || !mesh.indices || !mesh.draws || !mesh.draw_commands) return;
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), mesh.vertices);
    glVertexAttribPointer(uv_attribute, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), mesh.vertices + 3);
    glEnableVertexAttribArray(position_attribute);
    glEnableVertexAttribArray(uv_attribute);
    for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
        const auto& draw = mesh.draws[i];
        if (!draw.visible || dh2::infectedpreview::omit_diagnostic_draw(
                draw.node_id, draw.material_id) ||
            draw.first_index > mesh.index_count ||
            draw.index_count > mesh.index_count - draw.first_index) continue;
        const GLuint diffuse = bound_texture(mesh, draw, "Diffuse");
        const GLuint specular = bound_texture(mesh, draw, "Specular");
        const GLuint alpha = bound_texture(mesh, draw, "AlphaMap");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, alpha);
        glUniform1f(has_diffuse_uniform, diffuse ? 1.0f : 0.0f);
        glUniform1f(has_specular_uniform, specular ? 1.0f : 0.0f);
        glUniform1f(has_alpha_uniform, alpha ? 1.0f : 0.0f);
        glUniform4f(color_uniform, 0.38f, 0.42f, 0.34f, 1.0f);
        glDepthMask(alpha ? GL_FALSE : GL_TRUE);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(draw.index_count),
                       GL_UNSIGNED_SHORT, mesh.indices + draw.first_index);
        if (alpha) glDepthMask(GL_TRUE);
    }
    glDisableVertexAttribArray(position_attribute);
    glDisableVertexAttribArray(uv_attribute);
}

} // namespace

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_InfectedVillagePreviewActivity_loadInfectedVillage(
        JNIEnv* env, jclass, jbyteArray mlx_array, jbyteArray catalogue_array,
        jbyteArray mgp0_array, jbyteArray mvp0_array,
        jbyteArray mgp1_array, jbyteArray mvp1_array,
        jobjectArray texture_paths, jobjectArray texture_data) {
    pthread_mutex_lock(&guard);
    clear_scene();
    if (!program) {
        pthread_mutex_unlock(&guard);
        return make_string(env,
            "GLES preview pipeline is unavailable: shader compilation or program linking failed; no source geometry was loaded.");
    }
    std::uint8_t* inputs[6]{};
    std::size_t input_sizes[6]{};
    jbyteArray arrays[6] = {mlx_array, catalogue_array, mgp0_array,
                            mvp0_array, mgp1_array, mvp1_array};
    bool ok = true;
    for (unsigned i = 0; i < 6; ++i)
        if (!copy_java_bytes(env, arrays[i], &inputs[i], &input_sizes[i])) { ok = false; break; }
    if (!ok) {
        free_bytes(inputs, 6);
        pthread_mutex_unlock(&guard);
        return make_string(env, "Required packaged MLX, BDAE, MGP, or MVP bytes could not be read.");
    }
    dh2::infectedpreview::SourceFiles source{
        inputs[0], input_sizes[0], inputs[1], input_sizes[1],
        {inputs[2], inputs[4]}, {input_sizes[2], input_sizes[4]},
        {inputs[3], inputs[5]}, {input_sizes[3], input_sizes[5]}};
    dh2::infectedpreview::Diagnostic diagnostic{};
    const auto load_status = dh2::infectedpreview::load(&preview, &source, &diagnostic);
    free_bytes(inputs, 6);
    if (load_status != dh2::infectedpreview::Error::ok) {
        pthread_mutex_unlock(&guard);
        return make_string(env, diagnostic.message);
    }

    const jsize path_count = texture_paths ? env->GetArrayLength(texture_paths) : 0;
    const jsize data_count = texture_data ? env->GetArrayLength(texture_data) : 0;
    if (path_count != data_count || path_count <= 0 ||
        path_count > static_cast<jsize>(max_textures)) {
        clear_scene();
        pthread_mutex_unlock(&guard);
        return make_string(env, "Manifest-listed module texture arrays are incomplete.");
    }
    bool seen[3]{};
    for (jsize i = 0; i < path_count; ++i) {
        auto path_string = static_cast<jstring>(env->GetObjectArrayElement(texture_paths, i));
        auto data_array = static_cast<jbyteArray>(env->GetObjectArrayElement(texture_data, i));
        if (!path_string || !data_array) { ok = false; break; }
        const char* path = env->GetStringUTFChars(path_string, nullptr);
        std::uint8_t* encoded = nullptr;
        std::size_t encoded_size = 0;
        ok = path && allowed_texture(path) && copy_java_bytes(env, data_array, &encoded, &encoded_size);
        bool accepted = false;
        if (ok) {
            for (unsigned slot = 0; slot < 3; ++slot)
                if (std::strcmp(path, allowed_texture_paths[slot]) == 0 && !seen[slot]) {
                    accepted = decode_texture(&textures[texture_count], path, encoded, encoded_size);
                    if (accepted) { seen[slot] = true; ++texture_count; }
                    break;
                }
        }
        std::free(encoded);
        if (path) env->ReleaseStringUTFChars(path_string, path);
        env->DeleteLocalRef(path_string);
        env->DeleteLocalRef(data_array);
        if (!accepted) { ok = false; break; }
    }
    if (env->ExceptionCheck()) { env->ExceptionClear(); ok = false; }
    for (bool present : seen) ok = ok && present;
    if (!ok) {
        clear_scene();
        pthread_mutex_unlock(&guard);
        return make_string(env, "Only the three manifest-verified module sampler textures are accepted; decode failed or a path was unresolved.");
    }
    for (std::uint32_t i = 0; i < texture_count; ++i) {
        textures[i].gpu = upload_texture(&textures[i]);
        if (!textures[i].gpu) {
            clear_scene();
            pthread_mutex_unlock(&guard);
            return make_string(env, "A checked Infected Village sampler texture could not be uploaded to GLES.");
        }
    }
    establish_bounds();
    scene_ready = true;
    char report[512]{};
    std::uint32_t draw_count = 0, omitted_root_guides = 0, omitted_floor_fallbacks = 0;
    for (const auto& mesh : preview.modules)
        for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
            if (dh2::infectedpreview::omit_unresolved_root_guide(
                    mesh.draws[i].node_id, mesh.draws[i].material_id)) ++omitted_root_guides;
            else if (dh2::infectedpreview::omit_unresolved_floor_fallback(
                    mesh.draws[i].node_id, mesh.draws[i].material_id)) ++omitted_floor_fallbacks;
            else if (mesh.draws[i].visible) ++draw_count;
        }
    std::snprintf(report, sizeof(report),
        "Loaded 2 authored module roots · 50 MGP/MVP records · %u/%u source draws shown · %u root ColorMaterial guides omitted and %u Standard_8 floor draws omitted as untextured preview fallback (native render visibility unresolved) · 3 checked sampler textures. Drag to orbit, pinch to zoom.",
        draw_count, preview.modules[0].draw_commands + preview.modules[1].draw_commands,
        omitted_root_guides, omitted_floor_fallbacks);
    pthread_mutex_unlock(&guard);
    return make_string(env, report);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedVillagePreviewActivity_setView(
        JNIEnv*, jclass, jfloat yaw, jfloat pitch, jfloat zoom) {
    pthread_mutex_lock(&guard);
    camera_yaw = yaw;
    camera_pitch = std::max(0.25f, std::min(1.35f, pitch));
    camera_zoom = std::max(0.55f, std::min(4.0f, zoom));
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedVillagePreviewActivity_surfaceCreated(JNIEnv*, jclass) {
    pthread_mutex_lock(&guard);
    // surfaceCreated is running in a new/current EGL context. Static GLuint
    // values from a prior activity/context name unrelated objects here; even
    // deleting them can leave GL_INVALID_VALUE queued for the first upload.
    discard_scene_without_gl();
    forget_gl_program();
    while (glGetError() != GL_NO_ERROR) {}
    const GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    const GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
    if (vs && fs) {
        program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        GLint linked = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (linked != GL_TRUE) {
            char log[768]{};
            glGetProgramInfoLog(program, sizeof(log), nullptr, log);
            __android_log_print(ANDROID_LOG_ERROR, tag, "program link failed: %s", log);
            glDeleteProgram(program);
            program = 0;
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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    pthread_mutex_unlock(&guard);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedVillagePreviewActivity_surfaceChanged(
        JNIEnv*, jclass, jint width, jint height) {
    screen_width = width > 0 ? width : 1;
    screen_height = height > 0 ? height : 1;
    glViewport(0, 0, screen_width, screen_height);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedVillagePreviewActivity_draw(JNIEnv*, jclass) {
    glClearColor(0.025f, 0.038f, 0.052f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pthread_mutex_lock(&guard);
    if (program && scene_ready) {
        glUseProgram(program);
        glUniform1i(diffuse_sampler, 0);
        glUniform1i(specular_sampler, 1);
        glUniform1i(alpha_sampler, 2);
        glUniform3f(center_uniform, center[0], center[1], center[2]);
        glUniform1f(scale_uniform, scene_scale);
        glUniform1f(yaw_uniform, camera_yaw);
        glUniform1f(pitch_uniform, camera_pitch);
        glUniform1f(zoom_uniform, camera_zoom);
        glUniform1f(aspect_uniform, static_cast<float>(screen_height) / screen_width);
        render_mesh(preview.modules[0]);
        render_mesh(preview.modules[1]);
    }
    pthread_mutex_unlock(&guard);
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        __android_log_print(ANDROID_LOG_ERROR, tag, "frame GLES error: 0x%x", error);
}

extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_InfectedVillagePreviewActivity_destroyPreview(JNIEnv*, jclass) {
    pthread_mutex_lock(&guard);
    // Activity destruction follows GLSurfaceView.onPause(); leave GPU cleanup
    // to EGL context teardown instead of calling GL from the Activity thread.
    discard_scene_without_gl();
    forget_gl_program();
    pthread_mutex_unlock(&guard);
}
