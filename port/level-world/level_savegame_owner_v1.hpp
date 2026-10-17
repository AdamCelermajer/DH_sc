#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

namespace dh2::level_savegame_owner_v1 {

struct Object {
    std::uintptr_t identity{};
    std::string name;          // GameObject name written by __SaveObjects.
    std::string manager_key;   // ObjectManager tree key's retained name.
    std::int32_t type_word{};  // GameObject +0x64.
    std::uint8_t enabled{};    // GameObject +0x28 save gate.
    std::uint8_t is_character{};
    std::uint8_t is_player{};
    std::uint8_t is_local_player{};
    std::uint8_t source_flag_129{};
};

struct SerializeServices {
    void* context{};
    // Character/GameObject virtual slot +0x10 (Serialize). The provider must
    // serialize this same object's source fields; profile Save is unrelated.
    bool (*serialize)(void*, std::uintptr_t,
                      std::vector<std::uint8_t>&, std::string&){};
};

struct GameObjectFields {
    std::uint8_t object_base_flag_80{};
    std::uint8_t object_base_flag_8a{};
    std::int32_t game_object_word_270{};
};
struct TriggerFields {
    GameObjectFields game_object{};
    std::int32_t pending_3b8{};
    std::int32_t max_count_3b4{};
};
struct ContainerFields {
    GameObjectFields game_object{};
    std::uint8_t state_394{};
};
struct DoorFields {
    GameObjectFields game_object{};
    std::int32_t state_3a8{};
};
bool serialize_game_object(const GameObjectFields*,
                           std::vector<std::uint8_t>& output,
                           std::string& error);
// Trigger::Serialize at ELF 0x398978: GameObject's exact six-byte payload,
// then source +0x3b8 and +0x3b4 words in that order.
bool serialize_trigger(const TriggerFields*,
                       std::vector<std::uint8_t>& output,
                       std::string& error);
// Container::Serialize at ELF 0x39f8e0 appends its +0x394 state byte.
bool serialize_container(const ContainerFields*,
                         std::vector<std::uint8_t>& output,
                         std::string& error);
// Door::Serialize at ELF 0x3e7b74 appends its +0x3a8 DoorStates word.
bool serialize_door(const DoorFields*,
                    std::vector<std::uint8_t>& output,
                    std::string& error);
bool serialize_info_word(std::int32_t source_info_word,
                         std::vector<std::uint8_t>& output,
                         std::string& error);

enum class Status : std::uint32_t {
    complete, invalid_argument, provider_unavailable, provider_failed,
    size_overflow, allocation_failed
};

struct Result {
    std::uint32_t source_objects{};
    std::uint32_t saved_objects{};
    std::uint32_t serialized_bytes{};
};

// LevelSavegame::__SaveObjects at ELF 0x462d84. `objects` must be the single
// ObjectManager map walk in ascending signed source-handle order. Result bytes
// are committed only after every eligible object's virtual Save provider
// succeeds.
Status serialize_objects(const Object* objects, std::size_t count,
                         std::uint32_t online,
                         const SerializeServices*,
                         std::vector<std::uint8_t>& output,
                         Result*, std::string& error);

// LevelSavegame::GetFilename's source sprintf template. Inputs retain source
// argument order; do not infer slot/hub/row/difficulty from renderer state.
bool format_filename(std::uint32_t source_arg0, std::int32_t source_arg1,
                     std::int32_t source_arg2, std::int32_t source_arg3,
                     std::string& output, std::string& error);

// LevelSavegame::GetCheckpointFilename at ELF 0x4622c8. Checkpoints use the
// same LevelSavegame INFO/OBJS image under a distinct slot/difficulty name.
bool format_checkpoint_filename(std::int32_t source_save_slot,
                                std::int32_t source_difficulty,
                                std::string& output, std::string& error);

struct Section {
    std::string tag;
    std::vector<std::uint8_t> payload;
};

// The selected LevelSavegame owns this independent file image and its INFO /
// OBJS sections; it never aliases PlayerSavegame or its profile Transport.
bool serialize_savegame(const Section* sections, std::size_t count,
                        std::vector<std::uint8_t>& output,
                        std::string& error);

// Validate the source Savegame section framing emitted by serialize_savegame
// and expose its canonical INFO word. OBJS is retained as an opaque payload;
// its rows are validated/applied by the Level ObjectManager loader.
bool read_savegame_info_word(const std::uint8_t* image, std::size_t image_size,
                             std::int32_t& info_word, std::string& error);

struct PublishServices {
    void* context{};
    // Existing save-file service; receives the exact LevelSavegame filename
    // and Savegame::saveAll image. Returns true only after successful publish.
    bool (*publish)(void*, const std::string& filename,
                    const std::vector<std::uint8_t>& image,
                    std::string& error){};
};
struct SaveResult {
    std::uint32_t object_rows{};
    std::uint32_t saved_objects{};
    std::uint32_t bytes{};
    std::uint32_t publishes{};
};
enum class SaveStatus : std::uint32_t {
    saved, invalid_state, provider_unavailable, provider_failed
};

// Port-owned LevelSavegame +4 Savegame. Construction is bound to the filename
// already produced by source GetFilename; campaign PlayerSavegame is separate.
class Owner {
public:
    bool construct(std::string source_filename, std::int32_t info_word,
                   std::string& error);
    SaveStatus save_all(const Object* objects, std::size_t count,
                        std::uint32_t online,
                        const SerializeServices* serializers,
                        const PublishServices* publisher,
                        SaveResult*, std::string& error);
    // Source Savegame::Load on this existing LevelSavegame owner. The filename
    // remains the regular per-level name after LoadCheckPoint's temporary
    // checkpoint-name substitution has unwound.
    bool load_image(const std::uint8_t* image, std::size_t image_size,
                    std::string& error);
    void destroy() noexcept;
    bool set_gate_39(std::uint8_t value) noexcept;
    bool constructed() const noexcept { return constructed_; }
    std::uint8_t gate_39() const noexcept { return gate_39_; }
    const std::string& filename() const noexcept { return filename_; }
    const std::vector<std::uint8_t>& last_image() const noexcept { return image_; }
private:
    std::string filename_;
    std::int32_t info_word_{};
    std::vector<std::uint8_t> image_;
    bool constructed_{};
    std::uint8_t gate_39_{};
};

} // namespace dh2::level_savegame_owner_v1
