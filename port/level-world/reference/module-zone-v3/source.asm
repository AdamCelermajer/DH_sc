
# _ZN10GameObject15SetRelativeAABBERK4aabbIfEb
0038b110: push {r4, r5, r6, r7, r8, lr}
0038b114: ldr r5, [r1]
0038b118: mov r3, r1
0038b11c: mov r4, r0
0038b120: str r5, [r0, #0x144]
0038b124: ldr r7, [r1, #4]
0038b128: mov r1, r5
0038b12c: str r7, [r0, #0x148]
0038b130: ldr r2, [r3, #8]
0038b134: str r2, [r0, #0x14c]
0038b138: ldr r0, [r3, #0xc]
0038b13c: str r0, [r4, #0x150]
0038b140: ldr r6, [r3, #0x10]
0038b144: str r6, [r4, #0x154]
0038b148: ldr r3, [r3, #0x14]
0038b14c: str r3, [r4, #0x158]
0038b150: bl #0x30e3ac
0038b154: mov r1, #0
0038b158: mov r8, r0
0038b15c: bl #0x30df8c
0038b160: cmp r0, #0
0038b164: beq #0x38b188
0038b168: mov r1, r7
0038b16c: mov r0, r6
0038b170: bl #0x30e3ac
0038b174: mov r1, #0
0038b178: bl #0x30df8c
0038b17c: cmp r0, #0
0038b180: movne r3, #1
0038b184: strbne r3, [r4, #0x2f9]
0038b188: mov r1, #0x41000000
0038b18c: mov r0, r8
0038b190: add r1, r1, #0x200000
0038b194: bl #0x30e70c
0038b198: cmp r0, #0
0038b19c: beq #0x38b1c8
0038b1a0: mov r1, #0x40000000
0038b1a4: add r1, r1, #0xa00000
0038b1a8: mov r0, r5
0038b1ac: bl #0x30e3ac
0038b1b0: mov r1, #0x40000000
0038b1b4: str r0, [r4, #0x144]
0038b1b8: add r1, r1, #0xa00000
0038b1bc: ldr r0, [r4, #0x150]
0038b1c0: bl #0x30eba4
0038b1c4: str r0, [r4, #0x150]
0038b1c8: ldr r5, [r4, #0x148]
0038b1cc: ldr r0, [r4, #0x154]
0038b1d0: mov r1, r5
0038b1d4: bl #0x30e3ac
0038b1d8: mov r1, #0x41000000
0038b1dc: add r1, r1, #0x200000
0038b1e0: bl #0x30e70c
0038b1e4: cmp r0, #0
0038b1e8: beq #0x38b214
0038b1ec: mov r1, #0x40000000
0038b1f0: add r1, r1, #0xa00000
0038b1f4: mov r0, r5
0038b1f8: bl #0x30e3ac
0038b1fc: mov r1, #0x40000000
0038b200: str r0, [r4, #0x148]
0038b204: add r1, r1, #0xa00000
0038b208: ldr r0, [r4, #0x154]
0038b20c: bl #0x30eba4
0038b210: str r0, [r4, #0x154]
0038b214: mov r0, r4
0038b218: bl #0x38aac8
0038b21c: mov r0, r4
0038b220: pop {r4, r5, r6, r7, r8, lr}
0038b224: b #0x393ea0

# _ZN10GameObject14UpdatePFObjectEv
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

# _ZN10GameObject21CheckSpawnProbabilityEv
0038bd64: push {r4, r5, lr}
0038bd68: sub sp, sp, #0x14
0038bd6c: add r4, sp, #4
0038bd70: mov r1, r0
0038bd74: mov r5, r0
0038bd78: mov r0, r4
0038bd7c: bl #0x33dd2c
0038bd80: mov r0, r4
0038bd84: bl #0x33ff54
0038bd88: ldr r4, [pc, #0xc4]
0038bd8c: subs r3, r0, #0
0038bd90: add r4, pc, r4
0038bd94: beq #0x38bdbc
0038bd98: ldr r3, [r3]
0038bd9c: mov lr, pc
0038bda0: ldr pc, [r3, #0x28]
0038bda4: cmp r0, #0
0038bda8: beq #0x38bdbc
0038bdac: mvn r0, #1
0038bdb0: str r0, [r5, #0x270]
0038bdb4: add sp, sp, #0x14
0038bdb8: pop {r4, r5, pc}
0038bdbc: ldr r0, [r5, #0x270]
0038bdc0: cmn r0, #1
0038bdc4: bne #0x38bdb4
0038bdc8: bl #0x7fd794
0038bdcc: ldrb r3, [r0, #5]
0038bdd0: cmp r3, #0
0038bdd4: beq #0x38be44
0038bdd8: ldr r3, [r5, #0x108]
0038bddc: cmn r3, #1
0038bde0: beq #0x38be44
0038bde4: ldr r0, [r5, #0xfc]
0038bde8: subs r0, r0, #0
0038bdec: movne r0, #1
0038bdf0: bl #0x38bc3c
0038bdf4: str r0, [r5, #0x270]
0038bdf8: ldr r3, [r5, #0x274]
0038bdfc: cmp r0, r3
0038be00: blt #0x38bdac
0038be04: mov r1, #0
0038be08: ldr r3, [r5]
0038be0c: mov r0, r5
0038be10: mov lr, pc
0038be14: ldr pc, [r3, #0x40]
0038be18: mov r0, r5
0038be1c: bl #0x33ddb4
0038be20: mov r3, #0
0038be24: strb r3, [r5, #0x82]
0038be28: ldr r3, [pc, #0x28]
0038be2c: mov r1, r5
0038be30: ldr r3, [r4, r3]
0038be34: ldr r0, [r3, #0x38]
0038be38: bl #0x3432f8
0038be3c: ldr r0, [r5, #0x270]
0038be40: b #0x38bdb4
0038be44: mov r0, #0
0038be48: bl #0x38bc3c
0038be4c: str r0, [r5, #0x270]
0038be50: b #0x38bdf8
0038be54: rsbeq r8, r0, r0, lsl #26
0038be58: strdeq r3, r4, [r0], -r4

# _ZN8RoomZone17DeclarePropertiesEv
00396554: b #0x397df8

# _ZN8RoomZoneC1EN10ObjectBase6GO_IDSE
00396558: push {r4, r5, r6, lr}
0039655c: mov r2, #0
00396560: mov r3, #1
00396564: ldr r5, [pc, #0x50]
00396568: mov r4, r0
0039656c: bl #0x397ca0
00396570: ldr r3, [pc, #0x48]
00396574: add r5, pc, r5
00396578: mov r1, #1
0039657c: ldr r3, [r5, r3]
00396580: add r2, r4, #0x394
00396584: strb r1, [r4, #0x389]
00396588: add r0, r3, #0xf4
0039658c: add ip, r3, #8
00396590: add r3, r3, #0xe8
00396594: str r3, [r4, #4]
00396598: mov r3, #0
0039659c: str r0, [r4, #0x24]
003965a0: str ip, [r4]
003965a4: strb r3, [r4, #0x390]
003965a8: str r2, [r4, #0x398]
003965ac: strb r1, [r4, #0x388]
003965b0: str r2, [r4, #0x394]
003965b4: mov r0, r4
003965b8: pop {r4, r5, r6, pc}
003965bc: subseq lr, pc, ip, lsl r5
003965c0: andeq r2, r0, r4, ror r7

# _ZN4ZoneC1EN10ObjectBase6GO_IDSEbb
00397c2c: push {r4, r5, r6, r7, r8, lr}
00397c30: ldr r4, [pc, #0x60]
00397c34: mov r6, r0
00397c38: mov r5, r2
00397c3c: mov r7, r3
00397c40: bl #0x38c398
00397c44: ldr r3, [pc, #0x50]
00397c48: add r4, pc, r4
00397c4c: mov r2, #0
00397c50: ldr r3, [r4, r3]
00397c54: str r2, [r6, #0x37c]
00397c58: strb r5, [r6, #0x380]
00397c5c: add r1, r3, #0xf4
00397c60: add r0, r3, #8
00397c64: add r3, r3, #0xe8
00397c68: str r3, [r6, #4]
00397c6c: mov r3, #0
00397c70: str r3, [r6, #0x384]
00397c74: mov r3, #1
00397c78: str r0, [r6]
00397c7c: str r1, [r6, #0x24]
00397c80: strb r7, [r6, #0x381]
00397c84: strb r3, [r6, #0x84]
00397c88: str r2, [r6, #0x374]
00397c8c: str r2, [r6, #0x378]
00397c90: mov r0, r6
00397c94: pop {r4, r5, r6, r7, r8, pc}
00397c98: subseq ip, pc, r8, asr #28
00397c9c: andeq r0, r0, r4, lsr sp

# _ZN4Zone19InitWithBoundingBoxERKN6glitch4core8aabbox3dIfEE
00397594: push {r4, r5, r6, r7, r8, sl, lr}
00397598: mov r8, r1
0039759c: sub sp, sp, #0x34
003975a0: ldr r1, [r1, #4]
003975a4: mov r4, r0
003975a8: ldr r0, [r8, #0x10]
003975ac: bl #0x30e3ac
003975b0: ldr r1, [r8, #8]
003975b4: mov r6, r0
003975b8: ldr r0, [r8, #0x14]
003975bc: bl #0x30e3ac
003975c0: ldr r1, [r8]
003975c4: mov r5, r0
003975c8: ldr r0, [r8, #0xc]
003975cc: bl #0x30e3ac
003975d0: str r6, [r4, #0x378]
003975d4: str r5, [r4, #0x37c]
003975d8: str r0, [r4, #0x374]
003975dc: ldr r3, [r8]
003975e0: ldr r5, [pc, #0x128]
003975e4: str r3, [r4, #0x144]
003975e8: ldr r3, [r8, #4]
003975ec: add r5, pc, r5
003975f0: str r3, [r4, #0x148]
003975f4: ldr r3, [r8, #8]
003975f8: str r3, [r4, #0x14c]
003975fc: ldr r3, [r8, #0xc]
00397600: str r3, [r4, #0x150]
00397604: ldr r3, [r8, #0x10]
00397608: str r3, [r4, #0x154]
0039760c: ldr r3, [r8, #0x14]
00397610: str r3, [r4, #0x158]
00397614: ldr r1, [r8, #0x10]
00397618: ldr r0, [r8, #4]
0039761c: bl #0x30eba4
00397620: mov r1, #0x3f000000
00397624: bl #0x30ed6c
00397628: ldr r1, [r8, #0x14]
0039762c: mov r7, r0
00397630: ldr r0, [r8, #8]
00397634: bl #0x30eba4
00397638: mov r1, #0x3f000000
0039763c: bl #0x30ed6c
00397640: ldr r1, [r8, #0xc]
00397644: mov r6, r0
00397648: ldr r0, [r8]
0039764c: bl #0x30eba4
00397650: mov r1, #0x3f000000
00397654: bl #0x30ed6c
00397658: add r1, sp, #0x24
0039765c: str r0, [sp, #0x24]
00397660: mov r2, #1
00397664: mov r0, r4
00397668: str r7, [sp, #0x28]
0039766c: str r6, [sp, #0x2c]
00397670: bl #0x393db4
00397674: ldrb r3, [r4, #0x380]
00397678: cmp r3, #0
0039767c: beq #0x397708
00397680: ldr r3, [pc, #0x8c]
00397684: mov r1, #0
00397688: mov r0, #0x28
0039768c: ldr r3, [r5, r3]
00397690: mov r6, r1
00397694: ldr sl, [r3, #0x44]
00397698: bl #0x310570
0039769c: ldrb r8, [r4, #0x381]
003976a0: mov ip, #1
003976a4: movw r3, #0x51e
003976a8: cmp r8, r6
003976ac: mvn lr, #4
003976b0: moveq r8, r3
003976b4: movne r8, #4
003976b8: mov r1, sl
003976bc: mov r3, ip
003976c0: mov r2, r4
003976c4: str lr, [sp, #0xc]
003976c8: mov lr, #0x800
003976cc: mov r7, r0
003976d0: str lr, [sp, #0x10]
003976d4: str r8, [sp, #0x14]
003976d8: stm sp, {r6, ip}
003976dc: str r6, [sp, #8]
003976e0: str r6, [sp, #0x18]
003976e4: bl #0x46f2f0
003976e8: ldr r3, [pc, #0x28]
003976ec: mov r0, r4
003976f0: mov r1, r7
003976f4: ldr r3, [r5, r3]
003976f8: mov r2, r6
003976fc: add r3, r3, #8
00397700: str r3, [r7]
00397704: bl #0x394bf8
00397708: add sp, sp, #0x34
0039770c: pop {r4, r5, r6, r7, r8, sl, pc}
00397710: subseq sp, pc, r4, lsr #9
00397714: strdeq r3, r4, [r0], -r4
00397718: andeq r3, r0, r0, lsl #13

# _ZN10ObjectBase19TestEnableConditionEb
0033e6d4: push {r4, r5, r6, r7, r8, lr}
0033e6d8: ldr r4, [pc, #0xe0]
0033e6dc: ldr r6, [pc, #0xe0]
0033e6e0: mov r5, r0
0033e6e4: add r4, pc, r4
0033e6e8: ldr r3, [r4, r6]
0033e6ec: mov r7, r1
0033e6f0: mov r2, #1
0033e6f4: ldr r0, [r3, #0x40]
0033e6f8: mov r1, #0
0033e6fc: bl #0x36e478
0033e700: ldr r3, [r0, #0x660]
0033e704: cmp r3, #0
0033e708: beq #0x33e728
0033e70c: movw r2, #0x14e8
0033e710: ldr r3, [r3, r2]
0033e714: cmp r3, #0
0033e718: beq #0x33e768
0033e71c: ldrb r3, [r3, #0x14]
0033e720: cmp r3, #0
0033e724: beq #0x33e768
0033e728: ldr r0, [r4, r6]
0033e72c: bl #0x31f594
0033e730: cmp r0, #0
0033e734: beq #0x33e770
0033e738: ldr r3, [r5, #0xec]
0033e73c: ldr r2, [r0, #0x118]
0033e740: ldrb r1, [r5, #0xf1]
0033e744: cmn r3, #1
0033e748: moveq r3, #0
0033e74c: cmp r3, r2
0033e750: ble #0x33e7b8
0033e754: mov r0, r5
0033e758: mov r1, #0
0033e75c: bl #0x33ddc8
0033e760: ldrb r0, [r5, #0x8a]
0033e764: pop {r4, r5, r6, r7, r8, pc}
0033e768: ldrb r0, [r5, #0x8a]
0033e76c: pop {r4, r5, r6, r7, r8, pc}
0033e770: ldrb r1, [r5, #0xf1]
0033e774: eor r1, r1, #1
0033e778: cmp r1, #0
0033e77c: beq #0x33e754
0033e780: add r4, r5, #0x8c
0033e784: mov r0, r4
0033e788: bl #0x33e5f8
0033e78c: cmp r0, #0
0033e790: beq #0x33e754
0033e794: mov r0, r5
0033e798: mov r1, #1
0033e79c: bl #0x33ddc8
0033e7a0: cmp r7, #0
0033e7a4: beq #0x33e760
0033e7a8: mov r0, r4
0033e7ac: mov r1, #1
0033e7b0: bl #0x33dd24
0033e7b4: b #0x33e760
0033e7b8: eor r1, r1, #1
0033e7bc: b #0x33e778
0033e7c0: rsbeq r6, r5, ip, lsr #7
0033e7c4: strdeq r3, r4, [r0], -r4

# _ZN13RootSceneNode18RefreshBoundingBoxEv
0035c854: push {r4, r5, r6, lr}
0035c858: add r1, r0, #0x130
0035c85c: mov r4, r0
0035c860: bl #0x65cf8c
0035c864: ldr r3, [r4]
0035c868: mov r0, r4
0035c86c: mov lr, pc
0035c870: ldr pc, [r3, #0xa0]
0035c874: ldr r1, [r0]
0035c878: mov r5, r0
0035c87c: ldr r0, [r4, #0x130]
0035c880: bl #0x30e3ac
0035c884: str r0, [r4, #0x130]
0035c888: ldr r1, [r5, #4]
0035c88c: ldr r0, [r4, #0x134]
0035c890: bl #0x30e3ac
0035c894: str r0, [r4, #0x134]
0035c898: ldr r1, [r5, #8]
0035c89c: ldr r0, [r4, #0x138]
0035c8a0: bl #0x30e3ac
0035c8a4: ldr r3, [r4]
0035c8a8: str r0, [r4, #0x138]
0035c8ac: mov r0, r4
0035c8b0: mov lr, pc
0035c8b4: ldr pc, [r3, #0xa0]
0035c8b8: ldr r1, [r0]
0035c8bc: mov r5, r0
0035c8c0: ldr r0, [r4, #0x13c]
0035c8c4: bl #0x30e3ac
0035c8c8: str r0, [r4, #0x13c]
0035c8cc: ldr r1, [r5, #4]
0035c8d0: ldr r0, [r4, #0x140]
0035c8d4: bl #0x30e3ac
0035c8d8: str r0, [r4, #0x140]
0035c8dc: ldr r1, [r5, #8]
0035c8e0: ldr r0, [r4, #0x144]
0035c8e4: bl #0x30e3ac
0035c8e8: str r0, [r4, #0x144]
0035c8ec: pop {r4, r5, r6, pc}

# _ZN6glitch7collada10CSceneNode18computeBoundingBoxEv
0065cda8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065cdac: movw r3, #0x6164
0065cdb0: sub sp, sp, #0x24
0065cdb4: movt r3, #0x4d65
0065cdb8: mov r7, r0
0065cdbc: str r3, [sp, #4]
0065cdc0: ldr r4, [r7, #0xf4]!
0065cdc4: movw sl, #0x6164
0065cdc8: movw r8, #0x6164
0065cdcc: movw r6, #0x6164
0065cdd0: add r3, sp, #8
0065cdd4: cmp r7, r4
0065cdd8: mov r5, r0
0065cddc: movt sl, #0x7365
0065cde0: movt r8, #0x6d65
0065cde4: movt r6, #0x6e65
0065cde8: add fp, r0, #0x130
0065cdec: mov sb, #0
0065cdf0: str r3, [sp]
0065cdf4: beq #0x65cec0
0065cdf8: cmp r4, #0
0065cdfc: moveq r3, r4
0065ce00: subne r3, r4, #4
0065ce04: mov r0, r3
0065ce08: ldr r3, [r3]
0065ce0c: mov lr, pc
0065ce10: ldr pc, [r3, #0xbc]
0065ce14: cmp r0, r8
0065ce18: cmpne r0, sl
0065ce1c: beq #0x65ce30
0065ce20: ldr r3, [sp, #4]
0065ce24: cmp r0, r3
0065ce28: cmpne r0, r6
0065ce2c: bne #0x65ceb4
0065ce30: cmp r0, r6
0065ce34: beq #0x65cf4c
0065ce38: cmp sb, #0
0065ce3c: bne #0x65cec8
0065ce40: cmp r4, #0
0065ce44: moveq r3, r4
0065ce48: subne r3, r4, #4
0065ce4c: mov r0, r3
0065ce50: ldr r3, [r3]
0065ce54: mov lr, pc
0065ce58: ldr pc, [r3, #0x30]
0065ce5c: ldr r3, [r0]
0065ce60: cmp r4, #0
0065ce64: mov sb, #1
0065ce68: str r3, [r5, #0x130]
0065ce6c: ldr r3, [r0, #4]
0065ce70: str r3, [r5, #0x134]
0065ce74: ldr r3, [r0, #8]
0065ce78: str r3, [r5, #0x138]
0065ce7c: ldr r3, [r0, #0xc]
0065ce80: str r3, [r5, #0x13c]
0065ce84: ldr r3, [r0, #0x10]
0065ce88: str r3, [r5, #0x140]
0065ce8c: ldr r3, [r0, #0x14]
0065ce90: str r3, [r5, #0x144]
0065ce94: moveq r3, r4
0065ce98: subne r3, r4, #4
0065ce9c: mov r0, r3
0065cea0: ldr r3, [r3]
0065cea4: mov lr, pc
0065cea8: ldr pc, [r3, #0x40]
0065ceac: mov r1, fp
0065ceb0: bl #0x597548
0065ceb4: ldr r4, [r4]
0065ceb8: cmp r7, r4
0065cebc: bne #0x65cdf8
0065cec0: add sp, sp, #0x24
0065cec4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065cec8: cmp r4, #0
0065cecc: moveq r3, r4
0065ced0: subne r3, r4, #4
0065ced4: mov r0, r3
0065ced8: ldr r3, [r3]
0065cedc: mov lr, pc
0065cee0: ldr pc, [r3, #0x30]
0065cee4: ldr r3, [r0]
0065cee8: cmp r4, #0
0065ceec: str r3, [sp, #8]
0065cef0: ldr r3, [r0, #4]
0065cef4: str r3, [sp, #0xc]
0065cef8: ldr r3, [r0, #8]
0065cefc: str r3, [sp, #0x10]
0065cf00: ldr r3, [r0, #0xc]
0065cf04: str r3, [sp, #0x14]
0065cf08: ldr r3, [r0, #0x10]
0065cf0c: str r3, [sp, #0x18]
0065cf10: ldr r3, [r0, #0x14]
0065cf14: str r3, [sp, #0x1c]
0065cf18: moveq r3, r4
0065cf1c: subne r3, r4, #4
0065cf20: mov r0, r3
0065cf24: ldr r3, [r3]
0065cf28: mov lr, pc
0065cf2c: ldr pc, [r3, #0x40]
0065cf30: ldr r1, [sp]
0065cf34: bl #0x597548
0065cf38: mov r0, fp
0065cf3c: ldr r1, [sp]
0065cf40: bl #0x35c150
0065cf44: ldr r4, [r4]
0065cf48: b #0x65ceb8
0065cf4c: cmp r4, #0
0065cf50: moveq r3, r4
0065cf54: subne r3, r4, #4
0065cf58: mov r0, r3
0065cf5c: ldr r3, [r3]
0065cf60: mov lr, pc
0065cf64: ldr pc, [r3, #0xf4]
0065cf68: b #0x65ce38

# _ZNK6glitch7collada10CSceneNode18computeBoundingBoxERNS_4core8aabbox3dIfEE
0065cf8c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065cf90: sub sp, sp, #0x24
0065cf94: mov r5, r1
0065cf98: bl #0x5971c8
0065cf9c: mov r6, r0
0065cfa0: ldr r4, [r6, #4]!
0065cfa4: movw r8, #0x6164
0065cfa8: movw r7, #0x6164
0065cfac: movw sb, #0x6164
0065cfb0: movw fp, #0x6164
0065cfb4: add r3, sp, #8
0065cfb8: cmp r6, r4
0065cfbc: movt r8, #0x7365
0065cfc0: movt r7, #0x6d65
0065cfc4: movt sb, #0x4d65
0065cfc8: movt fp, #0x6e65
0065cfcc: mov sl, #0
0065cfd0: str r3, [sp, #4]
0065cfd4: beq #0x65d01c
0065cfd8: cmp r4, #0
0065cfdc: moveq r3, r4
0065cfe0: subne r3, r4, #4
0065cfe4: mov r0, r3
0065cfe8: ldr r3, [r3]
0065cfec: mov lr, pc
0065cff0: ldr pc, [r3, #0xbc]
0065cff4: cmp r0, r7
0065cff8: cmpne r0, r8
0065cffc: beq #0x65d0ac
0065d000: cmp r0, sb
0065d004: beq #0x65d0ac
0065d008: cmp r0, fp
0065d00c: beq #0x65d028
0065d010: ldr r4, [r4]
0065d014: cmp r6, r4
0065d018: bne #0x65cfd8
0065d01c: mov r0, sl
0065d020: add sp, sp, #0x24
0065d024: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065d028: mov r3, #0xbf000000
0065d02c: cmp r4, #0
0065d030: add r3, r3, #0x800000
0065d034: str r3, [sp, #8]
0065d038: str r3, [sp, #0xc]
0065d03c: str r3, [sp, #0x10]
0065d040: moveq r0, r4
0065d044: mov r3, #0x3f800000
0065d048: subne r0, r4, #4
0065d04c: ldr r1, [sp, #4]
0065d050: str r3, [sp, #0x14]
0065d054: str r3, [sp, #0x18]
0065d058: str r3, [sp, #0x1c]
0065d05c: bl #0x65cf8c
0065d060: cmp r0, #0
0065d064: beq #0x65d010
0065d068: cmp sl, #0
0065d06c: bne #0x65d13c
0065d070: ldr sl, [sp, #8]
0065d074: ldr ip, [sp, #0xc]
0065d078: ldr r0, [sp, #0x10]
0065d07c: ldr r1, [sp, #0x14]
0065d080: ldr r2, [sp, #0x18]
0065d084: ldr r3, [sp, #0x1c]
0065d088: str sl, [r5]
0065d08c: str ip, [r5, #4]
0065d090: str r0, [r5, #8]
0065d094: str r1, [r5, #0xc]
0065d098: str r2, [r5, #0x10]
0065d09c: str r3, [r5, #0x14]
0065d0a0: mov sl, #1
0065d0a4: ldr r4, [r4]
0065d0a8: b #0x65d014
0065d0ac: cmp sl, #0
0065d0b0: bne #0x65d10c
0065d0b4: cmp r4, #0
0065d0b8: moveq r3, r4
0065d0bc: subne r3, r4, #4
0065d0c0: mov r0, r3
0065d0c4: ldr r3, [r3]
0065d0c8: mov lr, pc
0065d0cc: ldr pc, [r3, #0x34]
0065d0d0: ldr r3, [r0]
0065d0d4: mov sl, #1
0065d0d8: str r3, [r5]
0065d0dc: ldr r3, [r0, #4]
0065d0e0: str r3, [r5, #4]
0065d0e4: ldr r3, [r0, #8]
0065d0e8: str r3, [r5, #8]
0065d0ec: ldr r3, [r0, #0xc]
0065d0f0: str r3, [r5, #0xc]
0065d0f4: ldr r3, [r0, #0x10]
0065d0f8: str r3, [r5, #0x10]
0065d0fc: ldr r3, [r0, #0x14]
0065d100: str r3, [r5, #0x14]
0065d104: ldr r4, [r4]
0065d108: b #0x65d014
0065d10c: cmp r4, #0
0065d110: moveq r3, r4
0065d114: subne r3, r4, #4
0065d118: mov r0, r3
0065d11c: ldr r3, [r3]
0065d120: mov lr, pc
0065d124: ldr pc, [r3, #0x34]
0065d128: mov r1, r0
0065d12c: mov r0, r5
0065d130: bl #0x35c150
0065d134: ldr r4, [r4]
0065d138: b #0x65d014
0065d13c: mov r0, r5
0065d140: ldr r1, [sp, #4]
0065d144: bl #0x35c150
0065d148: ldr r4, [r4]
0065d14c: b #0x65d014

# _ZNK6glitch5scene10ISceneNode25getTransformedBoundingBoxEv
0059770c: push {r4, r5, r6, lr}
00597710: ldr r3, [r0, #0x11c]
00597714: mov r4, r0
00597718: tst r3, #0x100
0059771c: addeq r5, r0, #0xd4
00597720: beq #0x597780
00597724: ldr r3, [r0]
00597728: mov lr, pc
0059772c: ldr pc, [r3, #0x30]
00597730: ldr r2, [r0]
00597734: mov r3, r0
00597738: add r5, r4, #0xd4
0059773c: str r2, [r4, #0xd4]
00597740: ldr r2, [r3, #4]
00597744: add r0, r4, #0x24
00597748: mov r1, r5
0059774c: str r2, [r4, #0xd8]
00597750: ldr r2, [r3, #8]
00597754: str r2, [r4, #0xdc]
00597758: ldr r2, [r3, #0xc]
0059775c: str r2, [r4, #0xe0]
00597760: ldr r2, [r3, #0x10]
00597764: str r2, [r4, #0xe4]
00597768: ldr r3, [r3, #0x14]
0059776c: str r3, [r4, #0xe8]
00597770: bl #0x597548
00597774: ldr r3, [r4, #0x11c]
00597778: bic r3, r3, #0x100
0059777c: str r3, [r4, #0x11c]
00597780: mov r0, r5
00597784: pop {r4, r5, r6, pc}

# _ZN6glitch7collada10CSceneNodeC2ERKNS0_16CColladaDatabaseEPNS0_5SNodeE
0065d2b4: push {r4, r5, r6, r7, lr}
0065d2b8: mov r5, r2
0065d2bc: sub sp, sp, #0x2c
0065d2c0: mvn r2, #0
0065d2c4: mov r4, r1
0065d2c8: add r1, r1, #4
0065d2cc: mov r6, r0
0065d2d0: mov r7, r3
0065d2d4: bl #0x583b34
0065d2d8: ldr r2, [r5]
0065d2dc: ldr r3, [pc, #0xfc]
0065d2e0: str r2, [r6, #0x14c]
0065d2e4: ldr r1, [r5, #4]
0065d2e8: cmp r2, #0
0065d2ec: add r3, pc, r3
0065d2f0: str r1, [r6, #0x150]
0065d2f4: beq #0x65d308
0065d2f8: ldr r1, [r2, #4]
0065d2fc: cmp r1, #0
0065d300: addne r1, r1, #1
0065d304: strne r1, [r2, #4]
0065d308: ldr r2, [pc, #0xd4]
0065d30c: cmp r7, #0
0065d310: ldr r2, [r3, r2]
0065d314: add r2, r2, #4
0065d318: str r2, [r6, #0x148]
0065d31c: ldr r3, [r4]
0065d320: str r3, [r6]
0065d324: ldr r3, [r3, #-0x1c]
0065d328: ldr r2, [r4, #0x1c]
0065d32c: str r2, [r6, r3]
0065d330: ldr r3, [r6]
0065d334: ldr r2, [r4, #0x20]
0065d338: ldr r3, [r3, #-0xc]
0065d33c: str r2, [r6, r3]
0065d340: str r7, [r6, #0x154]
0065d344: beq #0x65d3d4
0065d348: ldr r1, [r7, #4]
0065d34c: mov r0, r6
0065d350: bl #0x598a04
0065d354: ldr r3, [r6, #0x154]
0065d358: mov r0, r6
0065d35c: add r1, sp, #0x1c
0065d360: ldr r2, [r3, #0xc]
0065d364: str r2, [sp, #0x1c]
0065d368: ldr r2, [r3, #0x10]
0065d36c: str r2, [sp, #0x20]
0065d370: ldr r3, [r3, #0x14]
0065d374: str r3, [sp, #0x24]
0065d378: bl #0x59712c
0065d37c: ldr r3, [r6, #0x154]
0065d380: mov r0, r6
0065d384: mov r1, sp
0065d388: ldr r2, [r3, #0x18]
0065d38c: str r2, [sp]
0065d390: ldr r2, [r3, #0x1c]
0065d394: str r2, [sp, #4]
0065d398: ldr r2, [r3, #0x20]
0065d39c: str r2, [sp, #8]
0065d3a0: ldr r3, [r3, #0x24]
0065d3a4: str r3, [sp, #0xc]
0065d3a8: bl #0x5970f4
0065d3ac: ldr r3, [r6, #0x154]
0065d3b0: mov r0, r6
0065d3b4: add r1, sp, #0x10
0065d3b8: ldr r2, [r3, #0x28]
0065d3bc: str r2, [sp, #0x10]
0065d3c0: ldr r2, [r3, #0x2c]
0065d3c4: str r2, [sp, #0x14]
0065d3c8: ldr r3, [r3, #0x30]
0065d3cc: str r3, [sp, #0x18]
0065d3d0: bl #0x5970c4
0065d3d4: mov r0, r6
0065d3d8: add sp, sp, #0x2c
0065d3dc: pop {r4, r5, r6, r7, pc}
0065d3e0: eorseq r7, r3, r4, lsr #15
0065d3e4: strheq r1, [r0], -r4

# _ZN13RootSceneNodeC1ERKN6glitch7collada16CColladaDatabaseE
0035d824: push {r4, r5, r6, lr}
0035d828: ldr r5, [pc, #0xd0]
0035d82c: ldr r3, [pc, #0xd0]
0035d830: ldr r2, [pc, #0xd0]
0035d834: add r5, pc, r5
0035d838: ldr r3, [r5, r3]
0035d83c: ldr r2, [r5, r2]
0035d840: mov r6, #1
0035d844: ldr ip, [r3, #0x3c]
0035d848: add r2, r2, #8
0035d84c: str r2, [r0, #0x20c]
0035d850: str r6, [r0, #0x210]
0035d854: str ip, [r0]
0035d858: ldr lr, [r3, #0x40]
0035d85c: ldr ip, [ip, #-0xc]
0035d860: mov r2, r1
0035d864: add r1, r3, #4
0035d868: str lr, [r0, ip]
0035d86c: mov r4, r0
0035d870: bl #0x65b844
0035d874: ldr r3, [pc, #0x90]
0035d878: add r2, r4, #0x1bc
0035d87c: mov r0, r2
0035d880: ldr r3, [r5, r3]
0035d884: str r2, [r4, #0x1cc]
0035d888: str r2, [r4, #0x1d0]
0035d88c: add r2, r3, #0x124
0035d890: add r3, r3, #0x1c
0035d894: str r3, [r4]
0035d898: str r2, [r4, #0x20c]
0035d89c: mov r1, #0x10
0035d8a0: bl #0x31167c
0035d8a4: ldr r2, [r4, #0x1cc]
0035d8a8: mov r5, #0
0035d8ac: add r3, r4, #0x1d4
0035d8b0: strb r5, [r2]
0035d8b4: mov r0, r3
0035d8b8: str r3, [r4, #0x1e4]
0035d8bc: str r3, [r4, #0x1e8]
0035d8c0: mov r1, #0x10
0035d8c4: bl #0x31167c
0035d8c8: ldr r3, [r4, #0x1e4]
0035d8cc: mov r0, r4
0035d8d0: strb r5, [r3]
0035d8d4: strb r6, [r4, #0x208]
0035d8d8: strb r5, [r4, #0x20a]
0035d8dc: strb r5, [r4, #0x1ec]
0035d8e0: str r5, [r4, #0x1f0]
0035d8e4: str r5, [r4, #0x1f4]
0035d8e8: str r5, [r4, #0x1f8]
0035d8ec: str r5, [r4, #0x1fc]
0035d8f0: strb r6, [r4, #0x200]
0035d8f4: str r5, [r4, #0x204]
0035d8f8: strb r5, [r4, #0x209]
0035d8fc: pop {r4, r5, r6, pc}
0035d900: rsbeq r7, r3, ip, asr r2
0035d904: strdeq r1, r2, [r0], -r4
0035d908: andeq r2, r0, r4, asr #22
0035d90c: andeq r3, r0, r8, lsl #18

# _ZN6glitch7collada14CRootSceneNodeC1ERKNS0_16CColladaDatabaseE
0065b734: push {r4, r5, r6, r7, r8, sb, sl, lr}
0065b738: ldr r5, [pc, #0xf4]
0065b73c: ldr r3, [pc, #0xf4]
0065b740: ldr r2, [pc, #0xf4]
0065b744: add r5, pc, r5
0065b748: ldr r3, [r5, r3]
0065b74c: ldr r2, [r5, r2]
0065b750: mov r6, #1
0065b754: ldr ip, [r3, #0x30]
0065b758: add r2, r2, #8
0065b75c: str r2, [r0, #0x1bc]
0065b760: str r6, [r0, #0x1c0]
0065b764: str ip, [r0]
0065b768: ldr lr, [r3, #0x34]
0065b76c: ldr ip, [ip, #-0xc]
0065b770: mov r2, r1
0065b774: add r1, r3, #4
0065b778: str lr, [r0, ip]
0065b77c: mov r3, #0
0065b780: mov r4, r0
0065b784: bl #0x65d2b4
0065b788: ldr r2, [pc, #0xb0]
0065b78c: mov r1, #0
0065b790: mov r3, r4
0065b794: ldr r2, [r5, r2]
0065b798: add lr, r4, #0x178
0065b79c: add ip, r4, #0x180
0065b7a0: add r0, r4, #0x188
0065b7a4: add sl, r4, #0x158
0065b7a8: add r8, r4, #0x160
0065b7ac: add sb, r2, #0x124
0065b7b0: add r7, r4, #0x168
0065b7b4: add r5, r4, #0x170
0065b7b8: add r2, r2, #0x1c
0065b7bc: str r2, [r4]
0065b7c0: str r0, [r4, #0x18c]
0065b7c4: str r0, [r4, #0x188]
0065b7c8: add r2, r4, #0x1b4
0065b7cc: str sb, [r4, #0x1bc]
0065b7d0: str sl, [r4, #0x15c]
0065b7d4: str r8, [r4, #0x164]
0065b7d8: str r7, [r4, #0x16c]
0065b7dc: str r5, [r4, #0x174]
0065b7e0: str lr, [r4, #0x17c]
0065b7e4: str ip, [r4, #0x184]
0065b7e8: str sl, [r4, #0x158]
0065b7ec: str r8, [r4, #0x160]
0065b7f0: str r7, [r4, #0x168]
0065b7f4: str r5, [r4, #0x170]
0065b7f8: str lr, [r4, #0x178]
0065b7fc: str ip, [r4, #0x180]
0065b800: str r1, [r4, #0x194]
0065b804: strb r1, [r3, #0x190]!
0065b808: mov r0, r4
0065b80c: str r3, [r4, #0x19c]
0065b810: str r6, [r4, #0x1ac]
0065b814: str r2, [r4, #0x1b8]
0065b818: str r3, [r4, #0x198]
0065b81c: str r1, [r4, #0x1a0]
0065b820: strb r1, [r4, #0x1a8]
0065b824: str r2, [r4, #0x1b4]
0065b828: bl #0x59719c
0065b82c: mov r0, r4
0065b830: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0065b834: eorseq sb, r3, ip, asr #6
0065b838: andeq r3, r0, r4, asr ip
0065b83c: andeq r2, r0, r4, asr #22
0065b840: andeq r1, r0, ip, lsl #11

# _ZN12VisualObject4SyncEv
0038ba74: push {r4, lr}
0038ba78: mov r4, r0
0038ba7c: bl #0x470cb8
0038ba80: mov r0, r4
0038ba84: bl #0x472948
0038ba88: mov r0, r4
0038ba8c: pop {r4, lr}
0038ba90: b #0x472860

# _ZN12VisualObject12SyncPositionEv
00470cb8: ldr r1, [r0, #4]
00470cbc: cmp r1, #0
00470cc0: bxeq lr
00470cc4: add r1, r1, #0x160
00470cc8: b #0x470c24

# _ZN12VisualObject12SyncRotationEv
00472948: ldr r1, [r0, #4]
0047294c: cmp r1, #0
00472950: bxeq lr
00472954: add r1, r1, #0x16c
00472958: b #0x472874

# _ZN12VisualObject11SyncScalingEv
00472860: ldr r1, [r0, #4]
00472864: cmp r1, #0
00472868: bxeq lr
0047286c: add r1, r1, #0x120
00472870: b #0x4727ac

# _ZN12VisualObjectC1EP10GameObjectRKSsS3_
00472a0c: push {r4, r5, r6, r7, r8, sl, lr}
00472a10: ldr r6, [pc, #0x234]
00472a14: ldr r5, [pc, #0x234]
00472a18: mov ip, #0xbf000000
00472a1c: add r6, pc, r6
00472a20: ldr r5, [r6, r5]
00472a24: mov lr, #0
00472a28: add ip, ip, #0x800000
00472a2c: mov r7, r1
00472a30: add r1, r5, #8
00472a34: mov r5, #0
00472a38: mov r8, r3
00472a3c: sub sp, sp, #0xc
00472a40: str r1, [r0]
00472a44: str lr, [r0, #0x24]
00472a48: str ip, [r0, #0x74]
00472a4c: str lr, [r0, #0x10]
00472a50: str lr, [r0, #0x14]
00472a54: str lr, [r0, #0x18]
00472a58: str lr, [r0, #0x1c]
00472a5c: str lr, [r0, #0x20]
00472a60: str ip, [r0, #0x58]
00472a64: str ip, [r0, #0x5c]
00472a68: str ip, [r0, #0x60]
00472a6c: str ip, [r0, #0x64]
00472a70: str ip, [r0, #0x68]
00472a74: str r7, [r0, #4]
00472a78: str r5, [r0, #8]
00472a7c: str r5, [r0, #0xc]
00472a80: strb r5, [r0, #0x28]
00472a84: str r5, [r0, #0x2c]
00472a88: str r5, [r0, #0x30]
00472a8c: str r5, [r0, #0x34]
00472a90: str r5, [r0, #0x38]
00472a94: strb r5, [r0, #0x3c]
00472a98: str r5, [r0, #0x40]
00472a9c: str r5, [r0, #0x44]
00472aa0: str r5, [r0, #0x48]
00472aa4: str r5, [r0, #0x4c]
00472aa8: str r5, [r0, #0x50]
00472aac: str r5, [r0, #0x54]
00472ab0: strb r5, [r0, #0x6c]
00472ab4: strb r5, [r0, #0x7c]
00472ab8: strb r5, [r0, #0x7d]
00472abc: strb r5, [r0, #0x7e]
00472ac0: strb r5, [r0, #0x7f]
00472ac4: str r5, [r0, #0x80]
00472ac8: str r5, [r0, #0x84]
00472acc: str r5, [r0, #0x88]
00472ad0: str r5, [r0, #0x8c]
00472ad4: str r5, [r0, #0x90]
00472ad8: str r5, [r0, #0x94]
00472adc: str r5, [r0, #0x9c]
00472ae0: str r5, [r0, #0xa0]
00472ae4: str r5, [r0, #0xa4]
00472ae8: strb r5, [r0, #0xa9]
00472aec: mov sl, r2
00472af0: mov r4, r0
00472af4: bl #0x50a564
00472af8: ldr ip, [r8, #0x10]
00472afc: ldr r2, [r8, #0x14]
00472b00: ldr r1, [sl, #0x14]
00472b04: mov r3, r5
00472b08: cmp ip, r2
00472b0c: moveq r2, r5
00472b10: mvn ip, #0x80000000
00472b14: str ip, [sp]
00472b18: bl #0x50a504
00472b1c: cmp r0, r5
00472b20: str r0, [r4, #8]
00472b24: beq #0x472c40
00472b28: mov r1, r7
00472b2c: mov r0, r4
00472b30: bl #0x47295c
00472b34: ldr r0, [r4, #8]
00472b38: bl #0x35c854
00472b3c: mov r0, r4
00472b40: bl #0x4718f0
00472b44: ldr r3, [pc, #0x108]
00472b48: ldr r1, [r4, #8]
00472b4c: ldr r5, [r6, r3]
00472b50: ldr r3, [r5, #0x10]
00472b54: ldr r3, [r3, #0x1c]
00472b58: ldr r3, [r3, #4]
00472b5c: mov r0, r3
00472b60: ldr r3, [r3]
00472b64: mov lr, pc
00472b68: ldr pc, [r3, #0x5c]
00472b6c: ldr r3, [r5, #0x10]
00472b70: ldr r0, [r3, #0x1c]
00472b74: bl #0x350ee0
00472b78: ldr r3, [r5, #0x10]
00472b7c: ldr r2, [pc, #0xd4]
00472b80: ldr r1, [r4, #8]
00472b84: ldr r0, [r3, #0x1c]
00472b88: add r2, pc, r2
00472b8c: mov r3, #1
00472b90: bl #0x35a0e4
00472b94: subs r2, r0, #0
00472b98: beq #0x472c08
00472b9c: mov r3, #1
00472ba0: strb r3, [r4, #0x28]
00472ba4: ldr r3, [r5, #0x10]
00472ba8: movw r1, #0x6164
00472bac: movt r1, #0x6d65
00472bb0: ldr r3, [r3, #0x1c]
00472bb4: mov r0, r3
00472bb8: ldr r3, [r3]
00472bbc: mov lr, pc
00472bc0: ldr pc, [r3, #0x1c]
00472bc4: cmp r0, #0
00472bc8: str r0, [r4, #0xc]
00472bcc: beq #0x472bec
00472bd0: ldr r3, [r0]
00472bd4: ldr r3, [r3, #-0xc]
00472bd8: add r0, r0, r3
00472bdc: ldr r3, [r0, #4]
00472be0: add r3, r3, #1
00472be4: str r3, [r0, #4]
00472be8: ldr r0, [r4, #0xc]
00472bec: mov r1, #0
00472bf0: strb r1, [r0, #0x138]
00472bf4: ldr r3, [r4, #0xc]
00472bf8: mov r0, r3
00472bfc: ldr r3, [r3]
00472c00: mov lr, pc
00472c04: ldr pc, [r3, #0x48]
00472c08: mov r0, r4
00472c0c: bl #0x47211c
00472c10: mov r0, r4
00472c14: bl #0x470a54
00472c18: mov r1, #0
00472c1c: mov r0, #8
00472c20: bl #0x310570
00472c24: ldr r1, [r4, #8]
00472c28: mov r5, r0
00472c2c: mov r2, #0
00472c30: bl #0x474d30
00472c34: mov r0, r4
00472c38: mov r1, r5
00472c3c: bl #0x470a84
00472c40: mov r0, r4
00472c44: add sp, sp, #0xc
00472c48: pop {r4, r5, r6, r7, r8, sl, pc}
00472c4c: subseq r2, r2, r4, ror r0
00472c50: muleq r0, r4, lr
00472c54: strdeq r3, r4, [r0], -r4
00472c58: subeq sl, r5, r0, lsr #22
