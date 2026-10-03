
# _ZN9Character14_F_ResetResultERNS_12AttackResultE
003af3b0: mov      r2, #0
003af3b4: add      r3, r0, #0xc
003af3b8: str      r2, [r3], #4
003af3bc: str      r2, [r3], #4
003af3c0: str      r2, [r3], #4
003af3c4: str      r2, [r3], #4
003af3c8: str      r2, [r3], #4
003af3cc: str      r2, [r3], #4
003af3d0: mvn      r1, #0
003af3d4: str      r2, [r3]
003af3d8: str      r1, [r0, #0x24]
003af3dc: str      r2, [r0, #0x1c]
003af3e0: str      r1, [r0]
003af3e4: str      r1, [r0, #0xc]
003af3e8: str      r1, [r0, #8]
003af3ec: str      r1, [r0, #4]
003af3f0: str      r1, [r0, #0x20]
003af3f4: bx       lr

# _ZN9Character18_F_CalculateResultERNS_12AttackResultEPS_S2_iiii
003b2638: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b263c: ldr      r6, [pc, #0x7c0]
003b2640: ldr      r7, [pc, #0x7c0]
003b2644: mov      r5, r0
003b2648: add      r6, pc, r6
003b264c: ldr      ip, [r6, r7]
003b2650: ldr      r0, [pc, #0x7b4]
003b2654: sub      sp, sp, #0x60
003b2658: ldr      ip, [ip]
003b265c: add      r0, pc, r0
003b2660: mov      r4, r3
003b2664: mov      sl, r1
003b2668: mov      sb, r2
003b266c: ldr      r8, [sp, #0x84]
003b2670: str      ip, [sp, #0x5c]
003b2674: bl       #0x3136b4
003b2678: mov      r0, r5
003b267c: bl       #0x3af3b0
003b2680: ldr      r3, [sp, #0x80]
003b2684: ubfx     ip, r4, #0x1b, #1
003b2688: str      r4, [r5, #0x1c]
003b268c: str      r3, [r5, #0x20]
003b2690: str      r8, [r5, #0x24]
003b2694: mov      r0, sl
003b2698: mov      r1, sb
003b269c: mov      r2, r8
003b26a0: ubfx     r3, r4, #0x1a, #1
003b26a4: str      ip, [sp]
003b26a8: bl       #0x3b059c
003b26ac: tst      r4, #5
003b26b0: bne      #0x3b27e8
003b26b4: tst      r4, #0xa
003b26b8: bne      #0x3b2b44
003b26bc: ldrb     r8, [r5, #0x18]
003b26c0: ands     r8, r8, #3
003b26c4: bne      #0x3b286c
003b26c8: tst      r4, #0x10
003b26cc: bne      #0x3b2ac0
003b26d0: tst      r4, #0x20
003b26d4: bne      #0x3b2a70
003b26d8: tst      r4, #0x40
003b26dc: mvneq    r8, #0
003b26e0: bne      #0x3b2b08
003b26e4: tst      r4, #0x80
003b26e8: beq      #0x3b287c
003b26ec: cmn      r8, #1
003b26f0: beq      #0x3b2de0
003b26f4: ldr      r3, [pc, #0x714]
003b26f8: add      r3, pc, r3
003b26fc: ldr      r0, [r3, #0x1c]
003b2700: cmp      r0, #0
003b2704: beq      #0x3b28c8
003b2708: ldr      r1, [r3, #0x20]
003b270c: cmp      r1, #0
003b2710: beq      #0x3b28c8
003b2714: mov      r2, r8
003b2718: mov      r3, #0
003b271c: bl       #0x3b0e8c
003b2720: ldrb     r3, [r5, #0x18]
003b2724: bfi      r3, r0, #4, #1
003b2728: strb     r3, [r5, #0x18]
003b272c: tst      r4, #0x200
003b2730: bne      #0x3b2a28
003b2734: tst      r4, #0x400
003b2738: bne      #0x3b2d24
003b273c: tst      r4, #0x800
003b2740: bne      #0x3b2948
003b2744: tst      r4, #0x1000
003b2748: bne      #0x3b2ce8
003b274c: tst      r4, #0x2000
003b2750: bne      #0x3b2994
003b2754: tst      r4, #0x4000
003b2758: bne      #0x3b2cac
003b275c: tst      r4, #0x8000
003b2760: bne      #0x3b29e0
003b2764: tst      r4, #0x10000
003b2768: bne      #0x3b2c70
003b276c: ldr      r3, [pc, #0x6a0]
003b2770: add      r8, sp, #0x44
003b2774: ldr      sl, [r6, r3]
003b2778: mov      r0, sl
003b277c: bl       #0x337888
003b2780: ldr      r1, [pc, #0x690]
003b2784: add      r2, sp, #0x40
003b2788: mov      r0, r8
003b278c: add      r1, pc, r1
003b2790: bl       #0x3140ec
003b2794: mov      r1, r8
003b2798: mov      r0, sl
003b279c: bl       #0x337a88
003b27a0: mov      r0, r8
003b27a4: bl       #0x3139ac
003b27a8: ands     r3, r4, #0x20000
003b27ac: bne      #0x3b28d0
003b27b0: tst      r4, #0x40000
003b27b4: bne      #0x3b2c00
003b27b8: tst      r4, #0x80000
003b27bc: bne      #0x3b2d60
003b27c0: ldr      r0, [pc, #0x654]
003b27c4: add      r0, pc, r0
003b27c8: bl       #0x3136b8
003b27cc: ldr      r3, [r6, r7]
003b27d0: ldr      r2, [sp, #0x5c]
003b27d4: ldr      r3, [r3]
003b27d8: cmp      r2, r3
003b27dc: bne      #0x3b2e00
003b27e0: add      sp, sp, #0x60
003b27e4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b27e8: mov      r0, #0x64
003b27ec: bl       #0x3af6d8
003b27f0: ldr      r3, [pc, #0x628]
003b27f4: mov      r2, r0
003b27f8: add      r3, pc, r3
003b27fc: ldr      r0, [r3, #0x1c]
003b2800: cmp      r0, #0
003b2804: beq      #0x3b28bc
003b2808: ldr      r1, [r3, #0x20]
003b280c: cmp      r1, #0
003b2810: beq      #0x3b28bc
003b2814: ldr      ip, [r3, #0x2c]
003b2818: lsl      r2, r2, #8
003b281c: str      ip, [sp]
003b2820: ldrb     ip, [r3, #0x30]
003b2824: mov      r3, #0
003b2828: str      ip, [sp, #4]
003b282c: bl       #0x3b1df0
003b2830: sxth     r0, r0
003b2834: ubfx     r3, r0, #8, #8
003b2838: strb     r3, [sp, #0x11]
003b283c: strb     r0, [sp, #0x10]
003b2840: ldrh     r3, [sp, #0x10]
003b2844: strh     r3, [sp, #0x3c]
003b2848: lsr      r2, r3, #8
003b284c: uxtb     r1, r3
003b2850: ldrb     r3, [r5, #0x18]
003b2854: bfi      r3, r1, #0, #1
003b2858: bfi      r3, r2, #1, #1
003b285c: strb     r3, [r5, #0x18]
003b2860: ldrb     r8, [r5, #0x18]
003b2864: ands     r8, r8, #3
003b2868: beq      #0x3b26c8
003b286c: ldr      r0, [pc, #0x5b0]
003b2870: add      r0, pc, r0
003b2874: bl       #0x3136b8
003b2878: b        #0x3b27cc
003b287c: tst      r4, #0x100
003b2880: beq      #0x3b272c
003b2884: cmn      r8, #1
003b2888: beq      #0x3b2df0
003b288c: ldr      r3, [pc, #0x594]
003b2890: add      r3, pc, r3
003b2894: ldr      r0, [r3, #0x1c]
003b2898: cmp      r0, #0
003b289c: beq      #0x3b28c8
003b28a0: ldr      r1, [r3, #0x20]
003b28a4: cmp      r1, #0
003b28a8: beq      #0x3b28c8
003b28ac: mov      r2, r8
003b28b0: mov      r3, #2
003b28b4: bl       #0x3b0e8c
003b28b8: b        #0x3b2720
003b28bc: mov      r2, #0
003b28c0: mov      r1, r2
003b28c4: b        #0x3b2850
003b28c8: mov      r0, #0
003b28cc: b        #0x3b2720
003b28d0: ldr      ip, [pc, #0x554]
003b28d4: add      ip, pc, ip
003b28d8: ldr      r1, [ip, #0x1c]
003b28dc: cmp      r1, #0
003b28e0: beq      #0x3b2bd8
003b28e4: ldr      r2, [ip, #0x20]
003b28e8: cmp      r2, #0
003b28ec: beq      #0x3b2bd8
003b28f0: mov      r3, #0
003b28f4: str      r3, [sp]
003b28f8: ldr      r0, [ip, #0x2c]
003b28fc: str      r0, [sp, #4]
003b2900: ldrb     lr, [ip, #0x30]
003b2904: add      r0, sp, #0x1c
003b2908: str      lr, [sp, #8]
003b290c: ldrb     ip, [ip, #0x31]
003b2910: str      ip, [sp, #0xc]
003b2914: bl       #0x3b1fb8
003b2918: add      r0, sp, #0x28
003b291c: ldm      r0, {r0, r1, r2, r4}
003b2920: ldr      ip, [sp, #0x24]
003b2924: ldr      lr, [sp, #0x20]
003b2928: ldr      r3, [sp, #0x1c]
003b292c: stm      r5, {r3, r4}
003b2930: str      lr, [r5, #0x24]
003b2934: str      ip, [r5, #0x10]
003b2938: str      r0, [r5, #0x14]
003b293c: str      r1, [r5, #8]
003b2940: str      r2, [r5, #0xc]
003b2944: b        #0x3b27c0
003b2948: mov      r0, #0x64
003b294c: bl       #0x3af6d8
003b2950: ldr      r3, [pc, #0x4d8]
003b2954: mov      r2, r0
003b2958: add      r3, pc, r3
003b295c: ldr      r0, [r3, #0x1c]
003b2960: cmp      r0, #0
003b2964: beq      #0x3b2bb8
003b2968: ldr      r1, [r3, #0x20]
003b296c: cmp      r1, #0
003b2970: beq      #0x3b2bb8
003b2974: lsl      r2, r2, #8
003b2978: mov      r3, #0
003b297c: bl       #0x3b0fa0
003b2980: ldrb     r3, [r5, #0x18]
003b2984: tst      r4, #0x2000
003b2988: bfi      r3, r0, #6, #1
003b298c: strb     r3, [r5, #0x18]
003b2990: beq      #0x3b2754
003b2994: mov      r0, #0x64
003b2998: bl       #0x3af6d8
003b299c: ldr      r3, [pc, #0x490]
003b29a0: mov      r2, r0
003b29a4: add      r3, pc, r3
003b29a8: ldr      r0, [r3, #0x1c]
003b29ac: cmp      r0, #0
003b29b0: beq      #0x3b2bc0
003b29b4: ldr      r1, [r3, #0x20]
003b29b8: cmp      r1, #0
003b29bc: beq      #0x3b2bc0
003b29c0: lsl      r2, r2, #8
003b29c4: mov      r3, #0
003b29c8: bl       #0x3b0c64
003b29cc: ldrb     r3, [r5, #0x18]
003b29d0: tst      r4, #0x8000
003b29d4: bfi      r3, r0, #5, #1
003b29d8: strb     r3, [r5, #0x18]
003b29dc: beq      #0x3b2764
003b29e0: mov      r0, #0x64
003b29e4: bl       #0x3af6d8
003b29e8: ldr      r3, [pc, #0x448]
003b29ec: mov      r2, r0
003b29f0: add      r3, pc, r3
003b29f4: ldr      r0, [r3, #0x1c]
003b29f8: cmp      r0, #0
003b29fc: beq      #0x3b2bc8
003b2a00: ldr      r1, [r3, #0x20]
003b2a04: cmp      r1, #0
003b2a08: beq      #0x3b2bc8
003b2a0c: lsl      r2, r2, #8
003b2a10: mov      r3, #0
003b2a14: bl       #0x3b0ad4
003b2a18: ldrb     r3, [r5, #0x19]
003b2a1c: bfi      r3, r0, #0, #1
003b2a20: strb     r3, [r5, #0x19]
003b2a24: b        #0x3b276c
003b2a28: mov      r0, #0x64
003b2a2c: bl       #0x3af6d8
003b2a30: ldr      r3, [pc, #0x404]
003b2a34: mov      r2, r0
003b2a38: add      r3, pc, r3
003b2a3c: ldr      r0, [r3, #0x1c]
003b2a40: cmp      r0, #0
003b2a44: beq      #0x3b2bb0
003b2a48: ldr      r1, [r3, #0x20]
003b2a4c: cmp      r1, #0
003b2a50: beq      #0x3b2bb0
003b2a54: lsl      r2, r2, #8
003b2a58: mov      r3, #0
003b2a5c: bl       #0x3b0d78
003b2a60: ldrb     r3, [r5, #0x18]
003b2a64: bfi      r3, r0, #7, #1
003b2a68: strb     r3, [r5, #0x18]
003b2a6c: b        #0x3b273c
003b2a70: mov      r0, #0x64
003b2a74: bl       #0x3af6d8
003b2a78: ldr      r3, [pc, #0x3c0]
003b2a7c: lsl      r8, r0, #8
003b2a80: add      r3, pc, r3
003b2a84: ldr      r0, [r3, #0x1c]
003b2a88: cmp      r0, #0
003b2a8c: beq      #0x3b2bd0
003b2a90: ldr      r1, [r3, #0x20]
003b2a94: cmp      r1, #0
003b2a98: beq      #0x3b2bd0
003b2a9c: mov      r2, r8
003b2aa0: mov      r3, #0
003b2aa4: bl       #0x3b0868
003b2aa8: ldrb     r3, [r5, #0x18]
003b2aac: tst      r4, #0x80
003b2ab0: bfi      r3, r0, #3, #1
003b2ab4: strb     r3, [r5, #0x18]
003b2ab8: beq      #0x3b287c
003b2abc: b        #0x3b26ec
003b2ac0: mov      r0, #0x64
003b2ac4: bl       #0x3af6d8
003b2ac8: ldr      r3, [pc, #0x374]
003b2acc: mov      r2, r0
003b2ad0: add      r3, pc, r3
003b2ad4: ldr      r0, [r3, #0x1c]
003b2ad8: cmp      r0, #0
003b2adc: beq      #0x3b2bf8
003b2ae0: ldr      r1, [r3, #0x20]
003b2ae4: cmp      r1, #0
003b2ae8: beq      #0x3b2bf8
003b2aec: lsl      r2, r2, #8
003b2af0: mov      r3, r8
003b2af4: bl       #0x3b0918
003b2af8: ldrb     r3, [r5, #0x18]
003b2afc: bfi      r3, r0, #2, #1
003b2b00: strb     r3, [r5, #0x18]
003b2b04: b        #0x3b26d0
003b2b08: mov      r0, #0x64
003b2b0c: bl       #0x3af6d8
003b2b10: ldr      r3, [pc, #0x330]
003b2b14: lsl      r8, r0, #8
003b2b18: add      r3, pc, r3
003b2b1c: ldr      r0, [r3, #0x1c]
003b2b20: cmp      r0, #0
003b2b24: beq      #0x3b2bd0
003b2b28: ldr      r1, [r3, #0x20]
003b2b2c: cmp      r1, #0
003b2b30: beq      #0x3b2bd0
003b2b34: mov      r2, r8
003b2b38: mov      r3, #2
003b2b3c: bl       #0x3b0868
003b2b40: b        #0x3b2aa8
003b2b44: mov      r0, #0x64
003b2b48: bl       #0x3af6d8
003b2b4c: ldr      r3, [pc, #0x2f8]
003b2b50: mov      r2, r0
003b2b54: add      r3, pc, r3
003b2b58: ldr      r0, [r3, #0x1c]
003b2b5c: cmp      r0, #0
003b2b60: beq      #0x3b28bc
003b2b64: ldr      r1, [r3, #0x20]
003b2b68: cmp      r1, #0
003b2b6c: beq      #0x3b28bc
003b2b70: ldr      ip, [r3, #0x2c]
003b2b74: lsl      r2, r2, #8
003b2b78: str      ip, [sp]
003b2b7c: ldrb     ip, [r3, #0x30]
003b2b80: mov      r3, #2
003b2b84: str      ip, [sp, #4]
003b2b88: bl       #0x3b1df0
003b2b8c: sxth     r0, r0
003b2b90: ubfx     r3, r0, #8, #8
003b2b94: strb     r3, [sp, #0x11]
003b2b98: strb     r0, [sp, #0x10]
003b2b9c: ldrh     r3, [sp, #0x10]
003b2ba0: strh     r3, [sp, #0x38]
003b2ba4: lsr      r2, r3, #8
003b2ba8: uxtb     r1, r3
003b2bac: b        #0x3b2850
003b2bb0: mov      r0, #0
003b2bb4: b        #0x3b2a60
003b2bb8: mov      r0, #0
003b2bbc: b        #0x3b2980
003b2bc0: mov      r0, #0
003b2bc4: b        #0x3b29cc
003b2bc8: mov      r0, #0
003b2bcc: b        #0x3b2a18
003b2bd0: mov      r0, #0
003b2bd4: b        #0x3b2aa8
003b2bd8: mov      r3, #0
003b2bdc: mov      lr, r3
003b2be0: mov      ip, r3
003b2be4: mov      r0, r3
003b2be8: mov      r1, r3
003b2bec: mov      r2, r3
003b2bf0: mov      r4, r3
003b2bf4: b        #0x3b292c
003b2bf8: mov      r0, #0
003b2bfc: b        #0x3b2af8
003b2c00: ldr      ip, [pc, #0x248]
003b2c04: add      ip, pc, ip
003b2c08: ldr      r1, [ip, #0x1c]
003b2c0c: cmp      r1, #0
003b2c10: beq      #0x3b2db8
003b2c14: ldr      r2, [ip, #0x20]
003b2c18: cmp      r2, #0
003b2c1c: beq      #0x3b2db8
003b2c20: mov      r0, #2
003b2c24: str      r0, [sp]
003b2c28: ldr      r0, [ip, #0x2c]
003b2c2c: str      r0, [sp, #4]
003b2c30: ldrb     lr, [ip, #0x30]
003b2c34: add      r0, sp, #0x1c
003b2c38: str      lr, [sp, #8]
003b2c3c: ldrb     ip, [ip, #0x31]
003b2c40: str      ip, [sp, #0xc]
003b2c44: bl       #0x3b1fb8
003b2c48: add      r0, sp, #0x28
003b2c4c: ldm      r0, {r0, r1, r2, lr}
003b2c50: ldr      ip, [sp, #0x24]
003b2c54: ldr      r3, [sp, #0x1c]
003b2c58: stm      r5, {r3, lr}
003b2c5c: str      ip, [r5, #0x10]
003b2c60: str      r0, [r5, #0x14]
003b2c64: str      r1, [r5, #8]
003b2c68: str      r2, [r5, #0xc]
003b2c6c: b        #0x3b27c0
003b2c70: mov      r0, #0x64
003b2c74: bl       #0x3af6d8
003b2c78: ldr      r3, [pc, #0x1d4]
003b2c7c: mov      r2, r0
003b2c80: add      r3, pc, r3
003b2c84: ldr      r0, [r3, #0x1c]
003b2c88: cmp      r0, #0
003b2c8c: beq      #0x3b2bc8
003b2c90: ldr      r1, [r3, #0x20]
003b2c94: cmp      r1, #0
003b2c98: beq      #0x3b2bc8
003b2c9c: lsl      r2, r2, #8
003b2ca0: mov      r3, #2
003b2ca4: bl       #0x3b0ad4
003b2ca8: b        #0x3b2a18
003b2cac: mov      r0, #0x64
003b2cb0: bl       #0x3af6d8
003b2cb4: ldr      r3, [pc, #0x19c]
003b2cb8: mov      r2, r0
003b2cbc: add      r3, pc, r3
003b2cc0: ldr      r0, [r3, #0x1c]
003b2cc4: cmp      r0, #0
003b2cc8: beq      #0x3b2bc0
003b2ccc: ldr      r1, [r3, #0x20]
003b2cd0: cmp      r1, #0
003b2cd4: beq      #0x3b2bc0
003b2cd8: lsl      r2, r2, #8
003b2cdc: mov      r3, #2
003b2ce0: bl       #0x3b0c64
003b2ce4: b        #0x3b29cc
003b2ce8: mov      r0, #0x64
003b2cec: bl       #0x3af6d8
003b2cf0: ldr      r3, [pc, #0x164]
003b2cf4: mov      r2, r0
003b2cf8: add      r3, pc, r3
003b2cfc: ldr      r0, [r3, #0x1c]
003b2d00: cmp      r0, #0
003b2d04: beq      #0x3b2bb8
003b2d08: ldr      r1, [r3, #0x20]
003b2d0c: cmp      r1, #0
003b2d10: beq      #0x3b2bb8
003b2d14: lsl      r2, r2, #8
003b2d18: mov      r3, #2
003b2d1c: bl       #0x3b0fa0
003b2d20: b        #0x3b2980
003b2d24: mov      r0, #0x64
003b2d28: bl       #0x3af6d8
003b2d2c: ldr      r3, [pc, #0x12c]
003b2d30: mov      r2, r0
003b2d34: add      r3, pc, r3
003b2d38: ldr      r0, [r3, #0x1c]
003b2d3c: cmp      r0, #0
003b2d40: beq      #0x3b2bb0
003b2d44: ldr      r1, [r3, #0x20]
003b2d48: cmp      r1, #0
003b2d4c: beq      #0x3b2bb0
003b2d50: lsl      r2, r2, #8
003b2d54: mov      r3, #2
003b2d58: bl       #0x3b0d78
003b2d5c: b        #0x3b2a60
003b2d60: ldr      ip, [pc, #0xfc]
003b2d64: add      ip, pc, ip
003b2d68: ldr      r1, [ip, #0x1c]
003b2d6c: cmp      r1, #0
003b2d70: beq      #0x3b2dd4
003b2d74: ldr      r2, [ip, #0x20]
003b2d78: cmp      r2, #0
003b2d7c: beq      #0x3b2dd4
003b2d80: mov      r3, #3
003b2d84: str      r3, [sp]
003b2d88: ldr      r3, [ip, #0x2c]
003b2d8c: add      r0, sp, #0x1c
003b2d90: str      r3, [sp, #4]
003b2d94: ldrb     lr, [ip, #0x30]
003b2d98: ldr      r3, [sp, #0x88]
003b2d9c: str      lr, [sp, #8]
003b2da0: ldrb     ip, [ip, #0x31]
003b2da4: str      ip, [sp, #0xc]
003b2da8: bl       #0x3b1fb8
003b2dac: ldr      r3, [sp, #0x1c]
003b2db0: str      r3, [r5]
003b2db4: b        #0x3b27c0
003b2db8: mov      r3, #0
003b2dbc: mov      ip, r3
003b2dc0: mov      r0, r3
003b2dc4: mov      r1, r3
003b2dc8: mov      r2, r3
003b2dcc: mov      lr, r3
003b2dd0: b        #0x3b2c58
003b2dd4: mov      r3, #0
003b2dd8: str      r3, [r5]
003b2ddc: b        #0x3b27c0
003b2de0: mov      r0, #0x64
003b2de4: bl       #0x3af6d8
003b2de8: lsl      r8, r0, #8
003b2dec: b        #0x3b26f4
003b2df0: mov      r0, #0x64
003b2df4: bl       #0x3af6d8
003b2df8: lsl      r8, r0, #8
003b2dfc: b        #0x3b288c
003b2e00: bl       #0x30e310
003b2e04: subseq   r2, lr, r8, asr #8
003b2e08: andeq    r4, r0, ip, lsr #1
003b2e0c: ldrsbeq  r1, [r1], #-0x64
003b2e10: subseq   r0, pc, r0, lsr #5
003b2e14: andeq    r0, r0, r4, lsl #17
003b2e18: subseq   r1, r1, ip, lsr #10
003b2e1c: subseq   r1, r1, ip, ror #10
003b2e20: subseq   r0, pc, r0, lsr #3
003b2e24: subseq   r1, r1, r0, asr #9
003b2e28: subseq   r0, pc, r8, lsl #2
003b2e2c: subseq   r0, pc, r4, asr #1
003b2e30: subseq   r0, pc, r0, asr #32
003b2e34: ldrsheq  pc, [lr], #-0xf4
003b2e38: subseq   pc, lr, r8, lsr #31
003b2e3c: subseq   pc, lr, r0, ror #30
003b2e40: subseq   pc, lr, r8, lsl pc
003b2e44: subseq   pc, lr, r8, asr #29
003b2e48: subseq   pc, lr, r0, lsl #29
003b2e4c: subseq   pc, lr, r4, asr #28

# _ZN9Character6HitForEjP10GameObject
003a8bc4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a8bc8: ldr      r4, [pc, #0x64c]
003a8bcc: ldr      r6, [pc, #0x64c]
003a8bd0: mov      fp, r1
003a8bd4: add      r4, pc, r4
003a8bd8: ldr      ip, [r4, r6]
003a8bdc: sub      sp, sp, #0x6c
003a8be0: ldr      r3, [r0]
003a8be4: ldr      r1, [ip]
003a8be8: mov      r5, r0
003a8bec: mov      r7, r2
003a8bf0: str      r1, [sp, #0x64]
003a8bf4: mov      lr, pc
003a8bf8: ldr      pc, [r3, #0x34]
003a8bfc: subs     sl, r0, #0
003a8c00: beq      #0x3a8c20
003a8c04: ldr      r3, [r4, r6]
003a8c08: ldr      r2, [sp, #0x64]
003a8c0c: ldr      r3, [r3]
003a8c10: cmp      r2, r3
003a8c14: bne      #0x3a9218
003a8c18: add      sp, sp, #0x6c
003a8c1c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a8c20: ldr      r0, [pc, #0x5fc]
003a8c24: mov      r1, sl
003a8c28: mov      r2, #1
003a8c2c: ldr      r3, [r4, r0]
003a8c30: str      r0, [sp, #8]
003a8c34: add      r8, sp, #0x4c
003a8c38: ldr      r0, [r3, #0x40]
003a8c3c: movw     r3, #0x1088
003a8c40: ldr      r3, [r5, r3]
003a8c44: str      r3, [sp, #0x1c]
003a8c48: movw     r3, #0x1090
003a8c4c: ldr      r3, [r5, r3]
003a8c50: str      r3, [sp, #0x18]
003a8c54: bl       #0x36e478
003a8c58: ldr      r2, [pc, #0x5c8]
003a8c5c: str      r2, [sp, #0xc]
003a8c60: ldr      sb, [r4, r2]
003a8c64: ldr      r0, [r0, #0x660]
003a8c68: str      r0, [sp, #0x14]
003a8c6c: mov      r0, sb
003a8c70: bl       #0x337888
003a8c74: ldr      r1, [pc, #0x5b0]
003a8c78: add      r2, sp, #0x30
003a8c7c: mov      r0, r8
003a8c80: add      r1, pc, r1
003a8c84: bl       #0x3140ec
003a8c88: mov      r0, sb
003a8c8c: mov      r1, r8
003a8c90: bl       #0x337a88
003a8c94: cmp      r0, #0
003a8c98: bne      #0x3a9014
003a8c9c: mov      r0, r8
003a8ca0: bl       #0x318254
003a8ca4: bl       #0x7fd794
003a8ca8: ldrb     r3, [r0, #5]
003a8cac: cmp      r3, #0
003a8cb0: beq      #0x3a8f30
003a8cb4: ldr      r0, [sp, #8]
003a8cb8: ldr      r3, [r4, r0]
003a8cbc: ldr      r3, [r3, #0x40]
003a8cc0: cmp      r3, #0
003a8cc4: asreq    r2, fp, #8
003a8cc8: streq    r2, [sp, #0x10]
003a8ccc: rsbeq    r2, fp, #0
003a8cd0: beq      #0x3a8d00
003a8cd4: ldr      r3, [r3, #0x714]
003a8cd8: cmp      r3, #0
003a8cdc: beq      #0x3a8f54
003a8ce0: cmp      r3, #5
003a8ce4: asreq    r0, fp, #8
003a8ce8: streq    r0, [sp, #0x10]
003a8cec: rsbeq    r2, fp, #0
003a8cf0: beq      #0x3a8d00
003a8cf4: mov      r0, #0
003a8cf8: str      r0, [sp, #0x10]
003a8cfc: mov      r2, r0
003a8d00: add      sl, r5, #0x560
003a8d04: mov      r0, sl
003a8d08: mov      r1, #0x24
003a8d0c: ldr      r8, [pc, #0x51c]
003a8d10: bl       #0x3e0708
003a8d14: ldr      r2, [sp, #8]
003a8d18: add      r8, pc, r8
003a8d1c: mov      r1, r8
003a8d20: ldr      r0, [r4, r2]
003a8d24: bl       #0x320e14
003a8d28: cmp      r0, #0
003a8d2c: beq      #0x3a8fcc
003a8d30: ldr      r3, [r5]
003a8d34: mov      r0, r5
003a8d38: mov      lr, pc
003a8d3c: ldr      pc, [r3, #0x28]
003a8d40: cmp      r0, #0
003a8d44: bne      #0x3a9038
003a8d48: mov      r0, sl
003a8d4c: mov      r1, #0x24
003a8d50: mov      r2, #0
003a8d54: bl       #0x3e07a0
003a8d58: movw     r3, #0x1088
003a8d5c: ldr      r3, [r5, r3]
003a8d60: cmp      r3, #0
003a8d64: movgt    r0, #0
003a8d68: strgt    r0, [sp, #0xc]
003a8d6c: ble      #0x3a90e8
003a8d70: ldr      r3, [r5]
003a8d74: mov      r0, r5
003a8d78: mov      lr, pc
003a8d7c: ldr      pc, [r3, #0x28]
003a8d80: cmp      r0, #0
003a8d84: beq      #0x3a8f64
003a8d88: movw     r3, #0x1090
003a8d8c: ldr      r3, [r5, r3]
003a8d90: movw     r2, #0x1088
003a8d94: ldr      r2, [r5, r2]
003a8d98: add      r3, r3, r3, lsr #31
003a8d9c: cmp      r2, r3, asr #1
003a8da0: bgt      #0x3a8f64
003a8da4: bl       #0x7fd794
003a8da8: ldrb     r3, [r0, #5]
003a8dac: cmp      r3, #0
003a8db0: beq      #0x3a9060
003a8db4: movw     r3, #0x1448
003a8db8: ldrb     r2, [r5, r3]
003a8dbc: cmp      r2, #0
003a8dc0: beq      #0x3a8e54
003a8dc4: ldr      r0, [sp, #0x14]
003a8dc8: mov      r8, #0
003a8dcc: strb     r8, [r5, r3]
003a8dd0: cmp      r5, r0
003a8dd4: beq      #0x3a91a4
003a8dd8: ldr      r3, [pc, #0x454]
003a8ddc: ldr      r2, [r4, r3]
003a8de0: ldr      r3, [pc, #0x450]
003a8de4: ldr      sl, [r2]
003a8de8: ldr      r3, [r4, r3]
003a8dec: cmp      sl, #0
003a8df0: ldr      r3, [r3]
003a8df4: str      r3, [sp, #0x14]
003a8df8: beq      #0x3a919c
003a8dfc: ldr      r3, [pc, #0x438]
003a8e00: ldr      fp, [pc, #0x438]
003a8e04: ldr      r3, [r4, r3]
003a8e08: add      fp, pc, fp
003a8e0c: ldr      sb, [r3]
003a8e10: b        #0x3a8e20
003a8e14: add      r8, r8, #1
003a8e18: cmp      r8, sl
003a8e1c: beq      #0x3a919c
003a8e20: mov      r0, fp
003a8e24: ldr      r1, [sb, r8, lsl #2]
003a8e28: bl       #0x30e31c
003a8e2c: cmp      r0, #0
003a8e30: bne      #0x3a8e14
003a8e34: mov      r1, r8
003a8e38: mov      ip, #0
003a8e3c: mov      r2, ip
003a8e40: ldr      r0, [sp, #0x14]
003a8e44: mov      r3, ip
003a8e48: str      ip, [sp]
003a8e4c: str      ip, [sp, #4]
003a8e50: bl       #0x36b80c
003a8e54: add      r8, sp, #0x20
003a8e58: mov      r1, r7
003a8e5c: mov      r0, r8
003a8e60: bl       #0x33dd2c
003a8e64: mov      r0, r8
003a8e68: bl       #0x33ff54
003a8e6c: ldr      r3, [pc, #0x3d0]
003a8e70: subs     r8, r0, #0
003a8e74: ldr      r3, [r4, r3]
003a8e78: ldr      r7, [r3]
003a8e7c: beq      #0x3a8c04
003a8e80: ldr      r3, [r8]
003a8e84: mov      lr, pc
003a8e88: ldr      pc, [r3, #0x28]
003a8e8c: cmp      r0, #0
003a8e90: beq      #0x3a8c04
003a8e94: cmp      r5, r8
003a8e98: beq      #0x3a8c04
003a8e9c: ldr      r2, [sp, #8]
003a8ea0: mov      r1, r8
003a8ea4: ldr      r3, [r4, r2]
003a8ea8: ldr      r0, [r3, #0x40]
003a8eac: bl       #0x36effc
003a8eb0: cmp      r0, #0
003a8eb4: beq      #0x3a8c04
003a8eb8: ldr      r3, [sp, #0x10]
003a8ebc: cmp      r3, #0xc7
003a8ec0: bgt      #0x3a9138
003a8ec4: ldr      r0, [sp, #0x10]
003a8ec8: cmp      r0, #0x95
003a8ecc: bgt      #0x3a9150
003a8ed0: ldr      r2, [sp, #0x10]
003a8ed4: cmp      r2, #0x63
003a8ed8: bgt      #0x3a9168
003a8edc: ldr      r3, [sp, #0x10]
003a8ee0: cmp      r3, #0x31
003a8ee4: bgt      #0x3a9180
003a8ee8: ldr      r0, [sp, #0x1c]
003a8eec: ldr      r2, [sp, #0x18]
003a8ef0: cmp      r0, r2
003a8ef4: bne      #0x3a8c04
003a8ef8: ldr      r3, [sp, #0xc]
003a8efc: cmp      r3, #0
003a8f00: beq      #0x3a8c04
003a8f04: mov      r0, r5
003a8f08: bl       #0x3a3158
003a8f0c: cmp      r0, #0
003a8f10: beq      #0x3a9204
003a8f14: ldr      r0, [pc, #0x32c]
003a8f18: add      r0, pc, r0
003a8f1c: bl       #0x3a3f70
003a8f20: mov      r1, r0
003a8f24: mov      r0, r7
003a8f28: bl       #0x3813b8
003a8f2c: b        #0x3a8c04
003a8f30: ldr      r2, [sp, #0x14]
003a8f34: cmp      r2, #0
003a8f38: beq      #0x3a8cf4
003a8f3c: ldr      r3, [r2]
003a8f40: mov      r0, r2
003a8f44: mov      lr, pc
003a8f48: ldr      pc, [r3, #0x34]
003a8f4c: cmp      r0, #0
003a8f50: bne      #0x3a8cf4
003a8f54: asr      r3, fp, #8
003a8f58: str      r3, [sp, #0x10]
003a8f5c: rsb      r2, fp, #0
003a8f60: b        #0x3a8d00
003a8f64: ldr      r3, [r5]
003a8f68: mov      r0, r5
003a8f6c: mov      lr, pc
003a8f70: ldr      pc, [r3, #0x28]
003a8f74: cmp      r0, #0
003a8f78: beq      #0x3a8e54
003a8f7c: movw     r8, #0x1448
003a8f80: ldrb     r3, [r5, r8]
003a8f84: cmp      r3, #0
003a8f88: bne      #0x3a8e54
003a8f8c: movw     r3, #0x1088
003a8f90: ldr      r0, [r5, r3]
003a8f94: bl       #0x30e964
003a8f98: movw     r3, #0x1090
003a8f9c: mov      sl, r0
003a8fa0: ldr      r0, [r5, r3]
003a8fa4: bl       #0x30e964
003a8fa8: mov      r1, #0x3f400000
003a8fac: bl       #0x30ed6c
003a8fb0: mov      r1, r0
003a8fb4: mov      r0, sl
003a8fb8: bl       #0x30e4b4
003a8fbc: cmp      r0, #0
003a8fc0: movne    r3, #1
003a8fc4: strbne   r3, [r5, r8]
003a8fc8: b        #0x3a8e54
003a8fcc: ldr      r3, [sp, #0xc]
003a8fd0: add      sb, sp, #0x34
003a8fd4: ldr      fp, [r4, r3]
003a8fd8: mov      r0, fp
003a8fdc: bl       #0x337888
003a8fe0: mov      r1, r8
003a8fe4: add      r2, sp, #0x2c
003a8fe8: mov      r0, sb
003a8fec: bl       #0x3140ec
003a8ff0: mov      r1, sb
003a8ff4: mov      r0, fp
003a8ff8: bl       #0x337a88
003a8ffc: mov      r8, r0
003a9000: mov      r0, sb
003a9004: bl       #0x318254
003a9008: cmp      r8, #0
003a900c: beq      #0x3a8d58
003a9010: b        #0x3a8d30
003a9014: mov      r0, r5
003a9018: bl       #0x3a3064
003a901c: cmp      r0, #0
003a9020: beq      #0x3a8c9c
003a9024: mov      r0, r8
003a9028: str      sl, [sp, #0x10]
003a902c: bl       #0x318254
003a9030: ldr      r2, [sp, #0x10]
003a9034: b        #0x3a8d00
003a9038: ldr      r3, [r5, #0x418]
003a903c: cmp      r3, #0
003a9040: beq      #0x3a8d58
003a9044: mov      r0, r3
003a9048: ldr      r3, [r3]
003a904c: mov      lr, pc
003a9050: ldr      pc, [r3, #0x28]
003a9054: cmp      r0, #0
003a9058: bne      #0x3a8d58
003a905c: b        #0x3a8d48
003a9060: mov      r0, r5
003a9064: bl       #0x3bb8e4
003a9068: subs     r8, r0, #0
003a906c: bne      #0x3a8db4
003a9070: ldr      r0, [sp, #8]
003a9074: ldr      r3, [r4, r0]
003a9078: ldr      r3, [r3, #0x4c]
003a907c: ldrb     r3, [r3, #0x2d]
003a9080: cmp      r3, #0
003a9084: beq      #0x3a8db4
003a9088: ldr      r3, [pc, #0x1bc]
003a908c: ldr      r1, [pc, #0x1bc]
003a9090: mov      r2, #1
003a9094: ldr      sl, [r4, r3]
003a9098: add      r1, pc, r1
003a909c: mov      r0, sl
003a90a0: bl       #0x4591f0
003a90a4: cmn      r0, #1
003a90a8: mov      r1, r0
003a90ac: beq      #0x3a90c0
003a90b0: mov      r0, sl
003a90b4: mov      r3, r8
003a90b8: mvn      r2, #0
003a90bc: bl       #0x4605c0
003a90c0: ldr      r2, [sp, #8]
003a90c4: mov      r1, #0
003a90c8: ldr      r3, [r4, r2]
003a90cc: ldr      r2, [r3, #0x4c]
003a90d0: ldr      r3, [pc, #0x17c]
003a90d4: strb     r1, [r2, #0x2d]
003a90d8: ldr      r3, [r4, r3]
003a90dc: ldr      r0, [r3]
003a90e0: bl       #0x317e98
003a90e4: b        #0x3a8db4
003a90e8: mov      r0, sl
003a90ec: mov      r1, #0x24
003a90f0: mov      r2, #0
003a90f4: bl       #0x3e07a0
003a90f8: mov      r2, #0
003a90fc: ldr      r0, [r5, #0x378]
003a9100: mov      r1, r7
003a9104: bl       #0x40570c
003a9108: ldr      r3, [r5]
003a910c: mov      r0, r5
003a9110: mov      lr, pc
003a9114: ldr      pc, [r3, #0x54]
003a9118: cmp      r0, #0
003a911c: moveq    r3, #3
003a9120: streq    r3, [r5, #0x11c]
003a9124: movne    r2, #1
003a9128: moveq    r3, #1
003a912c: strne    r2, [sp, #0xc]
003a9130: streq    r3, [sp, #0xc]
003a9134: b        #0x3a8d70
003a9138: ldr      r0, [pc, #0x118]
003a913c: add      r0, pc, r0
003a9140: bl       #0x3a3f70
003a9144: mov      r1, r0
003a9148: mov      r0, r7
003a914c: bl       #0x3813b8
003a9150: ldr      r0, [pc, #0x104]
003a9154: add      r0, pc, r0
003a9158: bl       #0x3a3f70
003a915c: mov      r1, r0
003a9160: mov      r0, r7
003a9164: bl       #0x3813b8
003a9168: ldr      r0, [pc, #0xf0]
003a916c: add      r0, pc, r0
003a9170: bl       #0x3a3f70
003a9174: mov      r1, r0
003a9178: mov      r0, r7
003a917c: bl       #0x3813b8
003a9180: ldr      r0, [pc, #0xdc]
003a9184: add      r0, pc, r0
003a9188: bl       #0x3a3f70
003a918c: mov      r1, r0
003a9190: mov      r0, r7
003a9194: bl       #0x3813b8
003a9198: b        #0x3a8ee8
003a919c: mvn      r1, #0
003a91a0: b        #0x3a8e38
003a91a4: ldr      r3, [pc, #0x88]
003a91a8: ldr      r2, [r4, r3]
003a91ac: ldr      r3, [pc, #0x84]
003a91b0: ldr      sl, [r2]
003a91b4: ldr      r3, [r4, r3]
003a91b8: cmp      sl, r8
003a91bc: ldr      r3, [r3]
003a91c0: str      r3, [sp, #0x14]
003a91c4: beq      #0x3a919c
003a91c8: ldr      r3, [pc, #0x6c]
003a91cc: ldr      sb, [pc, #0x94]
003a91d0: ldr      r3, [r4, r3]
003a91d4: add      sb, pc, sb
003a91d8: ldr      fp, [r3]
003a91dc: b        #0x3a91ec
003a91e0: add      r8, r8, #1
003a91e4: cmp      r8, sl
003a91e8: beq      #0x3a919c
003a91ec: mov      r0, sb
003a91f0: ldr      r1, [fp, r8, lsl #2]
003a91f4: bl       #0x30e31c
003a91f8: cmp      r0, #0
003a91fc: bne      #0x3a91e0
003a9200: b        #0x3a8e34
003a9204: mov      r0, r5
003a9208: bl       #0x3a3144
003a920c: cmp      r0, #0
003a9210: beq      #0x3a8c04
003a9214: b        #0x3a8f14
003a9218: bl       #0x30e310
003a921c: ldrheq   fp, [lr], #-0xec
003a9220: andeq    r4, r0, ip, lsr #1
003a9224: strdeq   r3, r4, [r0], -r4
003a9228: andeq    r0, r0, r4, lsl #17
003a922c: subseq   sl, r1, r8, lsl #15
003a9230: subseq   sl, r1, r0, lsl #14
003a9234: andeq    r3, r0, r8, lsr sp
003a9238: andeq    r0, r0, r4, lsr #27
003a923c: andeq    r3, r0, r8, lsr #19
003a9240: subseq   sl, r1, r0, asr r6
003a9244: andeq    r1, r0, r0, ror sp

# _ZN6CharAI8OnAttackEiib
003d0e7c: push     {r4, r5, r6, r7, r8, lr}
003d0e80: mov      r4, r0
003d0e84: ldr      r0, [r0, #4]
003d0e88: mov      r5, r1
003d0e8c: mov      r1, #0
003d0e90: add      r0, r0, #0x3c8
003d0e94: mov      r7, r2
003d0e98: mov      r6, r3
003d0e9c: bl       #0x3d67f4
003d0ea0: cmp      r0, #0
003d0ea4: beq      #0x3d0ed0
003d0ea8: ldr      ip, [r4, #0x1c]
003d0eac: cmp      ip, #0
003d0eb0: beq      #0x3d0ed0
003d0eb4: mov      r0, ip
003d0eb8: mov      r1, r5
003d0ebc: mov      r2, r7
003d0ec0: mov      r3, r6
003d0ec4: ldr      ip, [ip]
003d0ec8: mov      lr, pc
003d0ecc: ldr      pc, [ip, #0xa8]
003d0ed0: pop      {r4, r5, r6, r7, r8, pc}
