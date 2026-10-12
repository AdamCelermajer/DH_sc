0045d0e0 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045d0e4 d8409fe5 ldr r4, [pc, #0xd8]
0045d0e8 d8609fe5 ldr r6, [pc, #0xd8]
0045d0ec d8109fe5 ldr r1, [pc, #0xd8]
0045d0f0 04408fe0 add r4, pc, r4
0045d0f4 063094e7 ldr r3, [r4, r6]
0045d0f8 017094e7 ldr r7, [r4, r1]
0045d0fc 38d04de2 sub sp, sp, #0x38
0045d100 003093e5 ldr r3, [r3]
0045d104 0080a0e1 mov r8, r0
0045d108 0700a0e1 mov r0, r7
0045d10c 34308de5 str r3, [sp, #0x34]
0045d110 0290a0e1 mov sb, r2
0045d114 0ca098e5 ldr sl, [r8, #0xc]
0045d118 da69fbeb bl #0x337888
0045d11c ac109fe5 ldr r1, [pc, #0xac]
0045d120 1c508de2 add r5, sp, #0x1c
0045d124 18208de2 add r2, sp, #0x18
0045d128 01108fe0 add r1, pc, r1
0045d12c 0500a0e1 mov r0, r5
0045d130 eddbfaeb bl #0x3140ec
0045d134 0510a0e1 mov r1, r5
0045d138 0700a0e1 mov r0, r7
0045d13c 516afbeb bl #0x337a88
0045d140 0500a0e1 mov r0, r5
0045d144 42ecfaeb bl #0x318254
0045d148 84309fe5 ldr r3, [pc, #0x84]
0045d14c 0c708de2 add r7, sp, #0xc
0045d150 0c209ae5 ldr r2, [sl, #0xc]
0045d154 031094e7 ldr r1, [r4, r3]
0045d158 0050a0e3 mov r5, #0
0045d15c 0930a0e1 mov r3, sb
0045d160 381091e5 ldr r1, [r1, #0x38]
0045d164 0700a0e1 mov r0, r7
0045d168 00508de5 str r5, [sp]
0045d16c 04508de5 str r5, [sp, #4]
0045d170 cab6fbeb bl #0x34aca0
0045d174 0700a0e1 mov r0, r7
0045d178 0510a0e1 mov r1, r5
0045d17c 0f8bfbeb bl #0x33fdc0
0045d180 000050e3 cmp r0, #0
0045d184 0400000a beq #0x45d19c
0045d188 f43090e5 ldr r3, [r0, #0xf4]
0045d18c 120053e3 cmp r3, #0x12
0045d190 10008805 streq r0, [r8, #0x10]
0045d194 7957c005 strbeq r5, [r0, #0x779]
0045d198 0100000a beq #0x45d1a4
0045d19c 0030a0e3 mov r3, #0
0045d1a0 103088e5 str r3, [r8, #0x10]
0045d1a4 063094e7 ldr r3, [r4, r6]
0045d1a8 34209de5 ldr r2, [sp, #0x34]
0045d1ac 003093e5 ldr r3, [r3]
0045d1b0 030052e1 cmp r2, r3
0045d1b4 0100001a bne #0x45d1c0
0045d1b8 38d08de2 add sp, sp, #0x38
0045d1bc f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045d1c0 52c4faeb bl #0x30e310
0045d1c4 a0795300 subseq r7, r3, r0, lsr #19
0045d1c8 ac400000 andeq r4, r0, ip, lsr #1
0045d1cc 84080000 andeq r0, r0, r4, lsl #17
0045d1d0 58ff4600 subeq pc, r6, r8, asr pc
0045d1d4 f4370000 strdeq r3, r4, [r0], -r4
