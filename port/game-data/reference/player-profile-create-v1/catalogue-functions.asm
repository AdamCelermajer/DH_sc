_ZN14PlayerSavegame18SG_GetNextFreeSlotEv 0x4660c8
004660c8: push {r4, r5, r6, lr}
004660cc: sub sp, sp, #0x10
004660d0: add r4, sp, #4
004660d4: mov r0, r4
004660d8: mov r1, #0
004660dc: bl #0x46403c
004660e0: ldmib sp, {r0, r1}
004660e4: bl #0x466040
004660e8: ldr r3, [sp, #4]
004660ec: ldr r2, [sp, #8]
004660f0: rsb r2, r3, r2
004660f4: asr r2, r2, #3
004660f8: add r5, r2, r2, lsl #2
004660fc: add r5, r5, r5, lsl #4
00466100: add r5, r5, r5, lsl #8
00466104: add r5, r5, r5, lsl #16
00466108: adds r5, r2, r5, lsl #1
0046610c: beq #0x46612c
00466110: mov r6, #0
00466114: mov r5, r6
00466118: add r3, r3, r6
0046611c: ldr r0, [r3, #0x14]
00466120: bl #0x4636a8
00466124: cmp r5, r0
00466128: beq #0x466140
0046612c: mov r0, r4
00466130: bl #0x313f30
00466134: mov r0, r5
00466138: add sp, sp, #0x10
0046613c: pop {r4, r5, r6, pc}
00466140: bl #0x464d5c
00466144: cmp r0, #0
00466148: beq #0x46612c
0046614c: ldr r3, [sp, #4]
00466150: ldr r2, [sp, #8]
00466154: add r5, r5, #1
00466158: add r6, r6, #0x18
0046615c: rsb r2, r3, r2
00466160: asr r2, r2, #3
00466164: add r1, r2, r2, lsl #2
00466168: add r1, r1, r1, lsl #4
0046616c: add r1, r1, r1, lsl #8
00466170: add r1, r1, r1, lsl #16
00466174: add r2, r2, r1, lsl #1
00466178: cmp r5, r2
0046617c: blo #0x466118
00466180: b #0x46612c

_ZN14PlayerSavegame18SG_GetSavegameListEb 0x46403c
0046403c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00464040: ldr r8, [pc, #0x274]
00464044: ldr r2, [pc, #0x274]
00464048: ldr sl, [pc, #0x274]
0046404c: sub sp, sp, #0x34
00464050: add r8, pc, r8
00464054: str r2, [sp, #8]
00464058: ldr r2, [r8, r2]
0046405c: ldr r3, [r8, sl]
00464060: mov r6, r0
00464064: mov r0, #0
00464068: ldr r2, [r2]
0046406c: str r0, [r6, #8]
00464070: str r0, [r6]
00464074: str r0, [r6, #4]
00464078: ldr r3, [r3, #0x10]
0046407c: str r2, [sp, #0x2c]
00464080: str r1, [sp, #4]
00464084: ldr r3, [r3, #0x34]
00464088: cmp r3, r0
0046408c: beq #0x464178
00464090: ldr r1, [pc, #0x230]
00464094: mov r0, r3
00464098: mov r2, r6
0046409c: ldr r3, [r3]
004640a0: add r1, pc, r1
004640a4: mov lr, pc
004640a8: ldr pc, [r3, #0x7c]
004640ac: ldr r3, [pc, #0x218]
004640b0: ldr r5, [pc, #0x218]
004640b4: ldr sb, [pc, #0x218]
004640b8: add r3, pc, r3
004640bc: str r3, [sp, #0xc]
004640c0: add r5, pc, r5
004640c4: add sb, pc, sb
004640c8: ldr fp, [r6]
004640cc: add r4, sp, #0x10
004640d0: b #0x4640e8
004640d4: mov r1, fp
004640d8: mov r0, r6
004640dc: mov r2, r4
004640e0: bl #0x3303ec
004640e4: mov fp, r0
004640e8: ldr r3, [r6, #4]
004640ec: cmp fp, r3
004640f0: beq #0x46416c
004640f4: ldr r7, [fp, #0x14]
004640f8: mov r1, r5
004640fc: mov r0, r7
00464100: bl #0x30ebd4
00464104: cmp r0, #0
00464108: bne #0x4640d4
0046410c: mov r0, r7
00464110: mov r1, sb
00464114: bl #0x30ebd4
00464118: cmp r0, #0
0046411c: bne #0x4640d4
00464120: mov r0, r7
00464124: ldr r1, [sp, #0xc]
00464128: bl #0x30ebd4
0046412c: cmp r0, #0
00464130: bne #0x4640d4
00464134: bl #0x4634f4
00464138: mov r1, r0
0046413c: mov r0, r7
00464140: bl #0x30ebd4
00464144: cmp r0, #0
00464148: beq #0x4640d4
0046414c: bl #0x4634e4
00464150: mov r1, r0
00464154: mov r0, r7
00464158: bl #0x30ebd4
0046415c: cmp r0, #0
00464160: beq #0x4640d4
00464164: add fp, fp, #0x18
00464168: b #0x4640e8
0046416c: ldr r3, [sp, #4]
00464170: cmp r3, #0
00464174: bne #0x46419c
00464178: ldr r2, [sp, #8]
0046417c: mov r0, r6
00464180: ldr r3, [r8, r2]
00464184: ldr r2, [sp, #0x2c]
00464188: ldr r3, [r3]
0046418c: cmp r2, r3
00464190: bne #0x4642b8
00464194: add sp, sp, #0x34
00464198: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046419c: ldr r2, [r6]
004641a0: rsb fp, r2, fp
004641a4: asr r3, fp, #3
004641a8: add r1, r3, r3, lsl #2
004641ac: add r1, r1, r1, lsl #4
004641b0: add r1, r1, r1, lsl #8
004641b4: add r1, r1, r1, lsl #16
004641b8: add r3, r3, r1, lsl #1
004641bc: cmp r3, #0
004641c0: beq #0x464178
004641c4: mov r1, #0
004641c8: str r1, [sp, #4]
004641cc: add sb, sp, #0x14
004641d0: mov r4, #0
004641d4: mov r7, r4
004641d8: b #0x4641e8
004641dc: ldm r6, {r2, fp}
004641e0: mov r4, r5
004641e4: rsb fp, r2, fp
004641e8: asr r3, fp, #3
004641ec: add r1, r3, r3, lsl #2
004641f0: add r1, r1, r1, lsl #4
004641f4: add r1, r1, r1, lsl #8
004641f8: add r1, r1, r1, lsl #16
004641fc: add r3, r3, r1, lsl #1
00464200: sub r1, r3, #1
00464204: cmp r1, r7
00464208: bls #0x4642a0
0046420c: ldr r3, [r8, sl]
00464210: add r5, r4, #0x18
00464214: add r0, r2, r4
00464218: ldr r3, [r3, #0x10]
0046421c: add r2, r2, r5
00464220: ldr r1, [r2, #0x14]
00464224: ldr r3, [r3, #0x34]
00464228: ldr r2, [r0, #0x14]
0046422c: add r7, r7, #1
00464230: mov r0, r3
00464234: ldr r3, [r3]
00464238: mov lr, pc
0046423c: ldr pc, [r3, #0xb4]
00464240: cmp r0, #0
00464244: beq #0x4641dc
00464248: ldr r1, [r6]
0046424c: mov r0, sb
00464250: add r1, r1, r4
00464254: bl #0x32b918
00464258: ldr r3, [r6]
0046425c: add r0, r3, r4
00464260: add r3, r3, r5
00464264: cmp r0, r3
00464268: beq #0x464280
0046426c: ldr r2, [r3, #0x10]
00464270: ldr r1, [r3, #0x14]
00464274: bl #0x3109e0
00464278: ldr r0, [r6]
0046427c: add r0, r0, r5
00464280: cmp r0, sb
00464284: beq #0x464294
00464288: ldr r1, [sp, #0x28]
0046428c: ldr r2, [sp, #0x24]
00464290: bl #0x3109e0
00464294: mov r0, sb
00464298: bl #0x3139ac
0046429c: b #0x4641dc
004642a0: ldr r1, [sp, #4]
004642a4: add r1, r1, #1
004642a8: cmp r1, r3
004642ac: str r1, [sp, #4]
004642b0: blo #0x4641d0
004642b4: b #0x464178
004642b8: bl #0x30e310
004642bc: subseq r0, r3, r0, asr #20
004642c0: andeq r4, r0, ip, lsr #1
004642c4: strdeq r3, r4, [r0], -r4
004642c8: subeq sb, r6, r8, lsr #1
004642cc: subeq fp, sl, r0, lsr #17
004642d0: subeq sl, r5, r0, lsr #9
004642d4: subeq sb, r6, r4, lsl r1

_ZN14PlayerSavegame22SG_GetSlotFromFilenameEPKc 0x4636a8
004636a8: push {r4, lr}
004636ac: mov r4, r0
004636b0: bl #0x4634e4
004636b4: bl #0x30de54
004636b8: add r0, r4, r0
004636bc: pop {r4, lr}
004636c0: b #0x30e094

_ZN6glitch2os5Timer11getRealTimeEv 0x60b0cc
0060b0cc: str lr, [sp, #-4]!
0060b0d0: sub sp, sp, #0xc
0060b0d4: mov r1, #0
0060b0d8: mov r0, sp
0060b0dc: bl #0x30e724
0060b0e0: ldr r2, [sp, #4]
0060b0e4: movw r3, #0x4dd3
0060b0e8: movt r3, #0x1062
0060b0ec: smull r1, r3, r3, r2
0060b0f0: asr r2, r2, #0x1f
0060b0f4: rsb r3, r2, r3, asr #6
0060b0f8: ldr r2, [sp]
0060b0fc: mov r0, #0x3e8
0060b100: mla r0, r0, r2, r3
0060b104: add sp, sp, #0xc
0060b108: ldm sp!, {pc}
