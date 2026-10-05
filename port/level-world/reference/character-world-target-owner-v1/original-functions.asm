
# _ZNK13ObjectManager16GetObjectsByTypeEPKcRSt4listIP9CharacterSaIS4_EE
003426b0: push     {r4, r5, r6, r7, r8, sl, lr}
003426b4: sub      sp, sp, #0x14
003426b8: add      r5, sp, #4
003426bc: mov      r6, r0
003426c0: mov      r0, r5
003426c4: mov      r4, r1
003426c8: mov      r7, r2
003426cc: add      r8, r6, #0xc
003426d0: bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
003426d4: ldr      r6, [r6, #0x14]
003426d8: cmp      r8, r6
003426dc: beq      #0x342764
003426e0: ldr      r0, [r6, #0x2c]
003426e4: cmp      r0, #0
003426e8: beq      #0x342738
003426ec: add      r0, r0, #4
003426f0: bl       #0x510b4c ; _ZN11PropertyMap16GetThisClassNameEv
003426f4: mov      r1, r4
003426f8: bl       #0x30e31c ; 
003426fc: cmp      r0, #0
00342700: bne      #0x342738
00342704: ldr      r3, [r6, #0x10]
00342708: mov      r0, r5
0034270c: str      r3, [sp, #4]
00342710: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00342714: mov      sl, r0
00342718: mov      r0, r7
0034271c: bl       #0x342690 ; 
00342720: str      sl, [r0, #8]
00342724: ldr      r3, [r7, #4]
00342728: str      r7, [r0]
0034272c: str      r3, [r0, #4]
00342730: str      r0, [r3]
00342734: str      r0, [r7, #4]
00342738: ldr      r2, [r6, #0xc]
0034273c: cmp      r2, #0
00342740: bne      #0x34274c
00342744: b        #0x34276c
00342748: mov      r2, r3
0034274c: ldr      r3, [r2, #8]
00342750: cmp      r3, #0
00342754: bne      #0x342748
00342758: mov      r6, r2
0034275c: cmp      r8, r6
00342760: bne      #0x3426e0
00342764: add      sp, sp, #0x14
00342768: pop      {r4, r5, r6, r7, r8, sl, pc}
0034276c: ldr      r3, [r6, #4]
00342770: ldr      r1, [r3, #0xc]
00342774: cmp      r6, r1
00342778: bne      #0x342794
0034277c: mov      r6, r3
00342780: ldr      r3, [r3, #4]
00342784: ldr      r2, [r3, #0xc]
00342788: cmp      r2, r6
0034278c: beq      #0x34277c
00342790: ldr      r2, [r6, #0xc]
00342794: cmp      r3, r2
00342798: movne    r6, r3
0034279c: b        #0x3426d8

# _ZNK6CharAI11AI_IsFriendEPK10GameObject
003d511c: push     {r4, r5, r6, r7, lr}
003d5120: ldr      r4, [pc, #0x280]
003d5124: cmp      r1, #0
003d5128: sub      sp, sp, #0x1c
003d512c: mov      r5, r0
003d5130: add      r4, pc, r4
003d5134: beq      #0x3d5330
003d5138: add      r6, sp, #0xc
003d513c: mov      r0, r6
003d5140: bl       #0x33dd70 ; _ZNK10ObjectBase9GetHandleEv
003d5144: mov      r0, r6
003d5148: mov      r1, #0
003d514c: bl       #0x33ff8c ; _ZNK12ObjectHandle9GetObjectEb
003d5150: subs     r6, r0, #0
003d5154: bne      #0x3d5164
003d5158: mov      r0, #0
003d515c: add      sp, sp, #0x1c
003d5160: pop      {r4, r5, r6, r7, pc}
003d5164: ldr      r7, [r6, #0xf4]
003d5168: cmp      r7, #0
003d516c: bne      #0x3d5158
003d5170: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d5174: cmp      r0, #0
003d5178: blt      #0x3d52dc
003d517c: mov      r0, r6
003d5180: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d5184: ldr      r7, [pc, #0x220]
003d5188: ldr      r3, [r4, r7]
003d518c: ldr      r3, [r3]
003d5190: cmp      r0, r3
003d5194: blt      #0x3d51bc
003d5198: ldr      r3, [pc, #0x210]
003d519c: ldr      r3, [r4, r3]
003d51a0: ldr      r3, [r3]
003d51a4: cmp      r3, #2
003d51a8: moveq    r3, #0
003d51ac: streq    r3, [r3]
003d51b0: beq      #0x3d51bc
003d51b4: cmp      r3, #1
003d51b8: beq      #0x3d5374
003d51bc: ldr      r0, [r5, #4]
003d51c0: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d51c4: cmp      r0, #0
003d51c8: blt      #0x3d5284
003d51cc: ldr      r0, [r5, #4]
003d51d0: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d51d4: ldr      r3, [r4, r7]
003d51d8: ldr      r3, [r3]
003d51dc: cmp      r0, r3
003d51e0: blt      #0x3d5208
003d51e4: ldr      r3, [pc, #0x1c4]
003d51e8: ldr      r3, [r4, r3]
003d51ec: ldr      r3, [r3]
003d51f0: cmp      r3, #2
003d51f4: moveq    r3, #0
003d51f8: streq    r3, [r3]
003d51fc: beq      #0x3d5208
003d5200: cmp      r3, #1
003d5204: beq      #0x3d5340
003d5208: ldr      r3, [pc, #0x1a4]
003d520c: ldr      r0, [r5, #4]
003d5210: ldr      r3, [r4, r3]
003d5214: ldr      r4, [r3]
003d5218: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d521c: mov      r3, #0xc
003d5220: mla      r4, r3, r0, r4
003d5224: mov      r0, r6
003d5228: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d522c: ldr      ip, [r4, #4]
003d5230: cmp      ip, #0
003d5234: beq      #0x3d5158
003d5238: ldr      r2, [r4, #8]
003d523c: ldr      r3, [r2, #4]
003d5240: cmp      r0, r3
003d5244: movne    r3, #0
003d5248: bne      #0x3d525c
003d524c: b        #0x3d5270
003d5250: ldr      r1, [r2, #4]
003d5254: cmp      r0, r1
003d5258: beq      #0x3d5270
003d525c: add      r3, r3, #1
003d5260: cmp      r3, ip
003d5264: add      r2, r2, #0xc
003d5268: bne      #0x3d5250
003d526c: b        #0x3d5158
003d5270: ldr      r0, [r2, #8]
003d5274: cmp      r0, #0
003d5278: movle    r0, #0
003d527c: movgt    r0, #1
003d5280: b        #0x3d515c
003d5284: ldr      r3, [pc, #0x124]
003d5288: ldr      r3, [r4, r3]
003d528c: ldr      r3, [r3]
003d5290: cmp      r3, #2
003d5294: moveq    r3, #0
003d5298: streq    r3, [r3]
003d529c: beq      #0x3d51cc
003d52a0: cmp      r3, #1
003d52a4: bne      #0x3d51cc
003d52a8: ldr      r0, [pc, #0x108]
003d52ac: ldr      r1, [pc, #0x108]
003d52b0: ldr      r2, [pc, #0x108]
003d52b4: ldr      r0, [r4, r0]
003d52b8: ldr      r3, [pc, #0x104]
003d52bc: mov      ip, #0xc6
003d52c0: add      r1, pc, r1
003d52c4: add      r2, pc, r2
003d52c8: add      r3, pc, r3
003d52cc: add      r0, r0, #0xa8
003d52d0: str      ip, [sp]
003d52d4: bl       #0x30e004 ; 
003d52d8: b        #0x3d51cc
003d52dc: ldr      r3, [pc, #0xcc]
003d52e0: ldr      r3, [r4, r3]
003d52e4: ldr      r3, [r3]
003d52e8: cmp      r3, #2
003d52ec: streq    r7, [r7]
003d52f0: beq      #0x3d517c
003d52f4: cmp      r3, #1
003d52f8: bne      #0x3d517c
003d52fc: ldr      r0, [pc, #0xb4]
003d5300: ldr      r1, [pc, #0xc0]
003d5304: ldr      r2, [pc, #0xc0]
003d5308: ldr      r0, [r4, r0]
003d530c: ldr      r3, [pc, #0xbc]
003d5310: mov      ip, #0xc4
003d5314: add      r1, pc, r1
003d5318: add      r2, pc, r2
003d531c: add      r3, pc, r3
003d5320: add      r0, r0, #0xa8
003d5324: str      ip, [sp]
003d5328: bl       #0x30e004 ; 
003d532c: b        #0x3d517c
003d5330: ldr      r1, [r0, #0x40]
003d5334: cmp      r1, #0
003d5338: beq      #0x3d5158
003d533c: b        #0x3d5138
003d5340: ldr      r0, [pc, #0x70]
003d5344: ldr      r1, [pc, #0x88]
003d5348: ldr      r2, [pc, #0x88]
003d534c: ldr      r0, [r4, r0]
003d5350: ldr      r3, [pc, #0x84]
003d5354: mov      ip, #0xc7
003d5358: add      r1, pc, r1
003d535c: add      r2, pc, r2
003d5360: add      r3, pc, r3
003d5364: add      r0, r0, #0xa8
003d5368: str      ip, [sp]
003d536c: bl       #0x30e004 ; 
003d5370: b        #0x3d5208
003d5374: ldr      r0, [pc, #0x3c]
003d5378: ldr      r1, [pc, #0x60]
003d537c: ldr      r2, [pc, #0x60]
003d5380: ldr      r0, [r4, r0]
003d5384: ldr      r3, [pc, #0x5c]
003d5388: mov      ip, #0xc5
003d538c: add      r1, pc, r1
003d5390: add      r2, pc, r2
003d5394: add      r3, pc, r3
003d5398: add      r0, r0, #0xa8
003d539c: str      ip, [sp]
003d53a0: bl       #0x30e004 ; 
003d53a4: b        #0x3d51bc
003d53a8: subseq   pc, fp, r0, ror #18
003d53ac: andeq    r2, r0, r4, asr #4
003d53b0: andeq    r3, r0, r0, asr #19
003d53b4: andeq    r4, r0, ip, lsr #12
003d53b8: andeq    r1, r0, r0, asr #19
003d53bc: subeq    sb, lr, r8, lsl r1
003d53c0: strheq   r0, [pc], #-0x34
003d53c4: strdeq   r0, r1, [pc], #-0x28
003d53c8: subeq    sb, lr, r4, asr #1
003d53cc: subeq    r0, pc, r0, lsl #6
003d53d0: subeq    r0, pc, r4, lsr #5
003d53d4: subeq    sb, lr, r0, lsl #1
003d53d8: subeq    r0, pc, ip, lsr r3
003d53dc: subeq    r0, pc, r0, ror #4
003d53e0: subeq    sb, lr, ip, asr #32
003d53e4: subeq    r0, pc, r8, lsr #5
003d53e8: subeq    r0, pc, ip, lsr #4

# _ZN17v2MixedController11Ctrl_LookAtERK7Point3DIfE
00408c50: b        #0x405260

# _ZN13ObjectManager26sProcessAcknowledgedPacketEii
00346284: ldr      r3, [pc, #0x24]
00346288: ldr      r2, [pc, #0x24]
0034628c: mov      ip, r0
00346290: add      r3, pc, r3
00346294: ldr      r0, [r3, r2]
00346298: mov      r2, r1
0034629c: ldr      r0, [r0, #0x38]
003462a0: cmp      r0, #0
003462a4: bxeq     lr
003462a8: mov      r1, ip
003462ac: b        #0x346178
003462b0: rsbeq    lr, r4, r0, lsl #16
003462b4: strdeq   r3, r4, [r0], -r4

# _ZN12ObjectHandleC2EP10ObjectBase
0033f578: push     {r4, r5, lr}
0033f57c: mov      r3, #0
0033f580: mvn      r2, #0
0033f584: cmp      r1, #0
0033f588: sub      sp, sp, #0x14
0033f58c: mov      r4, r0
0033f590: str      r3, [r0, #4]
0033f594: str      r2, [r0, #8]
0033f598: str      r3, [r0]
0033f59c: beq      #0x33f5c0
0033f5a0: mov      r0, sp
0033f5a4: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0033f5a8: ldm      sp, {r0, r1, r2}
0033f5ac: mov      r3, r4
0033f5b0: str      r0, [r3], #4
0033f5b4: mov      r5, sp
0033f5b8: str      r1, [r4, #4]
0033f5bc: str      r2, [r3, #4]
0033f5c0: mov      r0, r4
0033f5c4: add      sp, sp, #0x14
0033f5c8: pop      {r4, r5, pc}

# _ZN13ObjectManager19InitModulesFogColorERKSt6vectorI7Point3DIfESaIS2_EEi
00347100: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00347104: ldr      ip, [r1]
00347108: ldr      r3, [r1, #4]
0034710c: mov      r6, r0
00347110: ldr      r5, [pc, #0x23c]
00347114: rsb      r3, ip, r3
00347118: asr      r3, r3, #2
0034711c: add      r5, pc, r5
00347120: add      r0, r3, r3, lsl #2
00347124: sub      sp, sp, #0x34
00347128: add      r0, r0, r0, lsl #4
0034712c: add      r0, r0, r0, lsl #8
00347130: add      r0, r0, r0, lsl #16
00347134: add      r3, r3, r0, lsl #1
00347138: cmp      r3, #0
0034713c: bne      #0x347148
00347140: add      sp, sp, #0x34
00347144: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00347148: add      r3, sp, #0x20
0034714c: str      r3, [sp, #0xc]
00347150: mov      r0, r3
00347154: mov      r3, #0x64
00347158: mul      r3, r3, r2
0034715c: str      r3, [sp, #4]
00347160: bl       #0x3424b8 ; _ZNSt6vectorI7Point3DIfESaIS1_EEC1ERKS3_
00347164: ldr      r1, [sp, #0x24]
00347168: ldr      r0, [sp, #0x20]
0034716c: bl       #0x3411c4 ; _ZSt14random_shuffleIP7Point3DIfEEvT_S3_
00347170: mov      r3, r6
00347174: ldr      r4, [r3, #0x68]!
00347178: mov      r1, #0
0034717c: str      r1, [sp, #0x14]
00347180: cmp      r4, r3
00347184: str      r1, [sp, #0x18]
00347188: str      r1, [sp, #0x1c]
0034718c: beq      #0x3471a0
00347190: ldr      r4, [r4]
00347194: add      r1, r1, #1
00347198: cmp      r3, r4
0034719c: bne      #0x347190
003471a0: mov      r7, #0
003471a4: add      r0, sp, #0x14
003471a8: add      r2, sp, #0x2c
003471ac: str      r7, [sp, #0x2c]
003471b0: bl       #0x346430 ; _ZNSt6vectorIP6ModuleSaIS1_EE6resizeEjRKS1_
003471b4: ldr      r3, [r6, #0x68]
003471b8: ldr      r1, [sp, #0x14]
003471bc: b        #0x3471d0
003471c0: ldr      r2, [r3, #8]
003471c4: str      r2, [r1, r7]
003471c8: ldr      r3, [r3]
003471cc: add      r7, r7, #4
003471d0: cmp      r4, r3
003471d4: bne      #0x3471c0
003471d8: ldr      r3, [r6, #0x68]
003471dc: ldr      r0, [sp, #0x14]
003471e0: ldr      r1, [sp, #0x18]
003471e4: ldr      r4, [r3, #8]
003471e8: mov      r2, r4
003471ec: bl       #0x342314 ; _ZSt4sortIPP6Module20SortModuleByDistanceEvT_S4_T0_
003471f0: ldr      r2, [sp, #0x24]
003471f4: ldr      r3, [sp, #0x20]
003471f8: ldr      fp, [sp, #0x18]
003471fc: ldr      r7, [sp, #0x14]
00347200: rsb      r3, r3, r2
00347204: asr      r3, r3, #2
00347208: cmp      fp, r7
0034720c: add      r2, r3, r3, lsl #2
00347210: add      r2, r2, r2, lsl #4
00347214: add      r2, r2, r2, lsl #8
00347218: add      r2, r2, r2, lsl #16
0034721c: add      r2, r3, r2, lsl #1
00347220: str      r2, [sp, #8]
00347224: beq      #0x347318
00347228: ldr      r3, [pc, #0x128]
0034722c: str      r3, [sp]
00347230: ldr      r6, [r7]
00347234: ldr      r0, [r4, #0x160]
00347238: add      r7, r7, #4
0034723c: ldr      r1, [r6, #0x160]
00347240: bl       #0x30e3ac ; 
00347244: ldr      r1, [r6, #0x164]
00347248: mov      sl, r0
0034724c: ldr      r0, [r4, #0x164]
00347250: bl       #0x30e3ac ; 
00347254: ldr      r1, [r6, #0x168]
00347258: mov      sb, r0
0034725c: ldr      r0, [r4, #0x168]
00347260: bl       #0x30e3ac ; 
00347264: mov      r1, sl
00347268: mov      r8, r0
0034726c: mov      r0, sl
00347270: bl       #0x30ed6c ; 
00347274: mov      r1, sb
00347278: mov      sl, r0
0034727c: mov      r0, sb
00347280: bl       #0x30ed6c ; 
00347284: mov      r1, r0
00347288: mov      r0, sl
0034728c: bl       #0x30eba4 ; 
00347290: mov      r1, r8
00347294: mov      sl, r0
00347298: mov      r0, r8
0034729c: bl       #0x30ed6c ; 
003472a0: mov      r1, r0
003472a4: mov      r0, sl
003472a8: bl       #0x30eba4 ; 
003472ac: bl       #0x30e124 ; 
003472b0: ldr      r3, [sp]
003472b4: mov      sl, r0
003472b8: add      r0, r6, #0x3f0
003472bc: ldr      r1, [r5, r3]
003472c0: ldr      r8, [sp, #0x20]
003472c4: bl       #0x312b6c ; _ZNK7Point3DIfEeqERKS0_
003472c8: cmp      r0, #0
003472cc: mov      r0, sl
003472d0: beq      #0x34730c
003472d4: bl       #0x30e4cc ; 
003472d8: ldr      r1, [sp, #4]
003472dc: bl       #0x30e2a4 ; 
003472e0: ldr      r1, [sp, #8]
003472e4: bl       #0x30e904 ; 
003472e8: mov      r3, #0xc
003472ec: mul      r1, r3, r1
003472f0: ldr      r3, [r8, r1]
003472f4: add      r8, r8, r1
003472f8: str      r3, [r6, #0x3f0]
003472fc: ldr      r3, [r8, #4]
00347300: str      r3, [r6, #0x3f4]
00347304: ldr      r3, [r8, #8]
00347308: str      r3, [r6, #0x3f8]
0034730c: cmp      r7, fp
00347310: bne      #0x347230
00347314: ldr      fp, [sp, #0x14]
00347318: cmp      fp, #0
0034731c: beq      #0x34733c
00347320: ldr      r1, [sp, #0x1c]
00347324: rsb      r1, fp, r1
00347328: bic      r1, r1, #3
0034732c: cmp      r1, #0x80
00347330: bhi      #0x347348
00347334: mov      r0, fp
00347338: bl       #0x708f00 ; 
0034733c: ldr      r0, [sp, #0xc]
00347340: bl       #0x34611c ; _ZNSt6vectorI7Point3DIfESaIS1_EED1Ev
00347344: b        #0x347140
00347348: mov      r0, fp
0034734c: bl       #0x310440 ; _Z10CustomFreePv
00347350: b        #0x34733c
00347354: rsbeq    sp, r4, r4, ror sb
00347358: muleq    r0, r8, r4

# _Z14GetNewInstanceI8RoomZoneEP10ObjectBasev
00340f74: push     {r4, lr}
00340f78: mov      r1, #0
00340f7c: mov      r0, #0x39c
00340f80: bl       #0x310570 ; _Znwj15MemoryHintState
00340f84: mov      r1, #0xb
00340f88: mov      r4, r0
00340f8c: bl       #0x396558 ; _ZN8RoomZoneC1EN10ObjectBase6GO_IDSE
00340f90: mov      r0, r4
00340f94: pop      {r4, pc}

# _ZN13ObjectManager26GetObjectHandleByNetworkIdEi
003407a0: push     {r4, lr}
003407a4: ldr      r3, [r1, #0x100]!
003407a8: mov      r4, r0
003407ac: cmp      r1, r3
003407b0: beq      #0x3407d8
003407b4: ldr      r0, [r3, #8]
003407b8: cmp      r0, #0
003407bc: beq      #0x3407cc
003407c0: ldr      ip, [r0, #0x108]
003407c4: cmp      r2, ip
003407c8: beq      #0x3407ec
003407cc: ldr      r3, [r3]
003407d0: cmp      r1, r3
003407d4: bne      #0x3407b4
003407d8: mov      r0, r4
003407dc: mov      r1, #0
003407e0: bl       #0x33f524 ; _ZN12ObjectHandleC1EP10ObjectBase
003407e4: mov      r0, r4
003407e8: pop      {r4, pc}
003407ec: mov      r1, r0
003407f0: mov      r0, r4
003407f4: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
003407f8: mov      r0, r4
003407fc: pop      {r4, pc}

# _ZN13ObjectManager14DelRoomObjectsEPSt4listIP10GameObjectSaIS2_EE
00345fbc: push     {r4, r5, r6, lr}
00345fc0: ldr      r3, [pc, #0xb0]
00345fc4: subs     r4, r1, #0
00345fc8: sub      sp, sp, #8
00345fcc: mov      r5, r0
00345fd0: add      r3, pc, r3
00345fd4: beq      #0x346024
00345fd8: ldr      r0, [r5, #0x80]!
00345fdc: cmp      r5, r0
00345fe0: beq      #0x346000
00345fe4: ldr      r3, [r0, #8]
00345fe8: ldr      r6, [r0]
00345fec: cmp      r4, r3
00345ff0: beq      #0x346008
00345ff4: mov      r0, r6
00345ff8: cmp      r5, r0
00345ffc: bne      #0x345fe4
00346000: add      sp, sp, #8
00346004: pop      {r4, r5, r6, pc}
00346008: ldr      r3, [r0, #4]
0034600c: mov      r1, #0xc
00346010: str      r6, [r3]
00346014: str      r3, [r6, #4]
00346018: bl       #0x708f00 ; 
0034601c: mov      r0, r6
00346020: b        #0x345ff8
00346024: ldr      r2, [pc, #0x50]
00346028: ldr      r2, [r3, r2]
0034602c: ldr      r2, [r2]
00346030: cmp      r2, #2
00346034: streq    r4, [r4]
00346038: beq      #0x345fd8
0034603c: cmp      r2, #1
00346040: bne      #0x345fd8
00346044: ldr      r0, [pc, #0x34]
00346048: ldr      r1, [pc, #0x34]
0034604c: ldr      r2, [pc, #0x34]
00346050: ldr      r0, [r3, r0]
00346054: ldr      r3, [pc, #0x30]
00346058: movw     ip, #0x912
0034605c: add      r1, pc, r1
00346060: add      r2, pc, r2
00346064: add      r3, pc, r3
00346068: add      r0, r0, #0xa8
0034606c: str      ip, [sp]
00346070: bl       #0x30e004 ; 
00346074: b        #0x345fd8
00346078: rsbeq    lr, r4, r0, asr #21
0034607c: andeq    r3, r0, r0, asr #19
00346080: andeq    r1, r0, r0, asr #19
00346084: subseq   r8, r7, ip, ror r3
00346088: subseq   sp, r7, r8, lsr #31
0034608c: subseq   sl, r7, r4, lsr r2

# _ZN9Objective21RemoveObjectiveMarkerEv
00479f84: bx       lr

# _ZN19Objective_TalkToNPC21RemoveObjectiveMarkerEv
0047da6c: push     {r4, r5, r6, r7, r8, lr}
0047da70: ldrb     r2, [r0, #8]
0047da74: ldr      r3, [pc, #0xb0]
0047da78: mov      r4, r0
0047da7c: cmp      r2, #0
0047da80: ldr      r1, [r0, #0xc]
0047da84: add      r3, pc, r3
0047da88: beq      #0x47da98
0047da8c: ldr      r2, [r1, #4]
0047da90: cmp      r2, #5
0047da94: beq      #0x47da9c
0047da98: pop      {r4, r5, r6, r7, r8, pc}
0047da9c: ldr      r2, [pc, #0x8c]
0047daa0: ldr      r8, [r0, #0x24]
0047daa4: ldr      r3, [r3, r2]
0047daa8: ldr      r7, [r3, #0x38]
0047daac: ldr      r5, [r7, #0x60]!
0047dab0: cmp      r7, r5
0047dab4: beq      #0x47dad8
0047dab8: ldr      r6, [r5, #8]
0047dabc: mov      r0, r6
0047dac0: bl       #0x3b3d38 ; _ZN9Character18SafeGetCharPropsIdEv
0047dac4: cmp      r8, r0
0047dac8: beq      #0x47dadc
0047dacc: ldr      r5, [r5]
0047dad0: cmp      r7, r5
0047dad4: bne      #0x47dab8
0047dad8: pop      {r4, r5, r6, r7, r8, pc}
0047dadc: cmp      r6, #0
0047dae0: beq      #0x47da98
0047dae4: mov      r5, #0
0047dae8: strb     r5, [r6, #0x2fb]
0047daec: strb     r5, [r6, #0x2fa]
0047daf0: ldr      r0, [r4, #0x28]
0047daf4: cmp      r0, r5
0047daf8: beq      #0x47da98
0047dafc: str      r5, [r0, #0x28]
0047db00: mov      r1, #1
0047db04: bl       #0x492aa0 ; _ZN10AnimatedFX11SyncIrrDataEb
0047db08: ldr      r0, [r4, #0x28]
0047db0c: mov      r1, #0
0047db10: ldr      r3, [r0, #0x2c]
0047db14: cmp      r3, r5
0047db18: ldrne    r3, [r3, #8]
0047db1c: strne    r5, [r3, #0x204]
0047db20: ldrne    r0, [r4, #0x28]
0047db24: pop      {r4, r5, r6, r7, r8, lr}
0047db28: b        #0x492ef0
0047db2c: subseq   r7, r1, ip
0047db30: strdeq   r3, r4, [r0], -r4

# _ZN8RoomZone16AddInitialObjectEP10GameObject
00396a90: push     {r4, r5, r6, r7, r8, sl, lr}
00396a94: ldr      r4, [pc, #0x194]
00396a98: ldr      r7, [pc, #0x194]
00396a9c: sub      sp, sp, #0x44
00396aa0: add      r4, pc, r4
00396aa4: ldr      r3, [r4, r7]
00396aa8: subs     r5, r1, #0
00396aac: mov      r6, r0
00396ab0: ldr      r3, [r3]
00396ab4: str      r3, [sp, #0x3c]
00396ab8: beq      #0x396ad4
00396abc: ldr      r3, [r5]
00396ac0: mov      r0, r5
00396ac4: mov      lr, pc
00396ac8: ldr      pc, [r3, #0xc4]
00396acc: cmp      r0, #0
00396ad0: bne      #0x396af4
00396ad4: mov      r0, #0
00396ad8: ldr      r3, [r4, r7]
00396adc: ldr      r2, [sp, #0x3c]
00396ae0: ldr      r3, [r3]
00396ae4: cmp      r2, r3
00396ae8: bne      #0x396c2c
00396aec: add      sp, sp, #0x44
00396af0: pop      {r4, r5, r6, r7, r8, sl, pc}
00396af4: ldr      r8, [r5, #0x160]
00396af8: ldr      r0, [r6, #0x12c]
00396afc: mov      r1, r8
00396b00: bl       #0x30e9ac ; 
00396b04: cmp      r0, #0
00396b08: beq      #0x396ad4
00396b0c: mov      r0, r8
00396b10: ldr      r1, [r6, #0x138]
00396b14: bl       #0x30e9ac ; 
00396b18: cmp      r0, #0
00396b1c: beq      #0x396ad4
00396b20: ldr      r8, [r5, #0x164]
00396b24: ldr      r0, [r6, #0x130]
00396b28: mov      r1, r8
00396b2c: bl       #0x30e9ac ; 
00396b30: cmp      r0, #0
00396b34: beq      #0x396ad4
00396b38: mov      r0, r8
00396b3c: ldr      r1, [r6, #0x13c]
00396b40: bl       #0x30e9ac ; 
00396b44: cmp      r0, #0
00396b48: beq      #0x396ad4
00396b4c: ldrb     r3, [r5, #0x2ef]
00396b50: cmp      r3, #0
00396b54: beq      #0x396bec
00396b58: ldr      r3, [pc, #0xd8]
00396b5c: add      r8, sp, #0x24
00396b60: ldr      sl, [r4, r3]
00396b64: mov      r0, sl
00396b68: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00396b6c: ldr      r1, [pc, #0xc8]
00396b70: add      r2, sp, #8
00396b74: mov      r0, r8
00396b78: add      r1, pc, r1
00396b7c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00396b80: mov      r1, r8
00396b84: mov      r0, sl
00396b88: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00396b8c: mov      r0, r8
00396b90: bl       #0x318254 ; _ZNSsD1Ev
00396b94: ldr      r0, [r5, #0x2f4]
00396b98: cmp      r0, #0
00396b9c: beq      #0x396ba8
00396ba0: mov      r1, r5
00396ba4: bl       #0x3968cc ; _ZN8RoomZone12RemoveObjectEP10GameObject
00396ba8: mov      r8, #1
00396bac: mov      r0, r5
00396bb0: add      sl, r6, #0x394
00396bb4: str      r6, [r5, #0x2f4]
00396bb8: strb     r8, [r5, #0x2ef]
00396bbc: bl       #0x38c710 ; _ZN10GameObject11ZoneEnteredEv
00396bc0: mov      r0, sl
00396bc4: bl       #0x39670c ; 
00396bc8: str      r5, [r0, #8]
00396bcc: ldr      r2, [r6, #0x398]
00396bd0: mov      r3, r0
00396bd4: str      sl, [r0]
00396bd8: str      r2, [r3, #4]
00396bdc: mov      r0, r8
00396be0: str      r3, [r2]
00396be4: str      r3, [r6, #0x398]
00396be8: b        #0x396ad8
00396bec: ldr      r3, [pc, #0x44]
00396bf0: add      r8, sp, #0xc
00396bf4: ldr      sl, [r4, r3]
00396bf8: mov      r0, sl
00396bfc: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00396c00: ldr      r1, [pc, #0x38]
00396c04: add      r2, sp, #4
00396c08: mov      r0, r8
00396c0c: add      r1, pc, r1
00396c10: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00396c14: mov      r0, sl
00396c18: mov      r1, r8
00396c1c: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00396c20: mov      r0, r8
00396c24: bl       #0x318254 ; _ZNSsD1Ev
00396c28: b        #0x396ba8
00396c2c: bl       #0x30e310 ; 
00396c30: ldrsheq  sp, [pc], #-0xf0
00396c34: andeq    r4, r0, ip, lsr #1
00396c38: andeq    r0, r0, r4, lsl #17
00396c3c: subseq   fp, r2, r8, asr lr
00396c40: subseq   fp, r2, r4, asr #27

# _ZN8RoomZoneC1EN10ObjectBase6GO_IDSE
00396558: push     {r4, r5, r6, lr}
0039655c: mov      r2, #0
00396560: mov      r3, #1
00396564: ldr      r5, [pc, #0x50]
00396568: mov      r4, r0
0039656c: bl       #0x397ca0 ; _ZN4ZoneC2EN10ObjectBase6GO_IDSEbb
00396570: ldr      r3, [pc, #0x48]
00396574: add      r5, pc, r5
00396578: mov      r1, #1
0039657c: ldr      r3, [r5, r3]
00396580: add      r2, r4, #0x394
00396584: strb     r1, [r4, #0x389]
00396588: add      r0, r3, #0xf4
0039658c: add      ip, r3, #8
00396590: add      r3, r3, #0xe8
00396594: str      r3, [r4, #4]
00396598: mov      r3, #0
0039659c: str      r0, [r4, #0x24]
003965a0: str      ip, [r4]
003965a4: strb     r3, [r4, #0x390]
003965a8: str      r2, [r4, #0x398]
003965ac: strb     r1, [r4, #0x388]
003965b0: str      r2, [r4, #0x394]
003965b4: mov      r0, r4
003965b8: pop      {r4, r5, r6, pc}
003965bc: subseq   lr, pc, ip, lsl r5
003965c0: andeq    r2, r0, r4, ror r7

# _ZN12v2Controller10Cmd_LookAtEP10GameObject
004052bc: push     {r4, lr}
004052c0: ldrb     r2, [r0, #9]
004052c4: ldr      r3, [pc, #0x44]
004052c8: cmp      r2, #0
004052cc: add      r3, pc, r3
004052d0: bne      #0x4052f8
004052d4: ldr      r2, [pc, #0x38]
004052d8: ldr      r3, [r3, r2]
004052dc: ldrb     r3, [r3]
004052e0: cmp      r3, #0
004052e4: beq      #0x4052ec
004052e8: pop      {r4, pc}
004052ec: ldrb     r3, [r0, #8]
004052f0: cmp      r3, #0
004052f4: bne      #0x4052e8
004052f8: ldr      r3, [r0, #4]
004052fc: mov      r0, r3
00405300: ldr      r3, [r3]
00405304: mov      lr, pc
00405308: ldr      pc, [r3, #0x14]
0040530c: pop      {r4, pc}
00405310: subseq   pc, r8, r4, asr #15
00405314: andeq    r3, r0, r0, asr r6

# _ZN8RoomZoneD0Ev
003968b0: push     {r4, lr}
003968b4: mov      r4, r0
003968b8: bl       #0x396828 ; _ZN8RoomZoneD1Ev
003968bc: mov      r0, r4
003968c0: bl       #0x310440 ; _Z10CustomFreePv
003968c4: mov      r0, r4
003968c8: pop      {r4, pc}

# _ZN6CharAIC1Ev
003ced50: ldr      r3, [pc, #0x14c]
003ced54: ldr      r2, [pc, #0x14c]
003ced58: push     {r4, r5, lr}
003ced5c: add      r3, pc, r3
003ced60: ldr      r2, [r3, r2]
003ced64: mov      r4, r0
003ced68: mov      r1, #0
003ced6c: add      r2, r2, #8
003ced70: str      r2, [r4]
003ced74: ldr      r2, [pc, #0x130]
003ced78: mov      r0, #1
003ced7c: mvn      ip, #0
003ced80: mov      r5, r4
003ced84: strb     r0, [r4, #0x55]
003ced88: str      r1, [r4, #8]
003ced8c: str      r1, [r4, #0xc]
003ced90: strb     r1, [r4, #0x18]
003ced94: str      r1, [r4, #0x1c]
003ced98: str      r1, [r4, #0x20]
003ced9c: strb     r1, [r4, #0x24]
003ceda0: str      r1, [r4, #0x28]
003ceda4: strb     r1, [r4, #0x2c]
003ceda8: str      r1, [r4, #0x30]
003cedac: str      r1, [r4, #0x34]
003cedb0: str      r1, [r4, #0x3c]
003cedb4: str      r1, [r4, #0x40]
003cedb8: str      r1, [r4, #0x44]
003cedbc: strb     r1, [r4, #0x49]
003cedc0: strb     r0, [r4, #0x4a]
003cedc4: strb     r0, [r4, #0x4b]
003cedc8: strb     r1, [r4, #0x4c]
003cedcc: strb     r0, [r4, #0x4d]
003cedd0: str      r1, [r4, #0x50]
003cedd4: strb     r0, [r4, #0x54]
003cedd8: str      r1, [r4, #0x58]
003ceddc: mov      r0, r4
003cede0: str      r1, [r4, #0x60]
003cede4: str      ip, [r4, #0x10]
003cede8: str      ip, [r4, #0x14]
003cedec: str      ip, [r4, #0x38]
003cedf0: strb     r1, [r5, #0x5c]!
003cedf4: str      r5, [r4, #0x68]
003cedf8: str      r5, [r4, #0x64]
003cedfc: str      r1, [r4, #0x6c]
003cee00: str      r1, [r4, #0x80]
003cee04: strb     r1, [r0, #0x7c]!
003cee08: ldr      r5, [r3, r2]
003cee0c: mov      r2, r4
003cee10: str      r0, [r4, #0x88]
003cee14: str      r0, [r4, #0x84]
003cee18: str      r1, [r4, #0x8c]
003cee1c: str      r1, [r4, #0x98]
003cee20: add      r0, r4, #0xac
003cee24: strb     r1, [r2, #0x94]!
003cee28: str      r2, [r4, #0xa0]
003cee2c: str      r0, [r4, #0xb0]
003cee30: str      ip, [r4, #0xcc]
003cee34: strb     r1, [r4, #0xd1]
003cee38: str      r2, [r4, #0x9c]
003cee3c: str      r1, [r4, #0xa4]
003cee40: str      r0, [r4, #0xac]
003cee44: str      r1, [r4, #0xb4]
003cee48: str      r1, [r4, #0xb8]
003cee4c: str      r1, [r4, #0xbc]
003cee50: str      r1, [r4, #0xc0]
003cee54: str      r1, [r4, #0xc4]
003cee58: str      r1, [r4, #0xc8]
003cee5c: strb     r1, [r4, #0xd0]
003cee60: ldr      r1, [r5, #0x18]
003cee64: ldr      r2, [r5, #0x10]
003cee68: sub      sp, sp, #0xc
003cee6c: sub      r3, r1, #4
003cee70: cmp      r2, r3
003cee74: str      r4, [sp, #4]
003cee78: beq      #0x3cee98
003cee7c: str      r4, [r2]
003cee80: ldr      r3, [r5, #0x10]
003cee84: add      r3, r3, #4
003cee88: str      r3, [r5, #0x10]
003cee8c: mov      r0, r4
003cee90: add      sp, sp, #0xc
003cee94: pop      {r4, r5, pc}
003cee98: add      r0, sp, #4
003cee9c: bl       #0x3ce810 ; 
003ceea0: b        #0x3cee8c
003ceea4: subseq   r5, ip, r4, lsr sp
003ceea8: andeq    r4, r0, ip, asr #12
003ceeac: andeq    r4, r0, ip, lsr #19

# _ZN13ObjectManager23GetDynamicFogColorAtPosERK7Point3DIfEf
003419a8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003419ac: mov      r5, r1
003419b0: mov      r1, #0x42000000
003419b4: sub      sp, sp, #0x14
003419b8: mov      r4, r0
003419bc: add      r1, r1, #0xc80000
003419c0: mov      r0, r3
003419c4: mov      r7, r2
003419c8: bl       #0x30ed6c ; 
003419cc: ldr      r2, [pc, #0x1f0]
003419d0: mov      r3, #0
003419d4: mov      fp, r0
003419d8: str      r2, [sp, #8]
003419dc: add      r2, r5, #0x68
003419e0: str      r2, [sp, #4]
003419e4: str      r3, [r4, #8]
003419e8: str      r3, [r4]
003419ec: str      r3, [r4, #4]
003419f0: ldr      r3, [sp, #8]
003419f4: ldr      r2, [pc, #0x1cc]
003419f8: add      r3, pc, r3
003419fc: str      r3, [sp, #8]
00341a00: ldr      r6, [r5, #0x68]
00341a04: ldr      r3, [sp, #4]
00341a08: str      r2, [sp, #0xc]
00341a0c: cmp      r3, r6
00341a10: beq      #0x341ab8
00341a14: ldr      r5, [r6, #8]
00341a18: ldr      r1, [r7]
00341a1c: ldr      r0, [r5, #0x160]
00341a20: bl       #0x30e3ac ; 
00341a24: ldr      r1, [r7, #4]
00341a28: mov      sl, r0
00341a2c: ldr      r0, [r5, #0x164]
00341a30: bl       #0x30e3ac ; 
00341a34: ldr      r1, [r7, #8]
00341a38: mov      sb, r0
00341a3c: ldr      r0, [r5, #0x168]
00341a40: bl       #0x30e3ac ; 
00341a44: mov      r1, sl
00341a48: mov      r8, r0
00341a4c: mov      r0, sl
00341a50: bl       #0x30ed6c ; 
00341a54: mov      r1, sb
00341a58: mov      sl, r0
00341a5c: mov      r0, sb
00341a60: bl       #0x30ed6c ; 
00341a64: mov      r1, r0
00341a68: mov      r0, sl
00341a6c: bl       #0x30eba4 ; 
00341a70: mov      r1, r8
00341a74: mov      sl, r0
00341a78: mov      r0, r8
00341a7c: bl       #0x30ed6c ; 
00341a80: mov      r1, r0
00341a84: mov      r0, sl
00341a88: bl       #0x30eba4 ; 
00341a8c: bl       #0x30e124 ; 
00341a90: mov      r8, r0
00341a94: mov      r1, r8
00341a98: mov      r0, fp
00341a9c: bl       #0x30e4b4 ; 
00341aa0: cmp      r0, #0
00341aa4: bne      #0x341b30
00341aa8: ldr      r6, [r6]
00341aac: ldr      r3, [sp, #4]
00341ab0: cmp      r3, r6
00341ab4: bne      #0x341a14
00341ab8: ldr      r6, [r4]
00341abc: mov      r1, #0x43000000
00341ac0: add      r1, r1, #0x7f0000
00341ac4: mov      r0, r6
00341ac8: bl       #0x30e2f8 ; 
00341acc: ldr      r5, [r4, #4]
00341ad0: cmp      r0, #0
00341ad4: movne    r6, #0x43000000
00341ad8: addne    r6, r6, #0x7f0000
00341adc: mov      r1, #0x43000000
00341ae0: mov      r0, r5
00341ae4: str      r6, [r4]
00341ae8: add      r1, r1, #0x7f0000
00341aec: bl       #0x30e2f8 ; 
00341af0: ldr      r6, [r4, #8]
00341af4: cmp      r0, #0
00341af8: movne    r5, #0x43000000
00341afc: addne    r5, r5, #0x7f0000
00341b00: mov      r1, #0x43000000
00341b04: mov      r0, r6
00341b08: str      r5, [r4, #4]
00341b0c: add      r1, r1, #0x7f0000
00341b10: bl       #0x30e2f8 ; 
00341b14: cmp      r0, #0
00341b18: movne    r6, #0x43000000
00341b1c: addne    r6, r6, #0x7f0000
00341b20: str      r6, [r4, #8]
00341b24: mov      r0, r4
00341b28: add      sp, sp, #0x14
00341b2c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00341b30: ldr      r2, [sp, #8]
00341b34: ldr      r3, [sp, #0xc]
00341b38: add      r0, r5, #0x3f0
00341b3c: ldr      r1, [r2, r3]
00341b40: bl       #0x312b6c ; _ZNK7Point3DIfEeqERKS0_
00341b44: cmp      r0, #0
00341b48: mov      r1, r8
00341b4c: mov      r0, fp
00341b50: bne      #0x341aa8
00341b54: bl       #0x30e3ac ; 
00341b58: mov      r1, fp
00341b5c: bl       #0x30ec94 ; 
00341b60: ldr      r1, [r5, #0x3f4]
00341b64: mov      r8, r0
00341b68: bl       #0x30ed6c ; 
00341b6c: ldr      r1, [r5, #0x3f8]
00341b70: mov      sl, r0
00341b74: mov      r0, r8
00341b78: bl       #0x30ed6c ; 
00341b7c: ldr      r1, [r5, #0x3f0]
00341b80: mov      sb, r0
00341b84: mov      r0, r8
00341b88: bl       #0x30ed6c ; 
00341b8c: mov      r1, r0
00341b90: ldr      r0, [r4]
00341b94: bl       #0x30eba4 ; 
00341b98: mov      r1, sl
00341b9c: str      r0, [r4]
00341ba0: ldr      r0, [r4, #4]
00341ba4: bl       #0x30eba4 ; 
00341ba8: mov      r1, sb
00341bac: str      r0, [r4, #4]
00341bb0: ldr      r0, [r4, #8]
00341bb4: bl       #0x30eba4 ; 
00341bb8: str      r0, [r4, #8]
00341bbc: ldr      r6, [r6]
00341bc0: b        #0x341aac
00341bc4: mlseq    r5, r8, r0, r3
00341bc8: muleq    r0, r8, r4

# _ZN13ObjectManager13IsPacketValidEii
00342f30: push     {r4, r5, r6, r7, lr}
00342f34: ldr      ip, [r0, #0x14c]
00342f38: sub      sp, sp, #0x34
00342f3c: mov      r6, r0
00342f40: cmp      ip, #0
00342f44: mov      r4, r1
00342f48: mov      r7, r2
00342f4c: add      r5, r0, #0x148
00342f50: beq      #0x343030
00342f54: mov      r1, r5
00342f58: mov      r3, ip
00342f5c: b        #0x342f64
00342f60: mov      r3, r2
00342f64: ldr      r2, [r3, #0x10]
00342f68: cmp      r2, r4
00342f6c: ldrlt    r2, [r3, #0xc]
00342f70: ldrge    r2, [r3, #8]
00342f74: movlt    r3, r1
00342f78: mov      r1, r3
00342f7c: cmp      r2, #0
00342f80: bne      #0x342f60
00342f84: cmp      r5, r3
00342f88: beq      #0x3430bc
00342f8c: ldr      r2, [r3, #0x10]
00342f90: cmp      r2, r4
00342f94: bgt      #0x343030
00342f98: cmp      r5, r3
00342f9c: beq      #0x3430bc
00342fa0: cmp      ip, #0
00342fa4: movne    r2, r5
00342fa8: bne      #0x342fb4
00342fac: b        #0x343140
00342fb0: mov      ip, r3
00342fb4: ldr      r3, [ip, #0x10]
00342fb8: cmp      r3, r4
00342fbc: ldrlt    r3, [ip, #0xc]
00342fc0: ldrge    r3, [ip, #8]
00342fc4: movlt    ip, r2
00342fc8: mov      r2, ip
00342fcc: cmp      r3, #0
00342fd0: bne      #0x342fb0
00342fd4: cmp      r5, ip
00342fd8: beq      #0x342fec
00342fdc: ldr      r2, [ip, #0x10]
00342fe0: mov      r3, ip
00342fe4: cmp      r2, r4
00342fe8: ble      #0x343014
00342fec: add      r3, sp, #8
00342ff0: mov      lr, #0
00342ff4: add      r0, sp, #0x20
00342ff8: mov      r1, r5
00342ffc: add      r2, sp, #0x24
00343000: str      lr, [sp, #0xc]
00343004: str      ip, [sp, #0x24]
00343008: str      r4, [sp, #8]
0034300c: bl       #0x342bbc ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
00343010: ldr      r3, [sp, #0x20]
00343014: ldr      r1, [r3, #0x14]
00343018: mov      r0, r7
0034301c: bl       #0x8153a8 ; _ZN14CPacketManager18SequenceMoreRecentEjj
00343020: cmp      r0, #0
00343024: bne      #0x343038
00343028: add      sp, sp, #0x34
0034302c: pop      {r4, r5, r6, r7, pc}
00343030: mov      r3, r5
00343034: b        #0x342f98
00343038: ldr      ip, [r6, #0x14c]
0034303c: cmp      ip, #0
00343040: moveq    ip, r5
00343044: beq      #0x343074
00343048: mov      r2, r5
0034304c: b        #0x343054
00343050: mov      ip, r3
00343054: ldr      r3, [ip, #0x10]
00343058: cmp      r3, r4
0034305c: ldrlt    r3, [ip, #0xc]
00343060: ldrge    r3, [ip, #8]
00343064: movlt    ip, r2
00343068: mov      r2, ip
0034306c: cmp      r3, #0
00343070: bne      #0x343050
00343074: cmp      r5, ip
00343078: beq      #0x34308c
0034307c: ldr      r2, [ip, #0x10]
00343080: mov      r3, ip
00343084: cmp      r2, r4
00343088: ble      #0x3430b0
0034308c: mov      r3, sp
00343090: mov      lr, #0
00343094: mov      r1, r5
00343098: add      r0, sp, #0x18
0034309c: add      r2, sp, #0x1c
003430a0: stm      sp, {r4, lr}
003430a4: str      ip, [sp, #0x1c]
003430a8: bl       #0x342bbc ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
003430ac: ldr      r3, [sp, #0x18]
003430b0: str      r7, [r3, #0x14]
003430b4: mov      r0, #1
003430b8: b        #0x343028
003430bc: cmp      ip, #0
003430c0: moveq    ip, r5
003430c4: beq      #0x3430f4
003430c8: mov      r2, r5
003430cc: b        #0x3430d4
003430d0: mov      ip, r3
003430d4: ldr      r3, [ip, #0x10]
003430d8: cmp      r3, r4
003430dc: ldrlt    r3, [ip, #0xc]
003430e0: ldrge    r3, [ip, #8]
003430e4: movlt    ip, r2
003430e8: mov      r2, ip
003430ec: cmp      r3, #0
003430f0: bne      #0x3430d0
003430f4: cmp      r5, ip
003430f8: beq      #0x34310c
003430fc: ldr      r2, [ip, #0x10]
00343100: mov      r3, ip
00343104: cmp      r2, r4
00343108: ble      #0x3430b0
0034310c: add      r0, sp, #0x28
00343110: add      r3, sp, #0x10
00343114: mov      lr, #0
00343118: mov      r1, r5
0034311c: add      r2, sp, #0x2c
00343120: str      r4, [sp, #0x10]
00343124: str      lr, [sp, #0x14]
00343128: str      ip, [sp, #0x2c]
0034312c: bl       #0x342bbc ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
00343130: ldr      r3, [sp, #0x28]
00343134: mov      r0, #1
00343138: str      r7, [r3, #0x14]
0034313c: b        #0x343028
00343140: mov      ip, r5
00343144: b        #0x342fd4

# _ZTV13ObjectManager
0095c908: andeq    r0, r0, r0
0095c90c: andeq    r0, r0, r0
0095c910: eorseq   sb, r4, r4, lsl fp
0095c914: eorseq   sb, r4, r0, ror lr

# _ZN13ObjectManager17ProcessLostPacketEii
00348644: push     {r4, r5, r6, lr}
00348648: ldr      r3, [r0, #0x9c]
0034864c: mov      r4, r0
00348650: add      r5, r0, #0x98
00348654: cmp      r3, #0
00348658: beq      #0x3486f0
0034865c: mov      r6, r5
00348660: mov      r0, r3
00348664: b        #0x34866c
00348668: mov      r0, ip
0034866c: ldr      ip, [r0, #0x10]
00348670: cmp      ip, r1
00348674: ldrlt    ip, [r0, #0xc]
00348678: ldrge    ip, [r0, #8]
0034867c: movlt    r0, r6
00348680: mov      r6, r0
00348684: cmp      ip, #0
00348688: bne      #0x348668
0034868c: cmp      r5, r0
00348690: beq      #0x3486f8
00348694: ldr      ip, [r0, #0x10]
00348698: cmp      ip, r1
0034869c: bgt      #0x3486f0
003486a0: cmp      r5, r0
003486a4: beq      #0x3486f8
003486a8: ldr      r1, [r0, #0x14]!
003486ac: cmp      r1, r0
003486b0: beq      #0x3486f8
003486b4: ldr      ip, [r1, #8]
003486b8: cmp      ip, r2
003486bc: beq      #0x3486d0
003486c0: ldr      r1, [r1]
003486c4: cmp      r0, r1
003486c8: bne      #0x3486b4
003486cc: mov      r1, r0
003486d0: cmp      r1, r0
003486d4: beq      #0x348720
003486d8: ldr      r2, [r4, #0xa8]
003486dc: cmp      r2, #0
003486e0: bne      #0x3486fc
003486e4: mov      r0, r4
003486e8: pop      {r4, r5, r6, lr}
003486ec: b        #0x347fd0
003486f0: mov      r0, r5
003486f4: b        #0x3486a0
003486f8: pop      {r4, r5, r6, pc}
003486fc: mov      r1, r3
00348700: mov      r0, r5
00348704: bl       #0x345b4c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIiSaIiEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00348708: mov      r3, #0
0034870c: str      r5, [r4, #0xa4]
00348710: str      r3, [r4, #0xa8]
00348714: str      r5, [r4, #0xa0]
00348718: str      r3, [r4, #0x9c]
0034871c: b        #0x3486e4
00348720: pop      {r4, r5, r6, pc}

# _ZN13ObjectManager24GetNetworkIdByObjectBaseEPK10ObjectBase
003402f4: cmp      r1, #0
003402f8: mvneq    r0, #0
003402fc: ldrne    r0, [r1, #0x108]
00340300: bx       lr

# _ZN13ObjectManagerD1Ev
00349b14: ldr      r3, [pc, #0x34c]
00349b18: ldr      r2, [pc, #0x34c]
00349b1c: push     {r4, r5, r6, lr}
00349b20: add      r3, pc, r3
00349b24: ldr      r2, [r3, r2]
00349b28: mov      r4, r0
00349b2c: add      r2, r2, #8
00349b30: str      r2, [r0]
00349b34: bl       #0x3496b8 ; _ZN13ObjectManager5FlushEv
00349b38: ldr      r3, [r4, #0x1a4]
00349b3c: cmp      r3, #0
00349b40: beq      #0x349b68
00349b44: add      r5, r4, #0x194
00349b48: mov      r0, r5
00349b4c: ldr      r1, [r4, #0x198]
00349b50: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349b54: mov      r3, #0
00349b58: str      r5, [r4, #0x1a0]
00349b5c: str      r3, [r4, #0x1a4]
00349b60: str      r5, [r4, #0x19c]
00349b64: str      r3, [r4, #0x198]
00349b68: ldr      r3, [r4, #0x18c]
00349b6c: cmp      r3, #0
00349b70: beq      #0x349b98
00349b74: add      r5, r4, #0x17c
00349b78: mov      r0, r5
00349b7c: ldr      r1, [r4, #0x180]
00349b80: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349b84: mov      r3, #0
00349b88: str      r5, [r4, #0x188]
00349b8c: str      r3, [r4, #0x18c]
00349b90: str      r5, [r4, #0x184]
00349b94: str      r3, [r4, #0x180]
00349b98: ldr      r3, [r4, #0x174]
00349b9c: cmp      r3, #0
00349ba0: beq      #0x349bc8
00349ba4: add      r5, r4, #0x164
00349ba8: mov      r0, r5
00349bac: ldr      r1, [r4, #0x168]
00349bb0: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349bb4: mov      r3, #0
00349bb8: str      r5, [r4, #0x170]
00349bbc: str      r3, [r4, #0x174]
00349bc0: str      r5, [r4, #0x16c]
00349bc4: str      r3, [r4, #0x168]
00349bc8: ldr      r3, [r4, #0x158]
00349bcc: cmp      r3, #0
00349bd0: bne      #0x349e40
00349bd4: add      r0, r4, #0x130
00349bd8: bl       #0x345a4c ; _ZNSt4priv10_List_baseIiSaIiEE5clearEv
00349bdc: add      r0, r4, #0x128
00349be0: bl       #0x345a4c ; _ZNSt4priv10_List_baseIiSaIiEE5clearEv
00349be4: ldr      r0, [r4, #0x120]
00349be8: add      r6, r4, #0x120
00349bec: cmp      r0, r6
00349bf0: bne      #0x349bfc
00349bf4: b        #0x349c14
00349bf8: mov      r0, r5
00349bfc: ldr      r5, [r0]
00349c00: mov      r1, #0xc
00349c04: bl       #0x708f00 ; 
00349c08: cmp      r5, r6
00349c0c: bne      #0x349bf8
00349c10: mov      r0, r6
00349c14: str      r0, [r4, #0x120]
00349c18: str      r0, [r6, #4]
00349c1c: ldr      r3, [r4, #0x118]
00349c20: cmp      r3, #0
00349c24: bne      #0x349e18
00349c28: add      r0, r4, #0x100
00349c2c: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349c30: ldr      r3, [r4, #0xf0]
00349c34: cmp      r3, #0
00349c38: beq      #0x349c60
00349c3c: add      r5, r4, #0xe0
00349c40: mov      r0, r5
00349c44: ldr      r1, [r4, #0xe4]
00349c48: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349c4c: mov      r3, #0
00349c50: str      r5, [r4, #0xec]
00349c54: str      r3, [r4, #0xf0]
00349c58: str      r5, [r4, #0xe8]
00349c5c: str      r3, [r4, #0xe4]
00349c60: ldr      r3, [r4, #0xd8]
00349c64: cmp      r3, #0
00349c68: beq      #0x349c90
00349c6c: add      r5, r4, #0xc8
00349c70: mov      r0, r5
00349c74: ldr      r1, [r4, #0xcc]
00349c78: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349c7c: mov      r3, #0
00349c80: str      r5, [r4, #0xd4]
00349c84: str      r3, [r4, #0xd8]
00349c88: str      r5, [r4, #0xd0]
00349c8c: str      r3, [r4, #0xcc]
00349c90: ldr      r3, [r4, #0xc0]
00349c94: cmp      r3, #0
00349c98: beq      #0x349cc0
00349c9c: add      r5, r4, #0xb0
00349ca0: mov      r0, r5
00349ca4: ldr      r1, [r4, #0xb4]
00349ca8: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349cac: mov      r3, #0
00349cb0: str      r5, [r4, #0xbc]
00349cb4: str      r3, [r4, #0xc0]
00349cb8: str      r5, [r4, #0xb8]
00349cbc: str      r3, [r4, #0xb4]
00349cc0: ldr      r3, [r4, #0xa8]
00349cc4: cmp      r3, #0
00349cc8: bne      #0x349df0
00349ccc: add      r0, r4, #0x90
00349cd0: bl       #0x345914 ; _ZNSt4priv10_List_baseIP10GameObjectSaIS2_EE5clearEv
00349cd4: add      r0, r4, #0x88
00349cd8: bl       #0x345914 ; _ZNSt4priv10_List_baseIP10GameObjectSaIS2_EE5clearEv
00349cdc: ldr      r0, [r4, #0x80]
00349ce0: add      r6, r4, #0x80
00349ce4: cmp      r0, r6
00349ce8: bne      #0x349cf4
00349cec: b        #0x349d0c
00349cf0: mov      r0, r5
00349cf4: ldr      r5, [r0]
00349cf8: mov      r1, #0xc
00349cfc: bl       #0x708f00 ; 
00349d00: cmp      r5, r6
00349d04: bne      #0x349cf0
00349d08: mov      r0, r6
00349d0c: str      r0, [r4, #0x80]
00349d10: str      r0, [r6, #4]
00349d14: add      r0, r4, #0x70
00349d18: bl       #0x345b8c ; _ZNSt4priv10_List_baseIP9CharacterSaIS2_EE5clearEv
00349d1c: ldr      r0, [r4, #0x68]
00349d20: add      r6, r4, #0x68
00349d24: cmp      r6, r0
00349d28: bne      #0x349d34
00349d2c: b        #0x349d48
00349d30: mov      r0, r5
00349d34: ldr      r5, [r0]
00349d38: mov      r1, #0xc
00349d3c: bl       #0x708f00 ; 
00349d40: cmp      r6, r5
00349d44: bne      #0x349d30
00349d48: str      r6, [r4, #0x68]
00349d4c: add      r0, r4, #0x60
00349d50: str      r6, [r6, #4]
00349d54: bl       #0x345b8c ; _ZNSt4priv10_List_baseIP9CharacterSaIS2_EE5clearEv
00349d58: add      r0, r4, #0x44
00349d5c: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349d60: add      r0, r4, #0x3c
00349d64: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349d68: add      r0, r4, #0x34
00349d6c: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349d70: add      r0, r4, #0x2c
00349d74: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349d78: ldr      r0, [r4, #0x24]
00349d7c: add      r6, r4, #0x24
00349d80: cmp      r0, r6
00349d84: bne      #0x349d90
00349d88: b        #0x349da8
00349d8c: mov      r0, r5
00349d90: ldr      r5, [r0]
00349d94: mov      r1, #0xc
00349d98: bl       #0x708f00 ; 
00349d9c: cmp      r5, r6
00349da0: bne      #0x349d8c
00349da4: mov      r0, r6
00349da8: str      r0, [r4, #0x24]
00349dac: str      r0, [r6, #4]
00349db0: ldr      r3, [r4, #0x1c]
00349db4: cmp      r3, #0
00349db8: beq      #0x349de0
00349dbc: add      r5, r4, #0xc
00349dc0: mov      r0, r5
00349dc4: ldr      r1, [r4, #0x10]
00349dc8: bl       #0x347ed4 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349dcc: mov      r3, #0
00349dd0: str      r5, [r4, #0x18]
00349dd4: str      r3, [r4, #0x1c]
00349dd8: str      r5, [r4, #0x14]
00349ddc: str      r3, [r4, #0x10]
00349de0: add      r0, r4, #4
00349de4: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349de8: mov      r0, r4
00349dec: pop      {r4, r5, r6, pc}
00349df0: add      r5, r4, #0x98
00349df4: mov      r0, r5
00349df8: ldr      r1, [r4, #0x9c]
00349dfc: bl       #0x345b4c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIiSaIiEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349e00: mov      r3, #0
00349e04: str      r5, [r4, #0xa4]
00349e08: str      r3, [r4, #0xa8]
00349e0c: str      r5, [r4, #0xa0]
00349e10: str      r3, [r4, #0x9c]
00349e14: b        #0x349ccc
00349e18: add      r5, r4, #0x108
00349e1c: mov      r0, r5
00349e20: ldr      r1, [r4, #0x10c]
00349e24: bl       #0x3458d4 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt4listIP10ObjectBaseSaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349e28: mov      r3, #0
00349e2c: str      r5, [r4, #0x114]
00349e30: str      r3, [r4, #0x118]
00349e34: str      r5, [r4, #0x110]
00349e38: str      r3, [r4, #0x10c]
00349e3c: b        #0x349c28
00349e40: add      r5, r4, #0x148
00349e44: mov      r0, r5
00349e48: ldr      r1, [r4, #0x14c]
00349e4c: bl       #0x345c94 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349e50: mov      r3, #0
00349e54: str      r5, [r4, #0x154]
00349e58: str      r3, [r4, #0x158]
00349e5c: str      r5, [r4, #0x150]
00349e60: str      r3, [r4, #0x14c]
00349e64: b        #0x349bd4
00349e68: rsbeq    sl, r4, r0, ror pc
00349e6c: andeq    r1, r0, ip, asr r1

# _ZNK10GameObject17GetTargetPositionEv
003935dc: ldr      r3, [r0, #0x180]
003935e0: cmp      r3, #0
003935e4: beq      #0x3935f8
003935e8: ldrb     r3, [r0, #0x80]
003935ec: cmp      r3, #0
003935f0: addne    r0, r0, #0x184
003935f4: bxne     lr
003935f8: add      r0, r0, #0x160
003935fc: bx       lr

# _ZN8RoomZone6UpdateEv
00396e9c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00396ea0: ldr      r2, [pc, #0x218]
00396ea4: ldr      r3, [pc, #0x218]
00396ea8: sub      sp, sp, #0x3c
00396eac: add      r2, pc, r2
00396eb0: str      r3, [sp, #0x24]
00396eb4: mov      r4, r0
00396eb8: str      r2, [sp, #8]
00396ebc: ldr      r0, [r2, r3]
00396ec0: ldr      r2, [r4, #0x12c]
00396ec4: str      r2, [sp, #0x20]
00396ec8: ldr      r3, [r4, #0x130]
00396ecc: str      r3, [sp, #0x1c]
00396ed0: ldr      r2, [r4, #0x134]
00396ed4: str      r2, [sp, #0x18]
00396ed8: ldr      r3, [r4, #0x138]
00396edc: str      r3, [sp, #0x14]
00396ee0: ldr      r2, [r4, #0x13c]
00396ee4: str      r2, [sp, #0x10]
00396ee8: ldr      r3, [r4, #0x140]
00396eec: str      r3, [sp, #0xc]
00396ef0: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
00396ef4: ldr      r3, [r0, #0x128]
00396ef8: mov      r2, #0
00396efc: str      r2, [sp, #4]
00396f00: ldr      r3, [r3, #8]
00396f04: mov      r0, r3
00396f08: ldr      r3, [r3]
00396f0c: mov      lr, pc
00396f10: ldr      pc, [r3, #0x144]
00396f14: mov      r5, r0
00396f18: ldr      r8, [r5, #0xc]
00396f1c: mov      r1, #0
00396f20: mov      r0, r8
00396f24: bl       #0x30e4b4 ; 
00396f28: ldr      r7, [r5, #0x10]
00396f2c: cmp      r0, #0
00396f30: mov      r1, #0
00396f34: mov      r0, r7
00396f38: ldreq    fp, [sp, #0x14]
00396f3c: ldrne    fp, [sp, #0x20]
00396f40: bl       #0x30e4b4 ; 
00396f44: ldr      r6, [r5, #0x14]
00396f48: cmp      r0, #0
00396f4c: mov      r1, #0
00396f50: mov      r0, r6
00396f54: ldreq    sb, [sp, #0x10]
00396f58: ldrne    sb, [sp, #0x1c]
00396f5c: bl       #0x30e4b4 ; 
00396f60: mov      r1, fp
00396f64: cmp      r0, #0
00396f68: mov      r0, r8
00396f6c: ldreq    sl, [sp, #0xc]
00396f70: ldrne    sl, [sp, #0x18]
00396f74: bl       #0x30ed6c ; 
00396f78: mov      r1, sb
00396f7c: mov      r8, r0
00396f80: mov      r0, r7
00396f84: bl       #0x30ed6c ; 
00396f88: mov      r1, r0
00396f8c: mov      r0, r8
00396f90: bl       #0x30eba4 ; 
00396f94: mov      r1, sl
00396f98: mov      r7, r0
00396f9c: mov      r0, r6
00396fa0: bl       #0x30ed6c ; 
00396fa4: mov      r1, r0
00396fa8: mov      r0, r7
00396fac: bl       #0x30eba4 ; 
00396fb0: ldr      r1, [r5, #0x18]
00396fb4: bl       #0x30eba4 ; 
00396fb8: mov      r1, #0
00396fbc: bl       #0x30e2f8 ; 
00396fc0: cmp      r0, #0
00396fc4: beq      #0x397000
00396fc8: ldrb     r3, [r4, #0x389]
00396fcc: cmp      r3, #0
00396fd0: bne      #0x396fe0
00396fd4: ldrb     r3, [r4, #0x388]
00396fd8: cmp      r3, #0
00396fdc: beq      #0x396fe8
00396fe0: mov      r0, r4
00396fe4: bl       #0x396918 ; _ZN8RoomZone10DeActivateEv
00396fe8: mov      r3, #0
00396fec: strb     r3, [r4, #0x389]
00396ff0: mov      r3, #0
00396ff4: strb     r3, [r4, #0x388]
00396ff8: add      sp, sp, #0x3c
00396ffc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00397000: ldr      r3, [sp, #4]
00397004: add      r5, r5, #0x10
00397008: add      r3, r3, #1
0039700c: cmp      r3, #6
00397010: str      r3, [sp, #4]
00397014: bne      #0x396f18
00397018: ldrb     r3, [r4, #0x389]
0039701c: cmp      r3, #0
00397020: beq      #0x397030
00397024: ldrb     r3, [r4, #0x388]
00397028: cmp      r3, #0
0039702c: beq      #0x397038
00397030: mov      r0, r4
00397034: bl       #0x396d24 ; _ZN8RoomZone8ActivateEv
00397038: ldr      r2, [sp, #8]
0039703c: ldr      r3, [sp, #0x24]
00397040: mov      r5, #1
00397044: strb     r5, [r4, #0x389]
00397048: ldr      r6, [r2, r3]
0039704c: mov      r0, r4
00397050: ldr      r3, [r6, #0x38]
00397054: ldr      r2, [r3, #0xf8]
00397058: add      r2, r2, r5
0039705c: str      r2, [r3, #0xf8]
00397060: bl       #0x3964a8 ; _ZNK8RoomZone14HasBeenVisitedEv
00397064: subs     r1, r0, #0
00397068: bne      #0x396ff0
0039706c: ldr      r0, [r6, #0x40]
00397070: mov      r2, r5
00397074: bl       #0x36e478 ; _ZN13PlayerManager14GetLocalPlayerEib
00397078: ldr      r3, [r0, #0x660]
0039707c: cmp      r3, #0
00397080: beq      #0x396ff0
00397084: ldr      ip, [r3, #0x160]
00397088: ldr      r2, [r3, #0x164]
0039708c: ldr      r3, [r3, #0x168]
00397090: mov      r0, r4
00397094: add      r1, sp, #0x2c
00397098: str      ip, [sp, #0x2c]
0039709c: str      r2, [sp, #0x30]
003970a0: str      r3, [sp, #0x34]
003970a4: bl       #0x3964cc ; _ZNK8RoomZone9HasInsideERK7Point3DIfE
003970a8: cmp      r0, #0
003970ac: beq      #0x396ff0
003970b0: mov      r1, r5
003970b4: mov      r0, r4
003970b8: bl       #0x3964bc ; _ZN8RoomZone10SetVisitedEb
003970bc: b        #0x396ff0
003970c0: subseq   sp, pc, r4, ror #23
003970c4: strdeq   r3, r4, [r0], -r4

# _ZN13ObjectManager22AskNetResendByObjectIdEi
00345e30: push     {r4, r5, r6, lr}
00345e34: sub      sp, sp, #0x20
00345e38: add      r6, sp, #4
00345e3c: mov      r4, r1
00345e40: mov      r2, r4
00345e44: mov      r1, r0
00345e48: mov      r5, r0
00345e4c: mov      r0, r6
00345e50: bl       #0x3407a0 ; _ZN13ObjectManager26GetObjectHandleByNetworkIdEi
00345e54: mov      r0, r6
00345e58: bl       #0x33fee4 ; _ZN12ObjectHandlecvP10GameObjectEv
00345e5c: ldr      r3, [r0, #0x110]
00345e60: add      r6, sp, #0x20
00345e64: add      r5, r5, #0x194
00345e68: str      r3, [r6, #-8]!
00345e6c: mov      r0, r5
00345e70: mov      r1, r6
00345e74: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00345e78: ldr      r3, [r0, #4]
00345e7c: cmp      r3, #0
00345e80: beq      #0x345ed8
00345e84: mov      r1, r0
00345e88: sxth     ip, r4
00345e8c: b        #0x345e94
00345e90: mov      r3, r2
00345e94: ldrsh    r2, [r3, #0x10]
00345e98: cmp      r2, ip
00345e9c: ldrlt    r2, [r3, #0xc]
00345ea0: ldrge    r2, [r3, #8]
00345ea4: movlt    r3, r1
00345ea8: mov      r1, r3
00345eac: cmp      r2, #0
00345eb0: bne      #0x345e90
00345eb4: cmp      r0, r3
00345eb8: beq      #0x345ee0
00345ebc: ldrsh    r2, [r3, #0x10]
00345ec0: cmp      r2, ip
00345ec4: bgt      #0x345ed8
00345ec8: cmp      r0, r3
00345ecc: beq      #0x345ee0
00345ed0: add      sp, sp, #0x20
00345ed4: pop      {r4, r5, r6, pc}
00345ed8: mov      r3, r0
00345edc: b        #0x345ec8
00345ee0: mov      r1, r6
00345ee4: mov      r0, r5
00345ee8: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00345eec: add      r2, sp, #0x1e
00345ef0: mov      r1, r0
00345ef4: add      r0, sp, #0x10
00345ef8: strh     r4, [sp, #0x1e]
00345efc: bl       #0x344994 ; _ZNSt4priv8_Rb_treeIsSt4lessIsEsNS_9_IdentityIsEENS_11_SetTraitsTIsEESaIsEE13insert_uniqueERKs
00345f00: b        #0x345ed0

# _ZN13ObjectManager15sReadPacketDataEiiR12NetBitStream
00347d90: push     {r4, r5, r6, lr}
00347d94: mov      r6, r0
00347d98: mov      r0, r2
00347d9c: mov      r4, r2
00347da0: mov      r5, r1
00347da4: bl       #0x80e498 ; _ZN12NetBitStream7ReadBitEv
00347da8: ldr      r3, [pc, #0x34]
00347dac: cmp      r0, #0
00347db0: add      r3, pc, r3
00347db4: beq      #0x347de0
00347db8: ldr      r2, [pc, #0x28]
00347dbc: ldr      r3, [r3, r2]
00347dc0: ldr      r0, [r3, #0x38]
00347dc4: cmp      r0, #0
00347dc8: beq      #0x347de0
00347dcc: mov      r1, r6
00347dd0: mov      r2, r5
00347dd4: mov      r3, r4
00347dd8: pop      {r4, r5, r6, lr}
00347ddc: b        #0x34735c
00347de0: pop      {r4, r5, r6, pc}
00347de4: rsbeq    ip, r4, r0, ror #25
00347de8: strdeq   r3, r4, [r0], -r4

# _ZN12ObjectHandleC2Ev
0033f4f4: mov      r2, #0
0033f4f8: mvn      r1, #0
0033f4fc: str      r1, [r0, #8]
0033f500: str      r2, [r0, #4]
0033f504: str      r2, [r0]
0033f508: bx       lr

# _ZN13ObjectManager15GetObjectByNameEPKcibS1_
0034aca0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034aca4: ldr      r5, [pc, #0x38c]
0034aca8: ldr      ip, [pc, #0x38c]
0034acac: sub      sp, sp, #0x74
0034acb0: add      r5, pc, r5
0034acb4: str      ip, [sp, #0x14]
0034acb8: ldr      ip, [r5, ip]
0034acbc: str      r1, [sp, #0x18]
0034acc0: ldr      r1, [pc, #0x378]
0034acc4: ldr      ip, [ip]
0034acc8: str      r0, [sp, #0xc]
0034accc: mov      r4, r2
0034acd0: mov      r0, r2
0034acd4: add      r1, pc, r1
0034acd8: mov      r2, #6
0034acdc: str      ip, [sp, #0x6c]
0034ace0: mov      r7, r3
0034ace4: bl       #0x30ec7c ; 
0034ace8: ldrb     lr, [sp, #0x98]
0034acec: cmp      r0, #0
0034acf0: ldr      r6, [sp, #0x9c]
0034acf4: str      lr, [sp, #0x24]
0034acf8: beq      #0x34ad18
0034acfc: ldr      r1, [pc, #0x340]
0034ad00: mov      r0, r4
0034ad04: mov      r2, #0x10
0034ad08: add      r1, pc, r1
0034ad0c: bl       #0x30ec7c ; 
0034ad10: cmp      r0, #0
0034ad14: bne      #0x34af98
0034ad18: mvn      r7, #0
0034ad1c: ldr      r1, [pc, #0x324]
0034ad20: mov      r0, r4
0034ad24: add      r1, pc, r1
0034ad28: bl       #0x30e31c ; 
0034ad2c: cmp      r0, #0
0034ad30: beq      #0x34afb8
0034ad34: add      lr, sp, #0x40
0034ad38: mov      r0, lr
0034ad3c: str      lr, [sp, #0x20]
0034ad40: bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
0034ad44: ldr      r3, [pc, #0x300]
0034ad48: ldr      r0, [sp, #0x18]
0034ad4c: ldr      sb, [pc, #0x2fc]
0034ad50: ldr      r2, [pc, #0x2fc]
0034ad54: add      r3, pc, r3
0034ad58: ldr      r6, [r0, #0x14]
0034ad5c: str      r3, [sp, #8]
0034ad60: add      r3, sp, #0x34
0034ad64: add      sb, pc, sb
0034ad68: add      r8, r0, #0xc
0034ad6c: str      r2, [sp, #0x10]
0034ad70: add      sl, sp, #0x28
0034ad74: str      r3, [sp, #0x1c]
0034ad78: cmp      r6, r8
0034ad7c: beq      #0x34ae98
0034ad80: ldr      r3, [r6, #0x2c]
0034ad84: cmp      r3, #0
0034ad88: beq      #0x34ae6c
0034ad8c: cmn      r7, #1
0034ad90: beq      #0x34adac
0034ad94: ldr      r2, [r3, #0x64]
0034ad98: cmp      r7, r2
0034ad9c: beq      #0x34adac
0034ada0: ldrb     r3, [r3, #0x87]
0034ada4: cmp      r3, #0
0034ada8: beq      #0x34ae6c
0034adac: ldr      r0, [r6, #0x28]
0034adb0: mov      r1, r4
0034adb4: bl       #0x30e31c ; 
0034adb8: cmp      r0, #0
0034adbc: bne      #0x34ae10
0034adc0: ldr      r0, [sp, #0x20]
0034adc4: ldr      r1, [r6, #0x10]
0034adc8: add      r2, r0, #4
0034adcc: ldr      r0, [r2], #4
0034add0: ldr      r3, [sp, #0xc]
0034add4: ldr      r2, [r2]
0034add8: str      r1, [r3], #4
0034addc: ldr      ip, [sp, #0xc]
0034ade0: str      r2, [r3, #4]
0034ade4: str      r0, [ip, #4]
0034ade8: str      r1, [sp, #0x40]
0034adec: ldr      r0, [sp, #0x14]
0034adf0: ldr      r2, [sp, #0x6c]
0034adf4: ldr      r3, [r5, r0]
0034adf8: ldr      r0, [sp, #0xc]
0034adfc: ldr      r3, [r3]
0034ae00: cmp      r2, r3
0034ae04: bne      #0x34b034
0034ae08: add      sp, sp, #0x74
0034ae0c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034ae10: mov      r1, sb
0034ae14: mov      r0, r4
0034ae18: bl       #0x30e31c ; 
0034ae1c: subs     r1, r0, #0
0034ae20: beq      #0x34af60
0034ae24: ldr      r1, [sp, #8]
0034ae28: mov      r0, r4
0034ae2c: bl       #0x30e31c ; 
0034ae30: subs     r1, r0, #0
0034ae34: bne      #0x34ae6c
0034ae38: ldr      lr, [sp, #0x10]
0034ae3c: mov      r2, #1
0034ae40: ldr      r3, [r5, lr]
0034ae44: ldr      r0, [r3, #0x40]
0034ae48: bl       #0x36e478 ; _ZN13PlayerManager14GetLocalPlayerEib
0034ae4c: ldr      r1, [r6, #0x2c]
0034ae50: ldr      fp, [r0, #0x660]
0034ae54: mov      r0, sl
0034ae58: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0034ae5c: mov      r0, sl
0034ae60: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0034ae64: cmp      fp, r0
0034ae68: beq      #0x34adc0
0034ae6c: ldr      r3, [r6, #0xc]
0034ae70: cmp      r3, #0
0034ae74: bne      #0x34ae80
0034ae78: b        #0x34af2c
0034ae7c: mov      r3, r2
0034ae80: ldr      r2, [r3, #8]
0034ae84: cmp      r2, #0
0034ae88: bne      #0x34ae7c
0034ae8c: mov      r6, r3
0034ae90: cmp      r6, r8
0034ae94: bne      #0x34ad80
0034ae98: ldr      lr, [sp, #0x24]
0034ae9c: cmp      lr, #0
0034aea0: bne      #0x34afd8
0034aea4: ldr      r3, [pc, #0x1ac]
0034aea8: add      r4, sp, #0x54
0034aeac: ldr      r6, [r5, r3]
0034aeb0: mov      r0, r6
0034aeb4: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0034aeb8: ldr      r1, [pc, #0x19c]
0034aebc: add      r2, sp, #0x50
0034aec0: mov      r0, r4
0034aec4: add      r1, pc, r1
0034aec8: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0034aecc: mov      r0, r6
0034aed0: mov      r1, r4
0034aed4: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
0034aed8: ldr      r0, [sp, #0x68]
0034aedc: cmp      r0, r4
0034aee0: beq      #0x34af00
0034aee4: cmp      r0, #0
0034aee8: beq      #0x34af00
0034aeec: ldr      r1, [sp, #0x54]
0034aef0: rsb      r1, r0, r1
0034aef4: cmp      r1, #0x80
0034aef8: bhi      #0x34b02c
0034aefc: bl       #0x708f00 ; 
0034af00: ldr      r0, [sp, #0x20]
0034af04: ldr      r3, [sp, #0xc]
0034af08: add      r2, r0, #4
0034af0c: ldr      r1, [r2], #4
0034af10: ldr      r0, [sp, #0x40]
0034af14: ldr      r2, [r2]
0034af18: str      r0, [r3], #4
0034af1c: ldr      ip, [sp, #0xc]
0034af20: str      r2, [r3, #4]
0034af24: str      r1, [ip, #4]
0034af28: b        #0x34adec
0034af2c: ldr      r2, [r6, #4]
0034af30: ldr      r1, [r2, #0xc]
0034af34: cmp      r6, r1
0034af38: bne      #0x34af54
0034af3c: mov      r6, r2
0034af40: ldr      r2, [r2, #4]
0034af44: ldr      r3, [r2, #0xc]
0034af48: cmp      r3, r6
0034af4c: beq      #0x34af3c
0034af50: ldr      r3, [r6, #0xc]
0034af54: cmp      r2, r3
0034af58: movne    r6, r2
0034af5c: b        #0x34ad78
0034af60: ldr      ip, [sp, #0x10]
0034af64: mov      r2, #1
0034af68: ldr      r3, [r5, ip]
0034af6c: ldr      r0, [r3, #0x40]
0034af70: bl       #0x36e478 ; _ZN13PlayerManager14GetLocalPlayerEib
0034af74: ldr      r1, [r6, #0x2c]
0034af78: ldr      fp, [r0, #0x660]
0034af7c: ldr      r0, [sp, #0x1c]
0034af80: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0034af84: ldr      r0, [sp, #0x1c]
0034af88: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0034af8c: cmp      fp, r0
0034af90: bne      #0x34ae24
0034af94: b        #0x34adc0
0034af98: ldr      r1, [pc, #0xc0]
0034af9c: mov      r0, r4
0034afa0: mov      r2, #0xb
0034afa4: add      r1, pc, r1
0034afa8: bl       #0x30ec7c ; 
0034afac: cmp      r0, #0
0034afb0: bne      #0x34ad1c
0034afb4: b        #0x34ad18
0034afb8: ldr      ip, [sp, #0x24]
0034afbc: ldr      r1, [sp, #0x18]
0034afc0: mov      r2, r6
0034afc4: mov      r3, r7
0034afc8: ldr      r0, [sp, #0xc]
0034afcc: str      ip, [sp]
0034afd0: bl       #0x34b064 ; _ZN13ObjectManager22GetHighestThreatPlayerEPKcib
0034afd4: b        #0x34adec
0034afd8: ldr      r0, [sp, #0x18]
0034afdc: ldr      r2, [sp, #0x18]
0034afe0: add      r1, sp, #0x70
0034afe4: ldr      r3, [r0, #0x4c]
0034afe8: mov      r0, r6
0034afec: str      r3, [r1, #-0x24]!
0034aff0: add      r3, r3, #1
0034aff4: str      r3, [r2, #0x4c]
0034aff8: bl       #0x34952c ; _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
0034affc: mov      r6, r0
0034b000: mov      r0, r4
0034b004: bl       #0x30de54 ; 
0034b008: mov      r1, r4
0034b00c: add      r2, r4, r0
0034b010: mov      r0, r6
0034b014: bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
0034b018: ldr      r3, [sp, #0x20]
0034b01c: ldr      r1, [sp, #0x4c]
0034b020: add      r2, r3, #4
0034b024: ldr      r0, [r2], #4
0034b028: b        #0x34add0
0034b02c: bl       #0x310440 ; _Z10CustomFreePv
0034b030: b        #0x34af00
0034b034: bl       #0x30e310 ; 
0034b038: rsbeq    sb, r4, r0, ror #27
0034b03c: andeq    r4, r0, ip, lsr #1
0034b040: ldrsheq  r5, [r7], #-0x64
0034b044: subseq   r5, r7, r0, asr #12
0034b048: ldrheq   r5, [r7], #-0x6c
0034b04c: subseq   r5, r7, ip, ror r6
0034b050: subseq   r5, r7, r4, ror #12
0034b054: strdeq   r3, r4, [r0], -r4
0034b058: andeq    r0, r0, r4, lsl #17
0034b05c: subseq   r5, r7, r4, lsr r5
0034b060: subseq   r5, r7, ip, lsr #8

# _ZN13ObjectManager34ProcessNextGameObjectToStartUpdateEv
003460cc: push     {r4, lr}
003460d0: mov      r3, r0
003460d4: mov      r4, r0
003460d8: ldr      r0, [r3, #0x90]!
003460dc: cmp      r0, r3
003460e0: beq      #0x346118
003460e4: ldr      r3, [r0, #8]
003460e8: cmp      r3, #0
003460ec: beq      #0x3460fc
003460f0: mov      r0, r3
003460f4: bl       #0x38c710 ; _ZN10GameObject11ZoneEnteredEv
003460f8: ldr      r0, [r4, #0x90]
003460fc: ldr      r3, [r0]
00346100: ldr      r2, [r0, #4]
00346104: mov      r1, #0xc
00346108: str      r3, [r2]
0034610c: str      r2, [r3, #4]
00346110: pop      {r4, lr}
00346114: b        #0x708f00
00346118: pop      {r4, pc}

# _ZN13ObjectManagerD2Ev
00349e8c: ldr      r3, [pc, #0x34c]
00349e90: ldr      r2, [pc, #0x34c]
00349e94: push     {r4, r5, r6, lr}
00349e98: add      r3, pc, r3
00349e9c: ldr      r2, [r3, r2]
00349ea0: mov      r4, r0
00349ea4: add      r2, r2, #8
00349ea8: str      r2, [r0]
00349eac: bl       #0x3496b8 ; _ZN13ObjectManager5FlushEv
00349eb0: ldr      r3, [r4, #0x1a4]
00349eb4: cmp      r3, #0
00349eb8: beq      #0x349ee0
00349ebc: add      r5, r4, #0x194
00349ec0: mov      r0, r5
00349ec4: ldr      r1, [r4, #0x198]
00349ec8: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349ecc: mov      r3, #0
00349ed0: str      r5, [r4, #0x1a0]
00349ed4: str      r3, [r4, #0x1a4]
00349ed8: str      r5, [r4, #0x19c]
00349edc: str      r3, [r4, #0x198]
00349ee0: ldr      r3, [r4, #0x18c]
00349ee4: cmp      r3, #0
00349ee8: beq      #0x349f10
00349eec: add      r5, r4, #0x17c
00349ef0: mov      r0, r5
00349ef4: ldr      r1, [r4, #0x180]
00349ef8: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349efc: mov      r3, #0
00349f00: str      r5, [r4, #0x188]
00349f04: str      r3, [r4, #0x18c]
00349f08: str      r5, [r4, #0x184]
00349f0c: str      r3, [r4, #0x180]
00349f10: ldr      r3, [r4, #0x174]
00349f14: cmp      r3, #0
00349f18: beq      #0x349f40
00349f1c: add      r5, r4, #0x164
00349f20: mov      r0, r5
00349f24: ldr      r1, [r4, #0x168]
00349f28: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349f2c: mov      r3, #0
00349f30: str      r5, [r4, #0x170]
00349f34: str      r3, [r4, #0x174]
00349f38: str      r5, [r4, #0x16c]
00349f3c: str      r3, [r4, #0x168]
00349f40: ldr      r3, [r4, #0x158]
00349f44: cmp      r3, #0
00349f48: bne      #0x34a1b8
00349f4c: add      r0, r4, #0x130
00349f50: bl       #0x345a4c ; _ZNSt4priv10_List_baseIiSaIiEE5clearEv
00349f54: add      r0, r4, #0x128
00349f58: bl       #0x345a4c ; _ZNSt4priv10_List_baseIiSaIiEE5clearEv
00349f5c: ldr      r0, [r4, #0x120]
00349f60: add      r6, r4, #0x120
00349f64: cmp      r0, r6
00349f68: bne      #0x349f74
00349f6c: b        #0x349f8c
00349f70: mov      r0, r5
00349f74: ldr      r5, [r0]
00349f78: mov      r1, #0xc
00349f7c: bl       #0x708f00 ; 
00349f80: cmp      r5, r6
00349f84: bne      #0x349f70
00349f88: mov      r0, r6
00349f8c: str      r0, [r4, #0x120]
00349f90: str      r0, [r6, #4]
00349f94: ldr      r3, [r4, #0x118]
00349f98: cmp      r3, #0
00349f9c: bne      #0x34a190
00349fa0: add      r0, r4, #0x100
00349fa4: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349fa8: ldr      r3, [r4, #0xf0]
00349fac: cmp      r3, #0
00349fb0: beq      #0x349fd8
00349fb4: add      r5, r4, #0xe0
00349fb8: mov      r0, r5
00349fbc: ldr      r1, [r4, #0xe4]
00349fc0: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349fc4: mov      r3, #0
00349fc8: str      r5, [r4, #0xec]
00349fcc: str      r3, [r4, #0xf0]
00349fd0: str      r5, [r4, #0xe8]
00349fd4: str      r3, [r4, #0xe4]
00349fd8: ldr      r3, [r4, #0xd8]
00349fdc: cmp      r3, #0
00349fe0: beq      #0x34a008
00349fe4: add      r5, r4, #0xc8
00349fe8: mov      r0, r5
00349fec: ldr      r1, [r4, #0xcc]
00349ff0: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349ff4: mov      r3, #0
00349ff8: str      r5, [r4, #0xd4]
00349ffc: str      r3, [r4, #0xd8]
0034a000: str      r5, [r4, #0xd0]
0034a004: str      r3, [r4, #0xcc]
0034a008: ldr      r3, [r4, #0xc0]
0034a00c: cmp      r3, #0
0034a010: beq      #0x34a038
0034a014: add      r5, r4, #0xb0
0034a018: mov      r0, r5
0034a01c: ldr      r1, [r4, #0xb4]
0034a020: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
0034a024: mov      r3, #0
0034a028: str      r5, [r4, #0xbc]
0034a02c: str      r3, [r4, #0xc0]
0034a030: str      r5, [r4, #0xb8]
0034a034: str      r3, [r4, #0xb4]
0034a038: ldr      r3, [r4, #0xa8]
0034a03c: cmp      r3, #0
0034a040: bne      #0x34a168
0034a044: add      r0, r4, #0x90
0034a048: bl       #0x345914 ; _ZNSt4priv10_List_baseIP10GameObjectSaIS2_EE5clearEv
0034a04c: add      r0, r4, #0x88
0034a050: bl       #0x345914 ; _ZNSt4priv10_List_baseIP10GameObjectSaIS2_EE5clearEv
0034a054: ldr      r0, [r4, #0x80]
0034a058: add      r6, r4, #0x80
0034a05c: cmp      r0, r6
0034a060: bne      #0x34a06c
0034a064: b        #0x34a084
0034a068: mov      r0, r5
0034a06c: ldr      r5, [r0]
0034a070: mov      r1, #0xc
0034a074: bl       #0x708f00 ; 
0034a078: cmp      r5, r6
0034a07c: bne      #0x34a068
0034a080: mov      r0, r6
0034a084: str      r0, [r4, #0x80]
0034a088: str      r0, [r6, #4]
0034a08c: add      r0, r4, #0x70
0034a090: bl       #0x345b8c ; _ZNSt4priv10_List_baseIP9CharacterSaIS2_EE5clearEv
0034a094: ldr      r0, [r4, #0x68]
0034a098: add      r6, r4, #0x68
0034a09c: cmp      r6, r0
0034a0a0: bne      #0x34a0ac
0034a0a4: b        #0x34a0c0
0034a0a8: mov      r0, r5
0034a0ac: ldr      r5, [r0]
0034a0b0: mov      r1, #0xc
0034a0b4: bl       #0x708f00 ; 
0034a0b8: cmp      r6, r5
0034a0bc: bne      #0x34a0a8
0034a0c0: str      r6, [r4, #0x68]
0034a0c4: add      r0, r4, #0x60
0034a0c8: str      r6, [r6, #4]
0034a0cc: bl       #0x345b8c ; _ZNSt4priv10_List_baseIP9CharacterSaIS2_EE5clearEv
0034a0d0: add      r0, r4, #0x44
0034a0d4: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
0034a0d8: add      r0, r4, #0x3c
0034a0dc: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
0034a0e0: add      r0, r4, #0x34
0034a0e4: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
0034a0e8: add      r0, r4, #0x2c
0034a0ec: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
0034a0f0: ldr      r0, [r4, #0x24]
0034a0f4: add      r6, r4, #0x24
0034a0f8: cmp      r0, r6
0034a0fc: bne      #0x34a108
0034a100: b        #0x34a120
0034a104: mov      r0, r5
0034a108: ldr      r5, [r0]
0034a10c: mov      r1, #0xc
0034a110: bl       #0x708f00 ; 
0034a114: cmp      r5, r6
0034a118: bne      #0x34a104
0034a11c: mov      r0, r6
0034a120: str      r0, [r4, #0x24]
0034a124: str      r0, [r6, #4]
0034a128: ldr      r3, [r4, #0x1c]
0034a12c: cmp      r3, #0
0034a130: beq      #0x34a158
0034a134: add      r5, r4, #0xc
0034a138: mov      r0, r5
0034a13c: ldr      r1, [r4, #0x10]
0034a140: bl       #0x347ed4 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
0034a144: mov      r3, #0
0034a148: str      r5, [r4, #0x18]
0034a14c: str      r3, [r4, #0x1c]
0034a150: str      r5, [r4, #0x14]
0034a154: str      r3, [r4, #0x10]
0034a158: add      r0, r4, #4
0034a15c: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
0034a160: mov      r0, r4
0034a164: pop      {r4, r5, r6, pc}
0034a168: add      r5, r4, #0x98
0034a16c: mov      r0, r5
0034a170: ldr      r1, [r4, #0x9c]
0034a174: bl       #0x345b4c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIiSaIiEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
0034a178: mov      r3, #0
0034a17c: str      r5, [r4, #0xa4]
0034a180: str      r3, [r4, #0xa8]
0034a184: str      r5, [r4, #0xa0]
0034a188: str      r3, [r4, #0x9c]
0034a18c: b        #0x34a044
0034a190: add      r5, r4, #0x108
0034a194: mov      r0, r5
0034a198: ldr      r1, [r4, #0x10c]
0034a19c: bl       #0x3458d4 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt4listIP10ObjectBaseSaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
0034a1a0: mov      r3, #0
0034a1a4: str      r5, [r4, #0x114]
0034a1a8: str      r3, [r4, #0x118]
0034a1ac: str      r5, [r4, #0x110]
0034a1b0: str      r3, [r4, #0x10c]
0034a1b4: b        #0x349fa0
0034a1b8: add      r5, r4, #0x148
0034a1bc: mov      r0, r5
0034a1c0: ldr      r1, [r4, #0x14c]
0034a1c4: bl       #0x345c94 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
0034a1c8: mov      r3, #0
0034a1cc: str      r5, [r4, #0x154]
0034a1d0: str      r3, [r4, #0x158]
0034a1d4: str      r5, [r4, #0x150]
0034a1d8: str      r3, [r4, #0x14c]
0034a1dc: b        #0x349f4c

# _ZN14v2Controllable11Ctrl_LookAtERK7Point3DIfE
00404d68: bx       lr

# _ZN8RoomZoneD1Ev
00396828: push     {r4, r5, r6, lr}
0039682c: ldr      r2, [pc, #0x6c]
00396830: ldr      r3, [pc, #0x6c]
00396834: mov      r6, r0
00396838: add      r2, pc, r2
0039683c: ldr      r0, [r0, #0x394]
00396840: ldr      r3, [r2, r3]
00396844: add      r5, r6, #0x394
00396848: cmp      r0, r5
0039684c: add      r1, r3, #0xf4
00396850: add      ip, r3, #8
00396854: add      r3, r3, #0xe8
00396858: str      ip, [r6]
0039685c: str      r3, [r6, #4]
00396860: str      r1, [r6, #0x24]
00396864: bne      #0x396870
00396868: b        #0x396888
0039686c: mov      r0, r4
00396870: ldr      r4, [r0]
00396874: mov      r1, #0xc
00396878: bl       #0x708f00 ; 
0039687c: cmp      r4, r5
00396880: bne      #0x39686c
00396884: mov      r0, r5
00396888: str      r0, [r6, #0x394]
0039688c: str      r0, [r5, #4]
00396890: mov      r0, r6
00396894: bl       #0x397bc4 ; _ZN4ZoneD2Ev
00396898: mov      r0, r6
0039689c: pop      {r4, r5, r6, pc}
003968a0: subseq   lr, pc, r8, asr r2
003968a4: andeq    r2, r0, r4, ror r7

# _ZN12v2Controller10Cmd_LookAtERK7Point3DIfE
00405260: push     {r4, lr}
00405264: ldrb     r2, [r0, #9]
00405268: ldr      r3, [pc, #0x44]
0040526c: cmp      r2, #0
00405270: add      r3, pc, r3
00405274: bne      #0x40529c
00405278: ldr      r2, [pc, #0x38]
0040527c: ldr      r3, [r3, r2]
00405280: ldrb     r3, [r3]
00405284: cmp      r3, #0
00405288: beq      #0x405290
0040528c: pop      {r4, pc}
00405290: ldrb     r3, [r0, #8]
00405294: cmp      r3, #0
00405298: bne      #0x40528c
0040529c: ldr      r3, [r0, #4]
004052a0: mov      r0, r3
004052a4: ldr      r3, [r3]
004052a8: mov      lr, pc
004052ac: ldr      pc, [r3, #0x10]
004052b0: pop      {r4, pc}
004052b4: subseq   pc, r8, r0, lsr #16
004052b8: andeq    r3, r0, r0, asr r6

# _ZN13ObjectManager16sWritePacketDataEiiR12NetBitStream
00347074: ldr      r3, [pc, #0x7c]
00347078: push     {r4, r5, r6, r7, r8, lr}
0034707c: mov      r7, r0
00347080: ldr      r0, [pc, #0x74]
00347084: add      r3, pc, r3
00347088: mov      r6, r1
0034708c: ldr      r4, [r3, r0]
00347090: mov      r5, r2
00347094: mov      r0, r4
00347098: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
0034709c: cmp      r0, #0
003470a0: beq      #0x3470e4
003470a4: ldr      r3, [r0, #0x130]
003470a8: cmp      r3, #0x23
003470ac: ble      #0x3470e4
003470b0: ldr      r3, [r4, #0x38]
003470b4: cmp      r3, #0
003470b8: beq      #0x3470e4
003470bc: mov      r0, r5
003470c0: mov      r1, #1
003470c4: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
003470c8: ldr      r0, [r4, #0x38]
003470cc: mov      r1, r7
003470d0: mov      r2, r6
003470d4: mov      r3, r5
003470d8: bl       #0x346510 ; _ZN13ObjectManager26SerializeGameObjectNetDataEiiR12NetBitStream
003470dc: mov      r0, #1
003470e0: pop      {r4, r5, r6, r7, r8, pc}
003470e4: mov      r0, r5
003470e8: mov      r1, #0
003470ec: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
003470f0: mov      r0, #0
003470f4: pop      {r4, r5, r6, r7, r8, pc}
003470f8: rsbeq    sp, r4, ip, lsl #20
003470fc: strdeq   r3, r4, [r0], -r4

# _ZN13ObjectManager11LoadFromXMLEP12TiXmlElementPKcRK7Point3DIfEi
0034b868: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034b86c: ldr      r4, [pc, #0x338]
0034b870: ldr      r5, [pc, #0x338]
0034b874: subs     r7, r1, #0
0034b878: add      r4, pc, r4
0034b87c: ldr      r1, [r4, r5]
0034b880: mov      sl, r2
0034b884: sub      sp, sp, #0x25c
0034b888: ldr      r2, [r1]
0034b88c: mov      fp, r0
0034b890: str      r3, [sp, #0xc]
0034b894: str      r2, [sp, #0x254]
0034b898: beq      #0x34bb30
0034b89c: ldr      r1, [pc, #0x310]
0034b8a0: mov      r0, r7
0034b8a4: add      r1, pc, r1
0034b8a8: bl       #0x514c70 ; _ZNK12TiXmlElement9AttributeEPKc
0034b8ac: ldr      r1, [pc, #0x304]
0034b8b0: mov      r8, r0
0034b8b4: mov      r0, r7
0034b8b8: add      r1, pc, r1
0034b8bc: bl       #0x514c70 ; _ZNK12TiXmlElement9AttributeEPKc
0034b8c0: subs     sb, r0, #0
0034b8c4: beq      #0x34badc
0034b8c8: cmp      r8, #0
0034b8cc: beq      #0x34badc
0034b8d0: add      r6, sp, #0x2c
0034b8d4: mov      r0, r6
0034b8d8: bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
0034b8dc: cmp      sl, #0
0034b8e0: beq      #0x34b8f8
0034b8e4: mov      r0, sl
0034b8e8: mov      r1, r8
0034b8ec: bl       #0x30e31c ; 
0034b8f0: cmp      r0, #0
0034b8f4: bne      #0x34badc
0034b8f8: ldr      r1, [pc, #0x2bc]
0034b8fc: mov      r0, sb
0034b900: add      r1, pc, r1
0034b904: bl       #0x30e31c ; 
0034b908: cmp      r0, #0
0034b90c: beq      #0x34baf8
0034b910: add      sl, sp, #0x13c
0034b914: mov      r1, sb
0034b918: mov      r0, sl
0034b91c: bl       #0x30eae4 ; 
0034b920: ldr      r1, [pc, #0x298]
0034b924: mov      r2, sl
0034b928: add      r0, sp, #0x3c
0034b92c: add      r1, pc, r1
0034b930: mov      r3, #0x53
0034b934: bl       #0x30eae4 ; 
0034b938: ldr      ip, [sp, #0x280]
0034b93c: add      sl, sp, #0x10
0034b940: mov      r3, sb
0034b944: mov      r1, fp
0034b948: mov      r0, sl
0034b94c: mov      r2, r8
0034b950: mov      sb, #1
0034b954: str      ip, [sp]
0034b958: str      sb, [sp, #4]
0034b95c: bl       #0x34b520 ; _ZN13ObjectManager12GetNewObjectEPKcS1_ib
0034b960: ldr      r1, [sp, #0x14]
0034b964: add      r3, r6, #4
0034b968: ldr      r2, [sp, #0x10]
0034b96c: str      r1, [r3], #4
0034b970: ldr      ip, [sl, #8]
0034b974: add      r6, sp, #0x2c
0034b978: mov      r0, r6
0034b97c: mov      r1, #0
0034b980: str      ip, [r3]
0034b984: str      r2, [sp, #0x2c]
0034b988: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b98c: cmp      r0, #0
0034b990: beq      #0x34badc
0034b994: mov      r1, sb
0034b998: mov      r0, r6
0034b99c: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b9a0: add      r0, r0, #4
0034b9a4: bl       #0x513d78 ; _ZN11PropertyMap14InitPropertiesEv
0034b9a8: ldr      r1, [pc, #0x214]
0034b9ac: mov      r0, r7
0034b9b0: add      r1, pc, r1
0034b9b4: bl       #0x514c70 ; _ZNK12TiXmlElement9AttributeEPKc
0034b9b8: subs     fp, r0, #0
0034b9bc: beq      #0x34ba18
0034b9c0: mov      r1, sb
0034b9c4: mov      r0, r6
0034b9c8: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b9cc: add      sb, sp, #0x23c
0034b9d0: add      sl, r0, #4
0034b9d4: mov      r1, fp
0034b9d8: add      r2, sp, #0x38
0034b9dc: mov      r0, sb
0034b9e0: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0034b9e4: mov      r0, sl
0034b9e8: mov      r1, sb
0034b9ec: bl       #0x513fec ; _ZN11PropertyMap11SetTemplateERKSs
0034b9f0: ldr      r0, [sp, #0x250]
0034b9f4: cmp      r0, sb
0034b9f8: beq      #0x34ba18
0034b9fc: cmp      r0, #0
0034ba00: beq      #0x34ba18
0034ba04: ldr      r1, [sp, #0x23c]
0034ba08: rsb      r1, r0, r1
0034ba0c: cmp      r1, #0x80
0034ba10: bhi      #0x34bba0
0034ba14: bl       #0x708f00 ; 
0034ba18: mov      r1, #1
0034ba1c: mov      r0, r6
0034ba20: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034ba24: add      r0, r0, #4
0034ba28: bl       #0x5136ec ; _ZN11PropertyMap21LoadDefaultPropertiesEv
0034ba2c: mov      r1, #1
0034ba30: mov      r0, r6
0034ba34: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034ba38: mov      r1, r7
0034ba3c: add      r0, r0, #4
0034ba40: bl       #0x513a00 ; _ZN11PropertyMap20LoadOverridesFromXMLEP12TiXmlElement
0034ba44: ldr      r1, [pc, #0x17c]
0034ba48: mov      r0, r8
0034ba4c: add      r1, pc, r1
0034ba50: bl       #0x30e31c ; 
0034ba54: cmp      r0, #0
0034ba58: beq      #0x34bb84
0034ba5c: mov      r1, #1
0034ba60: mov      r0, r6
0034ba64: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034ba68: ldr      r3, [r0]
0034ba6c: mov      lr, pc
0034ba70: ldr      pc, [r3, #0x20]
0034ba74: cmp      r0, #0
0034ba78: beq      #0x34badc
0034ba7c: mov      r0, r6
0034ba80: bl       #0x33fee4 ; _ZN12ObjectHandlecvP10GameObjectEv
0034ba84: ldr      r3, [sp, #0xc]
0034ba88: mov      r8, r0
0034ba8c: ldr      r0, [r0, #0x164]
0034ba90: ldr      r1, [r3, #4]
0034ba94: bl       #0x30eba4 ; 
0034ba98: ldr      ip, [sp, #0xc]
0034ba9c: mov      r7, r0
0034baa0: ldr      r0, [r8, #0x168]
0034baa4: ldr      r1, [ip, #8]
0034baa8: bl       #0x30eba4 ; 
0034baac: ldr      r3, [sp, #0xc]
0034bab0: mov      r6, r0
0034bab4: ldr      r0, [r8, #0x160]
0034bab8: ldr      r1, [r3]
0034babc: bl       #0x30eba4 ; 
0034bac0: add      r1, sp, #0x20
0034bac4: str      r0, [sp, #0x20]
0034bac8: mov      r2, #1
0034bacc: mov      r0, r8
0034bad0: str      r7, [sp, #0x24]
0034bad4: str      r6, [sp, #0x28]
0034bad8: bl       #0x393db4 ; _ZN10GameObject11SetPositionERK7Point3DIfEb
0034badc: ldr      r3, [r4, r5]
0034bae0: ldr      r2, [sp, #0x254]
0034bae4: ldr      r3, [r3]
0034bae8: cmp      r2, r3
0034baec: bne      #0x34bba8
0034baf0: add      sp, sp, #0x25c
0034baf4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034baf8: add      sl, sp, #0x13c
0034bafc: mov      r1, sb
0034bb00: mov      r0, sl
0034bb04: bl       #0x30eae4 ; 
0034bb08: ldr      r1, [pc, #0xbc]
0034bb0c: add      sb, sp, #0x3c
0034bb10: mov      r2, sl
0034bb14: add      r1, pc, r1
0034bb18: mov      r0, sb
0034bb1c: mov      r3, #0x53
0034bb20: bl       #0x30eae4 ; 
0034bb24: mvn      ip, #0
0034bb28: str      ip, [sp, #0x280]
0034bb2c: b        #0x34b938
0034bb30: ldr      r3, [pc, #0x98]
0034bb34: ldr      r3, [r4, r3]
0034bb38: ldr      r3, [r3]
0034bb3c: cmp      r3, #2
0034bb40: streq    r7, [r7]
0034bb44: beq      #0x34badc
0034bb48: cmp      r3, #1
0034bb4c: bne      #0x34badc
0034bb50: ldr      r0, [pc, #0x7c]
0034bb54: ldr      r1, [pc, #0x7c]
0034bb58: ldr      r2, [pc, #0x7c]
0034bb5c: ldr      r0, [r4, r0]
0034bb60: ldr      r3, [pc, #0x78]
0034bb64: mov      ip, #0x20c
0034bb68: add      r1, pc, r1
0034bb6c: add      r2, pc, r2
0034bb70: add      r3, pc, r3
0034bb74: add      r0, r0, #0xa8
0034bb78: str      ip, [sp]
0034bb7c: bl       #0x30e004 ; 
0034bb80: b        #0x34badc
0034bb84: mov      r0, r6
0034bb88: mov      r1, #1
0034bb8c: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034bb90: ldr      r3, [r0]
0034bb94: mov      lr, pc
0034bb98: ldr      pc, [r3, #0x1c]
0034bb9c: b        #0x34ba5c
0034bba0: bl       #0x310440 ; _Z10CustomFreePv
0034bba4: b        #0x34ba18
0034bba8: bl       #0x30e310 ; 
0034bbac: rsbeq    sb, r4, r8, lsl r2
0034bbb0: andeq    r4, r0, ip, lsr #1
0034bbb4: ldrsbeq  r4, [r7], #-0x84
0034bbb8: subseq   r5, sb, r0, lsr r8
0034bbbc: subseq   r4, r7, r0, asr #22
0034bbc0: subseq   r4, r7, ip, lsr #22
0034bbc4: ldrheq   r4, [r7], #-0xa0
0034bbc8: subseq   r4, r7, r4, lsr #20
0034bbcc: subseq   r4, r7, r4, asr #18
0034bbd0: andeq    r3, r0, r0, asr #19
0034bbd4: andeq    r1, r0, r0, asr #19
0034bbd8: subseq   r2, r7, r0, ror r8
0034bbdc: subseq   r4, r7, ip, asr #17
0034bbe0: subseq   r4, r7, r8, lsr #14

# _ZN13ObjectManager26SerializeGameObjectNetDataEiiR12NetBitStream
00346510: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00346514: ldr      r5, [pc, #0xb48]
00346518: sub      sp, sp, #0xbc
0034651c: str      r0, [sp, #0xc]
00346520: ldr      r0, [pc, #0xb40]
00346524: add      r5, pc, r5
00346528: mov      fp, r3
0034652c: ldr      r4, [r5, r0]
00346530: str      r1, [sp, #0x34]
00346534: str      r2, [sp, #0x20]
00346538: mov      r0, r4
0034653c: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
00346540: ldr      r3, [sp, #0xc]
00346544: ldrb     r1, [r0, #0x3c]
00346548: mov      r2, #8
0034654c: add      r3, r3, #0x164
00346550: mov      r0, fp
00346554: str      r3, [sp, #8]
00346558: bl       #0x80e4e8 ; _ZN12NetBitStream9WriteByteEhj
0034655c: ldr      r2, [sp, #0xc]
00346560: add      r7, sp, #0x34
00346564: add      ip, sp, #0x94
00346568: add      r2, r2, #0x17c
0034656c: mov      r1, r7
00346570: ldr      r0, [sp, #8]
00346574: str      ip, [sp, #0x18]
00346578: str      r2, [sp, #0x10]
0034657c: str      ip, [sp, #0x94]
00346580: str      ip, [sp, #0x98]
00346584: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346588: mov      r1, r7
0034658c: ldr      r0, [sp, #0x10]
00346590: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346594: ldr      r0, [r0, #0x10]
00346598: str      r0, [sp, #0x2c]
0034659c: ldr      r0, [r4, #0x40]
003465a0: bl       #0x36f074 ; _ZN13PlayerManager20IsLocalPlayerHostingEv
003465a4: cmp      r0, #0
003465a8: beq      #0x346768
003465ac: ldr      r1, [sp, #0xc]
003465b0: ldr      r3, [r1, #0x128]!
003465b4: cmp      r3, r1
003465b8: beq      #0x3465e4
003465bc: ldr      r0, [sp, #0x34]
003465c0: ldr      r2, [r3, #8]
003465c4: cmp      r2, r0
003465c8: beq      #0x3465e4
003465cc: ldr      r3, [r3]
003465d0: cmp      r1, r3
003465d4: beq      #0x346b20
003465d8: ldr      r2, [r3, #8]
003465dc: cmp      r2, r0
003465e0: bne      #0x3465cc
003465e4: cmp      r1, r3
003465e8: beq      #0x346768
003465ec: ldr      r3, [sp, #0xc]
003465f0: mov      r1, r7
003465f4: add      r0, r3, #0x98
003465f8: bl       #0x345a8c ; _ZNSt3mapIiSt4listIiSaIiEESt4lessIiESaISt4pairIKiS2_EEEixIiEERS2_RKT_
003465fc: mov      r4, r0
00346600: bl       #0x3441d8 ; 
00346604: ldr      ip, [sp, #0x20]
00346608: mov      r3, r0
0034660c: mov      r1, #1
00346610: str      ip, [r0, #8]
00346614: ldr      r2, [r4, #4]
00346618: str      r4, [r0]
0034661c: mov      r0, fp
00346620: str      r2, [r3, #4]
00346624: str      r3, [r2]
00346628: str      r3, [r4, #4]
0034662c: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
00346630: ldr      r1, [sp, #0xc]
00346634: ldr      ip, [r1, #0xb4]
00346638: add      r1, r1, #0xb0
0034663c: cmp      ip, #0
00346640: ldreq    lr, [sp, #0x34]
00346644: moveq    ip, r1
00346648: beq      #0x34667c
0034664c: ldr      lr, [sp, #0x34]
00346650: mov      r2, r1
00346654: b        #0x34665c
00346658: mov      ip, r3
0034665c: ldr      r3, [ip, #0x10]
00346660: cmp      r3, lr
00346664: ldrlt    r3, [ip, #0xc]
00346668: ldrge    r3, [ip, #8]
0034666c: movlt    ip, r2
00346670: mov      r2, ip
00346674: cmp      r3, #0
00346678: bne      #0x346658
0034667c: cmp      r1, ip
00346680: beq      #0x346694
00346684: ldr      r2, [ip, #0x10]
00346688: mov      r3, ip
0034668c: cmp      r2, lr
00346690: ble      #0x3466b8
00346694: add      r3, sp, #0x84
00346698: str      ip, [sp, #0xac]
0034669c: add      r0, sp, #0xb0
003466a0: mov      ip, #0
003466a4: add      r2, sp, #0xac
003466a8: str      lr, [sp, #0x84]
003466ac: strh     ip, [sp, #0x88]
003466b0: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
003466b4: ldr      r3, [sp, #0xb0]
003466b8: ldrb     r1, [r3, #0x14]
003466bc: mov      r0, fp
003466c0: mov      r2, #8
003466c4: bl       #0x80e4e8 ; _ZN12NetBitStream9WriteByteEhj
003466c8: ldr      r1, [sp, #0xc]
003466cc: ldr      ip, [r1, #0xe4]
003466d0: add      r1, r1, #0xe0
003466d4: cmp      ip, #0
003466d8: ldreq    lr, [sp, #0x34]
003466dc: moveq    ip, r1
003466e0: beq      #0x346714
003466e4: ldr      lr, [sp, #0x34]
003466e8: mov      r2, r1
003466ec: b        #0x3466f4
003466f0: mov      ip, r3
003466f4: ldr      r3, [ip, #0x10]
003466f8: cmp      lr, r3
003466fc: ldrgt    r3, [ip, #0xc]
00346700: ldrle    r3, [ip, #8]
00346704: movgt    ip, r2
00346708: mov      r2, ip
0034670c: cmp      r3, #0
00346710: bne      #0x3466f0
00346714: cmp      r1, ip
00346718: beq      #0x34672c
0034671c: ldr      r2, [ip, #0x10]
00346720: mov      r3, ip
00346724: cmp      r2, lr
00346728: ble      #0x346750
0034672c: add      r3, sp, #0x7c
00346730: str      ip, [sp, #0xa4]
00346734: add      r0, sp, #0xa8
00346738: mov      ip, #0
0034673c: add      r2, sp, #0xa4
00346740: str      lr, [sp, #0x7c]
00346744: strh     ip, [sp, #0x80]
00346748: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
0034674c: ldr      r3, [sp, #0xa8]
00346750: ldrh     r2, [r3, #0x14]
00346754: mov      r1, #1
00346758: str      r1, [sp, #0x1c]
0034675c: add      r2, r2, r1
00346760: strh     r2, [r3, #0x14]
00346764: b        #0x34677c
00346768: mov      r1, #0
0034676c: mov      r0, fp
00346770: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
00346774: mov      r1, #0
00346778: str      r1, [sp, #0x1c]
0034677c: ldr      r2, [sp, #0xc]
00346780: mov      r1, r7
00346784: add      r2, r2, #0x108
00346788: mov      r0, r2
0034678c: str      r2, [sp, #0x24]
00346790: bl       #0x3452ac ; _ZNSt3mapIsSt4listIP10ObjectBaseSaIS2_EESt4lessIsESaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346794: ldr      r2, [fp, #0x10]
00346798: mov      r4, r0
0034679c: ldr      r6, [r0]
003467a0: ands     r3, r2, #7
003467a4: lsr      r1, r2, #3
003467a8: movne    r3, #1
003467ac: rsb      r1, r1, #0x570
003467b0: rsb      r1, r3, r1
003467b4: cmp      r1, #0
003467b8: ble      #0x347030
003467bc: add      r8, sp, #0x3c
003467c0: mov      r0, r8
003467c4: bl       #0x80e908 ; _ZN12NetBitStreamC1Ej
003467c8: cmp      r6, r4
003467cc: bne      #0x346e04
003467d0: ldr      r3, [sp, #0x1c]
003467d4: cmp      r3, #0
003467d8: bne      #0x346e50
003467dc: ldr      sb, [sp, #0xc]
003467e0: ldr      sl, [sp, #0xc]
003467e4: ldr      r6, [sb, #0x100]!
003467e8: str      fp, [sp, #0x14]
003467ec: str      r8, [sp, #0x28]
003467f0: cmp      r6, sb
003467f4: ldr      fp, [sp, #8]
003467f8: beq      #0x3468a8
003467fc: mov      r1, r7
00346800: mov      r0, fp
00346804: ldr      r8, [r6, #8]
00346808: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
0034680c: ldr      r4, [r0, #4]
00346810: mov      r5, r0
00346814: ldr      r1, [r8, #0x108]
00346818: cmp      r4, #0
0034681c: beq      #0x346ad0
00346820: sxth     r1, r1
00346824: mov      r2, r0
00346828: b        #0x346830
0034682c: mov      r4, r3
00346830: ldrsh    r3, [r4, #0x10]
00346834: cmp      r3, r1
00346838: ldrlt    r3, [r4, #0xc]
0034683c: ldrge    r3, [r4, #8]
00346840: movlt    r4, r2
00346844: mov      r2, r4
00346848: cmp      r3, #0
0034684c: bne      #0x34682c
00346850: cmp      r5, r4
00346854: beq      #0x346864
00346858: ldrsh    r3, [r4, #0x10]
0034685c: cmp      r3, r1
00346860: bgt      #0x346ad0
00346864: mov      r1, r8
00346868: mov      r0, sl
0034686c: ldr      r2, [sp, #0x34]
00346870: bl       #0x3409f4 ; _ZN13ObjectManager20IsObjectSerializableEP10ObjectBasei
00346874: subs     r1, r0, #0
00346878: beq      #0x346aec
0034687c: ldr      r0, [sp, #0x18]
00346880: bl       #0x343168 ; 
00346884: str      r8, [r0, #8]
00346888: ldr      r3, [sp, #0x98]
0034688c: ldr      r2, [sp, #0x18]
00346890: stm      r0, {r2, r3}
00346894: str      r0, [r3]
00346898: str      r0, [sp, #0x98]
0034689c: ldr      r6, [r6]
003468a0: cmp      r6, sb
003468a4: bne      #0x3467fc
003468a8: ldr      r3, [sp, #0x18]
003468ac: ldr      r5, [sp, #0x94]
003468b0: ldr      fp, [sp, #0x14]
003468b4: ldr      r8, [sp, #0x28]
003468b8: cmp      r5, r3
003468bc: moveq    r0, r3
003468c0: beq      #0x3468dc
003468c4: ldr      r2, [sp, #0x18]
003468c8: mov      r3, r5
003468cc: ldr      r3, [r3]
003468d0: cmp      r3, r2
003468d4: bne      #0x3468cc
003468d8: ldr      r0, [sp, #0x18]
003468dc: add      sl, sp, #0x8c
003468e0: mov      sb, #0
003468e4: add      ip, sp, #0xb4
003468e8: str      fp, [sp, #0x28]
003468ec: str      sl, [sp, #0x8c]
003468f0: str      sl, [sp, #0x90]
003468f4: mov      r3, sb
003468f8: str      sb, [sp, #0x14]
003468fc: str      ip, [sp, #0x30]
00346900: mov      fp, r0
00346904: b        #0x34693c
00346908: mov      r0, sl
0034690c: str      r3, [sp, #4]
00346910: bl       #0x343168 ; 
00346914: str      r4, [r0, #8]
00346918: ldr      r2, [sp, #0x90]
0034691c: ldr      r3, [sp, #4]
00346920: str      sl, [r0]
00346924: str      r2, [r0, #4]
00346928: mov      r6, r3
0034692c: str      r0, [r2]
00346930: str      r0, [sp, #0x90]
00346934: ldr      r5, [r5]
00346938: mov      r3, r6
0034693c: cmp      r5, fp
00346940: beq      #0x346b78
00346944: cmp      sb, #0
00346948: ldr      r4, [r5, #8]
0034694c: bne      #0x346908
00346950: mov      r0, r8
00346954: str      r3, [sp, #4]
00346958: bl       #0x80e410 ; _ZN12NetBitStream14SetRevertPointEv
0034695c: ldr      r6, [r4, #0x108]
00346960: ldr      r3, [sp, #4]
00346964: rsb      r3, r3, r6
00346968: cmp      r3, #1
0034696c: beq      #0x346b68
00346970: mov      r0, r8
00346974: mov      r1, #1
00346978: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
0034697c: mov      r0, r8
00346980: mov      r1, r6
00346984: mov      r2, #0x10
00346988: bl       #0x80e5dc ; _ZN12NetBitStream8WriteU32Ejj
0034698c: mov      r2, #8
00346990: mov      r0, r8
00346994: ldrb     r1, [r4, #0xf8]
00346998: bl       #0x80e4e8 ; _ZN12NetBitStream9WriteByteEhj
0034699c: ldr      r0, [sp, #8]
003469a0: mov      r1, r7
003469a4: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
003469a8: ldr      r3, [r0, #4]
003469ac: cmp      r3, #0
003469b0: beq      #0x346b30
003469b4: mov      r1, r0
003469b8: sxth     ip, r6
003469bc: b        #0x3469c4
003469c0: mov      r3, r2
003469c4: ldrsh    r2, [r3, #0x10]
003469c8: cmp      r2, ip
003469cc: ldrlt    r2, [r3, #0xc]
003469d0: ldrge    r2, [r3, #8]
003469d4: movlt    r3, r1
003469d8: mov      r1, r3
003469dc: cmp      r2, #0
003469e0: bne      #0x3469c0
003469e4: cmp      r0, r3
003469e8: beq      #0x346a74
003469ec: ldrsh    r2, [r3, #0x10]
003469f0: cmp      r2, ip
003469f4: bgt      #0x346b30
003469f8: cmp      r0, r3
003469fc: beq      #0x346a74
00346a00: ldr      r0, [r4, #0x100]
00346a04: bl       #0x81347c ; _ZN9NetStruct6ResendEv
00346a08: mov      r1, r7
00346a0c: ldr      r0, [sp, #8]
00346a10: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346a14: ldr      r3, [r0, #4]
00346a18: uxth     ip, r6
00346a1c: cmp      r3, #0
00346a20: sxthne   ip, ip
00346a24: movne    r1, r0
00346a28: bne      #0x346a34
00346a2c: b        #0x346a74
00346a30: mov      r3, r2
00346a34: ldrsh    r2, [r3, #0x10]
00346a38: cmp      r2, ip
00346a3c: ldrlt    r2, [r3, #0xc]
00346a40: ldrge    r2, [r3, #8]
00346a44: movlt    r3, r1
00346a48: mov      r1, r3
00346a4c: cmp      r2, #0
00346a50: bne      #0x346a30
00346a54: cmp      r0, r3
00346a58: beq      #0x346a74
00346a5c: ldrsh    r2, [r3, #0x10]
00346a60: cmp      r2, ip
00346a64: bgt      #0x346a74
00346a68: ldr      r1, [sp, #0x30]
00346a6c: str      r3, [sp, #0xb4]
00346a70: bl       #0x346090 ; _ZNSt4priv8_Rb_treeIsSt4lessIsEsNS_9_IdentityIsEENS_11_SetTraitsTIsEESaIsEE5eraseENS_17_Rb_tree_iteratorIsS6_EE
00346a74: ldr      r3, [r4]
00346a78: mov      r0, r4
00346a7c: ldr      r1, [sp, #0x1c]
00346a80: mov      lr, pc
00346a84: ldr      pc, [r3, #0x4c]
00346a88: ldr      r0, [r4, #0x100]
00346a8c: mov      r1, #1
00346a90: bl       #0x81366c ; _ZN9NetStruct10SetEnabledEb
00346a94: ldr      r3, [r4, #0x100]
00346a98: mov      r1, r8
00346a9c: ldr      r2, [sp, #0x34]
00346aa0: mov      r0, r3
00346aa4: ldr      ip, [r3]
00346aa8: ldr      r3, [sp, #0x20]
00346aac: mov      lr, pc
00346ab0: ldr      pc, [ip, #8]
00346ab4: ldr      r3, [sp, #0x58]
00346ab8: cmp      r3, #0
00346abc: bne      #0x346b38
00346ac0: ldr      r1, [sp, #0x14]
00346ac4: add      r1, r1, #1
00346ac8: str      r1, [sp, #0x14]
00346acc: b        #0x346934
00346ad0: mov      r1, r8
00346ad4: mov      r0, sl
00346ad8: ldr      r2, [sp, #0x34]
00346adc: bl       #0x3409f4 ; _ZN13ObjectManager20IsObjectSerializableEP10ObjectBasei
00346ae0: subs     r1, r0, #0
00346ae4: mov      r4, r5
00346ae8: bne      #0x34687c
00346aec: cmp      r5, r4
00346af0: beq      #0x346b08
00346af4: ldr      r3, [r8, #0x100]
00346af8: cmp      r3, #0
00346afc: bne      #0x34687c
00346b00: ldr      r6, [r6]
00346b04: b        #0x3468a0
00346b08: ldr      r0, [r8, #0x100]
00346b0c: cmp      r0, #0
00346b10: beq      #0x34689c
00346b14: bl       #0x81366c ; _ZN9NetStruct10SetEnabledEb
00346b18: ldr      r6, [r6]
00346b1c: b        #0x3468a0
00346b20: mov      r3, r1
00346b24: cmp      r1, r3
00346b28: bne      #0x3465ec
00346b2c: b        #0x346768
00346b30: mov      r3, r0
00346b34: b        #0x3469f8
00346b38: mov      r0, r8
00346b3c: bl       #0x80e83c ; _ZN12NetBitStream6RevertEv
00346b40: mov      r0, sl
00346b44: bl       #0x343168 ; 
00346b48: str      r4, [r0, #8]
00346b4c: ldr      r3, [sp, #0x90]
00346b50: mov      sb, #1
00346b54: str      sl, [r0]
00346b58: str      r3, [r0, #4]
00346b5c: str      r0, [r3]
00346b60: str      r0, [sp, #0x90]
00346b64: b        #0x346934
00346b68: mov      r0, r8
00346b6c: mov      r1, sb
00346b70: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
00346b74: b        #0x34698c
00346b78: ldr      fp, [sp, #0x28]
00346b7c: mov      r2, #0x10
00346b80: ldr      r1, [sp, #0x14]
00346b84: mov      r0, fp
00346b88: bl       #0x80e5dc ; _ZN12NetBitStream8WriteU32Ejj
00346b8c: mov      r0, fp
00346b90: mov      r1, r8
00346b94: bl       #0x80ed54 ; _ZN12NetBitStream11WriteStreamERS_
00346b98: mov      r1, r7
00346b9c: ldr      r0, [sp, #0x24]
00346ba0: bl       #0x3452ac ; _ZNSt3mapIsSt4listIP10ObjectBaseSaIS2_EESt4lessIsESaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346ba4: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00346ba8: cmp      sb, #0
00346bac: bne      #0x346e24
00346bb0: ldr      r5, [sp, #0xc]
00346bb4: ldr      r0, [r5, #0x128]!
00346bb8: cmp      r5, r0
00346bbc: beq      #0x346be0
00346bc0: ldr      r3, [r0, #8]
00346bc4: ldr      r2, [sp, #0x34]
00346bc8: ldr      r4, [r0]
00346bcc: cmp      r2, r3
00346bd0: beq      #0x346d30
00346bd4: mov      r0, r4
00346bd8: cmp      r5, r0
00346bdc: bne      #0x346bc0
00346be0: ldr      r2, [sp, #0x1c]
00346be4: cmp      r2, #0
00346be8: beq      #0x346d80
00346bec: mov      r1, #1
00346bf0: mov      r0, fp
00346bf4: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
00346bf8: ldr      r3, [sp, #0xc]
00346bfc: ldr      ip, [r3, #0xe4]
00346c00: add      r1, r3, #0xe0
00346c04: cmp      ip, #0
00346c08: ldreq    lr, [sp, #0x34]
00346c0c: moveq    ip, r1
00346c10: beq      #0x346c44
00346c14: ldr      lr, [sp, #0x34]
00346c18: mov      r2, r1
00346c1c: b        #0x346c24
00346c20: mov      ip, r3
00346c24: ldr      r3, [ip, #0x10]
00346c28: cmp      r3, lr
00346c2c: ldrlt    r3, [ip, #0xc]
00346c30: ldrge    r3, [ip, #8]
00346c34: movlt    ip, r2
00346c38: mov      r2, ip
00346c3c: cmp      r3, #0
00346c40: bne      #0x346c20
00346c44: cmp      r1, ip
00346c48: beq      #0x347000
00346c4c: ldr      r2, [ip, #0x10]
00346c50: mov      r3, ip
00346c54: cmp      r2, lr
00346c58: bgt      #0x347000
00346c5c: ldrb     r1, [r3, #0x14]
00346c60: mov      r0, fp
00346c64: mov      r2, #8
00346c68: bl       #0x80e4e8 ; _ZN12NetBitStream9WriteByteEhj
00346c6c: ldr      r1, [sp, #0x2c]
00346c70: cmp      r1, #0
00346c74: ble      #0x346d98
00346c78: mov      r0, fp
00346c7c: mov      r1, #1
00346c80: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
00346c84: mov      r0, fp
00346c88: ldr      r1, [sp, #0x2c]
00346c8c: mov      r2, #0x10
00346c90: bl       #0x80e5dc ; _ZN12NetBitStream8WriteU32Ejj
00346c94: ldr      r0, [sp, #0x10]
00346c98: mov      r1, r7
00346c9c: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346ca0: ldr      r4, [r0, #8]
00346ca4: ldr      r0, [sp, #0x10]
00346ca8: mov      r1, r7
00346cac: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346cb0: cmp      r4, r0
00346cb4: beq      #0x346d00
00346cb8: mov      r2, #0x10
00346cbc: mov      r0, fp
00346cc0: ldrsh    r1, [r4, #0x10]
00346cc4: bl       #0x80e5dc ; _ZN12NetBitStream8WriteU32Ejj
00346cc8: ldr      r2, [r4, #0xc]
00346ccc: cmp      r2, #0
00346cd0: bne      #0x346cdc
00346cd4: b        #0x346d4c
00346cd8: mov      r2, r3
00346cdc: ldr      r3, [r2, #8]
00346ce0: cmp      r3, #0
00346ce4: bne      #0x346cd8
00346ce8: ldr      r0, [sp, #0x10]
00346cec: mov      r1, r7
00346cf0: mov      r4, r2
00346cf4: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346cf8: cmp      r4, r0
00346cfc: bne      #0x346cb8
00346d00: ldr      r2, [sp, #0xc]
00346d04: ldrb     r3, [r2, #0xfd]
00346d08: cmp      r3, #0
00346d0c: bne      #0x346db4
00346d10: mov      r0, sl
00346d14: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00346d18: mov      r0, r8
00346d1c: bl       #0x80e790 ; _ZN12NetBitStreamD1Ev
00346d20: ldr      r0, [sp, #0x18]
00346d24: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00346d28: add      sp, sp, #0xbc
00346d2c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00346d30: ldr      r3, [r0, #4]
00346d34: mov      r1, #0xc
00346d38: str      r4, [r3]
00346d3c: str      r3, [r4, #4]
00346d40: bl       #0x708f00 ; 
00346d44: mov      r0, r4
00346d48: b        #0x346bd8
00346d4c: ldr      r3, [r4, #4]
00346d50: ldr      r1, [r3, #0xc]
00346d54: cmp      r4, r1
00346d58: bne      #0x346d74
00346d5c: mov      r4, r3
00346d60: ldr      r3, [r3, #4]
00346d64: ldr      r2, [r3, #0xc]
00346d68: cmp      r4, r2
00346d6c: beq      #0x346d5c
00346d70: ldr      r2, [r4, #0xc]
00346d74: cmp      r3, r2
00346d78: movne    r4, r3
00346d7c: b        #0x346ca4
00346d80: ldr      r1, [sp, #0x1c]
00346d84: mov      r0, fp
00346d88: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
00346d8c: ldr      r1, [sp, #0x2c]
00346d90: cmp      r1, #0
00346d94: bgt      #0x346c78
00346d98: mov      r0, fp
00346d9c: mov      r1, #0
00346da0: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
00346da4: ldr      r2, [sp, #0xc]
00346da8: ldrb     r3, [r2, #0xfd]
00346dac: cmp      r3, #0
00346db0: beq      #0x346d10
00346db4: bl       #0x7fbd74 ; _Z16GetConnectionMgrv
00346db8: mov      r2, #0
00346dbc: mov      r1, r0
00346dc0: add      r0, sp, #0x5c
00346dc4: bl       #0x7fd284 ; _ZN18CConnectionManager19GetConnMemberIdListEb
00346dc8: ldr      r3, [sp, #0x60]
00346dcc: ldr      r2, [r3, #-4]
00346dd0: ldr      r3, [sp, #0x34]
00346dd4: cmp      r2, r3
00346dd8: beq      #0x34704c
00346ddc: ldr      r0, [sp, #0x5c]
00346de0: cmp      r0, #0
00346de4: beq      #0x346d10
00346de8: ldr      r1, [sp, #0x64]
00346dec: rsb      r1, r0, r1
00346df0: bic      r1, r1, #3
00346df4: cmp      r1, #0x80
00346df8: bhi      #0x347028
00346dfc: bl       #0x708f00 ; 
00346e00: b        #0x346d10
00346e04: mov      r1, r7
00346e08: ldr      r0, [sp, #0x24]
00346e0c: bl       #0x3452ac ; _ZNSt3mapIsSt4listIP10ObjectBaseSaIS2_EESt4lessIsESaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346e10: mov      r1, r7
00346e14: ldr      r5, [r0]
00346e18: ldr      r0, [sp, #0x24]
00346e1c: bl       #0x3452ac ; _ZNSt3mapIsSt4listIP10ObjectBaseSaIS2_EESt4lessIsESaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346e20: b        #0x3468dc
00346e24: mov      r1, r7
00346e28: ldr      r0, [sp, #0x24]
00346e2c: bl       #0x3452ac ; _ZNSt3mapIsSt4listIP10ObjectBaseSaIS2_EESt4lessIsESaISt4pairIKsS4_EEEixIiEERS4_RKT_
00346e30: mov      r1, sl
00346e34: bl       #0x346474 ; _ZNSt4listIP10ObjectBaseSaIS1_EEaSERKS3_
00346e38: mov      r0, sl
00346e3c: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00346e40: mov      r0, fp
00346e44: mov      r1, #0
00346e48: bl       #0x80e42c ; _ZN12NetBitStream8WriteBitEj
00346e4c: b        #0x346c6c
00346e50: bl       #0x800f8c ; _ZN9CMatching3GetEv
00346e54: ldr      r3, [r0]
00346e58: mov      lr, pc
00346e5c: ldr      pc, [r3, #0x6c]
00346e60: ldr      r3, [pc, #0x204]
00346e64: str      r0, [sp, #0x28]
00346e68: ldr      r5, [r5, r3]
00346e6c: ldr      r2, [r5, #0x1c]
00346e70: ldr      r3, [r5, #0x18]
00346e74: rsb      r3, r3, r2
00346e78: asr      r3, r3, #2
00346e7c: add      r6, r3, r3, lsl #2
00346e80: add      r6, r6, r6, lsl #4
00346e84: add      r6, r6, r6, lsl #8
00346e88: add      r6, r6, r6, lsl #16
00346e8c: add      r6, r3, r6, lsl #1
00346e90: cmp      r6, #0
00346e94: ble      #0x346f1c
00346e98: ldr      r3, [pc, #0x1d0]
00346e9c: mov      r4, #0
00346ea0: add      r3, pc, r3
00346ea4: str      r3, [sp, #0x14]
00346ea8: b        #0x346eb8
00346eac: add      r4, r4, #1
00346eb0: cmp      r4, r6
00346eb4: beq      #0x346f1c
00346eb8: mov      r1, r4
00346ebc: mov      r0, r5
00346ec0: bl       #0x455bec ; _ZNK13ScriptManager15IsScriptRunningEi
00346ec4: cmp      r0, #0
00346ec8: beq      #0x346eac
00346ecc: mov      r1, r4
00346ed0: mov      r0, r5
00346ed4: bl       #0x455c40 ; _ZNK13ScriptManager15GetScriptModuleEi
00346ed8: mov      sb, r0
00346edc: bl       #0x80b1bc ; _ZN10CMessaging3GetEv
00346ee0: mov      r1, #1
00346ee4: mov      sl, r0
00346ee8: ldr      r0, [sp, #0x14]
00346eec: bl       #0x80a244 ; _ZN8CMessage13CreateMessageEPKcb
00346ef0: mov      ip, #4
00346ef4: str      r4, [r0, #0x54]
00346ef8: str      sb, [r0, #0x58]
00346efc: str      ip, [r0, #0x50]
00346f00: mov      r1, r0
00346f04: ldr      r2, [sp, #0x34]
00346f08: mov      r0, sl
00346f0c: add      r4, r4, #1
00346f10: bl       #0x80e23c ; _ZN10CMessaging9SendMsgToEP8CMessagei
00346f14: cmp      r4, r6
00346f18: bne      #0x346eb8
00346f1c: ldr      sb, [sp, #0xc]
00346f20: add      r6, sp, #0x68
00346f24: mov      sl, r8
00346f28: ldr      r4, [sb, #0x100]!
00346f2c: mov      r0, r6
00346f30: cmp      sb, r4
00346f34: beq      #0x346fd0
00346f38: ldr      r5, [r4, #8]
00346f3c: subs     r1, r5, #0
00346f40: beq      #0x346fc0
00346f44: ldr      r3, [r5, #0x100]
00346f48: cmp      r3, #0
00346f4c: beq      #0x346fc0
00346f50: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00346f54: mov      r0, r6
00346f58: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00346f5c: subs     r8, r0, #0
00346f60: beq      #0x346fa0
00346f64: ldr      r3, [r8]
00346f68: mov      lr, pc
00346f6c: ldr      pc, [r3, #0x28]
00346f70: cmp      r0, #0
00346f74: ldr      r1, [sp, #0x28]
00346f78: mov      r2, r8
00346f7c: ldr      r0, [sp, #0xc]
00346f80: beq      #0x346f90
00346f84: bl       #0x340948 ; _ZN13ObjectManager22IsRemotePlayerOfMemberEiP9Character
00346f88: cmp      r0, #0
00346f8c: beq      #0x346fc0
00346f90: mov      r0, r8
00346f94: bl       #0x3a30c4 ; _ZNK9Character10IsMerchantEv
00346f98: cmp      r0, #0
00346f9c: bne      #0x346fc0
00346fa0: ldr      r0, [sp, #0x18]
00346fa4: bl       #0x343168 ; 
00346fa8: str      r5, [r0, #8]
00346fac: ldr      r3, [sp, #0x98]
00346fb0: ldr      r2, [sp, #0x18]
00346fb4: stm      r0, {r2, r3}
00346fb8: str      r0, [r3]
00346fbc: str      r0, [sp, #0x98]
00346fc0: ldr      r4, [r4]
00346fc4: mov      r0, r6
00346fc8: cmp      sb, r4
00346fcc: bne      #0x346f38
00346fd0: ldr      r1, [sp, #0x18]
00346fd4: ldr      r5, [sp, #0x94]
00346fd8: mov      r8, sl
00346fdc: cmp      r5, r1
00346fe0: moveq    r0, r1
00346fe4: beq      #0x3468dc
00346fe8: ldr      r2, [sp, #0x18]
00346fec: mov      r3, r5
00346ff0: ldr      r3, [r3]
00346ff4: cmp      r3, r2
00346ff8: bne      #0x346ff0
00346ffc: b        #0x3468d8
00347000: add      r3, sp, #0x74
00347004: str      ip, [sp, #0x9c]
00347008: add      r0, sp, #0xa0
0034700c: mov      ip, #0
00347010: add      r2, sp, #0x9c
00347014: str      lr, [sp, #0x74]
00347018: strh     ip, [sp, #0x78]
0034701c: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
00347020: ldr      r3, [sp, #0xa0]
00347024: b        #0x346c5c
00347028: bl       #0x310440 ; _Z10CustomFreePv
0034702c: b        #0x346d10
00347030: mov      r0, fp
00347034: mov      r1, #0
00347038: mov      r2, #0x10
0034703c: bl       #0x80e5dc ; _ZN12NetBitStream8WriteU32Ejj
00347040: ldr      r0, [sp, #0x18]
00347044: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00347048: b        #0x346d28
0034704c: ldr      r0, [sp, #0xc]
00347050: bl       #0x340250 ; _ZN13ObjectManager18DoOnlineStateFlushEv
00347054: ldr      ip, [sp, #0xc]
00347058: mov      r3, #0
0034705c: strb     r3, [ip, #0xfd]
00347060: b        #0x346ddc
00347064: rsbeq    lr, r4, ip, ror #10
00347068: strdeq   r3, r4, [r0], -r4
0034706c: andeq    r1, r0, r0, lsr #20
00347070: subseq   r8, r7, r0, lsr #32

# _ZN8RoomZone14InitObjectListEv
00396c44: push     {r4, r5, r6, r7, lr}
00396c48: ldrb     r2, [r0, #0x390]
00396c4c: ldr      r3, [pc, #0xc8]
00396c50: sub      sp, sp, #0x14
00396c54: cmp      r2, #0
00396c58: mov      r6, r0
00396c5c: add      r3, pc, r3
00396c60: bne      #0x396ce0
00396c64: ldr      r2, [pc, #0xb4]
00396c68: add      r5, sp, #4
00396c6c: ldr      r3, [r3, r2]
00396c70: ldr      r3, [r3, #0x38]
00396c74: ldr      r4, [r3, #0x14]
00396c78: add      r7, r3, #0xc
00396c7c: cmp      r7, r4
00396c80: beq      #0x396cd8
00396c84: ldr      r1, [r4, #0x2c]
00396c88: cmp      r1, #0
00396c8c: beq      #0x396ca4
00396c90: mov      r0, r5
00396c94: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00396c98: mov      r0, r5
00396c9c: bl       #0x33fee4 ; _ZN12ObjectHandlecvP10GameObjectEv
00396ca0: mov      r1, r0
00396ca4: mov      r0, r6
00396ca8: bl       #0x396a90 ; _ZN8RoomZone16AddInitialObjectEP10GameObject
00396cac: ldr      r2, [r4, #0xc]
00396cb0: cmp      r2, #0
00396cb4: bne      #0x396cc0
00396cb8: b        #0x396ce8
00396cbc: mov      r2, r3
00396cc0: ldr      r3, [r2, #8]
00396cc4: cmp      r3, #0
00396cc8: bne      #0x396cbc
00396ccc: mov      r4, r2
00396cd0: cmp      r7, r4
00396cd4: bne      #0x396c84
00396cd8: mov      r3, #1
00396cdc: strb     r3, [r6, #0x390]
00396ce0: add      sp, sp, #0x14
00396ce4: pop      {r4, r5, r6, r7, pc}
00396ce8: ldr      r3, [r4, #4]
00396cec: ldr      r1, [r3, #0xc]
00396cf0: cmp      r4, r1
00396cf4: bne      #0x396d10
00396cf8: mov      r4, r3
00396cfc: ldr      r3, [r3, #4]
00396d00: ldr      r2, [r3, #0xc]
00396d04: cmp      r2, r4
00396d08: beq      #0x396cf8
00396d0c: ldr      r2, [r4, #0xc]
00396d10: cmp      r3, r2
00396d14: movne    r4, r3
00396d18: b        #0x396c7c
00396d1c: subseq   sp, pc, r4, lsr lr
00396d20: strdeq   r3, r4, [r0], -r4

# _ZN10GameObject10SetVisibleEb
0038b0f0: cmp      r1, #0
0038b0f4: ldr      r3, [r0, #0x2d8]
0038b0f8: ldrbne   r1, [r0, #0x8a]
0038b0fc: cmp      r3, #0
0038b100: strb     r1, [r0, #0x80]
0038b104: bxeq     lr
0038b108: mov      r0, r3
0038b10c: b        #0x4713d0

# _ZN13ObjectManager15AddNoRoomObjectEP10GameObject
00344184: push     {r4, r5, lr}
00344188: ldrb     r3, [r1, #0x2f8]
0034418c: sub      sp, sp, #0xc
00344190: mov      r4, r1
00344194: cmp      r3, #0
00344198: mov      r5, r0
0034419c: bne      #0x3441d0
003441a0: mov      r3, #0xc
003441a4: add      r0, sp, #8
003441a8: str      r3, [r0, #-4]!
003441ac: bl       #0x708ec0 ; 
003441b0: str      r4, [r0, #8]
003441b4: ldr      r3, [r5, #0x8c]
003441b8: add      r2, r5, #0x88
003441bc: stm      r0, {r2, r3}
003441c0: str      r0, [r3]
003441c4: mov      r3, #1
003441c8: str      r0, [r5, #0x8c]
003441cc: strb     r3, [r4, #0x2f8]
003441d0: add      sp, sp, #0xc
003441d4: pop      {r4, r5, pc}

# _ZN13ObjectManager15ForceFullUpdateEv
00347fd0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00347fd4: sub      sp, sp, #0xa4
00347fd8: mov      r5, r0
00347fdc: bl       #0x7fd794 ; _Z9GetOnlinev
00347fe0: ldrb     r3, [r0, #5]
00347fe4: ldr      r6, [pc, #0x650]
00347fe8: cmp      r3, #0
00347fec: add      r6, pc, r6
00347ff0: beq      #0x3482f0
00347ff4: ldr      r3, [r5, #0x118]
00347ff8: cmp      r3, #0
00347ffc: bne      #0x3483a8
00348000: bl       #0x320e98 ; _ZN15OnlineSingletonI15OnlineGameStateE11GetInstanceEv
00348004: ldr      r3, [r0, #0x34]
00348008: sub      r3, r3, #3
0034800c: cmp      r3, #1
00348010: bls      #0x3483e0
00348014: bl       #0x800f8c ; _ZN9CMatching3GetEv
00348018: mov      r1, r0
0034801c: ldr      r3, [r0]
00348020: add      r0, sp, #0x20
00348024: mov      lr, pc
00348028: ldr      pc, [r3, #0x88]
0034802c: ldr      r4, [sp, #0x20]
00348030: ldr      r0, [sp, #0x24]
00348034: cmp      r4, r0
00348038: beq      #0x3482d0
0034803c: ldr      r3, [pc, #0x5fc]
00348040: add      ip, sp, #0x64
00348044: add      r2, sp, #0x2c
00348048: ldr      fp, [r6, r3]
0034804c: add      r3, sp, #0x68
00348050: stm      sp, {r3, ip}
00348054: add      r3, sp, #0x70
00348058: add      ip, sp, #0x6c
0034805c: str      r2, [sp, #8]
00348060: str      r3, [sp, #0xc]
00348064: str      ip, [sp, #0x10]
00348068: add      r2, sp, #0x34
0034806c: add      r3, sp, #0x80
00348070: add      ip, sp, #0x7c
00348074: add      r4, r4, #4
00348078: add      sb, r5, #0x128
0034807c: add      r7, r5, #0xb0
00348080: add      r8, r5, #0xe0
00348084: add      sl, r5, #0xc8
00348088: str      r2, [sp, #0x14]
0034808c: str      r3, [sp, #0x18]
00348090: str      ip, [sp, #0x1c]
00348094: mov      r6, r5
00348098: ldr      r0, [fp, #0x40]
0034809c: bl       #0x36e09c ; _ZN13PlayerManager16GetHostingPlayerEv
003480a0: ldr      r2, [r4, #-4]
003480a4: ldr      r3, [r0, #0x1a0]
003480a8: cmp      r2, r3
003480ac: beq      #0x3482b8
003480b0: mov      r0, sb
003480b4: bl       #0x3441d8 ; 
003480b8: ldr      r3, [r4, #-4]
003480bc: str      r3, [r0, #8]
003480c0: ldr      r3, [r6, #0x12c]
003480c4: str      sb, [r0]
003480c8: str      r3, [r0, #4]
003480cc: str      r0, [r3]
003480d0: ldr      ip, [r6, #0xb4]
003480d4: str      r0, [r6, #0x12c]
003480d8: cmp      ip, #0
003480dc: beq      #0x348390
003480e0: ldr      r5, [r4, #-4]
003480e4: mov      r1, r7
003480e8: mov      r3, ip
003480ec: b        #0x3480f4
003480f0: mov      r3, r2
003480f4: ldr      r2, [r3, #0x10]
003480f8: cmp      r2, r5
003480fc: ldrlt    r2, [r3, #0xc]
00348100: ldrge    r2, [r3, #8]
00348104: movlt    r3, r1
00348108: mov      r1, r3
0034810c: cmp      r2, #0
00348110: bne      #0x3480f0
00348114: cmp      r7, r3
00348118: beq      #0x3482f8
0034811c: ldr      r2, [r3, #0x10]
00348120: cmp      r2, r5
00348124: movgt    r3, r7
00348128: cmp      r7, r3
0034812c: beq      #0x3482f8
00348130: cmp      ip, #0
00348134: movne    r2, r7
00348138: bne      #0x348144
0034813c: b        #0x348600
00348140: mov      ip, r3
00348144: ldr      r3, [ip, #0x10]
00348148: cmp      r5, r3
0034814c: ldrgt    r3, [ip, #0xc]
00348150: ldrle    r3, [ip, #8]
00348154: movgt    ip, r2
00348158: mov      r2, ip
0034815c: cmp      r3, #0
00348160: bne      #0x348140
00348164: cmp      r7, ip
00348168: beq      #0x34817c
0034816c: ldr      r2, [ip, #0x10]
00348170: mov      r3, ip
00348174: cmp      r5, r2
00348178: bge      #0x3481a4
0034817c: add      r3, sp, #0x3c
00348180: str      ip, [sp, #0x74]
00348184: add      r0, sp, #0x78
00348188: mov      ip, #0
0034818c: mov      r1, r7
00348190: add      r2, sp, #0x74
00348194: str      r5, [sp, #0x3c]
00348198: strh     ip, [sp, #0x40]
0034819c: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
003481a0: ldr      r3, [sp, #0x78]
003481a4: ldrh     r2, [r3, #0x14]
003481a8: add      r2, r2, #1
003481ac: strh     r2, [r3, #0x14]
003481b0: ldr      ip, [r6, #0xe4]
003481b4: cmp      ip, #0
003481b8: beq      #0x348384
003481bc: ldr      r5, [r4, #-4]
003481c0: mov      r2, r8
003481c4: b        #0x3481cc
003481c8: mov      ip, r3
003481cc: ldr      r3, [ip, #0x10]
003481d0: cmp      r5, r3
003481d4: ldrgt    r3, [ip, #0xc]
003481d8: ldrle    r3, [ip, #8]
003481dc: movgt    ip, r2
003481e0: mov      r2, ip
003481e4: cmp      r3, #0
003481e8: bne      #0x3481c8
003481ec: cmp      r8, ip
003481f0: beq      #0x348204
003481f4: ldr      r2, [ip, #0x10]
003481f8: mov      r3, ip
003481fc: cmp      r2, r5
00348200: ble      #0x34822c
00348204: ldr      r3, [sp, #0x14]
00348208: str      ip, [sp, #0x6c]
0034820c: ldr      r0, [sp, #0xc]
00348210: mov      ip, #0
00348214: mov      r1, r8
00348218: ldr      r2, [sp, #0x10]
0034821c: str      r5, [sp, #0x34]
00348220: strh     ip, [sp, #0x38]
00348224: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
00348228: ldr      r3, [sp, #0x70]
0034822c: mov      r2, #0
00348230: strh     r2, [r3, #0x14]
00348234: ldr      ip, [r6, #0xcc]
00348238: cmp      ip, #0
0034823c: beq      #0x34839c
00348240: ldr      r5, [r4, #-4]
00348244: mov      r2, sl
00348248: b        #0x348250
0034824c: mov      ip, r3
00348250: ldr      r3, [ip, #0x10]
00348254: cmp      r3, r5
00348258: ldrlt    r3, [ip, #0xc]
0034825c: ldrge    r3, [ip, #8]
00348260: movlt    ip, r2
00348264: mov      r2, ip
00348268: cmp      r3, #0
0034826c: bne      #0x34824c
00348270: cmp      sl, ip
00348274: beq      #0x348288
00348278: ldr      r2, [ip, #0x10]
0034827c: mov      r3, ip
00348280: cmp      r5, r2
00348284: bge      #0x3482b0
00348288: ldr      r3, [sp, #8]
0034828c: str      ip, [sp, #0x64]
00348290: ldr      r0, [sp]
00348294: mov      ip, #0
00348298: mov      r1, sl
0034829c: ldr      r2, [sp, #4]
003482a0: str      r5, [sp, #0x2c]
003482a4: strh     ip, [sp, #0x30]
003482a8: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
003482ac: ldr      r3, [sp, #0x68]
003482b0: mvn      r2, #0
003482b4: strh     r2, [r3, #0x14]
003482b8: ldr      r3, [sp, #0x24]
003482bc: mov      r2, r4
003482c0: add      r4, r4, #4
003482c4: cmp      r2, r3
003482c8: bne      #0x348098
003482cc: ldr      r0, [sp, #0x20]
003482d0: cmp      r0, #0
003482d4: beq      #0x3482f0
003482d8: ldr      r1, [sp, #0x28]
003482dc: rsb      r1, r0, r1
003482e0: bic      r1, r1, #3
003482e4: cmp      r1, #0x80
003482e8: bhi      #0x348608
003482ec: bl       #0x708f00 ; 
003482f0: add      sp, sp, #0xa4
003482f4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003482f8: cmp      ip, #0
003482fc: moveq    ip, r7
00348300: beq      #0x348330
00348304: mov      r2, r7
00348308: b        #0x348310
0034830c: mov      ip, r3
00348310: ldr      r3, [ip, #0x10]
00348314: cmp      r5, r3
00348318: ldrgt    r3, [ip, #0xc]
0034831c: ldrle    r3, [ip, #8]
00348320: movgt    ip, r2
00348324: mov      r2, ip
00348328: cmp      r3, #0
0034832c: bne      #0x34830c
00348330: cmp      r7, ip
00348334: beq      #0x348348
00348338: ldr      r2, [ip, #0x10]
0034833c: mov      r3, ip
00348340: cmp      r5, r2
00348344: bge      #0x348370
00348348: add      r3, sp, #0x44
0034834c: str      ip, [sp, #0x7c]
00348350: ldr      r0, [sp, #0x18]
00348354: mov      ip, #0
00348358: mov      r1, r7
0034835c: ldr      r2, [sp, #0x1c]
00348360: str      r5, [sp, #0x44]
00348364: strh     ip, [sp, #0x48]
00348368: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
0034836c: ldr      r3, [sp, #0x80]
00348370: mov      r2, #0
00348374: strh     r2, [r3, #0x14]
00348378: ldr      ip, [r6, #0xe4]
0034837c: cmp      ip, #0
00348380: bne      #0x3481bc
00348384: ldr      r5, [r4, #-4]
00348388: mov      ip, r8
0034838c: b        #0x3481ec
00348390: ldr      r5, [r4, #-4]
00348394: mov      r3, r7
00348398: b        #0x348128
0034839c: ldr      r5, [r4, #-4]
003483a0: mov      ip, sl
003483a4: b        #0x348270
003483a8: add      r4, r5, #0x108
003483ac: mov      r0, r4
003483b0: ldr      r1, [r5, #0x10c]
003483b4: bl       #0x3458d4 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt4listIP10ObjectBaseSaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
003483b8: mov      r3, #0
003483bc: str      r3, [r5, #0x118]
003483c0: str      r3, [r5, #0x10c]
003483c4: str      r4, [r5, #0x114]
003483c8: str      r4, [r5, #0x110]
003483cc: bl       #0x320e98 ; _ZN15OnlineSingletonI15OnlineGameStateE11GetInstanceEv
003483d0: ldr      r3, [r0, #0x34]
003483d4: sub      r3, r3, #3
003483d8: cmp      r3, #1
003483dc: bhi      #0x348014
003483e0: add      r4, r5, #0x128
003483e4: mov      r0, r4
003483e8: bl       #0x3441d8 ; 
003483ec: mov      r3, #1
003483f0: str      r3, [r0, #8]
003483f4: ldr      r3, [r5, #0x12c]
003483f8: str      r4, [r0]
003483fc: add      r1, r5, #0xb0
00348400: str      r3, [r0, #4]
00348404: str      r0, [r3]
00348408: ldr      ip, [r5, #0xb4]
0034840c: str      r0, [r5, #0x12c]
00348410: cmp      ip, #0
00348414: beq      #0x3485f8
00348418: mov      r4, r1
0034841c: mov      r3, ip
00348420: b        #0x348428
00348424: mov      r3, r2
00348428: ldr      r2, [r3, #0x10]
0034842c: cmp      r2, #0
00348430: ldrle    r2, [r3, #0xc]
00348434: ldrgt    r2, [r3, #8]
00348438: movle    r3, r4
0034843c: mov      r4, r3
00348440: cmp      r2, #0
00348444: bne      #0x348424
00348448: cmp      r1, r3
0034844c: beq      #0x348610
00348450: ldr      r2, [r3, #0x10]
00348454: cmp      r2, #1
00348458: bgt      #0x3485f8
0034845c: cmp      r1, r3
00348460: beq      #0x348610
00348464: cmp      ip, #0
00348468: movne    r2, r1
0034846c: bne      #0x348478
00348470: b        #0x348634
00348474: mov      ip, r3
00348478: ldr      r3, [ip, #0x10]
0034847c: cmp      r3, #0
00348480: ldrle    r3, [ip, #0xc]
00348484: ldrgt    r3, [ip, #8]
00348488: movle    ip, r2
0034848c: mov      r2, ip
00348490: cmp      r3, #0
00348494: bne      #0x348474
00348498: cmp      r1, ip
0034849c: beq      #0x3484b0
003484a0: ldr      r2, [ip, #0x10]
003484a4: mov      r3, ip
003484a8: cmp      r2, #1
003484ac: ble      #0x3484d8
003484b0: add      r3, sp, #0x5c
003484b4: mov      lr, #1
003484b8: str      ip, [sp, #0x94]
003484bc: add      r0, sp, #0x98
003484c0: mov      ip, #0
003484c4: add      r2, sp, #0x94
003484c8: str      lr, [sp, #0x5c]
003484cc: strh     ip, [sp, #0x60]
003484d0: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
003484d4: ldr      r3, [sp, #0x98]
003484d8: ldrh     r2, [r3, #0x14]
003484dc: add      r2, r2, #1
003484e0: strh     r2, [r3, #0x14]
003484e4: ldr      ip, [r5, #0xe4]
003484e8: add      r1, r5, #0xe0
003484ec: cmp      ip, #0
003484f0: moveq    ip, r1
003484f4: beq      #0x348524
003484f8: mov      r2, r1
003484fc: b        #0x348504
00348500: mov      ip, r3
00348504: ldr      r3, [ip, #0x10]
00348508: cmp      r3, #0
0034850c: ldrle    r3, [ip, #0xc]
00348510: ldrgt    r3, [ip, #8]
00348514: movle    ip, r2
00348518: mov      r2, ip
0034851c: cmp      r3, #0
00348520: bne      #0x348500
00348524: cmp      r1, ip
00348528: beq      #0x34853c
0034852c: ldr      r2, [ip, #0x10]
00348530: mov      r3, ip
00348534: cmp      r2, #1
00348538: ble      #0x348564
0034853c: add      r3, sp, #0x54
00348540: mov      lr, #1
00348544: str      ip, [sp, #0x8c]
00348548: add      r0, sp, #0x90
0034854c: mov      ip, #0
00348550: add      r2, sp, #0x8c
00348554: str      lr, [sp, #0x54]
00348558: strh     ip, [sp, #0x58]
0034855c: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
00348560: ldr      r3, [sp, #0x90]
00348564: mov      r2, #0
00348568: strh     r2, [r3, #0x14]
0034856c: ldr      ip, [r5, #0xcc]
00348570: add      r1, r5, #0xc8
00348574: cmp      ip, #0
00348578: moveq    ip, r1
0034857c: beq      #0x3485ac
00348580: mov      r2, r1
00348584: b        #0x34858c
00348588: mov      ip, r3
0034858c: ldr      r3, [ip, #0x10]
00348590: cmp      r3, #0
00348594: ldrle    r3, [ip, #0xc]
00348598: ldrgt    r3, [ip, #8]
0034859c: movle    ip, r2
003485a0: mov      r2, ip
003485a4: cmp      r3, #0
003485a8: bne      #0x348588
003485ac: cmp      r1, ip
003485b0: beq      #0x3485c4
003485b4: ldr      r2, [ip, #0x10]
003485b8: mov      r3, ip
003485bc: cmp      r2, #1
003485c0: ble      #0x3485ec
003485c4: add      r3, sp, #0x4c
003485c8: mov      lr, #1
003485cc: str      ip, [sp, #0x84]
003485d0: add      r0, sp, #0x88
003485d4: mov      ip, #0
003485d8: add      r2, sp, #0x84
003485dc: str      lr, [sp, #0x4c]
003485e0: strh     ip, [sp, #0x50]
003485e4: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
003485e8: ldr      r3, [sp, #0x88]
003485ec: mvn      r2, #0
003485f0: strh     r2, [r3, #0x14]
003485f4: b        #0x3482f0
003485f8: mov      r3, r1
003485fc: b        #0x34845c
00348600: mov      ip, r7
00348604: b        #0x348164
00348608: bl       #0x310440 ; _Z10CustomFreePv
0034860c: b        #0x3482f0
00348610: add      r3, sp, #0xa0
00348614: mov      r2, #1
00348618: str      r2, [r3, #-4]!
0034861c: mov      r0, r1
00348620: mov      r1, r3
00348624: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00348628: mov      r2, #0
0034862c: strh     r2, [r0]
00348630: b        #0x3484e4
00348634: mov      ip, r1
00348638: b        #0x348498
0034863c: rsbeq    ip, r4, r4, lsr #21
00348640: strdeq   r3, r4, [r0], -r4

# _ZNK8RoomZone4DrawEv
00396544: bx       lr

# _ZNSt4priv10_List_baseIP8RoomZoneSaIS2_EE5clearEv
004542e8: push     {r4, r5, r6, lr}
004542ec: mov      r5, r0
004542f0: ldr      r0, [r0]
004542f4: cmp      r0, r5
004542f8: bne      #0x454304
004542fc: b        #0x45431c
00454300: mov      r0, r4
00454304: ldr      r4, [r0]
00454308: mov      r1, #0xc
0045430c: bl       #0x708f00 ; 
00454310: cmp      r4, r5
00454314: bne      #0x454300
00454318: mov      r0, r5
0045431c: str      r0, [r5, #4]
00454320: str      r0, [r5]
00454324: pop      {r4, r5, r6, pc}

# _ZThn16_N17v2MixedController11Ctrl_LookAtERK7Point3DIfE
00408c48: sub      r0, r0, #0x10
00408c4c: b        #0x408c50

# _ZN10GameObjectC2EN10ObjectBase6GO_IDSE
0038c398: push     {r4, r5, r6, r7, lr}
0038c39c: ldr      r6, [pc, #0x254]
0038c3a0: sub      sp, sp, #0xc
0038c3a4: mov      r4, r0
0038c3a8: bl       #0x33f310 ; _ZN10ObjectBaseC2ENS_6GO_IDSE
0038c3ac: ldr      r2, [pc, #0x248]
0038c3b0: add      r6, pc, r6
0038c3b4: mov      r3, #0
0038c3b8: ldr      r2, [r6, r2]
0038c3bc: mov      r5, #0
0038c3c0: str      r3, [r4, #0x120]
0038c3c4: add      r1, r2, #0xe4
0038c3c8: add      r0, r2, #8
0038c3cc: add      r2, r2, #0xd8
0038c3d0: str      r2, [r4, #4]
0038c3d4: str      r1, [r4, #0x24]
0038c3d8: str      r0, [r4]
0038c3dc: str      r3, [r4, #0x124]
0038c3e0: str      r3, [r4, #0x128]
0038c3e4: str      r3, [r4, #0x12c]
0038c3e8: str      r3, [r4, #0x130]
0038c3ec: str      r3, [r4, #0x134]
0038c3f0: str      r3, [r4, #0x138]
0038c3f4: str      r3, [r4, #0x13c]
0038c3f8: str      r3, [r4, #0x140]
0038c3fc: str      r3, [r4, #0x144]
0038c400: str      r3, [r4, #0x148]
0038c404: str      r3, [r4, #0x14c]
0038c408: str      r3, [r4, #0x150]
0038c40c: str      r3, [r4, #0x154]
0038c410: str      r3, [r4, #0x158]
0038c414: str      r3, [r4, #0x160]
0038c418: str      r3, [r4, #0x164]
0038c41c: str      r3, [r4, #0x168]
0038c420: str      r3, [r4, #0x16c]
0038c424: str      r3, [r4, #0x170]
0038c428: str      r3, [r4, #0x174]
0038c42c: str      r3, [r4, #0x178]
0038c430: str      r3, [r4, #0x184]
0038c434: str      r3, [r4, #0x188]
0038c438: str      r3, [r4, #0x18c]
0038c43c: str      r3, [r4, #0x190]
0038c440: str      r3, [r4, #0x194]
0038c444: strb     r5, [r4, #0x15c]
0038c448: str      r5, [r4, #0x180]
0038c44c: add      r0, r4, #0x1c8
0038c450: str      r3, [r4, #0x198]
0038c454: str      r3, [r4, #0x1c0]
0038c458: str      r3, [r4, #0x19c]
0038c45c: str      r3, [r4, #0x1a0]
0038c460: str      r3, [r4, #0x1a4]
0038c464: str      r3, [r4, #0x1a8]
0038c468: str      r3, [r4, #0x1ac]
0038c46c: str      r3, [r4, #0x1b0]
0038c470: strb     r5, [r4, #0x1b4]
0038c474: strb     r5, [r4, #0x1b5]
0038c478: str      r3, [r4, #0x1b8]
0038c47c: str      r3, [r4, #0x1bc]
0038c480: strb     r5, [r4, #0x1c4]
0038c484: bl       #0x524644 ; _ZN8PFObjectC1Ev
0038c488: add      r3, r4, #0x278
0038c48c: mvn      r7, #0
0038c490: mov      r2, #0x64
0038c494: str      r2, [r4, #0x274]
0038c498: mov      r0, r3
0038c49c: str      r3, [r4, #0x288]
0038c4a0: str      r3, [r4, #0x28c]
0038c4a4: str      r5, [r4, #0x26c]
0038c4a8: str      r7, [r4, #0x270]
0038c4ac: mov      r1, #0x10
0038c4b0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c4b4: ldr      r2, [r4, #0x288]
0038c4b8: add      r3, r4, #0x290
0038c4bc: mov      r0, r3
0038c4c0: strb     r5, [r2]
0038c4c4: mov      r1, #0x10
0038c4c8: str      r3, [r4, #0x2a0]
0038c4cc: str      r3, [r4, #0x2a4]
0038c4d0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c4d4: ldr      r2, [r4, #0x2a0]
0038c4d8: add      r3, r4, #0x2a8
0038c4dc: mov      r0, r3
0038c4e0: strb     r5, [r2]
0038c4e4: mov      r1, #0x10
0038c4e8: str      r3, [r4, #0x2b8]
0038c4ec: str      r3, [r4, #0x2bc]
0038c4f0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c4f4: ldr      r2, [r4, #0x2b8]
0038c4f8: add      r3, r4, #0x2c0
0038c4fc: mov      r0, r3
0038c500: strb     r5, [r2]
0038c504: mov      r1, #0x10
0038c508: str      r3, [r4, #0x2d0]
0038c50c: str      r3, [r4, #0x2d4]
0038c510: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c514: ldr      r3, [r4, #0x2d0]
0038c518: mov      ip, #1
0038c51c: add      r6, r4, #0x304
0038c520: strb     r5, [r3]
0038c524: mov      r2, ip
0038c528: strb     ip, [r4, #0x2ee]
0038c52c: strb     ip, [r4, #0x2fb]
0038c530: str      r5, [r4, #0x2d8]
0038c534: str      r5, [r4, #0x2dc]
0038c538: str      r5, [r4, #0x2e0]
0038c53c: str      r5, [r4, #0x2e4]
0038c540: str      r5, [r4, #0x2e8]
0038c544: strb     r5, [r4, #0x2ec]
0038c548: strb     r5, [r4, #0x2ed]
0038c54c: strb     r5, [r4, #0x2ef]
0038c550: strb     r5, [r4, #0x2f0]
0038c554: str      r5, [r4, #0x2f4]
0038c558: strb     r5, [r4, #0x2f8]
0038c55c: strb     r5, [r4, #0x2f9]
0038c560: strb     r5, [r4, #0x2fa]
0038c564: strb     r5, [r4, #0x2fc]
0038c568: str      r5, [r4, #0x300]
0038c56c: mov      r3, r5
0038c570: mov      r1, r5
0038c574: mov      r0, r6
0038c578: str      ip, [sp]
0038c57c: bl       #0x4a2730 ; _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii
0038c580: add      r3, r4, #0x358
0038c584: mov      r0, r3
0038c588: str      r3, [r4, #0x368]
0038c58c: str      r3, [r4, #0x36c]
0038c590: mov      r1, #0x10
0038c594: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c598: ldr      r1, [r4, #0x368]
0038c59c: mov      r2, #0xc2000000
0038c5a0: mov      r3, #0x42000000
0038c5a4: strb     r5, [r1]
0038c5a8: add      r2, r2, #0xc80000
0038c5ac: add      r3, r3, #0xc80000
0038c5b0: mov      r1, #0x370
0038c5b4: strh     r7, [r4, r1]
0038c5b8: mov      r0, r4
0038c5bc: str      r2, [r4, #0x14c]
0038c5c0: str      r3, [r4, #0x158]
0038c5c4: str      r2, [r4, #0x144]
0038c5c8: str      r2, [r4, #0x148]
0038c5cc: str      r3, [r4, #0x150]
0038c5d0: str      r3, [r4, #0x154]
0038c5d4: strb     r5, [r4, #0x373]
0038c5d8: strb     r5, [r4, #0x372]
0038c5dc: bl       #0x38aac8 ; _ZN10GameObject18UpdateAbsoluteAABBEv
0038c5e0: mov      r0, r6
0038c5e4: mov      r1, r4
0038c5e8: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
0038c5ec: mov      r0, r4
0038c5f0: add      sp, sp, #0xc
0038c5f4: pop      {r4, r5, r6, r7, pc}
0038c5f8: rsbeq    r8, r0, r0, ror #13
0038c5fc: andeq    r2, r0, r0, ror sp

# _ZN13ObjectManager16IsOnlineDeferredEP10ObjectBase
00347dec: push     {r4, r5, r6, r7, lr}
00347df0: sub      sp, sp, #0x14
00347df4: mov      r5, r1
00347df8: mov      r7, r0
00347dfc: bl       #0x800f8c ; _ZN9CMatching3GetEv
00347e00: ldr      r3, [r0]
00347e04: mov      r1, r0
00347e08: add      r0, sp, #4
00347e0c: mov      lr, pc
00347e10: ldr      pc, [r3, #0x88]
00347e14: ldr      r6, [sp, #4]
00347e18: ldr      r3, [sp, #8]
00347e1c: mov      r0, r6
00347e20: cmp      r6, r3
00347e24: beq      #0x347e90
00347e28: add      r7, r7, #0x108
00347e2c: mov      r1, r6
00347e30: mov      r0, r7
00347e34: bl       #0x3452ac ; _ZNSt3mapIsSt4listIP10ObjectBaseSaIS2_EESt4lessIsESaISt4pairIKsS4_EEEixIiEERS4_RKT_
00347e38: mov      r1, r6
00347e3c: ldr      r4, [r0]
00347e40: mov      r0, r7
00347e44: bl       #0x3452ac ; _ZNSt3mapIsSt4listIP10ObjectBaseSaIS2_EESt4lessIsESaISt4pairIKsS4_EEEixIiEERS4_RKT_
00347e48: cmp      r4, r0
00347e4c: beq      #0x347e68
00347e50: ldr      r3, [r4, #8]
00347e54: cmp      r5, r3
00347e58: beq      #0x347e68
00347e5c: ldr      r4, [r4]
00347e60: cmp      r0, r4
00347e64: bne      #0x347e50
00347e68: mov      r0, r7
00347e6c: mov      r1, r6
00347e70: bl       #0x3452ac ; _ZNSt3mapIsSt4listIP10ObjectBaseSaIS2_EESt4lessIsESaISt4pairIKsS4_EEEixIiEERS4_RKT_
00347e74: cmp      r4, r0
00347e78: bne      #0x347ec0
00347e7c: ldr      r3, [sp, #8]
00347e80: add      r6, r6, #4
00347e84: cmp      r6, r3
00347e88: bne      #0x347e2c
00347e8c: ldr      r0, [sp, #4]
00347e90: mov      r4, #0
00347e94: cmp      r0, #0
00347e98: beq      #0x347eb4
00347e9c: ldr      r1, [sp, #0xc]
00347ea0: rsb      r1, r0, r1
00347ea4: bic      r1, r1, #3
00347ea8: cmp      r1, #0x80
00347eac: bhi      #0x347ecc
00347eb0: bl       #0x708f00 ; 
00347eb4: mov      r0, r4
00347eb8: add      sp, sp, #0x14
00347ebc: pop      {r4, r5, r6, r7, pc}
00347ec0: mov      r4, #1
00347ec4: ldr      r0, [sp, #4]
00347ec8: b        #0x347e94
00347ecc: bl       #0x310440 ; _Z10CustomFreePv
00347ed0: b        #0x347eb4

# _ZN13ObjectManager12GetNewObjectEPKcS1_ib
0034b520: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034b524: sub      sp, sp, #0x64
0034b528: str      r3, [sp, #0x18]
0034b52c: ldr      r3, [pc, #0x1c8]
0034b530: ldr      r7, [pc, #0x1c8]
0034b534: ldr      sl, [pc, #0x1c8]
0034b538: add      r3, pc, r3
0034b53c: str      r3, [sp, #0x24]
0034b540: ldr      r3, [pc, #0x1c0]
0034b544: add      r7, pc, r7
0034b548: ldrb     lr, [sp, #0x8c]
0034b54c: ldr      ip, [r7, sl]
0034b550: add      r3, pc, r3
0034b554: str      r3, [sp, #0x28]
0034b558: ldr      r3, [pc, #0x1ac]
0034b55c: ldr      ip, [ip]
0034b560: str      lr, [sp, #0x1c]
0034b564: ldr      r5, [pc, #0x1a4]
0034b568: ldr      lr, [pc, #0x1a4]
0034b56c: add      r3, pc, r3
0034b570: ldr      sb, [pc, #0x1a0]
0034b574: str      lr, [sp, #0x20]
0034b578: str      ip, [sp, #0x5c]
0034b57c: mov      r8, r0
0034b580: str      r1, [sp, #0x14]
0034b584: mov      r6, r2
0034b588: add      r5, pc, r5
0034b58c: str      r3, [sp, #0x2c]
0034b590: mov      r4, #0
0034b594: b        #0x34b5a4
0034b598: add      r4, r4, #8
0034b59c: cmp      r4, #0x108
0034b5a0: beq      #0x34b668
0034b5a4: ldr      fp, [r5, r4]
0034b5a8: mov      r0, r6
0034b5ac: mov      r1, fp
0034b5b0: bl       #0x30e31c ; 
0034b5b4: cmp      r0, #0
0034b5b8: bne      #0x34b598
0034b5bc: add      r3, r5, r4
0034b5c0: mov      lr, pc
0034b5c4: ldr      pc, [r3, #4]
0034b5c8: cmp      r0, #0
0034b5cc: beq      #0x34b61c
0034b5d0: str      fp, [r0, #0x20]
0034b5d4: ldr      ip, [sp, #0x88]
0034b5d8: mov      r2, r0
0034b5dc: ldr      r1, [sp, #0x14]
0034b5e0: str      ip, [sp, #4]
0034b5e4: ldr      ip, [sp, #0x1c]
0034b5e8: ldr      r3, [sp, #0x18]
0034b5ec: mov      r0, r8
0034b5f0: str      r6, [sp]
0034b5f4: str      ip, [sp, #8]
0034b5f8: bl       #0x34b270 ; _ZN13ObjectManager3AddEP10ObjectBasePKcS3_ib
0034b5fc: ldr      r3, [r7, sl]
0034b600: ldr      r2, [sp, #0x5c]
0034b604: mov      r0, r8
0034b608: ldr      r3, [r3]
0034b60c: cmp      r2, r3
0034b610: bne      #0x34b6f8
0034b614: add      sp, sp, #0x64
0034b618: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034b61c: ldr      r3, [r7, sb]
0034b620: ldr      r3, [r3]
0034b624: cmp      r3, #2
0034b628: streq    r0, [r0]
0034b62c: beq      #0x34b598
0034b630: cmp      r3, #1
0034b634: bne      #0x34b598
0034b638: ldr      r3, [sp, #0x20]
0034b63c: movw     ip, #0x51e
0034b640: ldr      r1, [sp, #0x24]
0034b644: ldr      r0, [r7, r3]
0034b648: ldr      r2, [sp, #0x28]
0034b64c: ldr      r3, [sp, #0x2c]
0034b650: add      r0, r0, #0xa8
0034b654: add      r4, r4, #8
0034b658: str      ip, [sp]
0034b65c: bl       #0x30e004 ; 
0034b660: cmp      r4, #0x108
0034b664: bne      #0x34b5a4
0034b668: ldr      r3, [pc, #0xac]
0034b66c: add      r4, sp, #0x44
0034b670: ldr      r5, [r7, r3]
0034b674: mov      r0, r5
0034b678: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0034b67c: ldr      r1, [pc, #0x9c]
0034b680: add      r2, sp, #0x40
0034b684: mov      r0, r4
0034b688: add      r1, pc, r1
0034b68c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0034b690: mov      r0, r5
0034b694: mov      r1, r4
0034b698: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
0034b69c: ldr      r0, [sp, #0x58]
0034b6a0: cmp      r0, r4
0034b6a4: beq      #0x34b6c4
0034b6a8: cmp      r0, #0
0034b6ac: beq      #0x34b6c4
0034b6b0: ldr      r1, [sp, #0x44]
0034b6b4: rsb      r1, r0, r1
0034b6b8: cmp      r1, #0x80
0034b6bc: bhi      #0x34b6f0
0034b6c0: bl       #0x708f00 ; 
0034b6c4: add      r4, sp, #0x34
0034b6c8: mov      r0, r4
0034b6cc: bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
0034b6d0: ldr      r0, [sp, #0x34]
0034b6d4: ldr      r1, [r4, #8]
0034b6d8: ldr      r2, [sp, #0x38]
0034b6dc: mov      r3, r8
0034b6e0: str      r0, [r3], #4
0034b6e4: str      r1, [r3, #4]
0034b6e8: str      r2, [r8, #4]
0034b6ec: b        #0x34b5fc
0034b6f0: bl       #0x310440 ; _Z10CustomFreePv
0034b6f4: b        #0x34b6c4
0034b6f8: bl       #0x30e310 ; 
0034b6fc: subseq   r2, r7, r0, lsr #29
0034b700: rsbeq    sb, r4, ip, asr #10
0034b704: andeq    r4, r0, ip, lsr #1
0034b708: subseq   r4, r7, r0, asr #25
0034b70c: subseq   r4, r7, ip, lsr #26
0034b710: rsbeq    r1, r1, r0, ror r2
0034b714: andeq    r1, r0, r0, asr #19
0034b718: andeq    r3, r0, r0, asr #19
0034b71c: andeq    r0, r0, r4, lsl #17
0034b720: subseq   r4, r7, r0, ror sp

# _ZThn884_N9Character11Ctrl_LookAtEP10GameObject
003ad9a4: sub      r0, r0, #0x374
003ad9a8: b        #0x3ad9ac

# _ZN13ObjectManager25ProcessAcknowledgedPacketEii
00346178: push     {r4, r5, r6, r7, r8, lr}
0034617c: mov      r7, r0
00346180: ldr      r4, [r7, #0x100]!
00346184: mov      r5, r1
00346188: mov      r6, r2
0034618c: cmp      r7, r4
00346190: mov      r8, r0
00346194: mov      r1, r5
00346198: mov      r2, r6
0034619c: beq      #0x3461d4
003461a0: ldr      r3, [r4, #8]
003461a4: ldr      r3, [r3, #0x100]
003461a8: cmp      r3, #0
003461ac: mov      r0, r3
003461b0: beq      #0x3461c0
003461b4: ldr      r3, [r3]
003461b8: mov      lr, pc
003461bc: ldr      pc, [r3, #0x40]
003461c0: ldr      r4, [r4]
003461c4: mov      r1, r5
003461c8: mov      r2, r6
003461cc: cmp      r7, r4
003461d0: bne      #0x3461a0
003461d4: ldr      r3, [r8, #0x9c]
003461d8: add      r8, r8, #0x98
003461dc: cmp      r3, #0
003461e0: beq      #0x346278
003461e4: mov      r1, r8
003461e8: b        #0x3461f0
003461ec: mov      r3, r2
003461f0: ldr      r2, [r3, #0x10]
003461f4: cmp      r2, r5
003461f8: ldrlt    r2, [r3, #0xc]
003461fc: ldrge    r2, [r3, #8]
00346200: movlt    r3, r1
00346204: mov      r1, r3
00346208: cmp      r2, #0
0034620c: bne      #0x3461ec
00346210: cmp      r8, r3
00346214: beq      #0x346280
00346218: ldr      r2, [r3, #0x10]
0034621c: cmp      r2, r5
00346220: bgt      #0x346278
00346224: cmp      r8, r3
00346228: beq      #0x346280
0034622c: ldr      r0, [r3, #0x14]!
00346230: cmp      r0, r3
00346234: beq      #0x346280
00346238: ldr      r2, [r0, #8]
0034623c: cmp      r2, r6
00346240: beq      #0x346254
00346244: ldr      r0, [r0]
00346248: cmp      r3, r0
0034624c: bne      #0x346238
00346250: mov      r0, r3
00346254: cmp      r0, r3
00346258: beq      #0x346280
0034625c: ldr      r3, [r0]
00346260: ldr      r2, [r0, #4]
00346264: mov      r1, #0xc
00346268: str      r3, [r2]
0034626c: str      r2, [r3, #4]
00346270: pop      {r4, r5, r6, r7, r8, lr}
00346274: b        #0x708f00
00346278: mov      r3, r8
0034627c: b        #0x346224
00346280: pop      {r4, r5, r6, r7, r8, pc}

# _ZN12CameraTarget17GetTargetPositionEv
00411a74: ldr      r0, [r0, #0xc]
00411a78: b        #0x3943b8

# _ZN13ObjectManager15GetObjectByTypeEPKc
00345c20: push     {r4, r5, lr}
00345c24: sub      sp, sp, #0xc
00345c28: mov      r5, r0
00345c2c: mov      r0, r1
00345c30: mov      r1, r2
00345c34: mov      r2, sp
00345c38: str      sp, [sp]
00345c3c: str      sp, [sp, #4]
00345c40: bl       #0x3426b0 ; _ZNK13ObjectManager16GetObjectsByTypeEPKcRSt4listIP9CharacterSaIS4_EE
00345c44: ldr      r2, [sp]
00345c48: mov      r4, sp
00345c4c: cmp      r2, r4
00345c50: beq      #0x345c84
00345c54: mov      r3, r2
00345c58: ldr      r3, [r3]
00345c5c: cmp      r3, r4
00345c60: bne      #0x345c58
00345c64: ldr      r1, [r2, #8]
00345c68: mov      r0, r5
00345c6c: bl       #0x33f524 ; _ZN12ObjectHandleC1EP10ObjectBase
00345c70: mov      r0, sp
00345c74: bl       #0x345b8c ; _ZNSt4priv10_List_baseIP9CharacterSaIS2_EE5clearEv
00345c78: mov      r0, r5
00345c7c: add      sp, sp, #0xc
00345c80: pop      {r4, r5, pc}
00345c84: mov      r0, r5
00345c88: mov      r1, #0
00345c8c: bl       #0x33f524 ; _ZN12ObjectHandleC1EP10ObjectBase
00345c90: b        #0x345c70

# _ZNK8RoomZone14HasBeenVisitedEv
003964a8: ldr      r3, [r0, #0x38c]
003964ac: cmp      r3, #0
003964b0: moveq    r0, #1
003964b4: ldrbne   r0, [r3, #0x3fc]
003964b8: bx       lr

# _ZN13ObjectiveList22RemoveObjectiveMarkersEv
0047a5cc: mov      ip, #0x14
0047a5d0: mov      r3, #1
0047a5d4: sub      sp, sp, #8
0047a5d8: mov      r1, ip
0047a5dc: mov      r2, r3
0047a5e0: str      ip, [sp]
0047a5e4: str      r3, [sp, #4]
0047a5e8: add      sp, sp, #8
0047a5ec: b        #0x47a540

# _ZN13ObjectManager10FakeRemoveE12ObjectHandle
00349240: push     {r4, r5, r6, r7, r8, lr}
00349244: sub      sp, sp, #0x18
00349248: add      r4, sp, #4
0034924c: mov      r5, r0
00349250: mov      r0, r4
00349254: stm      r4, {r1, r2, r3}
00349258: bl       #0x33fee4 ; _ZN12ObjectHandlecvP10GameObjectEv
0034925c: mov      r6, r0
00349260: ldr      r0, [r0, #0x2f4]
00349264: mov      r3, #1
00349268: strb     r3, [r6, #0x81]
0034926c: cmp      r0, #0
00349270: beq      #0x34927c
00349274: mov      r1, r6
00349278: bl       #0x3968cc ; _ZN8RoomZone12RemoveObjectEP10GameObject
0034927c: mov      r0, r5
00349280: mov      r1, r6
00349284: bl       #0x3462b8 ; _ZN13ObjectManager18RemoveNoRoomObjectEP10GameObject
00349288: mov      r2, r5
0034928c: ldr      r0, [r2, #0x90]!
00349290: cmp      r0, r2
00349294: beq      #0x3492b4
00349298: ldr      r3, [r0, #8]
0034929c: cmp      r6, r3
003492a0: beq      #0x3492b4
003492a4: ldr      r0, [r0]
003492a8: cmp      r2, r0
003492ac: bne      #0x349298
003492b0: mov      r0, r2
003492b4: cmp      r2, r0
003492b8: beq      #0x3492d4
003492bc: ldr      r3, [r0]
003492c0: ldr      r2, [r0, #4]
003492c4: mov      r1, #0xc
003492c8: str      r3, [r2]
003492cc: str      r2, [r3, #4]
003492d0: bl       #0x708f00 ; 
003492d4: mov      r0, r4
003492d8: mov      r1, #0
003492dc: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
003492e0: mov      r7, r5
003492e4: mov      r8, r0
003492e8: ldr      r0, [r7, #0x2c]!
003492ec: cmp      r7, r0
003492f0: beq      #0x349310
003492f4: ldr      r3, [r0, #8]
003492f8: ldr      r6, [r0]
003492fc: cmp      r8, r3
00349300: beq      #0x349468
00349304: mov      r0, r6
00349308: cmp      r7, r0
0034930c: bne      #0x3492f4
00349310: mov      r0, r4
00349314: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00349318: mov      r7, r5
0034931c: mov      r8, r0
00349320: ldr      r0, [r7, #0x70]!
00349324: cmp      r7, r0
00349328: beq      #0x349348
0034932c: ldr      r3, [r0, #8]
00349330: ldr      r6, [r0]
00349334: cmp      r8, r3
00349338: beq      #0x349484
0034933c: mov      r0, r6
00349340: cmp      r7, r0
00349344: bne      #0x34932c
00349348: mov      r0, r4
0034934c: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00349350: subs     r8, r0, #0
00349354: beq      #0x349394
00349358: mov      r7, r5
0034935c: ldr      r0, [r7, #0x60]!
00349360: cmp      r7, r0
00349364: beq      #0x349384
00349368: ldr      r3, [r0, #8]
0034936c: ldr      r6, [r0]
00349370: cmp      r8, r3
00349374: beq      #0x3494a0
00349378: mov      r0, r6
0034937c: cmp      r7, r0
00349380: bne      #0x349368
00349384: add      r0, r8, #0x3c8
00349388: bl       #0x3d2ff8 ; _ZN6CharAI15RemoveFromGroupEv
0034938c: mov      r0, r8
00349390: bl       #0x3a66f8 ; _ZN9Character5CleanEv
00349394: mov      r0, r4
00349398: mov      r1, #0
0034939c: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
003493a0: subs     r7, r0, #0
003493a4: beq      #0x3493b4
003493a8: ldr      r3, [r7, #0xf4]
003493ac: cmp      r3, #5
003493b0: beq      #0x3494ec
003493b4: mov      r0, r4
003493b8: mov      r1, #0
003493bc: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
003493c0: mov      r7, r5
003493c4: mov      r8, r0
003493c8: ldr      r0, [r7, #0x100]!
003493cc: cmp      r7, r0
003493d0: beq      #0x3493f0
003493d4: ldr      r3, [r0, #8]
003493d8: ldr      r6, [r0]
003493dc: cmp      r8, r3
003493e0: beq      #0x3494bc
003493e4: mov      r0, r6
003493e8: cmp      r7, r0
003493ec: bne      #0x3493d4
003493f0: ldr      r3, [r5, #0x10]
003493f4: ldr      r0, [sp, #4]
003493f8: cmp      r3, #0
003493fc: addeq    r6, r5, #0xc
00349400: beq      #0x349448
00349404: add      r6, r5, #0xc
00349408: mov      r1, r6
0034940c: b        #0x349414
00349410: mov      r3, r2
00349414: ldr      r2, [r3, #0x10]
00349418: cmp      r0, r2
0034941c: ldrgt    r2, [r3, #0xc]
00349420: ldrle    r2, [r3, #8]
00349424: movgt    r3, r1
00349428: mov      r1, r3
0034942c: cmp      r2, #0
00349430: bne      #0x349410
00349434: cmp      r6, r3
00349438: beq      #0x349448
0034943c: ldr      r2, [r3, #0x10]
00349440: cmp      r0, r2
00349444: bge      #0x3494d8
00349448: mov      r1, r4
0034944c: mov      r0, r6
00349450: bl       #0x33fc88 ; _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
00349454: ldr      r1, [r0, #0x18]
00349458: mov      r0, r5
0034945c: bl       #0x343188 ; _ZN13ObjectManager29AddOrphanRenderObjectToDeleteEP10ObjectBase
00349460: add      sp, sp, #0x18
00349464: pop      {r4, r5, r6, r7, r8, pc}
00349468: ldr      r3, [r0, #4]
0034946c: mov      r1, #0xc
00349470: str      r6, [r3]
00349474: str      r3, [r6, #4]
00349478: bl       #0x708f00 ; 
0034947c: mov      r0, r6
00349480: b        #0x349308
00349484: ldr      r3, [r0, #4]
00349488: mov      r1, #0xc
0034948c: str      r6, [r3]
00349490: str      r3, [r6, #4]
00349494: bl       #0x708f00 ; 
00349498: mov      r0, r6
0034949c: b        #0x349340
003494a0: ldr      r3, [r0, #4]
003494a4: mov      r1, #0xc
003494a8: str      r6, [r3]
003494ac: str      r3, [r6, #4]
003494b0: bl       #0x708f00 ; 
003494b4: mov      r0, r6
003494b8: b        #0x34937c
003494bc: ldr      r3, [r0, #4]
003494c0: mov      r1, #0xc
003494c4: str      r6, [r3]
003494c8: str      r3, [r6, #4]
003494cc: bl       #0x708f00 ; 
003494d0: mov      r0, r6
003494d4: b        #0x3493e8
003494d8: add      r1, sp, #0x18
003494dc: str      r3, [r1, #-4]!
003494e0: mov      r0, r6
003494e4: bl       #0x347f58 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE5eraseENS_17_Rb_tree_iteratorIS6_SA_EE
003494e8: b        #0x349448
003494ec: mov      r8, r5
003494f0: ldr      r0, [r8, #0x68]!
003494f4: cmp      r8, r0
003494f8: beq      #0x3493b4
003494fc: ldr      r3, [r0, #8]
00349500: ldr      r6, [r0]
00349504: cmp      r7, r3
00349508: movne    r0, r6
0034950c: bne      #0x3494f4
00349510: ldr      r3, [r0, #4]
00349514: mov      r1, #0xc
00349518: str      r6, [r3]
0034951c: str      r3, [r6, #4]
00349520: bl       #0x708f00 ; 
00349524: mov      r0, r6
00349528: b        #0x3494f4

# _ZN13ObjectManager22IsRemotePlayerOfMemberEiP9Character
00340948: push     {r4, r5, r6, r7, r8, lr}
0034094c: ldr      r6, [pc, #0x98]
00340950: subs     r7, r2, #0
00340954: mov      r4, r1
00340958: add      r6, pc, r6
0034095c: beq      #0x3409dc
00340960: bl       #0x8100dc ; _Z15GetNetPlayerMgrv
00340964: mov      r1, r4
00340968: bl       #0x812d50 ; _ZN17CNetPlayerManager25GetPlayerIdListByMemberIdEi
0034096c: ldr      r3, [r0]
00340970: ldr      r2, [r0, #4]
00340974: mov      r5, r0
00340978: rsb      r2, r3, r2
0034097c: lsrs     r2, r2, #2
00340980: beq      #0x3409dc
00340984: ldr      r1, [pc, #0x64]
00340988: mov      r2, #0
0034098c: mov      r4, r2
00340990: ldr      r6, [r6, r1]
00340994: ldr      r1, [r3, r2, lsl #2]
00340998: ldr      r0, [r6, #0x40]
0034099c: mov      r2, #0
003409a0: bl       #0x36dfb0 ; _ZN13PlayerManager21GetPlayerByInternalIDEib
003409a4: ldr      r3, [r0, #0x660]
003409a8: add      r4, r4, #1
003409ac: mov      r2, r4
003409b0: cmp      r3, #0
003409b4: beq      #0x3409c8
003409b8: ldr      r3, [r3, #0x108]
003409bc: ldr      r1, [r7, #0x108]
003409c0: cmp      r1, r3
003409c4: beq      #0x3409e4
003409c8: ldr      r3, [r5]
003409cc: ldr      r1, [r5, #4]
003409d0: rsb      r1, r3, r1
003409d4: cmp      r4, r1, asr #2
003409d8: blo      #0x340994
003409dc: mov      r0, #0
003409e0: pop      {r4, r5, r6, r7, r8, pc}
003409e4: mov      r0, #1
003409e8: pop      {r4, r5, r6, r7, r8, pc}
003409ec: rsbeq    r4, r5, r8, lsr r1
003409f0: strdeq   r3, r4, [r0], -r4

# _ZN13ObjectManager12ObjectReInitEv
003406ac: push     {r4, r5, r6, lr}
003406b0: mov      r6, r0
003406b4: ldr      r5, [r6, #0x2c]!
003406b8: mov      r1, #0
003406bc: cmp      r6, r5
003406c0: beq      #0x34070c
003406c4: ldr      r4, [r5, #8]
003406c8: cmp      r4, #0
003406cc: add      r0, r4, #0x8c
003406d0: beq      #0x3406fc
003406d4: bl       #0x33dd24 ; _ZN13ConditionData11SetAsTestedEb
003406d8: mov      r1, #0
003406dc: add      r0, r4, #0xb0
003406e0: bl       #0x33dd24 ; _ZN13ConditionData11SetAsTestedEb
003406e4: ldr      r3, [r4]
003406e8: mov      r0, r4
003406ec: mov      lr, pc
003406f0: ldr      pc, [r3, #0x24]
003406f4: cmp      r0, #0
003406f8: bne      #0x340710
003406fc: ldr      r5, [r5]
00340700: cmp      r6, r5
00340704: mov      r1, #0
00340708: bne      #0x3406c4
0034070c: pop      {r4, r5, r6, pc}
00340710: add      r4, r4, #0x3c8
00340714: mov      r0, r4
00340718: bl       #0x3cfd7c ; _ZN6CharAI16AI_ScriptCleanUpEv
0034071c: mov      r0, r4
00340720: bl       #0x3cfde4 ; _ZN6CharAI13AI_ScriptInitEv
00340724: ldr      r5, [r5]
00340728: b        #0x340700

# _ZThn4_N8RoomZone17DeclarePropertiesEv
0039654c: sub      r0, r0, #4
00396550: b        #0x396554

# _ZN13ObjectManager6Draw3DEv
00348988: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034898c: ldr      r5, [pc, #0x4fc]
00348990: ldr      r2, [pc, #0x4fc]
00348994: sub      sp, sp, #0x104
00348998: add      r5, pc, r5
0034899c: ldr      r3, [r5, r2]
003489a0: add      r4, sp, #0x1c
003489a4: mov      r6, #0
003489a8: ldr      r3, [r3]
003489ac: ldr      fp, [pc, #0x4e4]
003489b0: mov      r7, r0
003489b4: mov      r1, r6
003489b8: str      r2, [sp, #0x10]
003489bc: mov      r0, r4
003489c0: mov      r2, #0x40
003489c4: str      r3, [sp, #0xfc]
003489c8: bl       #0x30e460 ; 
003489cc: mov      r1, r6
003489d0: mov      r2, #0x40
003489d4: mov      r0, r4
003489d8: bl       #0x30e460 ; 
003489dc: ldr      r2, [r5, fp]
003489e0: mov      r3, #0x3f800000
003489e4: mov      r1, #1
003489e8: ldr      r2, [r2, #0x10]
003489ec: str      r3, [sp, #0x58]
003489f0: str      r3, [sp, #0x1c]
003489f4: str      r3, [sp, #0x30]
003489f8: str      r3, [sp, #0x44]
003489fc: strb     r1, [sp, #0x5c]
00348a00: ldr      r3, [r2, #0x10]
00348a04: mov      r2, r4
00348a08: add      r6, r7, #0xc
00348a0c: mov      r0, r3
00348a10: ldr      r3, [r3]
00348a14: mov      lr, pc
00348a18: ldr      pc, [r3, #0x6c]
00348a1c: ldr      r4, [r7, #0x14]
00348a20: cmp      r6, r4
00348a24: beq      #0x348a7c
00348a28: ldr      r3, [r4, #0x2c]
00348a2c: cmp      r3, #0
00348a30: beq      #0x348a50
00348a34: ldrb     r2, [r3, #0x80]
00348a38: cmp      r2, #0
00348a3c: beq      #0x348a50
00348a40: mov      r0, r3
00348a44: ldr      r3, [r3]
00348a48: mov      lr, pc
00348a4c: ldr      pc, [r3, #0x30]
00348a50: ldr      r2, [r4, #0xc]
00348a54: cmp      r2, #0
00348a58: bne      #0x348a64
00348a5c: b        #0x348e3c
00348a60: mov      r2, r3
00348a64: ldr      r3, [r2, #8]
00348a68: cmp      r3, #0
00348a6c: bne      #0x348a60
00348a70: mov      r4, r2
00348a74: cmp      r6, r4
00348a78: bne      #0x348a28
00348a7c: ldr      r3, [pc, #0x418]
00348a80: add      r4, sp, #0xe4
00348a84: ldr      r6, [r5, r3]
00348a88: mov      r0, r6
00348a8c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00348a90: ldr      r1, [pc, #0x408]
00348a94: add      r2, sp, #0xe0
00348a98: mov      r0, r4
00348a9c: add      r1, pc, r1
00348aa0: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00348aa4: mov      r0, r6
00348aa8: mov      r1, r4
00348aac: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00348ab0: mov      r6, r0
00348ab4: ldr      r0, [sp, #0xf8]
00348ab8: cmp      r0, r4
00348abc: beq      #0x348adc
00348ac0: cmp      r0, #0
00348ac4: beq      #0x348adc
00348ac8: ldr      r1, [sp, #0xe4]
00348acc: rsb      r1, r0, r1
00348ad0: cmp      r1, #0x80
00348ad4: bhi      #0x348e70
00348ad8: bl       #0x708f00 ; 
00348adc: cmp      r6, #0
00348ae0: beq      #0x348e1c
00348ae4: ldr      r3, [r5, fp]
00348ae8: ldr      r3, [r3, #0x10]
00348aec: ldr      r6, [r3, #0x10]
00348af0: movw     r3, #0xffff
00348af4: ldr      r4, [r6, #0xdc]
00348af8: ldrh     r2, [r4, #0x2e]
00348afc: cmp      r2, r3
00348b00: beq      #0x348e78
00348b04: add      r3, sp, #0xdc
00348b08: mov      r0, r3
00348b0c: str      r3, [sp, #0x14]
00348b10: mov      r1, r4
00348b14: mov      r3, #1
00348b18: bl       #0x5dd0e4 ; _ZN6glitch5video24CMaterialRendererManager19getMaterialInstanceEtb
00348b1c: ldr      r0, [sp, #0xdc]
00348b20: cmp      r0, #0
00348b24: moveq    r2, #0xff
00348b28: beq      #0x348b34
00348b2c: bl       #0x5c5d34 ; _ZNK6glitch5video9CMaterial12getTechniqueEv
00348b30: mov      r2, r0
00348b34: mov      r3, #0
00348b38: mov      r0, r6
00348b3c: ldr      r1, [sp, #0x14]
00348b40: bl       #0x5ad368 ; _ZN6glitch5video12IVideoDriver11setMaterialERKN5boost13intrusive_ptrINS0_9CMaterialEEEhPKNS3_INS0_19CVertexAttributeMapEEE
00348b44: mvn      r2, #0
00348b48: mov      r3, #0
00348b4c: strb     r2, [sp, #0xd9]
00348b50: mov      r2, #0x7f
00348b54: strb     r3, [sp, #0xda]
00348b58: strb     r2, [sp, #0xdb]
00348b5c: strb     r3, [sp, #0xd8]
00348b60: ldr      r4, [r7, #0x24]!
00348b64: add      r8, sp, #0x60
00348b68: mov      sb, r5
00348b6c: b        #0x348bc0
00348b70: ldr      r2, [r4, #8]
00348b74: ldr      r3, [r6]
00348b78: mov      r0, r6
00348b7c: ldr      r1, [r2, #0x13c]
00348b80: ldr      sl, [r2, #0x12c]
00348b84: ldr      r5, [r2, #0x130]
00348b88: ldr      lr, [r2, #0x134]
00348b8c: ldr      ip, [r2, #0x138]
00348b90: ldr      r2, [r2, #0x140]
00348b94: ldr      r3, [r3, #0x28]
00348b98: str      r1, [sp, #0x70]
00348b9c: str      r2, [sp, #0x74]
00348ba0: str      sl, [sp, #0x60]
00348ba4: str      r5, [sp, #0x64]
00348ba8: str      lr, [sp, #0x68]
00348bac: str      ip, [sp, #0x6c]
00348bb0: mov      r1, r8
00348bb4: ldr      r2, [sp, #0xd8]
00348bb8: blx      r3
00348bbc: ldr      r4, [r4]
00348bc0: cmp      r7, r4
00348bc4: bne      #0x348b70
00348bc8: ldr      r0, [sb, fp]
00348bcc: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
00348bd0: ldr      r3, [r0, #0x128]
00348bd4: mov      sl, #0
00348bd8: mvn      fp, #0
00348bdc: ldr      r3, [r3, #8]
00348be0: add      r7, sp, #0xcc
00348be4: mov      r5, sb
00348be8: mov      r0, r3
00348bec: ldr      r3, [r3]
00348bf0: mov      lr, pc
00348bf4: ldr      pc, [r3, #0x144]
00348bf8: mov      ip, #0x7f
00348bfc: mov      r8, r0
00348c00: strb     ip, [sp, #0xdb]
00348c04: add      r3, r8, #0x4c
00348c08: add      ip, r8, #0x2c
00348c0c: strb     sl, [sp, #0xda]
00348c10: strb     sl, [sp, #0xd9]
00348c14: strb     fp, [sp, #0xd8]
00348c18: str      ip, [sp, #0xc]
00348c1c: str      r3, [sp, #8]
00348c20: add      sb, r8, #0xc
00348c24: mov      r0, r6
00348c28: add      r1, r8, #0x6c
00348c2c: ldr      r3, [r6]
00348c30: ldr      r2, [sp, #0xd8]
00348c34: mov      r4, #0
00348c38: mov      lr, pc
00348c3c: ldr      pc, [r3, #0x28]
00348c40: mov      ip, #0x7f
00348c44: mov      r3, r7
00348c48: ldr      r1, [sp, #8]
00348c4c: ldr      r2, [sp, #0xc]
00348c50: mov      r0, sb
00348c54: strb     ip, [sp, #0xdb]
00348c58: add      r6, sp, #0xd8
00348c5c: strb     fp, [sp, #0xda]
00348c60: strb     sl, [sp, #0xd9]
00348c64: strb     sl, [sp, #0xd8]
00348c68: str      r4, [sp, #0xcc]
00348c6c: str      r4, [sp, #0xd0]
00348c70: str      r4, [sp, #0xd4]
00348c74: bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
00348c78: add      r2, r8, #0x3c
00348c7c: mov      r0, r7
00348c80: str      r2, [sp, #4]
00348c84: add      r7, sp, #0xc0
00348c88: mov      r1, r6
00348c8c: mov      r2, #0x64
00348c90: bl       #0x340154 ; _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
00348c94: mov      r3, r7
00348c98: ldr      r2, [sp, #4]
00348c9c: ldr      r1, [sp, #8]
00348ca0: mov      r0, sb
00348ca4: str      r4, [sp, #0xc0]
00348ca8: str      r4, [sp, #0xc4]
00348cac: str      r4, [sp, #0xc8]
00348cb0: bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
00348cb4: add      r3, r8, #0x5c
00348cb8: mov      r0, r7
00348cbc: mov      r1, r6
00348cc0: add      r7, sp, #0xb4
00348cc4: mov      r2, #0x64
00348cc8: str      r3, [sp]
00348ccc: bl       #0x340154 ; _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
00348cd0: mov      r3, r7
00348cd4: ldr      r2, [sp, #0xc]
00348cd8: ldr      r1, [sp]
00348cdc: mov      r0, sb
00348ce0: str      r4, [sp, #0xb4]
00348ce4: str      r4, [sp, #0xb8]
00348ce8: str      r4, [sp, #0xbc]
00348cec: bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
00348cf0: mov      r0, r7
00348cf4: mov      r1, r6
00348cf8: add      r7, sp, #0xa8
00348cfc: mov      r2, #0x64
00348d00: bl       #0x340154 ; _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
00348d04: mov      r3, r7
00348d08: ldm      sp, {r1, r2}
00348d0c: mov      r0, sb
00348d10: add      r8, r8, #0x1c
00348d14: str      r4, [sp, #0xa8]
00348d18: str      r4, [sp, #0xac]
00348d1c: str      r4, [sp, #0xb0]
00348d20: bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
00348d24: mov      r0, r7
00348d28: mov      r1, r6
00348d2c: add      r7, sp, #0x9c
00348d30: mov      r2, #0x64
00348d34: bl       #0x340154 ; _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
00348d38: mov      ip, #0x7f
00348d3c: mov      r3, r7
00348d40: ldr      r2, [sp, #0xc]
00348d44: ldr      r1, [sp, #8]
00348d48: mov      r0, r8
00348d4c: strb     ip, [sp, #0xdb]
00348d50: strb     fp, [sp, #0xd9]
00348d54: strb     sl, [sp, #0xd8]
00348d58: strb     fp, [sp, #0xda]
00348d5c: str      r4, [sp, #0x9c]
00348d60: str      r4, [sp, #0xa0]
00348d64: str      r4, [sp, #0xa4]
00348d68: bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
00348d6c: mov      r0, r7
00348d70: mov      r1, r6
00348d74: add      r7, sp, #0x90
00348d78: mov      r2, #0x64
00348d7c: bl       #0x340154 ; _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
00348d80: mov      r3, r7
00348d84: ldr      r1, [sp, #8]
00348d88: ldr      r2, [sp, #4]
00348d8c: mov      r0, r8
00348d90: str      r4, [sp, #0x90]
00348d94: str      r4, [sp, #0x94]
00348d98: str      r4, [sp, #0x98]
00348d9c: bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
00348da0: mov      r0, r7
00348da4: mov      r1, r6
00348da8: add      r7, sp, #0x84
00348dac: mov      r2, #0x64
00348db0: bl       #0x340154 ; _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
00348db4: mov      r3, r7
00348db8: ldr      r2, [sp, #0xc]
00348dbc: ldr      r1, [sp]
00348dc0: mov      r0, r8
00348dc4: str      r4, [sp, #0x84]
00348dc8: str      r4, [sp, #0x88]
00348dcc: str      r4, [sp, #0x8c]
00348dd0: bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
00348dd4: mov      r0, r7
00348dd8: mov      r1, r6
00348ddc: mov      r2, #0x64
00348de0: add      r7, sp, #0x78
00348de4: bl       #0x340154 ; _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
00348de8: ldm      sp, {r1, r2}
00348dec: mov      r3, r7
00348df0: mov      r0, r8
00348df4: str      r4, [sp, #0x80]
00348df8: str      r4, [sp, #0x78]
00348dfc: str      r4, [sp, #0x7c]
00348e00: bl       #0x3415d8 ; _ZNK6glitch4core7plane3dIfE25getIntersectionWithPlanesERKS2_S4_RNS0_8vector3dIfEE
00348e04: mov      r0, r7
00348e08: mov      r1, r6
00348e0c: mov      r2, #0x64
00348e10: bl       #0x340154 ; _Z21DBG_DisplayPointAsBoxRKN6glitch4core8vector3dIfEERKNS_5video6SColorEi
00348e14: ldr      r0, [sp, #0x14]
00348e18: bl       #0x310be8 ; _ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev
00348e1c: ldr      r2, [sp, #0x10]
00348e20: ldr      r3, [r5, r2]
00348e24: ldr      r2, [sp, #0xfc]
00348e28: ldr      r3, [r3]
00348e2c: cmp      r2, r3
00348e30: bne      #0x348e8c
00348e34: add      sp, sp, #0x104
00348e38: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00348e3c: ldr      r3, [r4, #4]
00348e40: ldr      r1, [r3, #0xc]
00348e44: cmp      r1, r4
00348e48: bne      #0x348e64
00348e4c: mov      r4, r3
00348e50: ldr      r3, [r3, #4]
00348e54: ldr      r2, [r3, #0xc]
00348e58: cmp      r2, r4
00348e5c: beq      #0x348e4c
00348e60: ldr      r2, [r4, #0xc]
00348e64: cmp      r2, r3
00348e68: movne    r4, r3
00348e6c: b        #0x348a20
00348e70: bl       #0x310440 ; _Z10CustomFreePv
00348e74: b        #0x348adc
00348e78: mov      r0, r4
00348e7c: mov      r1, #1
00348e80: bl       #0x5d8b28 ; _ZN6glitch5video24CMaterialRendererManager22createMaterialRendererENS0_15E_MATERIAL_TYPEE
00348e84: mov      r2, r0
00348e88: b        #0x348b04
00348e8c: bl       #0x30e310 ; 

# _ZNK9Character18GetCharAIFactionIdEv
003a3180: ldr      r0, [r0, #0xff8]
003a3184: ldr      r3, [pc, #0x24]
003a3188: cmp      r0, #0
003a318c: add      r3, pc, r3
003a3190: blt      #0x3a31a8
003a3194: ldr      r2, [pc, #0x18]
003a3198: ldr      r3, [r3, r2]
003a319c: ldr      r3, [r3]
003a31a0: cmp      r0, r3
003a31a4: bxlt     lr
003a31a8: mov      r0, #0xa
003a31ac: bx       lr
003a31b0: subseq   r1, pc, r4, lsl #18
003a31b4: andeq    r2, r0, r4, asr #4

# _ZN8RoomZone10SetVisitedEb
003964bc: ldr      r3, [r0, #0x38c]
003964c0: cmp      r3, #0
003964c4: strbne   r1, [r3, #0x3fc]
003964c8: bx       lr

# _ZTV8RoomZone
00964bd0: andeq    r0, r0, r0
00964bd4: andeq    r0, r0, r0
00964bd8: eorseq   r6, sb, r8, lsr #16
00964bdc: ldrhteq  r6, [sb], -r0
00964be0: eorseq   r0, r4, r4, lsr #2
00964be4: eorseq   sp, r8, ip, ror #15
00964be8: eorseq   fp, r8, r4, ror #19
00964bec: eorseq   fp, r8, ip, lsl sb
00964bf0: eorseq   r6, sb, r4, asr r5
00964bf4: eorseq   r6, sb, r8, asr #10
00964bf8: eorseq   r0, r4, r4, asr r0
00964bfc: ldrsbteq sp, [r3], -r0
00964c00: ldrsbteq sp, [r3], -r8
00964c04: mlaseq   sb, ip, lr, r6
00964c08: eorseq   r6, sb, r4, asr #10
00964c0c: eorseq   r0, r4, ip, lsr #1
00964c10: eorseq   sl, r8, r0, asr #21
00964c14: ldrshteq sp, [r3], -r0
00964c18: ldrshteq fp, [r8], -r0
00964c1c: eorseq   fp, r8, r8, lsr sl
00964c20: eorseq   fp, r8, r4, lsl #20
00964c24: eorseq   sp, r3, r8, lsl #26
00964c28: eorseq   sp, r3, ip, lsl #26
00964c2c: eorseq   sp, r3, r0, lsl sp
00964c30: eorseq   ip, r8, r8, asr #26
00964c34: eorseq   r0, r4, ip, asr r0
00964c38: eorseq   r0, r4, r4, rrx
00964c3c: eorseq   r0, r4, ip, rrx
00964c40: eorseq   r0, r4, r4, ror r0
00964c44: eorseq   r0, r4, ip, ror r0
00964c48: eorseq   r0, r4, r4, lsl #1
00964c4c: eorseq   r0, r4, ip, lsl #1
00964c50: mlaseq   r4, ip, r0, r0
00964c54: eorseq   r0, r4, r4, lsr #1
00964c58: eorseq   r8, r8, r8, lsr #7
00964c5c: ldrhteq  r0, [r4], -r4
00964c60: ldrhteq  r8, [r8], -r0
00964c64: eorseq   sl, r8, r0, lsr sp
00964c68: eorseq   sl, r8, r4, ror sp
00964c6c: eorseq   sl, r8, ip, ror sp
00964c70: ldrhteq  r0, [r4], -ip
00964c74: eorseq   fp, r8, r0, lsl r1
00964c78: eorseq   r0, r4, r0, asr #1
00964c7c: ldrsbteq r0, [r4], -ip
00964c80: eorseq   r0, r4, r4, ror #1
00964c84: ldrshteq r0, [r4], -r0
00964c88: ldrshteq r0, [r4], -ip
00964c8c: eorseq   r0, r4, r4, lsl #2
00964c90: eorseq   r0, r4, ip, lsl #2
00964c94: eorseq   r0, r4, r4, lsl r1
00964c98: eorseq   r0, r4, ip, lsl r1
00964c9c: eorseq   r6, sb, r0, lsr #9
00964ca0: eorseq   r5, sb, r0, asr #18
00964ca4: mlaseq   sb, ip, r4, r6
00964ca8: eorseq   r5, sb, r4, asr #18
00964cac: eorseq   r5, sb, r8, asr #18

# _ZN13ObjectManager15MarkForDeletionEP10ObjectBase
003432f8: push     {r4, r5, lr}
003432fc: mov      r4, r0
00343300: ldr      r3, [r4, #0x3c]!
00343304: sub      sp, sp, #0xc
00343308: mov      r5, r0
0034330c: cmp      r3, r4
00343310: beq      #0x343330
00343314: ldr      r2, [r3, #8]
00343318: cmp      r2, r1
0034331c: beq      #0x343330
00343320: ldr      r3, [r3]
00343324: cmp      r4, r3
00343328: bne      #0x343314
0034332c: mov      r3, r4
00343330: cmp      r4, r3
00343334: beq      #0x343340
00343338: add      sp, sp, #0xc
0034333c: pop      {r4, r5, pc}
00343340: mov      r0, r4
00343344: str      r1, [sp, #4]
00343348: bl       #0x343168 ; 
0034334c: ldr      r1, [sp, #4]
00343350: str      r1, [r0, #8]
00343354: ldr      r3, [r5, #0x40]
00343358: str      r4, [r0]
0034335c: str      r3, [r0, #4]
00343360: str      r0, [r3]
00343364: str      r0, [r5, #0x40]
00343368: b        #0x343338

# _ZN8RoomZoneD2Ev
003967a0: push     {r4, r5, r6, lr}
003967a4: ldr      r2, [pc, #0x6c]
003967a8: ldr      r3, [pc, #0x6c]
003967ac: mov      r6, r0
003967b0: add      r2, pc, r2
003967b4: ldr      r0, [r0, #0x394]
003967b8: ldr      r3, [r2, r3]
003967bc: add      r5, r6, #0x394
003967c0: cmp      r0, r5
003967c4: add      r1, r3, #0xf4
003967c8: add      ip, r3, #8
003967cc: add      r3, r3, #0xe8
003967d0: str      ip, [r6]
003967d4: str      r3, [r6, #4]
003967d8: str      r1, [r6, #0x24]
003967dc: bne      #0x3967e8
003967e0: b        #0x396800
003967e4: mov      r0, r4
003967e8: ldr      r4, [r0]
003967ec: mov      r1, #0xc
003967f0: bl       #0x708f00 ; 
003967f4: cmp      r4, r5
003967f8: bne      #0x3967e4
003967fc: mov      r0, r5
00396800: str      r0, [r6, #0x394]
00396804: str      r0, [r5, #4]
00396808: mov      r0, r6
0039680c: bl       #0x397bc4 ; _ZN4ZoneD2Ev
00396810: mov      r0, r6
00396814: pop      {r4, r5, r6, pc}
00396818: subseq   lr, pc, r0, ror #5
0039681c: andeq    r2, r0, r4, ror r7

# _ZNK12ObjectHandle9GetObjectEb
0033ff8c: b        #0x33fdc0

# _ZN13ObjectManager27FlushAllOrphanRenderObjectsEv
003454dc: push     {r4, r5, r6, lr}
003454e0: mov      r5, r0
003454e4: ldr      r4, [r5, #4]!
003454e8: mov      r6, #0
003454ec: cmp      r5, r4
003454f0: beq      #0x345520
003454f4: ldr      r3, [r4, #8]
003454f8: cmp      r3, #0
003454fc: beq      #0x345514
00345500: mov      r0, r3
00345504: ldr      r3, [r3]
00345508: mov      lr, pc
0034550c: ldr      pc, [r3, #4]
00345510: str      r6, [r4, #8]
00345514: ldr      r4, [r4]
00345518: cmp      r5, r4
0034551c: bne      #0x3454f4
00345520: mov      r0, r5
00345524: pop      {r4, r5, r6, lr}
00345528: b        #0x34526c

# _ZN13ObjectManager5FlushEv
003496b8: push     {r4, r5, r6, r7, r8, lr}
003496bc: mov      r4, r0
003496c0: sub      sp, sp, #8
003496c4: add      r5, r0, #0x60
003496c8: ldr      r6, [r0, #0x60]
003496cc: b        #0x3496e4
003496d0: ldr      r0, [r6, #8]
003496d4: bl       #0x38ba6c ; _ZN10GameObject15FlushTargetListEv
003496d8: ldr      r0, [r6, #8]
003496dc: bl       #0x33ddb4 ; _ZN10ObjectBase6DeleteEv
003496e0: ldr      r6, [r6]
003496e4: cmp      r5, r6
003496e8: bne      #0x3496d0
003496ec: ldr      r6, [r4, #0x60]
003496f0: b        #0x349708
003496f4: ldr      r0, [r6, #8]
003496f8: cmp      r0, #0
003496fc: addne    r0, r0, #0x3c8
00349700: bl       #0x3cb34c ; _ZN6CharAI15_UpdatePointersEv
00349704: ldr      r6, [r6]
00349708: cmp      r5, r6
0034970c: bne      #0x3496f4
00349710: ldr      r6, [r4, #0x14]
00349714: add      r7, r4, #0xc
00349718: mov      r8, #0
0034971c: cmp      r6, r7
00349720: beq      #0x3497a0
00349724: ldr      r3, [r6, #0x2c]
00349728: cmp      r3, #0
0034972c: beq      #0x349744
00349730: mov      r0, r3
00349734: ldr      r3, [r3]
00349738: mov      lr, pc
0034973c: ldr      pc, [r3, #4]
00349740: str      r8, [r6, #0x2c]
00349744: ldr      r2, [r6, #0xc]
00349748: cmp      r2, #0
0034974c: beq      #0x349768
00349750: mov      r6, r2
00349754: ldr      r3, [r6, #8]
00349758: cmp      r3, #0
0034975c: beq      #0x34971c
00349760: mov      r6, r3
00349764: b        #0x349754
00349768: ldr      r3, [r6, #4]
0034976c: ldr      r1, [r3, #0xc]
00349770: cmp      r6, r1
00349774: bne      #0x349790
00349778: mov      r6, r3
0034977c: ldr      r3, [r3, #4]
00349780: ldr      r2, [r3, #0xc]
00349784: cmp      r2, r6
00349788: beq      #0x349778
0034978c: ldr      r2, [r6, #0xc]
00349790: cmp      r2, r3
00349794: movne    r6, r3
00349798: cmp      r6, r7
0034979c: bne      #0x349724
003497a0: add      r0, r4, #0x90
003497a4: bl       #0x345914 ; _ZNSt4priv10_List_baseIP10GameObjectSaIS2_EE5clearEv
003497a8: ldr      r3, [r4, #0x1c]
003497ac: cmp      r3, #0
003497b0: bne      #0x349af0
003497b4: mov      r0, r5
003497b8: mov      r6, r4
003497bc: bl       #0x345b8c ; _ZNSt4priv10_List_baseIP9CharacterSaIS2_EE5clearEv
003497c0: ldr      r0, [r6, #0x68]!
003497c4: cmp      r0, r6
003497c8: bne      #0x3497d4
003497cc: b        #0x3497ec
003497d0: mov      r0, r5
003497d4: ldr      r5, [r0]
003497d8: mov      r1, #0xc
003497dc: bl       #0x708f00 ; 
003497e0: cmp      r5, r6
003497e4: bne      #0x3497d0
003497e8: mov      r0, r6
003497ec: str      r0, [r4, #0x6c]
003497f0: str      r0, [r4, #0x68]
003497f4: mov      r6, r4
003497f8: add      r0, r4, #0x100
003497fc: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349800: ldr      r0, [r6, #0x120]!
00349804: cmp      r6, r0
00349808: bne      #0x349814
0034980c: b        #0x349828
00349810: mov      r0, r5
00349814: ldr      r5, [r0]
00349818: mov      r1, #0xc
0034981c: bl       #0x708f00 ; 
00349820: cmp      r6, r5
00349824: bne      #0x349810
00349828: ldr      r3, [r4, #0x118]
0034982c: str      r6, [r4, #0x124]
00349830: str      r6, [r4, #0x120]
00349834: cmp      r3, #0
00349838: bne      #0x349ac8
0034983c: ldr      r3, [r4, #0x174]
00349840: cmp      r3, #0
00349844: beq      #0x34986c
00349848: add      r5, r4, #0x164
0034984c: mov      r0, r5
00349850: ldr      r1, [r4, #0x168]
00349854: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349858: mov      r3, #0
0034985c: str      r5, [r4, #0x170]
00349860: str      r3, [r4, #0x174]
00349864: str      r5, [r4, #0x16c]
00349868: str      r3, [r4, #0x168]
0034986c: ldr      r3, [r4, #0x18c]
00349870: cmp      r3, #0
00349874: beq      #0x34989c
00349878: add      r5, r4, #0x17c
0034987c: mov      r0, r5
00349880: ldr      r1, [r4, #0x180]
00349884: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349888: mov      r3, #0
0034988c: str      r5, [r4, #0x188]
00349890: str      r3, [r4, #0x18c]
00349894: str      r5, [r4, #0x184]
00349898: str      r3, [r4, #0x180]
0034989c: ldr      r3, [r4, #0x1a4]
003498a0: cmp      r3, #0
003498a4: beq      #0x3498cc
003498a8: add      r5, r4, #0x194
003498ac: mov      r0, r5
003498b0: ldr      r1, [r4, #0x198]
003498b4: bl       #0x345f04 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt3setIsS2_SaIsEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
003498b8: mov      r3, #0
003498bc: str      r5, [r4, #0x1a0]
003498c0: str      r3, [r4, #0x1a4]
003498c4: str      r5, [r4, #0x19c]
003498c8: str      r3, [r4, #0x198]
003498cc: add      r0, r4, #0x128
003498d0: bl       #0x345a4c ; _ZNSt4priv10_List_baseIiSaIiEE5clearEv
003498d4: mov      r6, r4
003498d8: add      r0, r4, #0x130
003498dc: bl       #0x345a4c ; _ZNSt4priv10_List_baseIiSaIiEE5clearEv
003498e0: ldr      r0, [r6, #0x80]!
003498e4: cmp      r6, r0
003498e8: bne      #0x3498f4
003498ec: b        #0x349908
003498f0: mov      r0, r5
003498f4: ldr      r5, [r0]
003498f8: mov      r1, #0xc
003498fc: bl       #0x708f00 ; 
00349900: cmp      r6, r5
00349904: bne      #0x3498f0
00349908: add      r5, r4, #0x88
0034990c: mov      r0, r5
00349910: str      r6, [r4, #0x84]
00349914: str      r6, [r4, #0x80]
00349918: bl       #0x345914 ; _ZNSt4priv10_List_baseIP10GameObjectSaIS2_EE5clearEv
0034991c: mov      r1, r5
00349920: mov      r0, r4
00349924: bl       #0x3427a0 ; _ZN13ObjectManager14AddRoomObjectsEPSt4listIP10GameObjectSaIS2_EE
00349928: ldr      r3, [r4, #0x158]
0034992c: mov      r5, #0
00349930: str      r5, [r4, #0x138]
00349934: cmp      r3, r5
00349938: str      r5, [r4, #0x13c]
0034993c: str      r5, [r4, #0x140]
00349940: bne      #0x349aa4
00349944: add      r1, sp, #8
00349948: mov      r5, #0
0034994c: str      r5, [r1, #-4]!
00349950: mov      r0, r7
00349954: strb     r5, [r4, #0x160]
00349958: bl       #0x34952c ; _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIiEERS0_RKT_
0034995c: mov      r3, #1
00349960: str      r3, [r4, #0x4c]
00349964: add      r0, r4, #0x3c
00349968: str      r5, [r4, #0x58]
0034996c: str      r5, [r4, #0x50]
00349970: str      r5, [r4, #0x54]
00349974: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349978: add      r0, r4, #0x2c
0034997c: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349980: add      r0, r4, #0x44
00349984: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349988: add      r0, r4, #0x34
0034998c: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00349990: mov      r6, r4
00349994: add      r0, r4, #0x70
00349998: bl       #0x345b8c ; _ZNSt4priv10_List_baseIP9CharacterSaIS2_EE5clearEv
0034999c: ldr      r0, [r6, #0x24]!
003499a0: cmp      r0, r6
003499a4: bne      #0x3499b0
003499a8: b        #0x3499c8
003499ac: mov      r0, r5
003499b0: ldr      r5, [r0]
003499b4: mov      r1, #0xc
003499b8: bl       #0x708f00 ; 
003499bc: cmp      r5, r6
003499c0: bne      #0x3499ac
003499c4: mov      r0, r6
003499c8: mov      r5, #0
003499cc: str      r0, [r4, #0x28]
003499d0: str      r0, [r4, #0x24]
003499d4: str      r5, [r4, #0x7c]
003499d8: mov      r0, r4
003499dc: bl       #0x3454dc ; _ZN13ObjectManager27FlushAllOrphanRenderObjectsEv
003499e0: ldr      r3, [r4, #0xa8]
003499e4: cmp      r3, r5
003499e8: beq      #0x349a0c
003499ec: add      r6, r4, #0x98
003499f0: mov      r0, r6
003499f4: ldr      r1, [r4, #0x9c]
003499f8: bl       #0x345b4c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIiSaIiEEENS_10_Select1stIS8_EENS_11_MapTraitsTIS8_EESaIS8_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
003499fc: str      r6, [r4, #0xa4]
00349a00: str      r5, [r4, #0xa8]
00349a04: str      r6, [r4, #0xa0]
00349a08: str      r5, [r4, #0x9c]
00349a0c: ldr      r3, [r4, #0xc0]
00349a10: cmp      r3, #0
00349a14: beq      #0x349a3c
00349a18: add      r5, r4, #0xb0
00349a1c: mov      r0, r5
00349a20: ldr      r1, [r4, #0xb4]
00349a24: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349a28: mov      r3, #0
00349a2c: str      r5, [r4, #0xbc]
00349a30: str      r3, [r4, #0xc0]
00349a34: str      r5, [r4, #0xb8]
00349a38: str      r3, [r4, #0xb4]
00349a3c: ldr      r3, [r4, #0xd8]
00349a40: cmp      r3, #0
00349a44: beq      #0x349a6c
00349a48: add      r5, r4, #0xc8
00349a4c: mov      r0, r5
00349a50: ldr      r1, [r4, #0xcc]
00349a54: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349a58: mov      r3, #0
00349a5c: str      r5, [r4, #0xd4]
00349a60: str      r3, [r4, #0xd8]
00349a64: str      r5, [r4, #0xd0]
00349a68: str      r3, [r4, #0xcc]
00349a6c: ldr      r3, [r4, #0xf0]
00349a70: cmp      r3, #0
00349a74: beq      #0x349a9c
00349a78: add      r5, r4, #0xe0
00349a7c: mov      r0, r5
00349a80: ldr      r1, [r4, #0xe4]
00349a84: bl       #0x345f84 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349a88: mov      r3, #0
00349a8c: str      r3, [r4, #0xf0]
00349a90: str      r5, [r4, #0xec]
00349a94: str      r5, [r4, #0xe8]
00349a98: str      r3, [r4, #0xe4]
00349a9c: add      sp, sp, #8
00349aa0: pop      {r4, r5, r6, r7, r8, pc}
00349aa4: add      r6, r4, #0x148
00349aa8: mov      r0, r6
00349aac: ldr      r1, [r4, #0x14c]
00349ab0: bl       #0x345c94 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349ab4: str      r6, [r4, #0x154]
00349ab8: str      r5, [r4, #0x158]
00349abc: str      r6, [r4, #0x150]
00349ac0: str      r5, [r4, #0x14c]
00349ac4: b        #0x349944
00349ac8: add      r5, r4, #0x108
00349acc: mov      r0, r5
00349ad0: ldr      r1, [r4, #0x10c]
00349ad4: bl       #0x3458d4 ; _ZNSt4priv8_Rb_treeIsSt4lessIsESt4pairIKsSt4listIP10ObjectBaseSaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349ad8: mov      r3, #0
00349adc: str      r5, [r4, #0x114]
00349ae0: str      r3, [r4, #0x118]
00349ae4: str      r5, [r4, #0x110]
00349ae8: str      r3, [r4, #0x10c]
00349aec: b        #0x34983c
00349af0: mov      r0, r7
00349af4: ldr      r1, [r4, #0x10]
00349af8: bl       #0x347ed4 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00349afc: mov      r3, #0
00349b00: str      r3, [r4, #0x1c]
00349b04: str      r7, [r4, #0x14]
00349b08: str      r3, [r4, #0x10]
00349b0c: str      r7, [r4, #0x18]
00349b10: b        #0x3497b4

# _ZN9Character11Ctrl_LookAtERK7Point3DIfE
003addbc: b        #0x393cec

# _ZN13ObjectManagerD0Ev
00349e70: push     {r4, lr}
00349e74: mov      r4, r0
00349e78: bl       #0x349b14 ; _ZN13ObjectManagerD1Ev
00349e7c: mov      r0, r4
00349e80: bl       #0x310440 ; _Z10CustomFreePv
00349e84: mov      r0, r4
00349e88: pop      {r4, pc}

# _ZN13ObjectManager18sProcessLostPacketEii
00348724: ldr      r3, [pc, #0x24]
00348728: ldr      r2, [pc, #0x24]
0034872c: mov      ip, r0
00348730: add      r3, pc, r3
00348734: ldr      r0, [r3, r2]
00348738: mov      r2, r1
0034873c: ldr      r0, [r0, #0x38]
00348740: cmp      r0, #0
00348744: bxeq     lr
00348748: mov      r1, ip
0034874c: b        #0x348644
00348750: rsbeq    ip, r4, r0, ror #6
00348754: strdeq   r3, r4, [r0], -r4

# _ZN13ObjectManager14AddRoomObjectsEPSt4listIP10GameObjectSaIS2_EE
003427a0: push     {r4, r5, r6, r7, lr}
003427a4: ldr      r6, [pc, #0x120]
003427a8: subs     r5, r1, #0
003427ac: sub      sp, sp, #0x14
003427b0: mov      r7, r0
003427b4: add      r6, pc, r6
003427b8: beq      #0x342844
003427bc: mov      r4, r7
003427c0: ldr      r3, [r4, #0x80]!
003427c4: cmp      r3, r4
003427c8: beq      #0x342814
003427cc: ldr      r2, [r3, #8]
003427d0: cmp      r5, r2
003427d4: beq      #0x3427e8
003427d8: ldr      r3, [r3]
003427dc: cmp      r4, r3
003427e0: bne      #0x3427cc
003427e4: mov      r3, r4
003427e8: cmp      r3, r4
003427ec: beq      #0x342814
003427f0: ldr      r3, [pc, #0xd8]
003427f4: ldr      r3, [r6, r3]
003427f8: ldr      r3, [r3]
003427fc: cmp      r3, #2
00342800: moveq    r3, #0
00342804: streq    r3, [r3]
00342808: beq      #0x342814
0034280c: cmp      r3, #1
00342810: beq      #0x342898
00342814: mov      r3, #0xc
00342818: add      r0, sp, #0x10
0034281c: str      r3, [r0, #-4]!
00342820: bl       #0x708ec0 ; 
00342824: str      r5, [r0, #8]
00342828: ldr      r3, [r7, #0x84]
0034282c: str      r4, [r0]
00342830: str      r3, [r0, #4]
00342834: str      r0, [r3]
00342838: str      r0, [r7, #0x84]
0034283c: add      sp, sp, #0x14
00342840: pop      {r4, r5, r6, r7, pc}
00342844: ldr      r3, [pc, #0x84]
00342848: ldr      r3, [r6, r3]
0034284c: ldr      r3, [r3]
00342850: cmp      r3, #2
00342854: streq    r5, [r5]
00342858: beq      #0x3427bc
0034285c: cmp      r3, #1
00342860: bne      #0x3427bc
00342864: ldr      r0, [pc, #0x68]
00342868: ldr      r1, [pc, #0x68]
0034286c: ldr      r2, [pc, #0x68]
00342870: ldr      r0, [r6, r0]
00342874: ldr      r3, [pc, #0x64]
00342878: movw     ip, #0x909
0034287c: add      r1, pc, r1
00342880: add      r2, pc, r2
00342884: add      r3, pc, r3
00342888: add      r0, r0, #0xa8
0034288c: str      ip, [sp]
00342890: bl       #0x30e004 ; 
00342894: b        #0x3427bc
00342898: ldr      r0, [pc, #0x34]
0034289c: ldr      r1, [pc, #0x40]
003428a0: ldr      r2, [pc, #0x40]
003428a4: ldr      r0, [r6, r0]
003428a8: ldr      r3, [pc, #0x3c]
003428ac: movw     ip, #0x90a
003428b0: add      r1, pc, r1
003428b4: add      r2, pc, r2
003428b8: add      r3, pc, r3
003428bc: add      r0, r0, #0xa8
003428c0: str      ip, [sp]
003428c4: bl       #0x30e004 ; 
003428c8: b        #0x342814

# _ZN13ObjectManager18NetworkUnInitLevelEv
00340bc4: push     {r4, lr}
00340bc8: mov      r4, r0
00340bcc: mov      r0, #3
00340bd0: bl       #0x8152c0 ; _ZN14CPacketManager20UnregisterPacketSlotE12PACKET_SLOTS
00340bd4: mov      r3, #0
00340bd8: strb     r3, [r4, #0x1ac]
00340bdc: pop      {r4, pc}

# _ZN13ObjectManager19HandleNoRoomObjectsEv
00345954: push     {r4, r5, r6, r7, r8, lr}
00345958: add      r5, r0, #0x88
0034595c: mov      r7, r0
00345960: mov      r0, r5
00345964: bl       #0x345914 ; _ZNSt4priv10_List_baseIP10GameObjectSaIS2_EE5clearEv
00345968: ldr      r4, [r7, #0x14]
0034596c: add      r6, r7, #0xc
00345970: cmp      r6, r4
00345974: beq      #0x3459d4
00345978: ldr      r8, [r4, #0x2c]
0034597c: cmp      r8, #0
00345980: beq      #0x3459a8
00345984: ldr      r3, [r8]
00345988: mov      r0, r8
0034598c: mov      lr, pc
00345990: ldr      pc, [r3, #0x20]
00345994: cmp      r0, #0
00345998: beq      #0x3459a8
0034599c: ldr      r3, [r8, #0x2f4]
003459a0: cmp      r3, #0
003459a4: beq      #0x345a0c
003459a8: ldr      r2, [r4, #0xc]
003459ac: cmp      r2, #0
003459b0: bne      #0x3459bc
003459b4: b        #0x3459d8
003459b8: mov      r2, r3
003459bc: ldr      r3, [r2, #8]
003459c0: cmp      r3, #0
003459c4: bne      #0x3459b8
003459c8: mov      r4, r2
003459cc: cmp      r6, r4
003459d0: bne      #0x345978
003459d4: pop      {r4, r5, r6, r7, r8, pc}
003459d8: ldr      r3, [r4, #4]
003459dc: ldr      r1, [r3, #0xc]
003459e0: cmp      r4, r1
003459e4: bne      #0x345a00
003459e8: mov      r4, r3
003459ec: ldr      r3, [r3, #4]
003459f0: ldr      r2, [r3, #0xc]
003459f4: cmp      r2, r4
003459f8: beq      #0x3459e8
003459fc: ldr      r2, [r4, #0xc]
00345a00: cmp      r3, r2
00345a04: movne    r4, r3
00345a08: b        #0x345970
00345a0c: ldr      r3, [r7, #0x88]
00345a10: cmp      r3, r5
00345a14: beq      #0x345a3c
00345a18: ldr      r2, [r3, #8]
00345a1c: cmp      r8, r2
00345a20: beq      #0x345a34
00345a24: ldr      r3, [r3]
00345a28: cmp      r5, r3
00345a2c: bne      #0x345a18
00345a30: mov      r3, r5
00345a34: cmp      r3, r5
00345a38: bne      #0x3459a8
00345a3c: mov      r1, r8
00345a40: mov      r0, r7
00345a44: bl       #0x344184 ; _ZN13ObjectManager15AddNoRoomObjectEP10GameObject
00345a48: b        #0x3459a8

# _ZN13ObjectManager12DoCharAIInitEv
0034064c: push     {r4, r5, r6, lr}
00340650: mov      r6, r0
00340654: ldr      r4, [r6, #0x2c]!
00340658: cmp      r6, r4
0034065c: beq      #0x34068c
00340660: ldr      r5, [r4, #8]
00340664: subs     r0, r5, #0
00340668: beq      #0x340680
0034066c: ldr      r3, [r5]
00340670: mov      lr, pc
00340674: ldr      pc, [r3, #0x24]
00340678: cmp      r0, #0
0034067c: bne      #0x340690
00340680: ldr      r4, [r4]
00340684: cmp      r6, r4
00340688: bne      #0x340660
0034068c: pop      {r4, r5, r6, pc}
00340690: add      r5, r5, #0x3c8
00340694: mov      r0, r5
00340698: bl       #0x3cfd7c ; _ZN6CharAI16AI_ScriptCleanUpEv
0034069c: mov      r0, r5
003406a0: bl       #0x3cfde4 ; _ZN6CharAI13AI_ScriptInitEv
003406a4: ldr      r4, [r4]
003406a8: b        #0x340684

# _ZThn16_N17v2MixedController11Ctrl_LookAtEP10GameObject
00408c3c: sub      r0, r0, #0x10
00408c40: b        #0x408c44

# _ZNK13ObjectManager16GetLightBaseListERSt4listIP9LightBaseSaIS2_EE
0034336c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00343370: ldr      r7, [pc, #0xd4]
00343374: ldr      r4, [r0, #0x14]
00343378: sub      sp, sp, #8
0034337c: mov      r6, r1
00343380: add      r5, r0, #0xc
00343384: add      r7, pc, r7
00343388: mov      sl, #0xc
0034338c: add      r8, sp, #4
00343390: cmp      r5, r4
00343394: beq      #0x343410
00343398: ldr      r0, [r4, #0x2c]
0034339c: cmp      r0, #0
003433a0: beq      #0x3433e4
003433a4: add      r0, r0, #4
003433a8: bl       #0x510b4c ; _ZN11PropertyMap16GetThisClassNameEv
003433ac: mov      r1, r7
003433b0: bl       #0x30e31c ; 
003433b4: cmp      r0, #0
003433b8: bne      #0x3433e4
003433bc: mov      r0, r8
003433c0: ldr      sb, [r4, #0x2c]
003433c4: str      sl, [sp, #4]
003433c8: bl       #0x708ec0 ; 
003433cc: str      sb, [r0, #8]
003433d0: ldr      r3, [r6, #4]
003433d4: str      r6, [r0]
003433d8: str      r3, [r0, #4]
003433dc: str      r0, [r3]
003433e0: str      r0, [r6, #4]
003433e4: ldr      r2, [r4, #0xc]
003433e8: cmp      r2, #0
003433ec: bne      #0x3433f8
003433f0: b        #0x343418
003433f4: mov      r2, r3
003433f8: ldr      r3, [r2, #8]
003433fc: cmp      r3, #0
00343400: bne      #0x3433f4
00343404: mov      r4, r2
00343408: cmp      r5, r4
0034340c: bne      #0x343398
00343410: add      sp, sp, #8
00343414: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00343418: ldr      r3, [r4, #4]
0034341c: ldr      r1, [r3, #0xc]
00343420: cmp      r4, r1
00343424: bne      #0x343440
00343428: mov      r4, r3
0034342c: ldr      r3, [r3, #4]
00343430: ldr      r2, [r3, #0xc]
00343434: cmp      r2, r4
00343438: beq      #0x343428
0034343c: ldr      r2, [r4, #0xc]
00343440: cmp      r2, r3
00343444: movne    r4, r3
00343448: b        #0x343390
0034344c: ldrsbeq  ip, [r7], #-0xfc

# _ZN6CharAIC2Ev
003cebf0: ldr      r3, [pc, #0x14c]
003cebf4: ldr      r2, [pc, #0x14c]
003cebf8: push     {r4, r5, lr}
003cebfc: add      r3, pc, r3
003cec00: ldr      r2, [r3, r2]
003cec04: mov      r4, r0
003cec08: mov      r1, #0
003cec0c: add      r2, r2, #8
003cec10: str      r2, [r4]
003cec14: ldr      r2, [pc, #0x130]
003cec18: mov      r0, #1
003cec1c: mvn      ip, #0
003cec20: mov      r5, r4
003cec24: strb     r0, [r4, #0x55]
003cec28: str      r1, [r4, #8]
003cec2c: str      r1, [r4, #0xc]
003cec30: strb     r1, [r4, #0x18]
003cec34: str      r1, [r4, #0x1c]
003cec38: str      r1, [r4, #0x20]
003cec3c: strb     r1, [r4, #0x24]
003cec40: str      r1, [r4, #0x28]
003cec44: strb     r1, [r4, #0x2c]
003cec48: str      r1, [r4, #0x30]
003cec4c: str      r1, [r4, #0x34]
003cec50: str      r1, [r4, #0x3c]
003cec54: str      r1, [r4, #0x40]
003cec58: str      r1, [r4, #0x44]
003cec5c: strb     r1, [r4, #0x49]
003cec60: strb     r0, [r4, #0x4a]
003cec64: strb     r0, [r4, #0x4b]
003cec68: strb     r1, [r4, #0x4c]
003cec6c: strb     r0, [r4, #0x4d]
003cec70: str      r1, [r4, #0x50]
003cec74: strb     r0, [r4, #0x54]
003cec78: str      r1, [r4, #0x58]
003cec7c: mov      r0, r4
003cec80: str      r1, [r4, #0x60]
003cec84: str      ip, [r4, #0x10]
003cec88: str      ip, [r4, #0x14]
003cec8c: str      ip, [r4, #0x38]
003cec90: strb     r1, [r5, #0x5c]!
003cec94: str      r5, [r4, #0x68]
003cec98: str      r5, [r4, #0x64]
003cec9c: str      r1, [r4, #0x6c]
003ceca0: str      r1, [r4, #0x80]
003ceca4: strb     r1, [r0, #0x7c]!
003ceca8: ldr      r5, [r3, r2]
003cecac: mov      r2, r4
003cecb0: str      r0, [r4, #0x88]
003cecb4: str      r0, [r4, #0x84]
003cecb8: str      r1, [r4, #0x8c]
003cecbc: str      r1, [r4, #0x98]
003cecc0: add      r0, r4, #0xac
003cecc4: strb     r1, [r2, #0x94]!
003cecc8: str      r2, [r4, #0xa0]
003ceccc: str      r0, [r4, #0xb0]
003cecd0: str      ip, [r4, #0xcc]
003cecd4: strb     r1, [r4, #0xd1]
003cecd8: str      r2, [r4, #0x9c]
003cecdc: str      r1, [r4, #0xa4]
003cece0: str      r0, [r4, #0xac]
003cece4: str      r1, [r4, #0xb4]
003cece8: str      r1, [r4, #0xb8]
003cecec: str      r1, [r4, #0xbc]
003cecf0: str      r1, [r4, #0xc0]
003cecf4: str      r1, [r4, #0xc4]
003cecf8: str      r1, [r4, #0xc8]
003cecfc: strb     r1, [r4, #0xd0]
003ced00: ldr      r1, [r5, #0x18]
003ced04: ldr      r2, [r5, #0x10]
003ced08: sub      sp, sp, #0xc
003ced0c: sub      r3, r1, #4
003ced10: cmp      r2, r3
003ced14: str      r4, [sp, #4]
003ced18: beq      #0x3ced38
003ced1c: str      r4, [r2]
003ced20: ldr      r3, [r5, #0x10]
003ced24: add      r3, r3, #4
003ced28: str      r3, [r5, #0x10]
003ced2c: mov      r0, r4
003ced30: add      sp, sp, #0xc
003ced34: pop      {r4, r5, pc}
003ced38: add      r0, sp, #4
003ced3c: bl       #0x3ce810 ; 
003ced40: b        #0x3ced2c

# _ZN13ObjectManager6RemoveE12ObjectHandle
00348ea4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00348ea8: sub      sp, sp, #0x20
00348eac: add      r4, sp, #4
00348eb0: mov      r5, r0
00348eb4: mov      r0, r4
00348eb8: stm      r4, {r1, r2, r3}
00348ebc: bl       #0x33fee4 ; _ZN12ObjectHandlecvP10GameObjectEv
00348ec0: subs     r7, r0, #0
00348ec4: beq      #0x348f34
00348ec8: ldr      r0, [r7, #0x2f4]
00348ecc: cmp      r0, #0
00348ed0: beq      #0x348edc
00348ed4: mov      r1, r7
00348ed8: bl       #0x3968cc ; _ZN8RoomZone12RemoveObjectEP10GameObject
00348edc: mov      r0, r5
00348ee0: mov      r1, r7
00348ee4: bl       #0x3462b8 ; _ZN13ObjectManager18RemoveNoRoomObjectEP10GameObject
00348ee8: mov      r2, r5
00348eec: ldr      r0, [r2, #0x90]!
00348ef0: cmp      r0, r2
00348ef4: beq      #0x348f14
00348ef8: ldr      r3, [r0, #8]
00348efc: cmp      r7, r3
00348f00: beq      #0x348f14
00348f04: ldr      r0, [r0]
00348f08: cmp      r2, r0
00348f0c: bne      #0x348ef8
00348f10: mov      r0, r2
00348f14: cmp      r2, r0
00348f18: beq      #0x348f34
00348f1c: ldr      r3, [r0]
00348f20: ldr      r2, [r0, #4]
00348f24: mov      r1, #0xc
00348f28: str      r3, [r2]
00348f2c: str      r2, [r3, #4]
00348f30: bl       #0x708f00 ; 
00348f34: mov      r0, r4
00348f38: mov      r1, #0
00348f3c: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
00348f40: mov      r8, r5
00348f44: mov      sl, r0
00348f48: ldr      r0, [r8, #0x2c]!
00348f4c: cmp      r8, r0
00348f50: beq      #0x348f70
00348f54: ldr      r3, [r0, #8]
00348f58: ldr      r6, [r0]
00348f5c: cmp      sl, r3
00348f60: beq      #0x3490ec
00348f64: mov      r0, r6
00348f68: cmp      r8, r0
00348f6c: bne      #0x348f54
00348f70: mov      r0, r4
00348f74: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00348f78: mov      r8, r5
00348f7c: mov      sl, r0
00348f80: ldr      r0, [r8, #0x70]!
00348f84: cmp      r8, r0
00348f88: beq      #0x348fa8
00348f8c: ldr      r3, [r0, #8]
00348f90: ldr      r6, [r0]
00348f94: cmp      sl, r3
00348f98: beq      #0x349108
00348f9c: mov      r0, r6
00348fa0: cmp      r8, r0
00348fa4: bne      #0x348f8c
00348fa8: mov      r0, r4
00348fac: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00348fb0: subs     sl, r0, #0
00348fb4: beq      #0x348fec
00348fb8: mov      r8, r5
00348fbc: ldr      r0, [r8, #0x60]!
00348fc0: cmp      r8, r0
00348fc4: beq      #0x348fe4
00348fc8: ldr      r3, [r0, #8]
00348fcc: ldr      r6, [r0]
00348fd0: cmp      sl, r3
00348fd4: beq      #0x349124
00348fd8: mov      r0, r6
00348fdc: cmp      r8, r0
00348fe0: bne      #0x348fc8
00348fe4: add      r0, sl, #0x3c8
00348fe8: bl       #0x3d2ff8 ; _ZN6CharAI15RemoveFromGroupEv
00348fec: mov      r0, r4
00348ff0: mov      r1, #0
00348ff4: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
00348ff8: subs     r8, r0, #0
00348ffc: beq      #0x34900c
00349000: ldr      r3, [r8, #0xf4]
00349004: cmp      r3, #5
00349008: beq      #0x349200
0034900c: bl       #0x7fd794 ; _Z9GetOnlinev
00349010: ldrb     r3, [r0, #5]
00349014: cmp      r3, #0
00349018: addeq    sl, r5, #0xc
0034901c: beq      #0x349058
00349020: mov      sb, r5
00349024: ldr      r6, [sb, #0x100]!
00349028: add      sl, r5, #0xc
0034902c: b        #0x349048
00349030: ldr      r8, [r6, #8]
00349034: bl       #0x33fc88 ; _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
00349038: ldr      r3, [r0, #0x18]
0034903c: cmp      r8, r3
00349040: beq      #0x349180
00349044: ldr      r6, [r6]
00349048: cmp      r6, sb
0034904c: mov      r0, sl
00349050: mov      r1, r4
00349054: bne      #0x349030
00349058: ldr      r3, [r5, #0x50]
0034905c: sub      r3, r3, #1
00349060: str      r3, [r5, #0x50]
00349064: ldrb     r3, [r7, #0x2fc]
00349068: cmp      r3, #0
0034906c: beq      #0x349140
00349070: mov      r1, r4
00349074: mov      r0, sl
00349078: bl       #0x33fc88 ; _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
0034907c: ldr      r1, [r0, #0x18]
00349080: mov      r0, r5
00349084: bl       #0x343188 ; _ZN13ObjectManager29AddOrphanRenderObjectToDeleteEP10ObjectBase
00349088: ldr      r3, [r5, #0x10]
0034908c: ldr      r0, [sp, #4]
00349090: cmp      r3, #0
00349094: beq      #0x3490d8
00349098: mov      r1, sl
0034909c: b        #0x3490a4
003490a0: mov      r3, r2
003490a4: ldr      r2, [r3, #0x10]
003490a8: cmp      r0, r2
003490ac: ldrgt    r2, [r3, #0xc]
003490b0: ldrle    r2, [r3, #8]
003490b4: movgt    r3, r1
003490b8: mov      r1, r3
003490bc: cmp      r2, #0
003490c0: bne      #0x3490a0
003490c4: cmp      sl, r3
003490c8: beq      #0x3490d8
003490cc: ldr      r2, [r3, #0x10]
003490d0: cmp      r0, r2
003490d4: bge      #0x34916c
003490d8: ldr      r3, [r5, #0x78]
003490dc: add      r3, r3, #1
003490e0: str      r3, [r5, #0x78]
003490e4: add      sp, sp, #0x20
003490e8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003490ec: ldr      r3, [r0, #4]
003490f0: mov      r1, #0xc
003490f4: str      r6, [r3]
003490f8: str      r3, [r6, #4]
003490fc: bl       #0x708f00 ; 
00349100: mov      r0, r6
00349104: b        #0x348f68
00349108: ldr      r3, [r0, #4]
0034910c: mov      r1, #0xc
00349110: str      r6, [r3]
00349114: str      r3, [r6, #4]
00349118: bl       #0x708f00 ; 
0034911c: mov      r0, r6
00349120: b        #0x348fa0
00349124: ldr      r3, [r0, #4]
00349128: mov      r1, #0xc
0034912c: str      r6, [r3]
00349130: str      r3, [r6, #4]
00349134: bl       #0x708f00 ; 
00349138: mov      r0, r6
0034913c: b        #0x348fdc
00349140: mov      r1, r4
00349144: mov      r0, sl
00349148: bl       #0x33fc88 ; _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
0034914c: ldr      r3, [r0, #0x18]
00349150: cmp      r3, #0
00349154: beq      #0x349088
00349158: mov      r0, r3
0034915c: ldr      r3, [r3]
00349160: mov      lr, pc
00349164: ldr      pc, [r3, #4]
00349168: b        #0x349088
0034916c: add      r1, sp, #0x20
00349170: str      r3, [r1, #-4]!
00349174: mov      r0, sl
00349178: bl       #0x347f58 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE5eraseENS_17_Rb_tree_iteratorIS6_SA_EE
0034917c: b        #0x3490d8
00349180: add      sb, sp, #0x10
00349184: mov      r1, r8
00349188: mov      r0, sb
0034918c: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00349190: mov      r0, sb
00349194: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00349198: subs     r3, r0, #0
0034919c: beq      #0x3491b4
003491a0: ldr      r3, [r3]
003491a4: mov      lr, pc
003491a8: ldr      pc, [r3, #0x28]
003491ac: cmp      r0, #0
003491b0: bne      #0x3491e0
003491b4: ldr      sb, [r8, #0x108]
003491b8: add      r8, r5, #0x120
003491bc: mov      r0, r8
003491c0: bl       #0x343148 ; 
003491c4: uxth     sb, sb
003491c8: strh     sb, [r0, #8]
003491cc: ldr      r3, [r5, #0x124]
003491d0: str      r8, [r0]
003491d4: str      r3, [r0, #4]
003491d8: str      r0, [r3]
003491dc: str      r0, [r5, #0x124]
003491e0: ldr      r3, [r6]
003491e4: ldr      r2, [r6, #4]
003491e8: mov      r0, r6
003491ec: mov      r1, #0xc
003491f0: str      r3, [r2]
003491f4: str      r2, [r3, #4]
003491f8: bl       #0x708f00 ; 
003491fc: b        #0x349058
00349200: mov      sl, r5
00349204: ldr      r0, [sl, #0x68]!
00349208: cmp      sl, r0
0034920c: beq      #0x34900c
00349210: ldr      r3, [r0, #8]
00349214: ldr      r6, [r0]
00349218: cmp      r8, r3
0034921c: movne    r0, r6
00349220: bne      #0x349208
00349224: ldr      r3, [r0, #4]
00349228: mov      r1, #0xc
0034922c: str      r6, [r3]
00349230: str      r3, [r6, #4]
00349234: bl       #0x708f00 ; 
00349238: mov      r0, r6
0034923c: b        #0x349208

# _ZN10GameObjectC1EN10ObjectBase6GO_IDSE
0038c130: push     {r4, r5, r6, r7, lr}
0038c134: ldr      r6, [pc, #0x254]
0038c138: sub      sp, sp, #0xc
0038c13c: mov      r4, r0
0038c140: bl       #0x33f310 ; _ZN10ObjectBaseC2ENS_6GO_IDSE
0038c144: ldr      r2, [pc, #0x248]
0038c148: add      r6, pc, r6
0038c14c: mov      r3, #0
0038c150: ldr      r2, [r6, r2]
0038c154: mov      r5, #0
0038c158: str      r3, [r4, #0x120]
0038c15c: add      r1, r2, #0xe4
0038c160: add      r0, r2, #8
0038c164: add      r2, r2, #0xd8
0038c168: str      r2, [r4, #4]
0038c16c: str      r1, [r4, #0x24]
0038c170: str      r0, [r4]
0038c174: str      r3, [r4, #0x124]
0038c178: str      r3, [r4, #0x128]
0038c17c: str      r3, [r4, #0x12c]
0038c180: str      r3, [r4, #0x130]
0038c184: str      r3, [r4, #0x134]
0038c188: str      r3, [r4, #0x138]
0038c18c: str      r3, [r4, #0x13c]
0038c190: str      r3, [r4, #0x140]
0038c194: str      r3, [r4, #0x144]
0038c198: str      r3, [r4, #0x148]
0038c19c: str      r3, [r4, #0x14c]
0038c1a0: str      r3, [r4, #0x150]
0038c1a4: str      r3, [r4, #0x154]
0038c1a8: str      r3, [r4, #0x158]
0038c1ac: str      r3, [r4, #0x160]
0038c1b0: str      r3, [r4, #0x164]
0038c1b4: str      r3, [r4, #0x168]
0038c1b8: str      r3, [r4, #0x16c]
0038c1bc: str      r3, [r4, #0x170]
0038c1c0: str      r3, [r4, #0x174]
0038c1c4: str      r3, [r4, #0x178]
0038c1c8: str      r3, [r4, #0x184]
0038c1cc: str      r3, [r4, #0x188]
0038c1d0: str      r3, [r4, #0x18c]
0038c1d4: str      r3, [r4, #0x190]
0038c1d8: str      r3, [r4, #0x194]
0038c1dc: strb     r5, [r4, #0x15c]
0038c1e0: str      r5, [r4, #0x180]
0038c1e4: add      r0, r4, #0x1c8
0038c1e8: str      r3, [r4, #0x198]
0038c1ec: str      r3, [r4, #0x1c0]
0038c1f0: str      r3, [r4, #0x19c]
0038c1f4: str      r3, [r4, #0x1a0]
0038c1f8: str      r3, [r4, #0x1a4]
0038c1fc: str      r3, [r4, #0x1a8]
0038c200: str      r3, [r4, #0x1ac]
0038c204: str      r3, [r4, #0x1b0]
0038c208: strb     r5, [r4, #0x1b4]
0038c20c: strb     r5, [r4, #0x1b5]
0038c210: str      r3, [r4, #0x1b8]
0038c214: str      r3, [r4, #0x1bc]
0038c218: strb     r5, [r4, #0x1c4]
0038c21c: bl       #0x524644 ; _ZN8PFObjectC1Ev
0038c220: add      r3, r4, #0x278
0038c224: mvn      r7, #0
0038c228: mov      r2, #0x64
0038c22c: str      r2, [r4, #0x274]
0038c230: mov      r0, r3
0038c234: str      r3, [r4, #0x288]
0038c238: str      r3, [r4, #0x28c]
0038c23c: str      r5, [r4, #0x26c]
0038c240: str      r7, [r4, #0x270]
0038c244: mov      r1, #0x10
0038c248: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c24c: ldr      r2, [r4, #0x288]
0038c250: add      r3, r4, #0x290
0038c254: mov      r0, r3
0038c258: strb     r5, [r2]
0038c25c: mov      r1, #0x10
0038c260: str      r3, [r4, #0x2a0]
0038c264: str      r3, [r4, #0x2a4]
0038c268: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c26c: ldr      r2, [r4, #0x2a0]
0038c270: add      r3, r4, #0x2a8
0038c274: mov      r0, r3
0038c278: strb     r5, [r2]
0038c27c: mov      r1, #0x10
0038c280: str      r3, [r4, #0x2b8]
0038c284: str      r3, [r4, #0x2bc]
0038c288: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c28c: ldr      r2, [r4, #0x2b8]
0038c290: add      r3, r4, #0x2c0
0038c294: mov      r0, r3
0038c298: strb     r5, [r2]
0038c29c: mov      r1, #0x10
0038c2a0: str      r3, [r4, #0x2d0]
0038c2a4: str      r3, [r4, #0x2d4]
0038c2a8: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c2ac: ldr      r3, [r4, #0x2d0]
0038c2b0: mov      ip, #1
0038c2b4: add      r6, r4, #0x304
0038c2b8: strb     r5, [r3]
0038c2bc: mov      r2, ip
0038c2c0: strb     ip, [r4, #0x2ee]
0038c2c4: strb     ip, [r4, #0x2fb]
0038c2c8: str      r5, [r4, #0x2d8]
0038c2cc: str      r5, [r4, #0x2dc]
0038c2d0: str      r5, [r4, #0x2e0]
0038c2d4: str      r5, [r4, #0x2e4]
0038c2d8: str      r5, [r4, #0x2e8]
0038c2dc: strb     r5, [r4, #0x2ec]
0038c2e0: strb     r5, [r4, #0x2ed]
0038c2e4: strb     r5, [r4, #0x2ef]
0038c2e8: strb     r5, [r4, #0x2f0]
0038c2ec: str      r5, [r4, #0x2f4]
0038c2f0: strb     r5, [r4, #0x2f8]
0038c2f4: strb     r5, [r4, #0x2f9]
0038c2f8: strb     r5, [r4, #0x2fa]
0038c2fc: strb     r5, [r4, #0x2fc]
0038c300: str      r5, [r4, #0x300]
0038c304: mov      r3, r5
0038c308: mov      r1, r5
0038c30c: mov      r0, r6
0038c310: str      ip, [sp]
0038c314: bl       #0x4a2730 ; _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii
0038c318: add      r3, r4, #0x358
0038c31c: mov      r0, r3
0038c320: str      r3, [r4, #0x368]
0038c324: str      r3, [r4, #0x36c]
0038c328: mov      r1, #0x10
0038c32c: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0038c330: ldr      r1, [r4, #0x368]
0038c334: mov      r2, #0xc2000000
0038c338: mov      r3, #0x42000000
0038c33c: strb     r5, [r1]
0038c340: add      r2, r2, #0xc80000
0038c344: add      r3, r3, #0xc80000
0038c348: mov      r1, #0x370
0038c34c: strh     r7, [r4, r1]
0038c350: mov      r0, r4
0038c354: str      r2, [r4, #0x14c]
0038c358: str      r3, [r4, #0x158]
0038c35c: str      r2, [r4, #0x144]
0038c360: str      r2, [r4, #0x148]
0038c364: str      r3, [r4, #0x150]
0038c368: str      r3, [r4, #0x154]
0038c36c: strb     r5, [r4, #0x373]
0038c370: strb     r5, [r4, #0x372]
0038c374: bl       #0x38aac8 ; _ZN10GameObject18UpdateAbsoluteAABBEv
0038c378: mov      r0, r6
0038c37c: mov      r1, r4
0038c380: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
0038c384: mov      r0, r4
0038c388: add      sp, sp, #0xc
0038c38c: pop      {r4, r5, r6, r7, pc}
0038c390: rsbeq    r8, r0, r8, asr #18
0038c394: andeq    r2, r0, r0, ror sp

# _ZN13ObjectManager14GetObjectByPtrEP10ObjectBase
00340c54: push     {r4, r5, r6, lr}
00340c58: mov      r6, r2
00340c5c: sub      sp, sp, #0x10
00340c60: mov      r4, r0
00340c64: bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
00340c68: cmp      r6, #0
00340c6c: beq      #0x340c9c
00340c70: mov      r1, r6
00340c74: mov      r0, sp
00340c78: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00340c7c: ldr      r0, [sp]
00340c80: ldr      r1, [sp, #8]
00340c84: ldr      r2, [sp, #4]
00340c88: mov      r3, r4
00340c8c: str      r0, [r3], #4
00340c90: mov      r5, sp
00340c94: str      r1, [r3, #4]
00340c98: str      r2, [r4, #4]
00340c9c: mov      r0, r4
00340ca0: add      sp, sp, #0x10
00340ca4: pop      {r4, r5, r6, pc}

# _ZN13ObjectManager6Draw2DEv
0034024c: bx       lr

# _ZNK10ObjectBase9GetHandleEv
0033dd70: ldr      r3, [pc, #0x34]
0033dd74: ldr      r2, [pc, #0x34]
0033dd78: push     {r4, lr}
0033dd7c: add      r3, pc, r3
0033dd80: ldr      r2, [r3, r2]
0033dd84: ldr      ip, [r1, #0x2c]
0033dd88: mov      r4, r0
0033dd8c: ldr      lr, [r2, #0x38]
0033dd90: mov      r2, #0xc
0033dd94: ldr      r3, [lr, #0x78]
0033dd98: str      r3, [ip, #8]
0033dd9c: ldr      r1, [r1, #0x2c]
0033dda0: bl       #0x30df38 ; 
0033dda4: mov      r0, r4
0033dda8: pop      {r4, pc}
0033ddac: rsbeq    r6, r5, r4, lsl sp
0033ddb0: strdeq   r3, r4, [r0], -r4

# _ZN8RoomZoneC2EN10ObjectBase6GO_IDSE
003965c4: push     {r4, r5, r6, lr}
003965c8: mov      r2, #0
003965cc: mov      r3, #1
003965d0: ldr      r5, [pc, #0x50]
003965d4: mov      r4, r0
003965d8: bl       #0x397ca0 ; _ZN4ZoneC2EN10ObjectBase6GO_IDSEbb
003965dc: ldr      r3, [pc, #0x48]
003965e0: add      r5, pc, r5
003965e4: mov      r1, #1
003965e8: ldr      r3, [r5, r3]
003965ec: add      r2, r4, #0x394
003965f0: strb     r1, [r4, #0x389]
003965f4: add      r0, r3, #0xf4
003965f8: add      ip, r3, #8
003965fc: add      r3, r3, #0xe8
00396600: str      r3, [r4, #4]
00396604: mov      r3, #0
00396608: str      r0, [r4, #0x24]
0039660c: str      ip, [r4]
00396610: strb     r3, [r4, #0x390]
00396614: str      r2, [r4, #0x398]
00396618: strb     r1, [r4, #0x388]
0039661c: str      r2, [r4, #0x394]
00396620: mov      r0, r4
00396624: pop      {r4, r5, r6, pc}
00396628: ldrheq   lr, [pc], #-0x40
0039662c: andeq    r2, r0, r4, ror r7

# _ZN14v2Controllable11Ctrl_LookAtEP10GameObject
00404d6c: bx       lr

# _ZN13ObjectManager20DoRemoteUpdateUpdateEf
00340274: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00340278: mov      r7, r0
0034027c: ldr      r5, [r7, #0x100]!
00340280: mov      r8, r1
00340284: mvn      sb, #0
00340288: cmp      r7, r5
0034028c: mov      sl, #0
00340290: beq      #0x3402f0
00340294: ldr      r4, [r5, #8]
00340298: ldr      r3, [r4]
0034029c: mov      r0, r4
003402a0: mov      lr, pc
003402a4: ldr      pc, [r3, #0x54]
003402a8: mov      r1, #0x42000000
003402ac: cmp      r0, #0
003402b0: add      r1, r1, #0x480000
003402b4: beq      #0x3402e4
003402b8: ldr      r6, [r4, #0x114]
003402bc: mov      r0, r6
003402c0: bl       #0x30e4b4 ; 
003402c4: cmp      r0, #0
003402c8: mov      r1, r6
003402cc: mov      r0, r8
003402d0: strne    sl, [r4, #0x114]
003402d4: strne    sb, [r4, #0x110]
003402d8: bne      #0x3402e4
003402dc: bl       #0x30eba4 ; 
003402e0: str      r0, [r4, #0x114]
003402e4: ldr      r5, [r5]
003402e8: cmp      r7, r5
003402ec: bne      #0x340294
003402f0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6CharAI10AI_IsEnemyEPK10GameObject
003d574c: push     {r4, r5, r6, r7, r8, lr}
003d5750: ldr      r4, [pc, #0x2fc]
003d5754: subs     r7, r1, #0
003d5758: sub      sp, sp, #0x18
003d575c: mov      r5, r0
003d5760: add      r4, pc, r4
003d5764: beq      #0x3d59d0
003d5768: add      r6, sp, #0xc
003d576c: mov      r0, r6
003d5770: mov      r1, r7
003d5774: bl       #0x33dd70 ; _ZNK10ObjectBase9GetHandleEv
003d5778: mov      r0, r6
003d577c: mov      r1, #0
003d5780: bl       #0x33ff8c ; _ZNK12ObjectHandle9GetObjectEb
003d5784: subs     r6, r0, #0
003d5788: bne      #0x3d57bc
003d578c: cmp      r7, #0
003d5790: beq      #0x3d57b0
003d5794: ldr      r3, [r7]
003d5798: mov      r0, r7
003d579c: ldr      r1, [r5, #4]
003d57a0: mov      lr, pc
003d57a4: ldr      pc, [r3, #0x88]
003d57a8: cmp      r0, #0
003d57ac: bne      #0x3d58e4
003d57b0: mov      r0, #0
003d57b4: add      sp, sp, #0x18
003d57b8: pop      {r4, r5, r6, r7, r8, pc}
003d57bc: ldr      r8, [r6, #0xf4]
003d57c0: cmp      r8, #0
003d57c4: bne      #0x3d578c
003d57c8: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d57cc: cmp      r0, #0
003d57d0: blt      #0x3d597c
003d57d4: mov      r0, r6
003d57d8: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d57dc: ldr      r7, [pc, #0x274]
003d57e0: ldr      r3, [r4, r7]
003d57e4: ldr      r3, [r3]
003d57e8: cmp      r0, r3
003d57ec: blt      #0x3d5814
003d57f0: ldr      r3, [pc, #0x264]
003d57f4: ldr      r3, [r4, r3]
003d57f8: ldr      r3, [r3]
003d57fc: cmp      r3, #2
003d5800: moveq    r3, #0
003d5804: streq    r3, [r3]
003d5808: beq      #0x3d5814
003d580c: cmp      r3, #1
003d5810: beq      #0x3d5a20
003d5814: ldr      r0, [r5, #4]
003d5818: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d581c: cmp      r0, #0
003d5820: blt      #0x3d5924
003d5824: ldr      r0, [r5, #4]
003d5828: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d582c: ldr      r3, [r4, r7]
003d5830: ldr      r3, [r3]
003d5834: cmp      r0, r3
003d5838: blt      #0x3d5860
003d583c: ldr      r3, [pc, #0x218]
003d5840: ldr      r3, [r4, r3]
003d5844: ldr      r3, [r3]
003d5848: cmp      r3, #2
003d584c: moveq    r3, #0
003d5850: streq    r3, [r3]
003d5854: beq      #0x3d5860
003d5858: cmp      r3, #1
003d585c: beq      #0x3d59ec
003d5860: ldr      r3, [r5, #4]
003d5864: mov      r0, r3
003d5868: ldr      r3, [r3]
003d586c: mov      lr, pc
003d5870: ldr      pc, [r3, #0x28]
003d5874: cmp      r0, #0
003d5878: bne      #0x3d5908
003d587c: ldr      r3, [pc, #0x1dc]
003d5880: ldr      r0, [r5, #4]
003d5884: ldr      r3, [r4, r3]
003d5888: ldr      r4, [r3]
003d588c: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d5890: mov      r3, #0xc
003d5894: mla      r4, r3, r0, r4
003d5898: mov      r0, r6
003d589c: bl       #0x3a3180 ; _ZNK9Character18GetCharAIFactionIdEv
003d58a0: ldr      ip, [r4, #4]
003d58a4: cmp      ip, #0
003d58a8: beq      #0x3d57b0
003d58ac: ldr      r2, [r4, #8]
003d58b0: ldr      r3, [r2, #4]
003d58b4: cmp      r0, r3
003d58b8: movne    r3, #0
003d58bc: bne      #0x3d58d0
003d58c0: b        #0x3d59e0
003d58c4: ldr      r1, [r2, #4]
003d58c8: cmp      r0, r1
003d58cc: beq      #0x3d59e0
003d58d0: add      r3, r3, #1
003d58d4: cmp      r3, ip
003d58d8: add      r2, r2, #0xc
003d58dc: bne      #0x3d58c4
003d58e0: b        #0x3d57b0
003d58e4: mov      r0, r7
003d58e8: ldr      r1, [r5, #4]
003d58ec: ldr      r3, [r7]
003d58f0: mov      lr, pc
003d58f4: ldr      pc, [r3, #0x90]
003d58f8: cmp      r0, #8
003d58fc: movne    r0, #0
003d5900: moveq    r0, #1
003d5904: b        #0x3d57b4
003d5908: ldr      r3, [r6]
003d590c: mov      r0, r6
003d5910: mov      lr, pc
003d5914: ldr      pc, [r3, #0x28]
003d5918: cmp      r0, #0
003d591c: bne      #0x3d57b0
003d5920: b        #0x3d587c
003d5924: ldr      r3, [pc, #0x130]
003d5928: ldr      r3, [r4, r3]
003d592c: ldr      r3, [r3]
003d5930: cmp      r3, #2
003d5934: moveq    r3, #0
003d5938: streq    r3, [r3]
003d593c: beq      #0x3d5824
003d5940: cmp      r3, #1
003d5944: bne      #0x3d5824
003d5948: ldr      r0, [pc, #0x114]
003d594c: ldr      r1, [pc, #0x114]
003d5950: ldr      r2, [pc, #0x114]
003d5954: ldr      r0, [r4, r0]
003d5958: ldr      r3, [pc, #0x110]
003d595c: movw     ip, #0x109
003d5960: add      r1, pc, r1
003d5964: add      r2, pc, r2
003d5968: add      r3, pc, r3
003d596c: add      r0, r0, #0xa8
003d5970: str      ip, [sp]
003d5974: bl       #0x30e004 ; 
003d5978: b        #0x3d5824
003d597c: ldr      r3, [pc, #0xd8]
003d5980: ldr      r3, [r4, r3]
003d5984: ldr      r3, [r3]
003d5988: cmp      r3, #2
003d598c: streq    r8, [r8]
003d5990: beq      #0x3d57d4
003d5994: cmp      r3, #1
003d5998: bne      #0x3d57d4
003d599c: ldr      r0, [pc, #0xc0]
003d59a0: ldr      r1, [pc, #0xcc]
003d59a4: ldr      r2, [pc, #0xcc]
003d59a8: ldr      r0, [r4, r0]
003d59ac: ldr      r3, [pc, #0xc8]
003d59b0: movw     ip, #0x107
003d59b4: add      r1, pc, r1
003d59b8: add      r2, pc, r2
003d59bc: add      r3, pc, r3
003d59c0: add      r0, r0, #0xa8
003d59c4: str      ip, [sp]
003d59c8: bl       #0x30e004 ; 
003d59cc: b        #0x3d57d4
003d59d0: ldr      r7, [r0, #0x40]
003d59d4: cmp      r7, #0
003d59d8: beq      #0x3d57b0
003d59dc: b        #0x3d5768
003d59e0: ldr      r0, [r2, #8]
003d59e4: lsr      r0, r0, #0x1f
003d59e8: b        #0x3d57b4
003d59ec: ldr      r0, [pc, #0x70]
003d59f0: ldr      r1, [pc, #0x88]
003d59f4: ldr      r2, [pc, #0x88]
003d59f8: ldr      r0, [r4, r0]
003d59fc: ldr      r3, [pc, #0x84]
003d5a00: movw     ip, #0x10a
003d5a04: add      r1, pc, r1
003d5a08: add      r2, pc, r2
003d5a0c: add      r3, pc, r3
003d5a10: add      r0, r0, #0xa8
003d5a14: str      ip, [sp]
003d5a18: bl       #0x30e004 ; 
003d5a1c: b        #0x3d5860
003d5a20: ldr      r0, [pc, #0x3c]
003d5a24: ldr      r1, [pc, #0x60]
003d5a28: ldr      r2, [pc, #0x60]
003d5a2c: ldr      r0, [r4, r0]
003d5a30: ldr      r3, [pc, #0x5c]
003d5a34: mov      ip, #0x108
003d5a38: add      r1, pc, r1
003d5a3c: add      r2, pc, r2
003d5a40: add      r3, pc, r3
003d5a44: add      r0, r0, #0xa8
003d5a48: str      ip, [sp]
003d5a4c: bl       #0x30e004 ; 
003d5a50: b        #0x3d5814
003d5a54: subseq   pc, fp, r0, lsr r3
003d5a58: andeq    r2, r0, r4, asr #4
003d5a5c: andeq    r3, r0, r0, asr #19
003d5a60: andeq    r4, r0, ip, lsr #12
003d5a64: andeq    r1, r0, r0, asr #19
003d5a68: subeq    r8, lr, r8, ror sl
003d5a6c: subeq    pc, lr, r4, lsl sp
003d5a70: subeq    pc, lr, r8, asr ip
003d5a74: subeq    r8, lr, r4, lsr #20
003d5a78: subeq    pc, lr, r0, ror #24
003d5a7c: subeq    pc, lr, r4, lsl #24
003d5a80: ldrdeq   r8, sb, [lr], #-0x94
003d5a84: umaaleq  pc, lr, r0, ip
003d5a88: strheq   pc, [lr], #-0xb4
003d5a8c: subeq    r8, lr, r0, lsr #19

# _ZThn884_N9Character11Ctrl_LookAtERK7Point3DIfE
003addb4: sub      r0, r0, #0x374
003addb8: b        #0x3addbc

# _ZN13ObjectManager21LoadGameObjectNetDataEiiR12NetBitStream
0034735c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00347360: ldr      ip, [pc, #0xa08]
00347364: ldr      lr, [pc, #0xa08]
00347368: sub      sp, sp, #0x94
0034736c: add      ip, pc, ip
00347370: str      r0, [sp, #0xc]
00347374: ldr      r0, [ip, lr]
00347378: str      lr, [sp, #0x18]
0034737c: str      ip, [sp, #0x10]
00347380: str      r1, [sp, #0x44]
00347384: mov      r7, r2
00347388: mov      r4, r3
0034738c: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
00347390: subs     r5, r0, #0
00347394: beq      #0x3473b0
00347398: mov      r0, r4
0034739c: mov      r1, #8
003473a0: bl       #0x80e564 ; _ZN12NetBitStream8ReadByteEj
003473a4: ldr      r3, [r5, #0x3c]
003473a8: cmp      r0, r3
003473ac: beq      #0x3473b8
003473b0: add      sp, sp, #0x94
003473b4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003473b8: add      r6, sp, #0x74
003473bc: mov      r0, r4
003473c0: str      r6, [sp, #0x74]
003473c4: str      r6, [sp, #0x78]
003473c8: bl       #0x80e498 ; _ZN12NetBitStream7ReadBitEv
003473cc: cmp      r0, #0
003473d0: streq    r0, [sp, #0x1c]
003473d4: movweq   sl, #0xffff
003473d8: bne      #0x3479ec
003473dc: mov      r0, r4
003473e0: mov      r1, #0x10
003473e4: bl       #0x80e630 ; _ZN12NetBitStream7ReadU32Ej
003473e8: subs     r5, r0, #0
003473ec: beq      #0x34776c
003473f0: mov      r2, r7
003473f4: ldr      r0, [sp, #0xc]
003473f8: ldr      r1, [sp, #0x44]
003473fc: bl       #0x342f30 ; _ZN13ObjectManager13IsPacketValidEii
00347400: ldr      r3, [sp, #0x1c]
00347404: str      r0, [sp, #0x14]
00347408: cmp      r3, #0
0034740c: addeq    fp, sp, #0x44
00347410: bne      #0x347bd4
00347414: cmp      r5, #0
00347418: ble      #0x347d1c
0034741c: ldr      r3, [pc, #0x954]
00347420: ldr      ip, [sp, #0xc]
00347424: ldr      lr, [sp, #0xc]
00347428: str      r3, [sp, #0x40]
0034742c: ldr      r3, [pc, #0x948]
00347430: ldr      r1, [pc, #0x948]
00347434: ldr      r2, [pc, #0x948]
00347438: add      r3, pc, r3
0034743c: str      r3, [sp, #0x34]
00347440: ldr      r3, [pc, #0x940]
00347444: mov      r7, #0
00347448: add      ip, ip, #0x17c
0034744c: add      r3, pc, r3
00347450: str      r3, [sp, #0x38]
00347454: ldr      r3, [pc, #0x930]
00347458: add      lr, lr, #0x194
0034745c: str      fp, [sp, #0x24]
00347460: add      r3, pc, r3
00347464: str      r1, [sp, #0x2c]
00347468: str      r2, [sp, #0x30]
0034746c: str      r3, [sp, #0x3c]
00347470: str      ip, [sp, #0x20]
00347474: str      lr, [sp, #0x28]
00347478: mov      r8, r7
0034747c: add      sb, sp, #0x48
00347480: mov      sl, r5
00347484: mov      fp, r6
00347488: mov      r0, r4
0034748c: bl       #0x80e498 ; _ZN12NetBitStream7ReadBitEv
00347490: cmp      r0, #0
00347494: addeq    r7, r7, #1
00347498: bne      #0x347798
0034749c: mov      r1, #8
003474a0: mov      r0, r4
003474a4: bl       #0x80e564 ; _ZN12NetBitStream8ReadByteEj
003474a8: ldr      r1, [sp, #0xc]
003474ac: mov      r2, r7
003474b0: mov      r6, r0
003474b4: mov      r0, sb
003474b8: bl       #0x3407a0 ; _ZN13ObjectManager26GetObjectHandleByNetworkIdEi
003474bc: mov      r0, sb
003474c0: mov      r1, #0
003474c4: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
003474c8: subs     r5, r0, #0
003474cc: beq      #0x3474e0
003474d0: ldrb     r3, [r5, #0xf8]
003474d4: cmp      r3, r6
003474d8: moveq    r3, #0
003474dc: beq      #0x347524
003474e0: sub      r6, r6, #1
003474e4: cmp      r6, #3
003474e8: addls    pc, pc, r6, lsl #2
003474ec: b        #0x3476ec
003474f0: b        #0x3476cc
003474f4: b        #0x34774c
003474f8: b        #0x3476ac
003474fc: b        #0x347500
00347500: mov      r1, #0
00347504: movw     r0, #0x718
00347508: bl       #0x310570 ; _Znwj15MemoryHintState
0034750c: mov      r3, #1
00347510: mov      r1, #0x14
00347514: mov      r2, #0
00347518: mov      r5, r0
0034751c: bl       #0x398f44 ; _ZN7TriggerC1EN10ObjectBase6GO_IDSEbb
00347520: mov      r3, #1
00347524: ldr      ip, [r5, #0x104]
00347528: cmp      ip, #0
0034752c: beq      #0x347558
00347530: cmp      r3, #0
00347534: bne      #0x3477ac
00347538: ldr      lr, [sp, #0x14]
0034753c: cmp      lr, #0
00347540: bne      #0x347800
00347544: mov      r0, ip
00347548: ldr      r3, [ip]
0034754c: mov      r1, r4
00347550: mov      lr, pc
00347554: ldr      pc, [r3, #0x1c]
00347558: add      r8, r8, #1
0034755c: cmp      sl, r8
00347560: bne      #0x347488
00347564: mov      r6, fp
00347568: ldr      fp, [sp, #0x24]
0034756c: ldr      r0, [sp, #0x20]
00347570: mov      r1, fp
00347574: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00347578: ldr      r3, [r0, #0x10]
0034757c: mov      r5, r0
00347580: cmp      r3, #0
00347584: bne      #0x347bb8
00347588: ldr      r0, [sp, #0x28]
0034758c: mov      r1, fp
00347590: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00347594: ldr      r3, [r0, #0x10]
00347598: mov      r5, r0
0034759c: cmp      r3, #0
003475a0: bne      #0x347c94
003475a4: ldr      r5, [sp, #0x74]
003475a8: add      r7, sp, #0x6c
003475ac: ldr      r8, [sp, #0x20]
003475b0: b        #0x3475d4
003475b4: mov      r1, fp
003475b8: mov      r0, r8
003475bc: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
003475c0: add      r2, r5, #8
003475c4: mov      r1, r0
003475c8: mov      r0, r7
003475cc: bl       #0x344994 ; _ZNSt4priv8_Rb_treeIsSt4lessIsEsNS_9_IdentityIsEENS_11_SetTraitsTIsEESaIsEE13insert_uniqueERKs
003475d0: ldr      r5, [r5]
003475d4: cmp      r5, r6
003475d8: bne      #0x3475b4
003475dc: mov      r0, r4
003475e0: bl       #0x80e498 ; _ZN12NetBitStream7ReadBitEv
003475e4: cmp      r0, #0
003475e8: bne      #0x347c5c
003475ec: ldr      r0, [sp, #0x1c]
003475f0: cmp      r0, #0
003475f4: bne      #0x347c04
003475f8: mov      r0, r4
003475fc: bl       #0x80e498 ; _ZN12NetBitStream7ReadBitEv
00347600: cmp      r0, #0
00347604: beq      #0x347680
00347608: ldr      r0, [sp, #0x14]
0034760c: cmp      r0, #0
00347610: beq      #0x347680
00347614: mov      r0, r4
00347618: mov      r1, #0x10
0034761c: bl       #0x80e630 ; _ZN12NetBitStream7ReadU32Ej
00347620: subs     r7, r0, #0
00347624: ble      #0x347680
00347628: ldr      r1, [sp, #0xc]
0034762c: mov      r5, #0
00347630: add      sl, sp, #0x64
00347634: add      r8, r1, #0x164
00347638: add      sb, sp, #0x8e
0034763c: str      r6, [sp, #0xc]
00347640: mov      r1, #0x10
00347644: mov      r0, r4
00347648: bl       #0x80e630 ; _ZN12NetBitStream7ReadU32Ej
0034764c: mov      r1, fp
00347650: mov      r6, r0
00347654: mov      r0, r8
00347658: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
0034765c: add      r5, r5, #1
00347660: mov      r1, r0
00347664: mov      r2, sb
00347668: mov      r0, sl
0034766c: strh     r6, [sp, #0x8e]
00347670: bl       #0x344994 ; _ZNSt4priv8_Rb_treeIsSt4lessIsEsNS_9_IdentityIsEENS_11_SetTraitsTIsEESaIsEE13insert_uniqueERKs
00347674: cmp      r7, r5
00347678: bne      #0x347640
0034767c: ldr      r6, [sp, #0xc]
00347680: ldr      r0, [sp, #0x74]
00347684: cmp      r0, r6
00347688: bne      #0x347694
0034768c: b        #0x3473b0
00347690: mov      r0, r4
00347694: ldr      r4, [r0]
00347698: mov      r1, #0xc
0034769c: bl       #0x708f00 ; 
003476a0: cmp      r4, r6
003476a4: bne      #0x347690
003476a8: b        #0x3473b0
003476ac: mov      r1, #0
003476b0: mov      r0, #0x6d0
003476b4: bl       #0x310570 ; _Znwj15MemoryHintState
003476b8: mov      r1, #2
003476bc: mov      r5, r0
003476c0: bl       #0x3e8274 ; _ZN4DoorC1EN10ObjectBase6GO_IDSE
003476c4: mov      r3, #1
003476c8: b        #0x347524
003476cc: mov      r1, #0
003476d0: movw     r0, #0x1f90
003476d4: bl       #0x310570 ; _Znwj15MemoryHintState
003476d8: mov      r1, #0
003476dc: mov      r5, r0
003476e0: bl       #0x3aa1b4 ; _ZN9CharacterC1EN10ObjectBase6GO_IDSE
003476e4: mov      r3, #1
003476e8: b        #0x347524
003476ec: ldr      r1, [sp, #0x10]
003476f0: ldr      r0, [sp, #0x2c]
003476f4: ldr      r3, [r1, r0]
003476f8: ldr      r6, [r3]
003476fc: cmp      r6, #2
00347700: moveq    r3, #0
00347704: streq    r3, [r3]
00347708: moveq    r3, #1
0034770c: beq      #0x347524
00347710: cmp      r6, #1
00347714: movne    r3, #1
00347718: bne      #0x347524
0034771c: ldr      r3, [sp, #0x10]
00347720: ldr      r2, [sp, #0x30]
00347724: mov      ip, #0x860
00347728: ldr      r1, [sp, #0x34]
0034772c: ldr      r0, [r3, r2]
00347730: ldr      r3, [sp, #0x3c]
00347734: ldr      r2, [sp, #0x38]
00347738: add      r0, r0, #0xa8
0034773c: str      ip, [sp]
00347740: bl       #0x30e004 ; 
00347744: mov      r3, r6
00347748: b        #0x347524
0034774c: mov      r1, #0
00347750: mov      r0, #0x6f0
00347754: bl       #0x310570 ; _Znwj15MemoryHintState
00347758: mov      r1, #0x14
0034775c: mov      r5, r0
00347760: bl       #0x3a06dc ; _ZN9ContainerC1EN10ObjectBase6GO_IDSE
00347764: mov      r3, #1
00347768: b        #0x347524
0034776c: ldr      r0, [sp, #0x74]
00347770: cmp      r0, r6
00347774: bne      #0x347780
00347778: b        #0x3473b0
0034777c: mov      r0, r4
00347780: ldr      r4, [r0]
00347784: mov      r1, #0xc
00347788: bl       #0x708f00 ; 
0034778c: cmp      r4, r6
00347790: bne      #0x34777c
00347794: b        #0x3473b0
00347798: mov      r0, r4
0034779c: mov      r1, #0x10
003477a0: bl       #0x80e630 ; _ZN12NetBitStream7ReadU32Ej
003477a4: mov      r7, r0
003477a8: b        #0x34749c
003477ac: mov      r0, ip
003477b0: ldr      r3, [ip]
003477b4: mov      r1, r4
003477b8: mov      lr, pc
003477bc: ldr      pc, [r3, #0x1c]
003477c0: ldr      r2, [sp, #0x14]
003477c4: cmp      r2, #0
003477c8: beq      #0x3477ec
003477cc: mov      r0, fp
003477d0: bl       #0x343148 ; 
003477d4: strh     r7, [r0, #8]
003477d8: ldr      r3, [sp, #0x78]
003477dc: str      fp, [r0]
003477e0: str      r3, [r0, #4]
003477e4: str      r0, [r3]
003477e8: str      r0, [sp, #0x78]
003477ec: mov      r0, r5
003477f0: ldr      r3, [r5]
003477f4: mov      lr, pc
003477f8: ldr      pc, [r3, #4]
003477fc: b        #0x347558
00347800: mov      r1, r4
00347804: ldr      r2, [sp, #0x44]
00347808: mov      r0, ip
0034780c: ldr      ip, [ip]
00347810: mov      lr, pc
00347814: ldr      pc, [ip, #0x14]
00347818: mov      r6, r0
0034781c: bl       #0x7fd794 ; _Z9GetOnlinev
00347820: bl       #0x7fd5b4 ; _ZN7COnline8IsServerEv
00347824: cmp      r0, #0
00347828: bne      #0x347974
0034782c: ldr      r3, [r5]
00347830: mov      r0, r5
00347834: ldr      r1, [sp, #0x1c]
00347838: mov      lr, pc
0034783c: ldr      pc, [r3, #0x50]
00347840: ldr      r2, [sp, #0x40]
00347844: ldr      ip, [sp, #0x10]
00347848: ldr      r3, [ip, r2]
0034784c: ldr      r2, [sp, #0x44]
00347850: ldr      r3, [r3]
00347854: str      r2, [r5, #0x110]
00347858: mov      r2, #0
0034785c: cmp      r6, r3
00347860: str      r2, [r5, #0x114]
00347864: beq      #0x3478ec
00347868: ldr      r0, [sp, #0x20]
0034786c: ldr      r1, [sp, #0x24]
00347870: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
00347874: ldr      r3, [r0, #4]
00347878: cmp      r3, #0
0034787c: beq      #0x347cf8
00347880: mov      r1, r0
00347884: sxth     ip, r7
00347888: b        #0x347890
0034788c: mov      r3, r2
00347890: ldrsh    r2, [r3, #0x10]
00347894: cmp      r2, ip
00347898: ldrlt    r2, [r3, #0xc]
0034789c: ldrge    r2, [r3, #8]
003478a0: movlt    r3, r1
003478a4: mov      r1, r3
003478a8: cmp      r2, #0
003478ac: bne      #0x34788c
003478b0: cmp      r0, r3
003478b4: beq      #0x3478ec
003478b8: ldrsh    r2, [r3, #0x10]
003478bc: cmp      r2, ip
003478c0: bgt      #0x347cf8
003478c4: cmp      r0, r3
003478c8: beq      #0x3478ec
003478cc: mov      r0, fp
003478d0: bl       #0x343148 ; 
003478d4: strh     r7, [r0, #8]
003478d8: ldr      r3, [sp, #0x78]
003478dc: str      fp, [r0]
003478e0: str      r3, [r0, #4]
003478e4: str      r0, [r3]
003478e8: str      r0, [sp, #0x78]
003478ec: ldr      r0, [sp, #0x28]
003478f0: ldr      r1, [sp, #0x24]
003478f4: bl       #0x345d04 ; _ZNSt3mapIsSt3setIsSt4lessIsESaIsEES2_SaISt4pairIKsS4_EEEixIiEERS4_RKT_
003478f8: ldr      r3, [r0, #4]
003478fc: cmp      r3, #0
00347900: beq      #0x347bb0
00347904: mov      r1, r0
00347908: sxth     ip, r7
0034790c: b        #0x347914
00347910: mov      r3, r2
00347914: ldrsh    r2, [r3, #0x10]
00347918: cmp      r2, ip
0034791c: ldrlt    r2, [r3, #0xc]
00347920: ldrge    r2, [r3, #8]
00347924: movlt    r3, r1
00347928: mov      r1, r3
0034792c: cmp      r2, #0
00347930: bne      #0x347910
00347934: cmp      r0, r3
00347938: beq      #0x347558
0034793c: ldrsh    r2, [r3, #0x10]
00347940: cmp      r2, ip
00347944: bgt      #0x347bb0
00347948: cmp      r0, r3
0034794c: beq      #0x347558
00347950: mov      r0, fp
00347954: bl       #0x343148 ; 
00347958: strh     r7, [r0, #8]
0034795c: ldr      r3, [sp, #0x78]
00347960: str      fp, [r0]
00347964: str      r3, [r0, #4]
00347968: str      r0, [r3]
0034796c: str      r0, [sp, #0x78]
00347970: b        #0x347558
00347974: ldr      r3, [r5, #0x110]
00347978: cmn      r3, #1
0034797c: beq      #0x347d00
00347980: ldr      r2, [sp, #0x44]
00347984: cmp      r2, r3
00347988: beq      #0x34782c
0034798c: ldr      r3, [r5]
00347990: mov      r0, r5
00347994: mov      lr, pc
00347998: ldr      pc, [r3, #0x24]
0034799c: cmp      r0, #0
003479a0: beq      #0x347558
003479a4: ldr      r3, [r5]
003479a8: mov      r0, r5
003479ac: mov      lr, pc
003479b0: ldr      pc, [r3, #0x28]
003479b4: cmp      r0, #0
003479b8: beq      #0x347558
003479bc: ldr      r1, [sp, #0x10]
003479c0: ldr      r0, [sp, #0x18]
003479c4: mov      r2, #1
003479c8: ldr      r3, [r1, r0]
003479cc: mov      r1, #0
003479d0: ldr      r0, [r3, #0x40]
003479d4: bl       #0x36e478 ; _ZN13PlayerManager14GetLocalPlayerEib
003479d8: ldr      r3, [r0, #0x660]
003479dc: ldr      r3, [r3, #0x108]
003479e0: cmp      r7, r3
003479e4: bne      #0x34782c
003479e8: b        #0x347558
003479ec: mov      r0, r4
003479f0: mov      r1, #8
003479f4: bl       #0x80e564 ; _ZN12NetBitStream8ReadByteEj
003479f8: mov      r8, r0
003479fc: ldr      r0, [sp, #0xc]
00347a00: mov      sl, r8
00347a04: ldr      ip, [r0, #0xb4]
00347a08: add      r5, r0, #0xb0
00347a0c: cmp      ip, #0
00347a10: beq      #0x347ba8
00347a14: ldr      r0, [sp, #0x44]
00347a18: mov      r1, r5
00347a1c: mov      r3, ip
00347a20: b        #0x347a28
00347a24: mov      r3, r2
00347a28: ldr      r2, [r3, #0x10]
00347a2c: cmp      r2, r0
00347a30: ldrlt    r2, [r3, #0xc]
00347a34: ldrge    r2, [r3, #8]
00347a38: movlt    r3, r1
00347a3c: mov      r1, r3
00347a40: cmp      r2, #0
00347a44: bne      #0x347a24
00347a48: cmp      r5, r3
00347a4c: beq      #0x347d50
00347a50: ldr      r2, [r3, #0x10]
00347a54: cmp      r2, r0
00347a58: bgt      #0x347ba8
00347a5c: cmp      r5, r3
00347a60: beq      #0x347d50
00347a64: cmp      ip, #0
00347a68: beq      #0x347d44
00347a6c: ldr      lr, [sp, #0x44]
00347a70: mov      r2, r5
00347a74: b        #0x347a7c
00347a78: mov      ip, r3
00347a7c: ldr      r3, [ip, #0x10]
00347a80: cmp      r3, lr
00347a84: ldrlt    r3, [ip, #0xc]
00347a88: ldrge    r3, [ip, #8]
00347a8c: movlt    ip, r2
00347a90: mov      r2, ip
00347a94: cmp      r3, #0
00347a98: bne      #0x347a78
00347a9c: cmp      r5, ip
00347aa0: beq      #0x347ab4
00347aa4: ldr      r2, [ip, #0x10]
00347aa8: mov      r3, ip
00347aac: cmp      r2, lr
00347ab0: ble      #0x347adc
00347ab4: add      r3, sp, #0x5c
00347ab8: str      ip, [sp, #0x84]
00347abc: add      r0, sp, #0x88
00347ac0: mov      ip, #0
00347ac4: mov      r1, r5
00347ac8: add      r2, sp, #0x84
00347acc: str      lr, [sp, #0x5c]
00347ad0: strh     ip, [sp, #0x60]
00347ad4: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
00347ad8: ldr      r3, [sp, #0x88]
00347adc: ldrsh    r3, [r3, #0x14]
00347ae0: sxth     sb, r8
00347ae4: cmp      r3, sb
00347ae8: blt      #0x347cb0
00347aec: ldr      lr, [sp, #0xc]
00347af0: ldr      ip, [lr, #0xb4]
00347af4: cmp      ip, #0
00347af8: beq      #0x347d38
00347afc: ldr      lr, [sp, #0x44]
00347b00: mov      r2, r5
00347b04: b        #0x347b0c
00347b08: mov      ip, r3
00347b0c: ldr      r3, [ip, #0x10]
00347b10: cmp      r3, lr
00347b14: ldrlt    r3, [ip, #0xc]
00347b18: ldrge    r3, [ip, #8]
00347b1c: movlt    ip, r2
00347b20: mov      r2, ip
00347b24: cmp      r3, #0
00347b28: bne      #0x347b08
00347b2c: cmp      r5, ip
00347b30: beq      #0x347b44
00347b34: ldr      r2, [ip, #0x10]
00347b38: mov      r3, ip
00347b3c: cmp      r2, lr
00347b40: ble      #0x347b6c
00347b44: add      r3, sp, #0x54
00347b48: str      ip, [sp, #0x7c]
00347b4c: mov      r1, r5
00347b50: mov      ip, #0
00347b54: add      r0, sp, #0x80
00347b58: add      r2, sp, #0x7c
00347b5c: str      lr, [sp, #0x54]
00347b60: strh     ip, [sp, #0x58]
00347b64: bl       #0x34371c ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKisENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE13insert_uniqueENS_17_Rb_tree_iteratorIS5_S9_EERKS5_
00347b68: ldr      r3, [sp, #0x80]
00347b6c: ldrsh    r3, [r3, #0x14]
00347b70: cmp      r3, sb
00347b74: movne    lr, #1
00347b78: strne    lr, [sp, #0x1c]
00347b7c: bne      #0x3473dc
00347b80: ldr      r1, [sp, #0xc]
00347b84: add      r0, r1, #0xe0
00347b88: add      r1, sp, #0x44
00347b8c: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347b90: ldrh     r3, [r0]
00347b94: mov      r2, #1
00347b98: str      r2, [sp, #0x1c]
00347b9c: add      r3, r3, r2
00347ba0: strh     r3, [r0]
00347ba4: b        #0x3473dc
00347ba8: mov      r3, r5
00347bac: b        #0x347a5c
00347bb0: mov      r3, r0
00347bb4: b        #0x347948
00347bb8: ldr      r1, [r0, #4]
00347bbc: bl       #0x345ccc ; _ZNSt4priv8_Rb_treeIsSt4lessIsEsNS_9_IdentityIsEENS_11_SetTraitsTIsEESaIsEE8_M_eraseEPNS_18_Rb_tree_node_baseE
00347bc0: mov      r3, #0
00347bc4: str      r3, [r5, #0x10]
00347bc8: stmib    r5, {r3, r5}
00347bcc: str      r5, [r5, #0xc]
00347bd0: b        #0x347588
00347bd4: ldr      ip, [sp, #0xc]
00347bd8: add      fp, sp, #0x44
00347bdc: mov      r1, fp
00347be0: add      r0, ip, #0xb0
00347be4: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347be8: ldrsh    r3, [r0]
00347bec: sxth     sl, sl
00347bf0: cmp      r3, sl
00347bf4: movne    sl, #0
00347bf8: moveq    sl, #1
00347bfc: str      sl, [sp, #0x14]
00347c00: b        #0x347414
00347c04: ldr      r1, [sp, #0xc]
00347c08: add      r5, r1, #0xc8
00347c0c: mov      r0, r5
00347c10: mov      r1, fp
00347c14: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347c18: ldrsh    r3, [r0]
00347c1c: cmp      r3, #0
00347c20: blt      #0x3475f8
00347c24: ldr      r2, [sp, #0xc]
00347c28: mov      r1, fp
00347c2c: add      r0, r2, #0xe0
00347c30: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347c34: mov      r1, fp
00347c38: ldrh     r7, [r0]
00347c3c: mov      r0, r5
00347c40: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347c44: ldrh     r3, [r0]
00347c48: cmp      r7, r3
00347c4c: ldreq    ip, [sp, #0xc]
00347c50: moveq    r3, #1
00347c54: strbeq   r3, [ip, #0x160]
00347c58: b        #0x3475f8
00347c5c: ldr      lr, [sp, #0xc]
00347c60: mov      r1, #8
00347c64: mov      r0, r4
00347c68: add      r5, lr, #0xc8
00347c6c: bl       #0x80e564 ; _ZN12NetBitStream8ReadByteEj
00347c70: mov      r1, fp
00347c74: mov      r7, r0
00347c78: mov      r0, r5
00347c7c: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347c80: mov      r0, r5
00347c84: mov      r1, fp
00347c88: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347c8c: strh     r7, [r0]
00347c90: b        #0x3475ec
00347c94: ldr      r1, [r0, #4]
00347c98: bl       #0x345ccc ; _ZNSt4priv8_Rb_treeIsSt4lessIsEsNS_9_IdentityIsEENS_11_SetTraitsTIsEESaIsEE8_M_eraseEPNS_18_Rb_tree_node_baseE
00347c9c: mov      r3, #0
00347ca0: str      r3, [r5, #0x10]
00347ca4: stmib    r5, {r3, r5}
00347ca8: str      r5, [r5, #0xc]
00347cac: b        #0x3475a4
00347cb0: add      fp, sp, #0x44
00347cb4: mov      r1, fp
00347cb8: mov      r0, r5
00347cbc: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347cc0: strh     r8, [r0]
00347cc4: ldr      lr, [sp, #0xc]
00347cc8: mov      r1, fp
00347ccc: add      r0, lr, #0xe0
00347cd0: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347cd4: mov      r1, #0
00347cd8: strh     r1, [r0]
00347cdc: ldr      r2, [sp, #0xc]
00347ce0: mov      r1, fp
00347ce4: add      r0, r2, #0xc8
00347ce8: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347cec: mvn      r3, #0
00347cf0: strh     r3, [r0]
00347cf4: b        #0x347aec
00347cf8: mov      r3, r0
00347cfc: b        #0x3478c4
00347d00: ldr      r0, [sp, #0xc]
00347d04: mov      r1, r5
00347d08: bl       #0x340848 ; _ZN13ObjectManager13CanSendUpdateEP10ObjectBase
00347d0c: cmp      r0, #0
00347d10: beq      #0x34782c
00347d14: ldr      r3, [r5, #0x110]
00347d18: b        #0x347980
00347d1c: ldr      lr, [sp, #0xc]
00347d20: ldr      r0, [sp, #0xc]
00347d24: add      lr, lr, #0x17c
00347d28: add      r0, r0, #0x194
00347d2c: str      lr, [sp, #0x20]
00347d30: str      r0, [sp, #0x28]
00347d34: b        #0x34756c
00347d38: ldr      lr, [sp, #0x44]
00347d3c: mov      ip, r5
00347d40: b        #0x347b2c
00347d44: ldr      lr, [sp, #0x44]
00347d48: mov      ip, r5
00347d4c: b        #0x347a9c
00347d50: add      r1, sp, #0x44
00347d54: mov      r0, r5
00347d58: bl       #0x343a90 ; _ZNSt3mapIisSt4lessIiESaISt4pairIKisEEEixIiEERsRKT_
00347d5c: mvn      r1, #0
00347d60: strh     r1, [r0]
00347d64: ldr      r2, [sp, #0xc]
00347d68: ldr      ip, [r2, #0xb4]
00347d6c: b        #0x347a64
00347d70: rsbeq    sp, r4, r4, lsr #14
00347d74: strdeq   r3, r4, [r0], -r4
00347d78: andeq    r2, r0, ip, asr lr
00347d7c: subseq   r6, r7, r0, lsr #31
00347d80: andeq    r3, r0, r0, asr #19
00347d84: andeq    r1, r0, r0, asr #19
00347d88: subseq   r7, r7, ip, lsl r1
00347d8c: subseq   r8, r7, r8, lsr lr

# _ZN9Character11Ctrl_LookAtEP10GameObject
003ad9ac: cmp      r1, #0
003ad9b0: push     {r4, r5, r6, lr}
003ad9b4: mov      r5, r0
003ad9b8: beq      #0x3ad9d8
003ad9bc: ldr      r3, [r0]
003ad9c0: mov      r0, r1
003ad9c4: ldr      r4, [r3, #0xd0]
003ad9c8: bl       #0x3935dc ; _ZNK10GameObject17GetTargetPositionEv
003ad9cc: mov      r1, r0
003ad9d0: mov      r0, r5
003ad9d4: blx      r4
003ad9d8: pop      {r4, r5, r6, pc}

# _ZN13ObjectManager3AddEP10ObjectBasePKcS3_ib
0034b270: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034b274: ldr      r4, [pc, #0x27c]
0034b278: sub      sp, sp, #0x2c
0034b27c: subs     r6, r2, #0
0034b280: add      r4, pc, r4
0034b284: mov      r5, r0
0034b288: mov      r8, r1
0034b28c: mov      r7, r3
0034b290: ldr      fp, [sp, #0x50]
0034b294: ldr      sl, [sp, #0x54]
0034b298: ldrb     sb, [sp, #0x58]
0034b29c: beq      #0x34b304
0034b2a0: cmp      r7, #0
0034b2a4: beq      #0x34b358
0034b2a8: mov      r4, #0
0034b2ac: mov      ip, #1
0034b2b0: mov      r2, r7
0034b2b4: mov      r3, sl
0034b2b8: mov      r0, r5
0034b2bc: mov      r1, r8
0034b2c0: str      ip, [sp]
0034b2c4: str      r4, [sp, #4]
0034b2c8: bl       #0x34aca0 ; _ZN13ObjectManager15GetObjectByNameEPKcibS1_
0034b2cc: mov      r0, r5
0034b2d0: mov      r1, r4
0034b2d4: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b2d8: cmp      r0, r4
0034b2dc: beq      #0x34b3ac
0034b2e0: cmp      r6, r4
0034b2e4: beq      #0x34b2f8
0034b2e8: mov      r0, r6
0034b2ec: ldr      r3, [r6]
0034b2f0: mov      lr, pc
0034b2f4: ldr      pc, [r3, #4]
0034b2f8: mov      r0, r5
0034b2fc: add      sp, sp, #0x2c
0034b300: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034b304: ldr      r3, [pc, #0x1f0]
0034b308: ldr      r3, [r4, r3]
0034b30c: ldr      r3, [r3]
0034b310: cmp      r3, #2
0034b314: streq    r6, [r6]
0034b318: beq      #0x34b2a0
0034b31c: cmp      r3, #1
0034b320: bne      #0x34b2a0
0034b324: ldr      r0, [pc, #0x1d4]
0034b328: ldr      r1, [pc, #0x1d4]
0034b32c: ldr      r2, [pc, #0x1d4]
0034b330: ldr      r0, [r4, r0]
0034b334: ldr      r3, [pc, #0x1d0]
0034b338: movw     ip, #0x428
0034b33c: add      r1, pc, r1
0034b340: add      r2, pc, r2
0034b344: add      r3, pc, r3
0034b348: add      r0, r0, #0xa8
0034b34c: str      ip, [sp]
0034b350: bl       #0x30e004 ; 
0034b354: b        #0x34b2a0
0034b358: ldr      r3, [pc, #0x19c]
0034b35c: ldr      r3, [r4, r3]
0034b360: ldr      r3, [r3]
0034b364: cmp      r3, #2
0034b368: streq    r7, [r7]
0034b36c: beq      #0x34b2a8
0034b370: cmp      r3, #1
0034b374: bne      #0x34b2a8
0034b378: ldr      r0, [pc, #0x180]
0034b37c: ldr      r1, [pc, #0x18c]
0034b380: ldr      r2, [pc, #0x18c]
0034b384: ldr      r0, [r4, r0]
0034b388: ldr      r3, [pc, #0x188]
0034b38c: movw     ip, #0x429
0034b390: add      r1, pc, r1
0034b394: add      r2, pc, r2
0034b398: add      r3, pc, r3
0034b39c: add      r0, r0, #0xa8
0034b3a0: str      ip, [sp]
0034b3a4: bl       #0x30e004 ; 
0034b3a8: b        #0x34b2a8
0034b3ac: mov      r1, r5
0034b3b0: add      r0, r8, #0xc
0034b3b4: bl       #0x33fc88 ; _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
0034b3b8: str      r6, [r0, #0x18]
0034b3bc: ldr      r3, [r8, #0x50]
0034b3c0: mov      r2, r5
0034b3c4: mov      r1, r7
0034b3c8: add      r3, r3, #1
0034b3cc: str      r3, [r8, #0x50]
0034b3d0: str      r6, [r5, #4]
0034b3d4: ldr      ip, [r6, #0x2c]
0034b3d8: ldr      lr, [r2], #4
0034b3dc: mov      r0, r6
0034b3e0: mov      r3, ip
0034b3e4: str      lr, [r3], #4
0034b3e8: ldr      lr, [r5, #4]
0034b3ec: add      r4, sp, #0x18
0034b3f0: str      lr, [ip, #4]
0034b3f4: ldr      r2, [r2, #4]
0034b3f8: str      r2, [r3, #4]
0034b3fc: bl       #0x34ac18 ; _ZN10ObjectBase7SetNameEPKc
0034b400: mov      r0, fp
0034b404: bl       #0x30de54 ; 
0034b408: mov      r1, fp
0034b40c: add      r2, fp, r0
0034b410: add      r0, r6, #0x48
0034b414: bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
0034b418: mov      r1, r6
0034b41c: mov      r0, r4
0034b420: str      sl, [r6, #0x64]
0034b424: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0034b428: mov      r0, r4
0034b42c: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0034b430: subs     r7, r0, #0
0034b434: beq      #0x34b45c
0034b438: add      r4, r8, #0x60
0034b43c: mov      r0, r4
0034b440: bl       #0x342690 ; 
0034b444: str      r7, [r0, #8]
0034b448: ldr      r3, [r8, #0x64]
0034b44c: str      r4, [r0]
0034b450: str      r3, [r0, #4]
0034b454: str      r0, [r3]
0034b458: str      r0, [r8, #0x64]
0034b45c: add      r4, sp, #0xc
0034b460: mov      r0, r4
0034b464: mov      r1, r6
0034b468: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0034b46c: mov      r0, r4
0034b470: mov      r1, #0
0034b474: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b478: subs     r4, r0, #0
0034b47c: beq      #0x34b48c
0034b480: ldr      r3, [r4, #0xf4]
0034b484: cmp      r3, #5
0034b488: beq      #0x34b4a4
0034b48c: cmp      sb, #0
0034b490: beq      #0x34b2f8
0034b494: mov      r0, r8
0034b498: mov      r1, r6
0034b49c: bl       #0x3431c0 ; _ZN13ObjectManager21AssignObjectNetworkIdEP10ObjectBase
0034b4a0: b        #0x34b2f8
0034b4a4: ldr      r0, [r6, #0x5c]
0034b4a8: ldr      r2, [r6, #0x58]
0034b4ac: rsb      r2, r0, r2
0034b4b0: cmp      r2, #6
0034b4b4: bne      #0x34b48c
0034b4b8: ldr      r1, [pc, #0x5c]
0034b4bc: add      r1, pc, r1
0034b4c0: bl       #0x30e5e0 ; 
0034b4c4: cmp      r0, #0
0034b4c8: bne      #0x34b48c
0034b4cc: mov      r3, #0xc
0034b4d0: add      r0, sp, #0x28
0034b4d4: str      r3, [r0, #-4]!
0034b4d8: bl       #0x708ec0 ; 
0034b4dc: str      r4, [r0, #8]
0034b4e0: ldr      r3, [r8, #0x6c]
0034b4e4: add      r2, r8, #0x68
0034b4e8: stm      r0, {r2, r3}
0034b4ec: str      r0, [r3]
0034b4f0: str      r0, [r8, #0x6c]
0034b4f4: b        #0x34b48c
0034b4f8: rsbeq    sb, r4, r0, lsl r8
0034b4fc: andeq    r3, r0, r0, asr #19
0034b500: andeq    r1, r0, r0, asr #19

# _ZN13ObjectManager20IsObjectSerializableEP10ObjectBasei
003409f4: push     {r4, r5, r6, r7, r8, lr}
003409f8: ldr      r4, [pc, #0x1bc]
003409fc: subs     r5, r1, #0
00340a00: sub      sp, sp, #0x10
00340a04: mov      r7, r0
00340a08: add      r4, pc, r4
00340a0c: mov      r6, r2
00340a10: beq      #0x340afc
00340a14: bl       #0x7fd794 ; _Z9GetOnlinev
00340a18: bl       #0x7fd5b4 ; _ZN7COnline8IsServerEv
00340a1c: cmp      r0, #0
00340a20: beq      #0x340a40
00340a24: ldr      r3, [r5, #0x110]
00340a28: cmn      r3, #1
00340a2c: beq      #0x340a40
00340a30: cmp      r6, r3
00340a34: beq      #0x340a40
00340a38: mov      r0, #1
00340a3c: b        #0x340b00
00340a40: ldrb     r3, [r5, #0x119]
00340a44: cmp      r3, #0
00340a48: beq      #0x340afc
00340a4c: ldr      r3, [r5, #0x100]
00340a50: cmp      r3, #0
00340a54: beq      #0x340afc
00340a58: ldr      r3, [r5]
00340a5c: mov      r0, r5
00340a60: mov      lr, pc
00340a64: ldr      pc, [r3, #0x54]
00340a68: cmp      r0, #0
00340a6c: bne      #0x340afc
00340a70: ldrb     r3, [r5, #0x118]
00340a74: cmp      r3, #0
00340a78: bne      #0x340afc
00340a7c: ldr      r3, [r5, #0x110]
00340a80: cmp      r6, r3
00340a84: beq      #0x340afc
00340a88: add      r8, sp, #4
00340a8c: mov      r0, r8
00340a90: mov      r1, r5
00340a94: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
00340a98: mov      r0, r8
00340a9c: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00340aa0: subs     r8, r0, #0
00340aa4: beq      #0x340a38
00340aa8: ldr      r3, [r8, #0x11c]
00340aac: cmp      r3, #0
00340ab0: ble      #0x340b08
00340ab4: ldr      r0, [r5, #0x100]
00340ab8: bl       #0x81347c ; _ZN9NetStruct6ResendEv
00340abc: ldr      r3, [r8, #0x11c]
00340ac0: cmp      r3, #0
00340ac4: subgt    r3, r3, #1
00340ac8: strgt    r3, [r8, #0x11c]
00340acc: movgt    r0, #1
00340ad0: bgt      #0x340b00
00340ad4: b        #0x340a38
00340ad8: add      r0, r8, #0x4f0
00340adc: add      r0, r0, #0xc
00340ae0: bl       #0x3c01c0 ; _ZNK16CharStateMachine13SM_IsInLimbusEv
00340ae4: cmp      r0, #0
00340ae8: beq      #0x340b54
00340aec: mov      r0, r8
00340af0: bl       #0x3a5248 ; _ZNK9Character10CanRespawnEv
00340af4: cmp      r0, #0
00340af8: beq      #0x340b54
00340afc: mov      r0, #0
00340b00: add      sp, sp, #0x10
00340b04: pop      {r4, r5, r6, r7, r8, pc}
00340b08: ldr      r3, [r8]
00340b0c: mov      lr, pc
00340b10: ldr      pc, [r3, #0x148]
00340b14: cmp      r0, #0
00340b18: beq      #0x340afc
00340b1c: mov      r3, #0x1480
00340b20: ldrb     r3, [r8, r3]
00340b24: cmp      r3, #0
00340b28: bne      #0x340afc
00340b2c: mov      r0, r8
00340b30: bl       #0x3a30c4 ; _ZNK9Character10IsMerchantEv
00340b34: cmp      r0, #0
00340b38: bne      #0x340afc
00340b3c: ldr      r3, [r8]
00340b40: mov      r0, r8
00340b44: mov      lr, pc
00340b48: ldr      pc, [r3, #0x34]
00340b4c: cmp      r0, #0
00340b50: bne      #0x340ad8
00340b54: ldr      r3, [r8]
00340b58: mov      r0, r8
00340b5c: mov      lr, pc
00340b60: ldr      pc, [r3, #0x28]
00340b64: cmp      r0, #0
00340b68: beq      #0x340a38
00340b6c: bl       #0x800f8c ; _ZN9CMatching3GetEv
00340b70: bl       #0x7fe4e0 ; _ZN9CMatching8IsServerEv
00340b74: subs     r2, r0, #0
00340b78: beq      #0x340b98
00340b7c: mov      r0, r7
00340b80: mov      r1, r6
00340b84: mov      r2, r8
00340b88: bl       #0x340948 ; _ZN13ObjectManager22IsRemotePlayerOfMemberEiP9Character
00340b8c: eor      r0, r0, #1
00340b90: uxtb     r0, r0
00340b94: b        #0x340b00
00340b98: ldr      r3, [pc, #0x20]
00340b9c: mov      r1, r8
00340ba0: ldr      r3, [r4, r3]
00340ba4: ldr      r0, [r3, #0x40]
00340ba8: bl       #0x36eea8 ; _ZN13PlayerManager20GetPlayerByCharacterEPK9Characterb
00340bac: ldrb     r3, [r0, #0x66c]
00340bb0: cmp      r3, #1
00340bb4: beq      #0x340b7c
00340bb8: b        #0x340afc
00340bbc: rsbeq    r4, r5, r8, lsl #1
00340bc0: strdeq   r3, r4, [r0], -r4

# _ZN8RoomZone10DeActivateEv
00396918: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0039691c: ldr      r7, [pc, #0x154]
00396920: ldr      r2, [pc, #0x154]
00396924: ldr      sl, [pc, #0x154]
00396928: add      r7, pc, r7
0039692c: ldr      r3, [r7, r2]
00396930: ldr      r5, [r7, sl]
00396934: sub      sp, sp, #0x4c
00396938: ldr      r3, [r3]
0039693c: mov      r8, r0
00396940: mov      r0, r5
00396944: str      r3, [sp, #0x44]
00396948: str      r2, [sp]
0039694c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00396950: ldr      r1, [pc, #0x12c]
00396954: add      r4, sp, #0x2c
00396958: add      r2, sp, #0x10
0039695c: add      r1, pc, r1
00396960: mov      r0, r4
00396964: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00396968: mov      r0, r5
0039696c: mov      r1, r4
00396970: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00396974: ldr      r0, [sp, #0x40]
00396978: cmp      r0, r4
0039697c: beq      #0x39699c
00396980: cmp      r0, #0
00396984: beq      #0x39699c
00396988: ldr      r1, [sp, #0x2c]
0039698c: rsb      r1, r0, r1
00396990: cmp      r1, #0x80
00396994: bhi      #0x396a6c
00396998: bl       #0x708f00 ; 
0039699c: ldr      r3, [pc, #0xe4]
003969a0: ldr      r4, [r8, #0x394]!
003969a4: ldr      sb, [pc, #0xe0]
003969a8: ldr      r3, [r7, r3]
003969ac: cmp      r8, r4
003969b0: add      sb, pc, sb
003969b4: ldr      r3, [r3, #0x38]
003969b8: add      r5, sp, #0x14
003969bc: add      fp, sp, #0xc
003969c0: str      r3, [sp, #4]
003969c4: beq      #0x396a34
003969c8: ldr      r0, [r4, #8]
003969cc: cmp      r0, #0
003969d0: beq      #0x3969d8
003969d4: bl       #0x38c69c ; _ZN10GameObject10ZoneExitedEv
003969d8: ldr      r6, [r7, sl]
003969dc: mov      r0, r6
003969e0: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003969e4: mov      r1, sb
003969e8: mov      r2, fp
003969ec: mov      r0, r5
003969f0: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003969f4: mov      r0, r6
003969f8: mov      r1, r5
003969fc: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00396a00: ldr      r0, [sp, #0x28]
00396a04: cmp      r0, r5
00396a08: beq      #0x396a28
00396a0c: cmp      r0, #0
00396a10: beq      #0x396a28
00396a14: ldr      r1, [sp, #0x14]
00396a18: rsb      r1, r0, r1
00396a1c: cmp      r1, #0x80
00396a20: bhi      #0x396a60
00396a24: bl       #0x708f00 ; 
00396a28: ldr      r4, [r4]
00396a2c: cmp      r8, r4
00396a30: bne      #0x3969c8
00396a34: ldr      r0, [sp, #4]
00396a38: mov      r1, r8
00396a3c: bl       #0x345fbc ; _ZN13ObjectManager14DelRoomObjectsEPSt4listIP10GameObjectSaIS2_EE
00396a40: ldr      r2, [sp]
00396a44: ldr      r3, [r7, r2]
00396a48: ldr      r2, [sp, #0x44]
00396a4c: ldr      r3, [r3]
00396a50: cmp      r2, r3
00396a54: bne      #0x396a74
00396a58: add      sp, sp, #0x4c
00396a5c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00396a60: bl       #0x310440 ; _Z10CustomFreePv
00396a64: ldr      r4, [r4]
00396a68: b        #0x396a2c
00396a6c: bl       #0x310440 ; _Z10CustomFreePv
00396a70: b        #0x39699c
00396a74: bl       #0x30e310 ; 
00396a78: subseq   lr, pc, r8, ror #2
00396a7c: andeq    r4, r0, ip, lsr #1
00396a80: andeq    r0, r0, r4, lsl #17
00396a84: subseq   ip, r2, r4, lsr r0
00396a88: strdeq   r3, r4, [r0], -r4
00396a8c: subseq   ip, r2, r0

# _ZN13ObjectManager13CanSendUpdateEP10ObjectBase
00340848: push     {r4, r5, lr}
0034084c: subs     r5, r1, #0
00340850: sub      sp, sp, #0x14
00340854: beq      #0x3408d4
00340858: ldr      r3, [r5]
0034085c: mov      r0, r5
00340860: mov      lr, pc
00340864: ldr      pc, [r3, #0x54]
00340868: cmp      r0, #0
0034086c: bne      #0x3408d4
00340870: ldr      r3, [r5, #0x100]
00340874: cmp      r3, #0
00340878: beq      #0x3408d4
0034087c: add      r4, sp, #4
00340880: mov      r0, r4
00340884: mov      r1, r5
00340888: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0034088c: mov      r0, r4
00340890: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
00340894: subs     r4, r0, #0
00340898: beq      #0x3408a8
0034089c: ldr      r3, [r4, #0x11c]
003408a0: cmp      r3, #0
003408a4: ble      #0x3408e0
003408a8: mov      r0, #1
003408ac: b        #0x3408d8
003408b0: add      r0, r4, #0x4f0
003408b4: add      r0, r0, #0xc
003408b8: bl       #0x3c01c0 ; _ZNK16CharStateMachine13SM_IsInLimbusEv
003408bc: cmp      r0, #0
003408c0: beq      #0x34092c
003408c4: mov      r0, r4
003408c8: bl       #0x3a5248 ; _ZNK9Character10CanRespawnEv
003408cc: cmp      r0, #0
003408d0: beq      #0x34092c
003408d4: mov      r0, #0
003408d8: add      sp, sp, #0x14
003408dc: pop      {r4, r5, pc}
003408e0: ldr      r3, [r4]
003408e4: mov      lr, pc
003408e8: ldr      pc, [r3, #0x148]
003408ec: cmp      r0, #0
003408f0: beq      #0x3408d4
003408f4: mov      r3, #0x1480
003408f8: ldrb     r3, [r4, r3]
003408fc: cmp      r3, #0
00340900: bne      #0x3408d4
00340904: mov      r0, r4
00340908: bl       #0x3a30c4 ; _ZNK9Character10IsMerchantEv
0034090c: cmp      r0, #0
00340910: bne      #0x3408d4
00340914: ldr      r3, [r4]
00340918: mov      r0, r4
0034091c: mov      lr, pc
00340920: ldr      pc, [r3, #0x34]
00340924: cmp      r0, #0
00340928: bne      #0x3408b0
0034092c: mov      r0, r4
00340930: ldr      r3, [r4]
00340934: mov      lr, pc
00340938: ldr      pc, [r3, #0x28]
0034093c: eor      r0, r0, #1
00340940: uxtb     r0, r0
00340944: b        #0x3408d8

# _ZN12ObjectHandleC1EP10ObjectBase
0033f524: push     {r4, r5, lr}
0033f528: mov      r3, #0
0033f52c: mvn      r2, #0
0033f530: cmp      r1, #0
0033f534: sub      sp, sp, #0x14
0033f538: mov      r4, r0
0033f53c: str      r3, [r0, #4]
0033f540: str      r2, [r0, #8]
0033f544: str      r3, [r0]
0033f548: beq      #0x33f56c
0033f54c: mov      r0, sp
0033f550: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0033f554: ldm      sp, {r0, r1, r2}
0033f558: mov      r3, r4
0033f55c: str      r0, [r3], #4
0033f560: mov      r5, sp
0033f564: str      r1, [r4, #4]
0033f568: str      r2, [r3, #4]
0033f56c: mov      r0, r4
0033f570: add      sp, sp, #0x14
0033f574: pop      {r4, r5, pc}

# _ZN13ObjectManager19DBG_DumpRoomObjectsEv
00340304: ldr      r1, [r0, #0x80]!
00340308: cmp      r1, r0
0034030c: bxeq     lr
00340310: ldr      r2, [r1, #8]
00340314: ldr      r3, [r2]
00340318: cmp      r3, r2
0034031c: beq      #0x34032c
00340320: ldr      r3, [r3]
00340324: cmp      r2, r3
00340328: bne      #0x340320
0034032c: ldr      r1, [r1]
00340330: cmp      r0, r1
00340334: bne      #0x340310
00340338: bx       lr

# _ZN13ObjectManagerC1Ev
0034a1e8: ldr      r2, [pc, #0x20c]
0034a1ec: ldr      ip, [pc, #0x20c]
0034a1f0: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034a1f4: add      r2, pc, r2
0034a1f8: ldr      ip, [r2, ip]
0034a1fc: mov      r1, r0
0034a200: mov      r3, #0
0034a204: add      ip, ip, #8
0034a208: str      ip, [r1], #4
0034a20c: add      r5, r0, #0x60
0034a210: mov      ip, r0
0034a214: str      r1, [r0, #8]
0034a218: str      r1, [r0, #4]
0034a21c: str      r3, [r0, #0x10]
0034a220: strb     r3, [ip, #0xc]!
0034a224: str      r5, [r0, #0x60]
0034a228: add      r5, r0, #0x80
0034a22c: str      r5, [r0, #0x84]
0034a230: ldr      r5, [r0, #0x60]
0034a234: add      lr, r0, #0x70
0034a238: str      lr, [r0, #0x70]
0034a23c: str      r5, [r0, #0x64]
0034a240: ldr      r5, [r0, #0x70]
0034a244: add      sb, r0, #0x24
0034a248: add      sl, r0, #0x2c
0034a24c: add      r8, r0, #0x34
0034a250: add      r7, r0, #0x3c
0034a254: add      r6, r0, #0x44
0034a258: add      fp, r0, #0x68
0034a25c: add      lr, r0, #0x88
0034a260: str      lr, [r0, #0x88]
0034a264: str      ip, [r0, #0x18]
0034a268: str      sb, [r0, #0x28]
0034a26c: str      sl, [r0, #0x30]
0034a270: str      r8, [r0, #0x38]
0034a274: str      r7, [r0, #0x40]
0034a278: str      r6, [r0, #0x48]
0034a27c: str      fp, [r0, #0x6c]
0034a280: str      r5, [r0, #0x74]
0034a284: str      ip, [r0, #0x14]
0034a288: str      sb, [r0, #0x24]
0034a28c: str      sl, [r0, #0x2c]
0034a290: str      r8, [r0, #0x34]
0034a294: str      r7, [r0, #0x3c]
0034a298: str      r6, [r0, #0x44]
0034a29c: str      r3, [r0, #0x1c]
0034a2a0: str      r3, [r0, #0x4c]
0034a2a4: str      r3, [r0, #0x50]
0034a2a8: str      r3, [r0, #0x54]
0034a2ac: str      r3, [r0, #0x58]
0034a2b0: str      r3, [r0, #0x5c]
0034a2b4: ldr      ip, [r0, #0x84]
0034a2b8: ldr      r5, [r0, #0x88]
0034a2bc: mov      r1, r0
0034a2c0: add      lr, r0, #0x90
0034a2c4: str      fp, [r0, #0x68]
0034a2c8: str      ip, [r0, #0x80]
0034a2cc: str      r5, [r0, #0x8c]
0034a2d0: str      lr, [r0, #0x94]
0034a2d4: str      lr, [r0, #0x90]
0034a2d8: str      r3, [r0, #0x78]
0034a2dc: str      r3, [r0, #0x7c]
0034a2e0: str      r3, [r0, #0x9c]
0034a2e4: mov      ip, r0
0034a2e8: strb     r3, [r1, #0x98]!
0034a2ec: str      r1, [r0, #0xa4]
0034a2f0: str      r1, [r0, #0xa0]
0034a2f4: str      r3, [r0, #0xa8]
0034a2f8: str      r3, [r0, #0xb4]
0034a2fc: mov      r1, r0
0034a300: strb     r3, [ip, #0xb0]!
0034a304: str      ip, [r0, #0xbc]
0034a308: str      ip, [r0, #0xb8]
0034a30c: str      r3, [r0, #0xc0]
0034a310: mov      ip, r0
0034a314: str      r3, [r0, #0xcc]
0034a318: strb     r3, [r1, #0xc8]!
0034a31c: str      r1, [r0, #0xd4]
0034a320: str      r1, [r0, #0xd0]
0034a324: add      lr, r0, #0x100
0034a328: mov      r1, r0
0034a32c: str      r3, [r0, #0xd8]
0034a330: str      r3, [r0, #0xe4]
0034a334: strb     r3, [ip, #0xe0]!
0034a338: add      r6, r0, #0x120
0034a33c: add      r5, r0, #0x128
0034a340: str      ip, [r0, #0xec]
0034a344: str      lr, [r0, #0x104]
0034a348: str      ip, [r0, #0xe8]
0034a34c: str      lr, [r0, #0x100]
0034a350: mov      ip, r0
0034a354: add      lr, r0, #0x130
0034a358: str      r3, [r0, #0xf0]
0034a35c: str      r3, [r0, #0xf8]
0034a360: str      r3, [r0, #0x10c]
0034a364: strb     r3, [r1, #0x108]!
0034a368: str      r1, [r0, #0x114]
0034a36c: str      r1, [r0, #0x110]
0034a370: str      r6, [r0, #0x120]
0034a374: str      r6, [r0, #0x124]
0034a378: str      r5, [r0, #0x12c]
0034a37c: str      lr, [r0, #0x134]
0034a380: str      r5, [r0, #0x128]
0034a384: str      lr, [r0, #0x130]
0034a388: str      r3, [r0, #0x118]
0034a38c: str      r3, [r0, #0x14c]
0034a390: mov      r1, r0
0034a394: strb     r3, [ip, #0x148]!
0034a398: str      ip, [r0, #0x154]
0034a39c: str      ip, [r0, #0x150]
0034a3a0: str      r3, [r0, #0x158]
0034a3a4: mov      ip, r0
0034a3a8: str      r3, [r0, #0x168]
0034a3ac: strb     r3, [r1, #0x164]!
0034a3b0: str      r1, [r0, #0x170]
0034a3b4: str      r1, [r0, #0x16c]
0034a3b8: str      r3, [r0, #0x174]
0034a3bc: mov      r1, r0
0034a3c0: str      r3, [r0, #0x180]
0034a3c4: strb     r3, [ip, #0x17c]!
0034a3c8: str      ip, [r0, #0x188]
0034a3cc: str      ip, [r0, #0x184]
0034a3d0: str      r3, [r0, #0x18c]
0034a3d4: str      r3, [r0, #0x198]
0034a3d8: strb     r3, [r1, #0x194]!
0034a3dc: mov      r4, r0
0034a3e0: str      r1, [r0, #0x1a0]
0034a3e4: str      r1, [r0, #0x19c]
0034a3e8: strb     r3, [r0, #0x1ac]
0034a3ec: str      r3, [r0, #0x1a4]
0034a3f0: bl       #0x3496b8 ; _ZN13ObjectManager5FlushEv
0034a3f4: mov      r0, r4
0034a3f8: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034a3fc: mlseq    r4, ip, r8, sl
0034a400: andeq    r1, r0, ip, asr r1

# _ZN13ObjectManager16NetworkInitLevelEv
00340be0: push     {r4, lr}
00340be4: sub      sp, sp, #8
00340be8: mov      r4, r0
00340bec: bl       #0x7fd794 ; _Z9GetOnlinev
00340bf0: ldrb     r3, [r0, #5]
00340bf4: ldr      r0, [pc, #0x44]
00340bf8: cmp      r3, #0
00340bfc: add      r0, pc, r0
00340c00: beq      #0x340c30
00340c04: ldr      r3, [pc, #0x38]
00340c08: ldr      ip, [pc, #0x38]
00340c0c: ldr      r1, [r0, r3]
00340c10: ldr      r3, [pc, #0x34]
00340c14: ldr      ip, [r0, ip]
00340c18: ldr      r2, [r0, r3]
00340c1c: ldr      r3, [pc, #0x2c]
00340c20: str      ip, [sp]
00340c24: ldr      r3, [r0, r3]
00340c28: mov      r0, #3
00340c2c: bl       #0x815258 ; _ZN14CPacketManager18RegisterPacketSlotE12PACKET_SLOTSPFbiiR12NetBitStreamEPFviiS2_EPFviiES8_
00340c30: mov      r3, #1
00340c34: strb     r3, [r4, #0x1ac]
00340c38: add      sp, sp, #8
00340c3c: pop      {r4, pc}
00340c40: mlseq    r5, r4, lr, r3
00340c44: andeq    r1, r0, ip, ror ip
00340c48: andeq    r2, r0, r0, asr #20
00340c4c: andeq    r1, r0, ip, asr r6
00340c50: andeq    r0, r0, r4, asr #17

# _ZNK13ObjectManager19GetNumObjectsByTypeEPKc
00345bcc: push     {r4, r5, lr}
00345bd0: sub      sp, sp, #0xc
00345bd4: mov      r2, sp
00345bd8: str      sp, [sp]
00345bdc: str      sp, [sp, #4]
00345be0: bl       #0x3426b0 ; _ZNK13ObjectManager16GetObjectsByTypeEPKcRSt4listIP9CharacterSaIS4_EE
00345be4: ldr      r3, [sp]
00345be8: mov      r4, sp
00345bec: cmp      r3, r4
00345bf0: moveq    r5, #0
00345bf4: beq      #0x345c0c
00345bf8: mov      r5, #0
00345bfc: ldr      r3, [r3]
00345c00: add      r5, r5, #1
00345c04: cmp      r3, r4
00345c08: bne      #0x345bfc
00345c0c: mov      r0, sp
00345c10: bl       #0x345b8c ; _ZNSt4priv10_List_baseIP9CharacterSaIS2_EE5clearEv
00345c14: mov      r0, r5
00345c18: add      sp, sp, #0xc
00345c1c: pop      {r4, r5, pc}

# _ZN8RoomZone17DeclarePropertiesEv
00396554: b        #0x397df8

# _ZN13ObjectManager6UpdateEf
0034a620: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034a624: ldr      r7, [pc, #0x5c4]
0034a628: ldr      sl, [pc, #0x5c4]
0034a62c: mov      r5, r0
0034a630: add      r7, pc, r7
0034a634: ldr      r3, [r7, sl]
0034a638: ldr      r0, [pc, #0x5b8]
0034a63c: sub      sp, sp, #0xb4
0034a640: ldr      r3, [r3]
0034a644: add      r0, pc, r0
0034a648: mov      r4, r1
0034a64c: str      r3, [sp, #0xac]
0034a650: bl       #0x3136b4 ; _Z20PushProfilingContextPKc
0034a654: bl       #0x7fd794 ; _Z9GetOnlinev
0034a658: ldrb     r3, [r0, #5]
0034a65c: cmp      r3, #0
0034a660: movne    r3, #1
0034a664: strbne   r3, [r5, #0xfd]
0034a668: strbne   r3, [r5, #0xfc]
0034a66c: ldr      r3, [pc, #0x588]
0034a670: ldr      r0, [r7, r3]
0034a674: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
0034a678: cmp      r0, #0
0034a67c: beq      #0x34a68c
0034a680: ldrb     r3, [r0, #0x144]
0034a684: cmp      r3, #0
0034a688: bne      #0x34aa8c
0034a68c: mov      r0, r5
0034a690: bl       #0x34163c ; _ZN13ObjectManager11UpdateRoomsEv
0034a694: mov      r1, r4
0034a698: mov      r0, r5
0034a69c: bl       #0x340274 ; _ZN13ObjectManager20DoRemoteUpdateUpdateEf
0034a6a0: mov      r4, r5
0034a6a4: mov      r0, r5
0034a6a8: bl       #0x3460cc ; _ZN13ObjectManager34ProcessNextGameObjectToStartUpdateEv
0034a6ac: ldr      ip, [r4, #0x34]!
0034a6b0: cmp      ip, r4
0034a6b4: addeq    r8, r5, #0x2c
0034a6b8: beq      #0x34a700
0034a6bc: mov      r3, ip
0034a6c0: ldr      r3, [r3]
0034a6c4: cmp      r4, r3
0034a6c8: bne      #0x34a6c0
0034a6cc: add      r8, r5, #0x2c
0034a6d0: mov      r0, r8
0034a6d4: str      ip, [sp, #0x68]
0034a6d8: add      r1, sp, #0x64
0034a6dc: add      ip, sp, #0x70
0034a6e0: add      r2, sp, #0x68
0034a6e4: add      r3, sp, #0x6c
0034a6e8: str      ip, [sp]
0034a6ec: str      r8, [sp, #0x64]
0034a6f0: str      r4, [sp, #0x6c]
0034a6f4: bl       #0x345428 ; _ZNSt4listIP10ObjectBaseSaIS1_EE25_M_splice_insert_dispatchINSt4priv14_List_iteratorIS1_St16_Nonconst_traitsIS1_EEEEEvS9_T_SA_RKSt12__false_type
0034a6f8: mov      r0, r4
0034a6fc: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
0034a700: mov      r6, r5
0034a704: ldr      r4, [r6, #0x3c]!
0034a708: cmp      r4, r6
0034a70c: beq      #0x34a760
0034a710: mov      r3, r4
0034a714: ldr      r3, [r3]
0034a718: cmp      r6, r3
0034a71c: bne      #0x34a714
0034a720: add      r2, sp, #0x54
0034a724: cmp      r4, r6
0034a728: add      sb, sp, #0x48
0034a72c: str      r2, [sp, #0xc]
0034a730: beq      #0x34a760
0034a734: ldr      r1, [r4, #8]
0034a738: ldrb     r3, [r1, #0x29]
0034a73c: cmp      r3, #0
0034a740: beq      #0x34ab14
0034a744: ldrb     r3, [r1, #0x82]
0034a748: cmp      r3, #0
0034a74c: bne      #0x34ab20
0034a750: ldr      fp, [r4]
0034a754: mov      r4, fp
0034a758: cmp      r4, r6
0034a75c: bne      #0x34a734
0034a760: mov      sb, r5
0034a764: ldr      r6, [sb, #0x44]!
0034a768: cmp      sb, r6
0034a76c: beq      #0x34a7b0
0034a770: ldr      r4, [r6, #8]
0034a774: ldrb     r3, [r4, #0xac]
0034a778: cmp      r3, #0
0034a77c: bne      #0x34a9c0
0034a780: ldr      r3, [r4, #0xa8]
0034a784: cmp      r3, #0
0034a788: beq      #0x34a9c0
0034a78c: mov      r1, #1
0034a790: mov      r0, r4
0034a794: bl       #0x33e6d4 ; _ZN10ObjectBase19TestEnableConditionEb
0034a798: mov      r0, r4
0034a79c: mov      r1, #1
0034a7a0: bl       #0x33e61c ; _ZN10ObjectBase20TestDisableConditionEb
0034a7a4: ldr      r6, [r6]
0034a7a8: cmp      sb, r6
0034a7ac: bne      #0x34a770
0034a7b0: mov      r3, #0
0034a7b4: str      r3, [r5, #0x58]
0034a7b8: str      r3, [r5, #0x5c]
0034a7bc: ldr      r4, [r5, #0x2c]
0034a7c0: add      r3, sp, #0x3c
0034a7c4: str      r3, [sp, #0xc]
0034a7c8: ldr      r3, [pc, #0x430]
0034a7cc: add      ip, sp, #0x24
0034a7d0: str      ip, [sp, #0x10]
0034a7d4: add      r2, sp, #0x30
0034a7d8: add      ip, sp, #0x60
0034a7dc: cmp      r8, r4
0034a7e0: add      r6, r5, #0x70
0034a7e4: str      r2, [sp, #0x14]
0034a7e8: str      r3, [sp, #0x18]
0034a7ec: str      ip, [sp, #0x1c]
0034a7f0: beq      #0x34a8dc
0034a7f4: ldr      sb, [r4, #8]
0034a7f8: cmp      sb, #0
0034a7fc: beq      #0x34a8cc
0034a800: ldr      r3, [sb]
0034a804: mov      r0, sb
0034a808: mov      lr, pc
0034a80c: ldr      pc, [r3, #0x24]
0034a810: cmp      r0, #0
0034a814: bne      #0x34aa64
0034a818: ldrb     r3, [sb, #0x85]
0034a81c: cmp      r3, #0
0034a820: beq      #0x34aa10
0034a824: ldrb     r3, [sb, #0x8a]
0034a828: cmp      r3, #0
0034a82c: beq      #0x34aa10
0034a830: ldrb     fp, [sb, #0x81]
0034a834: cmp      fp, #0
0034a838: bne      #0x34a9e0
0034a83c: ldr      r3, [sb]
0034a840: mov      r0, sb
0034a844: strb     fp, [sb, #0x88]
0034a848: mov      lr, pc
0034a84c: ldr      pc, [r3, #0x2c]
0034a850: ldr      r0, [sp, #0xc]
0034a854: mov      r1, sb
0034a858: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0034a85c: ldr      r0, [sp, #0xc]
0034a860: mov      r1, fp
0034a864: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034a868: cmp      r0, #0
0034a86c: beq      #0x34a8cc
0034a870: ldrb     r3, [sb, #0x88]
0034a874: ldrb     r2, [sb, #0x89]
0034a878: cmp      r2, r3
0034a87c: beq      #0x34a8cc
0034a880: cmp      r3, #0
0034a884: strb     r3, [sb, #0x89]
0034a888: bne      #0x34aad4
0034a88c: mov      r1, sb
0034a890: ldr      r0, [sp, #0x10]
0034a894: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0034a898: ldr      r0, [sp, #0x10]
0034a89c: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0034a8a0: mov      fp, r0
0034a8a4: ldr      r0, [r5, #0x70]
0034a8a8: cmp      r6, r0
0034a8ac: beq      #0x34a8cc
0034a8b0: ldr      r3, [r0, #8]
0034a8b4: ldr      sb, [r0]
0034a8b8: cmp      fp, r3
0034a8bc: beq      #0x34aab8
0034a8c0: mov      r0, sb
0034a8c4: cmp      r6, r0
0034a8c8: bne      #0x34a8b0
0034a8cc: ldr      sb, [r4]
0034a8d0: mov      r4, sb
0034a8d4: cmp      r8, r4
0034a8d8: bne      #0x34a7f4
0034a8dc: ldr      r5, [pc, #0x320]
0034a8e0: add      r4, sp, #0x94
0034a8e4: ldr      r6, [r7, r5]
0034a8e8: mov      r0, r6
0034a8ec: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0034a8f0: ldr      r1, [pc, #0x310]
0034a8f4: add      r2, sp, #0x78
0034a8f8: mov      r0, r4
0034a8fc: add      r1, pc, r1
0034a900: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0034a904: mov      r0, r6
0034a908: mov      r1, r4
0034a90c: mov      r2, #0
0034a910: bl       #0x337ddc ; _ZN13DebugSwitches9SetSwitchERKSsb
0034a914: ldr      r0, [sp, #0xa8]
0034a918: cmp      r0, r4
0034a91c: beq      #0x34a93c
0034a920: cmp      r0, #0
0034a924: beq      #0x34a93c
0034a928: ldr      r1, [sp, #0x94]
0034a92c: rsb      r1, r0, r1
0034a930: cmp      r1, #0x80
0034a934: bhi      #0x34abdc
0034a938: bl       #0x708f00 ; 
0034a93c: ldr      r5, [r7, r5]
0034a940: add      r4, sp, #0x7c
0034a944: mov      r0, r5
0034a948: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0034a94c: ldr      r1, [pc, #0x2b8]
0034a950: add      r2, sp, #0x74
0034a954: mov      r0, r4
0034a958: add      r1, pc, r1
0034a95c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0034a960: mov      r0, r5
0034a964: mov      r1, r4
0034a968: mov      r2, #0
0034a96c: bl       #0x337ddc ; _ZN13DebugSwitches9SetSwitchERKSsb
0034a970: ldr      r0, [sp, #0x90]
0034a974: cmp      r0, r4
0034a978: beq      #0x34a998
0034a97c: cmp      r0, #0
0034a980: beq      #0x34a998
0034a984: ldr      r1, [sp, #0x7c]
0034a988: rsb      r1, r0, r1
0034a98c: cmp      r1, #0x80
0034a990: bhi      #0x34abe4
0034a994: bl       #0x708f00 ; 
0034a998: ldr      r0, [pc, #0x270]
0034a99c: add      r0, pc, r0
0034a9a0: bl       #0x3136b8 ; _Z19PopProfilingContextPKc
0034a9a4: ldr      r3, [r7, sl]
0034a9a8: ldr      r2, [sp, #0xac]
0034a9ac: ldr      r3, [r3]
0034a9b0: cmp      r2, r3
0034a9b4: bne      #0x34abec
0034a9b8: add      sp, sp, #0xb4
0034a9bc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034a9c0: ldrb     r3, [r4, #0xd0]
0034a9c4: cmp      r3, #0
0034a9c8: bne      #0x34a7a4
0034a9cc: ldr      r3, [r4, #0xcc]
0034a9d0: cmp      r3, #0
0034a9d4: bne      #0x34a78c
0034a9d8: ldr      r6, [r6]
0034a9dc: b        #0x34a7a8
0034a9e0: mov      r1, sb
0034a9e4: mov      r0, r5
0034a9e8: bl       #0x3432f8 ; _ZN13ObjectManager15MarkForDeletionEP10ObjectBase
0034a9ec: ldr      sb, [r4]
0034a9f0: ldr      r3, [r4, #4]
0034a9f4: mov      r0, r4
0034a9f8: mov      r1, #0xc
0034a9fc: str      sb, [r3]
0034aa00: str      r3, [sb, #4]
0034aa04: bl       #0x708f00 ; 
0034aa08: mov      r4, sb
0034aa0c: b        #0x34a8d4
0034aa10: bl       #0x7fd794 ; _Z9GetOnlinev
0034aa14: ldrb     r3, [r0, #5]
0034aa18: cmp      r3, #0
0034aa1c: bne      #0x34aa70
0034aa20: mov      r2, #0
0034aa24: strb     r2, [sb, #0x86]
0034aa28: ldr      r3, [sb]
0034aa2c: mov      r0, sb
0034aa30: mov      lr, pc
0034aa34: ldr      pc, [r3, #0x24]
0034aa38: cmp      r0, #0
0034aa3c: beq      #0x34a8cc
0034aa40: ldr      ip, [sp, #0x18]
0034aa44: mov      r0, sb
0034aa48: ldr      r1, [sp, #0x1c]
0034aa4c: ldr      r3, [r7, ip]
0034aa50: mov      r2, #0
0034aa54: str      r3, [sp, #0x60]
0034aa58: bl       #0x3a7b24 ; _ZN9Character19UnLoadScriptProcessENSt4priv17_Rb_tree_iteratorISt4pairIKiPS_ENS0_11_MapTraitsTIS5_EEEEb
0034aa5c: ldr      sb, [r4]
0034aa60: b        #0x34a8d0
0034aa64: mov      r0, sb
0034aa68: bl       #0x3a4344 ; _ZN9Character16UpdateAIPointersEv
0034aa6c: b        #0x34a818
0034aa70: ldr      r3, [sb]
0034aa74: mov      r0, sb
0034aa78: mov      lr, pc
0034aa7c: ldr      pc, [r3, #0x54]
0034aa80: cmp      r0, #0
0034aa84: beq      #0x34aa20
0034aa88: b        #0x34a830
0034aa8c: ldrb     r3, [r0, #0x198]
0034aa90: cmp      r3, #0
0034aa94: bne      #0x34a68c
0034aa98: bl       #0x7fd794 ; _Z9GetOnlinev
0034aa9c: ldrb     r3, [r0, #5]
0034aaa0: cmp      r3, #0
0034aaa4: bne      #0x34a68c
0034aaa8: ldr      r0, [pc, #0x164]
0034aaac: add      r0, pc, r0
0034aab0: bl       #0x3136b8 ; _Z19PopProfilingContextPKc
0034aab4: b        #0x34a9a4
0034aab8: ldr      r3, [r0, #4]
0034aabc: mov      r1, #0xc
0034aac0: str      sb, [r3]
0034aac4: str      r3, [sb, #4]
0034aac8: bl       #0x708f00 ; 
0034aacc: mov      r0, sb
0034aad0: b        #0x34a8c4
0034aad4: mov      r1, sb
0034aad8: ldr      r0, [sp, #0x14]
0034aadc: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
0034aae0: ldr      r0, [sp, #0x14]
0034aae4: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0034aae8: mov      sb, r0
0034aaec: mov      r0, r6
0034aaf0: bl       #0x342690 ; 
0034aaf4: str      sb, [r0, #8]
0034aaf8: ldr      r3, [r5, #0x74]
0034aafc: str      r6, [r0]
0034ab00: str      r3, [r0, #4]
0034ab04: str      r0, [r3]
0034ab08: str      r0, [r5, #0x74]
0034ab0c: ldr      sb, [r4]
0034ab10: b        #0x34a8d0
0034ab14: ldrb     r3, [r1, #0x82]
0034ab18: cmp      r3, #0
0034ab1c: beq      #0x34ab30
0034ab20: sub      r3, r3, #1
0034ab24: strb     r3, [r1, #0x82]
0034ab28: ldr      fp, [r4]
0034ab2c: b        #0x34a754
0034ab30: mov      r0, r5
0034ab34: bl       #0x347dec ; _ZN13ObjectManager16IsOnlineDeferredEP10ObjectBase
0034ab38: cmp      r0, #0
0034ab3c: bne      #0x34ab98
0034ab40: ldr      r3, [r4, #8]
0034ab44: mov      r0, r3
0034ab48: ldr      r3, [r3]
0034ab4c: mov      lr, pc
0034ab50: ldr      pc, [r3, #0x24]
0034ab54: cmp      r0, #0
0034ab58: bne      #0x34aba0
0034ab5c: ldr      r1, [r4, #8]
0034ab60: mov      r0, sb
0034ab64: bl       #0x33f524 ; _ZN12ObjectHandleC1EP10ObjectBase
0034ab68: ldm      sb, {r1, r2, r3}
0034ab6c: mov      r0, r5
0034ab70: bl       #0x348ea4 ; _ZN13ObjectManager6RemoveE12ObjectHandle
0034ab74: ldr      fp, [r4]
0034ab78: ldr      r3, [r4, #4]
0034ab7c: mov      r0, r4
0034ab80: mov      r1, #0xc
0034ab84: str      fp, [r3]
0034ab88: str      r3, [fp, #4]
0034ab8c: bl       #0x708f00 ; 
0034ab90: mov      r4, fp
0034ab94: b        #0x34a758
0034ab98: ldr      r1, [r4, #8]
0034ab9c: b        #0x34a744
0034aba0: ldr      r3, [r4, #8]
0034aba4: mov      r0, r3
0034aba8: ldr      r3, [r3]
0034abac: mov      lr, pc
0034abb0: ldr      pc, [r3, #0x28]
0034abb4: cmp      r0, #0
0034abb8: beq      #0x34ab5c
0034abbc: ldr      r1, [r4, #8]
0034abc0: ldr      r0, [sp, #0xc]
0034abc4: bl       #0x33f524 ; _ZN12ObjectHandleC1EP10ObjectBase
0034abc8: ldr      ip, [sp, #0xc]
0034abcc: mov      r0, r5
0034abd0: ldm      ip, {r1, r2, r3}
0034abd4: bl       #0x349240 ; _ZN13ObjectManager10FakeRemoveE12ObjectHandle
0034abd8: b        #0x34ab74
0034abdc: bl       #0x310440 ; _Z10CustomFreePv
0034abe0: b        #0x34a93c
0034abe4: bl       #0x310440 ; _Z10CustomFreePv
0034abe8: b        #0x34a998
0034abec: bl       #0x30e310 ; 
0034abf0: rsbeq    sl, r4, r0, ror #8
0034abf4: andeq    r4, r0, ip, lsr #1
0034abf8: subseq   r5, r7, ip, lsr sp
0034abfc: strdeq   r3, r4, [r0], -r4
0034ac00: andeq    r1, r0, r4, lsr r1
0034ac04: andeq    r0, r0, r4, lsl #17

# _ZN8RoomZone12RemoveObjectEP10GameObject
003968cc: ldr      ip, [r0, #0x394]!
003968d0: cmp      ip, r0
003968d4: beq      #0x3968f4
003968d8: ldr      r3, [ip, #8]
003968dc: cmp      r3, r1
003968e0: beq      #0x3968f4
003968e4: ldr      ip, [ip]
003968e8: cmp      r0, ip
003968ec: bne      #0x3968d8
003968f0: mov      ip, r0
003968f4: cmp      r0, ip
003968f8: bxeq     lr
003968fc: ldr      r3, [ip]
00396900: ldr      r2, [ip, #4]
00396904: mov      r0, ip
00396908: mov      r1, #0xc
0039690c: str      r3, [r2]
00396910: str      r2, [r3, #4]
00396914: b        #0x708f00

# _ZN13ObjectManager18RemoveNoRoomObjectEP10GameObject
003462b8: push     {r4, lr}
003462bc: ldr      r3, [r0, #0x88]!
003462c0: mov      r4, r1
003462c4: cmp      r3, r0
003462c8: beq      #0x3462e8
003462cc: ldr      r2, [r3, #8]
003462d0: cmp      r2, r4
003462d4: beq      #0x3462e8
003462d8: ldr      r3, [r3]
003462dc: cmp      r0, r3
003462e0: bne      #0x3462cc
003462e4: mov      r3, r0
003462e8: cmp      r0, r3
003462ec: beq      #0x346310
003462f0: ldm      r3, {r2, ip}
003462f4: mov      r0, r3
003462f8: mov      r1, #0xc
003462fc: str      r2, [ip]
00346300: str      ip, [r2, #4]
00346304: bl       #0x708f00 ; 
00346308: mov      r3, #0
0034630c: strb     r3, [r4, #0x2f8]
00346310: pop      {r4, pc}

# _ZN13ObjectManager5SpawnEPKcS1_bb
0034b724: push     {r4, r5, r6, r7, r8, lr}
0034b728: sub      sp, sp, #8
0034b72c: ldrb     ip, [sp, #0x24]
0034b730: mvn      lr, #0
0034b734: mov      r4, r0
0034b738: str      lr, [sp]
0034b73c: str      ip, [sp, #4]
0034b740: mov      r6, r1
0034b744: mov      r5, r2
0034b748: mov      r7, r3
0034b74c: ldrb     r8, [sp, #0x20]
0034b750: bl       #0x34b520 ; _ZN13ObjectManager12GetNewObjectEPKcS1_ib
0034b754: mov      r0, r4
0034b758: mov      r1, #0
0034b75c: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b760: cmp      r0, #0
0034b764: beq      #0x34b7f4
0034b768: mov      r1, #1
0034b76c: mov      r0, r4
0034b770: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b774: add      r0, r0, #4
0034b778: bl       #0x513d78 ; _ZN11PropertyMap14InitPropertiesEv
0034b77c: mov      r1, #1
0034b780: mov      r0, r4
0034b784: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b788: add      r0, r0, #4
0034b78c: bl       #0x5136ec ; _ZN11PropertyMap21LoadDefaultPropertiesEv
0034b790: mov      r1, #1
0034b794: mov      r0, r4
0034b798: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b79c: mov      r1, r7
0034b7a0: bl       #0x34ac18 ; _ZN10ObjectBase7SetNameEPKc
0034b7a4: mov      r1, #1
0034b7a8: mov      r0, r4
0034b7ac: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b7b0: mov      r7, r0
0034b7b4: mov      r0, r5
0034b7b8: bl       #0x30de54 ; 
0034b7bc: mov      r1, r5
0034b7c0: add      r2, r5, r0
0034b7c4: add      r0, r7, #0x48
0034b7c8: bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
0034b7cc: cmp      r8, #0
0034b7d0: beq      #0x34b838
0034b7d4: mov      r1, #1
0034b7d8: mov      r0, r4
0034b7dc: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b7e0: ldr      r3, [r0]
0034b7e4: mov      lr, pc
0034b7e8: ldr      pc, [r3, #0x38]
0034b7ec: cmp      r0, #0
0034b7f0: bne      #0x34b800
0034b7f4: mov      r0, r4
0034b7f8: add      sp, sp, #8
0034b7fc: pop      {r4, r5, r6, r7, r8, pc}
0034b800: mov      r1, #0
0034b804: mov      r0, r4
0034b808: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b80c: add      r5, r6, #0x34
0034b810: mov      r7, r0
0034b814: mov      r0, r5
0034b818: bl       #0x343168 ; 
0034b81c: str      r7, [r0, #8]
0034b820: ldr      r3, [r6, #0x38]
0034b824: str      r5, [r0]
0034b828: str      r3, [r0, #4]
0034b82c: str      r0, [r3]
0034b830: str      r0, [r6, #0x38]
0034b834: b        #0x34b7f4
0034b838: mov      r1, #1
0034b83c: mov      r0, r4
0034b840: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b844: ldr      r3, [r0]
0034b848: mov      lr, pc
0034b84c: ldr      pc, [r3, #0x1c]
0034b850: mov      r0, r4
0034b854: mov      r1, #1
0034b858: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
0034b85c: mov      r1, #1
0034b860: bl       #0x33e6d4 ; _ZN10ObjectBase19TestEnableConditionEb
0034b864: b        #0x34b7d4

# _ZN8RoomZone9AddObjectEP10GameObject
0039672c: push     {r4, r5, lr}
00396730: mov      r4, r0
00396734: ldr      r3, [r4, #0x394]!
00396738: sub      sp, sp, #0xc
0039673c: mov      r5, r0
00396740: cmp      r3, r4
00396744: beq      #0x396764
00396748: ldr      r2, [r3, #8]
0039674c: cmp      r1, r2
00396750: beq      #0x396764
00396754: ldr      r3, [r3]
00396758: cmp      r4, r3
0039675c: bne      #0x396748
00396760: mov      r3, r4
00396764: cmp      r4, r3
00396768: beq      #0x396774
0039676c: add      sp, sp, #0xc
00396770: pop      {r4, r5, pc}
00396774: mov      r0, r4
00396778: str      r1, [sp, #4]
0039677c: bl       #0x39670c ; 
00396780: ldr      r1, [sp, #4]
00396784: str      r1, [r0, #8]
00396788: ldr      r3, [r5, #0x398]
0039678c: str      r4, [r0]
00396790: str      r3, [r0, #4]
00396794: str      r0, [r3]
00396798: str      r0, [r5, #0x398]
0039679c: b        #0x39676c

# _ZN13ObjectManager21AssignObjectNetworkIdEP10ObjectBase
003431c0: push     {r4, r5, r6, r7, r8, lr}
003431c4: mov      r5, r0
003431c8: mov      r4, r1
003431cc: bl       #0x7fd794 ; _Z9GetOnlinev
003431d0: ldrb     r3, [r0, #5]
003431d4: ldr      r6, [pc, #0x110]
003431d8: cmp      r3, #0
003431dc: add      r6, pc, r6
003431e0: beq      #0x343274
003431e4: ldr      r7, [r4, #0x44]
003431e8: ldr      r1, [pc, #0x100]
003431ec: mov      r0, r7
003431f0: add      r1, pc, r1
003431f4: bl       #0x30ebd4 ; 
003431f8: cmp      r0, r7
003431fc: beq      #0x3432d0
00343200: ldr      r3, [r4]
00343204: mov      r0, r4
00343208: mov      lr, pc
0034320c: ldr      pc, [r3, #0x24]
00343210: cmp      r0, #0
00343214: bne      #0x343278
00343218: ldr      r0, [r5, #0x13c]
0034321c: add      r3, r0, #1
00343220: str      r3, [r5, #0x13c]
00343224: add      r0, r0, #5
00343228: cmp      r0, #4
0034322c: str      r0, [r4, #0x108]
00343230: bgt      #0x3432b4
00343234: add      r6, r5, #0x100
00343238: mov      r0, r6
0034323c: bl       #0x343168 ; 
00343240: str      r4, [r0, #8]
00343244: ldr      r3, [r5, #0x104]
00343248: str      r6, [r0]
0034324c: str      r3, [r0, #4]
00343250: str      r0, [r3]
00343254: str      r0, [r5, #0x104]
00343258: ldr      r3, [r4, #0x100]
0034325c: cmp      r3, #0
00343260: beq      #0x343274
00343264: ldr      r3, [r5, #0x54]
00343268: add      r3, r3, #1
0034326c: str      r3, [r5, #0x54]
00343270: pop      {r4, r5, r6, r7, r8, pc}
00343274: pop      {r4, r5, r6, r7, r8, pc}
00343278: movw     r3, #0x14e4
0034327c: ldrb     r3, [r4, r3]
00343280: cmp      r3, #0
00343284: beq      #0x3432a0
00343288: ldr      r0, [r5, #0x140]
0034328c: add      r3, r0, #1
00343290: add      r0, r0, #0x2700
00343294: str      r3, [r5, #0x140]
00343298: add      r0, r0, #0x11
0034329c: b        #0x343228
003432a0: mov      r0, r4
003432a4: bl       #0x3a30ac ; _ZNK9Character10IsSummonedEv
003432a8: cmp      r0, #0
003432ac: beq      #0x343218
003432b0: b        #0x343288
003432b4: ldr      r3, [pc, #0x38]
003432b8: mov      r1, #1
003432bc: ldr      r3, [r6, r3]
003432c0: ldr      r3, [r3]
003432c4: str      r3, [r4, #0xfc]
003432c8: bl       #0x33ff90 ; _ZN6Random9GetRandomEib
003432cc: b        #0x343234
003432d0: add      r0, r0, #0x10
003432d4: bl       #0x30e094 ; 
003432d8: ldr      r3, [r5, #0x138]
003432dc: add      r0, r0, #1
003432e0: add      r3, r3, #1
003432e4: str      r3, [r5, #0x138]
003432e8: b        #0x343228
003432ec: strhteq  r1, [r5], #-0x84
003432f0: subseq   sp, r7, r8, asr r1
003432f4: andeq    r0, r0, r0, lsl fp

# _ZN12ObjectHandle9GetObjectEb
0033fdc0: push     {r4, r5, r6, r7, r8, lr}
0033fdc4: ldr      r4, [r0]
0033fdc8: ldr      r5, [pc, #0xc0]
0033fdcc: sub      sp, sp, #8
0033fdd0: cmp      r4, #0
0033fdd4: mov      r6, r0
0033fdd8: mov      r7, r1
0033fddc: add      r5, pc, r5
0033fde0: beq      #0x33fe20
0033fde4: ldr      r3, [pc, #0xa8]
0033fde8: ldr      r4, [r0, #4]
0033fdec: ldr      r3, [r5, r3]
0033fdf0: cmp      r4, #0
0033fdf4: ldr      r0, [r3, #0x38]
0033fdf8: ldr      r8, [r0, #0x78]
0033fdfc: beq      #0x33fe0c
0033fe00: ldr      r3, [r6, #8]
0033fe04: cmp      r3, r8
0033fe08: beq      #0x33fe20
0033fe0c: add      r0, r0, #0xc
0033fe10: mov      r1, r6
0033fe14: bl       #0x33fc88 ; _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
0033fe18: ldr      r4, [r0, #0x18]
0033fe1c: stmib    r6, {r4, r8}
0033fe20: cmp      r7, #0
0033fe24: beq      #0x33fe30
0033fe28: cmp      r4, #0
0033fe2c: beq      #0x33fe3c
0033fe30: mov      r0, r4
0033fe34: add      sp, sp, #8
0033fe38: pop      {r4, r5, r6, r7, r8, pc}
0033fe3c: ldr      r3, [pc, #0x54]
0033fe40: ldr      r3, [r5, r3]
0033fe44: ldr      r3, [r3]
0033fe48: cmp      r3, #2
0033fe4c: streq    r4, [r4]
0033fe50: beq      #0x33fe30
0033fe54: cmp      r3, #1
0033fe58: bne      #0x33fe30
0033fe5c: ldr      r0, [pc, #0x38]
0033fe60: ldr      r1, [pc, #0x38]
0033fe64: ldr      r2, [pc, #0x38]
0033fe68: ldr      r0, [r5, r0]
0033fe6c: ldr      r3, [pc, #0x34]
0033fe70: mov      ip, #0x31
0033fe74: add      r1, pc, r1
0033fe78: add      r2, pc, r2
0033fe7c: add      r3, pc, r3
0033fe80: add      r0, r0, #0xa8
0033fe84: str      ip, [sp]
0033fe88: bl       #0x30e004 ; 
0033fe8c: b        #0x33fe30
0033fe90: strhteq  r4, [r5], #-0xc4
0033fe94: strdeq   r3, r4, [r0], -r4
0033fe98: andeq    r3, r0, r0, asr #19
0033fe9c: andeq    r1, r0, r0, asr #19
0033fea0: subseq   lr, r7, r4, ror #10

# _ZNK8RoomZone9HasInsideERK7Point3DIfE
003964cc: push     {r4, r5, r6, lr}
003964d0: ldr      r5, [r1]
003964d4: mov      r6, r1
003964d8: mov      r4, r0
003964dc: mov      r1, r5
003964e0: ldr      r0, [r0, #0x12c]
003964e4: bl       #0x30e9ac ; 
003964e8: cmp      r0, #0
003964ec: beq      #0x39653c
003964f0: mov      r0, r5
003964f4: ldr      r1, [r4, #0x138]
003964f8: bl       #0x30e9ac ; 
003964fc: cmp      r0, #0
00396500: beq      #0x39653c
00396504: ldr      r5, [r6, #4]
00396508: ldr      r0, [r4, #0x130]
0039650c: mov      r1, r5
00396510: bl       #0x30e9ac ; 
00396514: cmp      r0, #0
00396518: beq      #0x39653c
0039651c: mov      r0, r5
00396520: ldr      r1, [r4, #0x13c]
00396524: bl       #0x30e9ac ; 
00396528: cmp      r0, #0
0039652c: mov      r0, #0
00396530: movne    r0, #1
00396534: uxtb     r0, r0
00396538: pop      {r4, r5, r6, pc}
0039653c: mov      r0, #0
00396540: pop      {r4, r5, r6, pc}

# _ZN13ObjectManager11UpdateRoomsEv
0034163c: push     {r4, r5, r6, lr}
00341640: mov      r4, r0
00341644: mov      r5, r0
00341648: ldr      r0, [pc, #0x44]
0034164c: add      r0, pc, r0
00341650: bl       #0x3136b4 ; _Z20PushProfilingContextPKc
00341654: mov      r3, #0
00341658: str      r3, [r4, #0xf8]
0034165c: ldr      r4, [r5, #0x24]!
00341660: b        #0x34167c
00341664: ldr      r3, [r4, #8]
00341668: mov      r0, r3
0034166c: ldr      r3, [r3]
00341670: mov      lr, pc
00341674: ldr      pc, [r3, #0x2c]
00341678: ldr      r4, [r4]
0034167c: cmp      r5, r4
00341680: bne      #0x341664
00341684: ldr      r0, [pc, #0xc]
00341688: add      r0, pc, r0
0034168c: pop      {r4, r5, r6, lr}
00341690: b        #0x3136b8
00341694: subseq   lr, r7, ip, lsr #24
00341698: ldrsheq  lr, [r7], #-0xb0

# _ZNK8RoomZone9IsZonableEv
003964a0: mov      r0, #0
003964a4: bx       lr

# _ZN17v2MixedController11Ctrl_LookAtEP10GameObject
00408c44: b        #0x4052bc

# _ZN8RoomZone8ActivateEv
00396d24: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00396d28: ldr      r7, [pc, #0x154]
00396d2c: ldr      r2, [pc, #0x154]
00396d30: ldr      sl, [pc, #0x154]
00396d34: add      r7, pc, r7
00396d38: ldr      r3, [r7, r2]
00396d3c: ldr      r5, [r7, sl]
00396d40: sub      sp, sp, #0x4c
00396d44: ldr      r3, [r3]
00396d48: mov      r8, r0
00396d4c: mov      r0, r5
00396d50: str      r3, [sp, #0x44]
00396d54: str      r2, [sp]
00396d58: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00396d5c: ldr      r1, [pc, #0x12c]
00396d60: add      r4, sp, #0x2c
00396d64: add      r2, sp, #0x10
00396d68: add      r1, pc, r1
00396d6c: mov      r0, r4
00396d70: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00396d74: mov      r0, r5
00396d78: mov      r1, r4
00396d7c: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00396d80: ldr      r0, [sp, #0x40]
00396d84: cmp      r0, r4
00396d88: beq      #0x396da8
00396d8c: cmp      r0, #0
00396d90: beq      #0x396da8
00396d94: ldr      r1, [sp, #0x2c]
00396d98: rsb      r1, r0, r1
00396d9c: cmp      r1, #0x80
00396da0: bhi      #0x396e78
00396da4: bl       #0x708f00 ; 
00396da8: ldr      r3, [pc, #0xe4]
00396dac: ldr      r4, [r8, #0x394]!
00396db0: ldr      sb, [pc, #0xe0]
00396db4: ldr      r3, [r7, r3]
00396db8: cmp      r8, r4
00396dbc: add      sb, pc, sb
00396dc0: ldr      r3, [r3, #0x38]
00396dc4: add      r5, sp, #0x14
00396dc8: add      fp, sp, #0xc
00396dcc: str      r3, [sp, #4]
00396dd0: beq      #0x396e40
00396dd4: ldr      r0, [r4, #8]
00396dd8: cmp      r0, #0
00396ddc: beq      #0x396de4
00396de0: bl       #0x38c710 ; _ZN10GameObject11ZoneEnteredEv
00396de4: ldr      r6, [r7, sl]
00396de8: mov      r0, r6
00396dec: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00396df0: mov      r1, sb
00396df4: mov      r2, fp
00396df8: mov      r0, r5
00396dfc: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00396e00: mov      r0, r6
00396e04: mov      r1, r5
00396e08: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00396e0c: ldr      r0, [sp, #0x28]
00396e10: cmp      r0, r5
00396e14: beq      #0x396e34
00396e18: cmp      r0, #0
00396e1c: beq      #0x396e34
00396e20: ldr      r1, [sp, #0x14]
00396e24: rsb      r1, r0, r1
00396e28: cmp      r1, #0x80
00396e2c: bhi      #0x396e6c
00396e30: bl       #0x708f00 ; 
00396e34: ldr      r4, [r4]
00396e38: cmp      r8, r4
00396e3c: bne      #0x396dd4
00396e40: ldr      r0, [sp, #4]
00396e44: mov      r1, r8
00396e48: bl       #0x3427a0 ; _ZN13ObjectManager14AddRoomObjectsEPSt4listIP10GameObjectSaIS2_EE
00396e4c: ldr      r2, [sp]
00396e50: ldr      r3, [r7, r2]
00396e54: ldr      r2, [sp, #0x44]
00396e58: ldr      r3, [r3]
00396e5c: cmp      r2, r3
00396e60: bne      #0x396e80
00396e64: add      sp, sp, #0x4c
00396e68: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00396e6c: bl       #0x310440 ; _Z10CustomFreePv
00396e70: ldr      r4, [r4]
00396e74: b        #0x396e38
00396e78: bl       #0x310440 ; _Z10CustomFreePv
00396e7c: b        #0x396da8
00396e80: bl       #0x30e310 ; 
00396e84: subseq   sp, pc, ip, asr sp
00396e88: andeq    r4, r0, ip, lsr #1
00396e8c: andeq    r0, r0, r4, lsl #17
00396e90: subseq   fp, r2, r8, lsr #24
00396e94: strdeq   r3, r4, [r0], -r4
00396e98: ldrsheq  fp, [r2], #-0xb4

# _ZN9CharacterC1EN10ObjectBase6GO_IDSE
003aa1b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8: add      ip, r0, #0x374
003aa1bc: sub      sp, sp, #0x3c
003aa1c0: mov      r4, r0
003aa1c4: str      ip, [sp, #0xc]
003aa1c8: bl       #0x38c398 ; _ZN10GameObjectC2EN10ObjectBase6GO_IDSE
003aa1cc: ldr      ip, [sp, #0xc]
003aa1d0: add      r5, r4, #0x4f0
003aa1d4: add      r5, r5, #0xc
003aa1d8: mov      r0, ip
003aa1dc: bl       #0x404db8 ; _ZN14v2ControllableC2Ev
003aa1e0: add      r0, r4, #0x3b4
003aa1e4: str      r0, [sp, #0x20]
003aa1e8: add      r0, r4, #0x37c
003aa1ec: bl       #0x3ff330 ; _ZN13ItemInventoryC2Ev
003aa1f0: add      r2, r4, #0x490
003aa1f4: add      r1, r4, #0x3c8
003aa1f8: add      r2, r2, #0xc
003aa1fc: ldr      r0, [sp, #0x20]
003aa200: str      r1, [sp, #0x1c]
003aa204: str      r2, [sp, #0x14]
003aa208: bl       #0x3dbb0c ; _ZN10CharTimersC2Ev
003aa20c: ldr      r0, [sp, #0x1c]
003aa210: bl       #0x3cebf0 ; _ZN6CharAIC2Ev
003aa214: ldr      r0, [sp, #0x14]
003aa218: bl       #0x3c8ff4 ; _ZN12CharAnimatorC2Ev
003aa21c: add      r3, r4, #0x560
003aa220: mov      r0, r5
003aa224: str      r3, [sp, #0x18]
003aa228: ldr      sb, [pc, #0x50c]
003aa22c: bl       #0x3c1b58 ; _ZN16CharStateMachineC2Ev
003aa230: ldr      r0, [sp, #0x18]
003aa234: bl       #0x3df084 ; _ZN14CharPropertiesC2Ev
003aa238: ldr      lr, [pc, #0x500]
003aa23c: add      sb, pc, sb
003aa240: mov      r8, #0
003aa244: ldr      lr, [sb, lr]
003aa248: mov      fp, #1
003aa24c: mvn      r6, #0
003aa250: add      sl, lr, #0x324
003aa254: str      sl, [sp, #0x34]
003aa258: add      sl, lr, #0x180
003aa25c: str      sl, [sp, #0x10]
003aa260: add      sl, lr, #0x1f4
003aa264: str      sl, [sp, #0x24]
003aa268: add      sl, lr, #0x220
003aa26c: str      sl, [sp, #0x28]
003aa270: add      sl, lr, #0x230
003aa274: str      sl, [sp, #0x2c]
003aa278: add      r0, lr, #8
003aa27c: add      r1, lr, #0x15c
003aa280: add      r2, lr, #0x168
003aa284: add      sl, lr, #0x304
003aa288: str      sl, [sp, #0x30]
003aa28c: stm      r4, {r0, r1}
003aa290: str      r2, [r4, #0x24]
003aa294: ldr      r0, [sp, #0x10]
003aa298: add      lr, lr, #0x314
003aa29c: add      r7, r4, #0x1380
003aa2a0: str      r0, [r4, #0x374]
003aa2a4: ldr      r1, [sp, #0x24]
003aa2a8: add      r3, r7, #0x18
003aa2ac: movw     sl, #0x13a8
003aa2b0: str      r1, [r4, #0x37c]
003aa2b4: ldr      r2, [sp, #0x28]
003aa2b8: add      r7, r7, #0x30
003aa2bc: str      r2, [r4, #0x3b4]
003aa2c0: ldr      r0, [sp, #0x2c]
003aa2c4: str      r0, [r4, #0x3c8]
003aa2c8: ldr      r1, [sp, #0x30]
003aa2cc: str      lr, [r4, #0x4fc]
003aa2d0: mov      r0, r3
003aa2d4: str      r1, [r4, #0x49c]
003aa2d8: ldr      r2, [sp, #0x34]
003aa2dc: mov      r1, #0x10
003aa2e0: str      r2, [r4, #0x560]
003aa2e4: movw     r2, #0x1394
003aa2e8: strb     r8, [r4, r2]
003aa2ec: movw     r2, #0x1395
003aa2f0: strb     r8, [r4, r2]
003aa2f4: movw     r2, #0x1396
003aa2f8: strb     fp, [r4, r2]
003aa2fc: movw     r2, #0x1397
003aa300: strb     r6, [r4, r2]
003aa304: movw     r2, #0x13ac
003aa308: str      r3, [r4, r2]
003aa30c: str      r3, [r4, sl]
003aa310: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003aa314: ldr      r3, [r4, sl]
003aa318: mov      sl, #0x13c0
003aa31c: mov      r0, r7
003aa320: strb     r8, [r3]
003aa324: movw     r3, #0x13c4
003aa328: str      r7, [r4, r3]
003aa32c: mov      r1, #0x10
003aa330: str      r7, [r4, sl]
003aa334: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003aa338: ldr      r2, [r4, sl]
003aa33c: add      r7, r4, sl
003aa340: add      r3, r7, #0xc
003aa344: strb     r8, [r2]
003aa348: movw     r2, #0x13c8
003aa34c: strh     r6, [r4, r2]
003aa350: movw     r2, #0x13ca
003aa354: strh     r6, [r4, r2]
003aa358: movw     sl, #0x13dc
003aa35c: movw     r2, #0x13e0
003aa360: str      r3, [r4, r2]
003aa364: mov      r0, r3
003aa368: str      r3, [r4, sl]
003aa36c: mov      r1, #0x10
003aa370: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003aa374: ldr      r3, [r4, sl]
003aa378: add      r7, r7, #0x28
003aa37c: movw     sl, #0x13f8
003aa380: strb     r8, [r3]
003aa384: movw     r3, #0x13e4
003aa388: strb     fp, [r4, r3]
003aa38c: movw     r3, #0x13fc
003aa390: str      r7, [r4, r3]
003aa394: mov      r0, r7
003aa398: str      r7, [r4, sl]
003aa39c: mov      r1, #0x10
003aa3a0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003aa3a4: ldr      r3, [r4, sl]
003aa3a8: add      r7, r4, #0x1400
003aa3ac: movw     sl, #0x1410
003aa3b0: strb     r8, [r3]
003aa3b4: movw     r3, #0x1414
003aa3b8: str      r7, [r4, r3]
003aa3bc: mov      r0, r7
003aa3c0: str      r7, [r4, sl]
003aa3c4: mov      r1, #0x10
003aa3c8: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003aa3cc: ldr      r3, [r4, sl]
003aa3d0: add      r7, r7, #0x18
003aa3d4: movw     sl, #0x1428
003aa3d8: strb     r8, [r3]
003aa3dc: movw     r3, #0x142c
003aa3e0: str      r7, [r4, r3]
003aa3e4: mov      r0, r7
003aa3e8: str      r7, [r4, sl]
003aa3ec: mov      r1, #0x10
003aa3f0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003aa3f4: ldr      r2, [r4, sl]
003aa3f8: mov      r3, #0
003aa3fc: mov      r1, #0xbf000000
003aa400: strb     r8, [r2]
003aa404: movw     r2, #0x14a8
003aa408: strb     r6, [r4, r2]
003aa40c: movw     r2, #0x1430
003aa410: strb     fp, [r4, r2]
003aa414: movw     r2, #0x1434
003aa418: str      r8, [r4, r2]
003aa41c: movw     r2, #0x1438
003aa420: str      r8, [r4, r2]
003aa424: movw     r2, #0x1448
003aa428: strb     fp, [r4, r2]
003aa42c: movw     r2, #0x1449
003aa430: strb     r8, [r4, r2]
003aa434: movw     r2, #0x144c
003aa438: str      r8, [r4, r2]
003aa43c: movw     r2, #0x1450
003aa440: str      r3, [r4, r2]
003aa444: movw     r2, #0x1454
003aa448: str      r3, [r4, r2]
003aa44c: movw     r2, #0x1458
003aa450: str      r3, [r4, r2]
003aa454: movw     r2, #0x145c
003aa458: str      r3, [r4, r2]
003aa45c: movw     r2, #0x1460
003aa460: str      r3, [r4, r2]
003aa464: movw     r2, #0x1464
003aa468: str      r3, [r4, r2]
003aa46c: movw     r2, #0x1468
003aa470: str      r3, [r4, r2]
003aa474: movw     r2, #0x146c
003aa478: str      r3, [r4, r2]
003aa47c: movw     r2, #0x1470
003aa480: str      r3, [r4, r2]
003aa484: movw     r2, #0x1474
003aa488: str      r3, [r4, r2]
003aa48c: movw     r2, #0x1478
003aa490: str      r3, [r4, r2]
003aa494: movw     r2, #0x147c
003aa498: str      r3, [r4, r2]
003aa49c: mov      r2, #0x1480
003aa4a0: strb     r8, [r4, r2]
003aa4a4: movw     r2, #0x1481
003aa4a8: strb     r8, [r4, r2]
003aa4ac: movw     r2, #0x1484
003aa4b0: str      r8, [r4, r2]
003aa4b4: movw     r2, #0x1488
003aa4b8: str      r8, [r4, r2]
003aa4bc: movw     r2, #0x148c
003aa4c0: str      r8, [r4, r2]
003aa4c4: movw     r2, #0x1490
003aa4c8: str      r8, [r4, r2]
003aa4cc: movw     r2, #0x1494
003aa4d0: str      r8, [r4, r2]
003aa4d4: movw     r2, #0x1498
003aa4d8: str      r6, [r4, r2]
003aa4dc: movw     r2, #0x149c
003aa4e0: str      r8, [r4, r2]
003aa4e4: movw     r2, #0x14a0
003aa4e8: str      r8, [r4, r2]
003aa4ec: movw     r2, #0x14a4
003aa4f0: str      r8, [r4, r2]
003aa4f4: movw     r2, #0x14aa
003aa4f8: strh     r8, [r4, r2]
003aa4fc: movw     r2, #0x14ac
003aa500: strb     r8, [r4, r2]
003aa504: movw     r2, #0x14d8
003aa508: str      r3, [r4, r2]
003aa50c: add      r1, r1, #0x800000
003aa510: movw     r2, #0x14fc
003aa514: str      r1, [r4, r2]
003aa518: movw     r2, #0x1504
003aa51c: str      r6, [r4, r2]
003aa520: movw     r2, #0x14ad
003aa524: strb     r8, [r4, r2]
003aa528: movw     r2, #0x14b0
003aa52c: str      r3, [r4, r2]
003aa530: movw     r2, #0x14b4
003aa534: str      r3, [r4, r2]
003aa538: movw     r2, #0x14b8
003aa53c: str      r3, [r4, r2]
003aa540: movw     r2, #0x14bc
003aa544: str      r3, [r4, r2]
003aa548: mov      r2, #0x14c0
003aa54c: str      r3, [r4, r2]
003aa550: movw     r2, #0x14c4
003aa554: str      r3, [r4, r2]
003aa558: movw     r3, #0x14c8
003aa55c: strb     r8, [r4, r3]
003aa560: movw     r3, #0x14ca
003aa564: strh     r6, [r4, r3]
003aa568: movw     r3, #0x14cc
003aa56c: str      r8, [r4, r3]
003aa570: movw     r3, #0x14d0
003aa574: strh     r8, [r4, r3]
003aa578: movw     r3, #0x14d4
003aa57c: str      r8, [r4, r3]
003aa580: movw     r3, #0x14dc
003aa584: strb     r8, [r4, r3]
003aa588: movw     r3, #0x14e4
003aa58c: strb     r8, [r4, r3]
003aa590: movw     r3, #0x14e5
003aa594: strb     r8, [r4, r3]
003aa598: movw     r3, #0x14e8
003aa59c: str      r8, [r4, r3]
003aa5a0: movw     r3, #0x14ec
003aa5a4: str      r8, [r4, r3]
003aa5a8: add      r7, r4, #0x1500
003aa5ac: movw     r3, #0x14f0
003aa5b0: add      r0, r4, #0x1a40
003aa5b4: strb     r8, [r4, r3]
003aa5b8: add      r0, r0, #8
003aa5bc: mov      r3, #0x1500
003aa5c0: add      r7, r7, #8
003aa5c4: str      r6, [r4, r3]
003aa5c8: str      r0, [sp, #0x10]
003aa5cc: mov      r0, r7
003aa5d0: bl       #0x3a6a24 ; _ZN9Character18NetStructCharacterC1Ev
003aa5d4: ldr      r0, [sp, #0x10]
003aa5d8: bl       #0x3a6a24 ; _ZN9Character18NetStructCharacterC1Ev
003aa5dc: add      r0, r4, #0x304
003aa5e0: mov      r1, r4
003aa5e4: strb     fp, [r4, #0x28]
003aa5e8: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
003aa5ec: strb     fp, [r4, #0x1c4]
003aa5f0: strb     fp, [r4, #0x85]
003aa5f4: mov      r0, #0x10
003aa5f8: mov      r1, r8
003aa5fc: bl       #0x310570 ; _Znwj15MemoryHintState
003aa600: ldr      r3, [pc, #0x13c]
003aa604: ldr      ip, [sp, #0xc]
003aa608: mov      r6, r0
003aa60c: ldr      r3, [sb, r3]
003aa610: cmp      ip, r8
003aa614: strb     r8, [r6, #0xa]
003aa618: add      r3, r3, #8
003aa61c: str      r8, [r0, #0xc]
003aa620: stm      r0, {r3, ip}
003aa624: strb     r8, [r6, #8]
003aa628: strb     r8, [r6, #9]
003aa62c: beq      #0x3aa6e0
003aa630: mov      r0, ip
003aa634: mov      r1, r6
003aa638: bl       #0x404e10 ; _ZN14v2Controllable13SetControllerEP12v2Controller
003aa63c: ldr      r3, [r4, #0x378]
003aa640: ldr      r0, [sp, #0x20]
003aa644: mov      r1, r4
003aa648: str      r4, [r3, #0xc]
003aa64c: bl       #0x3db480 ; _ZN10CharTimers12SetCharacterEP9Character
003aa650: ldr      r0, [sp, #0x1c]
003aa654: mov      r1, r4
003aa658: bl       #0x3cb7c0 ; _ZN6CharAI12SetCharacterEP9Character
003aa65c: ldr      r0, [sp, #0x14]
003aa660: mov      r1, r4
003aa664: bl       #0x3c9890 ; _ZN12CharAnimator12SetCharacterEP9Character
003aa668: mov      r0, r5
003aa66c: mov      r1, r4
003aa670: bl       #0x3c1600 ; _ZN16CharStateMachine12SetCharacterEP9Character
003aa674: ldr      r0, [sp, #0x18]
003aa678: mov      r1, r4
003aa67c: bl       #0x3dec0c ; _ZN14CharProperties12SetCharacterEP9Character
003aa680: mov      r6, #0
003aa684: str      r4, [r4, #0x380]
003aa688: mov      r1, r6
003aa68c: mov      r0, r5
003aa690: add      r6, r6, #1
003aa694: bl       #0x3c7318 ; _ZN16CharStateMachine13RegisterStateEi
003aa698: cmp      r6, #0x14
003aa69c: bne      #0x3aa688
003aa6a0: mov      r1, #0
003aa6a4: movw     r2, #0x14e0
003aa6a8: str      r1, [r4, r2]
003aa6ac: mvn      r3, #0
003aa6b0: movw     r2, #0x14f4
003aa6b4: str      r3, [r4, r2]
003aa6b8: str      r7, [r4, #0x100]
003aa6bc: ldr      sl, [sp, #0x10]
003aa6c0: movw     r2, #0x14f8
003aa6c4: mov      r0, r4
003aa6c8: str      sl, [r4, #0x104]
003aa6cc: str      r3, [r4, r2]
003aa6d0: mov      r3, #1
003aa6d4: strb     r3, [r4, #0xf8]
003aa6d8: add      sp, sp, #0x3c
003aa6dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0: ldr      r3, [pc, #0x60]
003aa6e4: ldr      r3, [sb, r3]
003aa6e8: ldr      r3, [r3]
003aa6ec: cmp      r3, #2
003aa6f0: streq    ip, [r4, #0x374]
003aa6f4: beq      #0x3aa630
003aa6f8: cmp      r3, #1
003aa6fc: bne      #0x3aa630
003aa700: ldr      r0, [pc, #0x44]
003aa704: ldr      r1, [pc, #0x44]
003aa708: ldr      r2, [pc, #0x44]
003aa70c: ldr      r0, [sb, r0]
003aa710: ldr      r3, [pc, #0x40]
003aa714: mov      lr, #0x44
003aa718: add      r1, pc, r1
003aa71c: add      r0, r0, #0xa8
003aa720: add      r2, pc, r2
003aa724: add      r3, pc, r3
003aa728: str      ip, [sp, #0xc]
003aa72c: str      lr, [sp]
003aa730: bl       #0x30e004 ; 
003aa734: ldr      ip, [sp, #0xc]
003aa738: b        #0x3aa630
003aa73c: subseq   sl, lr, r4, asr r8
003aa740: andeq    r2, r0, r8, lsl #28
003aa744: andeq    r2, r0, r4, lsr #21
003aa748: andeq    r3, r0, r0, asr #19
003aa74c: andeq    r1, r0, r0, asr #19
003aa750: subseq   r3, r1, r0, asr #25
003aa754: subseq   r8, r1, r0, lsr #27
003aa758: subseq   r8, r1, ip, lsr #27

# _ZN8RoomZone8InitPostEv
00396548: b        #0x39771c

# _ZN13ObjectManager24DeleteRandomOnlineObjectEv
0034072c: push     {r4, r5, r6, lr}
00340730: mov      r3, r0
00340734: ldr      r4, [r3, #0x100]!
00340738: mov      r5, r0
0034073c: cmp      r4, r3
00340740: mvneq    r0, #3
00340744: beq      #0x340760
00340748: mov      r0, #0
0034074c: ldr      r4, [r4]
00340750: add      r0, r0, #1
00340754: cmp      r3, r4
00340758: bne      #0x34074c
0034075c: sub      r0, r0, #4
00340760: mov      r1, #0
00340764: bl       #0x33ff90 ; _ZN6Random9GetRandomEib
00340768: ldr      r3, [r5, #0x100]
0034076c: add      r1, r0, #4
00340770: mov      r2, #0
00340774: b        #0x34078c
00340778: cmp      r2, r1
0034077c: ldr      r0, [r3, #8]
00340780: beq      #0x340798
00340784: ldr      r3, [r3]
00340788: add      r2, r2, #1
0034078c: cmp      r4, r3
00340790: bne      #0x340778
00340794: pop      {r4, r5, r6, pc}
00340798: pop      {r4, r5, r6, lr}
0034079c: b        #0x33ddb4

# _ZN13ObjectManager18DoOnlineStateFlushEv
00340250: ldr      r3, [r0, #0x100]!
00340254: mov      r1, #0
00340258: b        #0x340268
0034025c: ldr      r2, [r3, #8]
00340260: strb     r1, [r2, #0x119]
00340264: ldr      r3, [r3]
00340268: cmp      r0, r3
0034026c: bne      #0x34025c
00340270: bx       lr

# _ZThn36_N8RoomZoneD1Ev
00396820: sub      r0, r0, #0x24
00396824: b        #0x396828

# _ZN10ObjectBaseC1ENS_6GO_IDSE
0033f15c: push     {r4, r5, r6, r7, r8, lr}
0033f160: ldr      r6, [pc, #0x198]
0033f164: ldr      r2, [pc, #0x198]
0033f168: ldr      r3, [pc, #0x198]
0033f16c: add      r6, pc, r6
0033f170: ldr      r2, [r6, r2]
0033f174: ldr      r3, [r6, r3]
0033f178: mov      r4, r0
0033f17c: add      r2, r2, #8
0033f180: add      r0, r3, #8
0033f184: add      r3, r4, #8
0033f188: str      r2, [r4]
0033f18c: str      r0, [r4, #4]
0033f190: mov      r7, r1
0033f194: mov      r0, r3
0033f198: str      r3, [r4, #0x18]
0033f19c: str      r3, [r4, #0x1c]
0033f1a0: mov      r1, #0x10
0033f1a4: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f1a8: ldr      r2, [pc, #0x15c]
0033f1ac: ldr      r1, [r4, #0x18]
0033f1b0: mov      r5, #0
0033f1b4: ldr      r2, [r6, r2]
0033f1b8: strb     r5, [r1]
0033f1bc: add      r3, r4, #0x30
0033f1c0: add      r1, r2, #0x74
0033f1c4: add      r0, r2, #8
0033f1c8: add      r2, r2, #0x68
0033f1cc: stm      r4, {r0, r2}
0033f1d0: str      r1, [r4, #0x24]
0033f1d4: mov      r0, r3
0033f1d8: str      r5, [r4, #0x20]
0033f1dc: strb     r5, [r4, #0x28]
0033f1e0: strb     r5, [r4, #0x29]
0033f1e4: str      r5, [r4, #0x2c]
0033f1e8: str      r3, [r4, #0x40]
0033f1ec: str      r3, [r4, #0x44]
0033f1f0: mov      r1, #0x10
0033f1f4: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f1f8: ldr      r2, [r4, #0x40]
0033f1fc: add      r3, r4, #0x48
0033f200: mov      r0, r3
0033f204: strb     r5, [r2]
0033f208: mov      r1, #0x10
0033f20c: str      r3, [r4, #0x58]
0033f210: str      r3, [r4, #0x5c]
0033f214: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f218: ldr      r2, [r4, #0x58]
0033f21c: add      r3, r4, #0x68
0033f220: mvn      r6, #0
0033f224: strb     r5, [r2]
0033f228: mov      r1, #0x10
0033f22c: mov      r0, r3
0033f230: strb     r5, [r4, #0x60]
0033f234: str      r3, [r4, #0x78]
0033f238: str      r3, [r4, #0x7c]
0033f23c: str      r6, [r4, #0x64]
0033f240: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f244: ldr      r3, [r4, #0x78]
0033f248: add      r0, r4, #0x8c
0033f24c: strb     r5, [r3]
0033f250: mov      r3, #1
0033f254: strb     r3, [r4, #0x8a]
0033f258: strb     r5, [r4, #0x81]
0033f25c: strb     r5, [r4, #0x84]
0033f260: strb     r5, [r4, #0x85]
0033f264: strb     r5, [r4, #0x86]
0033f268: strb     r5, [r4, #0x88]
0033f26c: strb     r5, [r4, #0x89]
0033f270: bl       #0x33ed7c ; _ZN13ConditionDataC1Ev
0033f274: add      r0, r4, #0xb0
0033f278: bl       #0x33ed7c ; _ZN13ConditionDataC1Ev
0033f27c: add      r3, r4, #0xd4
0033f280: mov      r0, r3
0033f284: str      r3, [r4, #0xe4]
0033f288: str      r3, [r4, #0xe8]
0033f28c: mov      r1, #0x10
0033f290: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f294: ldr      r3, [r4, #0xe4]
0033f298: mov      r1, r5
0033f29c: mov      r0, #0xc
0033f2a0: strb     r5, [r3]
0033f2a4: mov      r3, #0
0033f2a8: str      r3, [r4, #0x114]
0033f2ac: strb     r5, [r4, #0xf0]
0033f2b0: strb     r5, [r4, #0xf1]
0033f2b4: strb     r5, [r4, #0xf8]
0033f2b8: str      r5, [r4, #0xfc]
0033f2bc: str      r5, [r4, #0x100]
0033f2c0: str      r5, [r4, #0x104]
0033f2c4: strb     r5, [r4, #0x10c]
0033f2c8: strb     r5, [r4, #0x118]
0033f2cc: strb     r5, [r4, #0x119]
0033f2d0: str      r5, [r4, #0x11c]
0033f2d4: str      r7, [r4, #0xf4]
0033f2d8: str      r6, [r4, #0x110]
0033f2dc: str      r6, [r4, #0xec]
0033f2e0: str      r6, [r4, #0x108]
0033f2e4: bl       #0x310570 ; _Znwj15MemoryHintState
0033f2e8: mov      r5, r0
0033f2ec: bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
0033f2f0: str      r5, [r4, #0x2c]
0033f2f4: mov      r0, r4
0033f2f8: str      r4, [r5, #4]
0033f2fc: pop      {r4, r5, r6, r7, r8, pc}
0033f300: rsbeq    r5, r5, r4, lsr #18
0033f304: andeq    r1, r0, ip, lsl #1
0033f308: ldrdeq   r3, r4, [r0], -ip
0033f30c: andeq    r3, r0, r4, lsl #23

# _ZN12ObjectHandleC1Ev
0033f50c: mov      r2, #0
0033f510: mvn      r1, #0
0033f514: str      r1, [r0, #8]
0033f518: str      r2, [r0, #4]
0033f51c: str      r2, [r0]
0033f520: bx       lr

# _ZN13ObjectManager22ForceEverythingVisibleEv
0034169c: push     {r4, r5, r6, lr}
003416a0: mov      r3, #0
003416a4: str      r3, [r0, #0xf8]
003416a8: mov      r5, r0
003416ac: ldr      r4, [r5, #0x24]!
003416b0: b        #0x3416c0
003416b4: ldr      r0, [r4, #8]
003416b8: bl       #0x396d24 ; _ZN8RoomZone8ActivateEv
003416bc: ldr      r4, [r4]
003416c0: cmp      r5, r4
003416c4: bne      #0x3416b4
003416c8: pop      {r4, r5, r6, pc}

# _ZN13ObjectManager8InitPostEv
0034552c: push     {r4, r5, r6, r7, lr}
00345530: ldr      r5, [pc, #0x374]
00345534: sub      sp, sp, #0x24
00345538: mov      r4, r0
0034553c: add      r5, pc, r5
00345540: ldr      r6, [r5, #0x1c]
00345544: ands     r6, r6, #1
00345548: beq      #0x345620
0034554c: ldr      r5, [pc, #0x35c]
00345550: add      r5, pc, r5
00345554: ldr      r6, [r5, #0x24]
00345558: ands     r6, r6, #1
0034555c: beq      #0x345644
00345560: add      r6, sp, #0x10
00345564: mov      r0, r6
00345568: bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
0034556c: ldr      r2, [r4, #0x7c]
00345570: cmp      r2, #0
00345574: bne      #0x34559c
00345578: ldr      r3, [pc, #0x334]
0034557c: ldr      r2, [r4, #0x68]
00345580: add      r3, pc, r3
00345584: str      r2, [r3, #0x28]
00345588: ldr      r2, [r4, #0x14]
0034558c: str      r2, [r3, #0x20]
00345590: ldr      r2, [r4, #0x7c]
00345594: add      r2, r2, #1
00345598: str      r2, [r4, #0x7c]
0034559c: ldr      r5, [pc, #0x314]
003455a0: add      r1, r4, #0x68
003455a4: add      r5, pc, r5
003455a8: ldr      r3, [r5, #0x28]
003455ac: cmp      r1, r3
003455b0: beq      #0x345744
003455b4: cmp      r2, #1
003455b8: beq      #0x345880
003455bc: ldr      r5, [pc, #0x2f8]
003455c0: add      r1, r4, #0xc
003455c4: add      r5, pc, r5
003455c8: ldr      r3, [r5, #0x20]
003455cc: cmp      r1, r3
003455d0: beq      #0x345708
003455d4: cmp      r2, #3
003455d8: beq      #0x3457a0
003455dc: cmp      r2, #4
003455e0: beq      #0x345668
003455e4: ldr      r2, [r3, #0xc]
003455e8: cmp      r2, #0
003455ec: bne      #0x3455f8
003455f0: b        #0x34576c
003455f4: mov      r2, r3
003455f8: ldr      r3, [r2, #8]
003455fc: cmp      r3, #0
00345600: bne      #0x3455f4
00345604: mov      r3, r2
00345608: ldr      r2, [pc, #0x2b0]
0034560c: mov      r0, #0
00345610: add      r2, pc, r2
00345614: str      r3, [r2, #0x20]
00345618: add      sp, sp, #0x24
0034561c: pop      {r4, r5, r6, r7, pc}
00345620: add      r7, r5, #0x1c
00345624: mov      r0, r7
00345628: bl       #0x30e76c ; 
0034562c: cmp      r0, #0
00345630: beq      #0x34554c
00345634: str      r6, [r5, #0x20]
00345638: mov      r0, r7
0034563c: bl       #0x30ea3c ; 
00345640: b        #0x34554c
00345644: add      r7, r5, #0x24
00345648: mov      r0, r7
0034564c: bl       #0x30e76c ; 
00345650: cmp      r0, #0
00345654: beq      #0x345560
00345658: str      r6, [r5, #0x28]
0034565c: mov      r0, r7
00345660: bl       #0x30ea3c ; 
00345664: b        #0x345560
00345668: ldr      r5, [r3, #0x2c]
0034566c: cmp      r5, #0
00345670: beq      #0x3455e4
00345674: ldr      r0, [pc, #0x248]
00345678: ldr      r1, [r5, #0x5c]
0034567c: add      r0, pc, r0
00345680: bl       #0x30e6e8 ; 
00345684: cmp      r0, #0
00345688: bne      #0x345840
0034568c: mov      r3, #0xc
00345690: add      r0, sp, #0x20
00345694: str      r3, [r0, #-4]!
00345698: bl       #0x708ec0 ; 
0034569c: str      r5, [r0, #8]
003456a0: ldr      r3, [r4, #0x28]
003456a4: add      r2, r4, #0x24
003456a8: stm      r0, {r2, r3}
003456ac: str      r0, [r3]
003456b0: str      r0, [r4, #0x28]
003456b4: mov      r0, r5
003456b8: bl       #0x396c44 ; _ZN8RoomZone14InitObjectListEv
003456bc: ldrb     r3, [r5, #0xac]
003456c0: cmp      r3, #0
003456c4: bne      #0x345818
003456c8: ldr      r3, [r5, #0xa8]
003456cc: cmp      r3, #0
003456d0: beq      #0x345818
003456d4: add      r6, r4, #0x44
003456d8: mov      r0, r6
003456dc: bl       #0x343168 ; 
003456e0: str      r5, [r0, #8]
003456e4: ldr      r2, [r4, #0x48]
003456e8: ldr      r3, [pc, #0x1d8]
003456ec: str      r6, [r0]
003456f0: str      r2, [r0, #4]
003456f4: add      r3, pc, r3
003456f8: str      r0, [r2]
003456fc: str      r0, [r4, #0x48]
00345700: ldr      r3, [r3, #0x20]
00345704: b        #0x3455e4
00345708: add      r2, r2, #1
0034570c: cmp      r2, #4
00345710: str      r2, [r4, #0x7c]
00345714: movne    r0, #1
00345718: bne      #0x345618
0034571c: ldr      r3, [r4, #0x14]
00345720: add      r0, r4, #0x2c
00345724: str      r3, [r5, #0x20]
00345728: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
0034572c: add      r0, r4, #0x44
00345730: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
00345734: add      r0, r4, #0x34
00345738: bl       #0x34526c ; _ZNSt4priv10_List_baseIP10ObjectBaseSaIS2_EE5clearEv
0034573c: mov      r0, #0
00345740: b        #0x345618
00345744: cmp      r2, #1
00345748: bne      #0x3455bc
0034574c: ldr      r3, [r4, #0x14]
00345750: mov      r2, #2
00345754: str      r2, [r4, #0x7c]
00345758: str      r3, [r5, #0x20]
0034575c: ldr      r2, [r4, #0x7c]
00345760: add      r2, r2, #1
00345764: str      r2, [r4, #0x7c]
00345768: b        #0x3455bc
0034576c: ldr      r1, [r3, #4]
00345770: ldr      r0, [r1, #0xc]
00345774: cmp      r0, r3
00345778: bne      #0x345794
0034577c: mov      r3, r1
00345780: ldr      r1, [r1, #4]
00345784: ldr      r2, [r1, #0xc]
00345788: cmp      r3, r2
0034578c: beq      #0x34577c
00345790: ldr      r2, [r3, #0xc]
00345794: cmp      r1, r2
00345798: movne    r3, r1
0034579c: b        #0x345608
003457a0: add      r4, sp, #4
003457a4: ldr      r1, [r3, #0x2c]
003457a8: mov      r0, r4
003457ac: bl       #0x33f524 ; _ZN12ObjectHandleC1EP10ObjectBase
003457b0: ldr      r2, [sp, #8]
003457b4: add      r6, r6, #4
003457b8: ldr      r3, [sp, #4]
003457bc: str      r2, [r6], #4
003457c0: ldr      r2, [r4, #8]
003457c4: add      r4, sp, #0x10
003457c8: mov      r0, r4
003457cc: mov      r1, #0
003457d0: str      r2, [r6]
003457d4: str      r3, [sp, #0x10]
003457d8: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
003457dc: cmp      r0, #0
003457e0: beq      #0x345810
003457e4: mov      r1, #1
003457e8: mov      r0, r4
003457ec: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
003457f0: ldr      r3, [r0]
003457f4: mov      lr, pc
003457f8: ldr      pc, [r3, #0x1c]
003457fc: mov      r1, #1
00345800: mov      r0, r4
00345804: bl       #0x33fdc0 ; _ZN12ObjectHandle9GetObjectEb
00345808: mov      r1, #0
0034580c: bl       #0x33e6d4 ; _ZN10ObjectBase19TestEnableConditionEb
00345810: ldr      r3, [r5, #0x20]
00345814: b        #0x3455e4
00345818: ldrb     r3, [r5, #0xd0]
0034581c: cmp      r3, #0
00345820: bne      #0x34589c
00345824: ldr      r3, [r5, #0xcc]
00345828: cmp      r3, #0
0034582c: bne      #0x3456d4
00345830: ldr      r3, [pc, #0x94]
00345834: add      r3, pc, r3
00345838: ldr      r3, [r3, #0x20]
0034583c: b        #0x3455e4
00345840: ldr      r3, [r5]
00345844: mov      r0, r5
00345848: mov      lr, pc
0034584c: ldr      pc, [r3, #0x38]
00345850: cmp      r0, #0
00345854: beq      #0x3456bc
00345858: add      r6, r4, #0x2c
0034585c: mov      r0, r6
00345860: bl       #0x343168 ; 
00345864: str      r5, [r0, #8]
00345868: ldr      r3, [r4, #0x30]
0034586c: str      r6, [r0]
00345870: str      r3, [r0, #4]
00345874: str      r0, [r3]
00345878: str      r0, [r4, #0x30]
0034587c: b        #0x3456bc
00345880: ldr      r0, [r3, #8]
00345884: bl       #0x38a88c ; _ZNK6Module10LoadModuleEv
00345888: ldr      r3, [r5, #0x28]
0034588c: ldr      r3, [r3]
00345890: str      r3, [r5, #0x28]
00345894: ldr      r2, [r4, #0x7c]
00345898: b        #0x3455bc
0034589c: ldr      r3, [pc, #0x2c]
003458a0: add      r3, pc, r3
003458a4: ldr      r3, [r3, #0x20]
003458a8: b        #0x3455e4
003458ac: rsbeq    ip, r5, r0, lsl #18
003458b0: rsbeq    ip, r5, ip, ror #17
003458b4: strhteq  ip, [r5], #-0x8c
003458b8: mlseq    r5, r8, r8, ip
003458bc: rsbeq    ip, r5, r8, ror r8
003458c0: rsbeq    ip, r5, ip, lsr #16
003458c4: ldrsheq  sl, [r7], #-0xc4
003458c8: rsbeq    ip, r5, r8, asr #14
003458cc: rsbeq    ip, r5, r8, lsl #12
003458d0: mlseq    r5, ip, r5, ip

# _ZN13ObjectManager11LoadFromXMLEP12TiXmlElementPKc
0034bbe4: str      lr, [sp, #-4]!
0034bbe8: sub      sp, sp, #0x1c
0034bbec: mov      ip, #0
0034bbf0: mvn      lr, #0
0034bbf4: add      r3, sp, #0xc
0034bbf8: str      ip, [sp, #0x14]
0034bbfc: str      lr, [sp]
0034bc00: str      ip, [sp, #0xc]
0034bc04: str      ip, [sp, #0x10]
0034bc08: bl       #0x34b868 ; _ZN13ObjectManager11LoadFromXMLEP12TiXmlElementPKcRK7Point3DIfEi
0034bc0c: add      sp, sp, #0x1c
0034bc10: ldm      sp!, {pc}

# _ZN13ObjectManager22GetHighestThreatPlayerEPKcib
0034b064: push     {r4, r5, r6, r7, r8, lr}
0034b068: ldr      r4, [pc, #0xf0]
0034b06c: ldr      r5, [pc, #0xf0]
0034b070: sub      sp, sp, #0x38
0034b074: add      r4, pc, r4
0034b078: ldr      lr, [r4, r5]
0034b07c: add      r6, sp, #0xc
0034b080: mov      ip, #0
0034b084: ldr      lr, [lr]
0034b088: mov      r7, r0
0034b08c: mov      r0, r6
0034b090: str      ip, [sp, #4]
0034b094: str      lr, [sp, #0x34]
0034b098: str      ip, [sp]
0034b09c: mov      r8, r1
0034b0a0: bl       #0x34aca0 ; _ZN13ObjectManager15GetObjectByNameEPKcibS1_
0034b0a4: mov      r0, r6
0034b0a8: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
0034b0ac: add      r0, r0, #0x3c8
0034b0b0: bl       #0x3d4a18 ; _ZNK6CharAI18AI_GetHighestAggroEv
0034b0b4: subs     r2, r0, #0
0034b0b8: beq      #0x34b0e8
0034b0bc: mov      r1, r8
0034b0c0: mov      r0, r7
0034b0c4: bl       #0x340c54 ; _ZN13ObjectManager14GetObjectByPtrEP10ObjectBase
0034b0c8: ldr      r3, [r4, r5]
0034b0cc: ldr      r2, [sp, #0x34]
0034b0d0: mov      r0, r7
0034b0d4: ldr      r3, [r3]
0034b0d8: cmp      r2, r3
0034b0dc: bne      #0x34b15c
0034b0e0: add      sp, sp, #0x38
0034b0e4: pop      {r4, r5, r6, r7, r8, pc}
0034b0e8: ldr      r3, [pc, #0x78]
0034b0ec: add      r6, sp, #0x1c
0034b0f0: ldr      r8, [r4, r3]
0034b0f4: mov      r0, r8
0034b0f8: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
0034b0fc: ldr      r1, [pc, #0x68]
0034b100: add      r2, sp, #0x18
0034b104: mov      r0, r6
0034b108: add      r1, pc, r1
0034b10c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0034b110: mov      r0, r8
0034b114: mov      r1, r6
0034b118: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
0034b11c: ldr      r0, [sp, #0x30]
0034b120: cmp      r0, r6
0034b124: beq      #0x34b144
0034b128: cmp      r0, #0
0034b12c: beq      #0x34b144
0034b130: ldr      r1, [sp, #0x1c]
0034b134: rsb      r1, r0, r1
0034b138: cmp      r1, #0x80
0034b13c: bhi      #0x34b154
0034b140: bl       #0x708f00 ; 
0034b144: mov      r0, r7
0034b148: mov      r1, #0
0034b14c: bl       #0x33f524 ; _ZN12ObjectHandleC1EP10ObjectBase
0034b150: b        #0x34b0c8
0034b154: bl       #0x310440 ; _Z10CustomFreePv
0034b158: b        #0x34b144
0034b15c: bl       #0x30e310 ; 
0034b160: rsbeq    sb, r4, ip, lsl sl
0034b164: andeq    r4, r0, ip, lsr #1
0034b168: andeq    r0, r0, r4, lsl #17
0034b16c: subseq   r5, r7, r8, lsl #6

# _ZN13ObjectManagerC2Ev
0034a404: ldr      r2, [pc, #0x20c]
0034a408: ldr      ip, [pc, #0x20c]
0034a40c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034a410: add      r2, pc, r2
0034a414: ldr      ip, [r2, ip]
0034a418: mov      r1, r0
0034a41c: mov      r3, #0
0034a420: add      ip, ip, #8
0034a424: str      ip, [r1], #4
0034a428: add      r5, r0, #0x60
0034a42c: mov      ip, r0
0034a430: str      r1, [r0, #8]
0034a434: str      r1, [r0, #4]
0034a438: str      r3, [r0, #0x10]
0034a43c: strb     r3, [ip, #0xc]!
0034a440: str      r5, [r0, #0x60]
0034a444: add      r5, r0, #0x80
0034a448: str      r5, [r0, #0x84]
0034a44c: ldr      r5, [r0, #0x60]
0034a450: add      lr, r0, #0x70
0034a454: str      lr, [r0, #0x70]
0034a458: str      r5, [r0, #0x64]
0034a45c: ldr      r5, [r0, #0x70]
0034a460: add      sb, r0, #0x24
0034a464: add      sl, r0, #0x2c
0034a468: add      r8, r0, #0x34
0034a46c: add      r7, r0, #0x3c
0034a470: add      r6, r0, #0x44
0034a474: add      fp, r0, #0x68
0034a478: add      lr, r0, #0x88
0034a47c: str      lr, [r0, #0x88]
0034a480: str      ip, [r0, #0x18]
0034a484: str      sb, [r0, #0x28]
0034a488: str      sl, [r0, #0x30]
0034a48c: str      r8, [r0, #0x38]
0034a490: str      r7, [r0, #0x40]
0034a494: str      r6, [r0, #0x48]
0034a498: str      fp, [r0, #0x6c]
0034a49c: str      r5, [r0, #0x74]
0034a4a0: str      ip, [r0, #0x14]
0034a4a4: str      sb, [r0, #0x24]
0034a4a8: str      sl, [r0, #0x2c]
0034a4ac: str      r8, [r0, #0x34]
0034a4b0: str      r7, [r0, #0x3c]
0034a4b4: str      r6, [r0, #0x44]
0034a4b8: str      r3, [r0, #0x1c]
0034a4bc: str      r3, [r0, #0x4c]
0034a4c0: str      r3, [r0, #0x50]
0034a4c4: str      r3, [r0, #0x54]
0034a4c8: str      r3, [r0, #0x58]
0034a4cc: str      r3, [r0, #0x5c]
0034a4d0: ldr      ip, [r0, #0x84]
0034a4d4: ldr      r5, [r0, #0x88]
0034a4d8: mov      r1, r0
0034a4dc: add      lr, r0, #0x90
0034a4e0: str      fp, [r0, #0x68]
0034a4e4: str      ip, [r0, #0x80]
0034a4e8: str      r5, [r0, #0x8c]
0034a4ec: str      lr, [r0, #0x94]
0034a4f0: str      lr, [r0, #0x90]
0034a4f4: str      r3, [r0, #0x78]
0034a4f8: str      r3, [r0, #0x7c]
0034a4fc: str      r3, [r0, #0x9c]
0034a500: mov      ip, r0
0034a504: strb     r3, [r1, #0x98]!
0034a508: str      r1, [r0, #0xa4]
0034a50c: str      r1, [r0, #0xa0]
0034a510: str      r3, [r0, #0xa8]
0034a514: str      r3, [r0, #0xb4]
0034a518: mov      r1, r0
0034a51c: strb     r3, [ip, #0xb0]!
0034a520: str      ip, [r0, #0xbc]
0034a524: str      ip, [r0, #0xb8]
0034a528: str      r3, [r0, #0xc0]
0034a52c: mov      ip, r0
0034a530: str      r3, [r0, #0xcc]
0034a534: strb     r3, [r1, #0xc8]!
0034a538: str      r1, [r0, #0xd4]
0034a53c: str      r1, [r0, #0xd0]
0034a540: add      lr, r0, #0x100
0034a544: mov      r1, r0
0034a548: str      r3, [r0, #0xd8]
0034a54c: str      r3, [r0, #0xe4]
0034a550: strb     r3, [ip, #0xe0]!
0034a554: add      r6, r0, #0x120
0034a558: add      r5, r0, #0x128
0034a55c: str      ip, [r0, #0xec]
0034a560: str      lr, [r0, #0x104]
0034a564: str      ip, [r0, #0xe8]
0034a568: str      lr, [r0, #0x100]
0034a56c: mov      ip, r0
0034a570: add      lr, r0, #0x130
0034a574: str      r3, [r0, #0xf0]
0034a578: str      r3, [r0, #0xf8]
0034a57c: str      r3, [r0, #0x10c]
0034a580: strb     r3, [r1, #0x108]!
0034a584: str      r1, [r0, #0x114]
0034a588: str      r1, [r0, #0x110]
0034a58c: str      r6, [r0, #0x120]
0034a590: str      r6, [r0, #0x124]
0034a594: str      r5, [r0, #0x12c]
0034a598: str      lr, [r0, #0x134]
0034a59c: str      r5, [r0, #0x128]
0034a5a0: str      lr, [r0, #0x130]
0034a5a4: str      r3, [r0, #0x118]
0034a5a8: str      r3, [r0, #0x14c]
0034a5ac: mov      r1, r0
0034a5b0: strb     r3, [ip, #0x148]!
0034a5b4: str      ip, [r0, #0x154]
0034a5b8: str      ip, [r0, #0x150]
0034a5bc: str      r3, [r0, #0x158]
0034a5c0: mov      ip, r0
0034a5c4: str      r3, [r0, #0x168]
0034a5c8: strb     r3, [r1, #0x164]!
0034a5cc: str      r1, [r0, #0x170]
0034a5d0: str      r1, [r0, #0x16c]
0034a5d4: str      r3, [r0, #0x174]
0034a5d8: mov      r1, r0
0034a5dc: str      r3, [r0, #0x180]
0034a5e0: strb     r3, [ip, #0x17c]!
0034a5e4: str      ip, [r0, #0x188]
0034a5e8: str      ip, [r0, #0x184]
0034a5ec: str      r3, [r0, #0x18c]
0034a5f0: str      r3, [r0, #0x198]
0034a5f4: strb     r3, [r1, #0x194]!
0034a5f8: mov      r4, r0
0034a5fc: str      r1, [r0, #0x1a0]
0034a600: str      r1, [r0, #0x19c]
0034a604: strb     r3, [r0, #0x1ac]
0034a608: str      r3, [r0, #0x1a4]
0034a60c: bl       #0x3496b8 ; _ZN13ObjectManager5FlushEv
0034a610: mov      r0, r4
0034a614: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034a618: rsbeq    sl, r4, r0, lsl #13
0034a61c: andeq    r1, r0, ip, asr r1

# _ZN10ObjectBaseC2ENS_6GO_IDSE
0033f310: push     {r4, r5, r6, r7, r8, lr}
0033f314: ldr      r6, [pc, #0x198]
0033f318: ldr      r2, [pc, #0x198]
0033f31c: ldr      r3, [pc, #0x198]
0033f320: add      r6, pc, r6
0033f324: ldr      r2, [r6, r2]
0033f328: ldr      r3, [r6, r3]
0033f32c: mov      r4, r0
0033f330: add      r2, r2, #8
0033f334: add      r0, r3, #8
0033f338: add      r3, r4, #8
0033f33c: str      r2, [r4]
0033f340: str      r0, [r4, #4]
0033f344: mov      r7, r1
0033f348: mov      r0, r3
0033f34c: str      r3, [r4, #0x18]
0033f350: str      r3, [r4, #0x1c]
0033f354: mov      r1, #0x10
0033f358: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f35c: ldr      r2, [pc, #0x15c]
0033f360: ldr      r1, [r4, #0x18]
0033f364: mov      r5, #0
0033f368: ldr      r2, [r6, r2]
0033f36c: strb     r5, [r1]
0033f370: add      r3, r4, #0x30
0033f374: add      r1, r2, #0x74
0033f378: add      r0, r2, #8
0033f37c: add      r2, r2, #0x68
0033f380: stm      r4, {r0, r2}
0033f384: str      r1, [r4, #0x24]
0033f388: mov      r0, r3
0033f38c: str      r5, [r4, #0x20]
0033f390: strb     r5, [r4, #0x28]
0033f394: strb     r5, [r4, #0x29]
0033f398: str      r5, [r4, #0x2c]
0033f39c: str      r3, [r4, #0x40]
0033f3a0: str      r3, [r4, #0x44]
0033f3a4: mov      r1, #0x10
0033f3a8: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f3ac: ldr      r2, [r4, #0x40]
0033f3b0: add      r3, r4, #0x48
0033f3b4: mov      r0, r3
0033f3b8: strb     r5, [r2]
0033f3bc: mov      r1, #0x10
0033f3c0: str      r3, [r4, #0x58]
0033f3c4: str      r3, [r4, #0x5c]
0033f3c8: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f3cc: ldr      r2, [r4, #0x58]
0033f3d0: add      r3, r4, #0x68
0033f3d4: mvn      r6, #0
0033f3d8: strb     r5, [r2]
0033f3dc: mov      r1, #0x10
0033f3e0: mov      r0, r3
0033f3e4: strb     r5, [r4, #0x60]
0033f3e8: str      r3, [r4, #0x78]
0033f3ec: str      r3, [r4, #0x7c]
0033f3f0: str      r6, [r4, #0x64]
0033f3f4: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f3f8: ldr      r3, [r4, #0x78]
0033f3fc: add      r0, r4, #0x8c
0033f400: strb     r5, [r3]
0033f404: mov      r3, #1
0033f408: strb     r3, [r4, #0x8a]
0033f40c: strb     r5, [r4, #0x81]
0033f410: strb     r5, [r4, #0x84]
0033f414: strb     r5, [r4, #0x85]
0033f418: strb     r5, [r4, #0x86]
0033f41c: strb     r5, [r4, #0x88]
0033f420: strb     r5, [r4, #0x89]
0033f424: bl       #0x33ed7c ; _ZN13ConditionDataC1Ev
0033f428: add      r0, r4, #0xb0
0033f42c: bl       #0x33ed7c ; _ZN13ConditionDataC1Ev
0033f430: add      r3, r4, #0xd4
0033f434: mov      r0, r3
0033f438: str      r3, [r4, #0xe4]
0033f43c: str      r3, [r4, #0xe8]
0033f440: mov      r1, #0x10
0033f444: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0033f448: ldr      r3, [r4, #0xe4]
0033f44c: mov      r1, r5
0033f450: mov      r0, #0xc
0033f454: strb     r5, [r3]
0033f458: mov      r3, #0
0033f45c: str      r3, [r4, #0x114]
0033f460: strb     r5, [r4, #0xf0]
0033f464: strb     r5, [r4, #0xf1]
0033f468: strb     r5, [r4, #0xf8]
0033f46c: str      r5, [r4, #0xfc]
0033f470: str      r5, [r4, #0x100]
0033f474: str      r5, [r4, #0x104]
0033f478: strb     r5, [r4, #0x10c]
0033f47c: strb     r5, [r4, #0x118]
0033f480: strb     r5, [r4, #0x119]
0033f484: str      r5, [r4, #0x11c]
0033f488: str      r7, [r4, #0xf4]
0033f48c: str      r6, [r4, #0x110]
0033f490: str      r6, [r4, #0xec]
0033f494: str      r6, [r4, #0x108]
0033f498: bl       #0x310570 ; _Znwj15MemoryHintState
0033f49c: mov      r5, r0
0033f4a0: bl       #0x33f50c ; _ZN12ObjectHandleC1Ev
0033f4a4: str      r5, [r4, #0x2c]
0033f4a8: mov      r0, r4
0033f4ac: str      r4, [r5, #4]
0033f4b0: pop      {r4, r5, r6, r7, r8, pc}
0033f4b4: rsbeq    r5, r5, r0, ror r7
0033f4b8: andeq    r1, r0, ip, lsl #1
0033f4bc: ldrdeq   r3, r4, [r0], -ip
0033f4c0: andeq    r3, r0, r4, lsl #23

# _ZN9CharacterC2EN10ObjectBase6GO_IDSE
003a9340: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a9344: add      ip, r0, #0x374
003a9348: sub      sp, sp, #0x3c
003a934c: mov      r4, r0
003a9350: str      ip, [sp, #0xc]
003a9354: bl       #0x38c398 ; _ZN10GameObjectC2EN10ObjectBase6GO_IDSE
003a9358: ldr      ip, [sp, #0xc]
003a935c: add      r5, r4, #0x4f0
003a9360: add      r5, r5, #0xc
003a9364: mov      r0, ip
003a9368: bl       #0x404db8 ; _ZN14v2ControllableC2Ev
003a936c: add      r0, r4, #0x3b4
003a9370: str      r0, [sp, #0x20]
003a9374: add      r0, r4, #0x37c
003a9378: bl       #0x3ff330 ; _ZN13ItemInventoryC2Ev
003a937c: add      r2, r4, #0x490
003a9380: add      r1, r4, #0x3c8
003a9384: add      r2, r2, #0xc
003a9388: ldr      r0, [sp, #0x20]
003a938c: str      r1, [sp, #0x1c]
003a9390: str      r2, [sp, #0x14]
003a9394: bl       #0x3dbb0c ; _ZN10CharTimersC2Ev
003a9398: ldr      r0, [sp, #0x1c]
003a939c: bl       #0x3cebf0 ; _ZN6CharAIC2Ev
003a93a0: ldr      r0, [sp, #0x14]
003a93a4: bl       #0x3c8ff4 ; _ZN12CharAnimatorC2Ev
003a93a8: add      r3, r4, #0x560
003a93ac: mov      r0, r5
003a93b0: str      r3, [sp, #0x18]
003a93b4: ldr      sb, [pc, #0x50c]
003a93b8: bl       #0x3c1b58 ; _ZN16CharStateMachineC2Ev
003a93bc: ldr      r0, [sp, #0x18]
003a93c0: bl       #0x3df084 ; _ZN14CharPropertiesC2Ev
003a93c4: ldr      lr, [pc, #0x500]
003a93c8: add      sb, pc, sb
003a93cc: mov      r8, #0
003a93d0: ldr      lr, [sb, lr]
003a93d4: mov      fp, #1
003a93d8: mvn      r6, #0
003a93dc: add      sl, lr, #0x324
003a93e0: str      sl, [sp, #0x34]
003a93e4: add      sl, lr, #0x180
003a93e8: str      sl, [sp, #0x10]
003a93ec: add      sl, lr, #0x1f4
003a93f0: str      sl, [sp, #0x24]
003a93f4: add      sl, lr, #0x220
003a93f8: str      sl, [sp, #0x28]
003a93fc: add      sl, lr, #0x230
003a9400: str      sl, [sp, #0x2c]
003a9404: add      r0, lr, #8
003a9408: add      r1, lr, #0x15c
003a940c: add      r2, lr, #0x168
003a9410: add      sl, lr, #0x304
003a9414: str      sl, [sp, #0x30]
003a9418: stm      r4, {r0, r1}
003a941c: str      r2, [r4, #0x24]
003a9420: ldr      r0, [sp, #0x10]
003a9424: add      lr, lr, #0x314
003a9428: add      r7, r4, #0x1380
003a942c: str      r0, [r4, #0x374]
003a9430: ldr      r1, [sp, #0x24]
003a9434: add      r3, r7, #0x18
003a9438: movw     sl, #0x13a8
003a943c: str      r1, [r4, #0x37c]
003a9440: ldr      r2, [sp, #0x28]
003a9444: add      r7, r7, #0x30
003a9448: str      r2, [r4, #0x3b4]
003a944c: ldr      r0, [sp, #0x2c]
003a9450: str      r0, [r4, #0x3c8]
003a9454: ldr      r1, [sp, #0x30]
003a9458: str      lr, [r4, #0x4fc]
003a945c: mov      r0, r3
003a9460: str      r1, [r4, #0x49c]
003a9464: ldr      r2, [sp, #0x34]
003a9468: mov      r1, #0x10
003a946c: str      r2, [r4, #0x560]
003a9470: movw     r2, #0x1394
003a9474: strb     r8, [r4, r2]
003a9478: movw     r2, #0x1395
003a947c: strb     r8, [r4, r2]
003a9480: movw     r2, #0x1396
003a9484: strb     fp, [r4, r2]
003a9488: movw     r2, #0x1397
003a948c: strb     r6, [r4, r2]
003a9490: movw     r2, #0x13ac
003a9494: str      r3, [r4, r2]
003a9498: str      r3, [r4, sl]
003a949c: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a94a0: ldr      r3, [r4, sl]
003a94a4: mov      sl, #0x13c0
003a94a8: mov      r0, r7
003a94ac: strb     r8, [r3]
003a94b0: movw     r3, #0x13c4
003a94b4: str      r7, [r4, r3]
003a94b8: mov      r1, #0x10
003a94bc: str      r7, [r4, sl]
003a94c0: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a94c4: ldr      r2, [r4, sl]
003a94c8: add      r7, r4, sl
003a94cc: add      r3, r7, #0xc
003a94d0: strb     r8, [r2]
003a94d4: movw     r2, #0x13c8
003a94d8: strh     r6, [r4, r2]
003a94dc: movw     r2, #0x13ca
003a94e0: strh     r6, [r4, r2]
003a94e4: movw     sl, #0x13dc
003a94e8: movw     r2, #0x13e0
003a94ec: str      r3, [r4, r2]
003a94f0: mov      r0, r3
003a94f4: str      r3, [r4, sl]
003a94f8: mov      r1, #0x10
003a94fc: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a9500: ldr      r3, [r4, sl]
003a9504: add      r7, r7, #0x28
003a9508: movw     sl, #0x13f8
003a950c: strb     r8, [r3]
003a9510: movw     r3, #0x13e4
003a9514: strb     fp, [r4, r3]
003a9518: movw     r3, #0x13fc
003a951c: str      r7, [r4, r3]
003a9520: mov      r0, r7
003a9524: str      r7, [r4, sl]
003a9528: mov      r1, #0x10
003a952c: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a9530: ldr      r3, [r4, sl]
003a9534: add      r7, r4, #0x1400
003a9538: movw     sl, #0x1410
003a953c: strb     r8, [r3]
003a9540: movw     r3, #0x1414
003a9544: str      r7, [r4, r3]
003a9548: mov      r0, r7
003a954c: str      r7, [r4, sl]
003a9550: mov      r1, #0x10
003a9554: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a9558: ldr      r3, [r4, sl]
003a955c: add      r7, r7, #0x18
003a9560: movw     sl, #0x1428
003a9564: strb     r8, [r3]
003a9568: movw     r3, #0x142c
003a956c: str      r7, [r4, r3]
003a9570: mov      r0, r7
003a9574: str      r7, [r4, sl]
003a9578: mov      r1, #0x10
003a957c: bl       #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
003a9580: ldr      r2, [r4, sl]
003a9584: mov      r3, #0
003a9588: mov      r1, #0xbf000000
003a958c: strb     r8, [r2]
003a9590: movw     r2, #0x14a8
003a9594: strb     r6, [r4, r2]
003a9598: movw     r2, #0x1430
003a959c: strb     fp, [r4, r2]
003a95a0: movw     r2, #0x1434
003a95a4: str      r8, [r4, r2]
003a95a8: movw     r2, #0x1438
003a95ac: str      r8, [r4, r2]
003a95b0: movw     r2, #0x1448
003a95b4: strb     fp, [r4, r2]
003a95b8: movw     r2, #0x1449
003a95bc: strb     r8, [r4, r2]
003a95c0: movw     r2, #0x144c
003a95c4: str      r8, [r4, r2]
003a95c8: movw     r2, #0x1450
003a95cc: str      r3, [r4, r2]
003a95d0: movw     r2, #0x1454
003a95d4: str      r3, [r4, r2]
003a95d8: movw     r2, #0x1458
003a95dc: str      r3, [r4, r2]
003a95e0: movw     r2, #0x145c
003a95e4: str      r3, [r4, r2]
003a95e8: movw     r2, #0x1460
003a95ec: str      r3, [r4, r2]
003a95f0: movw     r2, #0x1464
003a95f4: str      r3, [r4, r2]
003a95f8: movw     r2, #0x1468
003a95fc: str      r3, [r4, r2]
003a9600: movw     r2, #0x146c
003a9604: str      r3, [r4, r2]
003a9608: movw     r2, #0x1470
003a960c: str      r3, [r4, r2]
003a9610: movw     r2, #0x1474
003a9614: str      r3, [r4, r2]
003a9618: movw     r2, #0x1478
003a961c: str      r3, [r4, r2]
003a9620: movw     r2, #0x147c
003a9624: str      r3, [r4, r2]
003a9628: mov      r2, #0x1480
003a962c: strb     r8, [r4, r2]
003a9630: movw     r2, #0x1481
003a9634: strb     r8, [r4, r2]
003a9638: movw     r2, #0x1484
003a963c: str      r8, [r4, r2]
003a9640: movw     r2, #0x1488
003a9644: str      r8, [r4, r2]
003a9648: movw     r2, #0x148c
003a964c: str      r8, [r4, r2]
003a9650: movw     r2, #0x1490
003a9654: str      r8, [r4, r2]
003a9658: movw     r2, #0x1494
003a965c: str      r8, [r4, r2]
003a9660: movw     r2, #0x1498
003a9664: str      r6, [r4, r2]
003a9668: movw     r2, #0x149c
003a966c: str      r8, [r4, r2]
003a9670: movw     r2, #0x14a0
003a9674: str      r8, [r4, r2]
003a9678: movw     r2, #0x14a4
003a967c: str      r8, [r4, r2]
003a9680: movw     r2, #0x14aa
003a9684: strh     r8, [r4, r2]
003a9688: movw     r2, #0x14ac
003a968c: strb     r8, [r4, r2]
003a9690: movw     r2, #0x14d8
003a9694: str      r3, [r4, r2]
003a9698: add      r1, r1, #0x800000
003a969c: movw     r2, #0x14fc
003a96a0: str      r1, [r4, r2]
003a96a4: movw     r2, #0x1504
003a96a8: str      r6, [r4, r2]
003a96ac: movw     r2, #0x14ad
003a96b0: strb     r8, [r4, r2]
003a96b4: movw     r2, #0x14b0
003a96b8: str      r3, [r4, r2]
003a96bc: movw     r2, #0x14b4
003a96c0: str      r3, [r4, r2]
003a96c4: movw     r2, #0x14b8
003a96c8: str      r3, [r4, r2]
003a96cc: movw     r2, #0x14bc
003a96d0: str      r3, [r4, r2]
003a96d4: mov      r2, #0x14c0
003a96d8: str      r3, [r4, r2]
003a96dc: movw     r2, #0x14c4
003a96e0: str      r3, [r4, r2]
003a96e4: movw     r3, #0x14c8
003a96e8: strb     r8, [r4, r3]
003a96ec: movw     r3, #0x14ca
003a96f0: strh     r6, [r4, r3]
003a96f4: movw     r3, #0x14cc
003a96f8: str      r8, [r4, r3]
003a96fc: movw     r3, #0x14d0
003a9700: strh     r8, [r4, r3]
003a9704: movw     r3, #0x14d4
003a9708: str      r8, [r4, r3]
003a970c: movw     r3, #0x14dc
003a9710: strb     r8, [r4, r3]
003a9714: movw     r3, #0x14e4
003a9718: strb     r8, [r4, r3]
003a971c: movw     r3, #0x14e5
003a9720: strb     r8, [r4, r3]
003a9724: movw     r3, #0x14e8
003a9728: str      r8, [r4, r3]
003a972c: movw     r3, #0x14ec
003a9730: str      r8, [r4, r3]
003a9734: add      r7, r4, #0x1500
003a9738: movw     r3, #0x14f0
003a973c: add      r0, r4, #0x1a40
003a9740: strb     r8, [r4, r3]
003a9744: add      r0, r0, #8
003a9748: mov      r3, #0x1500
003a974c: add      r7, r7, #8
003a9750: str      r6, [r4, r3]
003a9754: str      r0, [sp, #0x10]
003a9758: mov      r0, r7
003a975c: bl       #0x3a6a24 ; _ZN9Character18NetStructCharacterC1Ev
003a9760: ldr      r0, [sp, #0x10]
003a9764: bl       #0x3a6a24 ; _ZN9Character18NetStructCharacterC1Ev
003a9768: add      r0, r4, #0x304
003a976c: mov      r1, r4
003a9770: strb     fp, [r4, #0x28]
003a9774: bl       #0x4a191c ; _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
003a9778: strb     fp, [r4, #0x1c4]
003a977c: strb     fp, [r4, #0x85]
003a9780: mov      r0, #0x10
003a9784: mov      r1, r8
003a9788: bl       #0x310570 ; _Znwj15MemoryHintState
003a978c: ldr      r3, [pc, #0x13c]
003a9790: ldr      ip, [sp, #0xc]
003a9794: mov      r6, r0
003a9798: ldr      r3, [sb, r3]
003a979c: cmp      ip, r8
003a97a0: strb     r8, [r6, #0xa]
003a97a4: add      r3, r3, #8
003a97a8: str      r8, [r0, #0xc]
003a97ac: stm      r0, {r3, ip}
003a97b0: strb     r8, [r6, #8]
003a97b4: strb     r8, [r6, #9]
003a97b8: beq      #0x3a986c
003a97bc: mov      r0, ip
003a97c0: mov      r1, r6
003a97c4: bl       #0x404e10 ; _ZN14v2Controllable13SetControllerEP12v2Controller
003a97c8: ldr      r3, [r4, #0x378]
003a97cc: ldr      r0, [sp, #0x20]
003a97d0: mov      r1, r4
003a97d4: str      r4, [r3, #0xc]
003a97d8: bl       #0x3db480 ; _ZN10CharTimers12SetCharacterEP9Character
003a97dc: ldr      r0, [sp, #0x1c]
003a97e0: mov      r1, r4
003a97e4: bl       #0x3cb7c0 ; _ZN6CharAI12SetCharacterEP9Character
003a97e8: ldr      r0, [sp, #0x14]
003a97ec: mov      r1, r4
003a97f0: bl       #0x3c9890 ; _ZN12CharAnimator12SetCharacterEP9Character
003a97f4: mov      r0, r5
003a97f8: mov      r1, r4
003a97fc: bl       #0x3c1600 ; _ZN16CharStateMachine12SetCharacterEP9Character
003a9800: ldr      r0, [sp, #0x18]
003a9804: mov      r1, r4
003a9808: bl       #0x3dec0c ; _ZN14CharProperties12SetCharacterEP9Character
003a980c: mov      r6, #0
003a9810: str      r4, [r4, #0x380]
003a9814: mov      r1, r6
003a9818: mov      r0, r5
003a981c: add      r6, r6, #1
003a9820: bl       #0x3c7318 ; _ZN16CharStateMachine13RegisterStateEi
003a9824: cmp      r6, #0x14
003a9828: bne      #0x3a9814
003a982c: mov      r1, #0
003a9830: movw     r2, #0x14e0
003a9834: str      r1, [r4, r2]
003a9838: mvn      r3, #0
003a983c: movw     r2, #0x14f4
003a9840: str      r3, [r4, r2]
003a9844: str      r7, [r4, #0x100]
003a9848: ldr      sl, [sp, #0x10]
003a984c: movw     r2, #0x14f8
003a9850: mov      r0, r4
003a9854: str      sl, [r4, #0x104]
003a9858: str      r3, [r4, r2]
003a985c: mov      r3, #1
003a9860: strb     r3, [r4, #0xf8]
003a9864: add      sp, sp, #0x3c
003a9868: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a986c: ldr      r3, [pc, #0x60]
003a9870: ldr      r3, [sb, r3]
003a9874: ldr      r3, [r3]
003a9878: cmp      r3, #2
003a987c: streq    ip, [r4, #0x374]
003a9880: beq      #0x3a97bc
003a9884: cmp      r3, #1
003a9888: bne      #0x3a97bc
003a988c: ldr      r0, [pc, #0x44]
003a9890: ldr      r1, [pc, #0x44]
003a9894: ldr      r2, [pc, #0x44]
003a9898: ldr      r0, [sb, r0]
003a989c: ldr      r3, [pc, #0x40]
003a98a0: mov      lr, #0x44
003a98a4: add      r1, pc, r1
003a98a8: add      r0, r0, #0xa8
003a98ac: add      r2, pc, r2
003a98b0: add      r3, pc, r3
003a98b4: str      ip, [sp, #0xc]
003a98b8: str      lr, [sp]
003a98bc: bl       #0x30e004 ; 
003a98c0: ldr      ip, [sp, #0xc]
003a98c4: b        #0x3a97bc
003a98c8: subseq   fp, lr, r8, asr #13
003a98cc: andeq    r2, r0, r8, lsl #28
003a98d0: andeq    r2, r0, r4, lsr #21
003a98d4: andeq    r3, r0, r0, asr #19
003a98d8: andeq    r1, r0, r0, asr #19
003a98dc: subseq   r4, r1, r4, lsr fp
003a98e0: subseq   sb, r1, r4, lsl ip
003a98e4: subseq   sb, r1, r0, lsr #24

# _ZThn36_N8RoomZoneD0Ev
003968a8: sub      r0, r0, #0x24
003968ac: b        #0x3968b0

# _ZN13ObjectManager29AddOrphanRenderObjectToDeleteEP10ObjectBase
00343188: push     {r4, r5, r6, lr}
0034318c: subs     r6, r1, #0
00343190: mov      r4, r0
00343194: beq      #0x3431bc
00343198: add      r5, r0, #4
0034319c: mov      r0, r5
003431a0: bl       #0x343168 ; 
003431a4: str      r6, [r0, #8]
003431a8: ldr      r3, [r4, #8]
003431ac: str      r5, [r0]
003431b0: str      r3, [r0, #4]
003431b4: str      r0, [r3]
003431b8: str      r0, [r4, #8]
003431bc: pop      {r4, r5, r6, pc}
