0045f4e8 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045f4ec c4409fe5 ldr r4, [pc, #0xc4]
0045f4f0 c4509fe5 ldr r5, [pc, #0xc4]
0045f4f4 c4109fe5 ldr r1, [pc, #0xc4]
0045f4f8 04408fe0 add r4, pc, r4
0045f4fc 053094e7 ldr r3, [r4, r5]
0045f500 01a094e7 ldr sl, [r4, r1]
0045f504 38d04de2 sub sp, sp, #0x38
0045f508 003093e5 ldr r3, [r3]
0045f50c 0290a0e1 mov sb, r2
0045f510 1c808de2 add r8, sp, #0x1c
0045f514 34308de5 str r3, [sp, #0x34]
0045f518 0c7090e5 ldr r7, [r0, #0xc]
0045f51c 0a00a0e1 mov r0, sl
0045f520 d860fbeb bl #0x337888
0045f524 98109fe5 ldr r1, [pc, #0x98]
0045f528 18208de2 add r2, sp, #0x18
0045f52c 0800a0e1 mov r0, r8
0045f530 01108fe0 add r1, pc, r1
0045f534 ecd2faeb bl #0x3140ec
0045f538 0810a0e1 mov r1, r8
0045f53c 0a00a0e1 mov r0, sl
0045f540 5061fbeb bl #0x337a88
0045f544 0800a0e1 mov r0, r8
0045f548 41e3faeb bl #0x318254
0045f54c 74109fe5 ldr r1, [pc, #0x74]
0045f550 0c608de2 add r6, sp, #0xc
0045f554 102097e5 ldr r2, [r7, #0x10]
0045f558 011094e7 ldr r1, [r4, r1]
0045f55c 00c0a0e3 mov ip, #0
0045f560 0930a0e1 mov r3, sb
0045f564 381091e5 ldr r1, [r1, #0x38]
0045f568 0600a0e1 mov r0, r6
0045f56c 04c08de5 str ip, [sp, #4]
0045f570 00c08de5 str ip, [sp]
0045f574 c9adfbeb bl #0x34aca0
0045f578 0600a0e1 mov r0, r6
0045f57c 7482fbeb bl #0x33ff54
0045f580 000050e3 cmp r0, #0
0045f584 0300000a beq #0x45f598
0045f588 4f0e80e2 add r0, r0, #0x4f0
0045f58c 0c0080e2 add r0, r0, #0xc
0045f590 0810d7e5 ldrb r1, [r7, #8]
0045f594 3689fdeb bl #0x3c1a74
0045f598 053094e7 ldr r3, [r4, r5]
0045f59c 34209de5 ldr r2, [sp, #0x34]
0045f5a0 003093e5 ldr r3, [r3]
0045f5a4 030052e1 cmp r2, r3
0045f5a8 0100001a bne #0x45f5b4
0045f5ac 38d08de2 add sp, sp, #0x38
0045f5b0 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045f5b4 55bbfaeb bl #0x30e310
