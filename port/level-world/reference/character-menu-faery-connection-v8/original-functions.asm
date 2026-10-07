# _ZN10GameObject15SetVisualObjectEPKcS1_b 394d34 380
00394d34 cmp r3, #0
00394d38 push {r4, r5, r6, r7, r8, lr}
00394d3c mov r4, r0
00394d40 mov r5, r1
00394d44 mov r6, r2
00394d48 beq #0x394dec
00394d4c cmp r1, #0
00394d50 add r8, r0, #0x290
00394d54 beq #0x394e6c
00394d58 mov r0, r5
00394d5c bl #0x30de54
00394d60 mov r1, r5
00394d64 add r2, r5, r0
00394d68 mov r0, r8
00394d6c bl #0x3109e0
00394d70 cmp r6, #0
00394d74 cmpne r5, #0
00394d78 add r7, r4, #0x2a8
00394d7c beq #0x394e28
00394d80 mov r0, r6
00394d84 bl #0x30de54
00394d88 add r2, r6, r0
00394d8c mov r1, r6
00394d90 mov r0, r7
00394d94 bl #0x3109e0
00394d98 ldr r2, [r4, #0x2a0]
00394d9c ldr r3, [r4, #0x2a4]
00394da0 cmp r2, r3
00394da4 beq #0x394e50
00394da8 mov r1, #0
00394dac mov r0, #0xac
00394db0 bl #0x310570
00394db4 mov r3, r7
00394db8 mov r5, r0
00394dbc mov r1, r4
00394dc0 mov r2, r8
00394dc4 bl #0x472a0c
00394dc8 ldr r3, [r5, #8]
00394dcc cmp r3, #0
00394dd0 beq #0x394e94
00394dd4 mov r0, r4
00394dd8 mov r1, r5
00394ddc bl #0x394338
00394de0 ldr r3, [r5, #8]
00394de4 str r4, [r3, #0x204]
00394de8 pop {r4, r5, r6, r7, r8, pc}
00394dec cmp r1, #0
00394df0 beq #0x394e68
00394df4 mov r0, r1
00394df8 ldr r1, [r4, #0x2a4]
00394dfc bl #0x30e31c
00394e00 cmp r0, #0
00394e04 bne #0x394e60
00394e08 cmp r6, #0
00394e0c beq #0x394e24
00394e10 mov r0, r6
00394e14 ldr r1, [r4, #0x2bc]
00394e18 bl #0x30e31c
00394e1c cmp r0, #0
00394e20 bne #0x394e60
00394e24 pop {r4, r5, r6, r7, r8, pc}
00394e28 ldr r6, [pc, #0x78]
00394e2c mov r0, r7
00394e30 add r6, pc, r6
00394e34 mov r2, r6
00394e38 mov r1, r6
00394e3c bl #0x3109e0
00394e40 ldr r2, [r4, #0x2a0]
00394e44 ldr r3, [r4, #0x2a4]
00394e48 cmp r2, r3
00394e4c bne #0x394da8
00394e50 mov r0, r4
00394e54 mov r1, #0
00394e58 pop {r4, r5, r6, r7, r8, lr}
00394e5c b #0x394338
00394e60 add r8, r4, #0x290
00394e64 b #0x394d58
00394e68 add r8, r0, #0x290
00394e6c ldr r5, [pc, #0x38]
00394e70 mov r0, r8
00394e74 add r7, r4, #0x2a8
00394e78 add r5, pc, r5
00394e7c mov r2, r5
00394e80 mov r1, r5
00394e84 bl #0x3109e0
00394e88 mov r6, r5
00394e8c mov r2, r5
00394e90 b #0x394d8c
00394e94 mov r0, r5
00394e98 ldr r3, [r5]
00394e9c mov lr, pc
00394ea0 ldr pc, [r3, #4]
00394ea4 pop {r4, r5, r6, r7, r8, pc}
00394ea8 ldrsbeq r6, [r3], #-0x98
# _ZNK9Character11GetCharTypeEv 3a3054 16
003a3054 push {r4, lr}
003a3058 bl #0x3a3024
003a305c ldr r0, [r0, #0x38]
003a3060 pop {r4, pc}
# _ZNK9Character10IsFollowerEv 3a307c 24
003a307c push {r4, lr}
003a3080 bl #0x3a3054
003a3084 cmp r0, #2
003a3088 movne r0, #0
003a308c moveq r0, #1
003a3090 pop {r4, pc}
# _ZNK9Character8IsFaerieEv 3a3094 24
003a3094 push {r4, lr}
003a3098 bl #0x3a3054
003a309c cmp r0, #3
003a30a0 movne r0, #0
003a30a4 moveq r0, #1
003a30a8 pop {r4, pc}
# _ZNK9Character16GetCharModelNameEv 3a54d4 464
003a54d4 push {r4, r5, r6, lr}
003a54d8 sub sp, sp, #8
003a54dc mov r6, r0
003a54e0 bl #0x3a31e8
003a54e4 ldr r4, [pc, #0x194]
003a54e8 cmn r0, #1
003a54ec mov r5, r0
003a54f0 add r4, pc, r4
003a54f4 beq #0x3a5534
003a54f8 mov r0, r6
003a54fc bl #0x3a3094
003a5500 cmp r0, #0
003a5504 bne #0x3a5540
003a5508 ldr r3, [r6]
003a550c mov r0, r6
003a5510 mov lr, pc
003a5514 ldr pc, [r3, #0x28]
003a5518 cmp r0, #0
003a551c beq #0x3a552c
003a5520 bl #0x38174c
003a5524 cmp r0, #0
003a5528 beq #0x3a559c
003a552c cmp r5, #0
003a5530 bge #0x3a556c
003a5534 mov r0, #0
003a5538 add sp, sp, #8
003a553c pop {r4, r5, r6, pc}
003a5540 ldr r6, [r6, #0x418]
003a5544 cmp r6, #0
003a5548 beq #0x3a552c
003a554c mvn r1, #0
003a5550 mov r0, r6
003a5554 bl #0x3bb98c
003a5558 mov r1, r0
003a555c mov r0, r6
003a5560 bl #0x3aeac0
003a5564 ldr r5, [r0, #0xc]
003a5568 b #0x3a552c
003a556c ldr r3, [pc, #0x110]
003a5570 ldr r3, [r4, r3]
003a5574 ldr r3, [r3]
003a5578 cmp r5, r3
003a557c bge #0x3a5534
003a5580 ldr r3, [pc, #0x100]
003a5584 mov r2, #0xc
003a5588 ldr r3, [r4, r3]
003a558c ldr r3, [r3]
003a5590 mla r5, r2, r5, r3
003a5594 ldr r0, [r5, #8]
003a5598 b #0x3a5538
003a559c ldr r3, [pc, #0xe8]
003a55a0 mov r1, r6
003a55a4 ldr r3, [r4, r3]
003a55a8 ldr r0, [r3, #0x40]
003a55ac bl #0x36effc
003a55b0 cmp r0, #0
003a55b4 bne #0x3a552c
003a55b8 movw r3, #0x13c8
003a55bc ldrsh r2, [r6, r3]
003a55c0 sub r3, r2, #0x120
003a55c4 sub r3, r3, #2
003a55c8 cmp r3, #2
003a55cc bls #0x3a5604
003a55d0 sub r3, r2, #0x144
003a55d4 sub r3, r3, #1
003a55d8 cmp r3, #2
003a55dc bls #0x3a5618
003a55e0 sub r2, r2, #0x104
003a55e4 sub r2, r2, #3
003a55e8 cmp r2, #2
003a55ec bhi #0x3a562c
003a55f0 ldr r3, [pc, #0x90]
003a55f4 ldr r3, [r4, r3]
003a55f8 ldr r3, [r3]
003a55fc ldr r0, [r3, #0x3a4]
003a5600 b #0x3a5538
003a5604 ldr r3, [pc, #0x7c]
003a5608 ldr r3, [r4, r3]
003a560c ldr r3, [r3]
003a5610 ldr r0, [r3, #0x38c]
003a5614 b #0x3a5538
003a5618 ldr r3, [pc, #0x68]
003a561c ldr r3, [r4, r3]
003a5620 ldr r3, [r3]
003a5624 ldr r0, [r3, #0x398]
003a5628 b #0x3a5538
003a562c ldr r3, [pc, #0x5c]
003a5630 ldr r3, [r4, r3]
003a5634 ldr r3, [r3]
003a5638 cmp r3, #2
003a563c streq r0, [r0]
003a5640 beq #0x3a552c
003a5644 cmp r3, #1
003a5648 bne #0x3a552c
003a564c ldr r0, [pc, #0x40]
003a5650 ldr r1, [pc, #0x40]
003a5654 ldr r2, [pc, #0x40]
003a5658 ldr r0, [r4, r0]
003a565c ldr r3, [pc, #0x3c]
003a5660 movw ip, #0x4a7
003a5664 add r1, pc, r1
003a5668 add r2, pc, r2
003a566c add r3, pc, r3
003a5670 add r0, r0, #0xa8
003a5674 str ip, [sp]
003a5678 bl #0x30e004
003a567c b #0x3a552c
003a5680 subseq pc, lr, r0, lsr #11
003a5684 andeq r3, r0, ip, lsl #24
003a5688 andeq r4, r0, r4, asr #6
003a568c strdeq r3, r4, [r0], -r4
003a5690 andeq r3, r0, r0, asr #19
003a5694 andeq r1, r0, r0, asr #19
003a5698 subseq r8, r1, r4, ror sp
003a569c subseq r8, r1, r0, lsl #30
003a56a0 subseq sp, r1, r4, lsr fp
# _ZN9CharacterC1EN10ObjectBase6GO_IDSE 3aa1b4 1448
003aa1b4 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8 add ip, r0, #0x374
003aa1bc sub sp, sp, #0x3c
003aa1c0 mov r4, r0
003aa1c4 str ip, [sp, #0xc]
003aa1c8 bl #0x38c398
003aa1cc ldr ip, [sp, #0xc]
003aa1d0 add r5, r4, #0x4f0
003aa1d4 add r5, r5, #0xc
003aa1d8 mov r0, ip
003aa1dc bl #0x404db8
003aa1e0 add r0, r4, #0x3b4
003aa1e4 str r0, [sp, #0x20]
003aa1e8 add r0, r4, #0x37c
003aa1ec bl #0x3ff330
003aa1f0 add r2, r4, #0x490
003aa1f4 add r1, r4, #0x3c8
003aa1f8 add r2, r2, #0xc
003aa1fc ldr r0, [sp, #0x20]
003aa200 str r1, [sp, #0x1c]
003aa204 str r2, [sp, #0x14]
003aa208 bl #0x3dbb0c
003aa20c ldr r0, [sp, #0x1c]
003aa210 bl #0x3cebf0
003aa214 ldr r0, [sp, #0x14]
003aa218 bl #0x3c8ff4
003aa21c add r3, r4, #0x560
003aa220 mov r0, r5
003aa224 str r3, [sp, #0x18]
003aa228 ldr sb, [pc, #0x50c]
003aa22c bl #0x3c1b58
003aa230 ldr r0, [sp, #0x18]
003aa234 bl #0x3df084
003aa238 ldr lr, [pc, #0x500]
003aa23c add sb, pc, sb
003aa240 mov r8, #0
003aa244 ldr lr, [sb, lr]
003aa248 mov fp, #1
003aa24c mvn r6, #0
003aa250 add sl, lr, #0x324
003aa254 str sl, [sp, #0x34]
003aa258 add sl, lr, #0x180
003aa25c str sl, [sp, #0x10]
003aa260 add sl, lr, #0x1f4
003aa264 str sl, [sp, #0x24]
003aa268 add sl, lr, #0x220
003aa26c str sl, [sp, #0x28]
003aa270 add sl, lr, #0x230
003aa274 str sl, [sp, #0x2c]
003aa278 add r0, lr, #8
003aa27c add r1, lr, #0x15c
003aa280 add r2, lr, #0x168
003aa284 add sl, lr, #0x304
003aa288 str sl, [sp, #0x30]
003aa28c stm r4, {r0, r1}
003aa290 str r2, [r4, #0x24]
003aa294 ldr r0, [sp, #0x10]
003aa298 add lr, lr, #0x314
003aa29c add r7, r4, #0x1380
003aa2a0 str r0, [r4, #0x374]
003aa2a4 ldr r1, [sp, #0x24]
003aa2a8 add r3, r7, #0x18
003aa2ac movw sl, #0x13a8
003aa2b0 str r1, [r4, #0x37c]
003aa2b4 ldr r2, [sp, #0x28]
003aa2b8 add r7, r7, #0x30
003aa2bc str r2, [r4, #0x3b4]
003aa2c0 ldr r0, [sp, #0x2c]
003aa2c4 str r0, [r4, #0x3c8]
003aa2c8 ldr r1, [sp, #0x30]
003aa2cc str lr, [r4, #0x4fc]
003aa2d0 mov r0, r3
003aa2d4 str r1, [r4, #0x49c]
003aa2d8 ldr r2, [sp, #0x34]
003aa2dc mov r1, #0x10
003aa2e0 str r2, [r4, #0x560]
003aa2e4 movw r2, #0x1394
003aa2e8 strb r8, [r4, r2]
003aa2ec movw r2, #0x1395
003aa2f0 strb r8, [r4, r2]
003aa2f4 movw r2, #0x1396
003aa2f8 strb fp, [r4, r2]
003aa2fc movw r2, #0x1397
003aa300 strb r6, [r4, r2]
003aa304 movw r2, #0x13ac
003aa308 str r3, [r4, r2]
003aa30c str r3, [r4, sl]
003aa310 bl #0x31167c
003aa314 ldr r3, [r4, sl]
003aa318 mov sl, #0x13c0
003aa31c mov r0, r7
003aa320 strb r8, [r3]
003aa324 movw r3, #0x13c4
003aa328 str r7, [r4, r3]
003aa32c mov r1, #0x10
003aa330 str r7, [r4, sl]
003aa334 bl #0x31167c
003aa338 ldr r2, [r4, sl]
003aa33c add r7, r4, sl
003aa340 add r3, r7, #0xc
003aa344 strb r8, [r2]
003aa348 movw r2, #0x13c8
003aa34c strh r6, [r4, r2]
003aa350 movw r2, #0x13ca
003aa354 strh r6, [r4, r2]
003aa358 movw sl, #0x13dc
003aa35c movw r2, #0x13e0
003aa360 str r3, [r4, r2]
003aa364 mov r0, r3
003aa368 str r3, [r4, sl]
003aa36c mov r1, #0x10
003aa370 bl #0x31167c
003aa374 ldr r3, [r4, sl]
003aa378 add r7, r7, #0x28
003aa37c movw sl, #0x13f8
003aa380 strb r8, [r3]
003aa384 movw r3, #0x13e4
003aa388 strb fp, [r4, r3]
003aa38c movw r3, #0x13fc
003aa390 str r7, [r4, r3]
003aa394 mov r0, r7
003aa398 str r7, [r4, sl]
003aa39c mov r1, #0x10
003aa3a0 bl #0x31167c
003aa3a4 ldr r3, [r4, sl]
003aa3a8 add r7, r4, #0x1400
003aa3ac movw sl, #0x1410
003aa3b0 strb r8, [r3]
003aa3b4 movw r3, #0x1414
003aa3b8 str r7, [r4, r3]
003aa3bc mov r0, r7
003aa3c0 str r7, [r4, sl]
003aa3c4 mov r1, #0x10
003aa3c8 bl #0x31167c
003aa3cc ldr r3, [r4, sl]
003aa3d0 add r7, r7, #0x18
003aa3d4 movw sl, #0x1428
003aa3d8 strb r8, [r3]
003aa3dc movw r3, #0x142c
003aa3e0 str r7, [r4, r3]
003aa3e4 mov r0, r7
003aa3e8 str r7, [r4, sl]
003aa3ec mov r1, #0x10
003aa3f0 bl #0x31167c
003aa3f4 ldr r2, [r4, sl]
003aa3f8 mov r3, #0
003aa3fc mov r1, #0xbf000000
003aa400 strb r8, [r2]
003aa404 movw r2, #0x14a8
003aa408 strb r6, [r4, r2]
003aa40c movw r2, #0x1430
003aa410 strb fp, [r4, r2]
003aa414 movw r2, #0x1434
003aa418 str r8, [r4, r2]
003aa41c movw r2, #0x1438
003aa420 str r8, [r4, r2]
003aa424 movw r2, #0x1448
003aa428 strb fp, [r4, r2]
003aa42c movw r2, #0x1449
003aa430 strb r8, [r4, r2]
003aa434 movw r2, #0x144c
003aa438 str r8, [r4, r2]
003aa43c movw r2, #0x1450
003aa440 str r3, [r4, r2]
003aa444 movw r2, #0x1454
003aa448 str r3, [r4, r2]
003aa44c movw r2, #0x1458
003aa450 str r3, [r4, r2]
003aa454 movw r2, #0x145c
003aa458 str r3, [r4, r2]
003aa45c movw r2, #0x1460
003aa460 str r3, [r4, r2]
003aa464 movw r2, #0x1464
003aa468 str r3, [r4, r2]
003aa46c movw r2, #0x1468
003aa470 str r3, [r4, r2]
003aa474 movw r2, #0x146c
003aa478 str r3, [r4, r2]
003aa47c movw r2, #0x1470
003aa480 str r3, [r4, r2]
003aa484 movw r2, #0x1474
003aa488 str r3, [r4, r2]
003aa48c movw r2, #0x1478
003aa490 str r3, [r4, r2]
003aa494 movw r2, #0x147c
003aa498 str r3, [r4, r2]
003aa49c mov r2, #0x1480
003aa4a0 strb r8, [r4, r2]
003aa4a4 movw r2, #0x1481
003aa4a8 strb r8, [r4, r2]
003aa4ac movw r2, #0x1484
003aa4b0 str r8, [r4, r2]
003aa4b4 movw r2, #0x1488
003aa4b8 str r8, [r4, r2]
003aa4bc movw r2, #0x148c
003aa4c0 str r8, [r4, r2]
003aa4c4 movw r2, #0x1490
003aa4c8 str r8, [r4, r2]
003aa4cc movw r2, #0x1494
003aa4d0 str r8, [r4, r2]
003aa4d4 movw r2, #0x1498
003aa4d8 str r6, [r4, r2]
003aa4dc movw r2, #0x149c
003aa4e0 str r8, [r4, r2]
003aa4e4 movw r2, #0x14a0
003aa4e8 str r8, [r4, r2]
003aa4ec movw r2, #0x14a4
003aa4f0 str r8, [r4, r2]
003aa4f4 movw r2, #0x14aa
003aa4f8 strh r8, [r4, r2]
003aa4fc movw r2, #0x14ac
003aa500 strb r8, [r4, r2]
003aa504 movw r2, #0x14d8
003aa508 str r3, [r4, r2]
003aa50c add r1, r1, #0x800000
003aa510 movw r2, #0x14fc
003aa514 str r1, [r4, r2]
003aa518 movw r2, #0x1504
003aa51c str r6, [r4, r2]
003aa520 movw r2, #0x14ad
003aa524 strb r8, [r4, r2]
003aa528 movw r2, #0x14b0
003aa52c str r3, [r4, r2]
003aa530 movw r2, #0x14b4
003aa534 str r3, [r4, r2]
003aa538 movw r2, #0x14b8
003aa53c str r3, [r4, r2]
003aa540 movw r2, #0x14bc
003aa544 str r3, [r4, r2]
003aa548 mov r2, #0x14c0
003aa54c str r3, [r4, r2]
003aa550 movw r2, #0x14c4
003aa554 str r3, [r4, r2]
003aa558 movw r3, #0x14c8
003aa55c strb r8, [r4, r3]
003aa560 movw r3, #0x14ca
003aa564 strh r6, [r4, r3]
003aa568 movw r3, #0x14cc
003aa56c str r8, [r4, r3]
003aa570 movw r3, #0x14d0
003aa574 strh r8, [r4, r3]
003aa578 movw r3, #0x14d4
003aa57c str r8, [r4, r3]
003aa580 movw r3, #0x14dc
003aa584 strb r8, [r4, r3]
003aa588 movw r3, #0x14e4
003aa58c strb r8, [r4, r3]
003aa590 movw r3, #0x14e5
003aa594 strb r8, [r4, r3]
003aa598 movw r3, #0x14e8
003aa59c str r8, [r4, r3]
003aa5a0 movw r3, #0x14ec
003aa5a4 str r8, [r4, r3]
003aa5a8 add r7, r4, #0x1500
003aa5ac movw r3, #0x14f0
003aa5b0 add r0, r4, #0x1a40
003aa5b4 strb r8, [r4, r3]
003aa5b8 add r0, r0, #8
003aa5bc mov r3, #0x1500
003aa5c0 add r7, r7, #8
003aa5c4 str r6, [r4, r3]
003aa5c8 str r0, [sp, #0x10]
003aa5cc mov r0, r7
003aa5d0 bl #0x3a6a24
003aa5d4 ldr r0, [sp, #0x10]
003aa5d8 bl #0x3a6a24
003aa5dc add r0, r4, #0x304
003aa5e0 mov r1, r4
003aa5e4 strb fp, [r4, #0x28]
003aa5e8 bl #0x4a191c
003aa5ec strb fp, [r4, #0x1c4]
003aa5f0 strb fp, [r4, #0x85]
003aa5f4 mov r0, #0x10
003aa5f8 mov r1, r8
003aa5fc bl #0x310570
003aa600 ldr r3, [pc, #0x13c]
003aa604 ldr ip, [sp, #0xc]
003aa608 mov r6, r0
003aa60c ldr r3, [sb, r3]
003aa610 cmp ip, r8
003aa614 strb r8, [r6, #0xa]
003aa618 add r3, r3, #8
003aa61c str r8, [r0, #0xc]
003aa620 stm r0, {r3, ip}
003aa624 strb r8, [r6, #8]
003aa628 strb r8, [r6, #9]
003aa62c beq #0x3aa6e0
003aa630 mov r0, ip
003aa634 mov r1, r6
003aa638 bl #0x404e10
003aa63c ldr r3, [r4, #0x378]
003aa640 ldr r0, [sp, #0x20]
003aa644 mov r1, r4
003aa648 str r4, [r3, #0xc]
003aa64c bl #0x3db480
003aa650 ldr r0, [sp, #0x1c]
003aa654 mov r1, r4
003aa658 bl #0x3cb7c0
003aa65c ldr r0, [sp, #0x14]
003aa660 mov r1, r4
003aa664 bl #0x3c9890
003aa668 mov r0, r5
003aa66c mov r1, r4
003aa670 bl #0x3c1600
003aa674 ldr r0, [sp, #0x18]
003aa678 mov r1, r4
003aa67c bl #0x3dec0c
003aa680 mov r6, #0
003aa684 str r4, [r4, #0x380]
003aa688 mov r1, r6
003aa68c mov r0, r5
003aa690 add r6, r6, #1
003aa694 bl #0x3c7318
003aa698 cmp r6, #0x14
003aa69c bne #0x3aa688
003aa6a0 mov r1, #0
003aa6a4 movw r2, #0x14e0
003aa6a8 str r1, [r4, r2]
003aa6ac mvn r3, #0
003aa6b0 movw r2, #0x14f4
003aa6b4 str r3, [r4, r2]
003aa6b8 str r7, [r4, #0x100]
003aa6bc ldr sl, [sp, #0x10]
003aa6c0 movw r2, #0x14f8
003aa6c4 mov r0, r4
003aa6c8 str sl, [r4, #0x104]
003aa6cc str r3, [r4, r2]
003aa6d0 mov r3, #1
003aa6d4 strb r3, [r4, #0xf8]
003aa6d8 add sp, sp, #0x3c
003aa6dc pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0 ldr r3, [pc, #0x60]
003aa6e4 ldr r3, [sb, r3]
003aa6e8 ldr r3, [r3]
003aa6ec cmp r3, #2
003aa6f0 streq ip, [r4, #0x374]
003aa6f4 beq #0x3aa630
003aa6f8 cmp r3, #1
003aa6fc bne #0x3aa630
003aa700 ldr r0, [pc, #0x44]
003aa704 ldr r1, [pc, #0x44]
003aa708 ldr r2, [pc, #0x44]
003aa70c ldr r0, [sb, r0]
003aa710 ldr r3, [pc, #0x40]
003aa714 mov lr, #0x44
003aa718 add r1, pc, r1
003aa71c add r0, r0, #0xa8
003aa720 add r2, pc, r2
003aa724 add r3, pc, r3
003aa728 str ip, [sp, #0xc]
003aa72c str lr, [sp]
003aa730 bl #0x30e004
003aa734 ldr ip, [sp, #0xc]
003aa738 b #0x3aa630
003aa73c subseq sl, lr, r4, asr r8
003aa740 andeq r2, r0, r8, lsl #28
003aa744 andeq r2, r0, r4, lsr #21
003aa748 andeq r3, r0, r0, asr #19
003aa74c andeq r1, r0, r0, asr #19
003aa750 subseq r3, r1, r0, asr #25
003aa754 subseq r8, r1, r0, lsr #27
003aa758 subseq r8, r1, ip, lsr #27
# _ZN9Character11ChangeFaeryEj 3ae99c 256
003ae99c push {r4, r5, r6, lr}
003ae9a0 sub sp, sp, #8
003ae9a4 mov r6, r1
003ae9a8 mov r4, r0
003ae9ac bl #0x3bb8e4
003ae9b0 mov r5, r0
003ae9b4 mov r1, r5
003ae9b8 mov r0, r4
003ae9bc bl #0x3bba20
003ae9c0 ldr r3, [pc, #0xbc]
003ae9c4 cmp r0, r6
003ae9c8 add r3, pc, r3
003ae9cc bhi #0x3ae9f4
003ae9d0 ldr r2, [pc, #0xb0]
003ae9d4 ldr r2, [r3, r2]
003ae9d8 ldr r2, [r2]
003ae9dc cmp r2, #2
003ae9e0 moveq r3, #0
003ae9e4 streq r3, [r3]
003ae9e8 beq #0x3ae9f4
003ae9ec cmp r2, #1
003ae9f0 beq #0x3aea50
003ae9f4 mov r0, r4
003ae9f8 mov r1, r6
003ae9fc mov r2, r5
003aea00 bl #0x3bb9d8
003aea04 add r0, r4, #0x3c8
003aea08 bl #0x3d8894
003aea0c ldr r4, [r4, #0x420]
003aea10 cmp r4, #0
003aea14 beq #0x3aea48
003aea18 mov r0, r4
003aea1c bl #0x3a54d4
003aea20 mov r2, #0
003aea24 mov r1, r0
003aea28 mov r3, #1
003aea2c mov r0, r4
003aea30 bl #0x394d34
003aea34 add r0, r4, #0x490
003aea38 add r0, r0, #0xc
003aea3c add sp, sp, #8
003aea40 pop {r4, r5, r6, lr}
003aea44 b #0x3c99a0
003aea48 add sp, sp, #8
003aea4c pop {r4, r5, r6, pc}
003aea50 ldr r0, [pc, #0x34]
003aea54 ldr r1, [pc, #0x34]
003aea58 ldr r2, [pc, #0x34]
003aea5c ldr r0, [r3, r0]
003aea60 ldr r3, [pc, #0x30]
003aea64 mov ip, #0x6b
003aea68 add r1, pc, r1
003aea6c add r2, pc, r2
003aea70 add r3, pc, r3
003aea74 add r0, r0, #0xa8
003aea78 str ip, [sp]
003aea7c bl #0x30e004
003aea80 b #0x3ae9f4
003aea84 subseq r6, lr, r8, asr #1
003aea88 andeq r3, r0, r0, asr #19
003aea8c andeq r1, r0, r0, asr #19
003aea90 subseq pc, r0, r0, ror sb
003aea94 subseq r4, r1, r4, ror #26
003aea98 subseq r4, r1, r8, lsl #27
# _ZN12CharAnimator25ANIM_AddSetToRenderObjectEv 3c99a0 344
003c99a0 push {r4, r5, r6, r7, r8, sl, lr}
003c99a4 ldr r4, [pc, #0x134]
003c99a8 ldr r6, [pc, #0x134]
003c99ac mov r5, r0
003c99b0 add r4, pc, r4
003c99b4 ldr r3, [r4, r6]
003c99b8 ldr r0, [r0, #4]
003c99bc sub sp, sp, #0x2c
003c99c0 ldr r3, [r3]
003c99c4 str r3, [sp, #0x24]
003c99c8 ldr r7, [r0, #0x2d8]
003c99cc cmp r7, #0
003c99d0 beq #0x3c9a40
003c99d4 bl #0x3a3094
003c99d8 cmp r0, #0
003c99dc beq #0x3c9a5c
003c99e0 mov r1, #0
003c99e4 mov r0, #0x14
003c99e8 ldr sl, [r7, #8]
003c99ec bl #0x310570
003c99f0 mov r1, sl
003c99f4 mov r8, r0
003c99f8 ldr r2, [r5, #0x3c]
003c99fc bl #0x4751b4
003c9a00 mov r0, r7
003c9a04 mov r1, r8
003c9a08 bl #0x470a84
003c9a0c mov r3, #0
003c9a10 strb r3, [r5, #0x54]
003c9a14 ldr r3, [pc, #0xcc]
003c9a18 ldr ip, [r7, #0x38]
003c9a1c mov r2, r5
003c9a20 ldr r1, [r4, r3]
003c9a24 ldr r3, [pc, #0xc0]
003c9a28 mov r0, ip
003c9a2c ldr ip, [ip]
003c9a30 ldr r3, [r4, r3]
003c9a34 str r5, [sp]
003c9a38 mov lr, pc
003c9a3c ldr pc, [ip, #0x2c]
003c9a40 ldr r3, [r4, r6]
003c9a44 ldr r2, [sp, #0x24]
003c9a48 ldr r3, [r3]
003c9a4c cmp r2, r3
003c9a50 bne #0x3c9adc
003c9a54 add sp, sp, #0x2c
003c9a58 pop {r4, r5, r6, r7, r8, sl, pc}
003c9a5c ldr r3, [pc, #0x8c]
003c9a60 add r8, sp, #0xc
003c9a64 ldr sl, [r4, r3]
003c9a68 mov r0, sl
003c9a6c bl #0x337888
003c9a70 ldr r1, [pc, #0x7c]
003c9a74 add r2, sp, #8
003c9a78 mov r0, r8
003c9a7c add r1, pc, r1
003c9a80 bl #0x3140ec
003c9a84 mov r0, sl
003c9a88 mov r1, r8
003c9a8c bl #0x337a88
003c9a90 mov sl, r0
003c9a94 mov r0, r8
003c9a98 bl #0x3139ac
003c9a9c cmp sl, #0
003c9aa0 bne #0x3c99e0
003c9aa4 mov r1, sl
003c9aa8 mov r0, #0x18
003c9aac ldr sl, [r7, #8]
003c9ab0 bl #0x310570
003c9ab4 mov r1, sl
003c9ab8 mov r8, r0
003c9abc ldr r2, [r5, #0x3c]
003c9ac0 bl #0x476ddc
003c9ac4 mov r0, r7
003c9ac8 mov r1, r8
003c9acc bl #0x470a84
003c9ad0 mov r3, #1
003c9ad4 strb r3, [r5, #0x54]
003c9ad8 b #0x3c9a14
003c9adc bl #0x30e310
003c9ae0 subseq fp, ip, r0, ror #1
003c9ae4 andeq r4, r0, ip, lsr #1
003c9ae8 strdeq r1, r2, [r0], -r0
003c9aec andeq r2, r0, r0, ror #14
003c9af0 andeq r0, r0, r4, lsl #17
003c9af4 subeq fp, pc, r4, ror #10
# _ZN6CharAIC2Ev 3cebf0 352
003cebf0 ldr r3, [pc, #0x14c]
003cebf4 ldr r2, [pc, #0x14c]
003cebf8 push {r4, r5, lr}
003cebfc add r3, pc, r3
003cec00 ldr r2, [r3, r2]
003cec04 mov r4, r0
003cec08 mov r1, #0
003cec0c add r2, r2, #8
003cec10 str r2, [r4]
003cec14 ldr r2, [pc, #0x130]
003cec18 mov r0, #1
003cec1c mvn ip, #0
003cec20 mov r5, r4
003cec24 strb r0, [r4, #0x55]
003cec28 str r1, [r4, #8]
003cec2c str r1, [r4, #0xc]
003cec30 strb r1, [r4, #0x18]
003cec34 str r1, [r4, #0x1c]
003cec38 str r1, [r4, #0x20]
003cec3c strb r1, [r4, #0x24]
003cec40 str r1, [r4, #0x28]
003cec44 strb r1, [r4, #0x2c]
003cec48 str r1, [r4, #0x30]
003cec4c str r1, [r4, #0x34]
003cec50 str r1, [r4, #0x3c]
003cec54 str r1, [r4, #0x40]
003cec58 str r1, [r4, #0x44]
003cec5c strb r1, [r4, #0x49]
003cec60 strb r0, [r4, #0x4a]
003cec64 strb r0, [r4, #0x4b]
003cec68 strb r1, [r4, #0x4c]
003cec6c strb r0, [r4, #0x4d]
003cec70 str r1, [r4, #0x50]
003cec74 strb r0, [r4, #0x54]
003cec78 str r1, [r4, #0x58]
003cec7c mov r0, r4
003cec80 str r1, [r4, #0x60]
003cec84 str ip, [r4, #0x10]
003cec88 str ip, [r4, #0x14]
003cec8c str ip, [r4, #0x38]
003cec90 strb r1, [r5, #0x5c]!
003cec94 str r5, [r4, #0x68]
003cec98 str r5, [r4, #0x64]
003cec9c str r1, [r4, #0x6c]
003ceca0 str r1, [r4, #0x80]
003ceca4 strb r1, [r0, #0x7c]!
003ceca8 ldr r5, [r3, r2]
003cecac mov r2, r4
003cecb0 str r0, [r4, #0x88]
003cecb4 str r0, [r4, #0x84]
003cecb8 str r1, [r4, #0x8c]
003cecbc str r1, [r4, #0x98]
003cecc0 add r0, r4, #0xac
003cecc4 strb r1, [r2, #0x94]!
003cecc8 str r2, [r4, #0xa0]
003ceccc str r0, [r4, #0xb0]
003cecd0 str ip, [r4, #0xcc]
003cecd4 strb r1, [r4, #0xd1]
003cecd8 str r2, [r4, #0x9c]
003cecdc str r1, [r4, #0xa4]
003cece0 str r0, [r4, #0xac]
003cece4 str r1, [r4, #0xb4]
003cece8 str r1, [r4, #0xb8]
003cecec str r1, [r4, #0xbc]
003cecf0 str r1, [r4, #0xc0]
003cecf4 str r1, [r4, #0xc4]
003cecf8 str r1, [r4, #0xc8]
003cecfc strb r1, [r4, #0xd0]
003ced00 ldr r1, [r5, #0x18]
003ced04 ldr r2, [r5, #0x10]
003ced08 sub sp, sp, #0xc
003ced0c sub r3, r1, #4
003ced10 cmp r2, r3
003ced14 str r4, [sp, #4]
003ced18 beq #0x3ced38
003ced1c str r4, [r2]
003ced20 ldr r3, [r5, #0x10]
003ced24 add r3, r3, #4
003ced28 str r3, [r5, #0x10]
003ced2c mov r0, r4
003ced30 add sp, sp, #0xc
003ced34 pop {r4, r5, pc}
003ced38 add r0, sp, #4
003ced3c bl #0x3ce810
003ced40 b #0x3ced2c
# _ZN5Level22PlaceFaeryAndFollowersEP9Character 3f0898 584
003f0898 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f089c ldr r7, [pc, #0x234]
003f08a0 ldr r8, [pc, #0x234]
003f08a4 sub sp, sp, #0x1c
003f08a8 add r7, pc, r7
003f08ac ldr r3, [r7, r8]
003f08b0 str r1, [sp]
003f08b4 add fp, sp, #0xc
003f08b8 ldr sb, [r3, #0x38]
003f08bc ldr r6, [sb, #0x60]!
003f08c0 cmp sb, r6
003f08c4 beq #0x3f0a1c
003f08c8 ldr r5, [r6, #8]
003f08cc cmp r5, #0
003f08d0 beq #0x3f0a10
003f08d4 mov r0, r5
003f08d8 bl #0x3a3094
003f08dc cmp r0, #0
003f08e0 beq #0x3f0a24
003f08e4 ldr r2, [sp]
003f08e8 mov r3, #0
003f08ec str r3, [sp, #0xc]
003f08f0 cmp r2, #0
003f08f4 str r3, [sp, #0x10]
003f08f8 str r3, [sp, #0x14]
003f08fc movne sl, r2
003f0900 beq #0x3f0a64
003f0904 mov r1, fp
003f0908 mov r0, sl
003f090c bl #0x393ae4
003f0910 mov r0, sl
003f0914 bl #0x3935dc
003f0918 ldr r1, [r0]
003f091c mov r4, r0
003f0920 ldr r0, [sp, #0xc]
003f0924 bl #0x30eba4
003f0928 str r0, [sp, #0xc]
003f092c ldr r1, [r4, #4]
003f0930 ldr r0, [sp, #0x10]
003f0934 bl #0x30eba4
003f0938 str r0, [sp, #0x10]
003f093c ldr r1, [r4, #8]
003f0940 ldr r0, [sp, #0x14]
003f0944 bl #0x30eba4
003f0948 mov r1, fp
003f094c mov r2, #1
003f0950 str r0, [sp, #0x14]
003f0954 mov r0, r5
003f0958 bl #0x393db4
003f095c mov r0, r5
003f0960 bl #0x393e90
003f0964 mov r0, r5
003f0968 bl #0x3a3094
003f096c cmp r0, #0
003f0970 beq #0x3f0a3c
003f0974 ldr r3, [r7, r8]
003f0978 ldr r0, [r3, #0x40]
003f097c ldr r3, [r0, #0x6c4]
003f0980 cmp r3, #0
003f0984 ble #0x3f09bc
003f0988 mov r4, #0
003f098c mov r1, r4
003f0990 mov r2, #0
003f0994 bl #0x36e744
003f0998 ldr r3, [r0, #0x660]
003f099c add r4, r4, #1
003f09a0 cmp r3, #0
003f09a4 strne r5, [r3, #0x420]
003f09a8 ldr r3, [r7, r8]
003f09ac ldr r0, [r3, #0x40]
003f09b0 ldr r3, [r0, #0x6c4]
003f09b4 cmp r4, r3
003f09b8 blt #0x3f098c
003f09bc ldr r3, [r5, #0x418]
003f09c0 mov r1, #0
003f09c4 mov r2, #1
003f09c8 str r3, [sp, #4]
003f09cc bl #0x36e478
003f09d0 ldr r4, [r0, #0x660]
003f09d4 cmp r4, #0
003f09d8 beq #0x3f0a4c
003f09dc add r5, r5, #0x3c8
003f09e0 mov r0, r5
003f09e4 mov r1, r4
003f09e8 bl #0x3d4d80
003f09ec mov r0, r4
003f09f0 mvn r1, #0
003f09f4 bl #0x3bb98c
003f09f8 mov r1, r0
003f09fc mov r0, sl
003f0a00 bl #0x3ae99c
003f0a04 mov r0, r5
003f0a08 ldr r1, [sp, #4]
003f0a0c bl #0x3d4d80
003f0a10 ldr r6, [r6]
003f0a14 cmp sb, r6
003f0a18 bne #0x3f08c8
003f0a1c add sp, sp, #0x1c
003f0a20 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f0a24 mov r0, r5
003f0a28 bl #0x3a307c
003f0a2c cmp r0, #0
003f0a30 bne #0x3f08e4
003f0a34 ldr r6, [r6]
003f0a38 b #0x3f0a14
003f0a3c mov r0, r5
003f0a40 bl #0x38c600
003f0a44 ldr r6, [r6]
003f0a48 b #0x3f0a14
003f0a4c add r5, r5, #0x3c8
003f0a50 mov r0, r5
003f0a54 mov r1, sl
003f0a58 bl #0x3d4d80
003f0a5c mov r0, sl
003f0a60 b #0x3f09f0
003f0a64 bl #0x7fd794
003f0a68 ldrb r3, [r0, #5]
003f0a6c cmp r3, #0
003f0a70 beq #0x3f0ab0
003f0a74 ldr r4, [r7, r8]
003f0a78 ldr r0, [r4, #0x40]
003f0a7c bl #0x36e09c
003f0a80 ldr sl, [r0, #0x660]
003f0a84 ldr r0, [r4, #0x40]
003f0a88 bl #0x36f074
003f0a8c cmp r0, #0
003f0a90 bne #0x3f0ac8
003f0a94 ldr r0, [r4, #0x40]
003f0a98 bl #0x36e09c
003f0a9c ldr r3, [r0, #0x670]
003f0aa0 mov r2, #0
003f0aa4 str r2, [r5, #0x114]
003f0aa8 str r3, [r5, #0x110]
003f0aac b #0x3f0ac8
003f0ab0 ldr r3, [r7, r8]
003f0ab4 ldr r1, [sp]
003f0ab8 mov r2, #1
003f0abc ldr r0, [r3, #0x40]
003f0ac0 bl #0x36e478
003f0ac4 ldr sl, [r0, #0x660]
003f0ac8 cmp sl, #0
003f0acc bne #0x3f0904
003f0ad0 ldr r6, [r6]
003f0ad4 b #0x3f0a14
003f0ad8 subseq r4, sl, r8, ror #3
003f0adc strdeq r3, r4, [r0], -r4
