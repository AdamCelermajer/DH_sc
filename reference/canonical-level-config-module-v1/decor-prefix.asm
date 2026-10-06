_ZN5Decor8InitPostEv
00388a98 push     {r4, r5, r6, lr}
00388a9c mov      r4, r0
00388aa0 bl       #0x38be5c ; _ZN10GameObject8InitPostEv
00388aa4 ldr      r0, [r4, #0x2d8]
00388aa8 ldr      r5, [pc, #0x68]
00388aac cmp      r0, #0
00388ab0 add      r5, pc, r5
00388ab4 beq      #0x388b14
00388ab8 bl       #0x470a54 ; _ZN12VisualObject12ApplyMeshBoxEv
00388abc ldr      r3, [r4, #0x2d8]
00388ac0 ldrb     r3, [r3, #0x28]
00388ac4 cmp      r3, #0
00388ac8 bne      #0x388ad8
00388acc mov      r0, r4
00388ad0 pop      {r4, r5, r6, lr}
00388ad4 b        #0x388730
00388ad8 ldr      r3, [pc, #0x3c]
00388adc mov      r1, #0
00388ae0 mov      r0, #0x28
00388ae4 ldr      r3, [r5, r3]
00388ae8 ldr      r6, [r3, #0x44]
00388aec bl       #0x310570 ; _Znwj15MemoryHintState
00388af0 mov      r1, r6
00388af4 mov      r5, r0
00388af8 mov      r2, r4
00388afc bl       #0x388a2c ; _ZN7PODecorC1EP13PhysicalWorldP10GameObjectb.clone.2
00388b00 mov      r0, r4
00388b04 mov      r1, r5
00388b08 mov      r2, #0
00388b0c bl       #0x394bf8 ; _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
00388b10 b        #0x388acc
00388b14 pop      {r4, r5, r6, pc}
00388b18 rsbeq    fp, r0, r0, ror #31
00388b1c strdeq   r3, r4, [r0], -r4
_ZN5Decor12LoadFloorMapEv
00388730 push     {r4, lr}
00388734 ldr      r2, [r0, #0x2d8]
00388738 ldr      r3, [pc, #0x68]
0038873c mov      r4, r0
00388740 cmp      r2, #0
00388744 add      r3, pc, r3
00388748 beq      #0x388758
0038874c ldrb     r1, [r0, #0x375]
00388750 cmp      r1, #0
00388754 bne      #0x38875c
00388758 pop      {r4, pc}
0038875c ldr      r1, [r2, #8]
00388760 ldr      r2, [r0, #0x64]
00388764 ldr      r0, [pc, #0x40]
00388768 ldr      r0, [r3, r0]
0038876c ldr      r3, [r4, #0x44]
00388770 bl       #0x523c14 ; _ZN7PFWorld8LoadRoomEPN6glitch5scene10ISceneNodeEjPKc
00388774 cmp      r0, #0
00388778 beq      #0x38879c
0038877c ldrb     r3, [r4, #0x376]
00388780 add      r1, r4, #0x12c
00388784 cmp      r3, #0
00388788 ldr      r3, [r0, #0x24]
0038878c orrne    r3, r3, #1
00388790 biceq    r3, r3, #1
00388794 str      r3, [r0, #0x24]
00388798 bl       #0x388218 ; _ZN6PFRoom17ExtendBoundingBoxERK4aabbIfE
0038879c mov      r3, #0
003887a0 strb     r3, [r4, #0x375]
003887a4 pop      {r4, pc}
003887a8 rsbeq    ip, r0, ip, asr #6
003887ac andeq    r1, r0, r4, lsl #4
