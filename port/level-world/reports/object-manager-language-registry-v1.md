# Shared source ObjectManager language registry

Source constructor34a1e8 initializes an empty object-map header at+0c and
circular Character list at+60. Source Add34b270 publishes the actual object
under the existing signed handle key, sets name/archetype/network fields,
then GetHandle→AsChar appends a real Character at the list tail. Null AsChar
does not append. The language traversal observes signed map key order and
Character registration insertion order; it never selects visible/nearest actors.

ObjectManagerLanguageRegistryV1 owns only stable registration/tree/list nodes,
borrowing canonical source objects through lifetime leases. source_added needs
the actual assigned signed handle key, semantic object identity, canonical
type+f4 pointer, actual optional type14+819 pointer and actual AsChar result.
The source factory/Add owner must emit registrations after its genuine delivery;
this helper does not claim source Spawn/Add name-search/delete/network behavior.
source_removed and source_flush retire the same registration borrows. Registry
copy/move is disabled because its intrusive projected graph borrows stable nodes.

Tree shape is a stable native successor-order projection, not a reconstructed
STL red-black allocation shape. It preserves source signed-key traversal order.
The original constructor/registration instructions are captured in
object-manager-language-source-v1.asm. These facts do not turn the development
DACT descriptor into a source ObjectManager: DACT.kind1/2 explicitly describes
actor/decor rendering and omits real factory GO_IDs and source handle keys.

settings_language_scene_v2 preserves the original traversal and required
callbacks but requires localization_valid only when the live type14 branch
is reached. V1's additional non14 pointer requirement remains frozen. Ordinary
Character records therefore need no invented localization byte. Type3 refresh
can mutate type; V2 rereads it and requires a genuine byte if it becomes14.

All new production and test files pass strict Android arm64 syntax. Focused
ASan/UBSan regression source covers constructor sentinels, nonempty mixed object
registrations, signed-key order, Character insertion order, actual borrowed byte
invalidation, duplicate/removal/flush and live14 missing-provider failure. Its
inputs are declared registration/predicate fixtures, not an authentic Crypt
factory. WSL compilation/execution attempts stalled without output and were
interrupted locally; no host runtime PASS is claimed for this new regression.

Full player/Gear/V6/original-movie startup remains pending canonical factory
registrations from ImmutableLevelPreparation XML/declarations. The typed
registration contract is handed to the factory agent so menus/language consume
the same live scene rather than constructing a second fixture World.
