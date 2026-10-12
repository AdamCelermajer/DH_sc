0045d95c f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045d960 10419fe5 ldr r4, [pc, #0x110]
0045d964 10619fe5 ldr r6, [pc, #0x110]
0045d968 10119fe5 ldr r1, [pc, #0x110]
0045d96c 04408fe0 add r4, pc, r4
0045d970 063094e7 ldr r3, [r4, r6]
0045d974 018094e7 ldr r8, [r4, r1]
0045d978 38d04de2 sub sp, sp, #0x38
0045d97c 003093e5 ldr r3, [r3]
0045d980 0290a0e1 mov sb, r2
0045d984 1c708de2 add r7, sp, #0x1c
0045d988 34308de5 str r3, [sp, #0x34]
0045d98c 0ca090e5 ldr sl, [r0, #0xc]
0045d990 0800a0e1 mov r0, r8
0045d994 bb67fbeb bl #0x337888
0045d998 e4109fe5 ldr r1, [pc, #0xe4]
0045d99c 18208de2 add r2, sp, #0x18
0045d9a0 0700a0e1 mov r0, r7
0045d9a4 01108fe0 add r1, pc, r1
0045d9a8 cfd9faeb bl #0x3140ec
0045d9ac 0710a0e1 mov r1, r7
0045d9b0 0800a0e1 mov r0, r8
0045d9b4 3368fbeb bl #0x337a88
0045d9b8 0700a0e1 mov r0, r7
0045d9bc 24eafaeb bl #0x318254
0045d9c0 c0109fe5 ldr r1, [pc, #0xc0]
0045d9c4 0c508de2 add r5, sp, #0xc
0045d9c8 0c209ae5 ldr r2, [sl, #0xc]
0045d9cc 011094e7 ldr r1, [r4, r1]
0045d9d0 0070a0e3 mov r7, #0
0045d9d4 0930a0e1 mov r3, sb
0045d9d8 381091e5 ldr r1, [r1, #0x38]
0045d9dc 0500a0e1 mov r0, r5
0045d9e0 00708de5 str r7, [sp]
0045d9e4 04708de5 str r7, [sp, #4]
0045d9e8 acb4fbeb bl #0x34aca0
0045d9ec 0500a0e1 mov r0, r5
0045d9f0 0710a0e1 mov r1, r7
0045d9f4 f188fbeb bl #0x33fdc0
0045d9f8 070050e1 cmp r0, r7
0045d9fc 0600001a bne #0x45da1c
0045da00 063094e7 ldr r3, [r4, r6]
0045da04 34209de5 ldr r2, [sp, #0x34]
0045da08 003093e5 ldr r3, [r3]
0045da0c 030052e1 cmp r2, r3
0045da10 1700001a bne #0x45da74
0045da14 38d08de2 add sp, sp, #0x38
0045da18 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045da1c 0500a0e1 mov r0, r5
0045da20 4b89fbeb bl #0x33ff54
0045da24 005050e2 subs r5, r0, #0
0045da28 f4ffff0a beq #0x45da00
0045da2c f43401e3 movw r3, #0x14f4
0045da30 032095e7 ldr r2, [r5, r3]
0045da34 010072e3 cmn r2, #1
0045da38 0300000a beq #0x45da4c
0045da3c 003095e5 ldr r3, [r5]
0045da40 0110a0e3 mov r1, #1
0045da44 0fe0a0e1 mov lr, pc
0045da48 3cf193e5 ldr pc, [r3, #0x13c]
0045da4c f83401e3 movw r3, #0x14f8
0045da50 032095e7 ldr r2, [r5, r3]
0045da54 010072e3 cmn r2, #1
0045da58 e8ffff0a beq #0x45da00
0045da5c 0500a0e1 mov r0, r5
0045da60 003095e5 ldr r3, [r5]
0045da64 0210a0e3 mov r1, #2
0045da68 0fe0a0e1 mov lr, pc
0045da6c 3cf193e5 ldr pc, [r3, #0x13c]
0045da70 e2ffffea b #0x45da00
0045da74 25c2faeb bl #0x30e310
0045da78 24715300 subseq r7, r3, r4, lsr #2
0045da7c ac400000 andeq r4, r0, ip, lsr #1
0045da80 84080000 andeq r0, r0, r4, lsl #17
