_ZNK9Character9IsMonsterEv 3a3064 24
3a3064 push {r4, lr}
3a3068 bl #0x3a3054
3a306c cmp r0, #4
3a3070 movne r0, #0
3a3074 moveq r0, #1
3a3078 pop {r4, pc}
_ZNK9Character6IsBossEv 3a3158 20
3a3158 push {r4, lr}
3a315c bl #0x3a3024
3a3160 ldr r0, [r0, #0x14]
3a3164 ubfx r0, r0, #2, #1
3a3168 pop {r4, pc}
_ZN9CharacterC1EN10ObjectBase6GO_IDSE 3aa1b4 1448
3aa240 mov r8, #0
3aa4e4 movw r2, #0x14a0
3aa4e8 str r8, [r4, r2]
3aa4ec movw r2, #0x14a4
3aa4f0 str r8, [r4, r2]
3aa4f4 movw r2, #0x14aa
