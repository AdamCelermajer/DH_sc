#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh2::world {
// Original Structs::OpenableContainer, size 0x28 (vptr excluded).
struct OpenableContainerRowV1 {
    std::int32_t field4=0, sound=-1;
    bool keep_physics=false;
    std::int32_t loot=-1;
    std::string script;
    std::int32_t field1c=0, field20=0, visual=-1;
};
bool read_openable_container_record_v1(const std::uint8_t*,std::size_t,
    OpenableContainerRowV1&,std::size_t&,std::string&);
class OpenableContainerTableV1 {
    std::vector<std::string> names_;
    std::vector<OpenableContainerRowV1> rows_;
public:
    // Caller supplies the source Arrays::OpenableContainers group, including count.
    bool load(const std::uint8_t*,std::size_t,const std::uint8_t*,std::size_t,std::string&);
    bool resolve(const std::string&,std::int32_t&,OpenableContainerRowV1&,std::string&) const;
    std::size_t size()const{return rows_.size();}
};

// Canonical receiver fields: factory owns this object; no parallel save/world.
struct OpenableContainerFieldsV1 {
    static constexpr std::uint32_t source_type_f4=7; // factory argument; live type resides in canonical base
    std::int32_t state394=2, data374=-1;
    std::string data_desc, key_name;
    bool death_reset=false, key_consume=true;
    std::int32_t key_qty=1, key_id710=-1;
    std::uintptr_t opener398=0;
};
struct OpenableContainerServicesV1 {
    std::shared_ptr<void> owner;
    std::function<bool(std::int32_t&,std::int32_t&,std::string&)> spawn_roll_and_probability;
    std::function<bool(const std::string&,std::int32_t&,OpenableContainerRowV1&,std::string&)> resolve_row;
    std::function<bool(std::int32_t,std::string&)> visual_asset;
    std::function<bool(std::string&)> game_object_init_post, game_object_init_final;
    std::function<bool(bool&,std::string&)> meet_condition, has_visual, has_script, has_sound_manager;
    std::function<bool(std::string&)> bind_timeline_callbacks, apply_mesh_box, disable_object,
        create_attach_po_decor, detach_physical, source_on_interact;
    std::function<bool(const char*,bool&,std::string&)> play_animation;
    std::function<bool(std::uint32_t,std::uint32_t,std::string&)> scene_flags;
    std::function<bool(std::int32_t,std::string&)> load_sound, play_sound_3d;
    std::function<bool(const char*,const char*,std::string&)> load_object_script;
    std::function<bool(const std::string&,std::int32_t&,std::string&)> resolve_item_name;
    std::function<bool(std::uintptr_t,bool&,std::string&)> is_character;
    std::function<bool(std::uintptr_t,std::int32_t,bool&,std::int16_t&,std::string&)> find_key;
    // Source RemoveItemByID(key), NOT a manufactured quantity subtraction.
    std::function<bool(std::uintptr_t,std::int32_t,bool&,std::string&)> remove_key;
    std::function<bool(std::int32_t,std::uintptr_t,std::int32_t,bool,std::string&)> drop_loot_table;
    std::function<bool(const char*,std::uintptr_t,const char*,std::string&)> script_call;
};
class OpenableContainerOwnerV1 {
public:
    OpenableContainerOwnerV1(OpenableContainerFieldsV1&,OpenableContainerServicesV1);
    bool init_post(std::string&);
    bool init_final(std::string&);
    bool spawn(std::string&);
    bool interact_base(std::uintptr_t,std::string&);
    bool try_unlock(std::uintptr_t,bool&,std::string&);
    bool do_open(std::string&);
    bool animation_event(const char*,std::string&);
    bool animation_finished(bool timeline_active,std::string&);
    bool is_locked() const;
    bool is_interactive(bool disabled81) const;
    static constexpr std::int32_t loot_fixed_num_powers() noexcept{return -1;} // source39f35c
private:
    bool set_state(std::int32_t,std::string&);
    bool row(OpenableContainerRowV1&,std::string&);
    OpenableContainerFieldsV1& f_;
    OpenableContainerServicesV1 s_;
};
}
