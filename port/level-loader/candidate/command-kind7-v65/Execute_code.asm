0x45c5e8 70309fe5 ldr r3, [pc, #0x70]
0x45c5ec 70209fe5 ldr r2, [pc, #0x70]
0x45c5f0 70402de9 push {r4, r5, r6, lr}
0x45c5f4 03308fe0 add r3, pc, r3
0x45c5f8 025093e7 ldr r5, [r3, r2]
0x45c5fc 64209fe5 ldr r2, [pc, #0x64]
0x45c600 20d04de2 sub sp, sp, #0x20
0x45c604 04408de2 add r4, sp, #4
0x45c608 026093e7 ldr r6, [r3, r2]
0x45c60c 003095e5 ldr r3, [r5]
0x45c610 0600a0e1 mov r0, r6
0x45c614 1c308de5 str r3, [sp, #0x1c]
0x45c618 9a6cfbeb bl #0x337888
0x45c61c 48109fe5 ldr r1, [pc, #0x48]
0x45c620 0d20a0e1 mov r2, sp
0x45c624 0400a0e1 mov r0, r4
0x45c628 01108fe0 add r1, pc, r1
0x45c62c aedefaeb bl #0x3140ec
0x45c630 0410a0e1 mov r1, r4
0x45c634 0600a0e1 mov r0, r6
0x45c638 126dfbeb bl #0x337a88
0x45c63c 0400a0e1 mov r0, r4
0x45c640 03effaeb bl #0x318254
0x45c644 1c209de5 ldr r2, [sp, #0x1c]
0x45c648 003095e5 ldr r3, [r5]
0x45c64c 030052e1 cmp r2, r3
0x45c650 0100001a bne #0x45c65c
0x45c654 20d08de2 add sp, sp, #0x20
0x45c658 7080bde8 pop {r4, r5, r6, pc}
0x45c65c 2bc7faeb bl #0x30e310
