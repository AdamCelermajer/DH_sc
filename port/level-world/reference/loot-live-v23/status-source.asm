# 0x442ea4 _Z17NativeStopMessageRKN7gameswf7fn_callE
00442ea4: push {r4, r5, r6, r7, r8, sl, lr}
00442ea8: ldr r4, [pc, #0x52c]
00442eac: ldr r6, [pc, #0x52c]
00442eb0: ldr r2, [r0, #0x10]
00442eb4: add r4, pc, r4
00442eb8: ldr r3, [r4, r6]
00442ebc: sub sp, sp, #0x3c
00442ec0: cmp r2, #0
00442ec4: ldr r3, [r3]
00442ec8: mov r5, r0
00442ecc: str r3, [sp, #0x34]
00442ed0: ble #0x442efc
00442ed4: ldr r2, [r0, #0xc]
00442ed8: ldr r3, [r0, #0x14]
00442edc: mov r1, #0xc
00442ee0: ldr r2, [r2]
00442ee4: mla r3, r1, r3, r2
00442ee8: ldrb r3, [r3, #1]
00442eec: sub r3, r3, #3
00442ef0: uxtb r3, r3
00442ef4: cmp r3, #1
00442ef8: bls #0x442f18
00442efc: ldr r3, [r4, r6]
00442f00: ldr r2, [sp, #0x34]
00442f04: ldr r3, [r3]
00442f08: cmp r2, r3
00442f0c: bne #0x4433d8
00442f10: add sp, sp, #0x3c
00442f14: pop {r4, r5, r6, r7, r8, sl, pc}
00442f18: ldr r8, [pc, #0x4c4]
00442f1c: add r8, pc, r8
00442f20: ldr r3, [r8, #0x1c]
00442f24: tst r3, #1
00442f28: beq #0x443124
00442f2c: ldr r8, [pc, #0x4b4]
00442f30: add r8, pc, r8
00442f34: ldr r3, [r8, #0x38]
00442f38: tst r3, #1
00442f3c: beq #0x4430d4
00442f40: ldr r8, [pc, #0x4a4]
00442f44: add r8, pc, r8
00442f48: ldr r3, [r8, #0x54]
00442f4c: tst r3, #1
00442f50: beq #0x443084
00442f54: ldr r8, [pc, #0x494]
00442f58: add r8, pc, r8
00442f5c: ldr r3, [r8, #0x70]
00442f60: tst r3, #1
00442f64: beq #0x4431c4
00442f68: ldr r8, [pc, #0x484]
00442f6c: add r8, pc, r8
00442f70: ldr r3, [r8, #0x8c]
00442f74: tst r3, #1
00442f78: beq #0x443174
00442f7c: ldr r3, [r5, #0xc]
00442f80: ldr r2, [r5, #0x14]
00442f84: mov r0, #0xc
00442f88: ldr r3, [r3]
00442f8c: ldr sl, [pc, #0x464]
00442f90: add r5, sp, #0x1c
00442f94: mla r0, r0, r2, r3
00442f98: bl #0x796f5c
00442f9c: add r2, sp, #4
00442fa0: mov r1, r0
00442fa4: mov r0, r5
00442fa8: bl #0x3140ec
00442fac: ldr r3, [r4, sl]
00442fb0: mov r1, #0
00442fb4: mov r2, r1
00442fb8: ldr r0, [r3, #0x40]
00442fbc: bl #0x36e478
00442fc0: ldr r3, [pc, #0x434]
00442fc4: ldr r8, [sp, #0x30]
00442fc8: ldr r7, [sp, #0x2c]
00442fcc: add r3, pc, r3
00442fd0: ldr r2, [r3, #0x30]
00442fd4: ldr r1, [r3, #0x34]
00442fd8: rsb r7, r8, r7
00442fdc: rsb r3, r1, r2
00442fe0: cmp r7, r3
00442fe4: beq #0x443214
00442fe8: ldr r3, [pc, #0x410]
00442fec: add r3, pc, r3
00442ff0: ldr r2, [r3, #0x4c]
00442ff4: ldr r1, [r3, #0x50]
00442ff8: rsb r3, r1, r2
00442ffc: cmp r7, r3
00443000: beq #0x443288
00443004: ldr r7, [pc, #0x3f8]
00443008: mov r0, r5
0044300c: add r7, pc, r7
00443010: add r1, r7, #0x58
00443014: bl #0x43a7d4
00443018: cmp r0, #0
0044301c: beq #0x443314
00443020: bl #0x7fd794
00443024: ldrb r3, [r0, #5]
00443028: cmp r3, #0
0044302c: beq #0x44307c
00443030: ldr r3, [r4, sl]
00443034: ldr r0, [r3, #0x40]
00443038: bl #0x36f074
0044303c: cmp r0, #0
00443040: beq #0x443308
00443044: bl #0x80b1bc
00443048: mov r7, r0
0044304c: ldr r0, [pc, #0x3b4]
00443050: mov r1, #1
00443054: add r0, pc, r0
00443058: bl #0x80a244
0044305c: mvn r3, #0
00443060: mov r2, #3
00443064: mov r1, r0
00443068: str r2, [r0, #0x50]
0044306c: str r3, [r0, #0x58]
00443070: str r3, [r0, #0x54]
00443074: mov r0, r7
00443078: bl #0x80e2a4
0044307c: bl #0x442c1c
00443080: b #0x443308
00443084: add r7, r8, #0x54
00443088: mov r0, r7
0044308c: bl #0x30e76c
00443090: cmp r0, #0
00443094: beq #0x442f54
00443098: ldr r1, [pc, #0x36c]
0044309c: add r8, r8, #0x58
004430a0: add r2, sp, #0x10
004430a4: add r1, pc, r1
004430a8: mov r0, r8
004430ac: bl #0x3140ec
004430b0: mov r0, r7
004430b4: bl #0x30ea3c
004430b8: ldr r3, [pc, #0x350]
004430bc: mov r0, r8
004430c0: ldr r1, [r4, r3]
004430c4: ldr r3, [pc, #0x348]
004430c8: ldr r2, [r4, r3]
004430cc: bl #0x30e304
004430d0: b #0x442f54
004430d4: add r7, r8, #0x38
004430d8: mov r0, r7
004430dc: bl #0x30e76c
004430e0: cmp r0, #0
004430e4: beq #0x442f40
004430e8: ldr r1, [pc, #0x328]
004430ec: add r8, r8, #0x3c
004430f0: add r2, sp, #0x14
004430f4: add r1, pc, r1
004430f8: mov r0, r8
004430fc: bl #0x3140ec
00443100: mov r0, r7
00443104: bl #0x30ea3c
00443108: ldr r3, [pc, #0x300]
0044310c: mov r0, r8
00443110: ldr r1, [r4, r3]
00443114: ldr r3, [pc, #0x2f8]
00443118: ldr r2, [r4, r3]
0044311c: bl #0x30e304
00443120: b #0x442f40
00443124: add r7, r8, #0x1c
00443128: mov r0, r7
0044312c: bl #0x30e76c
00443130: cmp r0, #0
00443134: beq #0x442f2c
00443138: ldr r1, [pc, #0x2dc]
0044313c: add r8, r8, #0x20
00443140: add r2, sp, #0x18
00443144: add r1, pc, r1
00443148: mov r0, r8
0044314c: bl #0x3140ec
00443150: mov r0, r7
00443154: bl #0x30ea3c
00443158: ldr r3, [pc, #0x2b0]
0044315c: mov r0, r8
00443160: ldr r1, [r4, r3]
00443164: ldr r3, [pc, #0x2a8]
00443168: ldr r2, [r4, r3]
0044316c: bl #0x30e304
00443170: b #0x442f2c
00443174: add r7, r8, #0x8c
00443178: mov r0, r7
0044317c: bl #0x30e76c
00443180: cmp r0, #0
00443184: beq #0x442f7c
00443188: ldr r1, [pc, #0x290]
0044318c: add r8, r8, #0x90
00443190: add r2, sp, #8
00443194: add r1, pc, r1
00443198: mov r0, r8
0044319c: bl #0x3140ec
004431a0: mov r0, r7
004431a4: bl #0x30ea3c
004431a8: ldr r3, [pc, #0x260]
004431ac: mov r0, r8
004431b0: ldr r1, [r4, r3]
004431b4: ldr r3, [pc, #0x258]
004431b8: ldr r2, [r4, r3]
004431bc: bl #0x30e304
004431c0: b #0x442f7c
004431c4: add r7, r8, #0x70
004431c8: mov r0, r7
004431cc: bl #0x30e76c
004431d0: cmp r0, #0
004431d4: beq #0x442f68
004431d8: ldr r1, [pc, #0x244]
004431dc: add r8, r8, #0x74
004431e0: add r2, sp, #0xc
004431e4: add r1, pc, r1
004431e8: mov r0, r8
004431ec: bl #0x3140ec
004431f0: mov r0, r7
004431f4: bl #0x30ea3c
004431f8: ldr r3, [pc, #0x210]
004431fc: mov r0, r8
00443200: ldr r1, [r4, r3]
00443204: ldr r3, [pc, #0x208]
00443208: ldr r2, [r4, r3]
0044320c: bl #0x30e304
00443210: b #0x442f68
00443214: mov r0, r8
00443218: mov r2, r7
0044321c: bl #0x30e5e0
00443220: cmp r0, #0
00443224: bne #0x442fe8
00443228: ldr r7, [pc, #0x1f8]
0044322c: ldr r0, [r4, r7]
00443230: ldr r2, [r0, #0x14]
00443234: ldr r3, [r0, #4]
00443238: cmp r2, r3
0044323c: beq #0x443308
00443240: add r0, r0, #4
00443244: bl #0x383d10
00443248: ldr r3, [pc, #0x1dc]
0044324c: ldr r3, [r4, r3]
00443250: ldr r0, [r3]
00443254: cmp r0, #0
00443258: beq #0x443260
0044325c: bl #0x442c9c
00443260: ldr r3, [r4, r7]
00443264: ldr r2, [r3, #4]
00443268: ldr r3, [r3, #0x14]
0044326c: cmp r3, r2
00443270: beq #0x443308
00443274: ldr r3, [pc, #0x1b4]
00443278: ldr r3, [r4, r3]
0044327c: ldr r0, [r3]
00443280: bl #0x442c9c
00443284: b #0x443308
00443288: mov r0, r8
0044328c: mov r2, r7
00443290: bl #0x30e5e0
00443294: cmp r0, #0
00443298: bne #0x443004
0044329c: ldr r7, [pc, #0x190]
004432a0: ldr r3, [r4, r7]
004432a4: ldr r2, [r3, #4]
004432a8: ldr r1, [r3, #0x14]
004432ac: cmp r1, r2
004432b0: beq #0x443308
004432b4: ldr r1, [r3, #0xc]
004432b8: sub r1, r1, #8
004432bc: cmp r2, r1
004432c0: addne r2, r2, #8
004432c4: strne r2, [r3, #4]
004432c8: beq #0x44339c
004432cc: ldr r3, [pc, #0x164]
004432d0: ldr r3, [r4, r3]
004432d4: ldr r0, [r3]
004432d8: cmp r0, #0
004432dc: beq #0x4432e4
004432e0: bl #0x442a14
004432e4: ldr r3, [r4, r7]
004432e8: ldr r2, [r3, #4]
004432ec: ldr r3, [r3, #0x14]
004432f0: cmp r3, r2
004432f4: beq #0x443308
004432f8: ldr r3, [pc, #0x13c]
004432fc: ldr r3, [r4, r3]
00443300: ldr r0, [r3]
00443304: bl #0x442a14
00443308: mov r0, r5
0044330c: bl #0x3139ac
00443310: b #0x442efc
00443314: mov r0, r5
00443318: add r1, r7, #0x74
0044331c: bl #0x43a7d4
00443320: cmp r0, #0
00443324: bne #0x44307c
00443328: add r1, r7, #0x90
0044332c: mov r0, r5
00443330: bl #0x43a7d4
00443334: cmp r0, #0
00443338: beq #0x443308
0044333c: ldr r7, [pc, #0xfc]
00443340: ldr r0, [r4, r7]
00443344: ldr r2, [r0, #0x14]
00443348: ldr r3, [r0, #4]
0044334c: cmp r2, r3
00443350: beq #0x443308
00443354: add r0, r0, #4
00443358: bl #0x383da8
0044335c: ldr r3, [pc, #0xe0]
00443360: ldr r3, [r4, r3]
00443364: ldr r0, [r3]
00443368: cmp r0, #0
0044336c: beq #0x443374
00443370: bl #0x442da0
00443374: ldr r3, [r4, r7]
00443378: ldr r2, [r3, #4]
0044337c: ldr r3, [r3, #0x14]
00443380: cmp r3, r2
00443384: beq #0x443308
00443388: ldr r3, [pc, #0xb8]
0044338c: ldr r3, [r4, r3]
00443390: ldr r0, [r3]
00443394: bl #0x442da0
00443398: b #0x443308
0044339c: ldr r0, [r3, #8]
004433a0: cmp r0, #0
004433a4: beq #0x4433b0
004433a8: mov r1, #0x80
004433ac: bl #0x31bb44
004433b0: ldr r3, [r4, r7]
004433b4: ldr r2, [r3, #0x10]
004433b8: add r1, r2, #4
004433bc: str r1, [r3, #0x10]
004433c0: ldr r2, [r2, #4]
004433c4: add r1, r2, #0x80
004433c8: str r2, [r3, #4]
004433cc: str r1, [r3, #0xc]
004433d0: str r2, [r3, #8]
004433d4: b #0x4432cc
004433d8: bl #0x30e310
004433dc: ldrsbeq r1, [r5], #-0xbc
004433e0: andeq r4, r0, ip, lsr #1
004433e4: subseq r2, r6, r8, lsr ip
004433e8: subseq r2, r6, r4, lsr #24
004433ec: subseq r2, r6, r0, lsl ip
004433f0: ldrsheq r2, [r6], #-0xbc
004433f4: subseq r2, r6, r8, ror #23
004433f8: strdeq r3, r4, [r0], -r4
004433fc: subseq r2, r6, r8, lsl #23
00443400: subseq r2, r6, r8, ror #22
00443404: subseq r2, r6, r8, asr #22
00443408: subeq fp, r7, ip, ror #28
0044340c: subeq r8, r8, ip, lsr #11
00443410: andeq r1, r0, r8, lsr #6
00443414: muleq r0, r0, r8
00443418: subeq r8, r8, r4, asr sp
0044341c: strdeq r8, sb, [r8], #-0xcc
00443420: subeq r8, r8, ip, asr #25
00443424: subeq r8, r8, r4, ror ip
00443428: andeq r1, r0, ip, ror #9
0044342c: strdeq r3, r4, [r0], -r0
00443430: andeq r4, r0, ip, lsr #21
00443434: andeq r1, r0, r0, lsl ip
00443438: strheq r1, [r0], -r8
0044343c: andeq r3, r0, r8, lsr r3
00443440: ldrdeq r0, r1, [r0], -r8
00443444: andeq r3, r0, r4, ror r7
00443448: andeq r4, r0, ip, asr #4

# 0x43fec4 _Z26NativeGetNextStatusMessageRKN7gameswf7fn_callE
0043fec4: push {r4, r5, r6, r7, r8, sb, sl, lr}
0043fec8: ldr r4, [pc, #0x19c]
0043fecc: ldr r5, [pc, #0x19c]
0043fed0: ldr r7, [r0, #0x10]
0043fed4: add r4, pc, r4
0043fed8: ldr r3, [r4, r5]
0043fedc: sub r2, r7, #1
0043fee0: sub sp, sp, #0x50
0043fee4: ldr r3, [r3]
0043fee8: cmp r2, #1
0043feec: mov r6, r0
0043fef0: str r3, [sp, #0x4c]
0043fef4: bls #0x43ff14
0043fef8: ldr r3, [r4, r5]
0043fefc: ldr r2, [sp, #0x4c]
0043ff00: ldr r3, [r3]
0043ff04: cmp r2, r3
0043ff08: bne #0x440068
0043ff0c: add sp, sp, #0x50
0043ff10: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0043ff14: ldr r8, [r0, #0xc]
0043ff18: ldr sl, [r0, #0x14]
0043ff1c: mov sb, #0xc
0043ff20: ldr r3, [r8]
0043ff24: mla r3, sb, sl, r3
0043ff28: ldrsb r2, [r3, #1]
0043ff2c: cmp r2, #2
0043ff30: bne #0x43fef8
0043ff34: ldr ip, [r3, #8]
0043ff38: ldr lr, [r3, #4]
0043ff3c: str ip, [sp, #4]
0043ff40: str lr, [sp]
0043ff44: ldrd r0, r1, [sp]
0043ff48: mov r2, r0
0043ff4c: mov r3, r1
0043ff50: str ip, [sp, #0x10]
0043ff54: str lr, [sp, #0xc]
0043ff58: bl #0x30e2bc
0043ff5c: cmp r0, #0
0043ff60: bne #0x43fef8
0043ff64: cmp r7, #2
0043ff68: beq #0x440024
0043ff6c: ldr r1, [pc, #0x100]
0043ff70: add sl, sp, #0x34
0043ff74: add r2, sp, #0x14
0043ff78: add r1, pc, r1
0043ff7c: mov r0, sl
0043ff80: bl #0x3140ec
0043ff84: ldr r3, [pc, #0xec]
0043ff88: mov r1, #0
0043ff8c: mov r2, r1
0043ff90: ldr r3, [r4, r3]
0043ff94: ldr r0, [r3, #0x40]
0043ff98: bl #0x36e478
0043ff9c: ldr r3, [r6, #0x10]
0043ffa0: cmp r3, #2
0043ffa4: beq #0x440048
0043ffa8: add r7, sp, #0x18
0043ffac: mov r1, #0x10
0043ffb0: mov r0, r7
0043ffb4: str r7, [sp, #0x28]
0043ffb8: str r7, [sp, #0x2c]
0043ffbc: bl #0x31167c
0043ffc0: ldr r3, [pc, #0xb4]
0043ffc4: ldr r2, [sp, #0x28]
0043ffc8: mov r1, #0
0043ffcc: ldr r3, [r4, r3]
0043ffd0: strb r1, [r2]
0043ffd4: ldr r2, [r3, #0x14]
0043ffd8: ldr r8, [r3, #4]
0043ffdc: cmp r2, r8
0043ffe0: beq #0x440010
0043ffe4: cmp r8, r7
0043ffe8: beq #0x43fffc
0043ffec: mov r0, r7
0043fff0: ldr r1, [r8, #0x14]
0043fff4: ldr r2, [r8, #0x10]
0043fff8: bl #0x3109e0
0043fffc: ldr r3, [r8, #0x18]
00440000: ldr r0, [r6]
00440004: ldr r1, [sp, #0x2c]
00440008: str r3, [sp, #0x30]
0044000c: bl #0x797350
00440010: mov r0, r7
00440014: bl #0x3139ac
00440018: mov r0, sl
0044001c: bl #0x3139ac
00440020: b #0x43fef8
00440024: ldr r3, [r8]
00440028: sub sl, sl, #1
0044002c: mla sb, sb, sl, r3
00440030: ldrb r3, [sb, #1]
00440034: cmp r3, #1
00440038: beq #0x43ff6c
0044003c: cmp r3, #0
00440040: bne #0x43fef8
00440044: b #0x43ff6c
00440048: ldr r3, [r6, #0xc]
0044004c: ldr r2, [r6, #0x14]
00440050: mov r0, #0xc
00440054: ldr r3, [r3]
00440058: sub r2, r2, #1
0044005c: mla r0, r0, r2, r3
00440060: bl #0x797960
00440064: b #0x43ffa8
00440068: bl #0x30e310
0044006c: ldrheq r4, [r5], #-0xbc
00440070: andeq r4, r0, ip, lsr #1
00440074: subeq fp, r8, r0, lsr #27
00440078: strdeq r3, r4, [r0], -r4
0044007c: andeq r1, r0, ip, ror #9

# 0x3ecff8 _ZN18MenuMessageManagerI9StatusMsgLi4EE14EnqueueMessageERKS0_ib.clone.18
003ecff8: push {r4, r5, r6, r7, lr}
003ecffc: ldr r4, [pc, #0x12c]
003ed000: ldr r5, [pc, #0x12c]
003ed004: sub sp, sp, #0x34
003ed008: add r4, pc, r4
003ed00c: ldr r5, [r4, r5]
003ed010: mov r1, r0
003ed014: add r6, r5, #4
003ed018: mov r0, r6
003ed01c: bl #0x3be28c
003ed020: ldm r6, {r0, r1, r2, r3}
003ed024: add ip, sp, #0xc
003ed028: stm ip, {r0, r1, r2, r3}
003ed02c: add r0, r5, #0x14
003ed030: mov r1, ip
003ed034: bl #0x3bcf8c
003ed038: cmp r0, #1
003ed03c: beq #0x3ed048
003ed040: add sp, sp, #0x34
003ed044: pop {r4, r5, r6, r7, pc}
003ed048: ldr r3, [pc, #0xe8]
003ed04c: ldr r3, [r4, r3]
003ed050: ldr r7, [r3]
003ed054: bl #0x42ca8c
003ed058: bl #0x42cb8c
003ed05c: subs r6, r0, #0
003ed060: beq #0x3ed040
003ed064: ldr r5, [pc, #0xd0]
003ed068: ldr r3, [r4, r5]
003ed06c: ldr r2, [r3, #0x2c]
003ed070: cmp r2, #0
003ed074: beq #0x3ed0ac
003ed078: ldr r0, [r3, #0x28]
003ed07c: ldrb r3, [r0, #4]
003ed080: cmp r3, #0
003ed084: bne #0x3ed0c8
003ed088: ldr r1, [r0]
003ed08c: sub r1, r1, #1
003ed090: cmp r1, #0
003ed094: str r1, [r0]
003ed098: beq #0x3ed128
003ed09c: ldr r3, [r4, r5]
003ed0a0: mov r2, #0
003ed0a4: str r2, [r3, #0x2c]
003ed0a8: str r2, [r3, #0x28]
003ed0ac: ldr r3, [pc, #0x8c]
003ed0b0: ldr r0, [r4, r5]
003ed0b4: mov r2, r6
003ed0b8: ldr r1, [r4, r3]
003ed0bc: mov r3, #0
003ed0c0: ldr r1, [r1]
003ed0c4: bl #0x427ca0
003ed0c8: ldr r0, [r4, r5]
003ed0cc: bl #0x427d50
003ed0d0: mov ip, #0
003ed0d4: strb ip, [sp, #0x1c]
003ed0d8: mov r2, #0
003ed0dc: mov r3, #0
003ed0e0: mov ip, #2
003ed0e4: strd r2, r3, [sp, #0x28]
003ed0e8: strb ip, [sp, #0x1d]
003ed0ec: mov ip, #0
003ed0f0: str ip, [sp, #0x20]
003ed0f4: ldr ip, [sp, #0x2c]
003ed0f8: add r4, sp, #0x1c
003ed0fc: mov r1, r0
003ed100: str ip, [r4, #8]
003ed104: mov r0, r6
003ed108: mov ip, #1
003ed10c: mov r2, r7
003ed110: mov r3, r4
003ed114: str ip, [sp]
003ed118: bl #0x7abe0c
003ed11c: mov r0, r4
003ed120: bl #0x797124
003ed124: b #0x3ed040
003ed128: bl #0x752b38
003ed12c: b #0x3ed09c
003ed130: subseq r7, sl, r8, lsl #21
003ed134: andeq r1, r0, ip, ror #9
003ed138: andeq r4, r0, ip, lsr #21
003ed13c: andeq r2, r0, r0, ror r0
003ed140: muleq r0, r4, r5

# 0x3f9018 _ZN18MenuMessageManagerI9StatusMsgLi4EE21FlushEnqueuedMessagesEi.clone.25
003f9018: ldr r3, [pc, #0x3c]
003f901c: ldr r2, [pc, #0x3c]
003f9020: push {r4, r5, r6, lr}
003f9024: add r3, pc, r3
003f9028: ldr r4, [r3, r2]
003f902c: ldr r2, [r4, #0x14]
003f9030: ldr r3, [r4, #4]
003f9034: cmp r2, r3
003f9038: beq #0x3f9058
003f903c: add r5, r4, #4
003f9040: mov r0, r5
003f9044: bl #0x383d10
003f9048: ldr r2, [r4, #0x14]
003f904c: ldr r3, [r4, #4]
003f9050: cmp r2, r3
003f9054: bne #0x3f9040
003f9058: pop {r4, r5, r6, pc}
003f905c: subseq fp, sb, ip, ror #20
003f9060: andeq r1, r0, ip, ror #9

# 0x442c9c _ZNK18MenuMessageManagerI9StatusMsgLi4EE6InvokeEPKci.clone.52
00442c9c: push {r4, r5, r6, r7, lr}
00442ca0: sub sp, sp, #0x24
00442ca4: mov r6, r0
00442ca8: bl #0x42ca8c
00442cac: bl #0x42cb8c
00442cb0: ldr r4, [pc, #0xdc]
00442cb4: subs r5, r0, #0
00442cb8: add r4, pc, r4
00442cbc: beq #0x442d40
00442cc0: ldr r7, [pc, #0xd0]
00442cc4: ldr r3, [r4, r7]
00442cc8: ldr r2, [r3, #0x2c]
00442ccc: cmp r2, #0
00442cd0: beq #0x442d6c
00442cd4: ldr r0, [r3, #0x28]
00442cd8: ldrb r3, [r0, #4]
00442cdc: cmp r3, #0
00442ce0: beq #0x442d48
00442ce4: ldr r0, [r4, r7]
00442ce8: bl #0x427d50
00442cec: mov ip, #0
00442cf0: mov r2, #0
00442cf4: mov r3, #0
00442cf8: strb ip, [sp, #0xc]
00442cfc: mov ip, #2
00442d00: strd r2, r3, [sp, #0x18]
00442d04: strb ip, [sp, #0xd]
00442d08: mov ip, #0
00442d0c: str ip, [sp, #0x10]
00442d10: ldr ip, [sp, #0x1c]
00442d14: add r4, sp, #0xc
00442d18: mov r1, r0
00442d1c: str ip, [r4, #8]
00442d20: mov r0, r5
00442d24: mov ip, #1
00442d28: mov r2, r6
00442d2c: mov r3, r4
00442d30: str ip, [sp]
00442d34: bl #0x7abe0c
00442d38: mov r0, r4
00442d3c: bl #0x797124
00442d40: add sp, sp, #0x24
00442d44: pop {r4, r5, r6, r7, pc}
00442d48: ldr r1, [r0]
00442d4c: sub r1, r1, #1
00442d50: cmp r1, #0
00442d54: str r1, [r0]
00442d58: beq #0x442d8c
00442d5c: ldr r3, [r4, r7]
00442d60: mov r2, #0
00442d64: str r2, [r3, #0x2c]
00442d68: str r2, [r3, #0x28]
00442d6c: ldr r3, [pc, #0x28]
00442d70: ldr r0, [r4, r7]
00442d74: mov r2, r5
00442d78: ldr r1, [r4, r3]
00442d7c: mov r3, #0
00442d80: ldr r1, [r1]
00442d84: bl #0x427ca0
00442d88: b #0x442ce4
00442d8c: bl #0x752b38
00442d90: b #0x442d5c
00442d94: ldrsbeq r1, [r5], #-0xd8
00442d98: andeq r2, r0, r0, ror r0
00442d9c: muleq r0, r4, r5
