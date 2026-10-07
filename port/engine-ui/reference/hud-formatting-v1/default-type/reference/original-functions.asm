
# _ZNK14CharProperties8_GetTypeEi
003deed8: ldr      r3, [pc, #0x28]
003deedc: mov      r2, r1
003deee0: ldr      r1, [pc, #0x24]
003deee4: push     {r4, lr}
003deee8: add      r3, pc, r3
003deeec: ldr      ip, [r3, r1]
003deef0: ldr      r1, [ip]
003deef4: add      r1, r1, #0x384
003deef8: bl       #0x3dedb4
003deefc: cmn      r0, #1
003def00: moveq    r0, #0x10
003def04: pop      {r4, pc}
003def08: subseq   r5, fp, r8, lsr #23
003def0c: andeq    r2, r0, r0, asr fp

# _ZNK14CharProperties11_GetDefaultEi
003def10: ldr      r3, [pc, #0x14]
003def14: mov      r2, r1
003def18: ldr      r1, [pc, #0x10]
003def1c: add      r3, pc, r3
003def20: ldr      ip, [r3, r1]
003def24: ldr      r1, [ip]
003def28: b        #0x3dedb4
003def2c: subseq   r5, fp, r4, ror fp
003def30: andeq    r2, r0, r0, asr fp
