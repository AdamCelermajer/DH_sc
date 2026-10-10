#include "renderer.hpp"

#include "frame_perf.hpp"
#include "platform_context_identity.hpp"
#include <GL/gl.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <type_traits>

namespace dh::foundation {
namespace {

constexpr float pi = 3.14159265358979323846f;
#if defined(_WIN32)
constexpr const char* render_context_requirement=
    "GameObject visual quality requires this renderer's current WGL context/thread";
#else
constexpr const char* render_context_requirement=
    "GameObject visual quality requires this renderer's current host context/thread";
#endif

Vec3 subtract(Vec3 a, Vec3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
float dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
Vec3 cross(Vec3 a, Vec3 b) {
    return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
Vec3 normalize(Vec3 value, Vec3 fallback) {
    const float lengthSquared = dot(value, value);
    if (!std::isfinite(lengthSquared) || lengthSquared < 1.0e-12f) return fallback;
    const float inverse = 1.0f/std::sqrt(lengthSquared);
    return {value.x*inverse, value.y*inverse, value.z*inverse};
}

Mat4 viewMatrix(const Camera& camera) {
    const Vec3 forward = normalize(subtract(camera.target, camera.eye), {0,0,-1});
    Vec3 side = cross(forward, camera.up);
    if (dot(side, side) < 1.0e-12f) {
        side = cross(forward, std::abs(forward.y) < 0.9f ? Vec3{0,1,0} : Vec3{1,0,0});
    }
    side = normalize(side, {1,0,0});
    const Vec3 up = cross(side, forward);
    return {side.x, up.x, -forward.x, 0,
            side.y, up.y, -forward.y, 0,
            side.z, up.z, -forward.z, 0,
            -dot(side,camera.eye), -dot(up,camera.eye), dot(forward,camera.eye), 1};
}

void applyMaterial(const Material& material, bool blended, float alphaReference) {
    {auto& counters=perf::FramePerf::get().counters();static std::uint32_t lastTexture=0xffffffffu;++counters.stateChanges;if(material.texture!=lastTexture){++counters.texBinds;lastTexture=material.texture;}} // B066 stats
    if (material.lightingEnabled) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);
    glColor4fv(material.color.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material.color.data());
    if (material.texture != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, material.texture);
    } else {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if(material.sourcePass) {
        const auto& source=*material.sourcePass;
        if(source.cull)glEnable(GL_CULL_FACE);else glDisable(GL_CULL_FACE);
        glCullFace(source.cullFace);glFrontFace(source.frontFace);
        if(source.depthTest)glEnable(GL_DEPTH_TEST);else glDisable(GL_DEPTH_TEST);
        glDepthFunc(source.depthFunction);glDepthMask(source.depthWrite?GL_TRUE:GL_FALSE);
        if(source.alphaTest){glEnable(GL_ALPHA_TEST);glAlphaFunc(GL_GREATER,std::clamp(material.alphaReference,0.f,1.f));}
        else glDisable(GL_ALPHA_TEST);
        if(source.blend){glEnable(GL_BLEND);glBlendFunc(source.blendSource,source.blendDestination);}
        else glDisable(GL_BLEND);
        return;
    }
    glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);glCullFace(GL_BACK);glFrontFace(GL_CCW);
    if (material.doubleSided) glDisable(GL_CULL_FACE);
    else glEnable(GL_CULL_FACE);
    if (alphaReference > 0.0f) {
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, std::clamp(alphaReference, 0.0f, 1.0f));
    } else glDisable(GL_ALPHA_TEST);
    if (blended) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, material.additive ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    } else {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
}

} // namespace

Mat4 identity() {
    return {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
}

Mat4 translation(Vec3 position) {
    Mat4 result = identity();
    result[12] = position.x;
    result[13] = position.y;
    result[14] = position.z;
    return result;
}

bool Renderer::initialize(int width, int height) {
    qualityContext_=qualityDeviceContext_=0;qualityThread_=0;
    const auto context=current_render_context_identity();
    const auto device=current_render_drawable_identity();
    if (!context||!device) return false;
    resize(width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    if(glGetError()!=GL_NO_ERROR)return false;
    if(perf::FramePerf::get().enabled()){GLint maxTexture=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxTexture);std::ostringstream o;o<<"Perf GL vendor="<<reinterpret_cast<const char*>(glGetString(GL_VENDOR))<<" renderer="<<reinterpret_cast<const char*>(glGetString(GL_RENDERER))<<" version="<<reinterpret_cast<const char*>(glGetString(GL_VERSION))<<" maxTexture="<<maxTexture<<" extensions="<<(glGetString(GL_EXTENSIONS)?reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)):"")<<" viewport="<<width<<'x'<<height;perf::FramePerf::get().note(o.str());}
    qualityContext_=context;
    qualityDeviceContext_=device;
    qualityThread_=current_render_thread_identity();
    return true;
}
bool Renderer::game_object_visual_quality(bool& full_quality,std::string& error)const{
    if(!qualityContext_||!qualityDeviceContext_||!qualityThread_||
       qualityContext_!=current_render_context_identity()||
       qualityDeviceContext_!=current_render_drawable_identity()||
       qualityThread_!=current_render_thread_identity()){
        error=render_context_requirement;return false;
    }
    if(visualQuality_!=VisualAssetQuality::Full&&visualQuality_!=VisualAssetQuality::ReducedOptional){
        error="Unknown renderer visual asset quality policy";return false;
    }
    full_quality=visualQuality_==VisualAssetQuality::Full;error.clear();return true;
}

void Renderer::resize(int width, int height) {
    width_ = std::max(1, width);
    height_ = std::max(1, height);
    glViewport(0, 0, width_, height_);
}

void Renderer::beginFrame(const Camera& camera) {
    camera_ = camera;
    glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);glCullFace(GL_BACK);glFrontFace(GL_CCW);
    glDisable(GL_BLEND);glDisable(GL_ALPHA_TEST);
    glDepthMask(GL_TRUE);
    glClearColor(0.035f, 0.045f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    applyCamera(camera);
}

// B066: view-frustum planes (clip = projection * view) for conservative culling of static level geometry.
void Renderer::updateFrustum(const Camera& camera) {
    const float fov = std::isfinite(camera.verticalFovDegrees)
        ? std::clamp(camera.verticalFovDegrees, 1.0f, 175.0f) : 60.0f;
    const float nearPlane = std::isfinite(camera.nearPlane) ? std::max(0.001f, camera.nearPlane) : 0.1f;
    const float farPlane = std::isfinite(camera.farPlane) ? std::max(nearPlane+1.0f, camera.farPlane) : 5000.0f;
    const double top = nearPlane*std::tan(fov*pi/360.0f);
    const double aspect = std::isfinite(camera.aspectRatio) && camera.aspectRatio > 0.0f
        ? static_cast<double>(camera.aspectRatio) : static_cast<double>(width_)/height_;
    const double right = top*aspect;
    const Mat4 view = viewMatrix(camera);
    // Projection (glFrustum, symmetric): rows of P.
    const double p[4][4] = {{nearPlane/right,0,0,0},{0,nearPlane/top,0,0},
        {0,0,-(double(farPlane)+nearPlane)/(double(farPlane)-nearPlane),-2.0*double(farPlane)*nearPlane/(double(farPlane)-nearPlane)},{0,0,-1,0}};
    double m[4][4];   // m = P * V, V column-major
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) {
        double sum = 0; for (int k = 0; k < 4; ++k) sum += p[i][k]*view[j*4+k];
        m[i][j] = sum;
    }
    const auto plane = [&](int sign, int row) {
        std::array<float, 4> result{};
        for (int j = 0; j < 4; ++j) result[j] = float(m[3][j] + sign*m[row][j]);
        return result;
    };
    frustum_ = {plane(1,0), plane(-1,0), plane(1,1), plane(-1,1), plane(1,2), plane(-1,2)};
    frustumValid_ = true;
    for (const auto& pl : frustum_) for (float v : pl) if (!std::isfinite(v)) frustumValid_ = false;
}

void Renderer::applyCamera(const Camera& camera) {
    updateFrustum(camera); // B066

    const float fov = std::isfinite(camera.verticalFovDegrees)
        ? std::clamp(camera.verticalFovDegrees, 1.0f, 175.0f) : 60.0f;
    const float nearPlane = std::isfinite(camera.nearPlane)
        ? std::max(0.001f, camera.nearPlane) : 0.1f;
    const float farPlane = std::isfinite(camera.farPlane)
        ? std::max(nearPlane+1.0f, camera.farPlane) : 5000.0f;
    const double top = nearPlane*std::tan(fov*pi/360.0f);
    const double aspect = std::isfinite(camera.aspectRatio) && camera.aspectRatio > 0.0f
        ? static_cast<double>(camera.aspectRatio) : static_cast<double>(width_)/height_;
    const double right = top*aspect;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-right, right, -top, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
    const Mat4 view = viewMatrix(camera);
    glLoadMatrixf(view.data());
    const GLfloat ambient[] = {0.65f,0.65f,0.65f,1};
    const GLfloat diffuse[] = {0.65f,0.65f,0.65f,1};
    const GLfloat direction[] = {0.3f,1.0f,0.4f,0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, direction);
}

bool Renderer::withViewport(int x, int y, int width, int height,
                            const Camera& camera, const std::function<void()>& drawContent) {
    bool quality = false;
    std::string error;
    if (width <= 0 || height <= 0 || !drawContent ||
        !game_object_visual_quality(quality, error)) return false;
    // Calculate intersections in wide integers to avoid overflowing UI bounds.
    using Wide = std::int64_t;
    GLint parentViewport[4]; glGetIntegerv(GL_VIEWPORT, parentViewport);
    Wide left = std::max<Wide>(parentViewport[0], x);
    Wide bottom = std::max<Wide>(parentViewport[1], y);
    Wide right = std::min<Wide>(Wide(parentViewport[0])+parentViewport[2], Wide(x)+width);
    Wide top = std::min<Wide>(Wide(parentViewport[1])+parentViewport[3], Wide(y)+height);
    if (glIsEnabled(GL_SCISSOR_TEST)) {
        GLint clip[4]; glGetIntegerv(GL_SCISSOR_BOX, clip);
        left = std::max(left, Wide(clip[0])); bottom = std::max(bottom, Wide(clip[1]));
        right = std::min(right, Wide(clip[0]) + clip[2]);
        top = std::min(top, Wide(clip[1]) + clip[3]);
    }
    if (right <= left || top <= bottom) return false;
    const int savedWidth = width_, savedHeight = height_;
    const Camera savedCamera = camera_;
    GLint matrixMode; glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glMatrixMode(GL_PROJECTION); glPushMatrix();
    glMatrixMode(GL_MODELVIEW); glPushMatrix();
    const auto restore = [&] {
        glMatrixMode(GL_MODELVIEW); glPopMatrix();
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glPopClientAttrib(); glPopAttrib(); glMatrixMode(matrixMode);
        width_ = savedWidth; height_ = savedHeight; camera_ = savedCamera;
        updateFrustum(camera_); // B066
    };
    glViewport(x, y, width, height);
    glEnable(GL_SCISSOR_TEST);
    glScissor(static_cast<int>(left), static_cast<int>(bottom),
              static_cast<int>(right-left), static_cast<int>(top-bottom));
    width_ = width; height_ = height; camera_ = camera;
    glDepthMask(GL_TRUE); glClearDepth(1.0); glClear(GL_DEPTH_BUFFER_BIT);
    applyCamera(camera);
    try { drawContent(); } catch (...) { restore(); throw; }
    restore();
    return true;
}

void Renderer::draw(const Mesh& mesh, const Mat4& transform, RenderPass pass) {
    drawInternal(mesh, transform, pass, nullptr);
}

void Renderer::drawRange(const Mesh& mesh, const DrawRange& range, const Mat4& transform) {
    drawInternal(mesh, transform, RenderPass::All, &range);
}

namespace {
RangeStats computeRangeStats(const Mesh& mesh, const DrawRange& range) {
    RangeStats stats;
    const bool indexed = !mesh.indices.empty();
    const std::size_t size = indexed ? mesh.indices.size() : mesh.vertices.size();
    const std::size_t first = std::min(range.firstIndex, size);
    const std::size_t count = std::min(range.indexCount, size-first);
    constexpr float big = std::numeric_limits<float>::max();
    Vec3 low{big,big,big}, high{-big,-big,-big};
    for (std::size_t element=first; element < first+count; ++element) {
        const std::size_t vertex = indexed ? mesh.indices[element] : element;
        if (vertex >= mesh.vertices.size()) continue;
        const auto& v = mesh.vertices[vertex];
        if (v.color[3] < 1.0f) stats.vertexAlpha = true;
        low.x=std::min(low.x,v.position.x); low.y=std::min(low.y,v.position.y); low.z=std::min(low.z,v.position.z);
        high.x=std::max(high.x,v.position.x); high.y=std::max(high.y,v.position.y); high.z=std::max(high.z,v.position.z);
        stats.valid = true;
    }
    if (stats.valid) {
        stats.low = low; stats.high = high;
        stats.center = {(low.x+high.x)*0.5f,(low.y+high.y)*0.5f,(low.z+high.z)*0.5f};
    }
    return stats;
}

// Buffer-object entry points (GL 1.5), resolved through the host's loader; opengl32.dll only exports GL 1.1.
struct BufferApi {
    using Gen = void (APIENTRY*)(GLsizei, GLuint*);
    using Bind = void (APIENTRY*)(GLenum, GLuint);
    using Data = void (APIENTRY*)(GLenum, std::ptrdiff_t, const void*, GLenum);
    using Delete = void (APIENTRY*)(GLsizei, const GLuint*);
    Gen gen = nullptr; Bind bind = nullptr; Data data = nullptr; Delete del = nullptr;
    void* (*loader)(const char*) = nullptr;
    bool ready() const { return gen && bind && data && del; }
};
BufferApi& bufferApi(void* (*loader)(const char*)) {
    static BufferApi api;
    if (loader && api.loader != loader) {
        api.loader = loader;
        api.gen = reinterpret_cast<BufferApi::Gen>(loader("glGenBuffers"));
        api.bind = reinterpret_cast<BufferApi::Bind>(loader("glBindBuffer"));
        api.data = reinterpret_cast<BufferApi::Data>(loader("glBufferData"));
        api.del = reinterpret_cast<BufferApi::Delete>(loader("glDeleteBuffers"));
    }
    return api;
}
constexpr GLenum kArrayBuffer = 0x8892, kElementBuffer = 0x8893, kStaticDraw = 0x88E4;
constexpr std::uint64_t rangeKey(const DrawRange& range) {
    return (std::uint64_t(range.firstIndex) << 32) ^ std::uint64_t(range.indexCount & 0xffffffffu);
}
} // namespace

Renderer::StaticMesh* Renderer::staticFor(const Mesh& mesh) const {
    if (!mesh.staticGeometry || mesh.vertices.empty()) return nullptr;
    auto& entry = staticMeshes_[&mesh];
    if (entry.vertexData == mesh.vertices.data() && entry.vertexCount == mesh.vertices.size() &&
        entry.indexData == mesh.indices.data() && entry.indexCount == mesh.indices.size()) return &entry;
    auto& api = bufferApi(glLoader_);
    if (entry.gpu && api.ready()) {
        const GLuint buffers[2] = {entry.vbo, entry.ibo};
        api.del(entry.ibo ? 2 : 1, buffers);
    }
    entry = StaticMesh{};
    entry.vertexData = mesh.vertices.data(); entry.vertexCount = mesh.vertices.size();
    entry.indexData = mesh.indices.data(); entry.indexCount = mesh.indices.size();
    static_assert(sizeof(Vertex) == 48, "Vertex must be tightly packed for buffer upload");
    const bool indexed = !mesh.indices.empty();
    if (glLoader_ && api.ready() && mesh.vertices.size() < (std::size_t(1) << 26) &&
        (!indexed || std::none_of(mesh.indices.begin(), mesh.indices.end(),
                                  [&](std::uint32_t index) { return index >= mesh.vertices.size(); }))) {
        while (glGetError() != GL_NO_ERROR) {}
        GLuint buffers[2] = {0, 0};
        api.gen(indexed ? 2 : 1, buffers);
        api.bind(kArrayBuffer, buffers[0]);
        api.data(kArrayBuffer, std::ptrdiff_t(mesh.vertices.size()*sizeof(Vertex)), mesh.vertices.data(), kStaticDraw);
        api.bind(kArrayBuffer, 0);
        if (indexed) {
            api.bind(kElementBuffer, buffers[1]);
            api.data(kElementBuffer, std::ptrdiff_t(mesh.indices.size()*sizeof(std::uint32_t)), mesh.indices.data(), kStaticDraw);
            api.bind(kElementBuffer, 0);
        }
        if (glGetError() == GL_NO_ERROR) {
            entry.vbo = buffers[0]; entry.ibo = indexed ? buffers[1] : 0; entry.gpu = true;
        } else api.del(indexed ? 2 : 1, buffers);
    }
    return &entry;
}

void Renderer::invalidateStaticGeometry() {
    auto& api = bufferApi(glLoader_);
    for (auto& item : staticMeshes_) {
        auto& entry = item.second;
        if (entry.gpu && api.ready()) {
            const GLuint buffers[2] = {entry.vbo, entry.ibo};
            api.del(entry.ibo ? 2 : 1, buffers);
        }
    }
    staticMeshes_.clear();
}

RangeStats Renderer::rangeStats(const Mesh& mesh, const DrawRange& range) const {
    if (auto* entry = staticFor(mesh)) {
        const auto key = rangeKey(range);
        const auto found = entry->ranges.find(key);
        if (found != entry->ranges.end()) return found->second;
        return entry->ranges.emplace(key, computeRangeStats(mesh, range)).first->second;
    }
    return computeRangeStats(mesh, range);
}

bool Renderer::rangeVisible(const Mesh& mesh, const DrawRange& range, const Mat4& transform) const {
    if (!frustumValid_ || !mesh.staticGeometry) return true;
    const RangeStats stats = rangeStats(mesh, range);
    if (!stats.valid) return true;
    std::array<int, 6> outside{};
    for (int corner = 0; corner < 8; ++corner) {
        const float x = (corner & 1) ? stats.high.x : stats.low.x;
        const float y = (corner & 2) ? stats.high.y : stats.low.y;
        const float z = (corner & 4) ? stats.high.z : stats.low.z;
        const float wx = transform[0]*x+transform[4]*y+transform[8]*z+transform[12];
        const float wy = transform[1]*x+transform[5]*y+transform[9]*z+transform[13];
        const float wz = transform[2]*x+transform[6]*y+transform[10]*z+transform[14];
        for (int plane = 0; plane < 6; ++plane) {
            const auto& p = frustum_[plane];
            if (p[0]*wx+p[1]*wy+p[2]*wz+p[3] < 0.0f) ++outside[plane];
        }
    }
    for (int plane = 0; plane < 6; ++plane) if (outside[plane] == 8) return false;
    return true;
}

bool Renderer::isTransparent(const Mesh& mesh, const DrawRange& range) const {
    if(range.material.sourcePass)return range.material.sourcePass->blend;
    if (range.material.additive || range.material.color[3] < 1.0f) return true;
    const auto found = textureAlpha_.find(range.material.texture);
    const bool textureAlpha = found != textureAlpha_.end() && found->second.hasAlpha;
    const bool binaryAlpha = textureAlpha && !found->second.hasPartialAlpha;
    if (range.material.alphaReference > 0.0f) return false;
    if (rangeStats(mesh, range).vertexAlpha) return true;
    return (range.material.transparent || textureAlpha) && !binaryAlpha;
}

void Renderer::drawInternal(const Mesh& mesh, const Mat4& transform, RenderPass pass,
                            const DrawRange* singleRange) {
    static_assert(std::is_standard_layout_v<Vertex>);
    if (mesh.vertices.empty()) return;
    const bool indexed = !mesh.indices.empty();
    const std::size_t elementCount = indexed ? mesh.indices.size() : mesh.vertices.size();
    const StaticMesh* staticMesh = staticFor(mesh);
    const bool gpu = staticMesh && staticMesh->gpu;   // B066: buffer objects, indices validated once at upload
    // Reject malformed meshes before the driver can dereference an invalid vertex.
    if (indexed && !gpu) {
        const std::size_t first = singleRange ? std::min(singleRange->firstIndex, elementCount) : 0;
        const std::size_t count = singleRange ? std::min(singleRange->indexCount,elementCount-first) : elementCount;
        if (std::any_of(mesh.indices.begin()+first, mesh.indices.begin()+first+count,
                [&](std::uint32_t index) { return index >= mesh.vertices.size(); })) return;
    }
    auto& api = bufferApi(glLoader_);

    glPushMatrix();
    glMultMatrixf(transform.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    const auto* bytes = reinterpret_cast<const unsigned char*>(mesh.vertices.data());
    const auto offsetPointer = [](std::size_t offset) { return reinterpret_cast<const void*>(offset); };
    bool colorsFromBuffer = false;
    if (gpu) {
        api.bind(kArrayBuffer, staticMesh->vbo);
        if (indexed) api.bind(kElementBuffer, staticMesh->ibo);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), offsetPointer(offsetof(Vertex, position)));
        glNormalPointer(GL_FLOAT, sizeof(Vertex), offsetPointer(offsetof(Vertex, normal)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), offsetPointer(offsetof(Vertex, u)));
    } else {
        // Reuse capacity across scene and character draws rather than allocating a
        // full scene-sized color array every frame.
        if (vertexColors_.size() < mesh.vertices.size()) vertexColors_.resize(mesh.vertices.size());
        glColorPointer(4, GL_FLOAT, sizeof(vertexColors_[0]), vertexColors_.data());
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), bytes+offsetof(Vertex, position));
        glNormalPointer(GL_FLOAT, sizeof(Vertex), bytes+offsetof(Vertex, normal));
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), bytes+offsetof(Vertex, u));
    }

    const auto hasVertexAlpha = [&](const DrawRange& range) { return rangeStats(mesh, range).vertexAlpha; };

    const auto drawRange = [&](const DrawRange& range) {
        if (range.firstIndex >= elementCount || range.indexCount == 0) return;
        const std::size_t count = std::min(range.indexCount, elementCount-range.firstIndex);
        if (count > static_cast<std::size_t>(std::numeric_limits<GLsizei>::max())) return;
        const auto found = textureAlpha_.find(range.material.texture);
        const bool binaryAlpha = found != textureAlpha_.end() && found->second.hasAlpha && !found->second.hasPartialAlpha;
        const bool textureAlpha = found != textureAlpha_.end() && found->second.hasAlpha;
        const float alphaReference = range.material.alphaReference > 0.0f ? range.material.alphaReference
            : binaryAlpha ? 1.0f/255.0f : 0.0f;
        const bool blended = range.material.additive || range.material.color[3] < 1.0f
            || (range.material.alphaReference <= 0.0f && hasVertexAlpha(range))
            || ((range.material.transparent || textureAlpha) && alphaReference <= 0.0f);
        applyMaterial(range.material, blended, alphaReference);
        const auto& tint = range.material.color;
        const bool untinted = tint[0] == 1.0f && tint[1] == 1.0f && tint[2] == 1.0f && tint[3] == 1.0f;
        if (gpu && untinted) {
            if (!colorsFromBuffer) {
                api.bind(kArrayBuffer, staticMesh->vbo);
                glColorPointer(4, GL_FLOAT, sizeof(Vertex), offsetPointer(offsetof(Vertex, color)));
                colorsFromBuffer = true;
            }
        } else {
            if (gpu) {
                if (vertexColors_.size() < mesh.vertices.size()) vertexColors_.resize(mesh.vertices.size());
                api.bind(kArrayBuffer, 0);
                glColorPointer(4, GL_FLOAT, sizeof(vertexColors_[0]), vertexColors_.data());
                colorsFromBuffer = false;
            }
            for (std::size_t element = range.firstIndex; element < range.firstIndex+count; ++element) {
                const std::size_t vertex = indexed ? mesh.indices[element] : element;
                for (std::size_t channel = 0; channel < 4; ++channel)
                    vertexColors_[vertex][channel] = mesh.vertices[vertex].color[channel]*tint[channel];
            }
        }
        {auto& c=perf::FramePerf::get().counters();++c.calls;c.triangles+=count/3;
         if(gpu)++c.vboDraws;else c.clientBytes+=mesh.vertices.size()*(sizeof(Vertex)+sizeof(vertexColors_[0]));} // B062/B066 draw statistics
        if (indexed) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT,
                           gpu ? offsetPointer(range.firstIndex*sizeof(std::uint32_t)) : static_cast<const void*>(mesh.indices.data()+range.firstIndex));
        } else if (range.firstIndex <= static_cast<std::size_t>(std::numeric_limits<GLint>::max())) {
            glDrawArrays(GL_TRIANGLES, static_cast<GLint>(range.firstIndex), static_cast<GLsizei>(count));
        }
    };

    const auto isBlended = [&](const DrawRange& range) {
        if(range.material.sourcePass)return range.material.sourcePass->blend;
        const auto found = textureAlpha_.find(range.material.texture);
        const bool textureAlpha = found != textureAlpha_.end() && found->second.hasAlpha;
        const bool binaryAlpha = textureAlpha && !found->second.hasPartialAlpha;
        return range.material.additive || range.material.color[3] < 1.0f
            || (range.material.alphaReference <= 0.0f && hasVertexAlpha(range))
            || ((range.material.transparent || textureAlpha) && range.material.alphaReference <= 0.0f && !binaryAlpha);
    };

    const auto rangeDepth = [&](const DrawRange& range) {
        // Range bounding-box center is a stable approximation for transparent
        // mesh sorting. Intersecting polygons require an authored ordering policy.
        const RangeStats stats = rangeStats(mesh, range);
        if (!stats.valid) return 0.0f;
        const Vec3 center = stats.center;
        const Vec3 world{transform[0]*center.x+transform[4]*center.y+transform[8]*center.z+transform[12],
                         transform[1]*center.x+transform[5]*center.y+transform[9]*center.z+transform[13],
                         transform[2]*center.x+transform[6]*center.y+transform[10]*center.z+transform[14]};
        return dot(subtract(world,camera_.eye), normalize(subtract(camera_.target,camera_.eye),{0,0,-1}));
    };

    if (singleRange) drawRange(*singleRange);
    else if (mesh.ranges.empty()) {
        const DrawRange range{0,elementCount,{}};
        const bool blended = isBlended(range);
        if (pass == RenderPass::All || (blended ? pass == RenderPass::Transparent : pass == RenderPass::Opaque))
            drawRange(range);
    }
    else {
        if (pass != RenderPass::Transparent)
            for (const auto& range : mesh.ranges)
                if (!isBlended(range)) drawRange(range);
        if (pass != RenderPass::Opaque) {
            std::vector<std::pair<float, const DrawRange*>> sorted;
            for (const auto& range : mesh.ranges)
                if (isBlended(range)) sorted.emplace_back(rangeDepth(range), &range);
            std::stable_sort(sorted.begin(),sorted.end(), [](const auto& a,const auto& b) { return a.first > b.first; });
            for (const auto& entry : sorted) drawRange(*entry.second);
        }
    }
    if (gpu) {
        api.bind(kArrayBuffer, 0);
        if (indexed) api.bind(kElementBuffer, 0);
    }
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}

void Renderer::endFrame() { glFlush(); }

std::uint32_t Renderer::createTexture(int width, int height, const std::uint8_t* rgba) {
    if (width <= 0 || height <= 0 || !rgba) return 0;
    GLint maximumSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maximumSize);
    if (width > maximumSize || height > maximumSize) return 0;
    GLint oldBinding = 0;
    GLint oldAlignment = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldBinding);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldAlignment);
    GLuint texture = 0;
    ++perf::FramePerf::get().counters().textureUploads;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    const GLenum error = glGetError();
    glPixelStorei(GL_UNPACK_ALIGNMENT, oldAlignment);
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(oldBinding));
    if (error != GL_NO_ERROR) {
        glDeleteTextures(1, &texture);
        return 0;
    }
    TextureAlpha alpha;
    const std::size_t pixels = static_cast<std::size_t>(width)*static_cast<std::size_t>(height);
    for (std::size_t pixel = 0; pixel < pixels; ++pixel) {
        const std::uint8_t value = rgba[pixel*4+3];
        alpha.hasAlpha = alpha.hasAlpha || value < 255;
        alpha.hasPartialAlpha = alpha.hasPartialAlpha || (value > 0 && value < 255);
    }
    textureAlpha_[texture] = alpha;
    return texture;
}

bool Renderer::textureHasAlpha(std::uint32_t texture) const {
    const auto found = textureAlpha_.find(texture);
    return found != textureAlpha_.end() && found->second.hasAlpha;
}

void Renderer::destroyTexture(std::uint32_t texture) {
    if (texture != 0) {
        const GLuint name = texture;
        glDeleteTextures(1, &name);
        textureAlpha_.erase(texture);
    }
}

} // namespace dh::foundation
