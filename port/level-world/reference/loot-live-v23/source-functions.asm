# 0x36b80c _ZN15VoxSoundManager4PlayEibiib
0036b80c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b810: ldr r4, [pc, #0x268]
0036b814: ldr r5, [pc, #0x268]
0036b818: ldr lr, [pc, #0x268]
0036b81c: add r4, pc, r4
0036b820: ldr ip, [r4, r5]
0036b824: ldr r6, [r4, lr]
0036b828: sub sp, sp, #0x8c
0036b82c: ldr ip, [ip]
0036b830: mov r7, r0
0036b834: mov r0, r6
0036b838: str r3, [sp, #0x14]
0036b83c: str ip, [sp, #0x84]
0036b840: mov sl, r1
0036b844: mov fp, r2
0036b848: ldrb sb, [sp, #0xb4]
0036b84c: bl #0x337888
0036b850: ldr r1, [pc, #0x234]
0036b854: add r8, sp, #0x6c
0036b858: add r2, sp, #0x68
0036b85c: add r1, pc, r1
0036b860: mov r0, r8
0036b864: bl #0x3140ec
0036b868: mov r0, r6
0036b86c: mov r1, r8
0036b870: bl #0x337a88
0036b874: mov r6, r0
0036b878: mov r0, r8
0036b87c: bl #0x3139ac
0036b880: cmp r6, #0
0036b884: bne #0x36b9e0
0036b888: cmp sl, #0
0036b88c: blt #0x36b9e0
0036b890: cmp sb, #0
0036b894: beq #0x36ba20
0036b898: ldr r3, [pc, #0x1f0]
0036b89c: ldr r3, [r4, r3]
0036b8a0: ldrb r3, [r3]
0036b8a4: cmp r3, #0
0036b8a8: bne #0x36ba00
0036b8ac: ldr r3, [pc, #0x1e0]
0036b8b0: mov r2, #0xc
0036b8b4: add r1, sp, #0x60
0036b8b8: ldr r3, [r4, r3]
0036b8bc: add ip, sp, #0x5c
0036b8c0: add r8, r7, #0x64
0036b8c4: ldr r3, [r3]
0036b8c8: mov r0, r8
0036b8cc: mla sl, r2, sl, r3
0036b8d0: add r3, sp, #0x58
0036b8d4: ldr r6, [sl, #4]
0036b8d8: add r2, sp, #0x50
0036b8dc: stm sp, {r1, ip}
0036b8e0: mov r1, r6
0036b8e4: add ip, sp, #0x54
0036b8e8: str ip, [sp, #8]
0036b8ec: bl #0x8896f4
0036b8f0: ldr r3, [r7, #8]
0036b8f4: ldr r1, [r3, r6, lsl #2]
0036b8f8: cmp r1, #0
0036b8fc: beq #0x36ba5c
0036b900: ldr r0, [r7]
0036b904: bl #0x8624f8
0036b908: cmp r0, #0
0036b90c: beq #0x36b9e0
0036b910: ldr r0, [sp, #0x14]
0036b914: bl #0x30e964
0036b918: mov r1, #0x44000000
0036b91c: add r1, r1, #0x7a0000
0036b920: bl #0x30ec94
0036b924: ldr r3, [r7, #8]
0036b928: ldr r2, [sp, #0x5c]
0036b92c: mov sl, r0
0036b930: ldr r1, [r3, r6, lsl #2]
0036b934: ldr r0, [r7]
0036b938: bl #0x862618
0036b93c: add ip, sp, #0x67
0036b940: str ip, [sp]
0036b944: add ip, sp, #0x44
0036b948: mov r1, r6
0036b94c: mov r0, r8
0036b950: add r2, sp, #0x4c
0036b954: add r3, sp, #0x48
0036b958: str ip, [sp, #4]
0036b95c: add ip, sp, #0x40
0036b960: str ip, [sp, #8]
0036b964: bl #0x889894
0036b968: ldr r3, [r7, #8]
0036b96c: ldr r1, [r7]
0036b970: mov r8, #0
0036b974: ldr r2, [r3, r6, lsl #2]
0036b978: add r6, sp, #0x18
0036b97c: ldr r3, [sp, #0x4c]
0036b980: mov r0, r6
0036b984: str r8, [sp]
0036b988: bl #0x862438
0036b98c: ldr r0, [r7]
0036b990: mov r1, r6
0036b994: mov r2, r8
0036b998: mov r3, #1
0036b99c: bl #0x861d60
0036b9a0: ldr r3, [sp, #0x40]
0036b9a4: mov r2, r8
0036b9a8: ldr r0, [r7]
0036b9ac: mov r1, r6
0036b9b0: bl #0x861950
0036b9b4: ldr r3, [sp, #0xb0]
0036b9b8: sub r3, r3, #1
0036b9bc: cmp r3, #0x1d
0036b9c0: bls #0x36ba48
0036b9c4: ldr r0, [r7]
0036b9c8: mov r3, sl
0036b9cc: mov r1, r6
0036b9d0: ldrb r2, [sp, #0x67]
0036b9d4: bl #0x8621c0
0036b9d8: mov r0, r6
0036b9dc: bl #0x8683ac
0036b9e0: ldr r3, [r4, r5]
0036b9e4: ldr r2, [sp, #0x84]
0036b9e8: mov r0, #0
0036b9ec: ldr r3, [r3]
0036b9f0: cmp r2, r3
0036b9f4: bne #0x36ba7c
0036b9f8: add sp, sp, #0x8c
0036b9fc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036ba00: ldr r3, [pc, #0x90]
0036ba04: mov r0, sl
0036ba08: mov r2, fp
0036ba0c: ldr r1, [r4, r3]
0036ba10: mov r3, #2
0036ba14: ldr r1, [r1]
0036ba18: bl #0x531348
0036ba1c: b #0x36b9e0
0036ba20: bl #0x7fd794
0036ba24: ldrb r3, [r0, #5]
0036ba28: cmp r3, #0
0036ba2c: beq #0x36b898
0036ba30: ldr r3, [pc, #0x64]
0036ba34: ldr r3, [r4, r3]
0036ba38: ldrb r3, [r3]
0036ba3c: cmp r3, #0
0036ba40: bne #0x36b9e0
0036ba44: b #0x36b898
0036ba48: ldr r0, [r7]
0036ba4c: mov r1, r6
0036ba50: ldr r2, [sp, #0x48]
0036ba54: bl #0x862058
0036ba58: b #0x36b9c4
0036ba5c: mov r1, r6
0036ba60: mov r0, r7
0036ba64: bl #0x3699fc
0036ba68: ldr r3, [r7, #8]
0036ba6c: ldr r1, [r3, r6, lsl #2]
0036ba70: cmp r1, #0
0036ba74: beq #0x36b9e0
0036ba78: b #0x36b900
0036ba7c: bl #0x30e310
0036ba80: rsbeq sb, r2, r4, ror r2
0036ba84: andeq r4, r0, ip, lsr #1
0036ba88: andeq r0, r0, r4, lsl #17
0036ba8c: subseq r5, r5, ip, lsl #17
0036ba90: andeq r3, r0, r0, lsr fp
0036ba94: andeq r3, r0, ip, lsr lr
0036ba98: andeq r0, r0, r0, lsl #13
0036ba9c: andeq r2, r0, r0, lsr #31

# 0x393ea0 _ZN10GameObject14UpdatePFObjectEv
00393ea0: push {r4, r5, r6, r7, lr}
00393ea4: ldr r3, [r0, #0x1c8]
00393ea8: ldr r5, [pc, #0xd8]
00393eac: sub sp, sp, #0xc
00393eb0: cmp r3, #0
00393eb4: mov r4, r0
00393eb8: add r5, pc, r5
00393ebc: beq #0x393ee8
00393ec0: ldr r3, [r0]
00393ec4: mov lr, pc
00393ec8: ldr pc, [r3, #0xb4]
00393ecc: cmp r0, #0
00393ed0: bne #0x393f38
00393ed4: ldr r0, [r4, #0x2dc]
00393ed8: cmp r0, #0
00393edc: beq #0x393ef0
00393ee0: bl #0x46e750
00393ee4: str r0, [r4, #0x1d0]
00393ee8: add sp, sp, #0xc
00393eec: pop {r4, r5, r6, r7, pc}
00393ef0: ldr r1, [r4, #0x144]
00393ef4: ldr r0, [r4, #0x150]
00393ef8: bl #0x30e3ac
00393efc: ldr r1, [r4, #0x148]
00393f00: mov r5, r0
00393f04: ldr r0, [r4, #0x154]
00393f08: bl #0x30e3ac
00393f0c: mov r6, r0
00393f10: mov r1, r6
00393f14: mov r0, r5
00393f18: bl #0x30e70c
00393f1c: cmp r0, #0
00393f20: movne r5, r6
00393f24: mov r0, r5
00393f28: mov r1, #0x3f000000
00393f2c: bl #0x30ed6c
00393f30: str r0, [r4, #0x1d0]
00393f34: b #0x393ee8
00393f38: ldr r3, [r4]
00393f3c: mov r0, r4
00393f40: ldr r7, [r4, #0x2dc]
00393f44: mov lr, pc
00393f48: ldr pc, [r3, #0xb8]
00393f4c: ldr r3, [r4]
00393f50: mov r6, r0
00393f54: mov r0, r4
00393f58: mov lr, pc
00393f5c: ldr pc, [r3, #0xbc]
00393f60: ldr r3, [pc, #0x24]
00393f64: subs r7, r7, #0
00393f68: movne r7, #1
00393f6c: str r0, [sp]
00393f70: mov r2, r7
00393f74: ldr r0, [r5, r3]
00393f78: add r1, r4, #0x1c8
00393f7c: mov r3, r6
00393f80: bl #0x528234
00393f84: b #0x393ed4

# 0x38ae2c _ZN10GameObject15UpdateIdleSoundEv
0038ae2c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038ae30: ldr r4, [pc, #0x2a0]
0038ae34: ldr r6, [pc, #0x2a0]
0038ae38: sub sp, sp, #0x2c
0038ae3c: add r4, pc, r4
0038ae40: ldr r3, [r4, r6]
0038ae44: mov r5, r0
0038ae48: ldr r2, [r3]
0038ae4c: cmp r2, #0
0038ae50: beq #0x38aea8
0038ae54: ldrb r8, [r0, #0x373]
0038ae58: cmp r8, #0
0038ae5c: bne #0x38aeb0
0038ae60: ldr r3, [pc, #0x278]
0038ae64: ldr sl, [r4, r3]
0038ae68: mov r0, sl
0038ae6c: bl #0x31f594
0038ae70: cmp r0, #0
0038ae74: beq #0x38aea8
0038ae78: ldr r0, [sl, #0x40]
0038ae7c: mov r1, r8
0038ae80: mov r2, #1
0038ae84: bl #0x36e478
0038ae88: ldr r3, [r0, #0x660]
0038ae8c: cmp r3, #0
0038ae90: beq #0x38aea8
0038ae94: mov r0, sl
0038ae98: bl #0x31f594
0038ae9c: ldr r3, [r0, #0x130]
0038aea0: cmp r3, #0x26
0038aea4: beq #0x38aedc
0038aea8: add sp, sp, #0x2c
0038aeac: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038aeb0: ldrb r2, [r0, #0x372]
0038aeb4: cmp r2, #0
0038aeb8: beq #0x38aea8
0038aebc: mov r2, #0
0038aec0: strb r2, [r0, #0x372]
0038aec4: mov r2, #0x370
0038aec8: ldrsh r1, [r0, r2]
0038aecc: ldr r0, [r3]
0038aed0: mov r2, #0x3e8
0038aed4: bl #0x369fec
0038aed8: b #0x38aea8
0038aedc: ldr r1, [pc, #0x200]
0038aee0: ldr r2, [pc, #0x200]
0038aee4: ldr r0, [sl, #0x2c]
0038aee8: add r1, pc, r1
0038aeec: add r2, pc, r2
0038aef0: bl #0x4c4bdc
0038aef4: bl #0x30e964
0038aef8: ldr r3, [pc, #0x1ec]
0038aefc: mov r7, r0
0038af00: mov r1, r8
0038af04: ldr r3, [r4, r3]
0038af08: ldr r0, [sl, #0x40]
0038af0c: mov r2, #1
0038af10: ldr ip, [r3, #8]
0038af14: ldr lr, [r3]
0038af18: ldr r3, [r3, #4]
0038af1c: str ip, [sp, #0x24]
0038af20: str lr, [sp, #0x1c]
0038af24: str r3, [sp, #0x20]
0038af28: bl #0x36e478
0038af2c: ldr r3, [r0, #0x660]
0038af30: cmp r3, #0
0038af34: beq #0x38b0c8
0038af38: mov r1, r8
0038af3c: ldr r0, [sl, #0x40]
0038af40: mov r2, #1
0038af44: bl #0x36e478
0038af48: ldr r3, [r0, #0x660]
0038af4c: ldr r1, [r3, #0x160]
0038af50: str r1, [sp, #0x1c]
0038af54: ldr sl, [r3, #0x164]
0038af58: str sl, [sp, #0x20]
0038af5c: ldr r8, [r3, #0x168]
0038af60: str r8, [sp, #0x24]
0038af64: ldr r0, [r5, #0x160]
0038af68: bl #0x30e3ac
0038af6c: mov r1, sl
0038af70: mov sb, r0
0038af74: ldr r0, [r5, #0x164]
0038af78: bl #0x30e3ac
0038af7c: mov r1, r8
0038af80: mov fp, r0
0038af84: ldr r0, [r5, #0x168]
0038af88: bl #0x30e3ac
0038af8c: mov r1, sb
0038af90: mov sl, r0
0038af94: mov r0, sb
0038af98: bl #0x30ed6c
0038af9c: mov r1, fp
0038afa0: mov r8, r0
0038afa4: mov r0, fp
0038afa8: bl #0x30ed6c
0038afac: mov r1, r0
0038afb0: mov r0, r8
0038afb4: bl #0x30eba4
0038afb8: mov r1, sl
0038afbc: mov r8, r0
0038afc0: mov r0, sl
0038afc4: bl #0x30ed6c
0038afc8: mov r1, r0
0038afcc: mov r0, r8
0038afd0: bl #0x30eba4
0038afd4: bl #0x30e8a4
0038afd8: bl #0x30e1c0
0038afdc: bl #0x30e6a0
0038afe0: ldrb r3, [r5, #0x372]
0038afe4: mov r8, r0
0038afe8: cmp r3, #0
0038afec: beq #0x38b04c
0038aff0: mov r0, r7
0038aff4: mov r1, r8
0038aff8: bl #0x30e9ac
0038affc: cmp r0, #0
0038b000: beq #0x38aea8
0038b004: mov r0, r7
0038b008: mov r1, #0
0038b00c: bl #0x30e2f8
0038b010: cmp r0, #0
0038b014: beq #0x38aea8
0038b018: ldr r3, [r4, r6]
0038b01c: mov r2, #0
0038b020: strb r2, [r5, #0x372]
0038b024: ldr r0, [r3]
0038b028: mov r3, #0x370
0038b02c: ldrsh r1, [r5, r3]
0038b030: mov r2, #0x3e8
0038b034: add r3, sp, #0x1c
0038b038: str r7, [sp]
0038b03c: bl #0x36a218
0038b040: ldrb r3, [r5, #0x372]
0038b044: cmp r3, #0
0038b048: bne #0x38aea8
0038b04c: mov r1, r8
0038b050: mov r0, r7
0038b054: bl #0x30e2f8
0038b058: cmp r0, #0
0038b05c: beq #0x38aea8
0038b060: mov r0, r7
0038b064: mov r1, #0
0038b068: bl #0x30e2f8
0038b06c: cmp r0, #0
0038b070: beq #0x38aea8
0038b074: ldr r3, [r4, r6]
0038b078: mov ip, #1
0038b07c: strb ip, [r5, #0x372]
0038b080: ldr r7, [r5, #0x160]
0038b084: ldr r6, [r5, #0x164]
0038b088: ldr r4, [r5, #0x168]
0038b08c: ldr r0, [r3]
0038b090: mov lr, #0xbf000000
0038b094: mov r3, #0x370
0038b098: ldrsh r1, [r5, r3]
0038b09c: add lr, lr, #0x800000
0038b0a0: mov r3, ip
0038b0a4: add r2, sp, #0x10
0038b0a8: str r7, [sp, #0x10]
0038b0ac: str r6, [sp, #0x14]
0038b0b0: str r4, [sp, #0x18]
0038b0b4: str lr, [sp, #8]
0038b0b8: str ip, [sp]
0038b0bc: str lr, [sp, #4]
0038b0c0: bl #0x36b5d8
0038b0c4: b #0x38aea8
0038b0c8: ldr r1, [sp, #0x1c]
0038b0cc: ldr sl, [sp, #0x20]
0038b0d0: ldr r8, [sp, #0x24]
0038b0d4: b #0x38af64
0038b0d8: rsbeq sb, r0, r4, asr ip
0038b0dc: andeq r0, r0, r4, lsr #27
0038b0e0: strdeq r3, r4, [r0], -r4
0038b0e4: subseq r7, r3, r0, ror r4
0038b0e8: subseq r7, r3, ip, ror r4
0038b0ec: andeq r3, r0, ip, lsr #30

# 0x3940c0 _ZN10GameObject10UpdatePathEv
003940c0: push {r4, r5, r6, r7, r8, sl, lr}
003940c4: ldr r6, [pc, #0x258]
003940c8: ldr r7, [pc, #0x258]
003940cc: ldr ip, [r0, #0x160]
003940d0: add r6, pc, r6
003940d4: ldr r3, [r6, r7]
003940d8: ldr r2, [r0, #0x164]
003940dc: sub sp, sp, #0x2c
003940e0: ldr r1, [r3]
003940e4: ldr r3, [r0, #0x168]
003940e8: str ip, [r0, #0x1e0]
003940ec: str r1, [sp, #0x24]
003940f0: str r3, [r0, #0x1e8]
003940f4: str r2, [r0, #0x1e4]
003940f8: ldr r3, [r0]
003940fc: mov r5, r0
00394100: mov lr, pc
00394104: ldr pc, [r3, #0x5c]
00394108: cmp r0, #0
0039410c: beq #0x3941a8
00394110: mov r3, r5
00394114: ldr r4, [r3, #0x200]!
00394118: cmp r4, r3
0039411c: beq #0x394150
00394120: ldr r4, [r4]
00394124: cmp r3, r4
00394128: bne #0x394120
0039412c: ldr r3, [pc, #0x1f8]
00394130: add r8, r5, #0x1a8
00394134: add r1, r5, #0x1c8
00394138: ldr r0, [r6, r3]
0039413c: mov r2, r8
00394140: bl #0x52d838
00394144: mov r0, r5
00394148: mov r1, r8
0039414c: bl #0x393600
00394150: mov r0, r5
00394154: bl #0x39361c
00394158: cmp r0, #0
0039415c: beq #0x3941d4
00394160: ldrb r3, [r5, #0x1b4]
00394164: cmp r3, #0
00394168: beq #0x394184
0039416c: ldr r3, [r5, #0x200]
00394170: cmp r3, r4
00394174: beq #0x394314
00394178: ldr r3, [r3]
0039417c: cmp r3, r4
00394180: bne #0x394178
00394184: ldrb r3, [r5, #0x1b5]
00394188: cmp r3, #0
0039418c: beq #0x3941c4
00394190: ldr r3, [r5, #0x1cc]
00394194: ldrb r2, [r5, #0x1c4]
00394198: orr r3, r3, #2
0039419c: cmp r2, #0
003941a0: str r3, [r5, #0x1cc]
003941a4: bne #0x394278
003941a8: ldr r3, [r6, r7]
003941ac: ldr r2, [sp, #0x24]
003941b0: ldr r3, [r3]
003941b4: cmp r2, r3
003941b8: bne #0x394320
003941bc: add sp, sp, #0x2c
003941c0: pop {r4, r5, r6, r7, r8, sl, pc}
003941c4: ldr r3, [r5, #0x1cc]
003941c8: bic r3, r3, #2
003941cc: str r3, [r5, #0x1cc]
003941d0: b #0x3941a8
003941d4: mov r4, #1
003941d8: ldr r1, [r5, #0x164]
003941dc: ldr r0, [r5, #0x1ac]
003941e0: strb r4, [r5, #0x1b4]
003941e4: bl #0x30e3ac
003941e8: ldr r1, [r5, #0x168]
003941ec: mov sl, r0
003941f0: ldr r0, [r5, #0x1b0]
003941f4: bl #0x30e3ac
003941f8: ldr r1, [r5, #0x160]
003941fc: mov r8, r0
00394200: ldr r0, [r5, #0x1a8]
00394204: bl #0x30e3ac
00394208: mov r1, sp
0039420c: str r0, [sp]
00394210: mov r2, r4
00394214: mov r0, r5
00394218: str sl, [sp, #4]
0039421c: str r8, [sp, #8]
00394220: bl #0x393be8
00394224: ldrb r3, [r5, #0x1b5]
00394228: cmp r3, #0
0039422c: beq #0x3941c4
00394230: ldr r3, [r5]
00394234: mov r0, r5
00394238: mov lr, pc
0039423c: ldr pc, [r3, #0x7c]
00394240: cmp r0, #0
00394244: beq #0x394184
00394248: ldr r3, [pc, #0xdc]
0039424c: add r8, r5, #0x1b8
00394250: add r1, r5, #0x1c8
00394254: ldr r0, [r6, r3]
00394258: mov r2, r8
0039425c: bl #0x527cc4
00394260: mov r0, r5
00394264: mov r1, r8
00394268: mov r2, r4
0039426c: bl #0x393be8
00394270: ldrb r3, [r5, #0x1b5]
00394274: b #0x394188
00394278: ldr r3, [pc, #0xb0]
0039427c: add r4, sp, #0xc
00394280: ldr r8, [r6, r3]
00394284: mov r0, r8
00394288: bl #0x337888
0039428c: mov r0, r4
00394290: mov r1, #0x15
00394294: str r4, [sp, #0x1c]
00394298: str r4, [sp, #0x20]
0039429c: bl #0x31167c
003942a0: ldr r1, [pc, #0x8c]
003942a4: mov r2, #0x14
003942a8: ldr r0, [sp, #0x20]
003942ac: add r1, pc, r1
003942b0: bl #0x30e868
003942b4: add r3, r0, #0x14
003942b8: str r3, [sp, #0x1c]
003942bc: mov r3, #0
003942c0: strb r3, [r0, #0x14]
003942c4: mov r1, r4
003942c8: mov r0, r8
003942cc: bl #0x337a88
003942d0: mov r8, r0
003942d4: eor r8, r8, #1
003942d8: mov r0, r4
003942dc: bl #0x3139ac
003942e0: tst r8, #0xff
003942e4: beq #0x3941a8
003942e8: ldr r3, [pc, #0x3c]
003942ec: add r4, r5, #0x1b8
003942f0: add r2, r5, #0x1c8
003942f4: mov r1, r4
003942f8: ldr r0, [r6, r3]
003942fc: bl #0x525d60
00394300: mov r0, r5
00394304: mov r1, r4
00394308: mov r2, #1
0039430c: bl #0x393be8
00394310: b #0x3941a8
00394314: mov r0, r5
00394318: bl #0x3938f8
0039431c: b #0x394184
00394320: bl #0x30e310
00394324: rsbeq r0, r0, r0, asr #19
00394328: andeq r4, r0, ip, lsr #1
0039432c: andeq r1, r0, r4, lsl #4
00394330: andeq r0, r0, r4, lsl #17
00394334: ldrheq lr, [r2], #-0x5c

# 0x393e90 _ZN10GameObject19ForceUpdatePositionEv
00393e90: ldr r0, [r0, #0x2d8]
00393e94: cmp r0, #0
00393e98: bxeq lr
00393e9c: b #0x470bd0

# 0x3460cc _ZN13ObjectManager34ProcessNextGameObjectToStartUpdateEv
003460cc: push {r4, lr}
003460d0: mov r3, r0
003460d4: mov r4, r0
003460d8: ldr r0, [r3, #0x90]!
003460dc: cmp r0, r3
003460e0: beq #0x346118
003460e4: ldr r3, [r0, #8]
003460e8: cmp r3, #0
003460ec: beq #0x3460fc
003460f0: mov r0, r3
003460f4: bl #0x38c710
003460f8: ldr r0, [r4, #0x90]
003460fc: ldr r3, [r0]
00346100: ldr r2, [r0, #4]
00346104: mov r1, #0xc
00346108: str r3, [r2]
0034610c: str r2, [r3, #4]
00346110: pop {r4, lr}
00346114: b #0x708f00
00346118: pop {r4, pc}

# 0x3ebca0 _ZN10ItemObject8InitPostEv
003ebca0: bx lr

# 0x3ec0f0 _ZN10ItemObject9InitAgainER13ItemInventoryjPK9Character
003ec0f0: push {r4, r5, r6, r7, r8, sl, lr}
003ec0f4: add r7, r0, #0x374
003ec0f8: sub sp, sp, #0x34
003ec0fc: mov r5, #0
003ec100: mov r4, r0
003ec104: mov r6, r3
003ec108: mov r0, r1
003ec10c: mov r3, #1
003ec110: mov r1, r2
003ec114: mov r2, r7
003ec118: str r5, [sp]
003ec11c: bl #0x3ffa44
003ec120: mov r0, r7
003ec124: mov r1, r5
003ec128: bl #0x3fc61c
003ec12c: ldr r5, [pc, #0x1bc]
003ec130: subs r7, r0, #0
003ec134: add r5, pc, r5
003ec138: beq #0x3ec29c
003ec13c: bl #0x3f9e08
003ec140: ldr r3, [r0, #0x54]
003ec144: cmn r3, #1
003ec148: beq #0x3ec184
003ec14c: ldr r3, [pc, #0x1a0]
003ec150: mov r0, r7
003ec154: ldr r3, [r5, r3]
003ec158: ldr r8, [r3]
003ec15c: bl #0x3f9e08
003ec160: ldr r3, [r0, #0x54]
003ec164: mov r2, #0x14
003ec168: mla r8, r2, r3, r8
003ec16c: mov r3, #0x3b4
003ec170: ldrh r2, [r8, #4]
003ec174: strh r2, [r4, r3]
003ec178: ldrh r8, [r8, #8]
003ec17c: movw r3, #0x3b6
003ec180: strh r8, [r4, r3]
003ec184: ldr r8, [r4, #0x2d8]
003ec188: cmp r8, #0
003ec18c: beq #0x3ec1bc
003ec190: mov r0, r7
003ec194: bl #0x3fa710
003ec198: ldr r2, [r8, #0x38]
003ec19c: mov r3, #0
003ec1a0: mov r1, r3
003ec1a4: ldr ip, [r2]
003ec1a8: mov r0, r2
003ec1ac: str r3, [sp]
003ec1b0: mov r2, r3
003ec1b4: mov lr, pc
003ec1b8: ldr pc, [ip, #0x1c]
003ec1bc: cmp r6, #0
003ec1c0: strne r6, [r4, #0x3bc]
003ec1c4: mov r3, #0x3b4
003ec1c8: ldrsh r1, [r4, r3]
003ec1cc: ldr r3, [pc, #0x124]
003ec1d0: ldr lr, [r4, #0x1b0]
003ec1d4: ldr r6, [r4, #0x1ac]
003ec1d8: ldr r3, [r5, r3]
003ec1dc: ldr r8, [r4, #0x1a8]
003ec1e0: mov ip, #0xbf000000
003ec1e4: ldr r0, [r3]
003ec1e8: add ip, ip, #0x800000
003ec1ec: mov r7, #1
003ec1f0: add r2, sp, #0x24
003ec1f4: mov r3, #0
003ec1f8: str lr, [sp, #0x2c]
003ec1fc: str ip, [sp, #8]
003ec200: str ip, [sp, #4]
003ec204: str r8, [sp, #0x24]
003ec208: str r6, [sp, #0x28]
003ec20c: str r7, [sp]
003ec210: bl #0x36b5d8
003ec214: ldr r3, [pc, #0xe0]
003ec218: mov r6, #0
003ec21c: mov r1, r6
003ec220: ldr r3, [r5, r3]
003ec224: mov r0, #0x28
003ec228: ldr sl, [r3, #0x44]
003ec22c: bl #0x310570
003ec230: mvn ip, #2
003ec234: str ip, [sp, #0xc]
003ec238: mov ip, #0x40
003ec23c: mov r1, sl
003ec240: mov r2, r4
003ec244: mov r3, r6
003ec248: str ip, [sp, #0x10]
003ec24c: mov ip, #4
003ec250: mov r8, r0
003ec254: str ip, [sp, #0x14]
003ec258: str r7, [sp, #4]
003ec25c: str r7, [sp]
003ec260: str r6, [sp, #8]
003ec264: str r6, [sp, #0x18]
003ec268: bl #0x46f2f0
003ec26c: ldr r3, [pc, #0x8c]
003ec270: mov r0, r4
003ec274: mov r1, r8
003ec278: ldr r3, [r5, r3]
003ec27c: mov r2, r6
003ec280: add r3, r3, #8
003ec284: str r3, [r8]
003ec288: bl #0x394bf8
003ec28c: mov r0, r4
003ec290: bl #0x3ebca4
003ec294: add sp, sp, #0x34
003ec298: pop {r4, r5, r6, r7, r8, sl, pc}
003ec29c: ldr r3, [pc, #0x60]
003ec2a0: ldr r3, [r5, r3]
003ec2a4: ldr r3, [r3]
003ec2a8: cmp r3, #2
003ec2ac: streq r7, [r7]
003ec2b0: beq #0x3ec1c4
003ec2b4: cmp r3, #1
003ec2b8: bne #0x3ec1c4
003ec2bc: ldr r0, [pc, #0x44]
003ec2c0: ldr r1, [pc, #0x44]
003ec2c4: ldr r2, [pc, #0x44]
003ec2c8: ldr r0, [r5, r0]
003ec2cc: ldr r3, [pc, #0x40]
003ec2d0: movw ip, #0x1c7
003ec2d4: add r1, pc, r1
003ec2d8: add r2, pc, r2
003ec2dc: add r3, pc, r3
003ec2e0: add r0, r0, #0xa8
003ec2e4: str ip, [sp]
003ec2e8: bl #0x30e004
003ec2ec: b #0x3ec1c4
003ec2f0: subseq r8, sl, ip, asr sb
003ec2f4: andeq r0, r0, ip, lsr sb
003ec2f8: andeq r0, r0, r4, lsr #27
003ec2fc: strdeq r3, r4, [r0], -r4
003ec300: muleq r0, r0, r6
003ec304: andeq r3, r0, r0, asr #19
003ec308: andeq r1, r0, r0, asr #19
003ec30c: subeq r2, sp, r4, lsl #2
003ec310: subeq sb, sp, r0, asr pc
003ec314: subeq sb, sp, r4, asr pc

# 0x3ebee4 _ZN10ItemObject6UpdateEv
003ebee4: push {r4, r5, r6, r7, r8, lr}
003ebee8: mov r4, r0
003ebeec: bl #0x39361c
003ebef0: ldr r5, [pc, #0xa0]
003ebef4: cmp r0, #0
003ebef8: add r5, pc, r5
003ebefc: bne #0x3ebf8c
003ebf00: mov r0, r4
003ebf04: bl #0x38cbe8
003ebf08: ldr r0, [r4, #0x3c8]
003ebf0c: cmp r0, #0
003ebf10: beq #0x3ebf28
003ebf14: bl #0x498cdc
003ebf18: cmp r0, #0
003ebf1c: bne #0x3ebf54
003ebf20: ldr r0, [r4, #0x3c8]
003ebf24: bl #0x498e5c
003ebf28: mov r6, #0x3b8
003ebf2c: ldrh r7, [r4, r6]
003ebf30: sxth r3, r7
003ebf34: cmp r3, #0
003ebf38: ble #0x3ebf50
003ebf3c: ldr r3, [pc, #0x58]
003ebf40: ldr r0, [r5, r3]
003ebf44: bl #0x31f66c
003ebf48: rsb r0, r0, r7
003ebf4c: strh r0, [r4, r6]
003ebf50: pop {r4, r5, r6, r7, r8, pc}
003ebf54: ldr r3, [r4, #0x3c8]
003ebf58: add r1, r4, #0x160
003ebf5c: ldr r0, [r3, #8]
003ebf60: bl #0x496d9c
003ebf64: ldr r3, [r4, #0x3c4]
003ebf68: cmp r3, #0
003ebf6c: beq #0x3ebf20
003ebf70: movw r2, #0x14a4
003ebf74: ldr r3, [r3, r2]
003ebf78: cmp r4, r3
003ebf7c: beq #0x3ebf20
003ebf80: mov r0, r4
003ebf84: bl #0x3ebcf8
003ebf88: b #0x3ebf20
003ebf8c: mov r0, r4
003ebf90: bl #0x3938f8
003ebf94: b #0x3ebf00

# 0x3ebca8 _ZN10ItemObject18UpdateLocalizationEv
003ebca8: push {r4, lr}
003ebcac: add r0, r0, #0x374
003ebcb0: mov r1, #0
003ebcb4: bl #0x3fc61c
003ebcb8: cmp r0, #0
003ebcbc: beq #0x3ebcc8
003ebcc0: pop {r4, lr}
003ebcc4: b #0x3fc1b4
003ebcc8: pop {r4, pc}

# 0x38b8b8 _ZN10GameObject19RequireOnlineUpdateEv
0038b8b8: push {r4, lr}
0038b8bc: mov r4, r0
0038b8c0: bl #0x7fd794
0038b8c4: ldrb r3, [r0, #5]
0038b8c8: cmp r3, #0
0038b8cc: beq #0x38b8f4
0038b8d0: ldr r3, [r4, #0x100]
0038b8d4: cmp r3, #0
0038b8d8: beq #0x38b8f4
0038b8dc: bl #0x7fd794
0038b8e0: bl #0x7fd5b4
0038b8e4: cmp r0, #0
0038b8e8: beq #0x38b8f8
0038b8ec: mov r3, #1
0038b8f0: strb r3, [r4, #0x119]
0038b8f4: pop {r4, pc}
0038b8f8: ldr r3, [r4]
0038b8fc: mov r0, r4
0038b900: mov lr, pc
0038b904: ldr pc, [r3, #0x54]
0038b908: cmp r0, #0
0038b90c: bne #0x38b8f4
0038b910: b #0x38b8ec

# 0x38aac8 _ZN10GameObject18UpdateAbsoluteAABBEv
0038aac8: push {r4, r5, r6, r7, r8, sb, sl, lr}
0038aacc: mov r4, r0
0038aad0: ldr sl, [r4, #0x148]
0038aad4: ldr r0, [r0, #0x144]
0038aad8: ldr r8, [r4, #0x14c]
0038aadc: ldr r7, [r4, #0x150]
0038aae0: ldr r6, [r4, #0x154]
0038aae4: ldr r5, [r4, #0x158]
0038aae8: ldr r1, [r4, #0x160]
0038aaec: str r0, [r4, #0x12c]
0038aaf0: str sl, [r4, #0x130]
0038aaf4: str r8, [r4, #0x134]
0038aaf8: str r7, [r4, #0x138]
0038aafc: str r6, [r4, #0x13c]
0038ab00: str r5, [r4, #0x140]
0038ab04: bl #0x30eba4
0038ab08: ldr r1, [r4, #0x164]
0038ab0c: str r0, [r4, #0x12c]
0038ab10: mov r0, sl
0038ab14: bl #0x30eba4
0038ab18: ldr r1, [r4, #0x168]
0038ab1c: str r0, [r4, #0x130]
0038ab20: mov r0, r8
0038ab24: bl #0x30eba4
0038ab28: ldr r1, [r4, #0x160]
0038ab2c: str r0, [r4, #0x134]
0038ab30: mov r0, r7
0038ab34: bl #0x30eba4
0038ab38: ldr r1, [r4, #0x164]
0038ab3c: str r0, [r4, #0x138]
0038ab40: mov r0, r6
0038ab44: bl #0x30eba4
0038ab48: ldr r1, [r4, #0x168]
0038ab4c: str r0, [r4, #0x13c]
0038ab50: mov r0, r5
0038ab54: bl #0x30eba4
0038ab58: str r0, [r4, #0x140]
0038ab5c: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x38cbe8 _ZN10GameObject6UpdateEv
0038cbe8: push {r4, r5, r6, r7, r8, lr}
0038cbec: ldr r5, [pc, #0x138]
0038cbf0: ldr r7, [pc, #0x138]
0038cbf4: mov r4, r0
0038cbf8: add r5, pc, r5
0038cbfc: ldr r3, [r5, r7]
0038cc00: ldr r0, [pc, #0x12c]
0038cc04: sub sp, sp, #0x20
0038cc08: ldr r3, [r3]
0038cc0c: add r0, pc, r0
0038cc10: add r6, sp, #4
0038cc14: str r3, [sp, #0x1c]
0038cc18: bl #0x3136b4
0038cc1c: ldr r3, [pc, #0x114]
0038cc20: ldr r8, [r5, r3]
0038cc24: mov r0, r8
0038cc28: bl #0x337888
0038cc2c: ldr r1, [pc, #0x108]
0038cc30: mov r2, sp
0038cc34: mov r0, r6
0038cc38: add r1, pc, r1
0038cc3c: bl #0x3140ec
0038cc40: mov r1, r6
0038cc44: mov r0, r8
0038cc48: bl #0x337a88
0038cc4c: mov r0, r6
0038cc50: bl #0x318254
0038cc54: ldr r3, [pc, #0xe4]
0038cc58: ldr r3, [r5, r3]
0038cc5c: ldr r3, [r3, #0x38]
0038cc60: ldr r2, [r3, #0x58]
0038cc64: add r2, r2, #1
0038cc68: str r2, [r3, #0x58]
0038cc6c: ldr r1, [r4, #0x2e4]
0038cc70: cmp r1, #0
0038cc74: beq #0x38cc90
0038cc78: ldr r3, [r4]
0038cc7c: mov r0, r4
0038cc80: mov lr, pc
0038cc84: ldr pc, [r3, #0x98]
0038cc88: mov r3, #0
0038cc8c: str r3, [r4, #0x2e4]
0038cc90: ldr r3, [r4, #0x174]
0038cc94: ldr lr, [r4, #0x160]
0038cc98: ldr ip, [r4, #0x164]
0038cc9c: ldr r1, [r4, #0x16c]
0038cca0: ldr r2, [r4, #0x170]
0038cca4: ldr r0, [r4, #0x168]
0038cca8: str r3, [r4, #0x1a4]
0038ccac: str lr, [r4, #0x190]
0038ccb0: str ip, [r4, #0x194]
0038ccb4: str r1, [r4, #0x19c]
0038ccb8: str r2, [r4, #0x1a0]
0038ccbc: str r0, [r4, #0x198]
0038ccc0: mov r0, r4
0038ccc4: bl #0x3940c0
0038ccc8: mov r0, r4
0038cccc: bl #0x393710
0038ccd0: mov r0, r4
0038ccd4: bl #0x3943cc
0038ccd8: mov r0, r4
0038ccdc: bl #0x393d74
0038cce0: mov r0, r4
0038cce4: bl #0x38b8b8
0038cce8: mov r3, #0x370
0038ccec: ldrsh r3, [r4, r3]
0038ccf0: cmp r3, #0
0038ccf4: blt #0x38cd00
0038ccf8: mov r0, r4
0038ccfc: bl #0x38ae2c
0038cd00: ldr r0, [pc, #0x3c]
0038cd04: add r0, pc, r0
0038cd08: bl #0x3136b8
0038cd0c: ldr r3, [r5, r7]
0038cd10: ldr r2, [sp, #0x1c]
0038cd14: ldr r3, [r3]
0038cd18: cmp r2, r3
0038cd1c: bne #0x38cd28
0038cd20: add sp, sp, #0x20
0038cd24: pop {r4, r5, r6, r7, r8, pc}
0038cd28: bl #0x30e310
0038cd2c: mlseq r0, r8, lr, r7
0038cd30: andeq r4, r0, ip, lsr #1
0038cd34: subseq r5, r3, r4, asr r8
0038cd38: andeq r0, r0, r4, lsl #17
0038cd3c: subseq r3, r3, r0, ror #14
0038cd40: strdeq r3, r4, [r0], -r4
0038cd44: subseq r5, r3, ip, asr r7

# 0x393d74 _ZN10GameObject20UpdateTargetPositionEv
00393d74: push {r4, lr}
00393d78: ldr r1, [r0, #0x180]
00393d7c: sub sp, sp, #0x10
00393d80: mov r4, r0
00393d84: cmp r1, #0
00393d88: beq #0x393dac
00393d8c: add r0, sp, #4
00393d90: bl #0x597180
00393d94: ldr r2, [sp, #8]
00393d98: ldr r3, [sp, #0xc]
00393d9c: ldr r1, [sp, #4]
00393da0: str r2, [r4, #0x188]
00393da4: str r3, [r4, #0x18c]
00393da8: str r1, [r4, #0x184]
00393dac: add sp, sp, #0x10
00393db0: pop {r4, pc}

# 0x393710 _ZN10GameObject14UpdateRotationEv
00393710: push {r4, r5, r6, r7, r8, lr}
00393714: ldr r3, [r0]
00393718: mov r4, r0
0039371c: mov lr, pc
00393720: ldr pc, [r3, #0xac]
00393724: mov r1, #0
00393728: mov r5, r0
0039372c: bl #0x30e70c
00393730: ldr r3, [pc, #0x160]
00393734: cmp r0, #0
00393738: add r3, pc, r3
0039373c: beq #0x39377c
00393740: ldr r3, [r4, #0x178]
00393744: str r3, [r4, #0x174]
00393748: ldr r3, [r4, #0x2d8]
0039374c: cmp r3, #0
00393750: beq #0x393778
00393754: ldr r3, [r4]
00393758: mov r0, r4
0039375c: mov lr, pc
00393760: ldr pc, [r3, #0x70]
00393764: cmp r0, #0
00393768: beq #0x393778
0039376c: ldr r0, [r4, #0x2d8]
00393770: pop {r4, r5, r6, r7, r8, lr}
00393774: b #0x472948
00393778: pop {r4, r5, r6, r7, r8, pc}
0039377c: ldr r2, [pc, #0x118]
00393780: ldr r0, [r3, r2]
00393784: bl #0x31f66c
00393788: movw r1, #0xfdb
0039378c: mov r6, r0
00393790: movt r1, #0x4149
00393794: mov r0, r5
00393798: bl #0x30ed6c
0039379c: mov r5, r0
003937a0: mov r0, r6
003937a4: bl #0x30e2e0
003937a8: movw r1, #0x126f
003937ac: movt r1, #0x3a83
003937b0: bl #0x30ed6c
003937b4: mov r1, r0
003937b8: mov r0, r5
003937bc: bl #0x30ed6c
003937c0: ldr r6, [r4, #0x178]
003937c4: ldr r7, [r4, #0x174]
003937c8: mov r8, r0
003937cc: mov r0, r6
003937d0: mov r1, r7
003937d4: bl #0x30e3ac
003937d8: movw r1, #0xfdb
003937dc: movt r1, #0x4049
003937e0: mov r5, r0
003937e4: bl #0x30e2f8
003937e8: cmp r0, #0
003937ec: bne #0x393864
003937f0: movw r1, #0xfdb
003937f4: mov r0, r5
003937f8: movt r1, #0xc049
003937fc: bl #0x30e70c
00393800: cmp r0, #0
00393804: beq #0x39381c
00393808: movw r1, #0xfdb
0039380c: mov r0, r5
00393810: movt r1, #0x40c9
00393814: bl #0x30eba4
00393818: mov r5, r0
0039381c: bic r0, r5, #0x80000000
00393820: mov r1, r8
00393824: bl #0x30e70c
00393828: cmp r0, #0
0039382c: strne r6, [r4, #0x174]
00393830: bne #0x393748
00393834: mov r0, r5
00393838: mov r1, #0
0039383c: bl #0x30e70c
00393840: cmp r0, #0
00393844: beq #0x39387c
00393848: mov r0, r7
0039384c: mov r1, r8
00393850: bl #0x30e3ac
00393854: mov r3, #0
00393858: str r0, [r4, #0x174]
0039385c: str r3, [r4, #0x17c]
00393860: b #0x393748
00393864: movw r1, #0xfdb
00393868: mov r0, r5
0039386c: movt r1, #0x40c9
00393870: bl #0x30e3ac
00393874: mov r5, r0
00393878: b #0x39381c
0039387c: mov r0, r8
00393880: mov r1, r7
00393884: bl #0x30eba4
00393888: mov r3, #1
0039388c: str r0, [r4, #0x174]
00393890: str r3, [r4, #0x17c]
00393894: b #0x393748
00393898: rsbeq r1, r0, r8, asr r3
0039389c: strdeq r3, r4, [r0], -r4

# 0x3ece80 _ZN10ItemObject8InitOnceEi
003ece80: ldr r3, [pc, #0xcc]
003ece84: push {r4, r5, r6, lr}
003ece88: add r3, pc, r3
003ece8c: mov r5, r1
003ece90: mov r1, r3
003ece94: mov r3, #0x3ac
003ece98: strh r5, [r0, r3]
003ece9c: mov r4, r0
003ecea0: add r2, r1, #0x22
003ecea4: add r0, r0, #0x290
003ecea8: bl #0x3109e0
003eceac: ldr r3, [pc, #0xa4]
003eceb0: mov r2, #0x40000000
003eceb4: cmp r5, #0
003eceb8: add r2, r2, #0xc00000
003ecebc: str r2, [r4, #0x3b0]
003ecec0: add r3, pc, r3
003ecec4: blt #0x3ecedc
003ecec8: ldr r2, [pc, #0x8c]
003ececc: ldr r2, [r3, r2]
003eced0: ldr r2, [r2]
003eced4: cmp r5, r2
003eced8: blt #0x3ecf0c
003ecedc: ldr r1, [pc, #0x7c]
003ecee0: add r0, r4, #0x2a8
003ecee4: add r1, pc, r1
003ecee8: add r2, r1, #0x12
003eceec: bl #0x3109e0
003ecef0: mov r0, r4
003ecef4: bl #0x38be5c
003ecef8: ldr r0, [r4, #0x2d8]
003ecefc: cmp r0, #0
003ecf00: beq #0x3ecf50
003ecf04: pop {r4, r5, r6, lr}
003ecf08: b #0x470a54
003ecf0c: ldr r2, [pc, #0x50]
003ecf10: ldr r3, [r3, r2]
003ecf14: mov r2, #0x14
003ecf18: ldr r3, [r3]
003ecf1c: mla r5, r2, r5, r3
003ecf20: ldr r5, [r5, #0x10]
003ecf24: mov r0, r5
003ecf28: bl #0x30de54
003ecf2c: mov r1, r5
003ecf30: add r2, r5, r0
003ecf34: add r0, r4, #0x2a8
003ecf38: bl #0x3109e0
003ecf3c: mov r0, r4
003ecf40: bl #0x38be5c
003ecf44: ldr r0, [r4, #0x2d8]
003ecf48: cmp r0, #0
003ecf4c: bne #0x3ecf04
003ecf50: pop {r4, r5, r6, pc}
003ecf54: subeq sb, sp, r8, lsl r4
003ecf58: ldrsbeq r7, [sl], #-0xb0
003ecf5c: andeq r4, r0, r0, lsl r5
003ecf60: subeq sb, sp, r4, ror #7
003ecf64: andeq r0, r0, ip, lsr sb

# 0x36b5d8 _ZN15VoxSoundManager6Play3DEiRKN6glitch4core8vector3dIfEEbiff
0036b5d8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b5dc: ldr r4, [pc, #0x200]
0036b5e0: ldr r5, [pc, #0x200]
0036b5e4: ldr r7, [pc, #0x200]
0036b5e8: add r4, pc, r4
0036b5ec: ldr ip, [r4, r5]
0036b5f0: ldr r6, [r4, r7]
0036b5f4: sub sp, sp, #0x74
0036b5f8: ldr ip, [ip]
0036b5fc: mov sb, r0
0036b600: mov r0, r6
0036b604: str r3, [sp, #0x1c]
0036b608: str ip, [sp, #0x6c]
0036b60c: mov sl, r1
0036b610: mov fp, r2
0036b614: bl #0x337888
0036b618: ldr r1, [pc, #0x1d0]
0036b61c: add r8, sp, #0x54
0036b620: add r2, sp, #0x38
0036b624: add r1, pc, r1
0036b628: mov r0, r8
0036b62c: bl #0x3140ec
0036b630: mov r0, r6
0036b634: mov r1, r8
0036b638: bl #0x337a88
0036b63c: mov r6, r0
0036b640: mov r0, r8
0036b644: bl #0x3139ac
0036b648: cmp r6, #0
0036b64c: beq #0x36b670
0036b650: ldr r3, [r4, r5]
0036b654: ldr r2, [sp, #0x6c]
0036b658: mov r0, #0
0036b65c: ldr r3, [r3]
0036b660: cmp r2, r3
0036b664: bne #0x36b7e0
0036b668: add sp, sp, #0x74
0036b66c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036b670: ldr r3, [pc, #0x17c]
0036b674: ldr r0, [r4, r3]
0036b678: bl #0x31f594
0036b67c: cmp r0, #0
0036b680: beq #0x36b650
0036b684: ldr r3, [r0, #0x130]
0036b688: cmp r3, #0x26
0036b68c: bne #0x36b650
0036b690: bl #0x7fd794
0036b694: ldrb r3, [r0, #5]
0036b698: cmp r3, #0
0036b69c: beq #0x36b6b4
0036b6a0: ldr r3, [pc, #0x150]
0036b6a4: ldr r3, [r4, r3]
0036b6a8: ldrb r3, [r3]
0036b6ac: cmp r3, #0
0036b6b0: bne #0x36b650
0036b6b4: cmp sl, #0
0036b6b8: blt #0x36b650
0036b6bc: ldr r3, [pc, #0x138]
0036b6c0: ldr r3, [r4, r3]
0036b6c4: ldrb r3, [r3]
0036b6c8: cmp r3, #0
0036b6cc: bne #0x36b79c
0036b6d0: ldr r3, [pc, #0x128]
0036b6d4: mov r2, #0xc
0036b6d8: ldr r3, [r4, r3]
0036b6dc: ldr r3, [r3]
0036b6e0: mla sl, r2, sl, r3
0036b6e4: ldr r3, [sl, #8]
0036b6e8: ldr r8, [sl, #4]
0036b6ec: cmp r3, #1
0036b6f0: beq #0x36b7bc
0036b6f4: add ip, sp, #0x30
0036b6f8: str ip, [sp]
0036b6fc: add ip, sp, #0x2c
0036b700: add r3, sp, #0x28
0036b704: mov r1, r8
0036b708: add r2, sp, #0x20
0036b70c: str ip, [sp, #4]
0036b710: add r0, sb, #0x64
0036b714: add ip, sp, #0x24
0036b718: str ip, [sp, #8]
0036b71c: bl #0x8896f4
0036b720: ldr r7, [r4, r7]
0036b724: add r6, sp, #0x3c
0036b728: mov r0, r7
0036b72c: bl #0x337888
0036b730: ldr r1, [pc, #0xcc]
0036b734: add r2, sp, #0x34
0036b738: mov r0, r6
0036b73c: add r1, pc, r1
0036b740: bl #0x3140ec
0036b744: mov r1, r6
0036b748: mov r0, r7
0036b74c: bl #0x337a88
0036b750: mov r0, r6
0036b754: bl #0x3139ac
0036b758: ldr ip, [sp, #0x30]
0036b75c: mov r0, sb
0036b760: mov r1, r8
0036b764: str ip, [sp]
0036b768: ldr ip, [sp, #0x2c]
0036b76c: ldr r2, [sp, #0x20]
0036b770: ldr r3, [sp, #0x28]
0036b774: str ip, [sp, #4]
0036b778: ldr ip, [sp, #0x24]
0036b77c: str fp, [sp, #0xc]
0036b780: str ip, [sp, #8]
0036b784: ldr ip, [sp, #0x9c]
0036b788: str ip, [sp, #0x10]
0036b78c: ldr ip, [sp, #0xa0]
0036b790: str ip, [sp, #0x14]
0036b794: bl #0x36a7c0
0036b798: b #0x36b650
0036b79c: ldr r3, [pc, #0x64]
0036b7a0: mov r0, sl
0036b7a4: ldr r2, [sp, #0x1c]
0036b7a8: ldr r1, [r4, r3]
0036b7ac: mov r3, #2
0036b7b0: ldr r1, [r1]
0036b7b4: bl #0x531348
0036b7b8: b #0x36b650
0036b7bc: mov ip, #0xbf000000
0036b7c0: add ip, ip, #0x800000
0036b7c4: mov r0, sb
0036b7c8: mov r1, r8
0036b7cc: mov r2, fp
0036b7d0: mov r3, ip
0036b7d4: str ip, [sp]
0036b7d8: bl #0x36b420
0036b7dc: b #0x36b650
0036b7e0: bl #0x30e310
0036b7e4: rsbeq sb, r2, r8, lsr #9
0036b7e8: andeq r4, r0, ip, lsr #1
0036b7ec: andeq r0, r0, r4, lsl #17
0036b7f0: subseq r5, r5, r4, asr #21
0036b7f4: strdeq r3, r4, [r0], -r4
0036b7f8: andeq r2, r0, r0, lsr #31
0036b7fc: andeq r3, r0, r0, lsr fp
0036b800: andeq r3, r0, ip, lsr lr
0036b804: subseq r5, r5, r4, asr #19
0036b808: andeq r0, r0, r0, lsl #13

# 0x3943cc _ZN10GameObject16UpdateSubObjectsEv
003943cc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003943d0: ldr r3, [r0, #0x2d8]
003943d4: ldr r5, [pc, #0x5c8]
003943d8: sub sp, sp, #0x24
003943dc: cmp r3, #0
003943e0: mov r4, r0
003943e4: add r5, pc, r5
003943e8: beq #0x3943fc
003943ec: mov r0, r3
003943f0: ldr r3, [r3]
003943f4: mov lr, pc
003943f8: ldr pc, [r3, #8]
003943fc: ldr r3, [r4, #0x2dc]
00394400: cmp r3, #0
00394404: beq #0x394418
00394408: mov r0, r3
0039440c: ldr r3, [r3]
00394410: mov lr, pc
00394414: ldr pc, [r3, #0x20]
00394418: ldr r3, [r4]
0039441c: mov r0, r4
00394420: mov lr, pc
00394424: ldr pc, [r3, #0x60]
00394428: ldr r3, [r4]
0039442c: mov r7, r0
00394430: mov r0, r4
00394434: mov lr, pc
00394438: ldr pc, [r3, #0x64]
0039443c: mov r8, r0
00394440: mov r0, r4
00394444: bl #0x39361c
00394448: ldr r1, [r4, #0x2dc]
0039444c: mov r6, r0
00394450: cmp r1, #0
00394454: moveq sl, r1
00394458: beq #0x39450c
0039445c: ldr r3, [r1, #0x14]
00394460: ldrh r3, [r3]
00394464: tst r3, #8
00394468: beq #0x394780
0039446c: mov sl, #0
00394470: cmp r8, #0
00394474: beq #0x394888
00394478: ldr r1, [r4, #0x160]
0039447c: ldr r0, [r4, #0x1a8]
00394480: bl #0x30e3ac
00394484: ldr r1, [r4, #0x164]
00394488: mov fp, r0
0039448c: ldr r0, [r4, #0x1ac]
00394490: bl #0x30e3ac
00394494: ldr r1, [r4, #0x168]
00394498: mov sb, r0
0039449c: ldr r0, [r4, #0x1b0]
003944a0: bl #0x30e3ac
003944a4: mov r1, fp
003944a8: mov r8, r0
003944ac: mov r0, fp
003944b0: str fp, [sp, #0xc]
003944b4: str sb, [sp, #0x10]
003944b8: str r8, [sp, #0x14]
003944bc: bl #0x30ed6c
003944c0: mov r1, sb
003944c4: mov fp, r0
003944c8: mov r0, sb
003944cc: bl #0x30ed6c
003944d0: mov r1, r0
003944d4: mov r0, fp
003944d8: bl #0x30eba4
003944dc: mov r1, r8
003944e0: mov sb, r0
003944e4: mov r0, r8
003944e8: bl #0x30ed6c
003944ec: mov r1, r0
003944f0: mov r0, sb
003944f4: bl #0x30eba4
003944f8: mov r1, #0
003944fc: mov r8, r0
00394500: bl #0x30e2f8
00394504: cmp r0, #0
00394508: bne #0x39489c
0039450c: cmp r7, #0
00394510: beq #0x39468c
00394514: ldr r0, [r4, #0x2d8]
00394518: cmp r0, #0
0039451c: beq #0x39452c
00394520: cmp sl, #0
00394524: beq #0x394664
00394528: bl #0x470cb8
0039452c: ldr r0, [r4, #0x2dc]
00394530: cmp r0, #0
00394534: beq #0x394544
00394538: ldr r1, [r4, #0x160]
0039453c: ldr r2, [r4, #0x164]
00394540: bl #0x46ea80
00394544: ldr r3, [r4]
00394548: mov r0, r4
0039454c: mov lr, pc
00394550: ldr pc, [r3, #0x68]
00394554: ldr r3, [r4]
00394558: mov r7, r0
0039455c: mov r0, r4
00394560: mov lr, pc
00394564: ldr pc, [r3, #0x6c]
00394568: cmp r7, #0
0039456c: beq #0x394634
00394570: ldr r3, [r4, #0x2d8]
00394574: cmp r3, #0
00394578: beq #0x394634
0039457c: mov r0, r3
00394580: bl #0x470ccc
00394584: ldr r3, [r4, #0x2d8]
00394588: cmp r3, #0
0039458c: beq #0x3945c0
00394590: ldr r3, [r4]
00394594: mov r0, r4
00394598: mov lr, pc
0039459c: ldr pc, [r3, #0x70]
003945a0: cmp r0, #0
003945a4: beq #0x3945b0
003945a8: ldr r0, [r4, #0x2d8]
003945ac: bl #0x472948
003945b0: ldr r0, [r4, #0x2d8]
003945b4: cmp r0, #0
003945b8: beq #0x3945c0
003945bc: bl #0x472860
003945c0: ldr r3, [r4]
003945c4: mov r0, r4
003945c8: mov lr, pc
003945cc: ldr pc, [r3, #0x74]
003945d0: cmp r0, #0
003945d4: beq #0x394824
003945d8: cmp r6, #0
003945dc: beq #0x3945f8
003945e0: ldr r1, [r4, #0x160]
003945e4: ldr r2, [r4, #0x164]
003945e8: ldr r3, [r4, #0x168]
003945ec: str r1, [r4, #0x1a8]
003945f0: str r2, [r4, #0x1ac]
003945f4: str r3, [r4, #0x1b0]
003945f8: mov r0, r4
003945fc: bl #0x38aac8
00394600: ldr r3, [r4, #0x2e0]
00394604: cmp r3, #0
00394608: beq #0x39462c
0039460c: mov r0, r3
00394610: ldr r3, [r3]
00394614: mov lr, pc
00394618: ldr pc, [r3, #0xc]
0039461c: ldr r3, [r4, #0x2e0]
00394620: ldr r2, [r3, #4]
00394624: cmp r2, #2
00394628: beq #0x3946a0
0039462c: add sp, sp, #0x24
00394630: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00394634: cmp r0, #0
00394638: beq #0x394584
0039463c: ldr r0, [r4, #0x2dc]
00394640: cmp r0, #0
00394644: beq #0x394584
00394648: ldr r3, [r0, #0x14]
0039464c: ldrh r3, [r3]
00394650: tst r3, #8
00394654: bne #0x394584
00394658: bl #0x46e858
0039465c: str r0, [r4, #0x174]
00394660: b #0x394584
00394664: bl #0x47124c
00394668: ldr r3, [r4, #0x2dc]
0039466c: cmp r3, #0
00394670: beq #0x39468c
00394674: ldr r3, [r3, #0x14]
00394678: mov r1, #0
0039467c: ldrh r2, [r3]
00394680: str r1, [r3, #0x8c]
00394684: bic r2, r2, #8
00394688: strh r2, [r3]
0039468c: ldr r0, [r4, #0x2d8]
00394690: cmp r0, #0
00394694: beq #0x39452c
00394698: bl #0x470cb8
0039469c: b #0x39452c
003946a0: ldr r3, [r3, #0x3c]
003946a4: cmp r3, #3
003946a8: beq #0x394964
003946ac: cmp r3, #4
003946b0: beq #0x3946bc
003946b4: cmp r3, #1
003946b8: bne #0x39462c
003946bc: ldr r3, [pc, #0x2e4]
003946c0: ldr r0, [r5, r3]
003946c4: bl #0x31f594
003946c8: ldr r5, [r0, #0x128]
003946cc: ldrb r3, [r5, #0x25]
003946d0: cmp r3, #0
003946d4: beq #0x39462c
003946d8: ldr r4, [r4, #0x2e0]
003946dc: mov r0, sp
003946e0: ldr r1, [r5, #4]
003946e4: bl #0x597180
003946e8: ldr r1, [sp]
003946ec: ldr r0, [r4, #0xc]
003946f0: bl #0x30e3ac
003946f4: ldr r1, [sp, #4]
003946f8: mov r8, r0
003946fc: ldr r0, [r4, #0x10]
00394700: bl #0x30e3ac
00394704: ldr r1, [sp, #8]
00394708: mov r7, r0
0039470c: ldr r0, [r4, #0x14]
00394710: bl #0x30e3ac
00394714: mov r1, r8
00394718: mov r6, r0
0039471c: mov r0, r8
00394720: bl #0x30ed6c
00394724: mov r1, r7
00394728: mov r4, r0
0039472c: mov r0, r7
00394730: bl #0x30ed6c
00394734: mov r1, r0
00394738: mov r0, r4
0039473c: bl #0x30eba4
00394740: mov r1, r6
00394744: mov r4, r0
00394748: mov r0, r6
0039474c: bl #0x30ed6c
00394750: mov r1, r0
00394754: mov r0, r4
00394758: bl #0x30eba4
0039475c: mov r1, #0x40000000
00394760: add r1, r1, #0x400000
00394764: bl #0x30e9ac
00394768: cmp r0, #0
0039476c: beq #0x39462c
00394770: mov r0, r5
00394774: mov r1, #0
00394778: bl #0x41161c
0039477c: b #0x39462c
00394780: add r0, sp, #0x18
00394784: bl #0x46e818
00394788: ldr sl, [sp, #0x18]
0039478c: ldr r1, [r4, #0x160]
00394790: mov r0, sl
00394794: bl #0x30e3ac
00394798: mov r1, #0x3f800000
0039479c: bic r0, r0, #0x80000000
003947a0: bl #0x30e2f8
003947a4: cmp r0, #0
003947a8: bne #0x3947cc
003947ac: ldr r1, [r4, #0x164]
003947b0: ldr r0, [sp, #0x1c]
003947b4: bl #0x30e3ac
003947b8: mov r1, #0x3f800000
003947bc: bic r0, r0, #0x80000000
003947c0: bl #0x30e2f8
003947c4: cmp r0, #0
003947c8: beq #0x39446c
003947cc: ldr r2, [sp, #0x1c]
003947d0: ldr r3, [r4]
003947d4: str sl, [r4, #0x160]
003947d8: str r2, [r4, #0x164]
003947dc: mov r0, r4
003947e0: mov lr, pc
003947e4: ldr pc, [r3, #0x74]
003947e8: cmp r0, #1
003947ec: movne sl, #1
003947f0: bne #0x394470
003947f4: ldr r3, [pc, #0x1b0]
003947f8: add r1, r4, #0x160
003947fc: add r2, r4, #0x1c8
00394800: ldr r0, [r5, r3]
00394804: bl #0x525d84
00394808: subs sl, r0, #0
0039480c: bne #0x394470
00394810: ldr r0, [r4, #0x2dc]
00394814: ldr r1, [r4, #0x160]
00394818: ldr r2, [r4, #0x164]
0039481c: bl #0x46ea80
00394820: b #0x394470
00394824: ldr r3, [r4]
00394828: mov r0, r4
0039482c: mov lr, pc
00394830: ldr pc, [r3, #0x78]
00394834: cmp r0, #0
00394838: bne #0x39491c
0039483c: add r7, r4, #0x160
00394840: ldr r3, [pc, #0x164]
00394844: mov r1, r7
00394848: add r2, r4, #0x1c8
0039484c: ldr r0, [r5, r3]
00394850: bl #0x525d84
00394854: cmp r0, #0
00394858: bne #0x394874
0039485c: ldr r0, [r4, #0x2dc]
00394860: cmp r0, #0
00394864: beq #0x394874
00394868: ldr r1, [r4, #0x160]
0039486c: ldr r2, [r4, #0x164]
00394870: bl #0x46ea80
00394874: ldr r0, [r4, #0x2d8]
00394878: cmp r0, #0
0039487c: beq #0x3945d8
00394880: bl #0x470cb8
00394884: b #0x3945d8
00394888: mov r1, #0
0039488c: ldr r0, [r4, #0x2dc]
00394890: mov r2, r1
00394894: bl #0x46e918
00394898: b #0x39450c
0039489c: ldr r3, [r4]
003948a0: mov r0, r4
003948a4: mov lr, pc
003948a8: ldr pc, [r3, #0xa8]
003948ac: mov r1, #0x43000000
003948b0: mov sb, r0
003948b4: add r1, r1, #0xc80000
003948b8: mov r0, r8
003948bc: bl #0x30e2f8
003948c0: cmp r0, #0
003948c4: beq #0x394980
003948c8: add r0, sp, #0xc
003948cc: bl #0x34d0b0
003948d0: ldr r1, [sp, #0xc]
003948d4: mov r0, sb
003948d8: bl #0x30ed6c
003948dc: ldr r1, [sp, #0x10]
003948e0: mov fp, r0
003948e4: mov r0, sb
003948e8: str fp, [sp, #0xc]
003948ec: bl #0x30ed6c
003948f0: mov r1, sb
003948f4: mov r8, r0
003948f8: ldr r0, [sp, #0x14]
003948fc: str r8, [sp, #0x10]
00394900: bl #0x30ed6c
00394904: str r0, [sp, #0x14]
00394908: mov r1, fp
0039490c: mov r2, r8
00394910: ldr r0, [r4, #0x2dc]
00394914: bl #0x46e918
00394918: b #0x39450c
0039491c: ldr r3, [pc, #0x84]
00394920: ldr r0, [r5, r3]
00394924: bl #0x31f594
00394928: cmp r0, #0
0039492c: beq #0x39483c
00394930: add r7, r4, #0x160
00394934: mov r1, r7
00394938: add r2, r4, #0x1b8
0039493c: bl #0x3f94f4
00394940: cmp r0, #0
00394944: bne #0x394840
00394948: ldr r1, [r4, #0x1e0]
0039494c: ldr r2, [r4, #0x1e4]
00394950: ldr r3, [r4, #0x1e8]
00394954: str r1, [r4, #0x160]
00394958: str r2, [r4, #0x164]
0039495c: str r3, [r4, #0x168]
00394960: b #0x39485c
00394964: ldr r3, [pc, #0x3c]
00394968: ldr r0, [r5, r3]
0039496c: bl #0x31f594
00394970: mov r1, #1
00394974: ldr r0, [r0, #0x128]
00394978: bl #0x41161c
0039497c: b #0x39462c
00394980: mov r1, #0
00394984: ldr r0, [r4, #0x2dc]
00394988: mov r2, r1
0039498c: bl #0x46e918
00394990: ldr r0, [r4, #0x2dc]
00394994: ldr r1, [r4, #0x160]
00394998: ldr r2, [r4, #0x164]
0039499c: bl #0x46ea80
003949a0: b #0x39450c
003949a4: rsbeq r0, r0, ip, lsr #13
003949a8: strdeq r3, r4, [r0], -r4
003949ac: andeq r1, r0, r4, lsl #4
