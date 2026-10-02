// A source-built development room. The room is one checked floor module from
// the original cache; actor placement, camera and visual hit cues are authored.
// This is separate from MainActivity's asset diagnostics and original engine.
#include "scene_buffers.hpp"
#include "../texture-assets/texture.hpp"

#include <GLES2/gl2.h>
#include <android/log.h>
#include <jni.h>
#include <pthread.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <new>

namespace {
using dh2::viewer::SceneMesh;
constexpr unsigned frame_count = 20;
constexpr unsigned max_enemies = 16;
constexpr float pi = 3.14159265358979323846f;
pthread_mutex_t world_guard = PTHREAD_MUTEX_INITIALIZER;
struct Pixels { std::uint8_t* bytes; int width, height; };
struct World {
    SceneMesh room{}, rest{}, walk[frame_count]{}, idle[frame_count]{}, attack[frame_count]{};
    Pixels stone{}, character{};
    std::uint8_t* model_bytes = nullptr;
    std::size_t model_size = 0;
    float center_x = 0, center_y = 0, model_floor = 0, model_scale = 1;
    std::int32_t idle_duration = 0, attack_duration = 0;
    bool ready = false, motions_ready = false;
};
World world{};
GLuint world_program = 0, stone_texture = 0, character_texture = 0;
GLint position = -1, uv = -1, offset = -1, heading = -1, camera = -1;
GLint aspect = -1, color = -1, textured = -1, sampler = -1;
int width = 1, height = 1;
bool textures_dirty = true;
float camera_x = 0, camera_y = 0;
double previous_frame = 0, walk_clock = 0, idle_clock = 0;

const char vs_source[] =
    "attribute vec3 aPosition;attribute vec2 aUv;varying vec2 vUv;"
    "uniform vec3 uOffset;uniform float uHeading;uniform vec2 uCamera;uniform float uAspect;"
    "void main(){float c=cos(uHeading),s=sin(uHeading);"
    "vec3 p=vec3(c*aPosition.x-s*aPosition.y,s*aPosition.x+c*aPosition.y,aPosition.z)+uOffset;"
    "p.xy-=uCamera;float vy=.62*p.y+.7846*p.z;float vz=-.7846*p.y+.62*p.z;"
    "gl_Position=vec4(p.x/(2.8*uAspect),vy/2.8,-vz/20.0,1.0);vUv=aUv;}";
const char fs_source[] =
    "precision mediump float;varying vec2 vUv;uniform sampler2D uTexture;"
    "uniform vec4 uColor;uniform float uTextured;"
    "void main(){vec4 t=texture2D(uTexture,vUv);"
    "gl_FragColor=mix(vec4(1.0),t,uTextured)*uColor;}";

double now() {
    timespec t{}; clock_gettime(CLOCK_MONOTONIC, &t);
    return double(t.tv_sec) + double(t.tv_nsec) / 1000000000.0;
}
void release(World& w) {
    dh2_viewer_scene_mesh_free(&w.room); dh2_viewer_scene_mesh_free(&w.rest);
    for (auto& frame : w.walk) dh2_viewer_scene_mesh_free(&frame);
    for (auto& frame : w.idle) dh2_viewer_scene_mesh_free(&frame);
    for (auto& frame : w.attack) dh2_viewer_scene_mesh_free(&frame);
    std::free(w.model_bytes);
    std::free(w.stone.bytes); std::free(w.character.bytes); w = {};
}
std::uint8_t* copy_input(JNIEnv* env, jbyteArray array, std::size_t& size) {
    size = array ? static_cast<std::size_t>(env->GetArrayLength(array)) : 0;
    if (!size || size > 32U * 1024U * 1024U) return nullptr;
    auto* bytes = static_cast<std::uint8_t*>(std::malloc(size));
    if (bytes) env->GetByteArrayRegion(array, 0, static_cast<jsize>(size),
                                      reinterpret_cast<jbyte*>(bytes));
    if (env->ExceptionCheck()) { std::free(bytes); return nullptr; }
    return bytes;
}
bool decode(JNIEnv* env, jbyteArray input, Pixels& pixels) {
    std::size_t size = 0; auto* bytes = copy_input(env, input, size);
    if (!bytes) return false;
    dh2::textures::TextureView t{};
    bool ok = dh2_texture_open(&t, bytes, size) == dh2::textures::Error::ok &&
        t.width <= 4096 && t.height <= 4096 && t.width && t.height &&
        (t.format == dh2::textures::Format::pvrtc_2bpp ||
         t.format == dh2::textures::Format::pvrtc_4bpp);
    if (ok) {
        const auto length = std::size_t(t.width) * t.height * 4;
        pixels.bytes = static_cast<std::uint8_t*>(std::malloc(length));
        ok = pixels.bytes && dh2_texture_decode_rgba8(pixels.bytes, length,
            t.width * 4, bytes, size) == dh2::textures::Error::ok;
        if (ok) { pixels.width = static_cast<int>(t.width); pixels.height = static_cast<int>(t.height); }
    }
    std::free(bytes); return ok;
}
void room_coordinates(SceneMesh& mesh) {
    for (unsigned i = 0; i < mesh.vertex_count; ++i) {
        mesh.vertices[i*5] = (mesh.vertices[i*5] - 45000.0f) / 1000.0f;
        mesh.vertices[i*5+1] = (mesh.vertices[i*5+1] - 10000.0f) / 1000.0f;
        mesh.vertices[i*5+2] = (mesh.vertices[i*5+2] - 1260.73f) / 1000.0f;
    }
}
void character_coordinates(SceneMesh& mesh, float center_x, float center_y,
                           float floor, float scale) {
    for (unsigned i = 0; i < mesh.vertex_count; ++i) {
        // The supplied walk advances along -Y. Give the development actor a
        // consistent +Y forward basis before applying the session heading.
        mesh.vertices[i*5] = -(mesh.vertices[i*5] - center_x) * scale;
        mesh.vertices[i*5+1] = -(mesh.vertices[i*5+1] - center_y) * scale;
        mesh.vertices[i*5+2] = (mesh.vertices[i*5+2] - floor) * scale;
    }
}
bool motion_frames(SceneMesh* frames, const dh2::resources::BresView& model,
                   const dh2::pose::Clip& clip, const World& w) {
    std::int32_t root_track = -1; float root_origin[4]{};
    for (unsigned i = 0; i < clip.count; ++i) {
        const char* target = dh2_animation_target(&clip.tracks[i]);
        if (target && std::strcmp(target,"Bip01-node") == 0 &&
            dh2_animation_type(&clip.tracks[i],0) == 1) root_track = static_cast<std::int32_t>(i);
    }
    if (root_track < 0 || dh2_pose_sample(&clip,static_cast<unsigned>(root_track),
            clip.start,root_origin) != dh2::pose::Error::ok) return false;
    for (unsigned frame = 0; frame < frame_count; ++frame) {
        const std::int32_t time = clip.start + static_cast<std::int32_t>(
            std::int64_t(clip.end-clip.start)*frame/frame_count);
        float root_position[4]{};
        if (dh2_world_scene_mesh_at(&frames[frame],&model,&clip,time) != dh2::viewer::SceneMeshError::ok ||
            dh2_pose_sample(&clip,static_cast<unsigned>(root_track),time,root_position) != dh2::pose::Error::ok)
            return false;
        // World travel belongs to the development session, while the original
        // joint rotations and vertical motion remain in the sampled pose.
        character_coordinates(frames[frame],w.center_x+root_position[0]-root_origin[0],
            w.center_y+root_position[1]-root_origin[1],w.model_floor,w.model_scale);
    }
    return true;
}
bool floor_at(const SceneMesh& room, float x, float y, float* z) {
    if (!room.vertices || !std::isfinite(x) || !std::isfinite(y)) return false;
    bool found = false; float highest = -10000.0f;
    for (unsigned i = 0; i+2 < room.index_count; i += 3) {
        const float* a = room.vertices + room.indices[i]*5;
        const float* b = room.vertices + room.indices[i+1]*5;
        const float* c = room.vertices + room.indices[i+2]*5;
        const float d = (b[1]-c[1])*(a[0]-c[0]) + (c[0]-b[0])*(a[1]-c[1]);
        if (std::fabs(d) < 0.000001f) continue;
        const float u = ((b[1]-c[1])*(x-c[0])+(c[0]-b[0])*(y-c[1]))/d;
        const float v = ((c[1]-a[1])*(x-c[0])+(a[0]-c[0])*(y-c[1]))/d;
        if (u < -0.0001f || v < -0.0001f || u+v > 1.0001f) continue;
        const float current = u*a[2]+v*b[2]+(1-u-v)*c[2];
        if (current > highest) { highest = current; found = true; }
    }
    if (found && z) *z = highest;
    return found;
}
GLuint shader(GLenum kind, const char* source) {
    const GLuint result = glCreateShader(kind);
    glShaderSource(result, 1, &source, nullptr); glCompileShader(result);
    GLint valid = GL_FALSE; glGetShaderiv(result, GL_COMPILE_STATUS, &valid);
    if (!valid) {
        char log[512]{}; glGetShaderInfoLog(result, sizeof(log), nullptr, log);
        __android_log_print(ANDROID_LOG_ERROR, "DH2World", "shader rejected: %s", log);
        glDeleteShader(result); return 0;
    }
    return result;
}
void upload(GLuint texture, const Pixels& pixels) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Original floor UVs deliberately tile beyond [0,1]. These checked PVRTC
    // images are powers of two, so repeating the stone is GLES2 compatible.
    const GLint wrap = texture == stone_texture ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixels.width, pixels.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels.bytes);
}
void mesh_draw(const SceneMesh& mesh, GLuint texture, float x, float y, float z,
               float angle, float r, float g, float b, float alpha = 1.0f) {
    glBindTexture(GL_TEXTURE_2D, texture); glUniform1f(textured, texture ? 1.0f : 0.0f);
    glUniform3f(offset, x, y, z); glUniform1f(heading, angle);
    glUniform4f(color, r, g, b, alpha);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), mesh.vertices);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), mesh.vertices+3);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.index_count), GL_UNSIGNED_SHORT, mesh.indices);
}
void ring(float x, float y, float z, float radius, float r, float g, float b, float alpha) {
    float points[33*5]{}; std::uint16_t faces[32*3]{};
    for (unsigned i = 0; i < 32; ++i) {
        const float angle = 2*pi*i/32;
        points[(i+1)*5] = std::cos(angle)*radius;
        points[(i+1)*5+1] = std::sin(angle)*radius;
        faces[i*3] = 0; faces[i*3+1] = static_cast<std::uint16_t>(i+1);
        faces[i*3+2] = static_cast<std::uint16_t>((i+1)%32+1);
    }
    SceneMesh circle{}; circle.vertices = points; circle.indices = faces; circle.index_count = 96;
    mesh_draw(circle, 0, x, y, z, 0, r, g, b, alpha);
}
}

extern "C" bool dh2_world_walkable(float x, float y) {
    pthread_mutex_lock(&world_guard);
    // Authored avatar radius on the recovered floor's triangle footprint.
    const bool valid = world.ready && floor_at(world.room,x,y,nullptr) &&
        floor_at(world.room,x-.12f,y,nullptr) && floor_at(world.room,x+.12f,y,nullptr) &&
        floor_at(world.room,x,y-.12f,nullptr) && floor_at(world.room,x,y+.12f,nullptr);
    pthread_mutex_unlock(&world_guard); return valid;
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_loadWorld(
    JNIEnv* env, jclass, jbyteArray room, jbyteArray stone, jbyteArray character,
    jbyteArray character_texture_input, jbyteArray animation) {
    auto* candidate = static_cast<World*>(std::calloc(1, sizeof(World)));
    auto* clip = static_cast<dh2::pose::Clip*>(std::malloc(sizeof(dh2::pose::Clip)));
    if (!candidate || !clip) { std::free(candidate); std::free(clip); return env->NewStringUTF("Room could not load: memory allocation failed."); }
    new (candidate) World{}; new (clip) dh2::pose::Clip{};
    std::size_t room_size = 0, character_size = 0, animation_size = 0;
    auto* room_bytes = copy_input(env,room,room_size);
    auto* character_bytes = copy_input(env,character,character_size);
    auto* animation_bytes = copy_input(env,animation,animation_size);
    dh2::resources::BresView room_view{}, character_view{}, animation_view{};
    bool valid = room_bytes && character_bytes && animation_bytes &&
        dh2_bres_open(&room_view,room_bytes,room_size) == dh2::resources::BresError::ok &&
        dh2_bres_open(&character_view,character_bytes,character_size) == dh2::resources::BresError::ok &&
        dh2_bres_open(&animation_view,animation_bytes,animation_size) == dh2::resources::BresError::ok &&
        dh2_world_scene_mesh(&candidate->room,&room_view,"_floor_X_voidmaze_nswe_00-node") == dh2::viewer::SceneMeshError::ok &&
        dh2_world_scene_mesh(&candidate->rest,&character_view,nullptr) == dh2::viewer::SceneMeshError::ok &&
        candidate->rest.skin_joints &&
        dh2_pose_clip_open(clip,&animation_view,0) == dh2::pose::Error::ok && clip->end > clip->start &&
        decode(env,stone,candidate->stone) && decode(env,character_texture_input,candidate->character);
    float minimum[3] = {INFINITY,INFINITY,INFINITY}, maximum[3] = {-INFINITY,-INFINITY,-INFINITY};
    if (valid) {
        for (unsigned i = 0; i < candidate->rest.vertex_count; ++i)
            for (unsigned axis = 0; axis < 3; ++axis) {
                const float value = candidate->rest.vertices[i*5+axis];
                if (value < minimum[axis]) minimum[axis] = value;
                if (value > maximum[axis]) maximum[axis] = value;
            }
        valid = std::isfinite(maximum[2]-minimum[2]) && maximum[2] > minimum[2];
    }
    if (valid) {
        const float cx = (minimum[0]+maximum[0])/2, cy = (minimum[1]+maximum[1])/2;
        const float scale = .86f/(maximum[2]-minimum[2]);
        candidate->center_x = cx; candidate->center_y = cy;
        candidate->model_floor = minimum[2]; candidate->model_scale = scale;
        room_coordinates(candidate->room);
        character_coordinates(candidate->rest,cx,cy,minimum[2],scale);
        valid = motion_frames(candidate->walk,character_view,*clip,*candidate);
        valid = valid && floor_at(candidate->room,0,0,nullptr);
    }
    std::free(room_bytes); std::free(animation_bytes); std::free(clip);
    if (valid) { candidate->model_bytes = character_bytes; candidate->model_size = character_size; }
    else std::free(character_bytes);
    if (!valid) { release(*candidate); std::free(candidate); return env->NewStringUTF("Room could not load: required cache model, motion or texture was rejected."); }
    candidate->ready = true;
    pthread_mutex_lock(&world_guard); release(world); world = *candidate;
    camera_x = camera_y = 0; textures_dirty = true; previous_frame = 0; walk_clock = 0; idle_clock = 0;
    pthread_mutex_unlock(&world_guard); std::free(candidate);
    return env->NewStringUTF("Development room ready: original maze floor and animated warrior. Move, approach targets and attack.");
}

extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_loadMotions(
    JNIEnv* env, jclass, jbyteArray idle, jbyteArray attack) {
    auto* frames = static_cast<SceneMesh*>(std::calloc(frame_count*2,sizeof(SceneMesh)));
    auto* clips = static_cast<dh2::pose::Clip*>(std::calloc(2,sizeof(dh2::pose::Clip)));
    if (!frames || !clips) { std::free(frames); std::free(clips); return env->NewStringUTF("Extra motions could not load: memory allocation failed."); }
    std::size_t idle_size = 0, attack_size = 0;
    auto* idle_bytes = copy_input(env,idle,idle_size); auto* attack_bytes = copy_input(env,attack,attack_size);
    dh2::resources::BresView idle_view{}, attack_view{}, model{};
    bool valid = idle_bytes && attack_bytes &&
        dh2_bres_open(&idle_view,idle_bytes,idle_size) == dh2::resources::BresError::ok &&
        dh2_bres_open(&attack_view,attack_bytes,attack_size) == dh2::resources::BresError::ok &&
        dh2_pose_clip_open(&clips[0],&idle_view,0) == dh2::pose::Error::ok &&
        dh2_pose_clip_open(&clips[1],&attack_view,0) == dh2::pose::Error::ok &&
        clips[0].end > clips[0].start && clips[1].end > clips[1].start;
    pthread_mutex_lock(&world_guard);
    if (valid) valid = world.ready && dh2_bres_open(&model,world.model_bytes,world.model_size) == dh2::resources::BresError::ok &&
        motion_frames(frames,model,clips[0],world) && motion_frames(frames+frame_count,model,clips[1],world);
    if (valid) {
        for (unsigned i = 0; i < frame_count; ++i) {
            dh2_viewer_scene_mesh_free(&world.idle[i]); dh2_viewer_scene_mesh_free(&world.attack[i]);
            world.idle[i] = frames[i]; world.attack[i] = frames[frame_count+i];
            frames[i] = {}; frames[frame_count+i] = {};
        }
        world.idle_duration = clips[0].end-clips[0].start;
        world.attack_duration = clips[1].end-clips[1].start;
        world.motions_ready = true; idle_clock = 0;
    }
    pthread_mutex_unlock(&world_guard);
    for (unsigned i = 0; i < frame_count*2; ++i) dh2_viewer_scene_mesh_free(&frames[i]);
    std::free(frames); std::free(clips); std::free(idle_bytes); std::free(attack_bytes);
    return env->NewStringUTF(valid ? "Original idle and attack motions ready." : "Extra motions rejected; walk preview remains available.");
}

extern "C" JNIEXPORT jboolean JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_walkable(JNIEnv*, jclass, jfloat x, jfloat y) {
    return dh2_world_walkable(x,y);
}
extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_surfaceCreated(JNIEnv*, jclass) {
    const GLuint vs = shader(GL_VERTEX_SHADER,vs_source), fs = shader(GL_FRAGMENT_SHADER,fs_source);
    if (!vs || !fs) { if (vs) glDeleteShader(vs); if (fs) glDeleteShader(fs); return; }
    world_program = glCreateProgram(); glAttachShader(world_program,vs); glAttachShader(world_program,fs);
    glLinkProgram(world_program); glDeleteShader(vs); glDeleteShader(fs);
    GLint linked = GL_FALSE; glGetProgramiv(world_program,GL_LINK_STATUS,&linked);
    if (!linked) { glDeleteProgram(world_program); world_program = 0; return; }
    position = glGetAttribLocation(world_program,"aPosition"); uv = glGetAttribLocation(world_program,"aUv");
    offset = glGetUniformLocation(world_program,"uOffset"); heading = glGetUniformLocation(world_program,"uHeading");
    camera = glGetUniformLocation(world_program,"uCamera"); aspect = glGetUniformLocation(world_program,"uAspect");
    color = glGetUniformLocation(world_program,"uColor"); textured = glGetUniformLocation(world_program,"uTextured");
    sampler = glGetUniformLocation(world_program,"uTexture");
    glGenTextures(1,&stone_texture); glGenTextures(1,&character_texture);
    pthread_mutex_lock(&world_guard); textures_dirty = true; pthread_mutex_unlock(&world_guard);
    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_surfaceChanged(JNIEnv*, jclass, jint w, jint h) {
    width = w > 0 ? w : 1; height = h > 0 ? h : 1; glViewport(0,0,width,height);
}
extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_GameplayActivity_drawFrame(JNIEnv* env, jclass, jfloatArray snapshot) {
    glClearColor(.025f,.035f,.055f,1); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if (!world_program || !snapshot) return;
    const jsize size = env->GetArrayLength(snapshot);
    if (size < 6 || size > static_cast<jsize>(6+max_enemies*5)) return;
    float state[6+max_enemies*5]{}; env->GetFloatArrayRegion(snapshot,0,size,state);
    for (jsize i = 0; i < size; ++i) if (!std::isfinite(state[i])) return;
    if (state[5] < 0 || state[5] > max_enemies || std::floor(state[5]) != state[5]) return;
    const int enemies = static_cast<int>(state[5]);
    if (enemies < 0 || enemies > static_cast<int>(max_enemies) || size != 6+enemies*5) return;
    pthread_mutex_lock(&world_guard);
    if (!world.ready) { pthread_mutex_unlock(&world_guard); return; }
    const double time = now(); double dt = previous_frame ? time-previous_frame : 0;
    previous_frame = time; if (dt > .1) dt = .1; if (dt < 0) dt = 0;
    const float follow = static_cast<float>(1-std::exp(-dt*5));
    camera_x += (state[0]*.45f-camera_x)*follow; camera_y += (state[1]*.45f-camera_y)*follow;
    if (state[3] > .5f) walk_clock += dt;
    idle_clock += dt;
    if (textures_dirty) { upload(stone_texture,world.stone); upload(character_texture,world.character); textures_dirty = false; }
    glUseProgram(world_program); glActiveTexture(GL_TEXTURE0); glUniform1i(sampler,0);
    glUniform2f(camera,camera_x,camera_y); glUniform1f(aspect,static_cast<float>(width)/height);
    glEnableVertexAttribArray(position); glEnableVertexAttribArray(uv);
    mesh_draw(world.room,stone_texture,0,0,0,0,.85f,.90f,1.0f);
    const auto walk_index = static_cast<unsigned>(std::fmod(walk_clock,.799)/.799*frame_count);
    const double idle_seconds = world.idle_duration/1000.0;
    const auto idle_index = world.motions_ready ? static_cast<unsigned>(
        std::fmod(idle_clock,idle_seconds)/idle_seconds*frame_count) : 0;
    const float attack_progress = state[4] > 1 ? 0 : state[4] < 0 ? 1 : 1-state[4];
    const auto attack_index = static_cast<unsigned>(attack_progress*(frame_count-1));
    const auto& standing = world.motions_ready ? world.idle[idle_index] : world.walk[0];
    const auto& player_pose = world.motions_ready && state[4] > 0 ? world.attack[attack_index] :
        state[3] > .5f ? world.walk[walk_index] : standing;
    float floor = .04f; floor_at(world.room,state[0],state[1],&floor);
    glDepthMask(GL_FALSE); ring(state[0],state[1],floor+.002f,.25f,.08f,.35f,.7f,.38f); glDepthMask(GL_TRUE);
    mesh_draw(player_pose,character_texture,state[0],state[1],floor+.01f,state[2],1,1,1);
    for (int i = 0; i < enemies; ++i) {
        const float* enemy = state+6+i*5;
        if (enemy[3] <= 0) continue;
        float ground = .04f; floor_at(world.room,enemy[0],enemy[1],&ground);
        glDepthMask(GL_FALSE); ring(enemy[0],enemy[1],ground+.003f,.26f,.9f,.12f,.08f,.45f); glDepthMask(GL_TRUE);
        const float hit = enemy[4] < 0 ? 0 : enemy[4] > 1 ? 1 : enemy[4];
        mesh_draw(standing,character_texture,enemy[0],enemy[1],ground+.01f,enemy[2],1,.48f+hit*.5f,.45f+hit*.5f);
        // A simple world-space health indicator is authored for the test scene.
        float bar_points[20] = {-.28f,0,1.05f,0,0, -.28f+.56f*enemy[3],0,1.05f,0,0,
            -.28f+.56f*enemy[3],0,1.10f,0,0, -.28f,0,1.10f,0,0};
        const std::uint16_t bar_faces[6] = {0,1,2,2,3,0};
        SceneMesh bar{}; bar.vertices = bar_points; bar.indices = const_cast<std::uint16_t*>(bar_faces); bar.index_count = 6;
        mesh_draw(bar,0,enemy[0],enemy[1],ground,0,.8f,.12f,.09f);
    }
    if (state[4] > 0) {
        const float pulse = state[4] > 1 ? 1 : state[4];
        glDepthMask(GL_FALSE); ring(state[0],state[1],floor+.07f,.40f+.55f*(1-pulse),1,.75f,.15f,pulse*.45f); glDepthMask(GL_TRUE);
    }
    glDisableVertexAttribArray(position); glDisableVertexAttribArray(uv);
    pthread_mutex_unlock(&world_guard);
}
