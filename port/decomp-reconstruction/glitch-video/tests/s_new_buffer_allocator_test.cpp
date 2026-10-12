// Host check for detail::SNewBufferAllocator::allocate and CTextureManager::markTextureAsUnloadable.
#include <cstdio>

#include "glitch-video/buffer.hpp"
#include "glitch-video/s_new_buffer_allocator.hpp"
#include "glitch-video/texture.hpp"
#include "glitch-video/texture_manager.hpp"

using namespace glitch::video;

int main() {
    int failures = 0;

    // allocate: the buffer keeps its identity, takes the new size and owns the new data.
    IBuffer buffer;
    buffer.m_refCount = 1;
    detail::SNewBufferAllocator<IVideoDriver> allocator{boost::intrusive_ptr<IBuffer>(&buffer)};
    auto result = allocator.allocate(16);
    if (result.get() != &buffer) { std::puts("allocate: wrong buffer"); ++failures; }
    if (buffer.m_size != 16 || buffer.m_data == nullptr) { std::puts("allocate: reset not applied"); ++failures; }
    if ((buffer.m_flags12 & IBuffer::kFlagOwnsData) == 0) { std::puts("allocate: data not owned"); ++failures; }
    if (buffer.m_refCount != 2) { std::puts("allocate: refcount not bumped"); ++failures; }
    buffer.m_data = nullptr;  // avoid freeing the test's storage through reset paths

    // markTextureAsUnloadable: appended once, a second call leaves the list alone.
    CTextureManager manager;
    ITexture texture;
    boost::intrusive_ptr<ITexture> handle(&texture);
    manager.markTextureAsUnloadable(handle);
    manager.markTextureAsUnloadable(handle);
    if (manager.m_textures.size() != 1 || manager.m_textures[0] != &texture) {
        std::puts("markTextureAsUnloadable: list not updated once");
        ++failures;
    }

    std::printf("%s\n", failures == 0 ? "ok" : "FAILED");
    return failures == 0 ? 0 : 1;
}
