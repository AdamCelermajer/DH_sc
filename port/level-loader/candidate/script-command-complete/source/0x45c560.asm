0045c560 70309fe5 ldr r3, [pc, #0x70]
0045c564 70209fe5 ldr r2, [pc, #0x70]
0045c568 70402de9 push {r4, r5, r6, lr}
0045c56c 03308fe0 add r3, pc, r3
0045c570 025093e7 ldr r5, [r3, r2]
0045c574 64209fe5 ldr r2, [pc, #0x64]
0045c578 20d04de2 sub sp, sp, #0x20
0045c57c 04408de2 add r4, sp, #4
0045c580 026093e7 ldr r6, [r3, r2]
0045c584 003095e5 ldr r3, [r5]
0045c588 0600a0e1 mov r0, r6
0045c58c 1c308de5 str r3, [sp, #0x1c]
0045c590 bc6cfbeb bl #0x337888
0045c594 48109fe5 ldr r1, [pc, #0x48]
0045c598 0d20a0e1 mov r2, sp
0045c59c 0400a0e1 mov r0, r4
0045c5a0 01108fe0 add r1, pc, r1
0045c5a4 d0defaeb bl #0x3140ec
0045c5a8 0410a0e1 mov r1, r4
0045c5ac 0600a0e1 mov r0, r6
0045c5b0 346dfbeb bl #0x337a88
0045c5b4 0400a0e1 mov r0, r4
0045c5b8 25effaeb bl #0x318254
0045c5bc 1c209de5 ldr r2, [sp, #0x1c]
0045c5c0 003095e5 ldr r3, [r5]
0045c5c4 030052e1 cmp r2, r3
0045c5c8 0100001a bne #0x45c5d4
0045c5cc 20d08de2 add sp, sp, #0x20
0045c5d0 7080bde8 pop {r4, r5, r6, pc}
0045c5d4 4dc7faeb bl #0x30e310
0045c5d8 24855300 subseq r8, r3, r4, lsr #10
0045c5dc ac400000 andeq r4, r0, ip, lsr #1
0045c5e0 84080000 andeq r0, r0, r4, lsl #17
0045c5e4 e00a4700 subeq r0, r7, r0, ror #21
