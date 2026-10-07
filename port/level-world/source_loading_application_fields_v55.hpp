#pragma once
#include <cstdint>
#include <string>
namespace dh2::application {
// Bounded same-Application source field/global projection, NOT whole C1.
// Application's original process-static storage supplies BSS byteec=0;
// C1 32d878 explicitly stores byteb4=1. Original cheat/global counter BSS
// cells start zero. No UI-overlay-derived values or loading clocks exist.
struct SourceLoadingApplicationFieldsV55 {
 std::uint32_t frame74{}; // C1 32d834 stores0; _Update common tail32c7e4..f0 increments.
 std::uint32_t dt8c{}; // Application.GetDt31f66c reads this actual frame cell.
 // C1 32d7dc/32d86c..88c constructs empty source std::string atbc.
 // IsUsingUncompiledData320678 compares its cc/d0 begin/end before matching
 // extensions. Retain the real string, rather than a copied mode boolean.
 std::string uncompiled_data_path_bc;
 bool native_dt_produced_v93{}; // Adapter provenance; not a source gameplay flag.
 std::uint8_t byte_ec{},byte_b4{1};
 // SAME process Application.GoToMainMenu cells. C1 32d874 stores ab=1;
 // b0 has no C1 store and begins zero in the original static BSS instance.
 std::uint8_t byte_ab{1};std::int32_t event_b0{};
 //Produced only by the real source Stage32 warm-start stores.
 std::uint8_t byte94_v95{};bool native_byte94_produced_v95{};
 std::uint8_t handle_cheats_inGame9f640a{};
 std::uint32_t g_bigI9f6890{},g_bigV9f688c{};
 // Original GS.Update phase4 writes these real process LuaScript globals.
 std::uint32_t lua_num_calls9a2400{};
 std::uint8_t lua_dump_call_list9a2404{};
};
}
