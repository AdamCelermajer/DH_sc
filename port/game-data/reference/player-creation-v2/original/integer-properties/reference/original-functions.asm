
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
