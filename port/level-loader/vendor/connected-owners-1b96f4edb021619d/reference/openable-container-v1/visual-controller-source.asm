_ZN17AnimSetController8PlayClipEPKcbij
00474f68 mov      r0, #0
00474f6c bx       lr
_ZN17AnimSetController8PlayClipEjbij
00474ff0 cmn      r1, #1
00474ff4 push     {r4, r5, r6, r7, r8, sb, sl, lr}
00474ff8 mov      r4, r1
00474ffc mov      r8, r2
00475000 mov      r7, r0
00475004 beq      #0x4750bc
00475008 ldr      r1, [sp, #0x20]
0047500c bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00475010 subs     r5, r0, #0
00475014 beq      #0x4750c4
00475018 ldr      r3, [r5]
0047501c mov      lr, pc
00475020 ldr      pc, [r3, #0x44]
00475024 mov      r6, r0
00475028 mov      r0, r5
0047502c bl       #0x65f114 ; _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
00475030 mov      sl, r0
00475034 mov      r0, r5
00475038 bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
0047503c mov      r1, r4
00475040 mov      sb, r0
00475044 mov      r0, r5
00475048 bl       #0x3674ac ; _ZN11AnimatorSet19SetCurrentAnimationEi
0047504c cmn      r0, #1
00475050 mov      r4, r0
00475054 beq      #0x4750bc
00475058 ldr      r3, [r6, #0x34]
0047505c cmp      r3, #0
00475060 beq      #0x475078
00475064 mov      r0, r5
00475068 ldr      r3, [r5]
0047506c ldr      r1, [r7, #0xc]
00475070 mov      lr, pc
00475074 ldr      pc, [r3, #0x30]
00475078 cmp      sl, r4
0047507c beq      #0x4750d8
00475080 mov      r1, r8
00475084 mov      r0, r6
00475088 ldr      r3, [r6]
0047508c mov      lr, pc
00475090 ldr      pc, [r3, #0x40]
00475094 mov      r0, r6
00475098 mov      r1, #0x3f800000
0047509c ldr      r3, [r6]
004750a0 mov      lr, pc
004750a4 ldr      pc, [r3, #0x48]
004750a8 ldr      r0, [r7, #4]
004750ac ldrb     r1, [r7, #0x10]
004750b0 bl       #0x35d624 ; _ZN13RootSceneNode7NewAnimEb
004750b4 mov      r0, #1
004750b8 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004750bc mov      r0, #0
004750c0 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004750c4 bl       #0x65f114 ; _ZNK6glitch7collada21CSceneNodeAnimatorSet19getCurrentAnimationEv
004750c8 mov      r0, r5
004750cc bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
004750d0 mov      r0, r5
004750d4 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004750d8 ldr      r3, [r6]
004750dc mov      r0, r6
004750e0 mov      lr, pc
004750e4 ldr      pc, [r3, #0x44]
004750e8 cmp      r0, #0
004750ec bne      #0x475080
004750f0 cmp      sb, #0
004750f4 ldr      r3, [r6]
004750f8 ldr      r1, [r6, #0x10]
004750fc ldrne    sb, [sb, #0x10]
00475100 ldr      r3, [r3, #0xc]
00475104 mov      r0, r6
00475108 add      r1, sb, r1
0047510c blx      r3
00475110 b        #0x475080
_ZN17AnimSetController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00474fb0 push     {r4, r5, r6, r7, r8, lr}
00474fb4 mov      r5, r1
00474fb8 mov      r1, #0
00474fbc mov      r7, r2
00474fc0 mov      r6, r3
00474fc4 ldr      r4, [sp, #0x18]
00474fc8 bl       #0x4748b8 ; _ZN14AnimController7GetAnimEj
00474fcc cmp      r0, #0
00474fd0 beq      #0x474fec
00474fd4 mov      r1, r5
00474fd8 mov      r2, r7
00474fdc mov      r3, r6
00474fe0 str      r4, [sp, #0x18]
00474fe4 pop      {r4, r5, r6, r7, r8, lr}
00474fe8 b        #0x367510
00474fec pop      {r4, r5, r6, r7, r8, pc}
_ZN17AnimSetControllerC1EP13RootSceneNodei
004751b4 push     {r4, r5, r6, lr}
004751b8 mov      r6, r2
004751bc ldr      r5, [pc, #0x78]
004751c0 mov      r2, #1
004751c4 mov      r4, r0
004751c8 bl       #0x474e44 ; _ZN14AnimControllerC2EP13RootSceneNodeb
004751cc ldr      r3, [pc, #0x6c]
004751d0 add      r5, pc, r5
004751d4 str      r6, [r4, #8]
004751d8 ldr      r3, [r5, r3]
004751dc mov      r1, r6
004751e0 add      r3, r3, #8
004751e4 str      r3, [r4]
004751e8 mov      r3, #0
004751ec str      r3, [r4, #0xc]
004751f0 mov      r3, #1
004751f4 strb     r3, [r4, #0x10]
004751f8 ldr      r3, [pc, #0x44]
004751fc ldr      r0, [r5, r3]
00475200 bl       #0x4762c4 ; _ZN14AnimSetManager11GetAnimatorEi
00475204 subs     r5, r0, #0
00475208 beq      #0x475234
0047520c ldr      r3, [r4, #4]
00475210 mov      r1, r5
00475214 mov      r0, r3
00475218 ldr      r3, [r3]
0047521c mov      lr, pc
00475220 ldr      pc, [r3, #0x6c]
00475224 ldr      r3, [r5]
00475228 ldr      r0, [r3, #-0xc]
0047522c add      r0, r5, r0
00475230 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00475234 mov      r0, r4
00475238 pop      {r4, r5, r6, pc}
0047523c subseq   pc, r1, r0, asr #17
00475240 andeq    r3, r0, ip, lsr #10
00475244 andeq    r4, r0, r8, lsr r8
_ZN12VisualObjectC1EP10GameObjectRKSsS3_
00472a0c push     {r4, r5, r6, r7, r8, sl, lr}
00472a10 ldr      r6, [pc, #0x234]
00472a14 ldr      r5, [pc, #0x234]
00472a18 mov      ip, #0xbf000000
00472a1c add      r6, pc, r6
00472a20 ldr      r5, [r6, r5]
00472a24 mov      lr, #0
00472a28 add      ip, ip, #0x800000
00472a2c mov      r7, r1
00472a30 add      r1, r5, #8
00472a34 mov      r5, #0
00472a38 mov      r8, r3
00472a3c sub      sp, sp, #0xc
00472a40 str      r1, [r0]
00472a44 str      lr, [r0, #0x24]
00472a48 str      ip, [r0, #0x74]
00472a4c str      lr, [r0, #0x10]
00472a50 str      lr, [r0, #0x14]
00472a54 str      lr, [r0, #0x18]
00472a58 str      lr, [r0, #0x1c]
00472a5c str      lr, [r0, #0x20]
00472a60 str      ip, [r0, #0x58]
00472a64 str      ip, [r0, #0x5c]
00472a68 str      ip, [r0, #0x60]
00472a6c str      ip, [r0, #0x64]
00472a70 str      ip, [r0, #0x68]
00472a74 str      r7, [r0, #4]
00472a78 str      r5, [r0, #8]
00472a7c str      r5, [r0, #0xc]
00472a80 strb     r5, [r0, #0x28]
00472a84 str      r5, [r0, #0x2c]
00472a88 str      r5, [r0, #0x30]
00472a8c str      r5, [r0, #0x34]
00472a90 str      r5, [r0, #0x38]
00472a94 strb     r5, [r0, #0x3c]
00472a98 str      r5, [r0, #0x40]
00472a9c str      r5, [r0, #0x44]
00472aa0 str      r5, [r0, #0x48]
00472aa4 str      r5, [r0, #0x4c]
00472aa8 str      r5, [r0, #0x50]
00472aac str      r5, [r0, #0x54]
00472ab0 strb     r5, [r0, #0x6c]
00472ab4 strb     r5, [r0, #0x7c]
00472ab8 strb     r5, [r0, #0x7d]
00472abc strb     r5, [r0, #0x7e]
00472ac0 strb     r5, [r0, #0x7f]
00472ac4 str      r5, [r0, #0x80]
00472ac8 str      r5, [r0, #0x84]
00472acc str      r5, [r0, #0x88]
00472ad0 str      r5, [r0, #0x8c]
00472ad4 str      r5, [r0, #0x90]
00472ad8 str      r5, [r0, #0x94]
00472adc str      r5, [r0, #0x9c]
00472ae0 str      r5, [r0, #0xa0]
00472ae4 str      r5, [r0, #0xa4]
00472ae8 strb     r5, [r0, #0xa9]
00472aec mov      sl, r2
00472af0 mov      r4, r0
00472af4 bl       #0x50a564 ; _ZN12AssetManager15GetAssetManagerEv
00472af8 ldr      ip, [r8, #0x10]
00472afc ldr      r2, [r8, #0x14]
00472b00 ldr      r1, [sl, #0x14]
00472b04 mov      r3, r5
00472b08 cmp      ip, r2
00472b0c moveq    r2, r5
00472b10 mvn      ip, #0x80000000
00472b14 str      ip, [sp]
00472b18 bl       #0x50a504 ; _ZN12AssetManager13loadSceneNodeEPKcS1_bi
00472b1c cmp      r0, r5
00472b20 str      r0, [r4, #8]
00472b24 beq      #0x472c40
00472b28 mov      r1, r7
00472b2c mov      r0, r4
00472b30 bl       #0x47295c ; _ZN12VisualObject9SetParentEP10GameObject
00472b34 ldr      r0, [r4, #8]
00472b38 bl       #0x35c854 ; _ZN13RootSceneNode18RefreshBoundingBoxEv
00472b3c mov      r0, r4
00472b40 bl       #0x4718f0 ; _ZN12VisualObject27_FindModularSkinnedMeshNodeEv
00472b44 ldr      r3, [pc, #0x108]
00472b48 ldr      r1, [r4, #8]
00472b4c ldr      r5, [r6, r3]
00472b50 ldr      r3, [r5, #0x10]
00472b54 ldr      r3, [r3, #0x1c]
00472b58 ldr      r3, [r3, #4]
00472b5c mov      r0, r3
00472b60 ldr      r3, [r3]
00472b64 mov      lr, pc
00472b68 ldr      pc, [r3, #0x5c]
00472b6c ldr      r3, [r5, #0x10]
00472b70 ldr      r0, [r3, #0x1c]
00472b74 bl       #0x350ee0 ; _ZN12SceneManager13ForceRegisterEv
00472b78 ldr      r3, [r5, #0x10]
00472b7c ldr      r2, [pc, #0xd4]
00472b80 ldr      r1, [r4, #8]
00472b84 ldr      r0, [r3, #0x1c]
00472b88 add      r2, pc, r2
00472b8c mov      r3, #1
00472b90 bl       #0x35a0e4 ; _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPKcb
00472b94 subs     r2, r0, #0
00472b98 beq      #0x472c08
00472b9c mov      r3, #1
00472ba0 strb     r3, [r4, #0x28]
00472ba4 ldr      r3, [r5, #0x10]
00472ba8 movw     r1, #0x6164
00472bac movt     r1, #0x6d65
00472bb0 ldr      r3, [r3, #0x1c]
00472bb4 mov      r0, r3
00472bb8 ldr      r3, [r3]
00472bbc mov      lr, pc
00472bc0 ldr      pc, [r3, #0x1c]
00472bc4 cmp      r0, #0
00472bc8 str      r0, [r4, #0xc]
00472bcc beq      #0x472bec
00472bd0 ldr      r3, [r0]
00472bd4 ldr      r3, [r3, #-0xc]
00472bd8 add      r0, r0, r3
00472bdc ldr      r3, [r0, #4]
00472be0 add      r3, r3, #1
00472be4 str      r3, [r0, #4]
00472be8 ldr      r0, [r4, #0xc]
00472bec mov      r1, #0
00472bf0 strb     r1, [r0, #0x138]
00472bf4 ldr      r3, [r4, #0xc]
00472bf8 mov      r0, r3
00472bfc ldr      r3, [r3]
00472c00 mov      lr, pc
00472c04 ldr      pc, [r3, #0x48]
00472c08 mov      r0, r4
00472c0c bl       #0x47211c ; _ZN12VisualObject11CalcMeshBoxEv
00472c10 mov      r0, r4
00472c14 bl       #0x470a54 ; _ZN12VisualObject12ApplyMeshBoxEv
00472c18 mov      r1, #0
00472c1c mov      r0, #8
00472c20 bl       #0x310570 ; _Znwj15MemoryHintState
00472c24 ldr      r1, [r4, #8]
00472c28 mov      r5, r0
00472c2c mov      r2, #0
00472c30 bl       #0x474d30 ; _ZN14AnimControllerC1EP13RootSceneNodeb
00472c34 mov      r0, r4
00472c38 mov      r1, r5
00472c3c bl       #0x470a84 ; _ZN12VisualObject17SetAnimControllerEP14AnimController
00472c40 mov      r0, r4
00472c44 add      sp, sp, #0xc
00472c48 pop      {r4, r5, r6, r7, r8, sl, pc}
00472c4c subseq   r2, r2, r4, ror r0
00472c50 muleq    r0, r4, lr
00472c54 strdeq   r3, r4, [r0], -r4
00472c58 subeq    sl, r5, r0, lsr #22
