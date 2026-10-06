_ZN12VisualObject11CalcMeshBoxEv
0047211c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00472120 ldr      r2, [pc, #0x5d4]
00472124 ldr      r3, [r0, #0xc]
00472128 sub      sp, sp, #0x64
0047212c add      r2, pc, r2
00472130 cmp      r3, #0
00472134 str      r2, [sp, #8]
00472138 mov      r4, r0
0047213c beq      #0x472408
00472140 mov      r0, r3
00472144 ldr      r3, [r3]
00472148 mov      lr, pc
0047214c ldr      pc, [r3, #0x30]
00472150 ldr      r1, [r0]
00472154 mov      r3, r0
00472158 ldr      r2, [r4, #0xc]
0047215c str      r1, [r4, #0x10]
00472160 ldr      r1, [r0, #4]
00472164 mov      r0, r2
00472168 str      r1, [r4, #0x14]
0047216c ldr      r3, [r3, #8]
00472170 str      r3, [r4, #0x18]
00472174 ldr      r3, [r2]
00472178 mov      lr, pc
0047217c ldr      pc, [r3, #0x30]
00472180 ldr      r2, [r0, #0xc]
00472184 mov      r3, r0
00472188 ldr      r0, [r4, #0xc]
0047218c str      r2, [r4, #0x1c]
00472190 ldr      r2, [r3, #0x10]
00472194 str      r2, [r4, #0x20]
00472198 ldr      r3, [r3, #0x14]
0047219c str      r3, [r4, #0x24]
004721a0 bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
004721a4 ldr      r3, [r0]
004721a8 mov      lr, pc
004721ac ldr      pc, [r3, #0x90]
004721b0 mov      r3, r0
004721b4 ldr      r1, [r0]
004721b8 ldr      r0, [r4, #0x10]
004721bc ldr      r6, [r3, #4]
004721c0 ldr      r5, [r3, #8]
004721c4 bl       #0x30ed6c
004721c8 mov      r1, r6
004721cc str      r0, [r4, #0x10]
004721d0 ldr      r0, [r4, #0x14]
004721d4 bl       #0x30ed6c
004721d8 mov      r1, r5
004721dc str      r0, [r4, #0x14]
004721e0 ldr      r0, [r4, #0x18]
004721e4 bl       #0x30ed6c
004721e8 str      r0, [r4, #0x18]
004721ec ldr      r0, [r4, #0xc]
004721f0 bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
004721f4 ldr      r3, [r0]
004721f8 mov      lr, pc
004721fc ldr      pc, [r3, #0x90]
00472200 mov      r3, r0
00472204 ldr      r1, [r0]
00472208 ldr      r0, [r4, #0x1c]
0047220c ldr      r6, [r3, #4]
00472210 ldr      r5, [r3, #8]
00472214 bl       #0x30ed6c
00472218 mov      r1, r6
0047221c str      r0, [r4, #0x1c]
00472220 ldr      r0, [r4, #0x20]
00472224 bl       #0x30ed6c
00472228 mov      r1, r5
0047222c str      r0, [r4, #0x20]
00472230 ldr      r0, [r4, #0x24]
00472234 bl       #0x30ed6c
00472238 str      r0, [r4, #0x24]
0047223c ldr      r3, [r4, #8]
00472240 add      r5, sp, #0x10
00472244 mov      r6, #0
00472248 mov      r0, r3
0047224c ldr      r3, [r3]
00472250 mov      lr, pc
00472254 ldr      pc, [r3, #0x40]
00472258 mov      r2, #0x41
0047225c mov      r1, r0
00472260 mov      r0, r5
00472264 strb     r6, [sp, #0x50]
00472268 bl       #0x30e868
0047226c mov      r3, #0
00472270 mov      r1, r5
00472274 add      r0, r4, #0x10
00472278 str      r3, [sp, #0x48]
0047227c str      r3, [sp, #0x40]
00472280 str      r3, [sp, #0x44]
00472284 strb     r6, [sp, #0x50]
00472288 bl       #0x312da8 ; _ZN7Point3DIfE9transformERKN6glitch4core8CMatrix4IfEE
0047228c mov      r1, r5
00472290 add      r0, r4, #0x1c
00472294 bl       #0x312da8 ; _ZN7Point3DIfE9transformERKN6glitch4core8CMatrix4IfEE
00472298 ldr      r7, [r4, #0x1c]
0047229c ldr      r5, [r4, #0x10]
004722a0 mov      r0, r7
004722a4 mov      r1, r5
004722a8 bl       #0x30e70c
004722ac mov      r1, r5
004722b0 cmp      r0, r6
004722b4 mov      r0, r7
004722b8 movne    fp, r7
004722bc moveq    fp, r5
004722c0 bl       #0x30e2f8
004722c4 cmp      r0, #0
004722c8 ldr      r6, [r4, #0x20]
004722cc moveq    r7, r5
004722d0 ldr      r5, [r4, #0x14]
004722d4 mov      r0, r6
004722d8 str      r7, [r4, #0x1c]
004722dc mov      r1, r5
004722e0 str      fp, [r4, #0x10]
004722e4 bl       #0x30e70c
004722e8 mov      r1, r5
004722ec cmp      r0, #0
004722f0 mov      r0, r6
004722f4 movne    sb, r6
004722f8 moveq    sb, r5
004722fc bl       #0x30e2f8
00472300 cmp      r0, #0
00472304 ldr      r8, [r4, #0x18]
00472308 moveq    r6, r5
0047230c ldr      r5, [r4, #0x24]
00472310 mov      r1, r8
00472314 str      r6, [r4, #0x20]
00472318 mov      r0, r5
0047231c str      sb, [r4, #0x14]
00472320 bl       #0x30e70c
00472324 mov      r1, r8
00472328 cmp      r0, #0
0047232c mov      r0, r5
00472330 movne    sl, r5
00472334 moveq    sl, r8
00472338 bl       #0x30e2f8
0047233c cmp      r0, #0
00472340 moveq    r5, r8
00472344 mov      r1, fp
00472348 str      r5, [r4, #0x24]
0047234c mov      r0, r7
00472350 str      sl, [r4, #0x18]
00472354 bl       #0x30e3ac
00472358 mov      r1, #0x3f000000
0047235c bl       #0x30ed6c
00472360 mov      r1, sb
00472364 mov      r7, r0
00472368 mov      r0, r6
0047236c bl       #0x30e3ac
00472370 mov      r1, #0x3f000000
00472374 bl       #0x30ed6c
00472378 mov      r1, sl
0047237c mov      r8, r0
00472380 mov      r0, r5
00472384 bl       #0x30e3ac
00472388 mov      r1, #0x3f000000
0047238c bl       #0x30ed6c
00472390 ldr      ip, [sp, #8]
00472394 ldr      r3, [pc, #0x364]
00472398 mov      r6, r0
0047239c mov      r1, r7
004723a0 ldr      r5, [ip, r3]
004723a4 ldr      r0, [r5]
004723a8 bl       #0x30e3ac
004723ac str      r0, [r4, #0x10]
004723b0 ldr      r1, [r5]
004723b4 mov      r0, r7
004723b8 bl       #0x30eba4
004723bc str      r0, [r4, #0x1c]
004723c0 ldr      r0, [r5, #4]
004723c4 mov      r1, r8
004723c8 bl       #0x30e3ac
004723cc str      r0, [r4, #0x14]
004723d0 ldr      r1, [r5, #4]
004723d4 mov      r0, r8
004723d8 bl       #0x30eba4
004723dc str      r0, [r4, #0x20]
004723e0 ldr      r0, [r5, #8]
004723e4 mov      r1, r6
004723e8 bl       #0x30e3ac
004723ec str      r0, [r4, #0x18]
004723f0 ldr      r1, [r5, #8]
004723f4 mov      r0, r6
004723f8 bl       #0x30eba4
004723fc str      r0, [r4, #0x24]
00472400 add      sp, sp, #0x64
00472404 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00472408 ldr      ip, [sp, #8]
0047240c ldr      r0, [pc, #0x2f0]
00472410 mvn      r1, #0x80000000
00472414 sub      r1, r1, #0x800000
00472418 ldr      r5, [ip, r0]
0047241c mvn      r2, #0x800000
00472420 str      r1, [r4, #0x18]
00472424 str      r1, [r4, #0x10]
00472428 str      r1, [r4, #0x14]
0047242c str      r2, [r4, #0x24]
00472430 str      r2, [r4, #0x1c]
00472434 str      r2, [r4, #0x20]
00472438 ldr      r2, [r5, #0x10]
0047243c str      r3, [sp, #0x5c]
00472440 str      r3, [sp, #0x54]
00472444 str      r3, [sp, #0x58]
00472448 ldr      r3, [r2, #0x1c]
0047244c add      r6, sp, #0x54
00472450 movw     r1, #0x6164
00472454 mov      r0, r3
00472458 ldr      ip, [r3]
0047245c movt     r1, #0x7365
00472460 ldr      r3, [r4, #8]
00472464 mov      r2, r6
00472468 mov      lr, pc
0047246c ldr      pc, [ip, #0x20]
00472470 ldr      r0, [sp, #0x54]
00472474 ldr      r3, [sp, #0x58]
00472478 rsb      r3, r0, r3
0047247c asrs     r3, r3, #2
00472480 str      r3, [sp, #0xc]
00472484 beq      #0x472690
00472488 ldr      r2, [sp, #0xc]
0047248c cmp      r2, #0
00472490 beq      #0x472680
00472494 mov      r5, #0
00472498 b        #0x4724a0
0047249c ldr      r0, [sp, #0x54]
004724a0 ldr      r3, [r0, r5, lsl #2]
004724a4 mov      r0, r3
004724a8 ldr      r3, [r3]
004724ac mov      lr, pc
004724b0 ldr      pc, [r3, #0x30]
004724b4 ldr      r3, [sp, #0x54]
004724b8 ldr      sl, [r0, #8]
004724bc ldr      fp, [r0]
004724c0 ldr      r3, [r3, r5, lsl #2]
004724c4 ldr      sb, [r0, #4]
004724c8 mov      r0, r3
004724cc ldr      r3, [r3]
004724d0 mov      lr, pc
004724d4 ldr      pc, [r3, #0x30]
004724d8 ldr      r2, [sp, #0x54]
004724dc mov      r3, r0
004724e0 ldr      r6, [r0, #0x14]
004724e4 ldr      r8, [r0, #0xc]
004724e8 ldr      r0, [r2, r5, lsl #2]
004724ec ldr      r7, [r3, #0x10]
004724f0 bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
004724f4 cmp      r0, #0
004724f8 beq      #0x4725bc
004724fc ldr      r3, [sp, #0x54]
00472500 ldr      r0, [r3, r5, lsl #2]
00472504 bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
00472508 ldr      r3, [r0]
0047250c mov      lr, pc
00472510 ldr      pc, [r3, #0x90]
00472514 mov      r2, r0
00472518 ldr      r3, [r2, #4]
0047251c ldr      r2, [r2, #8]
00472520 ldr      r1, [r0]
00472524 mov      r0, fp
00472528 stm      sp, {r2, r3}
0047252c bl       #0x30ed6c
00472530 ldr      r3, [sp, #4]
00472534 mov      fp, r0
00472538 mov      r0, sb
0047253c mov      r1, r3
00472540 bl       #0x30ed6c
00472544 ldr      r2, [sp]
00472548 mov      sb, r0
0047254c mov      r0, sl
00472550 mov      r1, r2
00472554 bl       #0x30ed6c
00472558 ldr      r3, [sp, #0x54]
0047255c mov      sl, r0
00472560 ldr      r0, [r3, r5, lsl #2]
00472564 bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
00472568 ldr      r3, [r0]
0047256c mov      lr, pc
00472570 ldr      pc, [r3, #0x90]
00472574 mov      r2, r0
00472578 ldr      r3, [r2, #4]
0047257c ldr      r2, [r2, #8]
00472580 ldr      r1, [r0]
00472584 mov      r0, r8
00472588 stm      sp, {r2, r3}
0047258c bl       #0x30ed6c
00472590 ldr      r3, [sp, #4]
00472594 mov      r8, r0
00472598 mov      r0, r7
0047259c mov      r1, r3
004725a0 bl       #0x30ed6c
004725a4 ldr      r2, [sp]
004725a8 mov      r7, r0
004725ac mov      r0, r6
004725b0 mov      r1, r2
004725b4 bl       #0x30ed6c
004725b8 mov      r6, r0
004725bc ldr      r3, [r4, #0x10]
004725c0 mov      r1, fp
004725c4 add      r5, r5, #1
004725c8 mov      r0, r3
004725cc str      r3, [sp, #4]
004725d0 bl       #0x30e2f8
004725d4 cmp      r0, #0
004725d8 ldr      r3, [sp, #4]
004725dc movne    r3, fp
004725e0 ldr      fp, [r4, #0x14]
004725e4 str      r3, [r4, #0x10]
004725e8 mov      r1, sb
004725ec mov      r0, fp
004725f0 bl       #0x30e2f8
004725f4 cmp      r0, #0
004725f8 movne    fp, sb
004725fc ldr      sb, [r4, #0x18]
00472600 mov      r1, sl
00472604 str      fp, [r4, #0x14]
00472608 mov      r0, sb
0047260c bl       #0x30e2f8
00472610 cmp      r0, #0
00472614 movne    sb, sl
00472618 ldr      sl, [r4, #0x1c]
0047261c mov      r1, r8
00472620 str      sb, [r4, #0x18]
00472624 mov      r0, sl
00472628 bl       #0x30e70c
0047262c cmp      r0, #0
00472630 movne    sl, r8
00472634 ldr      r8, [r4, #0x20]
00472638 mov      r1, r7
0047263c str      sl, [r4, #0x1c]
00472640 mov      r0, r8
00472644 bl       #0x30e70c
00472648 cmp      r0, #0
0047264c movne    r8, r7
00472650 ldr      r7, [r4, #0x24]
00472654 str      r8, [r4, #0x20]
00472658 mov      r1, r6
0047265c mov      r0, r7
00472660 bl       #0x30e70c
00472664 ldr      r3, [sp, #0xc]
00472668 cmp      r0, #0
0047266c movne    r7, r6
00472670 cmp      r5, r3
00472674 str      r7, [r4, #0x24]
00472678 bne      #0x47249c
0047267c ldr      r0, [sp, #0x54]
00472680 cmp      r0, #0
00472684 beq      #0x47223c
00472688 bl       #0x310450 ; _Z10GlitchFreePv
0047268c b        #0x47223c
00472690 ldr      r3, [r5, #0x10]
00472694 movw     r1, #0x6164
00472698 movt     r1, #0x6d65
0047269c ldr      ip, [r3, #0x1c]
004726a0 mov      r2, r6
004726a4 ldr      r3, [r4, #8]
004726a8 mov      r0, ip
004726ac ldr      ip, [ip]
004726b0 mov      lr, pc
004726b4 ldr      pc, [ip, #0x20]
004726b8 ldr      r0, [sp, #0x54]
004726bc ldr      r3, [sp, #0x58]
004726c0 rsb      r3, r0, r3
004726c4 asrs     r3, r3, #2
004726c8 str      r3, [sp, #0xc]
004726cc bne      #0x472488
004726d0 mov      r3, #0
004726d4 cmp      r0, #0
004726d8 str      r3, [r4, #0x24]
004726dc str      r3, [r4, #0x10]
004726e0 str      r3, [r4, #0x14]
004726e4 str      r3, [r4, #0x18]
004726e8 str      r3, [r4, #0x1c]
004726ec str      r3, [r4, #0x20]
004726f0 beq      #0x472400
004726f4 bl       #0x310450 ; _Z10GlitchFreePv
004726f8 b        #0x472400
004726fc subseq   r2, r2, r4, ror #18
00472700 andeq    r3, r0, ip, lsr #30
00472704 strdeq   r3, r4, [r0], -r4
_ZN12VisualObject12ApplyMeshBoxEv
00470a54 push     {r4, lr}
00470a58 ldr      r3, [r0, #4]
00470a5c mov      r1, r0
00470a60 cmp      r3, #0
00470a64 beq      #0x470a80
00470a68 mov      r0, r3
00470a6c ldrb     r2, [r1, #0x28]
00470a70 ldr      r3, [r3]
00470a74 add      r1, r1, #0x10
00470a78 mov      lr, pc
00470a7c ldr      pc, [r3, #0x9c]
00470a80 pop      {r4, pc}
_ZN12VisualObject11SyncScalingEv
00472860 ldr      r1, [r0, #4]
00472864 cmp      r1, #0
00472868 bxeq     lr
0047286c add      r1, r1, #0x120
00472870 b        #0x4727ac
_ZN12VisualObject12SyncRotationEv
00472948 ldr      r1, [r0, #4]
0047294c cmp      r1, #0
00472950 bxeq     lr
00472954 add      r1, r1, #0x16c
00472958 b        #0x472874
_ZN12VisualObject9SetParentEP10GameObject
0047295c push     {r4, r5, r6, lr}
00472960 ldr      r4, [pc, #0x9c]
00472964 cmp      r1, #0
00472968 str      r1, [r0, #4]
0047296c mov      r5, r0
00472970 add      r4, pc, r4
00472974 beq      #0x4729c8
00472978 ldrb     r3, [r1, #0x84]
0047297c cmp      r3, #0
00472980 bne      #0x4729a4
00472984 mov      r0, r5
00472988 bl       #0x38ba74 ; _ZN12VisualObject4SyncEv
0047298c ldr      r3, [pc, #0x74]
00472990 ldr      r0, [r5, #8]
00472994 mov      r1, #1
00472998 ldr      r2, [r4, r3]
0047299c pop      {r4, r5, r6, lr}
004729a0 b        #0x50e484
004729a4 mov      r0, r1
004729a8 ldr      r3, [r1]
004729ac mov      lr, pc
004729b0 ldr      pc, [r3, #0x80]
004729b4 cmp      r0, #0
004729b8 beq      #0x4729cc
004729bc ldr      r3, [r5, #4]
004729c0 cmp      r3, #0
004729c4 bne      #0x472984
004729c8 pop      {r4, r5, r6, pc}
004729cc mov      r0, r5
004729d0 bl       #0x38ba74 ; _ZN12VisualObject4SyncEv
004729d4 ldr      r6, [r5, #8]
004729d8 ldr      r3, [r6]
004729dc mov      r0, r6
004729e0 ldr      r4, [r3, #0xa4]
004729e4 mov      lr, pc
004729e8 ldr      pc, [r3, #0xa0]
004729ec mov      r1, r0
004729f0 mov      r0, r6
004729f4 blx      r4
004729f8 ldr      r0, [r5, #8]
004729fc pop      {r4, r5, r6, lr}
00472a00 b        #0x50f220
00472a04 subseq   r2, r2, r0, lsr #2
00472a08 andeq    r3, r0, r4, ror r0
_ZN12AssetManager13loadSceneNodeEPKcS1_bi
0050a504 ldr      r0, [pc, #0x2c]
0050a508 ldr      r3, [pc, #0x2c]
0050a50c str      r4, [sp, #-4]!
0050a510 add      r0, pc, r0
0050a514 ldr      r4, [r0, r3]
0050a518 subs     ip, r2, #0
0050a51c movne    ip, #1
0050a520 mov      r3, #0
0050a524 ldr      r0, [r4, #0x10]
0050a528 ldr      r0, [r0, #0x1c]
0050a52c str      ip, [sp, #4]
0050a530 ldm      sp!, {r4}
0050a534 b        #0x3596f8
0050a538 subeq    sl, r8, r0, lsl #11
0050a53c strdeq   r3, r4, [r0], -r4
