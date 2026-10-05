# Same-owner faery gameplay candidate

New faery_gameplay_v1.hpp/.cpp borrows the live PlayerGameplayBinding. It
checks Character identity, retained SkillV6 owner, Save identity and actual
resolved/saved property backing. No second VM, Save, spell instances, cooldown
store or world faery is created.

## Implemented source projection

AI_SpellInfo0x3d7da8 reads current selected Save faery (difficulty=-1 resolved
by the caller's live descriptor), indexes retained spell slots and calls the
existing original GetInfo coordinator with level0. The helper projects only
the same borrowed spell-vector pointer into that coordinator. It resolves the
same retained Session/VM freshly; null spell returns original fraction0.

AI_IsSpellUsable0x3d80b4 has ordered early exits for actual FSM current6
(UsingSkill), current7(Casting), and script load_step<=6. Remaining execution
calls original spell CheckUsable through the same existing callback/session
implementation. AI_IsSpellActive0x3d7d9c is original constantfalse, not an
invented spell-running predicate.

gameplay_hud.cpp's genuinely unlocked spell branches now call these helpers;
locked branch remains original fraction0/unusable. Native diagnostics retain
failed service status. No campaign faery is unlocked for demonstration.

## Commands and selection

faery_hud_use_v1 reproduces NativeHUDSpell0x43cabc ordering:
actual CTRLIsAllowed; if allowed actual controller Cmd_BeginCast(false), then
Cmd_EndCast(false). Missing services fail and retain earlier delivered calls.
The renderer must supply its real controller and cast state services. This
candidate does not claim to implement missing casting FSM, animations or
network services by merely calling an OnSkillUse Lua callback.

faery_select_v1 checks actual campaign unlock eligibility, stores selection
in same Save, calls actual retained UpdateAllSkills and asks the real world
owner for Character+420 faery identity. Nonnull faery requires original model
refresh and Animator AddSetToRenderObject. A real null pointer follows source
no-faery-model branch. NativeHUDSetActiveFaery then requires current Level
PlaceFaeryAndFollowers (a real absent-Level branch can be delivered by that
provider). No fake identity or generic model is supplied.

Required renderer hooks are FaeryWorldServicesV1 controller_allowed,
begin_cast/end_cast, faery_character, refresh_model, add_animation_set,
place_faery_and_followers. Actual original AI_BeginSpell0x3d81c0 chooses
selected Faery SpellType, checks usability, resets its own spell fields and
requests SM_SetCastState; controller forced/global/locked gates also apply.
These deeper services must not be replaced by accepted no-ops.

## Save successor and evidence

PlayerSavegameV1 set_current_faery(rawID,tier,error) is an explicit source API
successor. It stores signed raw bits into current_faery[tier]; no unlock or
row initialization. Original Character::SG_SetCurrentFaerie0x3bb9d8 stores
the raw word directly at Save+0xac+tier*4 without checking ID or initialization.
Source difficulty=-1 reads global selected difficulty; helper requires caller
to resolve that actual owner rather than introducing a default global.
Native tier>=3 is rejected before unsafe storage access. Historical frozen
manifests remain unchanged and are not claimed to validate successor bytes.

Original ARM Unicorn store oracle:21 cases (3 tiers x IDs0,1,4,5,0x7fffffff,
0x80000000,0xffffffff). Matching native Android5554 executable passed those
cases, preserved other tiers and kept all faery storage uninitialized. Receipts
faery-save-original-store-v1.json and faery-save-store-android-v1.json.

Isolated Android NativeHUDSpell routing fixtures passed5 cases: controller
rejection, begin/end ordering, begin failure, end failure, missing end with
reached begin prefix. Explicit fixture identity is not a gameplay actor.
Receipt faery-hud-command-android-v1.json. Strict ARM64Android24 syntax passed
faery_gameplay_v1.cpp, gameplay_hud.cpp and player_savegame_v1.cpp with
-Wall -Wextra -Werror. Full unlocked campaign casting/model rendering remains
unverified until the real renderer hooks are delivered. Add helper cpp to
Android CMake; root retains renderer/native_app/Java integration ownership.
