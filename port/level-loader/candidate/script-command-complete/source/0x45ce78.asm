0045ce78 f0412de9 push {r4, r5, r6, r7, r8, lr}
0045ce7c a8409fe5 ldr r4, [pc, #0xa8]
0045ce80 a8509fe5 ldr r5, [pc, #0xa8]
0045ce84 a8209fe5 ldr r2, [pc, #0xa8]
0045ce88 04408fe0 add r4, pc, r4
0045ce8c 053094e7 ldr r3, [r4, r5]
0045ce90 027094e7 ldr r7, [r4, r2]
0045ce94 20d04de2 sub sp, sp, #0x20
0045ce98 003093e5 ldr r3, [r3]
0045ce9c 04608de2 add r6, sp, #4
0045cea0 1c308de5 str r3, [sp, #0x1c]
0045cea4 0c8090e5 ldr r8, [r0, #0xc]
0045cea8 0700a0e1 mov r0, r7
0045ceac 756afbeb bl #0x337888
0045ceb0 80109fe5 ldr r1, [pc, #0x80]
0045ceb4 0d20a0e1 mov r2, sp
0045ceb8 0600a0e1 mov r0, r6
0045cebc 01108fe0 add r1, pc, r1
0045cec0 89dcfaeb bl #0x3140ec
0045cec4 0610a0e1 mov r1, r6
0045cec8 0700a0e1 mov r0, r7
0045cecc ed6afbeb bl #0x337a88
0045ced0 0600a0e1 mov r0, r6
0045ced4 deecfaeb bl #0x318254
0045ced8 5c309fe5 ldr r3, [pc, #0x5c]
0045cedc 0010a0e3 mov r1, #0
0045cee0 0120a0e3 mov r2, #1
0045cee4 033094e7 ldr r3, [r4, r3]
0045cee8 400093e5 ldr r0, [r3, #0x40]
0045ceec 6145fceb bl #0x36e478
0045cef0 600690e5 ldr r0, [r0, #0x660]
0045cef4 000050e3 cmp r0, #0
0045cef8 0300000a beq #0x45cf0c
0045cefc 0c2098e5 ldr r2, [r8, #0xc]
0045cf00 081098e5 ldr r1, [r8, #8]
0045cf04 0030e0e3 mvn r3, #0
0045cf08 a47bfdeb bl #0x3bbda0
0045cf0c 053094e7 ldr r3, [r4, r5]
0045cf10 1c209de5 ldr r2, [sp, #0x1c]
0045cf14 003093e5 ldr r3, [r3]
0045cf18 030052e1 cmp r2, r3
0045cf1c 0100001a bne #0x45cf28
0045cf20 20d08de2 add sp, sp, #0x20
0045cf24 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045cf28 f8c4faeb bl #0x30e310
0045cf2c 087c5300 subseq r7, r3, r8, lsl #24
0045cf30 ac400000 andeq r4, r0, ip, lsr #1
0045cf34 84080000 andeq r0, r0, r4, lsl #17
0045cf38 c4014700 subeq r0, r7, r4, asr #3
0045cf3c f4370000 strdeq r3, r4, [r0], -r4
