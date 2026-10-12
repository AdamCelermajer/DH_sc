0045dc80 f0452de9 push {r4, r5, r6, r7, r8, sl, lr}
0045dc84 f8409fe5 ldr r4, [pc, #0xf8]
0045dc88 f8509fe5 ldr r5, [pc, #0xf8]
0045dc8c f8109fe5 ldr r1, [pc, #0xf8]
0045dc90 04408fe0 add r4, pc, r4
0045dc94 053094e7 ldr r3, [r4, r5]
0045dc98 017094e7 ldr r7, [r4, r1]
0045dc9c 3cd04de2 sub sp, sp, #0x3c
0045dca0 003093e5 ldr r3, [r3]
0045dca4 0280a0e1 mov r8, r2
0045dca8 1c608de2 add r6, sp, #0x1c
0045dcac 34308de5 str r3, [sp, #0x34]
0045dcb0 0ca090e5 ldr sl, [r0, #0xc]
0045dcb4 0700a0e1 mov r0, r7
0045dcb8 f266fbeb bl #0x337888
0045dcbc cc109fe5 ldr r1, [pc, #0xcc]
0045dcc0 18208de2 add r2, sp, #0x18
0045dcc4 0600a0e1 mov r0, r6
0045dcc8 01108fe0 add r1, pc, r1
0045dccc 06d9faeb bl #0x3140ec
0045dcd0 0610a0e1 mov r1, r6
0045dcd4 0700a0e1 mov r0, r7
0045dcd8 6a67fbeb bl #0x337a88
0045dcdc 0600a0e1 mov r0, r6
0045dce0 5be9faeb bl #0x318254
0045dce4 0c609ae5 ldr r6, [sl, #0xc]
0045dce8 a4109fe5 ldr r1, [pc, #0xa4]
0045dcec 0600a0e1 mov r0, r6
0045dcf0 01108fe0 add r1, pc, r1
0045dcf4 88c1faeb bl #0x30e31c
0045dcf8 000050e3 cmp r0, #0
0045dcfc 0900001a bne #0x45dd28
0045dd00 90309fe5 ldr r3, [pc, #0x90]
0045dd04 033094e7 ldr r3, [r4, r3]
0045dd08 0000c3e5 strb r0, [r3]
0045dd0c 053094e7 ldr r3, [r4, r5]
0045dd10 34209de5 ldr r2, [sp, #0x34]
0045dd14 003093e5 ldr r3, [r3]
0045dd18 030052e1 cmp r2, r3
0045dd1c 1700001a bne #0x45dd80
0045dd20 3cd08de2 add sp, sp, #0x3c
0045dd24 f085bde8 pop {r4, r5, r6, r7, r8, sl, pc}
0045dd28 6c309fe5 ldr r3, [pc, #0x6c]
0045dd2c 0c708de2 add r7, sp, #0xc
0045dd30 0620a0e1 mov r2, r6
0045dd34 031094e7 ldr r1, [r4, r3]
0045dd38 0060a0e3 mov r6, #0
0045dd3c 0830a0e1 mov r3, r8
0045dd40 381091e5 ldr r1, [r1, #0x38]
0045dd44 0700a0e1 mov r0, r7
0045dd48 00608de5 str r6, [sp]
0045dd4c 04608de5 str r6, [sp, #4]
0045dd50 d2b3fbeb bl #0x34aca0
0045dd54 0700a0e1 mov r0, r7
0045dd58 0610a0e1 mov r1, r6
0045dd5c 1788fbeb bl #0x33fdc0
0045dd60 060050e1 cmp r0, r6
0045dd64 e8ffff0a beq #0x45dd0c
0045dd68 0700a0e1 mov r0, r7
0045dd6c 7888fbeb bl #0x33ff54
0045dd70 000050e3 cmp r0, #0
0045dd74 78339015 ldrne r3, [r0, #0x378]
0045dd78 0860c315 strbne r6, [r3, #8]
0045dd7c e2ffffea b #0x45dd0c
0045dd80 62c1faeb bl #0x30e310
0045dd84 006e5300 subseq r6, r3, r0, lsl #28
0045dd88 ac400000 andeq r4, r0, ip, lsr #1
0045dd8c 84080000 andeq r0, r0, r4, lsl #17
0045dd90 b8f34600 strheq pc, [r6], #-0x38
0045dd94 58b24600 subeq fp, r6, r8, asr r2
0045dd98 50360000 andeq r3, r0, r0, asr r6
0045dd9c f4370000 strdeq r3, r4, [r0], -r4
