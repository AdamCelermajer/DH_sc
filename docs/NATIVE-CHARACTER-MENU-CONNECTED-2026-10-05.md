# Expanded in-game menu ActionScript composition

The v2 connected fixture replaces both earlier AS receiver and inventory-list
fixtures with retained original GameSWF objects. Native queries and actions run
through real `fn_call` values in the existing movie scope. Inventory listing
allocates real AS rows, writes them through genuine setters, appends to a real
AS array while retaining its prefix, then reads each field back from those
objects. Automatic equipment and required-provider failure paths also pass
through that interface. The earlier v1 fixture and audit remain intact.

The graph retains one real GearV1 inventory/property owner, one V3 skill/session
owner and its actual bound Save for each of Knight, Mage and Rogue. It serves
207 stat members, 341 item members and 27 skill members, with three whole
training transactions, 27 actions and 15 provider/no-player guard checks.
The fixture additionally exercises quantity-one removal from each actual
Gear-owned inventory, equipment-reference cleanup and rejection of destructive
observer reentry. Transfer/potion differential and full Drop/Transmute remain
separate unfinished obligations; the mutation adapter is not frozen.

The host uses the original shared/HUD SWFs for transport. It does not load or
exercise authored character-screen tab navigation, layout or pointer input.
GPU, startup settings/localization, the enclosing offline World and HUD refresh
remain explicit fixtures. Complete saving and faery switching are still needed.
The actual skill-combat successor, menu stack and mesh effects have separate
source owners and must be connected to this same live player before publication.

`port/engine-ui/reports/character-menu-connected-v2-host-audit.json` records
both sanitized/optimized runs, source, executable, SWF and linked-snapshot
hashes. It is candidate composition evidence, not a whole-menu freeze or APK
checkpoint. The visible emulator still runs the accepted equipped-player APK.
