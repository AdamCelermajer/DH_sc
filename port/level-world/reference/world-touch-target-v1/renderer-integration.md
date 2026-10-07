# World click targeting source route

`world_click_target_v1` ports whole Character::Ctrl_Click3addc8. It borrows the
actual player's pending14c8/skill14ca/destination14b0/origin14bc/click-target413
and the SAME existing TargetState48/TargetServices16. It owns no mutable target,
FSM, geometry, health or selection list. Retain every world iterator and actor
through synchronous calls, and reload successors from the actual ObjectManager
Character list/tree in source order. Generic tree cursor must perform genuine
shared-handle copy/GetObject(false) before delivering the resolved identity.

Controls: source CTRLIsAllowed gate; Application320e74 IsUsingDPad; same FSM
SM_IsCasting/SM_IsUsingSkill. While casting/using, release clears pending and
skill; press stores both source point vectors and marks pending. Otherwise the
source loops all Characters (LookAt position before self skip), virtual
IsInteractive(owner), optional mode-only IsEnemy, whole IsNeutral; smallest
strictly closer squared world-point distance wins only after original IsNearby.
IsNearby uses actual absolute AABB plus/minus radius*50 with inclusive comparisons.
Radius must come from actual DesignSettings+2c(enemy)/+50(friend), not a touch
constant. Equal distances preserve original insertion order.

Selection executes Debug key `isTracingChar_CTRL`, stores click-target413=1,
then existing whole AI_SetTarget(mode0). Root must bind the same target owner
already used by automatic combat, script SetTarget and HUD query. There is no
separate HP-bar target. Target setter failure preserves its actual prefix.

On released/no-mode/no-character match, source scans the actual generic handle
tree, skips source type-name5c equal to exact `Block`, and uses actual radius+54.
Generic selection traces then sets the same target without writing413. Remaining
no-target path queries `UseClickToMove`, preserves the mode early return, traces,
sets targetNULL, executes source SyncLastTarget and actual movement virtuale4/ec.
Do not silently consume it as successful selection when movement is missing.

`character_world_ai_neutral_v1` ports whole AI_IsNeutral3d5a98 using the existing
WorldAiRelationshipV1/WorldAiServicesV1 handle/kind/faction/assertion graph.
It cannot be replaced by !(IsEnemy||IsFriend): source callback order differs,
non-character/null/missing-row entries return true, and assertion lines226..229
must retain the source boundary. Root can expose it through the same private
World runtime AI service/faction rows/current-target borrow used by relationship().

`WorldClickFieldsOwnerV1` is constructor-backed source storage to retain once in
the canonical Character transport. Both vectors and pending byte are zero;
skill14ca=-1; click-target413=1 comes from CharAI+4b constructor, not a default
inferred from usage. Constructor oracle executes actual pending store block and
CharAI field prefix; full Character/AI global queue construction is not claimed.
Its `borrow()` accepts the same existing TargetState48 and TargetServices16.

`world_click_using_dpad_v1()` borrows the existing `OwnedHudSettingsV1` and reads
exact source GetSavedOption("DPad")!=0. It creates no settings/save owner. Missing
settings backing is a failure; do not infer mode from current widget style.

`world_touch_projection_v1` ports source TranslateScreenToWorld and PFWorld/
PFRoom iteration over actual borrowed room/floor vectors. It truncates finite
in-range screen coordinates as source f2iz; source passes includeSpecial=false,
so floors with flags24&0x03000000 are skipped. First floor hit in first room wins,
not a minimum-distance sweep across all rooms. The actual floor source51b874
gets its scene node40 triangleSelector via virtualb0 then actual SceneCollision
Manager getCollisionPoint. Bind that same selector/scene graph. A source miss
preserves output point. `renderer_world_touch_target_v1.inc` joins projection,
actual Cmd_Click controller gates and recovered click selection; root input hook
supplies actual scene viewport coordinates. No renderer file is modified here.

Original iPhone HUDControls::Update41a780 actually translates screen position
through PFWorld525884 and invokes Cmd_MoveTo4054e4. It does not call Cmd_Click.
Therefore enabling a world tap to invoke this recovered Click route is a new
input binding, not a claim that original iPhone dispatch did so. Source Cmd_Click
4051a8 controller gates must still run. Source PF screen translation calls actual
camera SceneCollisionManager GetRayFromScreenCoordinates and actual floor ray
intersection525800. Root must bind live camera/viewport/floor producers. Source
scene-node bounding-box picking is present in the engine but has no recovered
caller in this gameplay route; do not substitute it or rendered-screen boxes as
the original selector. Screen HUD silhouette bounds remain presentation data.

Original control oracle PASS seven cases executing Ctrl_Click + IsNearby +
SyncLastTarget with external services explicitly marked fixtures. Native tests
cover same-field effects, real whole target setter, strict tie order, missing
predicates, setter prefix failure, Block exclusion and boundary/NaN comparisons.
Both ABI syntax checks pass. Projection oracle additionally passes three original
TranslateScreen/PFWorld/PFRoom cases (camera and actual floor leaf remain declared
external services). Actual world input integration remains unverified.

Neutral original oracle additionally passes eight cases executing the whole
AI_IsNeutral body: relation -1/0/1/2, absent entry, noncharacter kind, unresolved
handle, and NULL argument resolving the current target. Its declared external
services are GetHandle/GetObject/GetFaction; assertion branches are not included.
Exact source order is handle, object, target faction twice, owner faction three
times, target faction. The native test covers explicit assertion failure too,
but has only been syntax checked in this lane.

Native click test sources: tests/world_click_target_v1.cpp,
world_click_target_v1.cpp, character_target_bindings.cpp. Use function sections
and linker garbage collection for standalone builds so unrelated Lua bind
entrypoints are discarded; otherwise link the actual script runtime. Neutral
test sources: tests/character_world_ai_neutral_v1.cpp,
character_world_ai_neutral_v1.cpp (no VM dependency).
Projection test sources: tests/world_touch_projection_v1.cpp,
world_touch_projection_v1.cpp (no VM dependency).
