
# _ZN15PyDataConstants10reloadDataEP11IStreamBasePKc
004c540c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004c5410: ldr r2, [pc, #0x278]
004c5414: ldr r3, [pc, #0x278]
004c5418: sub sp, sp, #0x24c
004c541c: add r2, pc, r2
004c5420: str r2, [sp, #4]
004c5424: ldr ip, [sp, #4]
004c5428: str r3, [sp, #0xc]
004c542c: ldr r3, [r2, r3]
004c5430: ldr r2, [pc, #0x260]
004c5434: mov sb, r0
004c5438: ldr r3, [r3]
004c543c: ldr r6, [ip, r2]
004c5440: mov r5, r1
004c5444: str r3, [sp, #0x244]
004c5448: mov r0, r6
004c544c: bl #0x337888
004c5450: ldr r1, [pc, #0x244]
004c5454: add r4, sp, #0x22c
004c5458: add r2, sp, #0x28
004c545c: add r1, pc, r1
004c5460: mov r0, r4
004c5464: bl #0x3140ec
004c5468: mov r1, r4
004c546c: mov r0, r6
004c5470: bl #0x337a88
004c5474: mov r0, r4
004c5478: add r4, sp, #0x24
004c547c: bl #0x318254
004c5480: mov r0, r5
004c5484: mov r1, r4
004c5488: bl #0x3df1a0
004c548c: mov r3, #1
004c5490: cmp r3, #0
004c5494: str r3, [sp, #0x18]
004c5498: bne #0x4c54dc
004c549c: add r3, r4, #2
004c54a0: add r4, r4, #1
004c54a4: ldrb r1, [r3, #1]
004c54a8: ldrb r2, [r4, #-1]
004c54ac: cmp r3, r4
004c54b0: eor r2, r1, r2
004c54b4: strb r2, [r4, #-1]
004c54b8: ldrb r1, [r3, #1]
004c54bc: eor r2, r2, r1
004c54c0: strb r2, [r3, #1]
004c54c4: ldrb r1, [r4, #-1]
004c54c8: sub r3, r3, #1
004c54cc: eor r2, r2, r1
004c54d0: strb r2, [r4, #-1]
004c54d4: add r4, r4, #1
004c54d8: bhi #0x4c54a4
004c54dc: ldr r3, [sp, #0x24]
004c54e0: cmp r3, #0
004c54e4: beq #0x4c5668
004c54e8: add r8, sp, #0x1c
004c54ec: add r1, sp, #0x20
004c54f0: mov r2, #0
004c54f4: add r3, r1, #1
004c54f8: add ip, r8, #2
004c54fc: str r1, [sp, #0x10]
004c5500: add sb, sb, #4
004c5504: str r2, [sp, #8]
004c5508: add sl, sp, #0x12c
004c550c: str r3, [sp, #0x14]
004c5510: add r6, sp, #0x2c
004c5514: add fp, r8, #1
004c5518: str ip, [sp]
004c551c: mov r0, r5
004c5520: mov r1, sl
004c5524: mov r2, #0x100
004c5528: mov r3, #0
004c552c: bl #0x317734
004c5530: cmp r0, #0
004c5534: beq #0x4c5668
004c5538: mov r0, r5
004c553c: bl #0x313a90
004c5540: mov r3, #1
004c5544: cmp r3, #0
004c5548: str r0, [sp, #0x20]
004c554c: str r3, [sp, #0x18]
004c5550: bne #0x4c55a0
004c5554: ldr r1, [sp, #0x10]
004c5558: ldr r3, [sp, #0x14]
004c555c: add r2, r1, #2
004c5560: ldrb r0, [r2, #1]
004c5564: ldrb r1, [r3, #-1]
004c5568: cmp r2, r3
004c556c: mov r4, r2
004c5570: eor r1, r0, r1
004c5574: strb r1, [r3, #-1]
004c5578: ldrb r0, [r2, #1]
004c557c: eor r1, r1, r0
004c5580: strb r1, [r2, #1]
004c5584: ldrb r0, [r3, #-1]
004c5588: sub r2, r2, #1
004c558c: eor r1, r1, r0
004c5590: strb r1, [r3, #-1]
004c5594: add r3, r3, #1
004c5598: bhi #0x4c5560
004c559c: ldr r0, [sp, #0x20]
004c55a0: cmp r0, #0
004c55a4: beq #0x4c5650
004c55a8: mov r4, #0
004c55ac: mov r7, #1
004c55b0: mov r0, r5
004c55b4: mov r1, r6
004c55b8: mov r2, #0x100
004c55bc: mov r3, #0
004c55c0: bl #0x317734
004c55c4: cmp r0, #0
004c55c8: beq #0x4c5668
004c55cc: mov r0, r5
004c55d0: mov r1, r8
004c55d4: bl #0x459090
004c55d8: cmp r7, #0
004c55dc: str r7, [sp, #0x18]
004c55e0: bne #0x4c5624
004c55e4: ldr r2, [sp]
004c55e8: mov r3, fp
004c55ec: ldrb r0, [r2, #1]
004c55f0: ldrb r1, [r3, #-1]
004c55f4: cmp r2, r3
004c55f8: eor r1, r0, r1
004c55fc: strb r1, [r3, #-1]
004c5600: ldrb r0, [r2, #1]
004c5604: eor r1, r1, r0
004c5608: strb r1, [r2, #1]
004c560c: ldrb r0, [r3, #-1]
004c5610: sub r2, r2, #1
004c5614: eor r1, r1, r0
004c5618: strb r1, [r3, #-1]
004c561c: add r3, r3, #1
004c5620: bhi #0x4c55ec
004c5624: mov r1, sl
004c5628: mov r0, sb
004c562c: bl #0x4c5274
004c5630: mov r1, r6
004c5634: bl #0x4c4c30
004c5638: ldr r3, [sp, #0x1c]
004c563c: add r4, r4, #1
004c5640: str r3, [r0]
004c5644: ldr r3, [sp, #0x20]
004c5648: cmp r3, r4
004c564c: bhi #0x4c55b0
004c5650: ldr r2, [sp, #8]
004c5654: ldr r3, [sp, #0x24]
004c5658: add r2, r2, #1
004c565c: cmp r3, r2
004c5660: str r2, [sp, #8]
004c5664: bhi #0x4c551c
004c5668: ldr r1, [sp, #4]
004c566c: ldr ip, [sp, #0xc]
004c5670: ldr r2, [sp, #0x244]
004c5674: ldr r3, [r1, ip]
004c5678: ldr r3, [r3]
004c567c: cmp r2, r3
004c5680: bne #0x4c568c
004c5684: add sp, sp, #0x24c
004c5688: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004c568c: bl #0x30e310
004c5690: subeq pc, ip, r4, ror r6
004c5694: andeq r4, r0, ip, lsr #1
004c5698: andeq r0, r0, r4, lsl #17
004c569c: subeq r3, r1, ip, asr lr

# _ZN12StreamReader10readStringEP11IStreamBasePcy
00317734: push {r4, r5, r6, r7, r8, sb, lr}
00317738: sub sp, sp, #0xc
0031773c: mov sb, r3
00317740: mov r6, r1
00317744: mov r7, r0
00317748: mov r8, r2
0031774c: bl #0x313a90
00317750: mov r3, #1
00317754: cmp r3, #0
00317758: mov r4, r0
0031775c: str r0, [sp, #4]
00317760: str r3, [sp]
00317764: bne #0x3177b0
00317768: add r3, sp, #4
0031776c: add r2, r3, #2
00317770: add r3, r3, #1
00317774: ldrb r0, [r2, #1]
00317778: ldrb r1, [r3, #-1]
0031777c: cmp r3, r2
00317780: eor r1, r0, r1
00317784: strb r1, [r3, #-1]
00317788: ldrb r0, [r2, #1]
0031778c: eor r1, r1, r0
00317790: strb r1, [r2, #1]
00317794: ldrb r0, [r3, #-1]
00317798: sub r2, r2, #1
0031779c: eor r1, r1, r0
003177a0: strb r1, [r3, #-1]
003177a4: add r3, r3, #1
003177a8: blo #0x317774
003177ac: ldr r4, [sp, #4]
003177b0: mvn r0, #0
003177b4: adds r0, r0, r8
003177b8: mvn r1, #0
003177bc: adc r1, r1, sb
003177c0: mov r3, #0
003177c4: cmp r3, r1
003177c8: mov r5, r4
003177cc: beq #0x317818
003177d0: mov r0, r7
003177d4: ldr ip, [r7]
003177d8: mov r1, r6
003177dc: mov r2, r5
003177e0: mov lr, pc
003177e4: ldr pc, [ip, #0x18]
003177e8: mov r0, #0
003177ec: cmp sb, #0
003177f0: strb r0, [r6, r5]
003177f4: bhi #0x317810
003177f8: beq #0x317808
003177fc: and r0, r0, #1
00317800: add sp, sp, #0xc
00317804: pop {r4, r5, r6, r7, r8, sb, pc}
00317808: cmp r8, r4
0031780c: bls #0x3177fc
00317810: mov r0, #1
00317814: b #0x3177fc
00317818: cmp r4, r0
0031781c: movhi r5, r0
00317820: movls r5, r4
00317824: b #0x3177d0

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

# _ZNSt3mapISsS_ISsiSt4lessISsESaISt4pairIKSsiEEES1_SaIS2_IS3_S6_EEEixIA256_cEERS6_RKT_
004c5274: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004c5278: ldr r4, [pc, #0x184]
004c527c: ldr sb, [pc, #0x184]
004c5280: sub sp, sp, #0x9c
004c5284: add r4, pc, r4
004c5288: ldr r3, [r4, sb]
004c528c: mov fp, r0
004c5290: mov r6, r1
004c5294: ldr r3, [r3]
004c5298: str r3, [sp, #0x94]
004c529c: bl #0x4c45a0
004c52a0: cmp r0, fp
004c52a4: mov r5, r0
004c52a8: beq #0x4c533c
004c52ac: add r7, sp, #0x7c
004c52b0: mov r1, r6
004c52b4: add r2, sp, #0x30
004c52b8: mov r0, r7
004c52bc: bl #0x3140ec
004c52c0: ldr r3, [sp, #0x90]
004c52c4: ldr r1, [r5, #0x24]
004c52c8: ldr r8, [r5, #0x20]
004c52cc: ldr sl, [sp, #0x8c]
004c52d0: mov r0, r3
004c52d4: rsb r8, r1, r8
004c52d8: rsb sl, r3, sl
004c52dc: cmp r8, sl
004c52e0: movlt r2, r8
004c52e4: movge r2, sl
004c52e8: bl #0x30e5e0
004c52ec: cmp r0, #0
004c52f0: mov r3, r5
004c52f4: bne #0x4c5330
004c52f8: cmp sl, r8
004c52fc: blt #0x4c5334
004c5300: mov r0, r7
004c5304: str r3, [sp, #4]
004c5308: bl #0x318254
004c530c: ldr r3, [sp, #4]
004c5310: ldr r1, [r4, sb]
004c5314: ldr r2, [sp, #0x94]
004c5318: add r0, r3, #0x28
004c531c: ldr r3, [r1]
004c5320: cmp r2, r3
004c5324: bne #0x4c5400
004c5328: add sp, sp, #0x9c
004c532c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004c5330: bge #0x4c5300
004c5334: mov r0, r7
004c5338: bl #0x318254
004c533c: add sl, sp, #0x64
004c5340: add r7, sp, #0x98
004c5344: mov r8, #0
004c5348: mov r1, r6
004c534c: add r2, sp, #0x2c
004c5350: add r6, sp, #0x34
004c5354: mov r0, sl
004c5358: bl #0x3140ec
004c535c: strb r8, [r7, #-0x8c]!
004c5360: ldr r2, [sp, #0x74]
004c5364: mov r0, r6
004c5368: ldr r1, [sp, #0x78]
004c536c: str r8, [sp, #0x10]
004c5370: str r7, [sp, #0x14]
004c5374: str r7, [sp, #0x18]
004c5378: str r8, [sp, #0x1c]
004c537c: str r6, [sp, #0x44]
004c5380: str r6, [sp, #0x48]
004c5384: bl #0x3116e8
004c5388: mov r1, r7
004c538c: add r0, r6, #0x18
004c5390: bl #0x4c43dc
004c5394: mov r3, r6
004c5398: mov r1, fp
004c539c: add r0, sp, #0x28
004c53a0: add r2, sp, #0x24
004c53a4: str r5, [sp, #0x24]
004c53a8: bl #0x4c4d70
004c53ac: mov r0, r6
004c53b0: ldr r5, [sp, #0x28]
004c53b4: bl #0x4c41c0
004c53b8: ldr r3, [sp, #0x1c]
004c53bc: cmp r3, r8
004c53c0: bne #0x4c53d4
004c53c4: mov r0, sl
004c53c8: bl #0x318254
004c53cc: mov r3, r5
004c53d0: b #0x4c5310
004c53d4: mov r0, r7
004c53d8: ldr r1, [sp, #0x10]
004c53dc: bl #0x3f1b40
004c53e0: mov r0, sl
004c53e4: str r7, [sp, #0x18]
004c53e8: str r8, [sp, #0x1c]
004c53ec: str r7, [sp, #0x14]
004c53f0: str r8, [sp, #0x10]
004c53f4: bl #0x318254
004c53f8: mov r3, r5
004c53fc: b #0x4c5310
004c5400: bl #0x30e310
004c5404: subeq pc, ip, ip, lsl #16
004c5408: andeq r4, r0, ip, lsr #1

# _ZNSt3mapISsiSt4lessISsESaISt4pairIKSsiEEEixIA256_cEERiRKT_
004c4c30: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004c4c34: ldr r4, [pc, #0x12c]
004c4c38: ldr r7, [pc, #0x12c]
004c4c3c: sub sp, sp, #0x6c
004c4c40: add r4, pc, r4
004c4c44: ldr r3, [r4, r7]
004c4c48: mov r8, r0
004c4c4c: mov r6, r1
004c4c50: ldr r3, [r3]
004c4c54: str r3, [sp, #0x64]
004c4c58: bl #0x4c4690
004c4c5c: cmp r0, r8
004c4c60: mov r5, r0
004c4c64: beq #0x4c4cf8
004c4c68: add sl, sp, #0x4c
004c4c6c: mov r1, r6
004c4c70: add r2, sp, #0x14
004c4c74: mov r0, sl
004c4c78: bl #0x3140ec
004c4c7c: ldr r3, [sp, #0x60]
004c4c80: ldr r1, [r5, #0x24]
004c4c84: ldr fp, [r5, #0x20]
004c4c88: ldr sb, [sp, #0x5c]
004c4c8c: mov r0, r3
004c4c90: rsb fp, r1, fp
004c4c94: rsb sb, r3, sb
004c4c98: cmp fp, sb
004c4c9c: movlt r2, fp
004c4ca0: movge r2, sb
004c4ca4: bl #0x30e5e0
004c4ca8: cmp r0, #0
004c4cac: mov r3, r5
004c4cb0: bne #0x4c4cec
004c4cb4: cmp sb, fp
004c4cb8: blt #0x4c4cf0
004c4cbc: mov r0, sl
004c4cc0: str r3, [sp, #4]
004c4cc4: bl #0x318254
004c4cc8: ldr r3, [sp, #4]
004c4ccc: ldr r1, [r4, r7]
004c4cd0: ldr r2, [sp, #0x64]
004c4cd4: add r0, r3, #0x28
004c4cd8: ldr r3, [r1]
004c4cdc: cmp r2, r3
004c4ce0: bne #0x4c4d64
004c4ce4: add sp, sp, #0x6c
004c4ce8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004c4cec: bge #0x4c4cbc
004c4cf0: mov r0, sl
004c4cf4: bl #0x318254
004c4cf8: add sl, sp, #0x34
004c4cfc: mov r1, r6
004c4d00: add r2, sp, #0x10
004c4d04: add r6, sp, #0x18
004c4d08: mov r0, sl
004c4d0c: bl #0x3140ec
004c4d10: mov r0, r6
004c4d14: ldr r1, [sp, #0x48]
004c4d18: ldr r2, [sp, #0x44]
004c4d1c: str r6, [sp, #0x28]
004c4d20: str r6, [sp, #0x2c]
004c4d24: bl #0x3116e8
004c4d28: mov r3, r6
004c4d2c: mov ip, #0
004c4d30: mov r1, r8
004c4d34: add r2, sp, #8
004c4d38: add r0, sp, #0xc
004c4d3c: str ip, [sp, #0x30]
004c4d40: str r5, [sp, #8]
004c4d44: bl #0x414b40
004c4d48: ldr r5, [sp, #0xc]
004c4d4c: mov r0, r6
004c4d50: bl #0x318254
004c4d54: mov r0, sl
004c4d58: bl #0x318254
004c4d5c: mov r3, r5
004c4d60: b #0x4c4ccc
004c4d64: bl #0x30e310
004c4d68: subeq pc, ip, r0, asr lr
004c4d6c: andeq r4, r0, ip, lsr #1

# _ZNK15PyDataConstants11getConstantEPKcS1_
004c4bdc: push {r4, lr}
004c4be0: add r4, r0, #4
004c4be4: sub sp, sp, #8
004c4be8: str r1, [sp, #4]
004c4bec: mov r0, r4
004c4bf0: add r1, sp, #4
004c4bf4: str r2, [sp]
004c4bf8: bl #0x4c4998
004c4bfc: cmp r0, r4
004c4c00: beq #0x4c4c28
004c4c04: add r4, r0, #0x28
004c4c08: mov r0, r4
004c4c0c: mov r1, sp
004c4c10: bl #0x414484
004c4c14: cmp r0, r4
004c4c18: ldrne r0, [r0, #0x28]
004c4c1c: beq #0x4c4c28
004c4c20: add sp, sp, #8
004c4c24: pop {r4, pc}
004c4c28: mov r0, #0
004c4c2c: b #0x4c4c20

# _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsSt3mapISsiS2_SaIS3_IS4_iEEEENS_10_Select1stIS9_EENS_11_MapTraitsTIS9_EESaIS9_EE7_M_findIPKcEEPNS_18_Rb_tree_node_baseERKT_
004c4998: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004c499c: ldr fp, [pc, #0x15c]
004c49a0: ldr r2, [pc, #0x15c]
004c49a4: sub sp, sp, #0x54
004c49a8: add fp, pc, fp
004c49ac: ldr r3, [fp, r2]
004c49b0: str r2, [sp, #0xc]
004c49b4: str r0, [sp, #8]
004c49b8: ldr r4, [r0, #4]
004c49bc: ldr r3, [r3]
004c49c0: mov r8, r1
004c49c4: cmp r4, #0
004c49c8: str r3, [sp, #0x4c]
004c49cc: beq #0x4c4ad4
004c49d0: mov sl, r0
004c49d4: add r7, sp, #0x34
004c49d8: add sb, sp, #0x18
004c49dc: ldr r1, [r8]
004c49e0: mov r2, sb
004c49e4: mov r0, r7
004c49e8: bl #0x3140ec
004c49ec: ldr r3, [r4, #0x24]
004c49f0: ldr r1, [sp, #0x48]
004c49f4: ldr r6, [r4, #0x20]
004c49f8: ldr r5, [sp, #0x44]
004c49fc: mov r0, r3
004c4a00: rsb r6, r3, r6
004c4a04: rsb r5, r1, r5
004c4a08: cmp r5, r6
004c4a0c: movlt r2, r5
004c4a10: movge r2, r6
004c4a14: bl #0x30e5e0
004c4a18: subs r3, r0, #0
004c4a1c: bne #0x4c4a34
004c4a20: cmp r6, r5
004c4a24: mvnlt r3, #0
004c4a28: blt #0x4c4a34
004c4a2c: movle r3, #0
004c4a30: movgt r3, #1
004c4a34: mov r0, r7
004c4a38: str r3, [sp, #4]
004c4a3c: bl #0x318254
004c4a40: ldr r3, [sp, #4]
004c4a44: cmp r3, #0
004c4a48: movge sl, r4
004c4a4c: ldrlt r4, [r4, #0xc]
004c4a50: ldrge r4, [r4, #8]
004c4a54: cmp r4, #0
004c4a58: bne #0x4c49dc
004c4a5c: ldr r3, [sp, #8]
004c4a60: cmp sl, r3
004c4a64: beq #0x4c4ad8
004c4a68: add r4, sp, #0x1c
004c4a6c: ldr r1, [r8]
004c4a70: add r2, sp, #0x14
004c4a74: mov r0, r4
004c4a78: bl #0x3140ec
004c4a7c: ldr r3, [sp, #0x30]
004c4a80: ldr r1, [sl, #0x24]
004c4a84: ldr r5, [sl, #0x20]
004c4a88: ldr r6, [sp, #0x2c]
004c4a8c: mov r0, r3
004c4a90: rsb r5, r1, r5
004c4a94: rsb r6, r3, r6
004c4a98: cmp r5, r6
004c4a9c: movlt r2, r5
004c4aa0: movge r2, r6
004c4aa4: bl #0x30e5e0
004c4aa8: subs r7, r0, #0
004c4aac: bne #0x4c4ac4
004c4ab0: cmp r6, r5
004c4ab4: mvnlt r7, #0
004c4ab8: blt #0x4c4ac4
004c4abc: movle r7, #0
004c4ac0: movgt r7, #1
004c4ac4: mov r0, r4
004c4ac8: bl #0x318254
004c4acc: cmp r7, #0
004c4ad0: bge #0x4c4ad8
004c4ad4: ldr sl, [sp, #8]
004c4ad8: ldr r2, [sp, #0xc]
004c4adc: mov r0, sl
004c4ae0: ldr r3, [fp, r2]
004c4ae4: ldr r2, [sp, #0x4c]
004c4ae8: ldr r3, [r3]
004c4aec: cmp r2, r3
004c4af0: bne #0x4c4afc
004c4af4: add sp, sp, #0x54
004c4af8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004c4afc: bl #0x30e310
004c4b00: subeq r0, sp, r8, ror #1
004c4b04: andeq r4, r0, ip, lsr #1

# _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE7_M_findIPKcEEPNS_18_Rb_tree_node_baseERKT_
00414484: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00414488: ldr r2, [pc, #0x1e0]
0041448c: ldr r3, [pc, #0x1e0]
00414490: sub sp, sp, #0x54
00414494: add r2, pc, r2
00414498: str r3, [sp, #0xc]
0041449c: ldr r3, [r2, r3]
004144a0: str r2, [sp, #4]
004144a4: str r0, [sp, #8]
004144a8: ldr r5, [r0, #4]
004144ac: ldr r3, [r3]
004144b0: mov sb, r1
004144b4: cmp r5, #0
004144b8: str r3, [sp, #0x4c]
004144bc: beq #0x41462c
004144c0: mov sl, r0
004144c4: add r0, sp, #0x18
004144c8: add r8, sp, #0x34
004144cc: str r0, [sp]
004144d0: b #0x4144f4
004144d4: mov r0, r4
004144d8: bl #0x708f00
004144dc: cmp fp, #0
004144e0: movge sl, r5
004144e4: ldrlt r5, [r5, #0xc]
004144e8: ldrge r5, [r5, #8]
004144ec: cmp r5, #0
004144f0: beq #0x414590
004144f4: ldr r1, [sb]
004144f8: ldr r2, [sp]
004144fc: mov r0, r8
00414500: bl #0x3140ec
00414504: ldr r3, [r5, #0x24]
00414508: ldr r4, [sp, #0x48]
0041450c: ldr r7, [r5, #0x20]
00414510: ldr r6, [sp, #0x44]
00414514: mov r0, r3
00414518: rsb r7, r3, r7
0041451c: rsb r6, r4, r6
00414520: cmp r6, r7
00414524: movlt r2, r6
00414528: movge r2, r7
0041452c: mov r1, r4
00414530: bl #0x30e5e0
00414534: subs fp, r0, #0
00414538: bne #0x414550
0041453c: cmp r7, r6
00414540: mvnlt fp, #0
00414544: blt #0x414550
00414548: movle fp, #0
0041454c: movgt fp, #1
00414550: cmp r4, r8
00414554: beq #0x4144dc
00414558: cmp r4, #0
0041455c: beq #0x4144dc
00414560: ldr r1, [sp, #0x34]
00414564: rsb r1, r4, r1
00414568: cmp r1, #0x80
0041456c: bls #0x4144d4
00414570: mov r0, r4
00414574: bl #0x310440
00414578: cmp fp, #0
0041457c: movge sl, r5
00414580: ldrlt r5, [r5, #0xc]
00414584: ldrge r5, [r5, #8]
00414588: cmp r5, #0
0041458c: bne #0x4144f4
00414590: ldr r1, [sp, #8]
00414594: cmp sl, r1
00414598: beq #0x414630
0041459c: add r5, sp, #0x1c
004145a0: ldr r1, [sb]
004145a4: add r2, sp, #0x14
004145a8: mov r0, r5
004145ac: bl #0x3140ec
004145b0: ldr r3, [sl, #0x24]
004145b4: ldr r4, [sp, #0x30]
004145b8: ldr r7, [sl, #0x20]
004145bc: ldr r6, [sp, #0x2c]
004145c0: mov r1, r3
004145c4: rsb r7, r3, r7
004145c8: rsb r6, r4, r6
004145cc: cmp r7, r6
004145d0: movlt r2, r7
004145d4: movge r2, r6
004145d8: mov r0, r4
004145dc: bl #0x30e5e0
004145e0: subs r8, r0, #0
004145e4: bne #0x4145fc
004145e8: cmp r6, r7
004145ec: mvnlt r8, #0
004145f0: blt #0x4145fc
004145f4: movle r8, #0
004145f8: movgt r8, #1
004145fc: cmp r4, r5
00414600: beq #0x414624
00414604: cmp r4, #0
00414608: beq #0x414624
0041460c: ldr r1, [sp, #0x1c]
00414610: rsb r1, r4, r1
00414614: cmp r1, #0x80
00414618: bhi #0x414658
0041461c: mov r0, r4
00414620: bl #0x708f00
00414624: cmp r8, #0
00414628: bge #0x414630
0041462c: ldr sl, [sp, #8]
00414630: ldr r0, [sp, #4]
00414634: ldr r2, [sp, #0xc]
00414638: ldr r3, [r0, r2]
0041463c: ldr r2, [sp, #0x4c]
00414640: mov r0, sl
00414644: ldr r3, [r3]
00414648: cmp r2, r3
0041464c: bne #0x41466c
00414650: add sp, sp, #0x54
00414654: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00414658: mov r0, r4
0041465c: bl #0x310440
00414660: cmp r8, #0
00414664: blt #0x41462c
00414668: b #0x414630
0041466c: bl #0x30e310
00414670: ldrsheq r0, [r8], #-0x5c
00414674: andeq r4, r0, ip, lsr #1
