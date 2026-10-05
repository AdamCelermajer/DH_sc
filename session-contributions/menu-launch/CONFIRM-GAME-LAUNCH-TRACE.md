Class Confirm source trace (investigation, no launch implementation yet)

The original Android SWF sprite428 frame0 assigns btn_Confirm.onRelease: play MenuConfirm, goto label wait, play. The class-confirm work happens in frame29, not in MenuCharacterSelect::OnEvent and not in that immediate onRelease body.

The preserved original frame29 actions at 0x3f6b1..0x3f865:
1. _root.SlotID = NativeCreateSaveSlot(_root.PlayerName, _root.PlayerClass).
2. NativeAssignSaveSlotToPlayer(_root.SlotID, 0).
3. _root.menu_MainMenu.current_slot = _root.SlotID.
4. NativePopAllAbove("menu_MainMenu").
5. If InvitePending, NativeStartFromGCInvite(SlotID); otherwise NativePushMenu("menu_StartGame").

At the start of this investigation all three ordinary-path functions CreateSaveSlot, AssignSaveSlotToPlayer and PopAllAbove were absent from the snapshot original_ui_session native action registrations. v54 connects PopAllAbove; CreateSaveSlot and AssignSaveSlotToPlayer remain absent. The immediate sound is registered. This distinguishes the delayed launch gap from class arrow handling. No game-launch success is claimed.

Bounded original ARM disassembly is retained in confirm-source-v54.asm. NativeCreateSaveSlot 0x43f630 (size0x1f8) resolves the selected class string against the original class table, validates IsPlayableClassID, asks SG_GetNextFreeSlot, constructs PlayerSavegame(slot,1,false), sets name/class/starting fields, generates seeds/date and calls SG_Save, then returns the numeric slot. This cannot be faithfully replaced with a direct hardcoded-world launch or an invented save record.

NativeAssignSaveSlotToPlayer 0x43cf50 (size0x64) converts its two numeric arguments and invokes AssignSaveSlotToPlayer(slot,playerIndex) for nonnegative values. NativePopAllAbove 0x43ac28 (size0x68) validates a single string and calls the menu manager virtual method. These owner methods and menu_StartGame remain to be traced and connected to the native save/game runtime.

The main checkout and main emulator were untouched. v54 now includes the independently verified stack callback; see CONFIRM-STACK-CALLBACK.md for exact test scope. It still does not implement save creation or game launch.
