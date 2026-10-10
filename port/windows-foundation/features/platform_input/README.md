# Semantic platform input

`SemanticInput` converts transport events to existing `InputActions` and button
edges without requiring Win32 or Android headers. The platform retains ownership
of event polling, coordinate conversion, and authored HUD/menu hit testing.

Feed `key(VK, down)` from Win32 keyboard events or state changes. Feed mouse left
button and move events to `pointer(mouseId, phase, point)`, with a stable ID that
does not collide with touch IDs. Android down/pointer-down, move, up/pointer-up,
and cancel map to the corresponding `PointerPhase` using each native pointer ID.
Use the same viewport coordinates for events and `Surface::hit`.

Hit tests must use actual rendered/authored receiver shapes. The adapter does not
invent touch rectangles. Return `Control::menu_item` and an item ID for the active
menu's authored shapes, `Control::profile` for the profile receiver, and matching
controls for the gameplay HUD. `Surface::joystick` returns normalized movement
from the original stick's origin/current positions. Once captured, an attack
finger stays held while moving outside the attack button; up/cancel releases it.
Click actions require an up on the same authored item as the down.

Call `set_menu_open` before dispatching a newly opened/closed menu. This consumes
gameplay inputs, cancels pointer captures, and requires held keys to be released
before they can control gameplay again. `lose_focus` handles focus/app suspension.
`set_level_input_enabled(false)` corresponds to the source disabled-level gate.

Call `take_frame(controllerBlocked)` once per game frame. Forward `frame.actions`
through existing controller admission. Use `frame.attack` press/held/release for
combo ownership, and pass skill edges/slot indices to actual skill commands. This
adapter does not synthesize casts, stats, cooldowns, target actors, or successful
controller commands. The source attack path chooses `Cmd_UseOOI(NULL)` when an
object of interest exists, otherwise `Cmd_Attack(NULL)`; that choice belongs to
the retained HUD/controller owner, not the platform adapter.

Source references: `port/engine-ui/hud_attack_control_v46.hpp/.cpp` source event
4=press, 6=release, 7=release-outside/cancel; held attack dispatch and disabled
level gate. `authored_gameplay_hud_v1` and `hud_manager` define authored skill1..3,
spell, potion, and joystick receivers. The original touch layout and controller
gates remain authoritative. PC bindings are an explicit new transport mapping:
WASD/Shift movement, Space attack, E interaction, Tab target, C profile,
1/2/3 skill slots, 4 spell, 5 potion, Escape menu back.

The adapter and tests are isolated; no Android/compatibility backend is modified.
Root integration must wire actual window mouse events and authored hit tests;
standalone test success does not demonstrate in-game menu or touch integration.
