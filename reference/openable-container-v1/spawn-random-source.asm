_ZN6Random9GetRandomEib.clone.2
0038bc3c ldr      r3, [pc, #0x110]
0038bc40 cmp      r0, #0
0038bc44 push     {r4, r5}
0038bc48 add      r3, pc, r3
0038bc4c beq      #0x38bcd4
0038bc50 ldr      r2, [pc, #0x100]
0038bc54 movw     r1, #0xe6ab
0038bc58 movw     ip, #0xdb17
0038bc5c ldr      r4, [r3, r2]
0038bc60 movt     ip, #0x2b52
0038bc64 movw     r0, #0xf26b
0038bc68 ldr      r2, [r4]
0038bc6c movt     r0, #0xda
0038bc70 mul      r2, r1, r2
0038bc74 movw     r1, #0xb503
0038bc78 add      r2, r2, #0x2b000
0038bc7c add      r2, r2, #0x3fc
0038bc80 add      r2, r2, #1
0038bc84 umull    r5, ip, ip, r2
0038bc88 movt     r1, #0xa57e
0038bc8c rsb      r5, ip, r2
0038bc90 add      ip, ip, r5, lsr #1
0038bc94 ldr      r5, [pc, #0xc0]
0038bc98 ldr      r5, [r3, r5]
0038bc9c lsr      r3, ip, #0x17
0038bca0 mls      r3, r0, r3, r2
0038bca4 ldr      r0, [r5, #4]
0038bca8 umull    ip, r2, r1, r3
0038bcac mov      r1, #0x63
0038bcb0 lsr      r2, r2, #6
0038bcb4 mls      r2, r1, r2, r3
0038bcb8 add      r1, r0, #1
0038bcbc str      r1, [r5, #4]
0038bcc0 eor      r0, r2, r2, asr #31
0038bcc4 sub      r0, r0, r2, asr #31
0038bcc8 str      r3, [r4]
0038bccc pop      {r4, r5}
0038bcd0 bx       lr
0038bcd4 ldr      r2, [pc, #0x84]
0038bcd8 movw     r1, #0xe6ab
0038bcdc movw     ip, #0xdb17
0038bce0 ldr      r4, [r3, r2]
0038bce4 movt     ip, #0x2b52
0038bce8 movw     r0, #0xf26b
0038bcec ldr      r2, [r4]
0038bcf0 movt     r0, #0xda
0038bcf4 mul      r2, r1, r2
0038bcf8 movw     r1, #0xb503
0038bcfc add      r2, r2, #0x2b000
0038bd00 add      r2, r2, #0x3fc
0038bd04 add      r2, r2, #1
0038bd08 umull    r5, ip, ip, r2
0038bd0c movt     r1, #0xa57e
0038bd10 rsb      r5, ip, r2
0038bd14 add      ip, ip, r5, lsr #1
0038bd18 ldr      r5, [pc, #0x3c]
0038bd1c ldr      r5, [r3, r5]
0038bd20 lsr      r3, ip, #0x17
0038bd24 mls      r3, r0, r3, r2
0038bd28 ldr      r0, [r5]
0038bd2c umull    ip, r2, r1, r3
0038bd30 mov      r1, #0x63
0038bd34 lsr      r2, r2, #6
0038bd38 mls      r2, r1, r2, r3
0038bd3c add      r1, r0, #1
0038bd40 str      r1, [r5]
0038bd44 eor      r0, r2, r2, asr #31
0038bd48 sub      r0, r0, r2, asr #31
0038bd4c str      r3, [r4]
0038bd50 b        #0x38bccc
0038bd54 rsbeq    r8, r0, r8, asr #28
0038bd58 andeq    r0, r0, r0, lsl fp
0038bd5c andeq    r1, r0, r8, lsl #1
0038bd60 muleq    r0, r4, ip
