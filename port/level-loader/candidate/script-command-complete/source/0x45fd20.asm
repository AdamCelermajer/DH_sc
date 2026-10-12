0045fd20 f0412de9 push {r4, r5, r6, r7, r8, lr}
0045fd24 e0409fe5 ldr r4, [pc, #0xe0]
0045fd28 e0609fe5 ldr r6, [pc, #0xe0]
0045fd2c e0209fe5 ldr r2, [pc, #0xe0]
0045fd30 04408fe0 add r4, pc, r4
0045fd34 063094e7 ldr r3, [r4, r6]
0045fd38 027094e7 ldr r7, [r4, r2]
0045fd3c 28d04de2 sub sp, sp, #0x28
0045fd40 003093e5 ldr r3, [r3]
0045fd44 0c508de2 add r5, sp, #0xc
0045fd48 24308de5 str r3, [sp, #0x24]
0045fd4c 0c8090e5 ldr r8, [r0, #0xc]
0045fd50 0700a0e1 mov r0, r7
0045fd54 cb5efbeb bl #0x337888
0045fd58 b8109fe5 ldr r1, [pc, #0xb8]
0045fd5c 08208de2 add r2, sp, #8
0045fd60 0500a0e1 mov r0, r5
0045fd64 01108fe0 add r1, pc, r1
0045fd68 dfd0faeb bl #0x3140ec
0045fd6c 0510a0e1 mov r1, r5
0045fd70 0700a0e1 mov r0, r7
0045fd74 435ffbeb bl #0x337a88
0045fd78 0500a0e1 mov r0, r5
0045fd7c 34e1faeb bl #0x318254
0045fd80 94309fe5 ldr r3, [pc, #0x94]
0045fd84 030094e7 ldr r0, [r4, r3]
0045fd88 01fefaeb bl #0x31f594
0045fd8c 000050e3 cmp r0, #0
0045fd90 1000000a beq #0x45fdd8
0045fd94 84309fe5 ldr r3, [pc, #0x84]
0045fd98 1c1190e5 ldr r1, [r0, #0x11c]
0045fd9c 080098e5 ldr r0, [r8, #8]
0045fda0 033094e7 ldr r3, [r4, r3]
0045fda4 0120a0e3 mov r2, #1
0045fda8 005093e5 ldr r5, [r3]
0045fdac 0030a0e3 mov r3, #0
0045fdb0 00008de5 str r0, [sp]
0045fdb4 0500a0e1 mov r0, r5
0045fdb8 ee2ffceb bl #0x36bd78
0045fdbc 3130d5e5 ldrb r3, [r5, #0x31]
0045fdc0 000053e3 cmp r3, #0
0045fdc4 0a00001a bne #0x45fdf4
0045fdc8 54109fe5 ldr r1, [pc, #0x54]
0045fdcc 0500a0e1 mov r0, r5
0045fdd0 01108fe0 add r1, pc, r1
0045fdd4 ce25fceb bl #0x369514
0045fdd8 063094e7 ldr r3, [r4, r6]
0045fddc 24209de5 ldr r2, [sp, #0x24]
0045fde0 003093e5 ldr r3, [r3]
0045fde4 030052e1 cmp r2, r3
0045fde8 0600001a bne #0x45fe08
0045fdec 28d08de2 add sp, sp, #0x28
0045fdf0 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045fdf4 2c109fe5 ldr r1, [pc, #0x2c]
0045fdf8 0500a0e1 mov r0, r5
0045fdfc 01108fe0 add r1, pc, r1
0045fe00 c325fceb bl #0x369514
0045fe04 f3ffffea b #0x45fdd8
0045fe08 40b9faeb bl #0x30e310
0045fe0c 604d5300 subseq r4, r3, r0, ror #26
0045fe10 ac400000 andeq r4, r0, ip, lsr #1
0045fe14 84080000 andeq r0, r0, r4, lsl #17
0045fe18 1cd34600 subeq sp, r6, ip, lsl r3
0045fe1c f4370000 strdeq r3, r4, [r0], -r4
0045fe20 a40d0000 andeq r0, r0, r4, lsr #27
0045fe24 581e4600 subeq r1, r6, r8, asr lr
0045fe28 241e4600 subeq r1, r6, r4, lsr #28
