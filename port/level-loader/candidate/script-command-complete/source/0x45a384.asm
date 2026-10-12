0045a384 f0432de9 push {r4, r5, r6, r7, r8, sb, lr}
0045a388 bc429fe5 ldr r4, [pc, #0x2bc]
0045a38c bc529fe5 ldr r5, [pc, #0x2bc]
0045a390 34d04de2 sub sp, sp, #0x34
0045a394 04408fe0 add r4, pc, r4
0045a398 056094e7 ldr r6, [r4, r5]
0045a39c 0600a0e1 mov r0, r6
0045a3a0 7b14fbeb bl #0x31f594
0045a3a4 000050e3 cmp r0, #0
0045a3a8 1c00000a beq #0x45a420
0045a3ac 0600a0e1 mov r0, r6
0045a3b0 7714fbeb bl #0x31f594
0045a3b4 9831d0e5 ldrb r3, [r0, #0x198]
0045a3b8 000053e3 cmp r3, #0
0045a3bc 5c00001a bne #0x45a534
0045a3c0 b149ffeb bl #0x42ca8c
0045a3c4 f43090e5 ldr r3, [r0, #0xf4]
0045a3c8 0110a0e3 mov r1, #1
0045a3cc 0300a0e1 mov r0, r3
0045a3d0 003093e5 ldr r3, [r3]
0045a3d4 0fe0a0e1 mov lr, pc
0045a3d8 38f093e5 ldr pc, [r3, #0x38]
0045a3dc 056094e7 ldr r6, [r4, r5]
0045a3e0 0010a0e3 mov r1, #0
0045a3e4 0120a0e3 mov r2, #1
0045a3e8 400096e5 ldr r0, [r6, #0x40]
0045a3ec 2150fceb bl #0x36e478
0045a3f0 603690e5 ldr r3, [r0, #0x660]
0045a3f4 000053e3 cmp r3, #0
0045a3f8 0500000a beq #0x45a414
0045a3fc 400096e5 ldr r0, [r6, #0x40]
0045a400 0010a0e3 mov r1, #0
0045a404 0120a0e3 mov r2, #1
0045a408 1a50fceb bl #0x36e478
0045a40c 600690e5 ldr r0, [r0, #0x660]
0045a410 673efdeb bl #0x3a9db4
0045a414 050094e7 ldr r0, [r4, r5]
0045a418 0010a0e3 mov r1, #0
0045a41c 9114fbeb bl #0x31f668
0045a420 db8c0eeb bl #0x7fd794
0045a424 0530d0e5 ldrb r3, [r0, #5]
0045a428 000053e3 cmp r3, #0
0045a42c 4900001a bne #0x45a558
0045a430 9549ffeb bl #0x42ca8c
0045a434 d449ffeb bl #0x42cb8c
0045a438 0070a0e1 mov r7, r0
0045a43c 9249ffeb bl #0x42ca8c
0045a440 d149ffeb bl #0x42cb8c
0045a444 18360deb bl #0x7a7cac
0045a448 41670ceb bl #0x774154
0045a44c 00229fe5 ldr r2, [pc, #0x200]
0045a450 0060a0e3 mov r6, #0
0045a454 0010a0e1 mov r1, r0
0045a458 02208fe0 add r2, pc, r2
0045a45c 0630a0e1 mov r3, r6
0045a460 0700a0e1 mov r0, r7
0045a464 00608de5 str r6, [sp]
0045a468 67460deb bl #0x7abe0c
0045a46c e4319fe5 ldr r3, [pc, #0x1e4]
0045a470 0110a0e3 mov r1, #1
0045a474 032094e7 ldr r2, [r4, r3]
0045a478 dc319fe5 ldr r3, [pc, #0x1dc]
0045a47c 3010c2e5 strb r1, [r2, #0x30]
0045a480 033094e7 ldr r3, [r4, r3]
0045a484 0060c3e5 strb r6, [r3]
0045a488 c18c0eeb bl #0x7fd794
0045a48c 0530d0e5 ldrb r3, [r0, #5]
0045a490 060053e1 cmp r3, r6
0045a494 2400000a beq #0x45a52c
0045a498 053094e7 ldr r3, [r4, r5]
0045a49c 400093e5 ldr r0, [r3, #0x40]
0045a4a0 c43690e5 ldr r3, [r0, #0x6c4]
0045a4a4 060053e1 cmp r3, r6
0045a4a8 0d0000ca bgt #0x45a4e4
0045a4ac 1e0000ea b #0x45a52c
0045a4b0 603697e5 ldr r3, [r7, #0x660]
0045a4b4 000053e3 cmp r3, #0
0045a4b8 0300000a beq #0x45a4cc
0045a4bc 0300a0e1 mov r0, r3
0045a4c0 003093e5 ldr r3, [r3]
0045a4c4 0fe0a0e1 mov lr, pc
0045a4c8 40f093e5 ldr pc, [r3, #0x40]
0045a4cc 053094e7 ldr r3, [r4, r5]
0045a4d0 016086e2 add r6, r6, #1
0045a4d4 400093e5 ldr r0, [r3, #0x40]
0045a4d8 c43690e5 ldr r3, [r0, #0x6c4]
0045a4dc 030056e1 cmp r6, r3
0045a4e0 110000aa bge #0x45a52c
0045a4e4 0610a0e1 mov r1, r6
0045a4e8 0020a0e3 mov r2, #0
0045a4ec 9450fceb bl #0x36e744
0045a4f0 003090e5 ldr r3, [r0]
0045a4f4 0070a0e1 mov r7, r0
0045a4f8 0fe0a0e1 mov lr, pc
0045a4fc 50f093e5 ldr pc, [r3, #0x50]
0045a500 001050e2 subs r1, r0, #0
0045a504 e9ffff0a beq #0x45a4b0
0045a508 0700a0e1 mov r0, r7
0045a50c 0110a0e3 mov r1, #1
0045a510 fafcffeb bl #0x459900
0045a514 053094e7 ldr r3, [r4, r5]
0045a518 016086e2 add r6, r6, #1
0045a51c 400093e5 ldr r0, [r3, #0x40]
0045a520 c43690e5 ldr r3, [r0, #0x6c4]
0045a524 030056e1 cmp r6, r3
0045a528 edffffba blt #0x45a4e4
0045a52c 34d08de2 add sp, sp, #0x34
0045a530 f083bde8 pop {r4, r5, r6, r7, r8, sb, pc}
0045a534 24319fe5 ldr r3, [pc, #0x124]
0045a538 033094e7 ldr r3, [r4, r3]
0045a53c 0030d3e5 ldrb r3, [r3]
0045a540 000053e3 cmp r3, #0
0045a544 9dffff1a bne #0x45a3c0
0045a548 918c0eeb bl #0x7fd794
0045a54c 0530d0e5 ldrb r3, [r0, #5]
0045a550 000053e3 cmp r3, #0
0045a554 b5ffff0a beq #0x45a430
0045a558 056094e7 ldr r6, [r4, r5]
0045a55c 0010a0e3 mov r1, #0
0045a560 0120a0e3 mov r2, #1
0045a564 400096e5 ldr r0, [r6, #0x40]
0045a568 c24ffceb bl #0x36e478
0045a56c 603690e5 ldr r3, [r0, #0x660]
0045a570 000053e3 cmp r3, #0
0045a574 adffff0a beq #0x45a430
0045a578 0010a0e3 mov r1, #0
0045a57c 0120a0e3 mov r2, #1
0045a580 400096e5 ldr r0, [r6, #0x40]
0045a584 bb4ffceb bl #0x36e478
0045a588 603690e5 ldr r3, [r0, #0x660]
0045a58c 0300a0e1 mov r0, r3
0045a590 003093e5 ldr r3, [r3]
0045a594 0fe0a0e1 mov lr, pc
0045a598 34f093e5 ldr pc, [r3, #0x34]
0045a59c 000050e3 cmp r0, #0
0045a5a0 a2ffff0a beq #0x45a430
0045a5a4 0010a0e3 mov r1, #0
0045a5a8 0120a0e1 mov r2, r1
0045a5ac 400096e5 ldr r0, [r6, #0x40]
0045a5b0 b04ffceb bl #0x36e478
0045a5b4 a8209fe5 ldr r2, [pc, #0xa8]
0045a5b8 28109de5 ldr r1, [sp, #0x28]
0045a5bc 0030e0e3 mvn r3, #0
0045a5c0 022094e7 ldr r2, [r4, r2]
0045a5c4 030051e1 cmp r1, r3
0045a5c8 0060a0e1 mov r6, r0
0045a5cc 0010a0e3 mov r1, #0
0045a5d0 082082e2 add r2, r2, #8
0045a5d4 1000a0e3 mov r0, #0x10
0045a5d8 0080a0e3 mov r8, #0
0045a5dc 0090a0e3 mov sb, #0
0045a5e0 0c008de5 str r0, [sp, #0xc]
0045a5e4 f081cde1 strd r8, sb, [sp, #0x10]
0045a5e8 2410cde5 strb r1, [sp, #0x24]
0045a5ec 08208de5 str r2, [sp, #8]
0045a5f0 18308de5 str r3, [sp, #0x18]
0045a5f4 1c308de5 str r3, [sp, #0x1c]
0045a5f8 20108de5 str r1, [sp, #0x20]
0045a5fc 08708d02 addeq r7, sp, #8
0045a600 0300000a beq #0x45a614
0045a604 08708de2 add r7, sp, #8
0045a608 0700a0e1 mov r0, r7
0045a60c 28308de5 str r3, [sp, #0x28]
0045a610 5bea0eeb bl #0x814f84
0045a614 4c309fe5 ldr r3, [pc, #0x4c]
0045a618 e20f86e2 add r0, r6, #0x388
0045a61c 201087e2 add r1, r7, #0x20
0045a620 033094e7 ldr r3, [r4, r3]
0045a624 083083e2 add r3, r3, #8
0045a628 08308de5 str r3, [sp, #8]
0045a62c 883396e5 ldr r3, [r6, #0x388]
0045a630 0fe0a0e1 mov lr, pc
0045a634 1cf093e5 ldr pc, [r3, #0x1c]
0045a638 2c309fe5 ldr r3, [pc, #0x2c]
0045a63c 033094e7 ldr r3, [r4, r3]
0045a640 083083e2 add r3, r3, #8
0045a644 08308de5 str r3, [sp, #8]
0045a648 78ffffea b #0x45a430
0045a64c fca65300 ldrsheq sl, [r3], #-0x6c
0045a650 f4370000 strdeq r3, r4, [r0], -r4
0045a654 f82b4700 strdeq r2, r3, [r7], #-0xb8
0045a658 201a0000 andeq r1, r0, r0, lsr #20
0045a65c ec3d0000 andeq r3, r0, ip, ror #27
0045a660 a02f0000 andeq r2, r0, r0, lsr #31
0045a664 84290000 andeq r2, r0, r4, lsl #19
0045a668 3c350000 andeq r3, r0, ip, lsr r5
0045a66c a8100000 andeq r1, r0, r8, lsr #1
