#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace gameswf { struct glyph_provider; }
namespace dh2::ui {
struct SwfFontServices {
    void* context{};
    // Genuine caller font-name/style resolution. False is a provider miss/error;
    // no fallback path or substitute font is chosen by this module.
    bool (*read)(void*,const char* name,bool bold,bool italic,std::vector<std::uint8_t>&,std::string&){};
    void (*diagnostic)(void*,const char* message){};
};
class SwfFreetypeProvider {
public:
    explicit SwfFreetypeProvider(const SwfFontServices&,float source_scale=1.0f);
    ~SwfFreetypeProvider();
    SwfFreetypeProvider(const SwfFreetypeProvider&)=delete;
    SwfFreetypeProvider& operator=(const SwfFreetypeProvider&)=delete;
    // Supply to SwfServices.glyphs. Caller resolver context and image texture owner
    // must outlive all movies. No global provider is installed/cleared here.
    gameswf::glyph_provider* borrowed_provider() const;
    std::string error() const;
private:
    struct Impl;std::unique_ptr<Impl> impl_;
};
}
