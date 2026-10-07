# Original player heading transport

Add `character_heading_owner_v1.cpp` to the native target and include its header.
Include `renderer_player_heading_v1.inc` after retained PlayerSkillsRuntime and
prince event/facts declarations. This is proposed wiring, not compiled live.

PlayerSkillsRuntime members/declarations:
```
dh2::character::ControllerCommandState32 heading_command{};
dh2::physical::NativeBody* heading_body{};
std::uint8_t heading_remote_updated_byte=0;
std::unique_ptr<dh2::character::CharacterHeadingOwnerV1> heading_owner;
static bool heading_position(void*,std::uintptr_t,const float*&,std::string&);
static bool heading_remote(void*,bool&,std::string&);
static bool heading_physics(void*,bool&,std::string&);
static bool heading_event(void*,std::uint32_t,std::string&);
static void bind_heading(PlayerSkillsRuntime&);
```
ObjectBase ctor33f2c8 stores byte118=0 (r5=0); this new byte is its sole
projection until actual source network writers are composed. IsRemotelyUpdated
33dd10 returns true if network110!=-1, otherwise actual byte118. Existing
combat_fields.network_id is the same constructor-owned110. Physics query
3a2e44 is exact SAME machine.flags bit1. Do not substitute platform online mode.

Call bind_heading once with the existing actual player graph, not per frame.
Retained world reload does not invalidate borrowed player runtime/TargetState;
target_position dynamically queries the current registered actor. The body
pointer is refreshed before each command and remains null when removed.

Replace renderer1959..1976 development heading/destination/state-event block:
actual authored HUD Update calls source_player_heading_v1(scaled direction,
false,error) only when its source active bit and CTRLIsAllowed permit;
release6/7 calls source_player_heading_v1(nullptr,true,error) after source stick
reset. Do not issue Stop every inactive frame, synthesize destination+1000,
emit c351 directly, or add .08 deadzone. Preserve all three components and
magnitude from AuthoredJoystickV1. Whole original Character event0/3f relays
through SAME CharAI; the inc does not substitute prince_event(c351).

GameObject Stop uses authoritative subobjects position/destination, not the
controller's per-frame copies (actor_runtime.cpp85/94 synchronizes these).
Source effect order: DropPath, destination, heading/path bytes, zero direction,
physical policy, NativeBody linear/angular/position/sleep, Character event3f.
Existing state focus `character_service(stop)` currently executes physical
before DropPath; replace its body with heading_owner->object_stop(error) and
refresh facts to preserve original ordering without dispatching extra3f.

Earlier host owner checks PASS35 each O1/O2 ASAN/UBSAN; that fixture incorrectly
made TargetState AI identity equal Character identity. Current source validates
targets.owner->identity and its revised fixture deliberately separates101/1,
including mismatch rejection. Current host rerun awaits WSL; strict production
header-order compile PASS ARM64/x86_64 after this correction. Host uses actual frozen native heading/target/path/body
kernels with service observers; no original ARM differential or live movement
claim for this new owner. Existing live authored movement remains separate
until root wires and validates this transport.
