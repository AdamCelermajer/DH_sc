#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

namespace dh2::data::savegame_options_v1 {
// The source option record borrows its immutable table definition. These are
// logical fields, not an ARM std::map/node or Structs object overlay.
struct Definition {std::int32_t toggled_value;std::int32_t type;};
struct Record {const Definition* definition;std::int32_t value;};
enum class Status : std::int32_t {complete,invalid_argument,missing_definition,storage_failure};

// Read-only view of the one active Application settings owner. The view is
// borrowed, queried synchronously, and never copied into the gameplay Owner.
class ReadOnlySettings {
public:
    virtual ~ReadOnlySettings()=default;
    virtual bool has_option(const char*)const noexcept=0;
    virtual Status get_option(const char*,std::int32_t*)const noexcept=0;
    virtual Status is_option_toggled(const char*,bool*)const noexcept=0;
    virtual std::size_t option_count()const noexcept=0;
};
// One Application settings owner is active on the game/UI thread. Binding the
// same owner is idempotent; another live owner is rejected until it is unbound.
bool bind_read_only_settings(const ReadOnlySettings*)noexcept;
bool unbind_read_only_settings(const ReadOnlySettings*)noexcept;

class Owner {
    std::map<std::string,Record,std::less<>> options_;
public:
    Owner()=default; // SavegameManager constructor starts with an empty map.
    Owner(const Owner&)=delete;Owner& operator=(const Owner&)=delete;
    // Loader boundary only; this does not reconstruct option initialization,
    // setOption, settings-file load/save or the full SavegameManager lifetime.
    Status deliver_record(const char*,const Definition*,std::int32_t);
    Status has_option(const char*,bool*)const noexcept;
    Status get_option(const char*,std::int32_t*)const noexcept;
    Status is_option_toggled(const char*,bool*)const noexcept;
    std::size_t size()const noexcept;
};
// Application captures its manager pointer before hasOption. Native services
// borrow this owner beside the existing Application/PlayerSavegame owners.
struct Application {std::uintptr_t identity;const Owner* manager;};
Status get_saved_option(const Application*,const char*,std::int32_t*)noexcept;
Status is_saved_option_on(const Application*,const char*,bool*)noexcept;
// First NUL terminates keys; missing getOption=-1, missing application option=0.
// Toggled is type==0 AND value==definition.toggled_value, not value!=0.
// Single owning thread; borrowed definitions and key storage live through calls.
// The reconstructed map query callers exclude original allocator/tree bodies.
}
