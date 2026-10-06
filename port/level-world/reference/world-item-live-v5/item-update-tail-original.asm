
GameObjectUpdate 0038cbe8
0038cbe8 push {r4, r5, r6, r7, r8, lr}
0038cbec ldr r5, [pc, #0x138]
0038cbf0 ldr r7, [pc, #0x138]
0038cbf4 mov r4, r0
0038cbf8 add r5, pc, r5
0038cbfc ldr r3, [r5, r7]
0038cc00 ldr r0, [pc, #0x12c]
0038cc04 sub sp, sp, #0x20
0038cc08 ldr r3, [r3]
0038cc0c add r0, pc, r0
0038cc10 add r6, sp, #4
0038cc14 str r3, [sp, #0x1c]
0038cc18 bl #0x3136b4
0038cc1c ldr r3, [pc, #0x114]
0038cc20 ldr r8, [r5, r3]
0038cc24 mov r0, r8
0038cc28 bl #0x337888
0038cc2c ldr r1, [pc, #0x108]
0038cc30 mov r2, sp
0038cc34 mov r0, r6
0038cc38 add r1, pc, r1
0038cc3c bl #0x3140ec
0038cc40 mov r1, r6
0038cc44 mov r0, r8
0038cc48 bl #0x337a88
0038cc4c mov r0, r6
0038cc50 bl #0x318254
0038cc54 ldr r3, [pc, #0xe4]
0038cc58 ldr r3, [r5, r3]
0038cc5c ldr r3, [r3, #0x38]
0038cc60 ldr r2, [r3, #0x58]
0038cc64 add r2, r2, #1
0038cc68 str r2, [r3, #0x58]
0038cc6c ldr r1, [r4, #0x2e4]
0038cc70 cmp r1, #0
0038cc74 beq #0x38cc90
0038cc78 ldr r3, [r4]
0038cc7c mov r0, r4
0038cc80 mov lr, pc
0038cc84 ldr pc, [r3, #0x98]
0038cc88 mov r3, #0
0038cc8c str r3, [r4, #0x2e4]
0038cc90 ldr r3, [r4, #0x174]
0038cc94 ldr lr, [r4, #0x160]
0038cc98 ldr ip, [r4, #0x164]
0038cc9c ldr r1, [r4, #0x16c]
0038cca0 ldr r2, [r4, #0x170]
0038cca4 ldr r0, [r4, #0x168]
0038cca8 str r3, [r4, #0x1a4]
0038ccac str lr, [r4, #0x190]
0038ccb0 str ip, [r4, #0x194]
0038ccb4 str r1, [r4, #0x19c]
0038ccb8 str r2, [r4, #0x1a0]
0038ccbc str r0, [r4, #0x198]
0038ccc0 mov r0, r4
0038ccc4 bl #0x3940c0
0038ccc8 mov r0, r4
0038cccc bl #0x393710
0038ccd0 mov r0, r4
0038ccd4 bl #0x3943cc
0038ccd8 mov r0, r4
0038ccdc bl #0x393d74
0038cce0 mov r0, r4
0038cce4 bl #0x38b8b8
0038cce8 mov r3, #0x370
0038ccec ldrsh r3, [r4, r3]
0038ccf0 cmp r3, #0
0038ccf4 blt #0x38cd00
0038ccf8 mov r0, r4
0038ccfc bl #0x38ae2c
0038cd00 ldr r0, [pc, #0x3c]
0038cd04 add r0, pc, r0
0038cd08 bl #0x3136b8
0038cd0c ldr r3, [r5, r7]
0038cd10 ldr r2, [sp, #0x1c]
0038cd14 ldr r3, [r3]
0038cd18 cmp r2, r3
0038cd1c bne #0x38cd28
0038cd20 add sp, sp, #0x20
0038cd24 pop {r4, r5, r6, r7, r8, pc}
0038cd28 bl #0x30e310
0038cd2c mlseq r0, r8, lr, r7
0038cd30 andeq r4, r0, ip, lsr #1
0038cd34 subseq r5, r3, r4, asr r8
0038cd38 andeq r0, r0, r4, lsl #17
0038cd3c subseq r3, r3, r0, ror #14
0038cd40 strdeq r3, r4, [r0], -r4
0038cd44 subseq r5, r3, ip, asr r7

RequireOnlineUpdate 0038b8b8
0038b8b8 push {r4, lr}
0038b8bc mov r4, r0
0038b8c0 bl #0x7fd794
0038b8c4 ldrb r3, [r0, #5]
0038b8c8 cmp r3, #0
0038b8cc beq #0x38b8f4
0038b8d0 ldr r3, [r4, #0x100]
0038b8d4 cmp r3, #0
0038b8d8 beq #0x38b8f4
0038b8dc bl #0x7fd794
0038b8e0 bl #0x7fd5b4
0038b8e4 cmp r0, #0
0038b8e8 beq #0x38b8f8
0038b8ec mov r3, #1
0038b8f0 strb r3, [r4, #0x119]
0038b8f4 pop {r4, pc}
0038b8f8 ldr r3, [r4]
0038b8fc mov r0, r4
0038b900 mov lr, pc
0038b904 ldr pc, [r3, #0x54]
0038b908 cmp r0, #0
0038b90c bne #0x38b8f4
0038b910 b #0x38b8ec
0038b914 sub r0, r0, #0x24
0038b918 b #0x38b91c
0038b91c push {r4, r5, lr}
0038b920 mov r5, r1
0038b924 sub sp, sp, #0x14
0038b928 mov r4, r0
0038b92c bl #0x33e0f8
0038b930 add r1, sp, #0xc
