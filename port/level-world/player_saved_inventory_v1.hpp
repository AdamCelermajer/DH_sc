#pragma once
#include "../game-data/fresh_inventory_owned_v4.hpp"
#include "../game-data/item_power_tables_v5.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include <string>

namespace dh2::player_saved_inventory_v1 {
struct Bindings {
 data::PlayerSavegameV1* save=nullptr;
 data::FreshInventoryOwnedV4* inventory=nullptr;
 data::PropertyView* properties=nullptr;
 const data::OwnedInventoryServicesV4* equipment_services=nullptr;
 data::ItemPowerTablesV5::Borrow powers;
 // The source caller's newly constructed Item, before AddItemInstance takes
 // ownership. This stable slot is owned by the existing Item lifetime owner.
 // Published before constructor callbacks; reused for a split after the first
 // AddItem transfer. Native failures retain the actual construction/power
 // prefix. Retire through V4's mandatory observer before actual destruction.
 std::unique_ptr<data::ItemInstanceV1>* incoming=nullptr;
};
enum class Stage : std::uint32_t {not_started,character,header,gold,selection,
 item_name,item_fields,construct,set_value,identified,power_name,add_power,
 add_item,equip_first,equip_second,complete};
struct Result {
 Stage stage=Stage::not_started;
 std::uint32_t source_caller=0,consumed=0,read_calls=0,string_reads=0,
 declared_items=0,completed_items=0,declared_powers=0,completed_powers=0,
 constructors=0,set_values=0,identified_stores=0,add_powers=0,add_items=0,
 equips=0,selection_stores=0;
 std::int32_t item_id=-1,power_id=-1,inserted_index=-1;
};
enum class Status {complete,invalid_argument,busy,failed};
class Runtime {
 Bindings bindings_;bool busy_=false;
public:
 explicit Runtime(Bindings);
 Status load(data::Bytes payload,Result*,std::string& error);
};
// Whole PlayerSavegame::__LoadInventory1024B@0x46a3a0. Borrowed Save Character
// is read freshly at original mutation sites; one V4 inventory owns all items,
// gold, selection and both equipment sets. No profile parser, Save/VM/property
// owner, item registry, invented empty inventory or hidden recalc/Skin/vitals.
// Constructor/add/equip reuse V4. SetValue tails the mandatory _UpdateName;
// AddPower(-1 mode) is mandatory through the same equipment descriptor and
// must use its existing ItemPresentation/text owner and actual power resources.
// Owners, input bytes, incoming slot and service contexts stay live through
// synchronous calls. Reentry on this Runtime rejects without touching outputs.
// Failed incoming items must be retired by the existing lifetime owner before
// Presentation/text teardown; this borrower never destroys them on failure.
// Corrupt/unbounded streams and source assertion/unsafe-pointer domains fail
// at the reached native boundary, retaining the completed source prefix.
}
