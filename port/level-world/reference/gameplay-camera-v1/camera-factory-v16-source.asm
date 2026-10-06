_ZN6glitch5scene24CDefaultSceneNodeFactoryC1EPNS0_13CSceneManagerEPNS_3gui14ICursorControlERKN5boost13intrusive_ptrINS_2io11IFileSystemEEE
006bb008 push     {r4, r5, r6, r7, r8, lr}
006bb00c ldr      r5, [pc, #0x4c8]
006bb010 ldr      ip, [pc, #0x4c8]
006bb014 ldr      r6, [pc, #0x4c8]
006bb018 add      r5, pc, r5
006bb01c ldr      ip, [r5, ip]
006bb020 ldr      lr, [r5, r6]
006bb024 mov      r4, r0
006bb028 add      ip, ip, #8
006bb02c mov      r0, #0
006bb030 mov      r7, #1
006bb034 ldr      lr, [lr]
006bb038 str      r2, [r4, #0x18]
006bb03c str      r7, [r4, #4]
006bb040 str      ip, [r4]
006bb044 str      r0, [r4, #0x10]
006bb048 str      r1, [r4, #0x14]
006bb04c str      r0, [r4, #8]
006bb050 str      r0, [r4, #0xc]
006bb054 ldr      r3, [r3]
006bb058 sub      sp, sp, #0x1c8
006bb05c str      lr, [sp, #0x1c4]
006bb060 cmp      r3, r0
006bb064 str      r3, [r4, #0x1c]
006bb068 ldrne    r2, [r3, #4]
006bb06c add      r8, sp, #0x1a8
006bb070 movw     r1, #0x7563
006bb074 addne    r2, r2, r7
006bb078 strne    r2, [r3, #4]
006bb07c ldr      r2, [pc, #0x464]
006bb080 movt     r1, #0x6562
006bb084 mov      r0, r8
006bb088 add      r2, pc, r2
006bb08c add      r7, r4, #8
006bb090 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb094 mov      r0, r7
006bb098 mov      r1, r8
006bb09c bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb0a0 ldr      r0, [sp, #0x1c0]
006bb0a4 add      r8, r8, #4
006bb0a8 cmp      r0, r8
006bb0ac beq      #0x6bb0bc
006bb0b0 cmp      r0, #0
006bb0b4 beq      #0x6bb0bc
006bb0b8 bl       #0x310450 ; _Z10GlitchFreePv
006bb0bc ldr      r2, [pc, #0x428]
006bb0c0 add      r8, sp, #0x18c
006bb0c4 movw     r1, #0x7073
006bb0c8 add      r2, pc, r2
006bb0cc movt     r1, #0x7268
006bb0d0 mov      r0, r8
006bb0d4 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb0d8 mov      r0, r7
006bb0dc mov      r1, r8
006bb0e0 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb0e4 ldr      r0, [sp, #0x1a4]
006bb0e8 add      r8, r8, #4
006bb0ec cmp      r0, r8
006bb0f0 beq      #0x6bb100
006bb0f4 cmp      r0, #0
006bb0f8 beq      #0x6bb100
006bb0fc bl       #0x310450 ; _Z10GlitchFreePv
006bb100 ldr      r2, [pc, #0x3e8]
006bb104 add      r8, sp, #0x170
006bb108 movw     r1, #0x6574
006bb10c add      r2, pc, r2
006bb110 movt     r1, #0x7478
006bb114 mov      r0, r8
006bb118 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb11c mov      r0, r7
006bb120 mov      r1, r8
006bb124 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb128 ldr      r0, [sp, #0x188]
006bb12c add      r8, r8, #4
006bb130 cmp      r0, r8
006bb134 beq      #0x6bb144
006bb138 cmp      r0, #0
006bb13c beq      #0x6bb144
006bb140 bl       #0x310450 ; _Z10GlitchFreePv
006bb144 ldr      r2, [pc, #0x3a8]
006bb148 add      r8, sp, #0x154
006bb14c movw     r1, #0x6574
006bb150 add      r2, pc, r2
006bb154 movt     r1, #0x7272
006bb158 mov      r0, r8
006bb15c bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb160 mov      r0, r7
006bb164 mov      r1, r8
006bb168 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb16c ldr      r0, [sp, #0x16c]
006bb170 add      r8, r8, #4
006bb174 cmp      r0, r8
006bb178 beq      #0x6bb188
006bb17c cmp      r0, #0
006bb180 beq      #0x6bb188
006bb184 bl       #0x310450 ; _Z10GlitchFreePv
006bb188 ldr      r2, [pc, #0x368]
006bb18c add      r8, sp, #0x138
006bb190 movw     r1, #0x6b73
006bb194 add      r2, pc, r2
006bb198 movt     r1, #0x5f79
006bb19c mov      r0, r8
006bb1a0 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb1a4 mov      r0, r7
006bb1a8 mov      r1, r8
006bb1ac bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb1b0 ldr      r0, [sp, #0x150]
006bb1b4 add      r8, r8, #4
006bb1b8 cmp      r0, r8
006bb1bc beq      #0x6bb1cc
006bb1c0 cmp      r0, #0
006bb1c4 beq      #0x6bb1cc
006bb1c8 bl       #0x310450 ; _Z10GlitchFreePv
006bb1cc ldr      r2, [pc, #0x328]
006bb1d0 add      r8, sp, #0x11c
006bb1d4 movw     r1, #0x6873
006bb1d8 add      r2, pc, r2
006bb1dc movt     r1, #0x7764
006bb1e0 mov      r0, r8
006bb1e4 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb1e8 mov      r0, r7
006bb1ec mov      r1, r8
006bb1f0 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb1f4 ldr      r0, [sp, #0x134]
006bb1f8 add      r8, r8, #4
006bb1fc cmp      r0, r8
006bb200 beq      #0x6bb210
006bb204 cmp      r0, #0
006bb208 beq      #0x6bb210
006bb20c bl       #0x310450 ; _Z10GlitchFreePv
006bb210 ldr      r2, [pc, #0x2e8]
006bb214 add      r8, sp, #0x100
006bb218 movw     r1, #0x656d
006bb21c add      r2, pc, r2
006bb220 movt     r1, #0x6873
006bb224 mov      r0, r8
006bb228 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb22c mov      r0, r7
006bb230 mov      r1, r8
006bb234 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb238 ldr      r0, [sp, #0x118]
006bb23c add      r8, r8, #4
006bb240 cmp      r0, r8
006bb244 beq      #0x6bb254
006bb248 cmp      r0, #0
006bb24c beq      #0x6bb254
006bb250 bl       #0x310450 ; _Z10GlitchFreePv
006bb254 ldr      r2, [pc, #0x2a8]
006bb258 add      r8, sp, #0xe4
006bb25c movw     r1, #0x676c
006bb260 add      r2, pc, r2
006bb264 movt     r1, #0x7468
006bb268 mov      r0, r8
006bb26c bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb270 mov      r0, r7
006bb274 mov      r1, r8
006bb278 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb27c ldr      r0, [sp, #0xfc]
006bb280 add      r8, r8, #4
006bb284 cmp      r0, r8
006bb288 beq      #0x6bb298
006bb28c cmp      r0, #0
006bb290 beq      #0x6bb298
006bb294 bl       #0x310450 ; _Z10GlitchFreePv
006bb298 ldr      r2, [pc, #0x268]
006bb29c add      r8, sp, #0xc8
006bb2a0 movw     r1, #0x6d65
006bb2a4 add      r2, pc, r2
006bb2a8 movt     r1, #0x7974
006bb2ac mov      r0, r8
006bb2b0 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb2b4 mov      r0, r7
006bb2b8 mov      r1, r8
006bb2bc bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb2c0 ldr      r0, [sp, #0xe0]
006bb2c4 add      r8, r8, #4
006bb2c8 cmp      r0, r8
006bb2cc beq      #0x6bb2dc
006bb2d0 cmp      r0, #0
006bb2d4 beq      #0x6bb2dc
006bb2d8 bl       #0x310450 ; _Z10GlitchFreePv
006bb2dc ldr      r2, [pc, #0x228]
006bb2e0 add      r8, sp, #0xac
006bb2e4 movw     r1, #0x6d64
006bb2e8 add      r2, pc, r2
006bb2ec movt     r1, #0x796d
006bb2f0 mov      r0, r8
006bb2f4 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb2f8 mov      r0, r7
006bb2fc mov      r1, r8
006bb300 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb304 ldr      r0, [sp, #0xc4]
006bb308 add      r8, r8, #4
006bb30c cmp      r0, r8
006bb310 beq      #0x6bb320
006bb314 cmp      r0, #0
006bb318 beq      #0x6bb320
006bb31c bl       #0x310450 ; _Z10GlitchFreePv
006bb320 ldr      r2, [pc, #0x1e8]
006bb324 add      r8, sp, #0x90
006bb328 movw     r1, #0x6163
006bb32c add      r2, pc, r2
006bb330 movt     r1, #0x5f6d
006bb334 mov      r0, r8
006bb338 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb33c mov      r0, r7
006bb340 mov      r1, r8
006bb344 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb348 ldr      r0, [sp, #0xa8]
006bb34c add      r8, r8, #4
006bb350 cmp      r0, r8
006bb354 beq      #0x6bb364
006bb358 cmp      r0, #0
006bb35c beq      #0x6bb364
006bb360 bl       #0x310450 ; _Z10GlitchFreePv
006bb364 ldr      r2, [pc, #0x1a8]
006bb368 add      r8, sp, #0x74
006bb36c movw     r1, #0x6962
006bb370 add      r2, pc, r2
006bb374 movt     r1, #0x6c6c
006bb378 mov      r0, r8
006bb37c bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb380 mov      r0, r7
006bb384 mov      r1, r8
006bb388 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb38c ldr      r0, [sp, #0x8c]
006bb390 add      r8, r8, #4
006bb394 cmp      r0, r8
006bb398 beq      #0x6bb3a8
006bb39c cmp      r0, #0
006bb3a0 beq      #0x6bb3a8
006bb3a4 bl       #0x310450 ; _Z10GlitchFreePv
006bb3a8 ldr      r2, [pc, #0x168]
006bb3ac add      r8, sp, #0x58
006bb3b0 movw     r1, #0x6d61
006bb3b4 add      r2, pc, r2
006bb3b8 movt     r1, #0x6873
006bb3bc mov      r0, r8
006bb3c0 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb3c4 mov      r0, r7
006bb3c8 mov      r1, r8
006bb3cc bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb3d0 ldr      r0, [sp, #0x70]
006bb3d4 add      r8, r8, #4
006bb3d8 cmp      r0, r8
006bb3dc beq      #0x6bb3ec
006bb3e0 cmp      r0, #0
006bb3e4 beq      #0x6bb3ec
006bb3e8 bl       #0x310450 ; _Z10GlitchFreePv
006bb3ec ldr      r2, [pc, #0x128]
006bb3f0 add      r8, sp, #0x3c
006bb3f4 movw     r1, #0x7470
006bb3f8 add      r2, pc, r2
006bb3fc movt     r1, #0x6c63
006bb400 mov      r0, r8
006bb404 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb408 mov      r0, r7
006bb40c mov      r1, r8
006bb410 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb414 ldr      r0, [sp, #0x54]
006bb418 add      r8, r8, #4
006bb41c cmp      r0, r8
006bb420 beq      #0x6bb430
006bb424 cmp      r0, #0
006bb428 beq      #0x6bb430
006bb42c bl       #0x310450 ; _Z10GlitchFreePv
006bb430 ldr      r2, [pc, #0xe8]
006bb434 add      r8, sp, #0x20
006bb438 movw     r1, #0x6163
006bb43c add      r2, pc, r2
006bb440 movt     r1, #0x4d6d
006bb444 mov      r0, r8
006bb448 bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb44c mov      r0, r7
006bb450 mov      r1, r8
006bb454 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb458 ldr      r0, [sp, #0x38]
006bb45c add      r8, r8, #4
006bb460 cmp      r0, r8
006bb464 beq      #0x6bb474
006bb468 cmp      r0, #0
006bb46c beq      #0x6bb474
006bb470 bl       #0x310450 ; _Z10GlitchFreePv
006bb474 ldr      r2, [pc, #0xa8]
006bb478 add      r8, sp, #4
006bb47c movw     r1, #0x6163
006bb480 add      r2, pc, r2
006bb484 movt     r1, #0x466d
006bb488 mov      r0, r8
006bb48c bl       #0x6ba774 ; _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
006bb490 mov      r0, r7
006bb494 mov      r1, r8
006bb498 bl       #0x6ba990 ; _ZNSt6vectorIN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairENS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS3_
006bb49c ldr      r0, [sp, #0x1c]
006bb4a0 add      r8, r8, #4
006bb4a4 cmp      r0, r8
006bb4a8 beq      #0x6bb4b8
006bb4ac cmp      r0, #0
006bb4b0 beq      #0x6bb4b8
006bb4b4 bl       #0x310450 ; _Z10GlitchFreePv
006bb4b8 ldr      r3, [r5, r6]
006bb4bc ldr      r2, [sp, #0x1c4]
006bb4c0 mov      r0, r4
006bb4c4 ldr      r3, [r3]
006bb4c8 cmp      r2, r3
006bb4cc bne      #0x6bb4d8
006bb4d0 add      sp, sp, #0x1c8
006bb4d4 pop      {r4, r5, r6, r7, r8, pc}
006bb4d8 bl       #0x30e310
006bb4dc eoreq    sb, sp, r8, ror sl
006bb4e0 ldrdeq   r4, r5, [r0], -r8
006bb4e4 andeq    r4, r0, ip, lsr #1
006bb4e8 eoreq    r0, r3, r8, lsr r2
006bb4ec eoreq    r4, r2, r0, ror r2
006bb4f0 eoreq    sp, r0, r4, lsl #24
006bb4f4 eoreq    r0, r3, r8, ror r1
006bb4f8 eoreq    r0, r3, ip, lsr r1
006bb4fc eoreq    r0, r3, r0, lsl #2
006bb500 eoreq    r0, r3, ip, asr #1
_ZN6glitch5scene24CDefaultSceneNodeFactory12addSceneNodeENS0_17E_SCENE_NODE_TYPEEPNS0_10ISceneNodeE
006b9f30 push     {r4, r5, r6, r7, r8, sb, sl, lr}
006b9f34 movw     r3, #0x7470
006b9f38 movt     r3, #0x6c63
006b9f3c cmp      r1, r3
006b9f40 sub      sp, sp, #0x1a0
006b9f44 mov      r6, r0
006b9f48 mov      r5, r2
006b9f4c beq      #0x6ba4ac
006b9f50 ble      #0x6b9fa8
006b9f54 movw     r3, #0x676c
006b9f58 movt     r3, #0x7468
006b9f5c cmp      r1, r3
006b9f60 beq      #0x6ba2c8
006b9f64 ble      #0x6ba080
006b9f68 movw     r3, #0x6d64
006b9f6c movt     r3, #0x796d
006b9f70 cmp      r1, r3
006b9f74 beq      #0x6ba528
006b9f78 movw     r3, #0x6d65
006b9f7c movt     r3, #0x7974
006b9f80 cmp      r1, r3
006b9f84 beq      #0x6ba50c
006b9f88 movw     r3, #0x6574
006b9f8c movt     r3, #0x7478
006b9f90 cmp      r1, r3
006b9f94 beq      #0x6ba250
006b9f98 mov      r4, #0
006b9f9c mov      r0, r4
006b9fa0 add      sp, sp, #0x1a0
006b9fa4 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
006b9fa8 movw     r3, #0x6b73
006b9fac movt     r3, #0x5f79
006b9fb0 cmp      r1, r3
006b9fb4 beq      #0x6ba3dc
006b9fb8 ble      #0x6ba16c
006b9fbc movw     r3, #0x656d
006b9fc0 movt     r3, #0x6873
006b9fc4 cmp      r1, r3
006b9fc8 beq      #0x6ba360
006b9fcc movw     r3, #0x6d61
006b9fd0 movt     r3, #0x6873
006b9fd4 cmp      r1, r3
006b9fd8 beq      #0x6ba2e4
006b9fdc movw     r3, #0x7563
006b9fe0 movt     r3, #0x6562
006b9fe4 cmp      r1, r3
006b9fe8 bne      #0x6b9f98
006b9fec ldr      r3, [r0, #0x14]
006b9ff0 mov      r4, #0x3f800000
006b9ff4 add      r6, sp, #0x19c
006b9ff8 mov      r1, #0x60000
006b9ffc ldr      r2, [r3, #0x14]
006ba000 add      r1, r1, #3
006ba004 mov      r3, r4
006ba008 mov      r0, r6
006ba00c bl       #0x6d8cb4 ; _ZN6glitch5scene14createCubeMeshEjPNS_5video12IVideoDriverEf
006ba010 mov      r3, #0
006ba014 mov      r1, #0
006ba018 mov      r0, #0x140
006ba01c str      r3, [sp, #0x60]
006ba020 str      r4, [sp, #0x154]
006ba024 str      r3, [sp, #0x158]
006ba028 str      r3, [sp, #0x15c]
006ba02c str      r3, [sp, #0x160]
006ba030 str      r3, [sp, #0x58]
006ba034 str      r3, [sp, #0x5c]
006ba038 str      r4, [sp, #0x64]
006ba03c str      r4, [sp, #0x14c]
006ba040 str      r4, [sp, #0x150]
006ba044 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba048 add      ip, sp, #0x58
006ba04c str      ip, [sp]
006ba050 mov      r1, r6
006ba054 add      ip, sp, #0x14c
006ba058 mvn      r2, #0
006ba05c add      r3, sp, #0x158
006ba060 mov      r4, r0
006ba064 str      ip, [sp, #4]
006ba068 bl       #0x585118 ; _ZN6glitch5scene14CMeshSceneNodeC1ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS8_10quaternionESC_
006ba06c ldr      r0, [sp, #0x19c]
006ba070 cmp      r0, #0
006ba074 beq      #0x6ba138
006ba078 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba07c b        #0x6ba138
006ba080 movw     r3, #0x7073
006ba084 movt     r3, #0x7268
006ba088 cmp      r1, r3
006ba08c beq      #0x6ba5bc
006ba090 movw     r3, #0x6574
006ba094 movt     r3, #0x7272
006ba098 cmp      r1, r3
006ba09c beq      #0x6ba544
006ba0a0 movw     r3, #0x6962
006ba0a4 movt     r3, #0x6c6c
006ba0a8 cmp      r1, r3
006ba0ac bne      #0x6b9f98
006ba0b0 ldr      r2, [r0, #0x14]
006ba0b4 mvn      r6, #0
006ba0b8 mov      r3, #0
006ba0bc mov      r1, #0
006ba0c0 mov      r0, #0x1f4
006ba0c4 ldr      r7, [r2, #0x14]
006ba0c8 str      r3, [sp, #0x168]
006ba0cc str      r3, [sp, #0xa4]
006ba0d0 str      r3, [sp, #0xa8]
006ba0d4 str      r3, [sp, #0xac]
006ba0d8 str      r3, [sp, #0x164]
006ba0dc strb     r6, [sp, #0x174]
006ba0e0 strb     r6, [sp, #0x175]
006ba0e4 strb     r6, [sp, #0x176]
006ba0e8 strb     r6, [sp, #0x177]
006ba0ec strb     r6, [sp, #0x170]
006ba0f0 strb     r6, [sp, #0x171]
006ba0f4 strb     r6, [sp, #0x172]
006ba0f8 strb     r6, [sp, #0x173]
006ba0fc bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba100 add      ip, sp, #0x164
006ba104 str      ip, [sp]
006ba108 ldr      ip, [sp, #0x174]
006ba10c mov      r4, r0
006ba110 mov      r1, r7
006ba114 str      ip, [sp, #4]
006ba118 ldr      ip, [sp, #0x170]
006ba11c mov      r2, r6
006ba120 add      r3, sp, #0xa4
006ba124 str      ip, [sp, #8]
006ba128 bl       #0x5818fc ; _ZN6glitch5scene19CBillboardSceneNodeC1EPNS_5video12IVideoDriverEiRKNS_4core8vector3dIfEERKNS5_11dimension2dIfEENS2_6SColorESE_
006ba12c cmp      r4, #0
006ba130 beq      #0x6b9f98
006ba134 add      r4, r4, #4
006ba138 cmp      r4, #0
006ba13c cmpne    r5, #0
006ba140 beq      #0x6b9f9c
006ba144 mov      r0, r5
006ba148 ldr      r3, [r5]
006ba14c mov      r1, r4
006ba150 mov      lr, pc
006ba154 ldr      pc, [r3, #0x5c]
006ba158 ldr      r3, [r4]
006ba15c ldr      r0, [r3, #-0xc]
006ba160 add      r0, r4, r0
006ba164 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba168 b        #0x6b9f9c
006ba16c movw     r3, #0x6163
006ba170 movt     r3, #0x4d6d
006ba174 cmp      r1, r3
006ba178 beq      #0x6ba6bc
006ba17c movw     r3, #0x6163
006ba180 movt     r3, #0x5f6d
006ba184 cmp      r1, r3
006ba188 beq      #0x6ba660
006ba18c movw     r3, #0x6163
006ba190 movt     r3, #0x466d
006ba194 cmp      r1, r3
006ba198 bne      #0x6b9f98
006ba19c mov      sb, #0x42000000
006ba1a0 mov      r7, #0
006ba1a4 add      sb, sb, #0xc80000
006ba1a8 mov      r1, #0
006ba1ac mov      r0, #0x38c
006ba1b0 mov      r8, r1
006ba1b4 str      r7, [sp, #0xbc]
006ba1b8 str      r7, [sp, #0xc0]
006ba1bc str      r7, [sp, #0xc4]
006ba1c0 str      r7, [sp, #0xb0]
006ba1c4 str      r7, [sp, #0xb4]
006ba1c8 str      sb, [sp, #0xb8]
006ba1cc bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba1d0 add      r2, sp, #0xbc
006ba1d4 add      r3, sp, #0xb0
006ba1d8 mvn      r1, #0
006ba1dc mov      r4, r0
006ba1e0 str      r8, [sp]
006ba1e4 bl       #0x583734 ; _ZN6glitch5scene16CCameraSceneNodeC1EiRKNS_4core8vector3dIfEES6_b
006ba1e8 mov      r1, r8
006ba1ec mov      r0, #0x64
006ba1f0 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba1f4 mov      r3, #0x43000000
006ba1f8 ldr      r1, [r6, #0x18]
006ba1fc mov      sl, r0
006ba200 mov      r2, sb
006ba204 add      r3, r3, #0xfa0000
006ba208 str      r7, [sp]
006ba20c str      r8, [sp, #0xc]
006ba210 str      r8, [sp, #4]
006ba214 str      r8, [sp, #8]
006ba218 bl       #0x6c8f10 ; _ZN6glitch5scene27CSceneNodeAnimatorCameraFPSC1EPNS_3gui14ICursorControlEfffPNS_7SKeyMapEjb
006ba21c mov      r1, sl
006ba220 mov      r0, r4
006ba224 ldr      r3, [r4]
006ba228 mov      lr, pc
006ba22c ldr      pc, [r3, #0x6c]
006ba230 ldr      r3, [sl]
006ba234 ldr      r0, [r3, #-0xc]
006ba238 add      r0, sl, r0
006ba23c bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba240 ldr      r0, [r6, #0x14]
006ba244 mov      r1, r4
006ba248 bl       #0x5890c0 ; _ZN6glitch5scene13CSceneManager15setActiveCameraEPNS0_16ICameraSceneNodeE
006ba24c b        #0x6ba138
006ba250 ldr      r2, [r0, #0x14]
006ba254 mov      r3, #0
006ba258 mvn      r6, #0
006ba25c ldr      r7, [r2, #0x2c]
006ba260 mov      r1, #0
006ba264 mov      r2, #0x64
006ba268 mov      r0, #0x1a8
006ba26c str      r3, [sp, #0x130]
006ba270 strb     r2, [sp, #0x197]
006ba274 str      r3, [sp, #0x128]
006ba278 str      r3, [sp, #0x12c]
006ba27c strb     r6, [sp, #0x194]
006ba280 strb     r6, [sp, #0x195]
006ba284 strb     r6, [sp, #0x196]
006ba288 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba28c ldr      ip, [pc, #0x4dc]
006ba290 mov      r4, r0
006ba294 add      lr, sp, #0x128
006ba298 add      ip, pc, ip
006ba29c str      ip, [sp, #4]
006ba2a0 ldr      ip, [sp, #0x194]
006ba2a4 mov      r1, r6
006ba2a8 mov      r3, r7
006ba2ac mov      r2, #0
006ba2b0 str      lr, [sp]
006ba2b4 str      ip, [sp, #8]
006ba2b8 bl       #0x6d5ce0 ; _ZN6glitch5scene14CTextSceneNodeC1EiPNS_3gui8IGUIFontEPNS0_22ISceneCollisionManagerERKNS_4core8vector3dIfEEPKwNS_5video6SColorE
006ba2bc cmp      r4, #0
006ba2c0 bne      #0x6ba134
006ba2c4 b        #0x6b9f98
006ba2c8 mov      r1, #0
006ba2cc mov      r0, #0x15c
006ba2d0 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba2d4 mov      r1, #1
006ba2d8 mov      r4, r0
006ba2dc bl       #0x5840f0 ; _ZN6glitch5scene15CLightSceneNodeC1Eb
006ba2e0 b        #0x6ba138
006ba2e4 mov      ip, #0
006ba2e8 mov      r3, #0
006ba2ec mov      r2, #0x3f800000
006ba2f0 mov      r1, ip
006ba2f4 mov      r0, #0x17c
006ba2f8 str      r3, [sp, #0x20]
006ba2fc str      r2, [sp, #0x94]
006ba300 str      ip, [sp, #0x16c]
006ba304 str      r3, [sp, #0x98]
006ba308 str      r3, [sp, #0x9c]
006ba30c str      r3, [sp, #0xa0]
006ba310 str      r3, [sp, #0x18]
006ba314 str      r3, [sp, #0x1c]
006ba318 str      r2, [sp, #0x24]
006ba31c str      r2, [sp, #0x8c]
006ba320 str      r2, [sp, #0x90]
006ba324 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba328 add      ip, sp, #0x18
006ba32c str      ip, [sp]
006ba330 add      r1, sp, #0x16c
006ba334 add      ip, sp, #0x8c
006ba338 mvn      r2, #0
006ba33c add      r3, sp, #0x98
006ba340 mov      r4, r0
006ba344 str      ip, [sp, #4]
006ba348 bl       #0x6f6b7c ; _ZN6glitch5scene22CAnimatedMeshSceneNodeC1ERKN5boost13intrusive_ptrINS0_13IAnimatedMeshEEEiRKNS_4core8vector3dIfEERKNS8_10quaternionESC_
006ba34c ldr      r0, [sp, #0x16c]
006ba350 cmp      r0, #0
006ba354 beq      #0x6ba138
006ba358 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba35c b        #0x6ba138
006ba360 mov      ip, #0
006ba364 mov      r3, #0
006ba368 mov      r2, #0x3f800000
006ba36c mov      r1, ip
006ba370 mov      r0, #0x140
006ba374 str      r3, [sp, #0x30]
006ba378 str      r2, [sp, #0x100]
006ba37c str      ip, [sp, #0x178]
006ba380 str      r3, [sp, #0x104]
006ba384 str      r3, [sp, #0x108]
006ba388 str      r3, [sp, #0x10c]
006ba38c str      r3, [sp, #0x28]
006ba390 str      r3, [sp, #0x2c]
006ba394 str      r2, [sp, #0x34]
006ba398 str      r2, [sp, #0xf8]
006ba39c str      r2, [sp, #0xfc]
006ba3a0 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba3a4 add      ip, sp, #0x28
006ba3a8 str      ip, [sp]
006ba3ac add      r1, sp, #0x178
006ba3b0 add      ip, sp, #0xf8
006ba3b4 mvn      r2, #0
006ba3b8 add      r3, sp, #0x104
006ba3bc mov      r4, r0
006ba3c0 str      ip, [sp, #4]
006ba3c4 bl       #0x585118 ; _ZN6glitch5scene14CMeshSceneNodeC1ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS8_10quaternionESC_
006ba3c8 ldr      r0, [sp, #0x178]
006ba3cc cmp      r0, #0
006ba3d0 beq      #0x6ba138
006ba3d4 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba3d8 b        #0x6ba138
006ba3dc ldr      r2, [r0, #0x14]
006ba3e0 mov      r3, #0
006ba3e4 mov      r1, r3
006ba3e8 mov      r0, #0x174
006ba3ec ldr      r6, [r2, #0x14]
006ba3f0 str      r3, [sp, #0x190]
006ba3f4 str      r3, [sp, #0x18c]
006ba3f8 str      r3, [sp, #0x188]
006ba3fc str      r3, [sp, #0x184]
006ba400 str      r3, [sp, #0x180]
006ba404 str      r3, [sp, #0x17c]
006ba408 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba40c add      ip, sp, #0x188
006ba410 str      ip, [sp]
006ba414 add      ip, sp, #0x184
006ba418 str      ip, [sp, #4]
006ba41c add      ip, sp, #0x180
006ba420 str      ip, [sp, #8]
006ba424 add      ip, sp, #0x17c
006ba428 str      ip, [sp, #0xc]
006ba42c mov      r1, r6
006ba430 mvn      ip, #0
006ba434 add      r2, sp, #0x190
006ba438 add      r3, sp, #0x18c
006ba43c mov      r4, r0
006ba440 str      ip, [sp, #0x10]
006ba444 bl       #0x6cf564 ; _ZN6glitch5scene16CSkyBoxSceneNodeC1EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS2_8ITextureEEESA_SA_SA_SA_SA_i
006ba448 ldr      r0, [sp, #0x17c]
006ba44c cmp      r0, #0
006ba450 beq      #0x6ba458
006ba454 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba458 ldr      r0, [sp, #0x180]
006ba45c cmp      r0, #0
006ba460 beq      #0x6ba468
006ba464 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba468 ldr      r0, [sp, #0x184]
006ba46c cmp      r0, #0
006ba470 beq      #0x6ba478
006ba474 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba478 ldr      r0, [sp, #0x188]
006ba47c cmp      r0, #0
006ba480 beq      #0x6ba488
006ba484 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba488 ldr      r0, [sp, #0x18c]
006ba48c cmp      r0, #0
006ba490 beq      #0x6ba498
006ba494 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba498 ldr      r0, [sp, #0x190]
006ba49c cmp      r0, #0
006ba4a0 beq      #0x6ba138
006ba4a4 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba4a8 b        #0x6ba138
006ba4ac mov      r3, #0
006ba4b0 mov      r2, #0x3f800000
006ba4b4 mov      r1, #0
006ba4b8 mov      r0, #0x190
006ba4bc str      r3, [sp, #0x7c]
006ba4c0 str      r2, [sp, #0x70]
006ba4c4 str      r3, [sp, #0x80]
006ba4c8 str      r3, [sp, #0x84]
006ba4cc str      r3, [sp, #0x88]
006ba4d0 str      r3, [sp, #0x74]
006ba4d4 str      r3, [sp, #0x78]
006ba4d8 str      r2, [sp, #0x68]
006ba4dc str      r2, [sp, #0x6c]
006ba4e0 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba4e4 add      ip, sp, #0x74
006ba4e8 str      ip, [sp]
006ba4ec mov      r1, #1
006ba4f0 add      ip, sp, #0x68
006ba4f4 mvn      r2, #0
006ba4f8 add      r3, sp, #0x80
006ba4fc mov      r4, r0
006ba500 str      ip, [sp, #4]
006ba504 bl       #0x6c2478 ; _ZN6glitch5scene24CParticleSystemSceneNodeC1EbiRKNS_4core8vector3dIfEES6_S6_
006ba508 b        #0x6ba138
006ba50c mov      r1, #0
006ba510 mov      r0, #0x150
006ba514 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba518 mvn      r1, #0
006ba51c mov      r4, r0
006ba520 bl       #0x5839d8 ; _ZN6glitch5scene15CEmptySceneNodeC1Ei
006ba524 b        #0x6ba138
006ba528 mov      r1, #0
006ba52c mov      r0, #0x194
006ba530 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba534 mvn      r1, #0
006ba538 mov      r4, r0
006ba53c bl       #0x6bb77c ; _ZN6glitch5scene29CDummyTransformationSceneNodeC1Ei
006ba540 b        #0x6ba138
006ba544 mov      r3, #0
006ba548 mov      r2, #0x3f800000
006ba54c mov      r1, #0
006ba550 mov      r0, #0x214
006ba554 str      r3, [sp, #0x40]
006ba558 str      r2, [sp, #0x118]
006ba55c str      r3, [sp, #0x11c]
006ba560 str      r3, [sp, #0x120]
006ba564 str      r3, [sp, #0x124]
006ba568 str      r3, [sp, #0x38]
006ba56c str      r3, [sp, #0x3c]
006ba570 str      r2, [sp, #0x44]
006ba574 str      r2, [sp, #0x110]
006ba578 str      r2, [sp, #0x114]
006ba57c bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba580 add      lr, sp, #0x11c
006ba584 str      lr, [sp, #8]
006ba588 add      lr, sp, #0x38
006ba58c mov      ip, #0x11
006ba590 str      lr, [sp, #0xc]
006ba594 add      r1, r6, #0x1c
006ba598 add      lr, sp, #0x110
006ba59c mvn      r2, #0
006ba5a0 mov      r3, #4
006ba5a4 mov      r4, r0
006ba5a8 str      ip, [sp, #4]
006ba5ac str      lr, [sp, #0x10]
006ba5b0 str      ip, [sp]
006ba5b4 bl       #0x6d1b5c ; _ZN6glitch5scene17CTerrainSceneNodeC1ERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEijiNS0_20E_TERRAIN_PATCH_SIZEERKNS_4core8vector3dIfEERKNSA_10quaternionESE_
006ba5b8 b        #0x6ba138
006ba5bc ldr      r3, [r0, #0x14]
006ba5c0 add      r6, sp, #0x198
006ba5c4 mov      r1, #0x60000
006ba5c8 ldr      r2, [r3, #0x14]
006ba5cc mov      r3, #0x40000000
006ba5d0 mov      ip, #0x10
006ba5d4 add      r1, r1, #3
006ba5d8 add      r3, r3, #0xa00000
006ba5dc mov      r0, r6
006ba5e0 str      ip, [sp, #4]
006ba5e4 str      ip, [sp]
006ba5e8 bl       #0x6d7808 ; _ZN6glitch5scene16createSphereMeshEjPNS_5video12IVideoDriverEfjj
006ba5ec mov      r3, #0
006ba5f0 mov      r2, #0x3f800000
006ba5f4 mov      r1, #0
006ba5f8 mov      r0, #0x140
006ba5fc str      r3, [sp, #0x50]
006ba600 str      r2, [sp, #0x13c]
006ba604 str      r3, [sp, #0x140]
006ba608 str      r3, [sp, #0x144]
006ba60c str      r3, [sp, #0x148]
006ba610 str      r3, [sp, #0x48]
006ba614 str      r3, [sp, #0x4c]
006ba618 str      r2, [sp, #0x54]
006ba61c str      r2, [sp, #0x134]
006ba620 str      r2, [sp, #0x138]
006ba624 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba628 add      ip, sp, #0x48
006ba62c str      ip, [sp]
006ba630 mov      r1, r6
006ba634 add      ip, sp, #0x134
006ba638 mvn      r2, #0
006ba63c add      r3, sp, #0x140
006ba640 mov      r4, r0
006ba644 str      ip, [sp, #4]
006ba648 bl       #0x585118 ; _ZN6glitch5scene14CMeshSceneNodeC1ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS8_10quaternionESC_
006ba64c ldr      r0, [sp, #0x198]
006ba650 cmp      r0, #0
006ba654 beq      #0x6ba138
006ba658 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba65c b        #0x6ba138
006ba660 mov      r2, #0x42000000
006ba664 mov      r3, #0
006ba668 add      r2, r2, #0xc80000
006ba66c mov      r1, #0
006ba670 mov      r0, #0x38c
006ba674 str      r3, [sp, #0xe4]
006ba678 str      r2, [sp, #0xe8]
006ba67c str      r3, [sp, #0xec]
006ba680 str      r3, [sp, #0xf0]
006ba684 str      r3, [sp, #0xf4]
006ba688 str      r3, [sp, #0xe0]
006ba68c bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba690 mov      ip, #0
006ba694 mov      r4, r0
006ba698 mvn      r1, #0
006ba69c add      r2, sp, #0xec
006ba6a0 add      r3, sp, #0xe0
006ba6a4 str      ip, [sp]
006ba6a8 bl       #0x583734 ; _ZN6glitch5scene16CCameraSceneNodeC1EiRKNS_4core8vector3dIfEES6_b
006ba6ac ldr      r0, [r6, #0x14]
006ba6b0 mov      r1, r4
006ba6b4 bl       #0x5890c0 ; _ZN6glitch5scene13CSceneManager15setActiveCameraEPNS0_16ICameraSceneNodeE
006ba6b8 b        #0x6ba138
006ba6bc mov      r2, #0x42000000
006ba6c0 mov      r3, #0
006ba6c4 mov      r1, #0
006ba6c8 add      r2, r2, #0xc80000
006ba6cc mov      r0, #0x38c
006ba6d0 mov      r7, r1
006ba6d4 str      r3, [sp, #0xcc]
006ba6d8 str      r2, [sp, #0xd0]
006ba6dc str      r3, [sp, #0xd4]
006ba6e0 str      r3, [sp, #0xd8]
006ba6e4 str      r3, [sp, #0xdc]
006ba6e8 str      r3, [sp, #0xc8]
006ba6ec bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba6f0 add      r2, sp, #0xd4
006ba6f4 add      r3, sp, #0xc8
006ba6f8 mvn      r1, #0
006ba6fc mov      r4, r0
006ba700 str      r7, [sp]
006ba704 bl       #0x583734 ; _ZN6glitch5scene16CCameraSceneNodeC1EiRKNS_4core8vector3dIfEES6_b
006ba708 mov      r1, r7
006ba70c mov      r0, #0x80
006ba710 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
006ba714 movw     r2, #0x8000
006ba718 mov      r3, #0x43000000
006ba71c movw     ip, #0x8000
006ba720 mov      r7, r0
006ba724 movt     ip, #0x44bb
006ba728 ldr      r1, [r6, #0x18]
006ba72c movt     r2, #0xc4bb
006ba730 add      r3, r3, #0x480000
006ba734 str      ip, [sp]
006ba738 bl       #0x6ca338 ; _ZN6glitch5scene28CSceneNodeAnimatorCameraMayaC1EPNS_3gui14ICursorControlEfff
006ba73c mov      r1, r7
006ba740 mov      r0, r4
006ba744 ldr      r3, [r4]
006ba748 mov      lr, pc
006ba74c ldr      pc, [r3, #0x6c]
006ba750 ldr      r3, [r7]
006ba754 ldr      r0, [r3, #-0xc]
006ba758 add      r0, r7, r0
006ba75c bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
006ba760 ldr      r0, [r6, #0x14]
006ba764 mov      r1, r4
006ba768 bl       #0x5890c0 ; _ZN6glitch5scene13CSceneManager15setActiveCameraEPNS0_16ICameraSceneNodeE
006ba76c b        #0x6ba138
006ba770 eoreq    r1, r3, r8
_ZN6glitch5scene16CCameraSceneNodeC1EiRKNS_4core8vector3dIfEES6_b
00583734 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00583738 ldr      r5, [pc, #0x128]
0058373c ldr      ip, [pc, #0x128]
00583740 ldr      lr, [pc, #0x128]
00583744 add      r5, pc, r5
00583748 ldr      ip, [r5, ip]
0058374c ldr      lr, [r5, lr]
00583750 mov      sl, #1
00583754 ldr      r6, [ip, #0x24]
00583758 add      lr, lr, #8
0058375c str      lr, [r0, #0x384]
00583760 str      sl, [r0, #0x388]
00583764 str      r6, [r0]
00583768 ldr      r7, [r6, #-0xc]
0058376c ldr      fp, [ip, #0x28]
00583770 sub      sp, sp, #0x24
00583774 mov      lr, r1
00583778 mov      sb, r2
0058377c add      r1, ip, #4
00583780 add      ip, sp, #0x14
00583784 str      fp, [r0, r7]
00583788 mov      r6, #0
0058378c mov      r8, #0x3f800000
00583790 mov      r2, lr
00583794 mov      r7, r3
00583798 str      ip, [sp]
0058379c mov      r3, sb
005837a0 add      ip, sp, #8
005837a4 mov      r4, r0
005837a8 str      ip, [sp, #4]
005837ac ldrb     sb, [sp, #0x48]
005837b0 str      r6, [sp, #0x14]
005837b4 str      r6, [sp, #0x18]
005837b8 str      r6, [sp, #0x1c]
005837bc str      r8, [sp, #8]
005837c0 str      r8, [sp, #0xc]
005837c4 str      r8, [sp, #0x10]
005837c8 bl       #0x582b18 ; _ZN6glitch5scene16ICameraSceneNodeC2EiRKNS_4core8vector3dIfEES6_S6_
005837cc ldr      r3, [pc, #0xa0]
005837d0 add      r0, r4, #0x168
005837d4 ldr      r3, [r5, r3]
005837d8 add      r2, r3, #0x180
005837dc add      r1, r3, #0x1c
005837e0 add      r3, r3, #0x19c
005837e4 str      r1, [r4]
005837e8 str      r2, [r4, #0x130]
005837ec str      r3, [r4, #0x384]
005837f0 ldr      r3, [r7]
005837f4 str      r3, [r4, #0x138]
005837f8 ldr      r3, [r7, #4]
005837fc str      r3, [r4, #0x13c]
00583800 ldr      r3, [r7, #8]
00583804 str      r6, [r4, #0x14c]
00583808 str      r8, [r4, #0x15c]
0058380c str      r3, [r4, #0x140]
00583810 mov      r3, #0x45000000
00583814 add      r3, r3, #0x3b8000
00583818 str      r3, [r4, #0x160]
0058381c strb     sb, [r4, #0x164]
00583820 strb     sl, [r4, #0x165]
00583824 str      r6, [r4, #0x144]
00583828 str      r8, [r4, #0x148]
0058382c bl       #0x582bd8 ; _ZN6glitch5scene12SViewFrustumC1Ev
00583830 movw     r3, #0xd97c
00583834 movt     r3, #0x3fa0
00583838 str      r3, [r4, #0x154]
0058383c movw     r3, #0xaaab
00583840 movt     r3, #0x3faa
00583844 mov      r0, r4
00583848 str      r3, [r4, #0x158]
0058384c bl       #0x58364c ; _ZN6glitch5scene16CCameraSceneNode27recalculateProjectionMatrixEv
00583850 mov      r0, r4
00583854 mov      r1, #0
00583858 bl       #0x59719c ; _ZN6glitch5scene10ISceneNode19setAutomaticCullingENS0_14E_CULLING_TYPEE
0058385c mov      r0, r4
00583860 add      sp, sp, #0x24
00583864 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00583868 subeq    r1, r1, ip, asr #6
0058386c andeq    r1, r0, r4, asr #1
00583870 andeq    r2, r0, r4, asr #22
00583874 andeq    r0, r0, r8, lsr #28
_ZN14CameraOverviewC1Ev
0041106c push     {r4, r5, r6, r7, lr}
00411070 ldr      r5, [pc, #0x1b0]
00411074 sub      sp, sp, #0x44
00411078 mov      r4, r0
0041107c bl       #0x40e720 ; _ZN10CameraBaseC2Ev
00411080 ldr      r2, [pc, #0x1a4]
00411084 add      r5, pc, r5
00411088 ldr      r6, [pc, #0x1a0]
0041108c ldr      r2, [r5, r2]
00411090 mov      r3, #0
00411094 ldr      r1, [r5, r6]
00411098 add      r0, r2, #0x28
0041109c add      r2, r2, #8
004110a0 str      r0, [r4, #0xc]
004110a4 str      r2, [r4]
004110a8 str      r3, [r4, #0x24]
004110ac str      r3, [r4, #0x10]
004110b0 str      r3, [r4, #0x14]
004110b4 str      r3, [r4, #0x18]
004110b8 str      r3, [r4, #0x1c]
004110bc str      r3, [r4, #0x20]
004110c0 ldr      r3, [r1, #0x10]
004110c4 movw     r1, #0x6163
004110c8 movt     r1, #0x5f6d
004110cc ldr      r2, [r3, #0x1c]
004110d0 ldr      r3, [r2, #0xd0]
004110d4 ldr      r2, [r2, #0xcc]
004110d8 rsb      r3, r2, r3
004110dc asrs     r3, r3, #2
004110e0 ldrne    r3, [r2]
004110e4 mov      r2, #0
004110e8 mov      r0, r3
004110ec ldr      r3, [r3]
004110f0 mov      lr, pc
004110f4 ldr      pc, [r3, #0xc]
004110f8 cmp      r0, #0
004110fc str      r0, [r4, #8]
00411100 str      r0, [r4, #4]
00411104 beq      #0x41121c
00411108 ldr      r2, [r0]
0041110c ldr      r3, [r5, r6]
00411110 mov      r5, #0
00411114 ldr      r2, [r2, #-0xc]
00411118 mov      r6, #0x3f800000
0041111c add      r7, sp, #0xc
00411120 add      r0, r0, r2
00411124 ldr      r2, [r0, #4]
00411128 add      r2, r2, #1
0041112c str      r2, [r0, #4]
00411130 ldr      r3, [r3, #0x10]
00411134 ldr      r1, [r4, #4]
00411138 ldr      r3, [r3, #0x1c]
0041113c ldr      r3, [r3, #4]
00411140 mov      r0, r3
00411144 ldr      r3, [r3]
00411148 mov      lr, pc
0041114c ldr      pc, [r3, #0x5c]
00411150 movw     ip, #0x5000
00411154 movt     ip, #0x46c3
00411158 movw     r2, #0x8e39
0041115c movw     r3, #0x6000
00411160 movw     r1, #0xf877
00411164 movt     r2, #0x3fe3
00411168 movt     r3, #0x466a
0041116c mov      r0, r4
00411170 movt     r1, #0x3edb
00411174 str      ip, [sp]
00411178 mov      ip, #1
0041117c str      ip, [sp, #4]
00411180 bl       #0x40e9a8 ; _ZN10CameraBase7SetDataEffffb
00411184 ldr      r0, [r4, #8]
00411188 add      r1, sp, #0x34
0041118c ldr      r3, [r0]
00411190 ldr      r3, [r3, #0x114]
00411194 str      r5, [sp, #0x34]
00411198 str      r6, [sp, #0x38]
0041119c str      r5, [sp, #0x3c]
004111a0 blx      r3
004111a4 ldr      r0, [r4, #8]
004111a8 add      r1, sp, #0x28
004111ac ldr      r3, [r0]
004111b0 ldr      r3, [r3, #0x104]
004111b4 str      r5, [sp, #0x28]
004111b8 str      r5, [sp, #0x2c]
004111bc str      r5, [sp, #0x30]
004111c0 blx      r3
004111c4 add      r1, sp, #0x1c
004111c8 mov      r0, r7
004111cc str      r6, [sp, #0x20]
004111d0 str      r5, [sp, #0xc]
004111d4 str      r5, [sp, #0x10]
004111d8 str      r5, [sp, #0x14]
004111dc str      r6, [sp, #0x18]
004111e0 str      r5, [sp, #0x1c]
004111e4 str      r5, [sp, #0x24]
004111e8 bl       #0x410e48 ; _ZN6glitch4core10quaternion13fromAngleAxisEfRKNS0_8vector3dIfEE.clone.1
004111ec ldr      r3, [r4, #4]
004111f0 mov      r1, r7
004111f4 mov      r0, r3
004111f8 ldr      r3, [r3]
004111fc mov      lr, pc
00411200 ldr      pc, [r3, #0x9c]
00411204 movw     r3, #0x4000
00411208 mov      r1, r5
0041120c ldr      r0, [r4, #4]
00411210 mov      r2, r5
00411214 movt     r3, #0x469c
00411218 bl       #0x597154 ; _ZN6glitch5scene10ISceneNode11setPositionEfff
0041121c mov      r0, r4
00411220 add      sp, sp, #0x44
00411224 pop      {r4, r5, r6, r7, pc}
00411228 subseq   r3, r8, ip, lsl #20
0041122c andeq    r3, r0, r8, lsl #22
00411230 strdeq   r3, r4, [r0], -r4
_ZN10CameraBase7SetDataEffffb
0040e9a8 push     {r4, r5, r6, lr}
0040e9ac ldr      ip, [r0, #8]
0040e9b0 sub      sp, sp, #0x10
0040e9b4 mov      r4, r0
0040e9b8 cmp      ip, #0
0040e9bc mov      r5, r2
0040e9c0 mov      r6, r3
0040e9c4 beq      #0x40ea48
0040e9c8 mov      r0, ip
0040e9cc ldr      r3, [ip]
0040e9d0 mov      lr, pc
0040e9d4 ldr      pc, [r3, #0x13c]
0040e9d8 ldr      r3, [r4, #8]
0040e9dc mov      r1, r5
0040e9e0 mov      r0, r3
0040e9e4 ldr      r3, [r3]
0040e9e8 mov      lr, pc
0040e9ec ldr      pc, [r3, #0x138]
0040e9f0 ldr      r3, [r4, #8]
0040e9f4 mov      r1, r6
0040e9f8 mov      r0, r3
0040e9fc ldr      r3, [r3]
0040ea00 mov      lr, pc
0040ea04 ldr      pc, [r3, #0x130]
0040ea08 ldr      r3, [r4, #8]
0040ea0c ldr      r1, [sp, #0x20]
0040ea10 mov      r0, r3
0040ea14 ldr      r3, [r3]
0040ea18 mov      lr, pc
0040ea1c ldr      pc, [r3, #0x134]
0040ea20 ldr      r0, [r4, #8]
0040ea24 mov      r2, #0
0040ea28 mov      ip, #0x3f800000
0040ea2c ldr      r3, [r0]
0040ea30 add      r1, sp, #4
0040ea34 ldr      r3, [r3, #0x114]
0040ea38 str      r2, [sp, #8]
0040ea3c str      ip, [sp, #0xc]
0040ea40 str      r2, [sp, #4]
0040ea44 blx      r3
0040ea48 add      sp, sp, #0x10
0040ea4c pop      {r4, r5, r6, pc}
_ZN6glitch4core10quaternion13fromAngleAxisEfRKNS0_8vector3dIfEE.clone.1
00410e48 push     {r4, r5, r6, lr}
00410e4c movw     r3, #0x7b80
00410e50 movt     r3, #0x3f06
00410e54 str      r3, [r0, #0xc]
00410e58 mov      r4, r0
00410e5c mov      r5, r1
00410e60 ldr      r0, [r1]
00410e64 movw     r1, #0xd4d0
00410e68 movt     r1, #0x3f59
00410e6c bl       #0x30ed6c
00410e70 movw     r1, #0xd4d0
00410e74 str      r0, [r4]
00410e78 ldr      r0, [r5, #4]
00410e7c movt     r1, #0x3f59
00410e80 bl       #0x30ed6c
00410e84 movw     r1, #0xd4d0
00410e88 str      r0, [r4, #4]
00410e8c ldr      r0, [r5, #8]
00410e90 movt     r1, #0x3f59
00410e94 bl       #0x30ed6c
00410e98 str      r0, [r4, #8]
00410e9c mov      r0, r4
00410ea0 pop      {r4, r5, r6, pc}
