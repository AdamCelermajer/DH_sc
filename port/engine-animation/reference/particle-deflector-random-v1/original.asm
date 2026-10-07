# 0x69c6ec _ZNK6glitch2ps8PDSphere8generateERNS0_8PSRandomE
0069c6ec: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0069c6f0: ldr r6, [pc, #0x164]
0069c6f4: mov r3, #0
0069c6f8: sub sp, sp, #0x14
0069c6fc: mov r4, r0
0069c700: str r3, [r0, #8]
0069c704: mov r5, r1
0069c708: mov sb, r2
0069c70c: str r3, [r0]
0069c710: str r3, [r0, #4]
0069c714: add r6, pc, r6
0069c718: add fp, sp, #4
0069c71c: mov r0, fp
0069c720: mov r1, sb
0069c724: bl #0x637ac4
0069c728: ldr r1, [r6]
0069c72c: ldr r0, [sp, #4]
0069c730: bl #0x30e3ac
0069c734: ldr r1, [r6, #4]
0069c738: mov sl, r0
0069c73c: ldr r0, [sp, #8]
0069c740: bl #0x30e3ac
0069c744: ldr r1, [r6, #8]
0069c748: mov r8, r0
0069c74c: ldr r0, [sp, #0xc]
0069c750: bl #0x30e3ac
0069c754: mov r7, r0
0069c758: mov r1, sl
0069c75c: mov r0, sl
0069c760: str sl, [r4]
0069c764: str r8, [r4, #4]
0069c768: str r7, [r4, #8]
0069c76c: bl #0x30ed6c
0069c770: mov r1, r8
0069c774: mov sl, r0
0069c778: mov r0, r8
0069c77c: bl #0x30ed6c
0069c780: mov r1, r0
0069c784: mov r0, sl
0069c788: bl #0x30eba4
0069c78c: mov r1, r7
0069c790: mov r8, r0
0069c794: mov r0, r7
0069c798: bl #0x30ed6c
0069c79c: mov r1, r0
0069c7a0: mov r0, r8
0069c7a4: bl #0x30eba4
0069c7a8: mov r1, #0x3e800000
0069c7ac: bl #0x30e2f8
0069c7b0: cmp r0, #0
0069c7b4: bne #0x69c71c
0069c7b8: mov r0, r4
0069c7bc: bl #0x35e8e0
0069c7c0: ldrb r3, [r5, #0x28]
0069c7c4: cmp r3, #0
0069c7c8: beq #0x69c82c
0069c7cc: ldr r6, [r5, #0x10]
0069c7d0: ldr r1, [r4, #4]
0069c7d4: mov r0, r6
0069c7d8: bl #0x30ed6c
0069c7dc: ldr r1, [r5, #8]
0069c7e0: bl #0x30eba4
0069c7e4: ldr r1, [r4, #8]
0069c7e8: mov r8, r0
0069c7ec: mov r0, r6
0069c7f0: bl #0x30ed6c
0069c7f4: ldr r1, [r5, #0xc]
0069c7f8: bl #0x30eba4
0069c7fc: ldr r1, [r4]
0069c800: mov r7, r0
0069c804: mov r0, r6
0069c808: bl #0x30ed6c
0069c80c: ldr r1, [r5, #4]
0069c810: bl #0x30eba4
0069c814: str r8, [r4, #4]
0069c818: str r0, [r4]
0069c81c: str r7, [r4, #8]
0069c820: mov r0, r4
0069c824: add sp, sp, #0x14
0069c828: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0069c82c: mov r0, sb
0069c830: ldr r6, [r5, #0x14]
0069c834: bl #0x62fe78
0069c838: bl #0x30e6a0
0069c83c: ldr r1, [r5, #0x20]
0069c840: bl #0x30ed6c
0069c844: mov r1, r0
0069c848: mov r0, r6
0069c84c: bl #0x30eba4
0069c850: ldr r1, [r4, #4]
0069c854: mov r6, r0
0069c858: b #0x69c7d8
0069c85c: eorseq sl, r5, ip, asr #29

# 0x69c4ac _ZN6glitch2ps8PSRandom6NRandfEf
0069c4ac: push {r4, r5, r6, r7, r8, lr}
0069c4b0: mov r6, r0
0069c4b4: mov r8, r1
0069c4b8: mov r0, r6
0069c4bc: bl #0x62fe78
0069c4c0: bl #0x30e6a0
0069c4c4: mov r1, r0
0069c4c8: bl #0x30eba4
0069c4cc: mov r1, #0x3f800000
0069c4d0: bl #0x30e3ac
0069c4d4: mov r4, r0
0069c4d8: mov r0, r6
0069c4dc: bl #0x62fe78
0069c4e0: bl #0x30e6a0
0069c4e4: mov r1, r0
0069c4e8: bl #0x30eba4
0069c4ec: mov r1, #0x3f800000
0069c4f0: bl #0x30e3ac
0069c4f4: mov r1, r4
0069c4f8: mov r7, r0
0069c4fc: mov r0, r4
0069c500: bl #0x30ed6c
0069c504: mov r1, r7
0069c508: mov r5, r0
0069c50c: mov r0, r7
0069c510: bl #0x30ed6c
0069c514: mov r1, r0
0069c518: mov r0, r5
0069c51c: bl #0x30eba4
0069c520: mov r1, #0x3f800000
0069c524: mov r5, r0
0069c528: bl #0x30e2f8
0069c52c: cmp r0, #0
0069c530: mov r1, #0
0069c534: mov r0, r5
0069c538: bne #0x69c4b8
0069c53c: bl #0x30df8c
0069c540: cmp r0, #0
0069c544: bne #0x69c4b8
0069c548: mov r0, r5
0069c54c: bl #0x30deb4
0069c550: mov r1, #0xc0000000
0069c554: bl #0x30ed6c
0069c558: mov r1, r5
0069c55c: bl #0x30ec94
0069c560: bl #0x30e124
0069c564: mov r1, r0
0069c568: mov r0, r4
0069c56c: bl #0x30ed6c
0069c570: mov r1, r8
0069c574: bl #0x30ed6c
0069c578: pop {r4, r5, r6, r7, r8, pc}

# 0x62fe78 _ZN6glitch2ps8PSRandom4RandEv
0062fe78: push {r4, lr}
0062fe7c: ldr r3, [r0]
0062fe80: movw r2, #0x89c9
0062fe84: movt r2, #0x5e47
0062fe88: mov r1, r0
0062fe8c: smull r0, r2, r2, r3
0062fe90: asr ip, r3, #0x1f
0062fe94: asr r2, r2, #0xe
0062fe98: rsb r0, ip, r2
0062fe9c: movw lr, #0xadc8
0062fea0: mls r3, lr, r0, r3
0062fea4: movw r0, #0xbc8f
0062fea8: mul r3, r0, r3
0062feac: rsb r2, r2, ip
0062feb0: movw r0, #0xd47
0062feb4: mla r0, r0, r2, r3
0062feb8: cmp r0, #0
0062febc: str r0, [r1]
0062fec0: sublt r0, r0, #0x80000001
0062fec4: strlt r0, [r1]
0062fec8: bl #0x30ed30
0062fecc: mov r2, #0x80000000
0062fed0: mvn r3, #0xbe000000
0062fed4: asr r2, r2, #9
0062fed8: sub r3, r3, #0x200000
0062fedc: bl #0x30e340
0062fee0: pop {r4, pc}

# 0x637ac4 _ZN6glitch2ps8PSRandom7RandVecEv
00637ac4: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00637ac8: mov r4, r0
00637acc: mov r0, r1
00637ad0: mov r5, r1
00637ad4: bl #0x62fe78
00637ad8: mov sl, r0
00637adc: mov r0, r5
00637ae0: mov fp, r1
00637ae4: bl #0x62fe78
00637ae8: mov r8, r0
00637aec: mov r0, r5
00637af0: mov sb, r1
00637af4: bl #0x62fe78
00637af8: mov r6, r0
00637afc: mov r7, r1
00637b00: mov r0, sl
00637b04: mov r1, fp
00637b08: bl #0x30e6a0
00637b0c: mov r1, sb
00637b10: str r0, [r4]
00637b14: mov r0, r8
00637b18: bl #0x30e6a0
00637b1c: mov r1, r7
00637b20: str r0, [r4, #4]
00637b24: mov r0, r6
00637b28: bl #0x30e6a0
00637b2c: str r0, [r4, #8]
00637b30: mov r0, r4
00637b34: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# 0x69e008 _ZNK6glitch2ps6PDCone8generateERNS0_8PSRandomE
0069e008: push {r4, r5, r6, r7, r8, sb, sl, lr}
0069e00c: mov r6, r0
0069e010: mov r0, r2
0069e014: mov r7, r2
0069e018: mov r4, r1
0069e01c: bl #0x62fe78
0069e020: bl #0x30e6a0
0069e024: mov r5, r0
0069e028: mov r0, r7
0069e02c: bl #0x62fe78
0069e030: bl #0x30e6a0
0069e034: mov r1, r0
0069e038: bl #0x30eba4
0069e03c: movw r1, #0xfdb
0069e040: movt r1, #0x4049
0069e044: bl #0x30ed6c
0069e048: mov r8, r0
0069e04c: mov r0, r7
0069e050: ldr r7, [r4, #0x38]
0069e054: bl #0x62fe78
0069e058: bl #0x30e6a0
0069e05c: ldr r1, [r4, #0x44]
0069e060: bl #0x30ed6c
0069e064: mov r1, r0
0069e068: mov r0, r7
0069e06c: bl #0x30eba4
0069e070: mov r7, r0
0069e074: mov r0, r8
0069e078: bl #0x30e754
0069e07c: mov r1, r7
0069e080: bl #0x30ed6c
0069e084: mov sl, r0
0069e088: mov r0, r8
0069e08c: bl #0x30eb08
0069e090: mov r1, r7
0069e094: bl #0x30ed6c
0069e098: mov r1, r5
0069e09c: mov r7, r0
0069e0a0: mov r0, sl
0069e0a4: bl #0x30ed6c
0069e0a8: mov r1, r5
0069e0ac: mov r8, r0
0069e0b0: mov r0, r7
0069e0b4: bl #0x30ed6c
0069e0b8: ldr r1, [r4, #0x14]
0069e0bc: mov r7, r0
0069e0c0: mov r0, r5
0069e0c4: bl #0x30ed6c
0069e0c8: ldr r1, [r4, #8]
0069e0cc: bl #0x30eba4
0069e0d0: ldr r1, [r4, #0x20]
0069e0d4: mov sl, r0
0069e0d8: mov r0, r8
0069e0dc: bl #0x30ed6c
0069e0e0: mov r1, r0
0069e0e4: mov r0, sl
0069e0e8: bl #0x30eba4
0069e0ec: ldr r1, [r4, #0x2c]
0069e0f0: mov sl, r0
0069e0f4: mov r0, r7
0069e0f8: bl #0x30ed6c
0069e0fc: mov r1, r0
0069e100: mov r0, sl
0069e104: bl #0x30eba4
0069e108: ldr r1, [r4, #0x18]
0069e10c: mov sb, r0
0069e110: mov r0, r5
0069e114: bl #0x30ed6c
0069e118: ldr r1, [r4, #0xc]
0069e11c: bl #0x30eba4
0069e120: ldr r1, [r4, #0x24]
0069e124: mov sl, r0
0069e128: mov r0, r8
0069e12c: bl #0x30ed6c
0069e130: mov r1, r0
0069e134: mov r0, sl
0069e138: bl #0x30eba4
0069e13c: ldr r1, [r4, #0x30]
0069e140: mov sl, r0
0069e144: mov r0, r7
0069e148: bl #0x30ed6c
0069e14c: mov r1, r0
0069e150: mov r0, sl
0069e154: bl #0x30eba4
0069e158: ldr r1, [r4, #0x10]
0069e15c: mov sl, r0
0069e160: mov r0, r5
0069e164: bl #0x30ed6c
0069e168: ldr r1, [r4, #4]
0069e16c: bl #0x30eba4
0069e170: ldr r1, [r4, #0x1c]
0069e174: mov r5, r0
0069e178: mov r0, r8
0069e17c: bl #0x30ed6c
0069e180: mov r1, r0
0069e184: mov r0, r5
0069e188: bl #0x30eba4
0069e18c: ldr r1, [r4, #0x28]
0069e190: mov r5, r0
0069e194: mov r0, r7
0069e198: bl #0x30ed6c
0069e19c: mov r1, r0
0069e1a0: mov r0, r5
0069e1a4: bl #0x30eba4
0069e1a8: str r0, [r6]
0069e1ac: str sb, [r6, #4]
0069e1b0: str sl, [r6, #8]
0069e1b4: mov r0, r6
0069e1b8: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x69b534 _ZNK6glitch2ps5PDBox8generateERNS0_8PSRandomE
0069b534: push {r4, r5, r6, r7, r8, sb, sl, lr}
0069b538: sub sp, sp, #0x10
0069b53c: mov r4, r1
0069b540: mov r5, r0
0069b544: mov r1, r2
0069b548: add r0, sp, #4
0069b54c: bl #0x637ac4
0069b550: ldr r8, [sp, #4]
0069b554: ldr r1, [r4, #0x3c]
0069b558: ldr r7, [sp, #8]
0069b55c: mov r0, r8
0069b560: bl #0x30ed6c
0069b564: ldr r1, [r4, #8]
0069b568: bl #0x30eba4
0069b56c: ldr r1, [r4, #0x48]
0069b570: mov sl, r0
0069b574: mov r0, r7
0069b578: bl #0x30ed6c
0069b57c: mov r1, r0
0069b580: mov r0, sl
0069b584: bl #0x30eba4
0069b588: ldr r6, [sp, #0xc]
0069b58c: ldr r1, [r4, #0x54]
0069b590: mov sl, r0
0069b594: mov r0, r6
0069b598: bl #0x30ed6c
0069b59c: mov r1, r0
0069b5a0: mov r0, sl
0069b5a4: bl #0x30eba4
0069b5a8: ldr r1, [r4, #0x40]
0069b5ac: mov sb, r0
0069b5b0: mov r0, r8
0069b5b4: bl #0x30ed6c
0069b5b8: ldr r1, [r4, #0xc]
0069b5bc: bl #0x30eba4
0069b5c0: ldr r1, [r4, #0x4c]
0069b5c4: mov sl, r0
0069b5c8: mov r0, r7
0069b5cc: bl #0x30ed6c
0069b5d0: mov r1, r0
0069b5d4: mov r0, sl
0069b5d8: bl #0x30eba4
0069b5dc: ldr r1, [r4, #0x58]
0069b5e0: mov sl, r0
0069b5e4: mov r0, r6
0069b5e8: bl #0x30ed6c
0069b5ec: mov r1, r0
0069b5f0: mov r0, sl
0069b5f4: bl #0x30eba4
0069b5f8: ldr r1, [r4, #0x38]
0069b5fc: mov sl, r0
0069b600: mov r0, r8
0069b604: bl #0x30ed6c
0069b608: ldr r1, [r4, #4]
0069b60c: bl #0x30eba4
0069b610: ldr r1, [r4, #0x44]
0069b614: mov r8, r0
0069b618: mov r0, r7
0069b61c: bl #0x30ed6c
0069b620: mov r1, r0
0069b624: mov r0, r8
0069b628: bl #0x30eba4
0069b62c: ldr r1, [r4, #0x50]
0069b630: mov r7, r0
0069b634: mov r0, r6
0069b638: bl #0x30ed6c
0069b63c: mov r1, r0
0069b640: mov r0, r7
0069b644: bl #0x30eba4
0069b648: str r0, [r5]
0069b64c: str sb, [r5, #4]
0069b650: str sl, [r5, #8]
0069b654: mov r0, r5
0069b658: add sp, sp, #0x10
0069b65c: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x69ade8 _ZNK6glitch2ps7PDPoint8generateERNS0_8PSRandomE
0069ade8: ldr r2, [r1, #4]
0069adec: str r2, [r0]
0069adf0: ldr r2, [r1, #8]
0069adf4: str r2, [r0, #4]
0069adf8: ldr r2, [r1, #0xc]
0069adfc: str r2, [r0, #8]
0069ae00: bx lr

# 0x69c57c _ZN6glitch2ps8PSRandom8NRandVecEf
0069c57c: push {r4, r5, r6, r7, r8, sb, sl, lr}
0069c580: mov r5, r0
0069c584: mov r4, r1
0069c588: mov sl, r2
0069c58c: mov r0, r4
0069c590: bl #0x62fe78
0069c594: bl #0x30e6a0
0069c598: mov r1, r0
0069c59c: bl #0x30eba4
0069c5a0: mov r1, #0x3f800000
0069c5a4: bl #0x30e3ac
0069c5a8: mov r7, r0
0069c5ac: mov r0, r4
0069c5b0: bl #0x62fe78
0069c5b4: bl #0x30e6a0
0069c5b8: mov r1, r0
0069c5bc: bl #0x30eba4
0069c5c0: mov r1, #0x3f800000
0069c5c4: bl #0x30e3ac
0069c5c8: mov r1, r7
0069c5cc: mov r6, r0
0069c5d0: mov r0, r7
0069c5d4: bl #0x30ed6c
0069c5d8: mov r1, r6
0069c5dc: mov r8, r0
0069c5e0: mov r0, r6
0069c5e4: bl #0x30ed6c
0069c5e8: mov r1, r0
0069c5ec: mov r0, r8
0069c5f0: bl #0x30eba4
0069c5f4: mov r1, #0x3f800000
0069c5f8: mov r8, r0
0069c5fc: bl #0x30e2f8
0069c600: cmp r0, #0
0069c604: mov r1, #0
0069c608: mov r0, r8
0069c60c: bne #0x69c58c
0069c610: bl #0x30df8c
0069c614: cmp r0, #0
0069c618: bne #0x69c58c
0069c61c: mov r0, r8
0069c620: bl #0x30deb4
0069c624: mov r1, #0xc0000000
0069c628: bl #0x30ed6c
0069c62c: mov r1, r8
0069c630: bl #0x30ec94
0069c634: bl #0x30e124
0069c638: mov r1, sl
0069c63c: mov r8, r0
0069c640: mov r0, r4
0069c644: bl #0x69c4ac
0069c648: mov r1, r8
0069c64c: mov r4, r0
0069c650: mov r0, r7
0069c654: bl #0x30ed6c
0069c658: mov r1, sl
0069c65c: bl #0x30ed6c
0069c660: mov r1, r8
0069c664: str r0, [r5]
0069c668: mov r0, r6
0069c66c: bl #0x30ed6c
0069c670: mov r1, sl
0069c674: bl #0x30ed6c
0069c678: str r4, [r5, #8]
0069c67c: str r0, [r5, #4]
0069c680: mov r0, r5
0069c684: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x69c688 _ZNK6glitch2ps6PDBlob8generateERNS0_8PSRandomE
0069c688: push {r4, r5, r6, r7, lr}
0069c68c: mov r5, r1
0069c690: sub sp, sp, #0x14
0069c694: mov r4, r0
0069c698: mov r1, r2
0069c69c: add r0, sp, #4
0069c6a0: ldr r2, [r5, #0x10]
0069c6a4: bl #0x69c57c
0069c6a8: ldr r1, [sp, #8]
0069c6ac: ldr r0, [r5, #8]
0069c6b0: bl #0x30eba4
0069c6b4: ldr r1, [sp, #0xc]
0069c6b8: mov r7, r0
0069c6bc: ldr r0, [r5, #0xc]
0069c6c0: bl #0x30eba4
0069c6c4: ldr r1, [sp, #4]
0069c6c8: mov r6, r0
0069c6cc: ldr r0, [r5, #4]
0069c6d0: bl #0x30eba4
0069c6d4: str r7, [r4, #4]
0069c6d8: str r0, [r4]
0069c6dc: str r6, [r4, #8]
0069c6e0: mov r0, r4
0069c6e4: add sp, sp, #0x14
0069c6e8: pop {r4, r5, r6, r7, pc}

# 0x69e1bc _ZNK6glitch2ps10PDCylinder8generateERNS0_8PSRandomE
0069e1bc: push {r4, r5, r6, r7, r8, sb, sl, lr}
0069e1c0: mov r5, r0
0069e1c4: mov r0, r2
0069e1c8: mov r6, r2
0069e1cc: mov r4, r1
0069e1d0: bl #0x62fe78
0069e1d4: bl #0x30e6a0
0069e1d8: mov r8, r0
0069e1dc: mov r0, r6
0069e1e0: bl #0x62fe78
0069e1e4: bl #0x30e6a0
0069e1e8: mov r1, r0
0069e1ec: bl #0x30eba4
0069e1f0: movw r1, #0xfdb
0069e1f4: movt r1, #0x4049
0069e1f8: bl #0x30ed6c
0069e1fc: mov sl, r0
0069e200: mov r0, r6
0069e204: ldr r6, [r4, #0x38]
0069e208: bl #0x62fe78
0069e20c: bl #0x30e6a0
0069e210: ldr r1, [r4, #0x44]
0069e214: bl #0x30ed6c
0069e218: mov r1, r0
0069e21c: mov r0, r6
0069e220: bl #0x30eba4
0069e224: mov r6, r0
0069e228: mov r0, sl
0069e22c: bl #0x30e754
0069e230: mov r1, r6
0069e234: bl #0x30ed6c
0069e238: mov r7, r0
0069e23c: mov r0, sl
0069e240: bl #0x30eb08
0069e244: mov r1, r6
0069e248: bl #0x30ed6c
0069e24c: ldr r1, [r4, #0x14]
0069e250: mov r6, r0
0069e254: mov r0, r8
0069e258: bl #0x30ed6c
0069e25c: ldr r1, [r4, #8]
0069e260: bl #0x30eba4
0069e264: ldr r1, [r4, #0x20]
0069e268: mov sl, r0
0069e26c: mov r0, r7
0069e270: bl #0x30ed6c
0069e274: mov r1, r0
0069e278: mov r0, sl
0069e27c: bl #0x30eba4
0069e280: ldr r1, [r4, #0x2c]
0069e284: mov sl, r0
0069e288: mov r0, r6
0069e28c: bl #0x30ed6c
0069e290: mov r1, r0
0069e294: mov r0, sl
0069e298: bl #0x30eba4
0069e29c: ldr r1, [r4, #0x18]
0069e2a0: mov sb, r0
0069e2a4: mov r0, r8
0069e2a8: bl #0x30ed6c
0069e2ac: ldr r1, [r4, #0xc]
0069e2b0: bl #0x30eba4
0069e2b4: ldr r1, [r4, #0x24]
0069e2b8: mov sl, r0
0069e2bc: mov r0, r7
0069e2c0: bl #0x30ed6c
0069e2c4: mov r1, r0
0069e2c8: mov r0, sl
0069e2cc: bl #0x30eba4
0069e2d0: ldr r1, [r4, #0x30]
0069e2d4: mov sl, r0
0069e2d8: mov r0, r6
0069e2dc: bl #0x30ed6c
0069e2e0: mov r1, r0
0069e2e4: mov r0, sl
0069e2e8: bl #0x30eba4
0069e2ec: ldr r1, [r4, #0x10]
0069e2f0: mov sl, r0
0069e2f4: mov r0, r8
0069e2f8: bl #0x30ed6c
0069e2fc: ldr r1, [r4, #4]
0069e300: bl #0x30eba4
0069e304: ldr r1, [r4, #0x1c]
0069e308: mov r8, r0
0069e30c: mov r0, r7
0069e310: bl #0x30ed6c
0069e314: mov r1, r0
0069e318: mov r0, r8
0069e31c: bl #0x30eba4
0069e320: ldr r1, [r4, #0x28]
0069e324: mov r7, r0
0069e328: mov r0, r6
0069e32c: bl #0x30ed6c
0069e330: mov r1, r0
0069e334: mov r0, r7
0069e338: bl #0x30eba4
0069e33c: str r0, [r5]
0069e340: str sb, [r5, #4]
0069e344: str sl, [r5, #8]
0069e348: mov r0, r5
0069e34c: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x69ae54 _ZNK6glitch2ps6PDLine8generateERNS0_8PSRandomE
0069ae54: push {r4, r5, r6, r7, r8, lr}
0069ae58: mov r4, r0
0069ae5c: mov r0, r2
0069ae60: mov r5, r1
0069ae64: bl #0x62fe78
0069ae68: bl #0x30e6a0
0069ae6c: ldr r1, [r5, #0x14]
0069ae70: mov r6, r0
0069ae74: bl #0x30ed6c
0069ae78: ldr r1, [r5, #8]
0069ae7c: bl #0x30eba4
0069ae80: ldr r1, [r5, #0x18]
0069ae84: mov r8, r0
0069ae88: mov r0, r6
0069ae8c: bl #0x30ed6c
0069ae90: ldr r1, [r5, #0xc]
0069ae94: bl #0x30eba4
0069ae98: ldr r1, [r5, #0x10]
0069ae9c: mov r7, r0
0069aea0: mov r0, r6
0069aea4: bl #0x30ed6c
0069aea8: ldr r1, [r5, #4]
0069aeac: bl #0x30eba4
0069aeb0: str r8, [r4, #4]
0069aeb4: str r0, [r4]
0069aeb8: str r7, [r4, #8]
0069aebc: mov r0, r4
0069aec0: pop {r4, r5, r6, r7, r8, pc}
