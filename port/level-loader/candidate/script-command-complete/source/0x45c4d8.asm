0045c4d8 70309fe5 ldr r3, [pc, #0x70]
0045c4dc 70209fe5 ldr r2, [pc, #0x70]
0045c4e0 70402de9 push {r4, r5, r6, lr}
0045c4e4 03308fe0 add r3, pc, r3
0045c4e8 025093e7 ldr r5, [r3, r2]
0045c4ec 64209fe5 ldr r2, [pc, #0x64]
0045c4f0 20d04de2 sub sp, sp, #0x20
0045c4f4 04408de2 add r4, sp, #4
0045c4f8 026093e7 ldr r6, [r3, r2]
0045c4fc 003095e5 ldr r3, [r5]
0045c500 0600a0e1 mov r0, r6
0045c504 1c308de5 str r3, [sp, #0x1c]
0045c508 de6cfbeb bl #0x337888
0045c50c 48109fe5 ldr r1, [pc, #0x48]
0045c510 0d20a0e1 mov r2, sp
0045c514 0400a0e1 mov r0, r4
0045c518 01108fe0 add r1, pc, r1
0045c51c f2defaeb bl #0x3140ec
0045c520 0410a0e1 mov r1, r4
0045c524 0600a0e1 mov r0, r6
0045c528 566dfbeb bl #0x337a88
0045c52c 0400a0e1 mov r0, r4
0045c530 47effaeb bl #0x318254
0045c534 1c209de5 ldr r2, [sp, #0x1c]
0045c538 003095e5 ldr r3, [r5]
0045c53c 030052e1 cmp r2, r3
0045c540 0100001a bne #0x45c54c
0045c544 20d08de2 add sp, sp, #0x20
0045c548 7080bde8 pop {r4, r5, r6, pc}
0045c54c 6fc7faeb bl #0x30e310
0045c550 ac855300 subseq r8, r3, ip, lsr #11
0045c554 ac400000 andeq r4, r0, ip, lsr #1
0045c558 84080000 andeq r0, r0, r4, lsl #17
0045c55c 680b4700 subeq r0, r7, r8, ror #22
