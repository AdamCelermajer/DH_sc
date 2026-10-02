#ifndef DH2_LUA_RUNTIME_H
#define DH2_LUA_RUNTIME_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct dh2_lua dh2_lua;
/* Modern owned runtime, not the original engine's C++ object layout.
 * Single owner/thread, no reentry. Bytes are borrowed only during the call.
 * Diagnostics are truncated to the caller's capacity and always terminated.
 * Numeric bridge callbacks are installed; gameplay object callbacks are absent. */
dh2_lua *dh2_lua_create(size_t memory_limit);
void dh2_lua_destroy(dh2_lua *runtime);
/* Source-only compiler. Does not execute the chunk. */
int dh2_lua_compile(dh2_lua *runtime, const void *source, size_t bytes,
                    char *error, size_t capacity);
/* Validate a complete integer constant file, then atomically merge owned Lua
 * mappings. Duplicate keys use the last value. Unknown GetPyCst returns zero;
 * wrong argument types/count return no results. Rejection preserves mappings. */
int dh2_lua_import_constants(dh2_lua *runtime,const void *bytes,size_t size,
                             char *error,size_t capacity);
/* Import one complete ordered name table as a named class. Atomic replacement;
 * first duplicate wins. GetPyOID/GetPyStruct use the same installed class maps
 * and return -1 for unknown class/member. Original static Structs field tables
 * are installed at creation; runtime imports can replace a class. Names and
 * values are owned by Lua; name must be 1..255 bytes without NUL. */
int dh2_lua_import_names(dh2_lua *runtime,const char *name,size_t length,
                         const void *bytes,size_t size,char *error,size_t capacity);
/* Atomically install an owned complete character property dataset (<=4 MiB).
 * DH2CreatePropertyState(row) creates a diagnostic property userdata with
 * GetProp/SetProp numeric/boolean methods. Each object retains its dataset
 * generation. It loads the base row and recomposes with empty buffs; derived
 * stat lifecycle is available through the separately imported class/item/power
 * diagnostic methods. Actual Character/gameplay objects remain absent. */
int dh2_lua_import_character_properties(dh2_lua *runtime,const void *bytes,size_t size,
                                       char *error,size_t capacity);
/* Atomically import an owned complete class-rule file (<=4 MiB). Newly created
 * diagnostic property objects retain the current class dataset generation.
 * ApplyClass(id[,fromFinal]) applies to base and recomposes with empty buffs;
 * it does not reset/reload base or implement an original Character lifecycle.
 * Objects created before class import lack class data until recreated. */
int dh2_lua_import_character_classes(dh2_lua *runtime,const void *bytes,size_t size,
                                    char *error,size_t capacity);
/* Atomic owned full eight-table loot file. New diagnostic property objects
 * retain its generation. EquipItem(set,slot,row) and SelectEquipmentSet(set)
 * are authored snapshot controls. Original named bonus methods and HasShield
 * use the source projection; numeric/boolean/nil first bonus arguments only.
 * Equip requirements and full inventory lifecycle remain unimplemented. */
int dh2_lua_import_loot_tables(dh2_lua *runtime,const void *bytes,size_t size,
                             char *error,size_t capacity);
/* Atomic owned complete item-power tables. New property objects retain their
 * power generation. Authored EquipGear (16 slots in two sets), SetItemPowers
 * (up to 32 per slot), UpdateBaseProperties, UpdateGearsProperties and
 * RecalculateProperties expose reconstructed empty-buff stat lifecycle through
 * diagnostic objects. EquipItem/EquipGear clear that slot's assigned powers.
 * Equipment selection and power assignment require an explicit gear update.
 * These objects are not original Character, inventory or gameplay objects. */
int dh2_lua_import_item_powers(dh2_lua *,const void *,size_t,char *,size_t);
/* Atomic owned full quest table import. Authored record getters and counted-kill
 * objective creation preserve dataset generations. No original quest compile,
 * condition/reward dispatch, world state or persistence. */
int dh2_lua_import_quests(dh2_lua *,const void *,size_t,char *,size_t);
/* Controlled execution helper; instruction budget counts in 1000-op blocks.
 * Installs base/math/table/string, with filesystem loaders and print removed.
 * Returns zero on success; stack and hook are cleared after each call. */
int dh2_lua_execute(dh2_lua *runtime, const void *source, size_t bytes,
                    uint32_t instruction_blocks, char *error, size_t capacity);
/* Authored session interface to an already installed Lua global function.
 * Up to 16 finite numeric arguments and 32 finite numeric results are copied;
 * output is written only after the exact result count/tags validate. Lua
 * execution is protected by the same allocation and instruction limits as
 * execute. Function side effects are not rolled back after a Lua error. */
int dh2_lua_call_numbers(dh2_lua *,const char *function,
                         const float *arguments,size_t argument_count,
                         float *results,size_t result_count,
                         uint32_t instruction_blocks,char *error,size_t capacity);
size_t dh2_lua_memory_used(const dh2_lua *runtime);
#ifdef __cplusplus
}
#endif
#endif
