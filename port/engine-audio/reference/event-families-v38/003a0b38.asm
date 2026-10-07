# _ZN9Container8InteractEP10GameObject
003a0b38 push     {r4, r5, r6, r7, lr}
003a0b3c ldr      r3, [r0, #0x394]
003a0b40 ldr      r5, [pc, #0x110]
003a0b44 sub      sp, sp, #0x24
003a0b48 sub      r3, r3, #3
003a0b4c cmp      r3, #1
003a0b50 mov      r4, r0
003a0b54 add      r5, pc, r5
003a0b58 bls      #0x3a0c34
003a0b5c str      r1, [r0, #0x398]
003a0b60 mov      r1, #4
003a0b64 bl       #0x39f3cc
003a0b68 ldr      r3, [r4]
003a0b6c mov      r0, r4
003a0b70 mov      lr, pc
003a0b74 ldr      pc, [r3, #0xdc]
003a0b78 subs     r1, r0, #0
003a0b7c beq      #0x3a0c3c
003a0b80 ldr      r3, [r4, #0x2d8]
003a0b84 cmp      r3, #0
003a0b88 beq      #0x3a0c4c
003a0b8c mov      r0, r4
003a0b90 mov      r1, #3
003a0b94 bl       #0x39f3cc
003a0b98 ldr      r2, [r4, #0x2d8]
003a0b9c ldr      r1, [pc, #0xb8]
003a0ba0 mov      r3, #0
003a0ba4 ldr      ip, [r2, #0x38]
003a0ba8 add      r1, pc, r1
003a0bac mov      r2, r3
003a0bb0 mov      r0, ip
003a0bb4 ldr      ip, [ip]
003a0bb8 str      r3, [sp]
003a0bbc mov      lr, pc
003a0bc0 ldr      pc, [ip, #0x20]
003a0bc4 ldr      r2, [pc, #0x94]
003a0bc8 ldr      r3, [r4]
003a0bcc mov      r0, r4
003a0bd0 ldr      r2, [r5, r2]
003a0bd4 ldr      r7, [r2]
003a0bd8 mov      lr, pc
003a0bdc ldr      pc, [r3, #0xd8]
003a0be0 ldr      lr, [r4, #0x168]
003a0be4 ldr      r5, [r4, #0x164]
003a0be8 ldr      r6, [r4, #0x160]
003a0bec mov      ip, #0xbf000000
003a0bf0 add      ip, ip, #0x800000
003a0bf4 mov      r1, r0
003a0bf8 mov      r3, #0
003a0bfc str      lr, [sp, #0x1c]
003a0c00 mov      r0, r7
003a0c04 mov      lr, #1
003a0c08 add      r2, sp, #0x14
003a0c0c str      r6, [sp, #0x14]
003a0c10 str      r5, [sp, #0x18]
003a0c14 str      lr, [sp]
003a0c18 str      ip, [sp, #8]
003a0c1c str      ip, [sp, #4]
003a0c20 bl       #0x36b5d8
003a0c24 mov      r0, r4
003a0c28 ldr      r3, [r4]
003a0c2c mov      lr, pc
003a0c30 ldr      pc, [r3, #0x2c]
003a0c34 add      sp, sp, #0x24
003a0c38 pop      {r4, r5, r6, r7, pc}
003a0c3c mov      r0, r4
003a0c40 mov      r2, r1
003a0c44 bl       #0x394bf8
003a0c48 b        #0x3a0b80
003a0c4c mov      r0, r4
003a0c50 bl       #0x3a0a98
003a0c54 b        #0x3a0bc4
003a0c58 subseq   r3, pc, ip, lsr pc
003a0c5c subseq   r1, r2, r0, lsr pc
003a0c60 andeq    r0, r0, r4, lsr #27
