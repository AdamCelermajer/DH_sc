0045f78c f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045f790 18419fe5 ldr r4, [pc, #0x118]
0045f794 18719fe5 ldr r7, [pc, #0x118]
0045f798 18219fe5 ldr r2, [pc, #0x118]
0045f79c 04408fe0 add r4, pc, r4
0045f7a0 073094e7 ldr r3, [r4, r7]
0045f7a4 02a094e7 ldr sl, [r4, r2]
0045f7a8 38d04de2 sub sp, sp, #0x38
0045f7ac 003093e5 ldr r3, [r3]
0045f7b0 0060a0e1 mov r6, r0
0045f7b4 0a00a0e1 mov r0, sl
0045f7b8 0c8096e5 ldr r8, [r6, #0xc]
0045f7bc 34308de5 str r3, [sp, #0x34]
0045f7c0 3060fbeb bl #0x337888
0045f7c4 f0109fe5 ldr r1, [pc, #0xf0]
0045f7c8 1c508de2 add r5, sp, #0x1c
0045f7cc 18208de2 add r2, sp, #0x18
0045f7d0 01108fe0 add r1, pc, r1
0045f7d4 0500a0e1 mov r0, r5
0045f7d8 43d2faeb bl #0x3140ec
0045f7dc 0510a0e1 mov r1, r5
0045f7e0 0a00a0e1 mov r0, sl
0045f7e4 a760fbeb bl #0x337a88
0045f7e8 0500a0e1 mov r0, r5
0045f7ec 98e2faeb bl #0x318254
0045f7f0 105096e5 ldr r5, [r6, #0x10]
0045f7f4 000055e3 cmp r5, #0
0045f7f8 0a00000a beq #0x45f828
0045f7fc 0500a0e1 mov r0, r5
0045f800 003095e5 ldr r3, [r5]
0045f804 0fe0a0e1 mov lr, pc
0045f808 10f093e5 ldr pc, [r3, #0x10]
0045f80c 073094e7 ldr r3, [r4, r7]
0045f810 34209de5 ldr r2, [sp, #0x34]
0045f814 003093e5 ldr r3, [r3]
0045f818 030052e1 cmp r2, r3
0045f81c 2200001a bne #0x45f8ac
0045f820 38d08de2 add sp, sp, #0x38
0045f824 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045f828 9734ffeb bl #0x42ca8c
0045f82c 8c109fe5 ldr r1, [pc, #0x8c]
0045f830 00a0a0e1 mov sl, r0
0045f834 100098e5 ldr r0, [r8, #0x10]
0045f838 01108fe0 add r1, pc, r1
0045f83c e4bcfaeb bl #0x30ebd4
0045f840 000050e3 cmp r0, #0
0045f844 1400000a beq #0x45f89c
0045f848 0a00a0e1 mov r0, sl
0045f84c ce34ffeb bl #0x42cb8c
0045f850 0090a0e1 mov sb, r0
0045f854 0a00a0e1 mov r0, sl
0045f858 cb34ffeb bl #0x42cb8c
0045f85c 12210deb bl #0x7a7cac
0045f860 3b520ceb bl #0x774154
0045f864 58209fe5 ldr r2, [pc, #0x58]
0045f868 0c808de2 add r8, sp, #0xc
0045f86c 01c0a0e3 mov ip, #1
0045f870 0010a0e1 mov r1, r0
0045f874 02208fe0 add r2, pc, r2
0045f878 0900a0e1 mov r0, sb
0045f87c 0830a0e1 mov r3, r8
0045f880 1050cde5 strb r5, [sp, #0x10]
0045f884 00c08de5 str ip, [sp]
0045f888 0c50cde5 strb r5, [sp, #0xc]
0045f88c 0dc0cde5 strb ip, [sp, #0xd]
0045f890 5d310deb bl #0x7abe0c
0045f894 0800a0e1 mov r0, r8
0045f898 21de0ceb bl #0x797124
0045f89c 105096e5 ldr r5, [r6, #0x10]
0045f8a0 000055e3 cmp r5, #0
0045f8a4 d8ffff0a beq #0x45f80c
0045f8a8 d3ffffea b #0x45f7fc
0045f8ac 97bafaeb bl #0x30e310
0045f8b0 f4525300 ldrsheq r5, [r3], #-0x24
0045f8b4 ac400000 andeq r4, r0, ip, lsr #1
0045f8b8 84080000 andeq r0, r0, r4, lsl #17
0045f8bc b0d84600 strheq sp, [r6], #-0x80
0045f8c0 d0d64600 ldrdeq sp, lr, [r6], #-0x60
0045f8c4 4cd84600 subeq sp, r6, ip, asr #16
