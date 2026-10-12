00460e68 f0452de9 push {r4, r5, r6, r7, r8, sl, lr}
00460e6c 50419fe5 ldr r4, [pc, #0x150]
00460e70 50519fe5 ldr r5, [pc, #0x150]
00460e74 50219fe5 ldr r2, [pc, #0x150]
00460e78 04408fe0 add r4, pc, r4
00460e7c 053094e7 ldr r3, [r4, r5]
00460e80 026094e7 ldr r6, [r4, r2]
00460e84 ccd04de2 sub sp, sp, #0xcc
00460e88 003093e5 ldr r3, [r3]
00460e8c aca08de2 add sl, sp, #0xac
00460e90 94708de2 add r7, sp, #0x94
00460e94 c4308de5 str r3, [sp, #0xc4]
00460e98 0c8090e5 ldr r8, [r0, #0xc]
00460e9c 0600a0e1 mov r0, r6
00460ea0 785afbeb bl #0x337888
00460ea4 24119fe5 ldr r1, [pc, #0x124]
00460ea8 14208de2 add r2, sp, #0x14
00460eac 0a00a0e1 mov r0, sl
00460eb0 01108fe0 add r1, pc, r1
00460eb4 8cccfaeb bl #0x3140ec
00460eb8 0a10a0e1 mov r1, sl
00460ebc 0600a0e1 mov r0, r6
00460ec0 f05afbeb bl #0x337a88
00460ec4 0a00a0e1 mov r0, sl
00460ec8 e1dcfaeb bl #0x318254
00460ecc 0600a0e1 mov r0, r6
00460ed0 6c5afbeb bl #0x337888
00460ed4 f8109fe5 ldr r1, [pc, #0xf8]
00460ed8 10208de2 add r2, sp, #0x10
00460edc 0700a0e1 mov r0, r7
00460ee0 01108fe0 add r1, pc, r1
00460ee4 80ccfaeb bl #0x3140ec
00460ee8 0600a0e1 mov r0, r6
00460eec 0710a0e1 mov r1, r7
00460ef0 e45afbeb bl #0x337a88
00460ef4 0060a0e1 mov r6, r0
00460ef8 0700a0e1 mov r0, r7
00460efc d4dcfaeb bl #0x318254
00460f00 000056e3 cmp r6, #0
00460f04 2600000a beq #0x460fa4
00460f08 0c3098e5 ldr r3, [r8, #0xc]
00460f0c 000053e3 cmp r3, #0
00460f10 2300000a beq #0x460fa4
00460f14 bc309fe5 ldr r3, [pc, #0xbc]
00460f18 7c608de2 add r6, sp, #0x7c
00460f1c 64708de2 add r7, sp, #0x64
00460f20 03109fe7 ldr r1, [pc, r3]
00460f24 0c208de2 add r2, sp, #0xc
00460f28 0600a0e1 mov r0, r6
00460f2c 6eccfaeb bl #0x3140ec
00460f30 0c1098e5 ldr r1, [r8, #0xc]
00460f34 08208de2 add r2, sp, #8
00460f38 0700a0e1 mov r0, r7
00460f3c 6accfaeb bl #0x3140ec
00460f40 94309fe5 ldr r3, [pc, #0x94]
00460f44 94109fe5 ldr r1, [pc, #0x94]
00460f48 94209fe5 ldr r2, [pc, #0x94]
00460f4c 033094e7 ldr r3, [r4, r3]
00460f50 01108fe0 add r1, pc, r1
00460f54 02208fe0 add r2, pc, r2
00460f58 2c0093e5 ldr r0, [r3, #0x2c]
00460f5c 1e8f01eb bl #0x4c4bdc
00460f60 18808de2 add r8, sp, #0x18
00460f64 0030a0e1 mov r3, r0
00460f68 00c0e0e3 mvn ip, #0
00460f6c 0610a0e1 mov r1, r6
00460f70 0720a0e1 mov r2, r7
00460f74 0800a0e1 mov r0, r8
00460f78 00c08de5 str ip, [sp]
00460f7c c94bffeb bl #0x433ea8
00460f80 0700a0e1 mov r0, r7
00460f84 b2dcfaeb bl #0x318254
00460f88 0600a0e1 mov r0, r6
00460f8c b0dcfaeb bl #0x318254
00460f90 0800a0e1 mov r0, r8
00460f94 0110a0e3 mov r1, #1
00460f98 93ffffeb bl #0x460dec
00460f9c 0800a0e1 mov r0, r8
00460fa0 c2fbfaeb bl #0x31feb0
00460fa4 053094e7 ldr r3, [r4, r5]
00460fa8 c4209de5 ldr r2, [sp, #0xc4]
00460fac 003093e5 ldr r3, [r3]
00460fb0 030052e1 cmp r2, r3
00460fb4 0100001a bne #0x460fc0
00460fb8 ccd08de2 add sp, sp, #0xcc
00460fbc f085bde8 pop {r4, r5, r6, r7, r8, sl, pc}
00460fc0 d2b4faeb bl #0x30e310
00460fc4 183c5300 subseq r3, r3, r8, lsl ip
00460fc8 ac400000 andeq r4, r0, ip, lsr #1
00460fcc 84080000 andeq r0, r0, r4, lsl #17
00460fd0 d0c14600 ldrdeq ip, sp, [r6], #-0x10
00460fd4 08c24600 subeq ip, r6, r8, lsl #4
00460fd8 ac5b4f00 subeq r5, pc, ip, lsr #23
00460fdc f4370000 strdeq r3, r4, [r0], -r4
00460fe0 d8bc4600 ldrdeq fp, ip, [r6], #-0xc8
00460fe4 b4c14600 strheq ip, [r6], #-0x14
