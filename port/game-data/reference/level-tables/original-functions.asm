
# _ZN6Arrays14FastTravelList4readEP11IStreamBase
004ba8e4: push {r4, r5, r6, r7, r8, sl, lr}
004ba8e8: sub sp, sp, #0xc
004ba8ec: mov sl, r0
004ba8f0: bl #0x313a90
004ba8f4: ldr r6, [pc, #0x124]
004ba8f8: mov r3, #1
004ba8fc: cmp r3, #0
004ba900: str r0, [sp, #4]
004ba904: str r3, [sp]
004ba908: add r6, pc, r6
004ba90c: bne #0x4ba954
004ba910: add r3, sp, #4
004ba914: add r2, r3, #2
004ba918: add r3, r3, #1
004ba91c: ldrb r0, [r2, #1]
004ba920: ldrb r1, [r3, #-1]
004ba924: cmp r2, r3
004ba928: eor r1, r0, r1
004ba92c: strb r1, [r3, #-1]
004ba930: ldrb r0, [r2, #1]
004ba934: eor r1, r1, r0
004ba938: strb r1, [r2, #1]
004ba93c: ldrb r0, [r3, #-1]
004ba940: sub r2, r2, #1
004ba944: eor r1, r1, r0
004ba948: strb r1, [r3, #-1]
004ba94c: add r3, r3, #1
004ba950: bhi #0x4ba91c
004ba954: bl #0x4a6874
004ba958: ldr r7, [pc, #0xc4]
004ba95c: ldr r4, [sp, #4]
004ba960: mov r5, #0x1c
004ba964: ldr r3, [r6, r7]
004ba968: mul r0, r5, r4
004ba96c: str r4, [r3]
004ba970: add r0, r0, #8
004ba974: mov r1, #1
004ba978: bl #0x31056c
004ba97c: cmp r4, #0
004ba980: str r5, [r0]
004ba984: str r4, [r0, #4]
004ba988: add r3, r0, #8
004ba98c: beq #0x4ba9bc
004ba990: ldr r1, [pc, #0x90]
004ba994: mov r2, #0
004ba998: mov ip, r2
004ba99c: ldr r1, [r6, r1]
004ba9a0: add r1, r1, #8
004ba9a4: add r2, r2, #1
004ba9a8: cmp r2, r4
004ba9ac: str r1, [r0, #8]
004ba9b0: str ip, [r0, #0x18]
004ba9b4: add r0, r0, #0x1c
004ba9b8: bne #0x4ba9a4
004ba9bc: ldr r2, [r6, r7]
004ba9c0: ldr r8, [pc, #0x64]
004ba9c4: ldr r1, [r2]
004ba9c8: ldr r2, [r6, r8]
004ba9cc: cmp r1, #0
004ba9d0: str r3, [r2]
004ba9d4: beq #0x4baa18
004ba9d8: mov r4, #0
004ba9dc: mov r5, r4
004ba9e0: b #0x4ba9ec
004ba9e4: ldr r3, [r6, r8]
004ba9e8: ldr r3, [r3]
004ba9ec: add r0, r3, r4
004ba9f0: mov r1, sl
004ba9f4: ldr r3, [r3, r4]
004ba9f8: mov lr, pc
004ba9fc: ldr pc, [r3, #0xc]
004baa00: ldr r3, [r6, r7]
004baa04: add r5, r5, #1
004baa08: add r4, r4, #0x1c
004baa0c: ldr r3, [r3]
004baa10: cmp r3, r5
004baa14: bhi #0x4ba9e4
004baa18: add sp, sp, #0xc
004baa1c: pop {r4, r5, r6, r7, r8, sl, pc}
004baa20: subeq sl, sp, r8, lsl #3
004baa24: strdeq r4, r5, [r0], -r4
004baa28: andeq r1, r0, r8, ror r0
004baa2c: andeq r4, r0, ip, ror sb

# _ZN6Arrays9LevelList4readEP11IStreamBase
004ba794: push {r4, r5, r6, r7, r8, sl, lr}
004ba798: sub sp, sp, #0xc
004ba79c: mov sl, r0
004ba7a0: bl #0x313a90
004ba7a4: ldr r6, [pc, #0x128]
004ba7a8: mov r3, #1
004ba7ac: cmp r3, #0
004ba7b0: str r0, [sp, #4]
004ba7b4: str r3, [sp]
004ba7b8: add r6, pc, r6
004ba7bc: bne #0x4ba804
004ba7c0: add r3, sp, #4
004ba7c4: add r2, r3, #2
004ba7c8: add r3, r3, #1
004ba7cc: ldrb r0, [r2, #1]
004ba7d0: ldrb r1, [r3, #-1]
004ba7d4: cmp r2, r3
004ba7d8: eor r1, r0, r1
004ba7dc: strb r1, [r3, #-1]
004ba7e0: ldrb r0, [r2, #1]
004ba7e4: eor r1, r1, r0
004ba7e8: strb r1, [r2, #1]
004ba7ec: ldrb r0, [r3, #-1]
004ba7f0: sub r2, r2, #1
004ba7f4: eor r1, r1, r0
004ba7f8: strb r1, [r3, #-1]
004ba7fc: add r3, r3, #1
004ba800: bhi #0x4ba7cc
004ba804: ldr r7, [pc, #0xcc]
004ba808: bl #0x4a66f4
004ba80c: ldr r4, [sp, #4]
004ba810: ldr r3, [r6, r7]
004ba814: mov r1, #1
004ba818: add r0, r4, r4, lsl #3
004ba81c: add r0, r0, r1
004ba820: str r4, [r3]
004ba824: lsl r0, r0, #3
004ba828: bl #0x31056c
004ba82c: mov r3, #0x48
004ba830: cmp r4, #0
004ba834: stm r0, {r3, r4}
004ba838: add r3, r0, #8
004ba83c: beq #0x4ba870
004ba840: ldr r1, [pc, #0x94]
004ba844: mov r2, #0
004ba848: ldr ip, [r6, r1]
004ba84c: mov r1, r2
004ba850: add ip, ip, #8
004ba854: add r2, r2, #1
004ba858: cmp r2, r4
004ba85c: str ip, [r0, #8]
004ba860: str r1, [r0, #0x14]
004ba864: str r1, [r0, #0x28]
004ba868: add r0, r0, #0x48
004ba86c: bne #0x4ba854
004ba870: ldr r2, [r6, r7]
004ba874: ldr r8, [pc, #0x64]
004ba878: ldr r1, [r2]
004ba87c: ldr r2, [r6, r8]
004ba880: cmp r1, #0
004ba884: str r3, [r2]
004ba888: beq #0x4ba8cc
004ba88c: mov r4, #0
004ba890: mov r5, r4
004ba894: b #0x4ba8a0
004ba898: ldr r3, [r6, r8]
004ba89c: ldr r3, [r3]
004ba8a0: add r0, r3, r4
004ba8a4: mov r1, sl
004ba8a8: ldr r3, [r3, r4]
004ba8ac: mov lr, pc
004ba8b0: ldr pc, [r3, #0xc]
004ba8b4: ldr r3, [r6, r7]
004ba8b8: add r5, r5, #1
004ba8bc: add r4, r4, #0x48
004ba8c0: ldr r3, [r3]
004ba8c4: cmp r3, r5
004ba8c8: bhi #0x4ba898
004ba8cc: add sp, sp, #0xc
004ba8d0: pop {r4, r5, r6, r7, r8, sl, pc}
004ba8d4: ldrdeq sl, fp, [sp], #-0x28
004ba8d8: andeq r1, r0, r0, asr #17
004ba8dc: andeq r3, r0, r8, lsr r4
004ba8e0: andeq r0, r0, r4, ror r8

# _ZN7Structs21FastTravelDestination4readEP11IStreamBase
004fcff0: push {r4, r5, r6, lr}
004fcff4: mov r4, r0
004fcff8: sub sp, sp, #8
004fcffc: mov r0, r1
004fd000: mov r5, r1
004fd004: add r1, r4, #4
004fd008: bl #0x459090
004fd00c: mov r3, #1
004fd010: cmp r3, #0
004fd014: str r3, [sp, #4]
004fd018: bne #0x4fd05c
004fd01c: add r3, r4, #5
004fd020: add r2, r4, #6
004fd024: ldrb r0, [r2, #1]
004fd028: ldrb r1, [r3, #-1]
004fd02c: cmp r2, r3
004fd030: eor r1, r0, r1
004fd034: strb r1, [r3, #-1]
004fd038: ldrb r0, [r2, #1]
004fd03c: eor r1, r1, r0
004fd040: strb r1, [r2, #1]
004fd044: ldrb r0, [r3, #-1]
004fd048: sub r2, r2, #1
004fd04c: eor r1, r1, r0
004fd050: strb r1, [r3, #-1]
004fd054: add r3, r3, #1
004fd058: bhi #0x4fd024
004fd05c: mov r0, r5
004fd060: add r1, r4, #8
004fd064: bl #0x459090
004fd068: mov r3, #1
004fd06c: cmp r3, #0
004fd070: str r3, [sp, #4]
004fd074: bne #0x4fd0b8
004fd078: add r3, r4, #9
004fd07c: add r2, r4, #0xa
004fd080: ldrb r0, [r2, #1]
004fd084: ldrb r1, [r3, #-1]
004fd088: cmp r2, r3
004fd08c: eor r1, r0, r1
004fd090: strb r1, [r3, #-1]
004fd094: ldrb r0, [r2, #1]
004fd098: eor r1, r1, r0
004fd09c: strb r1, [r2, #1]
004fd0a0: ldrb r0, [r3, #-1]
004fd0a4: sub r2, r2, #1
004fd0a8: eor r1, r1, r0
004fd0ac: strb r1, [r3, #-1]
004fd0b0: add r3, r3, #1
004fd0b4: bhi #0x4fd080
004fd0b8: mov r0, r5
004fd0bc: add r1, r4, #0xc
004fd0c0: bl #0x3df1a0
004fd0c4: mov r3, #1
004fd0c8: cmp r3, #0
004fd0cc: str r3, [sp, #4]
004fd0d0: bne #0x4fd114
004fd0d4: add r3, r4, #0xd
004fd0d8: add r2, r4, #0xe
004fd0dc: ldrb r0, [r2, #1]
004fd0e0: ldrb r1, [r3, #-1]
004fd0e4: cmp r2, r3
004fd0e8: eor r1, r0, r1
004fd0ec: strb r1, [r3, #-1]
004fd0f0: ldrb r0, [r2, #1]
004fd0f4: eor r1, r1, r0
004fd0f8: strb r1, [r2, #1]
004fd0fc: ldrb r0, [r3, #-1]
004fd100: sub r2, r2, #1
004fd104: eor r1, r1, r0
004fd108: strb r1, [r3, #-1]
004fd10c: add r3, r3, #1
004fd110: bhi #0x4fd0dc
004fd114: ldr r0, [r4, #0x10]
004fd118: cmp r0, #0
004fd11c: beq #0x4fd124
004fd120: bl #0x310440
004fd124: ldr r0, [r4, #0xc]
004fd128: mov r1, #1
004fd12c: mov r6, #0
004fd130: add r0, r0, r1
004fd134: bl #0x31056c
004fd138: ldr r2, [r4, #0xc]
004fd13c: mov r1, r0
004fd140: str r0, [r4, #0x10]
004fd144: mov r3, r6
004fd148: mov r0, r5
004fd14c: bl #0x317454
004fd150: ldr r3, [r4, #0xc]
004fd154: ldr r2, [r4, #0x10]
004fd158: mov r0, r5
004fd15c: add r1, r4, #0x14
004fd160: strb r6, [r2, r3]
004fd164: bl #0x459090
004fd168: mov r3, #1
004fd16c: cmp r3, r6
004fd170: str r3, [sp, #4]
004fd174: bne #0x4fd1b8
004fd178: add r3, r4, #0x15
004fd17c: add r2, r4, #0x16
004fd180: ldrb r0, [r2, #1]
004fd184: ldrb r1, [r3, #-1]
004fd188: cmp r2, r3
004fd18c: eor r1, r0, r1
004fd190: strb r1, [r3, #-1]
004fd194: ldrb r0, [r2, #1]
004fd198: eor r1, r1, r0
004fd19c: strb r1, [r2, #1]
004fd1a0: ldrb r0, [r3, #-1]
004fd1a4: sub r2, r2, #1
004fd1a8: eor r1, r1, r0
004fd1ac: strb r1, [r3, #-1]
004fd1b0: add r3, r3, #1
004fd1b4: bhi #0x4fd180
004fd1b8: mov r0, r5
004fd1bc: add r1, r4, #0x18
004fd1c0: bl #0x459090
004fd1c4: mov r3, #1
004fd1c8: cmp r3, #0
004fd1cc: str r3, [sp, #4]
004fd1d0: bne #0x4fd214
004fd1d4: add r3, r4, #0x1a
004fd1d8: add r4, r4, #0x19
004fd1dc: ldrb r1, [r3, #1]
004fd1e0: ldrb r2, [r4, #-1]
004fd1e4: cmp r3, r4
004fd1e8: eor r2, r1, r2
004fd1ec: strb r2, [r4, #-1]
004fd1f0: ldrb r1, [r3, #1]
004fd1f4: eor r2, r2, r1
004fd1f8: strb r2, [r3, #1]
004fd1fc: ldrb r1, [r4, #-1]
004fd200: sub r3, r3, #1
004fd204: eor r2, r2, r1
004fd208: strb r2, [r4, #-1]
004fd20c: add r4, r4, #1
004fd210: bhi #0x4fd1dc
004fd214: add sp, sp, #8
004fd218: pop {r4, r5, r6, pc}

# _ZN7Structs16LevelDeclaration4readEP11IStreamBase
004fca84: push {r4, r5, r6, lr}
004fca88: mov r4, r0
004fca8c: sub sp, sp, #8
004fca90: mov r0, r1
004fca94: mov r5, r1
004fca98: add r1, r4, #4
004fca9c: bl #0x4db89c
004fcaa0: mov r0, r5
004fcaa4: add r1, r4, #8
004fcaa8: bl #0x3df1a0
004fcaac: mov r3, #1
004fcab0: cmp r3, #0
004fcab4: str r3, [sp, #4]
004fcab8: bne #0x4fcafc
004fcabc: add r3, r4, #9
004fcac0: add r2, r4, #0xa
004fcac4: ldrb r0, [r2, #1]
004fcac8: ldrb r1, [r3, #-1]
004fcacc: cmp r3, r2
004fcad0: eor r1, r0, r1
004fcad4: strb r1, [r3, #-1]
004fcad8: ldrb r0, [r2, #1]
004fcadc: eor r1, r1, r0
004fcae0: strb r1, [r2, #1]
004fcae4: ldrb r0, [r3, #-1]
004fcae8: sub r2, r2, #1
004fcaec: eor r1, r1, r0
004fcaf0: strb r1, [r3, #-1]
004fcaf4: add r3, r3, #1
004fcaf8: blo #0x4fcac4
004fcafc: ldr r0, [r4, #0xc]
004fcb00: cmp r0, #0
004fcb04: beq #0x4fcb0c
004fcb08: bl #0x310440
004fcb0c: ldr r0, [r4, #8]
004fcb10: mov r1, #1
004fcb14: mov r6, #0
004fcb18: add r0, r0, r1
004fcb1c: bl #0x31056c
004fcb20: ldr r2, [r4, #8]
004fcb24: mov r1, r0
004fcb28: str r0, [r4, #0xc]
004fcb2c: mov r3, r6
004fcb30: mov r0, r5
004fcb34: bl #0x317454
004fcb38: ldr r3, [r4, #8]
004fcb3c: ldr r2, [r4, #0xc]
004fcb40: mov r0, r5
004fcb44: add r1, r4, #0x10
004fcb48: strb r6, [r2, r3]
004fcb4c: bl #0x459090
004fcb50: mov r3, #1
004fcb54: cmp r3, r6
004fcb58: str r3, [sp, #4]
004fcb5c: bne #0x4fcba0
004fcb60: add r3, r4, #0x11
004fcb64: add r2, r4, #0x12
004fcb68: ldrb r0, [r2, #1]
004fcb6c: ldrb r1, [r3, #-1]
004fcb70: cmp r3, r2
004fcb74: eor r1, r0, r1
004fcb78: strb r1, [r3, #-1]
004fcb7c: ldrb r0, [r2, #1]
004fcb80: eor r1, r1, r0
004fcb84: strb r1, [r2, #1]
004fcb88: ldrb r0, [r3, #-1]
004fcb8c: sub r2, r2, #1
004fcb90: eor r1, r1, r0
004fcb94: strb r1, [r3, #-1]
004fcb98: add r3, r3, #1
004fcb9c: blo #0x4fcb68
004fcba0: add r1, r4, #0x14
004fcba4: mov r0, r5
004fcba8: bl #0x4db89c
004fcbac: mov r0, r5
004fcbb0: add r1, r4, #0x18
004fcbb4: bl #0x459090
004fcbb8: mov r3, #1
004fcbbc: cmp r3, #0
004fcbc0: str r3, [sp, #4]
004fcbc4: bne #0x4fcc08
004fcbc8: add r3, r4, #0x19
004fcbcc: add r2, r4, #0x1a
004fcbd0: ldrb r0, [r2, #1]
004fcbd4: ldrb r1, [r3, #-1]
004fcbd8: cmp r3, r2
004fcbdc: eor r1, r0, r1
004fcbe0: strb r1, [r3, #-1]
004fcbe4: ldrb r0, [r2, #1]
004fcbe8: eor r1, r1, r0
004fcbec: strb r1, [r2, #1]
004fcbf0: ldrb r0, [r3, #-1]
004fcbf4: sub r2, r2, #1
004fcbf8: eor r1, r1, r0
004fcbfc: strb r1, [r3, #-1]
004fcc00: add r3, r3, #1
004fcc04: blo #0x4fcbd0
004fcc08: mov r0, r5
004fcc0c: add r1, r4, #0x1c
004fcc10: bl #0x3df1a0
004fcc14: mov r3, #1
004fcc18: cmp r3, #0
004fcc1c: str r3, [sp, #4]
004fcc20: bne #0x4fcc64
004fcc24: add r3, r4, #0x1d
004fcc28: add r2, r4, #0x1e
004fcc2c: ldrb r0, [r2, #1]
004fcc30: ldrb r1, [r3, #-1]
004fcc34: cmp r3, r2
004fcc38: eor r1, r0, r1
004fcc3c: strb r1, [r3, #-1]
004fcc40: ldrb r0, [r2, #1]
004fcc44: eor r1, r1, r0
004fcc48: strb r1, [r2, #1]
004fcc4c: ldrb r0, [r3, #-1]
004fcc50: sub r2, r2, #1
004fcc54: eor r1, r1, r0
004fcc58: strb r1, [r3, #-1]
004fcc5c: add r3, r3, #1
004fcc60: blo #0x4fcc2c
004fcc64: ldr r0, [r4, #0x20]
004fcc68: cmp r0, #0
004fcc6c: beq #0x4fcc74
004fcc70: bl #0x310440
004fcc74: ldr r0, [r4, #0x1c]
004fcc78: mov r1, #1
004fcc7c: mov r6, #0
004fcc80: add r0, r0, r1
004fcc84: bl #0x31056c
004fcc88: ldr r2, [r4, #0x1c]
004fcc8c: mov r1, r0
004fcc90: str r0, [r4, #0x20]
004fcc94: mov r3, r6
004fcc98: mov r0, r5
004fcc9c: bl #0x317454
004fcca0: ldr r3, [r4, #0x1c]
004fcca4: ldr r2, [r4, #0x20]
004fcca8: mov r0, r5
004fccac: add r1, r4, #0x24
004fccb0: strb r6, [r2, r3]
004fccb4: bl #0x459090
004fccb8: mov r3, #1
004fccbc: cmp r3, r6
004fccc0: str r3, [sp, #4]
004fccc4: bne #0x4fcd08
004fccc8: add r3, r4, #0x25
004fcccc: add r2, r4, #0x26
004fccd0: ldrb r0, [r2, #1]
004fccd4: ldrb r1, [r3, #-1]
004fccd8: cmp r3, r2
004fccdc: eor r1, r0, r1
004fcce0: strb r1, [r3, #-1]
004fcce4: ldrb r0, [r2, #1]
004fcce8: eor r1, r1, r0
004fccec: strb r1, [r2, #1]
004fccf0: ldrb r0, [r3, #-1]
004fccf4: sub r2, r2, #1
004fccf8: eor r1, r1, r0
004fccfc: strb r1, [r3, #-1]
004fcd00: add r3, r3, #1
004fcd04: blo #0x4fccd0
004fcd08: mov r0, r5
004fcd0c: add r1, r4, #0x28
004fcd10: bl #0x459090
004fcd14: mov r3, #1
004fcd18: cmp r3, #0
004fcd1c: str r3, [sp, #4]
004fcd20: bne #0x4fcd64
004fcd24: add r3, r4, #0x29
004fcd28: add r2, r4, #0x2a
004fcd2c: ldrb r0, [r2, #1]
004fcd30: ldrb r1, [r3, #-1]
004fcd34: cmp r3, r2
004fcd38: eor r1, r0, r1
004fcd3c: strb r1, [r3, #-1]
004fcd40: ldrb r0, [r2, #1]
004fcd44: eor r1, r1, r0
004fcd48: strb r1, [r2, #1]
004fcd4c: ldrb r0, [r3, #-1]
004fcd50: sub r2, r2, #1
004fcd54: eor r1, r1, r0
004fcd58: strb r1, [r3, #-1]
004fcd5c: add r3, r3, #1
004fcd60: blo #0x4fcd2c
004fcd64: mov r0, r5
004fcd68: add r1, r4, #0x2c
004fcd6c: bl #0x459090
004fcd70: mov r3, #1
004fcd74: cmp r3, #0
004fcd78: str r3, [sp, #4]
004fcd7c: bne #0x4fcdc0
004fcd80: add r3, r4, #0x2d
004fcd84: add r2, r4, #0x2e
004fcd88: ldrb r0, [r2, #1]
004fcd8c: ldrb r1, [r3, #-1]
004fcd90: cmp r3, r2
004fcd94: eor r1, r0, r1
004fcd98: strb r1, [r3, #-1]
004fcd9c: ldrb r0, [r2, #1]
004fcda0: eor r1, r1, r0
004fcda4: strb r1, [r2, #1]
004fcda8: ldrb r0, [r3, #-1]
004fcdac: sub r2, r2, #1
004fcdb0: eor r1, r1, r0
004fcdb4: strb r1, [r3, #-1]
004fcdb8: add r3, r3, #1
004fcdbc: blo #0x4fcd88
004fcdc0: mov r0, r5
004fcdc4: add r1, r4, #0x30
004fcdc8: bl #0x459090
004fcdcc: mov r3, #1
004fcdd0: cmp r3, #0
004fcdd4: str r3, [sp, #4]
004fcdd8: bne #0x4fce1c
004fcddc: add r3, r4, #0x31
004fcde0: add r2, r4, #0x32
004fcde4: ldrb r0, [r2, #1]
004fcde8: ldrb r1, [r3, #-1]
004fcdec: cmp r3, r2
004fcdf0: eor r1, r0, r1
004fcdf4: strb r1, [r3, #-1]
004fcdf8: ldrb r0, [r2, #1]
004fcdfc: eor r1, r1, r0
004fce00: strb r1, [r2, #1]
004fce04: ldrb r0, [r3, #-1]
004fce08: sub r2, r2, #1
004fce0c: eor r1, r1, r0
004fce10: strb r1, [r3, #-1]
004fce14: add r3, r3, #1
004fce18: blo #0x4fcde4
004fce1c: mov r0, r5
004fce20: add r1, r4, #0x34
004fce24: bl #0x459090
004fce28: mov r3, #1
004fce2c: cmp r3, #0
004fce30: str r3, [sp, #4]
004fce34: bne #0x4fce78
004fce38: add r3, r4, #0x35
004fce3c: add r2, r4, #0x36
004fce40: ldrb r0, [r2, #1]
004fce44: ldrb r1, [r3, #-1]
004fce48: cmp r3, r2
004fce4c: eor r1, r0, r1
004fce50: strb r1, [r3, #-1]
004fce54: ldrb r0, [r2, #1]
004fce58: eor r1, r1, r0
004fce5c: strb r1, [r2, #1]
004fce60: ldrb r0, [r3, #-1]
004fce64: sub r2, r2, #1
004fce68: eor r1, r1, r0
004fce6c: strb r1, [r3, #-1]
004fce70: add r3, r3, #1
004fce74: blo #0x4fce40
004fce78: mov r0, r5
004fce7c: add r1, r4, #0x38
004fce80: bl #0x459090
004fce84: mov r3, #1
004fce88: cmp r3, #0
004fce8c: str r3, [sp, #4]
004fce90: bne #0x4fced4
004fce94: add r3, r4, #0x39
004fce98: add r2, r4, #0x3a
004fce9c: ldrb r0, [r2, #1]
004fcea0: ldrb r1, [r3, #-1]
004fcea4: cmp r3, r2
004fcea8: eor r1, r0, r1
004fceac: strb r1, [r3, #-1]
004fceb0: ldrb r0, [r2, #1]
004fceb4: eor r1, r1, r0
004fceb8: strb r1, [r2, #1]
004fcebc: ldrb r0, [r3, #-1]
004fcec0: sub r2, r2, #1
004fcec4: eor r1, r1, r0
004fcec8: strb r1, [r3, #-1]
004fcecc: add r3, r3, #1
004fced0: blo #0x4fce9c
004fced4: mov r0, r5
004fced8: add r1, r4, #0x3c
004fcedc: bl #0x459090
004fcee0: mov r3, #1
004fcee4: cmp r3, #0
004fcee8: str r3, [sp, #4]
004fceec: bne #0x4fcf30
004fcef0: add r3, r4, #0x3d
004fcef4: add r2, r4, #0x3e
004fcef8: ldrb r0, [r2, #1]
004fcefc: ldrb r1, [r3, #-1]
004fcf00: cmp r3, r2
004fcf04: eor r1, r0, r1
004fcf08: strb r1, [r3, #-1]
004fcf0c: ldrb r0, [r2, #1]
004fcf10: eor r1, r1, r0
004fcf14: strb r1, [r2, #1]
004fcf18: ldrb r0, [r3, #-1]
004fcf1c: sub r2, r2, #1
004fcf20: eor r1, r1, r0
004fcf24: strb r1, [r3, #-1]
004fcf28: add r3, r3, #1
004fcf2c: blo #0x4fcef8
004fcf30: mov r0, r5
004fcf34: add r1, r4, #0x40
004fcf38: bl #0x459090
004fcf3c: mov r3, #1
004fcf40: cmp r3, #0
004fcf44: str r3, [sp, #4]
004fcf48: bne #0x4fcf8c
004fcf4c: add r3, r4, #0x41
004fcf50: add r2, r4, #0x42
004fcf54: ldrb r0, [r2, #1]
004fcf58: ldrb r1, [r3, #-1]
004fcf5c: cmp r3, r2
004fcf60: eor r1, r0, r1
004fcf64: strb r1, [r3, #-1]
004fcf68: ldrb r0, [r2, #1]
004fcf6c: eor r1, r1, r0
004fcf70: strb r1, [r2, #1]
004fcf74: ldrb r0, [r3, #-1]
004fcf78: sub r2, r2, #1
004fcf7c: eor r1, r1, r0
004fcf80: strb r1, [r3, #-1]
004fcf84: add r3, r3, #1
004fcf88: blo #0x4fcf54
004fcf8c: mov r0, r5
004fcf90: add r1, r4, #0x44
004fcf94: bl #0x459090
004fcf98: mov r3, #1
004fcf9c: cmp r3, #0
004fcfa0: str r3, [sp, #4]
004fcfa4: bne #0x4fcfe8
004fcfa8: add r3, r4, #0x46
004fcfac: add r4, r4, #0x45
004fcfb0: ldrb r1, [r3, #1]
004fcfb4: ldrb r2, [r4, #-1]
004fcfb8: cmp r4, r3
004fcfbc: eor r2, r1, r2
004fcfc0: strb r2, [r4, #-1]
004fcfc4: ldrb r1, [r3, #1]
004fcfc8: eor r2, r2, r1
004fcfcc: strb r2, [r3, #1]
004fcfd0: ldrb r1, [r4, #-1]
004fcfd4: sub r3, r3, #1
004fcfd8: eor r2, r2, r1
004fcfdc: strb r2, [r4, #-1]
004fcfe0: add r4, r4, #1
004fcfe4: blo #0x4fcfb0
004fcfe8: add sp, sp, #8
004fcfec: pop {r4, r5, r6, pc}

# _ZN12StreamReader6readAsIbEEvP11IStreamBasePT_
004db89c: str lr, [sp, #-4]!
004db8a0: mov r3, #0
004db8a4: sub sp, sp, #0xc
004db8a8: ldr ip, [r0]
004db8ac: mov r2, #1
004db8b0: mov lr, pc
004db8b4: ldr pc, [ip, #0x18]
004db8b8: ldr r3, [pc, #0x74]
004db8bc: cmp r0, #1
004db8c0: add r3, pc, r3
004db8c4: beq #0x4db8f4
004db8c8: ldr r2, [pc, #0x68]
004db8cc: ldr r2, [r3, r2]
004db8d0: ldr r2, [r2]
004db8d4: cmp r2, #2
004db8d8: moveq r3, #0
004db8dc: streq r3, [r3]
004db8e0: beq #0x4db8ec
004db8e4: cmp r2, #1
004db8e8: beq #0x4db900
004db8ec: add sp, sp, #0xc
004db8f0: ldm sp!, {pc}
004db8f4: cmp r1, #0
004db8f8: beq #0x4db8ec
004db8fc: b #0x4db8c8
004db900: ldr r0, [pc, #0x34]
004db904: ldr r1, [pc, #0x34]
004db908: ldr r2, [pc, #0x34]
004db90c: ldr r0, [r3, r0]
004db910: ldr r3, [pc, #0x30]
004db914: mov ip, #0x50
004db918: add r1, pc, r1
004db91c: add r2, pc, r2
004db920: add r3, pc, r3
004db924: add r0, r0, #0xa8
004db928: str ip, [sp]
004db92c: bl #0x30e004
004db930: b #0x4db8ec
004db934: ldrdeq sb, sl, [fp], #-0x10
004db938: andeq r3, r0, r0, asr #19
004db93c: andeq r1, r0, r0, asr #19
004db940: eorseq r2, lr, r0, asr #21
004db944: eorseq r2, lr, r4, ror #23
004db948: eorseq r4, lr, r0, lsr #8

# _ZN12StreamReader6readAsIjEET_P11IStreamBase
00313a90: str lr, [sp, #-4]!
00313a94: sub sp, sp, #0x14
00313a98: mov r3, #0
00313a9c: ldr ip, [r0]
00313aa0: add r1, sp, #0xc
00313aa4: mov r2, #4
00313aa8: mov lr, pc
00313aac: ldr pc, [ip, #0x18]
00313ab0: ldr r3, [pc, #0x78]
00313ab4: cmp r0, #4
00313ab8: add r3, pc, r3
00313abc: beq #0x313af0
00313ac0: ldr r2, [pc, #0x6c]
00313ac4: ldr r2, [r3, r2]
00313ac8: ldr r2, [r2]
00313acc: cmp r2, #2
00313ad0: moveq r3, #0
00313ad4: streq r3, [r3]
00313ad8: beq #0x313ae4
00313adc: cmp r2, #1
00313ae0: beq #0x313afc
00313ae4: ldr r0, [sp, #0xc]
00313ae8: add sp, sp, #0x14
00313aec: ldm sp!, {pc}
00313af0: cmp r1, #0
00313af4: beq #0x313ae4
00313af8: b #0x313ac0
00313afc: ldr r0, [pc, #0x34]
00313b00: ldr r1, [pc, #0x34]
00313b04: ldr r2, [pc, #0x34]
00313b08: ldr r0, [r3, r0]
00313b0c: ldr r3, [pc, #0x30]
00313b10: mov ip, #0x44
00313b14: add r1, pc, r1
00313b18: add r2, pc, r2
00313b1c: add r3, pc, r3
00313b20: add r0, r0, #0xa8
00313b24: str ip, [sp]
00313b28: bl #0x30e004
00313b2c: b #0x313ae4

# _ZN12StreamReader6readAsIjEEvP11IStreamBasePT_
003df1a0: str lr, [sp, #-4]!
003df1a4: mov r3, #0
003df1a8: sub sp, sp, #0xc
003df1ac: ldr ip, [r0]
003df1b0: mov r2, #4
003df1b4: mov lr, pc
003df1b8: ldr pc, [ip, #0x18]
003df1bc: ldr r3, [pc, #0x74]
003df1c0: cmp r0, #4
003df1c4: add r3, pc, r3
003df1c8: beq #0x3df1f8
003df1cc: ldr r2, [pc, #0x68]
003df1d0: ldr r2, [r3, r2]
003df1d4: ldr r2, [r2]
003df1d8: cmp r2, #2
003df1dc: moveq r3, #0
003df1e0: streq r3, [r3]
003df1e4: beq #0x3df1f0
003df1e8: cmp r2, #1
003df1ec: beq #0x3df204
003df1f0: add sp, sp, #0xc
003df1f4: ldm sp!, {pc}
003df1f8: cmp r1, #0
003df1fc: beq #0x3df1f0
003df200: b #0x3df1cc
003df204: ldr r0, [pc, #0x34]
003df208: ldr r1, [pc, #0x34]
003df20c: ldr r2, [pc, #0x34]
003df210: ldr r0, [r3, r0]
003df214: ldr r3, [pc, #0x30]
003df218: mov ip, #0x50
003df21c: add r1, pc, r1
003df220: add r2, pc, r2
003df224: add r3, pc, r3
003df228: add r0, r0, #0xa8
003df22c: str ip, [sp]
003df230: bl #0x30e004
003df234: b #0x3df1f0
003df238: subseq r5, fp, ip, asr #17
003df23c: andeq r3, r0, r0, asr #19
003df240: andeq r1, r0, r0, asr #19
003df244: strheq pc, [sp], #-0x1c
003df248: subeq pc, sp, r0, ror #5
003df24c: subeq r0, lr, ip, lsl fp

# _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
00459090: str lr, [sp, #-4]!
00459094: mov r3, #0
00459098: sub sp, sp, #0xc
0045909c: ldr ip, [r0]
004590a0: mov r2, #4
004590a4: mov lr, pc
004590a8: ldr pc, [ip, #0x18]
004590ac: ldr r3, [pc, #0x74]
004590b0: cmp r0, #4
004590b4: add r3, pc, r3
004590b8: beq #0x4590e8
004590bc: ldr r2, [pc, #0x68]
004590c0: ldr r2, [r3, r2]
004590c4: ldr r2, [r2]
004590c8: cmp r2, #2
004590cc: moveq r3, #0
004590d0: streq r3, [r3]
004590d4: beq #0x4590e0
004590d8: cmp r2, #1
004590dc: beq #0x4590f4
004590e0: add sp, sp, #0xc
004590e4: ldm sp!, {pc}
004590e8: cmp r1, #0
004590ec: beq #0x4590e0
004590f0: b #0x4590bc
004590f4: ldr r0, [pc, #0x34]
004590f8: ldr r1, [pc, #0x34]
004590fc: ldr r2, [pc, #0x34]
00459100: ldr r0, [r3, r0]
00459104: ldr r3, [pc, #0x30]
00459108: mov ip, #0x50
0045910c: add r1, pc, r1
00459110: add r2, pc, r2
00459114: add r3, pc, r3
00459118: add r0, r0, #0xa8
0045911c: str ip, [sp]
00459120: bl #0x30e004
00459124: b #0x4590e0
00459128: ldrsbeq fp, [r3], #-0x9c
0045912c: andeq r3, r0, r0, asr #19
00459130: andeq r1, r0, r0, asr #19
00459134: subeq r5, r6, ip, asr #5
00459138: strdeq r5, r6, [r6], #-0x30
0045913c: subeq r6, r6, ip, lsr #24

# _ZN12StreamReader12readStringExEP11IStreamBasePcy
00317454: push {r4, r5, r6, lr}
00317458: ldr ip, [r0]
0031745c: mov r4, r2
00317460: mov r5, r3
00317464: mov lr, pc
00317468: ldr pc, [ip, #0x18]
0031746c: cmp r0, r4
00317470: mov r0, #0
00317474: beq #0x317480
00317478: and r0, r0, #1
0031747c: pop {r4, r5, r6, pc}
00317480: cmp r1, r5
00317484: moveq r0, #1
00317488: and r0, r0, #1
0031748c: pop {r4, r5, r6, pc}

# _ZN6Arrays14FastTravelList8finalizeEv
004a6874: push {r4, r5, r6, r7, r8, lr}
004a6878: ldr r5, [pc, #0xcc]
004a687c: ldr r7, [pc, #0xcc]
004a6880: add r5, pc, r5
004a6884: ldr r3, [r5, r7]
004a6888: ldr r3, [r3]
004a688c: cmp r3, #0
004a6890: beq #0x4a6948
004a6894: ldr r8, [pc, #0xb8]
004a6898: ldr r2, [r5, r8]
004a689c: ldr r2, [r2]
004a68a0: cmp r2, #0
004a68a4: beq #0x4a68f4
004a68a8: mov r4, #0
004a68ac: mov r6, r4
004a68b0: b #0x4a68bc
004a68b4: ldr r3, [r5, r7]
004a68b8: ldr r3, [r3]
004a68bc: add r0, r3, r4
004a68c0: ldr r3, [r3, r4]
004a68c4: mov lr, pc
004a68c8: ldr pc, [r3, #8]
004a68cc: ldr r3, [r5, r8]
004a68d0: add r6, r6, #1
004a68d4: add r4, r4, #0x1c
004a68d8: ldr r3, [r3]
004a68dc: cmp r3, r6
004a68e0: bhi #0x4a68b4
004a68e4: ldr r3, [r5, r7]
004a68e8: ldr r3, [r3]
004a68ec: cmp r3, #0
004a68f0: beq #0x4a693c
004a68f4: ldr r2, [r3, #-4]
004a68f8: mov r0, #0x1c
004a68fc: mla r0, r0, r2, r3
004a6900: cmp r3, r0
004a6904: bne #0x4a6910
004a6908: b #0x4a6934
004a690c: mov r0, r4
004a6910: sub r4, r0, #0x1c
004a6914: ldr r3, [r0, #-0x1c]
004a6918: mov r0, r4
004a691c: mov lr, pc
004a6920: ldr pc, [r3]
004a6924: ldr r3, [r5, r7]
004a6928: ldr r0, [r3]
004a692c: cmp r0, r4
004a6930: bne #0x4a690c
004a6934: sub r0, r0, #8
004a6938: bl #0x310440
004a693c: ldr r3, [r5, r7]
004a6940: mov r2, #0
004a6944: str r2, [r3]
004a6948: pop {r4, r5, r6, r7, r8, pc}
004a694c: subeq lr, lr, r0, lsl r2
004a6950: andeq r4, r0, ip, ror sb
004a6954: strdeq r4, r5, [r0], -r4

# _ZN6Arrays9LevelList8finalizeEv
004a66f4: push {r4, r5, r6, r7, r8, lr}
004a66f8: ldr r5, [pc, #0xcc]
004a66fc: ldr r7, [pc, #0xcc]
004a6700: add r5, pc, r5
004a6704: ldr r3, [r5, r7]
004a6708: ldr r3, [r3]
004a670c: cmp r3, #0
004a6710: beq #0x4a67c8
004a6714: ldr r8, [pc, #0xb8]
004a6718: ldr r2, [r5, r8]
004a671c: ldr r2, [r2]
004a6720: cmp r2, #0
004a6724: beq #0x4a6774
004a6728: mov r4, #0
004a672c: mov r6, r4
004a6730: b #0x4a673c
004a6734: ldr r3, [r5, r7]
004a6738: ldr r3, [r3]
004a673c: add r0, r3, r4
004a6740: ldr r3, [r3, r4]
004a6744: mov lr, pc
004a6748: ldr pc, [r3, #8]
004a674c: ldr r3, [r5, r8]
004a6750: add r6, r6, #1
004a6754: add r4, r4, #0x48
004a6758: ldr r3, [r3]
004a675c: cmp r3, r6
004a6760: bhi #0x4a6734
004a6764: ldr r3, [r5, r7]
004a6768: ldr r3, [r3]
004a676c: cmp r3, #0
004a6770: beq #0x4a67bc
004a6774: ldr r2, [r3, #-4]
004a6778: mov r0, #0x48
004a677c: mla r0, r0, r2, r3
004a6780: cmp r3, r0
004a6784: bne #0x4a6790
004a6788: b #0x4a67b4
004a678c: mov r0, r4
004a6790: sub r4, r0, #0x48
004a6794: ldr r3, [r0, #-0x48]
004a6798: mov r0, r4
004a679c: mov lr, pc
004a67a0: ldr pc, [r3]
004a67a4: ldr r3, [r5, r7]
004a67a8: ldr r0, [r3]
004a67ac: cmp r0, r4
004a67b0: bne #0x4a678c
004a67b4: sub r0, r0, #8
004a67b8: bl #0x310440
004a67bc: ldr r3, [r5, r7]
004a67c0: mov r2, #0
004a67c4: str r2, [r3]
004a67c8: pop {r4, r5, r6, r7, r8, pc}
004a67cc: umaaleq lr, lr, r0, r3
004a67d0: andeq r0, r0, r4, ror r8
004a67d4: andeq r1, r0, r0, asr #17
