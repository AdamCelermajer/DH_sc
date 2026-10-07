# Native menu lifecycle integration V58

Source implementation added on 2026-10-06 after the Windows memory incident.
At initial V58 introduction no compiler, WSL, APK, emulator or device test was
launched. Subsequent V59/V91 source/ABI validation and integration are recorded
in `menu-lifecycle-v59-source-integration.md`; gameplay acceptance is still open.
The installed c6691313 checkpoint does **not** contain these changes.

| Subtask | Source state | Validation / remaining work |
|---|---|---|
| Application+ec shared with GS.Update | Both authored Show/Hide producers write the bound actual byte; root binding retains its Application lease and rejects replacement owners | Native fixture written; compilation and actual loading/menu checks pending |
| MenuManager.UnloadMenu42d4bc | Full ordering kernel linked into engine-ui: map icons for slot1, live primary slot/registry, render4 clear before owned7d read/D0, live erase, Flash reset, MultiUnload | Actual D0, map, Flash and resource services must still be bound |
| Generated MenuBase ownership7d | C1 default0 preserved; both actual PostLoad receiver producers set1 | Original source capture confirms42f298; runtime unloading pending |
| Live menu directory erase | Same MenuStackOwner registry mutated and view republished; lookup excludes erased records | Projection storage retained independently; this operation does not destroy source receivers or movies |
| MenuManager.Update42ea04 ordering | Slots0..3 or HUD3, actual Level198 gate, v10(dt,false), game-view HUD pair, Flash, captured menu count with live receiver rereads | Actual source prefix and visible-menu v1c providers remain required; no empty prefix accepted |
| GS frame/unload API | Root exports same-manager borrow/bind/unload and GS menu frame callbacks with owner/identity guards | Campaign agent can connect the API; exported callbacks fail until actual services are supplied |
| Behavioral fixtures | Tests cover destructor appending registry entries, unowned receivers, NULL/out-of-range slots, failure prefixes, HUD gating, visible-callback receiver mutation, captured count, early exit, same Application byte and retained lifetime | Written, **not run** under RAM hold |

Source authority: `reference/menu-manager-lifecycle-v58/manifest.json` and seven
bounded original ELF disassembly captures. These corroborate source call order;
they do not establish compiled or gameplay acceptance. The original ELF SHA256
is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.

The update prefix needs actual Application/PM/Debug services from42ea04..42ecac,
including local player+660, the OOI icon, wireframe/debug exits and listener88
cleanup. It cannot use the different BackToHud action-icon contract. MenuBase
Update421fe0 also performs real weak-context expiry and IsAnimOver/counter78;
it is not a literal no-op.

The menu session's V89/V90 packets remain separate uncompiled proposals. Its
graph-detach methods alone cannot implement complete MenuFX::Unload: the four
Controller::Reset calls and source state/auxiliary owners must also be supplied.
Whole-session reset_failed would destroy unrelated movie owners and is excluded.

Next functional acceptance remains the original main menu to a genuinely
constructed selected-profile GS/Swamp Level, then character menus, combat,
death/rewards and real save/continue. Crypt remains the installed demo route.
