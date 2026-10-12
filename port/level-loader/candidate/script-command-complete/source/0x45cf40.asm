0045cf40 f0412de9 push {r4, r5, r6, r7, r8, lr}
0045cf44 a8409fe5 ldr r4, [pc, #0xa8]
0045cf48 a8509fe5 ldr r5, [pc, #0xa8]
0045cf4c a8209fe5 ldr r2, [pc, #0xa8]
0045cf50 04408fe0 add r4, pc, r4
0045cf54 053094e7 ldr r3, [r4, r5]
0045cf58 027094e7 ldr r7, [r4, r2]
0045cf5c 20d04de2 sub sp, sp, #0x20
0045cf60 003093e5 ldr r3, [r3]
0045cf64 04608de2 add r6, sp, #4
0045cf68 1c308de5 str r3, [sp, #0x1c]
0045cf6c 0c8090e5 ldr r8, [r0, #0xc]
0045cf70 0700a0e1 mov r0, r7
0045cf74 436afbeb bl #0x337888
0045cf78 80109fe5 ldr r1, [pc, #0x80]
0045cf7c 0d20a0e1 mov r2, sp
0045cf80 0600a0e1 mov r0, r6
0045cf84 01108fe0 add r1, pc, r1
0045cf88 57dcfaeb bl #0x3140ec
0045cf8c 0610a0e1 mov r1, r6
0045cf90 0700a0e1 mov r0, r7
0045cf94 bb6afbeb bl #0x337a88
0045cf98 0600a0e1 mov r0, r6
0045cf9c acecfaeb bl #0x318254
0045cfa0 5c309fe5 ldr r3, [pc, #0x5c]
0045cfa4 0010a0e3 mov r1, #0
0045cfa8 0120a0e3 mov r2, #1
0045cfac 033094e7 ldr r3, [r4, r3]
0045cfb0 400093e5 ldr r0, [r3, #0x40]
0045cfb4 2f45fceb bl #0x36e478
0045cfb8 600690e5 ldr r0, [r0, #0x660]
0045cfbc 000050e3 cmp r0, #0
0045cfc0 0300000a beq #0x45cfd4
0045cfc4 0c2098e5 ldr r2, [r8, #0xc]
0045cfc8 081098e5 ldr r1, [r8, #8]
0045cfcc 0030e0e3 mvn r3, #0
0045cfd0 337bfdeb bl #0x3bbca4
0045cfd4 053094e7 ldr r3, [r4, r5]
0045cfd8 1c209de5 ldr r2, [sp, #0x1c]
0045cfdc 003093e5 ldr r3, [r3]
0045cfe0 030052e1 cmp r2, r3
0045cfe4 0100001a bne #0x45cff0
0045cfe8 20d08de2 add sp, sp, #0x20
0045cfec f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045cff0 c6c4faeb bl #0x30e310
0045cff4 407b5300 subseq r7, r3, r0, asr #22
0045cff8 ac400000 andeq r4, r0, ip, lsr #1
0045cffc 84080000 andeq r0, r0, r4, lsl #17
0045d000 fc004700 strdeq r0, r1, [r7], #-0xc
0045d004 f4370000 strdeq r3, r4, [r0], -r4
