#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dh2::native_visual_fx_manager_v1 {

struct Bytes { const std::uint8_t* data{}; std::size_t size{}; };

struct EffectStep {
    std::int32_t effect_dict_id{-1};
    bool force_cancel{};
    std::int32_t loop{};
    bool orient_once{};
    bool orient_with_anchor{};
    std::int32_t play_time{-1};
    std::int32_t pool_size{};
    std::int32_t redirect{};
    bool scale_with_anchor{true};
    bool self_illum{true};
    float speed{1.0f};
    std::string sub_object;
};

struct EffectSequence {
    std::uint32_t row{};
    std::string name;
    bool template_flag{};
    std::int32_t loop{};
    std::vector<EffectStep> steps;
    std::int32_t type{};
};

struct Catalog {
    std::vector<EffectSequence> sequences;
    std::vector<std::string> effect_names;
    std::vector<std::string> effect_paths;
    std::vector<std::string> dictionary_names;
    // The source reader finishes the first effect-array section at byte 12700;
    // the original file has a 253-byte suffix used by another serialized
    // section. Retain both values so callers do not mistake it for corruption.
    std::size_t effect_array_consumed{};
    std::size_t effect_array_suffix{};
};

bool load_catalog(Bytes effect_array, Bytes effect_names, Bytes effect_fields,
                  Bytes effect_dictionary, Bytes dictionary_names,
                  Catalog& output, std::string& error);

struct Backend {
    void* context{};
    // create must construct and load the engine's AnimatedFX for this exact
    // EffectDict row/path. It must not return success without an instance.
    bool (*create)(void*, std::int32_t effect_dict_id,
                   std::string_view source_path, void** instance,
                   std::string& error){};
    void (*destroy)(void*, void* instance){};
    // Applies the selected source AnimatedEffectTable row and performs the
    // source GrabAnimFX start sequence on the already loaded instance.
    bool (*start_sequence)(void*, void* instance,
                           const EffectSequence& sequence,
                           void* source_game_object,
                           std::string& error){};
    // DropAnimatedFX's reset path, including source/object detachment and
    // Irrlicht synchronization. This is distinct from objective-marker hide.
    bool (*reset_for_pool)(void*, void* instance, std::string& error){};
    bool (*bind_game_object)(void*, void* instance, void* object,
                             std::string& error){};
    bool (*bind_animator_character)(void*, void* instance, void* character,
                                    std::string& error){};
    bool (*sync_irr_data)(void*, void* instance, bool force,
                          std::string& error){};
    bool (*set_visible)(void*, void* instance, bool visible,
                        std::string& error){};
    bool (*resume_animator)(void*, void* instance,
                            std::string& error){};
    // One source manager frame tick per active FX. The backend advances the
    // attached animation/scene resources; source completion requests a pool
    // drop through DropAnimatedFX after this callback returns.
    bool (*update)(void*, void* instance, const EffectSequence& sequence,
                   bool& drop_to_pool, std::string& error){};
    // Renderer-side submission for an active instance. A backend that uses
    // the shared scene renderer may register its scene node there and leave
    // this callback empty; otherwise the manager's render phase calls it.
    bool (*render)(void*, void* instance, std::string& error){};
    // Set only when the concrete backend registers its AnimatedFX scene nodes
    // with the shared renderer and submits them outside Owner::Render.
    bool draw_registered_by_scene{};
};

enum class Status : std::uint8_t {
    ok, invalid_request, catalog_missing, backend_missing, load_failed,
    start_failed, pool_exhausted, stale_instance, lifecycle_failed,
    allocation_failed
};

struct PoolSnapshot {
    std::int32_t effect_dict_id{-1};
    std::size_t allocated{};
    std::size_t active{};
    std::size_t free{};
    std::size_t source_limit{6};
};

// One level-owned projection of the source VisualFXManager. It owns one
// AnimatedFX allocation pool per EffectDict row (six maximum), while the
// engine backend owns the actual scene/animation objects. Objective records
// retain the returned opaque AnimatedFX* at source Objective+0x28. Hiding an
// objective marker does not release that pointer; DropAnimatedFX or manager
// flush does.
class Owner final {
public:
    explicit Owner(const Catalog* catalog = nullptr) noexcept : catalog_(catalog) {}
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    ~Owner();

    void bind_catalog(const Catalog* catalog) noexcept { catalog_ = catalog; }
    void bind_backend(Backend backend) noexcept { backend_ = backend; }
    Status GrabAnimFX(std::int32_t effect_table_id, void* source_game_object,
                      void** output, std::string& error);
    Status DropAnimatedFX(void** instance, std::string& error);
    Status BindCharacter(void* instance, void* character, std::string& error);
    Status BindAnimatorCharacter(void* instance, void* character,
                                 std::string& error);
    Status SyncIrrData(void* instance, bool force, std::string& error);
    Status SetVisible(void* instance, bool visible, std::string& error);
    Status ResumeAnimator(void* instance, std::string& error);
    Status Update(std::string& error);
    Status Render(std::string& error);
    // Objective+0x28 is the single persistent handle slot. Install invokes
    // source RemoveObjectiveMarker on any prior handle (detach/hide but keep
    // it owned), then overwrites the slot with the new GrabAnimFX result before
    // applying the source marker attachment sequence. Source pool exhaustion
    // is an actual null Grab result and therefore clears Objective+0x28;
    // missing backend/catalog errors do not pretend a source Grab occurred.
    // Remove deliberately leaves the retained slot untouched.
    Status InstallObjectiveMarker(std::uintptr_t* objective_fx_slot,
                                  std::int32_t effect_table_id,
                                  void* target_character,
                                  void* source_game_object,
                                  std::string& error);
    Status RemoveObjectiveMarker(std::uintptr_t* objective_fx_slot,
                                 void* target_character,
                                 std::string& error);
    void Flush() noexcept;
    PoolSnapshot pool(std::int32_t effect_dict_id) const noexcept;
    const EffectSequence* sequence(std::int32_t effect_table_id) const noexcept;

private:
    struct Slot {
        void* instance{};
        std::int32_t effect_dict_id{-1};
        std::int32_t effect_table_id{-1};
        bool active{};
    };
    Slot* find(void* instance) noexcept;
    const Slot* find(void* instance) const noexcept;
    const Catalog* catalog_{};
    Backend backend_{};
    std::vector<Slot> slots_;
};

} // namespace dh2::native_visual_fx_manager_v1
