# Original character movie production startup transport

Use AuthoredCharacterStartupDispatcherV1 with its retained World/application
lease. Its `queries` lambda calls the existing CharacterPanelSession::dispatch
on a fresh synchronous player_gameplay_binding, passing CharacterMenuCallV1
objects directly. The query owner mutates the actual AS arrays/objects through
the protected bridge; no JSON snapshot, copied Save or manufactured player.
Reload/navigation/rollover point to their existing retained native owners.

Set SwfServices.native_action to dispatcher.dispatch and native_actions to
AuthoredCharacterMenuBridgeV1::native_actions before CharacterMenuMovieV1.load.
The separate movie owner supplies actual source font/GPU/APK services. Borrow
world/player/app contexts for the entire load and later source actions. The
dispatcher lease must not own the movie, avoiding a graph/provider cycle.

AuthoredCharacterApplicationV1 routes NativeLoadSettings through the complete
original hud startup wrapper. Supply existing SettingsStartupBindingV1 over
the SAME OwnedHudSettingsV1/private-file owner and actual language/scene/audio
services. The wrapper reaches load(false), updateSavedValues, optional actual
sound volume delivery, getLanguage, setLanguage. Missing any reached endpoint
rejects; successful private-file ENOENT follows actual source settings defaults.
NativeIsMultiplayerEnabled follows source device facts and performance gate,
then writes the actual bridge result; no hardcoded multiplayer flag.

Application wrapper O1 ASan/UBSan test passes, preserving call order/results,
source device early branches and required failure. Settings/device test inputs
remain declared fixtures, not proof of Android Application producer binding.
Strict Android startup/application/outer reload compilation passes.

Historical actual-cache CharacterMenuMovie startup observed these names:
NativeLoadSettings, NativeIsMultiplayerEnabled, NativeReloadSkills,
NativeSkillsGetSkillPointsLeft, NativeGetSkillDetails,
NativeSkillGetEquipedSkillsIDs, NativeChangeRolloverInputBehavior.
That historical test declared null skill-query player fixtures. Nonempty live
player data can reach additional callbacks: do not represent this as a complete
production activation list. The new dispatcher records unique genuinely reached
names in order via reached_callbacks(), and rejects the first unowned endpoint.

Outer reload now directly uses actual Gear check_item_requirements_v1 when no
override is supplied. Same-player class/buff-aware recalculation is available
through CharacterMenuRecalcOwnerV1; actual-cache O1 ASan/UBSan passes. Save Load8
and20 default-slot null-profile guards are genuine, with documented unsaved
skill reset behavior. MenuFX/IsSpecTime must still borrow the current actual
movie weak receiver inside its Scope. Source startup may invoke this before
CharacterMenuMovieV1 publishes its loaded flag: use the actual current graph
callback lease, not movie.invoke's post-load-only method.

Root now binds updateSavedValues to recovered source ResetOrientation (bx lr),
and its actual sound pointer is null, taking the genuine no-volume branch.
The v76 device helper modules are copied byte-for-byte with SHA receipt in
menu-device-v76-copy.json; real Android Build and current multiplayer-mode
producers still belong to root's actual platform/gameplay owners.
Production blockers are complete language scene/shared TextManager transport,
exact MenuManager registration/weak current RenderFX binding and
any additional actually reached callbacks. These adapters do not prove a fully
live player/movie composed host run or Android menu activation.
