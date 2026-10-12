# _ZN3rnd15RandomGenerator12LoadRuleFileEPKc 0x489508 size=1152
00489508: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0048950c: ldr r5, [pc, #0x434]
00489510: ldr r2, [pc, #0x434]
00489514: sub sp, sp, #0x64
00489518: add r5, pc, r5
0048951c: ldr r3, [r5, r2]
00489520: mov r4, r0
00489524: mov r0, r1
00489528: ldr r3, [r3]
0048952c: mov r8, r1
00489530: str r2, [sp, #8]
00489534: str r3, [sp, #0x5c]
00489538: bl #0x30de54 ; 
0048953c: add r6, r4, #0x174
00489540: add r2, r8, r0
00489544: mov r1, r8
00489548: mov r0, r6
0048954c: bl #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
00489550: ldr r3, [pc, #0x3f8]
00489554: mov r0, r6
00489558: ldr r3, [r5, r3]
0048955c: ldr r3, [r3, #0x10]
00489560: ldr r7, [r3, #0x34]
00489564: bl #0x4847ac ; $a
00489568: cmn r0, #1
0048956c: beq #0x4898e8
00489570: add sb, r0, #1
00489574: add sl, sp, #0x44
00489578: add ip, sp, #0x28
0048957c: add fp, r4, #0x15c
00489580: mov r0, sl
00489584: mov r1, r6
00489588: mov r2, #0
0048958c: mov r3, sb
00489590: str ip, [sp]
00489594: bl #0x415cd8 ; _ZNSsC1ERKSsjjRKSaIcE
00489598: cmp fp, sl
0048959c: beq #0x4895b0
004895a0: mov r0, fp
004895a4: ldr r1, [sp, #0x58]
004895a8: ldr r2, [sp, #0x54]
004895ac: bl #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
004895b0: ldr r0, [sp, #0x58]
004895b4: cmp r0, sl
004895b8: beq #0x4895d8
004895bc: cmp r0, #0
004895c0: beq #0x4895d8
004895c4: ldr r1, [sp, #0x44]
004895c8: rsb r1, r0, r1
004895cc: cmp r1, #0x80
004895d0: bhi #0x48991c
004895d4: bl #0x708f00 ; $a
004895d8: add sl, sp, #0x2c
004895dc: add ip, sp, #0x24
004895e0: mov r2, sb
004895e4: mov r0, sl
004895e8: mov r1, r6
004895ec: mvn r3, #0
004895f0: str ip, [sp]
004895f4: bl #0x415cd8 ; _ZNSsC1ERKSsjjRKSaIcE
004895f8: cmp r6, sl
004895fc: beq #0x489610
00489600: mov r0, r6
00489604: ldr r1, [sp, #0x40]
00489608: ldr r2, [sp, #0x3c]
0048960c: bl #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
00489610: ldr r0, [sp, #0x40]
00489614: cmp r0, sl
00489618: beq #0x489638
0048961c: cmp r0, #0
00489620: beq #0x489638
00489624: ldr r1, [sp, #0x2c]
00489628: rsb r1, r0, r1
0048962c: cmp r1, #0x80
00489630: bhi #0x489914
00489634: bl #0x708f00 ; $a
00489638: mov r2, #0
0048963c: mov r1, r8
00489640: mov r3, r2
00489644: ldr ip, [r7]
00489648: mov r0, r7
0048964c: mov lr, pc
00489650: ldr pc, [ip, #0x88]
00489654: cmp r0, #0
00489658: mov r3, r0
0048965c: str r0, [sp, #0x20]
00489660: beq #0x4898bc
00489664: ldr r3, [r3]
00489668: mov lr, pc
0048966c: ldr pc, [r3, #8]
00489670: mov r8, r0
00489674: bl #0x30e6f4 ; 
00489678: add r6, sp, #0x60
0048967c: ldr ip, [r6, #-0x40]!
00489680: str r0, [r4, #0x128]
00489684: mov r2, r8
00489688: asr r3, r2, #0x1f
0048968c: mov r1, r0
00489690: mov r0, ip
00489694: ldr ip, [ip]
00489698: mov lr, pc
0048969c: ldr pc, [ip, #0x18]
004896a0: ldr r3, [r7]
004896a4: mov r0, r7
004896a8: mov r1, r6
004896ac: mov lr, pc
004896b0: ldr pc, [r3, #0x78]
004896b4: mov r1, #0
004896b8: mov r0, #0x70
004896bc: bl #0x310570 ; _Znwj15MemoryHintState
004896c0: mov r6, r0
004896c4: bl #0x516ec4 ; _ZN13TiXmlDocumentC1Ev
004896c8: mov r2, r8
004896cc: str r6, [r4, #0x124]
004896d0: mov r0, r6
004896d4: ldr r1, [r4, #0x128]
004896d8: mov r3, #0
004896dc: bl #0x5164a8 ; _ZN13TiXmlDocument14LoadFromBufferEPKvi13TiXmlEncoding
004896e0: ldr r2, [pc, #0x26c]
004896e4: ldr ip, [r4, #0x124]
004896e8: add r6, sp, #0x18
004896ec: add r2, pc, r2
004896f0: add r1, sp, #0x1c
004896f4: mov r3, #0
004896f8: mov r0, r6
004896fc: str ip, [sp, #0x1c]
00489700: bl #0x514dd8 ; _ZNK11TiXmlHandle5ChildEPKci
00489704: mov r0, r6
00489708: bl #0x483a48 ; _ZNK11TiXmlHandle9ToElementEv
0048970c: subs r7, r0, #0
00489710: beq #0x4898bc
00489714: ldr r0, [r4, #0x18c]
00489718: cmp r0, #0
0048971c: beq #0x48972c
00489720: add r0, r0, #4
00489724: mov r1, r7
00489728: bl #0x513a00 ; _ZN11PropertyMap20LoadOverridesFromXMLEP12TiXmlElement
0048972c: ldr r8, [pc, #0x224]
00489730: mov r0, r7
00489734: add sl, r4, #0x12c
00489738: add r8, pc, r8
0048973c: mov r1, r8
00489740: bl #0x514c70 ; _ZNK12TiXmlElement9AttributeEPKc
00489744: cmp r0, #0
00489748: beq #0x489924
0048974c: mov r1, r8
00489750: mov r0, r7
00489754: bl #0x514c70 ; _ZNK12TiXmlElement9AttributeEPKc
00489758: mov sb, r0
0048975c: bl #0x30de54 ; 
00489760: add r2, sb, r0
00489764: ldr r8, [pc, #0x1f0]
00489768: mov r1, sb
0048976c: mov r0, sl
00489770: add r8, pc, r8
00489774: bl #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
00489778: mov r0, r7
0048977c: mov r1, r8
00489780: bl #0x514c70 ; _ZNK12TiXmlElement9AttributeEPKc
00489784: cmp r0, #0
00489788: add sl, r4, #0x144
0048978c: beq #0x489934
00489790: mov r1, r8
00489794: mov r0, r7
00489798: bl #0x514c70 ; _ZNK12TiXmlElement9AttributeEPKc
0048979c: mov r7, r0
004897a0: bl #0x30de54 ; 
004897a4: add r2, r7, r0
004897a8: mov r1, r7
004897ac: mov r0, sl
004897b0: bl #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
004897b4: mov r0, r4
004897b8: bl #0x4892ec ; _ZN3rnd15RandomGenerator10LoadBlocksEv
004897bc: ldr r1, [sp, #0x18]
004897c0: mov r8, r0
004897c4: mov r0, r4
004897c8: bl #0x4865f0 ; _ZN3rnd15RandomGenerator13LoadListRulesEP9TiXmlNode
004897cc: ldr r1, [sp, #0x18]
004897d0: and r8, r0, r8
004897d4: mov r0, r4
004897d8: bl #0x485cdc ; _ZN3rnd15RandomGenerator13LoadRoomPoolsEP9TiXmlNode
004897dc: ldr r2, [pc, #0x17c]
004897e0: cmp r8, #0
004897e4: moveq r8, #0
004897e8: andne r8, r0, #1
004897ec: mov r3, #0
004897f0: add r2, pc, r2
004897f4: mov r1, r6
004897f8: add r0, sp, #0x14
004897fc: bl #0x514dd8 ; _ZNK11TiXmlHandle5ChildEPKci
00489800: add r0, r4, #0x6c
00489804: ldr r1, [sp, #0x14]
00489808: bl #0x4913cc ; _ZN3rnd8RootRule11LoadFromXmlEP9TiXmlNode
0048980c: ldr r3, [r4, #0x118]
00489810: ldr r2, [r4, #0x11c]
00489814: and r8, r8, r0
00489818: rsb r2, r3, r2
0048981c: lsrs r2, r2, #2
00489820: beq #0x489898
00489824: ldr r1, [pc, #0x138]
00489828: ldr sb, [pc, #0x138]
0048982c: ldr fp, [pc, #0x138]
00489830: ldr r7, [pc, #0x138]
00489834: ldr sl, [pc, #0x138]
00489838: mov r2, #0
0048983c: add r1, pc, r1
00489840: add sb, pc, sb
00489844: add fp, pc, fp
00489848: str r1, [sp, #0xc]
0048984c: mov r6, r2
00489850: ldr r0, [r3, r2, lsl #2]
00489854: bl #0x490904 ; _ZN3rnd8RoomPool18ComputeSizeOfRulesEv
00489858: cmp r0, #0
0048985c: bne #0x48987c
00489860: ldr r3, [r5, r7]
00489864: ldr r3, [r3]
00489868: cmp r3, #2
0048986c: streq r0, [r0]
00489870: beq #0x48987c
00489874: cmp r3, #1
00489878: beq #0x4898c4
0048987c: ldr r3, [r4, #0x118]
00489880: ldr r1, [r4, #0x11c]
00489884: add r6, r6, #1
00489888: mov r2, r6
0048988c: rsb r1, r3, r1
00489890: cmp r6, r1, asr #2
00489894: blo #0x489850
00489898: ldr r2, [sp, #8]
0048989c: mov r0, r8
004898a0: ldr r3, [r5, r2]
004898a4: ldr r2, [sp, #0x5c]
004898a8: ldr r3, [r3]
004898ac: cmp r2, r3
004898b0: bne #0x489944
004898b4: add sp, sp, #0x64
004898b8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004898bc: mov r8, #0
004898c0: b #0x489898
004898c4: ldr r0, [r5, sl]
004898c8: mov ip, #0xa6
004898cc: mov r1, sb
004898d0: mov r2, fp
004898d4: ldr r3, [sp, #0xc]
004898d8: add r0, r0, #0xa8
004898dc: str ip, [sp]
004898e0: bl #0x30e004 ; 
004898e4: b #0x48987c
004898e8: ldr r6, [pc, #0x88]
004898ec: ldr r1, [pc, #0x88]
004898f0: mov r2, r8
004898f4: add r6, pc, r6
004898f8: add r6, r6, #0xc
004898fc: add r1, pc, r1
00489900: mov r0, r6
00489904: bl #0x30eae4 ; 
00489908: mov r2, #0
0048990c: mov r1, r6
00489910: b #0x489640
00489914: bl #0x310440 ; _Z10CustomFreePv
00489918: b #0x489638
0048991c: bl #0x310440 ; _Z10CustomFreePv
00489920: b #0x4895d8
00489924: ldr r2, [pc, #0x54]
00489928: add r2, pc, r2
0048992c: mov sb, r2
00489930: b #0x489764
00489934: ldr r2, [pc, #0x48]
00489938: add r2, pc, r2
0048993c: mov r7, r2
00489940: b #0x4897a8
00489944: bl #0x30e310 ; 
00489948: subseq fp, r0, r8, ror r5
0048994c: andeq r4, r0, ip, lsr #1
00489950: strdeq r3, r4, [r0], -r4
00489954: subeq fp, r4, ip, lsr #11
00489958: subeq r2, r8, r8, asr sb
0048995c: subeq fp, r4, r0, lsr r5
00489960: strheq fp, [r4], #-0x48
00489964: subeq fp, r4, ip, ror r4
00489968: umaaleq r4, r3, r8, fp
0048996c: subeq r4, r3, r4, lsr #26
00489970: andeq r3, r0, r0, asr #19
00489974: andeq r1, r0, r0, asr #19
00489978: subseq ip, r1, r4, lsl #18
0048997c: subeq fp, r4, ip, lsl #7
00489980: subeq r1, r4, r0, ror #29
00489984: ldrdeq r1, r2, [r4], #-0xe0
