#pragma once

#include <cstdint>

namespace dh::foundation {

// Renderer-affinity witness for the current host context, drawable and thread.
// Zero means that the requested resource is not current or available.
std::uintptr_t current_render_context_identity() noexcept;
std::uintptr_t current_render_drawable_identity() noexcept;
std::uint64_t current_render_thread_identity() noexcept;

} // namespace dh::foundation
