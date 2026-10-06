_Z19setOnAnimateEnabledPN6glitch5scene10ISceneNodeEb
0050e46c ldr      r3, [r0, #0x11c]
0050e470 cmp      r1, #0
0050e474 orrne    r3, r3, #0x200
0050e478 biceq    r3, r3, #0x200
0050e47c str      r3, [r0, #0x11c]
0050e480 bx       lr
