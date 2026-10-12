0045f8c8 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045f8cc a4419fe5 ldr r4, [pc, #0x1a4]
0045f8d0 a4719fe5 ldr r7, [pc, #0x1a4]
0045f8d4 a4219fe5 ldr r2, [pc, #0x1a4]
0045f8d8 04408fe0 add r4, pc, r4
0045f8dc 073094e7 ldr r3, [r4, r7]
0045f8e0 02a094e7 ldr sl, [r4, r2]
0045f8e4 54d04de2 sub sp, sp, #0x54
0045f8e8 003093e5 ldr r3, [r3]
0045f8ec 0060a0e1 mov r6, r0
0045f8f0 0a00a0e1 mov r0, sl
0045f8f4 0c8096e5 ldr r8, [r6, #0xc]
0045f8f8 4c308de5 str r3, [sp, #0x4c]
0045f8fc e15ffbeb bl #0x337888
0045f900 7c119fe5 ldr r1, [pc, #0x17c]
0045f904 34508de2 add r5, sp, #0x34
0045f908 30208de2 add r2, sp, #0x30
0045f90c 01108fe0 add r1, pc, r1
0045f910 0500a0e1 mov r0, r5
0045f914 f4d1faeb bl #0x3140ec
0045f918 0510a0e1 mov r1, r5
0045f91c 0a00a0e1 mov r0, sl
0045f920 5860fbeb bl #0x337a88
0045f924 0500a0e1 mov r0, r5
0045f928 49e2faeb bl #0x318254
0045f92c 105096e5 ldr r5, [r6, #0x10]
0045f930 000055e3 cmp r5, #0
0045f934 0a00000a beq #0x45f964
0045f938 0500a0e1 mov r0, r5
0045f93c 003095e5 ldr r3, [r5]
0045f940 0fe0a0e1 mov lr, pc
0045f944 0cf093e5 ldr pc, [r3, #0xc]
0045f948 073094e7 ldr r3, [r4, r7]
0045f94c 4c209de5 ldr r2, [sp, #0x4c]
0045f950 003093e5 ldr r3, [r3]
0045f954 030052e1 cmp r2, r3
0045f958 4500001a bne #0x45fa74
0045f95c 54d08de2 add sp, sp, #0x54
0045f960 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045f964 4834ffeb bl #0x42ca8c
0045f968 18119fe5 ldr r1, [pc, #0x118]
0045f96c 100098e5 ldr r0, [r8, #0x10]
0045f970 01108fe0 add r1, pc, r1
0045f974 96bcfaeb bl #0x30ebd4
0045f978 000050e3 cmp r0, #0
0045f97c 3800000a beq #0x45fa64
0045f980 04319fe5 ldr r3, [pc, #0x104]
0045f984 1c908de2 add sb, sp, #0x1c
0045f988 01a0a0e3 mov sl, #1
0045f98c 033094e7 ldr r3, [r4, r3]
0045f990 10b08de2 add fp, sp, #0x10
0045f994 540093e5 ldr r0, [r3, #0x54]
0045f998 7b34ffeb bl #0x42cb8c
0045f99c 0080a0e1 mov r8, r0
0045f9a0 c1200deb bl #0x7a7cac
0045f9a4 ea510ceb bl #0x774154
0045f9a8 e0209fe5 ldr r2, [pc, #0xe0]
0045f9ac 0010a0e1 mov r1, r0
0045f9b0 0530a0e1 mov r3, r5
0045f9b4 02208fe0 add r2, pc, r2
0045f9b8 0800a0e1 mov r0, r8
0045f9bc 00508de5 str r5, [sp]
0045f9c0 11310deb bl #0x7abe0c
0045f9c4 0800a0e1 mov r0, r8
0045f9c8 b7200deb bl #0x7a7cac
0045f9cc e0510ceb bl #0x774154
0045f9d0 0c008de5 str r0, [sp, #0xc]
0045f9d4 2c34ffeb bl #0x42ca8c
0045f9d8 0220a0e3 mov r2, #2
0045f9dc 080190e5 ldr r0, [r0, #0x108]
0045f9e0 1d20cde5 strb r2, [sp, #0x1d]
0045f9e4 1c50cde5 strb r5, [sp, #0x1c]
0045f9e8 d0bcfaeb bl #0x30ed30
0045f9ec f802cde1 strd r0, r1, [sp, #0x28]
0045f9f0 28c09de5 ldr ip, [sp, #0x28]
0045f9f4 0c309de5 ldr r3, [sp, #0xc]
0045f9f8 94209fe5 ldr r2, [pc, #0x94]
0045f9fc 20c08de5 str ip, [sp, #0x20]
0045fa00 2cc09de5 ldr ip, [sp, #0x2c]
0045fa04 0310a0e1 mov r1, r3
0045fa08 02208fe0 add r2, pc, r2
0045fa0c 0930a0e1 mov r3, sb
0045fa10 0800a0e1 mov r0, r8
0045fa14 08c089e5 str ip, [sb, #8]
0045fa18 00a08de5 str sl, [sp]
0045fa1c fa300deb bl #0x7abe0c
0045fa20 0900a0e1 mov r0, sb
0045fa24 bedd0ceb bl #0x797124
0045fa28 0800a0e1 mov r0, r8
0045fa2c 9e200deb bl #0x7a7cac
0045fa30 c7510ceb bl #0x774154
0045fa34 5c209fe5 ldr r2, [pc, #0x5c]
0045fa38 0010a0e1 mov r1, r0
0045fa3c 0b30a0e1 mov r3, fp
0045fa40 0800a0e1 mov r0, r8
0045fa44 02208fe0 add r2, pc, r2
0045fa48 1050cde5 strb r5, [sp, #0x10]
0045fa4c 00a08de5 str sl, [sp]
0045fa50 11a0cde5 strb sl, [sp, #0x11]
0045fa54 14a0cde5 strb sl, [sp, #0x14]
0045fa58 eb300deb bl #0x7abe0c
0045fa5c 0b00a0e1 mov r0, fp
0045fa60 afdd0ceb bl #0x797124
0045fa64 105096e5 ldr r5, [r6, #0x10]
0045fa68 000055e3 cmp r5, #0
0045fa6c b5ffff0a beq #0x45f948
0045fa70 b0ffffea b #0x45f938
0045fa74 25bafaeb bl #0x30e310
0045fa78 b8515300 ldrheq r5, [r3], #-0x18
0045fa7c ac400000 andeq r4, r0, ip, lsr #1
0045fa80 84080000 andeq r0, r0, r4, lsl #17
0045fa84 74d74600 subeq sp, r6, r4, ror r7
0045fa88 98d54600 umaaleq sp, r6, r8, r5
0045fa8c f4370000 strdeq r3, r4, [r0], -r4
0045fa90 7cc44600 subeq ip, r6, ip, ror r4
0045fa94 70a74600 subeq sl, r6, r0, ror r7
0045fa98 7cd64600 subeq sp, r6, ip, ror r6
