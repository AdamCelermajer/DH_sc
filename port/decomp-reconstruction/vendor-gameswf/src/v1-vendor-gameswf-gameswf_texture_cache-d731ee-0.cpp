// Batch v1-vendor-gameswf-gameswf_texture_cache-d731ee-0 (gameswf::texture_cache, part 1 of 1).
//
// Gameloft class, not in upstream GameSWF r1714. Rebuilt from the Hex-Rays pseudocode and checked
// against the assembly (tools/decomp_asm.py). Layout and member offsets are in
// vendor-gameswf/include/vendor-gameswf/texture_cache.hpp. The texture is gameswf::bitmap_info; its
// DH2 vtable slots (lock +0x18, unlock +0x1c, get_width +0x24, get_height +0x28) are declared in
// gameswf.h.
#include "vendor-gameswf/texture_cache.hpp"

#include <cstddef>
#include <cstring>

namespace gameswf {
namespace {

// One cell is 16 pixels in each direction.
constexpr int kCellSize = 16;

// Rounds a pixel count up to whole cells, with a minimum of one cell. This is the round-up that the
// original does on each size in get_region_size_requirement (signed division, so a negative input
// gives the same result).
int round_up_to_cells(int pixels) {
    int cells = pixels / kCellSize;
    if (pixels % kCellSize > 0) {
        ++cells;
    }
    int size = cells * kCellSize;
    if (size < kCellSize) {
        size = kCellSize;
    }
    return size;
}

}  // namespace

// 0x794030 gameswf::texture_cache::texture_cache(int, int, int, bool)   [C1]
// 0x7941c0 gameswf::texture_cache::texture_cache(int, int, int, bool)   [C2, same body]
// PARTIAL: the member zeroing, the bytes-per-pixel field and the size rounding are written. The
// texture itself is not created, so m_texture stays null and reset() is not called (reset() reads
// the texture). The original picks one of three branches:
//   bpp == 1           : s_render_handler vtable +0x0c (create_bitmap_info_alpha) with no data.
//   bpp != 1, !render  : builds a gameswf::image_rgba of the rounded size, clears its pixels, passes
//                        it to vtable +0x14 (create_bitmap_info_rgba) and destroys the image.
//   bpp != 1, render   : vtable +0x20 with (width, height) (no upstream method matches that slot).
// Each branch then stores the texture in m_texture, calls its +0x14 virtual (set_writable) and ends
// in reset(). The glitch render handler, gameswf::image_rgba and the +0x20 slot are not in the tree.
texture_cache::texture_cache(int width, int height, int bpp, bool render_target)
    : m_bpp(bpp) {
    get_region_size_requirement(width, height);
    (void)render_target;
}

// 0x758b8c gameswf::texture_cache::~texture_cache()   [D2]
// PARTIAL: the owned free-list branch is written. When m_free_list_borrowed is set (+0x2c), the
// original keeps the free-list buffer and only clears its count. gameswf::array frees its buffer in
// its own destructor, so that branch cannot be expressed without an array that gives up ownership.
// Nothing in this batch sets the flag.
texture_cache::~texture_cache() {
    if (m_locked != nullptr) {
        m_texture->unlock();
        m_locked = nullptr;
    }
    m_texture = nullptr;
    m_used.clear();
    if (!m_free_list_borrowed) {
        m_free.resize(0);
    }
    m_regions.resize(0);
}

// 0x793560 gameswf::texture_cache::get_region_size_requirement(int&, int&)
// The function is static: the caller passes two int references (texture_cache's constructor passes
// local ints). Rounds both values up to whole cells. The return value is the height from before the
// rounding plus 15; the assembly leaves it in r0 (LDR R3,[R1] ... ADD R0,R3,#0xF).
int texture_cache::get_region_size_requirement(int& width, int& height) {
    const int result = height + 15;
    width = round_up_to_cells(width);
    height = round_up_to_cells(height);
    return result;
}

// 0x756c30 gameswf::texture_cache::get_region_bounds(gameswf::texture_cache::region const*, gameswf::rect&)
// The grid index of the region is its offset from the start of the region array in units of 16
// bytes. The texture width in cells (width >> 4) gives the row length, so the region's column is
// index & (cells_per_row - 1) and its row is index / cells_per_row. The bounds are the cell origin
// plus the region's size, in pixels.
void texture_cache::get_region_bounds(const region* r, gameswf::rect& bounds) const {
    const int cells_per_row = m_texture->get_width() >> 4;
    const int index = static_cast<int>(r - &m_regions[0]);
    const float x0 = static_cast<float>(kCellSize * ((cells_per_row - 1) & index));
    const float y0 = static_cast<float>(kCellSize * (index / cells_per_row));
    bounds.m_x_min = x0;
    bounds.m_x_max = x0 + static_cast<float>(kCellSize * r->cells_w);
    bounds.m_y_min = y0;
    bounds.m_y_max = y0 + static_cast<float>(kCellSize * r->cells_h);
}

// 0x79352c gameswf::texture_cache::verify_integrity(gameswf::texture_cache::region*)
// The region argument is not read. The original calls the texture's get_width (vtable +0x24) and
// discards the result, then returns get_height (vtable +0x28).
int texture_cache::verify_integrity(const region*) const {
    (void)m_texture->get_width();
    return m_texture->get_height();
}

// 0x793d78 gameswf::texture_cache::verify_integrity()
// Runs the region probe once per free region (the free-list entry m_free[i] is passed as the region
// argument) and once per used region (the entry's region pointer is passed). It also builds a local set
// of the used regions and clears it at the end. Nothing is compared or reported in the original.
void texture_cache::verify_integrity() {
    for (int i = 0; i < m_free.size(); ++i) {
        verify_integrity(m_free[i]);
    }
    ::hash<region*, region*, ::fixed_size_hash<region*>> seen;
    for (auto it = m_used.begin(); it != m_used.end(); ++it) {
        region* used = it->second;
        verify_integrity(used);
        seen[used] = used;
    }
    seen.clear();
}

// 0x793ed8 gameswf::texture_cache::reset()
// Empties the used regions, advances the clock (both clock fields take the new value) and clears the
// locked pixels if the texture is locked. The region grid is reset to one free region covering the
// whole texture, in cells (width / 16 by height / 16).
void texture_cache::reset() {
    m_used.clear();
    m_free.resize(0);
    ++m_clock;
    m_last_clock = m_clock;

    const int width = m_texture->get_width();
    const int height = m_texture->get_height();
    const int cells_w = width / kCellSize;
    const int cells_h = height / kCellSize;
    if (m_locked != nullptr) {
        std::memset(m_locked, 0, static_cast<std::size_t>(m_bpp) * height * width);
    }
    m_regions.resize(cells_w * cells_h);
    region& whole = m_regions[0];
    whole.last_clock = 0;
    whole.cells_w = cells_w;
    whole.cells_h = cells_h;
    m_free.push_back(&whole);
}

// 0x794350 gameswf::texture_cache::subdivide_region(gameswf::texture_cache::region*, int, int)
// Cuts region r down to cells_w x cells_h. The strip to the right (same row, index + cells_w) and
// the strip below (index + cells_h * cells_per_row) become free regions with a zero clock, when they
// are not empty.
void texture_cache::subdivide_region(region* r, int cells_w, int cells_h) {
    const int old_w = r->cells_w;
    const int old_h = r->cells_h;
    const int cells_per_row = m_texture->get_width() / kCellSize;
    const int index = static_cast<int>(r - &m_regions[0]);

    if (old_w - cells_w > 0) {
        region* right = &m_regions[index + cells_w];
        right->cells_w = old_w - cells_w;
        right->cells_h = cells_h;
        right->last_clock = 0;
        m_free.push_back(right);
    }
    if (old_h - cells_h > 0) {
        region* below = &m_regions[index + cells_h * cells_per_row];
        below->cells_w = old_w;
        below->cells_h = old_h - cells_h;
        below->last_clock = 0;
        m_free.push_back(below);
    }
    r->cells_h = cells_h;
    r->cells_w = cells_w;
}

// 0x794488 gameswf::texture_cache::find_used_region(int, int)
// Scans the used regions in table order. A region qualifies when it is at least as large as the
// request (in cells) and its clock is the oldest so far; on a tie the earlier entry is kept. The
// winner is erased from the used hash and cut down to the request. The clock is copied to
// m_last_clock.
texture_cache::region* texture_cache::find_used_region(int width, int height) {
    const int cells_w = width / kCellSize;
    const int cells_h = height / kCellSize;

    region* best = nullptr;
    key best_key;
    for (auto it = m_used.begin(); it != m_used.end(); ++it) {
        region* candidate = it->second;
        if (cells_w > candidate->cells_w || cells_h > candidate->cells_h) {
            continue;
        }
        if (best != nullptr && best->last_clock <= candidate->last_clock) {
            continue;
        }
        best = candidate;
        best_key = it->first;
    }
    if (best == nullptr) {
        return nullptr;
    }

    m_used.erase(best_key);
    if (cells_w < best->cells_w || cells_h < best->cells_h) {
        subdivide_region(best, cells_w, cells_h);
    }
    m_last_clock = m_clock;
    return best;
}

// 0x794668 gameswf::texture_cache::find_available_region(int, int)
// Scans the free list for a region that fits the request (in cells). The one kept is the first
// candidate, replaced by a later one whose width or height is smaller (the original's test is an OR).
// The winner leaves the free list and is cut down to the request.
texture_cache::region* texture_cache::find_available_region(int width, int height) {
    const int cells_w = width / kCellSize;
    const int cells_h = height / kCellSize;
    const int count = m_free.size();
    if (count <= 0) {
        return nullptr;
    }

    int best = -1;
    for (int i = 0; i != count; ++i) {
        const region* candidate = m_free[i];
        if (cells_w <= candidate->cells_w && cells_h <= candidate->cells_h) {
            if (best == -1 || candidate->cells_w < m_free[best]->cells_w ||
                candidate->cells_h < m_free[best]->cells_h) {
                best = i;
            }
        }
    }
    if (best == -1) {
        return nullptr;
    }

    region* found = m_free[best];
    m_free.remove(best);
    if (cells_w < found->cells_w || cells_h < found->cells_h) {
        subdivide_region(found, cells_w, cells_h);
    }
    return found;
}

// 0x7c44d8 gameswf::texture_cache::lock()
// PARTIAL: the lock is not taken. The original calls the texture's lock (vtable +0x18), stores the
// returned pixel pointer in m_locked, and when the used hash is empty clears the pixels with
// bytes-per-pixel * height * width. bitmap_info declares lock() as int (gameswf.h, DH2), so the
// pointer cannot be carried on a 64-bit host yet. The clear is written as reset() writes it, but
// needs m_locked to be set first. Returns m_locked unchanged for now.
void* texture_cache::lock() {
    return m_locked;
}

// 0x7935ec gameswf::texture_cache::unlock_all(gameswf::player_context*)
// PARTIAL: the original is static. Its argument is the player context, and r1 is not read. It reaches
// two texture caches through player_context fields that are not in the tree: the object at +0x0c has
// a cache at +0x28, and the object at +0x10 has a cache at +0x0c. For each cache, if m_locked is set,
// the original calls the texture's unlock (vtable +0x1c) and clears m_locked, as the destructor does.
// Nothing is unlocked here until the player context layout is known.
void texture_cache::unlock_all(player_context*) {
}

}  // namespace gameswf
