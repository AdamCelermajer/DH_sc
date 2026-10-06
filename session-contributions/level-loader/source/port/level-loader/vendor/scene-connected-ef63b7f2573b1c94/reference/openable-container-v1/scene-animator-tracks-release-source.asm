_ZN6glitch7collada18CSceneNodeAnimator21removeAnimationTracksEv
0065d628 push     {r4, r5, r6, r7, r8, lr}
0065d62c ldr      r3, [r0, #0x44]
0065d630 ldr      r7, [r0, #0x48]
0065d634 mov      r6, r0
0065d638 rsb      r7, r3, r7
0065d63c asrs     r7, r7, #4
0065d640 beq      #0x65d690
0065d644 mov      r4, #0
0065d648 mov      r8, r4
0065d64c b        #0x65d654
0065d650 ldr      r3, [r6, #0x44]
0065d654 lsl      r5, r4, #4
0065d658 add      r3, r3, r5
0065d65c ldr      r3, [r3, #8]
0065d660 add      r4, r4, #1
0065d664 cmp      r3, #0
0065d668 beq      #0x65d688
0065d66c mov      r0, r3
0065d670 ldr      r3, [r3]
0065d674 mov      lr, pc
0065d678 ldr      pc, [r3, #4]
0065d67c ldr      r3, [r6, #0x44]
0065d680 add      r5, r3, r5
0065d684 str      r8, [r5, #8]
0065d688 cmp      r4, r7
0065d68c bne      #0x65d650
0065d690 pop      {r4, r5, r6, r7, r8, pc}
