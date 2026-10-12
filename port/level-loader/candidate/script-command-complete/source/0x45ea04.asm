0045ea04 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045ea08 f0409fe5 ldr r4, [pc, #0xf0]
0045ea0c f0609fe5 ldr r6, [pc, #0xf0]
0045ea10 f0109fe5 ldr r1, [pc, #0xf0]
0045ea14 04408fe0 add r4, pc, r4
0045ea18 063094e7 ldr r3, [r4, r6]
0045ea1c 018094e7 ldr r8, [r4, r1]
0045ea20 38d04de2 sub sp, sp, #0x38
0045ea24 003093e5 ldr r3, [r3]
0045ea28 0290a0e1 mov sb, r2
0045ea2c 1c708de2 add r7, sp, #0x1c
0045ea30 34308de5 str r3, [sp, #0x34]
0045ea34 0ca090e5 ldr sl, [r0, #0xc]
0045ea38 0800a0e1 mov r0, r8
0045ea3c 9163fbeb bl #0x337888
0045ea40 c4109fe5 ldr r1, [pc, #0xc4]
0045ea44 18208de2 add r2, sp, #0x18
0045ea48 0700a0e1 mov r0, r7
0045ea4c 01108fe0 add r1, pc, r1
0045ea50 a5d5faeb bl #0x3140ec
0045ea54 0710a0e1 mov r1, r7
0045ea58 0800a0e1 mov r0, r8
0045ea5c 0964fbeb bl #0x337a88
0045ea60 0700a0e1 mov r0, r7
0045ea64 fae5faeb bl #0x318254
0045ea68 a0109fe5 ldr r1, [pc, #0xa0]
0045ea6c 0c508de2 add r5, sp, #0xc
0045ea70 0c209ae5 ldr r2, [sl, #0xc]
0045ea74 011094e7 ldr r1, [r4, r1]
0045ea78 0070a0e3 mov r7, #0
0045ea7c 0930a0e1 mov r3, sb
0045ea80 381091e5 ldr r1, [r1, #0x38]
0045ea84 0500a0e1 mov r0, r5
0045ea88 00708de5 str r7, [sp]
0045ea8c 04708de5 str r7, [sp, #4]
0045ea90 82b0fbeb bl #0x34aca0
0045ea94 0500a0e1 mov r0, r5
0045ea98 0710a0e1 mov r1, r7
0045ea9c c784fbeb bl #0x33fdc0
0045eaa0 070050e1 cmp r0, r7
0045eaa4 0600001a bne #0x45eac4
0045eaa8 063094e7 ldr r3, [r4, r6]
0045eaac 34209de5 ldr r2, [sp, #0x34]
0045eab0 003093e5 ldr r3, [r3]
0045eab4 030052e1 cmp r2, r3
0045eab8 0f00001a bne #0x45eafc
0045eabc 38d08de2 add sp, sp, #0x38
0045eac0 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045eac4 0500a0e1 mov r0, r5
0045eac8 2185fbeb bl #0x33ff54
0045eacc 005050e2 subs r5, r0, #0
0045ead0 f4ffff0a beq #0x45eaa8
0045ead4 783395e5 ldr r3, [r5, #0x378]
0045ead8 0120a0e3 mov r2, #1
0045eadc 0710a0e1 mov r1, r7
0045eae0 0920c3e5 strb r2, [r3, #9]
0045eae4 780395e5 ldr r0, [r5, #0x378]
0045eae8 0720a0e1 mov r2, r7
0045eaec 069bfeeb bl #0x40570c
0045eaf0 783395e5 ldr r3, [r5, #0x378]
0045eaf4 0970c3e5 strb r7, [r3, #9]
0045eaf8 eaffffea b #0x45eaa8
0045eafc 03befaeb bl #0x30e310
0045eb00 7c605300 subseq r6, r3, ip, ror r0
0045eb04 ac400000 andeq r4, r0, ip, lsr #1
0045eb08 84080000 andeq r0, r0, r4, lsl #17
0045eb0c 34e64600 subeq lr, r6, r4, lsr r6
0045eb10 f4370000 strdeq r3, r4, [r0], -r4
