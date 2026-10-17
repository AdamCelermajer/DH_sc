// Standalone Android smoke check for the new checked asset readers.
// Synthetic fixtures are generated here; no proprietary game bytes are embedded.
#include "../texture-assets/texture.hpp"
#include "../material-bindings/bindings.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
using dh2::textures::TextureView;
using dh2::textures::Format;
using dh2::resources::BresView;
using dh2::resources::Library;
using dh2::materials::Image;
using dh2::materials::Effect;
using dh2::materials::EffectGroup;
using dh2::materials::ImageRef;
using dh2::materials::Material;
using dh2::materials::Parameter;

#define CHECK(condition, label) do { if (!(condition)) { \
    std::fprintf(stderr, "FAIL: %s\n", label); return false; \
} } while (0)

void put32(std::uint8_t* bytes, std::size_t offset, std::uint32_t value) {
    for (unsigned i = 0; i != 4; ++i)
        bytes[offset + i] = static_cast<std::uint8_t>(value >> (i * 8));
}

std::uint32_t append_string(std::uint8_t* bytes, std::size_t& end, const char* value) {
    const auto offset = static_cast<std::uint32_t>(end);
    const auto length = std::strlen(value) + 1;
    std::memcpy(bytes + end, value, length);
    end += length;
    return offset;
}

bool synthetic_texture() {
    std::uint8_t raw[92]{}; // 8-byte BTEX, 52-byte PVR v2 header, 32-byte 4bpp payload.
    std::memcpy(raw, "BTEXpvr\0", 8);
    put32(raw, 8, 52);        // PVR header length.
    put32(raw, 12, 8);        // Height.
    put32(raw, 16, 8);        // Width.
    put32(raw, 24, 25);       // PVRTC1 4bpp pixel type.
    put32(raw, 28, 32);       // Encoded data length.
    put32(raw, 32, 4);        // Bits per pixel.
    put32(raw, 52, 0x21525650); // PVR! tag.
    put32(raw, 56, 1);        // Surface count.
    for (unsigned i = 0; i != 4; ++i) {
        put32(raw, 60 + i * 8, 0);          // Select A endpoint everywhere.
        put32(raw, 64 + i * 8, 0x801ffc00); // Opaque red A, blue B.
    }
    TextureView view{};
    CHECK(dh2_texture_open(&view, raw, sizeof(raw)) == dh2::textures::Error::ok,
          "synthetic BTEX/PVR open");
    CHECK(view.width == 8 && view.height == 8 && view.payload_offset == 60 &&
          view.payload_size == 32 && view.format == Format::pvrtc_4bpp,
          "synthetic BTEX/PVR fields");
    std::uint8_t rgba[8 * 8 * 4]{};
    CHECK(dh2_texture_decode_rgba8(rgba, sizeof(rgba), 32, raw, sizeof(raw)) ==
          dh2::textures::Error::ok, "synthetic PVRTC decode");
    for (std::size_t i = 0; i < sizeof(rgba); i += 4)
        CHECK(rgba[i] == 255 && rgba[i + 1] == 0 && rgba[i + 2] == 0 &&
              rgba[i + 3] == 255, "synthetic PVRTC red pixels");
    CHECK(dh2_texture_open(&view, raw, sizeof(raw) - 1) ==
          dh2::textures::Error::invalid_payload, "truncated BTEX rejected");
    // The same four compressed words form a 16x8 PVRTC1 2bpp image.
    put32(raw, 16, 16);
    put32(raw, 24, 24);
    put32(raw, 32, 2);
    CHECK(dh2_texture_open(&view, raw, sizeof(raw)) == dh2::textures::Error::ok &&
          view.format == Format::pvrtc_2bpp, "synthetic 2bpp open");
    std::uint8_t rgba2[16 * 8 * 4]{};
    CHECK(dh2_texture_decode_rgba8(rgba2, sizeof(rgba2), 64, raw, sizeof(raw)) ==
          dh2::textures::Error::ok, "synthetic 2bpp decode");
    for (std::size_t i = 0; i < sizeof(rgba2); i += 4)
        CHECK(rgba2[i] == 255 && rgba2[i + 1] == 0 && rgba2[i + 2] == 0 &&
              rgba2[i + 3] == 255, "synthetic 2bpp red pixels");
    return true;
}

bool synthetic_material() {
    std::uint8_t raw[640]{};
    std::memcpy(raw, "BRES", 4);
    raw[4] = 0xfe; raw[5] = 0xff;
    put32(raw, 8, 60);            // BRES header size.
    put32(raw, 12, sizeof(raw));
    put32(raw, 16, 1);            // One valid header fixup.
    put32(raw, 24, 60);           // Fixup table starts at 60.
    put32(raw, 28, 64);           // End of fixup table.
    put32(raw, 32, 64);           // Root record.
    put32(raw, 36, sizeof(raw));  // Empty tail.
    put32(raw, 60, 24);           // Header field 24 is the fixup.
    constexpr std::size_t root = 64, images = 256, effects = 276;
    constexpr std::size_t materials = 392, group_images = 428;
    constexpr std::size_t parameters = 432, count_header = 456;
    constexpr std::size_t value_offset = 460, index_offset = 464;
    put32(raw, root + 0x4c, 1); put32(raw, root + 0x50, images);
    put32(raw, root + 0x54, 1); put32(raw, root + 0x58, effects);
    put32(raw, root + 0x5c, 1); put32(raw, root + 0x60, materials);
    std::size_t cursor = 468;
    const auto image_id = append_string(raw, cursor, "image0");
    const auto image_name = append_string(raw, cursor, "sample image");
    const auto image_path = append_string(raw, cursor, "data/3d/textures/example.tga");
    const auto effect_id = append_string(raw, cursor, "effect0");
    const auto effect_name = append_string(raw, cursor, "sample effect");
    const auto material_id = append_string(raw, cursor, "material0");
    const auto material_name = append_string(raw, cursor, "sample material");
    const auto effect_url = append_string(raw, cursor, "#effect0");
    const auto sampler_id = append_string(raw, cursor, "diffuse-sampler");
    const auto semantic = append_string(raw, cursor, "diffuse");
    CHECK(cursor <= sizeof(raw), "synthetic BRES string capacity");
    put32(raw, images, image_id); put32(raw, images + 4, image_name);
    put32(raw, images + 8, image_path);
    put32(raw, effects, effect_id); put32(raw, effects + 4, effect_name);
    for (std::size_t group = 0; group != 2; ++group) {
        const auto base = effects + 8 + group * 24;
        put32(raw, base + 16, 1); put32(raw, base + 20, group_images);
    }
    put32(raw, group_images, 0);
    put32(raw, materials, material_id); put32(raw, materials + 4, material_name);
    put32(raw, materials + 12, effect_url);
    put32(raw, materials + 16, 1); put32(raw, materials + 20, parameters);
    put32(raw, parameters, sampler_id); put32(raw, parameters + 4, semantic);
    put32(raw, parameters + 8, 11); put32(raw, parameters + 12, 1);
    put32(raw, parameters + 16, count_header);
    put32(raw, parameters + 20, value_offset);
    put32(raw, count_header, 1); put32(raw, value_offset, index_offset);
    put32(raw, index_offset, 0);

    BresView view{};
    CHECK(dh2_bres_open(&view, raw, sizeof(raw)) == dh2::resources::BresError::ok,
          "synthetic BRES open");
    CHECK(dh2_bres_library_count(&view, Library::image) == 1 &&
          dh2_bres_library_count(&view, Library::effect) == 1 &&
          dh2_bres_library_count(&view, Library::material) == 1,
          "synthetic BRES library counts");
    Image image{};
    CHECK(dh2_image_record(&image, &view, 0) == dh2::materials::Error::ok &&
          std::strcmp(image.source_path, "data/3d/textures/example.tga") == 0,
          "synthetic image path");
    Effect effect{};
    CHECK(dh2_effect_record(&effect, &view, 0) == dh2::materials::Error::ok,
          "synthetic effect record");
    Material material{};
    CHECK(dh2_material_record(&material, &view, 0) == dh2::materials::Error::ok &&
          dh2_material_local_effect(&material) == 0, "synthetic local effect link");
    Parameter parameter{};
    CHECK(dh2_material_parameter(&parameter, &material, 0) ==
          dh2::materials::Error::ok && parameter.type_code == 11,
          "synthetic sampler parameter");
    ImageRef sampler{};
    CHECK(dh2_material_sampler_image(&sampler, &material, 0) ==
          dh2::materials::Error::ok && sampler.index == 0 &&
          std::strcmp(sampler.source_path, image.source_path) == 0,
          "synthetic material sampler image");
    for (int i = 0; i != 2; ++i) {
        EffectGroup group{};
        ImageRef referenced{};
        CHECK(dh2_effect_group(&group, &effect, i) == dh2::materials::Error::ok &&
              group.image_count == 1 &&
              dh2_effect_group_image(&referenced, &group, 0) ==
                  dh2::materials::Error::ok && referenced.index == 0,
              "synthetic effect-group image");
    }
    put32(raw, index_offset, 1); // In-range BRES pointer, invalid image index.
    CHECK(dh2_material_sampler_image(&sampler, &material, 0) ==
          dh2::materials::Error::layout, "invalid sampler index rejected");
    CHECK(dh2_bres_open(&view, raw, 59) ==
          dh2::resources::BresError::short_header, "short BRES rejected");
    return true;
}

bool read_file(const char* name, std::uint8_t** out, std::size_t* size) {
    *out = nullptr; *size = 0;
    std::FILE* file = std::fopen(name, "rb");
    CHECK(file, "open optional external fixture");
    const bool length_ok = std::fseek(file, 0, SEEK_END) == 0;
    const long length = length_ok ? std::ftell(file) : -1;
    if (length <= 0 || length > 32L * 1024L * 1024L ||
        std::fseek(file, 0, SEEK_SET) != 0) {
        std::fclose(file);
        CHECK(false, "optional fixture length");
    }
    auto* bytes = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(length)));
    if (!bytes) {
        std::fclose(file);
        CHECK(false, "optional fixture allocation");
    }
    const bool complete = std::fread(bytes, 1, static_cast<std::size_t>(length), file) ==
                          static_cast<std::size_t>(length);
    std::fclose(file);
    if (!complete) { std::free(bytes); CHECK(false, "optional fixture read"); }
    *out = bytes; *size = static_cast<std::size_t>(length);
    return true;
}

bool external_texture(const char* name) {
    std::uint8_t* bytes{}; std::size_t size{};
    if (!read_file(name, &bytes, &size)) return false;
    TextureView view{};
    const auto opened = dh2_texture_open(&view, bytes, size);
    if (opened != dh2::textures::Error::ok) { std::free(bytes); CHECK(false, "external texture open"); }
    if (view.format == Format::pvrtc_2bpp || view.format == Format::pvrtc_4bpp) {
        const auto count = static_cast<std::size_t>(view.width) * view.height * 4;
        auto* rgba = static_cast<std::uint8_t*>(std::malloc(count));
        if (!rgba) { std::free(bytes); CHECK(false, "external texture RGBA allocation"); }
        const auto status = dh2_texture_decode_rgba8(rgba, count, view.width * 4,
                                                       bytes, size);
        std::free(rgba);
        if (status != dh2::textures::Error::ok) {
            std::free(bytes); CHECK(false, "external texture decode");
        }
    }
    std::printf("external texture: %ux%u format=%u bytes=%zu\n", view.width,
                view.height, static_cast<unsigned>(view.format), size);
    std::free(bytes);
    return true;
}

bool external_bres(const char* name) {
    std::uint8_t* bytes{}; std::size_t size{};
    if (!read_file(name, &bytes, &size)) return false;
    BresView view{};
    if (dh2_bres_open(&view, bytes, size) != dh2::resources::BresError::ok) {
        std::free(bytes); CHECK(false, "external BRES open");
    }
    const auto image_count = dh2_bres_library_count(&view, Library::image);
    const auto effect_count = dh2_bres_library_count(&view, Library::effect);
    const auto material_count = dh2_bres_library_count(&view, Library::material);
    for (std::uint32_t i = 0; i < image_count; ++i) {
        Image item{};
        if (dh2_image_record(&item, &view, i) != dh2::materials::Error::ok) {
            std::free(bytes); CHECK(false, "external BRES image");
        }
    }
    for (std::uint32_t i = 0; i < effect_count; ++i) {
        Effect item{};
        if (dh2_effect_record(&item, &view, i) != dh2::materials::Error::ok) {
            std::free(bytes); CHECK(false, "external BRES effect");
        }
        for (int group_index = 0; group_index != 2; ++group_index) {
            EffectGroup group{};
            if (dh2_effect_group(&group, &item, group_index) != dh2::materials::Error::ok) {
                std::free(bytes); CHECK(false, "external BRES effect group");
            }
            for (std::uint32_t j = 0; j < group.image_count; ++j) {
                ImageRef ref{};
                if (dh2_effect_group_image(&ref, &group, j) != dh2::materials::Error::ok) {
                    std::free(bytes); CHECK(false, "external BRES effect image");
                }
            }
        }
    }
    for (std::uint32_t i = 0; i < material_count; ++i) {
        Material item{};
        if (dh2_material_record(&item, &view, i) != dh2::materials::Error::ok) {
            std::free(bytes); CHECK(false, "external BRES material");
        }
        for (std::uint32_t j = 0; j < item.parameter_count; ++j) {
            Parameter parameter{};
            if (dh2_material_parameter(&parameter, &item, j) !=
                dh2::materials::Error::ok) {
                std::free(bytes); CHECK(false, "external BRES material parameter");
            }
            if (parameter.type_code == 11) {
                ImageRef ref{};
                if (dh2_material_sampler_image(&ref, &item, j) !=
                    dh2::materials::Error::ok) {
                    std::free(bytes); CHECK(false, "external BRES sampler image");
                }
            }
        }
    }
    std::printf("external BRES: images=%u effects=%u materials=%u bytes=%zu\n",
                image_count, effect_count, material_count, size);
    std::free(bytes);
    return true;
}
} // namespace

int main(int argc, char** argv) {
    if (argc > 3) {
        std::fprintf(stderr, "usage: %s [texture.tga [model.bdae]]\n", argv[0]);
        return 2;
    }
    if (!synthetic_texture() || !synthetic_material()) return 1;
    std::puts("synthetic texture and material checks passed");
    if (argc >= 2 && !external_texture(argv[1])) return 1;
    if (argc >= 3 && !external_bres(argv[2])) return 1;
    return 0;
}
