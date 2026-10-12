0045d75c f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045d760 ec409fe5 ldr r4, [pc, #0xec]
0045d764 ec509fe5 ldr r5, [pc, #0xec]
0045d768 ecc09fe5 ldr ip, [pc, #0xec]
0045d76c 04408fe0 add r4, pc, r4
0045d770 053094e7 ldr r3, [r4, r5]
0045d774 0ca094e7 ldr sl, [r4, ip]
0045d778 3cd04de2 sub sp, sp, #0x3c
0045d77c 003093e5 ldr r3, [r3]
0045d780 0080a0e1 mov r8, r0
0045d784 0a00a0e1 mov r0, sl
0045d788 34308de5 str r3, [sp, #0x34]
0045d78c 0290a0e1 mov sb, r2
0045d790 01b0a0e1 mov fp, r1
0045d794 0c7098e5 ldr r7, [r8, #0xc]
0045d798 3a68fbeb bl #0x337888
0045d79c bc109fe5 ldr r1, [pc, #0xbc]
0045d7a0 1c608de2 add r6, sp, #0x1c
0045d7a4 18208de2 add r2, sp, #0x18
0045d7a8 01108fe0 add r1, pc, r1
0045d7ac 0600a0e1 mov r0, r6
0045d7b0 4ddafaeb bl #0x3140ec
0045d7b4 0610a0e1 mov r1, r6
0045d7b8 0a00a0e1 mov r0, sl
0045d7bc b168fbeb bl #0x337a88
0045d7c0 0600a0e1 mov r0, r6
0045d7c4 a2eafaeb bl #0x318254
0045d7c8 94309fe5 ldr r3, [pc, #0x94]
0045d7cc 0ca08de2 add sl, sp, #0xc
0045d7d0 0c2097e5 ldr r2, [r7, #0xc]
0045d7d4 031094e7 ldr r1, [r4, r3]
0045d7d8 0060a0e3 mov r6, #0
0045d7dc 0930a0e1 mov r3, sb
0045d7e0 381091e5 ldr r1, [r1, #0x38]
0045d7e4 0a00a0e1 mov r0, sl
0045d7e8 00608de5 str r6, [sp]
0045d7ec 04608de5 str r6, [sp, #4]
0045d7f0 2ab5fbeb bl #0x34aca0
0045d7f4 0a00a0e1 mov r0, sl
0045d7f8 0610a0e1 mov r1, r6
0045d7fc 6f89fbeb bl #0x33fdc0
0045d800 003050e2 subs r3, r0, #0
0045d804 0200000a beq #0x45d814
0045d808 f42093e5 ldr r2, [r3, #0xf4]
0045d80c 020052e3 cmp r2, #2
0045d810 0800000a beq #0x45d838
0045d814 0030a0e3 mov r3, #0
0045d818 103088e5 str r3, [r8, #0x10]
0045d81c 053094e7 ldr r3, [r4, r5]
0045d820 34209de5 ldr r2, [sp, #0x34]
0045d824 003093e5 ldr r3, [r3]
0045d828 030052e1 cmp r2, r3
0045d82c 0700001a bne #0x45d850
0045d830 3cd08de2 add sp, sp, #0x3c
0045d834 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045d838 103088e5 str r3, [r8, #0x10]
0045d83c 1030d7e5 ldrb r3, [r7, #0x10]
0045d840 0b10a0e1 mov r1, fp
0045d844 1430c8e5 strb r3, [r8, #0x14]
0045d848 0728feeb bl #0x3e786c
0045d84c f2ffffea b #0x45d81c
0045d850 aec2faeb bl #0x30e310
0045d854 24735300 subseq r7, r3, r4, lsr #6
0045d858 ac400000 andeq r4, r0, ip, lsr #1
0045d85c 84080000 andeq r0, r0, r4, lsl #17
