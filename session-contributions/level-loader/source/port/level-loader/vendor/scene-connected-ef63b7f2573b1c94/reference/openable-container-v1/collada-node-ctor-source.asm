_ZN6glitch7collada10CSceneNodeC2ERKNS0_16CColladaDatabaseEPNS0_5SNodeE
0065d2b4 push     {r4, r5, r6, r7, lr}
0065d2b8 mov      r5, r2
0065d2bc sub      sp, sp, #0x2c
0065d2c0 mvn      r2, #0
0065d2c4 mov      r4, r1
0065d2c8 add      r1, r1, #4
0065d2cc mov      r6, r0
0065d2d0 mov      r7, r3
0065d2d4 bl       #0x583b34 ; _ZN6glitch5scene15CEmptySceneNodeC2Ei
0065d2d8 ldr      r2, [r5]
0065d2dc ldr      r3, [pc, #0xfc]
0065d2e0 str      r2, [r6, #0x14c]
0065d2e4 ldr      r1, [r5, #4]
0065d2e8 cmp      r2, #0
0065d2ec add      r3, pc, r3
0065d2f0 str      r1, [r6, #0x150]
0065d2f4 beq      #0x65d308
0065d2f8 ldr      r1, [r2, #4]
0065d2fc cmp      r1, #0
0065d300 addne    r1, r1, #1
0065d304 strne    r1, [r2, #4]
0065d308 ldr      r2, [pc, #0xd4]
0065d30c cmp      r7, #0
0065d310 ldr      r2, [r3, r2]
0065d314 add      r2, r2, #4
0065d318 str      r2, [r6, #0x148]
0065d31c ldr      r3, [r4]
0065d320 str      r3, [r6]
0065d324 ldr      r3, [r3, #-0x1c]
0065d328 ldr      r2, [r4, #0x1c]
0065d32c str      r2, [r6, r3]
0065d330 ldr      r3, [r6]
0065d334 ldr      r2, [r4, #0x20]
0065d338 ldr      r3, [r3, #-0xc]
0065d33c str      r2, [r6, r3]
0065d340 str      r7, [r6, #0x154]
0065d344 beq      #0x65d3d4
0065d348 ldr      r1, [r7, #4]
0065d34c mov      r0, r6
0065d350 bl       #0x598a04 ; _ZN6glitch5scene10ISceneNode7setNameEPKc
0065d354 ldr      r3, [r6, #0x154]
0065d358 mov      r0, r6
0065d35c add      r1, sp, #0x1c
0065d360 ldr      r2, [r3, #0xc]
0065d364 str      r2, [sp, #0x1c]
0065d368 ldr      r2, [r3, #0x10]
0065d36c str      r2, [sp, #0x20]
0065d370 ldr      r3, [r3, #0x14]
0065d374 str      r3, [sp, #0x24]
0065d378 bl       #0x59712c ; _ZN6glitch5scene10ISceneNode11setPositionERKNS_4core8vector3dIfEE
0065d37c ldr      r3, [r6, #0x154]
0065d380 mov      r0, r6
0065d384 mov      r1, sp
0065d388 ldr      r2, [r3, #0x18]
0065d38c str      r2, [sp]
0065d390 ldr      r2, [r3, #0x1c]
0065d394 str      r2, [sp, #4]
0065d398 ldr      r2, [r3, #0x20]
0065d39c str      r2, [sp, #8]
0065d3a0 ldr      r3, [r3, #0x24]
0065d3a4 str      r3, [sp, #0xc]
0065d3a8 bl       #0x5970f4 ; _ZN6glitch5scene10ISceneNode11setRotationERKNS_4core10quaternionE
0065d3ac ldr      r3, [r6, #0x154]
0065d3b0 mov      r0, r6
0065d3b4 add      r1, sp, #0x10
0065d3b8 ldr      r2, [r3, #0x28]
0065d3bc str      r2, [sp, #0x10]
0065d3c0 ldr      r2, [r3, #0x2c]
0065d3c4 str      r2, [sp, #0x14]
0065d3c8 ldr      r3, [r3, #0x30]
0065d3cc str      r3, [sp, #0x18]
0065d3d0 bl       #0x5970c4 ; _ZN6glitch5scene10ISceneNode8setScaleERKNS_4core8vector3dIfEE
0065d3d4 mov      r0, r6
0065d3d8 add      sp, sp, #0x2c
0065d3dc pop      {r4, r5, r6, r7, pc}
0065d3e0 eorseq   r7, r3, r4, lsr #15
0065d3e4 strheq   r1, [r0], -r4
