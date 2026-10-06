# _Z15NativePlayMusicRKN7gameswf7fn_callE
0043ad84 push     {r4, r5, lr}
0043ad88 ldr      r5, [r0, #0x10]
0043ad8c ldr      r4, [pc, #0x74]
0043ad90 sub      sp, sp, #0xc
0043ad94 cmp      r5, #1
0043ad98 add      r4, pc, r4
0043ad9c beq      #0x43ada8
0043ada0 add      sp, sp, #0xc
0043ada4 pop      {r4, r5, pc}
0043ada8 ldr      r3, [r0, #0xc]
0043adac ldr      r2, [r0, #0x14]
0043adb0 mov      r0, #0xc
0043adb4 ldr      r3, [r3]
0043adb8 mla      r0, r0, r2, r3
0043adbc ldrb     r3, [r0, #1]
0043adc0 sub      r3, r3, #3
0043adc4 uxtb     r3, r3
0043adc8 cmp      r3, #1
0043adcc bhi      #0x43ada0
0043add0 bl       #0x796f5c
0043add4 bl       #0x37ba84
0043add8 cmn      r0, #1
0043addc beq      #0x43ada0
0043ade0 ldr      r3, [pc, #0x24]
0043ade4 mov      r1, r0
0043ade8 mov      ip, #0x7d0
0043adec ldr      r0, [r4, r3]
0043adf0 mov      r2, r5
0043adf4 mov      r3, #0
0043adf8 ldr      r0, [r0]
0043adfc str      ip, [sp]
0043ae00 bl       #0x36bd78
0043ae04 b        #0x43ada0
0043ae08 ldrsheq  sb, [r5], #-0xc8
0043ae0c andeq    r0, r0, r4, lsr #27
