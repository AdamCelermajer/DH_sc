
ObjectiveGatherLootRegister 0047eaec
0047eaec push {r4, r5, r6, lr}
0047eaf0 mov r4, r0
0047eaf4 sub sp, sp, #8
0047eaf8 bl #0x47ae70
0047eafc ldrb r3, [r4, #8]
0047eb00 cmp r3, #0
0047eb04 beq #0x47eb54
0047eb08 ldr r6, [r4, #0x10]
0047eb0c ldr r2, [r4, #0xc]
0047eb10 mov r4, r6
0047eb14 ldr r3, [r4, #0x3ac]!
0047eb18 ldr r5, [r2, #0x24]
0047eb1c cmp r3, r4
0047eb20 beq #0x47eb40
0047eb24 ldr r2, [r3, #8]
0047eb28 cmp r5, r2
0047eb2c beq #0x47eb40
0047eb30 ldr r3, [r3]
0047eb34 cmp r4, r3
0047eb38 bne #0x47eb24
0047eb3c mov r3, r4
0047eb40 cmp r4, r3
0047eb44 beq #0x47eb5c
0047eb48 ldrb r2, [r3, #0xc]
0047eb4c add r2, r2, #1
0047eb50 strb r2, [r3, #0xc]
0047eb54 add sp, sp, #8
0047eb58 pop {r4, r5, r6, pc}
0047eb5c mov r3, #0x10
0047eb60 add r0, sp, #8
0047eb64 str r3, [r0, #-4]!
0047eb68 bl #0x708ec0
0047eb6c mov r3, #1
0047eb70 strb r3, [r0, #0xc]
0047eb74 str r5, [r0, #8]
0047eb78 ldr r3, [r6, #0x3b0]
0047eb7c str r4, [r0]
0047eb80 str r3, [r0, #4]
0047eb84 str r0, [r3]
0047eb88 str r0, [r6, #0x3b0]
0047eb8c b #0x47eb54

ObjectiveGatherLootUnregister 0047d6fc
0047d6fc push {r4, lr}
0047d700 mov r4, r0
0047d704 bl #0x47adbc
0047d708 ldrb r3, [r4, #8]
0047d70c cmp r3, #0
0047d710 bne #0x47d718
0047d714 pop {r4, pc}
0047d718 ldr r3, [r4, #0xc]
0047d71c ldr r0, [r4, #0x10]
0047d720 ldr r1, [r3, #0x24]
0047d724 add r0, r0, #0x37c
0047d728 pop {r4, lr}
0047d72c b #0x47d5fc

InventoryUnregisterGathering 0047d5fc
0047d5fc push {r4, r5, lr}
0047d600 mov r5, r0
0047d604 ldr r4, [r5, #0x30]!
0047d608 ldr r3, [pc, #0xd4]
0047d60c sub sp, sp, #0xc
0047d610 cmp r4, r5
0047d614 add r3, pc, r3
0047d618 beq #0x47d638
0047d61c ldr r2, [r4, #8]
0047d620 cmp r1, r2
0047d624 beq #0x47d638
0047d628 ldr r4, [r4]
0047d62c cmp r5, r4
0047d630 bne #0x47d61c
0047d634 mov r4, r5
0047d638 cmp r5, r4
0047d63c beq #0x47d67c
0047d640 ldrb r3, [r4, #0xc]
0047d644 sub r3, r3, #1
0047d648 uxtb r3, r3
0047d64c cmp r3, #0
0047d650 strb r3, [r4, #0xc]
0047d654 bne #0x47d6a8
0047d658 ldr r3, [r4]
0047d65c ldr r2, [r4, #4]
0047d660 mov r0, r4
0047d664 mov r1, #0x10
0047d668 str r3, [r2]
0047d66c str r2, [r3, #4]
0047d670 add sp, sp, #0xc
0047d674 pop {r4, r5, lr}
0047d678 b #0x708f00
0047d67c ldr r2, [pc, #0x64]
0047d680 ldr r2, [r3, r2]
0047d684 ldr r2, [r2]
0047d688 cmp r2, #2
0047d68c moveq r3, #0
0047d690 streq r3, [r3]
0047d694 beq #0x47d6a0
0047d698 cmp r2, #1
0047d69c beq #0x47d6b0
0047d6a0 cmp r5, r4
0047d6a4 bne #0x47d640
0047d6a8 add sp, sp, #0xc
0047d6ac pop {r4, r5, pc}
0047d6b0 ldr r0, [pc, #0x34]
0047d6b4 ldr r1, [pc, #0x34]
0047d6b8 ldr r2, [pc, #0x34]
0047d6bc ldr r0, [r3, r0]
0047d6c0 ldr r3, [pc, #0x30]
0047d6c4 movw ip, #0x14f
0047d6c8 add r1, pc, r1
0047d6cc add r2, pc, r2
0047d6d0 add r3, pc, r3
0047d6d4 add r0, r0, #0xa8
0047d6d8 str ip, [sp]
0047d6dc bl #0x30e004
0047d6e0 b #0x47d6a0
0047d6e4 subseq r7, r1, ip, ror r4
0047d6e8 andeq r3, r0, r0, asr #19
0047d6ec andeq r1, r0, r0, asr #19
0047d6f0 subeq r0, r4, r0, lsl sp
0047d6f4 ldrdeq r0, r1, [r5], #-0x7c
0047d6f8 subeq r0, r5, r8, lsl #16

InventoryC1 003ff200
003ff200 ldr r3, [pc, #0x120]
003ff204 ldr r2, [pc, #0x120]
003ff208 push {r4, r5, r6, r7, r8, sb, sl, lr}
003ff20c add r3, pc, r3
003ff210 ldr r2, [r3, r2]
003ff214 mov r6, r0
003ff218 mov r1, #0
003ff21c add r2, r2, #8
003ff220 str r2, [r6]
003ff224 mvn r2, #0x80000000
003ff228 sub sp, sp, #0x10
003ff22c add r0, r0, #0x30
003ff230 str r2, [r6, #0x28]
003ff234 mvn r2, #0
003ff238 mov r7, r1
003ff23c strb r2, [r6, #0x2c]
003ff240 str r0, [r6, #0x34]
003ff244 str r1, [r6, #4]
003ff248 str r1, [r6, #8]
003ff24c str r1, [r6, #0xc]
003ff250 str r1, [r6, #0x10]
003ff254 str r1, [r6, #0x14]
003ff258 str r1, [r6, #0x18]
003ff25c str r1, [r6, #0x1c]
003ff260 str r1, [r6, #0x20]
003ff264 str r1, [r6, #0x24]
003ff268 strb r1, [r6, #0x2d]
003ff26c strb r1, [r6, #0x2e]
003ff270 strb r1, [r6, #0x2f]
003ff274 str r0, [r6, #0x30]
003ff278 add sl, r6, #0x14
003ff27c mov r8, sp
003ff280 mov r5, r1
003ff284 add sb, sp, #0xc
003ff288 mov r0, sl
003ff28c mov r1, sp
003ff290 str r5, [sp]
003ff294 str r5, [sp, #4]
003ff298 str r5, [sp, #8]
003ff29c bl #0x3ff178
003ff2a0 ldr r0, [sp]
003ff2a4 cmp r0, #0
003ff2a8 beq #0x3ff2c4
003ff2ac ldr r1, [sp, #8]
003ff2b0 rsb r1, r0, r1
003ff2b4 bic r1, r1, #3
003ff2b8 cmp r1, #0x80
003ff2bc bhi #0x3ff320
003ff2c0 bl #0x708f00
003ff2c4 mov r4, #0
003ff2c8 ldr r0, [r6, #0x14]
003ff2cc str r5, [sp, #0xc]
003ff2d0 add r0, r0, r7
003ff2d4 ldmib r0, {r1, r3}
003ff2d8 cmp r1, r3
003ff2dc beq #0x3ff314
003ff2e0 str r5, [r1]
003ff2e4 ldr r3, [r0, #4]
003ff2e8 add r3, r3, #4
003ff2ec str r3, [r0, #4]
003ff2f0 add r4, r4, #1
003ff2f4 cmp r4, #9
003ff2f8 bne #0x3ff2c8
003ff2fc add r7, r7, #0xc
003ff300 cmp r7, #0x18
003ff304 bne #0x3ff288
003ff308 mov r0, r6
003ff30c add sp, sp, #0x10
003ff310 pop {r4, r5, r6, r7, r8, sb, sl, pc}
003ff314 mov r2, sb
003ff318 bl #0x3fef4c
003ff31c b #0x3ff2f0
003ff320 bl #0x310440
003ff324 b #0x3ff2c4
003ff328 subseq r5, sb, r4, lsl #17
003ff32c strdeq r2, r3, [r0], -ip

InventoryC2 003ff330
003ff330 ldr r3, [pc, #0x120]
003ff334 ldr r2, [pc, #0x120]
003ff338 push {r4, r5, r6, r7, r8, sb, sl, lr}
003ff33c add r3, pc, r3
003ff340 ldr r2, [r3, r2]
003ff344 mov r6, r0
003ff348 mov r1, #0
003ff34c add r2, r2, #8
003ff350 str r2, [r6]
003ff354 mvn r2, #0x80000000
003ff358 sub sp, sp, #0x10
003ff35c add r0, r0, #0x30
003ff360 str r2, [r6, #0x28]
003ff364 mvn r2, #0
003ff368 mov r7, r1
003ff36c strb r2, [r6, #0x2c]
003ff370 str r0, [r6, #0x34]
003ff374 str r1, [r6, #4]
003ff378 str r1, [r6, #8]
003ff37c str r1, [r6, #0xc]
003ff380 str r1, [r6, #0x10]
003ff384 str r1, [r6, #0x14]
003ff388 str r1, [r6, #0x18]
003ff38c str r1, [r6, #0x1c]
003ff390 str r1, [r6, #0x20]
003ff394 str r1, [r6, #0x24]
003ff398 strb r1, [r6, #0x2d]
003ff39c strb r1, [r6, #0x2e]
003ff3a0 strb r1, [r6, #0x2f]
003ff3a4 str r0, [r6, #0x30]
003ff3a8 add sl, r6, #0x14
003ff3ac mov r8, sp
003ff3b0 mov r5, r1
003ff3b4 add sb, sp, #0xc
003ff3b8 mov r0, sl
003ff3bc mov r1, sp
003ff3c0 str r5, [sp]
003ff3c4 str r5, [sp, #4]
003ff3c8 str r5, [sp, #8]
003ff3cc bl #0x3ff178
003ff3d0 ldr r0, [sp]
003ff3d4 cmp r0, #0
003ff3d8 beq #0x3ff3f4
003ff3dc ldr r1, [sp, #8]
003ff3e0 rsb r1, r0, r1
003ff3e4 bic r1, r1, #3
003ff3e8 cmp r1, #0x80
003ff3ec bhi #0x3ff450
003ff3f0 bl #0x708f00
003ff3f4 mov r4, #0
003ff3f8 ldr r0, [r6, #0x14]
003ff3fc str r5, [sp, #0xc]
003ff400 add r0, r0, r7
003ff404 ldmib r0, {r1, r3}
003ff408 cmp r1, r3
003ff40c beq #0x3ff444
003ff410 str r5, [r1]
003ff414 ldr r3, [r0, #4]
003ff418 add r3, r3, #4
003ff41c str r3, [r0, #4]
003ff420 add r4, r4, #1
003ff424 cmp r4, #9
003ff428 bne #0x3ff3f8
003ff42c add r7, r7, #0xc
003ff430 cmp r7, #0x18
003ff434 bne #0x3ff3b8
003ff438 mov r0, r6
003ff43c add sp, sp, #0x10
003ff440 pop {r4, r5, r6, r7, r8, sb, sl, pc}
003ff444 mov r2, sb
003ff448 bl #0x3fef4c
003ff44c b #0x3ff420
003ff450 bl #0x310440
003ff454 b #0x3ff3f4
003ff458 subseq r5, sb, r4, asr r7
003ff45c strdeq r2, r3, [r0], -ip

ContainerGetDataId 003a1844
003a1844 ldr r3, [r0, #0x388]
003a1848 ldr r0, [r0, #0x38c]
003a184c cmp r3, r0
003a1850 beq #0x3a1858
003a1854 b #0x3a1708
003a1858 mvn r0, #0
003a185c bx lr

ContainerGetVisual 003a1670
003a1670 ldr r0, [r0, #0x374]
003a1674 ldr r3, [pc, #0x24]
003a1678 cmn r0, #1
003a167c add r3, pc, r3
003a1680 bxeq lr
003a1684 ldr r2, [pc, #0x18]
003a1688 ldr r3, [r3, r2]
003a168c mov r2, #0x28
003a1690 ldr r3, [r3]
003a1694 mla r0, r2, r0, r3
003a1698 ldr r0, [r0, #0x24]
003a169c bx lr
003a16a0 subseq r3, pc, r4, lsl r4
003a16a4 ldrdeq r3, r4, [r0], -ip

ContainerGetScript 003a1634
003a1634 ldr r2, [r0, #0x374]
003a1638 ldr r3, [pc, #0x28]
003a163c cmn r2, #1
003a1640 add r3, pc, r3
003a1644 moveq r0, #0
003a1648 bxeq lr
003a164c ldr r1, [pc, #0x18]
003a1650 ldr r3, [r3, r1]
003a1654 mov r1, #0x28
003a1658 ldr r3, [r3]
003a165c mla r2, r1, r2, r3
003a1660 ldr r0, [r2, #0x18]
003a1664 bx lr
003a1668 subseq r3, pc, r0, asr r4
003a166c ldrdeq r3, r4, [r0], -ip
