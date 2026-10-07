00866380 push {r4, r5, r6, lr}
00866384 ldr r6, [pc, #0x234]
00866388 ldr r1, [pc, #0x234]
0086638c mov r5, #0
00866390 add r6, pc, r6
00866394 ldr r1, [r6, r1]
00866398 mov r4, r0
0086639c strd r2, r3, [r0, #8]
008663a0 add r1, r1, #8
008663a4 str r1, [r0]
008663a8 str r5, [r0, #0x10]
008663ac add r0, r0, #0x18
008663b0 bl #0x8935d0
008663b4 ldr r3, [pc, #0x20c]
008663b8 mvn lr, #0x80000000
008663bc mov ip, #0x43000000
008663c0 ldr r3, [r6, r3]
008663c4 mov r2, #0
008663c8 mov r1, #0x3f800000
008663cc add r6, r3, #8
008663d0 str r6, [r4]
008663d4 ldr r6, [sp, #0x10]
008663d8 mov r0, #1
008663dc sub lr, lr, #0x800000
008663e0 str r6, [r4, #0x2c]
008663e4 ldr r6, [sp, #0x14]
008663e8 add ip, ip, #0xb40000
008663ec strb r0, [r4, #0x1c]
008663f0 str r6, [r4, #0x30]
008663f4 mov r6, #0x42000000
008663f8 add r6, r6, #0xc80000
008663fc str r0, [r4, #0x20]
00866400 str r2, [r4, #0x48]
00866404 str r2, [r4, #0x50]
00866408 str r2, [r4, #0x54]
0086640c strb r0, [r4, #0x58]
00866410 str r2, [r4, #0x5c]
00866414 str r2, [r4, #0x64]
00866418 str r2, [r4, #0x68]
0086641c strb r0, [r4, #0x6c]
00866420 str r2, [r4, #0x78]
00866424 str r2, [r4, #0x80]
00866428 str r2, [r4, #0x84]
0086642c strb r0, [r4, #0x88]
00866430 str r5, [r4, #0x28]
00866434 strb r5, [r4, #0x34]
00866438 str r1, [r4, #0x38]
0086643c str r1, [r4, #0x3c]
00866440 str r1, [r4, #0x40]
00866444 str r1, [r4, #0x44]
00866448 str r1, [r4, #0x4c]
0086644c str r1, [r4, #0x60]
00866450 str r1, [r4, #0x70]
00866454 str r1, [r4, #0x74]
00866458 str r1, [r4, #0x7c]
0086645c strb r5, [r4, #0x8c]
00866460 strb r5, [r4, #0x8d]
00866464 str r5, [r4, #0x90]
00866468 str r5, [r4, #0x94]
0086646c strb r5, [r4, #0x98]
00866470 str r6, [r4, #0xc8]
00866474 str ip, [r4, #0xd4]
00866478 str lr, [r4, #0xdc]
0086647c ldr r6, [sp, #0x18]
00866480 mov r3, r5
00866484 str r6, [r4, #0x110]
00866488 ldr r6, [sp, #0x1c]
0086648c str r2, [r4, #0x9c]
00866490 str r2, [r4, #0xa0]
00866494 str r2, [r4, #0xa4]
00866498 str r2, [r4, #0xa8]
0086649c str r2, [r4, #0xac]
008664a0 str r2, [r4, #0xb0]
008664a4 str r2, [r4, #0xb4]
008664a8 str r2, [r4, #0xb8]
008664ac str r2, [r4, #0xbc]
008664b0 str r2, [r4, #0xd8]
008664b4 str r6, [r4, #0x118]
008664b8 str r1, [r4, #0x124]
008664bc strb r5, [r4, #0x99]
008664c0 str r5, [r4, #0xc0]
008664c4 str lr, [r4, #0xc4]
008664c8 str r1, [r4, #0xcc]
008664cc str ip, [r4, #0xd0]
008664d0 str r5, [r4, #0xec]
008664d4 str r5, [r4, #0xf0]
008664d8 str r5, [r4, #0xf4]
008664dc str r5, [r4, #0xf8]
008664e0 str r5, [r4, #0xfc]
008664e4 str r5, [r4, #0x108]
008664e8 str r5, [r4, #0x10c]
008664ec str r5, [r4, #0x114]
008664f0 strb r5, [r4, #0x11c]
008664f4 str r2, [r4, #0x120]
008664f8 str r2, [r4, #0x128]
008664fc str r2, [r4, #0x12c]
00866500 mvn r2, #0
00866504 strb r0, [r4, #0x130]
00866508 str r2, [r4, #0x134]
0086650c strb r5, [r4, #0x11d]
00866510 str r5, [r4, #0x138]
00866514 str r5, [r4, #0x13c]
00866518 mov r2, r4
0086651c mov r0, r5
00866520 add r3, r3, #1
00866524 cmp r3, #0xb
00866528 strb r0, [r2, #0xe0]
0086652c mov r1, #0
00866530 add r2, r2, #1
00866534 bne #0x866520
00866538 ldr r3, [r4, #0x118]
0086653c str r1, [r4, #0x104]
00866540 str r1, [r4, #0x100]
00866544 cmp r3, r1
00866548 beq #0x866574
0086654c ldr r2, [r3, #0x30]
00866550 ldr r1, [r3, #0x28]
00866554 ldr r0, [r3, #0x34]
00866558 asr r2, r2, #3
0086655c mul r2, r1, r2
00866560 ldr r1, [r3, #0x2c]
00866564 mul r3, r0, r2
00866568 mul r2, r1, r2
0086656c str r3, [r4, #0x24]
00866570 str r2, [r4, #0x20]
00866574 mov r0, r4
00866578 bl #0x8652b8
0086657c ldr r3, [r4, #0x118]
00866580 ldr r2, [r3, #0x50]
00866584 cmp r2, #0
00866588 ldreq r3, [r3, #0x3c]
0086658c movne r3, #0
00866590 mov r0, r3
00866594 ldr r3, [r3]
00866598 mov lr, pc
0086659c ldr pc, [r3, #0x18]
008665a0 mov r3, #0
008665a4 mov r2, #1
008665a8 str r0, [r4, #0x140]
008665ac strb r2, [r4, #0x145]
008665b0 strb r3, [r4, #0x146]
008665b4 strb r3, [r4, #0x144]
008665b8 mov r0, r4
008665bc pop {r4, r5, r6, pc}
008665c0 andseq lr, r2, r0, lsl #14
008665c4 andeq r4, r0, r4, lsr r7
008665c8 strdeq r2, r3, [r0], -ip
