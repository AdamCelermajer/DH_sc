0045c8c0 f0412de9 push {r4, r5, r6, r7, r8, lr}
0045c8c4 b4409fe5 ldr r4, [pc, #0xb4]
0045c8c8 b4609fe5 ldr r6, [pc, #0xb4]
0045c8cc b4209fe5 ldr r2, [pc, #0xb4]
0045c8d0 04408fe0 add r4, pc, r4
0045c8d4 063094e7 ldr r3, [r4, r6]
0045c8d8 028094e7 ldr r8, [r4, r2]
0045c8dc 20d04de2 sub sp, sp, #0x20
0045c8e0 003093e5 ldr r3, [r3]
0045c8e4 0800a0e1 mov r0, r8
0045c8e8 04508de2 add r5, sp, #4
0045c8ec 1c308de5 str r3, [sp, #0x1c]
0045c8f0 e46bfbeb bl #0x337888
0045c8f4 90109fe5 ldr r1, [pc, #0x90]
0045c8f8 0d20a0e1 mov r2, sp
0045c8fc 8c709fe5 ldr r7, [pc, #0x8c]
0045c900 01108fe0 add r1, pc, r1
0045c904 0500a0e1 mov r0, r5
0045c908 f7ddfaeb bl #0x3140ec
0045c90c 0510a0e1 mov r1, r5
0045c910 0800a0e1 mov r0, r8
0045c914 5b6cfbeb bl #0x337a88
0045c918 0500a0e1 mov r0, r5
0045c91c 4ceefaeb bl #0x318254
0045c920 073094e7 ldr r3, [r4, r7]
0045c924 0010a0e3 mov r1, #0
0045c928 0120a0e3 mov r2, #1
0045c92c 400093e5 ldr r0, [r3, #0x40]
0045c930 d046fceb bl #0x36e478
0045c934 600690e5 ldr r0, [r0, #0x660]
0045c938 000050e3 cmp r0, #0
0045c93c 0100000a beq #0x45c948
0045c940 0110a0e3 mov r1, #1
0045c944 897bfdeb bl #0x3bb770
0045c948 070094e7 ldr r0, [r4, r7]
0045c94c 100bfbeb bl #0x31f594
0045c950 000050e3 cmp r0, #0
0045c954 0100000a beq #0x45c960
0045c958 0110a0e3 mov r1, #1
0045c95c 354afeeb bl #0x3ef238
0045c960 063094e7 ldr r3, [r4, r6]
0045c964 1c209de5 ldr r2, [sp, #0x1c]
0045c968 003093e5 ldr r3, [r3]
0045c96c 030052e1 cmp r2, r3
0045c970 0100001a bne #0x45c97c
0045c974 20d08de2 add sp, sp, #0x20
0045c978 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045c97c 63c6faeb bl #0x30e310
0045c980 c0815300 subseq r8, r3, r0, asr #3
0045c984 ac400000 andeq r4, r0, ip, lsr #1
0045c988 84080000 andeq r0, r0, r4, lsl #17
0045c98c 80074700 subeq r0, r7, r0, lsl #15
0045c990 f4370000 strdeq r3, r4, [r0], -r4
