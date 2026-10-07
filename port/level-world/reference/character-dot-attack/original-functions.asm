
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

# _ZN9Character13F_ApplyResultERKNS_12AttackResultEPS_S3_b
003b10b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b10b8: ldr      r7, [pc, #0xcb4]
003b10bc: ldr      sb, [pc, #0xcb4]
003b10c0: mov      r4, r0
003b10c4: add      r7, pc, r7
003b10c8: ldr      r0, [r7, sb]
003b10cc: mov      r5, r2
003b10d0: sub      sp, sp, #0x15c
003b10d4: ldr      r2, [r0]
003b10d8: mov      r8, r3
003b10dc: mov      r6, r1
003b10e0: str      r2, [sp, #0x154]
003b10e4: bl       #0x7fd794
003b10e8: ldrb     r3, [r0, #5]
003b10ec: cmp      r3, #0
003b10f0: bne      #0x3b1440
003b10f4: ldrb     r3, [r4, #0x18]
003b10f8: ldr      fp, [pc, #0xc7c]
003b10fc: add      r8, sp, #0x13c
003b1100: tst      r3, #3
003b1104: movw     r3, #0x14d0
003b1108: ldrheq   r2, [r6, r3]
003b110c: ldr      sl, [r7, fp]
003b1110: movne    r2, #0
003b1114: addeq    r2, r2, #1
003b1118: strh     r2, [r6, r3]
003b111c: mov      r0, sl
003b1120: bl       #0x337888
003b1124: ldr      r1, [pc, #0xc54]
003b1128: add      r2, sp, #0x48
003b112c: mov      r0, r8
003b1130: add      r1, pc, r1
003b1134: bl       #0x3140ec
003b1138: mov      r0, sl
003b113c: mov      r1, r8
003b1140: bl       #0x337a88
003b1144: cmp      r0, #0
003b1148: beq      #0x3b14cc
003b114c: mov      r0, r8
003b1150: bl       #0x3139ac
003b1154: ldr      r1, [r4, #0x10]
003b1158: mov      r0, r6
003b115c: bl       #0x3bdca4
003b1160: mov      r0, r6
003b1164: ldr      r1, [r4, #0x14]
003b1168: bl       #0x3bdbb8
003b116c: ldr      r3, [r5]
003b1170: mov      r0, r5
003b1174: mov      lr, pc
003b1178: ldr      pc, [r3, #0x34]
003b117c: cmp      r0, #0
003b1180: beq      #0x3b1204
003b1184: mov      r0, r5
003b1188: bl       #0x3bc6b8
003b118c: mov      r0, r4
003b1190: mov      r1, r6
003b1194: mov      r2, r5
003b1198: bl       #0x3af77c
003b119c: mov      r0, r4
003b11a0: mov      r1, r6
003b11a4: mov      r2, r5
003b11a8: bl       #0x3afee0
003b11ac: ldr      r3, [r4, #0x1c]
003b11b0: tst      r3, #0x20000000
003b11b4: beq      #0x3b1788
003b11b8: ldr      r3, [r5]
003b11bc: mov      r0, r5
003b11c0: mov      lr, pc
003b11c4: ldr      pc, [r3, #0x28]
003b11c8: cmp      r0, #0
003b11cc: bne      #0x3b1758
003b11d0: ldr      r3, [r6]
003b11d4: mov      r0, r6
003b11d8: mov      lr, pc
003b11dc: ldr      pc, [r3, #0x28]
003b11e0: cmp      r0, #0
003b11e4: bne      #0x3b13e0
003b11e8: ldr      r3, [r7, sb]
003b11ec: ldr      r2, [sp, #0x154]
003b11f0: ldr      r3, [r3]
003b11f4: cmp      r2, r3
003b11f8: bne      #0x3b1d70
003b11fc: add      sp, sp, #0x15c
003b1200: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b1204: ldr      r1, [r4, #8]
003b1208: cmp      r1, #0
003b120c: ble      #0x3b122c
003b1210: ldr      r2, [r4, #0xc]
003b1214: cmp      r2, #0
003b1218: ble      #0x3b122c
003b121c: asr      r1, r1, #8
003b1220: add      r0, r5, #0x560
003b1224: ldr      r3, [r4, #4]
003b1228: bl       #0x3e2720
003b122c: ldr      r2, [r4, #0x1c]
003b1230: ldr      r3, [r5]
003b1234: mov      r0, r5
003b1238: tst      r2, #0x18000000
003b123c: moveq    ip, #0
003b1240: movne    ip, #1
003b1244: str      ip, [sp, #0xc]
003b1248: mov      lr, pc
003b124c: ldr      pc, [r3, #0x28]
003b1250: cmp      r0, #0
003b1254: bne      #0x3b17ec
003b1258: ldrb     r3, [r4, #0x18]
003b125c: tst      r3, #2
003b1260: bne      #0x3b1834
003b1264: tst      r3, #4
003b1268: bne      #0x3b18a4
003b126c: tst      r3, #0x10
003b1270: bne      #0x3b1914
003b1274: tst      r3, #0x80
003b1278: bne      #0x3b196c
003b127c: tst      r3, #0x40
003b1280: beq      #0x3b1304
003b1284: ldr      r3, [r4, #0x1c]
003b1288: add      r1, r6, #0xff0
003b128c: add      r1, r1, #4
003b1290: tst      r3, #0x1000
003b1294: movne    r2, #0xb9
003b1298: moveq    r2, #0x8c
003b129c: add      r0, r6, #0x560
003b12a0: bl       #0x3dedb4
003b12a4: mov      r1, r0
003b12a8: add      r0, r5, #0x4f0
003b12ac: mov      r3, r6
003b12b0: mov      ip, #0
003b12b4: asr      r1, r1, #8
003b12b8: mov      r2, #1
003b12bc: add      r0, r0, #0xc
003b12c0: str      ip, [sp]
003b12c4: bl       #0x3c5ffc
003b12c8: ldr      sl, [r7, fp]
003b12cc: add      r8, sp, #0x7c
003b12d0: mov      r0, sl
003b12d4: bl       #0x337888
003b12d8: ldr      r1, [pc, #0xaa4]
003b12dc: add      r2, sp, #0x28
003b12e0: mov      r0, r8
003b12e4: add      r1, pc, r1
003b12e8: bl       #0x3140ec
003b12ec: mov      r1, r8
003b12f0: mov      r0, sl
003b12f4: bl       #0x337a88
003b12f8: mov      r0, r8
003b12fc: bl       #0x3139ac
003b1300: ldrb     r3, [r4, #0x18]
003b1304: tst      r3, #0x20
003b1308: beq      #0x3b136c
003b130c: ldr      r3, [r4, #0x1c]
003b1310: add      r1, r6, #0xff0
003b1314: add      r1, r1, #4
003b1318: tst      r3, #0x4000
003b131c: movne    r2, #0xbb
003b1320: moveq    r2, #0x8f
003b1324: add      r0, r6, #0x560
003b1328: bl       #0x3dedb4
003b132c: asrs     r1, r0, #8
003b1330: bne      #0x3b19e0
003b1334: ldr      sl, [r7, fp]
003b1338: add      r8, sp, #0x64
003b133c: mov      r0, sl
003b1340: bl       #0x337888
003b1344: ldr      r1, [pc, #0xa3c]
003b1348: add      r2, sp, #0x24
003b134c: mov      r0, r8
003b1350: add      r1, pc, r1
003b1354: bl       #0x3140ec
003b1358: mov      r0, sl
003b135c: mov      r1, r8
003b1360: bl       #0x337a88
003b1364: mov      r0, r8
003b1368: bl       #0x3139ac
003b136c: ldrb     r3, [r4, #0x19]
003b1370: tst      r3, #1
003b1374: beq      #0x3b1184
003b1378: ldr      r3, [r4, #0x1c]
003b137c: add      r1, r6, #0xff0
003b1380: add      r1, r1, #4
003b1384: tst      r3, #0x10000
003b1388: movne    r2, #0xbd
003b138c: moveq    r2, #0x92
003b1390: add      r0, r6, #0x560
003b1394: bl       #0x3dedb4
003b1398: asr      r1, r0, #8
003b139c: add      r0, r5, #0x560
003b13a0: bl       #0x3e2a5c
003b13a4: ldr      sl, [r7, fp]
003b13a8: add      r8, sp, #0x4c
003b13ac: mov      r0, sl
003b13b0: bl       #0x337888
003b13b4: ldr      r1, [pc, #0x9d0]
003b13b8: add      r2, sp, #0x20
003b13bc: mov      r0, r8
003b13c0: add      r1, pc, r1
003b13c4: bl       #0x3140ec
003b13c8: mov      r0, sl
003b13cc: mov      r1, r8
003b13d0: bl       #0x337a88
003b13d4: mov      r0, r8
003b13d8: bl       #0x3139ac
003b13dc: b        #0x3b1184
003b13e0: ldr      r3, [pc, #0x9a8]
003b13e4: mov      r1, r6
003b13e8: mov      r2, #0
003b13ec: ldr      r3, [r7, r3]
003b13f0: ldr      r0, [r3, #0x40]
003b13f4: bl       #0x36eea8
003b13f8: ldrh     r3, [r4, #0x18]
003b13fc: ldr      r6, [r0, #0x670]
003b1400: tst      r3, #0x160
003b1404: bne      #0x3b1a4c
003b1408: ldr      r8, [pc, #0x984]
003b140c: mov      r0, r5
003b1410: ldr      r3, [r5]
003b1414: mov      lr, pc
003b1418: ldr      pc, [r3, #0x34]
003b141c: cmp      r0, #0
003b1420: bne      #0x3b1a38
003b1424: ldr      r2, [r4]
003b1428: ldr      r0, [r7, r8]
003b142c: mov      r3, r6
003b1430: asr      r2, r2, #8
003b1434: mov      r1, #1
003b1438: bl       #0x3790e0
003b143c: b        #0x3b11e8
003b1440: ldr      r3, [r6]
003b1444: mov      r0, r6
003b1448: mov      lr, pc
003b144c: ldr      pc, [r3, #0x54]
003b1450: cmp      r0, #0
003b1454: bne      #0x3b17c4
003b1458: cmp      r8, #0
003b145c: bne      #0x3b10f4
003b1460: cmp      r5, #0
003b1464: beq      #0x3b1aa8
003b1468: ldr      sl, [r5, #0x108]
003b146c: ldr      r8, [r6, #0x108]
003b1470: lsr      r3, sl, #0x1f
003b1474: orrs     r3, r3, r8, lsr #31
003b1478: beq      #0x3b14a0
003b147c: ldr      r3, [pc, #0x914]
003b1480: ldr      r3, [r7, r3]
003b1484: ldr      r3, [r3]
003b1488: cmp      r3, #2
003b148c: moveq    r3, #0
003b1490: streq    r3, [r3]
003b1494: beq      #0x3b14a0
003b1498: cmp      r3, #1
003b149c: beq      #0x3b1d08
003b14a0: bl       #0x80b1bc
003b14a4: mov      r1, sl
003b14a8: mov      fp, r0
003b14ac: mov      r2, r4
003b14b0: mov      r0, r8
003b14b4: mov      r3, #1
003b14b8: bl       #0x3af330
003b14bc: mov      r1, r0
003b14c0: mov      r0, fp
003b14c4: bl       #0x80e2a4
003b14c8: b        #0x3b10f4
003b14cc: ldr      r3, [pc, #0x8c8]
003b14d0: add      ip, sp, #0x124
003b14d4: mov      r0, sl
003b14d8: add      r3, pc, r3
003b14dc: str      ip, [sp, #0xc]
003b14e0: str      r3, [sp, #8]
003b14e4: bl       #0x337888
003b14e8: ldr      r3, [sp, #8]
003b14ec: add      r2, sp, #0x44
003b14f0: ldr      r0, [sp, #0xc]
003b14f4: mov      r1, r3
003b14f8: bl       #0x3140ec
003b14fc: mov      r0, sl
003b1500: ldr      r1, [sp, #0xc]
003b1504: bl       #0x337a88
003b1508: cmp      r0, #0
003b150c: ldr      r3, [sp, #8]
003b1510: beq      #0x3b17d0
003b1514: ldr      r3, [r5]
003b1518: mov      r0, r5
003b151c: mov      lr, pc
003b1520: ldr      pc, [r3, #0x28]
003b1524: cmp      r0, #0
003b1528: bne      #0x3b1a2c
003b152c: movw     r3, #0x14f0
003b1530: ldrb     r3, [r5, r3]
003b1534: cmp      r3, #0
003b1538: bne      #0x3b1a2c
003b153c: ldr      r0, [sp, #0xc]
003b1540: bl       #0x3139ac
003b1544: mov      r0, r8
003b1548: bl       #0x3139ac
003b154c: ldr      r3, [r4, #0x1c]
003b1550: tst      r3, #0x400000
003b1554: bne      #0x3b1a00
003b1558: ldr      r8, [r4]
003b155c: cmp      r8, #0
003b1560: ble      #0x3b1154
003b1564: ldr      r2, [pc, #0x824]
003b1568: ldr      r3, [r7, r2]
003b156c: str      r2, [sp, #0xc]
003b1570: ldr      r3, [r3, #0x40]
003b1574: ldr      sl, [r3, #0x6c4]
003b1578: cmp      sl, #1
003b157c: ble      #0x3b15c4
003b1580: mov      r0, r6
003b1584: bl       #0x3a3064
003b1588: cmp      r0, #0
003b158c: beq      #0x3b1ad8
003b1590: sub      r0, sl, #1
003b1594: bl       #0x30e964
003b1598: ldr      r3, [pc, #0x800]
003b159c: ldr      r3, [r7, r3]
003b15a0: ldr      r3, [r3]
003b15a4: ldr      r1, [r3, #0x34]
003b15a8: bl       #0x30ed6c
003b15ac: mov      r1, #0x3f800000
003b15b0: bl       #0x30eba4
003b15b4: bl       #0x30e4cc
003b15b8: ldr      r8, [r4]
003b15bc: mul      r8, r8, r0
003b15c0: str      r8, [r4]
003b15c4: mov      r0, r6
003b15c8: bl       #0x3bd394
003b15cc: mov      sl, r0
003b15d0: ldr      r0, [r4]
003b15d4: bl       #0x30e964
003b15d8: mov      r1, #0x3b800000
003b15dc: bl       #0x30ed6c
003b15e0: mov      r1, r0
003b15e4: mov      r0, sl
003b15e8: bl       #0x30ed6c
003b15ec: add      sl, r5, #0x3c8
003b15f0: mov      r2, r0
003b15f4: mov      r1, r6
003b15f8: mov      r0, sl
003b15fc: bl       #0x3d7c68
003b1600: mov      r1, #0
003b1604: bl       #0x30e2f8
003b1608: cmp      r0, #0
003b160c: bne      #0x3b1a64
003b1610: ldrb     r3, [r4, #0x18]
003b1614: ldr      r2, [r5, #0x110]
003b1618: and      r3, r3, #0x80
003b161c: uxtb     r3, r3
003b1620: cmp      r3, #0
003b1624: ldrne    r3, [r4, #0x1c]
003b1628: ubfxne   r3, r3, #0x14, #1
003b162c: cmn      r2, #1
003b1630: strb     r3, [r5, #0x53b]
003b1634: beq      #0x3b1b2c
003b1638: ldr      r3, [r4, #0x1c]
003b163c: tst      r3, #0x200000
003b1640: beq      #0x3b1680
003b1644: ldr      r8, [r4, #0x24]
003b1648: cmn      r8, #1
003b164c: addne    r8, r8, #0x7c
003b1650: beq      #0x3b1cd0
003b1654: mov      r0, r5
003b1658: bl       #0x3935dc
003b165c: ldr      r3, [pc, #0x740]
003b1660: mov      ip, #0
003b1664: mov      r2, r0
003b1668: mov      r1, r8
003b166c: ldr      r0, [r7, r3]
003b1670: add      r3, r5, #0x16c
003b1674: str      ip, [sp, #4]
003b1678: str      ip, [sp]
003b167c: bl       #0x495888
003b1680: ldr      r3, [r5]
003b1684: mov      r0, r5
003b1688: mov      lr, pc
003b168c: ldr      pc, [r3, #0x34]
003b1690: cmp      r0, #0
003b1694: beq      #0x3b16b4
003b1698: ldrb     r3, [r4, #0x18]
003b169c: ldrb     r2, [r4, #0x19]
003b16a0: and      r3, r3, #0xbf
003b16a4: bfc      r2, #0, #1
003b16a8: bfc      r3, #5, #1
003b16ac: strb     r2, [r4, #0x19]
003b16b0: strb     r3, [r4, #0x18]
003b16b4: ldrb     r3, [r4, #0x18]
003b16b8: tst      r3, #8
003b16bc: beq      #0x3b1154
003b16c0: ldr      r3, [r6]
003b16c4: mov      r0, r6
003b16c8: mov      lr, pc
003b16cc: ldr      pc, [r3, #0x28]
003b16d0: cmp      r0, #0
003b16d4: beq      #0x3b1154
003b16d8: ldr      r3, [sp, #0xc]
003b16dc: ldr      r0, [r7, r3]
003b16e0: bl       #0x31f594
003b16e4: cmp      r0, #0
003b16e8: beq      #0x3b1154
003b16ec: ldr      r8, [r0, #0x128]
003b16f0: cmp      r8, #0
003b16f4: beq      #0x3b1154
003b16f8: mov      r0, r8
003b16fc: mov      r1, r6
003b1700: bl       #0x40f980
003b1704: cmp      r0, #0
003b1708: beq      #0x3b1154
003b170c: mov      r3, #0
003b1710: mov      r0, r6
003b1714: add      r1, sp, #0x14
003b1718: str      r3, [sp, #0x1c]
003b171c: str      r3, [sp, #0x14]
003b1720: str      r3, [sp, #0x18]
003b1724: bl       #0x393ae4
003b1728: ldr      r3, [pc, #0x678]
003b172c: ldr      r1, [r8, #0x80]
003b1730: mov      ip, #0x1c
003b1734: ldr      r3, [r7, r3]
003b1738: mov      r0, r8
003b173c: mov      r2, #0
003b1740: ldr      lr, [r3]
003b1744: mov      r3, #1
003b1748: mla      r1, ip, r1, lr
003b174c: ldr      r1, [r1, #0xc]
003b1750: bl       #0x40f904
003b1754: b        #0x3b1154
003b1758: ldr      r3, [pc, #0x630]
003b175c: mov      r1, r5
003b1760: mov      r2, #0
003b1764: ldr      r3, [r7, r3]
003b1768: ldr      r8, [pc, #0x624]
003b176c: ldr      r0, [r3, #0x40]
003b1770: bl       #0x36eea8
003b1774: mov      r1, #3
003b1778: ldr      r2, [r0, #0x670]
003b177c: ldr      r0, [r7, r8]
003b1780: bl       #0x3790ec
003b1784: b        #0x3b11d0
003b1788: add      r0, r6, #0x3c8
003b178c: mov      r1, r6
003b1790: mov      r2, r5
003b1794: mov      r3, r4
003b1798: ldr      ip, [r6, #0x3c8]
003b179c: mov      lr, pc
003b17a0: ldr      pc, [ip, #0xb4]
003b17a4: ldr      ip, [r5, #0x3c8]
003b17a8: add      r0, r5, #0x3c8
003b17ac: mov      r1, r6
003b17b0: mov      r2, r5
003b17b4: mov      r3, r4
003b17b8: mov      lr, pc
003b17bc: ldr      pc, [ip, #0xb4]
003b17c0: b        #0x3b11b8
003b17c4: cmp      r8, #0
003b17c8: beq      #0x3b11e8
003b17cc: b        #0x3b10f4
003b17d0: mov      r1, r3
003b17d4: ldr      r3, [pc, #0x5b4]
003b17d8: ldr      r0, [r7, r3]
003b17dc: bl       #0x320e14
003b17e0: cmp      r0, #0
003b17e4: beq      #0x3b152c
003b17e8: b        #0x3b1514
003b17ec: add      r0, r5, #0x4f0
003b17f0: add      r0, r0, #0xc
003b17f4: mov      r1, #0
003b17f8: bl       #0x3c0260
003b17fc: cmp      r0, #0
003b1800: beq      #0x3b1258
003b1804: ldrb     r3, [r4, #0x18]
003b1808: tst      r3, #0x16
003b180c: bne      #0x3b125c
003b1810: ldr      r2, [r4]
003b1814: cmp      r2, #0
003b1818: orrle    r3, r3, #2
003b181c: orrgt    r3, r3, #0x10
003b1820: strble   r3, [r4, #0x18]
003b1824: uxtble   r3, r3
003b1828: strbgt   r3, [r4, #0x18]
003b182c: tst      r3, #2
003b1830: beq      #0x3b1264
003b1834: add      r0, r5, #0x4f0
003b1838: add      r0, r0, #0xc
003b183c: mov      r1, r6
003b1840: mov      r2, #0
003b1844: bl       #0x3c5b3c
003b1848: ldr      r3, [r5]
003b184c: mov      r0, r5
003b1850: mov      lr, pc
003b1854: ldr      pc, [r3, #0x28]
003b1858: cmp      r0, #0
003b185c: bne      #0x3b1bf0
003b1860: ldr      sl, [r7, fp]
003b1864: add      r8, sp, #0xdc
003b1868: mov      r0, sl
003b186c: bl       #0x337888
003b1870: ldr      r1, [pc, #0x534]
003b1874: add      r2, sp, #0x38
003b1878: mov      r0, r8
003b187c: add      r1, pc, r1
003b1880: bl       #0x3140ec
003b1884: mov      r1, r8
003b1888: mov      r0, sl
003b188c: bl       #0x337a88
003b1890: mov      r0, r8
003b1894: bl       #0x3139ac
003b1898: ldrb     r3, [r4, #0x18]
003b189c: tst      r3, #4
003b18a0: beq      #0x3b126c
003b18a4: add      r0, r5, #0x4f0
003b18a8: add      r0, r0, #0xc
003b18ac: mov      r1, r6
003b18b0: mov      r2, #0
003b18b4: bl       #0x3c5c60
003b18b8: ldr      r3, [r5]
003b18bc: mov      r0, r5
003b18c0: mov      lr, pc
003b18c4: ldr      pc, [r3, #0x28]
003b18c8: cmp      r0, #0
003b18cc: bne      #0x3b1b80
003b18d0: ldr      sl, [r7, fp]
003b18d4: add      r8, sp, #0xc4
003b18d8: mov      r0, sl
003b18dc: bl       #0x337888
003b18e0: ldr      r1, [pc, #0x4c8]
003b18e4: add      r2, sp, #0x34
003b18e8: mov      r0, r8
003b18ec: add      r1, pc, r1
003b18f0: bl       #0x3140ec
003b18f4: mov      r1, r8
003b18f8: mov      r0, sl
003b18fc: bl       #0x337a88
003b1900: mov      r0, r8
003b1904: bl       #0x3139ac
003b1908: ldrb     r3, [r4, #0x18]
003b190c: tst      r3, #0x10
003b1910: beq      #0x3b1274
003b1914: add      r0, r5, #0x4f0
003b1918: mov      r1, r6
003b191c: ldr      r2, [sp, #0xc]
003b1920: add      r0, r0, #0xc
003b1924: bl       #0x3c5d84
003b1928: ldr      sl, [r7, fp]
003b192c: add      r8, sp, #0xac
003b1930: mov      r0, sl
003b1934: bl       #0x337888
003b1938: ldr      r1, [pc, #0x474]
003b193c: add      r2, sp, #0x30
003b1940: mov      r0, r8
003b1944: add      r1, pc, r1
003b1948: bl       #0x3140ec
003b194c: mov      r1, r8
003b1950: mov      r0, sl
003b1954: bl       #0x337a88
003b1958: mov      r0, r8
003b195c: bl       #0x3139ac
003b1960: ldrb     r3, [r4, #0x18]
003b1964: tst      r3, #0x80
003b1968: beq      #0x3b127c
003b196c: ldr      r1, [r4, #0x1c]
003b1970: add      r0, r5, #0x4f0
003b1974: add      r0, r0, #0xc
003b1978: ubfx     r1, r1, #0x14, #1
003b197c: mov      r2, r6
003b1980: ldr      r3, [sp, #0xc]
003b1984: bl       #0x3c5ea0
003b1988: ldr      r3, [r5]
003b198c: mov      r0, r5
003b1990: mov      lr, pc
003b1994: ldr      pc, [r3, #0x28]
003b1998: cmp      r0, #0
003b199c: bne      #0x3b1c60
003b19a0: ldr      sl, [r7, fp]
003b19a4: add      r8, sp, #0x94
003b19a8: mov      r0, sl
003b19ac: bl       #0x337888
003b19b0: ldr      r1, [pc, #0x400]
003b19b4: add      r2, sp, #0x2c
003b19b8: mov      r0, r8
003b19bc: add      r1, pc, r1
003b19c0: bl       #0x3140ec
003b19c4: mov      r1, r8
003b19c8: mov      r0, sl
003b19cc: bl       #0x337a88
003b19d0: mov      r0, r8
003b19d4: bl       #0x3139ac
003b19d8: ldrb     r3, [r4, #0x18]
003b19dc: b        #0x3b127c
003b19e0: ldr      ip, [sp, #0xc]
003b19e4: add      r0, r5, #0x4f0
003b19e8: add      r0, r0, #0xc
003b19ec: mov      r2, #1
003b19f0: mov      r3, r6
003b19f4: str      ip, [sp]
003b19f8: bl       #0x3c6144
003b19fc: b        #0x3b1334
003b1a00: ldr      r3, [r6, #0x39c]
003b1a04: ldr      r2, [r4]
003b1a08: add      r0, r6, #0x37c
003b1a0c: lsl      r3, r3, #8
003b1a10: cmp      r3, r2
003b1a14: movge    r3, r2
003b1a18: asr      r1, r3, #8
003b1a1c: str      r3, [r4]
003b1a20: rsb      r1, r1, #0
003b1a24: bl       #0x3fe164
003b1a28: b        #0x3b1558
003b1a2c: ldr      r0, [sp, #0xc]
003b1a30: bl       #0x3139ac
003b1a34: b        #0x3b114c
003b1a38: ldr      r0, [r7, r8]
003b1a3c: mov      r1, #0
003b1a40: mov      r2, r6
003b1a44: bl       #0x3790ec
003b1a48: b        #0x3b1424
003b1a4c: ldr      r8, [pc, #0x340]
003b1a50: mov      r1, #2
003b1a54: mov      r2, r6
003b1a58: ldr      r0, [r7, r8]
003b1a5c: bl       #0x3790ec
003b1a60: b        #0x3b140c
003b1a64: ldr      r3, [r7, fp]
003b1a68: add      sl, sp, #0x10c
003b1a6c: mov      r0, r3
003b1a70: str      r3, [sp, #8]
003b1a74: bl       #0x337888
003b1a78: ldr      r1, [pc, #0x33c]
003b1a7c: add      r2, sp, #0x40
003b1a80: mov      r0, sl
003b1a84: add      r1, pc, r1
003b1a88: bl       #0x3140ec
003b1a8c: ldr      r3, [sp, #8]
003b1a90: mov      r1, sl
003b1a94: mov      r0, r3
003b1a98: bl       #0x337a88
003b1a9c: mov      r0, sl
003b1aa0: bl       #0x3139ac
003b1aa4: b        #0x3b1610
003b1aa8: ldr      r3, [pc, #0x2e8]
003b1aac: ldr      r3, [r7, r3]
003b1ab0: ldr      r3, [r3]
003b1ab4: cmp      r3, #2
003b1ab8: streq    r5, [r5]
003b1abc: beq      #0x3b1ac8
003b1ac0: cmp      r3, #1
003b1ac4: beq      #0x3b1d3c
003b1ac8: ldr      r8, [r6, #0x108]
003b1acc: mov      r3, #1
003b1ad0: mvn      sl, #0
003b1ad4: b        #0x3b1474
003b1ad8: ldr      r3, [r6]
003b1adc: mov      r0, r6
003b1ae0: mov      lr, pc
003b1ae4: ldr      pc, [r3, #0x28]
003b1ae8: cmp      r0, #0
003b1aec: beq      #0x3b15c4
003b1af0: sub      r0, sl, #1
003b1af4: bl       #0x30e964
003b1af8: ldr      r3, [pc, #0x2a0]
003b1afc: ldr      r3, [r7, r3]
003b1b00: ldr      r3, [r3]
003b1b04: ldr      r1, [r3, #0x38]
003b1b08: bl       #0x30ed6c
003b1b0c: mov      r1, #0x3f800000
003b1b10: bl       #0x30eba4
003b1b14: bl       #0x30e4cc
003b1b18: mov      r1, r0
003b1b1c: mov      r0, r8
003b1b20: bl       #0x30e2a4
003b1b24: mov      r8, r0
003b1b28: b        #0x3b15c4
003b1b2c: ldr      r3, [r7, fp]
003b1b30: add      sl, sp, #0xf4
003b1b34: mov      r0, r3
003b1b38: str      r3, [sp, #8]
003b1b3c: bl       #0x337888
003b1b40: ldr      r1, [pc, #0x278]
003b1b44: add      r2, sp, #0x3c
003b1b48: mov      r0, sl
003b1b4c: add      r1, pc, r1
003b1b50: bl       #0x3140ec
003b1b54: ldr      r3, [sp, #8]
003b1b58: mov      r1, sl
003b1b5c: mov      r0, r3
003b1b60: bl       #0x337a88
003b1b64: mov      r0, sl
003b1b68: bl       #0x3139ac
003b1b6c: mov      r0, r5
003b1b70: mov      r1, r8
003b1b74: mov      r2, r6
003b1b78: bl       #0x3a8bc4
003b1b7c: b        #0x3b1638
003b1b80: add      r8, r5, #0x560
003b1b84: mov      r0, r8
003b1b88: mov      r1, #0xd6
003b1b8c: mov      r2, #1
003b1b90: bl       #0x3e0798
003b1b94: ldr      r3, [pc, #0x228]
003b1b98: mov      r0, r8
003b1b9c: mov      r1, #0xd6
003b1ba0: ldr      r3, [r7, r3]
003b1ba4: mov      r2, #0
003b1ba8: ldr      r8, [r3]
003b1bac: bl       #0x3df6e0
003b1bb0: cmp      r0, #0x1f4
003b1bb4: blt      #0x3b18d0
003b1bb8: ldr      r3, [pc, #0x1d0]
003b1bbc: mov      r1, r5
003b1bc0: ldr      r3, [r7, r3]
003b1bc4: ldr      r0, [r3, #0x40]
003b1bc8: bl       #0x36effc
003b1bcc: cmp      r0, #0
003b1bd0: beq      #0x3b18d0
003b1bd4: ldr      r0, [pc, #0x1ec]
003b1bd8: add      r0, pc, r0
003b1bdc: bl       #0x3a3f70
003b1be0: mov      r1, r0
003b1be4: mov      r0, r8
003b1be8: bl       #0x3813b8
003b1bec: b        #0x3b18d0
003b1bf0: add      r8, r5, #0x560
003b1bf4: mov      r0, r8
003b1bf8: mov      r1, #0xd7
003b1bfc: mov      r2, #1
003b1c00: bl       #0x3e0798
003b1c04: ldr      r3, [pc, #0x1b8]
003b1c08: mov      r0, r8
003b1c0c: mov      r1, #0xd7
003b1c10: ldr      r3, [r7, r3]
003b1c14: mov      r2, #0
003b1c18: ldr      r8, [r3]
003b1c1c: bl       #0x3df6e0
003b1c20: cmp      r0, #0x1f4
003b1c24: blt      #0x3b1860
003b1c28: ldr      r3, [pc, #0x160]
003b1c2c: mov      r1, r5
003b1c30: ldr      r3, [r7, r3]
003b1c34: ldr      r0, [r3, #0x40]
003b1c38: bl       #0x36effc
003b1c3c: cmp      r0, #0
003b1c40: beq      #0x3b1860
003b1c44: ldr      r0, [pc, #0x180]
003b1c48: add      r0, pc, r0
003b1c4c: bl       #0x3a3f70
003b1c50: mov      r1, r0
003b1c54: mov      r0, r8
003b1c58: bl       #0x3813b8
003b1c5c: b        #0x3b1860
003b1c60: add      r8, r5, #0x560
003b1c64: mov      r0, r8
003b1c68: mov      r1, #0xde
003b1c6c: mov      r2, #1
003b1c70: bl       #0x3e0798
003b1c74: ldr      r3, [pc, #0x148]
003b1c78: mov      r0, r8
003b1c7c: mov      r1, #0xde
003b1c80: ldr      r3, [r7, r3]
003b1c84: mov      r2, #0
003b1c88: ldr      r8, [r3]
003b1c8c: bl       #0x3df6e0
003b1c90: cmp      r0, #0x31
003b1c94: ble      #0x3b19a0
003b1c98: ldr      r3, [pc, #0xf0]
003b1c9c: mov      r1, r5
003b1ca0: ldr      r3, [r7, r3]
003b1ca4: ldr      r0, [r3, #0x40]
003b1ca8: bl       #0x36effc
003b1cac: cmp      r0, #0
003b1cb0: beq      #0x3b19a0
003b1cb4: ldr      r0, [pc, #0x114]
003b1cb8: add      r0, pc, r0
003b1cbc: bl       #0x3a3f70
003b1cc0: mov      r1, r0
003b1cc4: mov      r0, r8
003b1cc8: bl       #0x3813b8
003b1ccc: b        #0x3b19a0
003b1cd0: ldr      r3, [r5]
003b1cd4: mov      r0, r5
003b1cd8: mov      lr, pc
003b1cdc: ldr      pc, [r3, #0x34]
003b1ce0: cmp      r0, #0
003b1ce4: beq      #0x3b1cf8
003b1ce8: mov      r0, r5
003b1cec: bl       #0x3a3368
003b1cf0: mov      r8, r0
003b1cf4: b        #0x3b1654
003b1cf8: mov      r0, r5
003b1cfc: bl       #0x3a33d0
003b1d00: mov      r8, r0
003b1d04: b        #0x3b1654
003b1d08: ldr      r0, [pc, #0xc4]
003b1d0c: ldr      r1, [pc, #0xc4]
003b1d10: ldr      r2, [pc, #0xc4]
003b1d14: ldr      r0, [r7, r0]
003b1d18: ldr      r3, [pc, #0xc0]
003b1d1c: movw     ip, #0x2d2
003b1d20: add      r1, pc, r1
003b1d24: add      r2, pc, r2
003b1d28: add      r3, pc, r3
003b1d2c: add      r0, r0, #0xa8
003b1d30: str      ip, [sp]
003b1d34: bl       #0x30e004
003b1d38: b        #0x3b14a0
003b1d3c: ldr      r0, [pc, #0x90]
003b1d40: ldr      r1, [pc, #0x9c]
003b1d44: ldr      r2, [pc, #0x9c]
003b1d48: ldr      r0, [r7, r0]
003b1d4c: ldr      r3, [pc, #0x98]
003b1d50: mov      ip, #0x2c8
003b1d54: add      r1, pc, r1
003b1d58: add      r2, pc, r2
003b1d5c: add      r3, pc, r3
003b1d60: add      r0, r0, #0xa8
003b1d64: str      ip, [sp]
003b1d68: bl       #0x30e004
003b1d6c: b        #0x3b1ac8
003b1d70: bl       #0x30e310
003b1d74: subseq   r3, lr, ip, asr #19
003b1d78: andeq    r4, r0, ip, lsr #1
003b1d7c: andeq    r0, r0, r4, lsl #17
003b1d80: subseq   r2, r1, r0, ror fp
003b1d84: ldrsbeq  r2, [r1], #-0x94
003b1d88: subseq   r2, r1, r8, ror #18
003b1d8c: ldrsheq  r2, [r1], #-0x88
003b1d90: strdeq   r3, r4, [r0], -r4
003b1d94: andeq    r2, r0, r4, lsl r7
003b1d98: andeq    r3, r0, r0, asr #19
003b1d9c: ldrsbeq  r2, [r1], #-0x78
003b1da0: andeq    r3, r0, r8, asr #5
003b1da4: andeq    r1, r0, r8, lsl #22
003b1da8: ldrdeq   r3, r4, [r0], -r4
003b1dac: subseq   r2, r1, ip, lsr r4
003b1db0: subseq   r2, r1, ip, asr #7
003b1db4: subseq   r2, r1, r4, ror r3
003b1db8: ldrsheq  r2, [r1], #-0x2c
003b1dbc: subseq   r2, r1, ip, asr r2
003b1dc0: subseq   r2, r1, ip, ror #2
003b1dc4: andeq    r1, r0, r0, ror sp
003b1dc8: subseq   r2, r1, r0, lsr r1
003b1dcc: ldrheq   r2, [r1], #-0
003b1dd0: subseq   r2, r1, r0, rrx
003b1dd4: andeq    r1, r0, r0, asr #19
003b1dd8: ldrheq   ip, [r0], #-0x68
003b1ddc: subseq   r1, r1, r4, asr #30
003b1de0: subseq   r1, r1, r8, ror #29
003b1de4: subseq   ip, r0, r4, lsl #13
003b1de8: subseq   r1, r1, r0, lsr #29
003b1dec: ldrheq   r1, [r1], #-0xe4

# _Z16CF_CalcDotDamageP9CharacterS0_b
003b09c4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b09c8: add      r4, r1, #0xff0
003b09cc: subs     r7, r3, #0
003b09d0: add      r4, r4, #4
003b09d4: add      r6, r1, #0x560
003b09d8: mov      sl, r2
003b09dc: mov      r1, r4
003b09e0: movne    r2, #0xb5
003b09e4: moveq    r2, #0x7d
003b09e8: mov      r5, r0
003b09ec: mov      r0, r6
003b09f0: bl       #0x3dedb4
003b09f4: cmp      r0, #0
003b09f8: mov      r8, r0
003b09fc: mvnle    r4, #0
003b0a00: movle    r7, #0
003b0a04: ble      #0x3b0a78
003b0a08: cmp      r7, #0
003b0a0c: beq      #0x3b0a8c
003b0a10: mov      r1, r4
003b0a14: mov      r2, #0xb3
003b0a18: mov      r0, r6
003b0a1c: bl       #0x3dedb4
003b0a20: mov      r1, r4
003b0a24: mov      r7, r0
003b0a28: mov      r2, #0xb4
003b0a2c: mov      r0, r6
003b0a30: bl       #0x3dedb4
003b0a34: rsb      r0, r7, r0
003b0a38: bl       #0x3af6d8
003b0a3c: mov      r1, r4
003b0a40: add      r7, r0, r7
003b0a44: mov      r2, #0xb2
003b0a48: mov      r0, r6
003b0a4c: bl       #0x3dedb4
003b0a50: asr      r4, r0, #8
003b0a54: cmn      r4, #1
003b0a58: beq      #0x3b0a78
003b0a5c: add      r1, sl, #0xff0
003b0a60: add      r1, r1, #4
003b0a64: add      r0, sl, #0x560
003b0a68: add      r2, r4, #0x4a
003b0a6c: bl       #0x3dedb4
003b0a70: rsb      r7, r0, r7
003b0a74: bic      r7, r7, r7, asr #31
003b0a78: str      r8, [r5]
003b0a7c: str      r7, [r5, #4]
003b0a80: str      r4, [r5, #8]
003b0a84: mov      r0, r5
003b0a88: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0a8c: mov      r1, r4
003b0a90: mov      r2, #0x7b
003b0a94: mov      r0, r6
003b0a98: bl       #0x3dedb4
003b0a9c: mov      r1, r4
003b0aa0: mov      r7, r0
003b0aa4: mov      r2, #0x7c
003b0aa8: mov      r0, r6
003b0aac: bl       #0x3dedb4
003b0ab0: rsb      r0, r7, r0
003b0ab4: bl       #0x3af6d8
003b0ab8: mvn      r4, #0
003b0abc: add      r7, r7, r0
003b0ac0: str      r8, [r5]
003b0ac4: str      r7, [r5, #4]
003b0ac8: str      r4, [r5, #8]
003b0acc: mov      r0, r5
003b0ad0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN9Character11F_DotAttackERNS_12AttackResultEPS_S2_ii
003b2e68: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b2e6c: ldr      r4, [pc, #0x160]
003b2e70: ldr      r6, [pc, #0x160]
003b2e74: subs     r7, r1, #0
003b2e78: add      r4, pc, r4
003b2e7c: ldr      r1, [r4, r6]
003b2e80: mov      r5, r2
003b2e84: sub      sp, sp, #0x34
003b2e88: ldr      r2, [r1]
003b2e8c: mov      sb, r0
003b2e90: mov      fp, r3
003b2e94: str      r2, [sp, #0x2c]
003b2e98: beq      #0x3b2f28
003b2e9c: cmp      r5, #0
003b2ea0: beq      #0x3b2f7c
003b2ea4: ldr      r3, [pc, #0x130]
003b2ea8: add      r8, sp, #0x14
003b2eac: ldr      sl, [r4, r3]
003b2eb0: mov      r0, sl
003b2eb4: bl       #0x337888
003b2eb8: ldr      r1, [pc, #0x120]
003b2ebc: add      r2, sp, #0x10
003b2ec0: mov      r0, r8
003b2ec4: add      r1, pc, r1
003b2ec8: bl       #0x3140ec
003b2ecc: mov      r1, r8
003b2ed0: mov      r0, sl
003b2ed4: bl       #0x337a88
003b2ed8: mov      r0, r8
003b2edc: bl       #0x3139ac
003b2ee0: mvn      ip, #0
003b2ee4: str      ip, [sp]
003b2ee8: ldr      ip, [sp, #0x58]
003b2eec: mov      r3, #0x20000000
003b2ef0: mov      r2, r5
003b2ef4: add      r3, r3, #0x80000
003b2ef8: mov      r0, sb
003b2efc: mov      r1, r7
003b2f00: str      ip, [sp, #4]
003b2f04: str      fp, [sp, #8]
003b2f08: bl       #0x3b2638
003b2f0c: ldr      r3, [r4, r6]
003b2f10: ldr      r2, [sp, #0x2c]
003b2f14: ldr      r3, [r3]
003b2f18: cmp      r2, r3
003b2f1c: bne      #0x3b2fd0
003b2f20: add      sp, sp, #0x34
003b2f24: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b2f28: ldr      r3, [pc, #0xb4]
003b2f2c: ldr      r3, [r4, r3]
003b2f30: ldr      r3, [r3]
003b2f34: cmp      r3, #2
003b2f38: streq    r7, [r7]
003b2f3c: beq      #0x3b2e9c
003b2f40: cmp      r3, #1
003b2f44: bne      #0x3b2e9c
003b2f48: ldr      r0, [pc, #0x98]
003b2f4c: ldr      r1, [pc, #0x98]
003b2f50: ldr      r2, [pc, #0x98]
003b2f54: ldr      r0, [r4, r0]
003b2f58: ldr      r3, [pc, #0x94]
003b2f5c: movw     ip, #0x289
003b2f60: add      r1, pc, r1
003b2f64: add      r2, pc, r2
003b2f68: add      r3, pc, r3
003b2f6c: add      r0, r0, #0xa8
003b2f70: str      ip, [sp]
003b2f74: bl       #0x30e004
003b2f78: b        #0x3b2e9c
003b2f7c: ldr      r3, [pc, #0x60]
003b2f80: ldr      r3, [r4, r3]
003b2f84: ldr      r3, [r3]
003b2f88: cmp      r3, #2
003b2f8c: streq    r5, [r5]
003b2f90: beq      #0x3b2ea4
003b2f94: cmp      r3, #1
003b2f98: bne      #0x3b2ea4
003b2f9c: ldr      r0, [pc, #0x44]
003b2fa0: ldr      r1, [pc, #0x50]
003b2fa4: ldr      r2, [pc, #0x50]
003b2fa8: ldr      r0, [r4, r0]
003b2fac: ldr      r3, [pc, #0x4c]
003b2fb0: movw     ip, #0x28a
003b2fb4: add      r1, pc, r1
003b2fb8: add      r2, pc, r2
003b2fbc: add      r3, pc, r3
003b2fc0: add      r0, r0, #0xa8
003b2fc4: str      ip, [sp]
003b2fc8: bl       #0x30e004
003b2fcc: b        #0x3b2ea4
003b2fd0: bl       #0x30e310
003b2fd4: subseq   r1, lr, r8, lsl ip
003b2fd8: andeq    r4, r0, ip, lsr #1
003b2fdc: andeq    r0, r0, r4, lsl #17
003b2fe0: ldrsheq  r0, [r1], #-0xd4
003b2fe4: andeq    r3, r0, r0, asr #19
003b2fe8: andeq    r1, r0, r0, asr #19
003b2fec: subseq   fp, r0, r8, ror r4
003b2ff0: subseq   r0, r1, ip, ror #26
003b2ff4: subseq   r0, r1, r8, lsr #25
003b2ff8: subseq   fp, r0, r4, lsr #8
003b2ffc: subseq   lr, r0, r8, asr #18
003b3000: subseq   r0, r1, r4, asr ip


# _ZN9Character14CancelSneakingEv
003bc6b8: push     {r4, r5, r6, lr}
003bc6bc: ldr      r3, [r0]
003bc6c0: mov      r5, r0
003bc6c4: mov      lr, pc
003bc6c8: ldr      pc, [r3, #0x28]
003bc6cc: ldr      r4, [pc, #0xa8]
003bc6d0: cmp      r0, #0
003bc6d4: add      r4, pc, r4
003bc6d8: bne      #0x3bc760
003bc6dc: mov      r0, r5
003bc6e0: bl       #0x3bc690
003bc6e4: cmp      r0, #0
003bc6e8: bne      #0x3bc6f0
003bc6ec: pop      {r4, r5, r6, pc}
003bc6f0: mov      r0, r5
003bc6f4: bl       #0x3bc5fc
003bc6f8: ldr      r2, [r0, #4]
003bc6fc: cmp      r2, #0
003bc700: beq      #0x3bc6ec
003bc704: ldr      r1, [pc, #0x74]
003bc708: ldr      r0, [r0, #8]
003bc70c: mov      r3, #0x4c
003bc710: ldr      ip, [r4, r1]
003bc714: ldr      r1, [r0]
003bc718: ldr      ip, [ip]
003bc71c: mla      r1, r3, r1, ip
003bc720: ldr      r1, [r1, #0x1c]
003bc724: ands     r1, r1, #0x2000000
003bc728: movne    r1, #0
003bc72c: bne      #0x3bc754
003bc730: mov      r4, r3
003bc734: add      r1, r1, #1
003bc738: cmp      r1, r2
003bc73c: beq      #0x3bc6ec
003bc740: ldr      r3, [r0, r1, lsl #2]
003bc744: mla      r3, r4, r3, ip
003bc748: ldr      r3, [r3, #0x1c]
003bc74c: tst      r3, #0x2000000
003bc750: beq      #0x3bc734
003bc754: add      r0, r5, #0x3c8
003bc758: pop      {r4, r5, r6, lr}
003bc75c: b        #0x3d84e0
003bc760: add      r0, r5, #0x560
003bc764: mov      r1, #0x92
003bc768: mov      r2, #0
003bc76c: bl       #0x3e101c
003bc770: mov      r3, #1
003bc774: strb     r3, [r5, #0x415]
003bc778: b        #0x3bc6dc
003bc77c: ldrheq   r8, [sp], #-0x3c
003bc780: andeq    r4, r0, ip, lsl r4

# _Z16CF_SetCombatantsP9CharacterS0_ibb
003b059c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b05a0: ldr      r4, [pc, #0x70]
003b05a4: mov      r5, r1
003b05a8: add      r1, r0, #0xff0
003b05ac: add      r4, pc, r4
003b05b0: mov      r6, r2
003b05b4: add      r1, r1, #4
003b05b8: str      r0, [r4, #0x1c]
003b05bc: mov      r2, #0x13
003b05c0: str      r5, [r4, #0x20]
003b05c4: add      r0, r0, #0x560
003b05c8: mov      r8, r3
003b05cc: ldrb     r7, [sp, #0x20]
003b05d0: bl       #0x3dedb4
003b05d4: add      r1, r5, #0xff0
003b05d8: mov      sl, r0
003b05dc: mov      r2, #0x13
003b05e0: add      r1, r1, #4
003b05e4: add      r0, r5, #0x560
003b05e8: bl       #0x3dedb4
003b05ec: rsb      r0, r0, sl
003b05f0: mov      r3, #0
003b05f4: rsb      r2, r0, #0
003b05f8: strb     r3, [r4, #0x33]
003b05fc: str      r2, [r4, #0x28]
003b0600: str      r6, [r4, #0x2c]
003b0604: strb     r8, [r4, #0x30]
003b0608: strb     r7, [r4, #0x31]
003b060c: str      r0, [r4, #0x24]
003b0610: strb     r3, [r4, #0x32]
003b0614: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0618: subseq   r2, pc, ip, ror #7

# _ZN9Character26F_ApplyScrollingCombatTextERKNS_12AttackResultEPS_S3_
003af77c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003af780: mov      r6, r0
003af784: sub      sp, sp, #0x24
003af788: mov      r0, r2
003af78c: mov      r5, r2
003af790: mov      r7, r1
003af794: bl       #0x3a307c
003af798: ldr      r4, [pc, #0x4f0]
003af79c: cmp      r0, #0
003af7a0: add      r4, pc, r4
003af7a4: beq      #0x3af7b0
003af7a8: add      sp, sp, #0x24
003af7ac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003af7b0: bl       #0x413e90
003af7b4: mov      r8, r0
003af7b8: mov      r0, r5
003af7bc: bl       #0x3935dc
003af7c0: ldr      r3, [r0, #4]
003af7c4: ldr      r2, [r0]
003af7c8: ldr      sl, [r0, #8]
003af7cc: ldr      r1, [r5, #0x14c]
003af7d0: ldr      r0, [r5, #0x158]
003af7d4: str      r3, [sp, #0x18]
003af7d8: str      r2, [sp, #0x14]
003af7dc: bl       #0x30e3ac
003af7e0: mov      r1, r0
003af7e4: mov      r0, sl
003af7e8: bl       #0x30eba4
003af7ec: str      r0, [sp, #0x1c]
003af7f0: mov      r0, r7
003af7f4: bl       #0x3935dc
003af7f8: ldrb     r3, [r6, #0x18]
003af7fc: ldr      sl, [r6, #0x1c]
003af800: tst      r3, #1
003af804: bne      #0x3afa74
003af808: tst      r3, #2
003af80c: bne      #0x3afaf0
003af810: tst      r3, #4
003af814: bne      #0x3af95c
003af818: tst      r3, #0x20
003af81c: ubfx     sl, sl, #0x10, #1
003af820: beq      #0x3af8b8
003af824: cmp      sl, #0
003af828: movne    r1, #0xbb
003af82c: moveq    r1, #0x8f
003af830: add      r0, r7, #0x560
003af834: bl       #0x3af76c
003af838: lsrs     r0, r0, #8
003af83c: bne      #0x3afbc4
003af840: ldr      r3, [r6]
003af844: cmp      r3, #0
003af848: ble      #0x3af7a8
003af84c: ldr      r3, [r6, #0x1c]
003af850: tst      r3, #0x20000000
003af854: beq      #0x3afa34
003af858: ldr      r1, [pc, #0x434]
003af85c: mov      r0, r8
003af860: add      r1, pc, r1
003af864: bl       #0x414678
003af868: mov      r7, r0
003af86c: mov      r0, r5
003af870: ldr      r3, [r5]
003af874: mov      lr, pc
003af878: ldr      pc, [r3, #0x28]
003af87c: cmp      r0, #0
003af880: beq      #0x3af9e4
003af884: ldrb     r3, [r6, #0x18]
003af888: tst      r3, #8
003af88c: beq      #0x3afb9c
003af890: ldr      r3, [pc, #0x400]
003af894: ldr      r1, [pc, #0x400]
003af898: ldr      r2, [pc, #0x400]
003af89c: ldr      r3, [r4, r3]
003af8a0: add      r1, pc, r1
003af8a4: add      r2, pc, r2
003af8a8: ldr      r0, [r3, #0x2c]
003af8ac: bl       #0x4c4bdc
003af8b0: mov      ip, r0
003af8b4: b        #0x3afa14
003af8b8: ldrb     r3, [r6, #0x19]
003af8bc: tst      r3, #1
003af8c0: beq      #0x3af840
003af8c4: cmp      sl, #0
003af8c8: movne    r1, #0xbd
003af8cc: moveq    r1, #0x92
003af8d0: add      r0, r7, #0x560
003af8d4: bl       #0x3af76c
003af8d8: lsrs     r0, r0, #8
003af8dc: beq      #0x3af840
003af8e0: ldr      r1, [pc, #0x3bc]
003af8e4: mov      r0, r8
003af8e8: add      r1, pc, r1
003af8ec: bl       #0x414678
003af8f0: ldr      r3, [pc, #0x3a0]
003af8f4: ldr      r1, [pc, #0x3ac]
003af8f8: ldr      r2, [pc, #0x3ac]
003af8fc: ldr      sl, [r4, r3]
003af900: add      r1, pc, r1
003af904: add      r2, pc, r2
003af908: mov      fp, r0
003af90c: ldr      r0, [sl, #0x2c]
003af910: ldr      sb, [sl, #0x34]
003af914: bl       #0x4c4bdc
003af918: mov      r1, r0
003af91c: mov      r0, sb
003af920: bl       #0x508edc
003af924: ldr      r1, [pc, #0x384]
003af928: ldr      r2, [pc, #0x384]
003af92c: mov      sb, r0
003af930: ldr      r0, [sl, #0x2c]
003af934: add      r1, pc, r1
003af938: add      r2, pc, r2
003af93c: bl       #0x4c4bdc
003af940: mov      r1, fp
003af944: str      r0, [sp]
003af948: mov      r3, sb
003af94c: mov      r0, r8
003af950: add      r2, sp, #0x14
003af954: bl       #0x413dc4
003af958: b        #0x3af840
003af95c: ldr      r1, [pc, #0x354]
003af960: mov      r0, r8
003af964: add      r1, pc, r1
003af968: bl       #0x414678
003af96c: ldr      r2, [pc, #0x324]
003af970: ldr      r1, [pc, #0x344]
003af974: mov      r3, r0
003af978: ldr      sb, [r4, r2]
003af97c: ldr      r2, [pc, #0x33c]
003af980: add      r1, pc, r1
003af984: ldr      r0, [sb, #0x2c]
003af988: add      r2, pc, r2
003af98c: ldr      fp, [sb, #0x34]
003af990: str      r3, [sp, #0xc]
003af994: bl       #0x4c4bdc
003af998: mov      r1, r0
003af99c: mov      r0, fp
003af9a0: bl       #0x508edc
003af9a4: ldr      r1, [pc, #0x318]
003af9a8: ldr      r2, [pc, #0x318]
003af9ac: mov      fp, r0
003af9b0: add      r1, pc, r1
003af9b4: add      r2, pc, r2
003af9b8: ldr      r0, [sb, #0x2c]
003af9bc: bl       #0x4c4bdc
003af9c0: ldr      r3, [sp, #0xc]
003af9c4: str      r0, [sp]
003af9c8: add      r2, sp, #0x14
003af9cc: mov      r1, r3
003af9d0: mov      r0, r8
003af9d4: mov      r3, fp
003af9d8: bl       #0x413dc4
003af9dc: ldrb     r3, [r6, #0x18]
003af9e0: b        #0x3af818
003af9e4: ldrb     r3, [r6, #0x18]
003af9e8: tst      r3, #8
003af9ec: bne      #0x3afb74
003af9f0: ldr      r3, [pc, #0x2a0]
003af9f4: ldr      r1, [pc, #0x2d0]
003af9f8: ldr      r2, [pc, #0x2d0]
003af9fc: ldr      r3, [r4, r3]
003afa00: add      r1, pc, r1
003afa04: add      r2, pc, r2
003afa08: ldr      r0, [r3, #0x2c]
003afa0c: bl       #0x4c4bdc
003afa10: mov      ip, r0
003afa14: ldr      r3, [r6]
003afa18: mov      r0, r8
003afa1c: mov      r1, r7
003afa20: asr      r3, r3, #8
003afa24: add      r2, sp, #0x14
003afa28: str      ip, [sp]
003afa2c: bl       #0x413fa0
003afa30: b        #0x3af7a8
003afa34: add      r0, r7, #0x37c
003afa38: bl       #0x40019c
003afa3c: cmp      r0, #0
003afa40: beq      #0x3afb50
003afa44: ldr      r3, [r6, #0x1c]
003afa48: tst      r3, #0x4000000
003afa4c: beq      #0x3afc3c
003afa50: ldrb     r3, [r6, #0x18]
003afa54: tst      r3, #8
003afa58: beq      #0x3afc60
003afa5c: ldr      r1, [pc, #0x270]
003afa60: mov      r0, r8
003afa64: add      r1, pc, r1
003afa68: bl       #0x414678
003afa6c: mov      r7, r0
003afa70: b        #0x3af86c
003afa74: ldr      r1, [pc, #0x25c]
003afa78: mov      r0, r8
003afa7c: add      r1, pc, r1
003afa80: bl       #0x414678
003afa84: ldr      r3, [pc, #0x20c]
003afa88: ldr      r1, [pc, #0x24c]
003afa8c: ldr      r2, [pc, #0x24c]
003afa90: ldr      r4, [r4, r3]
003afa94: add      r1, pc, r1
003afa98: add      r2, pc, r2
003afa9c: mov      r6, r0
003afaa0: ldr      r0, [r4, #0x2c]
003afaa4: ldr      r5, [r4, #0x34]
003afaa8: bl       #0x4c4bdc
003afaac: mov      r1, r0
003afab0: mov      r0, r5
003afab4: bl       #0x508edc
003afab8: ldr      r1, [pc, #0x224]
003afabc: ldr      r2, [pc, #0x224]
003afac0: mov      r5, r0
003afac4: ldr      r0, [r4, #0x2c]
003afac8: add      r1, pc, r1
003afacc: add      r2, pc, r2
003afad0: bl       #0x4c4bdc
003afad4: mov      r1, r6
003afad8: str      r0, [sp]
003afadc: mov      r3, r5
003afae0: mov      r0, r8
003afae4: add      r2, sp, #0x14
003afae8: bl       #0x413dc4
003afaec: b        #0x3af7a8
003afaf0: ldr      r1, [pc, #0x1f4]
003afaf4: mov      r0, r8
003afaf8: add      r1, pc, r1
003afafc: bl       #0x414678
003afb00: ldr      r3, [pc, #0x190]
003afb04: ldr      r1, [pc, #0x1e4]
003afb08: ldr      r2, [pc, #0x1e4]
003afb0c: ldr      r4, [r4, r3]
003afb10: add      r1, pc, r1
003afb14: add      r2, pc, r2
003afb18: mov      r6, r0
003afb1c: ldr      r0, [r4, #0x2c]
003afb20: ldr      r5, [r4, #0x34]
003afb24: bl       #0x4c4bdc
003afb28: mov      r1, r0
003afb2c: mov      r0, r5
003afb30: bl       #0x508edc
003afb34: ldr      r1, [pc, #0x1bc]
003afb38: ldr      r2, [pc, #0x1bc]
003afb3c: mov      r5, r0
003afb40: add      r1, pc, r1
003afb44: ldr      r0, [r4, #0x2c]
003afb48: add      r2, pc, r2
003afb4c: b        #0x3afad0
003afb50: ldrb     r3, [r6, #0x18]
003afb54: tst      r3, #8
003afb58: beq      #0x3afc24
003afb5c: ldr      r1, [pc, #0x19c]
003afb60: mov      r0, r8
003afb64: add      r1, pc, r1
003afb68: bl       #0x414678
003afb6c: mov      r7, r0
003afb70: b        #0x3af86c
003afb74: ldr      r3, [pc, #0x11c]
003afb78: ldr      r1, [pc, #0x184]
003afb7c: ldr      r2, [pc, #0x184]
003afb80: ldr      r3, [r4, r3]
003afb84: add      r1, pc, r1
003afb88: add      r2, pc, r2
003afb8c: ldr      r0, [r3, #0x2c]
003afb90: bl       #0x4c4bdc
003afb94: mov      ip, r0
003afb98: b        #0x3afa14
003afb9c: ldr      r3, [pc, #0xf4]
003afba0: ldr      r1, [pc, #0x164]
003afba4: ldr      r2, [pc, #0x164]
003afba8: ldr      r3, [r4, r3]
003afbac: add      r1, pc, r1
003afbb0: add      r2, pc, r2
003afbb4: ldr      r0, [r3, #0x2c]
003afbb8: bl       #0x4c4bdc
003afbbc: mov      ip, r0
003afbc0: b        #0x3afa14
003afbc4: ldr      r1, [pc, #0x148]
003afbc8: mov      r0, r8
003afbcc: add      r1, pc, r1
003afbd0: bl       #0x414678
003afbd4: ldr      r3, [pc, #0xbc]
003afbd8: ldr      r1, [pc, #0x138]
003afbdc: ldr      r2, [pc, #0x138]
003afbe0: ldr      sl, [r4, r3]
003afbe4: add      r1, pc, r1
003afbe8: add      r2, pc, r2
003afbec: mov      fp, r0
003afbf0: ldr      r0, [sl, #0x2c]
003afbf4: ldr      sb, [sl, #0x34]
003afbf8: bl       #0x4c4bdc
003afbfc: mov      r1, r0
003afc00: mov      r0, sb
003afc04: bl       #0x508edc
003afc08: ldr      r1, [pc, #0x110]
003afc0c: ldr      r2, [pc, #0x110]
003afc10: mov      sb, r0
003afc14: add      r1, pc, r1
003afc18: ldr      r0, [sl, #0x2c]
003afc1c: add      r2, pc, r2
003afc20: b        #0x3af93c
003afc24: ldr      r1, [pc, #0xfc]
003afc28: mov      r0, r8
003afc2c: add      r1, pc, r1
003afc30: bl       #0x414678
003afc34: mov      r7, r0
003afc38: b        #0x3af86c
003afc3c: ldrb     r3, [r6, #0x18]
003afc40: tst      r3, #8
003afc44: beq      #0x3afc78
003afc48: ldr      r1, [pc, #0xdc]
003afc4c: mov      r0, r8
003afc50: add      r1, pc, r1
003afc54: bl       #0x414678
003afc58: mov      r7, r0
003afc5c: b        #0x3af86c
003afc60: ldr      r1, [pc, #0xc8]
003afc64: mov      r0, r8
003afc68: add      r1, pc, r1
003afc6c: bl       #0x414678
003afc70: mov      r7, r0
003afc74: b        #0x3af86c
003afc78: ldr      r1, [pc, #0xb4]
003afc7c: mov      r0, r8
003afc80: add      r1, pc, r1
003afc84: bl       #0x414678
003afc88: mov      r7, r0
003afc8c: b        #0x3af86c
003afc90: ldrsheq  r5, [lr], #-0x20
003afc94: subseq   r4, r1, r8, asr #5
003afc98: strdeq   r3, r4, [r0], -r4
003afc9c: subseq   r3, r1, r0, asr lr
003afca0: subseq   r4, r1, r4, lsl r3
003afca4: subseq   r4, r1, r0, ror #3
003afca8: subseq   pc, r0, r8, lsr #6
003afcac: ldrsheq  r4, [r1], #-0x1c
003afcb0: ldrheq   r3, [r1], #-0xdc
003afcb4: subseq   r4, r1, r0, ror #3
003afcb8: ldrsbeq  r4, [r1], #-0xc
003afcbc: subseq   pc, r0, r8, lsr #5
003afcc0: subseq   r4, r1, r8, lsl r1
003afcc4: subseq   r3, r1, r0, asr #26
003afcc8: subseq   r4, r1, r4, lsl #2
003afccc: ldrsheq  r3, [r1], #-0xc0
003afcd0: subseq   r4, r1, ip, lsr #32
003afcd4: ldrsbeq  r4, [r1], #-4
003afcd8: subseq   r3, r1, r4, asr #31

# _Z14CF__CalcDamageP9CharacterS0_iiibb
003b1fb8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b1fbc: sub      sp, sp, #0x2c
003b1fc0: ldr      ip, [sp, #0x50]
003b1fc4: mov      r4, r0
003b1fc8: mov      r5, r1
003b1fcc: cmp      ip, #1
003b1fd0: mov      sb, r2
003b1fd4: ldrb     fp, [sp, #0x58]
003b1fd8: ldrb     r8, [sp, #0x5c]
003b1fdc: bls      #0x3b201c
003b1fe0: cmp      ip, #2
003b1fe4: beq      #0x3b2234
003b1fe8: cmp      ip, #3
003b1fec: beq      #0x3b23f0
003b1ff0: mov      r3, #0
003b1ff4: str      r3, [r0, #0x18]
003b1ff8: str      r3, [r0]
003b1ffc: str      r3, [r0, #4]
003b2000: str      r3, [r0, #8]
003b2004: str      r3, [r0, #0xc]
003b2008: str      r3, [r0, #0x10]
003b200c: str      r3, [r0, #0x14]
003b2010: mov      r0, r4
003b2014: add      sp, sp, #0x2c
003b2018: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b201c: cmp      fp, #0
003b2020: bne      #0x3b23ac
003b2024: add      r7, r1, #0xff0
003b2028: add      r6, r1, #0x560
003b202c: add      r7, r7, #4
003b2030: mov      r2, #0x4f
003b2034: mov      r1, r7
003b2038: mov      r0, r6
003b203c: bl       #0x3dedb4
003b2040: mov      r1, r7
003b2044: mov      r2, #0x50
003b2048: mov      sl, r0
003b204c: mov      r0, r6
003b2050: bl       #0x3dedb4
003b2054: mov      r1, r7
003b2058: mov      ip, r0
003b205c: mov      r2, #0x61
003b2060: mov      r0, r6
003b2064: str      ip, [sp]
003b2068: bl       #0x3dedb4
003b206c: cmp      r8, #0
003b2070: asr      r0, r0, #8
003b2074: str      r0, [sp, #0x10]
003b2078: ldr      ip, [sp]
003b207c: bne      #0x3b256c
003b2080: bic      r3, sl, sl, asr #31
003b2084: bic      ip, ip, ip, asr #31
003b2088: rsb      r0, r3, ip
003b208c: str      ip, [sp, #0x14]
003b2090: str      r3, [sp]
003b2094: bl       #0x3af6d8
003b2098: mov      r1, fp
003b209c: mov      sl, r0
003b20a0: mov      r0, r6
003b20a4: bl       #0x3df8ac
003b20a8: ldr      r3, [sp]
003b20ac: add      sl, sl, r0
003b20b0: add      r0, sb, #0x4f0
003b20b4: add      sl, sl, r3
003b20b8: add      r0, r0, #0xc
003b20bc: str      sl, [sp, #4]
003b20c0: bl       #0x3c01ac
003b20c4: cmp      r0, #9
003b20c8: beq      #0x3b25fc
003b20cc: add      r2, sb, #0xff0
003b20d0: add      r2, r2, #4
003b20d4: add      r3, sb, #0x560
003b20d8: str      r2, [sp, #8]
003b20dc: mov      r1, r7
003b20e0: mov      r2, #0xc6
003b20e4: mov      r0, r6
003b20e8: str      r3, [sp, #0xc]
003b20ec: bl       #0x3dedb4
003b20f0: ldr      r1, [sp, #8]
003b20f4: mov      sl, r0
003b20f8: mov      r2, #0xc7
003b20fc: ldr      r0, [sp, #0xc]
003b2100: bl       #0x3dedb4
003b2104: cmp      sl, r0
003b2108: bgt      #0x3b2550
003b210c: movw     r3, #0x14d0
003b2110: ldrh     sl, [r5, r3]
003b2114: cmp      sl, #0
003b2118: beq      #0x3b2138
003b211c: mov      r2, #0x5e
003b2120: mov      r0, r6
003b2124: mov      r1, r7
003b2128: bl       #0x3dedb4
003b212c: ldr      r2, [sp, #4]
003b2130: mla      r2, sl, r0, r2
003b2134: str      r2, [sp, #4]
003b2138: ldr      r3, [pc, #0x4ec]
003b213c: add      r3, pc, r3
003b2140: ldrb     r3, [r3, #0x32]
003b2144: cmp      r3, #0
003b2148: bne      #0x3b2530
003b214c: ldr      r3, [pc, #0x4dc]
003b2150: mov      r2, #0x47
003b2154: mov      sl, #0x19
003b2158: add      r3, pc, r3
003b215c: ldrb     r3, [r3, #0x33]
003b2160: cmp      r3, #0
003b2164: ldrne    r0, [sp, #4]
003b2168: ldrne    r1, [sp, #0x14]
003b216c: addne    r0, r0, r1
003b2170: strne    r0, [sp, #4]
003b2174: ldr      r1, [sp, #8]
003b2178: ldr      r0, [sp, #0xc]
003b217c: bl       #0x3dedb4
003b2180: ldr      r2, [sp, #4]
003b2184: mul      sl, sl, r0
003b2188: sub      sl, r2, sl, asr #8
003b218c: cmp      sl, #0
003b2190: movle    sl, #0x100
003b2194: cmp      r8, #0
003b2198: movne    r3, #0
003b219c: strne    r3, [sp, #4]
003b21a0: strne    r3, [sp, #8]
003b21a4: beq      #0x3b24d4
003b21a8: ldr      r2, [sp, #0x10]
003b21ac: cmn      r2, #1
003b21b0: beq      #0x3b21ec
003b21b4: cmp      fp, #0
003b21b8: beq      #0x3b2454
003b21bc: mov      r2, #0x62
003b21c0: mov      r1, r7
003b21c4: mov      r0, r6
003b21c8: bl       #0x3dedb4
003b21cc: mov      r1, r7
003b21d0: mov      fp, r0
003b21d4: mov      r2, #0x63
003b21d8: mov      r0, r6
003b21dc: bl       #0x3dedb4
003b21e0: cmp      r0, #0
003b21e4: cmpge    fp, #0
003b21e8: bge      #0x3b247c
003b21ec: mov      r1, r5
003b21f0: mov      r2, sb
003b21f4: mov      r3, r8
003b21f8: add      r0, sp, #0x1c
003b21fc: bl       #0x3b09c4
003b2200: add      r1, sp, #0x1c
003b2204: ldm      r1, {r1, r2, r3}
003b2208: str      sl, [r4]
003b220c: ldr      r0, [sp, #0x10]
003b2210: str      r0, [r4, #4]
003b2214: ldr      r0, [sp, #8]
003b2218: str      r0, [r4, #8]
003b221c: ldr      r0, [sp, #4]
003b2220: str      r1, [r4, #0x10]
003b2224: str      r2, [r4, #0x14]
003b2228: str      r0, [r4, #0xc]
003b222c: str      r3, [r4, #0x18]
003b2230: b        #0x3b2010
003b2234: cmp      r8, #0
003b2238: bne      #0x3b2414
003b223c: add      r7, r1, #0xff0
003b2240: add      r6, r1, #0x560
003b2244: add      r7, r7, #4
003b2248: mov      r2, #0x4f
003b224c: mov      r1, r7
003b2250: mov      r0, r6
003b2254: bl       #0x3dedb4
003b2258: mov      r1, r7
003b225c: mov      r3, r0
003b2260: mov      r2, #0x50
003b2264: mov      r0, r6
003b2268: str      r3, [sp]
003b226c: bl       #0x3dedb4
003b2270: mov      r1, r7
003b2274: mov      sl, r0
003b2278: mov      r2, #0x61
003b227c: mov      r0, r6
003b2280: bl       #0x3dedb4
003b2284: ldr      r3, [sp]
003b2288: asr      r0, r0, #8
003b228c: str      r0, [sp, #0x54]
003b2290: orrs     r7, sl, r3
003b2294: beq      #0x3b2364
003b2298: bic      r7, r3, r3, asr #31
003b229c: bic      sl, sl, sl, asr #31
003b22a0: rsb      r0, r7, sl
003b22a4: str      sl, [sp, #8]
003b22a8: bl       #0x3af6d8
003b22ac: cmp      r8, #0
003b22b0: add      sl, r0, r7
003b22b4: beq      #0x3b2618
003b22b8: ldr      r1, [sp, #0x54]
003b22bc: cmn      r1, #1
003b22c0: addeq    fp, sb, #0x560
003b22c4: beq      #0x3b2324
003b22c8: ldr      r2, [sp, #0x54]
003b22cc: add      fp, sb, #0x560
003b22d0: mov      r0, fp
003b22d4: add      r1, r2, #0x4a
003b22d8: bl       #0x3af76c
003b22dc: movw     r3, #0x851f
003b22e0: asr      r7, r0, #8
003b22e4: movt     r3, #0x51eb
003b22e8: smull    r1, r3, r3, r7
003b22ec: asr      r7, r0, #0x1f
003b22f0: sub      r7, r7, r3, asr #5
003b22f4: adds     r7, r7, #1
003b22f8: bmi      #0x3b25e4
003b22fc: ldr      r2, [sp, #0x54]
003b2300: mov      r0, r6
003b2304: cmp      r7, #1
003b2308: movge    r7, #1
003b230c: add      r1, r2, #0xa6
003b2310: bl       #0x3af76c
003b2314: add      sl, r0, sl
003b2318: mul      sl, r7, sl
003b231c: cmp      sl, #0
003b2320: movle    sl, #0x100
003b2324: ldr      r3, [pc, #0x308]
003b2328: add      r1, sb, #0xff0
003b232c: mov      r0, fp
003b2330: add      r3, pc, r3
003b2334: ldrb     r3, [r3, #0x33]
003b2338: add      r1, r1, #4
003b233c: mov      r2, #0x49
003b2340: cmp      r3, #0
003b2344: ldrne    r3, [sp, #8]
003b2348: mov      r7, #0x33
003b234c: addne    sl, sl, r3
003b2350: bl       #0x3dedb4
003b2354: mul      r7, r7, r0
003b2358: sub      r7, sl, r7, asr #8
003b235c: cmp      r7, #0
003b2360: movle    r7, #0x100
003b2364: cmp      r8, #0
003b2368: beq      #0x3b2590
003b236c: mov      r1, r5
003b2370: mov      r2, sb
003b2374: mov      r3, #1
003b2378: add      r0, sp, #0x1c
003b237c: bl       #0x3b09c4
003b2380: add      r2, sp, #0x1c
003b2384: ldm      r2, {r2, r3, r8}
003b2388: mov      sl, #0
003b238c: mov      r5, sl
003b2390: mov      r1, #0
003b2394: str      r7, [r4]
003b2398: stmib    r4, {r1, r5, sl}
003b239c: str      r2, [r4, #0x10]
003b23a0: str      r3, [r4, #0x14]
003b23a4: str      r8, [r4, #0x18]
003b23a8: b        #0x3b2010
003b23ac: add      r7, r1, #0xff0
003b23b0: add      r6, r1, #0x560
003b23b4: add      r7, r7, #4
003b23b8: mov      r2, #0x51
003b23bc: mov      r1, r7
003b23c0: mov      r0, r6
003b23c4: bl       #0x3dedb4
003b23c8: mov      r1, r7
003b23cc: mov      r2, #0x52
003b23d0: mov      sl, r0
003b23d4: mov      r0, r6
003b23d8: bl       #0x3dedb4
003b23dc: mov      r1, r7
003b23e0: mov      ip, r0
003b23e4: mov      r2, #0x64
003b23e8: mov      r0, r6
003b23ec: b        #0x3b2064
003b23f0: mov      r2, #0
003b23f4: str      r3, [r0]
003b23f8: str      r2, [r0, #0x18]
003b23fc: str      r2, [r0, #4]
003b2400: str      r2, [r0, #8]
003b2404: str      r2, [r0, #0xc]
003b2408: str      r2, [r0, #0x10]
003b240c: str      r2, [r0, #0x14]
003b2410: b        #0x3b2010
003b2414: add      r7, r1, #0xff0
003b2418: add      r6, r1, #0x560
003b241c: add      r7, r7, #4
003b2420: mov      r2, #0xae
003b2424: mov      r1, r7
003b2428: mov      r0, r6
003b242c: bl       #0x3dedb4
003b2430: mov      r1, r7
003b2434: mov      r3, r0
003b2438: mov      r2, #0xaf
003b243c: mov      r0, r6
003b2440: str      r3, [sp]
003b2444: bl       #0x3dedb4
003b2448: ldr      r3, [sp]
003b244c: mov      sl, r0
003b2450: b        #0x3b2290
003b2454: mov      r1, r7
003b2458: mov      r2, #0x5f
003b245c: mov      r0, r6
003b2460: bl       #0x3dedb4
003b2464: mov      r1, r7
003b2468: mov      fp, r0
003b246c: mov      r2, #0x60
003b2470: mov      r0, r6
003b2474: bl       #0x3dedb4
003b2478: b        #0x3b21e0
003b247c: rsb      r0, fp, r0
003b2480: bl       #0x3af6d8
003b2484: ldr      r3, [sp, #0x10]
003b2488: mov      r6, r0
003b248c: ldr      r0, [sp, #0xc]
003b2490: add      r1, r3, #0x4a
003b2494: bl       #0x3af76c
003b2498: movw     r3, #0x851f
003b249c: asr      r2, r0, #8
003b24a0: movt     r3, #0x51eb
003b24a4: smull    r1, r3, r3, r2
003b24a8: asr      r0, r0, #0x1f
003b24ac: sub      r3, r0, r3, asr #5
003b24b0: adds     r3, r3, #1
003b24b4: bmi      #0x3b21ec
003b24b8: cmp      r3, #1
003b24bc: movge    r3, #1
003b24c0: add      r6, r6, fp
003b24c4: mla      sl, r6, r3, sl
003b24c8: cmp      sl, #0
003b24cc: movle    sl, #0x100
003b24d0: b        #0x3b21ec
003b24d4: mov      r1, #0x84
003b24d8: mov      r0, r6
003b24dc: bl       #0x3af76c
003b24e0: mul      r0, sl, r0
003b24e4: movw     r2, #0x851f
003b24e8: movt     r2, #0x51eb
003b24ec: asr      ip, r0, #8
003b24f0: smull    r1, ip, r2, ip
003b24f4: asr      r3, r0, #0x1f
003b24f8: rsb      r3, r3, ip, asr #5
003b24fc: mov      r1, #0x85
003b2500: mov      r0, r6
003b2504: str      r3, [sp, #8]
003b2508: str      r2, [sp]
003b250c: bl       #0x3af76c
003b2510: mul      r0, sl, r0
003b2514: ldr      r2, [sp]
003b2518: asr      r3, r0, #8
003b251c: asr      r0, r0, #0x1f
003b2520: smull    r1, r2, r2, r3
003b2524: rsb      r0, r0, r2, asr #5
003b2528: str      r0, [sp, #4]
003b252c: b        #0x3b21a8
003b2530: ldr      r0, [sp, #0xc]
003b2534: mov      r1, #0x79
003b2538: bl       #0x3af76c
003b253c: ldr      r3, [sp, #4]
003b2540: mul      r0, r3, r0
003b2544: asr      r0, r0, #8
003b2548: str      r0, [sp, #4]
003b254c: b        #0x3b214c
003b2550: mov      r1, #0x5d
003b2554: mov      r0, r6
003b2558: bl       #0x3af76c
003b255c: ldr      r1, [sp, #4]
003b2560: add      r1, r1, r0
003b2564: str      r1, [sp, #4]
003b2568: b        #0x3b210c
003b256c: mov      r1, #0xae
003b2570: mov      r0, r6
003b2574: bl       #0x3af76c
003b2578: mov      r1, #0xaf
003b257c: mov      sl, r0
003b2580: mov      r0, r6
003b2584: bl       #0x3af76c
003b2588: mov      ip, r0
003b258c: b        #0x3b2080
003b2590: mov      r1, #0x84
003b2594: mov      r0, r6
003b2598: bl       #0x3af76c
003b259c: mul      r3, r7, r0
003b25a0: movw     sl, #0x851f
003b25a4: movt     sl, #0x51eb
003b25a8: asr      r5, r3, #8
003b25ac: smull    r0, r5, sl, r5
003b25b0: asr      r3, r3, #0x1f
003b25b4: mov      r1, #0x85
003b25b8: mov      r0, r6
003b25bc: rsb      r5, r3, r5, asr #5
003b25c0: bl       #0x3af76c
003b25c4: mul      r0, r7, r0
003b25c8: mov      r2, r8
003b25cc: asr      r3, r0, #8
003b25d0: smull    r1, sl, sl, r3
003b25d4: asr      r0, r0, #0x1f
003b25d8: rsb      sl, r0, sl, asr #5
003b25dc: mov      r3, r8
003b25e0: b        #0x3b2390
003b25e4: ldr      r2, [sp, #0x54]
003b25e8: mov      r0, r6
003b25ec: mov      sl, #0x100
003b25f0: add      r1, r2, #0xa6
003b25f4: bl       #0x3af76c
003b25f8: b        #0x3b2324
003b25fc: mov      r1, #0x5c
003b2600: mov      r0, r6
003b2604: bl       #0x3af76c
003b2608: ldr      r1, [sp, #4]
003b260c: add      r1, r1, r0
003b2610: str      r1, [sp, #4]
003b2614: b        #0x3b20cc
003b2618: mov      r1, fp
003b261c: mov      r0, r6
003b2620: bl       #0x3df8ac
003b2624: add      sl, sl, r0
003b2628: b        #0x3b22b8
003b262c: subseq   r0, pc, ip, asr r8
003b2630: subseq   r0, pc, r0, asr #16
003b2634: subseq   r0, pc, r8, ror #12

# _ZNK9Character27GetEffectiveThreatPerDamageEv
003bd394: add      r1, r0, #0xff0
003bd398: push     {r4, lr}
003bd39c: add      r1, r1, #4
003bd3a0: mov      r2, #0xcc
003bd3a4: add      r0, r0, #0x560
003bd3a8: bl       #0x3dedb4
003bd3ac: bl       #0x30e964
003bd3b0: mov      r1, #0x3b800000
003bd3b4: bl       #0x30ed6c
003bd3b8: pop      {r4, pc}

# _ZN6CharAI11AI_AddAggroEP9Characterf
003d7c68: push     {r4, r5, r6, r7, lr}
003d7c6c: ldr      r3, [pc, #0x110]
003d7c70: subs     r4, r1, #0
003d7c74: sub      sp, sp, #0xc
003d7c78: mov      r5, r0
003d7c7c: add      r3, pc, r3
003d7c80: mov      r6, r2
003d7c84: beq      #0x3d7d18
003d7c88: ldr      r3, [r5, #0x80]
003d7c8c: add      r0, r5, #0x7c
003d7c90: cmp      r3, #0
003d7c94: beq      #0x3d7d10
003d7c98: mov      r1, r0
003d7c9c: b        #0x3d7ca4
003d7ca0: mov      r3, r2
003d7ca4: ldr      r2, [r3, #0x10]
003d7ca8: cmp      r4, r2
003d7cac: ldrhi    r2, [r3, #0xc]
003d7cb0: ldrls    r2, [r3, #8]
003d7cb4: movhi    r3, r1
003d7cb8: mov      r1, r3
003d7cbc: cmp      r2, #0
003d7cc0: bne      #0x3d7ca0
003d7cc4: cmp      r0, r3
003d7cc8: beq      #0x3d7d6c
003d7ccc: ldr      r2, [r3, #0x10]
003d7cd0: cmp      r4, r2
003d7cd4: blo      #0x3d7d10
003d7cd8: cmp      r0, r3
003d7cdc: beq      #0x3d7d6c
003d7ce0: ldr      r7, [r3, #0x14]
003d7ce4: mov      r0, r6
003d7ce8: mov      r1, r7
003d7cec: bl       #0x30eba4
003d7cf0: mov      r1, r4
003d7cf4: mov      r2, r0
003d7cf8: mov      r0, r5
003d7cfc: bl       #0x3d79ec
003d7d00: mov      r1, r7
003d7d04: bl       #0x30e3ac
003d7d08: add      sp, sp, #0xc
003d7d0c: pop      {r4, r5, r6, r7, pc}
003d7d10: mov      r3, r0
003d7d14: b        #0x3d7cd8
003d7d18: ldr      r2, [pc, #0x68]
003d7d1c: ldr      r2, [r3, r2]
003d7d20: ldr      r2, [r2]
003d7d24: cmp      r2, #2
003d7d28: streq    r4, [r4]
003d7d2c: beq      #0x3d7c88
003d7d30: cmp      r2, #1
003d7d34: bne      #0x3d7c88
003d7d38: ldr      r0, [pc, #0x4c]
003d7d3c: ldr      r1, [pc, #0x4c]
003d7d40: ldr      r2, [pc, #0x4c]
003d7d44: ldr      r0, [r3, r0]
003d7d48: ldr      r3, [pc, #0x48]
003d7d4c: movw     ip, #0x292
003d7d50: add      r1, pc, r1
003d7d54: add      r2, pc, r2
003d7d58: add      r3, pc, r3
003d7d5c: add      r0, r0, #0xa8
003d7d60: str      ip, [sp]
003d7d64: bl       #0x30e004
003d7d68: b        #0x3d7c88
003d7d6c: mov      r0, r5
003d7d70: mov      r1, r4
003d7d74: mov      r2, r6
003d7d78: add      sp, sp, #0xc
003d7d7c: pop      {r4, r5, r6, r7, lr}
003d7d80: b        #0x3d79ec
003d7d84: subseq   ip, fp, r4, lsl lr
003d7d88: andeq    r3, r0, r0, asr #19
003d7d8c: andeq    r1, r0, r0, asr #19
003d7d90: subeq    r6, lr, r8, lsl #13
003d7d94: subseq   sl, r1, r4, lsr r3
003d7d98: subeq    sp, lr, r8, ror #16

# _ZN9Character18F_ApplyCombatSoundERKNS_12AttackResultEPS_S3_
003afee0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003afee4: ldr      r4, [pc, #0x2b8]
003afee8: ldr      r5, [pc, #0x2b8]
003afeec: sub      sp, sp, #0x6c
003afef0: add      r4, pc, r4
003afef4: ldr      r3, [r4, r5]
003afef8: mov      sb, r0
003afefc: mov      r0, r1
003aff00: ldr      r3, [r3]
003aff04: mov      r6, r2
003aff08: add      r8, sp, #0x4c
003aff0c: str      r3, [sp, #0x64]
003aff10: bl       #0x3a32d0
003aff14: mov      fp, r0
003aff18: mov      r0, r6
003aff1c: bl       #0x3a32d0
003aff20: ldr      r3, [pc, #0x284]
003aff24: mov      r7, r0
003aff28: ldr      sl, [r4, r3]
003aff2c: mov      r0, sl
003aff30: bl       #0x337888
003aff34: ldr      r1, [pc, #0x274]
003aff38: add      r2, sp, #0x48
003aff3c: mov      r0, r8
003aff40: add      r1, pc, r1
003aff44: bl       #0x3140ec
003aff48: mov      r0, sl
003aff4c: mov      r1, r8
003aff50: bl       #0x337a88
003aff54: mov      sl, r0
003aff58: mov      r0, r8
003aff5c: bl       #0x3139ac
003aff60: cmp      sl, #0
003aff64: beq      #0x3aff84
003aff68: ldr      r3, [r4, r5]
003aff6c: ldr      r2, [sp, #0x64]
003aff70: ldr      r3, [r3]
003aff74: cmp      r2, r3
003aff78: bne      #0x3b01a0
003aff7c: add      sp, sp, #0x6c
003aff80: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aff84: ldr      r3, [r6]
003aff88: mov      r0, r6
003aff8c: mov      lr, pc
003aff90: ldr      pc, [r3, #0x34]
003aff94: subs     r8, r0, #0
003aff98: bne      #0x3b0034
003aff9c: ldr      r3, [sb]
003affa0: cmp      r3, #0
003affa4: ble      #0x3aff68
003affa8: ldr      r0, [r7, #0xc]
003affac: cmp      r0, #0
003affb0: bne      #0x3b013c
003affb4: ldrb     r8, [r7, #0x24]
003affb8: cmp      r8, #0
003affbc: beq      #0x3b0058
003affc0: ldr      r0, [fp, #0x14]
003affc4: cmp      r0, #0
003affc8: beq      #0x3aff68
003affcc: ldr      r3, [pc, #0x1e0]
003affd0: ldr      r7, [fp, #0x18]
003affd4: ldr      r3, [r4, r3]
003affd8: ldr      sl, [r3]
003affdc: bl       #0x3af6d8
003affe0: ldr      r8, [r7, r0, lsl #2]
003affe4: mov      r0, r6
003affe8: bl       #0x3935dc
003affec: ldr      r7, [r0]
003afff0: ldr      r6, [r0, #4]
003afff4: ldr      lr, [r0, #8]
003afff8: mov      ip, #0xbf000000
003afffc: add      ip, ip, #0x800000
003b0000: mov      r0, sl
003b0004: mov      r1, r8
003b0008: add      r2, sp, #0x24
003b000c: mov      r3, #0
003b0010: str      r7, [sp, #0x24]
003b0014: str      r6, [sp, #0x28]
003b0018: str      lr, [sp, #0x2c]
003b001c: mov      lr, #1
003b0020: str      lr, [sp]
003b0024: str      ip, [sp, #8]
003b0028: str      ip, [sp, #4]
003b002c: bl       #0x36b5d8
003b0030: b        #0x3aff68
003b0034: ldr      r0, [r7, #4]
003b0038: cmp      r0, #0
003b003c: bne      #0x3b00c4
003b0040: ldr      r3, [sb]
003b0044: cmp      r3, #0
003b0048: ble      #0x3aff68
003b004c: ldrb     r8, [r7, #0x24]
003b0050: cmp      r8, #0
003b0054: bne      #0x3affc0
003b0058: ldrb     r3, [r7, #0x25]
003b005c: cmp      r3, #0
003b0060: beq      #0x3aff68
003b0064: ldr      r0, [fp, #0x1c]
003b0068: cmp      r0, #0
003b006c: beq      #0x3aff68
003b0070: ldr      r3, [pc, #0x13c]
003b0074: ldr      r7, [fp, #0x20]
003b0078: ldr      r3, [r4, r3]
003b007c: ldr      sb, [r3]
003b0080: bl       #0x3af6d8
003b0084: ldr      sl, [r7, r0, lsl #2]
003b0088: mov      r0, r6
003b008c: bl       #0x3935dc
003b0090: ldr      r6, [r0]
003b0094: ldr      lr, [r0, #4]
003b0098: ldr      r7, [r0, #8]
003b009c: mov      ip, #0xbf000000
003b00a0: add      ip, ip, #0x800000
003b00a4: mov      r0, sb
003b00a8: mov      r1, sl
003b00ac: mov      r3, r8
003b00b0: add      r2, sp, #0x18
003b00b4: str      r6, [sp, #0x18]
003b00b8: str      lr, [sp, #0x1c]
003b00bc: str      r7, [sp, #0x20]
003b00c0: b        #0x3b001c
003b00c4: ldr      r3, [pc, #0xe8]
003b00c8: ldr      r8, [r7, #8]
003b00cc: ldr      r3, [r4, r3]
003b00d0: ldr      r3, [r3]
003b00d4: str      r3, [sp, #0x10]
003b00d8: bl       #0x3af6d8
003b00dc: ldr      r8, [r8, r0, lsl #2]
003b00e0: mov      r0, r6
003b00e4: bl       #0x3935dc
003b00e8: ldr      r2, [r0, #4]
003b00ec: ldr      lr, [r0]
003b00f0: ldr      r3, [sp, #0x10]
003b00f4: str      r2, [sp, #0x14]
003b00f8: ldr      r0, [r0, #8]
003b00fc: str      lr, [sp, #0x3c]
003b0100: ldr      lr, [sp, #0x14]
003b0104: mov      ip, #0xbf000000
003b0108: str      r0, [sp, #0x44]
003b010c: add      ip, ip, #0x800000
003b0110: mov      r0, r3
003b0114: mov      r1, r8
003b0118: mov      r3, sl
003b011c: add      r2, sp, #0x3c
003b0120: str      lr, [sp, #0x40]
003b0124: mov      lr, #1
003b0128: str      lr, [sp]
003b012c: str      ip, [sp, #8]
003b0130: str      ip, [sp, #4]
003b0134: bl       #0x36b5d8
003b0138: b        #0x3b0040
003b013c: ldr      r3, [pc, #0x70]
003b0140: ldr      sl, [r7, #0x10]
003b0144: ldr      r3, [r4, r3]
003b0148: ldr      r3, [r3]
003b014c: str      r3, [sp, #0x10]
003b0150: bl       #0x3af6d8
003b0154: ldr      sl, [sl, r0, lsl #2]
003b0158: mov      r0, r6
003b015c: bl       #0x3935dc
003b0160: ldr      r2, [r0, #4]
003b0164: ldr      lr, [r0]
003b0168: ldr      r3, [sp, #0x10]
003b016c: str      r2, [sp, #0x14]
003b0170: ldr      r0, [r0, #8]
003b0174: str      lr, [sp, #0x30]
003b0178: ldr      lr, [sp, #0x14]
003b017c: mov      ip, #0xbf000000
003b0180: str      r0, [sp, #0x38]
003b0184: add      ip, ip, #0x800000
003b0188: mov      r0, r3
003b018c: mov      r1, sl
003b0190: mov      r3, r8
003b0194: add      r2, sp, #0x30
003b0198: str      lr, [sp, #0x34]
003b019c: b        #0x3b0124
003b01a0: bl       #0x30e310
003b01a4: subseq   r4, lr, r0, lsr #23
003b01a8: andeq    r4, r0, ip, lsr #1
003b01ac: andeq    r0, r0, r4, lsl #17
003b01b0: subseq   r3, r1, r0, lsr #25
003b01b4: andeq    r0, r0, r4, lsr #27


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

# _ZN6CharAI11AI_SetAggroEP9Characterf
003d79ec: push     {r4, r5, r6, r7, lr}
003d79f0: ldr      r3, [pc, #0x258]
003d79f4: subs     r4, r1, #0
003d79f8: sub      sp, sp, #0x2c
003d79fc: mov      r6, r0
003d7a00: add      r3, pc, r3
003d7a04: mov      r5, r2
003d7a08: beq      #0x3d7bd4
003d7a0c: ldr      r3, [r6, #4]
003d7a10: mov      r0, r3
003d7a14: ldr      r3, [r3]
003d7a18: mov      lr, pc
003d7a1c: ldr      pc, [r3, #0x28]
003d7a20: cmp      r0, #0
003d7a24: beq      #0x3d7a38
003d7a28: mov      r5, #0
003d7a2c: mov      r0, r5
003d7a30: add      sp, sp, #0x2c
003d7a34: pop      {r4, r5, r6, r7, pc}
003d7a38: ldr      r3, [r6, #4]
003d7a3c: mov      r0, r3
003d7a40: ldr      r3, [r3]
003d7a44: mov      lr, pc
003d7a48: ldr      pc, [r3, #0x34]
003d7a4c: cmp      r0, #0
003d7a50: bne      #0x3d7a28
003d7a54: ldr      r3, [r4]
003d7a58: mov      r0, r4
003d7a5c: mov      lr, pc
003d7a60: ldr      pc, [r3, #0x34]
003d7a64: cmp      r0, #0
003d7a68: bne      #0x3d7a28
003d7a6c: ldr      ip, [r6, #0x80]
003d7a70: add      r7, r6, #0x7c
003d7a74: cmp      ip, #0
003d7a78: movne    r1, r7
003d7a7c: movne    r3, ip
003d7a80: bne      #0x3d7a8c
003d7a84: b        #0x3d7bcc
003d7a88: mov      r3, r2
003d7a8c: ldr      r2, [r3, #0x10]
003d7a90: cmp      r4, r2
003d7a94: ldrhi    r2, [r3, #0xc]
003d7a98: ldrls    r2, [r3, #8]
003d7a9c: movhi    r3, r1
003d7aa0: mov      r1, r3
003d7aa4: cmp      r2, #0
003d7aa8: bne      #0x3d7a88
003d7aac: cmp      r7, r3
003d7ab0: beq      #0x3d7c34
003d7ab4: ldr      r2, [r3, #0x10]
003d7ab8: cmp      r4, r2
003d7abc: blo      #0x3d7bcc
003d7ac0: cmp      r7, r3
003d7ac4: beq      #0x3d7c34
003d7ac8: cmp      ip, #0
003d7acc: moveq    ip, r7
003d7ad0: beq      #0x3d7b00
003d7ad4: mov      r2, r7
003d7ad8: b        #0x3d7ae0
003d7adc: mov      ip, r3
003d7ae0: ldr      r3, [ip, #0x10]
003d7ae4: cmp      r4, r3
003d7ae8: ldrhi    r3, [ip, #0xc]
003d7aec: ldrls    r3, [ip, #8]
003d7af0: movhi    ip, r2
003d7af4: mov      r2, ip
003d7af8: cmp      r3, #0
003d7afc: bne      #0x3d7adc
003d7b00: cmp      r7, ip
003d7b04: beq      #0x3d7b18
003d7b08: ldr      r2, [ip, #0x10]
003d7b0c: mov      r3, ip
003d7b10: cmp      r4, r2
003d7b14: bhs      #0x3d7b40
003d7b18: add      r3, sp, #0x10
003d7b1c: mov      lr, #0
003d7b20: mov      r1, r7
003d7b24: add      r0, sp, #0x20
003d7b28: add      r2, sp, #0x24
003d7b2c: str      lr, [sp, #0x14]
003d7b30: str      ip, [sp, #0x24]
003d7b34: str      r4, [sp, #0x10]
003d7b38: bl       #0x3d7678
003d7b3c: ldr      r3, [sp, #0x20]
003d7b40: str      r5, [r3, #0x14]
003d7b44: ldr      ip, [r4, #0x460]
003d7b48: add      r1, r4, #0x450
003d7b4c: add      r1, r1, #0xc
003d7b50: cmp      ip, #0
003d7b54: beq      #0x3d7c28
003d7b58: ldr      r6, [r6, #4]
003d7b5c: mov      r2, r1
003d7b60: b        #0x3d7b68
003d7b64: mov      ip, r3
003d7b68: ldr      r3, [ip, #0x10]
003d7b6c: cmp      r6, r3
003d7b70: ldrhi    r3, [ip, #0xc]
003d7b74: ldrls    r3, [ip, #8]
003d7b78: movhi    ip, r2
003d7b7c: mov      r2, ip
003d7b80: cmp      r3, #0
003d7b84: bne      #0x3d7b64
003d7b88: cmp      r1, ip
003d7b8c: beq      #0x3d7ba0
003d7b90: ldr      r2, [ip, #0x10]
003d7b94: mov      r3, ip
003d7b98: cmp      r6, r2
003d7b9c: bhs      #0x3d7bc4
003d7ba0: add      r3, sp, #8
003d7ba4: mov      lr, #0
003d7ba8: add      r0, sp, #0x18
003d7bac: add      r2, sp, #0x1c
003d7bb0: str      r6, [sp, #8]
003d7bb4: str      lr, [sp, #0xc]
003d7bb8: str      ip, [sp, #0x1c]
003d7bbc: bl       #0x3d7678
003d7bc0: ldr      r3, [sp, #0x18]
003d7bc4: str      r5, [r3, #0x14]
003d7bc8: b        #0x3d7a2c
003d7bcc: mov      r3, r7
003d7bd0: b        #0x3d7ac0
003d7bd4: ldr      r2, [pc, #0x78]
003d7bd8: ldr      r2, [r3, r2]
003d7bdc: ldr      r2, [r2]
003d7be0: cmp      r2, #2
003d7be4: streq    r4, [r4]
003d7be8: beq      #0x3d7a0c
003d7bec: cmp      r2, #1
003d7bf0: bne      #0x3d7a0c
003d7bf4: ldr      r0, [pc, #0x5c]
003d7bf8: ldr      r1, [pc, #0x5c]
003d7bfc: ldr      r2, [pc, #0x5c]
003d7c00: ldr      r0, [r3, r0]
003d7c04: ldr      r3, [pc, #0x58]
003d7c08: mov      ip, #0x274
003d7c0c: add      r1, pc, r1
003d7c10: add      r2, pc, r2
003d7c14: add      r3, pc, r3
003d7c18: add      r0, r0, #0xa8
003d7c1c: str      ip, [sp]
003d7c20: bl       #0x30e004
003d7c24: b        #0x3d7a0c
003d7c28: ldr      r6, [r6, #4]
003d7c2c: mov      ip, r1
003d7c30: b        #0x3d7b88
003d7c34: ldr      r3, [r4, #0x3c8]
003d7c38: add      r0, r4, #0x3c8
003d7c3c: ldr      r1, [r6, #4]
003d7c40: mov      lr, pc
003d7c44: ldr      pc, [r3, #0x38]
003d7c48: ldr      ip, [r6, #0x80]
003d7c4c: b        #0x3d7ac8

# _Z16CF_SetCombatantsP9CharacterS0_ibb
003b059c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b05a0: ldr      r4, [pc, #0x70]
003b05a4: mov      r5, r1
003b05a8: add      r1, r0, #0xff0
003b05ac: add      r4, pc, r4
003b05b0: mov      r6, r2
003b05b4: add      r1, r1, #4
003b05b8: str      r0, [r4, #0x1c]
003b05bc: mov      r2, #0x13
003b05c0: str      r5, [r4, #0x20]
003b05c4: add      r0, r0, #0x560
003b05c8: mov      r8, r3
003b05cc: ldrb     r7, [sp, #0x20]
003b05d0: bl       #0x3dedb4
003b05d4: add      r1, r5, #0xff0
003b05d8: mov      sl, r0
003b05dc: mov      r2, #0x13
003b05e0: add      r1, r1, #4
003b05e4: add      r0, r5, #0x560
003b05e8: bl       #0x3dedb4
003b05ec: rsb      r0, r0, sl
003b05f0: mov      r3, #0
003b05f4: rsb      r2, r0, #0
003b05f8: strb     r3, [r4, #0x33]
003b05fc: str      r2, [r4, #0x28]
003b0600: str      r6, [r4, #0x2c]
003b0604: strb     r8, [r4, #0x30]
003b0608: strb     r7, [r4, #0x31]
003b060c: str      r0, [r4, #0x24]
003b0610: strb     r3, [r4, #0x32]
003b0614: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0618: subseq   r2, pc, ip, ror #7

# _ZNK9Character27GetEffectiveThreatPerDamageEv
003bd394: add      r1, r0, #0xff0
003bd398: push     {r4, lr}
003bd39c: add      r1, r1, #4
003bd3a0: mov      r2, #0xcc
003bd3a4: add      r0, r0, #0x560
003bd3a8: bl       #0x3dedb4
003bd3ac: bl       #0x30e964
003bd3b0: mov      r1, #0x3b800000
003bd3b4: bl       #0x30ed6c
003bd3b8: pop      {r4, pc}
