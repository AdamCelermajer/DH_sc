#include "glitch-video/null_shader.hpp"
#include "glitch-video/null_shader_manager.hpp"
#include "glitch-video/shader_code.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch::video;

int main() {
    // Shader code table: insert, find by name, unknown name gives null.
    ICodeShaderManager codes;
    auto a = boost::intrusive_ptr<IShaderCode>(new IShaderCode("alpha"));
    auto b = boost::intrusive_ptr<IShaderCode>(new IShaderCode("beta"));
    codes.addShaderCode(a);
    codes.addShaderCode(b);
    CHECK(codes.getShaderCode("alpha").get() == a.get());
    CHECK(codes.getShaderCode("beta").get() == b.get());
    CHECK(!codes.getShaderCode("gamma"));
    CHECK(a->m_refCount == 0);
    codes.removeUnusedShaderCodes();
    CHECK(!codes.getShaderCode("alpha"));

    // Named shader: hash is the id, mask is 1, dirty flag is cleared by updateHash.
    CNullShader named(42, "shader", nullptr);
    CHECK(named.getHashValue() == 42);
    CHECK(named.m_vertexAttributeMask == 1);
    CHECK(!named.m_hashDirty);
    named.m_id = 9;
    named.m_hashDirty = true;
    CHECK(named.getHashValue() == 9);   // dirty -> recomputed
    CHECK(named.getShaderSize() == 0);  // size is not computed by updateHash

    // Manager: same name yields the same shader; removeUnusedShaders empties it.
    CNullShaderManager manager(nullptr);
    auto s1 = manager.createShader("s");
    auto s2 = manager.createShader("s");
    CHECK(s1.get() == s2.get());
    CHECK(manager.createEmptyShader("ignored").get() != s1.get());
    manager.removeUnusedShaders();
    CHECK(manager.m_shaders.empty());

    std::printf("%s\n", failures ? "shader_code: FAILED" : "shader_code: ok");
    return failures != 0;
}
