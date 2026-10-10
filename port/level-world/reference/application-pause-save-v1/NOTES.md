# Android focus-loss player save

The original DEX `GameGLSurfaceView.onWindowFocusChanged(false)` invokes
`nativePause(1)`. The native thunk enters `appPause` and
`Application::Pause`. The pause routine obtains the current Level and invokes
`Level::SG_SavePlayer(0, false)` only when Level `+0x130 == 38` and byte
`+0x144 != 0`. It then saves settings only when the settings dirty byte is set.

The Android port delivers this callback from `MainActivity.onWindowFocusChanged`
through `GLSurfaceView.queueEvent`, so source Level/World/Character access stays
on the renderer thread. The activity waits for delivery before `onPause` can
pause the only frame pump. A 15-second timeout is logged as a failed delivery;
it is not reported as a successful save.

The player save path queries the same current Level's existing PlayerManager,
checks the source roster bound, fetches player index 0 with the original `true`
argument, and executes the Character `IsPlayer` / blocked-state / level / date /
entry-point / `SG_Save` sequence on the retained Character14e8 Save authority.
It does not call Level QuickSave or SaveAllPlayers.

Queue ordering is deliberate:

1. The Character profile writer serializes through the existing shared
   Application Savegame/FileManager queue.
2. The dirty-settings gate calls the existing synchronous settings writer;
   that writer first flushes matching settings-file jobs.
3. Before returning to Android lifecycle code, the adapter flushes the same
   process queue with a null filename, draining the profile write before the
   renderer is suspended. A queued save by itself is not a durability receipt.

This adaptation does not yet implement every side effect in
`Application::Pause` (sound pause flags, state-machine sleep, interrupt
handling, and orientation). It covers the IDA guarded player-save and dirty
settings persistence edge. The source regression tests the Level guards and
exact `(player=0, block=false)` callback arguments; device runtime durability
still needs verification.
