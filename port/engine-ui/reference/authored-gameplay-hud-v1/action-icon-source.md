# MenuManager action icon

Original ELF SHA25636498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80.
MenuManager C1 431d08 mvn r3,#0;431d0c store+108 initializes cache−1.
Update42eab4..42ead0 reads SIGNED Character byte14a8; unsigned CMP10
maps negative and above10 to5. Table8c9f28 contains eleven words
0,1,2,3,5,5,6,7,5,5,4. Matching cache skips the callback. Flash absent
skips publication; available Flash publishes cache42eaf0 before invoking
root FillActionIcon42eb60 with NUMBER(icon). Owner requires a bound actual
movie, so unavailable Flash is represented by an unavailable owner.

Root authored function dqhud action2ec0 invokes CurrentHud.FillActionIcon.
NativeSwapEquipment442294, NativeBackToHud and Script_ShowFlash likewise
refresh using existing MenuManager cache. refresh_action_icon follows this
callback, including source−1 before first update; no synthetic sword default.

Character C2 3a9340:3a93d8 r6=−1;3a9590 r2=14a8;3a9594 STRB r6.
OOI pointer14a4 source3a9678/67c stores constructor r8=0. This field is
separate from AI target Character408; callers must borrow the same AttackState64.

Host actual SWF: all256 signed-byte mappings, positive root callback and real
authored btimg frames for types0..11; PASS404 checks each O1/O2 ASAN/UBSAN.
Host CurrentHud selection is an explicit fixture, native callbacks external.
Production strict compile both Android ABIs PASS. Root integrates live update.
