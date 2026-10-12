# Same generic controller startup idle binding

Source-only. Shared Root read-only; no tests/build/native execution/hash/freeze.
Original dynamic symbol PlayClip signature is char*,bool,int,unsigned, not three
bool flags. The paired narrow header patch corrects only Door/Trigger typed
startup signatures; actual startup values true/0/0 and original body order stay.
AnimController.PlayClip4749f4 is348bytes: r2 is loop; priorityr3 unused; group from
stack selects GetAnim4748b8 actual ordinal. Native GetApplicator369160(NULL)
returnsNULL; out-of-range/absent group mutates no timeline. The supported SAME
retained generic visual constructor exposes zero/one root animator. This helper
uses that actual presence and existing visual.play/source base kernel. Native
startup ignored accepted return, so a missing named clip does not become an
invented startup failure. No pose update or extra animation clock is executed.

Include renderer_startup_idle_animation_v86.hpp before V69 fragment; after
receiving actual DoorStartup/TriggerStartup bag, invoke
bind_startup_idle_animation_v86(weak,output.startup). Preserve any incoming
genuine provider. Exact actor/visual slot and constructor lease are borrowed at
reach; no new controller/C1/timeline/graph/state occurs. Character/replaced
animator domains are rejected by existing generic identity API.

Main still owns Door animation event/completion behavior, real physical filter,
floor dead-end flags, interaction/network/save/quest/cinematic execution.
This helper does not pretend those callbacks exist. Single-owner header
dependencies require coherent rebuild at integrated first-SWAMP milestone.
