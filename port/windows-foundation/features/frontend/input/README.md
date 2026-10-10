# Frontend semantic input

`FrontendInput` receives actual enabled authored button IDs and a geometry hit
callback. The app maps IDs to the full source button paths supplied by frontend
art and flow. Coordinates have to be inverse projected into the same authored
stage used by the art. Menu replacement cancels captured presses. Mouse and
touch use stable pointer IDs; activation requires down/up on the same enabled
item. Keyboard arrows/Tab change focus and Return/Space release activates it.
These PC bindings are platform adaptations, not recovered original AS bindings.

Source policies recovered in the local IDA export under
`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so`:

- `pseudocode/0038/00385cf0.c`: QueryString clamps requested size to 100.
- `pseudocode/0038/00385a8c.c`: Update passes requested size + 1 to GetString.
- `pseudocode/0051/0051b444.c`: byte append, Backspace on down, Return completion
  on up, append limit leaves a terminator. Owned strings avoid the original
  static-buffer boundary hazard while retaining the requested byte limit.
- `pseudocode/0042/00422280.c`: SetPlayerName copies the C string unchanged and
  contains no alphabet or minimum length check.

`../flow/authored_navigation_evidence.json` records original
`dqmenus_droid.swf` SHA256
`c114c7d39b7fe0351a93a515c78f9457dda715d78aed6e3bcc3f4895120de421`.
The flow lane recovered authored EnterName `trim` (offset 35975) and
`isValidName` (offset 36337): only TAB/LF/CR/SPACE are trimmed for emptiness,
and the raw string is preserved. `validate_creation_name` implements that
acceptance check. The authored `isFullString` (offset 36470) limits keyboard
appending when length reaches eight; this is separate from name acceptance.
Callers supply the recovered requested keyboard byte size to `begin_name`.
No existing long name is silently truncated. Optional external/authored policies
explicitly return unavailable if not connected.

Compile the two cpp files with C++17 for the standalone test. On native Windows,
LLVM MinGW runtime DLLs must be on PATH. Tests cover source boundaries and raw
name preservation, exact whitespace behavior, keyboard release/repeat,
multi-touch capture/cancel, menu replacement, focus loss, and disabled input.

`ScreenInteraction` is the host bridge. It receives original art hit-region paths
(slash or dot separators), region-index+1 hits, and the selected slot fact; it
dispatches original EnterName QWERTY key values, Shift, Space, Delete and Confirm,
bounded class selection and Confirm to `Navigator`. It keeps the original
isCaps=false/initial UpperCase visibility distinction: first Shift can retain
UpperCase, as the decoded original does. Rebuild the surface after a screen,
class or keyboard-layer change, preserving it between pointer down/up events.
StartGame's `StartMenuButtons/btn_MENU_SINGLE_PLAYER` uses an explicitly supplied
difficulty fact and the real Navigator service. Missing difficulty/service fails.
Its tests link frontend_input.cpp, screen_interaction.cpp and flow/menu_flow.cpp.
