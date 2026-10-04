
# _ZN8MenuBase16FS_SetDifficultyEPKcS1_Pv
004222d8: push     {r4, r5, r6, lr}
004222dc: mov      r0, r1
004222e0: mov      r4, r1
004222e4: mov      r5, r2
004222e8: bl       #0x30de54
004222ec: mov      r1, r4
004222f0: add      r2, r4, r0
004222f4: add      r0, r5, #0x9c
004222f8: bl       #0x3109e0
004222fc: mov      r0, #1
00422300: pop      {r4, r5, r6, pc}

# _ZN9Character20SG_GetGameDifficultyEv
003bb8e4: movw     r3, #0x14e8
003bb8e8: ldr      r2, [r0, r3]
003bb8ec: ldr      r3, [pc, #0x1c]
003bb8f0: cmp      r2, #0
003bb8f4: add      r3, pc, r3
003bb8f8: mvneq    r0, #0
003bb8fc: bxeq     lr
003bb900: ldr      r2, [pc, #0xc]
003bb904: ldr      r3, [r3, r2]
003bb908: ldr      r0, [r3]
003bb90c: bx       lr

# _ZNK14CharProperties12PROPS_GetIntEib
003df6e0: ldr      r3, [pc, #0x28]
003df6e4: cmp      r2, #0
003df6e8: mov      r2, r1
003df6ec: addeq    r1, r0, #0xa90
003df6f0: add      r3, pc, r3
003df6f4: push     {r4, lr}
003df6f8: addeq    r1, r1, #4
003df6fc: ldrne    r1, [pc, #0x10]
003df700: ldrne    r1, [r3, r1]
003df704: bl       #0x3dedb4
003df708: asr      r0, r0, #8
003df70c: pop      {r4, pc}
003df710: subseq   r5, fp, r0, lsr #7
003df714: andeq    r1, r0, ip, asr #32

# _ZN9Character20SG_SetGameDifficultyEi
003bb950: movw     r3, #0x14e8
003bb954: ldr      r2, [r0, r3]
003bb958: ldr      r3, [pc, #0x24]
003bb95c: cmp      r2, #0
003bb960: add      r3, pc, r3
003bb964: bxeq     lr
003bb968: ldr      r2, [r2, #0x3c]
003bb96c: cmp      r1, r2
003bb970: bxgt     lr
003bb974: ldr      r2, [pc, #0xc]
003bb978: ldr      r3, [r3, r2]
003bb97c: str      r1, [r3]
003bb980: bx       lr
003bb984: subseq   sb, sp, r0, lsr r1
003bb988: muleq    r0, ip, sl

# _Z26NativeSetCurrentDifficultyRKN7gameswf7fn_callE
0043cd68: push     {r4, lr}
0043cd6c: ldr      r2, [r0, #0xc]
0043cd70: ldr      r3, [pc, #0x2c]
0043cd74: ldr      r0, [r0, #0x14]
0043cd78: ldr      r1, [r2]
0043cd7c: ldr      r2, [pc, #0x24]
0043cd80: add      r3, pc, r3
0043cd84: mov      ip, #0xc
0043cd88: ldr      r2, [r3, r2]
0043cd8c: mla      r0, ip, r0, r1
0043cd90: ldr      r4, [r2, #0x4c]
0043cd94: bl       #0x797a54
0043cd98: bl       #0x30ea24
0043cd9c: str      r0, [r4, #0xc]
0043cda0: pop      {r4, pc}
0043cda4: subseq   r7, r5, r0, lsl sp
0043cda8: strdeq   r3, r4, [r0], -r4
