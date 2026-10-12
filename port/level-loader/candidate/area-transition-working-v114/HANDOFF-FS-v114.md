# Original FS confirmation dispatch

Add menu_fs_command_v114.cpp once to the existing native owner. Use the matching header and latest area_transition_request_v114.hpp/.cpp. No second menu roster, native-function table, loading system or Save is created.

The actual RenderFX.fc handler controls routing. A NULL handler is the original no-call branch. MenuFX forwards only its current top state through virtual30. MenuBase.OnFSCommand forwards virtual38. MenuManager traces then resolves the literal menu_HUD_0 and calls its virtual38; do not replace it with the front menu or active character menu.

MenuBase.MyFSCommand handles NULL command as true before tracing. LoadLevel compares its actual args with the empty string, pops all menus on the SAME MenuFX render, then calls Application with filename, entry0, profileSlot0, f1false, useSpawntrue, mode0, bypassfalse, seeds0/0. NULL args are outside the original safe pointer domain. LoadLevel2 checks NULL/empty/missing member, resolves the original exact LevelList name, then forwards virtual38("LoadLevel", row20file), reporting true after delivered forwarding regardless of its inner handled result. Other names retain the existing Main native-function-table authority.

Main supplies current/trace, actual roster capture_receiver_v104, source virtual30/38 dispatch, real MenuFX PopAll, actual process LevelList and Application confirmation provider. Transport bool and original handled result remain separate. Terminal dispatch may legitimately change the GameState and therefore performs no later old-World reads or stale-scope rejection. Intermediate native calls retain current guards.

Confirmed Application profile slot and destination domain are explicit. Preserve native literal0. A different selected Save/profile is rejected before retirement; never silently rewrite the slot or change the selected actor. Raw filename paths use borrow_area_transition_filename_level_v114, the same original first-substring C1 selector, and preserve the raw App/GS filename. Member-name calls resolve row20file first.

Root provides scoped SWF FS transport, current independent serialized Save receipt, prepared profile/start, outer-frame sequence and genuine selected-profile restore completion. Old actor identity is a receipt, not the address of the newly allocated actor. Source-ready; no build, tests, compiler, proof/freeze, audit or device jobs ran.
