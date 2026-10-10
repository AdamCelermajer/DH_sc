#include "original_actor_labels.hpp"
#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "../engine-ui/localization.hpp"
#include "../script-runtime/script_constants.hpp"
#include <cstring>
#include <limits>
#include <stdexcept>
#include <utility>
namespace dh::foundation {
struct OriginalActorLabels::Impl {
    std::unique_ptr<AssetCatalog> assets;
    std::string root;
    dh2::ui::Localization localization;
    dh2_script_constants* constants = dh2_script_constants_create();
    bool ready = false;
    ~Impl() { dh2_script_constants_destroy(constants); }
    dh2::ui::LocalizationServices services() {
        dh2::ui::LocalizationServices s;
        s.context = this;
        s.open = [](void* c, const char* uri, bool& found, std::vector<std::uint8_t>& bytes,
                    std::uintptr_t& lease, std::string& error) {
            auto& self = *static_cast<Impl*>(c);
            found = false; lease = 0;
            try { bytes = read_content(*self.assets, self.root + "/" + uri);
                found = true; lease = 1; error.clear(); return true;
            } catch (const std::exception& e) { error = e.what(); return false; }
        };
        s.close = [](void*, std::uintptr_t lease, std::string& error) {
            if (lease != 1) { error = "Invalid label resource lease"; return false; }
            return true;
        };
        // Standalone text decoding has no debug state; callback is explicit.
        s.debug = [](void*, const char*, std::string&) { return true; };
        s.constant = [](void* c, const char* group, const char* key,
                        std::uint32_t& value, std::string& error) {
            std::int32_t raw = 0;
            if (dh2_script_constants_get(static_cast<Impl*>(c)->constants, group, key, &raw)) {
                error = std::string("Missing original localization constant: ") + group + "." + key;
                return false;
            }
            std::memcpy(&value, &raw, 4); return true;
        };
        return s;
    }
};
OriginalActorLabels::OriginalActorLabels() : impl_(std::make_unique<Impl>()) {}
OriginalActorLabels::~OriginalActorLabels() = default;
OriginalActorLabels::OriginalActorLabels(OriginalActorLabels&&) noexcept = default;
OriginalActorLabels& OriginalActorLabels::operator=(OriginalActorLabels&&) noexcept = default;
bool OriginalActorLabels::load(const AssetCatalog& assets, const std::string& root,
                              std::int32_t pack, std::string& error) {
    try {
        auto next = std::make_unique<Impl>();
        if (!next->constants) throw std::runtime_error("Original text constant allocation failed");
        next->assets = std::make_unique<AssetCatalog>(assets);
        next->root = normalize_content_uri(root);
        auto read = [&](const char* file) { return read_content(assets, next->root + "/pydata/" + file); };
        auto records = read("common_text_pyarray.bin"), names = read("common_text_pyarraynames.bin"),
             schema = read("common_text_pystructnames.bin"), constants = read("common_text_pycst.bin");
        dh2_script_constants_reload receipt{};
        if (constants.size() > UINT32_MAX ||
            dh2_script_constants_load(next->constants, constants.data(), std::uint32_t(constants.size()), &receipt))
            throw std::runtime_error("Original text constants rejected");
        auto span = [](const auto& b) { return dh2::ui::LocalizationBytes{b.data(), b.size()}; };
        if (!next->localization.load(span(records), span(names), span(schema), error) ||
            !next->localization.switch_pack(pack, false, error)) return false;
        next->ready = true; impl_ = std::move(next); error.clear(); return true;
    } catch (const std::exception& e) { error = e.what(); return false; }
}
bool OriginalActorLabels::string_id(std::int32_t id, std::string& out, std::string& error) {
    if (!impl_ || !impl_->ready || id < 0) { error = "Original actor name source unavailable"; return false; }
    std::string next; auto services = impl_->services();
    if (!impl_->localization.string_id(std::uint32_t(id), services, next, error)) return false;
    if (next == "#!WTF!#" || next == "#!SNL!#") { error = "Original actor name ID has no localized text"; return false; }
    out = std::move(next); return true;
}
bool OriginalActorLabels::display_name(const OriginalActorLabelInput& actor,
                                     std::string& out, std::string& error) {
    if (actor.player) {
        if (actor.saved_player_name.empty() || actor.saved_player_name.find('\0') != std::string::npos) {
            error = "Player display name requires original save name"; return false;
        }
        out = actor.saved_player_name; error.clear(); return true;
    }
    return string_id(actor.name_id, out, error);
}
bool OriginalActorLabels::label(const OriginalActorLabelInput& actor,
                              OriginalActorLabel& out, std::string& error) {
    OriginalActorLabel next;
    if (!display_name(actor, next.name, error)) return false;
    // ASR8 is floor division for negative values; no implementation-defined shift.
    next.numeric_level = actor.level_raw / 256 - (actor.level_raw < 0 && actor.level_raw % 256 != 0);
    next.level = actor.boss || next.numeric_level == -1 ? "??" : std::to_string(next.numeric_level);
    out = std::move(next); error.clear(); return true;
}
}
