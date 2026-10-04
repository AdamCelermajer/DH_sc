_ZN6Arrays19AnimatedEffectTable4readEP11IStreamBase
004bc504: push {r4, r5, r6, r7, r8, sl, lr}
004bc508: sub sp, sp, #0xc
004bc50c: mov sl, r0
004bc510: bl #0x313a90
004bc514: ldr r6, [pc, #0x120]
004bc518: mov r3, #1
004bc51c: cmp r3, #0
004bc520: str r0, [sp, #4]
004bc524: str r3, [sp]
004bc528: add r6, pc, r6
004bc52c: bne #0x4bc574
004bc530: add r3, sp, #4
004bc534: add r2, r3, #2
004bc538: add r3, r3, #1
004bc53c: ldrb r0, [r2, #1]
004bc540: ldrb r1, [r3, #-1]
004bc544: cmp r2, r3
004bc548: eor r1, r0, r1
004bc54c: strb r1, [r3, #-1]
004bc550: ldrb r0, [r2, #1]
004bc554: eor r1, r1, r0
004bc558: strb r1, [r2, #1]
004bc55c: ldrb r0, [r3, #-1]
004bc560: sub r2, r2, #1
004bc564: eor r1, r1, r0
004bc568: strb r1, [r3, #-1]
004bc56c: add r3, r3, #1
004bc570: bhi #0x4bc53c
004bc574: ldr r7, [pc, #0xc4]
004bc578: bl #0x4a8944
004bc57c: ldr r4, [sp, #4]
004bc580: ldr r3, [r6, r7]
004bc584: mov r1, #1
004bc588: add r0, r4, r4, lsl #1
004bc58c: add r0, r0, r1
004bc590: str r4, [r3]
004bc594: lsl r0, r0, #3
004bc598: bl #0x31056c
004bc59c: mov r3, #0x18
004bc5a0: cmp r4, #0
004bc5a4: stm r0, {r3, r4}
004bc5a8: add r3, r0, #8
004bc5ac: beq #0x4bc5d8
004bc5b0: ldr r1, [pc, #0x8c]
004bc5b4: mov r2, #0
004bc5b8: mov ip, r2
004bc5bc: ldr r1, [r6, r1]
004bc5c0: add r1, r1, #8
004bc5c4: add r2, r2, #1
004bc5c8: cmp r2, r4
004bc5cc: str r1, [r0, #8]
004bc5d0: str ip, [r0, #0x18]!
004bc5d4: bne #0x4bc5c4
004bc5d8: ldr r2, [r6, r7]
004bc5dc: ldr r8, [pc, #0x64]
004bc5e0: ldr r1, [r2]
004bc5e4: ldr r2, [r6, r8]
004bc5e8: cmp r1, #0
004bc5ec: str r3, [r2]
004bc5f0: beq #0x4bc634
004bc5f4: mov r4, #0
004bc5f8: mov r5, r4
004bc5fc: b #0x4bc608
004bc600: ldr r3, [r6, r8]
004bc604: ldr r3, [r3]
004bc608: add r0, r3, r4
004bc60c: mov r1, sl
004bc610: ldr r3, [r3, r4]
004bc614: mov lr, pc
004bc618: ldr pc, [r3, #0xc]
004bc61c: ldr r3, [r6, r7]
004bc620: add r5, r5, #1
004bc624: add r4, r4, #0x18
004bc628: ldr r3, [r3]
004bc62c: cmp r3, r5
004bc630: bhi #0x4bc600
004bc634: add sp, sp, #0xc
004bc638: pop {r4, r5, r6, r7, r8, sl, pc}
004bc63c: subeq r8, sp, r8, ror #10
004bc640: andeq r0, r0, r4, asr #13
004bc644: strheq r1, [r0], -ip
004bc648: andeq r3, r0, r0, ror sb

_ZN7Structs9AnimFXTpl4readEP11IStreamBase
004ed73c: push {r4, r5, r6, r7, lr}
004ed740: mov r5, r0
004ed744: sub sp, sp, #0xc
004ed748: mov r0, r1
004ed74c: mov r7, r1
004ed750: add r1, r5, #4
004ed754: bl #0x4db89c
004ed758: ldr r6, [pc, #0x210]
004ed75c: mov r0, r7
004ed760: add r1, r5, #8
004ed764: bl #0x459090
004ed768: mov r3, #1
004ed76c: cmp r3, #0
004ed770: str r3, [sp, #4]
004ed774: add r6, pc, r6
004ed778: bne #0x4ed7bc
004ed77c: add r3, r5, #9
004ed780: add r2, r5, #0xa
004ed784: ldrb r0, [r2, #1]
004ed788: ldrb r1, [r3, #-1]
004ed78c: cmp r2, r3
004ed790: eor r1, r0, r1
004ed794: strb r1, [r3, #-1]
004ed798: ldrb r0, [r2, #1]
004ed79c: eor r1, r1, r0
004ed7a0: strb r1, [r2, #1]
004ed7a4: ldrb r0, [r3, #-1]
004ed7a8: sub r2, r2, #1
004ed7ac: eor r1, r1, r0
004ed7b0: strb r1, [r3, #-1]
004ed7b4: add r3, r3, #1
004ed7b8: bhi #0x4ed784
004ed7bc: mov r0, r7
004ed7c0: add r1, r5, #0xc
004ed7c4: bl #0x3df1a0
004ed7c8: mov r3, #1
004ed7cc: cmp r3, #0
004ed7d0: str r3, [sp, #4]
004ed7d4: bne #0x4ed818
004ed7d8: add r3, r5, #0xd
004ed7dc: add r2, r5, #0xe
004ed7e0: ldrb r0, [r2, #1]
004ed7e4: ldrb r1, [r3, #-1]
004ed7e8: cmp r3, r2
004ed7ec: eor r1, r0, r1
004ed7f0: strb r1, [r3, #-1]
004ed7f4: ldrb r0, [r2, #1]
004ed7f8: eor r1, r1, r0
004ed7fc: strb r1, [r2, #1]
004ed800: ldrb r0, [r3, #-1]
004ed804: sub r2, r2, #1
004ed808: eor r1, r1, r0
004ed80c: strb r1, [r3, #-1]
004ed810: add r3, r3, #1
004ed814: blo #0x4ed7e0
004ed818: ldr r3, [r5, #0x10]
004ed81c: cmp r3, #0
004ed820: beq #0x4ed868
004ed824: ldr r2, [r3, #-4]
004ed828: mov r0, #0x30
004ed82c: mla r0, r0, r2, r3
004ed830: cmp r3, r0
004ed834: bne #0x4ed840
004ed838: b #0x4ed860
004ed83c: mov r0, r4
004ed840: sub r4, r0, #0x30
004ed844: ldr r3, [r0, #-0x30]
004ed848: mov r0, r4
004ed84c: mov lr, pc
004ed850: ldr pc, [r3]
004ed854: ldr r0, [r5, #0x10]
004ed858: cmp r0, r4
004ed85c: bne #0x4ed83c
004ed860: sub r0, r0, #8
004ed864: bl #0x310440
004ed868: ldr r4, [r5, #0xc]
004ed86c: mov r0, #6
004ed870: mov r1, #1
004ed874: mul r0, r0, r4
004ed878: add r0, r0, r1
004ed87c: lsl r0, r0, #3
004ed880: bl #0x31056c
004ed884: mov r3, #0x30
004ed888: cmp r4, #0
004ed88c: stm r0, {r3, r4}
004ed890: add r3, r0, #8
004ed894: beq #0x4ed8c4
004ed898: ldr r1, [pc, #0xd4]
004ed89c: mov r2, #0
004ed8a0: mov ip, r2
004ed8a4: ldr r1, [r6, r1]
004ed8a8: add r1, r1, #8
004ed8ac: add r2, r2, #1
004ed8b0: cmp r2, r4
004ed8b4: str r1, [r0, #8]
004ed8b8: str ip, [r0, #0x34]
004ed8bc: add r0, r0, #0x30
004ed8c0: bne #0x4ed8ac
004ed8c4: ldr r2, [r5, #0xc]
004ed8c8: str r3, [r5, #0x10]
004ed8cc: cmp r2, #0
004ed8d0: beq #0x4ed90c
004ed8d4: mov r4, #0
004ed8d8: mov r6, r4
004ed8dc: b #0x4ed8e4
004ed8e0: ldr r3, [r5, #0x10]
004ed8e4: add r0, r3, r4
004ed8e8: mov r1, r7
004ed8ec: ldr r3, [r3, r4]
004ed8f0: mov lr, pc
004ed8f4: ldr pc, [r3, #0xc]
004ed8f8: ldr r3, [r5, #0xc]
004ed8fc: add r6, r6, #1
004ed900: add r4, r4, #0x30
004ed904: cmp r3, r6
004ed908: bhi #0x4ed8e0
004ed90c: mov r0, r7
004ed910: add r1, r5, #0x14
004ed914: bl #0x459090
004ed918: mov r3, #1
004ed91c: cmp r3, #0
004ed920: str r3, [sp, #4]
004ed924: bne #0x4ed968
004ed928: add r3, r5, #0x16
004ed92c: add r5, r5, #0x15
004ed930: ldrb r1, [r3, #1]
004ed934: ldrb r2, [r5, #-1]
004ed938: cmp r3, r5
004ed93c: eor r2, r1, r2
004ed940: strb r2, [r5, #-1]
004ed944: ldrb r1, [r3, #1]
004ed948: eor r2, r2, r1
004ed94c: strb r2, [r3, #1]
004ed950: ldrb r1, [r5, #-1]
004ed954: sub r3, r3, #1
004ed958: eor r2, r2, r1
004ed95c: strb r2, [r5, #-1]
004ed960: add r5, r5, #1
004ed964: bhi #0x4ed930
004ed968: add sp, sp, #0xc
004ed96c: pop {r4, r5, r6, r7, pc}
004ed970: subeq r7, sl, ip, lsl r3
004ed974: andeq r0, r0, ip, lsl r7

_ZN7Structs6AnimFX4readEP11IStreamBase
00506990: push {r4, r5, r6, lr}
00506994: mov r4, r0
00506998: sub sp, sp, #8
0050699c: mov r0, r1
005069a0: mov r6, r1
005069a4: add r1, r4, #4
005069a8: bl #0x459090
005069ac: mov r3, #1
005069b0: cmp r3, #0
005069b4: str r3, [sp, #4]
005069b8: bne #0x5069fc
005069bc: add r3, r4, #5
005069c0: add r2, r4, #6
005069c4: ldrb r0, [r2, #1]
005069c8: ldrb r1, [r3, #-1]
005069cc: cmp r3, r2
005069d0: eor r1, r0, r1
005069d4: strb r1, [r3, #-1]
005069d8: ldrb r0, [r2, #1]
005069dc: eor r1, r1, r0
005069e0: strb r1, [r2, #1]
005069e4: ldrb r0, [r3, #-1]
005069e8: sub r2, r2, #1
005069ec: eor r1, r1, r0
005069f0: strb r1, [r3, #-1]
005069f4: add r3, r3, #1
005069f8: blo #0x5069c4
005069fc: add r1, r4, #8
00506a00: mov r0, r6
00506a04: bl #0x4db89c
00506a08: mov r0, r6
00506a0c: add r1, r4, #0xc
00506a10: bl #0x459090
00506a14: mov r3, #1
00506a18: cmp r3, #0
00506a1c: str r3, [sp, #4]
00506a20: bne #0x506a64
00506a24: add r3, r4, #0xd
00506a28: add r2, r4, #0xe
00506a2c: ldrb r0, [r2, #1]
00506a30: ldrb r1, [r3, #-1]
00506a34: cmp r3, r2
00506a38: eor r1, r0, r1
00506a3c: strb r1, [r3, #-1]
00506a40: ldrb r0, [r2, #1]
00506a44: eor r1, r1, r0
00506a48: strb r1, [r2, #1]
00506a4c: ldrb r0, [r3, #-1]
00506a50: sub r2, r2, #1
00506a54: eor r1, r1, r0
00506a58: strb r1, [r3, #-1]
00506a5c: add r3, r3, #1
00506a60: blo #0x506a2c
00506a64: add r1, r4, #0x10
00506a68: mov r0, r6
00506a6c: bl #0x4db89c
00506a70: mov r0, r6
00506a74: add r1, r4, #0x11
00506a78: bl #0x4db89c
00506a7c: mov r0, r6
00506a80: add r1, r4, #0x14
00506a84: bl #0x459090
00506a88: mov r3, #1
00506a8c: cmp r3, #0
00506a90: str r3, [sp, #4]
00506a94: bne #0x506ad8
00506a98: add r3, r4, #0x15
00506a9c: add r2, r4, #0x16
00506aa0: ldrb r0, [r2, #1]
00506aa4: ldrb r1, [r3, #-1]
00506aa8: cmp r3, r2
00506aac: eor r1, r0, r1
00506ab0: strb r1, [r3, #-1]
00506ab4: ldrb r0, [r2, #1]
00506ab8: eor r1, r1, r0
00506abc: strb r1, [r2, #1]
00506ac0: ldrb r0, [r3, #-1]
00506ac4: sub r2, r2, #1
00506ac8: eor r1, r1, r0
00506acc: strb r1, [r3, #-1]
00506ad0: add r3, r3, #1
00506ad4: blo #0x506aa0
00506ad8: mov r0, r6
00506adc: add r1, r4, #0x18
00506ae0: bl #0x459090
00506ae4: mov r3, #1
00506ae8: cmp r3, #0
00506aec: str r3, [sp, #4]
00506af0: bne #0x506b34
00506af4: add r3, r4, #0x19
00506af8: add r2, r4, #0x1a
00506afc: ldrb r0, [r2, #1]
00506b00: ldrb r1, [r3, #-1]
00506b04: cmp r2, r3
00506b08: eor r1, r0, r1
00506b0c: strb r1, [r3, #-1]
00506b10: ldrb r0, [r2, #1]
00506b14: eor r1, r1, r0
00506b18: strb r1, [r2, #1]
00506b1c: ldrb r0, [r3, #-1]
00506b20: sub r2, r2, #1
00506b24: eor r1, r1, r0
00506b28: strb r1, [r3, #-1]
00506b2c: add r3, r3, #1
00506b30: bhi #0x506afc
00506b34: mov r0, r6
00506b38: add r1, r4, #0x1c
00506b3c: bl #0x459090
00506b40: mov r3, #1
00506b44: cmp r3, #0
00506b48: str r3, [sp, #4]
00506b4c: bne #0x506b90
00506b50: add r3, r4, #0x1d
00506b54: add r2, r4, #0x1e
00506b58: ldrb r0, [r2, #1]
00506b5c: ldrb r1, [r3, #-1]
00506b60: cmp r2, r3
00506b64: eor r1, r0, r1
00506b68: strb r1, [r3, #-1]
00506b6c: ldrb r0, [r2, #1]
00506b70: eor r1, r1, r0
00506b74: strb r1, [r2, #1]
00506b78: ldrb r0, [r3, #-1]
00506b7c: sub r2, r2, #1
00506b80: eor r1, r1, r0
00506b84: strb r1, [r3, #-1]
00506b88: add r3, r3, #1
00506b8c: bhi #0x506b58
00506b90: add r1, r4, #0x20
00506b94: mov r0, r6
00506b98: bl #0x4db89c
00506b9c: mov r0, r6
00506ba0: add r1, r4, #0x21
00506ba4: bl #0x4db89c
00506ba8: mov r0, r6
00506bac: add r1, r4, #0x24
00506bb0: bl #0x4db94c
00506bb4: mov r3, #1
00506bb8: cmp r3, #0
00506bbc: str r3, [sp, #4]
00506bc0: bne #0x506c04
00506bc4: add r3, r4, #0x25
00506bc8: add r2, r4, #0x26
00506bcc: ldrb r0, [r2, #1]
00506bd0: ldrb r1, [r3, #-1]
00506bd4: cmp r2, r3
00506bd8: eor r1, r0, r1
00506bdc: strb r1, [r3, #-1]
00506be0: ldrb r0, [r2, #1]
00506be4: eor r1, r1, r0
00506be8: strb r1, [r2, #1]
00506bec: ldrb r0, [r3, #-1]
00506bf0: sub r2, r2, #1
00506bf4: eor r1, r1, r0
00506bf8: strb r1, [r3, #-1]
00506bfc: add r3, r3, #1
00506c00: bhi #0x506bcc
00506c04: mov r0, r6
00506c08: add r1, r4, #0x28
00506c0c: bl #0x3df1a0
00506c10: mov r3, #1
00506c14: cmp r3, #0
00506c18: str r3, [sp, #4]
00506c1c: bne #0x506c60
00506c20: add r3, r4, #0x29
00506c24: add r2, r4, #0x2a
00506c28: ldrb r0, [r2, #1]
00506c2c: ldrb r1, [r3, #-1]
00506c30: cmp r2, r3
00506c34: eor r1, r0, r1
00506c38: strb r1, [r3, #-1]
00506c3c: ldrb r0, [r2, #1]
00506c40: eor r1, r1, r0
00506c44: strb r1, [r2, #1]
00506c48: ldrb r0, [r3, #-1]
00506c4c: sub r2, r2, #1
00506c50: eor r1, r1, r0
00506c54: strb r1, [r3, #-1]
00506c58: add r3, r3, #1
00506c5c: bhi #0x506c28
00506c60: ldr r0, [r4, #0x2c]
00506c64: cmp r0, #0
00506c68: beq #0x506c70
00506c6c: bl #0x310440
00506c70: ldr r0, [r4, #0x28]
00506c74: mov r1, #1
00506c78: mov r5, #0
00506c7c: add r0, r0, r1
00506c80: bl #0x31056c
00506c84: ldr r2, [r4, #0x28]
00506c88: mov r1, r0
00506c8c: str r0, [r4, #0x2c]
00506c90: mov r3, r5
00506c94: mov r0, r6
00506c98: bl #0x317454
00506c9c: ldr r3, [r4, #0x28]
00506ca0: ldr r2, [r4, #0x2c]
00506ca4: strb r5, [r2, r3]
00506ca8: add sp, sp, #8
00506cac: pop {r4, r5, r6, pc}

_ZN6Arrays15CharEffectTable4readEP11IStreamBase
004bc3c0: push {r4, r5, r6, r7, r8, sl, lr}
004bc3c4: sub sp, sp, #0xc
004bc3c8: mov sl, r0
004bc3cc: bl #0x313a90
004bc3d0: ldr r6, [pc, #0x11c]
004bc3d4: mov r3, #1
004bc3d8: cmp r3, #0
004bc3dc: str r0, [sp, #4]
004bc3e0: str r3, [sp]
004bc3e4: add r6, pc, r6
004bc3e8: bne #0x4bc430
004bc3ec: add r3, sp, #4
004bc3f0: add r2, r3, #2
004bc3f4: add r3, r3, #1
004bc3f8: ldrb r0, [r2, #1]
004bc3fc: ldrb r1, [r3, #-1]
004bc400: cmp r2, r3
004bc404: eor r1, r0, r1
004bc408: strb r1, [r3, #-1]
004bc40c: ldrb r0, [r2, #1]
004bc410: eor r1, r1, r0
004bc414: strb r1, [r2, #1]
004bc418: ldrb r0, [r3, #-1]
004bc41c: sub r2, r2, #1
004bc420: eor r1, r1, r0
004bc424: strb r1, [r3, #-1]
004bc428: add r3, r3, #1
004bc42c: bhi #0x4bc3f8
004bc430: ldr r7, [pc, #0xc0]
004bc434: bl #0x4a87c4
004bc438: ldr r4, [sp, #4]
004bc43c: ldr r3, [r6, r7]
004bc440: mov r1, #1
004bc444: add r0, r4, r4, lsl #1
004bc448: add r0, r0, r1
004bc44c: str r4, [r3]
004bc450: lsl r0, r0, #3
004bc454: bl #0x31056c
004bc458: mov r3, #0x18
004bc45c: cmp r4, #0
004bc460: stm r0, {r3, r4}
004bc464: add r3, r0, #8
004bc468: beq #0x4bc490
004bc46c: ldr r1, [pc, #0x88]
004bc470: mov r2, #0
004bc474: ldr r1, [r6, r1]
004bc478: add r1, r1, #8
004bc47c: add r2, r2, #1
004bc480: cmp r2, r4
004bc484: str r1, [r0, #8]
004bc488: add r0, r0, #0x18
004bc48c: bne #0x4bc47c
004bc490: ldr r2, [r6, r7]
004bc494: ldr r8, [pc, #0x64]
004bc498: ldr r1, [r2]
004bc49c: ldr r2, [r6, r8]
004bc4a0: cmp r1, #0
004bc4a4: str r3, [r2]
004bc4a8: beq #0x4bc4ec
004bc4ac: mov r4, #0
004bc4b0: mov r5, r4
004bc4b4: b #0x4bc4c0
004bc4b8: ldr r3, [r6, r8]
004bc4bc: ldr r3, [r3]
004bc4c0: add r0, r3, r4
004bc4c4: mov r1, sl
004bc4c8: ldr r3, [r3, r4]
004bc4cc: mov lr, pc
004bc4d0: ldr pc, [r3, #0xc]
004bc4d4: ldr r3, [r6, r7]
004bc4d8: add r5, r5, #1
004bc4dc: add r4, r4, #0x18
004bc4e0: ldr r3, [r3]
004bc4e4: cmp r3, r5
004bc4e8: bhi #0x4bc4b8
004bc4ec: add sp, sp, #0xc
004bc4f0: pop {r4, r5, r6, r7, r8, sl, pc}
004bc4f4: subeq r8, sp, ip, lsr #13
004bc4f8: andeq r1, r0, ip, ror #3
004bc4fc: andeq r2, r0, r0, asr ip
004bc500: andeq r0, r0, r4, asr #30

_ZN7Structs10CharEffect4readEP11IStreamBase
004ed5a8: push {r4, r5, lr}
004ed5ac: mov r4, r0
004ed5b0: sub sp, sp, #0xc
004ed5b4: mov r0, r1
004ed5b8: mov r5, r1
004ed5bc: add r1, r4, #4
004ed5c0: bl #0x459090
004ed5c4: mov r3, #1
004ed5c8: cmp r3, #0
004ed5cc: str r3, [sp, #4]
004ed5d0: bne #0x4ed614
004ed5d4: add r3, r4, #5
004ed5d8: add r2, r4, #6
004ed5dc: ldrb r0, [r2, #1]
004ed5e0: ldrb r1, [r3, #-1]
004ed5e4: cmp r3, r2
004ed5e8: eor r1, r0, r1
004ed5ec: strb r1, [r3, #-1]
004ed5f0: ldrb r0, [r2, #1]
004ed5f4: eor r1, r1, r0
004ed5f8: strb r1, [r2, #1]
004ed5fc: ldrb r0, [r3, #-1]
004ed600: sub r2, r2, #1
004ed604: eor r1, r1, r0
004ed608: strb r1, [r3, #-1]
004ed60c: add r3, r3, #1
004ed610: blo #0x4ed5dc
004ed614: mov r0, r5
004ed618: add r1, r4, #8
004ed61c: bl #0x459090
004ed620: mov r3, #1
004ed624: cmp r3, #0
004ed628: str r3, [sp, #4]
004ed62c: bne #0x4ed670
004ed630: add r3, r4, #9
004ed634: add r2, r4, #0xa
004ed638: ldrb r0, [r2, #1]
004ed63c: ldrb r1, [r3, #-1]
004ed640: cmp r2, r3
004ed644: eor r1, r0, r1
004ed648: strb r1, [r3, #-1]
004ed64c: ldrb r0, [r2, #1]
004ed650: eor r1, r1, r0
004ed654: strb r1, [r2, #1]
004ed658: ldrb r0, [r3, #-1]
004ed65c: sub r2, r2, #1
004ed660: eor r1, r1, r0
004ed664: strb r1, [r3, #-1]
004ed668: add r3, r3, #1
004ed66c: bhi #0x4ed638
004ed670: mov r0, r5
004ed674: add r1, r4, #0xc
004ed678: bl #0x459090
004ed67c: mov r3, #1
004ed680: cmp r3, #0
004ed684: str r3, [sp, #4]
004ed688: bne #0x4ed6cc
004ed68c: add r3, r4, #0xd
004ed690: add r2, r4, #0xe
004ed694: ldrb r0, [r2, #1]
004ed698: ldrb r1, [r3, #-1]
004ed69c: cmp r3, r2
004ed6a0: eor r1, r0, r1
004ed6a4: strb r1, [r3, #-1]
004ed6a8: ldrb r0, [r2, #1]
004ed6ac: eor r1, r1, r0
004ed6b0: strb r1, [r2, #1]
004ed6b4: ldrb r0, [r3, #-1]
004ed6b8: sub r2, r2, #1
004ed6bc: eor r1, r1, r0
004ed6c0: strb r1, [r3, #-1]
004ed6c4: add r3, r3, #1
004ed6c8: blo #0x4ed694
004ed6cc: mov r0, r5
004ed6d0: add r1, r4, #0x10
004ed6d4: bl #0x459090
004ed6d8: mov r3, #1
004ed6dc: cmp r3, #0
004ed6e0: str r3, [sp, #4]
004ed6e4: bne #0x4ed728
004ed6e8: add r3, r4, #0x11
004ed6ec: add r2, r4, #0x12
004ed6f0: ldrb r0, [r2, #1]
004ed6f4: ldrb r1, [r3, #-1]
004ed6f8: cmp r3, r2
004ed6fc: eor r1, r0, r1
004ed700: strb r1, [r3, #-1]
004ed704: ldrb r0, [r2, #1]
004ed708: eor r1, r1, r0
004ed70c: strb r1, [r2, #1]
004ed710: ldrb r0, [r3, #-1]
004ed714: sub r2, r2, #1
004ed718: eor r1, r1, r0
004ed71c: strb r1, [r3, #-1]
004ed720: add r3, r3, #1
004ed724: blo #0x4ed6f0
004ed728: mov r0, r5
004ed72c: add r1, r4, #0x14
004ed730: bl #0x4db89c
004ed734: add sp, sp, #0xc
004ed738: pop {r4, r5, pc}

_ZN6Arrays19FootstepEffectTable4readEP11IStreamBase
004bc278: push {r4, r5, r6, r7, r8, lr}
004bc27c: sub sp, sp, #8
004bc280: mov r8, r0
004bc284: bl #0x313a90
004bc288: ldr r5, [pc, #0x120]
004bc28c: mov r3, #1
004bc290: cmp r3, #0
004bc294: str r0, [sp, #4]
004bc298: str r3, [sp]
004bc29c: add r5, pc, r5
004bc2a0: bne #0x4bc2e8
004bc2a4: add r3, sp, #4
004bc2a8: add r2, r3, #2
004bc2ac: add r3, r3, #1
004bc2b0: ldrb r0, [r2, #1]
004bc2b4: ldrb r1, [r3, #-1]
004bc2b8: cmp r2, r3
004bc2bc: eor r1, r0, r1
004bc2c0: strb r1, [r3, #-1]
004bc2c4: ldrb r0, [r2, #1]
004bc2c8: eor r1, r1, r0
004bc2cc: strb r1, [r2, #1]
004bc2d0: ldrb r0, [r3, #-1]
004bc2d4: sub r2, r2, #1
004bc2d8: eor r1, r1, r0
004bc2dc: strb r1, [r3, #-1]
004bc2e0: add r3, r3, #1
004bc2e4: bhi #0x4bc2b0
004bc2e8: ldr r6, [pc, #0xc4]
004bc2ec: bl #0x4a8650
004bc2f0: ldr r4, [sp, #4]
004bc2f4: ldr r3, [r5, r6]
004bc2f8: mov r1, #1
004bc2fc: lsl r0, r4, #5
004bc300: str r4, [r3]
004bc304: add r0, r0, #8
004bc308: bl #0x31056c
004bc30c: mov r3, #0x20
004bc310: cmp r4, #0
004bc314: stm r0, {r3, r4}
004bc318: add r3, r0, #8
004bc31c: beq #0x4bc354
004bc320: ldr r1, [pc, #0x90]
004bc324: mov r2, #0
004bc328: ldr ip, [r5, r1]
004bc32c: mov r1, r2
004bc330: add ip, ip, #8
004bc334: add r2, r2, #1
004bc338: cmp r2, r4
004bc33c: str ip, [r0, #8]
004bc340: str r1, [r0, #0x14]
004bc344: str r1, [r0, #0x1c]
004bc348: str r1, [r0, #0x24]
004bc34c: add r0, r0, #0x20
004bc350: bne #0x4bc334
004bc354: ldr r2, [r5, r6]
004bc358: ldr r7, [pc, #0x5c]
004bc35c: ldr r1, [r2]
004bc360: ldr r2, [r5, r7]
004bc364: cmp r1, #0
004bc368: str r3, [r2]
004bc36c: beq #0x4bc3a8
004bc370: mov r4, #0
004bc374: b #0x4bc380
004bc378: ldr r3, [r5, r7]
004bc37c: ldr r3, [r3]
004bc380: add r0, r3, r4, lsl #5
004bc384: mov r1, r8
004bc388: ldr r3, [r3, r4, lsl #5]
004bc38c: mov lr, pc
004bc390: ldr pc, [r3, #0xc]
004bc394: ldr r3, [r5, r6]
004bc398: add r4, r4, #1
004bc39c: ldr r3, [r3]
004bc3a0: cmp r3, r4
004bc3a4: bhi #0x4bc378
004bc3a8: add sp, sp, #8
004bc3ac: pop {r4, r5, r6, r7, r8, pc}
004bc3b0: strdeq r8, sb, [sp], #-0x74
004bc3b4: andeq r1, r0, ip, lsr #23
004bc3b8: strdeq r1, r2, [r0], -r0
004bc3bc: strheq r3, [r0], -r4

_ZN7Structs14FootstepEffect4readEP11IStreamBase
00506660: push {r4, r5, r6, r7, r8, lr}
00506664: mov r4, r0
00506668: sub sp, sp, #8
0050666c: mov r0, r1
00506670: mov r5, r1
00506674: add r1, r4, #4
00506678: bl #0x459090
0050667c: mov r3, #1
00506680: cmp r3, #0
00506684: str r3, [sp, #4]
00506688: bne #0x5066cc
0050668c: add r3, r4, #5
00506690: add r2, r4, #6
00506694: ldrb r0, [r2, #1]
00506698: ldrb r1, [r3, #-1]
0050669c: cmp r3, r2
005066a0: eor r1, r0, r1
005066a4: strb r1, [r3, #-1]
005066a8: ldrb r0, [r2, #1]
005066ac: eor r1, r1, r0
005066b0: strb r1, [r2, #1]
005066b4: ldrb r0, [r3, #-1]
005066b8: sub r2, r2, #1
005066bc: eor r1, r1, r0
005066c0: strb r1, [r3, #-1]
005066c4: add r3, r3, #1
005066c8: blo #0x506694
005066cc: mov r0, r5
005066d0: add r1, r4, #8
005066d4: bl #0x3df1a0
005066d8: mov r3, #1
005066dc: cmp r3, #0
005066e0: str r3, [sp, #4]
005066e4: bne #0x506728
005066e8: add r3, r4, #9
005066ec: add r2, r4, #0xa
005066f0: ldrb r0, [r2, #1]
005066f4: ldrb r1, [r3, #-1]
005066f8: cmp r3, r2
005066fc: eor r1, r0, r1
00506700: strb r1, [r3, #-1]
00506704: ldrb r0, [r2, #1]
00506708: eor r1, r1, r0
0050670c: strb r1, [r2, #1]
00506710: ldrb r0, [r3, #-1]
00506714: sub r2, r2, #1
00506718: eor r1, r1, r0
0050671c: strb r1, [r3, #-1]
00506720: add r3, r3, #1
00506724: blo #0x5066f0
00506728: ldr r0, [r4, #0xc]
0050672c: cmp r0, #0
00506730: beq #0x506738
00506734: bl #0x310440
00506738: ldr r0, [r4, #8]
0050673c: mov r1, #1
00506740: mov r6, #0
00506744: add r0, r0, r1
00506748: bl #0x31056c
0050674c: ldr r2, [r4, #8]
00506750: mov r1, r0
00506754: str r0, [r4, #0xc]
00506758: mov r3, r6
0050675c: mov r0, r5
00506760: bl #0x317454
00506764: ldr r3, [r4, #8]
00506768: ldr r2, [r4, #0xc]
0050676c: mov r0, r5
00506770: add r1, r4, #0x10
00506774: strb r6, [r2, r3]
00506778: bl #0x3df1a0
0050677c: mov r3, #1
00506780: cmp r3, r6
00506784: str r3, [sp, #4]
00506788: bne #0x5067cc
0050678c: add r3, r4, #0x11
00506790: add r2, r4, #0x12
00506794: ldrb r0, [r2, #1]
00506798: ldrb r1, [r3, #-1]
0050679c: cmp r3, r2
005067a0: eor r1, r0, r1
005067a4: strb r1, [r3, #-1]
005067a8: ldrb r0, [r2, #1]
005067ac: eor r1, r1, r0
005067b0: strb r1, [r2, #1]
005067b4: ldrb r0, [r3, #-1]
005067b8: sub r2, r2, #1
005067bc: eor r1, r1, r0
005067c0: strb r1, [r3, #-1]
005067c4: add r3, r3, #1
005067c8: blo #0x506794
005067cc: ldr r0, [r4, #0x14]
005067d0: cmp r0, #0
005067d4: beq #0x5067dc
005067d8: bl #0x310440
005067dc: ldr r0, [r4, #0x10]
005067e0: mov r1, #1
005067e4: lsl r0, r0, #2
005067e8: bl #0x31056c
005067ec: ldr r3, [r4, #0x10]
005067f0: str r0, [r4, #0x14]
005067f4: cmp r3, #0
005067f8: beq #0x50687c
005067fc: mov r6, #0
00506800: mov r8, #1
00506804: lsl r7, r6, #2
00506808: add r1, r0, r7
0050680c: mov r0, r5
00506810: bl #0x459090
00506814: str r8, [sp, #4]
00506818: cmp r8, #0
0050681c: ldr r3, [r4, #0x14]
00506820: bne #0x506868
00506824: add r7, r3, r7
00506828: add r3, r7, #2
0050682c: add r7, r7, #1
00506830: ldrb r1, [r3, #1]
00506834: ldrb r2, [r7, #-1]
00506838: cmp r7, r3
0050683c: eor r2, r1, r2
00506840: strb r2, [r7, #-1]
00506844: ldrb r1, [r3, #1]
00506848: eor r2, r2, r1
0050684c: strb r2, [r3, #1]
00506850: ldrb r1, [r7, #-1]
00506854: sub r3, r3, #1
00506858: eor r2, r2, r1
0050685c: strb r2, [r7, #-1]
00506860: add r7, r7, #1
00506864: blo #0x506830
00506868: ldr r3, [r4, #0x10]
0050686c: add r6, r6, #1
00506870: cmp r3, r6
00506874: ldrhi r0, [r4, #0x14]
00506878: bhi #0x506804
0050687c: mov r0, r5
00506880: add r1, r4, #0x18
00506884: bl #0x3df1a0
00506888: mov r3, #1
0050688c: cmp r3, #0
00506890: str r3, [sp, #4]
00506894: bne #0x5068d8
00506898: add r3, r4, #0x19
0050689c: add r2, r4, #0x1a
005068a0: ldrb r0, [r2, #1]
005068a4: ldrb r1, [r3, #-1]
005068a8: cmp r3, r2
005068ac: eor r1, r0, r1
005068b0: strb r1, [r3, #-1]
005068b4: ldrb r0, [r2, #1]
005068b8: eor r1, r1, r0
005068bc: strb r1, [r2, #1]
005068c0: ldrb r0, [r3, #-1]
005068c4: sub r2, r2, #1
005068c8: eor r1, r1, r0
005068cc: strb r1, [r3, #-1]
005068d0: add r3, r3, #1
005068d4: blo #0x5068a0
005068d8: ldr r0, [r4, #0x1c]
005068dc: cmp r0, #0
005068e0: beq #0x5068e8
005068e4: bl #0x310440
005068e8: ldr r0, [r4, #0x18]
005068ec: mov r1, #1
005068f0: lsl r0, r0, #2
005068f4: bl #0x31056c
005068f8: ldr r3, [r4, #0x18]
005068fc: str r0, [r4, #0x1c]
00506900: cmp r3, #0
00506904: beq #0x506988
00506908: mov r6, #0
0050690c: mov r8, #1
00506910: lsl r7, r6, #2
00506914: add r1, r0, r7
00506918: mov r0, r5
0050691c: bl #0x459090
00506920: str r8, [sp, #4]
00506924: cmp r8, #0
00506928: ldr r3, [r4, #0x1c]
0050692c: bne #0x506974
00506930: add r7, r3, r7
00506934: add r3, r7, #2
00506938: add r7, r7, #1
0050693c: ldrb r1, [r3, #1]
00506940: ldrb r2, [r7, #-1]
00506944: cmp r7, r3
00506948: eor r2, r1, r2
0050694c: strb r2, [r7, #-1]
00506950: ldrb r1, [r3, #1]
00506954: eor r2, r2, r1
00506958: strb r2, [r3, #1]
0050695c: ldrb r1, [r7, #-1]
00506960: sub r3, r3, #1
00506964: eor r2, r2, r1
00506968: strb r2, [r7, #-1]
0050696c: add r7, r7, #1
00506970: blo #0x50693c
00506974: ldr r3, [r4, #0x18]
00506978: add r6, r6, #1
0050697c: cmp r3, r6
00506980: ldrhi r0, [r4, #0x1c]
00506984: bhi #0x506910
00506988: add sp, sp, #8
0050698c: pop {r4, r5, r6, r7, r8, pc}

_ZN6Arrays10EffectDict4readEP11IStreamBase
004b8458: push {r4, r5, r6, r7, r8, sl, lr}
004b845c: sub sp, sp, #0xc
004b8460: mov sl, r0
004b8464: bl #0x313a90
004b8468: ldr r6, [pc, #0x124]
004b846c: mov r3, #1
004b8470: cmp r3, #0
004b8474: str r0, [sp, #4]
004b8478: str r3, [sp]
004b847c: add r6, pc, r6
004b8480: bne #0x4b84c8
004b8484: add r3, sp, #4
004b8488: add r2, r3, #2
004b848c: add r3, r3, #1
004b8490: ldrb r0, [r2, #1]
004b8494: ldrb r1, [r3, #-1]
004b8498: cmp r2, r3
004b849c: eor r1, r0, r1
004b84a0: strb r1, [r3, #-1]
004b84a4: ldrb r0, [r2, #1]
004b84a8: eor r1, r1, r0
004b84ac: strb r1, [r2, #1]
004b84b0: ldrb r0, [r3, #-1]
004b84b4: sub r2, r2, #1
004b84b8: eor r1, r1, r0
004b84bc: strb r1, [r3, #-1]
004b84c0: add r3, r3, #1
004b84c4: bhi #0x4b8490
004b84c8: bl #0x4a3ea4
004b84cc: ldr r7, [pc, #0xc4]
004b84d0: ldr r4, [sp, #4]
004b84d4: mov r5, #0xc
004b84d8: ldr r3, [r6, r7]
004b84dc: mul r0, r5, r4
004b84e0: str r4, [r3]
004b84e4: add r0, r0, #8
004b84e8: mov r1, #1
004b84ec: bl #0x31056c
004b84f0: cmp r4, #0
004b84f4: str r5, [r0]
004b84f8: str r4, [r0, #4]
004b84fc: add r3, r0, #8
004b8500: beq #0x4b8530
004b8504: ldr r1, [pc, #0x90]
004b8508: mov r2, #0
004b850c: mov ip, r2
004b8510: ldr r1, [r6, r1]
004b8514: add r1, r1, #8
004b8518: add r2, r2, #1
004b851c: cmp r2, r4
004b8520: str r1, [r0, #8]
004b8524: str ip, [r0, #0x10]
004b8528: add r0, r0, #0xc
004b852c: bne #0x4b8518
004b8530: ldr r2, [r6, r7]
004b8534: ldr r8, [pc, #0x64]
004b8538: ldr r1, [r2]
004b853c: ldr r2, [r6, r8]
004b8540: cmp r1, #0
004b8544: str r3, [r2]
004b8548: beq #0x4b858c
004b854c: mov r4, #0
004b8550: mov r5, r4
004b8554: b #0x4b8560
004b8558: ldr r3, [r6, r8]
004b855c: ldr r3, [r3]
004b8560: add r0, r3, r4
004b8564: mov r1, sl
004b8568: ldr r3, [r3, r4]
004b856c: mov lr, pc
004b8570: ldr pc, [r3, #0xc]
004b8574: ldr r3, [r6, r7]
004b8578: add r5, r5, #1
004b857c: add r4, r4, #0xc
004b8580: ldr r3, [r3]
004b8584: cmp r3, r5
004b8588: bhi #0x4b8558
004b858c: add sp, sp, #0xc
004b8590: pop {r4, r5, r6, r7, r8, sl, pc}
004b8594: subeq ip, sp, r4, lsl r6
004b8598: andeq r0, r0, r8, lsl #23
004b859c: strheq r0, [r0], -ip
004b85a0: strheq r1, [r0], -r8

_ZN7Structs11ColladaFile4readEP11IStreamBase
004dc424: push {r4, r5, r6, lr}
004dc428: mov r4, r0
004dc42c: sub sp, sp, #8
004dc430: mov r0, r1
004dc434: mov r6, r1
004dc438: add r1, r4, #4
004dc43c: bl #0x3df1a0
004dc440: mov r3, #1
004dc444: cmp r3, #0
004dc448: str r3, [sp, #4]
004dc44c: bne #0x4dc490
004dc450: add r3, r4, #5
004dc454: add r2, r4, #6
004dc458: ldrb r0, [r2, #1]
004dc45c: ldrb r1, [r3, #-1]
004dc460: cmp r3, r2
004dc464: eor r1, r0, r1
004dc468: strb r1, [r3, #-1]
004dc46c: ldrb r0, [r2, #1]
004dc470: eor r1, r1, r0
004dc474: strb r1, [r2, #1]
004dc478: ldrb r0, [r3, #-1]
004dc47c: sub r2, r2, #1
004dc480: eor r1, r1, r0
004dc484: strb r1, [r3, #-1]
004dc488: add r3, r3, #1
004dc48c: blo #0x4dc458
004dc490: ldr r0, [r4, #8]
004dc494: cmp r0, #0
004dc498: beq #0x4dc4a0
004dc49c: bl #0x310440
004dc4a0: ldr r0, [r4, #4]
004dc4a4: mov r1, #1
004dc4a8: mov r5, #0
004dc4ac: add r0, r0, r1
004dc4b0: bl #0x31056c
004dc4b4: ldr r2, [r4, #4]
004dc4b8: mov r1, r0
004dc4bc: str r0, [r4, #8]
004dc4c0: mov r3, r5
004dc4c4: mov r0, r6
004dc4c8: bl #0x317454
004dc4cc: ldr r3, [r4, #4]
004dc4d0: ldr r2, [r4, #8]
004dc4d4: strb r5, [r2, r3]
004dc4d8: add sp, sp, #8
004dc4dc: pop {r4, r5, r6, pc}
