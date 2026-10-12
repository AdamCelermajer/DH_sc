# _ZN11PropertyMap19SavePropertiesToXMLEP12TiXmlElementPKc 0x5134f0 size=388
005134f0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005134f4: ldr r3, [pc, #0x16c]
005134f8: ldr r4, [pc, #0x16c]
005134fc: sub sp, sp, #0x54
00513500: add r3, pc, r3
00513504: str r3, [sp, #4]
00513508: ldr r3, [r3, r4]
0051350c: cmp r2, #0
00513510: str r4, [sp, #8]
00513514: ldr r3, [r3]
00513518: mov r8, r0
0051351c: str r1, [sp, #0xc]
00513520: str r3, [sp, #0x4c]
00513524: movne r4, r2
00513528: beq #0x513658
0051352c: ldr ip, [sp, #0xc]
00513530: cmp ip, #0
00513534: beq #0x513600
00513538: mov r1, #0
0051353c: mov r0, #0x8c
00513540: bl #0x310570 ; _Znwj15MemoryHintState
00513544: mov r1, r4
00513548: mov sl, r0
0051354c: bl #0x51745c ; _ZN12TiXmlElementC1EPKc
00513550: mov r0, r8
00513554: bl #0x5134c0 ; _ZN11PropertyMap14GetPropertyMapEv
00513558: ldr r4, [r0, #8]
0051355c: mov r7, r0
00513560: add r6, sp, #0x1c
00513564: add sb, sp, #0x18
00513568: add r5, sp, #0x34
0051356c: add fp, sp, #0x10
00513570: cmp r7, r4
00513574: beq #0x5135f4
00513578: ldr r3, [r4, #0x28]
0051357c: cmp r3, #0
00513580: beq #0x5135c8
00513584: str r3, [sp, #0x10]
00513588: str r8, [sp, #0x14]
0051358c: ldr r1, [r3, #0x1c]
00513590: mov r2, sb
00513594: mov r0, r6
00513598: bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0051359c: mov r0, r5
005135a0: mov r1, fp
005135a4: bl #0x510fe0 ; _ZN16PropertyInstance8ToStringEv
005135a8: mov r0, sl
005135ac: mov r1, r6
005135b0: mov r2, r5
005135b4: bl #0x516400 ; _ZN12TiXmlElement12SetAttributeERKSsS1_
005135b8: mov r0, r5
005135bc: bl #0x318254 ; _ZNSsD1Ev
005135c0: mov r0, r6
005135c4: bl #0x318254 ; _ZNSsD1Ev
005135c8: ldr r2, [r4, #0xc]
005135cc: cmp r2, #0
005135d0: bne #0x5135dc
005135d4: b #0x513624
005135d8: mov r2, r3
005135dc: ldr r3, [r2, #8]
005135e0: cmp r3, #0
005135e4: bne #0x5135d8
005135e8: mov r4, r2
005135ec: cmp r7, r4
005135f0: bne #0x513578
005135f4: ldr r0, [sp, #0xc]
005135f8: mov r1, sl
005135fc: bl #0x515964 ; _ZN9TiXmlNode12LinkEndChildEPS_
00513600: ldr r2, [sp, #4]
00513604: ldr r1, [sp, #8]
00513608: ldr r3, [r2, r1]
0051360c: ldr r2, [sp, #0x4c]
00513610: ldr r3, [r3]
00513614: cmp r2, r3
00513618: bne #0x513664
0051361c: add sp, sp, #0x54
00513620: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00513624: ldr r3, [r4, #4]
00513628: ldr r1, [r3, #0xc]
0051362c: cmp r4, r1
00513630: bne #0x51364c
00513634: mov r4, r3
00513638: ldr r3, [r3, #4]
0051363c: ldr r2, [r3, #0xc]
00513640: cmp r2, r4
00513644: beq #0x513634
00513648: ldr r2, [r4, #0xc]
0051364c: cmp r2, r3
00513650: movne r4, r3
00513654: b #0x513570
00513658: ldr r4, [pc, #0x10]
0051365c: add r4, pc, r4
00513660: b #0x51352c
00513664: bl #0x30e310 ; 
00513668: umaaleq r1, r8, r0, r5
0051366c: andeq r4, r0, ip, lsr #1
00513670: eorseq ip, sl, ip, lsl #24
