# Original Crypt Idle startup producers

This is a bounded startup/readiness recovery, not a complete Character frame,
ObjectManager/RoomZone loader or AI implementation. Shipping ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`startup-probes.json` contains50 actual ARM32 cases:20 complete outer
GameObject/Character constructors with five distinct memory prefills,27 actual
Character zonability/ZoneEntered/ZoneExited cases and3 actual Type4
IdleCommonUpdate branches. Allocation/container/base/network/property/StateInfo
constructor services are explicit fixtures. Those deeper factories are not
proved by this probe; native comparisons in this report are deliberately0.

## Constructor fields and gates

| Producer | Actual resulting field | Evidence |
| --- | --- | --- |
| GameObject C1/C2 | Position and destinationXYZ0; headingXYZ0, heading active0 | `38c1ac..38c218`, complete outer execution |
| GameObject C1/C2 | zoned byte+2ee1; in-zone byte+2f0=0; zone owner+2f4NULL; physical+2dcNULL | `38c2c0..38c2ec`, complete outer execution |
| Character inline controller ctor | Controller+8 locked0,+9 forced0,+a0; controller+4 points embedded controllable, +c points Character | `3aa5fc..3aa648`, allocation identity supplied; actual404e10 SetController executes |
| CharAI ctor | paused+18=0; active+1c/pending+20/target+40NULL | `3ced50` and C2 executed during Character construction |
| FSM ctor | currentNULL,flags0,mask0,elapsed0,animation override-1; suppression0 | `3c1ac4` and C2 executed; machine+3c equals Character+538 |
| CharAnimator ctor | closed1,global/step speeds1,pending sequence-1 | `3c906c` and C2 executed |
| Character ctor | dead+1449=0; FX offset+1488=0 | `3aa430`, `3aa4b4..3aa4b8`, direct source stores |

The probes prove that Character+441/+442 retain the supplied allocation
prefill. Neither the Character nor CharAI constructor writes them. Do not label
zero-valued native `State/Facts` defaults as original producers for these
attack predicate bytes. Keep their first genuine writer/required predicate
boundary explicit. Likewise `walk_speed=1`, `attack_speed=1`, thresholds and
destination facts in the C++ convenience struct are not recovered startup
values; resolve them when a reachable service needs them.

`SetHeadingDirection393be8` is the genuine heading-active producer: it writes
XY,forcesZ0, evaluates single-precision X²+Y² against bits38d1b717, stores
active+1b5, normalizes magnitudes above1 and optionally calls LookTowards.
GameObject UpdatePath3940c0 supplies a direction from the actual path/destination
and invokes that producer. Idle Focus neither enables heading nor writes a
destination. `IsAtDestination39361c` compares actual path/destinationXY with
ownerXY using squared distance<6400. Constructor destination0 does not imply
that an actor moved to its authored position is at its destination.

Actual Type4 `IdleCommonUpdate3c0b78` with heading0 calls genuine IsPlayer and
returns immediately when false. It does not need player-manager, move, target,
physics, script tick or point-controller effects. Heading1 or255 instead emits
Character event0/payloadNULL immediately; that event must go through the genuine
AI/FSM relay, never be accepted by an empty fixture in live wiring.

## Zone and AI distinction

Character IsZonable3a36e4 first calls actual IsPlayer, then IsFaerie3a3094.
Player(Type1) and Faerie(Type3) return0. Other types tailcall
GameObject::MeetCondition38ab60, whose entire body is `mov r0,#1; bx lr`.
It is not GameObject::IsZonable3883b8 (the separate +2ed byte getter).
All four decoded Crypt kinds are Type4 and therefore zonable1.

CharAI Update3cfbf4 checks paused, forced/global block/locked, flags520 bit100,
then IsZonable/zoned/in-zone before target/master/aggro/OnUpdate. Idle Focus
flags2380 enable that policy bit, but do not set in-zone. Fresh constructor
in-zone0 is not the final loaded-scene producer:

* ObjectManager InitPost34552c phase3 invokes object InitPost virtual+1c at3457f8.
* Phase4 calls RoomZone::InitObjectList396c44 at3456b8. Its per-object
  AddInitialObject396a90 calls actual zonability and checks ownerXY against the
  zone's absolute min/max with inclusive float comparisons.
* Accepted entries store the genuine zone ownership pointer/byte and call
  ZoneEntered38c710 at396bbc. This stores in-zone1 before visibility/updating
  services. ZoneExited38c69c stores0 first. The executing27 cases verify the
  distinct Player/Faerie/Type4 and zoned0/nonzero updating branches.

Full zone geometry/manager enumeration is static instruction evidence here,
not a complete executed loader. Do not infer membership merely from DACT room
number or force in-zone1. A legitimate bounded Idle FSM/animation can run before
full target/master/aggro AI services exist; the source AI branch must fail
explicitly if it reaches missing providers, or stay source-skipped through a
genuine gate. No invented lock/forced/paused gate may replace those providers.

## Exact minimal integration chronology

Existing source stage proof in character-pre-spawn shows Level stage10 completes
ObjectManager InitPost before stage18 _LoadCharStates. Stage18 does initial FSM
selection before normal Character::Update. Thus preset3 can truthfully enter
Idle and produce an initial scene for rendering without pretending the full
Character frame has executed.

1. Retain one Character identity, decoded property/combat owners, StateInfo
   registry/current owner, controller/path, private CPU Scene/animation bank,
   TimerStore/script session, Debug singleton/module maps and native body.
   Reuse the single owner; do not assign current directly or allocate a second FSM.
2. Apply actual XML/default CAI1 metadata, property/class/equipment producers and
   source base visual scale. All11 current Crypt presets resolve3: five authored
   Idle and six missing/empty values resolved through the source default chain.
3. Preserve model-factory cached bbox chronology. GameObject InitPost precedes
   animation-set registration. The source SetAnimationSet/default-library merge
   does not apply a template/Idle pose. CharacterAnimationInstance.scene()
   immediately after create is the bounded fresh factory input for NPC body
   projection, before any start/scene_phase.
4. Preserve InitPost's delayed branch/FX/animation setup order. For nonplayer
   AI delayed_load!=0,3b4fdc..3b4ff0 stores Character+3ec=1 and skips
   LoadScriptProcess3b5010. The other branch loads there. Both then execute
   GrabAnimFX3b5040 and RegisterCharacterFXTable3b5050; animation
   registration follows at3b5090. Actual AI row+30 self_fx is-1, constructor
   offset1488=0, so Grab(-1) executes its Debug prefix then returnsNULL.
   Character Effects resolved index7=1 has blood-set79 and still requires genuine
   registration; it cannot be skipped because self_fx is empty.
5. Source SetInitialPosition3b53d4→SetPosition3b53e8→stored visual bbox Apply
   3b541c→Revive(NULL,true)3b542c. Revive calls InitPhysicalObject3a5ad4 before
   its return. Type4/nonstatic/nonplayer source creates the proved circle,
   group2/category10/maskd3f, mass-from-shapes then pin. Static authored byte84
   false and pinned mass0 are separate facts. Idle leaves this body pinned;
   genuine Move Focus later unpins, restoring mass and waking it. Do not unpin
   during Idle startup merely to make the body dynamic.
6. InitPost then _InitHpMp3b5434 and, unless source delayed-load byte3ec is set,
   InitScriptProcess(false)3b54c8, followed by AddToGroup3b5448. Required
   skills/spells/virtual init effects remain genuine services. All four AI rows
   have delayed_load1, so actual fresh Crypt InitPost skips both script calls.
   Active AIS remains null at this boundary. Current early private-session Init
   is a separately staged native operation, not this original caller chronology.
7. Complete genuine phase4 zone membership/updating and Level stage18
   initialize_level(preset3). That owner emits the original Idle Focus and
   state-change Character event1d. Actual current delayed Crypt startup has null
   active AIS here and source OnStateChanged3d0bec genuinely skips that delivery.
   Other non-delayed callers can already have active AIS: use the live pointer,
   never a universal null or always-published default.
8. Resolve Idle Facts from actual table Idle base, real constants stance mask and
   GetAnimStance. Type4 IsPlayerfalse selects stance0 before the signed count
   clamp; no visual weapon inference. Refresh State.body_present from native
   attachment. Start through genuine owned Focus/service selection and preserve
   synchronous animation-event observer/reentry. CPU scene sampling/rendering
   may run at this stage. The existing CharacterIdleEvents.initialize_idle
   requires published AIS as its bounded adapter contract; that is not the
   original delayed-monster startup producer and must not be claimed as such.
9. First eligible Character::Update3abe98 checks current state0/12/2, byte1480
   and active AIS+3e4 at3abf84..3ac380. Idle3/byte1480=0/activeNULL reaches
   LoadNInitScriptProcess(true)3ac404, before controller Update3abfd0,
   timers3ac02c and AI. Its full manager/save/UI loading branch remains required;
   this note captures that branch statically rather than pretending a complete
   frame. Bind the recovered actual session/lifecycle here when the upstream
   branch services are supplied. A subsequent frame must use the recovered scene→world
   Step→timers→AI/FSM/animator chronology, with genuine elapsed/clock providers
   and explicit missing services. This note does not authorize full AI acceptance.

`fx-data.json` is a separate native cache projection using source-proved property
and AI decoders, linked to historical stable data DSO ac2e2f7c…; it is not a full
original InitPost replay. All four kinds resolve AI40/68, Effects1, self_fx-1,
delayed_load1; property30 is RangeMinDistance0. `provenance.json` binds that
binary/input/source boundary and the original captures. No shared production,
CMake, renderer, APK or emulator was changed by this startup investigation.
