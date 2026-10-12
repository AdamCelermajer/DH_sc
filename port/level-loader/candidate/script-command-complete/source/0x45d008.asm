0045d008 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045d00c b8409fe5 ldr r4, [pc, #0xb8]
0045d010 b8509fe5 ldr r5, [pc, #0xb8]
0045d014 b8109fe5 ldr r1, [pc, #0xb8]
0045d018 04408fe0 add r4, pc, r4
0045d01c 053094e7 ldr r3, [r4, r5]
0045d020 018094e7 ldr r8, [r4, r1]
0045d024 38d04de2 sub sp, sp, #0x38
0045d028 003093e5 ldr r3, [r3]
0045d02c 0290a0e1 mov sb, r2
0045d030 1c708de2 add r7, sp, #0x1c
0045d034 34308de5 str r3, [sp, #0x34]
0045d038 0ca090e5 ldr sl, [r0, #0xc]
0045d03c 0800a0e1 mov r0, r8
0045d040 106afbeb bl #0x337888
0045d044 8c109fe5 ldr r1, [pc, #0x8c]
0045d048 18208de2 add r2, sp, #0x18
0045d04c 0700a0e1 mov r0, r7
0045d050 01108fe0 add r1, pc, r1
0045d054 24dcfaeb bl #0x3140ec
0045d058 0710a0e1 mov r1, r7
0045d05c 0800a0e1 mov r0, r8
0045d060 886afbeb bl #0x337a88
0045d064 0700a0e1 mov r0, r7
0045d068 79ecfaeb bl #0x318254
0045d06c 68109fe5 ldr r1, [pc, #0x68]
0045d070 0c608de2 add r6, sp, #0xc
0045d074 0c209ae5 ldr r2, [sl, #0xc]
0045d078 011094e7 ldr r1, [r4, r1]
0045d07c 00c0a0e3 mov ip, #0
0045d080 0930a0e1 mov r3, sb
0045d084 381091e5 ldr r1, [r1, #0x38]
0045d088 0600a0e1 mov r0, r6
0045d08c 04c08de5 str ip, [sp, #4]
0045d090 00c08de5 str ip, [sp]
0045d094 01b7fbeb bl #0x34aca0
0045d098 0600a0e1 mov r0, r6
0045d09c d4f0ffeb bl #0x4593f4
0045d0a0 000050e3 cmp r0, #0
0045d0a4 0000000a beq #0x45d0ac
0045d0a8 d008fdeb bl #0x39f3f0
0045d0ac 053094e7 ldr r3, [r4, r5]
0045d0b0 34209de5 ldr r2, [sp, #0x34]
0045d0b4 003093e5 ldr r3, [r3]
0045d0b8 030052e1 cmp r2, r3
0045d0bc 0100001a bne #0x45d0c8
0045d0c0 38d08de2 add sp, sp, #0x38
0045d0c4 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045d0c8 90c4faeb bl #0x30e310
0045d0cc 787a5300 subseq r7, r3, r8, ror sl
0045d0d0 ac400000 andeq r4, r0, ip, lsr #1
0045d0d4 84080000 andeq r0, r0, r4, lsl #17
0045d0d8 30004700 subeq r0, r7, r0, lsr r0
0045d0dc f4370000 strdeq r3, r4, [r0], -r4
