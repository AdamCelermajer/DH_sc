# Native character-menu ActionScript connection

The in-game character screen is owned by this checkout. The separate main-menu
chat owns launch menus and character selection. This work adds the typed native
connection required by stats, inventory/equipment and skills; it is not a new
APK checkpoint or a claim that the complete character screen works on Android.

`CharacterMenuAsBridgeV1` receives the original `gameswf::fn_call` inside the
existing SWF facade scope. Its injected dispatcher borrows the same native
player graph without adding a UI-library dependency on the World library.
Actual AS objects and the player are pinned through synchronous callbacks.
Member writes run the genuine setters/watchers, array appends retain existing
entries, and item-list rows use the original plain `as_object(player)` factory.
Numeric and boolean conversions use actual AS values. Bound-property getters
run only when conversion is reached. Invalid object handles and cross-player
objects fail before dereference; completed writes/appends remain visible after
a required native-provider failure.

The standalone transport audit passes ASan/UBSan and optimized O2: eight typed
callbacks, ten actual member writes, seven retained appends, four created
objects, five conversions, nested watcher reentry, six required guard checks,
and two bound-property checks. The test includes callback-state ownership and
release checks.

The connected audit also passes both builds for Knight, Mage and Rogue. It
routes native menu queries/actions through actual `SwfMovie`/ActionScript calls,
then reads results from the real AS receiver and array. The same native
equipment, properties, saved skills and authored Lua instances serve 207 stat
fields, 233 item fields, 27 skill fields, three training transactions and 21
actions. Stat assignment, equip/unequip, equipment swaps and skill-slot updates
reach their real graph; twelve required-provider/no-player guard checks pass.

The connected test snapshots the existing gameplay fixture without changing
the parallel worker's test. Its offline World, startup localization/settings,
GPU providers and HUD-refresh boundary remain explicit fixtures. It loads the
original shared/HUD SWFs to test transport; it does not exercise the authored
character-menu navigation, buttons, layout or visible Android screen. Inventory
listing/slot selection, drop/transmute, complete faery changes and saving still
need their source owners and integration. Targeting and combat/FX integration
are separate ongoing work.

Evidence: `port/engine-ui/reports/character-menu-as-bridge-v1-host-audit.json`.
It binds the bridge/test sources, private linked libraries, original SWF inputs,
both executable/results and fixture derivation. The gameplay menu owners remain
under development; this receipt is not a freeze of the entire menu or campaign.

The visible emulator and installed APK remain the accepted equipped-player
checkpoint `dh2-native-equipped-player-04fb89d1.apk`. The next APK checkpoint
should include a complete, connected, visible feature rather than this isolated
transport addition.
