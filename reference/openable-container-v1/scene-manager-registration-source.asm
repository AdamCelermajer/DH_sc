_ZN12SceneManager19_registerSceneNodesEPN6glitch5scene10ISceneNodeE
00357ea4 push     {r4, r5, r6, lr}
00357ea8 mov      r4, r0
00357eac ldr      r0, [r0, #0x440]
00357eb0 ldr      r2, [pc, #0x13c]
00357eb4 ldr      r3, [pc, #0x13c]
00357eb8 add      r0, r0, #1
00357ebc str      r0, [r4, #0x440]
00357ec0 add      r2, pc, r2
00357ec4 ldrb     r2, [r2, #0xc]
00357ec8 add      r3, pc, r3
00357ecc mov      r6, r1
00357ed0 cmp      r2, #0
00357ed4 beq      #0x357fd8
00357ed8 ldr      r2, [pc, #0x11c]
00357edc ldr      r2, [r3, r2]
00357ee0 mov      r1, #0
00357ee4 str      r1, [r4, #0x440]
00357ee8 ldrb     r2, [r2, #0x30]
00357eec cmp      r2, r1
00357ef0 movne    r1, #1
00357ef4 beq      #0x357fec
00357ef8 ldr      r2, [pc, #0x100]
00357efc add      r2, pc, r2
00357f00 strb     r1, [r2, #0xc]
00357f04 ldr      r2, [pc, #0xf8]
00357f08 ldr      r0, [r3, r2]
00357f0c bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
00357f10 ldrb     r3, [r4, #0x289]
00357f14 mov      r5, r0
00357f18 cmp      r3, #0
00357f1c bne      #0x357fa0
00357f20 ldr      r0, [r4, #0x440]
00357f24 ldr      r1, [r4, #0x444]
00357f28 bl       #0x30e904
00357f2c cmp      r1, #0
00357f30 beq      #0x357fa0
00357f34 cmp      r5, #0
00357f38 beq      #0x357f58
00357f3c ldr      r3, [r5, #0x158]
00357f40 cmp      r3, #0
00357f44 beq      #0x357f58
00357f48 ldr      r0, [r3, #0x34]
00357f4c cmp      r0, #0
00357f50 beq      #0x357f58
00357f54 bl       #0x50d71c ; _ZN5batch16GOBatchSceneNode23updateSegmentVisibilityEv
00357f58 ldr      r3, [r4, #0x47c]
00357f5c ldr      r6, [r4, #0x480]
00357f60 rsb      r6, r3, r6
00357f64 asr      r6, r6, #2
00357f68 cmp      r6, #0
00357f6c ble      #0x357f9c
00357f70 mov      r5, #0
00357f74 b        #0x357f7c
00357f78 ldr      r3, [r4, #0x47c]
00357f7c ldr      r3, [r3, r5, lsl #2]
00357f80 add      r5, r5, #1
00357f84 mov      r0, r3
00357f88 ldr      r3, [r3]
00357f8c mov      lr, pc
00357f90 ldr      pc, [r3]
00357f94 cmp      r5, r6
00357f98 bne      #0x357f78
00357f9c pop      {r4, r5, r6, pc}
00357fa0 mov      r0, r4
00357fa4 bl       #0x357dc0 ; _ZN12SceneManager16clearRenderListsEv
00357fa8 ldr      r3, [r4, #0x47c]
00357fac ldr      r2, [r4, #0x480]
00357fb0 mov      r1, r6
00357fb4 mov      r0, r4
00357fb8 cmp      r3, r2
00357fbc strne    r3, [r4, #0x480]
00357fc0 bl       #0x58b88c ; _ZN6glitch5scene13CSceneManager18registerSceneNodesEPNS0_10ISceneNodeE
00357fc4 mov      r3, #0
00357fc8 strb     r3, [r4, #0x448]
00357fcc str      r3, [r4, #0x440]
00357fd0 strb     r3, [r4, #0x289]
00357fd4 pop      {r4, r5, r6, pc}
00357fd8 ldr      r2, [pc, #0x1c]
00357fdc ldr      r1, [r3, r2]
00357fe0 ldrb     r1, [r1, #0x30]
00357fe4 cmp      r1, #0
00357fe8 bne      #0x357edc
00357fec ldrb     r1, [r4, #0x448]
00357ff0 b        #0x357ef8
00357ff4 rsbeq    sl, r4, r8, lsr #32
00357ff8 rsbeq    ip, r3, r8, asr #23
00357ffc andeq    r1, r0, r0, lsr #20
00358000 rsbeq    sb, r4, ip, ror #31
00358004 strdeq   r3, r4, [r0], -r4
_ZN12SceneManagerC1EPN6glitch5video12IVideoDriverEN5boost13intrusive_ptrINS0_2io11IFileSystemEEEPNS0_3gui14ICursorControlEPNS9_15IGUIEnvironmentE
00352c3c push     {r4, r5, r6, r7, r8, lr}
00352c40 ldr      r5, [pc, #0xf4]
00352c44 ldr      ip, [pc, #0xf4]
00352c48 ldr      lr, [pc, #0xf4]
00352c4c add      r5, pc, r5
00352c50 ldr      ip, [r5, ip]
00352c54 ldr      lr, [r5, lr]
00352c58 mov      r7, #1
00352c5c ldr      r6, [ip, #0x18]
00352c60 add      lr, lr, #8
00352c64 str      lr, [r0, #0x48c]
00352c68 str      r6, [r0]
00352c6c str      r7, [r0, #0x490]
00352c70 ldr      r7, [r6, #-0xc]
00352c74 ldr      r8, [ip, #0x1c]
00352c78 sub      sp, sp, #0x10
00352c7c mov      r6, r1
00352c80 str      r8, [r0, r7]
00352c84 add      r1, ip, #4
00352c88 ldr      ip, [sp, #0x28]
00352c8c mov      lr, r2
00352c90 str      r3, [sp]
00352c94 mov      r2, r6
00352c98 mov      r3, lr
00352c9c mov      r6, #0
00352ca0 mov      r4, r0
00352ca4 stmib    sp, {r6, ip}
00352ca8 bl       #0x58ddd0 ; _ZN6glitch5scene13CSceneManagerC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEPNS_3gui14ICursorControlEPNS0_10IMeshCacheEPNSC_15IGUIEnvironmentE
00352cac ldr      r3, [pc, #0x94]
00352cb0 str      r6, [r4, #0x28c]
00352cb4 strb     r6, [r4, #0x290]
00352cb8 ldr      r3, [r5, r3]
00352cbc add      r0, r4, #0x294
00352cc0 add      r2, r3, #0xc0
00352cc4 add      r3, r3, #0x1c
00352cc8 str      r3, [r4]
00352ccc str      r2, [r4, #0x48c]
00352cd0 bl       #0x40d7d4 ; _ZN15LightSetManagerC1Ev
00352cd4 mov      r2, #4
00352cd8 mov      r3, #0
00352cdc str      r2, [r4, #0x444]
00352ce0 mov      r2, #0x3f800000
00352ce4 str      r3, [r4, #0x45c]
00352ce8 str      r2, [r4, #0x460]
00352cec str      r6, [r4, #0x484]
00352cf0 str      r6, [r4, #0x438]
00352cf4 strb     r6, [r4, #0x43c]
00352cf8 str      r6, [r4, #0x440]
00352cfc strb     r6, [r4, #0x448]
00352d00 str      r6, [r4, #0x44c]
00352d04 str      r6, [r4, #0x450]
00352d08 str      r6, [r4, #0x454]
00352d0c str      r3, [r4, #0x458]
00352d10 str      r6, [r4, #0x464]
00352d14 str      r6, [r4, #0x468]
00352d18 str      r6, [r4, #0x46c]
00352d1c str      r6, [r4, #0x470]
00352d20 str      r6, [r4, #0x474]
00352d24 str      r6, [r4, #0x478]
00352d28 str      r6, [r4, #0x47c]
00352d2c str      r6, [r4, #0x480]
00352d30 mov      r0, r4
00352d34 add      sp, sp, #0x10
00352d38 pop      {r4, r5, r6, r7, r8, pc}
00352d3c rsbeq    r1, r4, r4, asr #28
00352d40 andeq    r3, r0, r0, asr #2
00352d44 andeq    r2, r0, r4, asr #22
00352d48 andeq    r4, r0, r0, lsl #4
_ZN12SceneManagerC2EPN6glitch5video12IVideoDriverEN5boost13intrusive_ptrINS0_2io11IFileSystemEEEPNS0_3gui14ICursorControlEPNS9_15IGUIEnvironmentE
00352d4c push     {r4, r5, r6, lr}
00352d50 sub      sp, sp, #0x10
00352d54 ldr      ip, [sp, #0x20]
00352d58 mov      r5, #0
00352d5c mov      r6, r1
00352d60 str      ip, [sp]
00352d64 ldr      ip, [sp, #0x24]
00352d68 add      r1, r1, #4
00352d6c mov      r4, r0
00352d70 stmib    sp, {r5, ip}
00352d74 bl       #0x58ddd0 ; _ZN6glitch5scene13CSceneManagerC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEPNS_3gui14ICursorControlEPNS0_10IMeshCacheEPNSC_15IGUIEnvironmentE
00352d78 ldr      r3, [r6]
00352d7c add      r0, r4, #0x294
00352d80 str      r3, [r4]
00352d84 ldr      r2, [r6, #0x10]
00352d88 ldr      r3, [r3, #-0x1c]
00352d8c str      r2, [r4, r3]
00352d90 ldr      r3, [r4]
00352d94 ldr      r2, [r6, #0x14]
00352d98 ldr      r3, [r3, #-0xc]
00352d9c str      r2, [r4, r3]
00352da0 str      r5, [r4, #0x28c]
00352da4 strb     r5, [r4, #0x290]
00352da8 bl       #0x40d7d4 ; _ZN15LightSetManagerC1Ev
00352dac mov      r2, #4
00352db0 mov      r3, #0
00352db4 str      r2, [r4, #0x444]
00352db8 mov      r2, #0x3f800000
00352dbc str      r3, [r4, #0x45c]
00352dc0 str      r2, [r4, #0x460]
00352dc4 str      r5, [r4, #0x484]
00352dc8 str      r5, [r4, #0x438]
00352dcc strb     r5, [r4, #0x43c]
00352dd0 str      r5, [r4, #0x440]
00352dd4 strb     r5, [r4, #0x448]
00352dd8 str      r5, [r4, #0x44c]
00352ddc str      r5, [r4, #0x450]
00352de0 str      r5, [r4, #0x454]
00352de4 str      r3, [r4, #0x458]
00352de8 str      r5, [r4, #0x464]
00352dec str      r5, [r4, #0x468]
00352df0 str      r5, [r4, #0x46c]
00352df4 str      r5, [r4, #0x470]
00352df8 str      r5, [r4, #0x474]
00352dfc str      r5, [r4, #0x478]
00352e00 str      r5, [r4, #0x47c]
00352e04 str      r5, [r4, #0x480]
00352e08 mov      r0, r4
00352e0c add      sp, sp, #0x10
00352e10 pop      {r4, r5, r6, pc}
_ZN6glitch5scene13CSceneManager18registerSceneNodesERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
00589128 push     {r4, r5, r6, lr}
0058912c ldr      r4, [r1]
00589130 ldr      r3, [r1, #4]
00589134 mov      r6, r1
00589138 mov      r5, r0
0058913c cmp      r4, r3
00589140 beq      #0x589164
00589144 ldr      r1, [r4], #4
00589148 ldr      r3, [r5]
0058914c mov      r0, r5
00589150 mov      lr, pc
00589154 ldr      pc, [r3, #0x28]
00589158 ldr      r3, [r6, #4]
0058915c cmp      r4, r3
00589160 bne      #0x589144
00589164 pop      {r4, r5, r6, pc}
_Z21NodeRemoveAllChildrenPN6glitch5scene10ISceneNodeE
0050e4d0 push     {r4, r5, r6, lr}
0050e4d4 mov      r6, r0
0050e4d8 ldr      r4, [r6, #0xf4]!
0050e4dc mov      r5, r0
0050e4e0 cmp      r4, r6
0050e4e4 beq      #0x50e510
0050e4e8 ldr      r3, [r5]
0050e4ec cmp      r4, #0
0050e4f0 sub      r1, r4, #4
0050e4f4 ldr      r3, [r3, #0x60]
0050e4f8 moveq    r1, r4
0050e4fc mov      r0, r5
0050e500 blx      r3
0050e504 ldr      r4, [r4]
0050e508 cmp      r6, r4
0050e50c bne      #0x50e4e8
0050e510 pop      {r4, r5, r6, pc}
