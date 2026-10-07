# Source work after the Prince bank checkpoint

Current checkpoint and live-test status is recorded in
`ai-timers-alias-source-status.md`. The sections below describe the earlier
source integration stage; they are not the newest candidate inventory.

The last saved, emulator-verified checkpoint remains
`dh2-native-prince-bank-4f5b7d11.apk`. The following work is newer than that APK.
It must not be attributed to the saved checkpoint or described as live gameplay.

`character_controller_commands` now reconstructs complete controller LookAt,
MoveTo and Stop gates and their selected Character wrappers. The original
instruction corpus and project-built ARM64 world library agree on 2,456 cases
and 2,329 ordered requests. The actual host world shared library replays the
same corpus under ASan/UBSan with zero findings, ten caller guards and eight
service-failure prefixes. Character's four-byte LookAt(Point) override branches
to GameObject.LookAt(Point); it is not empty. Complete point-facing, PathTo,
GameObject Stop and Character RaiseEvent remain explicit borrowed services.

`character_ai_update` reconstructs the complete CharAI update wrapper, including
active-script dispatch, zoning/visibility gates and initial-position restoration.
It is included in the native world build. Its source proofs and composed
selected-script replay are tracked in the level-world reports. Actual Prince
player classification suppresses the zonable NPC branch; that behavior must
not be replaced with generic NPC logic.

`script-runtime` contains a source-built Lua 5.1.4 backend with float32 numbers,
native 64-bit pointers and a fixed-width reader for the original chunk format.
The standalone host audit passes 438 checks with no sanitizer findings. Both
standalone Android ABI builds pass. The actual common script loads and its 29
empty callbacks and pure animation callbacks execute. Genuine StartTimer,
StopTimer and Trace bindings, full LuaManager registration and live character
ownership remain work in progress. This runtime is not yet in the checkpoint.

Pre-attack recovery corrects the earlier label at `3d67f4`: it is AI_CanAttack,
not a target getter. Its hostile-target, equipment and range services must be
connected before the complete animation/AI callback path can be called live.
The source flow is now integrated as `character_ai_state`; 1,024 cases and
2,006 ordered queries match the original through the project-built ARM64 world
library. The host world-DSO audit also passes under ASan/UBSan. Predicate service
backends remain explicit. Its project instruction runner supplies the existing
Android TLS-canary fixture; it does not prove physical device thread behavior.

Both Android project ABIs compile with the new controller and AI-update modules.
The current debug build is a source integration build, not a new saved runtime
checkpoint. No physical ARM64 device, complete NPC/quest progression, original
UI, audio or saves have been verified by this source work.
