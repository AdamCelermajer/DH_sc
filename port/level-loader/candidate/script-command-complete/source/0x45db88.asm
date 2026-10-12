0045db88 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045db8c d8409fe5 ldr r4, [pc, #0xd8]
0045db90 d8609fe5 ldr r6, [pc, #0xd8]
0045db94 d8109fe5 ldr r1, [pc, #0xd8]
0045db98 04408fe0 add r4, pc, r4
0045db9c 063094e7 ldr r3, [r4, r6]
0045dba0 018094e7 ldr r8, [r4, r1]
0045dba4 38d04de2 sub sp, sp, #0x38
0045dba8 003093e5 ldr r3, [r3]
0045dbac 0290a0e1 mov sb, r2
0045dbb0 1c708de2 add r7, sp, #0x1c
0045dbb4 34308de5 str r3, [sp, #0x34]
0045dbb8 0ca090e5 ldr sl, [r0, #0xc]
0045dbbc 0800a0e1 mov r0, r8
0045dbc0 3067fbeb bl #0x337888
0045dbc4 ac109fe5 ldr r1, [pc, #0xac]
0045dbc8 18208de2 add r2, sp, #0x18
0045dbcc 0700a0e1 mov r0, r7
0045dbd0 01108fe0 add r1, pc, r1
0045dbd4 44d9faeb bl #0x3140ec
0045dbd8 0710a0e1 mov r1, r7
0045dbdc 0800a0e1 mov r0, r8
0045dbe0 a867fbeb bl #0x337a88
0045dbe4 0700a0e1 mov r0, r7
0045dbe8 99e9faeb bl #0x318254
0045dbec 88109fe5 ldr r1, [pc, #0x88]
0045dbf0 0c508de2 add r5, sp, #0xc
0045dbf4 0c209ae5 ldr r2, [sl, #0xc]
0045dbf8 011094e7 ldr r1, [r4, r1]
0045dbfc 0070a0e3 mov r7, #0
0045dc00 0930a0e1 mov r3, sb
0045dc04 381091e5 ldr r1, [r1, #0x38]
0045dc08 0500a0e1 mov r0, r5
0045dc0c 00708de5 str r7, [sp]
0045dc10 04708de5 str r7, [sp, #4]
0045dc14 21b4fbeb bl #0x34aca0
0045dc18 0500a0e1 mov r0, r5
0045dc1c 0710a0e1 mov r1, r7
0045dc20 6688fbeb bl #0x33fdc0
0045dc24 070050e1 cmp r0, r7
0045dc28 0600001a bne #0x45dc48
0045dc2c 063094e7 ldr r3, [r4, r6]
0045dc30 34209de5 ldr r2, [sp, #0x34]
0045dc34 003093e5 ldr r3, [r3]
0045dc38 030052e1 cmp r2, r3
0045dc3c 0900001a bne #0x45dc68
0045dc40 38d08de2 add sp, sp, #0x38
0045dc44 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045dc48 0500a0e1 mov r0, r5
0045dc4c c088fbeb bl #0x33ff54
0045dc50 003050e2 subs r3, r0, #0
0045dc54 f4ffff0a beq #0x45dc2c
0045dc58 003093e5 ldr r3, [r3]
0045dc5c 0fe0a0e1 mov lr, pc
0045dc60 38f193e5 ldr pc, [r3, #0x138]
0045dc64 f0ffffea b #0x45dc2c
0045dc68 a8c1faeb bl #0x30e310
0045dc6c f86e5300 ldrsheq r6, [r3], #-0xe8
0045dc70 ac400000 andeq r4, r0, ip, lsr #1
0045dc74 84080000 andeq r0, r0, r4, lsl #17
0045dc78 b0f44600 strheq pc, [r6], #-0x40
0045dc7c f4370000 strdeq r3, r4, [r0], -r4
